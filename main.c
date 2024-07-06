#include "headers.h"

char shl[200];
pid_t pgid;

static sigjmp_buf jump_buffer;

char *remove_extra_spaces(char input[])
{
    size_t input_length = strlen(input);
    char *result = (char *)malloc((input_length + 1) * sizeof(char));
    if (result == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    bool space_seen = false; // Flag to track consecutive spaces
    size_t result_index = 0; // Index for the result string

    for (size_t i = 0; i < input_length; i++)
    {
        if ((input[i] == ' ' || input[i] == '\t'))
        {
            if (!space_seen)
            {
                if (result_index == 0)
                    continue;
                result[result_index++] = ' '; // Copy a single space
                space_seen = true;
            }
        }
        else
        {
            result[result_index++] = input[i]; // Copy non-space character
            space_seen = false;
        }
    }
    if (result[result_index - 1] == ' ' || result[result_index - 1] == '\t')
        result[result_index - 1] = '\0';
    else
        result[result_index] = '\0'; // Null-terminate the result string
    return result;
}

int runcmd(char *arg[], int n_args)
{
    if (!strcmp(arg[0], "warp"))
    {
        if (n_args == 0)
        {
            char *home = "~";
            int suc = warp(home, shl);
            if (suc == 0)
                return 0;
        }
        else
        {
            for (int j = 0; j < n_args; j++)
            {
                int suc = warp(arg[j + 1], shl);
                if (suc == 0)
                    return 0;
            }
            return 1;
        }
    }
    if (!strcmp(arg[0], "peek"))
    {
        char *path = ".";
        if (n_args == 0)
        {

            return peek(path, 0, 0, shl);
        }
        else if (n_args == 1)
        {
            if (!strcmp(arg[1], "-a"))
            {

                return peek(path, 1, 0, shl);
            }
            else if (!strcmp(arg[1], "-l"))
            {
                return peek(path, 0, 1, shl);
            }
            else if (!strcmp(arg[1], "-la") || !strcmp(arg[1], "-al"))
                return peek(path, 1, 1, shl);
            else if (strcmp(arg[1], "-a") && strcmp(arg[1], "-l"))
                return peek(arg[1], 0, 0, shl);
            else
                return peek(arg[1], 0, 0, shl);
        }
        else if (n_args == 2)
        {
            if ((!strcmp(arg[1], "-a") && !strcmp(arg[2], "-l")) || (!strcmp(arg[2], "-a") && !strcmp(arg[1], "-l")))
                return peek(path, 1, 1, shl);
            else if (!strcmp(arg[1], "-la") || !strcmp(arg[1], "-al"))
                return peek(arg[2], 1, 1, shl);
            else if (!strcmp(arg[1], "-a") && strcmp(arg[2], "-l"))
                return peek(arg[2], 1, 0, shl);
            else if (!strcmp(arg[1], "-l") && strcmp(arg[1], "-a"))
                return peek(arg[2], 0, 1, shl);
            else
            {
                printf("Error: peek: not valid\n");
                return 0;
            }
        }
        else if (n_args == 3)
        {
            if ((!strcmp(arg[1], "-a") && !strcmp(arg[2], "-l")) || (!strcmp(arg[2], "-a") && !strcmp(arg[1], "-l")))
                return peek(arg[3], 1, 1, shl);
            else
            {
                printf("Error: peek: %s %s are not valid flags!\n", arg[1], arg[2]);

                return 0;
            }
        }
    }
    if (!strcmp(arg[0], "pastevents"))
    {
        char *temp;
        if (n_args == 0)
        {
            temp = "@";
            return pastevents(temp);
        }
        else if (n_args == 1)
        {
            if (!strcmp(arg[1], "purge"))
            {
                temp = "#";
                return pastevents(temp);
            }
            else
            {
                printf("Error: pastevents: '%s' is not valid!\n", arg[1]);
                return 0;
            }
        }
        else if (n_args == 2)
        {
            if (!strcmp(arg[1], "execute"))
                return pastevents(arg[2]);
            else
            {
                printf("ERROR: pasevents: '%s' is not valid!\n", arg[1]);
                return 0;
            }
        }
    }
    if (!strcmp(arg[0], "seek"))
    {
        return Seek_proc(n_args + 1, arg);
    }
    if (!strcmp(arg[0], "proclore"))
    {
        return proclore(n_args + 1, arg);
    }
    if (!strcmp(arg[0], "activities"))
    {
        return activities();
    }
    if (!strcmp(arg[0], "ping"))
    {
        if (n_args != 2)
        {
            printf(stderr, "Usage: %s <pid> <signal_number>\n", arg[0]);
            return 0;
        }
        return ping(I(arg[1]), I(arg[2]));
    }
    if (!strcmp(arg[0], "fg"))
    {
        if (n_args != 1)
        {
            printf("Usage: pg <pid> \n");
            return 0;
        }
        return fg(I(arg[1]));
    }
    if (!strcmp(arg[0], "bg"))
    {
        if (n_args != 1)
        {
            printf("Usage: bg <pid> \n");
            return 0;
        }
        return bg(I(arg[1]));
    }
    if (!strcmp(arg[0], "neonate"))
    {
        return neonate(n_args, arg);
    }
    if (!strcmp(arg[0], "iMan"))
    {
        return iMan(n_args, arg);
    }
}

int check(char cmd[])
{
    if (!strcmp(cmd, "EXIT"))
        exit(1);
    if (!strcmp(cmd, "pastevents") || !strcmp(cmd, "warp") || !strcmp(cmd, "peek") || !strcmp(cmd, "seek") || !strcmp(cmd, "proclore") || !strcmp(cmd, "activities") || !strcmp(cmd, "ping") || !strcmp(cmd, "fg") || !strcmp(cmd, "bg") || !strcmp(cmd, "neonate") || !strcmp(cmd, "iMan"))
        return 1;
    else
        return 0;
}

int run(char *arg[], int n, int x)
{
    int suc = 0;
    if (n == 1)
        return exec(arg[0], 1);
    for (int i = 0; i < n - 1; i++)
    {
        // printf("%s\n",arg[i]);
        suc = exec(arg[i], 1);
        if (suc == 0)
            return 0;
    }
    if (x == 0)
    {
        char *temp = (char *)malloc(200 * sizeof(char));
        strcpy(temp, arg[n - 1]);
        char *space = " ";
        char *token = strtok(temp, space);
        if (check(token))
        {
            char **arrg = (char **)malloc(10 * sizeof(char *));
            for (int i = 0; i < 10; i++)
                arrg[i] = (char *)malloc(100 * sizeof(char));
            int j = 0;
            while (token != NULL)
            {
                strcpy(arrg[j++], token);
                token = strtok(NULL, space);
            }
            suc = runcmd(arrg, j - 1);
            if (suc == 0)
                return 0;
            else if (suc == 10)
                return 10;
        }
        else
            return exec(arg[n - 1], 0);
    }
    else
        return exec(arg[n - 1], 1);
}

// int ampersand(char cmd[])
// {
//     char *saveptr2;
//     char *token_ampersand;
//     char *ampersand_delim = "&";
//     token_ampersand = strtok(cmd, ampersand_delim);
//     while (token_ampersand != NULL)
//     {
//         // printf("%s\n", token_ampersand);
//         token_ampersand = strtok(NULL, ampersand_delim);
//     }
//     return 1;
// }
void cmd(char input[])
{   
    int l=strlen(input);
    int v=0;
    for(int i=0;i<l;i++)
    {if(input[i]!=' ' && input[i]!='\n')
    v++;}
    if(v==0)
    return;
    int suc, x = 0;
    char *temp = (char *)malloc(200 * sizeof(char));
    strcpy(temp, remove_extra_spaces(input));
    // printf("%s\n", input);
    //  printf("43\n");
    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n')
    {
        input[input_length - 1] = '\0';
    }

    char *ampersand_delim = "&";
    char *semicolon_delim = ";";
    char *space_delim = " ";

    char *token_ampersand, *token_semicolon, *token_space;

    char *saveptr1, *saveptr2;

    token_semicolon = strtok_r(input, semicolon_delim, &saveptr1);

    while (token_semicolon != NULL)
    {
        token_semicolon = remove_extra_spaces(token_semicolon);
        // Process each semicolon-separated token
        // printf("Semicolon token: %s\n", token_semicolon);

        // Check if the token contains '&' (background process)

        if (strstr(token_semicolon, ampersand_delim))
        {
            int len = strlen(token_semicolon);
            // printf("%c %c %c\n",token_semicolon[len-1],token_semicolon[len-2],token_semicolon[len-3]);
            int x = 0;
            if (token_semicolon[len - 1] == '&' || (token_semicolon[len - 1] == ' ' && token_semicolon[len - 2] == '&'))
                x = 1;

            token_ampersand = strtok_r(token_semicolon, ampersand_delim, &saveptr2);
            char **arg = (char **)malloc(10 * sizeof(char *));
            for (int i = 0; i < 10; i++)
                arg[i] = (char *)malloc(100 * sizeof(char));
            int j = 0;
            while (token_ampersand != NULL)
            {
                //  char* temp1=(char*)malloc(200*sizeof(char));
                token_ampersand = remove_extra_spaces(token_ampersand);

                strcpy(arg[j++], token_ampersand);
                // Process each ampersand-separated token
                // printf("Ampersand token: %s\n", arg[j-1]);
                // suc=exec(token_ampersand,1);
                // if(suc==0){
                // return;}

                token_ampersand = strtok_r(NULL, ampersand_delim, &saveptr2);
            }
            int suc = run(arg, j, x);
            if (suc == 0)
                return;
            else if (suc == 10)
                x = 1;
        }
        else if (strstr(token_semicolon, "|") || strstr(token_semicolon, ">") || strstr(token_semicolon, "<") || strstr(token_semicolon, ">>"))
        {
            int suc = PIPE(token_semicolon);
            if (suc == 0)
                return;
        }
        else
        {
            // Tokenize using space delimiter for non-background process
            // printf("43\n");
            char *temp1 = (char *)malloc(200 * sizeof(char));
            strcpy(temp1, token_semicolon);
            // printf("54\n");
            token_space = strtok(token_semicolon, space_delim);

            if (check(token_space))
            {
                char **arg = (char **)malloc(10 * sizeof(char *));
                for (int i = 0; i < 10; i++)
                    arg[i] = (char *)malloc(100 * sizeof(char));
                int j = 0;

                while (token_space != NULL)
                {
                    strcpy(arg[j++], token_space);
                    // Process each space-separated token
                    // printf("Space token: %s\n", token_space);
                    token_space = strtok(NULL, space_delim);
                }
                suc = runcmd(arg, j - 1);
                if (suc == 0)
                    return;
                else if (suc == 10)
                    x = 1;
            }
            else
            {
                //   printf("90\n");
                // printf("%s\n",temp1);
                suc = exec(temp1, 0);
                if (suc == 0)
                    return;
            }
            free(temp1);
        }

        token_semicolon = strtok_r(NULL, semicolon_delim, &saveptr1);
    }

    if (x != 1)
    {

        // printf("%d\n", x);
        // printf("21\n");
        // print_history();
        store_entry(temp);
        // save_history();
    }
    free(temp);
}

void exit_function()
{
    char *temp = (char*)malloc(7*sizeof(char));
    strcpy(temp,"EXIT\n");
    store_entry(temp);
    save_history();
    // printf("bye\n");
}

// void signalHandler(int signal)
// {
//     // Terminate the program
//     printf("\n");
//     exit(0);
// }

// Signal handler for SIGTSTP (Ctrl+Z)
void sigtstpHandler(int signo)
{
    P[countt - 1].status = 0;
    // printf("hi\n");

    P[countt - 1].bg = 1;
    ping(PPid, 20);

    siglongjmp(jump_buffer, 1);

    // printf("\nCtrl+Z: Process stopped. Type 'fg' to bring it back to the foreground.\n");
}
void sigintHandler(int signo)
{

    // printf("hi%d\n",PPid);
    ping(PPid, 9);
    siglongjmp(jump_buffer, 1);

    // printf("\nCtrl+C: Process interrupted. Type 'exit' to quit.\n");
}

void sigquitHandler(int signo)
{
    // printf("\nCtrl+D: Logging out. Goodbye!\n");
    char *temp = (char *)malloc(7 * sizeof(char));
    strcpy(temp, "EXIT\n");
    store_entry(temp);
    save_history();

    exit(0); // Terminate the program
}

int main()
{
    atexit(exit_function);
    getcwd(shl, 200);
    load_history();
    // signal(SIGTERM, signalHandler);
    signal(SIGINT, sigintHandler); // Handle Ctrl+C
    signal(SIGTSTP, sigtstpHandler);
    signal(SIGQUIT, sigquitHandler);

    // signal(SIGINT, signalHandler);
    int i = 0;
    int pid = getpid();
    pgid = getpgid(pid);

    // Keep accepting commands

    if (sigsetjmp(jump_buffer, 1) == 0)
    {
        while (1)
        {
            // i++;
            // printf("%d\n",i);

            // Print appropriate prompt with username, systemname and directory before accepting input
            // printf("234\n");
            prompt(shl);
            // printf("543\n");
            // printf("100\n");
            char input[4096];
            // fgets(input, 4096, stdin);
            
            // printf("hihii\n");
            
            if (fgets(input,4096,stdin)==NULL)
            {
                if (feof(stdin)){
                printf("\n");
                break;}
            }
            // read(STDIN_FILENO,input,sizeof(input));
            while (!strcmp(input, "\n"))
                fgets(input, 4096, stdin);
            // printf("%s",input);
            // printf("%d\n", strlen(input));
            
            check_background_processes();
            // printf("\n4343343\n");

            // int pid = fork();
            // if (pid == 0)
            // {

            cmd(input);

            // printf("544\n");
            // }
            // wait(NULL);
        }
    }
    else
    {
        while (1)
        {
            // i++;
            // printf("%d\n",i);

            // Print appropriate prompt with username, systemname and directory before accepting input
            // printf("234\n");
            
            prompt(shl);
            // fflush(stdin);
            // printf("543\n");
            // printf("100\n");
            char input[4096];
            // fflush(stdout);

            if (fgets(input, 4096, stdin) == NULL)
            {
                printf("\n");
                break;
            }
            // read(STDIN_FILENO,input,sizeof(input));
                fgets(input, 4096, stdin);
            // printf("%s",input);
            // printf("%d\n", strlen(input));
            check_background_processes();
            // printf("\n4343343\n");

            // int pid = fork();
            // if (pid == 0)
            // {

            cmd(input);
        }
    }
    return 0;
}
