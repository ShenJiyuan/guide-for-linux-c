/*
  Application:Make the mini-shell a little more powerful by allowing arguments to the commands.

  You can use the program for coping as the following instructions:
  (1) open your terminal
  (2) input: ./project1/MoreShell
  (3) you can exit the mini shell by inputing "exit"
  
*/

#include "cse356header.h"

#define MAXLINE 80

//****************function:parseline*******************
int parseline(char *line,char *command_array[])
{
   char *p;
   int count=0;
   p=strtok(line," ");
   while (p&&strcmp(p,"|")!=0){
       command_array[count]=p;
       count++;
       p=strtok(NULL," ");
   }
   return count;
}

//*****************function:main***********************
int main(void)
{
   //--------definitions----------
   char line[MAXLINE];
   char*command_array[MAXLINE/2+1];
   pid_t FORKPID;   

   while (1){
      //------prompt---------
      memset(line,0,MAXLINE);
      printf("myshell>");
      //------input---------
      fgets(line,MAXLINE,stdin);
      line[strlen(line)-1]=0;
      //------initialize command_array-----------------
      int i;
      for (i=0;i<(MAXLINE/2+1);i++) command_array[i]=0;
      //------parse command----------------------------
      int count=parseline(line,command_array);
      command_array[count++]=NULL;
      
      //------determine command------------------------
      if (!strncasecmp(line,"exit",4)) return 1;
      else if (count>0){
         FORKPID=fork();
         switch(FORKPID){
           //-1,fork failure
           case -1:
               printf("ERROR:Failed to fork.\n");break;
           // 0,child process
           case 0:
               if (execvp(line,command_array)==-1){
                    printf("ERROR:running command:'%s'\n",line);
                    exit(0);
               }
               //---------------execute command-----------------
               execvp(line,command_array);
               perror("execvp failed");
               break;
           //>0,parent process
           default:
               wait(NULL);                       
         }
      }   
   }

   return 1;
}
