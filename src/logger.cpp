
#include "logger.h"

namespace LOG 
{
	Logger::Logger() : 
		m_lvl(TRACE),
		m_fmt("%Y/%m/%d %H:%M:%S.%f") {};

	Logger& Logger::instance() {
		static Logger s_logger;
		return s_logger;
	}

	void Logger::set_level(Level lvl) {
		m_lvl = lvl;
	}

	void Logger::set_format(std::string fmt) {
		m_fmt = fmt;
	}

	void Logger::print(std::string msg) {
		print(m_lvl, msg);
	}

	void Logger::print(Level lvl, std::string msg) {
		std::string fmt = m_fmt;

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
		std::cout << std::put_time(&tm, fmt.c_str()) << " ";

		switch (lvl) {
		case TRACE :
			std::cout << "[TRACE]  ";
			break;
		case DEBUG:
			std::cout << "[DEBUG]  ";
			break;
		case INFO:
			std::cout << "[INFO]   ";
			break;
		case WARN:
			std::cout << "[WARN]   ";
			break;
		case ERROR:
			std::cout << "[ERROR]  ";
			break;
		case FATAL:
			std::cout << "[FATAL]  ";
			break;
		case NONE:
			break;
		}

		std::cout << msg << std::endl;
	}
}