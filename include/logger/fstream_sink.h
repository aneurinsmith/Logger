
#pragma once
#include "base_sink.h"

namespace LOG
{
	class FStreamSink : public basesink
	{
	public:
		FStreamSink();

	protected:
		void print(Message);

	};

	inline std::shared_ptr<FStreamSink> fStreamSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<FStreamSink>();
		return sink;
	}
}
