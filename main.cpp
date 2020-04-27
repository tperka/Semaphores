#include <iostream>
#include <unistd.h>
#include <sys/shm.h>
#include "sem.hpp"
#include "keys.hpp"
#include <wait.h>
using namespace std;


int main(int argc, char const *argv[])
{
    // shared int buffer
    int* buffer;
    //creation of buffer and its semaphores
    int buffer_id = shmget(SHARED_BUFFER_ID, (BUFFER_SIZE + 1) * sizeof(int), IPC_CREAT | 0666); 
    if(buffer_id == -1)
    {
        cerr<<"Error: shmget failed"<<endl;
        exit(1);
    }
    buffer = (int*) shmat(buffer_id, 0, 0);
    buffer[0] = 0;
    Semaphore mutex(MUTEX_SEMKEY);
    Semaphore full(FULL_SEMKEY);
    Semaphore empty(EMPTY_SEMKEY);
    Semaphore A_seen(A_SEEN_SEMKEY);
    Semaphore B_seen(B_SEEN_SEMKEY);
    Semaphore read(READ_SEMKEY);
    //initializing semaphores
    mutex.initialize(1);
    full.initialize(0);
    empty.initialize(BUFFER_SIZE);
    A_seen.initialize(1);
    B_seen.initialize(1);
    read.initialize(0);
    //start processes:
    pid_t child = fork();
     
    if(child == 0)
    {
        char* args[] = {"./consumer.out", NULL};
        execvp(args[0], args);
    }
    child = fork();
    if(child == 0)
    {
        char* args[] = {"./producer.out", NULL};
        execvp(args[0], args );
    }
    child = fork();
    if(child == 0)
    {
        char* args[] = {"./readerA.out", NULL};
        execvp(args[0], args);
    }
    child = fork();
    if(child == 0)
    {
        char* args[] = {"./readerB.out", NULL};
        execvp(args[0], args);
    }
   
    wait(NULL);
    return 0;
}
