#include "headers.h"

void ping(char **instr,int k)
{
    if(k!=2)
    {
        printf("%s INVALID FORMAT%s\n",RED,RESET);
        return;
    }
    pig = 1;
    int pid = atoi(instr[0]);
    int sig = atoi(instr[1]);
     if(kill(pid,0)!=0)
    {
        if(errno==ESRCH)
        {
            printf("%sINVALID PID GIVEN%s\n",RED,RESET);
            return;
        }
    }
    sig%=32;
    if (kill(pid,sig) == -1) {
        perror("kill"); 
        return;
    }
    printf("SIGNAL sent to process %d\n", pid);
    pig = -1 ;
}