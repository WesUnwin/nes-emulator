#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "windows.h"

#include "resource.h"

#include "NES.h"
#include "RomLoader.h"


INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	bool registerClass(HINSTANCE hInstance);

	bool createWindow(HINSTANCE hInstance, int nCmdShow);

	void setWindowSize(int clientAreaWidth, int clientAreaHeight);

private:
	static void MainWindow::setWindowSize2(HWND hwndMain, int clientAreaWidth, int clientAreaHeight);

	HWND hwnd;

	TCHAR szTitle[256];					// The title bar text
	TCHAR szWindowClass[256];			// the main window class name

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};


class MainWindowRedrawer : public RedrawListener
{
public:
	MainWindowRedrawer(HWND hwndMain) { this->hwndMainWindow = hwndMain; }
	~MainWindowRedrawer() {	}

	virtual void onRedrawRequest()
	{
		InvalidateRect(this->hwndMainWindow, 0, FALSE);
	}

private:
	HWND hwndMainWindow;
};



#endif
