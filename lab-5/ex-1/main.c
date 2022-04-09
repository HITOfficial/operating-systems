#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct Command{
    char * name;
    char **programs;
    int programs_counter;
}Command;

int COMMAND_LENGTH = 50;


char* remove_white_signs(char *start){
    // removing m start
    while(isspace(*start)) {
        start++;
    }
    if(*start){
        char *end = start + strlen(start) -1;
        while(isspace(*end)) {
            end--;
        }
        // end is on last character of command name, so end+1 character need to be '\0'
        *(end+1) = '\0';
    }

    return start;
}


char** parse_arguments(char * arguments){
    char **res = NULL;
    char *arg = strtok(arguments," ");
    int n = 0;

    while(arg){
        res = realloc(res,sizeof(char*)*++n);
        res[n-1] = arg;
        arg = strtok(NULL," ");
    }

    return res;
}

void execute_command(char **programs,int programs_counter){

    int fd[programs_counter][2];
    for(int i =0;i<programs_counter;i++)pipe(fd[i]);

    for(int i = 0;i<programs_counter;i++){

        pid_t pid = fork();

        if(pid == 0){
            if(i > 0) dup2(fd[i-1][0],STDIN_FILENO);
            if(i < programs_counter -1) dup2(fd[i][1],STDOUT_FILENO);

            for(int j = 0;j < programs_counter;j++) close(fd[j][1]);

            char **arguments = parse_arguments(programs[i]);

            execvp(arguments[0],arguments);
        }
    }

    for(int i = 0;i < programs_counter;i++) close(fd[i][1]);

    for(int i =0;i<programs_counter;i++) wait(NULL);

}

void free_commands_memory(Command *commands,int no_commands){
    for(int i =0;i<no_commands;i++){
        free(commands[i].name);
        for(int j=0;j<commands[i].programs_counter;j++){
            free(commands[i].programs[j]);
        }
    }

}


int main(int argc, char*argv[]){
    if(argc < 2){
        printf("Wrong number of args!");
        return 1;
    }

    char* patch = argv[1];
    FILE *fp = fopen(patch,"r");
    Command *commands = NULL;
    int commands_counter = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&line,&len,fp)) != -1){

        // checking if line contains new command name
        if(strchr(line,'=') != NULL){
            commands_counter++;
            commands = realloc(commands,sizeof(Command) * commands_counter);

            char *command_name = strtok(line,"=");
            // memorizing command
            commands[commands_counter-1].name = malloc(sizeof(command_name));
            command_name = remove_white_signs(command_name);
            strcpy(commands[commands_counter-1].name,command_name);

            int programs_counter = 0;
            char * argument_line;
            char **programs = NULL;

            while((argument_line = strtok(NULL,"|"))!=NULL) {
                programs_counter++;
                programs = realloc(programs,sizeof(char*) * programs_counter);
                argument_line = remove_white_signs(argument_line);
                programs[programs_counter-1] = malloc(sizeof(argument_line));
                strcpy(programs[programs_counter-1],argument_line);
            }

            commands[commands_counter-1].programs_counter = programs_counter;
            commands[commands_counter-1].programs = programs;

        }
    }


    // changing pointer position to start of the file
    fseek(fp,0,SEEK_SET);

    while((read = getline(&line,&len,fp)) != -1){
        // going to lines after declaring
        if(strchr(line,'=') == NULL ) {
            // skipping  the  white line
            read = getline(&line,&len,fp);

            char **to_exec = NULL;
            int programs_counter = 0;
            char * command = strtok(line,"|");

            while(command != NULL){
                command = remove_white_signs(command);

                for(int i =0;i<commands_counter;i++){
                    if(strcmp(command,commands[i].name) == 0){
                        for(int j=0;j<commands[i].programs_counter;j++){
                            programs_counter++;
                            to_exec = realloc(to_exec,programs_counter *sizeof(char*));
                            to_exec[programs_counter -1] = malloc(sizeof(commands[i].programs[j]));
                            strcpy(to_exec[programs_counter-1],commands[i].programs[j]);
                        }
                    }
                }
                command = strtok(NULL,"|");
            }
            execute_command(to_exec,programs_counter);
            free(command);
            free(to_exec);
        }

    }
    fclose(fp);
    free(line);
    free_commands_memory(commands,commands_counter);
    free(commands);
}