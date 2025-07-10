#include <iostream>
#include <vector>
#include <ctime>
#include <optional>

class WoodCode
{
public:
    WoodCode(std::string key)
    {
        initKeyArrays(key); // converts the key string to a vector of integers
    }

    std::string encode(std::string input)
    {
        auto result = checkInput(input);                        // checkInput() -> checks if the input contains valid chars
        if (result)
        {
            std::cerr << "Invalid input: '" << *result << "' is not a valid character" << std::endl;
            return "";
        }
        std::string date = getDate();                            // getDate() -> returns current date in "MMDDYY" format
        int dateOffset = getDateOffset(date);                    // getDateOffset() -> returns a number based on the current date
        std::string encodedData = encodeData(input, dateOffset); // encodeData() -> return a string with the encoded data

        return "W10304" + encodedData + date;
    }

private:
    std::vector<int> keyArray;
    std::vector<int> specialKeyArray;

    std::optional<char> checkInput(std::string& input)
    {
        // Use regex to check if the input contains only valid characters
        // If valid, return std::nullopt
        // If not valid, return the invalid char
        return std::nullopt;
    }

    std::string getDate()
    {
        time_t now = time(0);
        tm *localTime = localtime(&now);

        char dateStr[7];
        strftime(dateStr, 7, "%m%d%y", localTime);

        std::string outputDate = dateStr;
        return outputDate;
    }

    int getDateOffset(std::string& date)
    {
        int month = std::stoi(date.substr(0, 2));
        int day = std::stoi(date.substr(2, 2));
        int year = std::stoi(date.substr(4, 2));

        int sum = month + day + year;
        int offset = convertToBase(sum, 7);
        return offset;
    }

    std::string encodeData(std::string& input, int& dateOffset)
    {
        std::string encodedString = "";
        // Loop through each character in the input string {

        // If it's a letter, get it's value based on the keyArray (i.e. A=keyArray[0], B=keyArray[1], etc.)
        // If it's lowercase, add 100

        // If it's a number, check the previous and next characters
        // If prev = letter and next = letter, add 200
        // If prev = letter and next = num, combine the current and next, and add 200, skip next char (i++)
        // If prev = num and next = num, combine the current and next, and add 300, skip next char (i++)
        // If prev = num and next = letter, add 400

        // If it's a special character, get it's value based on the specialKeyArray and add 500

        // Add dateOffset
        // Format to a 3 digit string

        // Add to the encodedString
        // } end loop

        // Return the encodedString
        
        return "123"; // Placeholder for encoded data based on input and dateOffset
    }

    int convertToBase(int num, int base)
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

    void initKeyArrays(const std::string& key)
    {
        if (key.length() != 52 && key.length() != 64)
        {
            std::cerr << "Key must be 52 or 64 characters long." << std::endl;
            return;
        }

        keyArray.clear();
        for (int i = 0; i < 52; i += 2)
        {
            int value = std::stoi(key.substr(i, 2));
            keyArray.push_back(value);
        }

        if (key.length() == 52) { return; }

        specialKeyArray.clear();
        for (int i = 52; i < key.length(); i += 2)
        {
            int value = std::stoi(key.substr(i, 2));
            specialKeyArray.push_back(value);
        }
    }
};

int main()
{
    WoodCode woodCode("0003061215212430330104101316222531340205111420232632000102030405");
    std::string encodedString = woodCode.encode("Test");
    std::cout << encodedString << std::endl;
    return 0;
}