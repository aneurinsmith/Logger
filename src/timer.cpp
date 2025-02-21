
#include <timer.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

namespace LOG
{
	Timer::Timer(EpochUnit unit) : m_unit(unit) 
	{
		start_epoch = get_epoch(m_unit);
	}

	void Timer::start()
	{
		start_epoch = get_epoch(m_unit);
	}
	std::uint64_t Timer::stop()
	{
		return get_difference(start_epoch, get_epoch(m_unit));
	}



	std::uint64_t Timer::get_difference(std::uint64_t first, std::uint64_t second)
	{
		return (first > second) ? (first - second) : (second - first);
	}

	std::uint64_t Timer::get_epoch(EpochUnit unit)
	{
		using namespace std::chrono;
		auto now = system_clock::now();
		auto duration = now.time_since_epoch();
		return duration.count() / (std::uint64_t)unit;
	}



	std::string Timer::get_datetime(std::string fmt, uint64_t epoch)
	{
		std::string epoch_milliseconds = std::to_string(epoch).substr(10,3);
		std::string epoch_seconds = std::to_string(epoch).substr(0, 10);

		std::time_t t = static_cast<std::time_t>(std::stoll(epoch_seconds));
		std::tm tm = *localtime(&t);

		if (fmt.length() > 0) {
			size_t pos = fmt.find("%f");
			if (pos != std::string::npos) {
				fmt.replace(pos, 2, epoch_milliseconds);
			}
		}

		std::stringstream datetime;
		datetime << std::put_time(&tm, fmt.c_str());
		return datetime.str();
	}
}
