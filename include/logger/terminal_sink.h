
#pragma once
#include "base_sink.h"
#include <memory>

namespace LOG
{
	class TerminalSink : public basesink
	{
	public:
		TerminalSink(LOG::Level lvl, std::string fmt);

	protected:
		void write(std::string msg);

	private:
		void* m_pipe;
	};

	inline std::shared_ptr<TerminalSink> terminalSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<TerminalSink>(lvl, fmt);
		return sink;
	}
}
