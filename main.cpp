#include <iostream>
#include <unistd.h>
#include <sys/shm.h>
#include <wait.h>
#include <signal.h>

#include "sem.hpp"
#include "keys.hpp"

using namespace std;

void handleCtrlC(int)
{
    system("ipcrm -a");
    cout << endl<<"Everything has been cleared, exiting program..." << endl;
    exit(EXIT_SUCCESS);
}


int main(int argc, char const *argv[])
{
    //signal handler to handle Ctrl-C
    signal(SIGINT, handleCtrlC);
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
    mutex.setVal(1);
    full.setVal(0);
    empty.setVal(BUFFER_SIZE);
    A_seen.setVal(1);
    B_seen.setVal(1);
    read.setVal(0);
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
