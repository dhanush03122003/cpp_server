cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(cpp_server)

# Include directories
include_directories(include include/api include/utils)


# Prevent building tests for fetched dependencies
set(JSON_BuildTests OFF CACHE INTERNAL "")  # For nlohmann/json
set(BUILD_TESTING OFF CACHE INTERNAL "")    # Generic flag for many libraries
set(JSON_VALIDATOR_BUILD_TESTS OFF CACHE INTERNAL "")  # For json-schema-validator


include(FetchContent)

# Fetch nlohmann/json
FetchContent_Declare(
  json
 GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.2
)

# Fetch json-schema-validator
FetchContent_Declare(
  json_schema_validator
  GIT_REPOSITORY https://github.com/pboettch/json-schema-validator.git
  GIT_TAG        2.3.0
)

# Find OpenSSL package
# find_package(OpenSSL REQUIRED)

# Check if OpenSSL is found
#if (NOT OPENSSL_FOUND)
#    message(FATAL_ERROR "OpenSSL not found!")
#endif()

# Add executable
add_executable(cpp_server
    src/main.cpp
    src/SocketHandler.cpp
    src/Server.cpp
    src/HttpsClient.cpp 
    src/ResourceMapper.cpp 
    src/api/Hello.cpp
    src/utils/HttpUtils.cpp
    src/utils/StringUtils.cpp 
    src/utils/type_checker.cpp  
    src/api/Hello_Id.cpp
    )

FetchContent_MakeAvailable(json json_schema_validator)

# Link OpenSSL and ws2_32
target_link_libraries(cpp_server PRIVATE 
ws2_32 
# OpenSSL::SSL 
# OpenSSL::Crypto 
nlohmann_json::nlohmann_json
nlohmann_json_schema_validator
)
