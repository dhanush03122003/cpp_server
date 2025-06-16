#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include <string>
#include <unordered_map>
#include <functional>

#include "StringUtils.hpp"



#include <any>
#include <iostream>
#include <typeinfo>

class ValueProxy {
    std::any data;
    std::string type;
    std::string key;  // For better error messages

    public:
        ValueProxy(std::any data, const std::string& type, const std::string& key)
            : data(std::move(data)), type(type), key(key) {
        }

        operator int() const {
            try {
                if (type == "INT" || type == "SIGNED_INT")
                    return std::any_cast<int>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<int>(e);
            }
        }

        operator float() const {
            try {
                if (type == "FLOAT")
                    return std::any_cast<float>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<float>(e);
            }
        }

        operator std::string() const {
            try {
                if (type == "STR" || type == "ALNUM" || type == "UUID" || type == "ENUM")
                    return std::any_cast<std::string>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<std::string>(e);
            }
        }

        operator bool() const {
            try {
                if (type == "BOOL")
                    return std::any_cast<bool>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<bool>(e);
            }
        }
        operator double() const {
            try {
                if (type == "DOUBLE")
                    return std::any_cast<double>(data);
                throw std::bad_any_cast();
            } 
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<double>(e);
            }
        }

        operator long() const {
            try {
                if (type == "LONG")
                    return std::any_cast<long>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<long>(e);
            }
        }

        operator long long() const {
            try {
                if (type == "LONG_LONG")
                    return std::any_cast<long long>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<long long>(e);
            }
        }

        operator char() const {
            try {
                if (type == "CHAR")
                    return std::any_cast<char>(data);
                throw std::bad_any_cast();
            }
            catch (const std::bad_any_cast& e) {
                throw_type_mismatch<char>(e);
            }
        }


    private:
        template<typename T>
        [[noreturn]] void throw_type_mismatch(const std::bad_any_cast& e) const {
            throw std::runtime_error(
                "Type mismatch for key '" + key +
                "': stored type is " + type +
                ", requested type is " + typeid(T).name() +
                ". Exception: " + e.what()
            );
        }
};




class DynamicDict {
    struct Value {
        std::any data;
        std::string type;
    };
    std::unordered_map<std::string, Value> data;


public:
    void set(const std::string& key, const std::string& type, const std::string& value);

    void clear();
    ValueProxy get(const std::string& key) const;

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
    bool is_matching_data_type(const std::string& uri, const std::string& pattern);

}

#endif // TYPE_CHECKER_HPP
