#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace StringUtils {

	std::string trim(const std::string&);
	std::vector<std::string> split(const std::string& str, char delimiter);
	bool is_match_with(const std::string& uri, const std::string& pattern);
	std::string lstrip(const std::string& s, char ch);
	std::string rstrip(const std::string& s, char ch);
	std::string strip(const std::string& s, char ch);

}