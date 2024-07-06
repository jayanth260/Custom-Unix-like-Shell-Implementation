#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char **parse_command(char *args) {
    char **command = (char **)malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++) {
        command[i] = (char *)malloc(100 * sizeof(char));
        command[i][0] = '\0'; // Initialize each element as an empty string
    }

    int S = 0, G = 0, GG = 0;

    for (int i = 0; i < strlen(args); i++) {
        if (args[i] == '<') {
            S++;
            continue;
        }
        if (args[i] == '>' && args[i + 1] == '>') {
            GG++;
            i++;
            continue;
        }
        if (args[i] == '>') {
            G++;
        }
    }

    if (S > 1 || G + GG > 1) {
        printf("Invalid I/O redirection\n");
        return NULL;
    }

    char *token = strtok(args, "><");
    int arg_count = 0;

    while (token != NULL) {
        // token = remove_extra_spaces(token);
        strcpy(command[arg_count++], token);
        token = strtok(NULL, "><");
    }

    if (strlen(command[arg_count - 1]) == 0) {
        printf("Invalid Output File\n");
        return NULL;
    }

    if (G == 1 && S == 1) {
        strcpy(command[3], "><");
    } else if (G == 1 && S == 0) {
        strcpy(command[3], ">");
    } else if (G == 0 && GG == 0 && S == 1) {
        strcpy(command[3], "<");
    } else if (GG == 1 && S == 1) {
        strcpy(command[3], ">><");
    } else if (GG == 1 && S == 0) {
        strcpy(command[3], ">>");
    }

    return command;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <command with I/O redirection>\n", argv[0]);
        return 1;
    }

    int inp = dup(STDIN_FILENO);
    int out = dup(STDOUT_FILENO);

    char *args = argv[1];
    char **command = parse_command(args);

    if (command == NULL) {
        return 1; // Invalid I/O redirection
    }

    int inputFile = -2;
    int outputFile = -2;

    if (strstr(command[3], "<")) {
        if (access(command[1], F_OK) == -1) {
            printf("No such input file found!\n");
            return 1; // Exit with an error code
        }

        inputFile = open(command[1], O_RDONLY);
        close(STDIN_FILENO);
        dup2(inputFile, STDIN_FILENO);
        close(inputFile);
    }

    mode_t fileMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // Permissions 0644

    if (strstr(command[3], ">>")) {
        if ((outputFile = open(command[1], O_WRONLY | O_APPEND | O_CREAT, fileMode)) == -1) {
            printf("Failed to open the output file!\n");
            return 1;
        }
        close(STDOUT_FILENO);
        dup2(outputFile, STDOUT_FILENO);
    } else if (strstr(command[3], ">")) {
        if ((outputFile = open(command[1], O_WRONLY | O_TRUNC | O_CREAT, fileMode)) == -1) {
            printf("Failed to open the output file!\n");
            return 1;
        }
        close(STDOUT_FILENO);
        dup2(outputFile, STDOUT_FILENO);
    }

    // Execute the command
    char *cmd = command[0];
    char *cmd_args[2];
    cmd_args[0] = cmd;
    cmd_args[1] = NULL;
    execvp(cmd, cmd_args);

    // Restore the original file descriptors
    close(outputFile);
    dup2(out, STDOUT_FILENO);
    close(inp);
    dup2(inp, STDIN_FILENO);

    return 0;
}
