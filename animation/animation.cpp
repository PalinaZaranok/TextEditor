#include <windows.h>
#include "animation.h"
#include "../definitions.h"

int figureX = 0;
int figureY = 0;
int figureSize = 50;
int figureSpeedX = 3;
int figureSpeedY = 2;
int figureType = 0;

bool autoMove = true;
int manualDx = 0;
int manualDy = 0;
int manualSpeed = 5;

void ResetInactivityTimer() {
    lastActivityTime = GetTickCount();
}

void BlackOutScreen(HWND hWnd) {
    isScreenBlackedOut = true;

    RECT rect;
    GetClientRect(hWnd, &rect);
    figureX = (rect.right - rect.left) / 2;
    figureY = (rect.bottom - rect.top) / 2;
    figureType = rand() % 3;
    SetTimer(hWnd, animationTimerId, 33, NULL);
    DrawAnimatedFigure(hWnd);
}

void UpdateFigurePosition(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    if(autoMove){
        figureX += figureSpeedX;
        figureY += figureSpeedY;

        if (figureX - figureSize / 2 <= 0 || figureX + figureSize / 2 >= width){
            figureSpeedX = -figureSpeedX;
            figureType = (figureType + 1) % 3;
        }
        if (figureY - figureSize / 2 <= 0 || figureY + figureSize / 2 >= height) {
            figureSpeedY = -figureSpeedY;
            figureType = (figureType + 1) % 3;
        }
    } else {
        figureX += manualDx;
        figureY += manualDy;

        // чтобы не выходило за границы экрана
        int half = figureSize / 2;
        if (figureX - half < 0) figureX = half;
        if (figureY - half < 0) figureY = half;
        if (figureX + half > width) figureX = width - half;
        if (figureY + half > height) figureY = height - half;
    }
}


void DrawAnimatedFigure(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    // черный экран
    RECT rect;
    GetClientRect(hWnd, &rect);
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdc, &rect, blackBrush);
    DeleteObject(blackBrush);

    HBRUSH figureBrush = CreateSolidBrush(RGB(0, 0, 255));
    HPEN figurePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

    HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, figureBrush);
    HPEN oldPen = (HPEN) SelectObject(hdc, figurePen);
    switch (figureType) {
        case 0:
            Ellipse(hdc, figureX - figureSize / 2, figureY - figureSize / 2,
                    figureX + figureSize / 2, figureY + figureSize / 2);
            break;

        case 1:
            Rectangle(hdc, figureX - figureSize / 2, figureY - figureSize / 2,
                      figureX + figureSize / 2, figureY + figureSize / 2);
            break;

        case 2: {
            POINT triangle[3] = {
                    {figureX,                  figureY - figureSize / 2},
                    {figureX - figureSize / 2, figureY + figureSize / 2},
                    {figureX + figureSize / 2, figureY + figureSize / 2}
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
void RestoreScreen(HWND hWnd) {
    isScreenBlackedOut = false;
    KillTimer(hWnd, animationTimerId);
    InvalidateRect(hWnd, NULL, TRUE);
    UpdateWindow(hWnd);
}
