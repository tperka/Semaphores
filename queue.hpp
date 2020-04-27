#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include <cstdlib>

void push_back(int* queue, int value)
{
    queue[queue[0] + 1] = value;
    ++queue[0];
}

int pop_front(int *queue)
{
    int toReturn = queue[1];
    for(int i = 1; i <= queue[0]; ++i)
    {
        queue[i] = queue[i + 1];
    }
    --queue[0];
    return toReturn;
}
int getSize(int * queue)
{
    return queue[0];
}
void showQueue(int* queue)
{
    std::cout << "Queue: ";
    for(int i = 1; i <= queue[0]; ++i)
        std::cout << queue[i] << " ";

    std::cout<<std::endl;
}


#endif 