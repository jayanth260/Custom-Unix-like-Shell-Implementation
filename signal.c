#include "signal.h"

int II(const char *str) {
    return atoi(str);
}

int ping(int pid,int sig){
    // Check for the correct number of arguments

//    printf("323\n");

    // Parse the PID and signal number from command line arguments
   
    // Check if the PID exists
    if(pid==0){
    printf("\n");
    return 0;}
    // printf("%d %d\n",pid,sig);
    if (kill(pid, 0) != 0) {
        perror("No such process found");
        return 0;
    }

    // Send the signal to the specified process
    if (kill(pid, sig%32) == 0) {
        printf("Signal %d sent to PID %d\n", sig%32, pid);
    } else {
        perror("Error sending signal");
        return 0;
    }

    return 1;
}
