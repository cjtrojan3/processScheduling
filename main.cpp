#include <queue>
#include "headers/helpers.hpp"

int main(int argc, char* argv[]) {

	process currentProcess;

	if (argc != 2) {
		fprintf(stderr, "Input filename not provided\n");
		exit(0);
	}

	int selectedProcess;
	std::cout << "Which scheduler would you like to run?\n(1): Multi-level Feedback Scheduler(MFQS)\n(2): Real-Time Scheduler (RTS)\n(3): Windows Hybrid Scheduler (WHS)\n";
	std::cin >> selectedProcess;
	std::queue <process> processes;
	processes = populateQueue(processes, argv[1]);

	switch(selectedProcess) {
		case 1: executeMFQS(processes);
			break;
		case 2: executeRTS(processes);
			break;
		case 3: executeWHS();
			break;
		default: std::cout << "Hello :)\n";
	}
	/*
	if (selectedProcess == 1) {
		executeMFQS(processes);
	} else if (selectedProcess == 2) {
		executeRTS(processes);
	} else if (selectedProcess == 3) {
		executeWHS();
	} else {
		fprintf(stderr, "Invalid process selected. Closing.\n");
		exit(0);
	}
	*/
	return 0;
}
