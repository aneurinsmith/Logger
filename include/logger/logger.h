
#pragma once
#include <string>
#include <memory>
#include <vector>

#include "console_sink.h"
#include "fstream_sink.h"
#include "ostream_sink.h"

namespace LOG 
{
	enum Level : int;
	class Logger 
	{
	public:

		Logger(LOG::Level lvl = (Level)0);
		Logger(std::shared_ptr<basesink> sink, LOG::Level lvl = (Level)0);

		static Logger& instance();

		void set_level(LOG::Level lvl);

		void print(LOG::Level lvl, std::string msg);

		void add_sink(std::shared_ptr<basesink> sink);

	protected:
		std::vector<std::shared_ptr<basesink>> m_sinks;
		std::string m_fmt;
		LOG::Level m_lvl;

	private:
		bool is_loaded(std::vector<std::shared_ptr<basesink>> sinks, std::shared_ptr<basesink> sink);
	};

	inline void print(std::string msg)
	{
		Logger::instance().print(NONE, msg);
	}
	inline void print(LOG::Level lvl, std::string msg)
	{
		Logger::instance().print(lvl, msg);
	}
	inline void trace(std::string msg) 
	{
		Logger::instance().print(TRACE, msg);
	}
	inline void debug(std::string msg) 
	{
		Logger::instance().print(DEBUG, msg);
	}
	inline void info(std::string msg) 
	{
		Logger::instance().print(INFO, msg);
	}
	inline void warn(std::string msg) 
	{
		Logger::instance().print(WARN, msg);
	}
	inline void error(std::string msg) 
	{
		Logger::instance().print(ERROR, msg);
	}
	inline void fatal(std::string msg) 
	{
		Logger::instance().print(FATAL, msg);
	}

}
