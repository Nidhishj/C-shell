#include "headers.h"

void logg(char* order,char * logy)
{
    if(log_execute==1)
    {
        // log_execute=0;
        return;
    }
    char path[4097];
    memset(path,'\0',sizeof(path));
    strcpy(path,home);
    strcat(path,"/log.txt");
    int fd=open(path,O_CREAT|O_RDWR,0644);
    if (fd == -1) {
        fprintf(stderr,"%s",RED);
        perror("Error opening the file");
        fprintf(stderr,"%s",RESET);
    }
    else
    {
        if(strcmp(order,"change")==0)
        {// {   int fd2=open("file.txt",O_CREAT|O_RDONLY,0644);
            lseek(fd,0,SEEK_SET);
            char buffer[61500];
            memset(buffer,'\0',sizeof(buffer));
            char log_hist[15][4097];
            memset(log_hist,'\0',sizeof(log_hist));
            int size_read=read(fd,buffer,sizeof(buffer));
            buffer[size_read]='\0';
            char *token =strtok(buffer,"\n");
            int k=0;
            while (token!=NULL&&k<15)
            {
                strcpy(log_hist[k++],token);
                token=strtok(NULL,"\n");
            }
            for(int i=13;i>=0;i--)
            {
                memset(log_hist[i+1],'\0',sizeof(log_hist[i+1]));
                strcpy(log_hist[i+1],log_hist[i]);
            }
            // logy[strlen(logy)]='\n';
            strcpy(log_hist[0],logy);
            if(strcmp(log_hist[0],log_hist[1])==0)
            {
                return;
            }
            lseek(fd,0,SEEK_SET);
            ftruncate(fd, 0);
            // Write the updated log_hist array to the file
            lseek(fd, 0, SEEK_SET); 
            for (int i = 0; i < 15; i++) {
                if (strcmp(log_hist[i], "") != 0) {
                    write(fd, log_hist[i], strlen(log_hist[i]));
                    write(fd, "\n", 1);
                }
            }
        }
        else if(strcmp(order,"")==0)
        {
            lseek(fd,0,SEEK_SET);
            char buffer[61500];
            memset(buffer,'\0',sizeof(buffer));
            char log_hist[15][4097];
            memset(log_hist,'\0',sizeof(log_hist));
            int size_read=read(fd,buffer,sizeof(buffer));
            buffer[size_read]='\0';
            char *token =strtok(buffer,"\n");
            int k=0;
            while (token!=NULL&&k<15)
            {
                strcpy(log_hist[k++],token);
                token=strtok(NULL,"\n");
            }
            k--;
            for(k;k>=0;k--)
            {
                printf("%s\n",log_hist[k]);
            }
        }
        else if(strcmp(order,"purge")==0)
        {
            ftruncate(fd,0);
        }
        else
        {
            
            char token[4096];
            memset(token,'\0',sizeof(token));
            strcpy(token,order);
            strtok(token," ");
            if(strcmp(token,"execute")!=0)
            {
                printf("%sINVALID LOG COMMAND%s\n",RED,RESET);
                return;
            }
            else
            {
                char* abcd=strtok(NULL," ");
                strcpy(token,abcd);
                while (token!=NULL&&token[0]==' ')
                {
                strtok(token," ");
                }
                if(token==NULL)
                {
                    printf("%sNO INDEX MENTIONED%s\n",RED,RESET);
                }
                int b=0;
                for(int i=0;i<strlen(token);i++)
                {
                    if(token[i]==' ')
                    continue;
                    else if(token[i]>='0'&&token[i]<='9')
                    {
                        b*=10;
                        b+=token[i]-'0';
                    }
                    else
                    {
                        printf("%sINVALID INDEX MENTIONED%s\n",RED,RESET);
                        return;
                    }
                }
                if(b<=0||b>15)
                {
                   printf("%sINVALID INDEX MENTIONED%s\n",RED,RESET);
                    return;
                }
                else
                {
                    lseek(fd,0,SEEK_SET);
            char buffer[61500];
            memset(buffer,'\0',sizeof(buffer));
            char log_hist[15][4097];
            memset(log_hist,'\0',sizeof(log_hist));
            int size_read=read(fd,buffer,sizeof(buffer));
            buffer[size_read]='\0';
            char *token =strtok(buffer,"\n");
            int k=0;
            while (token!=NULL&&k<15)
            {
                strcpy(log_hist[k++],token);
                token=strtok(NULL,"\n");
            }
            if(b>k)
            {
                printf("%sNO HISTORY TILL THE INDEX MENTIONED%s\n",RED,RESET);
            }
            k=b-1;
            char comm_file[10000];
            char instr_file[10000];
            memset(comm_file,'\0',sizeof(comm_file));
            memset(instr_file,'\0',sizeof(instr_file));
            int l=strlen(log_hist[k]);
            char log_history[10000];
            char log_history2[10000];
            memset(log_history2,'\0',sizeof(log_history2));
            memset(log_history,'\0',sizeof(log_history));
            strcpy(log_history2,log_hist[k]);
            int len = strlen(log_history2);
            // int k=0;
            int new_length=len;
            for (int i = 0; i < l; i++) {
        if (log_history2[i] == '&') {
            new_length++;
        }
         int j = 0; // Index for the new string
    for (int i = 0; i < l; i++) {
        log_history[j++] = log_history2[i];
        if (log_history2[i] == '&') {
            log_history[j++] = ';';
        }
    }
            }
            char *tokeni;
            tokeni=strtok(log_history,";");
            char* diffi[4096];
            int vari=0;
            while(tokeni!=NULL)
            {
                if(tokeni!=NULL)
                {
                    diffi[vari]=(char*)malloc(sizeof(char)*strlen(tokeni));
                    memset(diffi[vari],'\0',sizeof(diffi[vari]));
                    strcpy(diffi[vari],tokeni);
                    // k++;
                    vari++;
                }   
                tokeni=strtok(NULL,";");
            }
            for(int i=0;i<vari;i++)
            {
                // log_execute=1;
                char instri[4097];
                memset(instri,'\0',sizeof(instri));
                memset(instr_file,'\0',sizeof(instr_file));
                strcpy(instri,diffi[i]);
                // char *token=strtok(diffi[i]," ");
                //so we get the command
                call_other(instri,log_hist[k]);
                log_execute=0;
            }
            for(int i=0;i<vari;i++)
            {
                free(diffi[i]);
            }
            }
            }
        }
    }
    close(fd);
}
