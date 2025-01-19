
#include "terminal_sink.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef win32
#include <Windows.h>
#endif

namespace LOG
{
	TerminalSink::TerminalSink(LOG::Level lvl, std::string fmt) : basesink(lvl, fmt)
	{

	}

	void TerminalSink::write(std::string msg)
	{

	}
}
