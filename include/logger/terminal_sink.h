
#pragma once
#include "base_sink.h"
#include <memory>

namespace LOG
{
	class TerminalSink : public basesink
	{
	public:

		TerminalSink();

	protected:
		void write(std::string msg);

	private:
		void* m_pipe;
	};

	inline std::shared_ptr<TerminalSink> terminalSink()
	{
		auto sink = std::make_shared<TerminalSink>();
		return sink;
	}
}
