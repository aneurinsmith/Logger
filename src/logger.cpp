
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
		for (auto sink : sinks) {
			sink->set_level(m_lvl);
		}
	}

	void Logger::set_format(std::string fmt) 
	{
		m_fmt = fmt;
		for (auto sink : sinks) {
			sink->set_format(m_fmt);
		}
	}

	void Logger::print(LOG::Level lvl, std::string msg) 
	{
		if (lvl >= m_lvl) {
			Message m;
			m.msg = msg;
			m.lvl = lvl;

			std::uint64_t epoch = Timer::get_epoch();

			for (auto sink : sinks) {
				if (lvl >= sink->m_lvl) {
					m.ts = Timer::get_datetime(sink->m_fmt, epoch);
					sink->write(m);
				}
			}
		}
	}

	void Logger::add_sink(std::shared_ptr<basesink> sink) 
	{
		sink->set_level(m_lvl);
		sink->set_format(m_fmt);
		sinks.push_back(sink);
		Logger::instance().sinks.push_back(sink);
	}

}
