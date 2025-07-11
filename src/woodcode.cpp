#include "woodcode.h"

#include <iostream>
#include <vector>
#include <ctime>
#include <optional>
#include <regex>
#include <unordered_map>
#include <string>

WoodCode::WoodCode(std::string keys, std::string values)
{
    initCharMap(keys, values); // converts the 2 strings into a charMap
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

    return "W10304" + encodedData + date;
}

std::string WoodCode::decode(std::string input)
{
    std::string version = input.substr(0, 6); // W10304
    if (version != "W10304")
    {
        std::cerr << "Invalid version: " << version << std::endl;
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
    // Use regex to check if the input contains only valid characters
    // If valid, return std::nullopt
    // If not valid, return the invalid char
    std::regex invalidChars("[^a-zA-Z0-9,.!?@ ]");

    std::smatch match;
    if (std::regex_search(input, match, invalidChars))
    {
        return match.str()[0]; // Return the first invalid character found
    }
    return std::nullopt;
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
            // If it's a number, check the previous and next characters
            char prev;
            char next;
            if (i == 0)
            {
                prev = 'a'; // If it's the first char, the prev was not a number
            }
            else
            {
                prev = input[i - 1];
            }

            if (i == input.length() - 1)
            {
                next = 'a'; // If it's the last char, the next is not a number
            }
            else
            {
                next = input[i + 1];
            }

            if (!std::isdigit(prev))
            {
                if (!std::isdigit(next))
                {
                    value = (currentChar - '0') + 200; // If prev = !num and next = !num, add 200
                }
                else
                {
                    // If prev = !num and next = num, combine the current and next, and add 200, skip next char (i++)
                    value = (currentChar - '0') * 10 + (next - '0'); // 1 * 10 + 2 = 12
                    value += 200;
                    i++;
                }
            }
            else
            {
                if (std::isdigit(next))
                {
                    // If prev = num and next = num, combine the current and next, and add 300, skip next char (i++)
                    value = (currentChar - '0') * 10 + (next - '0'); // 1 * 10 + 2 = 12
                    value += 300;
                    i++;
                }
                else
                {
                    // If prev = num and next = !num, add 400
                    value = (currentChar - '0') + 400;
                }
            }
        }
        else
        {
            // If it's a special character, get it's value based on the specialCharMap and add 500
            value = specialCharMap[currentChar] + 500;
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
    }

    std::string decodedString = "";

    // Loop through each value in inputChunks
    for (int &chunk : inputChunks)
    {
        // Get first digit, then remove it (172 -> firstDigit = 1, value = "72")
        int firstDigit = chunk / 100;
        int value = chunk % 100;                                      // 203 -> 3
        std::string valueString = std::to_string(chunk).substr(1, 2); // 203 -> "03"

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
            decodedChunk = valueString;
            break;

        // If it's 3, it's a number
        case 3:
            decodedChunk = valueString;
            break;

        // If it's 4, the second digit is a number
        case 4:
            decodedChunk = std::to_string(value % 10);
            break;

        // If it's 5, get it's char based on the specialReverseMap
        case 5:
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

void WoodCode::initCharMap(const std::string &keys, const std::string &values)
{
    if (keys.length() != 26 && keys.length() != 32)
    {
        std::cerr << "Key must be 26 or 32 characters long." << std::endl;
        return;
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
        return;
    }

    for (int i = 26; i < keys.length(); i++)
    {
        char keyChar = keys[i];
        int value = std::stoi(values.substr(i * 2, 2));
        specialCharMap[keyChar] = value;
        specialReverseMap[value] = keyChar;
    }
}
