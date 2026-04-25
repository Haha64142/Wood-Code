#include <cctype>
#include <iostream>
#include <string>

#include "woodcode.h"

int main() {
    WoodCode wc; // Initialize WoodCode with default keys and values
    if (!wc.initialized) {
        std::cerr << wc.initialized.message << "\n\n"
                  << "Error initializing WoodCode. Please download the latest version\n"
                  << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues\n";

        std::cout << "\nPress Enter to exit...";
        std::string input;
        std::getline(std::cin, input);
        return 1;
    }

    std::cout << "Welcome to WoodCode v1.4.1\n"
              << "This is a simple encoding/decoding tool created by Haha64142.\n"
              << "View the source code at https://github.com/Haha64142/Wood-Code\n"
              << "Type 'h' for help, 'e' to encode, 'd' to decode, or 'q' to quit.\n\n\n";

    while (true) {
        Result<std::string> result;
        std::string input;

        std::cout << "Would you like to Encode [e], Decode [d], Help [h], or Quit [q]? ";
        std::getline(std::cin, input);

        if (input.empty())
            continue;
        char option = std::tolower(static_cast<unsigned char>(input[0]));

        switch (option) {
        case 'e':
            // Encode
            std::cout << "Enter text to encode: ";
            std::getline(std::cin, input);
            result = wc.encode(input);

            if (result) {
                if (result.isWarning()) {
                    std::cout << result.message << '\n';
                }
                std::cout << "\nEncoded text: " << result.value << "\n\n";
            } else {
                std::cerr << result.message << "Encoding failed\n\n";
            }
            break;

        case 'd':
            // Decode
            std::cout << "Enter text to decode: ";
            std::getline(std::cin, input);
            result = wc.decode(input);

            if (result) {
                if (result.value.empty() ||
                    std::isspace(static_cast<unsigned char>(result.value.front())) ||
                    std::isspace(static_cast<unsigned char>(result.value.back()))) {
                    result.value = "'" + result.value + "'";
                }

                if (result.isWarning()) {
                    std::cout << result.message << '\n';
                }
                std::cout << "\nDecoded text: " << result.value << "\n\n";
            } else {
                std::cerr << result.message << "\nDecoding failed\n\n";
            }
            break;

        case 'h':
            std::cout << '\n'
                      << "Help Menu:\n"
                      << "View the source code at https://github.com/Haha64142/Wood-Code\n\n"
                      << "Type 'e' to encode text.\n"
                      << "Valid characters for encoding are: " << wc.getValidChars() << "\n\n"
                      << "Type 'd' to decode text.\n"
                      << "Type 'h' for this help menu.\n"
                      << "Type 'q' to quit the program.\n\n";
            break;

        case 'q':
            return 0;

        default:
            std::cerr << "\nInvalid Option\n\n";
        }
    }

    return 0;
}
