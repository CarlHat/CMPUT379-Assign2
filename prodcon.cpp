#include "header.h"

sem_t mutexCnt;
pthread_mutex_t mutexQ;
pthread_mutex_t mutexIO;
pthread_mutex_t mutexComplete;
pthread_mutex_t mutexAsk;
pthread_mutex_t mutexCLock;
pthread_cond_t mutexCond;
clock_t start; // https://linuxhint.com/timer-function-cpp/
queue<int> pQueue;
vector<int> tCompletes;
string fileName;
FILE *outFile;
bool notEOF;
int threadCnt;
int works;
int sleeps;
int asks;
int receives;

extern "C" void Trans(int);
extern "C" void Sleep(int);

void *consumer(void *args_p)
{
    int *id = (int *)args_p;
    while (true)
    {
        pcWrite(*id, -1, -1, "Ask");
        pthread_mutex_lock(&mutexAsk);
        asks++;
        pthread_mutex_unlock(&mutexAsk);
        sem_wait(&mutexCnt);
        if (emptyQ())
        {
            break;
        }
        int arg = popQ(*id);
        Trans(arg);
        pthread_mutex_lock(&mutexComplete);
        tCompletes[*id - 1]++;
        pthread_mutex_unlock(&mutexComplete);
        pcWrite(*id, arg, -1, "Complete");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    start = clock();
    sem_init(&mutexCnt, 0, 0);
    threadCnt = atoi(argv[1]);
    pthread_t thread_id[threadCnt];
    int consumerIds[threadCnt];

    notEOF = true;
    asks = 0;
    works = 0;
    sleeps = 0;
    tCompletes = vector<int>(threadCnt, 0);

    if (argv[2] != NULL && atoi(argv[2]) > 0)
    {
        fileName = "prodcon." + ((string)argv[2]) + ".log";
    }
    else
    {
        fileName = " prodcon.log";
    }

    for (int i = 0; i < threadCnt; i++)
    {
        // spawn threads consumer
        consumerIds[i] = i + 1;
        pthread_create(&thread_id[i], NULL, consumer, &consumerIds[i]);
    }

    // taking user input
    while (notEOF)
    {
        int status, arg;
        tie(status, arg) = cmdReadLine();
        if (status == STATUS_T)
        {
            addQ(arg);
            works++;
        }
        else if (status == STATUS_S)
        {
            pcWrite(0, arg, -1, "Sleep");
            Sleep(arg);
            sleeps++;
        }
        else
        {
            continue;
        }
    }

    pcWrite(0, -1, -1, "End");
    // wait for all threads to finish the print
    int *p_ret_value;
    for (pthread_t th : thread_id)
    {
        pthread_join(th, (void **)&p_ret_value);
    }

    summary();
    return 0;
}