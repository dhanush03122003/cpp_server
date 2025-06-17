#pragma once
#include "IResource.hpp"

#include <string>
#include <iostream>


class Hello_Id : public IResource {
public:
	QueryParamRules query_param_rules() override;
	HttpStatus get(std::string&) override;
	HttpStatus put(std::string&) override;
	HttpStatus post(std::string&) override;
	HttpStatus delete_(std::string&) override;
};