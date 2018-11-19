#include "headers/helpers.hpp"

void executeRTS(std::queue<process> processBacklog) {

	int clockTick = 0;

	queueWrapper* queue = new queueWrapper;
	
	int hardOrSoft; // 0 for hard 1 for soft
	std::cout << "(0)Hard or (1)Soft?\n";
	std::cin >> hardOrSoft;
	
	if (hardOrSoft != 0 || hardOrSoft != 1) {
		std::cout << "You're a moron.\n";
		exit(0);
	}
	
	while(true) {
	
		while (processBacklog.size() > 0 && processBacklog.front().arrival == clockTick) {
			process p = processBacklog.front();
			queue->processQueue.push(p);
			std::cout << "Popping off process with PID: " << p.pid << "\n";
			processBacklog.pop();
		}

		clockTick++;
	}
}
