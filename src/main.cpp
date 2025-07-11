#include <iostream>
#include <limits>
#include <string>
#include "woodcode.h"

int main()
{
    bool exit = false;

    WoodCode woodCode("ABCDEFGHIJKLMNOPQRSTUVWXYZ,.!?@ ", "0003061215212430330104101316222531340205111420232632000102030405");

    while (!exit)
    {
        std::string result;
        std::string input;

        std::cout << "Would you like to Encode [e], Decode [d], or Quit [q]? ";
        char option;
        option = std::cin.get();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        switch (option)
        {
        case 'e':
        case 'E':
            // Encode
            std::cout << "Enter text to encode: ";
            std::getline(std::cin, input);
            result = woodCode.encode(input);

            if (result.empty())
            {
                std::cerr << "Encoding failed" << std::endl
                          << std::endl;
            }
            else
            {
                std::cout << std::endl
                          << "Encoded text: '" << result << "'"
                          << std::endl
                          << std::endl;
            }
            break;

        case 'd':
        case 'D':
            // Decode
            std::cout << "Enter text to decode: ";
            std::getline(std::cin, input);
            result = woodCode.decode(input);

            if (result.empty())
            {
                std::cerr << "Decoding failed" << std::endl
                          << std::endl;
            }
            else
            {
                std::cout << std::endl
                          << "Decoded text: '" << result << "'"
                          << std::endl
                          << std::endl;
            }
            break;

        case 'q':
        case 'Q':
            exit = true;
            continue;

        default:
            std::cerr << std::endl
                      << "Invalid Option" << std::endl
                      << std::endl;
            continue;
        }
    }

    return 0;
}