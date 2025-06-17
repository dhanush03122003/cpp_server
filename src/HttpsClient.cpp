#include "HttpsClient.hpp"
#include <iostream>

HttpsClient::HttpsClient() {
    status_code = HTTP_200_OK;
    path_params = new DynamicDict();
    query_params = new DynamicDict();

}
HttpsClient::HttpsClient(std::string& request) : HttpsClient() {


    MethodMap["GET"] = GET;
    MethodMap["POST"] = POST;
    MethodMap["PUT"] = PUT;
    MethodMap["DELETE"] = DELETE_;

    request_uri = utils::_extract_uri(request);
    method = utils::_exctract_method(request);
    request_headers = utils::parse_headers(request);
	query_args_from_uri = utils::_extract_query_args(request);

    response_body = "";

}
HttpsClient::~HttpsClient() {
    delete path_params;
    delete query_params;

}
std::string HttpsClient::process_request() {


    if (request_uri.find("GET /favicon.ico") != std::string::npos) {
        return utils::construct_http_response(HTTP_404_NOT_FOUND, response_body="");
        //return utils::construct_404_response();
    }
   
    ResourceMapper resourcemapper;
    bool is_data_type_mismatch_in_uri = false;
    std::string match = utils::find_match(request_uri, resourcemapper.resource_mapper , is_data_type_mismatch_in_uri);
    auto it = resourcemapper.resource_mapper.find(match);

    //bool is_data_type_match = TypeChecker::is_matching_data_type(request_uri, match);

    if (it != resourcemapper.resource_mapper.end()) {
        std::unique_ptr<IResource> ptr = std::move(it->second);
        auto it = MethodMap.find(method);
		
		QueryParamRules rules = ptr->query_param_rules();
		bool is_valid_query_params = utils::valid_query_params(method, query_args_from_uri, rules, query_params_error_msg);
		bool is_valid_path_params = utils::valid_path_params(request_uri, match, path_params_error_msg);
        if (it != MethodMap.end() && is_valid_query_params && is_valid_path_params) {
            utils::set_params(*path_params, request_uri, match, *query_params, query_args_from_uri, rules, method);
            ptr->path_params = *path_params;
            ptr->query_params = *query_params;
            try {
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
            catch (const std::exception& e) {

                status_code = HTTP_500_INTERNAL_SERVER_ERROR;
                response_body = e.what();

            }
            
        }
        else {
            if (query_params_error_msg.length() > 0){
                status_code = HTTP_400_BAD_REQUEST;
                response_body = query_params_error_msg;
            }
            else if(path_params_error_msg.length() > 0) {
                status_code = HTTP_400_BAD_REQUEST;
                response_body = path_params_error_msg;
            }
            else {
                status_code = HTTP_405_METHOD_NOT_ALLOWED;
                response_body = method + " Method not allowed for the uri " + request_uri;
                std::cout << "No method found" << std::endl;
            }
        }
        return utils::construct_http_response(status_code, response_body);

    }
    if (!is_data_type_mismatch_in_uri) return utils::construct_http_response(HTTP_404_NOT_FOUND, "No matching endpoint " + request_uri + " was found on server");;

    // to do raise error 
    std::cout << "Invalid value provided for one or more path parameters." << std::endl;
    return utils::construct_http_response(HTTP_400_BAD_REQUEST, "Invalid value provided for one or more path parameters.");
	
}