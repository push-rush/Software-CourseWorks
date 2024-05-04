// TextBox.h
#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <windows.h>
#include <string>

class TextBox {
public:
    TextBox(HWND parentWnd, int x, int y, int width, int height);
    ~TextBox();

    std::string GetText() const;
    void SetText(const std::string& text);

    HWND GetHandle() const { return m_hTextBox; }

private:
    HWND m_hTextBox;
    WNDPROC m_oldWndProc;

    static LRESULT CALLBACK TextBoxProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // TEXTBOX_H