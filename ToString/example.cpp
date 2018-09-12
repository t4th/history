// include the basic windows header file
#include "tostr.h"

#include <windows.h>
#include <windowsx.h>
#include <string>

#define IDC_EDIT 100

LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam);

void SetText(HWND a_hWnd, std::wstring m_oInput) {
    DWORD StartPos, EndPos;
    SendMessage(a_hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&StartPos), reinterpret_cast<WPARAM>(&EndPos));
    int outLength = GetWindowTextLength(a_hWnd);
    SendMessage(a_hWnd, EM_SETSEL, outLength, outLength);
    m_oInput.append(TEXT("\r\n"));
    SendMessage(a_hWnd, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(m_oInput.c_str()));
    SendMessage(a_hWnd, EM_SETSEL, StartPos, EndPos);
    SendMessage(a_hWnd, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);
}

void SetTextA(HWND a_hWnd, std::string m_oInput) {
    DWORD StartPos, EndPos;
    SendMessageA(a_hWnd, EM_GETSEL, reinterpret_cast<WPARAM>(&StartPos), reinterpret_cast<WPARAM>(&EndPos));
    int outLength = GetWindowTextLength(a_hWnd);
    SendMessageA(a_hWnd, EM_SETSEL, outLength, outLength);
    m_oInput.append("\r\n");
    SendMessageA(a_hWnd, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(m_oInput.c_str()));
    SendMessageA(a_hWnd, EM_SETSEL, StartPos, EndPos);
    SendMessageA(a_hWnd, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);
}

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    HWND hWnd = 0;
    HWND hEdit = 0;

    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 800, 500 };    // set the size, but not the position
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

    hWnd = CreateWindowEx(NULL,
        L"WindowClass1",    // name of the window class
        L"form1",   // title of the window
        WS_OVERLAPPEDWINDOW,    // window style
        300,    // x-position of the window
        300,    // y-position of the window
        wr.right - wr.left,    // width of the window
        wr.bottom - wr.top,    // height of the window
        NULL,    // we have no parent window, NULL
        NULL,    // we aren't using menus, NULL
        hInstance,    // application handle
        NULL);    // used with multiple windows, NULL

    hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
        TEXT("EDIT"),
        NULL,
        WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE,
        50, 60, 700, 400,
        hWnd,
        (HMENU)IDC_EDIT,
        GetModuleHandle(NULL),
        NULL);

    RECT Rect = { 0 };
    GetWindowRect(hEdit, &Rect);
    MapWindowPoints(HWND_DESKTOP, GetParent(hEdit), (LPPOINT)&Rect, 2);

    int da = 666;
    double asd = 166.299;

    std::string sometxt2 = "some random text\r\n";
    std::wstring sometxt = L"some random text\r\n";
    
    std::string a2 = (ToStr)22 + "go\r\n" + sometxt2 + Rect.left + 2 + " " + asd + "\r\n" + 233 + " " + da
        + "\r\n0x" + Hex(0xccaabb, 8)
        + "\r\ndec: " + 67253082811
        + "\r\nhex: 0x" + Hex(67253082811, 16)
        + "\r\nbin: " + (Bin)0x124ca3b3
        + "\r\nbin: " + Bin(67253082811, 64)
        + "\r\nbin: " + Bin(0xcaccaabb, 40)
        + "\r\nbin: " + Bin(0x3accaabb, 20);

    std::wstring a = L" ";
    
    a = L"go\r\n" + sometxt + Rect.left + 2 + L" " + asd + L"\r\n" + 233 + L" " + da
        + L"\r\n0x" + Hex(0xccaabb, 8)
        + L"\r\ndec: " + 67253082811
        + L"\r\nhex: 0x" + Hex(67253082811, 16)
        + L"\r\nbin: " + (Bin)0x124ca3b3
        + L"\r\nbin: " + Bin(67253082811, 64)
        + L"\r\nbin: " + Bin(0xcaccaabb, 40)
        + L"\r\nbin: " + a + Bin(0x3accaabb, 20) + a;

    std::wstring b;
    b = ToStr(22) + L" " + Bin(0x3accaabb, 20) + b;
    b = (ToStr)22 + L"\r\nbin: " + Bin(0x3accaabb, 20) + b;

    //StringW lala = ToStr(2) + L"asdasd\r\n" + a + lala;
    
    SetTextA(hEdit, a2);
    SetText(hEdit, a);

    ShowWindow(hWnd, nCmdShow);

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
                       PostQuitMessage(0);
                       return 0;
    } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
