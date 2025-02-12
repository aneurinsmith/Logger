
#include "logger.h"
#include "timer.h"
#include <iostream>

namespace LOG 
{
	Logger::Logger(LOG::Level lvl, std::string fmt) : 
		m_lvl(lvl), 
		m_fmt(fmt) {}
	Logger::Logger(std::shared_ptr<basesink> sink, LOG::Level lvl, std::string fmt) :
		m_lvl(lvl),
		m_fmt(fmt)
	{
		add_sink(sink);
	}

	Logger& Logger::instance() 
	{
		static Logger s_logger;
		return s_logger;
	}

	void Logger::set_level(LOG::Level lvl) 
	{
		m_lvl = lvl;
	}

	void Logger::set_format(std::string fmt) 
	{
		m_fmt = fmt;
	}

	void Logger::print(LOG::Level lvl, std::string msg) 
	{
		if (lvl >= m_lvl) {
			std::string timestamp = Timer::get_datetime(m_fmt);
			std::string log_level = "";

			switch (lvl) {
			case LOG::TRACE: log_level = "[TRACE]"; break;
			case LOG::DEBUG: log_level = "[DEBUG]"; break;
			case LOG::INFO:  log_level = " [INFO]"; break;
			case LOG::WARN:  log_level = " [WARN]"; break;
			case LOG::ERROR: log_level = "[ERROR]"; break;
			case LOG::FATAL: log_level = "[FATAL]"; break;
			case LOG::NONE: break;
			}

			Message m;
			m.msg = msg;
			m.lvl = lvl;
			m.ts = Timer::get_datetime(m_fmt);

			for (auto sink : sinks) {
				sink->print(m);
			}
		}
	}

	void Logger::add_sink(std::shared_ptr<basesink> sink) 
	{
		sinks.push_back(sink);
		Logger::instance().sinks.push_back(sink);
	}

}
