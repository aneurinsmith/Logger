
#include "fstream_sink.h"
#include <fstream>

namespace LOG
{
	FStreamSink::FStreamSink()
	{
		
	}

	void FStreamSink::print(Message msg)
	{
		std::fstream file;
		file.open("log.txt", std::ios::out | std::ios::trunc);
		file << msg.msg << std::endl;
		file.close();
	}
}
