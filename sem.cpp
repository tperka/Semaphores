#include "sem.hpp"
#include "keys.hpp"
#include <iostream>
#include <sys/sem.h>
#include <boost/random/mersenne_twister.hpp>
using namespace std;


static boost::random::mt19937 gen(time(0));

Semaphore::Semaphore(key_t key)
{
    id = semget(key, 1, IPC_CREAT | 0666);
    if(id == -1)
    {
        cerr << "Error: failed to allocate semaphore "<< key <<endl;
        exit(EXIT_FAILURE);
    } 
}

void Semaphore::setVal(short value)
{
    union semun arg;
    unsigned short vals[1];
    vals[0] = value;
    arg.array = vals;
    if(semctl(id,0,SETALL,arg) == -1)
    {
            cerr << "Error: failed to initialize semaphore "<<id<<endl;
            exit(EXIT_FAILURE);
    }
}

void Semaphore::wait()
{
    struct sembuf arg = {0, -1, 0};
    if(semop(id, &arg, 1) == -1)
    {
        cerr << "Error: failed to wait on semaphore "<<id<<endl;
        exit(EXIT_FAILURE);
    }
}

void Semaphore::signal()
{
    struct sembuf arg = {0, 1, 0};
    if(semop(id, &arg, 1) == -1)
    {
        cerr << "Error: failed to signal on semaphore "<<id<<endl;
        exit(EXIT_FAILURE);
    }
}

int Semaphore::getVal()
{
    return semctl(id, 0, GETVAL, 0);
}

void randomSleep(int min, int max)
{
    usleep(1000000*min + gen()%(1000000)*(max-min));
}
