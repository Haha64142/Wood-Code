#pragma once

#include <optional>
#include <unordered_map>
#include <string>

namespace WoodCodeUtils
{
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

    bool initialized = false;

    // Public encode function
    std::string encode(std::string input) const;

    // Public decode function
    std::string decode(std::string input) const;

    // Returns a string of valid characters (For the help menu)
    std::string getValidChars() const;

private:
    std::unordered_map<char, int> charMap;
    std::unordered_map<char, int> specialCharMap;

    std::unordered_map<int, char> reverseMap;
    std::unordered_map<int, char> specialReverseMap;

    // Validates input string and returns first invalid character if found
    std::optional<char> checkInput(std::string &input) const;

    // Calculates date offset using base-7 conversion logic
    int getDateOffset(std::string &date) const;

    // Encodes data with the given date offset
    std::string encodeData(std::string &input, int &dateOffset) const;

    // Decodes data with the given date offset
    std::string decodeData(std::string &input, int &dateOffset) const;

    // Initializes the character map from key and value strings
    bool initCharMap(const std::string &keys, const std::string &values);
};
