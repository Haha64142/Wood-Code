#include <vector>
#include <ctime>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>

#include "woodcode.h"

// ================ WoodCodeUtils ================

bool WoodCodeUtils::isNum(std::string str)
{
    if (str.empty())
    {
        return false;
    }

    for (char &c : str)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }

    return true;
}

int WoodCodeUtils::convertToBase(int num, int base)
{
    std::string result = "";

    do
    {
        int remainder = num % base;
        result = std::to_string(remainder) + result;
        num /= base;
    } while (num > 0);
    return std::stoi(result);
}

std::string WoodCodeUtils::getDate()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);

    char dateStr[7];
    strftime(dateStr, 7, "%m%d%y", localTime);

    std::string outputDate = dateStr;
    return outputDate;
}

std::string WoodCodeUtils::escapeChar(char c)
{
    static const std::unordered_map<char, std::string> escapeTable = {
        {'\n', "\\n"},
        {'\t', "\\t"},
        {'\r', "\\r"},
        {'\v', "\\v"},
        {'\f', "\\f"},
        {'\a', "\\a"},
        {'\b', "\\b"},
        {'\0', "\\0"},
        {'\'', "\\'"},
        {'\"', "\\\""},
        {'\\', "\\\\"},
        {' ', "' ' (space)"}};

    // Check if it's in the escape table
    auto it = escapeTable.find(c);
    if (it != escapeTable.end())
    {
        return it->second;
    }

    // If it's a printable ASCII character, return it as a string
    if (std::isprint(static_cast<unsigned char>(c)))
    {
        return std::string(1, c);
    }

    // Fallback: print as hexadecimal (e.g., \x1b)
    std::ostringstream oss;
    oss << "\\x" << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(static_cast<unsigned char>(c));
    return oss.str();
}

std::string WoodCodeUtils::escapeString(const std::string &input)
{
    std::string result;
    result.reserve(input.size() * 2); // optimization: avoids reallocs

    for (const char &c : input)
    {
        result += escapeChar(c);
    }

    return result;
}

// ================ WoodCode ================

WoodCode::WoodCode(std::string keys, std::string values)
{
    // converts the 2 strings into a charMap
    initialized = initCharMap(keys, values);
}

Result<std::string> WoodCode::encode(std::string input) const
{
    // checkInput() -> checks if the input contains valid chars. Return true if a character is found
    Result<char> result = checkInput(input);
    if (result)
    {
        return Result<std::string>::Err("Invalid Input: " + WoodCodeUtils::escapeChar(result.value) + " is not a valid char");
    }
    std::string date = WoodCodeUtils::getDate();                     // getDate() -> returns current date in "MMDDYY" format
    int dateOffset = getDateOffset(date);                            // getDateOffset() -> returns a number based on the current date
    Result<std::string> encodedData = encodeData(input, dateOffset); // encodeData() -> return a string with the encoded data
    if (!encodedData)
    {
        return Result<std::string>::Err(encodedData.message);
    }

    return Result<std::string>::Ok("W10400" + encodedData.value + date);
}

Result<std::string> WoodCode::decode(std::string input) const
{
    if (input.length() < 12)
    {
        return Result<std::string>::Err("Invalid code: " + WoodCodeUtils::escapeString(input));
    }

    std::string header = input.substr(0, 6); // W10400
    if (header != "W10400")
    {
        if (header.front() != 'W' || !WoodCodeUtils::isNum(header.substr(1, 5)))
        {
            return Result<std::string>::Err("Invalid header: " + WoodCodeUtils::escapeString(header));
        }
        int version = std::stoi(header.substr(1, 5));
        std::string versionString = "v" + std::to_string(version / 10000) + "." +
                                    std::to_string((version / 100) % 100) + "." +
                                    std::to_string(version % 100);
        if (version < 10400)
        {
            return Result<std::string>::Err("Version: " + versionString + " is too old, please use WoodCode v1.4.0 or download an older decoder");
        }
        else if (version > 10400)
        {
            return Result<std::string>::Err("Version: " + versionString + " is too new, please use WoodCode v1.4.0 or download the latest decoder");
        }

        return Result<std::string>::Err("How the hell did you manage to get here.\nPlease download the latest version or submit a bug report at https://github.com/Haha64142/Wood-Code/issues");
    }

    std::string date = input.substr(input.length() - 6, 6); // Get the last 6 characters as date
    if (!WoodCodeUtils::isNum(date))
    {
        return Result<std::string>::Err("Invalid code: " + WoodCodeUtils::escapeString(input));
    }

    input = input.substr(6, input.length() - 12); // Remove the version and date from the input
    if (input.empty())
    {
        return Result<std::string>::Warn(input, "Warning: Decoded result is empty");
    }
    if (!WoodCodeUtils::isNum(input))
    {
        return Result<std::string>::Err("Invalid code: " + WoodCodeUtils::escapeString(input));
    }

    int dateOffset = getDateOffset(date);
    Result<std::string> decodedData = decodeData(input, dateOffset);

    if (decodedData.isOk() && decodedData.value.empty())
    {
        return Result<std::string>::Warn(decodedData.value, "Error: Input passed all checks but still returned an empty string.\nPlease download the latest version or submit a bug report at https://github.com/Haha64142/Wood-Code/issues");
    }

    return decodedData;
}

Result<char> WoodCode::checkInput(const std::string &input) const
{
    // Replace regex with a simple loop to check for valid characters, return true if an invalid char is found
    for (const char &c : input)
    {
        if (!std::isalnum(c) && specialCharMap.find(c) == specialCharMap.end())
        {
            return Result<char>::Ok(c); // Return the first invalid character, success = true
        }
    }
    return Result<char>::Err(""); // If valid, success = false
}

std::string WoodCode::getValidChars() const
{
    std::string validChars = "a-z, A-Z, 0-9, and special characters: '";
    for (const auto &pair : specialCharMap)
    {
        validChars += pair.first; // Add special chars
    }
    validChars += "'";
    return validChars;
}

int WoodCode::getDateOffset(const std::string &date) const
{
    int month = std::stoi(date.substr(0, 2));
    int day = std::stoi(date.substr(2, 2));
    int year = std::stoi(date.substr(4, 2));

    int sum = month + day + year;
    int offset = WoodCodeUtils::convertToBase(sum, 7);
    return offset;
}

Result<std::string> WoodCode::encodeData(const std::string &input, const int &dateOffset) const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    std::string encodedString = "";
    int salt = 0;

    // Loop through each character in the input string
    for (size_t i = 0, chunks = 0; i < input.length(); ++i, ++chunks)
    {
        if (chunks % 10 == 0)
        {
            salt = dist(gen);
            encodedString += std::to_string(500 + salt);
        }

        char currentChar = input.at(i);
        int value = 0;
        std::string valueString;

        // If it's a letter, get it's value based on the keyArray (e.g. A=keyArray[0], B=keyArray[1], etc.)
        if (std::isalpha(currentChar))
        {
            value = charMap.at(std::toupper(currentChar));

            if (std::islower(currentChar))
            {
                value += 100; // If it's lowercase, add 100
            }
        }
        else if (std::isdigit(currentChar))
        {
            // If it's a number, check the next character
            char next;
            if (i == input.length() - 1)
            {
                next = 'a'; // If it's the last char, the next is not a number
            }
            else
            {
                next = input.at(i + 1);
            }

            if (std::isdigit(next))
            {
                // If next = num, combine the current and next, and add 200, skip next char (++i)
                value = (currentChar - '0') * 10 + (next - '0'); // 1 * 10 + 2 = 12
                value += 200;
                ++i;
            }
            else
            {
                // If next = !num, add 300
                value = (currentChar - '0') + 300;
            }
        }
        else
        {
            // If it's a special character, get it's value based on the specialCharMap and add 400
            value = specialCharMap.at(currentChar) + 400;
        }

        // Add dateOffset
        value += dateOffset;
        value += salt;

        // Format to a 3 digit string
        valueString = std::to_string(value);
        if (valueString.length() > 3)
        {
            return Result<std::string>::Err("Error: Value exceeds 3 digits: " + valueString);
        }

        while (valueString.length() < 3)
        {
            valueString = "0" + valueString; // Pad with zeros to ensure 3 digits
        }

        // Add to the encodedString
        encodedString += valueString;
    }

    // Return the encodedString
    return Result<std::string>::Ok(encodedString);
}

Result<std::string> WoodCode::decodeData(const std::string &input, const int &dateOffset) const
{
    std::vector<int> inputChunks;

    // Split the input string into chunks of 3 characters
    for (size_t i = 0; i < input.length(); i += 3)
    {
        std::string chunk = input.substr(i, 3);
        int value = std::stoi(chunk);
        inputChunks.push_back(value);
    }

    // Subtract dateOffset from each chunk
    for (size_t i = 0; i < inputChunks.size(); ++i)
    {
        int &value = inputChunks.at(i);

        if (i % 11 == 0)
        {
            if (value < 500 || value > 599)
            {
                return Result<std::string>::Err("Invalid chunk:" + std::to_string(value));
            }

            continue;
        }

        value -= dateOffset;
        if (value < 0 || value > 599)
        {
            return Result<std::string>::Err("Invalid value after date offset: " + std::to_string(value));
        }
    }

    std::string decodedString = "";

    int salt = 0;

    // Loop through each value in inputChunks
    for (size_t i = 0; i < inputChunks.size(); ++i)
    {
        int &chunk = inputChunks.at(i);

        if (i % 11 == 0)
        {
            salt = chunk - 500;
            if (salt < 0 || salt > 99)
            {
                return Result<std::string>::Err("Invalid chunk:" + std::to_string(chunk));
            }
            continue;
        }
        else
        {
            chunk -= salt;
        }

        // Get first digit, then remove it (172 -> firstDigit = 1, value = 72)
        int firstDigit = chunk / 100;
        int value = chunk % 100; // 203 -> 3

        std::string decodedChunk;
        // Check firstDigit
        switch (firstDigit)
        {
        // If it's 0, get it's char based on the reverseMap and convert to upper
        case 0:
            decodedChunk = std::toupper(reverseMap.at(value));
            break;

        // If it's 1, get it's char based on the reverseMap and convert to lower
        case 1:
            decodedChunk = std::tolower(reverseMap.at(value));
            break;

        // If it's 2, it's a number
        case 2:
            // Convert value to a 2-digit string
            if (value < 10)
            {
                decodedChunk = "0" + std::to_string(value); // Pad with zero if single digit
            }
            else
            {
                decodedChunk = std::to_string(value);
            }
            break;

        // If it's 3, the second digit is a number
        case 3:
            decodedChunk = std::to_string(value % 10);
            break;

        // If it's 4, get it's char based on the specialReverseMap
        case 4:
            decodedChunk = specialReverseMap.at(value);
            break;
        }

        // Add to the decodedString
        decodedString += decodedChunk;
    }

    // Return the decodedString;
    return Result<std::string>::Ok(decodedString);
}

SimpleResult WoodCode::initCharMap(const std::string &keys, const std::string &values)
{
    if (keys.length() != 26 && keys.length() < 26)
    {
        return SimpleResult::Err("Key must be 26 characters or longer.\nError initializing character map.");
    }
    if (values.length() % 2 != 0)
    {
        return SimpleResult::Err("Values must be an even number of characters.\nError initializing character map.");
    }
    if (values.length() != keys.length() * 2)
    {
        return SimpleResult::Err("Values must be twice the length of keys.\nError initializing character map.");
    }

    charMap.clear();
    for (size_t i = 0; i < 26; ++i)
    {
        char keyChar = keys.at(i);
        int value = std::stoi(values.substr(i * 2, 2));
        charMap[keyChar] = value;
        reverseMap[value] = keyChar;
    }

    if (keys.length() == 26)
    {
        return SimpleResult::Ok();
    }

    for (size_t i = 26; i < keys.length(); ++i)
    {
        char keyChar = keys.at(i);
        int value = std::stoi(values.substr(i * 2, 2));
        specialCharMap[keyChar] = value;
        specialReverseMap[value] = keyChar;
    }
    return SimpleResult::Ok();
}
