#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>

extern char home[4097];
extern char prev_dir[4097];
extern int log_num;
extern int log_execute;
extern int terminal_input;
extern int terminal_output;
extern const char *GREEN;
extern const char *RESET;
extern const char *RED;
extern const char *YELLOW;
extern const char *WHITE;
extern const char *BLUE;
extern int time_taken;
extern char last_long[4096];
extern int num_background_pids;
typedef struct {
    pid_t pid;
    char command[256]; 
    int stat;
} BackgroundProcess;
typedef struct {
    char name[50];
    char command[100];
}alias;
extern BackgroundProcess bg_processes[4096];
extern struct sigaction sa;
extern int no_process;
extern int last_fg;
extern char fg_process[4096];
extern int pig;
extern int alias_count;
extern alias alias_array[100];

#include "display.h"
#include "hop.h"
#include "calling_comm.h"
#include "log.h"
#include "reveal.h"
#include "proclore.h"
#include "seek.h"
#include "system_commands.h"
#include "i_o_redirect.h"
#include "activities.h"
#include "ping.h"
#include "bg_fg.h"
#include "neonate.h"
#include "iman.h"
#endif