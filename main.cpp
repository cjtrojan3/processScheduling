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
int getActiveQueue(queueWrapper queues[], int numberOfQueues);
int getNextQueue(queueWrapper queues[], int numberOfQueues);

int clockTick = 0;
process currentProcess;

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
void executeMFQS(queue<process> processBacklog) {
	/*
	int numberOfQueues;
	cout << "How many queues would you like? (Max of 5)\n";
	cin >> numberOfQueues;
	
	if (numberOfQueues < 2 || numberOfQueues > 5) {
		fprintf(stderr, "Invalid number of queues. Closing.\n");
		exit(0);
	}
	*/
	
	// TODO Possibly take in as inputFile
	int timeQuantum1 = 3;
	
	queueWrapper *queues = new queueWrapper[3];
	for (int i = 0; i < 3; i++) {
		queueWrapper q1;
		if (i <= 2 - 2) {
			q1.timeQuantum = timeQuantum1;	
			q1.howManyTicks = 0;
		}
		queues[i] = q1;
		timeQuantum1 = timeQuantum1 * 2;		
	}	
	
	while (true) {
		
		// Add all new processes
		while (processBacklog.front().arrival == clockTick) {
			process p = processBacklog.front();
			queues[0].processQueue.push(p);
			cout << "Queuing process. PID: " << p.pid << "\n";
			processBacklog.pop();
			cout << "Backlog size: " << processBacklog.size() << "\n";
		}
		
		// Currently running queue
		int activeQueue = getActiveQueue(queues, 3);
		
		// No currently running processes - Start new one
		if (activeQueue == -1) {
			// Get highest queued process and start running it
			// If you came in at current clock tick, you cannot run
			int nextQueue = getNextQueue(queues, 3);
			// Queue empty
			if (nextQueue == -1) {
				if (processBacklog.size() > 0) {
					
				} else {
					// We are done. Nothing else to do.
				}
			} else {
				// Start new process and decrement 
			}
		} else {
			// We have a currently running process
			// Continue that process
			queues[activeQueue].howManyTicks++;
			queues[activeQueue].processQueue.front().burst--;
		}
		
		vector<process> updateQueueVector;
		
		// We only check for demotion if a process ran this clock tick
		/*
		if (activeQueue != -1 && processBacklog.size() > 0) {
			// Check if it's being loaded into the same queue that stuff is being promoted into
			// If yes, add to updateQueueVector
			// If no, just demote
		}
		*/
		
		// TODO Populate vector with aged processes
		// TODO Sort vector
		// 
		
		// Remove finished process and promote old processes
		
		// Regardless of what happens this clock tick, clock tick must increment
		clockTick++;
		
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

/**
 * If a queue is currently running a process, return the index to that queue.
 * If not, return -1 to know we can start at the top again.
 */
int getActiveQueue(queueWrapper queues[], int numberOfQueues) {
	for (int i = 0; i < numberOfQueues; i++) {
		if (queues[i].howManyTicks > 0) {
			return i;
		}
	}
	return -1;
}

int getNextQueue(queueWrapper queues[], int numberOfQueues) {
	for (int i = 0; i < numberOfQueues; i++) {
		if (queues[i].processQueue.size() > 0) {
			return i;
		}
	}
	return -1;
}