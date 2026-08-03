// ScopeTimer example

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

class ScopeTimer
{
public:
	explicit ScopeTimer(std::string name)
		: name_(std::move(name)),
		  start_(std::chrono::steady_clock::now())
	{
	}

	~ScopeTimer()
	{
		stop();
	}

	void stop()
	{
		if (stopped_)
			return;

		const auto end = std::chrono::steady_clock::now();
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_);

		std::cout << '[' << name_ << "] " << elapsed.count() << " ms\n";
		stopped_ = true;
	}

private:
	std::string name_;
	std::chrono::steady_clock::time_point start_;
	bool stopped_ = false;
};

int main()
{
	{
		ScopeTimer timer("Sleep");

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	{
		ScopeTimer timer("Loop");

		volatile int sum = 0;
		for (int i = 0; i < 100000000; ++i)
			sum += i;
	}

	return 0;
}
