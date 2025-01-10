
#include "terminal_sink.h"
#include <iostream>

namespace LOG
{
	void TerminalSink::print(LOG::Level lvl, std::string msg)
	{
		std::cout << msg << std::endl;
	}
}
