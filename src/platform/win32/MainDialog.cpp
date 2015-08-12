#include "MainDialog.h"


MainWindow::MainWindow()
{

}


MainWindow::~MainWindow()
{

}


bool MainWindow::registerClass(HINSTANCE hInstance)
{
	LoadString(hInstance, IDS_APP_TITLE, szTitle, 256);
	LoadString(hInstance, IDC_NES, szWindowClass, 256);


	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC) &(MainWindow::WndProc);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NES));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_NES);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	ATOM result = RegisterClassEx(&wcex);

	if (result == 0)
		return false;
	else 
		return true;
}

bool MainWindow::createWindow(HINSTANCE hInstance, int nCmdShow)
{
	this->hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 512, 480, NULL, NULL, hInstance, NULL);

	if (!this->hwnd)
		return false;
	
	ShowWindow(this->hwnd, nCmdShow);
	UpdateWindow(this->hwnd);

	gdiRenderer->setRedrawListener(new MainWindowRedrawer(this->hwnd));

	return true;
}


void MainWindow::setWindowSize(int clientAreaWidth, int clientAreaHeight)
{
	setWindowSize2(this->hwnd, clientAreaWidth, clientAreaHeight);
}


void MainWindow::setWindowSize2(HWND hwndMain, int clientAreaWidth, int clientAreaHeight)
{
	RECT areaRect;
	areaRect.left = 0;
	areaRect.right = clientAreaWidth;
	areaRect.top = 0;
	areaRect.bottom = clientAreaHeight;

	AdjustWindowRect(&areaRect, WS_OVERLAPPEDWINDOW, TRUE);

	RECT winRect;
	GetWindowRect(hwndMain, &winRect);

	SetWindowPos(hwndMain, 0, winRect.left, winRect.top, areaRect.right - areaRect.left, areaRect.bottom - areaRect.top, 0);
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hdcFrameBuffer;
	RECT clientAreaRect;
	iNesRom* rom;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_LOADROM:
			rom = RomLoader::chooseRomFileToLoad(hWnd);
			emu->loadROM(rom);
			break;
		case ID_WINDOWSIZE_1X:
			setWindowSize2(hWnd, 256, 240);
			break;
		case ID_WINDOWSIZE_2X:
			setWindowSize2(hWnd, 512, 480);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hdcFrameBuffer = gdiRenderer->getFrameBufferHDC();

		GetClientRect(hWnd, &clientAreaRect);

		StretchBlt(hdc, 0, 0, clientAreaRect.right, clientAreaRect.bottom, hdcFrameBuffer, 0, 0, 256, 240, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


