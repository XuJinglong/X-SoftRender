#pragma once

#include <Windows.h>
#include <stdint.h>
#include "MathLib.h"

enum class ERenderMode 
{
	WireFrame,
	Shape
};

class Render
{
public:
    Render();
    ~Render();

    void Init(HWND HWnd);
    void Update();
    void Exit();

private:
	void DrawTriangle(Point2D V1, Point2D V2, Point2D V3);
	void DrawUpTriangle(Point2D V1, Point2D V2, Point2D V3);
	void DrawDownTriangle(Point2D V1, Point2D V2, Point2D V3);
private:
    static const int ViewWidth = 800;
    static const int ViewHeight = 600;

    HWND DrawWnd = nullptr;
    HDC ScreenDC = nullptr;
    HDC BufferDC = nullptr;
    HBITMAP BitMap = nullptr;

    uint32_t* ColorBuffer = nullptr;

	ERenderMode RenderMode = ERenderMode::WireFrame;
	uint32_t WireFrameThickness = 5;
};