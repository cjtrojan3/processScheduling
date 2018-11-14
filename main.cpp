#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <vector>

using namespace std;

struct process {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;
};

//int getProcessCount(char* inputFilename);
void executeMFQS(queue<process> processes);
void executeRTS();
void executeWHS();
queue<process> populateQueue(queue<process> processes, char* inputFilename);
bool queueSorter(process process1, process process2);

int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "Input filename not provided\n");
		exit(0);
	}
	
	int selectedProcess;
	cout << "Which scheduler would you like to run?\n(1): Multi-level Feedback Scheduler(MFQS)\n(2): Real-Time Scheduler (RTS)\n(3): Windows Hybrid Scheduler (WHS)\n"; 
	cin >> selectedProcess;
	/*
	int processCount = getProcessCount(argv[1]);
	if (processCount == -1) {
		fprintf(stderr, "Process count failed. Closing\n");
		exit(0);
	}
	*/
	queue <process> processes;
	processes = populateQueue(processes, argv[1]);
	/*
	while (!processes.empty()) {
		cout << "Arrival: " << (processes.front()).arrival << "\n";
		processes.pop();
	}
	*/
	
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
	//cout << "How many queues would you like?\n";
	//cin >> numberOfQueues;
	
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
			process newProcess;
			char* token;
			
			// We have to convert line into a char*
			char* buf = strdup(line.c_str());
			
			token = strtok(buf, " ");
			int lineIterator = 0;
			while (token != NULL) {
				if (lineIterator == 0) {
					newProcess.pid = atoi(token);
				} else if (lineIterator == 1) {
					newProcess.burst = atoi(token);
				} else if (lineIterator == 2) {
					newProcess.arrival = atoi(token);
				} else if (lineIterator == 3) {
					newProcess.priority = atoi(token);
				} else if (lineIterator == 4) {
					newProcess.deadline = atoi(token);
				} else if (lineIterator == 5) {
					newProcess.io = atoi(token);
				}
				token = strtok(NULL, " ");
				lineIterator++;
			}
			
			// Remove invalid queues
			if (newProcess.pid < 0 ||
				newProcess.burst < 0 ||
				newProcess.arrival < 0 ||
				newProcess.priority < 0 ||
				newProcess.deadline < 0 ||
				newProcess.io < 0) {}
			else {
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
	return (process1.arrival < process2.arrival);
}