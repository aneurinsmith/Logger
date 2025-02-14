
#include <logger.h>
#include <timer.h>
#include <iostream>
#include <thread>

int main() 
{
	LOG::Timer timer;
	timer.start();

	LOG::print("Current Epoch: ", LOG::Timer::get_epoch());

	std::this_thread::sleep_for(std::chrono::seconds(1));

	LOG::print("Current Epoch: ", LOG::Timer::get_epoch());
	LOG::print("Time elapsed: ", timer.stop());

	LOG::print(LOG::Timer::get_datetime());

	return 0;
}
