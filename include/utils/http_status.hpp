#pragma once

#include <string>

enum HttpStatus {
    // Informational
    HTTP_100_CONTINUE = 100,
    HTTP_101_SWITCHING_PROTOCOLS = 101,

    // Success
    HTTP_200_OK = 200,
    HTTP_201_CREATED = 201,
    HTTP_204_NO_CONTENT = 204,

    // Redirection
    HTTP_301_MOVED_PERMANENTLY = 301,
    HTTP_302_FOUND = 302,

    // Client Errors
    HTTP_400_BAD_REQUEST = 400,
    HTTP_401_UNAUTHORIZED = 401,
    HTTP_403_FORBIDDEN = 403,
    HTTP_404_NOT_FOUND = 404,
    HTTP_405_METHOD_NOT_ALLOWED = 405,

    // Server Errors
    HTTP_500_INTERNAL_SERVER_ERROR = 500,
    HTTP_501_NOT_IMPLEMENTED = 501,
    HTTP_503_SERVICE_UNAVAILABLE = 503
};

inline std::string get_http_status_message(int code) {
    switch (code) {
        // Informational
    case HTTP_100_CONTINUE: return "Continue";
    case HTTP_101_SWITCHING_PROTOCOLS: return "Switching Protocols";

        // Success
    case HTTP_200_OK: return "OK";
    case HTTP_201_CREATED: return "Created";
    case HTTP_204_NO_CONTENT: return "No Content";

        // Redirection
    case HTTP_301_MOVED_PERMANENTLY: return "Moved Permanently";
    case HTTP_302_FOUND: return "Found";

        // Client Errors
    case HTTP_400_BAD_REQUEST: return "Bad Request";
    case HTTP_401_UNAUTHORIZED: return "Unauthorized";
    case HTTP_403_FORBIDDEN: return "Forbidden";
    case HTTP_404_NOT_FOUND: return "Not Found";
    case HTTP_405_METHOD_NOT_ALLOWED: return "Method Not Allowed";

        // Server Errors
    case HTTP_500_INTERNAL_SERVER_ERROR: return "Internal Server Error";
    case HTTP_501_NOT_IMPLEMENTED: return "Not Implemented";
    case HTTP_503_SERVICE_UNAVAILABLE: return "Service Unavailable";

    default: return "Unknown Status";
    }
}
