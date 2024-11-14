#include "stdafx.h"
#include "UI.h"

BOOL UI_FileOpenDlg(HWND hWnd, LPTSTR pstrFileName, LPTSTR Filter, DWORD Flags, LPTSTR InitDir, LPTSTR pstrTitleName)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 512;
	ofn.lpstrFilter = Filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = InitDir;
	ofn.Flags = Flags;
	return GetOpenFileName(&ofn);
}

BOOL UI_FileSaveDlg(HWND hWnd, LPTSTR pstrFileName, LPTSTR Filter, DWORD Flags, LPTSTR InitDir, LPTSTR pstrTitleName)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 512;
	ofn.lpstrFilter = Filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = InitDir;
	ofn.Flags = Flags;
	return GetSaveFileName(&ofn);
}

HWND UI_CreateComboBox(HWND hWnd, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID)
{
	HWND hBox;
	hBox = CreateWindow("COMBOBOX", 0, CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_GROUP | style, PosX, PosY, Width, Height, hWnd, (HMENU)ElementID, NULL, NULL);
	return hBox;
}

HWND UI_CreateListBox(HWND hWnd, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID)
{
	HWND hBox;
	hBox = CreateWindowW(L"LISTBOX", 0, WS_VISIBLE | WS_CHILD | WS_GROUP | LBS_HASSTRINGS | style, PosX, PosY, Width, Height, hWnd, (HMENU)ElementID, NULL, NULL);
	return hBox;
}

HWND UI_CreateTextBox(HWND hWnd, const TCHAR *Text, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID)
{
	HWND hEdit;
	hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", Text, WS_VISIBLE | WS_CHILD | WS_GROUP | style, PosX, PosY, Width, Height, hWnd, (HMENU)ElementID, NULL, NULL);
	return hEdit;
}

DWORD UI_ComboBoxAddLine(HWND hWnd, const TCHAR *Text)
{
	return SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)Text);
}

void UI_ListBoxAddLine(HWND hWnd, const TCHAR *Text)
{
	SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)Text);
}

void UI_ListBoxInsertLine(HWND hWnd, const TCHAR *Text, int line)
{
	SendMessage(hWnd, LB_INSERTSTRING, (WPARAM)line, (LPARAM)Text);
}

DWORD UI_ComboBoxGetCurrentLine(HWND hWnd)
{
	return SendMessage(hWnd, CB_GETCURSEL, 0, 0);
}

DWORD UI_ListBoxGetCurrentLine(HWND hWnd)
{
	return SendMessage(hWnd, LB_GETCURSEL, 0, 0);
}


DWORD UI_ListBoxGetCurrentLineText(HWND hWnd, TCHAR *buf)
{
	if (!buf)
		return 0;

	return SendMessage(hWnd, LB_GETTEXT, SendMessage(hWnd, LB_GETCURSEL, 0, 0), (LPARAM)buf);
}

void UI_ResetListBox(HWND hWnd)
{
	SendMessage(hWnd, LB_SETCURSEL, -1, 0);
	int size = SendMessage(hWnd, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < size; i++)
	{
		SendMessage(hWnd, LB_DELETESTRING, 0, 0);
	}
}

void UI_ResetComboBox(HWND hWnd)
{
	SendMessage(hWnd, CB_SETCURSEL, -1, 0);
	int size = SendMessage(hWnd, CB_GETCOUNT, 0, 0);
	for (int i = 0; i < size; i++)
	{
		SendMessage(hWnd, CB_DELETESTRING, 0, 0);
	}
}

HWND UI_CreateLabel(HWND hWnd, const TCHAR *Text, int PosX, int PosY, int Width, int Height, DWORD style, int ElementID)
{
	HWND hLabel;
	hLabel = CreateWindow("STATIC", Text, WS_VISIBLE | WS_CHILD | SS_LEFT | style, PosX, PosY, Width, Height, hWnd, (HMENU)ElementID, NULL, NULL);
	return hLabel;
}

void UI_SetLabelText(HWND hLabel, const TCHAR *Text)
{
	//SendMessage(hLabel, , (WPARAM)Text, 0);
	SetWindowText(hLabel, Text);
}

HFONT UI_CreateFont(TCHAR *Family, int size)
{
	return CreateFont(size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, Family);
}

void UI_SetElementFont(HWND hWnd, HFONT hFont)
{
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
}
