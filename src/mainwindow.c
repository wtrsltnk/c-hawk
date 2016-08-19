
#include <windows.h>

#include "editor.h"

static HINSTANCE hInstance;
static HWND wMain;
static HWND currentDialog;

static const char className[] = "Notepad3Window";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_CREATE:
    {
        createEditor(hInstance, hWnd);

        return 0;
    }

    case WM_SIZE:
        if (wParam != 1)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            resizeEditor(rc.left + 200, rc.top, rc.right - rc.left - 200, rc.bottom - rc.top);
        }
        return 0;

    case WM_COMMAND:
//        app.Command(LOWORD(wParam));
//        app.CheckMenus();
        return 0;

    case WM_NOTIFY:
//        app.Notify(reinterpret_cast<SCNotification *>(lParam));
        return 0;

    case WM_MENUSELECT:
//        app.CheckMenus();
        return 0;

    case WM_CLOSE:
    {
        destroyEditor();
        PostQuitMessage(0);
        return 0;
    }

    default:
        return DefWindowProc(hWnd, iMessage, wParam, lParam);
    }
}

static void RegisterWindowClass()
{
    const char resourceName[] = "notepad3";

    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = 0;
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = NULL;
    wndclass.lpszMenuName = resourceName;
    wndclass.lpszClassName = className;

    if (!RegisterClass(&wndclass))
        exit(FALSE);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    hInstance = hInstance;

    HACCEL hAccTable = LoadAccelerators(hInstance, "notepad3");

    LoadLibrary("SciLexer.dll");

    RegisterWindowClass();

    wMain = CreateWindowEx(
                 WS_EX_CLIENTEDGE,
                 className,
                 "Demonstration",
                 WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
                 WS_MINIMIZEBOX | WS_MAXIMIZEBOX |
                 WS_MAXIMIZE | WS_CLIPCHILDREN,
                 CW_USEDEFAULT, CW_USEDEFAULT,
                 CW_USEDEFAULT, CW_USEDEFAULT,
                 NULL,
                 NULL,
                 hInstance,
                 0);

    ShowWindow(wMain, nCmdShow);

    MSG msg;
    msg.wParam = 0;

    int running = 1;
    while (running > 0)
    {
        if (GetMessage(&msg, NULL, 0, 0) == FALSE)
            running = 0;
        if (currentDialog && running > 0)
        {
            if (!IsDialogMessage(currentDialog, &msg))
            {
                if (TranslateAccelerator(msg.hwnd, hAccTable, &msg) == 0)
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
        else if (running > 0)
        {
            if (TranslateAccelerator(wMain, hAccTable, &msg) == 0)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return msg.wParam;
}
