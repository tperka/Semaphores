#include "sem.hpp"
#include "keys.hpp"
#include <iostream>
#include<sys/sem.h>
#include <ctime>
#include <boost/random/mersenne_twister.hpp>
using namespace std;


static boost::random::mt19937 gen(time(0));

Semaphore::Semaphore(key_t key)
{
    id = semget(key, 1, IPC_CREAT | 0666);
    if(id == -1)
    {
        cerr << "Error: failed to allocate semaphore "<< key <<endl;
        exit(1);
    } 
}


void Semaphore::deallocate()
{
    union semun ignored_arg;
    if(semctl(id, 1, IPC_RMID, ignored_arg) == -1)
    {
        cerr << "Error: failed to deallocate semaphore "<< id <<endl;
        exit(1);
    }
}
void Semaphore::initialize(short value)
{
    union semun arg;
    unsigned short vals[1];
    vals[0] = value;
    arg.array = vals;
    if(semctl(id,0,SETALL,arg) == -1)
    {
            cerr << "Error: failed to initialize semaphore "<<id<<endl;
            exit(1);
    }
}

void Semaphore::wait()
{
    struct sembuf arg = {0, -1, 0};
    if(semop(id, &arg, 1) == -1)
    {
        cerr << "Error: failed to wait on semaphore "<<id<<endl;
        exit(1);
    }
}

void Semaphore::signal()
{
    struct sembuf arg = {0, 1, 0};
    if(semop(id, &arg, 1) == -1)
    {
        cerr << "Error: failed to signal on semaphore "<<id<<endl;
        exit(1);
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
