
#pragma once
#include "base_sink.h"
#include <iostream>

namespace LOG
{
	class OStreamSink : public basesink
	{
	public:

		void print(Level lvl, std::string msg)
		{
			if (lvl >= m_lvl) std::cout << msg << std::endl;;
		}

	};

	inline std::shared_ptr<OStreamSink> oStreamSink()
	{
		auto sink = std::make_shared<OStreamSink>();
		return sink;
	}
}