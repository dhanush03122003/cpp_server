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
QueryParamRules HelloHandler::query_param_rules() {
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
class HelloHandler : public Resource {
public:
    HttpStatus get(std::string& response_body) override {
        int limit = query_params.get("limit");
        response_body = "Hello World! Limit = " + std::to_string(limit) + "\n";
        return HTTP_200_OK;
    }

    HttpStatus post(std::string& response_body) override {
        response_body = "Created Hello Resource\n";
        return HTTP_201_CREATED;
    }

    HttpStatus put(std::string& response_body) override {
        response_body = "Updated Hello Resource\n";
        return HTTP_200_OK;
    }

    HttpStatus delete_(std::string& response_body) override {
        response_body = "Deleted Hello Resource\n";
        return HTTP_204_NO_CONTENT;
    }
};
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
cmake ..
cmake --build .
```

---

## 📢 Contributing

Contributions are welcome! Please open issues for bug reports or feature requests, and submit pull requests for enhancements.

