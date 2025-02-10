
#pragma once
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <memory>

namespace LOG
{
	enum Level : int
	{
		NONE,
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
	};

	class basesink
	{
		friend class Logger;
	public:

		basesink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f") :
			m_lvl(lvl),
			m_fmt(fmt) {}

		void set_format(std::string fmt)
		{
			m_fmt = fmt;
		}
		void set_level(LOG::Level lvl)
		{
			m_lvl = lvl;
		}

		void print(LOG::Level lvl, std::string msg)
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

			if (lvl >= m_lvl) {
				write(stream.str());
			}
		}

	protected:
		LOG::Level m_lvl;
		std::string m_fmt;

		virtual void write(std::string msg) = 0;

	};
}
