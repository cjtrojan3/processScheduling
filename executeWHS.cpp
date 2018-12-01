#include "headers/helpers.hpp"
#include <queue>
#define DEBUG
#define STATS

void executeWHS(std::queue<process> processBacklog) {
	
	// TODO Add fields to process
	// Time spent in I/O - Initialized to 0
	
	
	auto compare = [](const process& lhs, const process& rhs) {
       return lhs.deadline > rhs.deadline;
    };

	// Use this priority queue for holding I/O
	// Ones at the front with an I/O timer of 0 are done in I/O
	std::priority_queue<process, std::vector<process>, decltype(compare)> ioQueue(compare);
	
	
	int clockTick = 0;
	double totalWaitTime = 0;
	double totalTurnaroundTime = 0;
	int timeQuantum1, currentProcessIndex;
	std::vector<process> updateIOVector;
	
	std::cout << "Please enter the time quantum for each queue.\n";
	std::cin >> currentProcessIndex;
	
	queueWrapper *queues = new queueWrapper[100];
	for (int h = 0; h <= 99; h++) {
		queueWrapper q1;
		q1.timeQuantum = timeQuantum1;
		q1.howManyTicks = 0;
		queues[h] = q1;
	}
	
	while (true) {
		updateIOVector.clear();
		// Get a reference to the process that will run on this clock tick
		currentProcessIndex = -1;	// TODO - Function this off
		// See if any queue has a process being run
		for (int i = 99; i >= 0; i--) {
			if (queues[i].howManyTicks > 0) {
				currentProcessIndex = i;
			}
		}
		// If none, go down the queues and find one with a process at it
		if (currentProcessIndex == -1) {
			for (int j = 99; j >= 0; j--) {
				if (queues[j].processQueue.size() > 0) {
					currentProcessIndex = j;
				}
			}
		}
		
		// Read in new processes
		while (processBacklog.size() > 0 && processBacklog.front().arrival == clockTick) {
			process p = processBacklog.front();
			// We age the last ten queues
			if (p.priority >= 90) {
				p.waitTime = clockTick;
			}
			queues[p.priority].processQueue.push(p);

			#ifdef DEBUG
			std::cout << "Queuing process. PID: " << p.pid << "\n";
			#endif

			processBacklog.pop();

			#ifdef DEBUG
			std::cout << "Backlog size: " << processBacklog.size() << "\n";
			#endif
		}
		
		// Age processes in I/O
		// Move them into a vector and increment their ioAge
		while (ioQueue.size() > 0) {
			process p = ioQueue.top();
			p.ioAge++;
			ioQueue.pop();
			updateIOVector.push_back(p);
		}
		// Put them all back into the ioQueue
		while (updateIOVector.size() > 0) {
			ioQueue.push(updateIOVector.back());
			updateIOVector.pop_back();
		}
		
		// Move processes from I/O that are done back into ready queues (with promotion)
		while (ioQueue.size() > 0 && (ioQueue.top().io - ioQueue.top().ioAge == 0)) {
			process p1 = ioQueue.top();
			p1.ioAge = 0;
			// Cannot promote between 49 and 50
			if (p1.priority <= 49 && p1.priority+p1.io >= 50) {
				p1.priority = 50;
			} 
			// Cannot promote over 99
			else if (p1.priority <= 99 && p1.priority+p1.io >= 100) {
				p1.priority = 99;
			} 
			// Regular promotion
			else {
				p1.priority = (p1.priority + p1.io);
			}
		}
		
		// Promote bottom ten queues (process queue number + 10)
		for (int k = 0; k >= 9; k++) {
			// If a process has been in one of the bottom queues for 100 clock ticks, promote it
			while (queues[k].processQueue.size() > 0 && (clockTick - queues[k].processQueue.front().waitTime == 100)) {
				process p = queues[k].processQueue.front();
				p.waitTime = -1;
				queues[k].processQueue.pop();
				queues[k+10].processQueue.push(p);
			}
		}

		// Put process through CPU (the reference we got at the top)
		// Burst done? Record stats and get rid of it
		// At (time quantum - 1) && I/O? Move to I/O
		// At time quantum? Demote
		// Otherwise, do nothing. Keep going with this process on the next clock tick.
		
		// We have a process to run this clock tick
		if (currentProcessIndex > -1) {
			
		} 
		// No process to run this clock tick
		else {
			
		}
		
		
		clockTick++;
	}
}
