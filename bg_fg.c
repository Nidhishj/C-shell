#include "headers.h"

void fg_bg(char **instr,int k)
{
    if(k!=2)
    {
        printf("%s INVALID FORMAT %s",RED,RESET);
        return;
    }
    if(strcmp(instr[0],"bg")==0)
    {
        int k = atoi(instr[1]);
        if(kill(k,0)==-1)
        {

            if (errno == ESRCH) {
                printf("%sNo such process found\n%s", RED, RESET);
            }
            return;
        }
        if (kill(k, SIGCONT) == -1) {
        perror("kill");
    }
    }
    if(strcmp(instr[0],"fg")==0)
    {
        int k = atoi(instr[1]);
        if(kill(k,0)==-1)
        {
            if (errno == ESRCH) {
                printf("%sNo such process found\n%s", RED, RESET);
            }
            return;   
        }
         struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);
        if(kill(k,SIGCONT)==-1)
        {
            perror("kill");
        }
       
        last_fg=k;
        if(waitpid(k,NULL,WUNTRACED)<0)
        {
            perror("waitpid");
        }
         gettimeofday(&end_time, NULL);
            double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
            int time_taken_temp = 0;
            if (elapsed_time >= (int)elapsed_time + 0.5)
            {
                time_taken_temp = (int)elapsed_time;
                time_taken_temp++;
            }
            else
                time_taken_temp = (int)elapsed_time;
                
        for(int i=0;i<num_background_pids;i++)
        {
            if(bg_processes[i].pid==k)
            {if(time_taken_temp>2)
                {strcpy(last_long,bg_processes[i].command);
                time_taken=time_taken_temp;}
                num_background_pids--;
                for(int j=i;j<num_background_pids;j++)
                {
                    bg_processes[j]=bg_processes[j+1];
                }
                break;
            }
        }
        last_fg=-1;
    }
}
