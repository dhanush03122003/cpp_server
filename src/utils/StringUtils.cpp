#include "StringUtils.hpp"

std::string StringUtils::trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}

// Strip a specific char from the left
std::string StringUtils::lstrip(const std::string& s, char ch) {
    auto start = std::find_if(s.begin(), s.end(),
        [ch](unsigned char c) { return c != ch; });
    return std::string(start, s.end());
}

// Strip a specific char from the right
std::string StringUtils::rstrip(const std::string& s, char ch) {
    auto end = std::find_if(s.rbegin(), s.rend(),
        [ch](unsigned char c) { return c != ch; }).base();
    return std::string(s.begin(), end);
}

// Strip a specific char from both ends
std::string StringUtils::strip(const std::string& s, char ch) {
    auto start = std::find_if(s.begin(), s.end(),
        [ch](unsigned char c) { return c != ch; });
    auto end = std::find_if(s.rbegin(), s.rend(),
        [ch](unsigned char c) { return c != ch; }).base();

    if (start >= end) return "";

    return std::string(start, end);
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        parts.push_back(item);
    }
    return parts;
}

bool StringUtils::is_match_with(const std::string& uri, const std::string& pattern) {
    auto uriParts = split(uri, '/');
    auto patternParts = split(pattern, '/');

    if (uriParts.size() != patternParts.size())
        return false;

    for (size_t i = 0; i < patternParts.size(); ++i) {
        const auto& p = patternParts[i];
        const auto& u = uriParts[i];

        if (p.empty()) continue; // handle leading '/'

        // if it's a placeholder like <INT:id>
        if (p.front() == '<' && p.back() == '>') {
            // Optionally, you can check the type in the placeholder here
            continue;
        }

        if (p != u) {
            return false;
        }
    }

    return true;
}