#include <windows.h>
#include <commctrl.h>
#include "window.h"
#include "../definitions.h"
#include "../animation/animation.h"

HWND hEditControl = NULL;

WNDCLASS NewWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure) {
    WNDCLASS NWC = {0};
    NWC.hIcon = icon;
    NWC.hCursor = cursor;
    NWC.hInstance = hInst;
    NWC.lpszClassName = name;
    NWC.lpfnWndProc = procedure;
    NWC.hbrBackground = bgColor;

    return NWC;

}

// дескриптор окна, код сообщения, доппараметры для сообщения, доппараметр
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT message, WPARAM wparam,LPARAM lparam)
{
    switch(message){
        case WM_COMMAND:
            switch (wparam) {
                case OnMenuOpen:
                    MessageBoxA(hWnd, "Menu1 was clicked", "Menu1 worked", MB_OK);
                    break;
                case OnMenuSave:
                    MessageBoxA(hWnd, "Menu2 was clicked", "Menu2 worked", MB_OK);
                    break;
                case OnMenuExit:
                    PostQuitMessage(0);
                    break;
                case OnMenuProgram:
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(iddAboutBox), hWnd, AboutDlgProc);
                    break;
            }
            break;
        case WM_CREATE:
            MainWndAddMenus(hWnd);
            SetTimer(hWnd, inactivityTimerId,1000, NULL);
            MainWndAddEditControl(hWnd);
            break;
        case WM_SIZE:
            if (hEditControl) {
                RECT rcClient;
                GetClientRect(hWnd, &rcClient);

                int margin = 10;
                int width  = rcClient.right - rcClient.left - margin * 2;
                int height = rcClient.bottom - rcClient.top - margin * 2;

                MoveWindow(hEditControl, margin, margin, width, height, TRUE);
            }
        break;

        case WM_TIMER:
            if (wparam == inactivityTimerId) {
                DWORD currentTime = GetTickCount();
                if (currentTime - lastActivityTime > inactivityTimeout && !isScreenBlackedOut) {
                    BlackOutScreen(hWnd);
                }
            } else if (wparam == animationTimerId &&isScreenBlackedOut) {
                UpdateFigurePosition(hWnd);
                DrawAnimatedFigure(hWnd);
            }
        break;
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_MOUSEWHEEL:
            ResetInactivityTimer();
            if (isScreenBlackedOut) {
                RestoreScreen(hWnd);
            }
            break;
        case WM_KEYDOWN:
            ResetInactivityTimer();
            switch (wparam) {
                case 'W': case 'w':
                    manualDy = -manualSpeed;
                    autoMove = false;
                    break;
                case 'S': case 's':
                    manualDy = manualSpeed;
                    autoMove = false;
                    break;
                case 'A': case 'a':
                    manualDx = -manualSpeed;
                    autoMove = false;
                    break;
                case 'D': case 'd':
                    manualDx = manualSpeed;
                    autoMove = false;
                    break;
                default:
                    if (isScreenBlackedOut) {
                        RestoreScreen(hWnd);
                    }
                    break;
            }
            break;

        case WM_KEYUP:
            switch (wparam) {
                case 'W': case 'w':
                case 'S': case 's':
                    manualDy = 0;
                    break;
                case 'A': case 'a':
                case 'D': case 'd':
                    manualDx = 0;
                    break;
            }
            if (manualDx == 0 && manualDy == 0) {
                autoMove = true;
            }
            break;
        case WM_DESTROY:
            KillTimer(hWnd, inactivityTimerId);
            KillTimer(hWnd, animationTimerId);
            PostQuitMessage(0);
            break;

        default:
            return
        DefWindowProc(hWnd, message, wparam, lparam); // обрабатывает не явные сообщения окна
    }
        return 0;
}

LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,
                                  UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    HWND hParent = (HWND)dwRefData;

    switch (msg) {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MOUSEWHEEL:
            ResetInactivityTimer();
            if (isScreenBlackedOut) {
                RestoreScreen(hParent);
            }
            break;

        case WM_GETDLGCODE:
            // Говорим, что хотим все клавиши, чтобы они доходили до SoftwareMainProcedure
            return DLGC_WANTALLKEYS;
    }
    return DefSubclassProc(hWnd, msg, wParam, lParam);
}



void MainWndAddMenus(HWND hWnd)
{
HMENU rootMenu = CreateMenu();
HMENU fileMenu = CreateMenu();
HMENU editMenu = CreateMenu();

//actions of File's menu
AppendMenu(fileMenu, MF_STRING, OnMenuOpen, L"Open");
AppendMenu(fileMenu, MF_STRING, OnMenuSave, L"Save");
// AppendMenu(fileMenu, MF_SEPARATOR, NULL, NULL);
AppendMenu(fileMenu, MF_STRING, OnMenuExit, L"Exit");

//actions of Edit's menu
AppendMenu(editMenu, MF_STRING, OnMenuCopy, L"Copy");
AppendMenu(editMenu, MF_STRING, OnMenuPaste, L"Paste");
AppendMenu(editMenu, MF_STRING, OnMenuCut, L"Cut");


AppendMenu(rootMenu, MF_POPUP, (UINT_PTR)fileMenu, L"File");
AppendMenu(rootMenu, MF_POPUP, (UINT_PTR)editMenu, L"Edit");
AppendMenu(rootMenu, MF_STRING, OnMenuProgram, L"About the program");

SetMenu(hWnd, rootMenu);

}

void MainWndAddEditControl(HWND hWnd)
{
    hEditControl = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
                                  WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
                                  10, 10, 360, 200, hWnd, (HMENU)1,
                                  GetModuleHandle(NULL), NULL);

    SetWindowSubclass(hEditControl, EditSubclassProc, 0, (DWORD_PTR)hWnd);
}

INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hwndDlg, 0);
                    return TRUE;
            }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;
    }
    return FALSE;
}

