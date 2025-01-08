
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

	inline void print(std::string msg) {
		Logger::instance().print(Logger::instance().get_level(), msg);
	}
	inline void print(Level lvl, std::string msg) {
		Logger::instance().print(lvl, msg);
	}
}
