#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include "../headers/built-in.hpp"
#include "../headers/command.hpp"
#include "../headers/central.hpp"

// Built-in functions realization
// Quits the myshell
int myshell_exit(int in, int out, bool conv, std::vector<std::string>& arguments)
{
    return 0;
}

// Shows current working directory
int myshell_pwd(int in, int out, bool conv, std::vector<std::string>& arguments)
{

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // IO redirection
        if (out != 1)
        {
            if(dup2(out, 1) < 0)
            {
                perror("output redirection");
            }
            if(close(out) < 0)
            {
                perror("close");
            }
        }

        char * buffer = getwd(nullptr);
        if (buffer == nullptr)
        {
            perror("myshell_pwd: getwd");
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
            perror("myshell_pwd: fork");
        }
        else
        {
            if (out == STDOUT_FILENO || !conv)
            {
                do
                {
                    wpid = waitpid(pid, &status, WUNTRACED);
                }
                while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
        }
    }

    return 1;
}

// Changes working directory
int myshell_cd(int in, int out, bool conv, std::vector<std::string>& arguments)
{
    char* buffer = getwd(nullptr);
    if (buffer == nullptr)
    {
        perror("myshell_loop");
    }

    if (arguments.size() < 2 || arguments[1] == "--")
    {
        if (getenv("HOME") == nullptr)
        {
            std::cerr << "myshell: HOME is undefined\n";
            return 1;
        }
        if (chdir(getenv("HOME")) != 0)
        {
            perror("myshell_cd: chdir");
        }
        else
        {
            directory_history = buffer;
        }
    }
    else
    {
        if (arguments[1] == "-")
        {
            arguments[1] = directory_history;
        }

        if (chdir(arguments[1].c_str()) != 0)
        {
            perror("myshell_cd: chdir");
        }
        else
        {
            directory_history = buffer;
        }
    }
    return 1;
}

// Shows the time of executing
int myshell_time(int in, int out, bool conv, std::vector<std::string>& arguments)
{
    arguments.erase(arguments.begin(), arguments.begin() + 1);

    struct tms buf;
    if (times(&buf) == (clock_t) - 1)
    {
        perror("myshell_time: times");
        return 1;
    }
    clock_t t = clock();

    Command cmd(conv, arguments);
    cmd.Execute();

    t = clock() - t;
    if (times(&buf) == (clock_t) - 1)
    {
        perror("myshell_time: times");
        return 1;
    }

    // Real minutes
    double rm = floor(((double) t) * 1000 / CLOCKS_PER_SEC / 60);
    // Real seconds
    double rs = ((double) t) * 1000 / CLOCKS_PER_SEC - rm * 60;
    // User minutes
    double um = floor((double) (buf.tms_utime + buf.tms_cutime) * 1000 / CLOCKS_PER_SEC / 60);
    // User seconds
    double us = (double) (buf.tms_utime + buf.tms_cutime) * 1000 / CLOCKS_PER_SEC - um * 60;
    // Sys minutes
    double sm = floor((double) (buf.tms_stime + buf.tms_cstime) * 1000 / CLOCKS_PER_SEC / 60);
    // Sys seconds
    double ss = (double) (buf.tms_stime + buf.tms_cstime) * 1000 / CLOCKS_PER_SEC - sm * 60;

    std::cout << "real: " << std::setiosflags(std::ios::fixed) << std::setprecision(0) << rm << "m"
                << std::setiosflags(std::ios::fixed) << std::setprecision(3) << rs << "s" << std::endl;

    std::cout << "user: " << std::setiosflags(std::ios::fixed) << std::setprecision(0) << um << "m"
                << std::setiosflags(std::ios::fixed) << std::setprecision(3) << us << "s" << std::endl;

    std::cout << "sys: " << std::setiosflags(std::ios::fixed) << std::setprecision(0) << sm << "m"
                << std::setiosflags(std::ios::fixed) << std::setprecision(3) << ss << "s" << std::endl;

    return 1;
}

// Returns the number of built-un functions
int myshell_num_builtin(void)
{
    return sizeof(builtin_str) / sizeof(std::string);
}