[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description
 all headers are in .h files(headers.h is main and includes all other headers)

- main.c: shell starts here and takes commands and process them and calls corresponding fuctions for the commands to run from other files.
- prompt.c: (specification 1) contains functions to print the prrompt after every command.
- warp.c: (specificatoin 3) contains functions to execute 'warp' command.
- peek.c: (specification 4) contains functions to execute 'peek' command.
- pastevents.c: (specification 5) contains function to execute 'pastevents', 'pastevents purge', 'pastevents execute <int>' commands.
- exec.c: (specification 6,12) contains functions to execut system commands using 'execvp' syscall. can manage both foreground and background processess. Also have a struct to store the processess for activities command.
- proclore.c: (specification 7) contains functions to execute 'proclore' command.
- seek.c: (specification 8) contains functions to execute 'seek' command.
- IOredidec.c: (specification 9,10) contains functions to implement I/O Redirection and Pipes.
- signal.c: (specification 13) contains functions to implement 'ping' command .
- BF.c: (specification 14) contains functions to implement 'fg' and 'bg' commands.
- neonate.c: (specification 15) contains functions to implement 'neonate' command.
- iMan.c: (specification 16) contains functions to implement 'iMan' command.

- history.txt: pastevents are saved into this file everytime shell is terminated and pastevents are loaded into some cirular array everytime shell in started.



# Assumptions

1. commands are used as they are (in small letters) other formats should not be used.
2. 'Proclore' command gives all information of process iff the pid is valid (i.e. there should be a process) else, prints the pid and nothing else.
3. entering 'EXIT' exits the shell. "EXIT" is stored in history.txt everytime shell is terminated.
4. if a commnd contains 'pastevents' in it or command returns any error, command is not stored.
5. I am using hash maps to store command for background processess. so, there can be collisions if too many background processess are run and may print wrong name of command but pid wont be wrong. (here i am using array of size 1000 and hashing is sum of square of digits in pid).
6. 'warp -' will work same as 'warp .' and 'peek -' will work same as 'peek .' if warp and peek are used for first time. else, 'warp -' changes to previous directory and 'peek -' prints files and directories of directory which is used for previous peek. 
7. pastevents execute x , here x<=15.
8. pipes can be done only for system commands. only first command from left can be a user command in pipes.
9. ctrl-z works as the original ctr-z. it makes a process go to T state(signal 20).
10. fg and bg for stopped background process is which went to background by ctrl-z.
11. neonate prints recently swapned process not neccessarily running. And ctrl-c wont stop the process, user should only press x to stop process.
12. 




