// Button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <windows.h>

class Button 
{
public:
    Button(HWND parentWnd, int x, int y, int width, int height, LPCWSTR buttonText);
    ~Button();

    HWND GetHandle() const { return m_hButton; }
    void SetClickHandler(void(*handler)());
    void ProcessClicked();

private:
    HWND m_hButton;
    WNDPROC m_oldWndProc;
    void(*m_clickHandler)();

    static LRESULT CALLBACK ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
#endif // BUTTON_H