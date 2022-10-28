#include "header.h"

bool checkQ()
{
    bool status;
    pthread_mutex_lock(&mutexQ);
    status = (threadCnt * 2) > pQueue.size();
    pthread_mutex_unlock(&mutexQ);
    return status;
}

void pcWrite(int tId, int arg, int qSize, string cmd)
{   
    clock_t now = clock();
    int totalMili = (now-start)*1000/CLOCKS_PER_SEC;
    int sec = totalMili/1000;
    int mili = totalMili-(sec*1000);
    string myArg;
    string myQSize;
    if(arg==-1)
    {
        myArg=" ";
    }
    else
    {
        myArg=to_string(arg);
    }
    if(qSize==-1)
    {
        myQSize="    ";
    }
    else
    {
        myQSize="Q= " + to_string(qSize);
    }

    pthread_mutex_lock(&mutexIO);
    outFile = fopen(fileName.c_str(), "a");
    // https://cplusplus.com/reference/cstdio/fprintf/
    fprintf(outFile, "%d.%.3d %d %s %s\n", sec, mili, tId, myQSize.c_str(), myArg.c_str());
    fclose(outFile);
    pthread_mutex_unlock(&mutexIO);
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
    sem_wait(&mutexCnt);
    int arg;
    int qSize;
    pthread_mutex_lock(&mutexQ);
    arg = pQueue.front();
    pQueue.pop();
    qSize = pQueue.size();
    receives++;
    pthread_mutex_unlock(&mutexQ);
    
    pcWrite(id, arg, qSize, "Receive");
    // debug
    printf("%d take job %d\n", id, arg);

    return arg;
}

void *consumer(void *args_p)
{
    int *id = (int *) args_p;
    while(true)
    {
        printf("%d asking for job\n", *id);
        pthread_mutex_lock(&mutexAsk);
        asks++;
        pthread_mutex_unlock(&mutexAsk);
        pcWrite(*id, -1, -1, "Ask");
        int arg = popQ(*id);
        Trans(arg);
        pthread_mutex_lock(&mutexComplete);
        tCompletes[*id-1]++;
        pthread_mutex_unlock(&mutexComplete);
        pcWrite(*id, arg, -1, "Complete");
    }

}

tuple<int, int> cmdReadLine()
{
    string buffer;
    int status;
    int arg;

    printf("Enter a command: ");
    // fflush(stdout);
    if(!getline(cin, buffer))
    {
        if(cin.eof())
        {
            notEOF = false;
            return  {STATUS_ERROR, 0};
        }
        else
        {
            printf("Failed to read!\n");
            return  {STATUS_ERROR, 0};
        }
    }

    if (buffer.size() <= 1 || buffer.size() > 100)
    {
        printf("Command not valid!\n");
        return  {STATUS_ERROR, 0};
    }
    else if (!checkQ())
    {
        printf("Queue is full!\n");
        return  {STATUS_ERROR, 0};
    }
    else
    {
        if(buffer[0] == 'T')
        {
            status = STATUS_T;
        } 
        else if (buffer[0] == 'S')
        {
            status = STATUS_S;
        }
        else
        {
            printf("Invalid command!\n");
            status = STATUS_ERROR;
        }
        arg = atoi(buffer.erase(0, 1).c_str());
    }
    return {status, arg};
}

int main(int argc, char *argv[])
{
    start = clock();
    sem_init(&mutexCnt, 0, 0);
    threadCnt = atoi(argv[1]);
    pthread_t thread_id[threadCnt];
    int consumerIds[threadCnt];

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

    // outFile = fopen(fileName.c_str(), "wa");

    // spawn threads
    // printf("%d\n", threadCnt);
    // printf("%s\n", fileName.c_str());

    for(int i = 0; i < threadCnt; i++) {
        // spawn threads consumer
        consumerIds[i] = i+1;
        pthread_create(&thread_id[i], NULL, consumer, &consumerIds[i]);
    }

    notEOF = true;
    // taking user input
    while (notEOF)
    {
        int status, arg;
        tie(status, arg) = cmdReadLine();
        if(status == STATUS_T)
        {
            printf("add %d\n", arg);
            addQ(arg);
            works++;
        }
        else if (status == STATUS_S)
        {
            printf("sleep %d\n", arg);
            Sleep(arg);
            sleeps++;
            pcWrite(0, -1, -1, "Sleep");
        }
        else
        {
            continue;
        }
    }

    pcWrite(0, -1, -1, "End");
    // wait for all threads to finish the print
    // fclose(outFile);
    return 0;
}