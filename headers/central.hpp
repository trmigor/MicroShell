#ifndef CENTRAL
#define CENTRAL
#include <string>

static std::string directory_history;

// The interrupt handler
void signal_handler(int signal);

// Enter invitation
void invite(void);

// The main loop
int myshell_loop(void);

// The external functions launch
int myshell_launch(int in, int out, bool conv, std::vector<std::string>& arguments);

#endif