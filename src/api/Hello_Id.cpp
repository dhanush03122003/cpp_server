#include "Hello_Id.hpp"
QueryParamRules Hello_Id::query_param_rules() {

    QueryParamRules query_param_rules;

    return query_param_rules;
}

HttpStatus Hello_Id::get(std::string& response_body) {
    response_body = "Hello_Id World get";
    const std::string a = "ID";
    int id = path_params.get(a);

    std::cout << "Inside Hello_ID Get method , Path Param id: " << id << std::endl;
    return HTTP_200_OK;
}

HttpStatus Hello_Id::post(std::string& response_body) {
    response_body = "Hello_Id World post";
    return HTTP_201_CREATED;
}

HttpStatus Hello_Id::put(std::string& response_body) {
    response_body = "Hello_Id World put";
    return HTTP_200_OK;
}

HttpStatus Hello_Id::delete_(std::string& response_body) {
    response_body = "Hello_Id World delete";
    return HTTP_204_NO_CONTENT;
}
