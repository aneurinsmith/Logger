
#include "console_sink.h"
#include "../console/console.h"

namespace LOG
{
	ConsoleSink::ConsoleSink(std::string name, int size, std::string fmt) :
		basesink(fmt),
		console(new Console(name, size)) 
	{

	}

	void ConsoleSink::write(Message msg)
	{
		console->push(msg);
	}
}
