#include "headers.h"

void sigchld_handler(int signo)
{
    (void)signo; // To avoid unused parameter warning

    if (signal == SIGKILL && pig == -1)
    {
        return;
    }
    // De current stdout
    int abc = dup(STDOUT_FILENO);
    if (abc < 0)
    {
        perror("dup");
        return;
    }

    if (dup2(terminal_output, STDOUT_FILENO) < 0)
    {
        perror("dup2");
        close(abc);
        return;
    }
    int status;
    pid_t pid;

    // Use waitpid with WNOHANG to check for any terminated child processes
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // Find the corresponding background process
        for (int i = 0; i < num_background_pids; i++)
        {
            if (bg_processes[i].pid == pid)
            {
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
                {
                    printf("%s exited abnormally (%d) \n", bg_processes[i].command, pid);
                }
                else if (WIFSIGNALED(status))
                {
                    printf("%s exited abnormally (%d)\n", bg_processes[i].command, bg_processes[i].pid);
                }
                else if (WIFEXITED(status))
                {
                    bg_processes[i].stat = 1;
                    printf("%s exited normally (%d) \n", bg_processes[i].command, pid);
                }
                else
                {
                    printf("%s exited abnormally (%d)\n", bg_processes[i].command, pid);
                }

                // Remove the process from the array
                for (int j = i; j < num_background_pids - 1; j++)
                {
                    bg_processes[j] = bg_processes[j + 1];
                }
                num_background_pids--;
                memset(&bg_processes[num_background_pids], 0, sizeof(bg_processes[0]));

                break;
            }
        }
    }
    if (dup2(abc, STDOUT_FILENO) < 0)
    {
        perror("dup2");
    }
    close(abc);
}

void system_commands(char *command, char **args, int h, char *logy)
{
    int p = dup(STDOUT_FILENO);
    if (h >= 2 && (strcmp(args[h - 2], "&") == 0))
    {
        args[h - 2] = NULL;
        int pid_bg = fork();
        if (pid_bg < 0)
        {
            perror("FORK");
            return;
        }
        else if (pid_bg == 0)
        {   setpgid(0,0);
            if (execvp(command, args) == -1)
            {
                perror("INCORRECT");
                exit(1);
            }
        }
        else
        {
            if (num_background_pids < 4096)
            {
                int l = dup2(terminal_output, STDOUT_FILENO);
                if (l < 0)
                {
                    perror("dup2");
                }
                printf("%d\n", pid_bg);
                dup2(p, STDOUT_FILENO);
                bg_processes[num_background_pids].pid = pid_bg;
                strncpy(bg_processes[num_background_pids].command, command, sizeof(bg_processes[num_background_pids].command) - 1);
                bg_processes[num_background_pids].stat = 0;
                // strncpy(bg_processes[num_background_pids].command,comm)
                bg_processes[num_background_pids].command[sizeof(bg_processes[num_background_pids].command) - 1] = '\0';
                num_background_pids++;
            }
            else
            {
                printf("No more background processes can be stored  \n");
            }
        }
    }
    else if (h >= 2 && args[h - 2][strlen(args[h - 2]) - 1] == '&')
    {
        args[h - 2][strlen(args[h - 2]) - 1] = '\0';
        int pid_bg2 = fork();

        if (pid_bg2 < 0)
        {
            perror("FORK");
            exit(127);
        }
        else if (pid_bg2 == 0)
        {   setpgid(0,0);
            if (execvp(command, args) == -1)
            {
                perror("INCORRECT");
                exit(1);
            }
        }
        else
        {
            if (num_background_pids < 4096)
            {
                int l = dup2(terminal_output, STDOUT_FILENO);
                if (l < 0)
                {
                    perror("dup2");
                }
                printf("%d\n", pid_bg2);
                dup2(p, STDOUT_FILENO);
                bg_processes[num_background_pids].pid = pid_bg2;
                strncpy(bg_processes[num_background_pids].command, command, sizeof(bg_processes[num_background_pids].command) - 1);
                bg_processes[num_background_pids].command[sizeof(bg_processes[num_background_pids].command) - 1] = '\0';
                num_background_pids++;
            }
            else
            {
                printf("No more background processes can be stored  \n");
            }
        }
        return;
    }
    else
    {
        // pid_t pid_fg;
        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);
        memset(fg_process, '\0', sizeof(fg_process));
        strcat(fg_process, args[0]);
        int last_fg2 = fork();
        if (last_fg2 < 0)
        {
            perror("fork");
            return;
        }
        else if (last_fg2 == 0)
        {            
            setpgid(0,0);
            if (execvp(command, args) == -1)
            {
                printf("%sERROR : '%s' is not a valid command\n%s", RED, command, RESET);
                exit(1);
            }
        }
        else
        {
            int status;
            last_fg = last_fg2;
            waitpid(last_fg, &status, WUNTRACED);
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
            if (strcmp(last_long, logy) != 0)
            {
                memset(last_long, '\0', sizeof(last_long));
                if (time_taken_temp > 2)
                {
                    time_taken = time_taken_temp;
                    strcpy(last_long, logy);
                }
                last_fg = -1;
            }
        }
    }
}
