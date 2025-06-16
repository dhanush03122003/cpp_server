#pragma once

#include <utils/http_status.hpp>
#include "QueryParamRules.hpp"
#include "type_checker.hpp"
class IResource {
public:
    //IResource(int x, int y) {

    //    std::cout << x;
    //}
    virtual ~IResource() = default;

    DynamicDict query_params;
    DynamicDict path_params;

    virtual QueryParamRules query_param_rules() = 0;

    virtual HttpStatus get(std::string&)  =0 ;
    virtual HttpStatus post(std::string&) = 0;
    virtual HttpStatus put(std::string&) = 0;
    virtual HttpStatus delete_(std::string&) = 0;

};
