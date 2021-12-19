#include "Window.hpp"
#include <ShellScalingApi.h> // notwendig für high dpi scaling

bool key[256];

LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		key[wParam] = TRUE;
		break;
	case WM_KEYUP:
		key[wParam] = FALSE;
		break;
	case WM_MOUSEMOVE:
		//motionPos.xWin = GET_X_LPARAM(lParam);
		//motionPos.yWin = GET_Y_LPARAM(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window(int _width, int _height, bool _fullScreen) {
	hInstance = GetModuleHandle(nullptr);
	windowName = "Vulkan App";
	windowClassName = "VulkanWindow";
	width = _width;
	height = _height;
	fullScreen = _fullScreen;

	wc = {};
	wc.lpszClassName = windowClassName.c_str();
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.lpfnWndProc = mainWndProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	RegisterClass(&wc);
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE); // notwendig für high dpi scaling

	if (fullScreen) {
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize = sizeof(screenSettings);
		screenSettings.dmPelsWidth = width;
		screenSettings.dmPelsHeight = height;
		screenSettings.dmBitsPerPel = 32;
		screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&screenSettings, 0);

		dwStyle = WS_POPUP;
	}
	else {
		dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}
}

Window::~Window() {
	std::cout << "Close Window." << std::endl;
	if (window) DestroyWindow(window);
}

void Window::createWindow() {
	window = CreateWindow(windowClassName.c_str(), windowName.c_str(), dwStyle, 0, 0, width, height, NULL, NULL, hInstance, NULL);
}

void Window::showWindow() {
	if (fullScreen) ShowCursor(0);
	SetCursorPos(1920, 1080);
	ShowWindow(window, SW_SHOW);
}

bool Window::checkMessage() {
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return TRUE;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	else {
		if (key[VK_ESCAPE]) return TRUE;
	}
	return FALSE;
}

HWND Window::getWindow() { return window; }

HINSTANCE Window::getInstance() { return hInstance; }

int Window::getWidth() { return width; }

int Window::getHeight() { return height; }