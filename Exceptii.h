#pragma once
#include <exception>
#include <string>

class JocError : public std::exception {
protected:
    std::string mesaj;
public:
    explicit JocError(const std::string& m) : mesaj(">>> EROARE JOC: " + m) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

class InputInvalid : public JocError {
public:
    using JocError::JocError;
};

class ResurseInsuficiente : public JocError {
public:
    using JocError::JocError;
};

class ConfigurareInvalida : public JocError {
public:
    using JocError::JocError;
};
