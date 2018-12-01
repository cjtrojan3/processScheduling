struct process {
	int pid;
	int burst; // leave this one alone
	int totalBurst; // decrement this one
	int arrival;
	int originalPriority;
	int priority;
	int deadline;
	int io;
	int ioAge;
	int waitTime;
	int turnaroundTime;
	int age;
	int startTime;
	int endTime;
};
