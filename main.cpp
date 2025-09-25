#include <windows.h>
#include "definitions.h"
#include "window/window.h"
#include "animation/animation.h"

UINT_PTR inactivityTimerId = 1;
UINT_PTR animationTimerId = 2;
const UINT inactivityTimeout = 30000;
bool isScreenBlackedOut = false;
DWORD lastActivityTime = 0;


// идентификатор приложения, дескриптор предыдущего приложения, аргументы командной строки, флаг окна(развернуто, свернуто и т.д.)
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    //структура для описания параметров окна
    WNDCLASS SoftwareMainClass = NewWindowClass((HBRUSH)(COLOR_WINDOW), LoadCursor(NULL,
                                                                                   IDC_ARROW), hInst,
                                                LoadIcon(NULL, IDI_QUESTION),
                                                L"MainWndClass", SoftwareMainProcedure);

    //регистрация класса окна в Windows
    if(!RegisterClassW(&SoftwareMainClass)) {return -1;}


    HWND hWnd = CreateWindowEx(0,L"MainWndClass", L"Text Editor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL,
                 NULL, NULL);


    ShowWindow(hWnd, ncmdshow);
    UpdateWindow(hWnd);

    lastActivityTime = GetTickCount();

    MSG SoftwareMainMessage = {0};

    while(GetMessage(&SoftwareMainMessage, NULL, 0, 0)){
        TranslateMessage(&SoftwareMainMessage); // обработка клавиатурных событий
        DispatchMessage(&SoftwareMainMessage);  // отправка сообщений в функцию обработки сообщения

    }
    return 0;
}

