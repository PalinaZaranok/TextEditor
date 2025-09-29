#pragma once
#ifndef TEXTEDITOR_DEFINITIONS_H
#define TEXTEDITOR_DEFINITIONS_H

#endif //TEXTEDITOR_DEFINITIONS_H

#define IDOK 1
#define IDC_STATIC -1

#define OnMenuOpen    1
#define OnMenuSave    2
#define OnMenuExit    3
#define OnMenuCopy    4
#define OnMenuPaste   5
#define OnMenuCut     6
#define OnMenuProgram 7
#define iddAboutBox   8

#define WM_MOUSEMOVE       0x0200
#define WM_LBUTTONDOWN     0x0201
#define WM_RBUTTONDOWN     0x0204
#define WM_MBUTTONDOWN     0x0207
#define WM_MOUSEWHEEL      0x020A

extern UINT_PTR inactivityTimerId;
extern UINT_PTR animationTimerId;
extern const UINT inactivityTimeout;
extern bool isScreenBlackedOut;
extern DWORD lastActivityTime;

extern bool autoMove;
extern int manualDx;
extern int manualDy;
extern int manualSpeed;

WNDCLASS NewWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure);
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
void MainWndAddMenus(HWND hWnd);
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

void ResetInactivityTimer();
void BlackOutScreen(HWND hWnd);
void RestoreScreen(HWND hWnd);
void DrawAnimatedFigure(HWND hWnd);
void UpdateFigurePosition(HWND hWnd);
