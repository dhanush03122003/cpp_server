#pragma once
#include "IResource.hpp"

#include <string>
#include <iostream>

class Hello : public IResource {
public:
	HttpStatus get(std::string&) override ;
	HttpStatus put(std::string&) override;
	HttpStatus post(std::string&) override;
	HttpStatus delete_(std::string&) override;
};