#include <iostream>
#include "Log.h"

class ErrorBase : public std::exception, public DebugFormat {
private:
    std::string contents;

protected:
    ErrorBase(std::string contents) : contents(contents) {}
public:
    ErrorBase() = default;
    ErrorBase(const ErrorBase& e) = default;
    ErrorBase(ErrorBase&& e) = default;
    virtual ~ErrorBase() {}

    const char* what() const noexcept override { return contents.c_str(); }
    void dbg_fmt(std::ostream& out) const noexcept override { out << contents; }
}; 

class ArgumentError : public ErrorBase {
public:
    ArgumentError(const std::string& name, const std::string& value);
    ArgumentError(const std::string& name, const DebugFormat& value);
};

class NullError : public ErrorBase {
public:
    NullError(const std::string& name);
};

class FormatError : public ErrorBase {
public:
    FormatError(const std::string& target, const std::string& reason);
};

class RangeError : public ErrorBase {
public:
    template<typename T> 
    RangeError(const std::string& variable, T value, T min, T max);
};

class NotFoundError : public ErrorBase {
public:
    NotFoundError(const std::string& identifier);
    NotFoundError(const DebugFormat& identifier);
};

class OperatorError : public ErrorBase {
public:
    OperatorError(char oper, const std::string& operand1, const std::string& operand2 = std::string());
    OperatorError(const std::string& oper, const std::string& operand1, const std::string& operand2 = std::string());
    OperatorError(char oper, const DebugFormat& operand1);
    OperatorError(const std::string& oper, const DebugFormat& operand1, const DebugFormat& operand2);
    OperatorError(const DebugFormat& oper, const DebugFormat& operand1);
    OperatorError(const DebugFormat& oper, const DebugFormat& operand1, const DebugFormat& operand2);
};

class UnexepctedError : public ErrorBase {
public:
    UnexepctedError(const std::string& reason);
};

class OperationError : public ErrorBase {
public:
    OperationError(const std::string& action, const std::string& reason);
};

class ConversionError : public ErrorBase {
public:
    ConversionError(const std::string& action, const std::string& reason);
};

enum IOErrorKind {
    IEK_NotFound,
    IEK_Permissions,
    IEK_Invalid
};

class IOError : public ErrorBase {
public:
    IOError(const std::string& location, IOErrorKind kind);
};

/// @brief A function that converts the arguments into a single string
/// @tparam ...Args The types of the arguments
/// @param ...value The values to put into a string
/// @return A full string that contains the arguments in order
template<typename... Args>
std::string reason_formatter(Args... value) noexcept {
    std::vector<void*> to_print( &(value)... );
    std::stringstream ss;
    for (const void*& item : to_print)
        ss << *item;

    return ss.str();
}