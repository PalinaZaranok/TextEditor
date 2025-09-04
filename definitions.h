#pragma once
#ifndef TEXTEDITOR_DEFINITIONS_H
#define TEXTEDITOR_DEFINITIONS_H

#endif //TEXTEDITOR_DEFINITIONS_H

#define OnMenuOpen    1
#define OnMenuSave    2
#define OnMenuExit    3

#define OnMenuCopy    4
#define OnMenuPaste   5
#define OnMenuCut     6
#define OnMenuProgram 7
#define iddAboutBox   8
#define idOk          9


WNDCLASS NewWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
void MainWndAddMenus(HWND hWnd);