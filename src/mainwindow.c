#include "c-hawk.h"
#include "editor.h"
#include "resource.h"

void FillListWithLinesFromFile(const char* filename, HWND hWnd)
{
    FILE* file = fopen(filename, "r");
    if (file != NULL)
    {
        char line[MAX_PATH] = { 0 };
        while (fgets(line, MAX_PATH, file))
        {
            SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)line);
        }
        fclose(file);
    }
}

void SetupProject()
{
    char projectFiles[MAX_PATH] = { 0 };
    strcpy(projectFiles, projectRoot);
    strcat(projectFiles, "\\project-files.txt");
    FillListWithLinesFromFile(projectFiles, panelLists[0]);
    
    char projectIncludes[MAX_PATH] = { 0 };
    strcpy(projectIncludes, projectRoot);
    strcat(projectIncludes, "\\project-includes.txt");
    FillListWithLinesFromFile(projectIncludes, panelLists[1]);
    
    char projectLibs[MAX_PATH] = { 0 };
    strcpy(projectLibs, projectRoot);
    strcat(projectLibs, "\\project-libs.txt");
    FillListWithLinesFromFile(projectLibs, panelLists[2]);
}

void resizeProjectPanels(RECT rc)
{
    int openPanelCount = 0;
    for (int i = 0; i < 4; i++)
        if (panelStates[i] == 1) openPanelCount++;
    int panelHeight = openPanelCount > 0 ? (rc.bottom - rc.top - 80) / openPanelCount : 0;

    int top = rc.top;
    for (int i = 0; i < 4; i++)
    {
        SetWindowPos(panelLabels[i], 0, rc.left + 20, top, 180, 20, 0);
        SetWindowPos(panelButtons[i], 0, rc.left, top, 20, 20, 0);
        top += 20;
        if (panelStates[i] == 1)
        {
            SetWindowPos(panelLists[i], 0, rc.left, top, rc.left + 200, panelHeight, 0);
            top += panelHeight;
        }
        else
        {
            SetWindowPos(panelLists[i], 0, rc.left, top, 0, 0, 0);
        }
    }
}

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
        
        HFONT font = CreateFont(16, 0, 0, 0, 
                                FW_NORMAL, FALSE, FALSE, FALSE, 
                                ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
                                DEFAULT_PITCH, 
                                NULL); 
 
        for (int i = 0; i < 4; i++)
        {
            panelLabels[i] = CreateWindowExW(0
                , L"STATIC", panelNames[i]
                , WS_CHILD | WS_VISIBLE | SS_CENTER
                , 0, 0, 50, 21
                , hWnd, NULL, hInstance, NULL);
            SendMessage(panelLabels[i], WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);

            panelLists[i] = CreateWindowExW(WS_EX_STATICEDGE
                , L"LISTBOX", L""
                , WS_CHILD | WS_VISIBLE | WS_VSCROLL
                , 0, 0, 200, 200
                , hWnd, NULL, hInstance, NULL);
            SendMessage(panelLists[i], WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);

            panelButtons[i] = CreateWindowExW(WS_EX_STATICEDGE
                , L"BUTTON", panelStates[i] == 0 ? L">": L"v"
                , WS_CHILD | WS_VISIBLE | BS_FLAT
                , 0, 0, 20, 20
                , hWnd, (HMENU)(IDC_PANEL_BUTTONS+i), hInstance, NULL);
            SendMessage(panelButtons[i], WM_SETFONT, (WPARAM)font, (LPARAM)TRUE);
        }
        
        SetupProject();

        return 0;
    }

    case WM_SIZE:
    {
        if (wParam != 1)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            resizeEditor(rc.left+200, rc.top, rc.right - rc.left - 200, rc.bottom - rc.top);
            resizeProjectPanels(rc);
        }
        return 0;
    }

    case WM_COMMAND:
    {
        switch (LOWORD(wParam)) {
        case IDM_FILE_NEW:
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
        default:
            {
                if (LOWORD(wParam) >= IDC_PANEL_BUTTONS 
                    && LOWORD(wParam) < IDC_PANEL_BUTTONS + 4)
                {
                    int index = LOWORD(wParam) - IDC_PANEL_BUTTONS;
                    panelStates[index] = panelStates[index] == 0 ? 1 : 0;
                    SendMessage(panelButtons[index], WM_SETTEXT, (WPARAM)0, (LPARAM)(panelStates[index] == 0 ? L">": L"v"));
                    RECT rc;
                    GetClientRect(hWnd, &rc);
                    resizeProjectPanels(rc);
                }
                break;
            }
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
    wndclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
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
    if (argc > 1) strcpy(projectRoot, (const char*)argv[1]);
    
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