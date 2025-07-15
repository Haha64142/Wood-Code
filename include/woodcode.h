#pragma once

#include <unordered_map>
#include <string>

enum class ResultCode
{
    Success,
    Warning,
    Error,
};

struct SimpleResult
{
    ResultCode status;
    std::string message;

    // Factory: Success
    // Usage SimpleResult::Ok()
    static SimpleResult Ok() { return {ResultCode::Success, ""}; }

    // Factory: Warning
    // Usage SimpleResult::Warn("message")
    static SimpleResult Warn(const std::string &msg) { return {ResultCode::Warning, msg}; }

    // Factory: Error
    // Usage SimpleResult::Err("message")
    static SimpleResult Err(const std::string &msg) { return {ResultCode::Error, msg}; }

    bool isOk() const { return status == ResultCode::Success; }
    bool isWarning() const { return status == ResultCode::Warning; }
    bool isError() const { return status == ResultCode::Error; }

    // Treat the result like a bool
    explicit operator bool() const
    {
        return status != ResultCode::Error;
    }
};

template <typename T>
struct Result
{
    ResultCode status;
    T value;
    std::string message;

    // Factory: Success
    // Usage Result::Ok(value)
    static Result<T> Ok(const T &val) { return {ResultCode::Success, val, ""}; }

    // Factory: Warning
    // Usage Result::Warn(value, "message")
    static Result<T> Warn(const T &val, const std::string &msg) { return {ResultCode::Warning, val, msg}; }

    // Factory: Error
    // Usage Result::Err("message")
    static Result<T> Err(const std::string &msg) { return {ResultCode::Error, T{}, msg}; }

    bool isOk() const { return status == ResultCode::Success; }
    bool isWarning() const { return status == ResultCode::Warning; }
    bool isError() const { return status == ResultCode::Error; }

    // Treat the result like a bool
    explicit operator bool() const
    {
        return status != ResultCode::Error;
    }
};

namespace WoodCodeUtils
{
    // Checks if a string only contains digits
    bool isNum(std::string str);

    // Converts a non-printable char into it's escaped version ('\n' -> "\\n")
    std::string escapeChar(char c);

    // Runs escapeChar() on each character
    std::string escapeString(const std::string &str);

    // Converts a number to the given base and returns result as an int
    int convertToBase(int num, int base);

    // Gets the current date in MMDDYY format
    std::string getDate();
}

class WoodCode
{
public:
    // Constructor
    WoodCode(std::string keys = "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.!? '\"/\\-_+:*@#$%&",
             std::string values = "000306121521243033010410131622253134020511142023263200010203040506070809101112131415161718");

    SimpleResult initialized;

    // Public encode function
    Result<std::string> encode(std::string input) const;

    // Public decode function
    Result<std::string> decode(std::string input) const;

    // Returns a string of valid characters (For the help menu)
    std::string getValidChars() const;

private:
    std::unordered_map<char, int> charMap;
    std::unordered_map<char, int> specialCharMap;

    std::unordered_map<int, char> reverseMap;
    std::unordered_map<int, char> specialReverseMap;

    // Validates input string and returns first invalid character if found
    Result<char> checkInput(const std::string &input) const;

    // Calculates date offset using base-7 conversion logic
    int getDateOffset(const std::string &date) const;

    // Encodes data with the given date offset
    Result<std::string> encodeData(const std::string &input, const int &dateOffset) const;

    // Decodes data with the given date offset
    Result<std::string> decodeData(const std::string &input, const int &dateOffset) const;

    // Initializes the character map from key and value strings
    SimpleResult initCharMap(const std::string &keys, const std::string &values);
};
