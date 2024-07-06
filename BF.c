
#include "BF.h"
int fg(int pid) {
    
   
    
   
        
        // Remove trailing newline character
       
        
        // Tokenize the input comman

        
                    int status;
                    if (waitpid(pid, &status, WUNTRACED) != -1) {
                        tcsetpgrp(STDIN_FILENO, getpgid(pid));
                        tcsetpgrp(STDOUT_FILENO, getpgid(pid));
                        kill(pid, SIGCONT);
                        waitpid(pid, &status, WUNTRACED);
                        tcsetpgrp(STDIN_FILENO, getpid());
                        tcsetpgrp(STDOUT_FILENO, getpid());
                    } else {
                        printf("No such process found.\n");
                        
                    }

                    for(int i=0;i<countt;i++){
                        if(P[i].pid==pid)
                        P[i].status=0;
                        

                    }

        
    
    
    return 1;
}

int bg(int pid){
    int status;
                    if (kill(pid, SIGCONT) != -1) {
                        waitpid(pid, &status, WUNTRACED);
                    } else {
                        printf("No such process found.\n");}
                return 1;
}
