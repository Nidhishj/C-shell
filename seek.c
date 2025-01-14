#include "headers.h"
int d = 0, e = 0, f = 0;

int file_founds = 0;
char start[4096];
char e_special[4096];
// int is_folder=0;

int is_executable_seek(mode_t mode)
{
    return (mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH);
}

void recurssion(char *curr, char *to_find)
{
    if (access(curr, R_OK | X_OK) == -1)
    {
        return;
    }
    struct dirent *entry;
    DIR *dirp = opendir(curr);
    if (dirp == NULL)
    {
        printf("%sINVALID DIRECTORY\n%s", RED, RESET);
        return;
    }
    char file_names[4096];
    int count = 0;
    // while ((entry = readdir(dirp)))
    // {
    //     if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
    //     continue;
    //     file_names[count] = (char *)malloc(strlen(entry->d_name) + 1);
    //     strcpy(file_names[count],entry->d_name);
    //     count++;
    // }
    while ((entry = readdir(dirp)))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        strcpy(file_names, entry->d_name);
        struct stat file_stat;
        char file_path[4097];
        memset(file_path, '\0', sizeof(file_path));
        if (curr == NULL || file_names == NULL)
        {
            fprintf(stderr, "Error: current path or entry name is NULL.\n");
            continue;
        }
        snprintf(file_path, sizeof(file_path), "%s/%s", curr, file_names);
        if (access(file_path, F_OK) != 0)
        {
            continue;
        }
        if (stat(file_path, &file_stat) == 0)
        {
            if (access(file_path, R_OK) == 0)
            {
                if (S_ISDIR(file_stat.st_mode))
                {
                    // found folder
                    if (strncmp(file_names, to_find,strlen(to_find)) == 0 && f == 0)
                    {
                        if (e == 1 && file_founds == 0)
                        {
                            strcpy(e_special, file_path);
                        }
                        file_founds++;
                        printf("%s%s%s\n", BLUE, file_path + strlen(start), RESET);
                    }
                    recurssion(file_path, to_find);
                }
                else
                {
                    if (strncmp(file_names, to_find,strlen(to_find)) == 0 && d == 0)
                    {
                        if (e == 1 && file_founds == 0)
                        {
                            strcpy(e_special, file_path);
                        }
                        file_founds++;
                        printf("%s%s%s\n", GREEN, file_path + strlen(start), RESET);
                    }
                }
            }
        }
        else
        {
            fprintf(stderr, "stat error on %s", file_path);
            perror("");
        }
    }
    closedir(dirp);
}

void seek(char **instr, int k)
{
    d = 0, e = 0, f = 0;
    file_founds = 0;
    char from_called[4097];
    memset(from_called, '\0', sizeof(from_called));
    getcwd(from_called, sizeof(from_called));
    char to_search[4096];
    memset(to_search, '\0', sizeof(to_search));
    memset(start, '\0', sizeof(start));
    if (k == 0)
    {
        printf("%sNO FILE GIVEN TO SEARCH%s\n", RED, RESET);
        return;
    }
    for (int i = 0; i < k; i++)
    {
        if (instr[i][0] == '-')
        {
            if (strlen(instr[i]) > 1)
            {
                if (instr[i][1] == 'd')
                    d = 1;
                else if (instr[i][1] == 'e')
                    e = 1;
                else if (instr[i][1] == 'f')
                    f = 1;
            }
            else
            {
                if (strcmp(prev_dir, "") == 0)
                {
                    printf("%sNO PREVIOUS DIRECTORY\n%s", RED, RESET);
                    return;
                }
                if (strcmp(start, "") == 0)
                    strcpy(start, prev_dir);
                else
                {
                    printf("%sMORE THAN ONE PATH CANNOT BE GIVEN\n%s", RED, RESET);
                    return;
                }
            }
        }
        else if (strcmp(to_search, "") == 0)
        {
            strcpy(to_search, instr[i]);
        }
        else
        {
            if (strcmp(start, "") == 0)
            {
                strcpy(start, instr[i]);
                char arr[4097];
                getcwd(arr, sizeof(arr));
                if (start[0] == '~')
                {
                    chdir(home);
                    strcpy(start,home);
                    // memmove(start, start + 1, strlen(start));
                }
            }
            else
            {
                printf("%sMORE THAN ONE PATH CANNOT BE GIVEN\n%s", RED, RESET);
                return;
            }
        }
    }
    if (strcmp(start, "") == 0)
    {
        getcwd(start, sizeof(start));
    }
    if (d == 1 && f == 1)
    {
        printf("%sInvalid Flags!%s\n", RED, RESET);
        return;
    }
    recurssion(start, to_search);
    if (e == 1)
    {
        struct stat file_stat;
        if (stat(e_special, &file_stat) == 0)
        {
            if (file_founds == 1)
            {
                if (chdir(e_special) != 0)
                {
                    if (is_executable_seek(file_stat.st_mode))
                    {
                        int pid = fork();
                        if (pid < 0)
                        {
                            perror("FORK");
                            return;
                        }
                        if (pid == 0)
                        {
                            if (execl(e_special, e_special, (char *)NULL) == -1)
                            {
                                perror("Error executing file");
                            }
                        }
                        else
                        {
                            wait(NULL);
                        }
                    }
                    int fd = open(e_special, O_RDONLY, 0644);
                    if (fd == -1)
                    {
                        fprintf(stderr, "%s", RED);
                        perror("Error opening the file");
                        fprintf(stderr, "%s", RESET);
                    }
                    char buffer[61500];
                    memset(buffer, '\0', sizeof(buffer));
                    int size_read = read(fd, buffer, sizeof(buffer));
                    buffer[size_read] = '\0';
                    printf("%s", buffer);
                }
                else
                {
                    return;
                }
            }
        }
        else
        {
            perror("stat");
        }
    }
    chdir(from_called);
}