#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../headers/command.hpp"
#include "../headers/built-in.hpp"
#include "../headers/central.hpp"

// Constructors
Command::Command(void)
{

}

Command::Command(std::string cmd)
{
    this -> cmd = cmd;
}

Command::Command(std::vector<std::string> arguments)
{
    this -> arguments = arguments;
}

// Getters
// Returns the command line
const std::string Command::GetLine(void)
{
    return cmd;
}
// Returns the list of command arguments
const std::vector<std::string>& Command::GetArgs(void)
{
    return arguments;
}

// Reads the command line and saves in "cmd".
void Command::ReadLine(void)
{
    std::getline(std::cin, cmd);
}

// Splits the line into "arguments"
void Command::SplitLine(void)
{
    std::string element;
    std::stringstream s(cmd);
    while (s >> element)
    {
        arguments.push_back(element);
    }
}

// Executes the command
int Command::Execute(void)
{
    if (arguments.empty() || arguments[0].empty())
    {
        return 1;
    }

    for (int i = 0; i < myshell_num_builtin(); i++)
    {
        if (arguments[0] == builtin_str[i])
        {
            return (*builtin_func[i])(arguments);
        }
    }

    return myshell_launch(arguments);
}

// Destructors
Command::~Command()
{

}