#ifndef SYSTEMCOMM_H
#define SYSTEMCOMM_H

void system_commands(char* command,char **args,int h,char *logy);
void sigchld_handler(int sig);
// 
#endif