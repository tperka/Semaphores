#include "sem.hpp"
#include "keys.hpp"
#include "queue.hpp"

#include <sys/shm.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
using namespace std;

static boost::random::mt19937 gen(time(0));

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

    //doing work
    srand(time(0));
    for(;;)
    {
        //check if queue is not full, otherwise wait
        //cout << "mutex: "<<mutex.getVal()<<" full: "<<full.getVal() << " empty: "<<empty.getVal() << " A_seen: "<<A_seen.getVal() << " B_seen: " << B_seen.getVal() << " read: "<<read.getVal()<<endl;
        empty.wait();
        //it is not full, wait for the access
        int temp = gen() % 100;
        mutex.wait();
        //access possible, produce random item
        push_back(buffer, temp);
        cout<<"Producer added item: " << temp << endl;
        showQueue(buffer);
        //leave critical section
        mutex.signal();   
        full.signal();    //increase the amount of items
        //sleep for a random ammount of time (up to 1 sec)
        randomSleep(PRODUCER_MIN_SLEEP, PRODUCER_MAX_SLEEP);
    }    

    //detach
    shmdt(buffer);

    return 0;
}