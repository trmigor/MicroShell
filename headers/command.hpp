#ifndef COMMAND_CLASS
#define COMMAND_CLASS
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class Command
{
    private:
        std::string cmd;
        std::vector<std::string> arguments;
    public:

        // Constructors
        Command(void);
        Command(std::string cmd);
        Command(std::vector<std::string> arguments);

        // Getters
        // Returns the command line
        const std::string GetLine(void);
        // Returns the list of command arguments
        const std::vector<std::string>& GetArgs(void);
        // Returns the exit status
        int GetStatus(void);

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