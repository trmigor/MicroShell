#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../headers/command.hpp"
#include "../headers/built-in.hpp"
#include "../headers/central.hpp"

// The main loop
int myshell_loop(void)
{
    int status = 0;
    do
    {
        char* buffer = getwd(nullptr);
        if (buffer == nullptr)
        {
            perror("myshell_loop");
            return EXIT_FAILURE;
        }
        std::string path = buffer;
        int slash = path.find_last_of('/');
        path.erase(0, slash + 1);
        std::cout  << (path.empty() ? "[/" : "[") << path << "]" << (getuid() == 0 ? "! " : "> ");
        
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> diff_cmds;
        int dash_position = input.find_first_of('|');
        while(dash_position != std::string::npos)
        {
            diff_cmds.push_back(input.substr(0, dash_position));
            input.erase(0, dash_position + 1);
            dash_position = input.find_first_of('|');
        }
        diff_cmds.push_back(input);

        for (int i = 0; i < diff_cmds.size(); i++)
        {
            if (i == 0 && diff_cmds.size() != 1)
            {
                diff_cmds[i] += " > pipe_itar_specialized_0001rdwr ";
            }
            if (i == diff_cmds.size() - 1 && diff_cmds.size() != 1)
            {
                diff_cmds[i] += " < pipe_itar_specialized_0001rdwr ";
            }
            if (i != 0 && i != diff_cmds.size() - 1)
            {
                diff_cmds[i] += " < pipe_itar_specialized_0001rdwr > pipe_itar_specialized_0001rdwr ";
            }
            Command cmd(diff_cmds[i]);
            cmd.SplitLine();
            status = cmd.Execute();
        }

    }
    while(status != 0 && std::cin.eof() != 1);
    if (std::cin.eof() == 1)
    {
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}

// The external functions launch
int myshell_launch(std::vector<std::string>& arguments)
{
    pid_t pid, wpid;
    int status;

    std::vector<const char *> argv;
    for (int i = 0; i < arguments.size(); i++)
    {
        argv.push_back(arguments[i].c_str());
    }
    argv.push_back(nullptr);

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
                argv.erase(argv.begin() + i, argv.begin() + i + 2);
            }
            if (arguments[i] == "<")
            {
                close(STDIN_FILENO);
                open(arguments[i + 1].c_str(), O_RDONLY, 0600);
                argv.erase(argv.begin() + i, argv.begin() + i + 2);
            }
        }
        
        if (execvp(argv[0], (char * const *)&argv[0]) < 0)
        {
            perror("myshell_launch");
        }
        
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (pid < 0)
        {
            perror("myshell_launch");
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