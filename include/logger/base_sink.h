
#pragma once
#include "timer.h"
#include <sstream>
#include <memory>

namespace LOG
{
	enum Level : int
	{
		NONE,
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
	};

	struct Message
	{
		Level lvl;
		std::string ts;
		std::string msg;
	};

	class basesink
	{
		friend class Logger;

	protected:
		Level m_lvl;
		std::string m_fmt;

		void set_level(LOG::Level lvl)
		{
			m_lvl = lvl;
		}
		void set_format(std::string fmt)
		{
			m_fmt = fmt;
		}

		virtual void write(Message msg) = 0;

	};
}
