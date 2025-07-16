#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <filesystem>

#include "woodcode.h"

enum Command
{
    woodcode,
    App,
    Encode,
    Decode,
    Help,
};

int CommandCount = 5; // Number of items in enum Command

std::string strToLower(std::string str)
{
    for (char &c : str)
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return str;
}

std::string readFile(const std::filesystem::path &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    // Read entire file contents into a string
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

void printHelp(const std::filesystem::path &exeDir, Command command)
{
    std::string filename = "";
    switch (command)
    {
    case woodcode:
        filename = "woodcode.help";
        break;

    case App:
        filename = "app.help";
        break;

    case Encode:
        filename = "encode.help";
        break;

    case Decode:
        filename = "decode.help";
        break;

    case Help:
        filename = "help.help";
        break;

    default:
        // It's not supposed to get here
        std::cerr << "Someone has goofed up and broken the printHelp() function." << std::endl
                  << "Please download the latest version," << std::endl
                  << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;
        return;
    }

    std::filesystem::path helpPath = (exeDir.parent_path() / "help" / filename);
    std::string helpText = readFile(helpPath);
    if (helpText.empty())
    {
        std::cerr << "Error: Failed to print help text." << std::endl
                  << "Check that " << helpPath << " exists." << std::endl;
        return;
    }

    std::cout << std::endl
              << helpText << std::endl;
}

int main(int argc, char *argv[])
{
    std::filesystem::path exeDir = std::filesystem::canonical(argv[0]).parent_path(); // .../build/Release/bin/

    if (argc < 2)
    {
        printHelp(exeDir, woodcode);
        return 1;
    }

    std::string command = argv[1];

    if (command == "-h" || command == "--help" || command == "help")
    {
        Command helpCommand = woodcode;
        std::string helpInput;
        if (argc == 3)
        {
            helpInput = strToLower(argv[2]);
            if (helpInput == "woodcode")
            {
                helpCommand = woodcode;
            }
            else if (helpInput == "app")
            {
                helpCommand = App;
            }
            else if (helpInput == "encode")
            {
                helpCommand = Encode;
            }
            else if (helpInput == "decode")
            {
                helpCommand = Decode;
            }
            else if (helpInput == "help")
            {
                helpCommand = Help;
            }
            else if (helpInput == "all")
            {
                for (int i = 0; i < CommandCount; i++)
                {
                    printHelp(exeDir, static_cast<Command>(i));
                    std::cout << std::endl;
                }
                return 0;
            }
            else
            {
                std::cerr << "Error: '" << helpInput << "' is not a valid command." << std::endl
                          << "Use 'woodcode help' to see a list of available commands." << std::endl;
                return 1;
            }
        }
        printHelp(exeDir, helpCommand);
        return 0;
    }
    else if (command == "-v" || command == "--version")
    {
        std::cout << "WoodCode CLI v1.3.5" << std::endl
                  << "https://github.com/Haha64142/Wood-Code" << std::endl;
        return 0;
    }
    else if (command == "app")
    {
        std::string appCommand = "\"" + (exeDir / "WoodCodeApp.exe").string() + "\"";
        int result = std::system(appCommand.c_str());
        if (result != 0)
        {
            std::cerr << "Error running WoodCodeApp.exe. Please ensure it exists next to woodcode.exe" << std::endl;
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
            if (input == "--file" || input == "-f")
            {
                std::cout << "Warning: You entered 'woodcode encode " << input << "' without specifying a file name." << std::endl
                          << "If you intended to encode a file, use: woodcode encode " << input << " <file>" << std::endl
                          << std::endl
                          << "Encoding '" << input << "' to WoodCode" << std::endl
                          << std::endl;
            }
        }
        else if (argc == 4 && (std::string(argv[2]) == "--file" || std::string(argv[2]) == "-f"))
        {
            input = readFile(argv[3]);
        }
        else if (argc == 2)
        {
            printHelp(exeDir, Encode);
            return 1;
        }
        else
        {
            std::cerr << "Error: Invalid arguments for encoding." << std::endl;
            return 1;
        }

        WoodCode wc; // Initialize WoodCode with default keys and values
        if (!wc.initialized)
        {
            std::cerr << wc.initialized.message << std::endl
                      << std::endl
                      << "Error initializing WoodCode. Please download the latest version" << std::endl
                      << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;
            return 1;
        }

        Result<std::string> result = wc.encode(input);

        if (result)
        {
            if (result.isWarning())
            {
                std::cout << result.message << std::endl
                          << std::endl;
            }
            std::cout << "Encoded text: " << result.value << std::endl;
            return 0;
        }
        else
        {
            std::cerr << result.message << std::endl
                      << "Encoding failed" << std::endl;
            return 1;
        }
    }
    else if (command == "decode")
    {
        std::string input;
        if (argc == 3)
        {
            input = argv[2];
            if (input == "--file" || input == "-f")
            {
                std::cerr << "Error: You used option --file without specifying a file to use for decoding." << std::endl;
                return 1;
            }
        }
        else if (argc == 4 && (std::string(argv[2]) == "--file" || std::string(argv[2]) == "-f"))
        {
            input = readFile(argv[3]);
        }
        else if (argc == 2)
        {
            printHelp(exeDir, Decode);
            return 1;
        }
        else
        {
            std::cerr << "Error: Invalid arguments for decoding." << std::endl;
            return 1;
        }

        WoodCode woodCode; // Initialize WoodCode with default keys and values
        if (!woodCode.initialized)
        {
            std::cerr << "Error initializing WoodCode. Please download the latest version" << std::endl;
            std::cerr << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;
            return 1;
        }

        Result<std::string> result = woodCode.decode(input);

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
                std::cout << result.message << std::endl
                          << std::endl;
            }
            std::cout << "Decoded text: " << result.value << std::endl;
            return 0;
        }
        else
        {
            std::cerr << result.message << std::endl
                      << "Decoding failed" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Error: Unknown command '" << command << "'." << std::endl
                  << "Use 'woodcode help' for usage information." << std::endl;
    }

    return 0;
}
