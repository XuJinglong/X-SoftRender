#include "Render.h"

Render::Render()
{
}

Render::~Render()
{

}


void Render::Init(HWND HWnd)
{
	DrawWnd = HWnd;
	ScreenDC = GetDC(HWnd);
	BufferDC = CreateCompatibleDC(ScreenDC);

	BITMAPINFO BPInfo;
	memset(&BPInfo, 0, sizeof(BITMAPINFO));
	BPInfo.bmiHeader = { sizeof(BITMAPINFOHEADER), ViewWidth, ViewHeight, 1, 32, BI_RGB, 0,0,0,0,0 };
	BitMap = CreateDIBSection(BufferDC, &BPInfo, DIB_RGB_COLORS, (void**)&ColorBuffer, 0, 0);

	SelectObject(BufferDC, BitMap);
}

void Render::Exit() 
{
	DeleteObject(BitMap);
	ReleaseDC(DrawWnd, ScreenDC);
	DeleteDC(BufferDC);
}

void Render::Update()
{
	//Draw Scene
	//set point color and file the buffer
	//绘制物体的坐标转化为屏幕空间的点坐标，求得buffer中的position，直接设值即可，不必用二维数组
	for (int i = 0; i < ViewWidth * ViewHeight; i++)
	{
		ColorBuffer[i] = 0xc0c000;
	}

	/*for (int i = 0; i < ViewHeight; i++) 
	{
		for (int j = 0; j < ViewWidth; j++) 
		{
			ColorBuffer[i][j] = 0xc0c000;
		}
	}*/

	BitBlt(ScreenDC, 0, 0, ViewWidth, ViewHeight, BufferDC, 0, 0, SRCCOPY);

	Sleep(1);
}