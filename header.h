#ifndef MY_CONSTANT_INCLUDED
#define MY_CONSTANT_INCLUDED
#define STATUS_ERROR 0
#define STATUS_T 1
#define STATUS_S 2
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <vector>
#include <tuple>
#include <queue>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

extern sem_t mutexCnt;
extern pthread_mutex_t mutexQ;
extern pthread_mutex_t mutexIO;
extern pthread_mutex_t mutexComplete;
extern pthread_mutex_t mutexAsk;
extern pthread_mutex_t mutexCLock;
extern pthread_cond_t  mutexCond;
extern clock_t start;  // https://linuxhint.com/timer-function-cpp/
extern queue<int> pQueue;
extern vector<int> tCompletes;
extern string fileName;
extern FILE *outFile;
extern bool notEOF;
extern int threadCnt;
extern int works;
extern int sleeps;
extern int asks;
extern int receives;

// qcontrol.cpp
bool checkQ();
bool emptyQ();
void addQ(int arg);
int popQ(int id);

// iocontrol.cpp
void summary();
void pcWrite(int tId, int arg, int qSize, string cmd);
tuple<int, int> cmdReadLine();

#endif