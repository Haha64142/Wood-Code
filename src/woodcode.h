#pragma once

#include <optional>
#include <unordered_map>
#include <string>

class WoodCode
{
public:
    // Constructor
    WoodCode(std::string keys, std::string values);

    bool initialized = false;

    // Public encode function
    std::string encode(std::string input);

    // Public decode function
    std::string decode(std::string input);

    // Returns a string of valid characters (For the help menu)
    std::string getValidChars();

private:
    std::unordered_map<char, int> charMap;
    std::unordered_map<char, int> specialCharMap;

    std::unordered_map<int, char> reverseMap;
    std::unordered_map<int, char> specialReverseMap;

    // Validates input string and returns first invalid character if found
    std::optional<char> checkInput(std::string &input);

    // Gets the current date in MMDDYY format
    std::string getDate();

    // Calculates date offset using base-7 conversion logic
    int getDateOffset(std::string &date);

    // Encodes data with the given date offset
    std::string encodeData(std::string &input, int &dateOffset);

    // Decodes data with the given date offset
    std::string decodeData(std::string &input, int &dateOffset);

    // Converts a number to the given base and returns result as an int
    int convertToBase(int num, int base);

    // Initializes the character map from key and value strings
    bool initCharMap(const std::string &keys, const std::string &values);
};
