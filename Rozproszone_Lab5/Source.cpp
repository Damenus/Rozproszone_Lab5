//https://msdn.microsoft.com/pl-pl/library/bb384843.aspx
//https://social.msdn.microsoft.com/Forums/vstudio/en-us/17c2d97a-011b-4fb1-9563-4f095d9321e4/how-to-get-the-string-value-of-editbox-in-the-variable-in-win32-application?forum=vcgeneralt

// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;


// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwndButton;
HWND editctl;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 300,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	hwndButton = CreateWindowEx(0,
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		310,         // x position 
		10,         // y position 
		100,        // Button width
		50,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	editctl = CreateWindow(L"EDIT", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_WANTRETURN, 10, 10, 300, 100, hWnd, (HMENU)(101), hInst, 0);
	

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");
	RECT rect;

	HANDLE hCbMem;
	bool bAvail;
	HWND hwndNxtVwr = NULL;
	//LPSTR lpCbMem;
	//LPSTR lpProgMem;
	//LPSTR lpCbMem;
	//LPSTR lpProgMem;
	char* lpCbMem;
	char* lpProgMem = NULL;
	HGLOBAL hProgMem;

	switch (message)
	{
	case WM_CREATE:
		hwndNxtVwr = SetClipboardViewer(hWnd);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		
		bAvail = IsClipboardFormatAvailable(CF_TEXT); //czy schowek zawiera dane 
		if (bAvail) {
			OpenClipboard(hWnd);			
			hCbMem = GetClipboardData(CF_TEXT); 
			//	if (hCbMem != NULL) altenatywa dla dwóch linijek wy¿ej		
			
			hProgMem = GlobalAlloc(GHND, GlobalSize(hCbMem));
			if (hProgMem != NULL) {
				//lpCbMem = (LPSTR)GlobalLock(hCbMem);
				//lpProgMem = (LPSTR)GlobalLock(hProgMem);
				lpCbMem = (char*)GlobalLock(hCbMem);
				lpProgMem = (char*)GlobalLock(hProgMem);

				//lstrcpy(lpProgMem, lpCbMem);
				
				strcpy(lpProgMem, lpCbMem);
				
				size_t length = strlen(lpProgMem);
				wchar_t wtext[2000];
				mbstowcs(wtext, lpProgMem, strlen(lpProgMem) + 1);//Plus null
				
				DrawText(hdc, (LPCWSTR)wtext,
					-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


				GlobalUnlock(hCbMem);
				GlobalUnlock(hProgMem);
			}
	
			CloseClipboard();
		}

		GetClientRect(hWnd, &rect);
		EndPaint(hWnd, &ps);
		break;

	case WM_DRAWCLIPBOARD:
		if (hwndNxtVwr != NULL)
			/* jeœli nie jestem ostatni w ³añcuchu to muszê powiadomiæ
			nastêpny CbV */
		{
			SendMessage(hwndNxtVwr, message, wParam, lParam);
		}
		InvalidateRect(hWnd, NULL, TRUE); /* wygeneruje WM_PAINT */
		break;
	case WM_CHANGECBCHAIN:
		if ((HWND)wParam == hwndNxtVwr) /* jestem poprzednikiem usuwaj¹cego siê ? */
		{
			hwndNxtVwr = (HWND)LOWORD(lParam); /* handle nowego nastêpnika */
		}
		else
		{
			if (hwndNxtVwr != NULL) /* jeœli nie jestem ostatni w ³añcuchu to */
			{ /* muszê powiadomiæ mojego nastêpnika */
				SendMessage(hwndNxtVwr, message, wParam, lParam);
			}
		}
		break;
	case WM_DESTROY:
		ChangeClipboardChain(hWnd, hwndNxtVwr);
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if ((HWND)lParam == hwndButton) {

		//	TCHAR pStr[128];
		//	GetDlgItemText(hWnd, 101, pStr, 128);

			int wLen = SendMessage(editctl, WM_GETTEXTLENGTH, 0, 0);
			LPSTR textIn = (LPSTR)malloc(wLen + 1);
			SendMessage(editctl, WM_GETTEXT, wLen + 1, LPARAM(textIn));

			HANDLE hGlMem = GlobalAlloc(GHND, (DWORD)wLen + 1);

		
			LPSTR lpGlMem = (LPSTR)GlobalLock(hGlMem);
			
			for(int i = 0; i < wLen; i++)
			{
				*lpGlMem++ = textIn[i];
			}
			
			//for (int i = 0; i < wLen; *lpGlMem++ = *textIn && i++);

			GlobalUnlock(hGlMem);

			OpenClipboard(hWnd);
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hGlMem);
			CloseClipboard();
		}

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}