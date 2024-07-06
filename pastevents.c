#include "pastevents.h"
#include "headers.h"


char **history;
int head = -1;
int tail = 0;
int n = 0;
const int MAX_ENTRIES = 15;
const int MAX_ENTRY_LENGTH = 200;

void store_entry(char *entry) {
    // printf("21\n");
    if (history == NULL) {
        history = (char **)malloc(MAX_ENTRIES * sizeof(char *));
        for (int i = 0; i < MAX_ENTRIES; i++)
            history[i] = (char *)malloc(MAX_ENTRY_LENGTH * sizeof(char));
    }
    // printf("45\n");
    // Remove trailing newline, if present
    int len = strlen(entry);
    if (len > 0 && entry[len - 1] == '\n') {
        entry[len - 1] = '\0';
    }

    // Check for duplicate entry
    if (!strcmp(entry,"\0")||(head > -1 && !strcmp(history[head], entry)))
        return;
        // printf("99\n");

    head = (head + 1) % 15;

    strcpy(history[head], entry);

    if (n != MAX_ENTRIES)
        n++;

    if (n == MAX_ENTRIES && head == tail)
        tail = (tail + 1) % MAX_ENTRIES;
}

void print_history() {
    int num_printed = 0;
    int index = tail;

    while (num_printed < n) {
        printf("%s\n", history[index]);
        index = (index + 1) % MAX_ENTRIES;
        num_printed++;
    }
}

// Rest of the code remains the same...


void save_history() {
    int fd = open("/home/jayanth/Code/OSN_proj/mini-project-1-jayanth260/history.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    for (int i = 0; i < n; i++) {
        size_t length = strlen(history[(i + tail) % MAX_ENTRIES]);

        ssize_t bytesWritten = write(fd, history[(i + tail) % MAX_ENTRIES], length);
        if (bytesWritten == -1) {
            perror("write");
            close(fd);
            return;
        }

        // Add a newline after each entry
        char newline = '\n';
        ssize_t newlineWritten = write(fd, &newline, sizeof(char));
        if (newlineWritten == -1) {
            perror("write");
            close(fd);
            return;
        }
    }

    close(fd);
}


void load_history() {
    int fd = open("/home/jayanth/Code/OSN_proj/mini-project-1-jayanth260/history.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    if(history!=NULL){
        for(int i=0;i<n;i++)
        free(history[i]);
        free(history);
        history=NULL;
    }
   
    if (history == NULL) {
        history = (char **)malloc(MAX_ENTRIES * sizeof(char *));
        for (int i = 0; i < MAX_ENTRIES; i++)
            history[i] = (char *)malloc(MAX_ENTRY_LENGTH * sizeof(char));
    }
     head=-1;
    tail=0;
    n=0;

    char buffer[MAX_ENTRY_LENGTH];
    ssize_t bytesRead;
    // while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
    //     char *line = malloc(bytesRead + 1);
    //     if (line == NULL) {
    //         perror("malloc");
    //         close(fd);
    //         return;
    //     }
    //     memcpy(line, buffer, bytesRead);
    //     line[bytesRead] = '\0';
    //     store_entry(line);
    //      // Free allocated memory
    // }
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
    char *line_start = buffer; // Points to the start of the current line

    for (int i = 0; i < bytesRead; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0'; // Null-terminate the line
            store_entry(line_start);
            line_start = buffer + i + 1; // Move to the start of the next line
        }
    }

    // Store any remaining data as the last line if not already stored
    if (line_start < buffer + bytesRead) {
        store_entry(line_start);
    }
}

    close(fd);
}

int I(const char *str) {
    return atoi(str);
}

int pastevents(char arg[]) {
     // Load history from file

    if (!strcmp(arg, "@")) {
        print_history();
        return 10;
    } else if (!strcmp(arg, "#")) {
        
        int file = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
        close(file);
        load_history();
        return 10;
        // Handle pastevents execute
    } else {
        // Store the command
        // printf("%d\n",arg[0]);
        int x=I(arg);
        
        // printf("%s\n",history[(head-x+16)%15]);
        
        cmd(history[(head-x+16)%15]);
        return 10;
                 // Save history to file after storing a new entry
    }

    // Implement other shell functionality

    return 1;
}