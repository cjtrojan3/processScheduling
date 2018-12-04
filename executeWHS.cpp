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
	//double totalWaitTime = 0;
	//double totalTurnaroundTime = 0;
	int timeQuantum1;
	int currentProcessIndex;
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
		currentProcessIndex = -1;
		// See if any queue has a process being run
		int i = 99;
		while (i >= 0 && currentProcessIndex == -1) {
			if (queues[i].howManyTicks > 0) {
				currentProcessIndex = i;
			}
			i--;
		}
		// If none, go down the queues and find one with a process at it
		if (currentProcessIndex == -1) {
			int j = 99;
			while (j >= 0 && currentProcessIndex == -1) {
				if (queues[j].processQueue.size() > 0) {
					currentProcessIndex = j;
				}
				j--;
			}
		}
		
		// Read in new processes
		if (processBacklog.size() > 0) {
			while (processBacklog.size() > 0 && processBacklog.front().arrival == clockTick) {
				process p = processBacklog.front();
				// We age the last ten queues
				if (p.originalPriority <= 9) {
					p.age = clockTick;
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
			if (done && ioQueue.size() == 0) {
				std::cout << "We are done\n";
				exit(0);
			}
		}
		
		// Age processes in I/O
		if (ioQueue.size() > 0) {
			#ifdef DEBUG
			std::cout << "Aging IO\n";
			#endif
			// Move them into a vector and increment their ioAge
			while (ioQueue.size() > 0) {
				auto p = ioQueue.top();
				#ifdef DEBUG
				std::cout << "Putting process: " << p.pid << " into updateIOVector\n";
				#endif
				p.ioAge++;
				updateIOVector.push_back(p);
				ioQueue.pop();
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
		
		
		// Move processes from I/O that are done back into ready queues (with promotion)
		while (ioQueue.size() > 0 && (ioQueue.top().io - ioQueue.top().ioAge == 0)) {
			process p1 = ioQueue.top();
			p1.ioAge = 0;
			/*
			if (p1.priority <= 9) {
				// No longer aging
				if (p1.priority + p1.io >= 10) {
					p1.age = 0;
				} 
				// Negates the "waiting" it did in IO
				else {
					p1.age -= p1.io;
				}
			}
			*/
			// Cannot promote between 49 and 50
			if (p1.priority <= 49 && p1.priority + p1.io >= 49) {
				#ifdef DEBUG
				std::cout << "Moving process: " << p1.pid << " from IO to queue 49.\n";
				#endif
				p1.priority = 49;
				queues[49].processQueue.push(p1);
			} 
			// Cannot promote over 99
			else if ((p1.priority <= 99 && p1.priority >= 50) && p1.priority + p1.io >= 99) {
				#ifdef DEBUG
				std::cout << "Moving process: " << p1.pid << " from IO to queue 99.\n";
				#endif
				p1.priority = 99;
				queues[99].processQueue.push(p1);
			} 
			// Regular promotion
			else {
				#ifdef DEBUG
				std::cout << "Moving process: " << p1.pid << " from IO to queue " << p1.priority + p1.io << "\n";
				#endif
				p1.priority = (p1.priority + p1.io);
				queues[p1.priority].processQueue.push(p1);
				
			}
			ioQueue.pop();
		}
		
		
		// Promote bottom ten queues (process queue number + 10)
		for (int k = 0; k <= 9; k++) {
			// If a process has been in one of the bottom queues for 100 clock ticks, promote it
			if (queues[k].processQueue.size() > 0 && (clockTick - queues[k].processQueue.front().age == 100)) {
				queues[k].processQueue.front().age = 0;
				queues[k].processQueue.front().priority += 10;
				queues[k+10].processQueue.push(queues[k].processQueue.front());
				queues[k].processQueue.pop();
				if (k == currentProcessIndex) {
					currentProcessIndex += 10;
				}
			}
		}
		#ifdef DEBUG
		if (currentProcessIndex > -1) {
			std::cout << "Current active queue: " << currentProcessIndex << "\n";	
		}
		#endif

		// We have a process to run this clock tick
		if (currentProcessIndex > -1) {
			queues[currentProcessIndex].howManyTicks++;
			queues[currentProcessIndex].processQueue.front().totalBurst--;
			#ifdef DEBUG
			std::cout << "Process: " << queues[currentProcessIndex].processQueue.front().pid << " burst is now: " << queues[currentProcessIndex].processQueue.front().totalBurst << "\n";
			#endif
			
			// Process is done
			if (queues[currentProcessIndex].processQueue.front().totalBurst == 0) {
				// TODO Gather stats
				queues[currentProcessIndex].processQueue.pop();
				queues[currentProcessIndex].howManyTicks = 0;
				#ifdef DEBUG
				std::cout << "Process: " << queues[currentProcessIndex].processQueue.front().pid << " just finished.\n";
				#endif
			}
			
			// There is IO and we are at timeQuantum-1 (Send to IO)
			else if (queues[currentProcessIndex].processQueue.front().io > 0 && (queues[currentProcessIndex].howManyTicks == timeQuantum1-1)) {
				// Reset the queue
				queues[currentProcessIndex].howManyTicks = 0;
				// Add to ioQueue
				ioQueue.push(queues[currentProcessIndex].processQueue.front());
				#ifdef DEBUG
				std::cout << "Process: " << queues[currentProcessIndex].processQueue.front().pid << " is going to IO\n";
				#endif
				queues[currentProcessIndex].processQueue.pop();
			} 
			
			// Time quantum is up. Demote
			else if (queues[currentProcessIndex].howManyTicks == timeQuantum1) {
				queues[currentProcessIndex].howManyTicks = 0;
				// Check originalPriority - We cannot demote past it
				if ((queues[currentProcessIndex].processQueue.front().priority - timeQuantum1 < queues[currentProcessIndex].processQueue.front().originalPriority) && (currentProcessIndex >= 10)) {
					queues[currentProcessIndex].processQueue.front().priority = queues[currentProcessIndex].processQueue.front().originalPriority;
					queues[queues[currentProcessIndex].processQueue.front().originalPriority].processQueue.push(queues[currentProcessIndex].processQueue.front());
				} else {
					if (currentProcessIndex <= 9) {
						if (queues[currentProcessIndex].processQueue.front().age == 0) {
							queues[currentProcessIndex].processQueue.front().age = clockTick;
						}
					}
					// Cannot demote past 0
					else if (queues[currentProcessIndex].processQueue.front().priority >= 0 && queues[currentProcessIndex].processQueue.front().priority <= 49 && queues[currentProcessIndex].processQueue.front().priority - timeQuantum1 <= -1) {
						// Demote to 0
						queues[currentProcessIndex].processQueue.front().priority = 0;
						queues[0].processQueue.push(queues[currentProcessIndex].processQueue.front());
						queues[currentProcessIndex].processQueue.pop();
						
						if (queues[currentProcessIndex].processQueue.front().priority <= 9) {
							queues[currentProcessIndex].processQueue.front().age = clockTick;
						}
					}
					// Cannot demote into the other band
					else if (queues[currentProcessIndex].processQueue.front().priority >= 50 && queues[currentProcessIndex].processQueue.front().priority - timeQuantum1 <= 49) {
						// Demote to 50
						queues[currentProcessIndex].processQueue.front().priority = 50;
						queues[50].processQueue.push(queues[currentProcessIndex].processQueue.front());
						queues[currentProcessIndex].processQueue.pop();
					}
					// Demote as normal
					else {
						// Demote to (currentQueueIndex - timeQuantum1)
						queues[currentProcessIndex].processQueue.front().priority = queues[currentProcessIndex].processQueue.front().priority - timeQuantum1;
						queues[currentProcessIndex-timeQuantum1].processQueue.push(queues[currentProcessIndex].processQueue.front());
						queues[currentProcessIndex].processQueue.pop();
						
						if (queues[currentProcessIndex].processQueue.front().priority <= 9) {
							queues[currentProcessIndex].processQueue.front().age = clockTick;
						}
					}
				}
				
				
			}
		}
		clockTick++;
	}
}
