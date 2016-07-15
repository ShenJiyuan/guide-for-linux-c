/*
  Application: Make a new copy of an existing file using the MyCopy.
  
  Descriptions: This program create a new process(child process) to copy the files using MyCopy.

  You can use the program for coping as the following instructions:
  (1) open your terminal
  (2) input: ./project1/ForkCopy ***(your open file) ***(your write file)

  The program will finally copy a  file, and return the time in your terminal.
*/

#include "cse356header.h"

int main(int argc,char** argv)
{
   //time
   clock_t start,end;
   double elapsed;
   start=clock();

   //-------fork the child process-------
      pid_t FORKPID;
      FORKPID=fork();
   //---------switch for fork------------
      switch(FORKPID){
        //-1,fork failure
        case -1:
            printf("ERROR:Failed to fork.\n");break;
        // 0,child process
        case 0:
            execlp("./project1/MyCopy",argv[0],argv[1],argv[2],NULL);break;
        //>0,parent process
        default:           
            wait(NULL);//time
            end=clock();
            elapsed=((double)(end-start))/CLOCKS_PER_SEC*1000;
            printf("Time used:%f millisecond\n",elapsed);
     }
          
  return 0;
}


