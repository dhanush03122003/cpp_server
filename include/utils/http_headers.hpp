#ifndef HTTP_HEADERS_HPP
#define HTTP_HEADERS_HPP

#define HEADER_CONTENT_TYPE_JSON       "Content-Type: application/json\r\n"
#define HEADER_CONTENT_TYPE_HTML       "Content-Type: text/html\r\n"
#define HEADER_CONTENT_TYPE_PLAIN      "Content-Type: text/plain\r\n"

#define HEADER_CONNECTION_KEEP_ALIVE  "Connection: keep-alive\r\n"
#define HEADER_CONNECTION_CLOSE       "Connection: close\r\n"

#define HEADER_CACHE_CONTROL_NO_CACHE "Cache-Control: no-cache, no-store, must-revalidate\r\n"
#define HEADER_CACHE_CONTROL_PUBLIC   "Cache-Control: public, max-age=3600\r\n"

#define HEADER_ACCESS_CONTROL_ALLOW_ORIGIN_ALL "Access-Control-Allow-Origin: *\r\n"

#define HEADER_SERVER_INFO            "Server: MySocketServer/1.0\r\n"

#endif // HTTP_HEADERS_HPP
