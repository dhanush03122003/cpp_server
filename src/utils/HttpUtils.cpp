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
            if (equal_pos != std::string::npos) {
                std::string key = pair.substr(0, equal_pos);
                std::string value = pair.substr(equal_pos + 1);

                // Optional simple decoding: convert '+' to space
                std::replace(value.begin(), value.end(), '+', ' ');

                query_params[key] = value;
            }
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

    bool validate_query_params(
        const std::string& method,
        const std::map<std::string, std::string>& query_args_from_uri,
        const QueryParamRules& rules,
		DynamicDict& query_params,
        std::string& error_out) {

        const auto& method_rules = get_query_param_rules_for_method(rules, method);

        for (const auto& [key, _] : query_args_from_uri) {
            if (method_rules.find(key) == method_rules.end()) {
                error_out = "Unexpected query parameter: " + key;
                return false;
            }
        }

        for (const auto& [param_name, rule] : method_rules) {
            auto it = query_args_from_uri.find(param_name);

            if (rule.required && it == query_args_from_uri.end()) {
                error_out = "Missing required query parameter: " + param_name;
                return false;
            }

            if (it != query_args_from_uri.end()) {
                const std::string& value = it->second;
                const std::string& expected_type = rule.type;

                if (expected_type == "INT") {
                    if (!std::regex_match(value, std::regex("^-?\\d+$"))) {
                        error_out = "Invalid INT format for param: " + param_name;
                        return false;
                    }
                    if (!rule.min_value.empty() && std::stoi(value) < std::stoi(rule.min_value)) {
                        error_out = param_name + " is below minimum allowed value";
                        return false;
                    }
                    if (!rule.max_value.empty() && std::stoi(value) > std::stoi(rule.max_value)) {
                        error_out = param_name + " is above maximum allowed value";
                        return false;
                    }
                }
                else if (expected_type == "FLOAT") {
                    if (!std::regex_match(value, std::regex("^-?\\d+(\\.\\d+)?$"))) {
                        error_out = "Invalid FLOAT format for param: " + param_name;
                        return false;
                    }
                    if (!rule.min_value.empty() && std::stof(value) < std::stof(rule.min_value)) {
                        error_out = param_name + " is below minimum allowed value";
                        return false;
                    }
                    if (!rule.max_value.empty() && std::stof(value) > std::stof(rule.max_value)) {
                        error_out = param_name + " is above maximum allowed value";
                        return false;
                    }
                }
                else if (expected_type == "BOOL") {
                    if (value != "true" && value != "false" && value != "1" && value != "0") {
                        error_out = "Invalid BOOL value for param: " + param_name;
                        return false;
                    }
                }
                else if (expected_type == "STR") {
                    if (value.empty()) {
                        error_out = "Empty string for param: " + param_name;
                        return false;
                    }
                }
                else if (expected_type == "ALNUM") {
                    if (!std::regex_match(value, std::regex("^[a-zA-Z0-9]+$"))) {
                        error_out = "Invalid ALNUM format for param: " + param_name;
                        return false;
                    }
                }
                else if (expected_type == "UUID") {
                    if (!std::regex_match(value, std::regex("^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[1-5][0-9a-fA-F]{3}-[89ab][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$"))) {
                        error_out = "Invalid UUID format for param: " + param_name;
                        return false;
                    }
                }
                else if (expected_type == "ENUM") {
                    if (std::find(rule.allowed_values.begin(), rule.allowed_values.end(), value) == rule.allowed_values.end()) {
                        error_out = "Invalid value for ENUM param: " + param_name;
                        return false;
                    }
                }
                else {
                    error_out = "Unknown type rule for param: " + param_name;
                    return false;
                }
            }
        }

        return true;
    }

    bool set_params(DynamicDict& path_params, const std::string uri, const std::string uri_pattern, DynamicDict& query_params, std::map<std::string, std::string> query_args_from_uri, const QueryParamRules& rules ,const std::string method){
        // uri_pattern : /hello/<INT:ID>
        // uri : /hello/12345

        auto uriParts = StringUtils::split(uri, '/');
        auto patternParts = StringUtils::split(uri_pattern, '/');

        if (uriParts.size() != patternParts.size())
            return false;

        for (size_t i = 0; i < patternParts.size(); ++i) {
            const auto& p = patternParts[i];
            const auto& u = uriParts[i];

            if (!p.empty() && p.front() == '<' && p.back() == '>') {
                size_t colonPos = p.find(':');
                if (colonPos == std::string::npos)
                    return false;  // malformed pattern

                // Extract type and name
                std::string type = p.substr(1, colonPos - 1);
                std::string name = p.substr(colonPos + 1, p.size() - colonPos - 2); // -2 for ':' and '>'

                // Use TypeChecker::checkType to validate
                if (!TypeChecker::checkType(type, u))
                    return false;  // value doesn't match the expected type

                // Set in path_params
                path_params.set(name,type,u);
            }
            else {
                if (p != u)
                    return false;  // literal path segment mismatch
            }
        }
        const auto& method_rules = get_query_param_rules_for_method(rules, method);

        for (const auto& [key, value] : query_args_from_uri) {
            auto it = method_rules.find(key);
            query_params.set(key, it->second.type, value);
        }

        return true;
    }


    std::string construct_http_response(int status_code, const std::string& response_body, const std::string& content_type) {

        std::string full_body = "{ \"message\": \"" + response_body + "\" }";
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

    std::string construct_404_response() {
        std::string status_line = "HTTP/1.1 " + std::to_string((int)HTTP_404_NOT_FOUND) + " " + get_http_status_message(HTTP_404_NOT_FOUND) + "\r\n";

        // Combine headers and body
        std::string response = status_line

            + HEADER_SERVER_INFO
            + HEADER_CONTENT_TYPE_PLAIN
            + HEADER_CONNECTION_CLOSE
            + get_http_date()
            + "\r\n";  // End of headers

        return response;
    }
    std::string find_match(std::string& uri, std::map<std::string, std::unique_ptr<IResource> >& resource_mapper ,bool& is_data_type_mismatch_in_uri){

        for (const auto& it : resource_mapper) {

            if (StringUtils::is_match_with(uri, it.first) ) {
                if (TypeChecker::is_matching_data_type(uri, it.first)) {
                    is_data_type_mismatch_in_uri = false;
                    return it.first;
                }
                else {
                    is_data_type_mismatch_in_uri = true;
                }
                
            }
        }
        return "MATCHING_URI_NOT_FOUND";
    }
    //bool matching_data_type(const std::string& uri, const std::string& pattern) {
    //    auto uriParts = StringUtils::split(uri, '/');
    //    auto patternParts = StringUtils::split(pattern, '/');

    //    if (uriParts.size() != patternParts.size())
    //        return false;

    //    for (size_t i = 0; i < patternParts.size(); ++i) {
    //        const auto& p = patternParts[i];
    //        const auto& u = uriParts[i];

    //        if (!p.empty() && p.front() == '<' && p.back() == '>') {
    //            size_t colonPos = p.find(':');
    //            if (colonPos == std::string::npos)
    //                return false;  // malformed pattern

    //            // Extract type substring between '<' and ':'
    //            std::string type = p.substr(1, colonPos - 1);

    //            // Use TypeChecker::checkType to validate
    //            if (!TypeChecker::checkType(type, u))
    //                return false;  // value doesn't match the expected type
    //        }
    //        else {
    //            if (p != u)
    //                return false;  // literal path segment mismatch
    //        }
    //    }

    //    return true;
    //}

    
}
