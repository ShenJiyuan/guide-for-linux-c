/*
   Application: Mergesort algorithm implemented by using multiple threaded program.
   
   You can use the program for mergesort as the following instructions:
  (1) open your terminal
  (2) input: ./project1/MergesortMulti 

   The program will read file "./project1/data.in", mergesort your readin data and put the ordered data in the file "./project1/data.out". 

*/
#include "cse356header.h"
#include <pthread.h>


//*********************the structure of argument******************************
struct thread_arguments
{
   int array[50];
   int first_paramter;
   int second_paramter;
};
//*********************the structure of argument_read*************************
struct read
{
   int array[50];
   int len;
};

//*************************function1:merge*************************************
void merge (int a[],int left,int mid,int right)
{
   char tmp[right-left+1];
   int i=left,j=mid,k=0;
   while (i<mid && j<=right)
      if (a[i]<a[j]) tmp[k++]=a[i++];
      else tmp[k++]=a[j++];
   while (i<mid) tmp[k++]=a[i++];
   while (j<=right) tmp[k++]=a[j++];
   for (i=0,k=left;k<=right;) a[k++]=tmp[i++];
}

//***********************function2:mergeSort***********************************
void mergeSort(int a[],int left,int right)
{
   int mid=(left+right)/2;
   if (left==right) return;
   mergeSort(a,left,mid);
   mergeSort(a,mid+1,right);
   merge(a,left,mid+1,right);
}

//***********************function3:MergeSort***********************************
void *MergeSort(void* args)
{
   struct thread_arguments *a;
   a=(struct thread_arguments*) args;
   mergeSort(a->array,a->first_paramter,a->second_paramter);
   pthread_exit(NULL);
}
//***********************function4:read file**********************************
void *readFile(void* args)
{ 
   struct read * a;
   a=(struct read*) args;
   FILE *src;
   src=fopen("./project1/data.in","r");
   char ch;
   int i=0,j=0;
   while ((ch=getc(src))!=EOF){
      if (i==0) {a->len=ch-'0';i++;}
      else {
           if ((ch=getc(src))!=EOF)
             { a->array[j]=ch-'0';j++;}
           }
   }
   fclose(src);
}
//************************function:write file*********************************
void *writeFile(void* args)
{
   struct thread_arguments *a;
   a=(struct thread_arguments*) args;
   char c=' ';char tm;
   FILE *dest;
   dest=fopen("./project1/data.out","w+");
   int t;
   tm=a->second_paramter+1+'0';
   fputc(tm,dest);
   for (t=0;t<a->second_paramter+1;t++){
      fputc(c,dest);
      tm=a->array[t]+'0';
      fputc(tm,dest);
   }
   fclose(dest);
}

//*************************main function**************************************
int main(int argc,char**argv)
{
   //time
   clock_t start,end;
   double elapsed;
   start=clock();
//==============================================================================
   //---------------create a thread 't1'----------------------
   pthread_t t1;
   pthread_attr_t attr1;
   pthread_attr_init(&attr1);
   pthread_attr_setscope(&attr1,PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr1,PTHREAD_CREATE_JOINABLE);
   //---------------create a thread 't2'----------------------
   pthread_t t2;
   pthread_attr_t attr2;
   pthread_attr_init(&attr2);
   pthread_attr_setscope(&attr2,PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr2,PTHREAD_CREATE_JOINABLE);
   //---------------create a thread 't3'----------------------
   pthread_t t3;
   pthread_attr_t attr3;
   pthread_attr_init(&attr3);
   pthread_attr_setscope(&attr3,PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr3,PTHREAD_CREATE_JOINABLE);

//===========================================================================   
   //--------------read file thread---------------------
   struct read readd;
   int rq;
   rq=pthread_create(&t2,&attr2,readFile,&readd);
   if (rq) {
      printf("ERROR;return code from pthread_create(t1) is %d\n",rq);
      exit(-1);
   }
   rq=pthread_join(t2,NULL);
   if (rq){
      printf("ERROR;return code from pthread_join(t1) is %d\n",rq);
      exit(-1);
   }

//==============================================================================
   //----------------mergesort thread--------------------------
   struct thread_arguments my_arguments;
   int m;
   for (m=0;m<readd.len;m++) my_arguments.array[m]=readd.array[m];
   my_arguments.first_paramter=0;
   my_arguments.second_paramter=readd.len-1;
   int rc;
   rc=pthread_create(&t1,&attr1,MergeSort,&my_arguments);
   if (rc) {
      printf("ERROR;return code from pthread_create(t1) is %d\n",rc);
      exit(-1);
   }
   rc=pthread_join(t1,NULL);
   if (rc){
      printf("ERROR;return code from pthread_join(t1) is %d\n",rc);
      exit(-1);
   }

//===========================================================================   
   //--------------write file thread---------------------
   int rw;
   rw=pthread_create(&t3,&attr3,writeFile,&my_arguments);
   if (rw) {
      printf("ERROR;return code from pthread_create(t1) is %d\n",rw);
      exit(-1);
   }
   rw=pthread_join(t3,NULL);
   if (rw){
      printf("ERROR;return code from pthread_join(t1) is %d\n",rw);
      exit(-1);
   }

//==============================================================================
   //time
   end=clock();
   elapsed=((double)(end-start))/CLOCKS_PER_SEC*1000;
   printf("Time used:%f millisecond\n",elapsed);

   return 0;
}
