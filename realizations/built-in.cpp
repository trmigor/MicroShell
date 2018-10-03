#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../headers/built-in.hpp"
#include "../headers/command.hpp"

// Built-in functions realization
// Quits the myshell
int myshell_exit(std::vector<std::string>& arguments)
{
    return 0;
}

// Shows the help message
int myshell_help(std::vector<std::string>& arguments)
{
    int fd[2];
    fd[0] = 0;
    fd[1] = 1;

    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == "<")
        {
            fd[0] = open(arguments[i + 1].c_str(), O_RDONLY | O_CREAT, 0600);
            if (fd[0] < 0)
            {
                perror("myshell");
                return 1;
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
        if (arguments[i] == ">")
        {
            fd[1] = open(arguments[i + 1].c_str(), O_WRONLY | O_CREAT, 0600);
            if (fd[1] < 0)
            {
                perror("myshell");
                return 1;
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
    }

    write(fd[1], "MyShell\n", 8);
    write(fd[1], "Author: Igor Taraymovich\n\n", 26);
    write(fd[1], "Here is the list of built-in functions:\n", 40);

    for(int i = 0; i < myshell_num_builtin(); i++)
    {
        write(fd[1], " ", 1);
        write(fd[1], builtin_str[i].c_str(), builtin_str[i].size());
        write(fd[1], " - ", 3);
        write(fd[1], builtin_descript[i].c_str(), builtin_descript[i].size());
        write(fd[1], "\n", 1);
    }

    if (fd[0] != 0)
    {
        close(fd[0]);
    }
    if (fd[1] != 1)
    {
        close(fd[1]);
    }

    return 1;
}

// Shows current working directory
int myshell_pwd(std::vector<std::string>& arguments)
{
    int fd[2];
    fd[0] = 0;
    fd[1] = 1;

    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == "<")
        {
            fd[0] = open(arguments[i + 1].c_str(), O_RDONLY | O_CREAT, 0600);
            if (fd[0] < 0)
            {
                perror("myshell");
                return 1;
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
        if (arguments[i] == ">")
        {
            fd[1] = open(arguments[i + 1].c_str(), O_WRONLY | O_CREAT, 0600);
            if (fd[1] < 0)
            {
                perror("myshell");
                return 1;
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
    }

    char * buffer = getwd(nullptr);
    if (buffer == nullptr)
    {
        perror("myshell");
        return 1;
    }
    std::string path = buffer;
    write(fd[1], path.c_str(), path.size());
    write(fd[1], "\n", 1);

    if (fd[0] != 0)
    {
        close(fd[0]);
    }
    if (fd[1] != 1)
    {
        close(fd[1]);
    }

    return 1;
}

// Changes working directory
int myshell_cd(std::vector<std::string>& arguments)
{
    if(!getenv("HOME"))
    {
        std::cerr << "myshell: HOME is undefined\n";
    }
    if ((arguments.size() < 2 || arguments[1].empty()) && getenv("HOME"))
    {
        if (chdir(getenv("HOME")) != 0)
        {
            perror("myshell");
        }
    }
    else
    {
        if (chdir(arguments[1].c_str()) != 0)
        {
            perror("myshell");
        }
    }
    return 1;
}

// Shows the time of executing
int myshell_time(std::vector<std::string>& arguments)
{
    int fd[2];
    fd[0] = 0;
    fd[1] = 1;

    for (int i = 0; i < arguments.size(); i++)
    {
        if (arguments[i] == "<")
        {
            fd[0] = open(arguments[i + 1].c_str(), O_RDONLY | O_CREAT, 0600);
            if (fd[0] < 0)
            {
                perror("myshell");
                return 1;
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
        if (arguments[i] == ">")
        {
            fd[1] = open(arguments[i + 1].c_str(), O_WRONLY | O_CREAT, 0600);
            if (fd[1] < 0)
            {
                perror("myshell");
                return 1;
            }
            arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
        }
    }

    if (fd[0] != 0)
    {
        close(fd[0]);
    }
    if (fd[1] != 1)
    {
        close(fd[1]);
    }

    clock_t t = clock();
    Command cmd(arguments);
    

    return 1;
}

// Returns the number of built-un functions
int myshell_num_builtin(void)
{
    return sizeof(builtin_str) / sizeof(std::string);
}