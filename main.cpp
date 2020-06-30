#include <Windows.h>
#include <iostream>
#include "Render.h"

LRESULT CALLBACK WinProc(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam);
bool bCanExit = false;

int main()
{
    //get instance
    HINSTANCE HInstance = (HINSTANCE)GetModuleHandle(nullptr);

    //create WndClass
    WNDCLASSEX WndClass;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    WndClass.hIconSm = nullptr;
    WndClass.hInstance = HInstance;
    WndClass.lpfnWndProc = WinProc;
    WndClass.lpszClassName = TEXT("MainWClass");
    WndClass.lpszMenuName = nullptr;
    WndClass.style = CS_VREDRAW | CS_HREDRAW;

    //register WndClass
    RegisterClassEx(&WndClass);

    //create a window
    HWND HWnd = CreateWindowEx(
        0,
        TEXT("MainWClass"),
        TEXT("X-SoftRender"),
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        800,
        600,
        nullptr,
        nullptr,
        HInstance,
        nullptr
    );

    if (!HWnd)
    {
        std::cout<<"cerate window failed!!"<<std::endl;
        return -1;
    }

    //show window
    ShowWindow(HWnd, SW_NORMAL);

    //update window
    UpdateWindow(HWnd);

    //render
    Render* RenderInstance = new Render;
    RenderInstance->Init(HWnd);
    while(!bCanExit)
    {
        RenderInstance->Update();
        //dispatch message
        MSG Msg;
        while (PeekMessage(&Msg, nullptr, 0, 0, PM_NOREMOVE))
        {
            if (GetMessage(&Msg, nullptr, 0, 0))
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        }
    }
    RenderInstance->Exit();
    delete RenderInstance;
    return 0;
}

LRESULT CALLBACK WinProc(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
    switch (UMsg) 
    {
    case WM_KEYUP: 
    {
        if (WParam == VK_ESCAPE) 
        {
            bCanExit = true;
        }
    }
    }
    return DefWindowProc(HWnd, UMsg, WParam, LParam);
}
