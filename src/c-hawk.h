#ifndef _C_HAWK_H_
#define _C_HAWK_H_

#include <windows.h>
#include <stdio.h>

static HINSTANCE hInstance;
static HWND wMain;

static HWND panelLabels[4] = { NULL };
static HWND panelButtons[4] = { NULL };
static HWND panelLists[4] = { NULL };
static int panelStates[4] = { 1, 0, 0, 0};
static LPCWSTR panelNames[4] = { L"FILES", L"INCLUDES", L"LIBS", L"LIB DIRECTORIES" };

static HACCEL hAccTable;
static HWND currentDialog;

static const char appName[] = "C-Hawk";
static const char className[] = "C-HawkMainWindow";

static int cmdShow = 0;
static TCHAR projectRoot[MAX_PATH] = { '\0' };

LPCSTR DisplayMyMessage(HINSTANCE hinst, HWND hwndOwner, LPSTR lpszMessage);
void SetupProject();
void resizeProjectPanels(RECT rc);

#endif // _C_HAWK_H_