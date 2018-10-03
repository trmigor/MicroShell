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

int myshell_loop(void)
{
    int status = 0;
    do
    {
        Command cmd;
        char* buffer = getwd(nullptr);
        if (buffer == nullptr)
        {
            perror("myshell");
            return EXIT_FAILURE;
        }
        std::string path = buffer;
        int slash = path.find_last_of('/');
        path.erase(0, slash + 1);
        std::cout  << (path.empty() ? "[/" : "[") << path << "]" << (getuid() == 0 ? "! " : "> ");
        cmd.ReadLine();
        cmd.SplitLine();
        status = cmd.Execute();
    }
    while(status);
    return EXIT_SUCCESS;
}

int myshell_launch(const std::vector<std::string>& arguments)
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
        if (execvp(argv[0], (char * const *)&argv[0]) == -1)
        {
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    }
    else
        if (pid < 0)
        {
            perror("myshell");
        }
        else
        {
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            }
            while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

    return 1;
}