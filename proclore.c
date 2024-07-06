#include "proclore.h"

#define MAX_PATH_LENGTH 256
#define MAX_CMDLINE_LENGTH 2048

void print_process_info(const char *pid) {
    char proc_path[MAX_PATH_LENGTH];
    snprintf(proc_path, sizeof(proc_path), "/proc/%s", pid);

    // Print the process PID
    printf("PID: %s\n", pid);

    // Read the cmdline file to get the executable path
    char cmdline_path[MAX_PATH_LENGTH];
    snprintf(cmdline_path, sizeof(cmdline_path), "%s/cmdline", proc_path);
    
    int cmdline_fd = open(cmdline_path, O_RDONLY);
    if (cmdline_fd != -1) {
        char cmdline[MAX_CMDLINE_LENGTH];
        ssize_t bytesRead = read(cmdline_fd, cmdline, sizeof(cmdline));
        close(cmdline_fd);

        // Null-terminate the cmdline
        cmdline[bytesRead] = '\0';

        // Print the executable path
        printf("Executable Path: %s\n", cmdline);
    }

    // Read the status file to get process status and virtual memory
    char status_path[MAX_PATH_LENGTH];
    snprintf(status_path, sizeof(status_path), "%s/status", proc_path);

    FILE *status_file = fopen(status_path, "r");
    if (status_file != NULL) {
        char status_line[MAX_CMDLINE_LENGTH];
        while (fgets(status_line, sizeof(status_line), status_file) != NULL) {
            if (strstr(status_line, "State:") == status_line) {
                char state;
                sscanf(status_line, "State:\t%c", &state);
                printf("Process Status: %c", state);

                // Check if the process is foreground or background
                if (state == 'R' || state == 'S' || state == 'Z') {
                    char fd_path[MAX_PATH_LENGTH];
                    snprintf(fd_path, sizeof(fd_path), "%s/fd/0", proc_path);
                    if (access(fd_path, F_OK) == 0) {
                        printf("+");
                    }
                }
                printf("\n");
            } else if (strstr(status_line, "VmSize:") == status_line) {
                unsigned long vm_size;
                sscanf(status_line, "VmSize:\t%lu kB", &vm_size);
                printf("Virtual Memory: %lu kB\n", vm_size);
            }
        }
        fclose(status_file);
    }

    // Read the stat file to get process group
    char stat_path[MAX_PATH_LENGTH];
    snprintf(stat_path, sizeof(stat_path), "%s/stat", proc_path);

    FILE *stat_file = fopen(stat_path, "r");
    if (stat_file != NULL) {
        int pgrp;
        fscanf(stat_file, "%*d %*s %*c %d", &pgrp);
        printf("Process Group: %d\n", pgrp);
        fclose(stat_file);
    }
}

int proclore(int argc, char *argv[]) {
    if (argc < 2) {
        // If no argument provided, print information about the shell itself
        char shell_pid[10];
        snprintf(shell_pid, sizeof(shell_pid), "%d", getpid());
        print_process_info(shell_pid);
    } else {
        // Print information about the specified process(es)
        for (int i = 1; i < argc; i++) {
            print_process_info(argv[i]);
            printf("\n");
        }
    }

    return 1;
}
