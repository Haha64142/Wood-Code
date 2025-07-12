#include <iostream>
#include <fstream>
#include <string>
#include <limits>

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

void printHelp(Command command)
{
    switch (command)
    {
    case woodcode:
        std::cout << std::endl
                  << "Usage: woodcode [options] <command> [<args>]" << std::endl
                  << std::endl
                  << "If you prefer not to use the command line, you can run the WoodCodeApp executable named" << std::endl
                  << "WoodCodeApp.exe or run the command 'woodcode app'." << std::endl
                  << std::endl
                  << "Commands:" << std::endl
                  << "  app             Run the WoodCodeApp.exe if it exists" << std::endl
                  << "  encode          Encode a string or the contents of a file" << std::endl
                  << "  decode          Decode a string or the contents of a file" << std::endl
                  << "  help            Show this help message and exit" << std::endl
                  << std::endl
                  << "Options:" << std::endl
                  << "  -h, --help      Show this help message and exit" << std::endl
                  << "  -v, --version   Show version information" << std::endl
                  << std::endl
                  << "Run 'woodcode help <command>' for help with a specific command," << std::endl
                  << "or 'woodcode help all' to print all help messages." << std::endl
                  << std::endl
                  << "Github: https://github.com/Haha64142/Wood-Code" << std::endl;
        break;

    case App:
        std::cout << std::endl
                  << "Usage: woodcode app" << std::endl
                  << std::endl
                  << "Run WoodCodeApp.exe for users who prefer not to use the command line." << std::endl;
        break;

    case Encode:
        std::cout << std::endl
                  << "Usage: woodcode encode <text>" << std::endl
                  << "       woodcode encode [-f | --file] <file>" << std::endl
                  << std::endl
                  << "Encode a string into WoodCode v1.3.5." << std::endl
                  << std::endl
                  << "Arguments:" << std::endl
                  << "  <text>               The text you want to encode" << std::endl
                  << "  -f, --file <file>    Path to a file containing the text to encode" << std::endl
                  << "                       The file must contain only one line of text" << std::endl
                  << std::endl
                  << "Use 'woodcode decode' to decode a WoodCode string." << std::endl;
        break;

    case Decode:
        std::cout << std::endl
                  << "Usage: woodcode decode <text>" << std::endl
                  << "       woodcode decode [-f | --file] <file>" << std::endl
                  << std::endl
                  << "Decode a string using WoodCode v1.3.5." << std::endl
                  << std::endl
                  << "Arguments:" << std::endl
                  << "  <text>               The WoodCode string you want to decode" << std::endl
                  << "  -f, --file <file>    Path to a file containing the code to decode" << std::endl
                  << "                       The file must contain only one line of text" << std::endl
                  << std::endl
                  << "Use 'woodcode encode' to encode text." << std::endl;
        break;

    case Help:
        std::cout << std::endl
                  << "Usage: woodcode help" << std::endl
                  << "       woodcode help <command>" << std::endl
                  << "       woodcode all" << std::endl
                  << std::endl
                  << "Display general information about the woodcode CLI, or detailed help for a specific command." << std::endl
                  << std::endl
                  << "Arguments:" << std::endl
                  << "  (none)          Show general help and available commands" << std::endl
                  << "  all             Print all help messages" << std::endl
                  << "  <command>       Show help for the specified command" << std::endl;
        break;

    default: // It's not supposed to get here
        std::cerr << "Someone has goofed up and broken the printHelp() function." << std::endl
                  << "Please download the latest version," << std::endl
                  << "or submit a bug report at https://github.com/Haha64142/Wood-Code/issues" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printHelp(woodcode);
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
                    printHelp(static_cast<Command>(i));
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
        printHelp(helpCommand);
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
            if (input == "--file" || input == "-f")
            {
                std::cout << "Warning: You entered 'woodcode encode --file' without specifying a file name." << std::endl
                          << "If you intended to encode a file, use: woodcode encode --file <file>" << std::endl
                          << "Encoding '--file' to WoodCode" << std::endl
                          << std::endl;
            }
        }
        else if (argc == 4 && (std::string(argv[2]) == "--file" || std::string(argv[2]) == "-f"))
        {
            input = readFile(argv[3]);
        }
        else if (argc == 2)
        {
            printHelp(Encode);
            return 1;
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
        std::string input;
        if (argc == 3)
        {
            input = argv[2];
            if (input == "--file")
            {
                std::cerr << "Error: You used option --file without specifying a file to use for decoding." << std::endl;
                return 1;
            }
        }
        else if (argc == 4 && std::string(argv[2]) == "--file")
        {
            input = readFile(argv[3]);
        }
        else if (argc == 2)
        {
            printHelp(Decode);
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
        std::cerr << "Error: Unknown command '" << command << "'." << std::endl
                  << "Use 'woodcode --help' for usage information." << std::endl;
    }

    return 0;
}
