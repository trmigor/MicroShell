#include <unistd.h>
#include <csignal>
#include <iostream>
#include "headers/command.hpp"
#include "headers/built-in.hpp"
#include "headers/central.hpp"

int main(void)
{
    // Signal redirection
    std::signal(SIGINT, signal_handler);

    // Welcome message
    std::cout << std::endl << "MicroShell v0.3.0\n";
    std::cout << "Author: Igor Taraymovich\n\n";
    std::cout << "Here is the list of built-in functions:\n";

    for(int i = 0; i < myshell_num_builtin(); i++)
    {
        std::cout << " " << builtin_str[i] << " - " << builtin_descript[i] << std::endl;
    }
    std::cout << std::endl;

    // The main loop
    int return_value = myshell_loop();

    return return_value;
}