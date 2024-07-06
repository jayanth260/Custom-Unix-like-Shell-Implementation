#include "headers.h"

extern char *prtcmd;
void getrelpath(char curr[], char shl[])
{

    char *match = strstr(curr, shl);
    if (match == NULL)
        return;
    else
    {
        char *replacement = "~";
        size_t sub_len = strlen(shl);
        size_t replacement_len = strlen(replacement);

        // Calculate the length difference between the substring and replacement
        // int length_diff = replacement_len - sub_len;

        // Move characters to make space for the replacement
        memmove(match + replacement_len, match + sub_len, strlen(match + sub_len) + 1);

        // Copy the replacement string into the main string
        memcpy(match, replacement, replacement_len);
    }
}

void prompt(char shl[])
{

    // printf("%s\n",shl);
    // printf("4\n");
    char *user = getlogin();
    // printf("5\n");
    char curr[200];
    getcwd(curr, 200);
    // printf("%s\n",curr);

    // printf("3232\n");

    // printf("78\n");
    getrelpath(curr, shl);
    //  printf("%s\n",curr);

    struct utsname uts;
    // char buffer[1024];
    // ssize_t bytesRead;

    // int fd = open("/home/jayanth/Code/OSN_proj/mini-project-1-jayanth260/foreground.txt", O_RDONLY);
    // if (fd == -1) {
    //     perror("Failed to open the file");

    // }

    // bytesRead = read(fd, buffer, sizeof(buffer));
    //     // Process the read data
    //     // Here, we will simply print the read data
    //     // write(STDOUT_FILENO, buffer, bytesRead);

    // if (bytesRead == -1) {
    //     perror("Error reading from the file");
    //     close(fd);

    // }

    // close(fd);

    if (uname(&uts) < 0)
        perror("uname() error");

    if (time1 == 0)
        printf("<%s@%s:%s> ", user, uts.nodename, curr);
    else
    {
        printf("<%s@%s:%s %s :%ds> ", user, uts.nodename, curr, p_arg, time1);
        time1 = 0;
    }
}
