/*
    application:a concurrent multi-threaded solution to solve the Burger Buddies Problem.

    you can use the solution as the following syscall:
    ./BBC cook_num cashier_num customer_num rack_size
*/

#include"cse356header.h"
#include <semaphore.h>
#include <pthread.h>

//=====================variables and semaphores==================
int cook_num,cashier_num,customer_num,rack_num;
int count_customer=0,count_burger=0;
sem_t rack,burger,cashier;
sem_t customer_mutex,wake,cashier_mutex;  
//========================functon1:cook_thread===================
void *Cook_Thread(void* args)
{
	int id=*(int*)args;
	while(count_customer<=customer_num)
	{
		sem_wait(&rack);
		sleep(rand()%5);//make the burger
                if(count_burger<=customer_num)	
                    printf("Cook [%d] makes a burger.\n",id);
		count_burger++;                
		sem_post(&burger);
		if(count_customer>customer_num)
			break;
	}
	sem_post(&burger);
	pthread_exit(NULL);	
}
//======================function2:cashier_thread=================
void *Cashier_Thread(void *args)
{
	int id=*(int*)args;
	while(count_customer<=customer_num)
	{
		sem_wait(&wake);
	        if(count_customer<=customer_num)
			printf("Cashier [%d] accepts an order.\n",id);
		sem_wait(&cashier_mutex);
		sem_wait(&burger);
		sem_post(&cashier_mutex);
		if(count_customer<=customer_num)
			printf("Cashier [%d] takes a burger to customer.\n",id);
		count_customer++;
		sem_post(&rack);
		if(count_customer==customer_num)
			break;
	}
	if(count_customer==customer_num)
		count_customer++;
	sem_post(&wake);
	sem_post(&burger);
	pthread_exit(NULL);
}
//======================function3:customer_thread====================
void *Customer_Thread(void *args)
{	
	int id=*(int*)args;
	sleep(rand() % 5);	
	printf("Customer [%d] comes.\n",id);
	sem_post(&wake);
	sem_wait(&customer_mutex);
	sem_wait(&cashier);
	sem_post(&customer_mutex);

	sem_post(&cashier);
	pthread_exit(NULL);
}

//==========================main function=========================
int main(int argc,char *argv[])
{
    //***************check input form validity********************
    if (argc < 5)
    {
        printf("Error:The arguments is not enough!\n");
        exit(-1);
    }

    if (argc > 5)
    {
        printf("Error:More arguments than need!\n");
        exit(-1);
    }
    //***************get and check input data******************
    cook_num=atoi(argv[1]);
    cashier_num=atoi(argv[2]);
    customer_num=atoi(argv[3]);
    rack_num=atoi(argv[4]);
    srand((unsigned) time(NULL));
    
    if(cook_num<1) 
    {
         printf("Error:The number of Cooks isn't logical!\n");
         exit(-1);
    }
    if(cashier_num<1) 
    {
         printf("Error:The number of Cashiers isn't logical!\n");
         exit(-1);
    }
    if(customer_num<1) 
    {
         printf("Error: The number of Customers isn't logical!\n");
         exit(-1);
    }
    if(rack_num<1)
    { 
         printf("Error: The number of Racks isn't logical!\n");
         exit(-1);
    }   
    //*************initialize the semaphore***************************
    sem_init(&burger,0,0);
    sem_init(&cashier,0,cashier_num);
    sem_init(&rack,0,rack_num);
    sem_init(&customer_mutex,0,1);
    sem_init(&cashier_mutex,0,1);
    sem_init(&wake,0,0);
    
    //****************create ID arrays********************************
    int *id_cook=(int *) malloc(sizeof(int)*cook_num);
    int *id_cashier=(int *) malloc(sizeof(int)*cashier_num);
    int *id_customer=(int *) malloc(sizeof(int)*customer_num);
    
    //******************assigning ID**********************************
    int i;
    for(i=0;i<cook_num;++i)
    {
        id_cook[i]=i+1;
    }
    for(i=0;i<cashier_num;++i)
    {
        id_cashier[i]=i+1;
    }
    for(i=0;i<customer_num;++i)
    {
        id_customer[i]=i+1;
    }

    //******************create threads********************************
    printf("Cooks[%d], Cashiers[%d], Customers[%d]\nBegin run.\n", cook_num, cashier_num, customer_num);
    pthread_t *cookt = (pthread_t *) malloc(sizeof(pthread_t) * cook_num);
    pthread_t *customert = (pthread_t *) malloc(sizeof(pthread_t) * customer_num);
    pthread_t *cashiert = (pthread_t *) malloc(sizeof(pthread_t) * cashier_num);
    int rc;
    //create cook threads
    for(i=0;i<cook_num;++i)
    {
        rc=pthread_create(&cookt[i], NULL, Cook_Thread, &id_cook[i]);
	if (rc)
	{
	    printf("ERROR:return code from pthread_create(cookt[%d]) is %d.\n",i,rc);
	    exit(-1);
	}
    }
    //create cashier threads
    for(i=0;i<cashier_num;++i)
    {
	rc=pthread_create(&cashiert[i], NULL, Cashier_Thread, &id_cashier[i]);
	if (rc)
	{
	    printf("ERROR:return code from pthread_create(cashiert[%d]) is %d.\n",i,rc);
	    exit(-1);
	}
     }
    //create customer threads
    for(i=0;i<customer_num;++i)
    {
	rc=pthread_create(&customert[i], NULL, Customer_Thread, &id_customer[i]);
	if (rc)
	{
	    printf("ERROR:return code from pthread_create(customert[%d]) is %d.\n",i,rc);
	    exit(-1);
        }
    }
    //*******************join threads and wait for completion******************
    for (i=0;i<cook_num;++i)
    {
        rc = pthread_join(cookt[i], NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_join(cookt[%d]) is %d.\n", i, rc);
            exit(-1);
        }
    }

    for (i=0;i<cashier_num;++i)
    {
        rc = pthread_join(cashiert[i], NULL);
        if (rc) 
        {
            printf("ERROR; return code from pthread_join(cashiert[%d]) is %d.\n", i, rc);
            exit(-1);
        }
    }

    for (i=0;i<customer_num; ++i)
    {
        rc = pthread_join(customert[i], NULL);
        if (rc) 
        {
            printf("ERROR; return code from pthread_join(customert[%d]) is %d.\n", i, rc);
            exit(-1);
        }
    }

    //*********************destroy semaphores******************************
    sem_destroy(&cashier);
    sem_destroy(&rack);
    sem_destroy(&burger);
    sem_destroy(&customer_mutex);
    sem_destroy(&wake);
    sem_destroy(&cashier_mutex);

   return 0;
}
