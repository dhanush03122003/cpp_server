#include "Hello.hpp"

HttpStatus Hello::get(std::string& response_body) {
    response_body =  "Hello World get\n";
    return HTTP_200_OK; // 200 OK for successful GET
}

HttpStatus Hello::post(std::string& response_body) {
   response_body = "Hello World post\n";
    return HTTP_201_CREATED; // 201 Created is often used for successful POST
}

HttpStatus Hello::put(std::string& response_body) {
   response_body = "Hello World put\n";
    return HTTP_200_OK; // 200 OK or 204 No Content for successful PUT
}

HttpStatus Hello::delete_(std::string& response_body) {
   response_body = "Hello World delete\n";
    return HTTP_204_NO_CONTENT; // 204 No Content is typical for successful DELETE
}
