#include "headers.h"

typedef struct the_data{
    char a[2];
    int posi;
}the_data;

int l =0;

void pipe_it(char* instr,char ** instr_strings)
{
    if(instr[0]=='|'||instr[strlen(instr)-1]=='|')
    {
        printf("%s INVALID FORMAT%s\n",RED,RESET);
        return;
    }
    char* token =strtok(instr,"|");
    while (token!=NULL)
    {
        /* code */
        if(token!=NULL)
        {
            instr_strings[l]=(char*)malloc(sizeof(char)*strlen(token)+1);
            strcpy(instr_strings[l],token);
            l++;
        }
        token=strtok(NULL,"|");
    }
}

bool io(char* pipe_strings)
{
    int len =strlen(pipe_strings);
    char temp[len+1]; 
    memset(temp,'\0',sizeof(temp));
    strcpy(temp,pipe_strings);
    int k=0;
    for(int i=0;i<strlen(pipe_strings);i++)
    {
        if(pipe_strings[i]=='<')
        {
            k++;
        }
        else if(pipe_strings[i]=='>')
        {
            if(i+1<strlen(pipe_strings)&&pipe_strings[i+1]=='>')
            {
                k++;
                i++;
            }
            else
            {
                k++;
            }
        }
    }
    if(k==0)
    {
        call_other(pipe_strings,pipe_strings);
        return false;
    }
    if(k>=3)
    {
        printf("%scannot handle multiple input output%s\n",RED,RESET);
        return true;
    }
    the_data array[k];
    k=0;
    for(int i=0;i<strlen(pipe_strings)&&k<2;i++)
    {
        if(pipe_strings[i]=='<')
        {
            strcpy(array[k].a,"<");
            array[k].posi=i;
            k++;
        }
        else if(pipe_strings[i]=='>')
        {
            if(i+1<strlen(pipe_strings)&&pipe_strings[i+1]=='>')
            {
                strcpy(array[k].a,">>");
                array[k].posi=i;
                k++;
                i++;
            }
            else
            {
                strcpy(array[k].a,">");
                array[k].posi=i;
                k++;
            }
        }
    }
    int input = dup(STDIN_FILENO);
    if(input<0)
    {
        perror("dup");
        return true;
    }
    int output= dup(STDOUT_FILENO);
    if(output<0)
    {
        perror("dup");
        return true;
    }
    int prev = 0; 
    int end = len;
    char to_call[4096];
    memset(to_call,'\0',sizeof(to_call));
    int bg=0;
    for(int i=0;i<k;i++)
    {
        if(strcmp(array[i].a,"<")==0)
        {
            temp[array[i].posi]=' ';
            strncpy(to_call,temp+prev,array[i].posi-prev);
            prev+=strlen(to_call);
            // printf("%s",to_call);
            char* stringss[4096];
            int ki = 0;
            make_it(to_call,stringss,&ki);
            for(int i=1;i<ki;i++)
            {
                strcat(stringss[0]," ");
                strcat(stringss[0],stringss[i]);
            }
            strcpy(to_call,stringss[0]);
            //but what if ye hi last ho to 
            int ki_bef=ki;
            if(i!=k-1)
            {
                end = array[i+1].posi;
            }
            char to_read[4096];
            memset(to_read,'\0',sizeof(to_read));
            strncpy(to_read,temp+prev,end-prev);
            prev+=strlen(to_read);
            end = len;
            make_it(to_read,stringss,&ki);
            if(i==k-1)
            {
                if(ki-ki_bef==2)
                {
                    if(strcmp(stringss[ki-1],"&")==0)
                    {
                        bg=1;
                        ki--;
                    }
                }
                else if(ki-ki_bef==1)
                {
                    if(stringss[ki-1][strlen(stringss[ki-1])-1]=='&')
                    {
                        bg=1;
                        stringss[ki-1][strlen(stringss[ki-1])-1]='\0';
                    }
                }
            }
            if(ki-ki_bef!=1)
            {
                printf("%sINVALID INPUT%s",RED,RESET);
                // free_kr();
                return true;
            }
            struct stat file_check;
            if(stat(stringss[ki-1],&file_check)==0)
            {int p2=open(stringss[ki-1],O_RDONLY,0644);
            if(p2==-1)
            {
                perror("open");
                // free_kr();
                return true;
            }
            int k = dup2(p2,STDIN_FILENO);
            if(k<0)
            {
                perror("dup2");
                // free_kr();
                return true;
            }
            }
            else
            {
                printf("%sINVALID FILE%s\n",RED,RESET);
                // free_kr();
                return true;
            }
        }
        else if(strcmp(array[i].a,">>")==0)
        {
            temp[array[i].posi]=' ';
            temp[array[i].posi+1]=' ';
            char* stringss[4096];
            int ki = 0;
            if(strcmp(to_call,"")==0)
            {
                strncpy(to_call,temp+prev,array[i].posi-prev);
                prev+=strlen(to_call);
                
            make_it(to_call,stringss,&ki);
            for(int i=1;i<ki;i++)
            {
                strcat(stringss[0]," ");
                strcat(stringss[0],stringss[i]);
                strcpy(to_call,stringss[0]);
            }
            }
            char to_read[4096];
            int ki_bef=ki;
            if(i!=k-1)
            {
                end = array[i+1].posi;
            }
            memset(to_read,'\0',sizeof(to_read));
            strncpy(to_read,temp+prev,end-prev);
            make_it(to_read,stringss,&ki);
            if(i==k-1)
            {
                if(ki-ki_bef==2)
                {
                    if(strcmp(stringss[ki-1],"&")==0)
                    {
                        bg=1;
                        ki--;
                    }
                }
                else if(ki-ki_bef==1)
                {
                    if(stringss[ki-1][strlen(stringss[ki-1])-1]=='&')
                    {
                        bg=1;
                        stringss[ki-1][strlen(stringss[ki-1])-1]='\0';
                    }
                }
            }
            if(ki-ki_bef!=1)
            {
                printf("%sINVALID INPUT\n%s",RED,RESET);
                return true;
            }
            int p2=open(stringss[ki-1],O_APPEND|O_WRONLY|O_CREAT,0644);
            if(p2==-1)
            {
                perror("open");
                return true;
            }
            int kx = dup2(p2,STDOUT_FILENO);
            if(kx<0)
            {
                perror("dup2");
                return true;
            }
            close(p2);
        }
        else if(strcmp(array[i].a,">")==0)
        {
            temp[array[i].posi]=' ';
            char* stringss[4096];
            int ki = 0;
            if(strcmp(to_call,"")==0)
            {
                strncpy(to_call,temp+prev,array[i].posi-prev);
                prev+=strlen(to_call);
                
            make_it(to_call,stringss,&ki);
            for(int i=1;i<ki;i++)
            {
                strcat(stringss[0]," ");
                strcat(stringss[0],stringss[i]);
                strcpy(to_call,stringss[0]);

            }
            }
            char to_read[4096];
            int ki_bef=ki;
            if(i!=k-1)
            {
                end = array[i+1].posi;
            }
            memset(to_read,'\0',sizeof(to_read));
            strncpy(to_read,temp+prev,end-prev);
            make_it(to_read,stringss,&ki);
            if(i==k-1)
            {
                if(ki-ki_bef==2)
                {
                    if(strcmp(stringss[ki-1],"&")==0)
                    {
                        bg=1;
                        ki--;
                    }
                }
                else if(ki-ki_bef==1)
                {
                    if(stringss[ki-1][strlen(stringss[ki-1])-1]=='&')
                    {
                        bg=1;
                        stringss[ki-1][strlen(stringss[ki-1])-1]='\0';
                    }
                }
            }
            if(ki-ki_bef!=1)
            {
                printf("%sINVALID INPUT\n%s",RED,RESET);
                return true;
            }
            int p2=open(stringss[ki-1],O_WRONLY|O_CREAT|O_TRUNC,0644);
            if(p2==-1)
            {
                perror("open");
                return true;
            }
            int kx = dup2(p2,STDOUT_FILENO);
            if(kx<0)
            {
                perror("dup2");
                return true;
            }
            close(p2);
            // close(kx);
        }
    }
    if(bg==1)
    {
    strcat(to_call," &");
    }
    call_other(to_call,to_call);
    int l1=dup2(input,STDIN_FILENO);
    int l2=dup2(output,STDOUT_FILENO);
    close(input);
    close(output);
    return true;
}

void pipings(char *string)
{
    l=0;
    if (strstr(string, "|") == NULL)
    {
        io(string);
        return;
    }
    if(strstr(string,"||")!=NULL)
    {
        printf("%s INVALID FORMAT%s\n",RED,RESET);
        return; 
    }
    char *pipe_strings[4096];
    pipe_it(string, pipe_strings);  // Split string by pipe symbol "|"
    int l1 = dup(STDIN_FILENO);     // Save original stdin
    int l2 = dup(STDOUT_FILENO);    // Save original stdout
    int pipe_fd[2];                 // Array to store pipe file descriptors
    int prev_fd = -1;               // To store the read end of the previous pipe
    
    for(int i=0;i<l ;i++)
    {
        char  tken[4096];
        if(pipe_strings[i]==NULL)
        {
             printf("%s INVALID FORMAT%s\n",RED,RESET);
            return; 
        }
        memset(tken,'\0',sizeof(tken));
        strcpy(tken,pipe_strings[i]);
        char *token;
        token=strtok(tken," ");
        if(token==NULL)
        {
            printf("%s INVALID FORMAT%s\n",RED,RESET);
            return; 
        }
    }
    for (int i = 0; i < l; i++)     // 'l' is the number of piped commands
    {
        if (i < l - 1)
        {
            // Create a pipe except for the last command
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                return;
            }
        }

        int forki = fork();         // Fork to create child process
        if (forki < 0)
        {
            perror("fork");
            return;
        }
        else if (forki == 0)        // Child process
        {
            if (prev_fd != -1)
            {
                // If it's not the first command, use previous pipe's read end
                if (dup2(prev_fd, STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    exit(1);
                }
                close(prev_fd);     // Close the old read end
            }

            if (i < l - 1)
            {
                // If it's not the last command, redirect stdout to the pipe's write end
                if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    exit(1);
                }
                close(pipe_fd[1]);  // Close pipe's write end
            }

            close(pipe_fd[0]);      // Close unused read end of current pipe
            call_other(pipe_strings[i], pipe_strings[i]);  // Execute command
            exit(0);
        }
        else                        // Parent process
        {
            wait(NULL);             // Wait for the child process to finish
            close(pipe_fd[1]);      // Close write end in parent
            if (prev_fd != -1)
            {
                close(prev_fd);      // Close previous pipe read end
            }
            prev_fd = pipe_fd[0];   // Move current pipe read end to prev_fd
        }
    }
    dup2(l1, STDIN_FILENO);
    dup2(l2, STDOUT_FILENO);
    close(l1);
    close(l2);
}