#ifndef CENTRAL
#define CENTRAL
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int myshell_loop(void);
int myshell_launch(std::vector<std::string>& arguments);

#endif