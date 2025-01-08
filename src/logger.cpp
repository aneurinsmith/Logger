
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

	void Logger::print(string msg) {
		print(m_lvl, msg);
	}

	void Logger::print(Level lvl, string msg) {
		string output;

		switch (lvl) {
		case TRACE :
			output = "[TRACE]  ";
			break;
		case DEBUG:
			output = "[DEBUG]  ";
			break;
		case INFO:
			output = " [INFO]  ";
			break;
		case WARN:
			output = " [WARN]  ";
			break;
		case ERROR:
			output = "[ERROR]  ";
			break;
		case FATAL:
			output = "[FATAL]  ";
			break;
		case NONE:
			output = "";
			break;
		}

		output.append(msg);
		printf("%s\n", output.c_str());
	}

}