#include "../include/Button.hpp"

Button::Button(HWND parentWnd, int x, int y, int width, int height, LPCWSTR buttonText) : m_clickHandler(nullptr)
{
    m_hButton = CreateWindowW(
        L"BUTTON", buttonText, 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        x, y, width, height, 
        parentWnd, NULL, NULL, NULL
    );

    if (m_hButton == NULL) 
    {
        MessageBox(NULL, "Failed to create button!", "Error", MB_ICONERROR | MB_OK);
    }

    // 设置按钮点击事件的回调函数
    SetWindowLongPtr(m_hButton, GWLP_USERDATA, (LONG_PTR)this);
    m_oldWndProc = (WNDPROC)SetWindowLongPtr(m_hButton, GWLP_WNDPROC, (LONG_PTR)Button::ButtonProc);
}

Button::~Button() 
{
    if (m_hButton != NULL) 
    {
        SetWindowLongPtr(m_hButton, GWLP_WNDPROC, (LONG_PTR)m_oldWndProc);
        DestroyWindow(m_hButton);
    }
}

void Button::SetClickHandler(void(*handler)()) 
{
    m_clickHandler = handler;
}

void Button::ProcessClicked()
{
    POINT pt;
    GetCursorPos(&pt); // 获取鼠标屏幕坐标
    ScreenToClient(m_hButton, &pt); // 转换为相对于按钮客户区的坐标

    MessageBox(NULL, "Button clicked!", "Info", MB_ICONINFORMATION | MB_OK);
}

LRESULT CALLBACK Button::ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    Button* button = reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg) 
    {
        // case WM_COMMAND:
        // {
        //     if (LOWORD(wParam) == BN_CLICKED && button != nullptr && button->m_clickHandler != nullptr) 
        //     {
        //         button->m_clickHandler();
        //         return 0;
        //     }
        //     break;
        // }
        default:
        {
            break;
        }
    }

    return CallWindowProc(button->m_oldWndProc, hwnd, uMsg, wParam, lParam);
}