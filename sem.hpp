#ifndef SEM_HPP
#define SEM_HPP
#include <sys/types.h>
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short* array;
};

class Semaphore
{
private:
    int id;
public:
    Semaphore(key_t key);
    ~Semaphore();
    void initialize(short value);
    void wait();
    void post();
    bool check();
    void change(short value);
    int getVal();
};

void randomSleep();


#endif