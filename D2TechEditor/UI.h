#include "stdafx.h"
#include <ShellAPI.h>
#include <shlobj.h>
#include <atlbase.h>
#include <Commdlg.h>

BOOL UI_FileOpenDlg(HWND hWnd, LPTSTR pstrFileName, LPTSTR Filter, DWORD Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, LPTSTR InitDir = NULL, LPTSTR pstrTitleName = NULL);
BOOL UI_FileSaveDlg(HWND hWnd, LPTSTR pstrFileName, LPTSTR Filter, DWORD Flags = OFN_PATHMUSTEXIST, LPTSTR InitDir = NULL, LPTSTR pstrTitleName = NULL);
HWND UI_CreateComboBox(HWND hWnd, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID);
HWND UI_CreateListBox(HWND hWnd, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID);
HWND UI_CreateTextBox(HWND hWnd, const TCHAR *Text, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID);
DWORD UI_ComboBoxAddLine(HWND hWnd, const TCHAR *Text);
void UI_ListBoxAddLine(HWND hWnd, const TCHAR *Text);
void UI_ListBoxInsertLine(HWND hWnd, const TCHAR *Text, int line = -1);
DWORD UI_ComboBoxGetCurrentLine(HWND hWnd);
DWORD UI_ListBoxGetCurrentLine(HWND hWnd);
DWORD UI_ListBoxGetCurrentLineText(HWND hWnd, TCHAR *buf);
HWND UI_CreateLabel(HWND hWnd, const TCHAR *Text, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID);
void UI_SetLabelText(HWND hLabel, const TCHAR *Text);
HFONT UI_CreateFont(TCHAR *Family, int size);
void UI_SetElementFont(HWND hWnd, HFONT hFont);
void UI_ResetListBox(HWND hWnd);
void UI_ResetComboBox(HWND hWnd);