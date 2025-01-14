#include "headers.h"

int cmp(const void * a , const void * b)
{
    BackgroundProcess *a1=(BackgroundProcess*)a;
    BackgroundProcess *b1=(BackgroundProcess*)b;
    
    if(a1->pid>b1->pid)
    return 1;
    else if (a1->pid < b1->pid) {
        return -1;   
    } else {
        return 0;  
    }
}

void activity()
{
    if(num_background_pids==0)
    {
        printf("%s NOTHING TO DISPLAY %s\n",RED,RESET);
        return;
    }
    qsort(bg_processes,num_background_pids,sizeof(bg_processes[0]),cmp);
    for(int i=0;i<num_background_pids;i++)
    {
         char stat_path[4096];
        snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", bg_processes[i].pid);
        FILE *stat_file = fopen(stat_path, "r");
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stat_file) != NULL) 
        {
            char* token = strtok(buffer," ");
            token = strtok(NULL," ");
            strcpy(fg_process,token);
        }
        printf("%d : %s - ",bg_processes[i].pid,fg_process);
        char proc_path[256];
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", bg_processes[i].pid);
        int fd = open(proc_path, O_RDONLY);
        if (fd < 0) {
            perror("Error opening proc file");
            printf("UNKNOWN (Process might have exited)\n");
            continue;
        }
        // char buffer[1024];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);
        if (bytes_read <= 0) {
            printf("UNKNOWN (Error reading proc file)\n");
            continue;
        }
        buffer[bytes_read] = '\0';

        char *token = strtok(buffer, " "); 
        token = strtok(NULL, " ");       
        token = strtok(NULL, " ");  
        if(token!=NULL)
        { 
         char state = token[0];
            switch (state) {
                case 'R':
                    printf("Running\n");
                    break;
                case 'S':
                    printf("Running\n");
                    break;
                case 'D':
                    printf("UNINTERRUPTIBLE SLEEP\n");
                    break;
                case 'Z':
                    printf("Running\n");
                    break;
                case 'T':
                    printf("Stopped\n");
                    break;
                case 'X':
                    printf("stopped\n");
                    break;
                default:
                    printf("UNKNOWN STATE\n");
                    break;
            }
        }
        else {
            printf("UNKNOWN (Could not retrieve state)\n");
        }
    }
    
}      
