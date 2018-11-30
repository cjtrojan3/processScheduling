#include "headers/helpers.hpp"
#include <queue>
// #define DEBUG
// #define STATS

void executeRTS(std::queue<process> processBacklog) {

	unsigned long long totalProcessCount = (unsigned long long)processBacklog.size();
	int totalDroppedProcesses = 0;


	auto compare = [](const process& lhs, const process& rhs) {
       return lhs.deadline > rhs.deadline;
    };

	std::priority_queue<process, std::vector<process>, decltype(compare)> q1(compare);

	int clockTick = 0;
	double totalWaitTime = 0;
	double totalTurnaroundTime = 0;

	int hardOrSoft; // 0 for hard 1 for soft
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
					totalDroppedProcesses++;

					q1.pop();
				} else {

					std::cout << "A Process cannot be scheduled. Ending.";

					exit(0);
				}
			}

			const_cast<process&>(q1.top()).burst--;

			if (q1.top().burst == 0) {
				#ifdef DEBUG
				std::cout << "Process with PID: " << q1.top().pid << " is done.\n";
				#endif

				const_cast<process&>(q1.top()).endTime = clockTick;
				const_cast<process&>(q1.top()).turnaroundTime = q1.top().endTime - q1.top().arrival;
				const_cast<process&>(q1.top()).waitTime = q1.top().turnaroundTime - q1.top().totalBurst;
				// should only be -1 if it didn't have to wait
				// usually the first item but possibly others will have this case
				if (q1.top().waitTime == -1) {
					const_cast<process&>(q1.top()).waitTime = 0;
				}

				totalWaitTime += q1.top().waitTime;
				totalTurnaroundTime += q1.top().turnaroundTime;

				#ifdef STATS
				std::cout << "turnaround: " << q1.top().turnaroundTime << "\n";
				std::cout << "waitTime: " << q1.top().waitTime << "\n";
				#endif

				q1.pop();
			}
		} else if (processBacklog.size() == 0) {
			std::cout << std::endl;
			std::cout << "=========================================" << std::endl;
			std::cout << "              Statistics                 " << std::endl;
			std::cout << "=========================================\n" << std::endl;
			std::cout << "Total Process Count: " << totalProcessCount << "\n";
			std::cout << "Total number of procsses that finished: " << totalProcessCount - totalDroppedProcesses << "\n";
			std::cout << "Total Clock Ticks: " << clockTick << "\n";
			std::cout << "Average wait time: " << (double)(totalWaitTime / totalProcessCount) << "\n";
			std::cout << "Average turnaround time: " << (double)(totalTurnaroundTime / totalProcessCount) << "\n";			exit(0);
		}
		clockTick++;
	}
}
