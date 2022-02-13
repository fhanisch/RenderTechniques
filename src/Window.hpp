#pragma once
#include <Windows.h>
#include <iostream>

class Window {
	WNDCLASS wc;
	HWND window;
	HINSTANCE hInstance;
	std::string windowName;
	std::string windowClassName;
	int width;
	int height;
	bool fullScreen;
	DWORD dwStyle;
	DEVMODE screenSettings;
public:
	Window(int _width, int _height, bool _fullScreen);
	~Window();
	void createWindow();
	void showWindow();
	bool checkMessage();
	HWND getWindow();
	HINSTANCE getInstance();
	int getWidth();
	int getHeight();
};