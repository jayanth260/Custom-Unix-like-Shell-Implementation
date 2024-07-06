#include "pipe.h"

// int ccc=0;

char **arg(char args[])
{
    char **argss = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
        argss[i] = (char *)malloc(20 * sizeof(char));

    char *token = strtok(args, " ");
    int arg_count = 0;
    while (token != NULL)
    {
        argss[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ");
    }
    argss[arg_count] = NULL;

    return argss;
}

int create_child(int depth, int n, int *pipe1, char *args[])
{
    // char *commands[n][10];
    // for (int i = 0; i < n; i++)
    // {
    //     char *token = strtok(args[i], " ");
    //     int arg_count = 0;
    //     while (token != NULL)
    //     {
    //         commands[i][arg_count] = token;
    //         arg_count++;
    //         token = strtok(NULL, " ");
    //     }
    //     commands[i][arg_count] = NULL;

    //     printf("%s\n", commands[i][0]);
    // }
    // int n = 3; // Number of commands in the pipeline
    int pipes[n - 1][2];
    pid_t pid[n];

    // Create pipes
    for (int i = 0; i < n - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            exit(1);
        }
    }

    // Fork and execute commands
    for (int i = 0; i < n; i++)
    {
        pid[i] = fork();
        if (pid[i] == -1)
        {
            perror("fork");
            exit(1);
        }

        if (pid[i] == 0)
        { // Child process
            // Set up input and output redirection for commands
            if (i == 0)
            {
                // First command reads from stdin, so no input redirection needed
                dup2(pipes[i][1], STDOUT_FILENO); // Redirect output to the next command
            }
            else if (i == n - 1)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO); // Redirect input from the previous command
                // Last command writes to stdout, so no output redirection needed
            }
            else
            {
                dup2(pipes[i - 1][0], STDIN_FILENO); // Redirect input from the previous command
                dup2(pipes[i][1], STDOUT_FILENO);    // Redirect output to the next command
            }

            // Close all unused file descriptors
            for (int j = 0; j < n - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            int inp = dup(STDIN_FILENO);
    int out = dup(STDOUT_FILENO);

    char **arg = check1(args[i]);
    int len = strlen(arg[0]);
    arg[0][len] = ' ';
    arg[0][len + 1] = '\0';
    char *commands[10];

    char* tempp=(char**)malloc(100*sizeof(char));
    strcpy(tempp,arg[0]);
    char *token = strtok(arg[0], " ");
    int arg_count = 0;

    while (token != NULL) {
        commands[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    commands[arg_count] = NULL;
    int k = 1;
    int inputFile = -1;
    int outputFile = -1;

    if (strstr(arg[3], "<")) {
        if (access(arg[k], F_OK) == -1) {
            printf("No such input file found!\n");
            return 0;
        }

        inputFile = open(arg[k++], O_RDONLY);
        if (inputFile == -1) {
            perror("Failed to open input file");
            return 0;
        }

        close(STDIN_FILENO);
        dup2(inputFile, STDIN_FILENO);
    }

    mode_t fileMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // Permissions 0644

    if (strstr(arg[3], ">>")) {
        outputFile = open(arg[k], O_WRONLY | O_APPEND | O_CREAT, fileMode);
    } else if (strstr(arg[3], ">")) {
        outputFile = open(arg[k], O_WRONLY | O_TRUNC | O_CREAT, fileMode);
    }

    if (outputFile != -1) {
        close(STDOUT_FILENO);
        dup2(outputFile, STDOUT_FILENO);
        close(outputFile);
    }

    // Create a child process to execute the command
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Failed to fork");
        return 0;
    }

    if (child_pid == 0) {
        // This code runs in the child process
        if(check(commands[0])){
            cmd(tempp);
        }
        else{
        execvp(commands[0], commands);
        perror("execvp");
        exit(1);}
    }

    // Close the input and output redirection descriptors in the parent
    if (inputFile != -1) {
        close(inputFile);
    }
    if (outputFile != -1) {
        close(outputFile);
    }

    // Wait for the child process to finish
    int status;
    waitpid(child_pid, &status, 0);

    // Restore the original file descriptors in the parent process
    dup2(out, STDOUT_FILENO);
    dup2(inp, STDIN_FILENO);
    free(tempp);
            
            exit(0);
        
        }
    }

    // Close all pipe file descriptors in the parent
    for (int i = 0; i < n - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < n; i++)
    {
        waitpid(pid[i], NULL, 0);
    }

    return 1;
}

char **check1(char args[])
{
    int S = 0, G = 0, GG = 0;
    for (int i = 0; i < strlen(args); i++)
    {
        if (args[i] == '<')
        {
            S++;
            continue;
        }
        if (args[i] == '>' && args[i + 1] == '>')
        {
            GG++;
            i++;
            continue;
        }
        if (args[i] == '>')
            G++;
    }
    if (S > 1 || G + GG > 1)
    {
        printf("invalid I/O redirection\n");
        return NULL;
    }

    char **arg = (char **)malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++)
        arg[i] = (char *)malloc(100 * sizeof(char));
    int arg_count = 0;

    char *token = strtok(args, "><");
    while (token != NULL)
    {
        token = remove_extra_spaces(token);
        strcpy(arg[arg_count++], token);
        token = strtok(NULL, ">");
    }
    if (strlen(arg[arg_count - 1]) == 0)
    {
        printf("Invalid Output File\n");
        return NULL;
    }
    if (G == 1 && S == 1)
        arg[3] = "><";
    if (G == 1 && S == 0)
        arg[3] = ">";
    if (G == 0 && GG == 0 && S == 1)
        arg[3] = "<";
    if (GG == 1 && S == 1)
        arg[3] = ">><";
    if (GG == 1 && S == 0)
        arg[3] = ">>";

    return arg;
}

int manage_IO(char args[])
{
    int inp = dup(STDIN_FILENO);
    // printf("%d\n", STDIN_FILENO);
    int out = dup(STDOUT_FILENO);
    // printf("%d\n", STDOUT_FILENO);

    char **arg = check1(args);
    // for (int i = 0; i < 4; i++)
    //     printf("%s\n", arg[i]);
    int len = strlen(arg[0]);
    // printf("de%d\n",len);
    arg[0][len] = ' ';
    arg[0][len + 1] = '\0';
    char *commands[10];
    char* tempp=(char**)malloc(100*sizeof(char));
    strcpy(tempp,arg[0]);

    char *token = strtok(arg[0], " ");
    int arg_count = 0;
    while (token != NULL)
    {
        commands[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ");
    }
    commands[arg_count] = NULL;

    // printf("%s\n",commands[0]);

    // int len1=strlen(arg[0]);
    //     printf("sss%d\n",len1);
    //     arg[0][len1]='\0';

    int i = 1;
    int inputFile = -1; // Initialize to -1 to avoid closing an invalid file descriptor
    int outputFile = -1; // Initialize to -1 to avoid closing an invalid file descriptor

    if (strstr(arg[3], "<")) {
        if (access(arg[i], F_OK) == -1) {
            printf("No such input file found!\n");
            return 0; // Exit with an error code
        }

        inputFile = open(arg[i++], O_RDONLY);
        if (inputFile == -1) {
            perror("Failed to open input file");
            return 0; // Exit with an error code
        }

        close(STDIN_FILENO);
        dup2(inputFile, STDIN_FILENO);
    }

    mode_t fileMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // Permissions 0644

    if (strstr(arg[3], ">>")) {
        outputFile = open(arg[i], O_WRONLY | O_APPEND | O_CREAT, fileMode);
    } else if (strstr(arg[3], ">")) {
        outputFile = open(arg[i], O_WRONLY | O_TRUNC | O_CREAT, fileMode);
    }

    if (outputFile != -1) {
        close(STDOUT_FILENO);
        dup2(outputFile, STDOUT_FILENO);
        close(outputFile);
    }

    // Create a child process to execute the command
    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed to fork");
        return 0; // Exit with an error code
    }

    if (pid == 0) {
        // This code runs in the child process
        if(check(commands[0])){
            cmd(tempp);
        }
        else{
        execvp(commands[0], commands);
        perror("execvp");
        exit(1);}// Exit the child process with an error code
    } else {
        // This code runs in the parent process
        int status;
        waitpid(pid, &status, 0); // Wait for the child to finish
    }

    // Restore the original file descriptors in the parent process
    dup2(out, STDOUT_FILENO);
    dup2(inp, STDIN_FILENO);
    free(tempp);
    return 1;
}

int PIPE(char input[])
{

    // char input[200];
    // fgets(input, 200, stdin);

    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n')
    {
        input[input_length - 1] = '\0';
    }
    char *token = strtok(input, "|");

    char **args = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        args[i] = (char *)malloc(200 * sizeof(char));
    }
    int j = 0;
    while (token != NULL)
    {
        token = remove_extra_spaces(token);
        strcpy(args[j++], token);
        if (strlen(args[j - 1]) == 0)
        {
            printf("Invalid use of pipe\n");
            return 0;
        }
        // printf("%s\n", args[j - 1]);

        token = strtok(NULL, "|");
    }

    int n = j; // Change this to the desired number of commands
    // pid_t child_pids[n];

    // Create pipes

    // printf("%d\n", n);
    int s;

    if (n == 1)
    {

        s = manage_IO(args[0]);
    }
    else
        s = create_child(1, n, NULL, args);

    // Fork child processes and set up pipes

    // Close any remaining open pipe ends in the parent process

    // Wait for all child processes to finish
    // printf("hello\n");

    for (int i = 0; i < 10; i++)
        free(args[i]);
    free(args);

    return s;
}
