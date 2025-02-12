
#pragma once
#include "base_sink.h"

namespace LOG
{
	class OStreamSink : public basesink
	{
	public:
		OStreamSink(LOG::Level lvl, std::string fmt);

	protected:
		void write(std::string msg);

	};

	inline std::shared_ptr<OStreamSink> oStreamSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<OStreamSink>(lvl, fmt);
		return sink;
	}
}
