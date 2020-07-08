#pragma once

#include <Windows.h>
#include <stdint.h>
#include "MathLib.h"
#include "RawBox.h"
#include "Texture.h"

enum class ERenderMode
{
    WireFrame,
    Texture,
    Color,
    Max
};

enum class ERenderStyle
{
	Front,
	RayTrace,
	Max
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
	void RayTracingDrawTriangles(class Camera* Camera, class RawBox* DrawBox);
	void DrawTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3);
	void DrawUpTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, bool bDivided = false);
	void DrawDownTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, bool bDivided = false);

	bool GetRayCastPoint(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, const Vector3D& TraceStart, const Vector3D& Direction, RawVertex& RayCastV);
private:
    static const int ViewWidth = 600;
    static const int ViewHeight = 600;

    HWND DrawWnd = nullptr;
    HDC ScreenDC = nullptr;
    HDC BufferDC = nullptr;
    HBITMAP BitMap = nullptr;

    uint32_t* ColorBuffer = nullptr;
	float* ZBuffer = nullptr;

	Texture* Image;
public:
	ERenderMode RenderMode = ERenderMode::Texture;
	uint32_t WireFrameThickness = 1;

	ERenderStyle RenderStyle = ERenderStyle::Front;
};