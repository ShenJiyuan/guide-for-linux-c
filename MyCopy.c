/*
  Application:Make a new copy of an existing file using system calls.

  You can use the program for coping as the following instructions:
  (1) open your terminal
  (2) input: ./project1/MyCopy ***(your open file) ***(your write file)
  
  The program will finally copy a  file, and return the time in your terminal.
*/

#include "cse356header.h"

int main(int argc,char**argv)
{

   //time
   clock_t start,end;
   double elapsed;
   start=clock();
   

   //---------open read file---------
   FILE *src;
   src=fopen(argv[1],"r");
   if (src==NULL){
      printf("ERROR:Could not open file '%s'.\n",argv[1]);
      exit(-1);
   }
   //---------open write file--------
   FILE *dest;
   dest=fopen(argv[2],"w+");
   if (dest==NULL){
      printf("ERROR:Could not open file '%s'.\n",argv[2]);
      exit(-1);
   }
   //--------read & write ops--------
   char ch;
   while ((ch=getc(src))!=EOF)
      fputc(ch,dest);
   //--------close files-------------
   fclose(src);
   fclose(dest);


   //time
   end=clock();
   elapsed=((double)(end-start))/CLOCKS_PER_SEC*1000;
   printf("Time used:%f millisecond\n",elapsed);

   return 0;
}
