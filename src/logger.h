
#pragma once
#include <string>

enum Level {
	NONE,
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL
};

namespace LOG {
	class Logger {
	public:

		Logger();

		static Logger& instance();

		void set_level(Level lvl);
		Level get_level();

		void print(std::string msg);
		void print(Level lvl, std::string msg);

	private:
		Level m_lvl;
	};

	inline void set_level(Level lvl) {
		Logger::instance().set_level(lvl);
	}
	inline void print(std::string msg) {
		Logger::instance().print(Logger::instance().get_level(), msg);
	}
	inline void print(Level lvl, std::string msg) {
		Logger::instance().print(lvl, msg);
	}
	inline void trace(std::string msg) {
		Logger::instance().print(TRACE, msg);
	}
	inline void debug(std::string msg) {
		Logger::instance().print(DEBUG, msg);
	}
	inline void info(std::string msg) {
		Logger::instance().print(INFO, msg);
	}
	inline void warn(std::string msg) {
		Logger::instance().print(WARN, msg);
	}
	inline void error(std::string msg) {
		Logger::instance().print(ERROR, msg);
	}
	inline void fatal(std::string msg) {
		Logger::instance().print(FATAL, msg);
	}
}
