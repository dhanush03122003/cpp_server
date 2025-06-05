#pragma once

#include <utils/http_status.hpp>

class IResource {
public:
    virtual ~IResource() = default;
    virtual HttpStatus get(std::string&)  =0 ;
    virtual HttpStatus post(std::string&) = 0;
    virtual HttpStatus put(std::string&) = 0;
    virtual HttpStatus delete_(std::string&) = 0;

};
