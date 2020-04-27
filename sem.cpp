#include "sem.hpp"
#include "keys.hpp"
#include <iostream>
#include<sys/sem.h>
#include <ctime>
using namespace std;

Semaphore::Semaphore(key_t key)
{
    id = semget(key, 1, IPC_CREAT | 0666);
    if(id == -1)
    {
        cerr << "Error: failed to allocate semaphore "<< key <<endl;
        exit(1);
    } 
}


Semaphore::~Semaphore()
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

void Semaphore::post()
{
    struct sembuf arg = {0, 1, 0};
    if(semop(id, &arg, 1) == -1)
    {
        cerr << "Error: failed to post on semaphore "<<id<<endl;
        exit(1);
    }
}

bool Semaphore::check()
{
    struct sembuf arg = {0, -1, IPC_NOWAIT};
    if(semop(id, &arg, 1) == -1)
        return false;
    else
        return true;
}

void Semaphore::change(short value)
{
    if(value == 0)
        return;
    struct sembuf arg = {0, value, 0};
    if(semop(id, &arg, 1) == -1)
    {
        cerr << "Error: failed to change value of semaphore "<<id << endl;
        exit(1);
    }
}

int Semaphore::getVal()
{
    return semctl(id, 0, GETVAL, 0);
}



void randomSleep()
{
   srand(time(0));
   struct timespec arg = {(time_t)0, (long)rand()%1000000000};
   struct timespec res;
   nanosleep(&arg, &res);
}