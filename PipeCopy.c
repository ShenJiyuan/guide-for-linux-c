/*
  Application: Make a new copy of an existing file using pipe system calls.

  Descriptions:This program uses pipe system call.At first, I created a pipe. And then some variables , child process identifier and file pointers are defined. In the child process, it will write the target file to pipe. In the parent process, it will fork a new child process for reading from the pipe and creating the copy file,, and the new parent process will just wait.

  You can use the program for coping as the following instructions:
  (1) open your terminal
  (2) input: ./project1/PipeCopy ***(your open file) ***(your write file)

  The program will finally copy a  file, and return the time in your terminal.

*/

#include "cse356header.h"

int main(int argc,char** argv)
{
   //time
   clock_t start,end;
   double elapsed;
   start=clock();

   //--------create a  pipe---------
   int mypipe[2];
   if (pipe(mypipe)){
      fprintf(stderr,"Pipe failed.\n");
      return -1;
   }
   //-------name definitions--------
   pid_t FORKPID;
   FORKPID=fork();
   char ch;
   FILE *src;
   FILE *dest;
   //-------switch for fork---------
   switch(FORKPID){
        //-1,fork failure
        case -1:
            printf("ERROR:Failed to fork.\n");break;
        // 0,child process
        case 0:
            close(mypipe[0]);
            src=fopen(argv[1],"r");
            if (src==NULL){
                printf("ERROR:Could not open file '%s'.\n",argv[1]);
                exit(-1);
            }
            while((ch=getc(src))!=EOF)
                write(mypipe[1],&ch,1);
            fclose(src);
            close(mypipe[1]);
            break;
        //>0,parent process
        default:
            FORKPID=fork();
           
            switch(FORKPID){
                //-1,fork failure
                case -1:
                     printf("ERROR:Failed to fork.\n");break;
                // 0,child process
                case 0:
                     close(mypipe[1]);
                     dest=fopen(argv[2],"w+");
                     if (dest==NULL){
                        printf("ERROR:Could not open file '%s'.\n",argv[2]);
                        exit(-1);
                     }  
                     while(read(mypipe[0],&ch,1)>0)
                        fputc(ch,dest);
                     fclose(dest);
                     close(mypipe[0]);
                     break;
                //>0,parent process
                default:
                     wait(NULL); 
                     //time
                     end=clock();
                     elapsed=((double)(end-start))/CLOCKS_PER_SEC*1000;
                     printf("Time used:%f millisecond\n",elapsed);
            }
   }

  

  return 0;
}
