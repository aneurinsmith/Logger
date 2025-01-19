
namespace LOG
{
	class Terminal
	{
	public:

		Terminal();
		~Terminal();

	private:
		void* thread_handle;
		void* window_handle;
	};
}
