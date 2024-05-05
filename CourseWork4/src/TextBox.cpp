#include "../include/TextBox.hpp"

TextBox::TextBox(HWND parentWnd, int x, int y, int width, int height, LPCWSTR boxText)
: m_hTextBox(nullptr)
{
    m_hTextBox = CreateWindowW(
        L"EDIT", boxText, 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,
        x, y, width, height, 
        parentWnd, NULL, NULL, NULL
    );

    if (m_hTextBox == NULL) 
    {
        MessageBox(NULL, "Failed to create text box!", "Error", MB_ICONERROR | MB_OK);
    }

    // std::cout << "Textbox1: " << m_hTextBox << " " << "\n";

    // 设置文本框的窗口过程函数
    SetWindowLongPtr(m_hTextBox, GWLP_USERDATA, (LONG_PTR)this);
    m_oldWndProc = (WNDPROC)SetWindowLongPtr(m_hTextBox, GWLP_WNDPROC, (LONG_PTR)TextBox::TextBoxProc);
}

TextBox::~TextBox() 
{
    if (m_hTextBox != NULL) 
    {
        SetWindowLongPtr(m_hTextBox, GWLP_WNDPROC, (LONG_PTR)m_oldWndProc);
        DestroyWindow(m_hTextBox);
    }
}

std::string TextBox::GetText() const 
{
    int length = GetWindowTextLength(m_hTextBox);
    std::string text(length + 1, '\0');
    GetWindowText(m_hTextBox, &text[0], length + 1);
    return text;
}

void TextBox::SetText(const std::string& text) 
{
    SetWindowText(m_hTextBox, text.c_str());
}

HWND TextBox::GetHandle() const 
{ 
    return m_hTextBox; 
}

LRESULT CALLBACK TextBox::TextBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    TextBox* textBox = reinterpret_cast<TextBox*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg) 
    {
        case WM_CHAR:
        {
            // 处理键盘输入消息
            if (textBox != nullptr) 
            {
                // 如果是回车键，发送一个消息给父窗口，以通知用户按下了回车键
                if (wParam == VK_RETURN) 
                {
                    SendMessage(GetParent(hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(hwnd), EN_CHANGE), (LPARAM)hwnd);
                    return 0;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return CallWindowProc(textBox->m_oldWndProc, hwnd, uMsg, wParam, lParam);
}

void TextBox::SetFont(int fontSize)
{
    HFONT hFont = CreateFontW(
        fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial"
    );

    if (hFont != NULL)
    {
        SendMessage(m_hTextBox, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
}