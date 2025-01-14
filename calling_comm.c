#include "headers.h"

void make_it(char* instr,char ** instr_strings,int* k)
{
    char* token =strtok(instr," ");
    while (token!=NULL)
    {
        /* code */
        if(token!=NULL)
        {
            instr_strings[*k]=(char*)malloc(sizeof(char)*strlen(token)+1);
            strcpy(instr_strings[*k],token);
            (*k)++;
        }
        token=strtok(NULL," ");
    }
}


void shift_left(char *array[], int *size) {
    for (int i = 0; i < *size - 1; i++) {
        array[i] = array[i + 1];
    }
    array[*size - 1] = NULL;
    (*size)--;
}

void replace_alias(char *instr) {
    for (int i = 0; i < alias_count; i++) {
        // Find the alias name in the input string
        char *pos = strstr(instr, alias_array[i].name);
        if (pos != NULL) {
            // Calculate lengths
            int alias_len = strlen(alias_array[i].name);
            int command_len = strlen(alias_array[i].command);
            int instr_len = strlen(instr);

            // Shift the rest of the string to make space for the alias command
            if (command_len > alias_len) {
                // Make space for the new command
                memmove(pos + command_len, pos + alias_len, instr_len - alias_len + 1);
            } else if (command_len < alias_len) {
                // Shrink the string if the command is shorter than the alias name
                memmove(pos + command_len, pos + alias_len, instr_len - alias_len + 1);
            }

            // Copy the alias command in place of the alias name
            memcpy(pos, alias_array[i].command, command_len);
        }
    }
}

void call_other(char * instr,char *logy)
{   
    no_process=1;
    char *instr_strings[4096];
    int k=0;
    char copy[4097];
    memset(copy,'\0',sizeof(copy));
    replace_alias(instr);
    if(strstr(instr,"|")!=NULL||strstr(instr,">")!=NULL||strstr(instr,"<")!=NULL)
    {
        pipings(instr);
        return;
    }
    make_it(instr,instr_strings,&k);
    if(strcmp(instr_strings[0],"hop")==0)
    {
        shift_left(instr_strings, &k);
        hopi(instr_strings,k);
    }
    else if(strcmp(instr_strings[0],"log")==0)
    {
        shift_left(instr_strings, &k);
        if(k==0)
        {
            strcpy(copy,"");
        }
        else
        {
            for(int i=0;i<k;i++)
            {
                strcat(copy,instr_strings[i]);
                if(i!=k-1)
                {
                    copy[strlen(copy)+1]='\0';
                    copy[strlen(copy)]=' ';
                }
            }
        }
        logg(copy,logy);
        log_num=1;
    }
    else if(strcmp(instr_strings[0],"reveal")==0)
    {
        shift_left(instr_strings, &k);
        if(k==0)
        strcpy(copy,"");
        else
        {
            for(int i=0;i<k;i++)
            {
                strcat(copy,instr_strings[i]);
                if(i!=k-1)
                {
                    copy[strlen(copy)+1]='\0';
                    copy[strlen(copy)]=' ';
                }
            }
        }
        reveal(copy);
    }
    else if(strcmp(instr_strings[0],"proclore")==0)
    {
        shift_left(instr_strings, &k);
        if(k==0)
        strcpy(copy,"");
        else
        {
            for(int i=0;i<k;i++)
            {
                strcat(copy,instr_strings[i]);
                if(i!=k-1)
                {
                    copy[strlen(copy)+1]='\0';
                    copy[strlen(copy)]=' ';
                }
            }
        }
        proclore(copy);
    }
    else if(strcmp(instr_strings[0],"seek")==0)
    { 
        shift_left(instr_strings, &k);
        seek(instr_strings,k);
    }
    else if(strcmp(instr_strings[0],"activities")==0)
    {
        if(k!=1)
        {
            printf("%s INVALID FORMAT%s\n",RED,RESET);
            return;
        }
        activity();
    }
    else if(strcmp(instr_strings[0],"ping")==0)
    {
        shift_left(instr_strings,&k);
        ping(instr_strings,k);
    }
    else if(strcmp(instr_strings[0],"fg")==0||strcmp(instr_strings[0],"bg")==0)
    {
        fg_bg(instr_strings,k);
    }
    else if(strcmp(instr_strings[0],"neonate")==0)
    {
        neonate(instr_strings,k);
    }
    else if(strcmp(instr_strings[0],"iMan")==0)
    {
        iman(instr_strings,k);
    }
    else 
    {
        instr_strings[k] = NULL;
        k++;
        system_commands(instr_strings[0],instr_strings,k,logy);
    }
    free:
    for(int i=0;i<k;i++)
    {
        free(instr_strings[i]);
    }
}
