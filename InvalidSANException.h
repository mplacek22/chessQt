#pragma once

#include <stdexcept>

class InvalidSanException : public std::runtime_error {
public:
    explicit InvalidSanException(const std::string& msg)
        : std::runtime_error("Invalid standard chess notation: " + msg) {}
};