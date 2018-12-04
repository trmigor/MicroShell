#include <fcntl.h>
#include <glob.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "../headers/command.hpp"
#include "../headers/built-in.hpp"
#include "../headers/central.hpp"

// Constructors
Command::Command(void)
{
    conv = false;
    in = 0;
    out = 1;
}

Command::Command(bool conv)
{
    this -> conv = conv;
    in = 0;
    out = 1;
}

Command::Command(std::string cmd)
{
    conv = false;
    in = 0;
    out = 1;
    this -> cmd = cmd;
}

Command::Command(bool conv, std::string cmd)
{
    this -> conv = conv;
    in = 0;
    out = 1;
    this -> cmd = cmd;
}

Command::Command(std::vector<std::string> arguments)
{
    conv = false;
    in = 0;
    out = 1;
    this -> arguments = arguments;
}

Command::Command(bool conv, std::vector<std::string> arguments)
{
    this -> conv = conv;
    in = 0;
    out = 1;
    this -> arguments = arguments;
}

Command::Command(int in, int out, std::string cmd)
{
    conv = false;
    this -> cmd = cmd;
    this -> in = in;
    this -> out = out;
}

Command::Command(int in, int out, bool conv, std::string cmd)
{
    this -> conv = conv;
    this -> cmd = cmd;
    this -> in = in;
    this -> out = out;
}

Command::Command(int in, int out, std::vector<std::string> arguments)
{
    conv = false;
    this -> arguments = arguments;
    this -> in = in;
    this -> out = out;
}

Command::Command(int in, int out, bool conv, std::vector<std::string> arguments)
{
    this -> conv = conv;
    this -> arguments = arguments;
    this -> in = in;
    this -> out = out;
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
    try
    {
        std::getline(std::cin, cmd);
    }
    catch(std::ios_base::failure e)
    {
        std::cerr << "Can't get the line";
    }
}

// Splits the line into "arguments" variable
int Command::SplitLine(void)
{
    std::string element;
    std::stringstream s(cmd);
    while (s >> element)
    {
        arguments.push_back(element);
    }

    // IO redirection
    for (int i = 0; i < arguments.size(); i++)
    {
        if(arguments[i] == "<")
        {
            in = open(arguments[i + 1].c_str(), O_RDONLY, 0777);
            if (in < 0)
            {
                perror("input redirect");
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
            i--;
        }
        if(arguments[i] == ">")
        {
            out = open(arguments[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (out < 0)
            {
                perror("output redirect");
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
            i--;
        }
    }

    // Regular expressions
    for (int i = 0; i < arguments.size(); i++)
    {
        unsigned long star = std::string::npos, quest = std::string::npos;
        if (arguments[i].find_first_of('*') != std::string::npos ||
                arguments[i].find_first_of('?') != std::string::npos ||
                arguments[i].find_first_of('~') != std::string::npos)
        {
            std::vector<std::string> glob = Glob(arguments[i]);
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 1);
            arguments.insert(arguments.begin() + i, glob.begin(), glob.end());
        }
    }
    return 0;
}

// Regular expressions parse
std::vector<std::string> Command::Glob(const std::string& pattern)
{
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    int return_value = glob(pattern.c_str(), GLOB_TILDE, nullptr, &glob_result);
    if (return_value != 0)
    {
        globfree(&glob_result);
        std::stringstream ss;
        ss << "glob() failed with return_value " << return_value << std::endl;
        throw std::runtime_error(ss.str());
    }

    std::vector<std::string> filenames;
    for (int i = 0; i < glob_result.gl_pathc; i++)
    {
        filenames.push_back(std::string(glob_result.gl_pathv[i]));
    }

    globfree(&glob_result);

    return filenames;
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
            return (*builtin_func[i])(in, out, conv, arguments);
        }
    }

    return myshell_launch(in, out, conv, arguments);
}

// Destructors
Command::~Command()
{

}