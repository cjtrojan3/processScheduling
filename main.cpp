#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct process {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;
};

int getProcessCount(char* inputFilename);
void executeMFQS(char* inputFilename, int processCount);
void executeRTS(char* inputFilename);
void executeWHS(char* inputFilename);
void populateQueue(queue<process> processes, char* inputFilename);

int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "Input filename not provided\n");
		exit(0);
	}
	
	// Not sure if we're going to need this
	/*
	int processCount = getProcessCount(argv[1]);
	if (processCount == -1) {
		fprintf(stderr, "Process count failed. Closing\n");
		exit(0);
	}
	*/
	
	int selectedProcess;
	cout << "Which scheduler would you like to run?\n(1): Multi-level Feedback Scheduler(MFQS)\n(2): Real-Time Scheduler (RTS)\n(3): Windows Hybrid Scheduler (WHS)\n"; 
	cin >> selectedProcess;
	
	if (selectedProcess == 1) {
		executeMFQS(argv[1], processCount);
	} else if (selectedProcess == 2) {
		executeRTS(argv[1]);
	} else if (selectedProcess == 3) {
		executeWHS(argv[1]);
	} else {
		fprintf(stderr, "Invalid process selected. Closing.\n");
		exit(0);
	}
	return 0;
}

/**
 * Executes the MFQS process scheduler
 */
void executeMFQS(char* inputFilename, int processCount) {
	queue <process> processes;
	populateQueue(processes, inputFilename);
}

// TODO
void executeRTS(char* inputFilename) {
	
}

// TODO
void executeWHS(char* inputFilename) {
	
}

/*
int getProcessCount(char* inputFilename) {
	int numLines;
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
*/

/**
 * Populates the queue by parsing the file and creating process objects
 */
void populateQueue(queue<process> processes, char* inputFilename) {
	ifstream inputFile2(inputFilename);
	if (inputFile2) {
		string line;
		
		while(getline(inputFile2, line)) {
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
			
			cout << newProcess.pid << " " <<
				newProcess.burst << " " <<
				newProcess.arrival << " " <<
				newProcess.priority << " " <<
				newProcess.deadline << " " <<
				newProcess.burst << "\n";
			
			processes.push(newProcess);
		}
		inputFile2.close();
	}	
}