
#pragma once
#include "base_sink.h"
#include <memory>

namespace LOG
{
	class TerminalSink : public basesink
	{
	public:

		void print(LOG::Level lvl, std::string msg);

	};

	inline std::shared_ptr<TerminalSink> terminalSink()
	{
		auto sink = std::make_shared<TerminalSink>();
		return sink;
	}
}
