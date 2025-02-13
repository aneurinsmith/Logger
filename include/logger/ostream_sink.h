
#pragma once
#include "base_sink.h"

namespace LOG
{
	class OStreamSink : public basesink
	{
	protected:
		void write(Message);

	};

	inline std::shared_ptr<OStreamSink> oStreamSink()
	{
		auto sink = std::make_shared<OStreamSink>();
		return sink;
	}
}
