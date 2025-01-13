
#include "logger.h"

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
		for (auto& sink : sinks) {
			sink->set_level(lvl);
		}
	}

	void Logger::set_format(std::string fmt) 
	{
		m_fmt = fmt;
		for (auto& sink : sinks) {
			sink->set_format(fmt);
		}
	}

	void Logger::print(LOG::Level lvl, std::string msg) 
	{
		for (auto sink : sinks) {
			sink->print(lvl, msg);
		}
	}

	void Logger::add_sink(std::shared_ptr<basesink> sink) 
	{
		sinks.push_back(sink);
		Logger::instance().sinks.push_back(sink);
	}

}
