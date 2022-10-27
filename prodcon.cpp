// #include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <queue>
#include <ctime>

using namespace std;

sem_t mutexCnt;
pthread_mutex_t mutexQ;
pthread_mutex_t mutexIO;
clock_t start;
queue<int> pQueue;

bool checkQ(int threadCnt)
{
    bool status;
    pthread_mutex_lock(&mutexQ);
    status = (threadCnt * 2) > pQueue.size();
    pthread_mutex_unlock(&mutexQ);
    return status;
}

void addQ(int content)
{
    pthread_mutex_lock(&mutexQ);
    pQueue.push(content);
    pthread_mutex_unlock(&mutexQ);
    sem_post(&mutexCnt);
}

int popQ()
{
    sem_wait(&mutexCnt);
    int content;
    pthread_mutex_lock(&mutexQ);
    content = pQueue.front();
    pQueue.pop();
    pthread_mutex_unlock(&mutexQ);
    return content;
}

void consumerWrite(char *fileName, string content)
{   
    clock_t now = clock();
    int totalMili = (now-start)*1000/CLOCKS_PER_SEC;
    int sec = totalMili/1000;
    int mili = totalMili-(sec*1000);
    pthread_mutex_lock(&mutexIO);
    // mutex protected write
    pthread_mutex_unlock(&mutexIO);
}

void consumer(int id)
{
    while(true)
    {

    }

}

int main(int argc, char *argv[])
{
    start = clock();
    sem_init(&mutexCnt, 0, 0);
    int threadCnt = atoi(argv[1]);
    string fileName;
    if (argv[2] != NULL)
    {
        fileName = "prodcon." + ((string)argv[2]) + ".log";
    }
    else
    {
        fileName = " prodcon.log";
    }

    // spawn threads
    printf("%d", threadCnt);
    printf("%s", fileName.c_str());

    for(int i = 0; i < threadCnt; i++) {
        // spawn threads consumer
    }

    bool debug = true;
    // taking user input
    while (debug)
    {
        debug = false;
    }
    return 0;
}