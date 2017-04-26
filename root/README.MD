The attached program evaluates various locking algorithms in C. To run the code, simply run either of the following commands:
1. cd to the base-a3 folder and run "make"
2. ./mylocks

This executes the default base case as shown below:

pvu@asb9812u-a01:~/Desktop/submission/base-a3$ ./mylocks

|| INSTRUCTIONS ||
Usage of: ./mylocks -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid
testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock,
Example: ./mylocks -t 2 -i 10 -o 1000000 -c 1 -d 0

---------------------------------------------------
Default execution, performing ALL tests:

Test ID:         0
# Threads:       4
# Iterations:    1000000
# workOutsideCS: 0
# workInsideCS:  1

---------------------------------------------------

Test results are shown below...

Pthread Mutex Lock Test:
Total Count: 4000000
Time elapsed(ms): 332

Pthread Spinlock Test:
Total Count: 4000000
Time elapsed(ms): 82

TAS Lock Test:
Total Count: 4000000
Time elapsed(ms): 901

TTAS Lock Test:
Total Count: 4000000
Time elapsed(ms): 547

Mutex Lock Test:
Total Count: 4000000
Time elapsed(ms): 129

Busy Wait Queue Lock Test:
Total Count: 4000000
Time elapsed(ms): 452

pvu@asb9812u-a01:~/Desktop/submission/base-a3$ ^C
pvu@asb9812u-a01:~/Desktop/submission/base-a3$



3. ./mylocks -t 4 -i 1000000  -o 10 -c 100 -d 0

In this method, users can specify the:
# of threads -t,
# iterations -i,
# outside critical section operations -o
# insdie critical section operations -c
# testcase ID -d
  Where ID ranges from 0-6.
  testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock,

Sample Results:


pvu@asb9812u-a01:~/Desktop/submission/base-a3$ ./mylocks -t 4 -i 1000000  -o 10 -c 100 -d 0

|| INSTRUCTIONS ||
Usage of: ./mylocks -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid
testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock,
Example: ./mylocks -t 2 -i 10 -o 1000000 -c 1 -d 0

---------------------------------------------------
Default execution, performing ALL tests:

Test ID:         0
# Threads:       4
# Iterations:    1000000
# workOutsideCS: 10
# workInsideCS:  100

---------------------------------------------------

Test results are shown below...

Pthread Mutex Lock Test:
Total Count: 400000000
Time elapsed(ms): 1539

Pthread Spinlock Test:
Total Count: 400000000
Time elapsed(ms): 868

TAS Lock Test:
Total Count: 400000000
Time elapsed(ms): 2006

TTAS Lock Test:
Total Count: 400000000
Time elapsed(ms): 1023

Mutex Lock Test:
Total Count: 400000000
Time elapsed(ms): 931

Busy Wait Queue Lock Test:
Total Count: 400000000
Time elapsed(ms): 1183

pvu@asb9812u-a01:~/Desktop/submission/base-a3$


4. When you are satisfied with your results, run "make clean" to clear the make files
