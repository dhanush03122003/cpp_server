#pragma once
#include "IResource.hpp"

#include <string>
#include <iostream>


class Hello : public IResource {
public:
	QueryParamRules query_param_rules() override;
	HttpStatus get(json&) override;
	HttpStatus put(json&) override;
	HttpStatus post(json&) override;
	HttpStatus delete_(json&) override;
};