#include "sem.hpp"
#include "keys.hpp"
#include <sys/shm.h>
#include <iostream>
using namespace std;


int main(int argc, char const *argv[])
{
    //connecting to the queue and creating semaphores
    Semaphore mutex(MUTEX_SEMKEY);
    Semaphore full(FULL_SEMKEY);
    Semaphore empty(EMPTY_SEMKEY);
    Semaphore A_seen(A_SEEN_SEMKEY);
    Semaphore B_seen(B_SEEN_SEMKEY);
    Semaphore read(READ_SEMKEY);
    //connection established
    for(int i = 0;i < N; ++i)
    {
        //check if element was seen, wait if was
        A_seen.wait();
        //check if buffer is not empty, wait if it is
        if(full.getVal() == 0)
        {
            randomSleep();
            A_seen.post();
            continue;
        }
        //buffer not empty, wait for access
        mutex.wait();
        //access granted, set read
        read.wait();
        cout << "Item read by A" << endl;
        mutex.post();
        randomSleep();
    }
    return 0;

}
