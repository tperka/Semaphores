#include "keys.hpp"
#include "sem.hpp"
#include "queue.hpp"

#include <iostream>
#include <sys/shm.h>

using namespace std;

int main(int argc, const char** argv) {
    // shared int buffer
    int* buffer;
    //connecting to the buffer
    int buffer_id = shmget(SHARED_BUFFER_ID, (BUFFER_SIZE + 1) * sizeof(int), 0666); 
    if(buffer_id == -1)
    {
        cerr<<"Error: shmget failed"<<endl;
        exit(1);
    }
    buffer = (int*) shmat(buffer_id, 0, 0);
    Semaphore mutex(MUTEX_SEMKEY);
    Semaphore full(FULL_SEMKEY);
    Semaphore empty(EMPTY_SEMKEY);
    Semaphore A_seen(A_SEEN_SEMKEY);
    Semaphore B_seen(B_SEEN_SEMKEY);
    Semaphore read(READ_SEMKEY);
    //connection established

    for(int i = 0;i < N; ++i)
    {
        //is item already read?
        read.wait();
        //check if buffer is not empty
        full.wait();
        //not empty, wait for access
        mutex.wait();
        //access granted, remove item
        pop_front(buffer);
        cout << "Consumer took out item: " << buffer[1] << endl;
        showQueue(buffer); 
        //new element, reset readers' semaphores       
        read.initialize(0);
        A_seen.initialize(1);
        B_seen.initialize(1);
        mutex.post();
        empty.post();
        randomSleep();
    }
    shmdt(buffer);
    return 0;


}