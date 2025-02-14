
#include "fstream_sink.h"
#include "timer.h"
#include <iostream>
#include <iomanip>

#ifdef win32
	#include <windows.h>
	#include <shlobj.h>
#endif

namespace LOG
{
	FStreamSink::FStreamSink(std::string name, std::string fmt) :
		basesink(fmt),
		m_name(name + ".log")
	{
		m_file.open(m_name, std::ios::out);
	}

	FStreamSink::~FStreamSink()
	{
		m_file.close();
	}

	void FStreamSink::write(Message msg)
	{
		m_file << msg.get_fullString() << std::endl;
	}
}
