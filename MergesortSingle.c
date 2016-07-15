/*
  Application: Mergesort algorithm implemented by using a single threaded program.
   
   You can use the program for mergesort as the following instructions:
  (1) open your terminal
  (2) input: ./project1/MergesortSingle ***(your open file) ***(your write file)

   The program will read your open file, mergesort your readin data and put the ordered data in your write file. 
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


//*************************main function**************************************
int main(int argc,char**argv)
{
   //time
   clock_t start,end;
   double elapsed;
   start=clock();

   //---------------create a thread 't1'----------------------
   pthread_t t1;
   pthread_attr_t attr1;
   pthread_attr_init(&attr1);
   pthread_attr_setscope(&attr1,PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr1,PTHREAD_CREATE_JOINABLE);
   
   //--------------read file and get data---------------------
   int data[50];
   FILE *src;
   src=fopen("./project1/data.in","r");
   char ch;
   int i=0,j=0;
   int len;
   while ((ch=getc(src))!=EOF){
      if (i==0) {len=ch-'0';i++;}
      else {
           if ((ch=getc(src))!=EOF)
             { data[j]=ch-'0';j++;}
           }
   }
   fclose(src);

   //----------------launch a thread--------------------------
   struct thread_arguments my_arguments;
   int m;
   for (m=0;m<len;m++) my_arguments.array[m]=data[m];
   my_arguments.first_paramter=0;
   my_arguments.second_paramter=len-1;
   int rc;
   rc=pthread_create(&t1,&attr1,MergeSort,&my_arguments);
   if (rc) {
      printf("ERROR;return code from pthread_create(t1) is %d\n",rc);
      exit(-1);
   }
   
   //------------wait on a thread's completion----------------
   //void* status1;
   rc=pthread_join(t1,NULL);
   if (rc){
      printf("ERROR;return code from pthread_join(t1) is %d\n",rc);
      exit(-1);
   }

  //for (i=0;i<len;i++)
  //    printf("%d",my_arguments.array[i]);

   char c=' ';char tm;
   FILE *dest;
   dest=fopen("./project1/data.out","w+");
   int t;
   tm=len+'0';
   fputc(tm,dest);
   for (t=0;t<len;t++){
      fputc(c,dest);
      tm=my_arguments.array[t]+'0';
      fputc(tm,dest);
   }
   fclose(dest);

   //time
   end=clock();
   elapsed=((double)(end-start))/CLOCKS_PER_SEC*1000;
   printf("Time used:%f millisecond\n",elapsed);

   return 0;
}
