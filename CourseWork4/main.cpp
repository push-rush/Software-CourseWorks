#include <iostream>
#include <windows.h>

#include "./include/Button.hpp"
#include "./include/TextBox.hpp"
#include "./include/LogWindow.hpp"
#include "./include/ExpressionGenerator.hpp"

static Button *generateButton = nullptr;
static TextBox *classTextBox = nullptr;
static TextBox *numTextBox = nullptr;
static LogWindow *logWindow = nullptr;
static Combinator *combinator = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) 
    {
        case WM_CREATE: 
        {
            // 创建生成按钮
            WCHAR buttonText[] = L"生成表达式";
            generateButton = new Button(hwnd, 210, 85, 165, 40, buttonText);
            generateButton->SetFont(35);
            
            // 创建班级号输入框
            classTextBox = new TextBox(hwnd, 50, 75, 150, 35, L"年级...");
            classTextBox->SetFont(30);

            // 创建运算式数量输入框
            numTextBox = new TextBox(hwnd, 50, 120, 150, 35, L"数量...");
            numTextBox->SetFont(30);

            // 创建日志窗口
            logWindow = new LogWindow(hwnd);
            logWindow->SetBgBrush(CreateSolidBrush(RGB(255, 255, 255))); // 设置日志窗口背景颜色为白色

            // 创建Combinator实例
            combinator = new Combinator();

            break;
        }
        case WM_COMMAND:
        {
            if (!generateButton || !classTextBox || !numTextBox)
            {
                std::cout << "Textbox or button is null..." << "\n";
                break;
            }

            if (HIWORD(wParam) == EN_CHANGE && (HWND)lParam == classTextBox->GetHandle())
            {   // 处理文本框的回车键事件
                // HWND hTextBox = textBox1->GetHandle();
                HWND hTextBox = (HWND)lParam;
                int length = GetWindowTextLength(hTextBox);
                std::string text(length + 1, '\0');
                GetWindowText(hTextBox, &text[0], length + 1);
                logWindow->Log(text);
                std::cout << "classTextBox text: " << text << std::endl;
            }
            else if (HIWORD(wParam) == EN_CHANGE && (HWND)lParam == numTextBox->GetHandle())
            {   // 处理文本框的回车键事件
                // HWND hTextBox = textBox1->GetHandle();
                HWND hTextBox = (HWND)lParam;
                int length = GetWindowTextLength(hTextBox);
                std::string text(length + 1, '\0');
                GetWindowText(hTextBox, &text[0], length + 1);
                logWindow->Log(text);
                std::cout << "numTextBox text: " << text << std::endl;
            }
            else if ((HWND)lParam == generateButton->GetHandle() && HIWORD(wParam) == BN_CLICKED)
            {
                // 从班级号输入框获取班级号
                std::string classStr = classTextBox->GetText();
                if (!isValidInteger(classStr))
                {
                    MessageBoxW(hwnd, L"班级号必须是整数！", L"错误", MB_OK | MB_ICONERROR);
                    return 0;
                }
                int cls_id = std::stoi(classStr);

                // 从运算式数量输入框获取数量
                std::string numStr = numTextBox->GetText();
                if (!isValidInteger(numStr))
                {
                    MessageBoxW(hwnd, L"运算式数量必须是整数！", L"错误", MB_OK | MB_ICONERROR);
                    return 0;
                }
                int num = std::stoi(numStr);

                // 使用Combinator类生成表达式
                std::vector<std::string> expressions = combinator->generateRandomExpression(cls_id, num);

                // 将生成的表达式显示在日志窗口中
                for (const std::string& exp : expressions)
                {
                    float result = combinator->computeResult(exp);
                    
                    // 将小数点后为0的浮点数转换为整数输出
                    if (std::abs(result - std::round(result)) < 0.00001)
                    {
                        logWindow->Log(exp + " = " + std::to_string((int)result));
                    }
                    else
                    {
                        logWindow->Log(exp + " = " + std::to_string(result));
                    }
                }
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // 创建字体句柄
            HFONT hFont = CreateFontW(
                36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"
            );

            // 将字体句柄选入设备环境（DC）中
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

            // 绘制中文字符
            WCHAR text[] = L"四则运算式生成器";
            TextOutW(hdc, 15, 15, text, lstrlenW(text));

            // 恢复原来的字体句柄
            SelectObject(hdc, hOldFont);

            // 删除创建的字体句柄
            DeleteObject(hFont);

            // EndPaint(hwnd, &ps);

            // 获取 LogWindow 控件的设备上下文
            HDC hdcLogWindow = GetDC(logWindow->GetHandle());

            // 在这里使用 hdcLogWindow 进行绘制
            SetBkColor(hdcLogWindow, RGB(25, 30, 45)); // 设置背景色
            SetTextColor(hdcLogWindow, RGB(24, 25, 35)); // 设置文本颜色

            // 释放 LogWindow 控件的设备上下文
            ReleaseDC(logWindow->GetHandle(), hdcLogWindow);
            EndPaint(logWindow->GetHandle(), &ps);

            return 0;
        }
        case WM_ERASEBKGND:
        {
            // 获取主窗口的设备上下文
            HDC hdc = (HDC)wParam;

            // 创建画刷并填充背景
            HBRUSH hBrush = CreateSolidBrush(RGB(125, 125, 125));
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hBrush);

            // 释放画刷资源
            DeleteObject(hBrush);

            return (LRESULT)1; // 表示背景已经被擦除，不需要默认处理
        }
        case WM_KEYDOWN:
        {
            // 处理键盘按键按下事件
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
            return 0;
        }
        case WM_DESTROY: 
        {
            // 释放资源
            if (generateButton)
            {
                delete generateButton;
                generateButton = nullptr;
            }

            if (classTextBox)
            {
                delete classTextBox;
                classTextBox = nullptr;
            }

            if (numTextBox)
            {
                delete numTextBox;
                numTextBox = nullptr;
            }

            if (logWindow)
            {
                delete logWindow;
                logWindow = nullptr;
            }

            if (combinator)
            {
                delete combinator;
                combinator = nullptr;
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
    int windowWidth = 480;
    int windowHeight = 720;
    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(0, className, "Caculator", WS_OVERLAPPEDWINDOW,
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