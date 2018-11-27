struct process {
	int pid;
	int burst; // leave this one alone
	int totalBurst; // decrement this one
	int arrival;
	int priority;
	int deadline;
	int io;
	int waitTime;
	int turnaroundTime;
	int age;
	int startTime;
	int endTime;
};
