#ifndef BUILT_IN_FUNCTIONS
#define BUILT_IN_FUNCTIONS

// Built-in functions declaration
// Quits the myshell
int myshell_exit(int in, int out, bool conv, std::vector<std::string>& arguments);
// Shows current working directory
int myshell_pwd(int in, int out, bool conv, std::vector<std::string>& arguments);
// Changes working directory
int myshell_cd(int in, int out, bool conv, std::vector<std::string>& arguments);
// Shows the time of executing
int myshell_time(int in, int out, bool conv, std::vector<std::string>& arguments);

// Built-in functions name list
static std::string builtin_str[] = 
{
    "exit",
    "pwd",
    "cd",
    "time"
};

// Built-in functions descriptons list
static std::string builtin_descript[] = 
{
    "Quits the myshell",
    "Shows current working directory",
    "Changes working directory",
    "Shows the time of executing"
};

// Built-in functions list
static int (*builtin_func[]) (int, int, bool, std::vector<std::string>&) = 
{
    &myshell_exit,
    &myshell_pwd,
    &myshell_cd,
    &myshell_time
};

// Returns the number of built-un functions
int myshell_num_builtin(void);

#endif