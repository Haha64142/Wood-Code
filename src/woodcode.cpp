#include "woodcode.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <optional>
#include <unordered_map>
#include <string>

WoodCode::WoodCode(std::string keys, std::string values)
{
    // converts the 2 strings into a charMap
    initialized = initCharMap(keys, values); // initCharMap() -> returns true if successful
}

std::string WoodCode::encode(std::string input)
{
    auto result = checkInput(input); // checkInput() -> checks if the input contains valid chars
    if (result)
    {
        std::cerr << std::endl
                  << "Invalid input: '" << *result << "' is not a valid character" << std::endl;
        return "";
    }
    std::string date = getDate();                            // getDate() -> returns current date in "MMDDYY" format
    int dateOffset = getDateOffset(date);                    // getDateOffset() -> returns a number based on the current date
    std::string encodedData = encodeData(input, dateOffset); // encodeData() -> return a string with the encoded data

    return "W10305" + encodedData + date;
}

std::string WoodCode::decode(std::string input)
{
    std::string header = input.substr(0, 6); // W10305
    if (header != "W10305")
    {
        if (!(std::isalpha(header.front()) && std::isdigit(header.at(1)) && std::isdigit(header.at(2)) && std::isdigit(header.at(3)) && std::isdigit(header.at(4)) && std::isdigit(header.at(5))))
        {
            std::cerr << std::endl
                      << "Invalid header: " << header << std::endl;
            return "";
        }
        int version = std::stoi(header.substr(1, 5));
        if (version < 10305)
        {
            std::cerr << std::endl
                      << "Version: v" << header.substr(1, 2) << "." << header.substr(3, 2) << "." << header.substr(5, 2)
                      << " is too old, please use WoodCode v1.3.5 or download an older decoder" << std::endl;
            return "";
        }
        if (version > 10305)
        {
            std::cerr << std::endl
                      << "Version: v" << header.substr(1, 2) << "." << header.substr(3, 2) << "." << header.substr(5, 2)
                      << " is not supported, please use WoodCode v1.3.5 or download the appropriate decoder" << std::endl;
        }

        std::cerr << std::endl
                  << "Invalid header: " << header << std::endl;
        return "";
    }

    std::string date = input.substr(input.length() - 6, 6); // Get the last 6 characters as date

    input = input.substr(6, input.length() - 12); // Remove the version and date from the input

    int dateOffset = getDateOffset(date);
    std::string decodedData = decodeData(input, dateOffset);

    return decodedData;
}

std::optional<char> WoodCode::checkInput(std::string &input)
{
    // Replace regex with a simple loop to check for valid characters
    for (char &c : input)
    {
        if (!std::isalnum(c) && specialCharMap.find(c) == specialCharMap.end())
        {
            return c; // Return the first invalid character
        }
    }
    return std::nullopt; // If valid, return std::nullopt
}

std::string WoodCode::getValidChars()
{
    std::string validChars = "a-z, A-Z, 0-9, and special characters: '";
    for (const auto &pair : specialCharMap)
    {
        validChars += pair.first; // Add special chars
    }
    validChars += "'";
    return validChars;
}

std::string WoodCode::getDate()
{
    time_t now = time(0);
    tm *localTime = localtime(&now);

    char dateStr[7];
    strftime(dateStr, 7, "%m%d%y", localTime);

    std::string outputDate = dateStr;
    return outputDate;
}

int WoodCode::getDateOffset(std::string &date)
{
    int month = std::stoi(date.substr(0, 2));
    int day = std::stoi(date.substr(2, 2));
    int year = std::stoi(date.substr(4, 2));

    int sum = month + day + year;
    int offset = convertToBase(sum, 7);
    return offset;
}

std::string WoodCode::encodeData(std::string &input, int &dateOffset)
{
    std::string encodedString = "";

    // Loop through each character in the input string
    for (int i = 0; i < input.length(); i++)
    {
        char currentChar = input[i];
        int value = 0;
        std::string valueString;

        // If it's a letter, get it's value based on the keyArray (i.e. A=keyArray[0], B=keyArray[1], etc.)
        if (std::isalpha(currentChar))
        {
            value = charMap[std::toupper(currentChar)];

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
                next = input[i + 1];
            }

            if (std::isdigit(next))
            {
                // If next = num, combine the current and next, and add 200, skip next char (i++)
                value = (currentChar - '0') * 10 + (next - '0'); // 1 * 10 + 2 = 12
                value += 200;
                i++;
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
            value = specialCharMap[currentChar] + 400;
        }

        // Add dateOffset
        value += dateOffset;

        // Format to a 3 digit string
        valueString = std::to_string(value);
        if (valueString.length() > 3)
        {
            std::cerr << "Value exceeds 3 digits: " << valueString << std::endl;
            return "";
        }

        while (valueString.length() < 3)
        {
            valueString = "0" + valueString; // Pad with zeros to ensure 3 digits
        }

        // Add to the encodedString
        encodedString += valueString;
    }

    // Return the encodedString
    return encodedString;
}

std::string WoodCode::decodeData(std::string &input, int &dateOffset)
{
    std::vector<int> inputChunks;

    // Split the input string into chunks of 3 characters
    for (int i = 0; i < input.length(); i += 3)
    {
        std::string chunk = input.substr(i, 3);
        int value = std::stoi(chunk);
        inputChunks.push_back(value);
    }

    // Subtract dateOffset from each chunk
    for (int &value : inputChunks)
    {
        value -= dateOffset;
        if (value < 0 || value > 499)
        {
            std::cerr << std::endl
                      << "Invalid value after date offset: " << value << std::endl;
            return "";
        }
    }

    std::string decodedString = "";

    // Loop through each value in inputChunks
    for (int &chunk : inputChunks)
    {
        // Get first digit, then remove it (172 -> firstDigit = 1, value = 72)
        int firstDigit = chunk / 100;
        int value = chunk % 100; // 203 -> 3

        std::string decodedChunk;

        // Check firstDigit
        switch (firstDigit)
        {
        // If it's 0, get it's char based on the reverseMap and convert to upper
        case 0:
            decodedChunk = std::toupper(reverseMap[value]);
            break;

        // If it's 1, get it's char based on the reverseMap and convert to lower
        case 1:
            decodedChunk = std::tolower(reverseMap[value]);
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
            decodedChunk = specialReverseMap[value];
            break;
        }

        // Add to the decodedString
        decodedString += decodedChunk;
    }

    // Return the decodedString;
    return decodedString;
}

int WoodCode::convertToBase(int num, int base)
{
    std::string result = "";

    while (num > 0)
    {
        int remainder = num % base;
        result = std::to_string(remainder) + result;
        num /= base;
    }
    return std::stoi(result);
}

bool WoodCode::initCharMap(const std::string &keys, const std::string &values)
{
    if (keys.length() != 26 && keys.length() < 26)
    {
        std::cerr << "Key must be 26 characters or longer." << std::endl
                  << "Error initializing character map." << std::endl
                  << std::endl;
        return false;
    }
    if (values.length() % 2 != 0)
    {
        std::cerr << "Values must be an even number of characters." << std::endl
                  << "Error initializing character map." << std::endl
                  << std::endl;
        return false;
    }
    if (values.length() != keys.length() * 2)
    {
        std::cerr << "Values must be twice the length of keys." << std::endl
                  << "Error initializing character map." << std::endl
                  << std::endl;
        return false;
    }

    charMap.clear();
    for (int i = 0; i < 26; i++)
    {
        char keyChar = keys[i];
        int value = std::stoi(values.substr(i * 2, 2));
        charMap[keyChar] = value;
        reverseMap[value] = keyChar;
    }

    if (keys.length() == 26)
    {
        return true;
    }

    for (int i = 26; i < keys.length(); i++)
    {
        char keyChar = keys[i];
        int value = std::stoi(values.substr(i * 2, 2));
        specialCharMap[keyChar] = value;
        specialReverseMap[value] = keyChar;
    }
    return true;
}
