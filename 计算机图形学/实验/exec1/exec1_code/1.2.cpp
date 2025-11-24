//
// Created by 86180 on 25-10-17.
//
#include <windows.h>

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 获取窗口客户区尺寸
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            int clientWidth = clientRect.right - clientRect.left;
            int clientHeight = clientRect.bottom - clientRect.top;

            // 定义油画区域（居中，适应窗口）
            int paintingX = (clientWidth - 300) / 2;
            int paintingY = (clientHeight - 200) / 2;
            int paintingWidth = 300;
            int paintingHeight = 200;

            // 绘制油画
            // 天空（蓝色矩形）
            HBRUSH skyBrush = CreateSolidBrush(RGB(135, 206, 235)); // 天蓝色
            if (skyBrush) {
                RECT skyRect = {paintingX, paintingY, paintingX + paintingWidth, paintingY + paintingHeight};
                FillRect(hdc, &skyRect, skyBrush);
                DeleteObject(skyBrush);
            }

            // 草地（绿色矩形）
            HBRUSH grassBrush = CreateSolidBrush(RGB(34, 139, 34)); // 绿色
            if (grassBrush) {
                RECT grassRect = {paintingX, paintingY + paintingHeight / 2, paintingX + paintingWidth, paintingY + paintingHeight};
                FillRect(hdc, &grassRect, grassBrush);
                DeleteObject(grassBrush);
            }

            // 太阳（黄色圆形）
            HBRUSH sunBrush = CreateSolidBrush(RGB(255, 255, 0)); // 黄色
            if (sunBrush) {
                SelectObject(hdc, sunBrush);
                Ellipse(hdc, paintingX + paintingWidth - 60, paintingY + 20, paintingX + paintingWidth - 20, paintingY + 60);
                DeleteObject(sunBrush);
            }

            // 绘制相框
            // 阴影（灰色）
            HBRUSH shadowBrush = CreateSolidBrush(RGB(100, 100, 100));
            if (shadowBrush) {
                RECT shadowRect = {paintingX - 10, paintingY - 10, paintingX + paintingWidth + 10, paintingY + paintingHeight + 10};
                FrameRect(hdc, &shadowRect, shadowBrush);
                DeleteObject(shadowBrush);
            }

            // 金色边框（只画边框，不填充）
            HPEN framePen = CreatePen(PS_SOLID, 5, RGB(184, 134, 11)); // 金色
            if (framePen) {
                SelectObject(hdc, framePen);
                // 使用 Rectangle 只画边框（需要设置透明画刷）
                HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                Rectangle(hdc, paintingX - 5, paintingY - 5, paintingX + paintingWidth + 5, paintingY + paintingHeight + 5);
                SelectObject(hdc, oldBrush); // 恢复原来的画刷
                DeleteObject(framePen);
            }

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// 主函数（ANSI 版本）
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 注册窗口类
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "OilPaintingWindowClass";
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClassA(&wc)) {
        return 1; // 注册失败
    }

    // 创建窗口
    HWND hwnd = CreateWindowExA(
        0, "OilPaintingWindowClass", "Simple Oil Painting with GDI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );
    if (!hwnd) {
        return 1; // 创建窗口失败
    }

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}