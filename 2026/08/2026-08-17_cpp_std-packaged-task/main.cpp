#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <utility>

int calculate(int value){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return value * value;
}

int main(){
	std::packaged_task<int(int)> task(calculate);
	std::future<int> result = task.get_future();
	std::thread worker(std::move(task), 12);

	std::cout << "Calculating..\n";

	const int answer = result.get();
	worker.join();
	std::cout << "Result: " << answer << '\n';

	return 0;
}