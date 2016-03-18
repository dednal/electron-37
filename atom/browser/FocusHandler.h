#include <process.h>
#include <windows.h>

//using namespace std;
namespace atom {

	class FocusHandler
	{
	public:
		static FocusHandler* getInstance(HWND hWnd);
		static FocusHandler* getInstance();
		static void activeWindow(HWND hWnd);
		void start();
		void stop();
		void focus();
		static void call_focus(void* ptr);

	private:
		static FocusHandler* instance;
		FocusHandler(HWND hWnd);
		bool shouldThreadStop;
		bool threadStarted;
		HWND current_hWnd;
	};
}
