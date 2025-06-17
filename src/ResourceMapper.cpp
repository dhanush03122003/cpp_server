#include "api/Hello.hpp"
#include "api/Hello_Id.hpp"
#include "ResourceMapper.hpp"

template<typename T>
void ResourceMapper::register_uri(const std::string& uri) {
	resource_mapper[uri] = std::make_unique<T>();
}

ResourceMapper :: ResourceMapper() {

	register_uri<Hello>("/hello/<STR:NAME>");
	register_uri<Hello_Id>("/hello_id/<INT:ID>");

}
