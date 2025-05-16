cmake_minimum_required(VERSION 3.10)
project(cpp_server)

# Include directories
include_directories(include include/api include/utils)

# Find OpenSSL package
find_package(OpenSSL REQUIRED)

# Check if OpenSSL is found
if (NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found!")
endif()

# Add executable
add_executable(cpp_server
    src/main.cpp
    src/SocketHandler.cpp
    src/Server.cpp
    src/HttpsClient.cpp 
    src/ResourceMapper.cpp 
    src/api/Hello.cpp
    src/utils/HttpUtils.cpp
)

# Link OpenSSL and ws2_32
target_link_libraries(cpp_server PRIVATE ws2_32 OpenSSL::SSL OpenSSL::Crypto)
