#include "sem.hpp"
#include "keys.hpp"
#include <sys/shm.h>
#include <iostream>

using namespace std;


int main(int argc, char const *argv[])
{
    //connecting to the queue and creating semaphores
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
    for(;;)
    {
        //check if element was seen, wait if was
        //cout<<"czytelnik a czeka na nieprzeczytane element..."<<endl;
        A_seen.wait();
        //cout<<"czytelnik a doczekał się, czy kolejka nie jest pusta?"<<endl;
        //check if buffer is not empty, wait if it is
        if(full.getVal() == 0)
        {
            randomSleep(READER_A_MIN_SLEEP, READER_A_MAX_SLEEP);
            A_seen.signal();
            continue;
        }
        //buffer not empty, wait for access
        //cout << "czytelnik a czeka na dostęp do sekcji"<<endl;
        mutex.wait();
        //access granted, set read
        //cout << "czytelnik a doczekał się"<<endl;
        read.signal();
        cout << "A has read an item: " << buffer[1] << endl;
        mutex.signal();
        randomSleep(READER_A_MIN_SLEEP, READER_A_MAX_SLEEP);
    }
    return 0;

}
