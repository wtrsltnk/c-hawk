#include "editor.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include <stdio.h>

const int blockSize = 128 * 1024;

HWND wEditor;
BOOL isDirty;
char fullPath[MAX_PATH];

void InitialiseEditor();

void createEditor(HINSTANCE hInstance, HWND wParent)
{
    wEditor = CreateWindow(
                  "Scintilla",
                  "Source",
                  WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN,
                  0, 0,
                  100, 100,
                  wParent,
                  0,
                  hInstance,
                  0);
    InitialiseEditor();
    ShowWindow(wEditor, SW_SHOW);
    SetFocus(wEditor);
}

LRESULT SendEditor(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return SendMessage(wEditor, Msg, wParam, lParam);
}

void SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face)
{
    SendEditor(SCI_STYLESETFORE, style, fore);
    SendEditor(SCI_STYLESETBACK, style, back);
    if (size >= 1)
        SendEditor(SCI_STYLESETSIZE, style, size);
    if (face)
        SendEditor(SCI_STYLESETFONT, style, (LPARAM)face);
}

const COLORREF black = RGB(0,0,0);
const COLORREF white = RGB(0xff,0xff,0xff);

static const int LINE_MARGIN_INDEX = 0;

const char htmlKeyWords[] =
    "a abbr acronym address applet area b base basefont "
    "bdo big blockquote body br button caption center "
    "cite code col colgroup dd del dfn dir div dl dt em "
    "fieldset font form frame frameset h1 h2 h3 h4 h5 h6 "
    "head hr html i iframe img input ins isindex kbd label "
    "legend li link map menu meta noframes noscript "
    "object ol optgroup option p param pre q s samp "
    "script select small span strike strong style sub sup "
    "table tbody td textarea tfoot th thead title tr tt u ul "
    "var xmlns "
    "abbr accept-charset accept accesskey action align alink "
    "alt archive axis background bgcolor border "
    "cellpadding cellspacing char charoff charset checked cite "
    "class classid clear codebase codetype color cols colspan "
    "compact content coords "
    "data datafld dataformatas datapagesize datasrc datetime "
    "declare defer dir disabled enctype "
    "face for frame frameborder "
    "headers height href hreflang hspace http-equiv "
    "id ismap label lang language link longdesc "
    "marginwidth marginheight maxlength media method multiple "
    "name nohref noresize noshade nowrap "
    "object onblur onchange onclick ondblclick onfocus "
    "onkeydown onkeypress onkeyup onload onmousedown "
    "onmousemove onmouseover onmouseout onmouseup "
    "onreset onselect onsubmit onunload "
    "profile prompt readonly rel rev rows rowspan rules "
    "scheme scope shape size span src standby start style "
    "summary tabindex target text title type usemap "
    "valign value valuetype version vlink vspace width "
    "text password checkbox radio submit reset "
    "file hidden image "
    "public !doctype xml";

const char jsKeyWords[] =
    "break case catch continue default "
    "do else for function if return throw try var while";

const char vbsKeyWords[] =
    "and as byref byval case call const "
    "continue dim do each else elseif end error exit false for function global "
    "goto if in loop me new next not nothing on optional or private public "
    "redim rem resume select set sub then to true type while with "
    "boolean byte currency date double integer long object single string type "
    "variant";

const char cppKeyWords[] =
    "auto break case char const continue default do double else enum extern float for goto "
    "if inline int long register restrict return short signed sizeof static struct switch "
    "typedef union unsigned void volatile while";

void InitialiseEditor()
{
    SendEditor(SCI_SETLEXER, SCLEX_CPP, 0);
    SendEditor(SCI_SETSTYLEBITS, 7, 0);

    SendEditor(SCI_SETKEYWORDS, 0, (LPARAM)cppKeyWords);

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    SetAStyle(STYLE_DEFAULT, black, white, 11, "Consolas");
    SendEditor(SCI_STYLECLEARALL, 0, 0);	// Copies global style to all others

    SendEditor(SCI_SETMARGINWIDTHN, LINE_MARGIN_INDEX, 0);
    SendEditor(SCI_SETMARGINWIDTHN, 1, 0);
    SendEditor(SCI_SETMARGINWIDTHN, 2, 0);

    SendEditor(SCI_SETMARGINTYPEN,  LINE_MARGIN_INDEX, SC_MARGIN_NUMBER);
    SendEditor(SCI_SETMARGINWIDTHN, 1, SC_MARGIN_SYMBOL);
    SendEditor(SCI_SETMARGINWIDTHN, 2, SC_MARGIN_SYMBOL);

    int margin_size = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM) "_99999");
    SendEditor(SCI_SETMARGINWIDTHN, LINE_MARGIN_INDEX, margin_size);

    const COLORREF red = RGB(0xFF, 0, 0);
    const COLORREF offWhite = RGB(0xFF, 0xFB, 0xF0);
    const COLORREF darkGreen = RGB(0, 0x80, 0);
    const COLORREF darkBlue = RGB(0, 0, 0x80);

    // Hypertext default is used for all the document's text
    SetAStyle(SCE_H_DEFAULT, black, white, 11, "Times New Roman");

    // Unknown tags and attributes are highlighed in red.
    // If a tag is actually OK, it should be added in lower case to the htmlKeyWords string.
    SetAStyle(SCE_H_TAG, darkBlue, white, 0, 0);
    SetAStyle(SCE_H_TAGUNKNOWN, red, white, 0, 0);
    SetAStyle(SCE_H_ATTRIBUTE, darkBlue, white, 0, 0);
    SetAStyle(SCE_H_ATTRIBUTEUNKNOWN, red, white, 0, 0);
    SetAStyle(SCE_H_NUMBER, RGB(0x80,0,0x80), white, 0, 0);
    SetAStyle(SCE_H_DOUBLESTRING, RGB(0,0x80,0), white, 0, 0);
    SetAStyle(SCE_H_SINGLESTRING, RGB(0,0x80,0), white, 0, 0);
    SetAStyle(SCE_H_OTHER, RGB(0x80,0,0x80), white, 0, 0);
    SetAStyle(SCE_H_COMMENT, RGB(0x80,0x80,0), white, 0, 0);
    SetAStyle(SCE_H_ENTITY, RGB(0x80,0,0x80), white, 0, 0);

    SetAStyle(SCE_H_TAGEND, darkBlue, white, 0, 0);
    SetAStyle(SCE_H_XMLSTART, darkBlue, white, 0, 0);	// <?
    SetAStyle(SCE_H_XMLEND, darkBlue, white, 0, 0);		// ?>
    SetAStyle(SCE_H_SCRIPT, darkBlue, white, 0, 0);		// <script
    SetAStyle(SCE_H_ASP, RGB(0x4F, 0x4F, 0), RGB(0xFF, 0xFF, 0), 0, 0);	// <% ... %>
    SetAStyle(SCE_H_ASPAT, RGB(0x4F, 0x4F, 0), RGB(0xFF, 0xFF, 0), 0, 0);	// <%@ ... %>

    SetAStyle(SCE_HB_DEFAULT, black, white, 0, 0);
    SetAStyle(SCE_HB_COMMENTLINE, darkGreen, white, 0, 0);
    SetAStyle(SCE_HB_NUMBER, RGB(0,0x80,0x80), 0, 0, 0);
    SetAStyle(SCE_HB_WORD, darkBlue, white, 0, 0);
    SendEditor(SCI_STYLESETBOLD, SCE_HB_WORD, 1);
    SetAStyle(SCE_HB_STRING, RGB(0x80,0,0x80), 0, 0, 0);
    SetAStyle(SCE_HB_IDENTIFIER, black, white, 0, 0);

    // This light blue is found in the windows system palette so is safe to use even in 256 colour modes.
    const COLORREF lightBlue = RGB(0xA6, 0xCA, 0xF0);
    // Show the whole section of VBScript with light blue background
    for (int bstyle=SCE_HB_DEFAULT; bstyle<=SCE_HB_STRINGEOL; bstyle++) {
        SendEditor(SCI_STYLESETFONT, bstyle, (LPARAM)"Consolas");
        SendEditor(SCI_STYLESETBACK, bstyle, lightBlue);
        // This call extends the backround colour of the last style on the line to the edge of the window
        SendEditor(SCI_STYLESETEOLFILLED, bstyle, 1);
    }
    SendEditor(SCI_STYLESETBACK, SCE_HB_STRINGEOL, RGB(0x7F,0x7F,0xFF));
    SendEditor(SCI_STYLESETFONT, SCE_HB_COMMENTLINE, (LPARAM)"Consolas");

    SetAStyle(SCE_HBA_DEFAULT, black, 0, 0, 0);
    SetAStyle(SCE_HBA_COMMENTLINE, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HBA_NUMBER, RGB(0,0x80,0x80), 0, 0, 0);
    SetAStyle(SCE_HBA_WORD, darkBlue, 0, 0, 0);
    SendEditor(SCI_STYLESETBOLD, SCE_HBA_WORD, 1);
    SetAStyle(SCE_HBA_STRING, RGB(0x80,0,0x80), 0, 0, 0);
    SetAStyle(SCE_HBA_IDENTIFIER, black, 0, 0, 0);

    // Show the whole section of ASP VBScript with bright yellow background
    for (int bastyle=SCE_HBA_DEFAULT; bastyle<=SCE_HBA_STRINGEOL; bastyle++) {
        SendEditor(SCI_STYLESETFONT, bastyle, (LPARAM)"Consolas");
        SendEditor(SCI_STYLESETBACK, bastyle, RGB(0xFF, 0xFF, 0));
        // This call extends the backround colour of the last style on the line to the edge of the window
        SendEditor(SCI_STYLESETEOLFILLED, bastyle, 1);
    }
    SendEditor(SCI_STYLESETBACK, SCE_HBA_STRINGEOL, RGB(0xCF,0xCF,0x7F));
    SendEditor(SCI_STYLESETFONT, SCE_HBA_COMMENTLINE, (LPARAM)"Comic Sans MS");

    // If there is no need to support embedded Javascript, the following code can be dropped.
    // Javascript will still be correctly processed but will be displayed in just the default style.

    SetAStyle(SCE_HJ_START, RGB(0x80,0x80,0), 0, 0, 0);
    SetAStyle(SCE_HJ_DEFAULT, black, 0, 0, 0);
    SetAStyle(SCE_HJ_COMMENT, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HJ_COMMENTLINE, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HJ_COMMENTDOC, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HJ_NUMBER, RGB(0,0x80,0x80), 0, 0, 0);
    SetAStyle(SCE_HJ_WORD, black, 0, 0, 0);
    SetAStyle(SCE_HJ_KEYWORD, darkBlue, 0, 0, 0);
    SetAStyle(SCE_HJ_DOUBLESTRING, RGB(0x80,0,0x80), 0, 0, 0);
    SetAStyle(SCE_HJ_SINGLESTRING, RGB(0x80,0,0x80), 0, 0, 0);
    SetAStyle(SCE_HJ_SYMBOLS, black, 0, 0, 0);

    SetAStyle(SCE_HJA_START, RGB(0x80,0x80,0), 0, 0, 0);
    SetAStyle(SCE_HJA_DEFAULT, black, 0, 0, 0);
    SetAStyle(SCE_HJA_COMMENT, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HJA_COMMENTLINE, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HJA_COMMENTDOC, darkGreen, 0, 0, 0);
    SetAStyle(SCE_HJA_NUMBER, RGB(0,0x80,0x80), 0, 0, 0);
    SetAStyle(SCE_HJA_WORD, black, 0, 0, 0);
    SetAStyle(SCE_HJA_KEYWORD, darkBlue, 0, 0, 0);
    SetAStyle(SCE_HJA_DOUBLESTRING, RGB(0x80,0,0x80), 0, 0, 0);
    SetAStyle(SCE_HJA_SINGLESTRING, RGB(0x80,0,0x80), 0, 0, 0);
    SetAStyle(SCE_HJA_SYMBOLS, black, 0, 0, 0);

    // Show the whole section of Javascript with off white background
    for (int jstyle=SCE_HJ_DEFAULT; jstyle<=SCE_HJ_SYMBOLS; jstyle++) {
        SendEditor(SCI_STYLESETFONT, jstyle, (LPARAM)"Consolas");
        SendEditor(SCI_STYLESETBACK, jstyle, offWhite);
        SendEditor(SCI_STYLESETEOLFILLED, jstyle, 1);
    }
    SendEditor(SCI_STYLESETBACK, SCE_HJ_STRINGEOL, RGB(0xDF, 0xDF, 0x7F));
    SendEditor(SCI_STYLESETEOLFILLED, SCE_HJ_STRINGEOL, 1);

    // Show the whole section of Javascript with brown background
    for (int jastyle=SCE_HJA_DEFAULT; jastyle<=SCE_HJA_SYMBOLS; jastyle++) {
        SendEditor(SCI_STYLESETFONT, jastyle, (LPARAM)"Consolas");
        SendEditor(SCI_STYLESETBACK, jastyle, RGB(0xDF, 0xDF, 0x7F));
        SendEditor(SCI_STYLESETEOLFILLED, jastyle, 1);
    }
    SendEditor(SCI_STYLESETBACK, SCE_HJA_STRINGEOL, RGB(0x0,0xAF,0x5F));
    SendEditor(SCI_STYLESETEOLFILLED, SCE_HJA_STRINGEOL, 1);
}

void destroyEditor()
{
    DestroyWindow(wEditor);
}

void resizeEditor(int x, int y, int w, int h)
{
    SetWindowPos(wEditor, 0, x, y, w, h, 0);
}

void editorUndo()
{
    SendEditor(WM_UNDO, 0, 0);
}

void editorRedo()
{
    SendEditor(SCI_REDO, 0, 0);
}

void editorCut()
{
    SendEditor(WM_CUT, 0, 0);
}

void editorCopy()
{
    SendEditor(WM_COPY, 0, 0);
}

void editorPaste()
{
    SendEditor(WM_PASTE, 0, 0);
}

void editorDelete()
{
    SendEditor(WM_CLEAR, 0, 0);
}

void editorSelectAll()
{
    SendEditor(SCI_SELECTALL, 0, 0);
}
