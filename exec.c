#include "exec.h"

char p_arg[100];
int time1=0,countt=0;
Proc P[50];
int PPid;

#define MAX_COMMAND_LENGTH 1024
#define MAX_PROCESS_NAME 200

char **proc;


int hash(int pid)
{
    int i = 0;
    int x;
    while (pid != 0)
    {
        x = pid % 10;
        i = i + x * x;
        pid = pid / 10;
    }
    return i;
}

void storecomm(char cmd[], int pid)
{
    if (proc == NULL)
    {
        proc = (char **)malloc(1000 * sizeof(char *));
        for (int i = 0; i < 1000; i++)
            proc[i] = (char *)malloc(200 * sizeof(char));
    }

    strcpy(proc[hash(pid)], cmd);
}

void check_background_processes()
{
    int status;
    pid_t pid;
    char *procname;
    // printf("12\n");
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {  
        //  printf("23\n");
        if (WIFEXITED(status))
        {   
            // printf("54\n");
            for(int i=0;i<countt;i++){
                if(P[i].pid==pid)
                P[i].status=0;
            }
            printf("Background process '%s' with PID %d ended normally.\n", proc[hash(pid)], pid);
        }
        else if (WIFSIGNALED(status))
        {
            for(int i=0;i<countt;i++){
                if(P[i].pid==pid)
                P[i].status=0;
            }
            printf("Background process with PID %d ended abnormally.\n", pid);
        }
    }
}


int exec(char command[], int amp)
{

    // Remove newline character from command
    size_t len = strlen(command);
if (len > 0 && command[len - 1] == '\n') {
    command[len - 1] = '\0'; // Remove the trailing newline character
}

// command[strcspn(command, "\n")] = '\0';

    // printf("%s\n",command);

    // Check if the command is for background execution
    int background = 0;
    if (amp == 1)
    {
        background = 1;
        // command[strlen(command) - 1] = '\0'; // Remove '&'
    }

    // Get current time before executing the command
    int x=0;
    for(int i=0;i<strlen(command);i++){
        if(command[i]!=' ' )
        {x=1;
        break;}
    }
    // printf("12\n");
    if(x==1){
    time_t start_time;
    time(&start_time);

    pid_t pid = fork();
    if (pid == 0)
    {

        // int pid=getpid();
        // int Gpid=getpgid(pid);

        // printf("%d %d\n",pid,Gpid);
        // Child process
        char *args[MAX_COMMAND_LENGTH];
        char *token = strtok(command, " ");
        int arg_count = 0;
        while (token != NULL)
        {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        execvp(args[0], args);
        printf("ERROR: '%s' is not valid command\n",command);
        return 0;
        
    }
    else if (pid > 0)
    {
        // Parent process
        if (!background)
        {
            P[countt].pid=pid;
                    P[countt].status=0;
                    P[countt].bg=0;
                    strcpy(P[countt++].command,command);
            // Foreground process
            // printf("%d\n",pid);
                PPid=pid;
                // printf("%d\n",PPid);
                
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                time_t end_time;
                  
                time(&end_time);

                double time_taken = difftime(end_time, start_time);
                if (time_taken > 2)
                {
                    // printf("Foreground process '%s' took %.0lf seconds.\n", command, time_taken);
                    // int fd = open("/home/jayanth/Code/OSN_proj/mini-project-1-jayanth260/foreground.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    // if (fd == -1)
                    // {
                    //     perror("Failed to open the file");
                    //     return 1;
                    // }
                    // int length=strlen(command);
                    // char buffer[20];
                    // snprintf(buffer, sizeof(buffer), "%c", time_taken);

                    // ssize_t bytesWritten = write(fd, command, length);
                    // if (bytesWritten != length)
                    // {
                    //     perror("Error writing to the file");
                    //     close(fd);
                    //     return 0;
                    // }

                    // close(fd);

                    strcpy(p_arg,command);
                    
                    
                    time1=time_taken;
                }
            }
        }
        else
        {
            // Background process
            printf("Background process %s started with PID: %d\n", command,pid);
            storecomm(command,pid);
            P[countt].pid=pid;
            P[countt].bg=1;
            P[countt].status=1;
            strcpy(P[countt++].command,command);
        }
    }
    else
    {
        perror("fork");
        return 0;
    }}
    // printf("13\n");

    return 1;
}


int activities() {
    
    for(int i=0;i< countt;i++){
        char* temp;
        if(P[i].status==0)
        temp="Stopped";
        else
        temp = "Running";

        // char* B;
        // if(P[i].bg==1)
        // B="bg";
        // else
        // B="fg";

    printf("%d : %s - %s \n",P[i].pid,P[i].command,temp);}
    return 1;
}








