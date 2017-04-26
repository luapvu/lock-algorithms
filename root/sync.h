/*
 * NAME, etc.
 *
 *
 * sync.h
 */

#ifndef _my_SYNC1_H_
#define _my_SYNC1_H_

#include "atomic_ops.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct my_mutex_struct {
  /* FILL ME IN! */
  long delay; //specifies how long the thread should sleep for, random for each thread
  int failCount; //keeps track of how many times the thread fails in getting the lock, used to exponentially increase sleep time

  volatile long lockValue; //keep track of current lock status, 0 for free 1 for being used
  pthread_t ownerID; //the threadID which currently owns the lock
  int counter; //counter used for recurssive lock calls by the same thread
  int state; //used to check if the lock value has been changed for TTAS, 0 for free lock and 1 for occupied

};




typedef struct my_mutex_struct my_mutex_t;


int my_mutex_init(my_mutex_t *lock);
int my_mutex_unlock(my_mutex_t *lock);
int my_mutex_destroy(my_mutex_t *lock);

int my_mutex_lock(my_mutex_t *lock);
int my_mutex_trylock(my_mutex_t *lock);



/*Spinlock Starts here*/

struct my_spinlock_struct {
  /* FILL ME IN! */

    volatile long lockValue; //keep track of current lock status, 0 for free 1 for being used
    pthread_t ownerID; //the threadID which currently owns the lock
    int counter; //counter used for recurssive lock calls by the same thread
    int state; //used to check if the lock value has been changed for TTAS, 0 for free lock and 1 for occupied
};

typedef struct my_spinlock_struct my_spinlock_t;

int my_spinlock_init(my_spinlock_t *lock);
int my_spinlock_destroy(my_spinlock_t *lock);
int my_spinlock_unlock(my_spinlock_t *lock);

int my_spinlock_lockTAS(my_spinlock_t *lock);
int my_spinlock_lockTTAS(my_spinlock_t *lock);
int my_spinlock_trylock(my_spinlock_t *lock);


/*queuelock Starts here*/

struct my_queuelock_struct {
  /* FILL ME IN! */
  volatile long ticketNumber; //ticket number for threads to grab, should be atomic process and every thread has their own number
  int nowServingNumber; //current owner of the lock should have this ticket number

  pthread_t ownerID; //the threadID which currently owns the lock
  int counter; //counter used for recurssive lock calls by the same thread


};

typedef struct my_queuelock_struct my_queuelock_t;

int my_queuelock_init(my_queuelock_t *lock);
int my_queuelock_destroy(my_queuelock_t *lock);
int my_queuelock_unlock(my_queuelock_t *lock);

int my_queuelock_lock(my_queuelock_t *lock);
int my_queuelock_trylock(my_queuelock_t *lock);

#endif
