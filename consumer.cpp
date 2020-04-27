#include "keys.hpp"
#include "sem.hpp"
#include "queue.hpp"

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
        exit(EXIT_FAILURE);
    }
    buffer = (int*) shmat(buffer_id, 0, 0);
    Semaphore mutex(MUTEX_SEMKEY);
    Semaphore full(FULL_SEMKEY);
    Semaphore empty(EMPTY_SEMKEY);
    Semaphore A_seen(A_SEEN_SEMKEY);
    Semaphore B_seen(B_SEEN_SEMKEY);
    Semaphore read(READ_SEMKEY);
    //connection established
    int taken = 0;
    for(;;)
    {
        //is item already read?
        //cout << "konsument czeka na przeczytany item" << endl;
        read.wait();
        //check if buffer is not empty
        //cout << "konsument czeka niepustą kolejkę" << endl;
        full.wait();
        //not empty, wait for access
        //cout << "konsument czeka na dostęp" << endl;
        mutex.wait();
        //access granted, remove item
        taken = pop_front(buffer);
        cout << "Consumer took out item: " << taken << endl;
        showQueue(buffer); 
        //new element, reset readers' semaphores       
        read.setVal(0);
        A_seen.setVal(1);
        B_seen.setVal(1);
        mutex.signal();
        empty.signal();
        randomSleep(CONSUMER_MIN_SLEEP, CONSUMER_MAX_SLEEP);
    }
    shmdt(buffer);
    return 0;


}