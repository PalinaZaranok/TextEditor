#pragma once

#include "../definitions.h"

WNDCLASS NewWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
void MainWndAddMenus(HWND hWnd);
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MainWndAddEditControl(HWND hWnd);
LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,UINT_PTR uIdSubclass,
                                  DWORD_PTR dwRefData);
