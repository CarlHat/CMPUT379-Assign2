#include "header.h"

void summary()
{
    clock_t now = clock();
    int totalMili = (now - start) * 1000 / CLOCKS_PER_SEC;
    float sec = (float) totalMili / (float) 1000;
    int complete = 0;
    for (int i : tCompletes)
    {
        complete += i;
    }
    float perSec = (float) complete/sec;
    outFile = fopen(fileName.c_str(), "a");
    fprintf(outFile, "Summary:\n    Work    %5d\n    Ask     %5d\n    Receive %5d\n    Complete%5d\n    Sleep   %5d\n", works, asks, receives, complete, sleeps);
    for (int i = 0; i < threadCnt; i++)
    {
        int temp = i+1;
        fprintf(outFile, "    Thread%3d%4d\n", temp, tCompletes[i]);
    }
    fprintf(outFile, "Transactions per second: %.2f\n", perSec);
    fclose(outFile);
}

// protected write to the output file a formatted string will be witten to the file
void pcWrite(int tId, int arg, int qSize, string cmd)
{
    clock_t now = clock();
    int totalMili = (now - start) * 1000 / CLOCKS_PER_SEC;
    int sec = totalMili / 1000;
    int mili = totalMili - (sec * 1000);
    string myArg;
    string myQSize;
    if (arg == -1)
    {
        myArg = " ";
    }
    else
    {
        myArg = to_string(arg);
    }
    if (qSize == -1)
    {
        myQSize = "    ";
    }
    else
    {
        myQSize = "Q= " + to_string(qSize);
    }

    pthread_mutex_lock(&mutexIO);
    outFile = fopen(fileName.c_str(), "a");
    // https://cplusplus.com/reference/cstdio/fprintf/
    fprintf(outFile, "%3d.%.3d ID= %2d %s %-10s %s\n", sec, mili, tId, myQSize.c_str(), cmd.c_str(), myArg.c_str());
    fclose(outFile);
    pthread_mutex_unlock(&mutexIO);
}

// read and parse command line
tuple<int, int> cmdReadLine()
{
    string buffer;
    int status;
    int arg;

    printf("Enter a command: ");
    // fflush(stdout);
    if (!getline(cin, buffer))
    {
        if (cin.eof())
        {
            notEOF = false;
            for (int i =0; i < threadCnt; i++)
            {
                sem_post(&mutexCnt);    // resume all threads blocked by sem_wait when eof is reached
            }
            return {STATUS_ERROR, 0};
        }
        else
        {
            printf("Failed to read!\n");
            return {STATUS_ERROR, 0};
        }
    }

    if (buffer.size() <= 1 || buffer.size() > 100)
    {
        printf("Command not valid!\n");
        return {STATUS_ERROR, 0};
    }
    else
    {
        if (buffer[0] == 'T')
        {
            pthread_mutex_lock(&mutexCLock);
            if (!checkQ())
            {                
                pthread_cond_wait( &mutexCond, &mutexCLock );   // wait until one job is taken from queue              
            }
            pthread_mutex_unlock(&mutexCLock);
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