#include "headers.h"

char home[4097];
char prev_dir[4097];
int log_num = -1;
int log_execute = 0;

const char *YELLOW = "\033[0;33m";
const char *GREEN = "\033[0;32m";
const char *RESET = "\033[0m";
const char *RED = "\033[0;31m";
const char *WHITE = "\033[0;37m";
const char *BLUE = "\033[0;34m";
int time_taken = -1;
char last_long[4096];
int num_background_pids = 0;
BackgroundProcess bg_processes[4096];
int terminal_input;
int terminal_output;
int num_of_strings = 0;

void tokenizing_first(char *instr, char **instr_strings) {
    terminal_input = dup(STDIN_FILENO);
    if (terminal_input < 0) {
        perror("dup");
    }
    terminal_output = dup(STDOUT_FILENO);
    if (terminal_output < 0) {
        perror("dup");
    }
    char *token = strtok(instr, ";");
    while (token != NULL) {
        if (token != NULL) {
            instr_strings[num_of_strings] = (char *)malloc(sizeof(char) * strlen(token) + 1);
            strcpy(instr_strings[num_of_strings], token);
            num_of_strings++;
        }
        token = strtok(NULL, ";");
    }
}

int no_process = 0;
int last_fg = -1;
char fg_process[4096];
void handle_ctrl_d() {
    for (int i = 0; i < num_background_pids; i++) {
        kill(bg_processes[i].pid, SIGTERM); // Send termination signal to background processes
    }
    exit(0); // Exit the shell
}

int sigint=-1;
void sigint_handler(int sig) {
    printf("\n");
    if(last_fg==-1)
    {
        display();
        fflush(stdout);
        return;
    }
    if (last_fg != -1) {
        if (kill(last_fg, SIGINT) < 0) {
            perror("kill");
        }
        last_fg = -1;  
        return;// Reset last_fg to handle subsequent Ctrl-C signals
    }
}
void sigstop_handler(int sig) {
    // printf("%d\n",last_fg);
    printf("\n");
    if(last_fg==-1)
    {
        display();
        fflush(stdout);
        return;
    }
    if (last_fg != -1) {
            //    fflush(stdout);
        kill(last_fg,SIGTSTP);
        bg_processes[num_background_pids].pid=last_fg;
        strcpy(bg_processes[num_background_pids].command,fg_process);
        bg_processes[num_background_pids].stat=-1;
        num_background_pids++;
        last_fg=-1;
        return;// Reset last_fg to handle subsequent Ctrl-C signal  s
    }
}

alias alias_array[100];
int alias_count;
void alias_handler()
{
    FILE *file = fopen("myshrc.sh", "r");
    if (!file) {
        printf("Failed to open .shrc file.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "alias", 5) == 0) {
            char name[50], command[100];
            sscanf(line, "alias %s = %[^\n]", name, command);
            if (alias_count < 100) {
        strcpy(alias_array[alias_count].name, name);
        strcpy(alias_array[alias_count].command, command);
        alias_count++;
    } else {
        printf("Alias limit reached!\n");
    }
        }
    }

    fclose(file);
}   

int pig =-1;
int main() {
    alias_count=0;
    alias_handler();
    memset(fg_process,'\0',sizeof(fg_process));
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigemptyset(&sa.sa_mask);
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    // signal(SIGCHLD,sigchld_handler);
    signal(SIGINT,sigint_handler);
    signal(SIGTSTP,sigstop_handler);
    int fd = open("log.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        fprintf(stderr, "%s", RED);
        perror("open");
        fprintf(stderr, "%s", RESET);
    }
    close(fd);
    memset(home, '\0', sizeof(home));
    getcwd(home, sizeof(home)); // Store the current working directory
    memset(prev_dir, '\0', sizeof(prev_dir));
    while (1) {
        char arr[10000];
        memset(arr, '\0', sizeof(arr));
        display();
        if (fgets(arr, 10000, stdin) == NULL) {
            printf("\n");   
           handle_ctrl_d();
           exit(0);
        }
        arr[strlen(arr) - 1] = '\0';
        int l = strlen(arr);
        char brr[10000];
        memset(brr, '\0', sizeof(brr));
        strcpy(brr, arr);
        int k = 0;
        int new_length = l;
        for (int i = 0; i < l; i++) {
            if (brr[i] == '&') {
                new_length++;
            }
        }
        int j = 0; // Index for the new string
        for (int i = 0; i < l; i++) {
            arr[j++] = brr[i];
            if (brr[i] == '&') {
                arr[j++] = ';';
            }
        }
        arr[j] = '\0';
        l = strlen(arr);
        if (strstr(arr, ";;") != NULL) {
            printf("%sINVALID INPUT%s\n", RED, RESET);
            continue;
        }
        char logy[4096];
        char *diff_token_strings[4096];
        tokenizing_first(arr, diff_token_strings);
        for (int i = 0; i < num_of_strings; i++) {
            call_other(diff_token_strings[i], brr);
        }
        num_of_strings = 0;
        if (log_num == 1) {
            log_num = -1;
            continue;
        } else {
            char arri[4097];
            memset(arri, '\0', sizeof(arri));
            strcpy(arri, "log change");
            call_other(arri, brr);
            log_num = -1;
        }
    }
}
