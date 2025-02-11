
#pragma once
#include "base_sink.h"

namespace LOG
{
	class Console;
	class ConsoleSink : public basesink
	{
	public:
		ConsoleSink(LOG::Level lvl, std::string fmt);

	protected:
		void write(std::string msg);

	private:
		Console* console;
	};

	inline std::shared_ptr<ConsoleSink> consoleSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<ConsoleSink>(lvl, fmt);
		return sink;
	}
}
