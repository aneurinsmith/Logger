
#pragma once
#include <chrono>
#include <string>

namespace LOG 
{
	enum class EpochUnit
	{
		SECONDS = 1000000000,
		MILLISECONDS = 1000000,
		MICROSECONDS = 1000
	};

	class Timer
	{
	public:
		Timer(EpochUnit unit = EpochUnit::MILLISECONDS);

		void start();
		std::uint64_t stop();

		static std::uint64_t get_epoch(EpochUnit unit = EpochUnit::MILLISECONDS);
		static std::string get_datetime(std::string fmt = "%Y/%m/%d %H:%M:%S.%f", std::uint64_t epoch = get_epoch());

	private:
		std::uint64_t start_epoch;
		EpochUnit m_unit;

		std::uint64_t get_difference(std::uint64_t, std::uint64_t);
	};
}
