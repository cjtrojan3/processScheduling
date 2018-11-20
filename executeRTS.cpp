#include "headers/helpers.hpp"
#include <queue>
#define DEBUG

void executeRTS(std::queue<process> processBacklog) {
	
	auto compare = [](const process& lhs, const process& rhs) {
       return lhs.deadline > rhs.deadline;
    };
	
	std::priority_queue<process, std::vector<process>, decltype(compare)> q1(compare);

	int clockTick = 0;
	
	int hardOrSoft ; // 0 for hard 1 for soft
	std::cout << "(0)Hard or (1)Soft?\n";
	std::cin >> hardOrSoft;
	
	if (hardOrSoft != 0 && hardOrSoft != 1) {
		std::cout << "Invalid input. Ending.\n";
		exit(0);
	}
	
	while(true) {
	
		while (processBacklog.size() > 0 && processBacklog.front().arrival == clockTick) {
			process p = processBacklog.front();
			q1.push(p);
			
			#ifdef DEBUG
			std::cout << "Queuing process. PID: " << p.pid << "\n";
			#endif
			
			processBacklog.pop();
			
			#ifdef DEBUG
			std::cout << "Backlog size: " << processBacklog.size() << "\n";
			#endif
		}
		
		if (q1.size() > 0) {
			while (q1.size() > 0 && q1.top().deadline - clockTick < q1.top().burst) {
				if (hardOrSoft) {
					#ifdef DEBUG
					std::cout << "Cannot schedule process with PID: " << q1.top().pid << ". Removing it.\n";
					#endif
					q1.pop();
				} else {
					#ifdef DEBUG
					std::cout << "We cannot schedule this process. Ending.";
					#endif
					exit(0);
				}
			}
		
			const_cast<process&>(q1.top()).burst--;
			
			if (q1.top().burst == 0) {
				#ifdef DEBUG
				std::cout << "Process with PID: " << q1.top().pid << " is done.\n";
				#endif
				q1.pop();
			}
		} else if (processBacklog.size() == 0) {
			#ifdef DEBUG
			std::cout << "All processes are done. Ending.\n";
			#endif
			exit(0);
		}
		clockTick++;
	}
}
