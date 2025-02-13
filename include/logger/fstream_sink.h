
#pragma once
#include "base_sink.h"
#include <fstream>

namespace LOG
{
	class FStreamSink : public basesink
	{
	public:
		FStreamSink();
		~FStreamSink();

	protected:
		void write(Message);

	private:
		std::fstream file;

	};

	inline std::shared_ptr<FStreamSink> fStreamSink()
	{
		auto sink = std::make_shared<FStreamSink>();
		return sink;
	}
}
