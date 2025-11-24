#include <windows.h>
#include <iostream>

// 在程序开始时设置控制台代码页
void SetConsoleEncoding() {
    SetConsoleOutputCP(65001);  // 65001 = UTF-8
    SetConsoleCP(65001);
}

// 窗口过程函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 绘制函数
void DrawShapes(HDC hdc);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 设置控制台编码
    SetConsoleEncoding();
    // 窗口类名
    static TCHAR szClassName[] = TEXT("GraphicsDemo");
    
    // 窗口类
    WNDCLASSEX wndclass;
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szClassName;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    // 注册窗口类
    if (!RegisterClassEx(&wndclass)) {
        MessageBox(NULL, TEXT("程序注册失败！"), TEXT("错误"), MB_ICONERROR);
        return 0;
    }
    
    // 创建窗口
    HWND hwnd = CreateWindow(
        szClassName,                    // 窗口类名
        TEXT("GDI绘图演示"),            // 窗口标题
        WS_OVERLAPPEDWINDOW,          // 窗口样式
        CW_USEDEFAULT,                // x坐标
        CW_USEDEFAULT,                // y坐标
        800,                          // 宽度
        600,                          // 高度
        NULL,                         // 父窗口句柄
        NULL,                         // 菜单句柄
        hInstance,                    // 程序实例句柄
        NULL                          // 创建参数
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
    
    return msg.wParam;
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    
    switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            DrawShapes(hdc);
            EndPaint(hwnd, &ps);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, message, wParam, lParam);
}

// 绘制各种图形
void DrawShapes(HDC hdc) {
    // 创建画笔和画刷
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));     // 蓝色画笔
    HPEN hRedPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));   // 红色画笔
    HPEN hGreenPen = CreatePen(PS_DASH, 2, RGB(0, 128, 0));  // 绿色虚线笔
    
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0));       // 黄色画刷
    HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 100, 255));   // 蓝色画刷
    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 100, 100));  // 浅红色画刷
    
    // 设置字体
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                           DEFAULT_PITCH | FF_DONTCARE, TEXT("宋体"));
    
    // 1. 使用MoveTo和LineTo绘制线条
    SelectObject(hdc, hPen);
    SetTextAlign(hdc, TA_CENTER);
    // 使用宽字符避免中文乱码
    TextOutW(hdc, 150, 30, L"1. MoveTo/LineTo 绘制线条", 22);
    
    MoveToEx(hdc, 50, 60, NULL);      // 移动到起点
    LineTo(hdc, 250, 60);             // 画直线到终点
    LineTo(hdc, 150, 120);            // 继续画线
    LineTo(hdc, 50, 60);              // 闭合三角形
    
    // 2. 使用Rectangle绘制矩形
    SelectObject(hdc, hRedPen);
    SelectObject(hdc, hBrush);
    TextOutW(hdc, 400, 30, L"2. Rectangle 绘制矩形", 21);
    Rectangle(hdc, 320, 60, 480, 140);  // 绘制填充矩形
    
    // 3. 使用Ellipse绘制椭圆
    SelectObject(hdc, hGreenPen);
    SelectObject(hdc, hBlueBrush);
    TextOutW(hdc, 150, 180, L"3. Ellipse 绘制椭圆", 19);
    Ellipse(hdc, 50, 210, 250, 310);    // 绘制填充椭圆
    
    // 4. 使用Arc绘制圆弧
    SelectObject(hdc, hRedPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));  // 不填充
    TextOutW(hdc, 400, 180, L"4. Arc 绘制圆弧", 16);
    Arc(hdc, 320, 210, 480, 310,      // 外接矩形
        320, 260,                     // 起始点
        480, 260);                    // 结束点
    
    // 5. 使用Polygon绘制多边形
    SelectObject(hdc, hPen);
    SelectObject(hdc, hRedBrush);
    TextOutW(hdc, 150, 350, L"5. Polygon 绘制多边形", 21);
    
    POINT polygonPoints[] = {
        {150, 380},    // 顶点1
        {200, 400},    // 顶点2
        {180, 450},    // 顶点3
        {120, 450},    // 顶点4
        {100, 400}     // 顶点5
    };
    Polygon(hdc, polygonPoints, 5);   // 绘制五边形
    
    // 6. 综合示例：绘制房子
    SelectObject(hdc, hPen);
    SelectObject(hdc, hBlueBrush);
    TextOutW(hdc, 400, 350, L"6. 综合示例：绘制房子", 19);
    
    // 房子主体（矩形）
    Rectangle(hdc, 350, 400, 450, 500);
    
    // 屋顶（三角形，用MoveTo/LineTo）
    SelectObject(hdc, hRedPen);
    SelectObject(hdc, hRedBrush);
    MoveToEx(hdc, 330, 400, NULL);
    LineTo(hdc, 400, 350);
    LineTo(hdc, 470, 400);
    LineTo(hdc, 330, 400);
    
    // 门（矩形）
    SelectObject(hdc, hPen);
    SelectObject(hdc, CreateSolidBrush(RGB(139, 69, 19)));  // 棕色
    Rectangle(hdc, 385, 450, 415, 500);
    
    // 窗户（椭圆）
    SelectObject(hdc, hGreenPen);
    SelectObject(hdc, CreateSolidBrush(RGB(200, 200, 255)));
    Ellipse(hdc, 360, 420, 380, 440);
    Ellipse(hdc, 420, 420, 440, 440);
    
    // 清理资源
    DeleteObject(hPen);
    DeleteObject(hRedPen);
    DeleteObject(hGreenPen);
    DeleteObject(hBrush);
    DeleteObject(hBlueBrush);
    DeleteObject(hRedBrush);
    DeleteObject(hFont);
}