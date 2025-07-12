#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#include "woodcode.h"

void printHelp()
{
    std::cout << "Usage: woodcode [options] <command> <args>" << std::endl
              << std::endl
              << "If you prefer not to use the command line, you can run the WoodCodeApp executable named" << std::endl
              << "WoodCodeApp.exe or run the command `woodcode app`" << std::endl
              << std::endl
              << "Commands:" << std::endl
              << "  app             Run the WoodCodeApp.exe if it exists" << std::endl
              << "  encode <text>   Encode the given text" << std::endl
              << "  decode <text>   Decode the given text" << std::endl
              << "Options:" << std::endl
              << "  -h, --help      Show this help message" << std::endl
              << "  -v, --version   Show version information" << std::endl;
}

std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << "\n";
        return "";
    }

    // Read entire file contents into a string
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printHelp();
        return 1;
    }

    std::string command = argv[1];

    if (command == "-h" || command == "--help" || command == "help")
    {
        printHelp();
        return 0;
    }
    else if (command == "-v" || command == "--version")
    {
        std::cout << "WoodCode CLI v1.3.5" << std::endl;
        return 0;
    }
    else if (command == "app")
    {
        int result = std::system("WoodCodeApp.exe");
        if (result != 0)
        {
            std::cerr << "Error running WoodCodeApp.exe. Please ensure it exists in the current directory." << std::endl;
            return 1;
        }
        return 0;
    }
    else if (command == "encode")
    {
        std::string input;
        if (argc == 3)
        {
            input = argv[2];
        }
        else if (argc == 4 && std::string(argv[2]) == "--file")
        {
            input = readFile(argv[3]);
        }
        else
        {
            std::cerr << "Error: Invalid arguments for encoding." << std::endl;
            return 1;
        }

        WoodCode woodCode; // Initialize WoodCode with default keys and values
        if (!woodCode.initialized)
        {
            std::cerr << "Error initializing WoodCode. Please download the latest version" << std::endl;
            std::cerr << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;
            return 1;
        }

        std::string result = woodCode.encode(input);

        if (result.empty())
        {
            std::cerr << "Encoding failed" << std::endl;
            return 1;
        }
        else
        {
            std::cout << "Encoded text: " << result << std::endl;
            return 0;
        }
    }
    else if (command == "decode")
    {
        if (argc < 3)
        {
            std::cerr << "Error: Invalid number of arguments for encoding." << std::endl;
            return 1;
        }

        std::string input = argv[2];
        WoodCode woodCode; // Initialize WoodCode with default keys and values
        if (!woodCode.initialized)
        {
            std::cerr << "Error initializing WoodCode. Please download the latest version" << std::endl;
            std::cerr << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;
            return 1;
        }

        std::string result = woodCode.decode(input);

        if (result.empty())
        {
            std::cerr << "Decoding failed" << std::endl;
            return 1;
        }
        else
        {
            if (std::isspace(result.front()) || std::isspace(result.back()))
            {
                result = "'" + result + "'";
            }
            std::cout << "Decoded text: " << result << std::endl;
            return 0;
        }
    }
    else
    {
        std::cerr << "Error: Unknown command '" << command << "'. Use 'woodcode --help' for usage information." << std::endl;
    }

    return 0;
}
