/*
   Application: A shell-like program used to handle just "argument-less" commands,such as ls and date.
   
   You can use the program for coping as the following instructions:
  (1) open your terminal
  (2) input: ./project1/MyShell
  (3) you can exit the mini shell by inputing "exit"
*/

#include "cse356header.h"

//***********************function:parsecmd**************************
char parsecmd(char*s)
{
   if (!strncasecmp(s,"exit",4)) return 'T';
   else if (!strncasecmp(s,"ls",2)) return 1;
   else if (!strncasecmp(s,"date",4)) return 2;
   else return 0;
}

//***********************function:get_time**************************
void get_time(){
   struct tm *Tm;
   time_t t;

   time(&t);
   Tm=localtime(&t);

   printf("%d %d %d %d:%d:%d EST %d",Tm->tm_wday,Tm->tm_mon,Tm->tm_mday,Tm->tm_hour,Tm->tm_min,Tm->tm_sec,Tm->tm_year+1900);
   printf("\n");
}

//************************function:main*****************************
int main(int argc,char** argv)
{   
   char cmd[1024];
   char b='F';

    while (1){
        memset(cmd,0,1024);
        //-----------provide a prompt---------
        printf("myshell>");
        //----------receive command(1000)-----
        fgets(cmd,1000,stdin);
        //----------remove 'enter'------------
        cmd[strlen(cmd)-1]=0;
        //----------parse command-------------
        b=parsecmd(cmd);
        if (b=='T') return 1;
        switch(b){
            case 1: /*ls*/ system("ls");break;
            case 2: /*date*/get_time();break;
            case 0: /*illegal*/break;
        }
    }
    return 1;
}


