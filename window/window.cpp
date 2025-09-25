#include <windows.h>
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
                //default:
                //  break;
            }
            break;
        case WM_CREATE:
            MainWndAddMenus(hWnd);
            SetTimer(hWnd, inactivityTimerId,1000, NULL);
            MainWndAddEditControl(hWnd);
            break;
        case WM_SIZE:
        {
            if (hEditControl) {
                RECT rcClient;
                GetClientRect(hWnd, &rcClient);

                int margin = 10;
                int width  = rcClient.right - rcClient.left - margin * 2;
                int height = rcClient.bottom - rcClient.top - margin * 2;

                MoveWindow(hEditControl, margin, margin, width, height, TRUE);
            }
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
        //case WM_PAINT:
        //  break;
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
    hEditControl = CreateWindowEx(
            WS_EX_CLIENTEDGE,            // расширенный стиль (рамка)
            L"EDIT",                     // класс окна
            L"",                         // начальный текст
            WS_CHILD | WS_VISIBLE | WS_VSCROLL |
            ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, // стили
            10, 10,                      // позиция X, Y
            360, 200,                    // ширина, высота
            hWnd,                        // родительское окно
            (HMENU)1,                    // ID контрола
            GetModuleHandle(NULL),       // hInstance
            NULL                         // параметры создания
    );
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

