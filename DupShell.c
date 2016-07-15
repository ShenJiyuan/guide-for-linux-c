/*
   Application:Add to the mini-shell ability to execute command lines with commands connectedby pipes.
   
   You can use the program for coping as the following instructions:
  (1) open your terminal
  (2) input: ./project1/DupShell
  (3) you can exit the mini shell by inputing "exit"
  (4) testing example: "ls -l | wc"
*/

#include "cse356header.h"

//*****************function:parseLine*************************
int parseLine(char * command, char * command_array[]) { 
    char * p; 
    int count = 0; 
    p = strtok(command, " "); 
    while (p && strcmp(p,"|") != 0) { 
        command_array[count] = p; 
        count++; 
        p = strtok(NULL," "); 
    } 
    command_array[count]='\0';
    return count; 
}
 
//********************function:main***************************
int main(){
    //------------definition-------------
    char cmd[512]={0};        //command
    char *line[2];
    char *command_arrayA[10];
    char *command_arrayB[10];
    int pipe_flag=0;          //if there has a "|"
    int mypipe[2];
    int i=0;

    //------------while loop-------------
    while (1) {  
        //---------initialize--------------------- 
        pipe_flag = 0;
        line[0]=NULL;
        line[1]=NULL;
        printf("Dupshell > "); 

        //-----------get command------------------              
        if (fgets(cmd, sizeof(cmd), stdin)==NULL){
            printf("\n");
            continue;
        }
        int i=0;
        cmd[strlen(cmd)-1]='\0';  

        //---------check for pipe_flag----------- 
        while(i<strlen(cmd)){
            if(cmd[i]=='|'){
                if (i==0){
                    printf("\nError, unknown commande0.\n"); //| wc
                    return 0;
                }
                else{
                    pipe_flag=1;
                }
            }
            i++;
            //printf("pipe_flag=%d",pipe_flag);
        }

        //--------check for exit command---------
        if (!strcmp(cmd, "exit")){
            break;
        } 

        //---------line[0] & line[1]-------------
        line[0]=strtok(cmd, "|");
        //line[0][strlen(line[0])-1]='\0';
        line[1]=strtok(NULL, "|");
        //line[1][strlen(line[1])-1]='\0';
        if(pipe_flag && line[1]==NULL){
            printf("\nError, unknown commande1.\n");//ls |
            return 0;   
        }
         
        //----------use pipe when there's parsing command---------------------
        if(pipe_flag){
            if(pipe(mypipe)){   
                printf("Error\n");
                return 1;
            }
            int pidA=fork(); //fork child A    
            if(pidA==0){
                dup2(mypipe[1],STDOUT_FILENO);
                close(mypipe[0]);
                close(mypipe[1]); 
                //handle the command before pipe character
                parseLine(line[0], command_arrayA);
                if (execvp(command_arrayA[0],command_arrayA) == -1)
                    printf("Error: running command: '%s'\n",command_arrayA[0]);
                exit(0);
            }
            int pidB=fork();//fork child B
            if(pidB==0){
                dup2(mypipe[0],STDIN_FILENO);
                close(mypipe[1]);
                close(mypipe[0]);
                //handle the command after pipe character
                parseLine(line[1], command_arrayB);
                if (execvp(command_arrayB[0],command_arrayB) == -1)
                    printf("Error: running command: '%s'\n",command_arrayB[0]);
                //fflush(NULL);
                exit(0);
            }
            else{
                close(mypipe[1]);
                close(mypipe[0]);
                waitpid(pidA, NULL, 0);
                waitpid(pidB, NULL, 0);
                                 
            }
        }
        //fork a child when there's no parsing
        else{ 
            int pid=fork();
            switch (pid){
                case -1:
                    perror("Failed to fork\n");
                    break;
                case 0:
                    parseLine(cmd, command_arrayA);
                     
                    if (execvp(command_arrayA[0],command_arrayA) == -1){
                        printf("Error: running command: '%s'\n",command_arrayA[0]);
                    exit(0);
                    }
                default:
                    waitpid(pid, NULL, 0);
            }
        } 
         
         
    }
    return 0;
}
