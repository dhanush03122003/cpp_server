#include "HttpUtils.hpp"

#include "http_headers.hpp"

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
    std::string trim(const std::string& s) {
        auto start = s.begin();
        while (start != s.end() && std::isspace(*start)) {
            start++;
        }
        auto end = s.end();
        do {
            end--;
        } while (std::distance(start, end) > 0 && std::isspace(*end));
        return std::string(start, end + 1);
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
                std::string name = trim(line.substr(0, colon));
                std::string value = trim(line.substr(colon + 1));
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

        return first_line.substr(method_end + 1, uri_end - method_end - 1);;
    }

    std::string _exctract_method(const std::string& request) {
        size_t method_len = request.find(" ");
        std::string method = request.substr(0, method_len);
        std::transform(method.begin(), method.end(), method.begin(), ::toupper);
        return method;
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



}
