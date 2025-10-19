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

            // 设置画笔和画刷
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // 黑色画笔
            HBRUSH hBrush = CreateSolidBrush(RGB(150, 150, 150)); // 灰色填充
            SelectObject(hdc, hPen);
            SelectObject(hdc, hBrush);

            // 绘制头部（椭圆）
            Ellipse(hdc, 200, 50, 300, 150);

            // 绘制眼睛（两个小椭圆）
            SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255))); // 白色填充眼睛
            Ellipse(hdc, 220, 80, 240, 100); // 左眼
            Ellipse(hdc, 260, 80, 280, 100); // 右眼
            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0))); // 黑色瞳孔
            Ellipse(hdc, 225, 85, 235, 95); // 左瞳孔
            Ellipse(hdc, 265, 85, 275, 95); // 右瞳孔

            // 绘制鼻子（三角形）
            POINT nose[] = {{250, 100}, {240, 120}, {260, 120}};
            Polygon(hdc, nose, 3);

            // 绘制嘴（弧线）
            Arc(hdc, 230, 120, 270, 140, 230, 130, 270, 130);

            // 绘制身体（矩形）
            SelectObject(hdc, hBrush); // 恢复灰色填充
            Rectangle(hdc, 210, 150, 290, 250);

            // 绘制左臂（矩形）
            Rectangle(hdc, 170, 150, 210, 200);

            // 绘制右臂（矩形）
            Rectangle(hdc, 290, 150, 330, 200);

            // 绘制左腿（矩形）
            Rectangle(hdc, 220, 250, 240, 300);

            // 绘制右腿（矩形）
            Rectangle(hdc, 260, 250, 280, 300);

            // 清理 GDI 对象
            DeleteObject(hPen);
            DeleteObject(hBrush);
            DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 注册窗口类
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RobotWindowClass"; // 使用窄字符
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    // 创建窗口
    HWND hwnd = CreateWindow(
        "RobotWindowClass", // 使用窄字符
        "Robot Drawing with GDI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL, NULL, hInstance, NULL
    );

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}