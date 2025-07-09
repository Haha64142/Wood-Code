#include <iostream>

class WoodCode
{
public:
    WoodCode(std::string key)
    {
        masterKey = key;
    }

    std::string encode(std::string input)
    {
        std::string date = getDate();                            // getDate() -> returns current date in "MMDDYY" format
        int dateOffset = getDateOffset(date);                    // getDateOffset() -> returns a 2 digit number based on the current date
        std::string encodedData = encodeData(input, dateOffset); // encodeData() -> return a string with the encoded data

        return "W10304" + encodedData + date;
    }

private:
    std::string masterKey;

    std::string getDate()
    {
        return "000000"; // Placeholder for current date in "MMDDYY" format
    }

    int getDateOffset(std::string date)
    {
        return 00; // Placeholder for a 2 digit number based on the current date
    }

    std::string encodeData(std::string input, int dateOffset)
    {
        return "130"; // Placeholder for encoded data based on input and dateOffset
    }
};

int main()
{
    std::cout << "Test";
    return 0;
}