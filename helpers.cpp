#include "headers/helpers.hpp"

int getProcessCount(char* inputFilename) {
	int numLines = 0;
	// Iterate through to find out how many processes exist
	std::ifstream inputFile(inputFilename);
	std::string dummyLine;
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
std::queue<process> populateQueue(std::queue<process> processes, char* inputFilename) {
	std::ifstream inputFile(inputFilename);

	if (inputFile) {
		// Store processes in here for sorting
		std::vector<process> processVector;
		std::string line;

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
				newProcess.waitTime = -1;
				newProcess.turnaroundTime = -1;
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

bool sortByPid(process process1, process process2) {
	return (process1.pid < process2.pid);
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
