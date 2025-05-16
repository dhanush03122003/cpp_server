#include "HttpsClient.hpp"
#include <iostream>

HttpsClient::HttpsClient() {


}
HttpsClient::HttpsClient(std::string& request) {
    
    MethodMap["GET"] = GET;
    MethodMap["POST"] = POST;
    MethodMap["PUT"] = PUT;
    MethodMap["DELETE"] = DELETE_;

    request_uri = utils::_extract_uri(request);
    method = utils::_exctract_method(request);
    request_headers = utils::parse_headers(request);

    response_body = "";


}
HttpsClient::~HttpsClient() {

}
std::string HttpsClient::process_request() {


    if (request_uri.find("GET /favicon.ico") != std::string::npos) {
        return utils::construct_http_response(HTTP_404_NOT_FOUND, response_body="");
        //return utils::construct_404_response();
    }

    auto it = ResourceMapper::resource_mapper.find(request_uri);

    if (it != resource_mapper.end()) {
        std::unique_ptr<IResource> ptr = std::move(it->second);
        auto it = MethodMap.find(method);
        std::cout << method;
        if (it != MethodMap.end()) {
            switch (it->second) {
            case GET:
                status_code = ptr->get(response_body);
                break;
            case POST:
                status_code = ptr->post(response_body);
                break;
            case PUT:
                status_code = ptr->put(response_body);
                break;
            case DELETE_:
                status_code = ptr->delete_(response_body);
                break;

            default:
                status_code = HTTP_405_METHOD_NOT_ALLOWED;
                std::cout << "no method found" << std::endl;
                break;
            }
            
        }
        else {
            status_code = HTTP_405_METHOD_NOT_ALLOWED;
            response_body = method + " Method not allowed for the uri " + request_uri;
            std::cout << "No method found" << std::endl;
        }
        return utils::construct_http_response(status_code, response_body);

    }
    // to do raise error 
    std::cout << "No url found" << std::endl;
    return utils::construct_http_response(HTTP_404_NOT_FOUND, "No matching endpoint "+ request_uri + " was found on server");
	
}