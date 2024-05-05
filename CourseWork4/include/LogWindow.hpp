#ifndef LOG_WINDOW_
#define LOG_WINDOW_

#include <iostream>
#include <windows.h>
#include <string>

class LogWindow 
{
public:
    LogWindow(HWND parentWnd);
    ~LogWindow();

    void SetBgBrush(HBRUSH brush);
    void Log(const std::string& message);
    HWND GetHandle() const { return m_hLogEdit; }
    
private:
    HWND m_hLogEdit;
    WNDPROC m_oldWndProc;
    HBRUSH m_hBgBrush;

    static LRESULT CALLBACK LogWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    std::string EscapeCharacters(const std::string& str);
};
#endif
