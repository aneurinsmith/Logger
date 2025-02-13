
#include "logger.h"
#include "timer.h"
#include <algorithm>

namespace LOG 
{
	Logger::Logger(LOG::Level lvl) :
		m_lvl(lvl)
	{

	}
	Logger::Logger(std::shared_ptr<basesink> sink, LOG::Level lvl) :
		m_lvl(lvl)
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

	void Logger::print(LOG::Level lvl, std::string msg) 
	{
		if (lvl >= m_lvl) {
			std::uint64_t epoch = Timer::get_epoch();
			for (auto sink : m_sinks) {
				sink->write(Message(msg, lvl, sink->m_fmt, epoch));
			}
		}
	}

	void Logger::add_sink(std::shared_ptr<basesink> sink) 
	{
		if (!is_loaded(m_sinks, sink)) {
			m_sinks.push_back(sink);
		}
		if (!is_loaded(Logger::instance().m_sinks, sink)) {
			Logger::instance().m_sinks.push_back(sink);
		}
	}



	bool Logger::is_loaded(std::vector<std::shared_ptr<basesink>> sinks, std::shared_ptr<basesink> sink)
	{
		return (bool)std::count(sinks.begin(), sinks.end(), sink);
	}
}
