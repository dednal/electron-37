#include "FocusHandler.h"

namespace atom {

FocusHandler* FocusHandler::instance = NULL;

FocusHandler* FocusHandler::getInstance(HWND hWnd)
{
	if (instance == NULL)
	{
		instance = new FocusHandler(hWnd);
	}
	return instance;

}

FocusHandler* FocusHandler::getInstance()
{
	return instance;
}

FocusHandler::FocusHandler(HWND hWnd)
{
	current_hWnd = hWnd;
	shouldThreadStop = false;
	threadStarted = false;
}

void FocusHandler::activeWindow(HWND hWnd)
{
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	SetActiveWindow(hWnd);
	EnableWindow(hWnd, TRUE);
	SwitchToThisWindow(hWnd, TRUE);
}

void FocusHandler::call_focus(void* ptr)
{
	reinterpret_cast<FocusHandler*>(ptr)->focus();
}

void FocusHandler::start()
{
	if (!this->instance->threadStarted)
	{
		this->instance->threadStarted = true;
		_beginthread(&FocusHandler::call_focus, 0, NULL);
	}

}

void FocusHandler::stop()
{
	this->instance->shouldThreadStop = true;
	threadStarted = false;
}

void FocusHandler::focus()
{
	bool isActive = true;
	while (!this->instance->shouldThreadStop && this->instance->current_hWnd != NULL)
	{

		HWND currentActiveWindow = GetForegroundWindow();
		HDC hDC = ::GetWindowDC(NULL);
		if (currentActiveWindow != this->instance->current_hWnd)
		{
			isActive = false;
			::SetWindowPos(this->instance->current_hWnd, HWND_TOPMOST, 0, 0, ::GetDeviceCaps(hDC, HORZRES), ::GetDeviceCaps(hDC, VERTRES), SWP_NOMOVE | SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE);
			FocusHandler::activeWindow(this->instance->current_hWnd);
		}
		else
		{
			if (!isActive)
			{
				::SetWindowPos(this->instance->current_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(this->instance->current_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				FocusHandler::activeWindow(this->instance->current_hWnd);
			}
			isActive = true;
		}
		Sleep(1000L);
	}
	this->instance->shouldThreadStop = false;
	_endthread();
}

}
