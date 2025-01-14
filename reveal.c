#include "headers.h"

int cmpfunc(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");

    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int is_executable(mode_t mode)
{
    return (mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH);
}

void printing_details(char *dir, int a, int l)
{
    struct dirent *entry;
    DIR *dirp = opendir(dir);
    if (dirp == NULL)
    {
        printf("%sINVALID DIRECTORY\n%s", RED, RESET);
        return;
    }
    char *file_names[1024];
    int count = 0;
    struct stat file_stat2;
    if (lstat(dir, &file_stat2) == -1)
    {
        perror("stat");
        return;
    }
    long total_blocks = 0; // Variable to store total blocks
    while ((entry = readdir(dirp)))
    {
        char file_path[4097];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir, entry->d_name);
        struct stat file_stat;

        if (lstat(file_path, &file_stat) == -1)
        {
            perror("stat");
            continue;
        }

        // Accumulate total blocks in 512-byte units

        file_names[count] = (char *)malloc(strlen(entry->d_name) + 1);
        strcpy(file_names[count], entry->d_name);
        count++;
        if (a == 0 && file_names[count - 1][0] == '.')
        {
            continue;
        }
        else
            total_blocks += file_stat.st_blocks;
    }
    printf("TOTAL: %5ld\n", total_blocks / 2);
    qsort(file_names, count, sizeof(char *), cmpfunc);
    int printi = 0;
    for (int i = 0; i < count; i++)
    {
        if (a == 0 && file_names[i][0] == '.')
        {
            free(file_names[i]);
            continue;
        }
        else
        {
            if (printi == 0)
            {
                printi = 1;
            }
            char file_path[4097];
            memset(file_path, '\0', sizeof(file_path));
            snprintf(file_path, sizeof(file_path), "%s/%s", dir, file_names[i]);
            struct stat file_stat;
            if (stat(file_path, &file_stat) == 0)
            {

                if (l == 1)
                {
                    print_permissions(file_stat.st_mode);
                    printf("\t:");
                    struct passwd *pw = getpwuid(file_stat.st_uid);
                    if (!pw)
                    {
                        perror("getpwuid");
                        return;
                    }

                    // Get group information
                    struct group *gr = getgrgid(file_stat.st_gid);
                    if (!gr)
                    {
                        perror("getgrgid");
                        return;
                    }

                    // Print username (owner)
                    printf(" %20s  %20s  %20ld\t", pw->pw_name, gr->gr_name, file_stat.st_size);
                    char time_str[100];
                    struct tm *time_info = localtime(&file_stat.st_mtime);
                    strftime(time_str, sizeof(time_str), "%b %d %H:%M:%S %Y", time_info);
                    printf(" %s\t", time_str);
                }
                if (S_ISDIR(file_stat.st_mode))
                {
                    printf("%s%s%s\n", BLUE, file_names[i], RESET);
                }
                else if (is_executable(file_stat.st_mode))
                {
                    printf("%s%s%s\n", GREEN, file_names[i], RESET);
                }
                else if (S_ISREG(file_stat.st_mode))
                {
                    printf("%s%s%s\n", WHITE, file_names[i], RESET);
                }
            }
            else
            {
                perror("stat");
            }
        }
        free(file_names[i]);
    }
}

void reveal(char *instr)
{
    int a = 0, l = 0;
    int len = strlen(instr);
    for (int i = 0; i < len; i++)
    {
        if (instr[i] == ' ' || instr[i] == '\0')
            continue;
        else if (instr[i] == '-')
        {
            if (i + 1 < len)
            {
                if (instr[i + 1] == ' ')
                {
                    i++;
                    while (i < len)
                    {
                        if (instr[i] == ' ')
                        {
                            i++;
                        }
                        else
                        {
                            printf("%sINVALID COMMAND GIVEN%s\n", RED, RESET);
                            return;
                        }
                    }
                    goto prev;
                }
                else
                {
                    i++;
                    char token[4097];
                    strcpy(token, instr + i);
                    strtok(token, " ");
                    i += strlen(token);
                    int k = 0;
                    while (k < strlen(token))
                    {
                        if (token[k] == 'a')
                            a = 1;
                        else if (token[k] == 'l')
                        {
                            l = 1;
                        }
                        else
                        {
                            printf("%sINVALID FLAGS GIVEN\n%s", RED, RESET);
                            return;
                        }
                        k++;
                    }
                }
            }
            else
            {
            prev:
                if (strcmp(prev_dir, "") != 0)
                {
                    char dir[4097];
                    memset(dir, '\0', sizeof(dir));
                    strcpy(prev_dir, dir);
                    printing_details(dir, a, l);
                }
                else
                {
                    printf("%sNO PREVIOUS DIRECTORY%s", RED, RESET);
                    return;
                }
            }
        }
        else if (instr[i] == '.')
        {
            if (i + 1 < len && instr[i + 1] == '.')
            {
                char arr[4097];
                memset(arr, '\0', sizeof(arr));
                getcwd(arr, sizeof(arr));
                char hard_test[4097] = "/";
                if (strcmp(arr, hard_test) == 0)
                {
                    printf("%sYOU ARE ALREADY AT THE END OF SYSTEM%s\n", RED, RESET);
                    // strcpy(prev_dir,prev_before);
                    // chdir(before);
                    return;
                }
                for (int i = strlen(arr) - 1; i >= 0; i--)
                {
                    if (arr[i] == '/')
                    {
                        // strcpy(prev_dir, arr);
                        strncpy(arr, arr, i);
                        arr[i] = '\0';
                        printing_details(arr, a, l);
                        return;
                    }
                }
            }
            else if (i + 1 == len)
            {
                char arr[4097];
                memset(arr, '\0', sizeof(arr));
                getcwd(arr, sizeof(arr));
                printing_details(arr, a, l);
            }
            else if (i + 1 < len)
            {
                for (int j = i + 1; j < len; j++)
                {
                    if (instr[j] == ' ' || instr[j] == '\0')
                    {
                        continue;
                    }
                    else
                    {
                        printf("%sINVALID DIRECTORY%s\n", RED, RESET);
                        return;
                    }
                    char arr[4097];
                    memset(arr, '\0', sizeof(arr));
                    getcwd(arr, sizeof(arr));
                    printing_details(arr, a, l);
                }
            }
        }
        else if (instr[i] == '~')
        {
            printing_details(home, a, l);
        }
        else
        {
            char *token;
            token = strtok(instr + i, " ");

            i += strlen(token);
            for (i; i < len; i++)
            {
                if (instr[i] == ' ' || instr[i] == '\0')
                    continue;
                else
                {
                    printf("%sINVALID DIRECTORY%s\n", RED, RESET);
                    return;
                }
            }
            char arr2[4097];
            strcpy(arr2, token);
            if (token[0] != '/')
            {
                memmove(arr2 + 1, arr2, strlen(arr2));
                arr2[0] = '/';
            }
            char arr[4097];
            memset(arr, '\0', sizeof(arr));
            getcwd(arr, sizeof(arr));
            strcat(arr, arr2);
            
            DIR *dirp = opendir(arr);
            if (dirp == NULL)
            {
                dirp = opendir(arr2);
                if (dirp == NULL)
                {
                    // means both were not directory so either it can be abs path or rel path
                    // if rel path
                    struct stat file_stat;
                    if (stat(arr, &file_stat) == -1)
                    {
                        if (stat(arr2, &file_stat) == -1)
                        {
                            printf("%sINVALID DIRECTORY OR FILE\n%s", RED, RESET);
                            return;
                        }
                        if (S_ISREG(file_stat.st_mode) || is_executable(file_stat.st_mode))
                        {
                            if (l == 1)
                            {
                                print_permissions(file_stat.st_mode);
                                printf("\t:");
                                struct passwd *pw = getpwuid(file_stat.st_uid);
                                if (!pw)
                                {
                                    perror("getpwuid");
                                    return;
                                }

                                // Get group information
                                struct group *gr = getgrgid(file_stat.st_gid);
                                if (!gr)
                                {
                                    perror("getgrgid");
                                    return;
                                }

                                // Print username (owner)
                                printf(" %20s  %20s  %20ld\t", pw->pw_name, gr->gr_name, file_stat.st_size);
                                char time_str[100];
                                struct tm *time_info = localtime(&file_stat.st_mtime);
                                strftime(time_str, sizeof(time_str), "%b %d %H:%M:%S %Y", time_info);
                                printf(" %s\t", time_str);
                            }
                            if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("%s%s%s\n", BLUE, arr2, RESET);
                            }
                            else if (is_executable(file_stat.st_mode))
                            {
                                printf("%s%s%s\n", GREEN, arr2, RESET);
                            }
                            else if (S_ISREG(file_stat.st_mode))
                            {
                                printf("%s%s%s\n", WHITE, arr2, RESET);
                            }
                            return;
                        }
                    }
                    memmove(arr2 , arr2+1, strlen(arr2));
                    if (l == 1)
                            {
                                print_permissions(file_stat.st_mode);
                                printf("\t:");
                                struct passwd *pw = getpwuid(file_stat.st_uid);
                                if (!pw)
                                {
                                    perror("getpwuid");
                                    return;
                                }

                                // Get group information
                                struct group *gr = getgrgid(file_stat.st_gid);
                                if (!gr)
                                {
                                    perror("getgrgid");
                                    return;
                                }

                                // Print username (owner)
                                printf(" %20s  %20s  %20ld\t", pw->pw_name, gr->gr_name, file_stat.st_size);
                                char time_str[100];
                                struct tm *time_info = localtime(&file_stat.st_mtime);
                                strftime(time_str, sizeof(time_str), "%b %d %H:%M:%S %Y", time_info);
                                printf(" %s\t", time_str);
                            }
                            if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("%s%s%s\n", BLUE, arr2, RESET);
                            }
                            else if (is_executable(file_stat.st_mode))
                            {
                                printf("%s%s%s\n", GREEN, arr2, RESET);
                            }
                            else if (S_ISREG(file_stat.st_mode))
                            {
                                printf("%s%s%s\n", WHITE, arr2, RESET);
                            }
                            return;
                }
                else
                {
                    printing_details(arr2, a, l);
                    return;
                }
            }
            printing_details(arr, a, l);
            return;
        }
    }
    char arr[4097];
    memset(arr, '\0', sizeof(arr));
    getcwd(arr, sizeof(arr));
    printing_details(arr, a, l);
}
