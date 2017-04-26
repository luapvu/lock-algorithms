#include "main.h"

//Generic function for a thread to do work.
void *fnC()
{
    int i;
    for(i=0;i<1000000;i++)
    {
        c++;
    }
}


pthread_mutex_t count_mutex;
void *pthreadMutexTest()
{

    int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

		pthread_mutex_lock(&count_mutex);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		pthread_mutex_unlock(&count_mutex);

    }


}

void *pthreadSpinLockTest()
{

  int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

		pthread_spin_lock(&count_pthreadSpinLock);
		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
		pthread_spin_unlock(&count_pthreadSpinLock);

    }


}


void *spinLockTASTest() //Spin lock TAS
{

  int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

    //showing that a recursive call to the lock under the same thread does not
    //deadlock as long as there are an equal amout of unlocks
    my_spinlock_lockTAS(&mySpinLock);
  //  my_spinlock_lockTAS(&mySpinLock);
  //  my_spinlock_lockTAS(&mySpinLock);

		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
    //equal amout of unlocks as there are locks, to show recursive lock functionality
    my_spinlock_unlock(&mySpinLock);
  //  my_spinlock_unlock(&mySpinLock);
//    my_spinlock_unlock(&mySpinLock);

    }


}



void *spinLockTTASTest() //Spin lock TTAS
{

  int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

    //showing that a recursive call to the lock under the same thread does not
    //deadlock as long as there are an equal amout of unlocks
    my_spinlock_lockTTAS(&mySpinLock);
  //  my_spinlock_lockTTAS(&mySpinLock);
  //  my_spinlock_lockTTAS(&mySpinLock);

		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
    //equal amout of unlocks as there are locks, to show recursive lock functionality
    my_spinlock_unlock(&mySpinLock);
  //  my_spinlock_unlock(&mySpinLock);
  //  my_spinlock_unlock(&mySpinLock);

    }


}

void *mutexLockTest() //Exponential Backoff lock test
{

  int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

    //showing that a recursive call to the lock under the same thread does not
    //deadlock as long as there are an equal amout of unlocks
    my_mutex_lock(&myMutexLock);
  //  my_mutex_lock(&myMutexLock);
  //  my_mutex_lock(&myMutexLock);

		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
    //equal amout of unlocks as there are locks, to show recursive lock functionality
    my_mutex_unlock(&myMutexLock);
//    my_mutex_unlock(&myMutexLock);
//    my_mutex_unlock(&myMutexLock);

    }


}

void *queueLockTest() //Queue lock test
{

  int i;
	int j;
	int k;

	int localCount = 0;

    for(i=0;i<numItterations;i++)
    {

		for(j=0;j<workOutsideCS;j++)/*How much work is done outside the CS*/
		{
			localCount++;
		}

    //showing that a recursive call to the lock under the same thread does not
    //deadlock as long as there are an equal amout of unlocks
    my_queuelock_lock(&myQueueLock);
  //  my_queuelock_lock(&myQueueLock);
  //  my_queuelock_lock(&myQueueLock);



		for(k=0;k<workInsideCS;k++)/*How much work is done inside the CS*/
		{
			c++;
		}
    //equal amout of unlocks as there are locks, to show recursive lock functionality
    my_queuelock_unlock(&myQueueLock);
//    my_queuelock_unlock(&myQueueLock);
//    my_queuelock_unlock(&myQueueLock);



    }


}



int runTest(int testID)
{

/*You must create all data structures and threads for you experiments*/
my_spinlock_init(&mySpinLock);
my_mutex_init(&myMutexLock);
my_queuelock_init(&myQueueLock);

printf("\n");
printf("Test results are shown below... \n");
printf("\n");

if (testID == 0 || testID == 1 ) /*Pthread Mutex*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &pthreadMutexTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Pthread Mutex Lock Test: \n");
	printf("Total Count: %llu\n", c);
	result=timespecDiff(&stop,&start);
	printf("Time elapsed(ms): %llu\n",result/1000000);
  printf("\n");

}

if(testID == 0 || testID == 2) /*Pthread Spinlock*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &pthreadSpinLockTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("Pthread Spinlock Test: \n");
	printf("Total Count: %llu\n", c);
	result=timespecDiff(&stop,&start);
  printf("Time elapsed(ms): %llu\n",result/1000000);
  printf("\n");

}

if(testID == 0 || testID == 3) /*MySpinlockTAS*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &spinLockTASTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("TAS Lock Test: \n");
	printf("Total Count: %llu\n", c);
	result=timespecDiff(&stop,&start);
  printf("Time elapsed(ms): %llu\n",result/1000000);
  printf("\n");

}

/*....you must implement the other tests....*/
if(testID == 0 || testID == 4) /*MySpinlockTTAS*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &spinLockTTASTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

  printf("TTAS Lock Test: \n");
	printf("Total Count: %llu\n", c);
	result=timespecDiff(&stop,&start);
  printf("Time elapsed(ms): %llu\n",result/1000000);
  printf("\n");

}

if(testID == 0 || testID == 5) /*MyMutexLock*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &mutexLockTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);
  printf("Mutex Lock Test: \n");
	printf("Total Count: %llu\n", c);
	result=timespecDiff(&stop,&start);
  printf("Time elapsed(ms): %llu\n",result/1000000);
  printf("\n");

}


if(testID == 0 || testID == 6) /*queueLockTest*/
{
	c=0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit integer

	pthread_t *threads = (pthread_t* )malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(i=0;i<numThreads;i++)
	{

	 if( rt=(pthread_create( threads+i, NULL, &queueLockTest, NULL)) )
	{
		printf("Thread creation failed: %d\n", rt);
		return -1;
	}

	}

	for(i=0;i<numThreads;i++) //Wait for all threads to finish
	{
		 pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);
  printf("Busy Wait Queue Lock Test: \n");
	printf("Total Count: %llu\n", c);
	result=timespecDiff(&stop,&start);
  printf("Time elapsed(ms): %llu\n",result/1000000);
  printf("\n");

}



	return 0;
}

int testAndSetExample()
{
volatile long test = 0; //Test is set to 0
printf("Test before atomic OP:%zu\n",test);
tas(&test);
printf("Test after atomic OP:%zu\n",test);
}

int processInput(int argc, char *argv[])
{

/*testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock*/
/*You must write how to parse input from the command line here, your software should default to the values given below if no input is given*/

      //initialize values to default values.
      numThreads=4;
      numItterations=1000000;
      testID=0;
      workOutsideCS=0;
      workInsideCS=1;

      int val = 0; //parse input command lines
      while((val = getopt(argc, argv,  "t:i:o:c:d:")) != -1){ //getopt is a built in command to parse for flags, val will hold the parsed flag
        if(val == 't'){ // specifies number of threads
          numThreads = atoi(optarg); //optarg holds the argument corresponding to the flag

        }
        if(val == 'i'){ //specifies number of iterations
          numItterations = atoi(optarg);//optarg holds the argument corresponding to the flag

        }
        if(val == 'o'){ //specifies number of operations on the outside
          workOutsideCS = atoi(optarg);//optarg holds the argument corresponding to the flag

        }
        if(val == 'c'){ //specifies number of operations on the inside
          workInsideCS = atoi(optarg);//optarg holds the argument corresponding to the flag

        }
        if(val == 'd'){ //specifies test case number
          testID = atoi(optarg);//optarg holds the argument corresponding to the flag

        }

      }

      printf("---------------------------------------------------\n");
      if(testID == 0){ //pthreadMutex
        printf("Default execution, performing ALL tests:\n");

      }else if(testID == 1){ //pthreadMutex
        printf("Executed pthreadMutex:\n");

      }else if(testID == 2){ //pthreadSpinLock
        printf("Executed pthreadSpinLock:\n");

      }else if(testID == 3){ // mySpinLockTAS
        printf("Executed mySpinLockTAS:\n");

      }else if(testID == 4){  // mySpinLockTTAS
        printf("Executed mySpinLockTTAS: \n");

      }else if(testID == 5){  // myMutexTAS
        printf("Executed myMutexTAS: \n");

      }else if(testID == 6){ // myQueueLock
        printf("Executed myQueueLock (Busy wait): \n");

      }else{
        //if testID != 0-6 or there are not enough arguments / invalid arguments, we will run the base case execution
        printf("Default execution, performing ALL tests:\n");
      }

    printf("\n");
    printf("Test ID:         %d \n", testID);
    printf("# Threads:       %d \n", numThreads);
    printf("# Iterations:    %d \n", numItterations);
    printf("# workOutsideCS: %d \n", workOutsideCS);
    printf("# workInsideCS:  %d \n", workInsideCS);
    printf("\n");
    printf("---------------------------------------------------\n");

    return 0;
}


int main(int argc, char *argv[])
{
  printf("\n");
  printf("|| INSTRUCTIONS || \n");
	printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock, \n");
  printf("Example: ./mylocks -t 2 -i 10 -o 1000000 -c 1 -d 0 \n ");
  printf("\n");

	//testAndSetExample(); //Uncomment this line to see how to use TAS
  my_spinlock_init(&mySpinLock);
  my_mutex_init(&myMutexLock);
  my_queuelock_init(&myQueueLock);
  pthread_spin_init(&count_pthreadSpinLock, PTHREAD_PROCESS_PRIVATE);

	processInput(argc,argv);
	runTest(testID);

  return 0;

}
