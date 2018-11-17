#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "../headers/built-in.hpp"
#include "../headers/command.hpp"
#include "../headers/central.hpp"

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
                if (arguments[i + 1] != "pipe_itar_specialized_0001rdwr119638579")
                {
                    open(arguments[i + 1].c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
                }
                else
                {
                    dup2(fd[1], 1);
                    close(fd[1]);
                    close(fd[0]);
                }
                arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
                i--;
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
    pid_t pid, wpid;
    int status;

    arguments.erase(arguments.begin(), arguments.begin() + 1);

    struct tms buf;
    times(&buf);
    clock_t t = clock();

    Command cmd(arguments);
    cmd.Execute();

    t = clock() - t;
    times(&buf);

    double rm = floor(((double) t) / CLK_TCK / 60);
    double rs = ((double) t) / CLK_TCK - rm * 60;
    double um = floor((double) (buf.tms_utime + buf.tms_cutime) / sysconf(_SC_CLK_TCK) / 60);
    double us = (double) (buf.tms_utime + buf.tms_cutime) / sysconf(_SC_CLK_TCK) - um * 60;
    double sm = floor((double) (buf.tms_stime + buf.tms_cstime) / sysconf(_SC_CLK_TCK) / 60);
    double ss = (double) (buf.tms_stime + buf.tms_cstime) / sysconf(_SC_CLK_TCK) - sm * 60;

    pid = fork();
    if (pid == 0)
    {
        // IO redirection
        for (int i = 0; i < arguments.size(); i++)
        {
            if (arguments[i] == ">")
            {
                close(STDOUT_FILENO);
                if (arguments[i + 1] != "pipe_itar_specialized_0001rdwr119638579")
                {
                    open(arguments[i + 1].c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
                }
                else
                {
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                    close(fd[0]);
                }
                arguments.erase(arguments.begin() + i, arguments.begin() + i + 2);
                i--;
            }
        }

        std::cout << "real: " << std::setiosflags(std::ios::fixed) << std::setprecision(0) << rm << "m" << std::setiosflags(std::ios::fixed) << std::setprecision(3) << rs << "s" << std::endl;
        std::cout << "user: " << std::setiosflags(std::ios::fixed) << std::setprecision(0) << um << "m" << std::setiosflags(std::ios::fixed) << std::setprecision(3) << us << "s" << std::endl;
        std::cout << "sys: " << std::setiosflags(std::ios::fixed) << std::setprecision(0) << sm << "m" << std::setiosflags(std::ios::fixed) << std::setprecision(3) << ss << "s" << std::endl;

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