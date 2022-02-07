#include "Window.h"

bool Window::Initialize(HINSTANCE hInstance, int showWnd)
{
#ifdef _DEBUG
    // Create Console
    InitializeConsole();
#endif

    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.style = CS_HREDRAW | CS_VREDRAW; // Redraw on window resize
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
    wc.lpszClassName = TEXT("OurWindowClass");

    wc.lpfnWndProc =
        [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
        {
            switch (message)
            {
                // Message is called when window closes
            case WM_DESTROY:

                // Close the application
                PostQuitMessage(0);
                return 0;

            default:
                break;
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
        };

    // Register our window class
    RegisterClassEx(&wc);

    mWnd = CreateWindowEx(
        NULL,
        TEXT("OurWindowClass"),
        TEXT("DirectX11 Framework"),
        WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
        300,
        300,
        mWidth,
        mHeight,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(mWnd, showWnd);

    return true;
}

bool Window::InitializeConsole()
{
    if (!AllocConsole())
    {
        MessageBox(0, TEXT("Console alloc failed!"), TEXT("ERROR"), MB_OK);
    }

    SetConsoleTitleA("Debug Console");
    //if (!);
    //{
    //    MessageBox(0, TEXT("Setting console title failed!"), TEXT("ERROR"), MB_OK);
    //}

    typedef struct
    {
        char* _ptr;
        int _cnt;
        char* _base;
        int _flag;
        int _file;
        int _charbuf;
        int _bufsiz;
        char* _tmpfname;
    } FILE_COMPLETE;

    *(FILE_COMPLETE*)stdout = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
    *(FILE_COMPLETE*)stderr = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE), _O_TEXT), "w");
    *(FILE_COMPLETE*)stdin  = *(FILE_COMPLETE*)_fdopen(_open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT), "r");

    return true;
}
