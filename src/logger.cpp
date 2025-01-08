
#include "logger.h"

using namespace std;

namespace LOG {

	Logger::Logger() 
		: m_lvl(NONE) {};

	Logger& Logger::instance() {
		static Logger s_logger;
		return s_logger;
	}

	void Logger::set_level(Level lvl) {
		m_lvl = lvl;
	}
	Level Logger::get_level() {
		return m_lvl;
	}

	void Logger::print(string msg) {
		print(m_lvl, msg);
	}

	void Logger::print(Level lvl, string msg) {
		string output;

		switch (lvl) {
		case TRACE :
			output = "[trace] ";
			break;
		case DEBUG:
			output = "[debug] ";
			break;
		case INFO:
			output = "[info] ";
			break;
		case WARN:
			output = "[warn] ";
			break;
		case ERROR:
			output = "[error] ";
			break;
		case FATAL:
			output = "[fatal] ";
			break;
		}

		output.append(msg);
		printf("%s\n", output.c_str());
	}

}