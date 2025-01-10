
#pragma once
#include "base_sink.h"
#include <iostream>
#include <queue>

namespace LOG
{
	class TerminalSink : public basesink
	{
	public:

		void print(Level lvl, std::string msg)
		{
			std::cout << msg << std::endl;
		}

	};

	inline std::shared_ptr<TerminalSink> terminalSink()
	{
		auto sink = std::make_shared<TerminalSink>();
		return sink;
	}
}