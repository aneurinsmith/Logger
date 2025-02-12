
#pragma once
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

		virtual void print(Message msg) = 0;

	};
}
