
#ifdef libx11
#include "terminal.h"

namespace LOG
{
	Terminal::Terminal() :
		is_running(true)
	{
		thread = std::thread(&Terminal::WindowThread, this);
	}

	Terminal::~Terminal()
	{

	}

	void Terminal::enqueue(std::string msg)
	{

	}

	void Terminal::WindowThread(void* data)
	{

	}
}

#endif
