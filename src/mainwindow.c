#include "c-hawk.h"
#include "editor.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch (iMessage)
    {
    case WM_CREATE:
    {
        HMENU hMenu, hSubMenu;

        hMenu = CreateMenu();

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, IDM_FILE_NEW, "&New");
        AppendMenu(hSubMenu, MF_STRING, IDM_FILE_OPEN, "&Open");
        AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
        AppendMenu(hSubMenu, MF_STRING, IDM_FILE_EXIT, "E&xit");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

        hSubMenu = CreatePopupMenu();
        AppendMenu(hSubMenu, MF_STRING, IDM_PROJECT_NEW, "&New project");
        AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
        AppendMenu(hSubMenu, MF_STRING, IDM_FILE_NEW, "&Add new file");
        AppendMenu(hSubMenu, MF_STRING, IDM_PROJECT_ADD_INCLUDE, "&Add include directory");
        AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
        AppendMenu(hSubMenu, MF_STRING, IDM_PROJECT_BUILD, "&Build");
        AppendMenu(hSubMenu, MF_STRING, IDM_PROJECT_REBUILD, "Rebuild");
        AppendMenu(hSubMenu, MF_STRING, IDM_PROJECT_CLEAN, "Clean");
        AppendMenu(hSubMenu, MF_STRING, IDM_PROJECT_RUN, "&Run");
        AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Project");

        SetMenu(hWnd, hMenu);
        
        createEditor(hInstance, hWnd);
        
        wFilesLabel = CreateWindowExW(0
            , L"STATIC", L""
            , WS_CHILD | WS_VISIBLE | SS_CENTER
            , 7, 7, 50, 21
            , hWnd, NULL, hInstance, NULL);
        SendMessage(wFilesLabel, WM_SETTEXT, 0, (LPARAM)"FILES");

        wFilesList = CreateWindowExW(WS_EX_STATICEDGE
            , L"LISTBOX", L""
            , WS_CHILD | WS_VISIBLE | WS_VSCROLL
            , 0, 35, 200, 200
            , hWnd, NULL, hInstance, NULL);
            
        wIncludesLabel = CreateWindowExW(0
            , L"STATIC", L""
            , WS_CHILD | WS_VISIBLE | SS_CENTER
            , 7, 7, 50, 21
            , hWnd, NULL, hInstance, NULL);
        SendMessage(wIncludesLabel, WM_SETTEXT, 0, (LPARAM)"INCLUDES");

        wIncludesList = CreateWindowExW(WS_EX_STATICEDGE
            , L"LISTBOX", L""
            , WS_CHILD | WS_VISIBLE | WS_VSCROLL
            , 0, 35, 200, 200
            , hWnd, NULL, hInstance, NULL);

        HFONT font = CreateFont(16, 0, 0, 0,
                                FW_NORMAL, FALSE, FALSE, FALSE,
                                ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                DEFAULT_PITCH,
                                NULL);

        SendMessage(wFilesLabel, WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);
        SendMessage(wFilesList, WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);
        SendMessage(wIncludesLabel, WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);
        SendMessage(wIncludesList, WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);

        return 0;
    }

    case WM_SIZE:
        if (wParam != 1)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            resizeEditor(rc.left+200, rc.top, rc.right - rc.left - 200, rc.bottom - rc.top);
            SetWindowPos(wFilesLabel, 0, rc.left, rc.top, rc.left + 200, 20, 0);
            SetWindowPos(wFilesList, 0, rc.left, rc.top + 20, rc.left + 200, rc.bottom - rc.top - (300 + 20), 0);
            SetWindowPos(wIncludesLabel, 0, rc.left, rc.bottom - 300, rc.left + 200, 20, 0);
            SetWindowPos(wIncludesList, 0, rc.left, rc.bottom - 280, rc.left + 200, (300 - 20), 0);
        }
        return 0;

    case WM_COMMAND:
    {
        switch (LOWORD(wParam)) {
        case IDM_FILE_NEW:
            SendMessage(wFilesList, LB_ADDSTRING, 0, (LPARAM)"test1");
            break;
        case IDM_FILE_OPEN:
            break;
        case IDM_FILE_SAVE:
            break;
        case IDM_FILE_SAVEAS:
            break;
        case IDM_FILE_EXIT:
            PostQuitMessage(0);
            break;
        case IDM_PROJECT_ADD_INCLUDE:
            SendMessage(wIncludesList, LB_ADDSTRING, 0, (LPARAM)"scintilla/include");
            break;
        case IDM_PROJECT_NEW:
            DisplayMyMessage(hInstance, wMain, "Todo: Select a project folder" );
            break;

        case IDM_EDIT_UNDO:
            editorUndo();
            break;
        case IDM_EDIT_REDO:
            editorRedo();
            break;
        case IDM_EDIT_CUT:
             editorCut();
            break;
        case IDM_EDIT_COPY:
            editorCopy();
            break;
        case IDM_EDIT_PASTE:
            editorPaste();
            break;
        case IDM_EDIT_DELETE:
            editorDelete();
            break;
        case IDM_EDIT_SELECTALL:
            editorSelectAll();
            break;
        };
        return 0;
    }

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

int RunApp()
{
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

    ShowWindow(wMain, cmdShow);
    
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

int main(int argc, char* argv)
{
    if (argc > 1) strcpy(projectRoot, argv[1]);
    
    hInstance = GetModuleHandle(NULL);
    cmdShow = SW_SHOW;

    hAccTable = LoadAccelerators(hInstance, "notepad3");

    LoadLibrary("SciLexer.dll");

    RegisterWindowClass();

    if (projectRoot[0] == '\0')
    {
        LPCSTR folder = DisplayMyMessage(hInstance, wMain, "Todo: Select a project folder" );
        if (folder != NULL)
        {
            strcpy(projectRoot, folder);
            return RunApp();
        }
        
        return 0;
    }
    
    return RunApp();
}