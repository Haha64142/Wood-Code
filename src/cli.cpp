#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <filesystem>
#include <algorithm>
#include <vector>

#include "woodcode.h"

enum Command
{
    woodcode,
    App,
    Encode,
    Decode,
    Help,
};

size_t CommandCount = 5; // Number of items in enum Command

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
        std::cerr << "Failed to open file: " << filename << "\n";
        return "";
    }

    // Read entire file contents into a string
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
}

std::vector<std::string> splitLines(const std::string &content)
{
    std::vector<std::string> lines;
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line))
    {
        lines.push_back(line);
    }

    return lines;
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
        std::cerr << "Someone has goofed up and broken the printHelp() function.\n"
                  << "Please download the latest version,\n"
                  << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues\n";
        return;
    }

    std::filesystem::path helpPath = (exeDir.parent_path() / "help" / filename);
    std::string helpText = readFile(helpPath);
    if (helpText.empty())
    {
        std::cerr << "Error: Failed to print help text.\n"
                  << "Check that " << helpPath << " exists.\n";
        return;
    }

    std::cout << "\n"
              << helpText << "\n";
}

int main(int argc, char *argv[])
{
    std::filesystem::path exeDir = std::filesystem::absolute(argv[0]).parent_path(); // .../build/Release/bin/

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
                for (size_t i = 0; i < CommandCount; ++i)
                {
                    printHelp(exeDir, static_cast<Command>(i));
                    std::cout << "\n";
                }
                return 0;
            }
            else
            {
                std::cerr << "Error: '" << helpInput << "' is not a valid command.\n"
                          << "Use 'woodcode help' to see a list of available commands.\n";
                return 1;
            }
        }
        printHelp(exeDir, helpCommand);
        return 0;
    }
    else if (command == "-v" || command == "--version")
    {
        std::cout << "WoodCode CLI v1.4.0-dev\n"
                  << "https://github.com/Haha64142/Wood-Code\n";
        return 0;
    }
    else if (command == "app")
    {
        std::string appCommand = "\"" + (exeDir / "WoodCodeApp.exe").string() + "\"";
        int result = std::system(appCommand.c_str());
        if (result != 0)
        {
            std::cerr << "Error running WoodCodeApp.exe. Please ensure it exists next to woodcode.exe\n";
            return 1;
        }
        return 0;
    }
    else if (command == "encode")
    {
        std::string input;
        bool isFile = false;
        if (argc == 3)
        {
            input = argv[2];
            if (input == "--file" || input == "-f")
            {
                std::cout << "Warning: You entered 'woodcode encode " << input << "' without specifying a file name.\n"
                          << "If you intended to encode a file, use: woodcode encode " << input << " <file>\n\n"
                          << "Encoding '" << input << "' to WoodCode\n\n";
            }
        }
        else if (argc == 4 && (std::string(argv[2]) == "--file" || std::string(argv[2]) == "-f"))
        {
            input = readFile(argv[3]);
            if (input.empty())
            {
                std::cerr << "The file you've provided is empty. If you want to encode an empty string, use woodocode encode \"\"\n";
                return 1;
            }
            isFile = true;
        }
        else if (argc == 2)
        {
            printHelp(exeDir, Encode);
            return 1;
        }
        else
        {
            std::cerr << "Error: Invalid arguments for encoding.\n";
            return 1;
        }

        WoodCode wc; // Initialize WoodCode with default keys and values
        if (!wc.initialized)
        {
            std::cerr << wc.initialized.message
                      << "\n\nError initializing WoodCode. Please download the latest version\n"
                      << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues\n";
            return 1;
        }

        if (isFile)
        {
            std::vector<std::string> lines = splitLines(input);

            for (std::string line : lines)
            {
                if (line.empty())
                {
                    std::cout << "Empty line - skipping\n\n";
                    continue;
                }

                std::cout << "Input line: " << line << "\n";

                Result<std::string> result = wc.encode(line);

                if (result)
                {
                    if (result.isWarning())
                    {
                        std::cout << result.message << "\n\n";
                    }
                    std::cout << "Encoded text: " << result.value << "\n\n";
                }
                else
                {
                    std::cerr << "\n"
                              << result.message
                              << "\nEncoding failed\n";
                    return 1;
                }
            }
        }
        else
        {
            Result<std::string> result = wc.encode(input);

            if (result)
            {
                if (result.isWarning())
                {
                    std::cout << result.message << "\n\n";
                }
                std::cout << "Encoded text: " << result.value << "\n";
                return 0;
            }
            else
            {
                std::cerr << result.message
                          << "\nEncoding failed\n";
                return 1;
            }
        }
    }
    else if (command == "decode")
    {
        std::string input;
        bool isFile = false;
        if (argc == 3)
        {
            input = argv[2];
            if (input == "--file" || input == "-f")
            {
                std::cerr << "Error: You used option --file without specifying a file to use for decoding.\n";
                return 1;
            }
        }
        else if (argc == 4 && (std::string(argv[2]) == "--file" || std::string(argv[2]) == "-f"))
        {
            input = readFile(argv[3]);
            if (input.empty())
            {
                std::cerr << "The file you've provided is empty.\nYou must provide a file for decoding";
                return 1;
            }
            isFile = true;
        }
        else if (argc == 2)
        {
            printHelp(exeDir, Decode);
            return 1;
        }
        else
        {
            std::cerr << "Error: Invalid arguments for decoding.\n";
            return 1;
        }

        WoodCode wc; // Initialize WoodCode with default keys and values
        if (!wc.initialized)
        {
            std::cerr << wc.initialized.message
                      << "\n\nError initializing WoodCode. Please download the latest version\n"
                      << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues\n";
            return 1;
        }

        if (isFile)
        {
            std::vector<std::string> lines = splitLines(input);

            for (std::string line : lines)
            {
                if (line.empty())
                {
                    std::cout << "Empty line - skipping\n\n";
                    continue;
                }

                std::cout << "Input line: " << line << "\n";

                Result<std::string> result = wc.decode(line);

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
                        std::cout << result.message << "\n\n";
                    }
                    std::cout << "Decoded text: " << result.value << "\n\n";
                }
                else
                {
                    std::cerr << "\n"
                              << result.message
                              << "\nDecoding failed\n";
                }
            }
        }
        else
        {
            Result<std::string> result = wc.decode(input);

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
                    std::cout << result.message << "\n\n";
                }
                std::cout << "Decoded text: " << result.value << "\n";
                return 0;
            }
            else
            {
                std::cerr << result.message
                          << "\nDecoding failed\n";
                return 1;
            }
        }
    }
    else
    {
        std::cerr << "Error: Unknown command '" << command << "'.\n"
                  << "Use 'woodcode help' for usage information.\n";
    }

    return 0;
}
