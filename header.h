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

#include "tands.c"

using namespace std;

sem_t mutexCnt;
pthread_mutex_t mutexQ;
pthread_mutex_t mutexIO;
pthread_mutex_t mutexComplete;
pthread_mutex_t mutexAsk;
clock_t start;  // https://linuxhint.com/timer-function-cpp/
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

#endif