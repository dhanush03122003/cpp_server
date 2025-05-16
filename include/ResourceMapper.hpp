#include <map>
#include <string>
#include <memory>

#include "IResource.hpp"

//class ResourceMapper {
//protected:
//	std::map<std::string, std::unique_ptr<IResource>> resource_mapper;
//public:
//	ResourceMapper();
//
//};

class ResourceMapper {
protected:
    std::map<std::string, std::unique_ptr<IResource>> resource_mapper;

public:
    ResourceMapper();
    ResourceMapper(const ResourceMapper&) = delete;
    ResourceMapper& operator=(const ResourceMapper&) = delete;

    ResourceMapper(ResourceMapper&&) = default;
    ResourceMapper& operator=(ResourceMapper&&) = default;
};
