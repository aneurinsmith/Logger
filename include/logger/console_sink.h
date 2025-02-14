
#pragma once
#include "base_sink.h"

namespace LOG
{
	class Console;
	class ConsoleSink : public basesink
	{
	public:
		ConsoleSink(std::string name, int size, std::string fmt);

	protected:
		void write(Message);

	private:
		Console* console;
	};

	inline std::shared_ptr<ConsoleSink> consoleSink(std::string name = "", int size = 100, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<ConsoleSink>(name, size, fmt);
		return sink;
	}
}
