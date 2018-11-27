#include "headers/helpers.hpp"
// #define DEBUG

/**
 * Executes the MFQS process scheduler
 */
void executeMFQS(std::queue<process> processBacklog) {

	unsigned long long totalProcessCount = (unsigned long long)processBacklog.size();

	int clockTick = 0;
	double totalWaitTime = 0;
	double totalTurnaroundTime = 0;

	int numberOfQueues;
	std::cout << "How many queues would you like? (Max of 5)\n";
	std::cin >> numberOfQueues;

	int ageTime;
	std::cout << "How long would you like processes to age in the last queue?\n";
	std::cin >> ageTime;

	int timeQuantum1;
	std::cout << "What would you like the time quantum of the first queue to be?\n";
	std::cin >> timeQuantum1;
	std::cout << "\n";

	if (numberOfQueues < 2 || numberOfQueues > 5) {
		fprintf(stderr, "Invalid number of queues. Closing.\n");
		exit(0);
	}

	queueWrapper *queues = new queueWrapper[numberOfQueues];
	for (int i = 0; i < numberOfQueues; i++) {
		queueWrapper q1;
		q1.timeQuantum = timeQuantum1;
		q1.howManyTicks = 0;
		queues[i] = q1;
		timeQuantum1 = timeQuantum1 * 2;
	}

	while (true) {
		int activeQueue, nextQueue, demotionQueue;
		activeQueue = nextQueue = demotionQueue = 0;
		bool activeQueueRan, nextQueueRan, alreadyPromoted;
		activeQueueRan = nextQueueRan = alreadyPromoted = false;
		std::vector<process> updateQueueVector;

		// Add all new processes
		while (processBacklog.size() > 0 && processBacklog.front().arrival == clockTick) {
			process p = processBacklog.front();
			queues[0].processQueue.push(p);

			#ifdef DEBUG
			std::cout << "Queuing process. PID: " << p.pid << "\n";
			#endif

			processBacklog.pop();

			#ifdef DEBUG
			std::cout << "Backlog size: " << processBacklog.size() << "\n";
			#endif
		}

		// Currently running queue
		activeQueue = getActiveQueue(queues, numberOfQueues);

		// No currently running processes - Start new one
		if (activeQueue == -1) {
			// Get highest queued process and start running it
			nextQueue = getNextQueue(queues, numberOfQueues);
			// Queue empty
			if (nextQueue == -1) {
				if (processBacklog.size() > 0) {

					#ifdef DEBUG
					std::cout << "All queues are empty. Waiting for new ones.\n";
					#endif

				} else {
					std::cout << "All processes are done. Ending.\n";
					std::cout << "Total Process Count: " << totalProcessCount << "\n";
					std::cout << "Average wait time: " << (double)(totalWaitTime / totalProcessCount) << "\n";
					std::cout << "Average turnaround time: " << (double)(totalTurnaroundTime / totalProcessCount) << "\n";

					#ifdef DEBUG
					std::cout << "Total Wait Time: " << totalWaitTime << "\n";
					#endif

					exit(0);
				}
			} else {
				// Start new process and decrement
				if (queues[nextQueue].processQueue.front().arrival < clockTick) {

					if (queues[nextQueue].processQueue.front().waitTime == -1) {
						// waitTime = clockTick - arrival
						queues[nextQueue].processQueue.front().waitTime = clockTick - queues[nextQueue].processQueue.front().arrival;
						totalWaitTime += (double)(queues[nextQueue].processQueue.front().waitTime);
						queues[nextQueue].processQueue.front().startTime = clockTick;
					}

					#ifdef DEBUG
					std::cout << "Running process with PID: " << queues[nextQueue].processQueue.front().pid << " in queue: " << nextQueue << ".\n";
					std::cout << "Process burst: " << queues[nextQueue].processQueue.front().burst << "\n";
					#endif

					nextQueueRan = true;
					queues[nextQueue].howManyTicks++;
					queues[nextQueue].processQueue.front().burst--;
				} else {
					// Process arrived at this clock tick. We cannot decrement its burst yet.
				}
			}
		} else {

			#ifdef DEBUG
			std::cout << "Running process with PID: " << queues[activeQueue].processQueue.front().pid << " in queue: " << activeQueue << ".\n";
			std::cout << "Process burst: " << queues[activeQueue].processQueue.front().burst << "\n";
			#endif

			activeQueueRan = true;
			queues[activeQueue].howManyTicks++;
			queues[activeQueue].processQueue.front().burst--;
		}

		// We only check for demotion if a process ran this clock tick
		if (activeQueueRan || nextQueueRan) {
			demotionQueue = ((activeQueue != -1) ? activeQueue : nextQueue);

			if (queues[demotionQueue].processQueue.front().burst == 0) {

				queues[demotionQueue].processQueue.front().endTime = clockTick;
				// turnaround = endtime - startTime
				queues[demotionQueue].processQueue.front().turnaroundTime = (queues[demotionQueue].processQueue.front().endTime - queues[demotionQueue].processQueue.front().startTime);
				totalTurnaroundTime += queues[demotionQueue].processQueue.front().turnaroundTime;

				#ifdef DEBUG
				std::cout << "Process with PID: " << queues[demotionQueue].processQueue.front().pid << " is done.\n";
				std::cout << "Process with PID: " << queues[demotionQueue].processQueue.front().pid << "\'s wait time is: " << queues[demotionQueue].processQueue.front().waitTime << "\n";
				std::cout << "Process with PID: " << queues[demotionQueue].processQueue.front().pid << "\'s turnaround time is: " << queues[demotionQueue].processQueue.front().turnaroundTime << "\n";
				#endif

				queues[demotionQueue].processQueue.pop();
				queues[demotionQueue].howManyTicks = 0;
			} else if (queues[demotionQueue].howManyTicks < queues[demotionQueue].timeQuantum || (demotionQueue == numberOfQueues-1)){
				// No demotion
			} else {
				// We have demotion and promotion going into the same queue
				queues[demotionQueue].howManyTicks = 0;

				if (demotionQueue == numberOfQueues - 3) {
					alreadyPromoted = true;
					updateQueueVector.push_back(queues[demotionQueue].processQueue.front());
					queues[demotionQueue].processQueue.pop();

					if (queues[numberOfQueues-1].processQueue.size() > 0 && (clockTick - queues[numberOfQueues-1].processQueue.front().age >= ageTime)) {
						updateQueueVector.push_back(queues[numberOfQueues-1].processQueue.front());
						queues[numberOfQueues-1].processQueue.pop();

						sort(updateQueueVector.begin(), updateQueueVector.end(), sortByPid);

						for(int i=0; i <= 1; i++) {
							queues[numberOfQueues-2].processQueue.push(updateQueueVector.front());
							updateQueueVector.erase(updateQueueVector.begin());
						}

					} else {
						queues[demotionQueue + 1].processQueue.push(updateQueueVector.front());
						updateQueueVector.erase(updateQueueVector.begin());
					}
				}
				// Promote and demote independently
				else {

					if (demotionQueue == numberOfQueues-2) {
						queues[demotionQueue].processQueue.front().age = clockTick;
					}

					queues[demotionQueue+1].processQueue.push(queues[demotionQueue].processQueue.front());
					queues[demotionQueue].processQueue.pop();
				}
			}
		}

		if (demotionQueue != numberOfQueues-1) {
			if (!alreadyPromoted && queues[numberOfQueues-1].processQueue.size() > 0 && (clockTick - queues[numberOfQueues-1].processQueue.front().age >= ageTime)) {
				queues[numberOfQueues-2].processQueue.push(queues[numberOfQueues-1].processQueue.front());
				queues[numberOfQueues-1].processQueue.pop();
			}
		}
		clockTick++;
	}
}
