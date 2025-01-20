
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <iostream>

namespace LOG
{
	class Terminal
	{
	public:

		Terminal();
		~Terminal();

		void enqueue(std::string msg)
		{
			q.push(msg);
		}

	private:
		static void ThreadEntry(void* data);

		bool is_running;
		std::thread thread_handle;
		void* window_handle;
		void* output_handle;
		std::queue<std::string> q;
		std::mutex m;
		std::condition_variable cv;

		std::string dequeue()
		{
			std::string _msg = q.front();
			q.pop();

			return _msg;
		}

	};
}
