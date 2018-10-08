#ifndef BUILT_IN_FUNCTIONS
#define BUILT_IN_FUNCTIONS
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Built-in functions declaration
// Quits the myshell
int myshell_exit(std::vector<std::string>& arguments);
// Shows the help message
int myshell_help(std::vector<std::string>& arguments);
// Shows current working directory
int myshell_pwd(std::vector<std::string>& arguments);
// Changes working directory
int myshell_cd(std::vector<std::string>& arguments);
// Shows the time of executing
int myshell_time(std::vector<std::string>& arguments);

// Built-in functions name list
static std::string builtin_str[] = 
{
    "exit",
    "help",
    "pwd",
    "cd",
    "time"
};

// Built-in functions descriptons list
static std::string builtin_descript[] = 
{
    "Quits the myshell",
    "Shows the help message",
    "Shows current working directory",
    "Changes working directory",
    "Shows the time of executing"
};

// Built-in functions list
static int (*builtin_func[]) (std::vector<std::string>&) = 
{
    &myshell_exit,
    &myshell_help,
    &myshell_pwd,
    &myshell_cd,
    &myshell_time
};

static std::pair<std::string, std::string> last_pwds;

// Returns the number of built-un functions
int myshell_num_builtin(void);

#endif