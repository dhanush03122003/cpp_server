#pragma once

#include <string>
#include <map>
#include <vector>

struct ParamRule {
    bool required;
    std::string type;  // Supported: INT, FLOAT, STR, ALNUM, UUID, BOOL, ENUM

    // Optional fields
    std::vector<std::string> allowed_values;  // For ENUM
    std::string min_value; // For INT, FLOAT
    std::string max_value;
};

struct QueryParamRules {
    std::map<std::string, ParamRule> get;
    std::map<std::string, ParamRule> post;
    std::map<std::string, ParamRule> put;
    std::map<std::string, ParamRule> delete_;
};