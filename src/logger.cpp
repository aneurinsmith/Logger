
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "logger.h"

namespace LOG 
{
	Logger::Logger() : m_fmt("%Y/%m/%d %H:%M:%S.%f") {}
	Logger::Logger(std::string fmt) : m_fmt(fmt) {}

	Logger& Logger::instance() 
	{
		static Logger s_logger;
		return s_logger;
	}

	void Logger::set_level(LOG::Level lvl) 
	{
		m_lvl = lvl;
		for (auto& sink : sinks) {
			sink->set_level(lvl);
		}
	}

	void Logger::set_format(std::string fmt) 
	{
		m_fmt = fmt;
	}

	void Logger::print(LOG::Level lvl, std::string msg) 
	{
		std::stringstream stream;
		std::string fmt = m_fmt;

		if (m_fmt.length() > 0) {
			size_t pos = fmt.find("%f");
			if (pos != std::string::npos) {
				auto now = std::chrono::system_clock::now();
				auto ms = std::to_string((now.time_since_epoch().count() / 1000000) % 1000);
				if (ms.length() < 3) {
					ms = (std::string(3 - ms.length(), '0') + ms);
				}
				fmt.replace(pos, 2, ms);
			}

			std::time_t t = time(nullptr);
			std::tm tm = *localtime(&t);
			stream << std::put_time(&tm, fmt.c_str()) << " ";
		}

		switch (lvl) {
		case LOG::TRACE: stream << "[TRACE]  "; break;
		case LOG::DEBUG: stream << "[DEBUG]  "; break;
		case LOG::INFO:  stream << " [INFO]  "; break;
		case LOG::WARN:  stream << " [WARN]  "; break;
		case LOG::ERROR: stream << "[ERROR]  "; break;
		case LOG::FATAL: stream << "[FATAL]  "; break;
		case LOG::NONE: break;
		}

		stream << msg;

		for (auto& sink : sinks) {
			sink->print(lvl, stream.str());
		}
	}

	void Logger::add_sink(std::shared_ptr<basesink> sink) 
	{
		sinks.push_back(sink);
		Logger::instance().sinks.push_back(sink);
	}

}
