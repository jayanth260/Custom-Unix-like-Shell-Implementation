#include "warp.h"

char previous_dir[1024] = ""; 
int x=0;// Global variable to store the previous directory

void remove_consecutive_slashes(char *path) {
    int length = strlen(path);
    char *new_path = path; // Start new path at the same location as the original path
    int j=0;
    for (int i = 0, j = 0; i < length; i++) {
        if (i > 0 && path[i] == '/' && path[i - 1] == '/') {
            // Skip consecutive slashes
            continue;
        }
        new_path[j++] = path[i];
    }

    // Null-terminate the new path
    new_path[j] = '\0';
}

int warp(char path[],char shl[]) {
     char* dest =(char*)malloc(200*sizeof(char)); // Get the home directory path
    char current_dir[1024]; // Buffer to store current directory path
    
    // printf("%s\n",path);
    // Handle cases where no arguments are given
  

    // Loop through arguments and execute 'warp'
    
        

        // Handle '~', '-' and '-' symbols
        
        strcpy(dest,path);
        // printf("%s\n",dest);
        if (!strcmp(path,"~")) {
            // printf("34\n");
            strcpy(dest,shl);
            // printf("%s\n",dest);
        } else if (strcmp(path, "-") == 0) {
            if(x==1)
            strcpy(dest, previous_dir);
            else
            strcpy(dest,".");
        }
        else{
            if(strstr(path,"~")!=NULL)
            {
            strcpy(dest,getactpath(path,shl));
            
            }
        }
        

        // Store the current directory in 'previous_dir' before changing
        getcwd(previous_dir, sizeof(previous_dir));
        // printf("%sll\n",dest);

        if (chdir(dest) != 0) {

            printf("Error: warp: %s is not valid path!\n",dest);
            return 0; // Skip to the next argument if chdir fails
        }
        // printf("43\n");
        getcwd(current_dir, sizeof(current_dir));
        printf("%s\n", current_dir);

        free(dest);
    
    x=1;
    return 1;
}

// Implement getactpath function
char* getactpath(char path[], char shl[]) {

    
    
     char *tildePos = strchr(path, '~');
    if (tildePos == NULL) {
        return path;
    }

    // Get the user's home directory path
    char *home = shl;
   

    // Calculate the new path length
    size_t newPathLength = strlen(home) + strlen(tildePos + 1) + (tildePos - path);

    // Allocate memory for the new path
    char *newPath = malloc(newPathLength + 1);
    if (newPath == NULL) {
        perror("malloc");
        return NULL;
    }

    // Copy the path up to ~
    strncpy(newPath, path, tildePos - path);
    newPath[tildePos - path] = '\0';

    // Append the user's home directory path
    strcat(newPath, home);

    // Append the rest of the path after ~
    strcat(newPath, tildePos + 1);

    // Change the working directory
    
    
    // remove_consecutive_slashes(newPath); 


    
    return newPath;
}
