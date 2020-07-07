#pragma once

#include <Windows.h>
#include <stdint.h>
#include "MathLib.h"
#include "RawBox.h"
#include "Texture.h"

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
	void DrawTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3);
	void DrawUpTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, bool bDivided = false);
	void DrawDownTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, bool bDivided = false);
private:
    static const int ViewWidth = 600;
    static const int ViewHeight = 600;

    HWND DrawWnd = nullptr;
    HDC ScreenDC = nullptr;
    HDC BufferDC = nullptr;
    HBITMAP BitMap = nullptr;

    uint32_t* ColorBuffer = nullptr;
	float* ZBuffer = nullptr;

	bool bUseTexture = true;
	Texture* Image;
public:
	ERenderMode RenderMode = ERenderMode::Shape;
	uint32_t WireFrameThickness = 1;
};