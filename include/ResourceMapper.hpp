#pragma once

#include <map>
#include <string>
#include <memory>

#include "IResource.hpp"

class ResourceMapper {
public :
	std::map<std::string, std::unique_ptr<IResource>> resource_mapper;

	ResourceMapper();
	ResourceMapper(const ResourceMapper&) = delete;
	ResourceMapper& operator=(const ResourceMapper&) = delete;
	
	ResourceMapper(ResourceMapper&&) = default;
	ResourceMapper& operator=(ResourceMapper&&) = default;

	template<typename T>
	void register_uri(const std::string& uri);

};
