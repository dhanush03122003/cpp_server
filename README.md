⚙️ **C++ Lightweight REST Server**

A modular and extensible RESTful server implemented in modern C++17, built from the ground up using raw TCP sockets and WebSockets. Designed for flexibility and performance, this server supports HTTP/HTTPS, custom routing, and strict type-checked URI parameters without relying on external frameworks.

---

## 🚀 Key Features

- **Full REST Compliance**: Native support for GET, POST, PUT, and DELETE methods.
- **Flexible Routing**: Define endpoints with dynamic path variables and custom type constraints.
- **Typed Parameters**: Built-in parsing for INT, SIGNED_INT, FLOAT, STR, ALNUM, UUID, and more.
- **Query Validation**: Enforce required parameters, type checks, range constraints, and enumeration rules.
- **Extensible Architecture**: Each endpoint is a self-contained C++ class; override methods to customize behavior.

---

## 📦 Prerequisites

- C++17-compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake ≥ 3.10

---

## 🔠 Defining Routes

Use `ResourceMapper` to register endpoints with path patterns. Supported types include:

| Type         | Description            |
| ------------ | ---------------------- |
| `INT`        | Unsigned integer       |
| `SIGNED_INT` | Signed integer         |
| `FLOAT`      | Floating-point number  |
| `STR`        | Generic string         |
| `ALNUM`      | Alphanumeric string    |
| `UUID`       | 8-4-4-4-12 UUID format |

```cpp
// Register endpoints in ResourceMapper.cpp
    register_uri<Hello>("/hello");
    register_uri<Hello_Id>("/hello/<INT:ID>");
```

---

## 🧾 Query Parameter Validation

Define `QueryParamRules` within your handler class to enforce:

- **Presence** (`required`)
- **Type** (`INT`, `STR`, `BOOL`, `ENUM`)
- **Range** (`min_value`, `max_value`)
- **Allowed Values** (for enumerations)

```cpp
QueryParamRules Hello::query_param_rules() {
    QueryParamRules rules;

    ParamRule limit;
    limit.required = true;
    limit.type     = "INT";
    limit.min_value = "1";
    limit.max_value = "100";
    rules.get["limit"] = limit;

    ParamRule sort;
    sort.required = false;
    sort.type     = "ENUM";
    sort.allowed_values = {"asc", "desc"};
    rules.get["sort"] = sort;

    return rules;
}
```

---

## 🧱 Example Endpoint Implementation

```cpp
// include/api/Hello.hpp
#pragma once
#include "IResource.hpp"

#include <string>
#include <iostream>


class Hello : public IResource {
public:
	QueryParamRules query_param_rules() override;
	HttpStatus get(json&) override;
	HttpStatus put(json&) override;
	HttpStatus post(json&) override;
	HttpStatus delete_(json&) override;
};

```

---

```cpp
// src/api/Hello.cpp
#include "Hello.hpp"
QueryParamRules Hello::query_param_rules() {

    QueryParamRules query_param_rules;

    ParamRule limit_rule;
    limit_rule.required = true;
    limit_rule.type = "INT";
    limit_rule.min_value = "1";
    limit_rule.max_value = "100";

    ParamRule sort_rule;
    sort_rule.required = false;
    sort_rule.type = "ENUM";
    sort_rule.allowed_values = { "asc", "desc" };

    ParamRule is_active_rule;
    is_active_rule.required = false;
    is_active_rule.type = "BOOL";

    // Assign rules to the GET method
    query_param_rules.get["limit"] = limit_rule;
    query_param_rules.get["sort"] = sort_rule;
    query_param_rules.get["is_active"] = is_active_rule;

    return query_param_rules;
}

HttpStatus Hello::get(json& response_body) {
    const std::string a = "limit";
    int limit = query_params.get(a);  // ValueProxy allows implicit cast

    std::string name = path_params.get("NAME");

    std::cout << "Inside Hello_ID Get method, Query Param Limit: " << limit
        << " Path Param Name: " << name << std::endl;


    response_body["message"] = "Hello World GET";
    response_body["limit"] = limit;
    response_body["name"] = name;

    return HTTP_200_OK;
}

HttpStatus Hello::post(json& response_body) {
    response_body["message"] = "Hello World POST";

    std::string string_value = payload["string_key"];
	int int_value = payload["int_key"];

	response_body["string_value"] = string_value;
	response_body["int_value"] = int_value;

    return HTTP_201_CREATED;
}

HttpStatus Hello::put(json& response_body) {
    response_body["message"] = "Hello World PUT";
    return HTTP_200_OK;
}

HttpStatus Hello::delete_(json& response_body) {
    response_body["message"] = "Hello World DELETE";
    return HTTP_204_NO_CONTENT;
}


```

---

## 📝 Update CmakeLists.txt

Ensure your `CMakeLists.txt` includes the necessary components:

```cmake
add_executable(...
                src/api/Hello.cpp
)

```

---

## 🛠️ Building the Project

```bash
git clone https://github.com/your-username/cpp_server.git
cd cpp_server
mkdir build && cd build
cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build .
cd Debug
cpp_server.exe
```

---

## Sample Curl Cmd

```bash
curl -X POST "http://localhost:8080/hello/1" -H "Content-Type: application/json" -d "{\"int_key\":1, \"string_key\":\"value\"}"
{
    "int_value": 1,
    "message": "Hello World POST",
    "string_value": "value"
}
```

## 📢 Contributing

Contributions are welcome! Please open issues for bug reports or feature requests, and submit pull requests for enhancements.
