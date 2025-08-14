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

    return query_param_rules;
}

HttpStatus Hello::get(json& response_body) {
    const std::string a = "limit";
    int limit = query_params.get(a);  // ValueProxy allows implicit cast

    std::string name = path_params.get("NAME");

    std::cout << "Inside Hello_ID Get method, Query Param Limit: " << limit
        << " Path Param Name: " << name << std::endl;
    
    
    response_body["message"] = "Hello World GET";
    response_body["limit"] = limit;
    response_body["name"] = name;

    return HTTP_200_OK;
}

HttpStatus Hello::post(json& response_body) {
    response_body["message"] = "Hello World POST";

    std::string string_value = payload["string_key"];
	int int_value = payload["int_key"];
    
	response_body["string_value"] = string_value;
	response_body["int_value"] = int_value;

    return HTTP_201_CREATED;
}

HttpStatus Hello::put(json& response_body) {
    response_body["message"] = "Hello World PUT";
    return HTTP_200_OK;
}

HttpStatus Hello::delete_(json& response_body) {
    response_body["message"] = "Hello World DELETE";
    return HTTP_204_NO_CONTENT;
}
