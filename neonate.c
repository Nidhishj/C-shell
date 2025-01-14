#include "headers.h"
#include <fcntl.h>

#define PROC_DIR "/proc"

// Function to get the most recently created process PID
pid_t get_most_recent_pid() {
    DIR *dir;
    struct dirent *entry;
    pid_t most_recent_pid = 0;
    time_t most_recent_time = 0;

    dir = opendir(PROC_DIR);
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Check if the directory name is a number (i.e., a PID)
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            char path[256];
            struct stat statbuf;
            snprintf(path, sizeof(path), PROC_DIR "/%s/stat", entry->d_name);
            if (stat(path, &statbuf) == 0) {
                // Check if this PID is the most recent one
                if (statbuf.st_ctime > most_recent_time) {
                    most_recent_time = statbuf.st_ctime;
                    most_recent_pid = atoi(entry->d_name);
                }
            }
        }
    }

    closedir(dir);
    return most_recent_pid;
}

// Function to set terminal to non-blocking mode
void set_terminal_mode(int fd, struct termios *oldtio) {
    struct termios newtio;

    tcgetattr(fd, oldtio); // Get current terminal attributes
    newtio = *oldtio; // Copy to new attributes
    newtio.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(fd, TCSANOW, &newtio); // Set new attributes
}

// Function to restore terminal to original mode
void restore_terminal_mode(int fd, struct termios *oldtio) {
    tcsetattr(fd, TCSANOW, oldtio); // Restore original attributes
}

// Function to set stdin to non-blocking mode
void set_non_blocking_mode(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void set_blocking_mode(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    flags &= ~O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}

void neonate(char** instr, int k) {
    if (k != 3) {
        printf("INVALID INPUT\n");
        return;
    }

    int time_arg = atoi(instr[2]);
    if (time_arg <= 0) {
        fprintf(stderr, "Invalid time argument. Must be a positive integer.\n");
        return;
    }

    // Set terminal to non-blocking mode
    struct termios oldtio;
    set_terminal_mode(STDIN_FILENO, &oldtio);
    set_non_blocking_mode(STDIN_FILENO);

    struct timeval start_time, current_time;
    gettimeofday(&start_time, NULL);

    int stop = 0;
    while (!stop) {
        // Check if 'x' was pressed
        char input_char = fgetc(stdin);
        if (input_char == 'x') {
            stop = 1;
        }

        // Get current time and calculate elapsed time
        gettimeofday(&current_time, NULL);
        long elapsed_time = (current_time.tv_sec - start_time.tv_sec);

        // Perform computation only if elapsed time >= time_arg
        if (elapsed_time >= time_arg) {
            pid_t recent_pid = get_most_recent_pid();
            if (recent_pid != -1) {
                printf("%d\n", recent_pid);
            } else {
                printf("Error retrieving PID\n");
            }

            // Update start time for the next interval
            start_time = current_time;
        }

        // Sleep for a short period to reduce CPU usage
        usleep(100000); // 100 milliseconds
    }

    // Restore terminal settings before exiting
    restore_terminal_mode(STDIN_FILENO, &oldtio);
    set_blocking_mode(STDIN_FILENO);
    // printf("Stopping...\n");
    return;
}
