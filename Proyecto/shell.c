#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXLINE 80 /* The maximum length command */

int splitLine(char *str, char **lines){
    int i = 0;
    
    char * token = strtok(str, " ");
    while( token != NULL ) {
        lines[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    return i;
}

void executeNotPipeCommand(char **lines){
    pid_t pid = fork();
    if(pid == 0){
        if(execvp(lines[0], lines) < 0){
            printf("No se pudo ejecutar\n");
            exit(0);
        } 
        
    }
    else if(pid == -1){
        printf("Fail");
        return;
    }
    else{
        wait(NULL); 
        return;
    }
}

void executePipeCommand(char **piped, char ** normal){
    int pipes[2];
    int status;
    pid_t wpid;
    if(pipe(pipes) == -1){
        printf("Error creating pipe\n");
    }
    pid_t pid = fork();
    if(pid == 0){
        close(pipes[0]);
        dup2(pipes[1], STDOUT_FILENO);
        close(pipes[1]);
        if(execvp(normal[0], normal)) printf("Fail executing command");
        
        exit(0);
    }
    else if(pid == -1){
        printf("Fail");
    }
    else{
        pid_t pidDos = fork();
        if(pidDos == 0){
            close(pipes[1]);
            dup2(pipes[0], STDIN_FILENO);
            close(pipes[0]);
            if(execvp(piped[0], piped)) printf("Fail executing command");
            
            exit(0);
        }
        else if(pidDos == -1){
            printf("Fail");
        }
        
        else{
            wait(NULL);
            /*
            while((wpid = wait(&status) > 0)){
                printf("hola\n");
            }
            */
            // wait(NULL);
            // wait_pid(pid, NULL, 0);
            return;
        }
    }
    return;
}

void executeExecIn(char **lines, char **piped, char **normal){
    int status;
    pid_t pid = fork();
    if(pid == 0){
        status = open(piped[0], O_RDONLY, 0777);
        if(status == -1){
            fprintf(stderr, "Fail\n");
        } 
        dup2(status, STDIN_FILENO);
        execvp(normal[0], normal);
        exit(0);
    }
    else{
        wait(NULL);
    }
}

void executeExecOut(char **lines, char **piped, char **normal){
    int status;
    pid_t pid = fork();
    if(pid == 0){
        status = open(piped[0], O_WRONLY | O_CREAT, 0777);
        if(status == -1){
            fprintf(stderr, "Fail\n");
        } 
        dup2(status, STDOUT_FILENO);
        execvp(normal[0], normal);
        exit(0);
    }
    else{
        wait(NULL);
    }
}

int commandHasPipe(char **lines, int elementos, char * elemento){
    int x;
    for(x = 0; x < elementos; x++){
        if(strcmp(lines[x], elemento) == 0){
            return x;
        }
    }
    return 0;
}

void executeCd(char **lines){
    if(chdir(lines[1]) < 0) printf("Directory does not exist\n");
    char currentDirectory[100];
    getcwd(currentDirectory, sizeof(currentDirectory));
    printf("%s\n", currentDirectory);
}

void dividirEntradaPipe(char **piped, char **lines, char **normal, int elementos, int pipe){
    int x;
    for(x = 0; x < pipe; x++){
        normal[x] = lines[x];
    }
    normal[x] = NULL;
    int y = 0;
    for(x = pipe + 1; x < elementos; x++){
        piped[y] = lines[x];
        y++;
    }
    piped[y] = NULL;
}

int main(void)
{
    char input[MAXLINE];
    
    int shouldRun = 1; /* flag to determine when to exit program */
    int prev = 0;
    char linesHistory[MAXLINE];
    char * piped[60];
    char * normal[60];
    while (shouldRun) {
        
        char* lines[60];
        
        printf("osh>");
        fflush(stdout);
        fflush(stdin);
        fgets(input, MAXLINE, stdin);
        input[strcspn(input, "\n")] = 0;
        if(strcmp(input, "!!") != 0){
            strcpy(linesHistory, input);
        }
        

        int elementos = splitLine(input, lines);
        int pipe = commandHasPipe(lines, elementos, "|");
        int mayor = commandHasPipe(lines, elementos, ">");
        int menor = commandHasPipe(lines, elementos, "<");

        if(strcmp(lines[0], "cd") == 0){
            prev = 1;
            executeCd(lines);
        }
        else if(mayor != 0){
            dividirEntradaPipe(piped, lines, normal, elementos, mayor);
            executeExecOut(lines, piped, normal);
        }
        else if(menor != 0){
            dividirEntradaPipe(piped, lines, normal, elementos, menor);
            executeExecIn(lines, piped, normal);
        }
        else if(strcmp(lines[0], "!!") == 0){
            if(prev == 1){
                splitLine(linesHistory, lines);
                executeCd(lines);
            }
            else if(prev == 2){
                splitLine(linesHistory, lines);
                executeNotPipeCommand(lines);
            }
            else{
                char * piped[60];
                char * normal[60];
                splitLine(linesHistory, lines);
                dividirEntradaPipe(piped, lines, normal, elementos, pipe);
                executePipeCommand(piped, normal);
            } 
        }
        else if(pipe == 0){
            prev = 2;
            lines[elementos] = NULL;
            executeNotPipeCommand(lines);
        }
        else{
            char * piped[60];
            char * normal[60];
            dividirEntradaPipe(piped, lines, normal, elementos, pipe);
            executePipeCommand(piped, normal);
            prev = 3;
        }        
        // printf("lineshistory %s", lines[0]);
        printf("\n");
    }
    return 0;
}