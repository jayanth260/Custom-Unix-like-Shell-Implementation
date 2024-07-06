#include "peek.h"
#include "warp.h"

char previous_peek[200]="" ;
int y=0;

void list_files(char path[], int show_hidden, int show_details) {
    // printf("1\n");
    // printf("4354\n");
    // printf("%s\n",path);
    // printf("44\n");
    
    DIR *dir = opendir(path);
    
    if (!dir) {
        
        perror("opendir");
        return;
    }
    // printf("2\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue; // Skip hidden files if not showing hidden
        }
        // printf("3\n");

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (show_details) {
            // Display file information
            printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

            printf(" %2lu", file_stat.st_nlink);

            struct passwd *pwd = getpwuid(file_stat.st_uid);
            struct group *grp = getgrgid(file_stat.st_gid);
            printf(" %s %s", pwd ? pwd->pw_name : "unknown", grp ? grp->gr_name : "unknown");

            printf(" %8lld", (long long)file_stat.st_size);

            struct tm *tm_info = localtime(&file_stat.st_mtime);
            char time_buffer[20];
            strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", tm_info);
            printf(" %s", time_buffer);
        }

        // File name with color coding (when applicable)
        if (S_ISDIR(file_stat.st_mode)) {
            printf(COLOR_BLUE " %s" COLOR_WHITE "\n", entry->d_name);
        } else if (file_stat.st_mode & S_IXUSR || file_stat.st_mode & S_IXGRP || file_stat.st_mode & S_IXOTH) {
            printf(COLOR_GREEN " %s" COLOR_WHITE "\n", entry->d_name);
        } else {
            printf(" %s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int peek(char path[],int L,int A,char shl[]) {
    // printf("%s\n",path);
    char* current_dir=(char*)malloc(200*sizeof(char));
    
    
    // printf("%s\n",current_dir);
    if(!strcmp(path,"-"))
    {   
        // printf("32\n");
        if(y==1){
            // printf("%s\n",previous_peek);
        strcpy(current_dir,previous_peek);}
        else 
        strcpy(current_dir,".");
        strcpy(current_dir ,getactpath(current_dir,shl));
    }
    else{
        strcpy(current_dir ,getactpath(path,shl));

    }
    
    strcpy(previous_peek,current_dir);
    // printf("%s\n",current_dir);
    list_files(current_dir, L,A);
    // printf("32\n");

    
y=1;
return 1;
}