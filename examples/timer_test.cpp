
#include <timer.h>
#include <iostream>
#include <thread>

int main() 
{
	LOG::Timer timer;
	timer.start();

	std::cout << "Current Epoch: " << LOG::Timer::get_epoch() << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "Current Epoch: " << LOG::Timer::get_epoch() << std::endl;
	std::cout << "Time elapsed: " << timer.stop() << std::endl;

	std::cout << LOG::Timer::get_datetime();
}
