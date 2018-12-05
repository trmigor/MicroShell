#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "../headers/command.hpp"
#include "../headers/central.hpp"

static bool main_proc = true;

// The interrupt handler
void signal_handler(int signal)
{
    if (main_proc)
    {
        std::cout << std::endl;
        invite();
    }
}

// Enter invitation
void invite(void)
{
    char* buffer = getwd(nullptr);
    if (buffer == nullptr)
    {
        perror("myshell_loop");
    }
    std::string path = buffer;
    int slash = path.find_last_of('/');
    path.erase(0, slash + 1);
    std::cout << (path.empty() ? "[/" : "[") << path << "]" << (getuid() == 0 ? "! " : "> ") << std::flush;
}

// The main loop
int myshell_loop(void)
{
    int status = 0;
    do
    {
        invite();
        std::string input;
        try
        {
            std::getline(std::cin, input);
        }
        catch(std::ios_base::failure e)
        {
            std::cerr << "Can't get the line";
        }
        main_proc = false;
        std::vector<std::string> diff_cmds;
        int dash_position = input.find_first_of('|');
        while(dash_position != std::string::npos)
        {
            diff_cmds.push_back(input.substr(0, dash_position));
            input.erase(0, dash_position + 1);
            dash_position = input.find_first_of('|');
        }
        diff_cmds.push_back(input);

        int in = STDIN_FILENO;
        int fd[2];
        // Conveyor
        if (diff_cmds.size() > 1)
        {
            for (int i = 0; i < diff_cmds.size(); i++)
            {
                if(pipe(fd) < 0)
                {
                    perror("pipe");
                }
                int out = i == diff_cmds.size() - 1 ? STDOUT_FILENO : fd[1];
                Command cmd(in, out, true, diff_cmds[i]);
                // Last command
                if (cmd.SplitLine() < 0)
                {
                    status = 1;
                }
                else
                {
                    status = cmd.Execute();
                }
                if(close(fd[1]) < 0)
                {
                    perror("close");
                }
                in = fd[0];
            }
        }
        // Without conveyor
        else
        {
            Command cmd(in, STDOUT_FILENO, diff_cmds[0]);
            if (cmd.SplitLine() < 0)
            {
                status = 1;
            }
            else
            {
                status = cmd.Execute();
            }
        }
        main_proc = true;
    }
    while(status != 0 && std::cin.eof() != 1);

    if (std::cin.eof() == 1)
    {
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

// The external functions launch
int myshell_launch(int in, int out, bool conv, std::vector<std::string>& arguments)
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
        if (in != 0)
        {
            if(dup2(in, 0) < 0)
            {
                perror("input redirection");
            }
            if(close(in) < 0)
            {
                perror("close");
            }
        }
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
        
        execvp(argv[0], (char * const *)&argv[0]);
        
        perror("myshell_launch: exec");
        close(STDOUT_FILENO);
        exit(EXIT_FAILURE);
    }
    else
    {
        if (pid < 0)
        {
            perror("myshell_launch: fork");
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