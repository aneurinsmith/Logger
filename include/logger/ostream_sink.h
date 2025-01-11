
#pragma once
#include "base_sink.h"
#include <memory>

namespace LOG
{
	class OStreamSink : public basesink
	{
	public:

		void print(LOG::Level lvl, std::string msg);

	};

	inline std::shared_ptr<OStreamSink> oStreamSink()
	{
		auto sink = std::make_shared<OStreamSink>();
		return sink;
	}
}
