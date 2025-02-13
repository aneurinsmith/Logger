
#pragma once
#include "base_sink.h"

namespace LOG
{
	class OStreamSink : public basesink
	{
	public:
		OStreamSink(std::string fmt) :
			basesink(fmt) {}

	protected:
		void write(Message);

	};

	inline std::shared_ptr<OStreamSink> oStreamSink(std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<OStreamSink>(fmt);
		return sink;
	}
}
