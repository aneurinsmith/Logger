
#pragma once
#include "base_sink.h"

namespace LOG
{
	class OStreamSink : public basesink
	{
	public:
		OStreamSink();

	protected:
		void print(Message);

	};

	inline std::shared_ptr<OStreamSink> oStreamSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<OStreamSink>();
		return sink;
	}
}
