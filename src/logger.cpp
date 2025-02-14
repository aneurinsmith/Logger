
#include "logger.h"
#include "timer.h"
#include <algorithm>

namespace LOG 
{
	Logger::Logger() :
		Logger(NONE) {}
	Logger::Logger(LOG::Level lvl) :
		m_lvl(lvl) {}
	Logger::Logger(std::shared_ptr<basesink> sink) :
		Logger(NONE, sink) {}
	Logger::Logger(LOG::Level lvl, std::shared_ptr<basesink> sink) :
		m_lvl(lvl)
	{
		add_sink(sink);
	}



	Logger& Logger::instance() 
	{
		static Logger s_logger(LOG::oStreamSink());
		return s_logger;
	}

	void Logger::set_level(LOG::Level lvl) 
	{
		m_lvl = lvl;
	}

	void Logger::add_sink(std::shared_ptr<basesink> sink) 
	{
		if (!is_loaded(m_sinks, sink)) {
			m_sinks.push_back(sink);
		}
	}



	bool Logger::is_loaded(std::vector<std::shared_ptr<basesink>> sinks, std::shared_ptr<basesink> sink)
	{
		return (bool)std::count(sinks.begin(), sinks.end(), sink);
	}
}
