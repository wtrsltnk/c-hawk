#include "c-hawk.h"
#include "resource.h"

PTSTR pszCurDir; 
TCHAR achBuffer[MAX_PATH]; 

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_INITDIALOG: 
        {
            // Initialize the list box by filling it with files from 
            // the current directory. 
            pszCurDir = achBuffer; 
            GetCurrentDirectory(MAX_PATH, pszCurDir); 
            DlgDirList(hWnd, pszCurDir, IDC_FILELIST, ID_TEXT, DDL_DIRECTORY); 
            SetFocus(GetDlgItem(hWnd, IDC_FILELIST)); 
            break;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam)) 
            { 
                case IDOK: 
                    EndDialog(hWnd, wParam); 
                    return TRUE; 
 
                case IDCANCEL: 
                    EndDialog(hWnd, wParam); 
                    return TRUE;
                case IDC_FILELIST:
                    {
                        switch (HIWORD(wParam)) 
                        { 
                        case LBN_SELCHANGE:
                            {
                                HWND hwndList = GetDlgItem(hWnd, IDC_FILELIST); 

                                // Get selected index.
                                int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0); 

                                // Get item data.
                                char lpszBuffer[MAX_PATH];
                                int i = (int)SendMessage(hwndList, LB_GETTEXT, lbItem, lpszBuffer);

                                SetDlgItemText(hWnd, ID_TEXT, lpszBuffer); 
                                return TRUE; 
                            }
                        }
                        return TRUE;
                    }
            }
            break;
        }
    }
    return FALSE;
}

LPWORD lpwAlign(LPWORD lpIn)
{
    ULONG ul;

    ul = (ULONG)lpIn;
    ul ++;
    ul >>=1;
    ul <<=1;
    return (LPWORD)ul;
}

LPCSTR DisplayMyMessage(HINSTANCE hinst, HWND hwndOwner, LPSTR lpszMessage)
{
    HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    LRESULT ret;
    int nchar;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, 2048);
    if (!hgbl)
        return -1;
 
    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
 
    // Define a dialog box.
 
    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION;
    lpdt->cdit = 4;         // Number of controls
    lpdt->x  = 10;  lpdt->y  = 10;
    lpdt->cx = 280; lpdt->cy = 200;

    lpw = (LPWORD)(lpdt + 1);
    *lpw++ = 0;             // No menu
    *lpw++ = 0;             // Predefined dialog box class (by default)

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "My Dialog", -1, lpwsz, 50);
    lpw += nchar;

    //-----------------------
    // Define an OK button.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 10; lpdit->y  = 170;
    lpdit->cx = 130; lpdit->cy = 20;
    lpdit->id = IDOK;       // OK button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;        // Button class

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "Create", -1, lpwsz, 50);
    lpw += nchar;
    *lpw++ = 0;             // No creation data

    //-----------------------
    // Define a Cancel button.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 140; lpdit->y  = 170;
    lpdit->cx = 130; lpdit->cy = 20;
    lpdit->id = IDCANCEL;    // Help button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;        // Button class atom

    lpwsz = (LPWSTR)lpw;
    nchar = 1 + MultiByteToWideChar(CP_ACP, 0, "Cancel", -1, lpwsz, 50);
    lpw += nchar;
    *lpw++ = 0;             // No creation data

    //-----------------------
    // Define a static text control.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 10; lpdit->y  = 10;
    lpdit->cx = 260; lpdit->cy = 20;
    lpdit->id = ID_TEXT;    // Text identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0082;        // Static class

    for (lpwsz = (LPWSTR)lpw; *lpwsz++ = (WCHAR)*lpszMessage++;);
    lpw = (LPWORD)lpwsz;
    *lpw++ = 0;             // No creation data

    //-----------------------
    // Define a Listbox.
    //-----------------------
    lpw = lpwAlign(lpw);    // Align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE)lpw;
    lpdit->x  = 10; lpdit->y  = 30;
    lpdit->cx = 260; lpdit->cy = 140;
    lpdit->id = IDC_FILELIST;    // Help button identifier
    lpdit->style = WS_CHILD | WS_VISIBLE | LBS_NOTIFY;

    lpw = (LPWORD)(lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0083;        // Button class atom

    lpwsz = (LPWSTR)lpw;
    nchar = 0;//1 + MultiByteToWideChar(CP_ACP, 0, "Cancel", -1, lpwsz, 50);
    lpw += nchar;
    *lpw++ = 0;             // No creation data

    GlobalUnlock(hgbl); 
    ret = DialogBoxIndirect(hinst, 
                           (LPDLGTEMPLATE)hgbl, 
                           hwndOwner, 
                           (DLGPROC)DialogProc); 
    GlobalFree(hgbl); 
    if (ret == TRUE)
    {
        return pszCurDir;
    }
    return NULL; 
}
