#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../headers/built-in.hpp"
#include "../headers/command.hpp"
#include "../headers/central.hpp"

// Built-in functions realization
// Quits the myshell
int myshell_exit(std::vector<std::string>& arguments)
{
    return 0;
}

// Shows the help message
int myshell_help(std::vector<std::string>& arguments)
{
    std::streambuf *back_in = std::cin.rdbuf(),
                    *back_out = std::cout.rdbuf();
    std::ifstream input_file;
    std::ofstream output_file;

    // IO redirection
    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == "<")
        {
            input_file.open(arguments[i + 1]);
            std::cin.rdbuf(input_file.rdbuf());
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
        if (arguments[i] == ">")
        {
            output_file.open(arguments[i + 1]);
            std::cout.rdbuf(output_file.rdbuf());
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
    }

    std::cout << "MyShell\n";
    std::cout << "Author: Igor Taraymovich\n\n";
    std::cout << "Here is the list of built-in functions:\n";

    for(int i = 0; i < myshell_num_builtin(); i++)
    {
        std::cout << " " << builtin_str[i] << " - " << builtin_descript[i] << std::endl;
    }

    std::cin.rdbuf(back_in);
    std::cout.rdbuf(back_out);

    return 1;
}

// Shows current working directory
int myshell_pwd(std::vector<std::string>& arguments)
{
    std::streambuf *back_in = std::cin.rdbuf(),
                    *back_out = std::cout.rdbuf();
    std::ifstream input_file;
    std::ofstream output_file;

    // IO redirection
    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == "<")
        {
            input_file.open(arguments[i + 1]);
            std::cin.rdbuf(input_file.rdbuf());
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
        if (arguments[i] == ">")
        {
            output_file.open(arguments[i + 1]);
            std::cout.rdbuf(output_file.rdbuf());
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
    }

    char * buffer = getwd(nullptr);
    if (buffer == nullptr)
    {
        perror("myshell_pwd");
        return 1;
    }
    std::string path = buffer;
    std::cout << path << std::endl;

    std::cin.rdbuf(back_in);
    std::cout.rdbuf(back_out);

    return 1;
}

// Changes working directory
int myshell_cd(std::vector<std::string>& arguments)
{
    if (arguments.size() < 2)
    {
        if (getenv("HOME") == nullptr)
        {
            std::cerr << "myshell: HOME is undefined\n";
            return 1;
        }
        if (chdir(getenv("HOME")) != 0)
        {
            perror("myshell_cd");
        }
    }
    else
    {
        if (chdir(arguments[1].c_str()) != 0)
        {
            perror("myshell_cd");
        }
    }
    return 1;
}

// Shows the time of executing
int myshell_time(std::vector<std::string>& arguments)
{
    std::streambuf *back_in = std::cin.rdbuf(),
                    *back_out = std::cout.rdbuf();
    std::ifstream input_file;
    std::ofstream output_file;

    // IO redirection
    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == "<")
        {
            input_file.open(arguments[i + 1]);
            std::cin.rdbuf(input_file.rdbuf());
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
        if (arguments[i] == ">")
        {
            output_file.open(arguments[i + 1]);
            std::cout.rdbuf(output_file.rdbuf());
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
    }

    clock_t t = clock();
    Command cmd(arguments);
    
    std::cin.rdbuf(back_in);
    std::cout.rdbuf(back_out);

    return 1;
}

// Returns the number of built-un functions
int myshell_num_builtin(void)
{
    return sizeof(builtin_str) / sizeof(std::string);
}