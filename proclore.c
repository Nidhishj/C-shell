#include "headers.h"

void print_process_info(pid_t pid) {
    if(pid==0)
    {
        printf("%sINVALID PID GIVEN%s\n",RED,RESET);
        return;
    }
    if(kill(pid,0)!=0)
    {
        if(errno==ESRCH)
        {
            printf("%sINVALID PID GIVEN%s\n",RED,RESET);
            return;
        }
    }
    printf("pid : %d\n", pid);

    pid_t pgid = getpgid(pid);
    if (pgid == -1) {
        perror("Error getting process group ID");
    }
    // Get the process status and virtual memory usage from /proc/[pid]/stat
    char stat_path[4096];
    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);

    FILE *stat_file = fopen(stat_path, "r");
    if (stat_file) {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stat_file) != NULL) {
            // The 3rd field is the process status, and the 23rd field is the virtual memory size
            char *token = strtok(buffer, " ");
            // char *token;
            char *process_status = NULL;
            char *virtual_memory = NULL;
            char *pgrp=NULL;
            int gpid=__getpgid(pid);
    
            for (int i = 1; i <= 23; i++) {
                token = strtok(NULL, " ");
                if (i == 2) {
                    process_status = token;
                }
                else if(i==4)
                {
                    pgrp=token;
                }
                else if(i==7)
                {
                    // tgpid=token;
                      pid_t fg_pgid = tcgetpgrp(STDIN_FILENO);
                      if (fg_pgid == -1) {
                        perror("Error getting foreground process group ID");
                    }
                    else if(pgid==fg_pgid&&pgid==pid)
                    {
                        printf("process status: %s+\n",process_status);
                    }
                    else
                    {
                        printf("process status : %s\n", process_status);
                    }
                }
                 else if (i == 22) {
                    virtual_memory = token;
                }
            }

                printf("Process Group : %d\n", pgid);
            printf("Virtual memory : %s\n", virtual_memory);
        } else {
            perror("Error reading stat file");
        }
        fclose(stat_file);
    } else {
        perror("Error opening stat file");
    }

    // Get the executable path from /proc/[pid]/exe
    char exe_path[4096];
    snprintf(stat_path, sizeof(stat_path), "/proc/%d/exe", pid);
    ssize_t len = readlink(stat_path, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
        printf("executable path : %s\n", exe_path);
    } else {
        fprintf(stderr,"%s",RED);
        perror("Error reading executable path");
        fprintf(stderr,"%s",RESET);
    }
}

void proclore(char *instr)
{
    int len=strlen(instr);
    int num=0;
    bool num_check=false;
    for(int i=0;i<len;i++)
    {
        if(instr[i]==' ')
        continue;
        else 
        {
            if(instr[i]>='0'&&instr[i]<='9')
            {
                num_check=true;
                num=num*10+(instr[i]-'0');
                i++;
                for(i;i<len;i++)
                {
                    if(instr[i]>='0'&&instr[i]<='9')
                    {
                        num=num*10+(instr[i]-'0');
                    }
                    else if(instr[i]==' ')
                    {
                        for(i;i<len;i++)
                        {
                            if(instr[i]==' '||instr[i]=='\0')
                            {
                                continue;
                            }
                            else
                            {
                                printf("%sINVALID NOT A NUMBER%s\n",RED,RESET);
                                return;
                            }
                        }
                    }
                    else
                    {
                        printf("%sINVALID NOT A NUMBER%s\n",RED,RESET);
                        return;
                    }
                }
            }
            else
            {
                printf("%sINVALID NOT A NUMBER%s\n",RED,RESET);
                        return;
            }
        }
    }
    //else no number which is given would be considered correect
    if(num==0&&!num_check)
    {
        num=getpid();
    }
    print_process_info(num);
}