#include <iostream>
#include <windows.h>

#include "./include/Button.hpp"
#include "./include/TextBox.hpp"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    static Button *button1;
    static TextBox* textBox1;
    switch (uMsg) 
    {
        case WM_CREATE: 
        {
            // 创建按钮
            WCHAR text[] = L"确认";
            button1 = new Button(hwnd, 260, 50, 100, 25, text);
            
            // 创建文本框
            textBox1 = new TextBox(hwnd, 50, 50, 200, 20);
            break;
        }
        case WM_COMMAND: 
        {
            if (LOWORD(wParam) == BN_CLICKED && (HWND)lParam == button1->GetHandle()) 
            {
                button1->ProcessClicked();
            }
            else if (HIWORD(wParam) == EN_CHANGE) 
            {   // 处理文本框的回车键事件
                HWND hTextBox = (HWND)lParam;
                int length = GetWindowTextLength(hTextBox);
                std::string text(length + 1, '\0');
                GetWindowText(hTextBox, &text[0], length + 1);
                std::cout << "Entered text: " << text << std::endl;
            }
            break;
        }
        case WM_PAINT: 
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 绘制中文字符
            WCHAR text[] = L"四则运算式生成器";
            TextOutW(hdc, 15, 15, text, lstrlenW(text));

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_KEYDOWN:
        {
            // 处理键盘按键按下事件
            // std::cout << "Press key: " << wParam << "\n";  
            switch (wParam)
            {
                case VK_ESCAPE:
                {
                    // 当按下 ESC 键时，关闭窗口
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                }
                default:
                    break;
            }
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            // 处理键盘按键按下事件
            // 处理鼠标移动事件
            // 获取鼠标坐标
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            // std::cout << "Mouse pos: (" << x << ", " << y << ")\n";
            
            // 在这里可以对鼠标坐标进行处理
            return 0;
        }
        case WM_DESTROY: 
        {
            if (button1)
            {
                delete button1;
                button1 = nullptr;
            }

            if (textBox1)
            {
                delete textBox1;
                textBox1 = nullptr;
            }
            
            PostQuitMessage(0);
            return 0;
        }
        default:
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char *className = "MyWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    RegisterClass(&wc);

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 计算窗口位置，使其位于屏幕中心
    int windowWidth = 500;
    int windowHeight = 300;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(0, className, "My Window", WS_OVERLAPPEDWINDOW,
                               windowX, windowY, windowWidth, windowHeight,
                               NULL, NULL, hInstance, NULL);

    // 显示窗口
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}