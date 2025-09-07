#include <windows.h>
#include "definitions.h"



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
                    break;
            }
            break;
        case WM_CREATE:
            MainWndAddMenus(hWnd);
            break;
        //case WM_PAINT:
          //  break;
        case WM_DESTROY:
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
            // Инициализация диалогового окна
            return TRUE;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:      // Кнопка OK
                case IDCANCEL:  // Кнопка Cancel или крестик
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


