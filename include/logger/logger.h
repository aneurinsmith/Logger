
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

		template<typename... Args>
		void print(LOG::Level lvl, Args... msgs)
		{
			std::stringstream stream;
			using List = int[];
			(void)List
			{
				0, ((void)(stream << msgs), 0)...
			};

			if (lvl >= m_lvl) {
				std::uint64_t epoch = Timer::get_epoch();
				for (auto sink : m_sinks) {
					sink->write(Message(stream.str(), lvl, sink->m_fmt, epoch));
				}
			}
		}

		void add_sink(std::shared_ptr<basesink> sink);

	protected:
		std::vector<std::shared_ptr<basesink>> m_sinks;
		std::string m_fmt;
		LOG::Level m_lvl;

	private:
		bool is_loaded(std::vector<std::shared_ptr<basesink>> sinks, std::shared_ptr<basesink> sink);
	};

	template<typename... Args>
	inline void print(Args... msgs)
	{
		Logger::instance().print(NONE, msgs...);
	}
	template<typename... Args>
	inline void print(LOG::Level lvl, Args... msgs)
	{
		Logger::instance().print(lvl, msgs...);
	}
	template<typename... Args>
	inline void trace(Args... msgs)
	{
		Logger::instance().print(TRACE, msgs...);
	}
	template<typename... Args>
	inline void debug(Args... msgs)
	{
		Logger::instance().print(DEBUG, msgs...);
	}
	template<typename... Args>
	inline void info(Args... msgs)
	{
		Logger::instance().print(INFO, msgs...);
	}
	template<typename... Args>
	inline void warn(Args... msgs)
	{
		Logger::instance().print(WARN, msgs...);
	}
	template<typename... Args>
	inline void error(Args... msgs)
	{
		Logger::instance().print(ERROR, msgs...);
	}
	template<typename... Args>
	inline void fatal(Args... msgs)
	{
		Logger::instance().print(FATAL, msgs...);
	}
}
