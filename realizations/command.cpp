#include <fcntl.h>
#include <dirent.h>
#include <fnmatch.h>
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
    in = 0;
    out = 1;
}

Command::Command(std::string cmd)
{
    in = 0;
    out = 1;
    this -> cmd = cmd;
}

Command::Command(std::vector<std::string> arguments)
{
    in = 0;
    out = 1;
    this -> arguments = arguments;
}

Command::Command(int in, int out, std::string cmd)
{
    this -> cmd = cmd;
    this -> in = in;
    this -> out = out;
}

Command::Command(int in, int out, std::vector<std::string> arguments)
{
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
        if ((star = arguments[i].find_first_of('*')) != std::string::npos ||
                (quest = arguments[i].find_first_of('?')) != std::string::npos)
        {
            std::string path;
            int slash;
            if ((slash = arguments[i].find_last_of('/', std::min(star, quest))) != std::string::npos)
            {
                path = arguments[i].substr(0, slash);
            }
            else
            {
                path = ".";
            }
            struct stat st;
            if(stat(path.c_str(), &st) < 0)
            {
                perror(path.c_str());
                return -1;
            }
            if (S_ISDIR(st.st_mode))
            {
                DIR * d = opendir(path.c_str());
                if (d == nullptr)
                {
                    perror(path.c_str());
                    return -1;
                }
                for (dirent *de = readdir(d); de != nullptr; de = readdir(d))
                {
                    std::string fname = (path != "." ? (path + "/") : "") + de -> d_name;
                    if (fnmatch(arguments[i].c_str(), fname.c_str(), FNM_PATHNAME) == 0)
                    {
                        arguments.insert(arguments.begin() + i + 1, fname);
                    }
                }
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 1);
        }
    }
    return 0;
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
            return (*builtin_func[i])(in, out, arguments);
        }
    }

    return myshell_launch(in, out, arguments);
}

// Destructors
Command::~Command()
{

}