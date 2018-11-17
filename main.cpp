#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "headers/command.hpp"
#include "headers/built-in.hpp"
#include "headers/central.hpp"

int main(void)
{

    // Welcome message
    std::cout << "MyShell\n";
    std::cout << "Author: Igor Taraymovich\n\n";
    std::cout << "Here is the list of built-in functions:\n";

    for(int i = 0; i < myshell_num_builtin(); i++)
    {
        std::cout << " " << builtin_str[i] << " - " << builtin_descript[i] << std::endl;
    }

    // IO file create
    //int fd;
    /*if ((fd = open("pipe_itar_specialized_0001rdwr119638579", O_RDWR)) < 0)
    {
        if (mkfifo("pipe_itar_specialized_0001rdwr119638579", 0777) < 0)
        {
            perror("myshell_pipe_create");
        }
    }
    else
    {
        close(fd);
    }*/

    // The main loop
    int return_value = myshell_loop();

    /*// IO file remove
    if ((fd = open("pipe_itar_specialized_0001rdwr119638579", O_RDWR)) >= 0)
    {
        if (remove("pipe_itar_specialized_0001rdwr119638579") != 0 || close(fd) < 0)
        {
            perror("myshell_pipe_remove");
        }
    }*/

    return return_value;
}