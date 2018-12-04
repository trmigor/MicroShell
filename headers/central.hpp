#ifndef CENTRAL
#define CENTRAL

// The interrupt handler
void signal_handler(int signal);

// Enter invitation
void invite(void);

// The main loop
int myshell_loop(void);

// The external functions launch
int myshell_launch(int in, int out, bool conv, std::vector<std::string>& arguments);

#endif