#include "headers.h"

void display()
{
    no_process=0;
    struct passwd *pw;
    __uid_t uid=getuid();
    // const char *RESET = "\033[0m";  // Reset to default color
    pw=getpwuid(uid);
    // printf("<%s@",pw->pw_name);
    char host[255];
    char user[1000];
    char curr[4097];
    getcwd(curr,sizeof(curr));
    // printf("%s %s",curr,home);
    gethostname(host,sizeof(host));
    strcpy(user,pw->pw_name);
    // printf("%s%s",YELLOW,curr);//this will get the total of the current directory 
    if(strncmp(home,curr,strlen(home))==0)
    {
        printf("<%s%s@%s%s:~",YELLOW,user,RESET,host);
        for(int i=strlen(home);i<strlen(curr);i++)
        {
            printf("%c",curr[i]);
        }
        if(time_taken>2)
        {printf(" %s:",last_long);
        time_taken=-1;}
        printf("> ");
    }
    //if not then nothing just print it
    else
    { 
        if(time_taken<=2)
    {printf("<%s%s@%s%s:%s> ",YELLOW,user,RESET,host,curr);}
    else
    {printf("<%s%s@%s%s:%s %s > ",YELLOW,user,RESET,host,curr,last_long);}
    }
        time_taken=-1;

    return;
}

