#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>
#include "sync.h"
#include <string.h>

long long c = 0;
int numThreads;
int numItterations;
int workOutsideCS;
int workInsideCS;
int testID;

pthread_mutex_t count_mutex;
pthread_spinlock_t count_pthreadSpinLock;
my_spinlock_t mySpinLock;
my_mutex_t myMutexLock;
my_queuelock_t myQueueLock;


unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}
