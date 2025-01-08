
#pragma once
#include <string>

enum Level :int;
class basesink {
public:

	virtual void set_level(const Level) = 0;
	virtual void print(const Level, std::string msg) = 0;

protected:
	Level m_lvl;
};
