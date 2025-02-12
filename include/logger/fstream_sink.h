
#pragma once
#include "base_sink.h"

namespace LOG
{
	class FStreamSink : public basesink
	{
	public:
		FStreamSink(LOG::Level lvl, std::string fmt);

	protected:
		void write(std::string msg);

	};

	inline std::shared_ptr<FStreamSink> fStreamSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<FStreamSink>(lvl, fmt);
		return sink;
	}
}
