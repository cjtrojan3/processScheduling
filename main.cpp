#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

struct process {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;
	int waitTime;
	int turnaroundTime;
	// Used for aging
	// int timeSentToBottom;
};

struct queueWrapper {
	int timeQuantum;
	queue<process> processQueue;
	int howManyTicks;
};

//int getProcessCount(char* inputFilename);
void executeMFQS(queue<process> processes);
void executeRTS();
void executeWHS();
queue<process> populateQueue(queue<process> processes, char* inputFilename);
bool queueSorter(process process1, process process2);

int clockTick = -1;
int whichQueue = 0;

int main(int argc, char* argv[]) {
	
	
	
	if (argc != 2) {
		fprintf(stderr, "Input filename not provided\n");
		exit(0);
	}
	
	int selectedProcess;
	cout << "Which scheduler would you like to run?\n(1): Multi-level Feedback Scheduler(MFQS)\n(2): Real-Time Scheduler (RTS)\n(3): Windows Hybrid Scheduler (WHS)\n"; 
	cin >> selectedProcess;
	queue <process> processes;
	processes = populateQueue(processes, argv[1]);
	
	
	if (selectedProcess == 1) {
		executeMFQS(processes);
	} else if (selectedProcess == 2) {
		executeRTS();
	} else if (selectedProcess == 3) {
		executeWHS();
	} else {
		fprintf(stderr, "Invalid process selected. Closing.\n");
		exit(0);
	}
	return 0;
}

/**
 * Executes the MFQS process scheduler
 */
void executeMFQS(queue<process> processes) {
	int numberOfQueues;
	cout << "How many queues would you like? (Max of 5)\n";
	cin >> numberOfQueues;
	
	if (numberOfQueues < 2 || numberOfQueues > 5) {
		fprintf(stderr, "Invalid number of queues. Closing.\n");
		exit(0);
	}
	
	// TODO Possibly take in as inputFile
	int timeQuantum = 3;
	
	queueWrapper *queues = new queueWrapper[numberOfQueues];
	for (int i = 0; i < numberOfQueues; i++) {
		queueWrapper q1;
		if (i <= numberOfQueues - 2) {
			q1.timeQuantum = timeQuantum;	
			q1.howManyTicks = 0;
		}
		queues[i] = q1;
		timeQuantum = timeQuantum * 2;		
	}	
	
	/*
	
	1. increment clock tick
	2. add process to queue with arrival = clock tick
	2a. run process for specified time quantum
	3. once process finishes, check if a process in a higher queue is available
		done:
			dequeue process
		not done:
			demote to lower queue
	4. add all promoted / demoted processes to a vector and sort by pid
	5. increment age of processes in last queue
	6. Update active queue
	
	*/
	
	while(1) {
		bool changingQueue = true;
		cout << "Clocktick: " << clockTick << "\n";
		// 1. increment clock tick
		clockTick++;
		
		// 2. add process to queue with arrival time equal to clock tick
		while(processes.front().arrival == clockTick) {
			process p = processes.front();
			queues[0].processQueue.push(p);
			processes.pop();
		}
		
		// 2a. run the queue
		process current = queues[whichQueue].processQueue.front();
		current.burst--;
		queues[whichQueue].howManyTicks++;
		
		if (current.burst == 0) {
			// process is done
			queues[whichQueue].processQueue.pop();
			queues[whichQueue].howManyTicks = 0;
		}
		else if (queues[whichQueue].howManyTicks < queues[whichQueue].timeQuantum) {
			changingQueue = false;
		}
		else {
			// check if we are in last queue
			if (whichQueue == numberOfQueues-1) {
				
			} else {
				// demote
				queues[whichQueue].howManyTicks = 0;
				queues[whichQueue+1].processQueue.push(current);
				queues[whichQueue].processQueue.pop();
			}	
		}
		
		// Update the queue we are using
		if (changingQueue) {
			int z = 0;
			while (z <= numberOfQueues-1 && queues[z].processQueue.size() == 0) {
				z++;
			}
			if (z == numberOfQueues) {
				// We're done.
				cout << "Exiting program!\n";
				exit(0);
			} else {
				whichQueue = z;
			}
		}
	}
}

// TODO
void executeRTS() {
	
}

// TODO
void executeWHS() {
	
}

int getProcessCount(char* inputFilename) {
	int numLines = 0;
	// Iterate through to find out how many processes exist
	ifstream inputFile(inputFilename);
	string dummyLine;
	if (inputFile) {
		while(getline(inputFile, dummyLine)) {
			++numLines;
		}
	return numLines;
	} else {
		return -1;
	}
}

/**
 * Populates the queue by parsing the file, creating process objects,
 * sorting in a vector, then copying to a queue
 */
queue<process> populateQueue(queue<process> processes, char* inputFilename) {
	ifstream inputFile(inputFilename);
		
	if (inputFile) {
		// Store processes in here for sorting
		vector<process> processVector;
		string line;
		
		// Get first line and do nothing with iter_swap
		getline(inputFile, line);
		
		while(getline(inputFile, line)) {

			int pid, burst, arrival, priority, deadline, io;
			
			char* buf = strdup(line.c_str());
			
			sscanf(buf, "%d %d %d %d %d %d", &pid, &burst, &arrival, &priority, &deadline, &io);
			
			// Remove invalid queues
			if (pid < 0 ||
				burst < 0 ||
				arrival < 0 ||
				priority < 0 ||
				deadline < 0 ||
				io < 0) {}
			else {
				process newProcess;
				newProcess.pid = pid;
				newProcess.burst = burst;
				newProcess.arrival = arrival;
				newProcess.priority = priority;
				newProcess.deadline = deadline;
				newProcess.io = io;
				processVector.push_back(newProcess);
			}
		}
		inputFile.close();
		sort(processVector.begin(), processVector.end(), queueSorter);
		int j;
		for (j = 0; j < processVector.size(); j++) {
			processes.push(processVector.at(j));
		}
		return processes;
	}	
}

bool queueSorter(process process1, process process2) {
	if (process1.arrival == process2.arrival) {
		return (process1.pid < process2.pid);
	} else {
		return (process1.arrival < process2.arrival);	
	}
	
}