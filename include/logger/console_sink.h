
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
		void print(Message);

	private:
		Console* console;
	};

	inline std::shared_ptr<ConsoleSink> consoleSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<ConsoleSink>();
		return sink;
	}
}
