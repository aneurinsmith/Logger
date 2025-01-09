
#pragma once
#include "base_sink.h"
#include <iostream>

class OStreamSink : public basesink
{
public:

	void print(Level lvl, std::string msg) {
		if(lvl >= m_lvl) std::cout << msg;
	}

};
