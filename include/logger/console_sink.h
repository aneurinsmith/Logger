
#pragma once
#include "base_sink.h"
#include "console.h"

namespace LOG
{
	class ConsoleSink : public basesink
	{
	public:
		using basesink::basesink;

	protected:
		void write(std::string msg);

	private:
		Console console;
	};

	inline std::shared_ptr<ConsoleSink> consoleSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<ConsoleSink>(lvl, fmt);
		return sink;
	}
}
