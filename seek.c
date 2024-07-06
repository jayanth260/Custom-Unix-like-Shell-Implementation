#include "seek.h"
#include "headers.h"

#define MAX_PATH_LENGTH 256

#define COLOR_RESET "\x1B[0m"

char *remove_extension(char *filename)
{
    char *dot = strrchr(filename, '.');
    if (dot && dot != filename)
    {
        *dot = '\0';
    }
    return filename;
}

int num_matched_files = 0;
char *path1;

int seek_recursive(const char *target, const char *currentDirectory, int isFileOnly, int isDirOnly, int isExecute)
{
    DIR *dir = opendir(currentDirectory);
    if (!dir)
    {
        perror("opendir");
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", currentDirectory, entry->d_name);

        struct stat st;
        if (stat(path, &st) == 0)
        {
            if (S_ISDIR(st.st_mode))
            {
                seek_recursive(target, path, isFileOnly, isDirOnly, isExecute);
            }

            char entry_name[MAX_PATH_LENGTH];
            strcpy(entry_name, entry->d_name);
            char *entry_name_without_extension = remove_extension(entry_name);

            if (fnmatch(target, entry_name_without_extension, FNM_NOESCAPE) == 0)
            {
                if ((isFileOnly && S_ISREG(st.st_mode)) || (isDirOnly && S_ISDIR(st.st_mode)))
                {
                    

                    // if (isExecute && !access(path, X_OK)) {
                    //     if (S_ISREG(st.st_mode)) {
                    //         printf("Missing permissions for task!\n");
                    //     } else if (S_ISDIR(st.st_mode)) {
                    //         if (chdir(path) == 0) {
                    //             printf("Directory changed to %s\n", path);
                    //         } else {
                    //             perror("chdir");
                    //         }
                    //     }
                    // }
                    // else {
                    if (S_ISREG(st.st_mode))
                    {   num_matched_files++;
                        printf(COLOR_GREEN "%s/%s" COLOR_RESET "\n", currentDirectory, entry->d_name);
                        strcpy(path1, currentDirectory);
                        strcat(path1, "/");
                        strcat(path1, entry->d_name);
                        // printf("%s\n",path1);
                    }
                    else if (S_ISDIR(st.st_mode))
                    {   num_matched_files++;
                        printf(COLOR_BLUE "%s/%s" COLOR_RESET "\n", currentDirectory, entry->d_name);
                        strcpy(path1, currentDirectory);
                        strcat(path1, "/");
                        strcat(path1, entry->d_name);
                        // printf("%s\n",path1);
                    }
                    // }
                }
            }
        }
        // else {
        //     printf()
        //     // perror("stat");
        // }
    }

    // Handle the -e flag behavior

    closedir(dir);
    // printf("%s\n",path1);
}

int seek(const char *target, const char *targetDirectory, int isFileOnly, int isDirOnly, int isExecute)
{   int suc;
    path1 = (char *)malloc(300 * sizeof(char));
    int x= seek_recursive(target, targetDirectory, isFileOnly, isDirOnly, isExecute);
    if(x==0)
    return 0;
    // printf("%s\n",path1);
    if(num_matched_files==0){
        printf("No match found!\n");
        suc=1;
    }
    if (isExecute && num_matched_files == 1)
    {

        // printf("32\n");

        if (isFileOnly)
        {
            if (access(path1, R_OK) == 0){
            char *arg = (char *)malloc(300 * sizeof(char));
            strcpy(arg, "cat "); // Copy "cat " to arg
            strcat(arg, path1);  // Concatenate path1 to arg
            // printf("%s\n", arg);
            suc= exec(arg, 0);
            free(arg);}
            else{
                printf("Missing permissions for task!\n");
                suc=1;
            }
            
        }
        else if (isDirOnly)
        {   if (access(path1, X_OK) == 0){
            char **arg = (char **)malloc(2 * sizeof(char *));
            for (int i = 0; i < 2; i++)
                arg[i] = (char *)malloc(300 * sizeof(char));
            arg[0] = "warp";
            strcpy(arg[1], path1);

             suc=runcmd(arg, 1);
            
            for(int i=0;i<2;i++)
            free(arg[i]);
            free(arg);}
            else{
                printf("Missing permissions for task!\n");
                suc=1;
            }
            
        }

        //     struct stat st;
        //     if (stat(path, &st) == 0) {
        //         if (S_ISDIR(st.st_mode)) {
        //             if (chdir(target) == 0) {
        //                 printf("Directory changed to %s\n", target);
        //             } else {
        //                 perror("chdir");
        //             }
        //         } else if (S_ISREG(st.st_mode)) {
        //             FILE *file = fopen(target, "r");
        //             if (file) {
        //                 char line[MAX_PATH_LENGTH];
        //                 if (fgets(line, sizeof(line), file)) {
        //                     printf("Output: %s", line);
        //                 }
        //                 fclose(file);
        //             } else {
        //                 perror("fopen");
        //             }
        //         }
        //     } else {
        //         perror("stat");
        //     }
    }
    num_matched_files=0;
    free(path1);
    return suc;
}

int Seek_proc(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <flags> <search> [target_directory]\n", argv[0]);
        return 1;
    }
    // printf("%d\n", argc);

    int is_dir = 0, is_file = 0, execute = 0;
    int x = 0;

    for (int i = 1; i < argc - 1; i++)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-d") == 0)
            {
                is_dir = 1;
                x++;
            }
            else if (strcmp(argv[i], "-f") == 0)
            {
                is_file = 1;
                x++;
            }
            else if (strcmp(argv[i], "-e") == 0)
            {
                execute = 1;
                x++;
            }
            else
            {
                printf("Invalid flag: %s\n", argv[i]);
                return 0;
            }
        }
    }
    int v = 0;
    if (x == 0 && argc == 2)
        v = 1;
    else if (x == 0 && argc == 3)
        v = 2;
    else if (x == 1 && argc == 3)
        v = 1;
    else if (x == 1 && argc == 4)
        v = 2;
    else if (x == 2 && argc == 4)
        v = 1;
    else if (x == 2 && argc == 5)
        v = 2;
    if (is_file == 1 && is_dir == 1)
    {
        printf("can't use -f and -d at same time\n");
        return 0;
    }

    if (x > 2)
    {
        printf("seek: too many flags\n");
        return 0;
    }

    char *search = (char *)malloc(200 * sizeof(char));
    strcpy(search, argv[argc - v]);
    // printf("%s\n", search);
    char *target_directory = (char *)malloc(200 * sizeof(char));
    if ((x==0 && argc==3)||(x == 1 && argc == 4) || (x == 2 && argc == 5))
    {
        strcpy(target_directory, argv[argc - 1]);
    }
    else
        target_directory = ".";
    // printf("%s\n", target_directory);
    if(is_file==0 && is_dir==0){
        is_file=1 ;
        is_dir=1;
    }

    return seek(search, target_directory, is_file, is_dir, execute);

    
}
