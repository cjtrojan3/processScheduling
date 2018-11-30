#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <queue>
#include "queueWrapper.hpp"

std::queue<process> populateQueue(std::queue<process> processes, char* inputFilename);
bool queueSorter(process process1, process process2);
int getActiveQueue(queueWrapper queues[], int numberOfQueues);
int getNextQueue(queueWrapper queues[], int numberOfQueues);
bool sortByPid(process process1, process process2);
void executeMFQS(std::queue<process> processBacklog);
void executeRTS(std::queue<process> processBacklog);
void executeWHS(std::queue<process> processBacklog);
