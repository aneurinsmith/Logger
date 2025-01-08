
#include "logger.h"
#include "tools/chrono.h"

using namespace std;

namespace LOG 
{
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
		stringstream stream;

		switch (lvl) {
		case TRACE :
			stream << "[TRACE]  ";
			break;
		case DEBUG:
			stream << "[DEBUG]  ";
			break;
		case INFO:
			stream << " [INFO]  ";
			break;
		case WARN:
			stream << " [WARN]  ";
			break;
		case ERROR:
			stream << "[ERROR]  ";
			break;
		case FATAL:
			stream << "[FATAL]  ";
			break;
		default:
			stream << "";
		}

		stream << msg;
		printf("%s\n", stream.str().c_str());
	}
}