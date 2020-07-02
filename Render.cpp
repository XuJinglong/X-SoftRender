#include "Render.h"
#include "RawBox.h"
#include "Camera.h"

#include <vector>
#include <algorithm>

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

void Render::DrawTriangle(Point2D V1, Point2D V2, Point2D V3)
{
	std::vector<Point2D> OrderPoints = {V1, V2, V3};
	std::sort(OrderPoints.begin(), OrderPoints.end(), [](const Point2D& P1, const Point2D& P2) {return P1.Y < P2.Y; });
	Point2D Low = OrderPoints[0];
	Point2D Mid = OrderPoints[1];
	Point2D High = OrderPoints[2];
	if (Mid.Y == Low.Y) 
	{
		DrawUpTriangle(Low, Mid, High);
	}
	else if (Mid.Y == High.Y) 
	{
		DrawDownTriangle(Low, Mid, High);
	}
	else 
	{
		Point2D DivPoint;
		DivPoint.Y = Mid.Y;
		DivPoint.X = Low.X + (High.X - Low.X) * (Mid.Y - Low.Y) / (High.Y - Low.Y);
		DrawUpTriangle(DivPoint, Mid, High, true);
		DrawDownTriangle(Low, Mid, DivPoint, true);
	}
}

void Render::DrawUpTriangle(Point2D V1, Point2D V2, Point2D V3, bool bDivided)
{
	//注意填充规则
	//Draw Pixels
	Point2D LeftEnd = V1.X < V2.X ? V1 : V2;
	Point2D RightEnd = V1.X > V2.X ? V1 : V2;

	//TODO:屏幕空间裁剪
	//Scane lines
	int ScanY = V1.Y;
	while (ScanY < (int)V3.Y)
	{
		float LeftX = LeftEnd.X + (ScanY - LeftEnd.Y) * (V3.X - LeftEnd.X) / (V3.Y - LeftEnd.Y);
		float RightX = RightEnd.X + (ScanY - RightEnd.Y) * (V3.X - RightEnd.X) / (V3.Y - RightEnd.Y);

		if (LeftX == RightX) return;

		int Row = ScanY;
		int Column = 0;

		if (RenderMode == ERenderMode::Shape) 
		{
			for (int i = LeftX; i < (int)RightX; i++) 
			{
				Column = i;
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
				}
			}
		}
		else if (RenderMode == ERenderMode::WireFrame)
		{
			if (ScanY == (int)V1.Y && !bDivided)
			{
				for (int i = LeftX; i < (int)RightX; i++)
				{
					Column = i;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}
			else 
			{
				for (int i = LeftX - WireFrameThickness / 2 - 1; i < (int)LeftX + WireFrameThickness / 2; i++)
				{
					Column = i;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}

				for (int i = RightX - WireFrameThickness / 2 - 1; i < (int)RightX + WireFrameThickness / 2; i++)
				{
					Column = i;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}
		}
		ScanY += 1;
	}
}

void Render::DrawDownTriangle(Point2D V1, Point2D V2, Point2D V3, bool bDivided)
{
	//注意填充规则
	//Draw Pixels
	Point2D LeftEnd = V2.X < V3.X ? V2 : V3;
	Point2D RightEnd = V2.X > V3.X ? V2 : V3;

	//TODO:屏幕空间裁剪
	//Scane lines
	int ScanY = V1.Y;
	while (ScanY < (int)V3.Y)
	{
		float LeftX = LeftEnd.X + (ScanY - LeftEnd.Y) * (V1.X - LeftEnd.X) / (V1.Y - LeftEnd.Y);
		float RightX = RightEnd.X + (ScanY - RightEnd.Y) * (V1.X - RightEnd.X) / (V1.Y - RightEnd.Y);

		int Row = ScanY;
		int Column = 0;

		if (LeftX == RightX) 
		{
			Column = RightX;
			if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
			{
				ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
			}
		}

		if (RenderMode == ERenderMode::Shape)
		{
			for (int i = LeftX; i < (int)RightX; i++)
			{
				Column = i;
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
				}
			}
		}
		else if (RenderMode == ERenderMode::WireFrame)
		{
			if (ScanY == ((int)V3.Y - 1) && !bDivided) 
			{
				for (int i = LeftX; i < (int)RightX; i++)
				{
					Column = i;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}
			else 
			{
				for (int i = LeftX - WireFrameThickness / 2 - 1; i < (int)LeftX + WireFrameThickness / 2; i++)
				{
					Column = i;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}

				for (int i = RightX - WireFrameThickness / 2 - 1; i < (int)RightX + WireFrameThickness / 2; i++)
				{
					Column = i;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}
		}
		ScanY += 1;
	}
}

void Render::Update()
{
	//Draw back ground
	for (int i = 0; i < ViewWidth * ViewHeight; i++)
	{
		ColorBuffer[i] = 0x0000ff;
	}

	static RawBox* Box = new RawBox();
	static Camera* DefaultCamera = new Camera();
	DefaultCamera->Update();
	//TODO:遮挡剔除和背面消隐

	//Coordinate transform
	Matrix44 WroldMatrix = Matrix44::GetIdentity();
	Matrix44 CameraMatrix = DefaultCamera->GetCameraMatrix();
	Matrix44 PerspectiveMatrix = DefaultCamera->GetPerspectiveMatrix();
	
	std::vector<RawTriangle> ScreenTriangles;
	for (int i = 0; i < 12; i++) 
	{
		auto SrcTri = Box->TriList[i];
		RawTriangle OutTriangle;
		for (int j = 0; j < 3; j++) 
		{
			Vector4D SrcPoint(SrcTri.Vertexs[j].X, SrcTri.Vertexs[j].Y, SrcTri.Vertexs[j].Z);
			Vector4D OutPoint = SrcPoint * WroldMatrix * CameraMatrix * PerspectiveMatrix;
			//透视除法
			float Depth = OutPoint.W;
			OutPoint = Vector4D(OutPoint.X / OutPoint.W, OutPoint.Y / OutPoint.W, OutPoint.Z / OutPoint.W);
			//转换到屏幕空间：相机空间的y，z变换到屏幕的x，y
			float ScreenX = OutPoint.Y;
			float ScreenY = OutPoint.Z;
			DefaultCamera->TransViewToScreen(ViewWidth, ViewHeight, ScreenX, ScreenY);
			OutTriangle.Vertexs[j] = RawVertex(ScreenX, ScreenY, Depth);
		}
		ScreenTriangles.push_back(OutTriangle);
	}

	//Rasterization
	for (auto Tri : ScreenTriangles) 
	{
		Point2D V1(Tri.Vertexs[0].X, Tri.Vertexs[0].Y);
		Point2D V2(Tri.Vertexs[1].X, Tri.Vertexs[1].Y);
		Point2D V3(Tri.Vertexs[2].X, Tri.Vertexs[2].Y);

		DrawTriangle(V1, V2, V3);
	}

	//TODO:深度测试

	BitBlt(ScreenDC, 0, 0, ViewWidth, ViewHeight, BufferDC, 0, 0, SRCCOPY);

	Sleep(1);
}