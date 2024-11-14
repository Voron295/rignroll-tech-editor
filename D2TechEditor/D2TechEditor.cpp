// D2TechEditor.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "D2TechEditor.h"
#include "UI.h"
#include "Tech.h"
#pragma comment(lib,"shell32")
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

HWND hWnd;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_D2TECHEDITOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2TECHEDITOR));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2TECHEDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_D2TECHEDITOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

HFONT hMainFont;

HWND hLabelFileName;
HWND hCbBlocks;
HWND hLbFields;
HWND hBoxEdit[14];
HWND hLabelType;

bool isFileOpened = false;

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, "D2 Tech Editor", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 500, 480, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hMainFont = UI_CreateFont(TEXT("Tahoma"), 13);

   /*UI_SetElementFont(UI_CreateLabel(hWnd, L"Открыт файл:", 10, 5, 100, 30, 0, 0), hMainFont);
   hLabelFileName = UI_CreateLabel(hWnd, L"Файл1", 100, 5, 100, 30, 0, 0);
   UI_SetElementFont(hLabelFileName, hMainFont);*/

   hCbBlocks = UI_CreateComboBox(hWnd, 10, 5, 200, 30, WS_VSCROLL, 3000);
   UI_SetElementFont(hCbBlocks, hMainFont);
   hLbFields = UI_CreateListBox(hWnd, 10, 35, 200, 400, WS_VSCROLL | LBS_NOTIFY | WS_BORDER, 3001);
   UI_SetElementFont(hLbFields, hMainFont);

   UI_SetElementFont(UI_CreateLabel(hWnd, "Тип поля:", 250, 8, 60, 20, 0, 0), hMainFont);
   hLabelType = UI_CreateLabel(hWnd, "", 310, 8, 120, 20, 0, 0);
   UI_SetElementFont(hLabelType, hMainFont);

   for (int i = 0; i < 14; i++)
   {
	   hBoxEdit[i] = UI_CreateTextBox(hWnd, "", 250, 35 + 25 * i, 200, 20, 0, 3002 + i);
	   UI_SetElementFont(hBoxEdit[i], hMainFont);
	   ShowWindow(hBoxEdit[i], 0);
   }

   TechFile::Init();
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		case IDM_OPEN:
			TCHAR filename[512];
			if (UI_FileOpenDlg(hWnd, filename, "Tech files (*.tech)\0*.tech\0\0"))
			{
				isFileOpened = TechFile::LoadTech(filename, hCbBlocks, hLbFields, hBoxEdit, hLabelType);
			}
			break;

		case IDM_SAVE:
			if (isFileOpened)
			{
				TechFile::SaveFields();
				TechFile::Save();
			}
			break;

		case IDM_SAVEAS:
			if (isFileOpened)
			{
				TechFile::SaveFields();
				if (UI_FileSaveDlg(hWnd, filename, "Tech files (*.tech)\0*.tech\0\0"))
				{
					TechFile::Save(filename);
				}
			}
			break;

		case 3000:
			if (wmEvent == CBN_SELENDOK)
			{
				int id = UI_ComboBoxGetCurrentLine(hCbBlocks);
				TechFile::SaveFields();
				TechFile::LoadItem(id);
				TechFile::LoadField("");
			}
			break;

		case 3001:
			if (wmEvent == LBN_SELCHANGE)
			{
				char buf[256];
				UI_ListBoxGetCurrentLineText(hLbFields, buf);
				TechFile::SaveFields();
				TechFile::LoadField(buf);
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
