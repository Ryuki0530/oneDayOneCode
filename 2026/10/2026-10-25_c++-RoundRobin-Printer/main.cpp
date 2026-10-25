#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct PrintJob {
	std::string name;
	int remainingPages;
};

void processJobs(std::queue<PrintJob> jobs) {
	if (jobs.empty()) {
		std::cout << "No jobs\n";
		return;
	}

	std::vector<std::string> completed;

	while (!jobs.empty()) {
		PrintJob job = jobs.front();
		jobs.pop();

		const int printed = std::min(3, job.remainingPages);
		job.remainingPages -= printed;

		std::cout << job.name << ": printed=" << printed
				  << ", remaining=" << job.remainingPages << '\n';

		if (job.remainingPages > 0) {
			jobs.push(job);
		} else {
			completed.push_back(job.name);
		}
	}

	std::cout << "Completed:";
	for (const std::string& name : completed) {
		std::cout << ' ' << name;
	}
	std::cout << '\n';
}

//test
int main() {
	std::queue<PrintJob> jobs;
	jobs.push({"A", 7});
	jobs.push({"B", 2});
	jobs.push({"C", 4});

	processJobs(jobs);
	return 0;
}
