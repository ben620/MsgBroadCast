#include <windows.h>
#include <windowsx.h>
#include "resource.h"


#pragma data_seg("shared")
volatile UINT count = 0;
#pragma data_seg()
#pragma comment(linker, "/SECTION:shared,rws")

UINT countUpdate = WM_APP + 123;

BOOL OnInit(HWND hWnd, HWND hWndFocus, LPARAM lParam)
{
	SetDlgItemInt(hWnd, IDC_COUNT, count, FALSE);
	PostMessage(HWND_BROADCAST, countUpdate, 0, count);
	return TRUE;
}

INT_PTR WINAPI DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == countUpdate)
	{
		SetDlgItemInt(hWnd, IDC_COUNT, count, FALSE);
	}
	else
	{
		switch (uMsg)
		{
		case WM_INITDIALOG:
			OnInit(hWnd, (HWND)wParam, lParam);
			break;
		case WM_COMMAND:
			if (wParam == IDCANCEL)
			{
				EndDialog(hWnd, lParam);
			}
			break;
		}
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE hInst, LPSTR, int)
{
	countUpdate = RegisterWindowMessage(TEXT("CountUpdate"));
	InterlockedIncrement(&count);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	InterlockedDecrement(&count);
	PostMessage(HWND_BROADCAST, countUpdate, 0, count);
	return 0;
}