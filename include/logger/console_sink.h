
#pragma once
#include "base_sink.h"

namespace LOG
{
	class Console;
	class ConsoleSink : public basesink
	{
	public:
		ConsoleSink();

	protected:
		void write(Message);

	private:
		Console* console;
	};

	inline std::shared_ptr<ConsoleSink> consoleSink()
	{
		auto sink = std::make_shared<ConsoleSink>();
		return sink;
	}
}
