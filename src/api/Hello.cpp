#include "Hello.hpp"
QueryParamRules Hello::query_param_rules() {

    QueryParamRules query_param_rules;

    ParamRule limit_rule;
    limit_rule.required = true;
    limit_rule.type = "INT";
    limit_rule.min_value = "1";
    limit_rule.max_value = "100";

    ParamRule sort_rule;
    sort_rule.required = false;
    sort_rule.type = "ENUM";
    sort_rule.allowed_values = { "asc", "desc" };

    ParamRule is_active_rule;
    is_active_rule.required = false;
    is_active_rule.type = "BOOL";

    // Assign rules to the GET method
    query_param_rules.get["limit"] = limit_rule;
    query_param_rules.get["sort"] = sort_rule;
    query_param_rules.get["is_active"] = is_active_rule;

    return query_param_rules; // No query parameters expected
}

HttpStatus Hello::get(std::string& response_body ) {
    response_body =  "Hello World get";
    const std::string a = "limit";
    int limit = query_params.get(a);

    std::string name = path_params.get("NAME");

	std::cout << "Inside Hello_ID Get method , Query Param Limit: " << limit << " Path Param Name: "<< name << std::endl;
    return HTTP_200_OK; // 200 OK for successful GET
}

HttpStatus Hello::post(std::string& response_body) {
   response_body = "Hello World post";
   return HTTP_201_CREATED; // 201 Created is often used for successful POST
}

HttpStatus Hello::put(std::string& response_body) {
   response_body = "Hello World put";
   return HTTP_200_OK; // 200 OK or 204 No Content for successful PUT
}

HttpStatus Hello::delete_(std::string& response_body) {
   response_body = "Hello World delete";
   return HTTP_204_NO_CONTENT; // 204 No Content is typical for successful DELETE
}
