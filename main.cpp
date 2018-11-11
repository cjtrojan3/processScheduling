#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

struct process {
	int pid;
	int burst;
	int arrival;
	int priority;
	int deadline;
	int io;
};

void executeMFQS(char* inputFilename);

int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "Input filename not provided\n");
		exit(0);
	}
	
	int selectedProcess;
	
	cout << "Which scheduler would you like to run?\n(1): Multi-level Feedback Scheduler(MFQS)\n(2): Real-Time Scheduler (RTS)\n(3): Windows Hybrid Scheduler (WHS)\n"; 
	cin >> selectedProcess;
	
	if (selectedProcess == 1) {
		executeMFQS(argv[1]);
	} else if (selectedProcess == 2) {
		
	} else if (selectedProcess == 3) {
		
	} else {
		fprintf(stderr, "Invalid process selected. Closing.\n");
		exit(0);
	}
	return 0;
}

void executeMFQS(char* inputFilename) {
	string line;
	int numLines;
	
	// Iterate through to find out how many processes exist
	ifstream inputFile(inputFilename);
	string dummyLine;
	if(inputFile) {
		while(getline(inputFile, dummyLine)) {
			++numLines;
		}
	}
	
	// Iterate through again and create an array of process objects
	process processes[numLines];
	ifstream inputFile2(inputFilename);
	if (inputFile2) {
		while(getline(inputFile2, line)) {
			cout << line << "\n";
		}
		inputFile2.close();
	}
}