#include <string>  
#include <ctime>  
#include <iomanip>  
#include <sstream>  
#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include "http_status.hpp"
#include "StringUtils.hpp"

#include "utils/type_checker.hpp"
#include "utils/QueryParamRules.hpp"


#include "ResourceMapper.hpp"

enum Method {  
   GET,  
   POST,  
   PUT,  
   DELETE_  
};  

#define VALID_PATH_PARAM_DATA_TYPES "INT", "SIGNED_INT", "FLOAT", "STR", "ALNUM", "UUID"

#define VALID_QUERY_PARAM_DATA_TYPES "INT", "BOOL", "STR", "ENUM", 


namespace utils {  
   std::string get_http_date();  
   std::map<std::string, std::string> parse_headers(const std::string& );    
   std::string _extract_uri(const std::string& );
   std::string _exctract_method(const std::string& );
   std::map<std::string, std::string> _extract_query_args(const std::string&);
   std::string construct_http_response(int status_code, const std::string& response_body = "", const std::string& content_type = "application/json");
   std::string construct_404_response();
   std::string find_match(std::string&, std::map<std::string, std::unique_ptr<IResource>>&, bool& );
   bool is_matching_data_type(const std::string& uri, const std::string& pattern);

   bool valid_path_params(const std::string uri, const std::string uri_pattern, std::string& path_params_error_msg);
   bool valid_query_params(
       const std::string& method,
       const std::map<std::string, std::string>& query_args,
       const QueryParamRules& rules,
       std::string& error_out
   );
   void set_params(
       DynamicDict& path_params,
       const std::string uri,
       const std::string uri_pattern,
       DynamicDict& query_params,
       std::map<std::string, std::string> query_args_from_uri,
       const QueryParamRules& rules,
       const std::string method
   );
}
