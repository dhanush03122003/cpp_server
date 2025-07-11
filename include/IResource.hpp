#pragma once

#include <utils/http_status.hpp>
#include "QueryParamRules.hpp"
#include "type_checker.hpp"



#include <nlohmann/json.hpp>
using json = nlohmann::json;

class IResource {
public:
    //IResource(int x, int y) {

    //    std::cout << x;
    //}
    virtual ~IResource() = default;

    DynamicDict query_params;
    DynamicDict path_params;
    json payload;

    virtual QueryParamRules query_param_rules() = 0;

    virtual HttpStatus get(json&)  =0 ;
    virtual HttpStatus post(json&) = 0;
    virtual HttpStatus put(json&) = 0;
    virtual HttpStatus delete_(json&) = 0;

};
