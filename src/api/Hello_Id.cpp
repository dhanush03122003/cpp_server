#include "Hello_Id.hpp"
QueryParamRules Hello_Id::query_param_rules() {

    return get_empty_query_param_rules(); // no query params .
}

HttpStatus Hello_Id::get(json& response_body) {
    const std::string a = "ID";
    int id = path_params.get(a);  // path param

    std::cout << "Inside Hello_ID Get method, Path Param id: " << id << std::endl;

    response_body["message"] = "Hello_Id World GET";
    response_body["id"] = id;

    return HTTP_200_OK;
}

HttpStatus Hello_Id::post(json& response_body) {
    response_body["message"] = "Hello_Id World POST";
    return HTTP_201_CREATED;
}

HttpStatus Hello_Id::put(json& response_body) {
    response_body["message"] = "Hello_Id World PUT";
    return HTTP_200_OK;
}

HttpStatus Hello_Id::delete_(json& response_body) {
    response_body["message"] = "Hello_Id World DELETE";
    return HTTP_204_NO_CONTENT;
}
