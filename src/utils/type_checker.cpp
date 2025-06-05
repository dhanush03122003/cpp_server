#include "type_checker.hpp"
#include <sstream>
#include <cctype>

// Define functions with full namespace qualification

bool TypeChecker::isInteger(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool TypeChecker::isSignedInteger(const std::string& s) {
    if (s.empty()) return false;
    size_t start = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    if (start == s.size()) return false;
    for (size_t i = start; i < s.size(); ++i) {
        if (!std::isdigit(s[i])) return false;
    }
    return true;
}

bool TypeChecker::isFloat(const std::string& s) {
    if (s.empty()) return false;
    std::istringstream iss(s);
    float f;
    char c;
    if (!(iss >> f)) return false;
    return !(iss >> c);
}

bool TypeChecker::isString(const std::string& s) {
    if (isFloat(s) || isSignedInteger(s) || isInteger(s)) {
        return false;
    }
    return !s.empty() && s.find('/') == std::string::npos;
}

bool TypeChecker::isAlphaNum(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isalnum(c)) return false;
    }
    return true;
}

bool TypeChecker::isUUID(const std::string& s) {
    if (s.size() != 36) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            if (s[i] != '-') return false;
        }
        else if (!std::isxdigit(s[i])) {
            return false;
        }
    }
    return true;
}

std::unordered_map<std::string, TypeChecker::TypeCheckerFn>& TypeChecker::getTypeCheckers() {
    static std::unordered_map<std::string, TypeCheckerFn> typeCheckers{
        {"INT", isInteger},
        {"SIGNED_INT", isSignedInteger},
        {"FLOAT", isFloat},
        {"STR", isString},
        {"ALNUM", isAlphaNum},
        {"UUID", isUUID}
    };
    return typeCheckers;
}

bool TypeChecker::checkType(const std::string& typeName, const std::string& value) {
    auto& checkers = getTypeCheckers();
    auto it = checkers.find(typeName);
    if (it == checkers.end()) return false;
    return it->second(value);
}

bool TypeChecker::is_matching_data_type(const std::string& uri, const std::string& pattern , DynamicDict& path_args) {
    

    path_args.set("id", "INT", "10");
    auto uriParts = StringUtils::split(uri, '/');
    auto patternParts = StringUtils::split(pattern, '/');

    if (uriParts.size() != patternParts.size())
        return false;

    for (size_t i = 0; i < patternParts.size(); ++i) {
        const auto& p = patternParts[i];
        const auto& u = uriParts[i];

        if (!p.empty() && p.front() == '<' && p.back() == '>') {
            size_t colonPos = p.find(':');
            if (colonPos == std::string::npos)
                return false;  // malformed pattern

            // Extract type substring between '<' and ':'
            std::string type = p.substr(1, colonPos - 1);

            // Use TypeChecker::checkType to validate
            if (!TypeChecker::checkType(type, u))
                return false;  // value doesn't match the expected type
        }
        else {
            if (p != u)
                return false;  // literal path segment mismatch
        }
    }

    return true;
}


void DynamicDict::clear() {
    data.clear();
}

void DynamicDict::set(const std::string& key, const std::string& type, const std::string& value) {
    auto checkerIt = TypeChecker::getTypeCheckers().find(type);
    if (checkerIt == TypeChecker::getTypeCheckers().end()) {
        std::cerr << "Unknown type: " << type << std::endl;
        return;
    }

    // Validate value
    if (!checkerIt->second(value)) {
        std::cerr << "Value \"" << value << "\" does not match type " << type << std::endl;
        return;
    }

    // Convert and store
    if (type == "INT" || type == "SIGNED_INT") {
        data[key] = std::stoi(value);
    }
    else if (type == "FLOAT") {
        data[key] = std::stof(value);
    }
    else if (type == "STR" || type == "ALNUM" || type == "UUID") {
        data[key] = value;
    }
    else {
        data[key] = value;  // fallback as string
    }
}