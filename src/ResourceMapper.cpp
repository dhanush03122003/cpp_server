#include "api/hello.hpp"
#include "ResourceMapper.hpp"


ResourceMapper :: ResourceMapper() {

	resource_mapper["/hello"] = std::make_unique<Hello>();
   }

