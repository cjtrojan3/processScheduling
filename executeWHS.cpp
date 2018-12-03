#include "headers/helpers.hpp"
#include <queue>
#define DEBUG
#define STATS

int clockTick;

void executeWHS(std::queue<process> processBacklog) {
	
	// TODO Add fields to process
	// Time spent in I/O - Initialized to 0
	
	
	auto compare = [](const process& lhs, const process& rhs) {
       return (lhs.io - lhs.ioAge) > (rhs.io - rhs.ioAge);
    };

	// Use this priority queue for holding I/O
	// Ones at the front with an I/O timer of 0 are done in I/O
	std::priority_queue<process, std::vector<process>, decltype(compare)> ioQueue(compare);
	
	clockTick = 0;
	double totalWaitTime = 0;
	double totalTurnaroundTime = 0;
	int timeQuantum1, currentProcessIndex;
	std::vector<process> updateIOVector;
	
	std::cout << "Please enter the time quantum for each queue.\n";
	std::cin >> timeQuantum1;
	
	queueWrapper *queues = new queueWrapper[100];
	for (int h = 0; h <= 99; h++) {
		queueWrapper q1;
		q1.timeQuantum = timeQuantum1;
		q1.howManyTicks = 0;
		queues[h] = q1;
	}
	
	while (true) {
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
		#ifdef DEBUG
		if (currentProcessIndex > -1) {
			std::cout << "Current active queue: " << currentProcessIndex << "\n";	
		}
		#endif
		
		// Read in new processes
		if (processBacklog.size() > 0) {
			while (processBacklog.size() > 0 && processBacklog.front().arrival == clockTick) {
				process p = processBacklog.front();
				// We age the last ten queues
				if (p.originalPriority <= 9) {
					p.waitTime = clockTick;
				}
				queues[p.originalPriority].processQueue.push(p);

				#ifdef DEBUG
				std::cout << "Queuing process. PID: " << p.pid << "\n";
				#endif

				processBacklog.pop();

				#ifdef DEBUG
				std::cout << "Backlog size: " << processBacklog.size() << "\n";
				#endif
			}
		} 
		// Check to see if there are any more processes to run
		else {
			bool done = true;
			for (int k = 99; k >= 0; k--) {
				if (queues[k].processQueue.size() > 0) {
					done = false;
				}
			}
			if (done) {
				std::cout << "We are done\n";
				exit(0);
			}
		}
		/*
		// Age processes in I/O
		if (ioQueue.size() > 0) {
			#ifdef DEBUG
			std::cout << "Aging IO\n";
			#endif
			// Move them into a vector and increment their ioAge
			while (ioQueue.size() > 0) {
				process p = ioQueue.top();
				#ifdef DEBUG
				std::cout << "Putting process: " << p.pid << " into updateIOVector";
				#endif
				p.ioAge++;
				ioQueue.pop();
				updateIOVector.push_back(p);
			}
			
			// Put them all back into the ioQueue
			while (updateIOVector.size() > 0) {
				#ifdef DEBUG
				std::cout << "Putting process: " << updateIOVector.back().pid << " back into ioQueue\n";
				#endif
				ioQueue.push(updateIOVector.back());
				updateIOVector.pop_back();
			}
		}
		*/
		/*
		// Move processes from I/O that are done back into ready queues (with promotion)
		while (ioQueue.size() > 0 && (ioQueue.top().io - ioQueue.top().ioAge == 0)) {
			process p1 = ioQueue.top();
			ioQueue.pop();
			p1.ioAge = 0;
			// Cannot promote between 49 and 50
			if (p1.priority <= 49 && p1.priority+p1.io >= 50) {
				#ifdef DEBUG
				std::cout << "Moving process: " << p1.pid << " from IO to queue 49.\n";
				#endif
				p1.priority = 49;
			} 
			// Cannot promote over 99
			else if (p1.priority <= 99 && p1.priority+p1.io >= 100) {
				#ifdef DEBUG
				std::cout << "Moving process: " << p1.pid << " from IO to queue 99.\n";
				#endif
				p1.priority = 99;
			} 
			// Regular promotion
			else {
				#ifdef DEBUG
				std::cout << "Moving process: " << p1.pid << " from IO to queue " << p1.priority+p1.io << "\n";
				#endif
				p1.priority = (p1.priority + p1.io);
				
			}
		}
		*/
		/*
		// Promote bottom ten queues (process queue number + 10)
		for (int k = 0; k <= 9; k++) {
			// If a process has been in one of the bottom queues for 100 clock ticks, promote it
			if (queues[k].processQueue.size() > 0 && (clockTick - queues[k].processQueue.front().waitTime == 100)) {
				process p = queues[k].processQueue.front();
				p.waitTime = -1;
				queues[k].processQueue.pop();
				queues[k+10].processQueue.push(p);
			}
		}
		*/

		// We have a process to run this clock tick
		if (currentProcessIndex > -1) {
			process aaa = queues[currentProcessIndex].processQueue.front();
			queues[currentProcessIndex].howManyTicks++;
			aaa.totalBurst--;
			#ifdef DEBUG
			std::cout << "Process: " << aaa.pid << " burst is now: " << aaa.totalBurst << "\n";
			#endif
			
			// Process is done
			if (aaa.totalBurst == 0) {
				// TODO Gather stats
				queues[currentProcessIndex].processQueue.pop();
				queues[currentProcessIndex].howManyTicks = 0;
				#ifdef STATS
				std::cout << "Process: " << aaa.pid << " just finished.\n";
				#endif
			}
			/*
			// There is IO and we are at timeQuantum-1 (Send to IO)
			else if (aaa.io > 0 && (queues[currentProcessIndex].howManyTicks = timeQuantum1-1)) {
				// Reset the queue
				queues[currentProcessIndex].howManyTicks = 0;
				// Add to ioQueue
				ioQueue.push(aaa);
				#ifdef DEBUG
				std::cout << "Process: " << aaa.pid << " is going to IO";
				#endif
			} 
			*/
			// Time quantum is up. Demote
			else if (queues[currentProcessIndex].howManyTicks == timeQuantum1) {
				// Pop off the process
				queues[currentProcessIndex].processQueue.pop();
				queues[currentProcessIndex].howManyTicks = 0;
				// Check originalPriority - We cannot demote past it
				if (aaa.priority - timeQuantum1 < aaa.originalPriority) {
					aaa.priority = aaa.originalPriority;
					queues[aaa.originalPriority].processQueue.push(aaa);
				} else {
					// Cannot demote past 0
					if (aaa.priority >= 0 && aaa.priority <= 49 && aaa.priority - timeQuantum1 <= -1) {
						// Demote to 0
						aaa.priority = 0;
						queues[0].processQueue.push(aaa);
					}
					// Cannot demote into the other band
					else if (aaa.priority >= 50 && aaa.priority + timeQuantum1 <= 49) {
						// Demote to 50
						aaa.priority = 50;
						queues[50].processQueue.push(aaa);
					}
					// Demote as normal
					else {
						// Demote to (currentQueueIndex - timeQuantum1)
						aaa.priority = aaa.priority - timeQuantum1;
						queues[aaa.priority-timeQuantum1].processQueue.push(aaa);
					}
				}
			}
		}
		clockTick++;
	}
}
