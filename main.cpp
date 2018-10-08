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
    // IO file create
    int fd;
    if ((fd = open("pipe", O_RDWR)) < 0)
    {
        if (mkfifo("pipe", 0777) < 0)
        {
            perror("myshell_pipe_create");
        }
    }
    else
    {
        close(fd);
    }

    // The main loop
    int return_value = myshell_loop();

    // IO file remove
    if ((fd = open("pipe", O_RDWR)) >= 0)
    {
        if (remove("pipe") != 0 || close(fd) < 0)
        {
            perror("myshell_pipe_close");
        }
    }

    return return_value;
}