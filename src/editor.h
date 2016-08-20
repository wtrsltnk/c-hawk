#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <windows.h>

void createEditor(HINSTANCE hInstance, HWND wParent);
void destroyEditor();
void resizeEditor(int x, int y, int w, int h);
void editorUndo();
void editorRedo();
void editorCut();
void editorCopy();
void editorPaste();
void editorDelete();
void editorSelectAll();

#endif // _EDITOR_H_
