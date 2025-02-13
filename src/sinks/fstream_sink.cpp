
#include "fstream_sink.h"
#include <iostream>

namespace LOG
{
	FStreamSink::FStreamSink()
	{
		file.open("log.txt", std::ios::out);
	}

	FStreamSink::~FStreamSink()
	{
		file.close();
	}

	void FStreamSink::write(Message msg)
	{
		std::string log_level = "";

		switch (msg.lvl) {
		case LOG::TRACE: log_level = "[TRACE]  "; break;
		case LOG::DEBUG: log_level = "[DEBUG]  "; break;
		case LOG::INFO:  log_level = " [INFO]  "; break;
		case LOG::WARN:  log_level = " [WARN]  "; break;
		case LOG::ERROR: log_level = "[ERROR]  "; break;
		case LOG::FATAL: log_level = "[FATAL]  "; break;
		}

		file << msg.ts << "\t" << log_level << msg.msg << std::endl;
	}
}
