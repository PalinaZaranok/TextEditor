#include <windows.h>
#include "definitions.h"

UINT_PTR inactivityTimerId = 1;
UINT_PTR animationTimerId = 2;
const UINT inactivityTimeout = 30000;
bool isScreenBlackedOut = false;
DWORD lastActivityTime = 0;

int figureX = 0;
int figureY = 0;
int figureSize = 50;
int figureSpeedX = 3;
int figureSpeedY = 2;
int figureType = 0;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)(COLOR_WINDOW), LoadCursor(NULL,
                                                                                   IDC_ARROW), hInst,
                                                LoadIcon(NULL, IDI_QUESTION),
                                                L"MainWndClass", SoftwareMainProcedure);

    if(!RegisterClassW(&SoftwareMainClass)) {return -1;}


    HWND hWnd = CreateWindowEx(0,L"MainWndClass", L"Text Editor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL,
                 NULL, NULL);


    ShowWindow(hWnd, ncmdshow);
    UpdateWindow(hWnd);

    lastActivityTime = GetTickCount();

    MSG SoftwareMainMessage = {0};

    while(GetMessage(&SoftwareMainMessage, NULL, 0, 0)){
        TranslateMessage(&SoftwareMainMessage);
        DispatchMessage(&SoftwareMainMessage);

    }
    return 0;
}

WNDCLASS NewWindowClass(HBRUSH bgColor, HCURSOR cursor, HINSTANCE hInst, HICON icon, LPCWSTR name, WNDPROC procedure)
{
    WNDCLASS NWC = { 0 };
    NWC.hIcon = icon;
    NWC.hCursor = cursor;
    NWC.hInstance = hInst;
    NWC.lpszClassName = name;
    NWC.lpfnWndProc = procedure;
    NWC.hbrBackground = bgColor;

    return NWC;

}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
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
            SetTimer(hWnd, inactivityTimerId, 1000, NULL);
            break;
        case WM_TIMER:
            if (wparam == inactivityTimerId) {
                DWORD currentTime = GetTickCount();
                if (currentTime - lastActivityTime > inactivityTimeout && !isScreenBlackedOut) {
                    BlackOutScreen(hWnd);
                }
            } else if (wparam == animationTimerId && isScreenBlackedOut) {
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
        default: return DefWindowProc(hWnd, message, wparam, lparam);
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

void ResetInactivityTimer()
{
    lastActivityTime = GetTickCount();
}

void BlackOutScreen(HWND hWnd)
{
    isScreenBlackedOut = true;

    RECT rect;
    GetClientRect(hWnd, &rect);
    figureX = (rect.right - rect.left) / 2;
    figureY = (rect.bottom - rect.top) / 2;
    figureType = rand() % 3;
    SetTimer(hWnd, animationTimerId, 33, NULL);
    DrawAnimatedFigure(hWnd);
}

void UpdateFigurePosition(HWND hWnd)
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // изменение позициии
    figureX += figureSpeedX;
    figureY += figureSpeedY;

    // расчет границ экрана
    if (figureX - figureSize/2 <= 0 || figureX + figureSize/2 >= width) {
        figureSpeedX = -figureSpeedX;
        figureType = (figureType + 1) % 3;
    }
    // изменение типа фигуры
    if (figureY - figureSize/2 <= 0 || figureY + figureSize/2 >= height) {
        figureSpeedY = -figureSpeedY;
        figureType = (figureType + 1) % 3;
    }
}


void DrawAnimatedFigure(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);

    // черный экран
    RECT rect;
    GetClientRect(hWnd, &rect);
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rect, blackBrush);
    DeleteObject(blackBrush);

    HBRUSH figureBrush = CreateSolidBrush(RGB(0, 0, 255));
    HPEN figurePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, figureBrush);
    HPEN oldPen = (HPEN)SelectObject(hdc, figurePen);
    switch (figureType) {
        case 0:
            Ellipse(hdc, figureX - figureSize/2, figureY - figureSize/2,
                    figureX + figureSize/2, figureY + figureSize/2);
            break;

        case 1:
            Rectangle(hdc, figureX - figureSize/2, figureY - figureSize/2,
                      figureX + figureSize/2, figureY + figureSize/2);
            break;

        case 2:
        {
            POINT triangle[3] = {
                    {figureX, figureY - figureSize/2},
                    {figureX - figureSize/2, figureY + figureSize/2},
                    {figureX + figureSize/2, figureY + figureSize/2}
            };
            Polygon(hdc, triangle, 3);
        }
            break;
    }
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(figureBrush);
    DeleteObject(figurePen);

    ReleaseDC(hWnd, hdc);
}

// отображение начального окна
void RestoreScreen(HWND hWnd)
{
    isScreenBlackedOut = false;
    KillTimer(hWnd, animationTimerId);
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}
