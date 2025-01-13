
#pragma once
#include "base_sink.h"
#include <memory>

namespace LOG
{
	class OStreamSink : public basesink
	{
	public:
		using basesink::basesink;

	protected:
		void write(std::string msg);

	};

	inline std::shared_ptr<OStreamSink> oStreamSink(LOG::Level lvl = (Level)0, std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<OStreamSink>(lvl, fmt);
		return sink;
	}
}
