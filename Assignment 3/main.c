#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
//The same from perv homeworks
#include "dynarray.h"
//In order to check the foreground, can be done through the use of these global variables
//Checked through these
bool ground_mode = false;
bool ctrl_zflag = false;

struct command{
    char *name;
    char **args;
    int num_args;
    char *input;
    char *output;
    bool background;
};


/*
Summary: Parses through a command line string and extracts the command name, arguments and other input/outputs redirection information to store in a struct command
*/
char *expansion(char *word){

    int pid = getpid();

    char *saveptr;
    char *token;

    char *expanded = calloc(strlen(word) + 20, sizeof(char));  

    token = strtok_r(word, "$$", &saveptr);

    if(strncmp("$$", word, strlen("$$")) == 0)        
        sprintf(expanded, "%d%s", pid, token);
    else
        sprintf(expanded, "%s%d", token, pid);
    
    while(strstr(saveptr, "$$$")){
        token = strtok_r(NULL, "$$", &saveptr);
        sprintf(expanded, "%s%d%s", expanded, pid, token);
    }

    if(strlen(saveptr) != 0){
        saveptr++;
        sprintf(expanded, "%s%s", expanded, saveptr);
    }

    return expanded;
}

struct command *parseLine(char *currLine)
{
    struct command *command = malloc(sizeof(struct command));

    command->args = malloc(512*sizeof(char*)); 
    
    for(int i = 0; i < 512; i++){
        command->args[i] = NULL;    
    }
    command->num_args = 0;
    command->background = false;

    char *saveptr;
    char *saveptr2;

    char *token = strtok_r(currLine, " ", &saveptr);

    if(strstr(token, "$$"))
        token = expansion(token);

    if(strstr(token, "\n"))
        token = strtok_r(token, "\n", &saveptr2);
    command->name = calloc(strlen(token) + 1, sizeof(char));
    strcpy(command->name, token);

    command->args[0] = calloc(strlen(token) + 1, sizeof(char));
    strcpy(command->args[0], token);
    command->num_args++;

    token = strtok_r(NULL, " ", &saveptr);

    while(token){

        if(strstr(token, "$$"))
            token = expansion(token);
        
        if(strcmp("&\n", token) == 0){

            if(strcmp(command->name, "exit") != 0 && strcmp(command->name, "cd") != 0 && strcmp(command->name, "status") != 0)
                command->background = true;

            break;
            
        }
        
        if(strstr(token, "\n"))
            token = strtok_r(token, "\n", &saveptr2);
            

        if(strcmp("<", token) == 0){
            token = strtok_r(NULL, " ", &saveptr);
            if(strstr(token, "\n"))
                token = strtok_r(token, "\n", &saveptr2);

            command->input = calloc(strlen(token) + 1, sizeof(char));
            strcpy(command->input, token);
        }
        else if(strcmp(">", token) == 0){
            token = strtok_r(NULL, " ", &saveptr);
            if(strstr(token, "\n"))
                token = strtok_r(token, "\n", &saveptr2);

            command->output = calloc(strlen(token) + 1, sizeof(char));
            strcpy(command->output, token);
        }
        else{
            command->args[command->num_args] = calloc(strlen(token) + 1, sizeof(char));
            strcpy(command->args[command->num_args], token);
            command->num_args++;
        }
        
        token = strtok_r(NULL, " ", &saveptr);
    }  

    return command;
}



/*******************************************************************/
/************************This is the portion that basically works together**************************/
/******************************Alot of this is from the modules on Canvas****************************/



/*
Summary: Using this to toggle bw fground mode and normal mode when the user presses ctrl z to print message in respect to it
*/
void handle_SIGTSTP(int signo){
    char *message;

    if(ground_mode){
        ground_mode = false;
        message = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, message, 30);
        fflush(stdout); 
    }
    else{
        ground_mode = true;
        message = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, message, 50);
        fflush(stdout);          
    }
    ctrl_zflag = true;
}

/*
Summary:Similar to other but using it for the ctrl C. Either ignores the signlal or sets the default action depending on the parameter ignore
*/
void initSIGINT(bool ignore){
    struct sigaction SIGINT_action = {0};

    if(ignore)
        SIGINT_action.sa_handler = SIG_IGN;
    else
        SIGINT_action.sa_handler = SIG_DFL;

    sigfillset(&SIGINT_action.sa_mask);
    sigaction(SIGINT, &SIGINT_action, NULL);
}

/*
Summary: SIGTSTP signal received. Ignores the signal or assigns the function handle_SIGTSTP to handle the signal in respect to parameter ignore
*/
void initSIGTSTP(bool ignore){
    struct sigaction SIGTSTP_action = {0};

    if(ignore)
        SIGTSTP_action.sa_handler = SIG_IGN;
    else
        SIGTSTP_action.sa_handler = handle_SIGTSTP;

    sigfillset(&SIGTSTP_action.sa_mask);
    SIGTSTP_action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}

/*
Summary:CHecks the status of the child processes running i the background. Prints appr messages depending on whether backg process has exited normally or terminated due to signal. 
*/
void checkOnChildren(struct dynarray *background_pids){

    int childPid;
    int childStatus = 0;

    for(int i = 0; i < dynarray_size(background_pids); i++){

        childPid = dynarray_get(background_pids, i);

        if(childPid > 0){

            while(waitpid(childPid, &childStatus, WNOHANG) > 0){

                if (WIFEXITED(childStatus)){
                    printf("background pid %d is done: exit value %d\n", childPid, WEXITSTATUS(childStatus));
                    fflush(stdout);
                }
                else if (WIFSIGNALED(childStatus)){
                    printf("background pid %d is done: terminated by signal %d\n", childPid, WTERMSIG(childStatus));
                    fflush(stdout);
                }

                dynarray_remove(background_pids, i);
            }
        }
    }
}


//End
/*******************************************************************/



/*******************************************************************/
/************************This is the portion that basically works together**************************/

void execOtherCommands(struct command *command, int *status, struct dynarray *background_pids){

    int spawnPid = fork();

    switch(spawnPid){
        case -1:
            perror("fork() error \n");
            exit(1);
            break;

        case 0:
            initSIGTSTP(true);

            if(!command->background || ground_mode)
                initSIGINT(false);
            else
                initSIGINT(true);

            if(command->input || (command->background && !ground_mode)){

                int sourceFD;

                if(command->input)
                    sourceFD = open(command->input, O_RDONLY);
                else
                    sourceFD = open("/dev/null", O_RDONLY);

                if (sourceFD == -1) { 
                    printf("cannot open %s for input\n", command->input);  
                    fflush(stdout);
                    exit(1);
                }

                int in_result = dup2(sourceFD, 0);
                if (in_result == -1) { 
                    perror("source dup2()"); 
                    exit(2);
                }
                
                if(fcntl(in_result, F_GETFD))
                    fcntl(in_result, F_SETFD, FD_CLOEXEC);
            }

            if(command->output || (command->background && !ground_mode)){

                int targetFD;

                if(command->output)
                    targetFD = open(command->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                else
                    targetFD = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if (targetFD == -1) { 
                    printf("cannot open %s for output\n", command->output); 
                    fflush(stdout);
                    exit(1);
                }
                
                int out_result = dup2(targetFD, 1);
                if (out_result == -1) { 
                    perror("target dup2()"); 
                    exit(2);
                }

                if(fcntl(out_result, F_GETFD))
                    fcntl(out_result, F_SETFD, FD_CLOEXEC);
            }

            if (execvp(command->name, command->args) && !ctrl_zflag){ 
                printf("%s: no such file or directory\n", command->name);
                fflush(stdout);
				exit(1);
            }
            break; 

        default:
            initSIGINT(true);
            initSIGTSTP(false);

            if(command->background && !ground_mode){
                printf("background pid is %d\n", spawnPid);
                fflush(stdout);
                dynarray_insert(background_pids, spawnPid);
                break; 
            }
            else{
                waitpid(spawnPid, status, 0);

                if (WIFSIGNALED(*status) == 1 && WTERMSIG(*status) != 0)
                    printf("terminated by signal %d\n", WTERMSIG(*status));
                
                fflush(stdout);

                checkOnChildren(background_pids);
                    
            }
    } 
}

void executeCommand(struct command *command, int *status, struct dynarray *background_pids){

    if(strcmp(command->name, "exit") == 0){

        for(int i = 0; i < dynarray_size(background_pids); i++){
            
            int childPid = dynarray_get(background_pids, i);
            if(childPid > 0)
                kill(childPid, SIGKILL);
        }
        exit(0);
    }

    else if(strcmp(command->name, "cd") == 0){

        if(command->args[1] == NULL)
            chdir(getenv("HOME"));
        else
            chdir(command->args[1]);
    }

    else if(strcmp(command->name, "status") == 0){
        
        if(WIFEXITED(*status)){
            printf("exit value %d\n", WEXITSTATUS(*status));
        } 
        else if(WIFSIGNALED(*status)){
            printf("terminated by signal %d\n", WTERMSIG(*status));
        }
    }
    
    else{
        execOtherCommands(command, status, background_pids);
    }
}


//Finally the main function
int main() {

    int status = 0;

    size_t len = 0;
    ssize_t nread;

    char *buffer = malloc(2048*sizeof(char));

    struct dynarray *background_pids = dynarray_create();

    struct command *command;          

    while(true){
        ctrl_zflag = false;

        checkOnChildren(background_pids);

        printf(": ");
        fflush(stdout);
        nread = getline(&buffer, &len, stdin);

        if(strcmp("\n", buffer) != 0 && strncmp("#", buffer, 1) != 0){

            command = parseLine(buffer);

            executeCommand(command, &status, background_pids);
        }
        
    }

    return 0;
}
