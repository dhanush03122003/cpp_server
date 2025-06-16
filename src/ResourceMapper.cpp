#include "api/hello.hpp"
#include "ResourceMapper.hpp"

template<typename T>
void ResourceMapper::register_uri(const std::string& uri) {
	resource_mapper[uri] = std::make_unique<T>();
}

ResourceMapper :: ResourceMapper() {

	register_uri<Hello>("/hello");
	register_uri<Hello>("/hello/<INT:ID>");
	register_uri<Hello>("/hello/<STR:ID>");
}
