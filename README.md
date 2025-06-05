# ⚙️ C++ Lightweight REST Server

A modular, lightweight RESTful server built in modern **C++17** using **raw TCP sockets** and **WebSockets**, with support for **HTTP/HTTPS**, custom request routing, and flexible URI pattern matching.

---

## 🚀 Features

- ✅ Implements REST principles (`GET`, `POST`, `PUT`, `DELETE`)
- 🔒 Supports both HTTP and HTTPS (via OpenSSL)
- ⚙️ Modular endpoint registration using `ResourceMapper`
- 🧠 Custom path variable parsing with type constraints
- 🧩 Typed path parameters: `INT`, `SIGNED_INT`, `FLOAT`, `STR`, `ALNUM`, `UUID`
- 🧱 Each endpoint implemented as a class with overridable REST methods
- 🔌 Built from scratch using **raw sockets** (no third-party HTTP frameworks)

---

## 📦 Requirements

- **C++17** compatible compiler
- **CMake** ≥ 3.10
- **OpenSSL** (required for HTTPS support)

---

## 🗂️ URI Routing with ResourceMapper

Endpoints are defined in `ResourceMapper.cpp` using path patterns:

```cpp
resource_mapper["/hello"] = std::make_unique<Hello>();
resource_mapper["/hello/<INT:ID>"] = std::make_unique<Hello>();
resource_mapper["/hello/<STR:ID>"] = std::make_unique<Hello>();
```

### 🔠 Supported Path Types

| Type         | Description                |
|--------------|----------------------------|
| INT          | Unsigned integer           |
| SIGNED_INT   | Signed integer             |
| FLOAT        | Floating point number      |
| STR          | String                     |
| ALNUM        | Alphanumeric string        |
| UUID         | UUID format (8-4-4-4-12)  |

---

## 🧱 Example Endpoint Class

Each resource class (like `Hello`) overrides HTTP methods:

```cpp
#include "Hello.hpp"

HttpStatus Hello::get(std::string& response_body) {
    std::string id = path_args.get["ID"];
    response_body = "Hello World get\n";
    return HTTP_200_OK;
}

HttpStatus Hello::post(std::string& response_body) {
    response_body = "Hello World post\n";
    return HTTP_201_CREATED;
}

HttpStatus Hello::put(std::string& response_body) {
    response_body = "Hello World put\n";
    return HTTP_200_OK;
}

HttpStatus Hello::delete_(std::string& response_body) {
    response_body = "Hello World delete\n";
    return HTTP_204_NO_CONTENT;
}
```

---

<!-- ## 🔐 HTTPS Support

To enable secure connections:

- Install OpenSSL and ensure it is linked in `CMakeLists.txt`
- SSL certificates are loaded by the server to serve HTTPS traffic
- No third-party frameworks are used; only OpenSSL APIs

--- -->

## 🛠️ Build Instructions

To build the server, follow these steps:

```bash
git clone https://github.com/your-username/your-repo.git
cd your-repo
mkdir build && cd build
cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build .
```

---

<!-- ## 📤 Contributing

Contributions are welcome! Feel free to open issues or submit PRs to improve functionality, fix bugs, or add more features.

---

## 📜 License

This project is licensed under the MIT License. -->
