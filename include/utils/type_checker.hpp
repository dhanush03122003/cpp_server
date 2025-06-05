#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include <string>
#include <unordered_map>
#include <functional>

#include "StringUtils.hpp"



#include <any>
#include <iostream>
#include <typeinfo>
class DynamicDict {
    std::unordered_map<std::string, std::any> data;

public:
    void set(const std::string& key, const std::string& type, const std::string& value);

    void clear();
    template<typename T>
    T get(const std::string& key) const;

    /*void print() const;*/
};

namespace TypeChecker {

    using TypeCheckerFn = std::function<bool(const std::string&)>;

    // Type checker functions declarations
    bool isInteger(const std::string& s);
    bool isSignedInteger(const std::string& s);
    bool isFloat(const std::string& s);
    bool isString(const std::string& s);
    bool isAlphaNum(const std::string& s);
    bool isUUID(const std::string& s);

    // Returns the map from type names to checker functions
    std::unordered_map<std::string, TypeCheckerFn>& getTypeCheckers();

    // Check if the value matches the type specified by typeName
    bool checkType(const std::string& typeName, const std::string& value);
    bool is_matching_data_type(const std::string& uri, const std::string& pattern, DynamicDict& path_args);

}

template<typename T>
T DynamicDict::get(const std::string& key) const {
    auto it = data.find(key);
    if (it == data.end())
        throw std::runtime_error("Key not found: " + key);

    try {
        return std::any_cast<T>(it->second);
    }
    catch (const std::bad_any_cast& e) {
        std::string whatMsg = e.what();
        std::cout<<
    "Type mismatch for key '" + key +
    "': stored type is " + it->second.type().name() +
    ", requested type is " + typeid(T).name() +
    ". Exception: " + whatMsg;


        //throw std::runtime_error(
        //    "Type mismatch for key '" + key +
        //    "': stored type is " + it->second.type().name() +
        //    ", requested type is " + typeid(T).name() +
        //    ". Exception: " + whatMsg
        //);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
#endif // TYPE_CHECKER_HPP
