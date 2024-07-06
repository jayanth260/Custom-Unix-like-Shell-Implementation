#include "neonate.h"

int III(const char *str) {
    return atoi(str);
}


int kbhit(void) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}


int neonate(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s -n [time_arg]\n", argv[0]);
        return 1;
    }

    int time_arg = atoi(argv[2]);

    int size=2;
    int* book=(int*)malloc(2*sizeof(int));
    int count=0;

    if (time_arg <= 0) {
        fprintf(stderr, "Invalid time_arg value. Please provide a positive integer.\n");
        return 1;
    }

    // printf("Press 'x' to exit.\n");

    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    int pid,PID,i=0;


    while (1) {
        
        int min=1000000;

        // printf("Process-ID: %d\n", pid);
        DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char proc_path[256];
    char proc_stat[256];

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir))) {
        if (isdigit(entry->d_name[0])) {
            snprintf(proc_path, sizeof(proc_path), "/proc/%s", entry->d_name);
            snprintf(proc_stat, sizeof(proc_stat), "%s/stat", proc_path);

            if (stat(proc_stat, &statbuf) == 0) {
                time_t start_time = statbuf.st_ctime;
                time_t current_time;
                time(&current_time);

                if(min>(current_time - start_time) && (current_time - start_time)>=0)
                pid=III(entry->d_name);

                // printf("Process ID: %s, Start Time: %ld seconds ago\n", entry->d_name, current_time - start_time);
            }
        }
    }


    closedir(dir);

    if(PID!=pid)
    {
        for(int j=0;j<count;j++){
            if(pid==book[j])
            pid=PID;
        }
        if(count==size){
            book=(int*)realloc(book,2*size);
            size=size*2;
        }
        book[count++]=PID;
    }
    PID=pid;


    
    
    printf("%d\n",pid);
        sleep(time_arg);

        if (kbhit()) {
            char ch = getchar();
            if (ch == 'x') {
                break;
            }
        }
        i++;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    // printf("Exiting...\n");

    free(book);

    return 0;
}

