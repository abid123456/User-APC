// User-APC_GUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "assert.h"
#include "User-APC_GUI.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Papcfunc(
    ULONG_PTR Parameter
)
{
    HWND hWnd = (HWND)Parameter;
    MessageBox(hWnd, L"APC callback fired OK", L"Success", MB_ICONINFORMATION);
}

void set_test_APC(HWND hWnd)
{
    if (!QueueUserAPC(Papcfunc, GetCurrentThread(), (ULONG_PTR)hWnd))
    {
        MessageBox(hWnd, L"ERROR: QueueUserAPC failed", L"ERROR", MB_ICONERROR);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_USERAPCGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_USERAPCGUI));

    MSG msg;
    int nExitCode = 0;

    while (TRUE)
    {
        DWORD dwR = MsgWaitForMultipleObjectsEx(0, NULL, INFINITE, QS_ALLINPUT, MWMO_ALERTABLE | MWMO_INPUTAVAILABLE);
        if (dwR == WAIT_FAILED)
        {
            //Error
            assert(false);
            nExitCode = -1;
            break;
        }

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                //Normal exit
                return (int)msg.wParam;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return nExitCode;

    //MSG msg;
    //
    //// Main message loop:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}
    //
    //return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_USERAPCGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_USERAPCGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hDlg;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                set_test_APC(hWnd);
                hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                if (hDlg)
                {
                    ShowWindow(hDlg, SW_SHOW);

                    //Disable parent window to make ours into a modal dialog
                    EnableWindow(hWnd, FALSE);

                    MSG msg;
                    BOOL bStopStop = FALSE;

                    for (; !bStopStop;)
                    {
                        DWORD dwR = MsgWaitForMultipleObjectsEx(0, NULL, INFINITE, QS_ALLINPUT,
                            MWMO_ALERTABLE | MWMO_INPUTAVAILABLE);
                        if (dwR == WAIT_FAILED)
                        {
                            //Error
                            assert(false);
                            break;
                        }

                        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                        {
                            //Hack to ensure processing of EndDialog() calls
                            if (msg.message == WM_NULL && msg.hwnd == hDlg)
                            {
                                //Normal exit
                                bStopStop = true;
                                break;
                            }

                            if (!IsDialogMessage(hDlg, &msg))
                            {
                                if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
                                {
                                    TranslateMessage(&msg);
                                }

                                DispatchMessage(&msg);
                            }
                        }
                    }

                    DestroyWindow(hDlg);
                    hDlg = NULL;

                    //Re-enable parent window
                    EnableWindow(hWnd, TRUE);
                }
                else
                    assert(false);
                break;
            case IDM_EXIT:
                set_test_APC(hWnd);
                SleepEx(1000, TRUE);
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
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
