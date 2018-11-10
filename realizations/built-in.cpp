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

// Built-in functions realization
// Quits the myshell
int myshell_exit(std::vector<std::string>& arguments)
{
    return 0;
}

// Shows current working directory
int myshell_pwd(std::vector<std::string>& arguments)
{

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // IO redirection
        for (int i = 0; i < arguments.size(); i++)
        {
            if (arguments[i] == ">")
            {
                close(STDOUT_FILENO);
                int fd = open(arguments[i + 1].c_str(), O_WRONLY | O_CREAT, 0600);
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
        
        close(STDOUT_FILENO);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (pid < 0)
        {
            perror("myshell_pwd");
        }
        else
        {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            }
            while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }

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

    clock_t t = clock();
    Command cmd(arguments);

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // IO redirection
        for (int i = 0; i < arguments.size(); i++)
        {
            if (arguments[i] == ">")
            {
                close(STDOUT_FILENO);
                open(arguments[i + 1].c_str(), O_WRONLY | O_CREAT, 0600);
                arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
            }
        }

        //

        close(STDOUT_FILENO);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (pid < 0)
        {
            perror("myshell_time");
        }
        else
        {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            }
            while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    
    return 1;
}

// Returns the number of built-un functions
int myshell_num_builtin(void)
{
    return sizeof(builtin_str) / sizeof(std::string);
}