
#include "ostream_sink.h"
#include <iostream>

namespace LOG
{
	void OStreamSink::print(LOG::Level lvl, std::string msg)
	{
		if (lvl >= m_lvl) std::cout << msg << std::endl;;
	}
}
