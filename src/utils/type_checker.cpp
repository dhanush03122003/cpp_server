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
        {"LONG", isSignedInteger},
        {"LONG_LONG", isSignedInteger},
        {"FLOAT", isFloat},
        {"DOUBLE", isFloat},
        {"CHAR", [](const std::string& s) { return s.size() == 1; }},
        {"STR", isString},
        {"ALNUM", isAlphaNum},
        {"UUID", isUUID},
        {"ENUM", isAlphaNum},
        {"BOOL", [](const std::string& s) {
            return s == "true" || s == "false" || s == "0" || s == "1";
        }}
    };

    return typeCheckers;
}

bool TypeChecker::checkType(const std::string& typeName, const std::string& value) {
    auto& checkers = getTypeCheckers();
    auto it = checkers.find(typeName);
    if (it == checkers.end()) return false;
    return it->second(value);
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
    Value v{};
    v.type = type;
    // Convert and store
    if (type == "INT" || type == "SIGNED_INT") {
        v.data = std::stoi(value);
    }
    else if (type == "LONG") {
        v.data = std::stol(value);
    }
    else if (type == "LONG_LONG") {
        v.data = std::stoll(value);
    }
    else if (type == "DOUBLE") {
        v.data = std::stod(value);
    }
    else if (type == "FLOAT") {
        v.data = std::stof(value);
    }
    else if (type == "CHAR") {
        if (value.length() != 1) {
            std::cerr << "Value \"" << value << "\" is not a valid char\n";
            return;
        }
        v.data = value[0];
    }
    else if (type == "STR" || type == "ALNUM" || type == "UUID" || type == "ENUM") {
        v.data = value;
    }

	data[key] = v;
}

ValueProxy DynamicDict::get(const std::string& key) const {
    auto it = data.find(key);
    if (it == data.end()) {
        throw std::runtime_error("Key not found: " + key);
    }
    return ValueProxy(it->second.data, it->second.type, key);
}