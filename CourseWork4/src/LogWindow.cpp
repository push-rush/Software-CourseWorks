#include "../include/LogWindow.hpp"

LogWindow::LogWindow(HWND parentWnd) 
{
    RECT parentRect;
    GetClientRect(parentWnd, &parentRect);

    int width = parentRect.right;
    int height = parentRect.bottom / 4;
    int titleHeight = GetSystemMetrics(SM_CYCAPTION); // 获取标题栏的高度
    std::cout << "Create log window..." << "\n";
     
    // 创建多行编辑框
    m_hLogEdit = CreateWindowW(
        L"EDIT", L"LogOutput:\r\n", 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
        0, height, width, parentRect.bottom - height, 
        parentWnd, NULL, NULL, NULL
    );

    if (m_hLogEdit == NULL) 
    {
        MessageBox(NULL, "Failed to create log edit box!", "Error", MB_ICONERROR | MB_OK);
    }

    // 设置中文字体
    HFONT hFont = CreateFontW(22, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                              DEFAULT_PITCH | FF_SWISS, L"宋体");
    SendMessage(m_hLogEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    // 将 LogWindowProc 函数设置为多行编辑框的窗口过程函数
    SetWindowLongPtr(m_hLogEdit, GWLP_USERDATA, (LONG_PTR)this);
    m_oldWndProc = (WNDPROC)SetWindowLongPtr(m_hLogEdit, GWLP_WNDPROC, (LONG_PTR)LogWindow::LogWindowProc);

    std::cout << "Create log window over..." << "\n";
}

LogWindow::~LogWindow() 
{
    if (m_hLogEdit != NULL) 
    {
        DestroyWindow(m_hLogEdit);
    }
}

std::string LogWindow::EscapeCharacters(const std::string& str) 
{
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) 
    {
        switch (str[i]) 
        {
            case '\n':
                result += "\\n"; // 替换换行符为 \n
                break;
            case '\t':
                result += "\\t"; // 替换制表符为 \t
                break;
            // 在此处可以添加其他需要转义的字符
            default:
                result += str[i]; // 其他字符保持不变
                break;
        }
    }
    return result;
}

void LogWindow::Log(const std::string& message) 
{
    if (m_hLogEdit != NULL) 
    {
        std::string processedMessage = EscapeCharacters(message);

        int textLength = GetWindowTextLength(m_hLogEdit);
        SendMessage(m_hLogEdit, EM_SETSEL, textLength, textLength);
        SendMessage(m_hLogEdit, EM_REPLACESEL, FALSE, (LPARAM)message.c_str());
        // SendMessage(m_hLogEdit, EM_REPLACESEL, FALSE, (LPARAM)(message + "\r\n").c_str()); // 追加换行
        SendMessage(m_hLogEdit, EM_SCROLLCARET, 0, 0);
        
        // 在每次输出后插入一个换行符
        SendMessageW(m_hLogEdit, EM_SETSEL, -1, -1);
        SendMessageW(m_hLogEdit, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
    }
}

LRESULT CALLBACK LogWindow::LogWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    LogWindow* log_window = reinterpret_cast<LogWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    
    switch (uMsg) 
    {
        case WM_CREATE:
        {
            // 创建画刷并保存到LogWindow对象中
            log_window->m_hBgBrush = CreateSolidBrush(RGB(25, 30, 45));
            break;
        }
        case WM_DESTROY:
        {
            // 清理资源
            DeleteObject(log_window->m_hBgBrush);
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)log_window->m_oldWndProc);
            break;
        }
        case WM_CTLCOLOREDIT:
        {
            HDC hdcEdit = (HDC)wParam;
            SetBkColor(hdcEdit, RGB(0, 0, 0)); // 设置文本控件的文本背景色为白色
            SetTextColor(hdcEdit, RGB(125, 125, 125)); // 设置文本控件的文本颜色为黑色
            return (LRESULT)GetStockObject(NULL_BRUSH); // 返回空画刷句柄，以便窗口不绘制背景
        }
        case WM_ERASEBKGND:
        {
            // RECT rect;
            // GetClientRect(log_window->m_hLogEdit, &rect);
            // HDC hdcLogWindow = GetDC(log_window->m_hLogEdit);
            // FillRect(hdcLogWindow, &rect, log_window->m_hBgBrush);
            // return (LRESULT)1;
        }
        default:
        {
            // 默认处理
            break;
        }
    }
    // return CallWindowProc((WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA), hwnd, uMsg, wParam, lParam);
    CallWindowProc(log_window->m_oldWndProc, hwnd, uMsg, wParam, lParam);
}

void LogWindow::SetBgBrush(HBRUSH brush)
{
    this->m_hBgBrush = brush;
}