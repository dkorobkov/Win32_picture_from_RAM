// 
// An example of drawing from array in RAM to Windows window.
// It is lower level than using LoadImage(), GDI+ etc. 
// Of course it can be optimized but this one works :)
// It is a compilation and cleanup of other examples, whose
// authors' names are lost, sorry.
//
// See "ACTUAL EXAMPLE" below for the few precious lines of example 
// among all other crap belonging to Micro$oft.
// 
// By Dmitry Korobkov (dkorobkov at mail dot ru)
//
// Do WTF you want with this code. (WTFPL license, http://www.wtfpl.net/)
//


/*******************************************************************/
// This part comes from Microsoft Visual Studio and is not interesting. 

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


/*******************************************************************/
// THIS PART IS AN ACTUAL EXAMPLE


#define uint8_t unsigned char

// Array of data to be displayed, in 24-bit RRGGBBRRGGBB format
uint8_t* pShowData;
const int Width = 800;
const int Height = 600;
const int BytesPerPixel = 3;

HANDLE		 hBmpMapFile = NULL;
HANDLE		 hbmpBitmap = NULL;
BITMAPINFO	 bmpInfo;
HWND		 hWnd = NULL;


// Flag that we have DIB created. 

// For resizing, DIB must be recreated if source image size has changed.

// We need to create DIB after DC is available 
// that's why I placed it into WM_PAINT. A better place is
// drawing window's OnCreate() or something like that.
bool bDIBCreated = false; 

/* Written as stupid as possible, to be clear
 Creates the following picture:

  +------------------------------+
  | B   |     RED         |      |   
  | L   |                 |      |   
  | A   |-----------------|  G   |   
  | C   |     GREEN       |  R   |   
  | K   |                 |  A   |   
  |     |-----------------|  Y   |   
  |     |     BLUE        |      |   
  |     |                 |      |   
  +------------------------------+

 */
void CreateDemoImage(bool bVariant)
{
	uint8_t gray = 0x80;
	if(bVariant == true)
		gray = 0xb0;
	
	uint8_t black = 0x0;
	if(bVariant == true)
		black = 0x40;

	for(int y = 0; y < Height; y++)
	{
		for(int x = 0; x < Width; x++)
		{
			uint8_t r = 0, g = 0, b = 0;
			if(x < 100)
				r = g = b = black;				// Black stripe
			else if(x > (Width-100) )
				r = g = b = gray;			// White stripe
			else if(y < (Height / 3) )
				r = 0xff;					// Red stripe, g=b=0
			else if(y < 2 * (Height / 3) )
				g = 0xff;					// Green stripe, r=b=0		
			else
				b = 0xff;					// Blue stripe, r=g=0

			if(bVariant == false)
			{
				//              line                          column          pixel
				//             vvvvvv                        vvvvvvvv         v 
				pShowData[(y * Width * BytesPerPixel) + (x * BytesPerPixel) + 0] = r;
				pShowData[(y * Width * BytesPerPixel) + (x * BytesPerPixel) + 1] = g;
				pShowData[(y * Width * BytesPerPixel) + (x * BytesPerPixel) + 2] = b;
			}
			else
			{
				// Change some colors for dynamics
				pShowData[(y * Width * BytesPerPixel) + (x * BytesPerPixel) + 0] = g;
				pShowData[(y * Width * BytesPerPixel) + (x * BytesPerPixel) + 1] = b;
				pShowData[(y * Width * BytesPerPixel) + (x * BytesPerPixel) + 2] = r;
			}
		}
	}
}

void CreateImageDIB(int w, int h)
{
	if (hbmpBitmap)
		DeleteObject(hbmpBitmap);

	if (hBmpMapFile)
		CloseHandle(hBmpMapFile);

	int nMemSize = w * h * 3;
	hBmpMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nMemSize, NULL);

	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = w;
	bmpInfo.bmiHeader.biHeight = h;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = nMemSize;
	bmpInfo.bmiHeader.biXPelsPerMeter = 96;
	bmpInfo.bmiHeader.biYPelsPerMeter = 96;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;
	bmpInfo.bmiColors[0].rgbBlue = 255;
	bmpInfo.bmiColors[0].rgbGreen = 255;
	bmpInfo.bmiColors[0].rgbRed = 255;
	bmpInfo.bmiColors[0].rgbReserved = 0;
	PVOID pvBits = NULL;

	HDC hDC = GetDC(hWnd);
	hbmpBitmap = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, &pvBits, hBmpMapFile, 0);
	ReleaseDC(hWnd, hDC);

	DWORD dwErr = GetLastError();
}


// END OF ACTUAL EXAMPLE, MORE BELOW
/*******************************************************************/



int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RAM2SCREEN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

/*******************************************************************/
// THIS PART IS AN ACTUAL EXAMPLE
	pShowData = new uint8_t[Width * Height * BytesPerPixel];
	CreateDemoImage(false);
// END OF ACTUAL EXAMPLE, MORE BELOW
/*******************************************************************/

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_RAM2SCREEN);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete[] pShowData;

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_RAM2SCREEN);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_RAM2SCREEN;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
/*******************************************************************/
// THIS PART IS AN ACTUAL EXAMPLE
				case IDM_REDRAW:
					{
				    CreateDemoImage(true);
				    RECT rect = {0,0,Width,Height};
				    InvalidateRect(hWnd, &rect, TRUE);
					}
				    break;
// END OF ACTUAL EXAMPLE
/*******************************************************************/
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

/*******************************************************************/
// THIS PART IS AN ACTUAL EXAMPLE
			{
			BITMAP bm;

			if(bDIBCreated == false)
			{
				CreateImageDIB(Width, Height);
				bDIBCreated = true;
			}

			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmpBitmap);

			int nRes = SetDIBits(hdc, (HBITMAP)hbmpBitmap, 0, Height, pShowData, &bmpInfo, DIB_RGB_COLORS);

			GetObject(hbmpBitmap, sizeof(bm), &bm);

			BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);
			}
// END OF ACTUAL EXAMPLE
/*******************************************************************/


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

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
