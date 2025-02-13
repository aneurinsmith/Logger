
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
		file << msg.get_fullString() << std::endl;
	}
}
