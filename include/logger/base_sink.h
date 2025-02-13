
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
	private:
		Level m_lvl;
		std::uint64_t m_ts;
		std::string m_msg;

		std::string m_tsString;
		std::string m_lvlString;

	public:

		Message(std::string msg, Level lvl = NONE, std::string fmt = "", std::uint64_t ts = Timer::get_epoch()) :
			m_ts(ts), m_lvl(lvl), m_msg(msg)
		{
			if (fmt.size() > 0) {
				m_tsString = Timer::get_datetime(fmt, ts);
				m_tsString += std::string(8 - (m_tsString.length() % 8), ' ');
			}

			switch (lvl) {
			case LOG::TRACE: m_lvlString = "[TRACE]"; break;
			case LOG::DEBUG: m_lvlString = "[DEBUG]"; break;
			case LOG::INFO:  m_lvlString = " [INFO]"; break;
			case LOG::WARN:  m_lvlString = " [WARN]"; break;
			case LOG::ERROR: m_lvlString = "[ERROR]"; break;
			case LOG::FATAL: m_lvlString = "[FATAL]"; break;
			}
		}

		Level get_lvl()
		{
			return m_lvl;
		}

		std::string get_tsString()
		{
			return m_tsString;
		}

		std::string get_lvlString()
		{
			return m_lvlString;
		}

		std::string get_msgString()
		{
			return m_lvl != NONE ? "  " + m_msg : m_msg;
		}

		std::string get_fullString()
		{
			return get_tsString() + get_lvlString() + get_msgString();
		}
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
