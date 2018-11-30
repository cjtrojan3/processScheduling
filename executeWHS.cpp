#include "headers/helpers.hpp"
#include <queue>
#define DEBUG
#define STATS

void executeWHS(std::queue<process> processBacklog) {
	/*
	auto compare = [](const process& lhs, const process& rhs) {
       return lhs.deadline > rhs.deadline;
    };

	// Use this priority queue for holding I/O
	// Ones at the front with an I/O timer of 0 are done in I/O
	std::priority_queue<process, std::vector<process>, decltype(compare)> q1(compare);
	*/
	
	int clockTick = 0;
	double totalWaitTime = 0;
	double totalTurnaroundTime = 0;
	
	while (true) {
		// Get a reference to the process that will run on this clock tick
			// See if any queue has a process being run
			// If none, go down the queues and find one with a process at it
		// Read in new processes from the back log
			// If there are none, check to see if there is anything in the back log
				// Backlog empty? We are done.
				// Not empty? Just move on to the next clock tick
		// Age processes in I/O
			// while (I/O queue size > 0) {
				// Pop off front
				// Decrease I/O timer
				// Add to a vector
			// }
			// Move all processes from the vector back to the I/O queue
		// Move processes from I/O that are done back into ready queues (with promotion)
		// Promote bottom ten queues (process queue number + 10)
		// Put process through CPU (the reference we got at the top)
			// Burst done? Record stats and get rid of it
			// At (time quantum - 1) && I/O? Move to I/O
			// At time quantum? Demote
			// Otherwise, do nothing. Keep going with this process on the next clock tick.
	}
}
