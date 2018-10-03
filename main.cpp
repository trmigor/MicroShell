#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "headers/command.hpp"
#include "headers/built-in.hpp"
#include "headers/central.hpp"

int main(void) {

    return myshell_loop();
    
}