
#include <thread>

namespace LOG
{
	class Terminal
	{
	public:

		Terminal();
		~Terminal();

	private:
		static void ThreadEntry(void* data);

		bool is_running;
		std::thread thread_handle;
		void* window_handle;

	};
}
