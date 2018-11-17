#ifndef CENTRAL
#define CENTRAL

static int fd[2];

int myshell_loop(void);
int myshell_launch(std::vector<std::string>& arguments);

#endif