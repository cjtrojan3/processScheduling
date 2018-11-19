#include <queue>
#include "process.hpp"

struct queueWrapper {
	int timeQuantum;
	std::queue<process> processQueue;
	int howManyTicks;
};
