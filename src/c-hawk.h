#ifndef _C_HAWK_H_
#define _C_HAWK_H_

#include <windows.h>
#include <stdio.h>

static HINSTANCE hInstance;
static HWND wMain, wFilesLabel, wFilesList, wIncludesLabel, wIncludesList;
static HWND currentDialog;

static const char className[] = "C-HawkMainWindow";

LRESULT DisplayMyMessage(HINSTANCE hinst, HWND hwndOwner, LPSTR lpszMessage);

#endif // _C_HAWK_H_