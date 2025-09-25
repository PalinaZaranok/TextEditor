#pragma once

#include "../definitions.h"

void ResetInactivityTimer();
void BlackOutScreen(HWND hWnd);
void UpdateFigurePosition(HWND hWnd);
void DrawAnimatedFigure(HWND hWnd);
void RestoreScreen(HWND hWnd);
