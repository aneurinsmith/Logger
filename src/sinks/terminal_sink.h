
#pragma once
#include "base_sink.h"
#include <iostream>

class TerminalSink : public basesink
{
public:

	void print(Level lvl, std::string msg)
	{
		std::cout << msg << std::endl;
	}

};
