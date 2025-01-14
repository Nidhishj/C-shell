#include "headers.h"

bool hopi(char **instr,int k)
{
     char arr[4097];
    memset(arr,'\0',sizeof(arr));

    if(k==0)
    {
        getcwd(arr,sizeof(arr));
        chdir(home);
        printf("%s%s%s\n",GREEN,home,RESET);
        strcpy(prev_dir,arr);
    }
    else
    {        for(int i=0;i<k;i++)
        {
            memset(arr,'\0',sizeof(arr));
            getcwd(arr,sizeof(arr));
            if(instr[i][0]=='-')
            {
                if(strcmp(prev_dir,"")==0)
                {
                printf("%sTHERE IS NO PREVIOUS DIRECTORY%s\n",RED,RESET);
                return false;
                }
                chdir(prev_dir);
                printf("%s%s%s\n",GREEN,prev_dir,RESET);
                strcpy(prev_dir,arr);
            }
            else if(instr[i][0]=='~')
            {
                strcpy(prev_dir,arr);
                chdir(home);
                if(strlen(instr[i])>1)
                {
                    memmove(instr[i],instr[i]+2,strlen(instr[i]));
                    i--;
                    continue;
                }
                printf("%s%s%s\n",GREEN,home,RESET);
            }
            else 
            {
                if(chdir(instr[i])!=0)
                {
                    printf("%sINVALID DIRECTORY%s\n",RED,RESET);
                    return false;
                }
                strcpy(prev_dir,arr);
                printf("%s%s%s\n",GREEN,getcwd(arr,sizeof(arr)),RESET);
            }
        }
    }
}