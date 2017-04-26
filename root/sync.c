
#define _REENTRANT

#include "sync.h"
#include <stdio.h>

/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock) //set all values to zero
{
  lock->state = 0;
  lock->counter = 0;
  lock->ownerID =  0;
  lock->lockValue = 0;
}

int my_spinlock_destroy(my_spinlock_t *lock) //reset all values
{
  lock->state = 0;
  lock->counter = 0;
  lock->ownerID =  0;
  lock->lockValue = 0;
}

int my_spinlock_unlock(my_spinlock_t *lock)
{

  lock->counter--; //decrement the lock counter

  if(lock->counter == 0){ //if the counter is not zero, it means multiple recursive calls were used to obtain the lock by the same thread, so don't fully unlock
    lock->ownerID = 0; //only fully unlock when the calling thread has no outstanding calls to the lock, then set everything to 0 for the next thread.
    lock->state = 0;
    lock->lockValue = 0;
  }

}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{

  if(lock->ownerID == pthread_self()){ //if the calling thread already owns the lock, no need to tas, just increment the counter.
    //do nothing
    lock->counter++;
    return 0;
  }else{
    while(tas(&lock->lockValue)){ //if not the current owner of the lock, keep spinning on the lock
    };
    lock->state = 1; //if you win the lock, claim it by setting state to occupied and thread ID to the ownerID
    lock->ownerID = pthread_self();
    lock->counter++;
    return 0;
  }

}


int my_spinlock_lockTTAS(my_spinlock_t *lock) //similar to TAS, except we spin on the lock status instead of TAS on the lock value
{

    if(lock->ownerID == pthread_self()){
      lock->counter++;
      return 0;

    }else{
      while(1){ //will only exit if we win the lock
        while(lock->state){ //keep spinning until the lock looks free
        };
        if(!tas(&lock->lockValue)){//if the lock looks free try a tas, if you win set ownership to your thread ID and increment counter
          lock->state = 1;
          lock->counter++;
          lock->ownerID = pthread_self();
          return 0;
        }
      }

    }
}

int my_spinlock_trylock(my_spinlock_t *lock) //try to get lock
{
  if(!tas(&lock->lockValue)){//if the lock looks free try a tas, if you win set ownership to your thread ID and increment counter
    lock->state = 1;
    lock->counter++;
    lock->ownerID = pthread_self();
    return 0;
  }
}


/*
 * Mutex routines
 */

int my_mutex_init(my_mutex_t *lock) //initialize everything to zero
{

  lock->delay = 0;
  lock->failCount = 0;
  lock->state = 0;
  lock->counter = 0;
  lock->ownerID =  0;
  lock->lockValue = 0;

}

int my_mutex_destroy(my_mutex_t *lock) //set everything to zero
{
  lock->delay = 0;
  lock->failCount = 0;

  lock->state = 0;
  lock->counter = 0;
  lock->ownerID =  0;
  lock->lockValue = 0;
}

int my_mutex_unlock(my_mutex_t *lock)
{


  lock->counter--; //decrement the lock counter
  if(lock->counter == 0){//if the counter is not zero, it means multiple recursive calls were used to obtain the lock by the same thread, so don't fully unlock
    lock->delay = 0;
    lock->failCount = 0;
    lock->state = 0;
    lock->ownerID =  0; //only fully unlock when the calling thread has no outstanding calls to the lock, then set everything to 0 for the next thread.
    lock->lockValue = 0;
  }


}

int my_mutex_lock(my_mutex_t *lock)
{
  int minDelay = 1; //1ns hardcoded values, each thread will wait at least 1 ns
  int maxDelay = 10000; //sleep for a maximum of 10000 us
  lock-> delay = rand()%(maxDelay + 1 - minDelay) + minDelay; //lock will have a delay time of a rnadom number between 1 and 1000 ns
  useconds_t sleepTime = lock->delay; //amount of time to sleep in usleep if lock acquire fails


  if(lock->ownerID == pthread_self()){ //if the calling thread is the owner of the lock, increment counter and return. No need to do anything since it owns the lock
    lock->counter++;
    return 0;

  }else{
    while(1){//will only exit once the thread wins the lock
      while(lock->state){ //keep spinning until the lock looks free
      };
      if(my_mutex_trylock(lock)==0){ //when the lock looks free, try to obtain the lock
        return 0;
      }else{
        lock->failCount++; //Keep track of how many times the lock failed to increment sleep time exponentially
        usleep(sleepTime); //sleep for some nanoseconds
        if(sleepTime < maxDelay){
          sleepTime = (lock->failCount)*2*sleepTime; //update how long the thread should sleep if it fails next time
        }
      }


    }

  }


}

int my_mutex_trylock(my_mutex_t *lock) //when trying to obtain the lock, perform a TAS
{
  if(!tas(&lock->lockValue)){
    lock->state = 1;
    lock->counter++;
    lock->ownerID = pthread_self();
    return 0;
  }
}

/*
 * Queue Lock
 */

int my_queuelock_init(my_queuelock_t *lock) //set everything to zero initially
{
  lock->ticketNumber = 0;
  lock->nowServingNumber = 0;
  lock->counter = 0;
  lock->ownerID =  0;


}

int my_queuelock_destroy(my_queuelock_t *lock) //set everything to zero
{
  lock->ticketNumber = 0;
  lock->nowServingNumber = 0;
  lock->counter = 0;
  lock->ownerID =  0;


}

int my_queuelock_unlock(my_queuelock_t *lock) //unlock the queue lock
{
  lock->counter--; //in case of recursive calls, decrement the counter and check if it's 0
  if(lock->counter == 0){ //if it's zero we can set the owner ID to 0 and update the nowServing Number. This need not be atomic since only one thread will ever unlock at a time
    lock->ownerID =  0;
    lock->nowServingNumber++;
    return 0;
  }

}

int my_queuelock_lock(my_queuelock_t *lock)
{


    if(lock->ownerID == pthread_self()){ //if the thread already owns the lock, just increment the counter and return. No need to do any work since it owns the lock already. This prevents a deadlock.
      lock->counter++;
      return 0;

    }else{
      //get ticket number
     unsigned long myTicket = __sync_fetch_and_add(&lock->ticketNumber, 1);
      while(1){
        if(myTicket == lock->nowServingNumber ){
          //we win the lock
          lock->counter++;
          lock->ownerID = pthread_self();
          return 0;
        }

      }

    }
}


int my_queuelock_trylock(my_queuelock_t *lock) //try to obtain lock if number matches
{
  unsigned long myTicket = __sync_fetch_and_add(&lock->ticketNumber, 1);
  if(myTicket == lock->nowServingNumber ){
    //we win the lock
    lock->counter++;
    lock->ownerID = pthread_self();
    return 0;
  }

}
