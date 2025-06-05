#include <string>  
#include <ctime>  
#include <iomanip>  
#include <sstream>  
#include <map>
#include <vector>
#include <algorithm>
#include "http_status.hpp"
#include "StringUtils.hpp"

#include "utils/type_checker.hpp"

#include "ResourceMapper.hpp"

enum Method {  
   GET,  
   POST,  
   PUT,  
   DELETE_  
};  

namespace utils {  
   std::string get_http_date();  
   std::map<std::string, std::string> parse_headers(const std::string& );    
   std::string _extract_uri(const std::string& );
   std::string _exctract_method(const std::string& );
   std::string construct_http_response(int status_code, const std::string& response_body = "", const std::string& content_type = "application/json");
   std::string construct_404_response();
   std::string find_match(std::string&, std::map<std::string, std::unique_ptr<IResource>>&, DynamicDict&, bool& );
}
