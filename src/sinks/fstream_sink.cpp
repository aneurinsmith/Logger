
#include "fstream_sink.h"
#include <fstream>

namespace LOG
{
	FStreamSink::FStreamSink(LOG::Level lvl, std::string fmt) :
		basesink(lvl, fmt) 
	{
		
	}

	void FStreamSink::write(std::string msg)
	{
		std::fstream file;
		file.open("log.txt", std::ios::out | std::ios::trunc);
		file << msg << std::endl;
		file.close();
	}
}
