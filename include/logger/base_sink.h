
#pragma once
#include <string>

namespace LOG
{
	enum Level :int;
	class basesink
	{
	public:

		void set_level(const LOG::Level lvl)
		{
			m_lvl = lvl;
		}
		virtual void print(LOG::Level lvl, std::string msg) = 0;

	protected:
		LOG::Level m_lvl;
		std::string m_fmt;
	};
}
