#include <iostream>
#include <limits>
#include <string>
#include "woodcode.h"

int main()
{
    bool exit = false;

    WoodCode woodCode; // Initialize WoodCode with default keys and values
    if (!woodCode.initialized)
    {
        std::cerr << "Error initializing WoodCode. Please download the latest version" << std::endl;
        std::cerr << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;

        std::cout << std::endl
                  << "Press Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 1;
    }

    std::cout << "Welcome to WoodCode v1.3.5" << std::endl
              << "This is a simple encoding/decoding tool." << std::endl
              << "You can encode or decode text using a cipher created by Haha64142." << std::endl
              << "View the source code at https://github.com/Haha64142/Wood-Code" << std::endl
              << "Type 'h' for help, 'e' to encode, 'd' to decode, or 'q' to quit." << std::endl
              << std::endl
              << std::endl;

    while (!exit)
    {
        std::string result;
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
            result = woodCode.encode(input);

            if (result.empty())
            {
                std::cerr << "Encoding failed" << std::endl
                          << std::endl;
            }
            else
            {
                std::cout << std::endl
                          << "Encoded text: " << result << std::endl
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
                if (std::isspace(result.front()) || std::isspace(result.back()))
                {
                    result = "'" + result + "'";
                }
                std::cout << std::endl
                          << "Decoded text: " << result << std::endl
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
                      << "Valid characters for encoding are: " << woodCode.getValidChars() << std::endl
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
            continue;
        }
    }

    return 0;
}