//
// Created by 86180 on 25-10-17.
//
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <tchar.h>

using namespace std;

struct Bubble {
    int x, y;
    int r;
    int dx, dy;
    COLORREF color;
    bool active;
};

vector<Bubble> bubbles;
int score = 0;
HWND hMainWnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        srand((unsigned int)time(NULL));
        SetTimer(hwnd, 1, 50, NULL);  // 20 FPS
        hMainWnd = hwnd;
        return 0;

    case WM_LBUTTONDOWN:  // 鼠标左键点击吹泡泡
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            Bubble b;
            b.x = x;
            b.y = y;
            b.r = rand() % 30 + 20;  // 随机半径 20-50
            b.dx = rand() % 5 - 2;   // 左右漂移 -2 ~ 2
            b.dy = -(rand() % 5 + 5); // 向上速度 5~10
            b.color = RGB(rand() % 256, rand() % 256, rand() % 256);  // 随机颜色
            b.active = true;
            bubbles.push_back(b);
        }
        return 0;

    case WM_TIMER:  // 更新泡泡位置
        {
            for (auto it = bubbles.begin(); it != bubbles.end(); ) {
                if (!it->active) {
                    it = bubbles.erase(it);
                    continue;
                }
                it->x += it->dx;
                it->y += it->dy;
                it->dy += 0;  // 可添加重力 it->dy += 0.1;
                if (it->y + it->r < 0) {  // 泡泡飞出屏幕
                    score++;
                    it->active = false;
                }
                ++it;
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rcClient;
            GetClientRect(hwnd, &rcClient);

            // 双缓冲避免闪烁
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
            HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

            // 绘制背景（天空蓝）
            HBRUSH hBgBrush = CreateSolidBrush(RGB(135, 206, 250));
            FillRect(hdcMem, &rcClient, hBgBrush);
            DeleteObject(hBgBrush);

            // 绘制所有活跃泡泡
            for (const auto& b : bubbles) {
                if (b.active) {
                    HBRUSH hBrush = CreateSolidBrush(b.color);
                    SelectObject(hdcMem, hBrush);
                    Ellipse(hdcMem, b.x - b.r, b.y - b.r, b.x + b.r, b.y + b.r);
                    HPEN hPen = CreatePen(PS_NULL, 0, 0);
                    SelectObject(hdcMem, hPen);
                    DeleteObject(hPen);
                    DeleteObject(hBrush);
                }
            }

            // 绘制分数
            SetBkMode(hdcMem, TRANSPARENT);
            SetTextColor(hdcMem, RGB(255, 255, 255));
            TCHAR szScore[64];
            wsprintf(szScore, _T("吹泡泡数量: %d"), score);
            TextOut(hdcMem, 10, 10, szScore, _tcslen(szScore));

            // 复制到屏幕
            BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcMem, 0, 0, SRCCOPY);

            // 清理
            SelectObject(hdcMem, hbmOld);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);

            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = _T("BubbleGame");
    wc.lpszMenuName = NULL;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(_T("BubbleGame"), _T("使用GDI吹泡泡游戏"),
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
                            NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}