
#pragma once
#include "base_sink.h"
#include <fstream>

namespace LOG
{
	class FStreamSink : public basesink
	{
	public:
		FStreamSink(std::string fmt);
		~FStreamSink();

	protected:
		void write(Message);

	private:
		std::fstream file;

	};

	inline std::shared_ptr<FStreamSink> fStreamSink(std::string fmt = "%Y/%m/%d %H:%M:%S.%f")
	{
		auto sink = std::make_shared<FStreamSink>(fmt);
		return sink;
	}
}
