
#pragma once
#include <string>
#include <memory>
#include <vector>

#include "sinks/ostream_sink.h"
#include "sinks/terminal_sink.h"

enum Level: int 
{
	NONE,
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL,
};

namespace LOG 
{
	class Logger 
	{
	public:

		Logger();
		Logger(std::string fmt);

		static Logger& instance();

		void set_level(Level lvl);
		void set_format(std::string fmt);

		void print(Level lvl, std::string msg);

		void add_sink(std::shared_ptr<basesink> sink);

	protected:
		std::vector<std::shared_ptr<basesink>> sinks;
		std::string m_fmt;
		Level m_lvl;
	};


	inline void set_level(Level lvl) 
	{
		Logger::instance().set_level(lvl);
	}
	inline void print(std::string msg) 
	{
		Logger::instance().print(NONE, msg);
	}
	inline void print(Level lvl, std::string msg) 
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
