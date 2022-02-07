#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <fstream>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>


class Window
{
public:
	bool Initialize(HINSTANCE hInstance, int showWnd);

	const int GetWindowWidth()
	{
		return mWidth;
	}

	const int GetWindowHeight()
	{ 
		return mHeight;
	}

	HWND GetWindowHandle() 
	{ 
		return mWnd; 
	}

	void SetWindowTitle(std::string title) 
	{
		SetWindowTextA(mWnd, title.c_str());
	}

private:
	bool InitializeConsole();

private:
	HWND mWnd = {};
	const int mWidth  = 800;
	const int mHeight = 600;
};

