#include <iostream>
#include <limits>
#include <string>

#include "woodcode.h"

int main()
{
    bool exit = false;

    WoodCode wc; // Initialize WoodCode with default keys and values
    if (!wc.initialized)
    {
        std::cerr << wc.initialized.message << std::endl
                  << std::endl
                  << "Error initializing WoodCode. Please download the latest version" << std::endl
                  << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;

        std::cout << std::endl
                  << "Press Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 1;
    }

    std::cout << "Welcome to WoodCode v1.4.0-dev" << std::endl
              << "This is a simple encoding/decoding tool created by Haha64142." << std::endl
              << "View the source code at https://github.com/Haha64142/Wood-Code" << std::endl
              << "Type 'h' for help, 'e' to encode, 'd' to decode, or 'q' to quit." << std::endl
              << std::endl
              << std::endl;

    while (!exit)
    {
        Result<std::string> result;
        std::string input;

        std::cout << "Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? ";
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
            result = wc.encode(input);

            if (result)
            {
                if (result.isWarning())
                {
                    std::cout << result.message << std::endl;
                }
                std::cout << std::endl
                          << "Encoded text: " << result.value << std::endl
                          << std::endl;
            }
            else
            {
                std::cerr << result.message << std::endl
                          << "Encoding failed" << std::endl
                          << std::endl;
            }
            break;

        case 'd':
        case 'D':
            // Decode
            std::cout << "Enter text to decode: ";
            std::getline(std::cin, input);
            result = wc.decode(input);

            if (result)
            {
                if (!result.value.empty())
                {
                    if (std::isspace(result.value.front()) || std::isspace(result.value.back()))
                    {
                        result.value = "'" + result.value + "'";
                    }
                }

                if (result.isWarning())
                {
                    std::cout << result.message << std::endl;
                }
                std::cout << std::endl
                          << "Decoded text: " << result.value << std::endl
                          << std::endl;
            }
            else
            {
                std::cerr << result.message << std::endl
                          << "Decoding failed" << std::endl
                          << std::endl;
            }
            break;

        case 'h':
        case 'H':
            std::cout << std::endl
                      << "Help Menu:" << std::endl
                      << "View the source code at https://github.com/Haha64142/Wood-Code" << std::endl
                      << std::endl
                      << "Type 'e' to encode text." << std::endl
                      << "Valid characters for encoding are: " << wc.getValidChars() << std::endl
                      << std::endl
                      << "Type 'd' to decode text." << std::endl
                      << "Type 'h' for this help menu." << std::endl
                      << "Type 'q' to quit the program." << std::endl
                      << std::endl;
            break;

        case 'q':
        case 'Q':
            exit = true;
            continue;

        default:
            std::cerr << std::endl
                      << "Invalid Option" << std::endl
                      << std::endl;
        }
    }

    return 0;
}