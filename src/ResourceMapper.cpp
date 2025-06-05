#include "api/hello.hpp"
#include "ResourceMapper.hpp"


ResourceMapper :: ResourceMapper() {

	resource_mapper["/hello"] = std::make_unique<Hello>();
	resource_mapper["/hello/<INT:ID>"] = std::make_unique<Hello>();
	resource_mapper["/hello/<STR:ID>"] = std::make_unique<Hello>();
}