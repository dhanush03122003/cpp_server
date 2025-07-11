#include "HttpsClient.hpp"
#include <iostream>


//#include <nlohmann/json.hpp>
//using json = nlohmann::json;

HttpsClient::HttpsClient() {
    status_code = HTTP_200_OK;
    path_params = new DynamicDict();
    query_params = new DynamicDict();

}
HttpsClient::HttpsClient(std::string& request) : HttpsClient() {

    request_uri = utils::_extract_uri(request);
    method = utils::_exctract_method(request);
    payload = utils::extract_payload(request);
    request_headers = utils::parse_headers(request);
    query_args_from_uri = utils::_extract_query_args(request);
    response_body  = json::object();

}
HttpsClient::~HttpsClient() {
    delete path_params;
    delete query_params;

}
std::string HttpsClient::process_request() {

    if (request_uri.find("GET /favicon.ico") != std::string::npos) {
        return utils::construct_http_response(HTTP_404_NOT_FOUND, json{
            {"error", "Not Found"},
            {"message", "favicon.ico not found"}
            });
    }

    ResourceMapper resourcemapper;
    bool is_data_type_mismatch_in_uri = false;
    std::string match = utils::find_match(request_uri, resourcemapper.resource_mapper, is_data_type_mismatch_in_uri);
    auto it = resourcemapper.resource_mapper.find(match);

    if (it != resourcemapper.resource_mapper.end()) {
        std::unique_ptr<IResource> ptr = std::move(it->second);
        auto it = utils::MethodMap.find(method);

        QueryParamRules rules = ptr->query_param_rules();
        bool is_valid_query_params = utils::valid_query_params(method, query_args_from_uri, rules, query_params_error_msg);
        bool is_valid_path_params = utils::valid_path_params(request_uri, match, path_params_error_msg);

        if (it != utils::MethodMap.end() && is_valid_query_params && is_valid_path_params) {
            utils::set_params(*path_params, request_uri, match, *query_params, query_args_from_uri, rules, method);
            ptr->path_params = *path_params;
            ptr->query_params = *query_params;

            if (method != "GET") {
                ptr->payload = payload;
            }

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
                    response_body = json{
                        {"error", "Method not allowed"},
                        {"message", "No handler implemented for method"}
                    };
                    break;
                }
            }
            catch (const std::exception& e) {
                status_code = HTTP_500_INTERNAL_SERVER_ERROR;
                response_body = json{
                    {"error", "Internal server error"},
                    {"message", e.what()}
                };
            }

        }
        else {
            if (query_params_error_msg.length() > 0) {
                status_code = HTTP_400_BAD_REQUEST;
                response_body = json{
                    {"error", "Invalid query parameters"},
                    {"message", query_params_error_msg}
                };
            }
            else if (path_params_error_msg.length() > 0) {
                status_code = HTTP_400_BAD_REQUEST;
                response_body = json{
                    {"error", "Invalid path parameters"},
                    {"message", path_params_error_msg}
                };
            }
            else {
                status_code = HTTP_405_METHOD_NOT_ALLOWED;
                response_body = json{
                    {"error", "Method not allowed"},
                    {"message", method + " method is not allowed for URI: " + request_uri}
                };
                std::cout << "No method found" << std::endl;
            }
        }

        return utils::construct_http_response(status_code, response_body);
    }

    if (!is_data_type_mismatch_in_uri) {
        return utils::construct_http_response(HTTP_404_NOT_FOUND, json{
            {"error", "Not Found"},
            {"message", "No matching endpoint " + request_uri + " was found on server"}
            });
    }

    std::cout << "Invalid value provided for one or more path parameters." << std::endl;
    return utils::construct_http_response(HTTP_400_BAD_REQUEST, json{
        {"error", "Invalid Path Parameters"},
        {"message", "Invalid value provided for one or more path parameters."}
        });
}
