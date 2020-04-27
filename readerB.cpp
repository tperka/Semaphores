#include "sem.hpp"
#include "keys.hpp"
#include <sys/shm.h>
#include <iostream>
using namespace std;


int main(int argc, char const *argv[])
{
    //connecting to the buffer and creating semaphores
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
    for(;;)
    {
        //check if element was seen, wait if was
        //cout<<"czytelnik B czeka na nieprzeczytane element..."<<endl;
        B_seen.wait();
        //cout<<"czytelnik B doczekał się, czy kolejka nie jest pusta?"<<endl;
        //check if buffer is not empty, wait if it is
        if(full.getVal() == 0)
        {
            randomSleep(READER_B_MIN_SLEEP, READER_B_MAX_SLEEP);
            B_seen.signal();
            continue;
        }
        //buffer not empty, wait for access
        ////cout << "czytelnik B czeka na dostęp do sekcji"<<endl;
        mutex.wait();
        //access granted, set read
        //cout << "czytelnik B doczekał się, czyta"<<endl;
        read.signal();
        cout << "B has read an item: " << buffer[1] << endl;
        mutex.signal();
        randomSleep(READER_B_MIN_SLEEP, READER_B_MAX_SLEEP);
    }
    return 0;

}
