
#include "console_sink.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef win32
#include <Windows.h>
#endif

namespace LOG
{
	ConsoleSink::ConsoleSink(LOG::Level lvl, std::string fmt) : basesink(lvl, fmt)
	{

	}

	void ConsoleSink::write(std::string msg)
	{
		console.push(msg);
	}
}
