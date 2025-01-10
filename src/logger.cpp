
#include "logger.h"

namespace LOG 
{
	Logger::Logger() : m_fmt("%Y/%m/%d %H:%M:%S.%f") 
	{
		m_lvl = DEBUG;
	}

	Logger::Logger(std::string fmt) : m_fmt(fmt) 
	{
		m_lvl = DEBUG;
	}

	Logger& Logger::instance() 
	{
		static Logger s_logger;
		return s_logger;
	}

	void Logger::set_level(Level lvl) 
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

	void Logger::print(Level lvl, std::string msg) 
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
		case TRACE: stream << "[TRACE]  "; break;
		case DEBUG: stream << "[DEBUG]  "; break;
		case INFO:  stream << " [INFO]  "; break;
		case WARN:  stream << " [WARN]  "; break;
		case ERROR: stream << "[ERROR]  "; break;
		case FATAL: stream << "[FATAL]  "; break;
		case NONE: break;
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