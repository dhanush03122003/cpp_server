#include <string>
#include <map>
#include <algorithm>
#include <cctype>

#include <utils/HttpUtils.hpp>
#include <utils/http_status.hpp>
#include <utils/http_headers.hpp>


#include "ResourceMapper.hpp"


class HttpsClient :public ResourceMapper {
private:
	std::string request_uri;
	std::string method;
	std::map<std::string, std::string> request_headers;
	std::string response_headers;
	std::string response_body;
	HttpStatus status_code;


	std::map<std::string, Method> MethodMap;
	
public:
	HttpsClient();
	HttpsClient(std::string& request);
	~HttpsClient();
	std::string process_request();
	HttpsClient(const HttpsClient&) = delete;
	HttpsClient& operator=(const HttpsClient&) = delete;

	HttpsClient(HttpsClient&&) = default;
	HttpsClient& operator=(HttpsClient&&) = default;
	
};