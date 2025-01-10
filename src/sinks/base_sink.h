
#pragma once

namespace LOG
{
	enum Level :int;
	class basesink
	{
	public:

		void set_level(const Level lvl)
		{
			m_lvl = lvl;
		}
		virtual void print(Level lvl, std::string msg) = 0;

	protected:
		Level m_lvl;
		std::string m_fmt;
	};
}
