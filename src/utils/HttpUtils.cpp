#include "HttpUtils.hpp"

#include "http_headers.hpp"
#include "type_checker.hpp"

namespace utils {

    std::string get_http_date() {
        std::time_t now = std::time(nullptr);
        std::tm gm_time;
#ifdef _WIN32
        gmtime_s(&gm_time, &now);  // Windows-safe
#else
        gmtime_r(&now, &gm_time);  // POSIX-safe
#endif

        std::ostringstream oss;
        oss << std::put_time(&gm_time, "%a, %d %b %Y %H:%M:%S GMT");
        return "Date: " + oss.str() + "\r\n";
    }

    std::map<std::string, std::string> parse_headers(const std::string& request) {
        std::map<std::string, std::string> headers;
        std::istringstream stream(request);
        std::string line;
        // Skip the request line
        std::getline(stream, line);
        while (std::getline(stream, line) && line != "\r") {
            auto colon = line.find(':');
            if (colon != std::string::npos) {
                std::string name = StringUtils::trim(line.substr(0, colon));
                std::string value = StringUtils::trim(line.substr(colon + 1));
                headers[name] = value;
            }
        }
        return headers;
    }

    std::string _extract_uri(const std::string& request) {
        size_t line_end = request.find("\r\n");
        if (line_end == std::string::npos)
            line_end = request.find('\n');

        std::string first_line = request.substr(0, line_end);

        size_t method_end = first_line.find(' ');
        size_t uri_end = first_line.find(' ', method_end + 1);

        if (method_end == std::string::npos || uri_end == std::string::npos)
            return ""; // Malformed request

        std::string uri = first_line.substr(method_end + 1, uri_end - method_end - 1);

        // Remove query parameters if any
        size_t query_pos = uri.find('?');
        if (query_pos != std::string::npos) {
            uri = uri.substr(0, query_pos);
        }

        // Remove trailing slashes
        uri = StringUtils::rstrip(uri, '/');

        return uri;
    }


    std::string _exctract_method(const std::string& request) {
        size_t method_len = request.find(" ");
        std::string method = request.substr(0, method_len);
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);
        return method;
    }

    std::map<std::string, std::string> _extract_query_args(const std::string& request) {
        std::map<std::string, std::string> query_params;

        // Find the start of the request line: e.g., "GET /hello?limit=1 HTTP/1.1"
        std::size_t method_end = request.find(' ');
        if (method_end == std::string::npos) return query_params;

        std::size_t uri_end = request.find(' ', method_end + 1);
        if (uri_end == std::string::npos) return query_params;

        std::string uri = request.substr(method_end + 1, uri_end - method_end - 1);

        // Now extract query string from URI
        std::size_t question_mark_pos = uri.find('?');
        if (question_mark_pos == std::string::npos) {
            return query_params;  // No query string
        }

        std::string query_string = uri.substr(question_mark_pos + 1);
        std::stringstream ss(query_string);
        std::string pair;

        while (std::getline(ss, pair, '&')) {
            std::size_t equal_pos = pair.find('=');
            std::string key, value;

            if (equal_pos != std::string::npos) {
                key = pair.substr(0, equal_pos);
                value = pair.substr(equal_pos + 1);
            }
            else {
                key = pair;
                value = "";  // No value assigned
            }

            std::replace(value.begin(), value.end(), '+', ' ');  // Simple decoding
            query_params[key] = value;

            //std::cout << key << " = " << value << std::endl;
        }

        return query_params;
    }

    static const std::map<std::string, ParamRule>& get_query_param_rules_for_method(
        const QueryParamRules& rules, const std::string& method) {

        if (method == "GET") return rules.get;
        if (method == "POST") return rules.post;
        if (method == "PUT") return rules.put;
        if (method == "DELETE") return rules.delete_;
        static std::map<std::string, ParamRule> empty;
        return empty;
    }

    bool valid_path_params(const std::string uri, const std::string uri_pattern, std::string& path_params_error_msg) {
        auto uriParts = StringUtils::split(uri, '/');
        auto patternParts = StringUtils::split(uri_pattern, '/');

        if (uriParts.size() != patternParts.size()) {
            path_params_error_msg = "Mismatch in URI and pattern segments count.";
            return false;
        }

        for (size_t i = 0; i < patternParts.size(); ++i) {
            const auto& p = patternParts[i];
            const auto& u = uriParts[i];

            if (!p.empty() && p.front() == '<' && p.back() == '>') {
                size_t colonPos = p.find(':');
                if (colonPos == std::string::npos) {
                    path_params_error_msg = "Malformed pattern. Missing ':' in path parameter.";
                    return false;
                }

                // Extract type and name
                std::string type = p.substr(1, colonPos - 1);
                std::string name = p.substr(colonPos + 1, p.size() - colonPos - 2); // -2 for ':' and '>'

                // Simulate a valid type list (replace with your own list of valid types)
                const std::vector<std::string> valid_data_types = { VALID_PATH_PARAM_DATA_TYPES }; // Example valid types

                // Check if the type is valid
                if (std::find(valid_data_types.begin(), valid_data_types.end(), type) == valid_data_types.end()) {
                    path_params_error_msg = "Invalid type '" + type + "' for parameter '" + name + "' in pattern.";
                    return false;
                }

                // Validate value with TypeChecker
                if (!TypeChecker::checkType(type, u)) {
                    path_params_error_msg = "Value '" + u + "' for parameter '" + name + "' does not match expected type '" + type + "'.";
                    return false;
                }
            }
            else {
                // Check if it's a literal mismatch
                if (p != u) {
                    path_params_error_msg = "Literal mismatch: expected '" + p + "' but found '" + u + "' in parameter '" + p + "'.";
                    return false;
                }
            }
        }

        return true;
    }


    bool valid_query_params(
        const std::string& method,
        const std::map<std::string, std::string>& query_args_from_uri,
        const QueryParamRules& rules,
        std::string& query_params_error_msg) {

        const auto& method_rules = get_query_param_rules_for_method(rules, method);

        for (const auto& [key, _] : query_args_from_uri) {
            if (method_rules.find(key) == method_rules.end()) {
                query_params_error_msg = "Unexpected query parameter: " + key;
                return false;
            }
        }

        for (const auto& [param_name, rule] : method_rules) {
            auto it = query_args_from_uri.find(param_name);

            if (rule.required && it == query_args_from_uri.end()) {
                query_params_error_msg = "Missing required query parameter: " + param_name;
                return false;
            }

            if (it != query_args_from_uri.end()) {
                const std::string& value = it->second;
                const std::string& expected_type = rule.type;

                const std::vector<std::string> valid_data_types = { VALID_QUERY_PARAM_DATA_TYPES };

                // Check if the type is valid
                if (std::find(valid_data_types.begin(), valid_data_types.end(), expected_type) == valid_data_types.end()) {
                    query_params_error_msg = "Invalid data type " + expected_type + "for "+ param_name+"  parameter.";
                    return false;
                }


                if (expected_type == "INT") {
                    if (!std::regex_match(value, std::regex("^-?\\d+$"))) {
                        query_params_error_msg = "Invalid INT format for param: " + param_name;
                        return false;
                    }
                    if (!rule.min_value.empty() && std::stoi(value) < std::stoi(rule.min_value)) {
                        query_params_error_msg = param_name + " is below minimum allowed value";
                        return false;
                    }
                    if (!rule.max_value.empty() && std::stoi(value) > std::stoi(rule.max_value)) {
                        query_params_error_msg = param_name + " is above maximum allowed value";
                        return false;
                    }
                }
                else if (expected_type == "BOOL") {
                    if (value != "true" && value != "false" && value != "1" && value != "0") {
                        query_params_error_msg = "Invalid BOOL value for param: " + param_name;
                        return false;
                    }
                }
                else if (expected_type == "STR") {
                    if (value.empty()) {
                        query_params_error_msg = "Empty string for param: " + param_name;
                        return false;
                    }
                }
                else if (expected_type == "ENUM") {
                    if (std::find(rule.allowed_values.begin(), rule.allowed_values.end(), value) == rule.allowed_values.end()) {
                        query_params_error_msg = "Invalid value for ENUM param: " + param_name +" allowed values are [ ";
                        for (auto i : rule.allowed_values) { query_params_error_msg += "'" + i + "' "; }
                        query_params_error_msg += "]";
                        return false;
                    }
                }
                else {
                    query_params_error_msg = "Invalid data type " + expected_type + "for " + param_name + "  parameter.";
                    return false;
                }
            }
        }

        return true;
    }

    void set_params(DynamicDict& path_params, const std::string uri, const std::string uri_pattern, DynamicDict& query_params, std::map<std::string, std::string> query_args_from_uri, const QueryParamRules& rules ,const std::string method){
        // uri_pattern : /hello/<INT:ID>
        // uri : /hello/12345

        auto uriParts = StringUtils::split(uri, '/');
        auto patternParts = StringUtils::split(uri_pattern, '/');

        for (size_t i = 0; i < patternParts.size(); ++i) {
            const auto& p = patternParts[i];
            const auto& u = uriParts[i];

            if (!p.empty() && p.front() == '<' && p.back() == '>') {
                size_t colonPos = p.find(':');

                // Extract type and name
                std::string type = p.substr(1, colonPos - 1);
                std::string name = p.substr(colonPos + 1, p.size() - colonPos - 2); // -2 for ':' and '>'

                // Set in path_params
                path_params.set(name,type,u);
            }
        }
        const auto& method_rules = get_query_param_rules_for_method(rules, method);

        for (const auto& [key, value] : query_args_from_uri) {
            auto it = method_rules.find(key);
            query_params.set(key, it->second.type, value);
        }

    }

    std::string construct_http_response(int status_code, const json response_body, const std::string& content_type) {

        std::string full_body = response_body.dump(4);
        std::string content_length = std::to_string(full_body.size());
        std::string response = "HTTP/1.1 " + std::to_string(status_code) + " " + get_http_status_message(status_code) + "\r\n"
          
            HEADER_SERVER_INFO  
            + "Content-Type: " + content_type + "\r\n"
            + "Content-Length: " + content_length + "\r\n"
            HEADER_CONNECTION_CLOSE
            + get_http_date()
            + "\r\n" // End of headers
            + full_body;

        return response;
    }

    bool is_matching_data_type(const std::string& uri, const std::string& pattern) {


        auto uriParts = StringUtils::split(uri, '/');
        auto patternParts = StringUtils::split(pattern, '/');

        if (uriParts.size() != patternParts.size())
            return false;

        for (size_t i = 0; i < patternParts.size(); ++i) {
            const auto& p = patternParts[i];
            const auto& u = uriParts[i];

            if (!p.empty() && p.front() == '<' && p.back() == '>') {
                size_t colonPos = p.find(':');
                if (colonPos == std::string::npos)
                    return false;  // malformed pattern

                std::string type = p.substr(1, colonPos - 1);

                if (!TypeChecker::checkType(type, u))
                    return false;  // value doesn't match the expected type
            }
            else {
                if (p != u)
                    return false;  // literal path segment mismatch
            }
        }

        return true;
    }

    json extract_payload(const std::string request) {
        size_t pos = request.find('{');
        if (pos != std::string::npos) {
            try {
                return json::parse(request.substr(pos));
            }
            catch (const json::parse_error& e) {
                return json{
                    {"error", "JSON parse error"},
                    {"message", e.what()}
                };
            }
            catch (const std::exception& e) {
                return json{
                    {"error", "Exception while parsing"},
                    {"message", e.what()}
                };
            }
        }

        return json{
            {"error", "No JSON object found in request"}
        };
    }

    std::string find_match(std::string& uri, std::map<std::string, std::unique_ptr<IResource> >& resource_mapper ,bool& is_data_type_mismatch_in_path_args){

        for (const auto& it : resource_mapper) {

            if (StringUtils::is_match_with(uri, it.first) ) {
                if (utils::is_matching_data_type(uri, it.first)) {
                    is_data_type_mismatch_in_path_args = false;
                    return it.first;
                }
                else {
                    is_data_type_mismatch_in_path_args = true;
                }
                
            }
        }
        return "MATCHING_URI_NOT_FOUND";
    }

 
}
