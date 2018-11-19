#ifndef COMMAND_CLASS
#define COMMAND_CLASS
#include <vector>
#include <string>

class Command
{
    private:
        std::string cmd;
        std::vector<std::string> arguments;
        int in;
        int out;
    public:

        // Constructors
        Command(void);
        Command(std::string cmd);
        Command(std::vector<std::string> arguments);
        Command(int in, int out, std::string cmd);
        Command(int in, int out, std::vector<std::string> arguments);

        // Getters
        // Returns the command line
        const std::string GetLine(void);
        // Returns the list of command arguments
        const std::vector<std::string>& GetArgs(void);

        // Reads the command line and saves in "cmd".
        void ReadLine(void);

        // Splits the line into "arguments" variable
        int SplitLine(void);

        // Executes the command
        int Execute(void);

        // Destructors
        ~Command();
};

#endif