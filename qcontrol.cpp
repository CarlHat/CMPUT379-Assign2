#include "header.h"

bool checkQ()
{
    bool status;
    pthread_mutex_lock(&mutexQ);
    status = (threadCnt * 2) > ((int)pQueue.size());
    pthread_mutex_unlock(&mutexQ);
    return status;
}

bool emptyQ()
{
    bool status;
    pthread_mutex_lock(&mutexQ);
    status = ((int)pQueue.size()) == 0;
    pthread_mutex_unlock(&mutexQ);
    return status;
}

void addQ(int arg)
{
    int qSize;
    pthread_mutex_lock(&mutexQ);
    pQueue.push(arg);
    qSize = pQueue.size();
    pthread_mutex_unlock(&mutexQ);
    sem_post(&mutexCnt);
    pcWrite(0, arg, qSize, "Work");
}

int popQ(int id)
{
    int arg;
    int qSize;
    pthread_mutex_lock(&mutexQ);
    arg = pQueue.front();
    pthread_mutex_lock(&mutexCLock);
    pQueue.pop();
    pthread_cond_signal(&mutexCond);
    pthread_mutex_unlock(&mutexCLock);
    qSize = pQueue.size();
    receives++;
    pthread_mutex_unlock(&mutexQ);

    pcWrite(id, arg, qSize, "Receive");

    return arg;
}