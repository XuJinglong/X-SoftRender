#include "Render.h"
#include "Camera.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "Timer.h"

Render::Render()
{
	ZBuffer = new float[ViewHeight * ViewWidth];
	Image = new Texture("./X-SoftRender/Resources/container.jpg");
}

Render::~Render()
{
	delete[] ZBuffer;
	delete Image;
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

void Render::RayTracingDrawTriangles(Camera* Camera, RawBox* DrawBox)
{
	Timer FrameTimer;
	FrameTimer.Start();
	Matrix44 WroldMatrix = DrawBox->GetWorldMatrix();
	for (int i = 0; i < 12; i++) 
	{
		const RawTriangle& Tri = DrawBox->TriList[i];
		for (int j = 0; j < 3; j++) 
		{
			const RawVertex& V = Tri.Vertexs[j];
			Vector4D WorldPoint = Vector4D(V.X, V.Y, V.Z) * WroldMatrix;

			DrawBox->TransTriList[i].Vertexs[j].X = WorldPoint.X;
			DrawBox->TransTriList[i].Vertexs[j].Y = WorldPoint.Y;
			DrawBox->TransTriList[i].Vertexs[j].Z = WorldPoint.Z;
		}
	}
	for (int i = 0; i < ViewHeight; i++) 
	{
		for (int j = 0; j < ViewWidth; j++) 
		{
			Vector3D TraceStart = Camera->Location;
			//屏幕交点从屏幕空间转换为相机空间视平面上的点
			Vector3D ScreenPoint = Camera->TransScreenToView(ViewWidth, ViewHeight, j + 0.5, i + 0.5);
			//相机空间变为世界坐标系
			Vector4D IntersectPoint4D = Vector4D(ScreenPoint.X, ScreenPoint.Y, ScreenPoint.Z) * Camera->GetCameraInverseMatrix();
			Vector3D Direction = Vector3D(IntersectPoint4D.X - TraceStart.X, IntersectPoint4D.Y - TraceStart.Y, IntersectPoint4D.Z - TraceStart.Z).Normalize();

			for (int t = 0; t < 12; t++)
			{
				RawTriangle& Tri = DrawBox->TransTriList[t];

				RawVertex RayCastV;
				bool bTrace = GetRayCastPoint(Tri.Vertexs[0], Tri.Vertexs[1], Tri.Vertexs[2], TraceStart, Direction, RayCastV);
				if (bTrace && RayCastV.Z < ZBuffer[i * ViewWidth + j])
				{
					if (RenderMode == ERenderMode::Color)
					{
						ColorBuffer[i * ViewWidth + j] = RayCastV.Color.ToColorNum();
					}
					else if (RenderMode == ERenderMode::Texture)
					{
						ColorBuffer[i * ViewWidth + j] = Image->GetColorNum(RayCastV.Tex_X, RayCastV.Tex_Y);
					}
					ZBuffer[i * ViewWidth + j] = RayCastV.Z;
				}
			}
		}
	}
	FrameTimer.End();
}

void Render::DrawTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3)
{
	std::vector<RawVertex> OrderPoints = {V1, V2, V3};
	std::sort(OrderPoints.begin(), OrderPoints.end(), [](const RawVertex& P1, const RawVertex& P2) {return P1.Y < P2.Y; });
	RawVertex Low = OrderPoints[0];
	RawVertex Mid = OrderPoints[1];
	RawVertex High = OrderPoints[2];
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
		float Alpha = (Mid.Y - Low.Y) / (High.Y - Low.Y);
		RawVertex DivPoint = RawVertex::LineLerp(Low, High, Alpha);

		DrawUpTriangle(DivPoint, Mid, High, true);
		DrawDownTriangle(Low, Mid, DivPoint, true);
	}
}

void Render::DrawUpTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, bool bDivided)
{
	//注意填充规则
	//统一下加上取整，避免毛刺、缺线、多线、重线
	//Draw Pixels
	RawVertex LeftEnd = V1.X < V2.X ? V1 : V2;
	RawVertex RightEnd = V1.X > V2.X ? V1 : V2;

	//TODO:屏幕空间裁剪
	//Scane lines
	float LastLeftX = LeftEnd.X;
	float LastRightX = RightEnd.X;
	int ScanY = V1.Y + 1;
	while (ScanY < (int)(V3.Y + 1))
	{
		float Alpha = (ScanY - LeftEnd.Y) / (V3.Y - LeftEnd.Y);
		RawVertex LeftVex = RawVertex::LineLerp(LeftEnd, V3, Alpha);
		RawVertex RightVex = RawVertex::LineLerp(RightEnd, V3, Alpha);

		float LeftX = LeftVex.X;
		float RightX = RightVex.X;

		if (LeftX == RightX) return;

		int Row = ScanY;
		int Column = 0;

		if (RenderMode == ERenderMode::Texture || RenderMode == ERenderMode::Color)
		{
			for (int i = LeftX; i < (int)RightX; i++) 
			{
				Column = i;
				float AlphaX = (i - LeftX) / (RightX - LeftX);
				RawVertex XLerpVex = RawVertex::LineLerp(LeftVex, RightVex, AlphaX);
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					if (ZBuffer[Row * ViewWidth + Column] >= XLerpVex.Z) 
					{
						if (RenderMode == ERenderMode::Texture)
						{
							ColorBuffer[Row * ViewWidth + Column] = Image->GetColorNum(XLerpVex.Tex_X, XLerpVex.Tex_Y);
						}
						else 
						{
							ColorBuffer[Row * ViewWidth + Column] = XLerpVex.Color.ToColorNum();
						}
						ZBuffer[Row * ViewWidth + Column] = XLerpVex.Z;
					}
				}
			}
		}
		else if (RenderMode == ERenderMode::WireFrame)
		{
			if (ScanY == (int)(V1.Y + 1) && !bDivided)
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

			float Min = min(LastLeftX, LeftX);
			float Max = max(LastLeftX, LeftX);

			for (int i = Min + 1; i < Max; i++)
			{
				for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
				{
					Column = j;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}

			Min = min(LastRightX, RightX);
			Max = max(LastRightX, RightX);
			for (int i = Min + 1; i < Max; i++)
			{
				for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
				{
					Column = j;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}
		}
		LastLeftX = LeftX;
		LastRightX = RightX;
		ScanY += 1;
	}

	//补线
	if (RenderMode == ERenderMode::WireFrame) 
	{
		float Min = min(LastLeftX, V3.X);
		float Max = max(LastLeftX, V3.X);
		int Row = ScanY;
		for (int i = Min + 1; i < Max; i++)
		{
			for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
			{
				int Column = j;
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
				}
			}
		}

		Min = min(LastRightX, V3.X);
		Max = max(LastRightX, V3.X);
		for (int i = Min + 1; i < Max; i++)
		{
			for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
			{
				int Column = j;
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
				}
			}
		}
	}
}

void Render::DrawDownTriangle(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, bool bDivided)
{
	//注意填充规则
	//Draw Pixels
	RawVertex LeftEnd = V2.X < V3.X ? V2 : V3;
	RawVertex RightEnd = V2.X > V3.X ? V2 : V3;

	//TODO:屏幕空间裁剪
	//Scane lines
	float LastLeftX = V1.X;
	float LastRightX = V1.X;
	int ScanY = V1.Y + 1;
	while (ScanY < int(V3.Y + 1))
	{
		float Alpha = (ScanY - LeftEnd.Y) / (V1.Y - LeftEnd.Y);
		RawVertex LeftVex = RawVertex::LineLerp(LeftEnd, V1, Alpha);
		RawVertex RightVex = RawVertex::LineLerp(RightEnd, V1, Alpha);

		float LeftX = LeftVex.X;
		float RightX = RightVex.X;

		int Row = ScanY;
		int Column = 0;

		/*if (LeftX == RightX) 
		{
			Column = RightX;
			if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
			{
				if (ZBuffer[Row * ViewWidth + Column] >= LeftVex.Z)
				{
					if (RenderMode == ERenderMode::Texture)
					{
						ColorBuffer[Row * ViewWidth + Column] = Image->GetColorNum(LeftVex.Tex_X, LeftVex.Tex_Y);
					}
					else
					{
						ColorBuffer[Row * ViewWidth + Column] = LeftVex.Color.ToColorNum();
					}
					ZBuffer[Row * ViewWidth + Column] = LeftVex.Z;
				}
			}
		}*/

		if (RenderMode == ERenderMode::Texture || RenderMode == ERenderMode::Color)
		{
			for (int i = LeftX; i < (int)RightX; i++)
			{
				Column = i;
				float AlphaX = (i - LeftX) / (RightX - LeftX);
				RawVertex XLerpVex = RawVertex::LineLerp(LeftVex, RightVex, AlphaX);
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					if (ZBuffer[Row * ViewWidth + Column] >= XLerpVex.Z)
					{
						if (RenderMode == ERenderMode::Texture)
						{
							ColorBuffer[Row * ViewWidth + Column] = Image->GetColorNum(XLerpVex.Tex_X, XLerpVex.Tex_Y);
						}
						else
						{
							ColorBuffer[Row * ViewWidth + Column] = XLerpVex.Color.ToColorNum();
						}
						ZBuffer[Row * ViewWidth + Column] = XLerpVex.Z;
					}
				}
			}
		}
		else if (RenderMode == ERenderMode::WireFrame)
		{
			if (ScanY == (int)V3.Y && !bDivided) 
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

			float Min = min(LastLeftX, LeftX);
			float Max = max(LastLeftX, LeftX);

			for (int i = Min + 1; i < Max; i++)
			{
				for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
				{
					Column = j;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}

			Min = min(LastRightX, RightX);
			Max = max(LastRightX, RightX);
			for (int i = Min + 1; i < Max; i++)
			{
				for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
				{
					Column = j;
					if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
					{
						ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
					}
				}
			}
		}
		LastLeftX = LeftX;
		LastRightX = RightX;
		ScanY += 1;
	}

	if (RenderMode == ERenderMode::WireFrame) 
	{
		float Min = min(LastLeftX, LeftEnd.X);
		float Max = max(LastLeftX, LeftEnd.X);
		int Row = ScanY;
		for (int i = Min + 1; i < Max; i++)
		{
			for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
			{
				int Column = j;
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
				}
			}
		}

		Min = min(LastRightX, RightEnd.X);
		Max = max(LastRightX, RightEnd.X);
		for (int i = Min + 1; i < Max; i++)
		{
			for (int j = i - WireFrameThickness / 2 - 1; j < i + WireFrameThickness / 2; j++)
			{
				int Column = j;
				if (Row >= 0 && Row < ViewHeight && Column > 0 && Column < ViewWidth)
				{
					ColorBuffer[Row * ViewWidth + Column] = 0xffffff;
				}
			}
		}
	}
}

bool Render::GetRayCastPoint(const RawVertex& V1, const RawVertex& V2, const RawVertex& V3, const Vector3D& TraceStart, const Vector3D& Direction, RawVertex& RayCastV)
{
	//计算交点
	//E12
	//E13
	//https://www.cnblogs.com/graphics/archive/2010/08/09/1795348.html
	float t = -1.f;
	float u = -1.f;
	float v = -1.f;
	Point3D P1(V1.X, V1.Y, V1.Z);
	Point3D P2(V2.X, V2.Y, V2.Z);
	Point3D P3(V3.X, V3.Y, V3.Z);
	Vector3D E1 = P2 - P1;
	Vector3D E2 = P3 - P1;
	Vector3D D = Direction;
	Vector3D T = TraceStart - P1;
	Vector3D P = D.Cross(E2);
	Vector3D Q = T.Cross(E1);
	float Denominator = P.Dot(E1);
	if (abs(Denominator) < 0.0001f) 
	{
		return false;
	}
	t = Q.Dot(E2) / Denominator;
	u = P.Dot(T) / Denominator;
	v = Q.Dot(D) / Denominator;

	bool bTrace = t > 0.f && u >= 0.f && u <= 1.f && v >= 0.f && (u + v) <= 1.f;
	if (bTrace) 
	{
		Vector3D TracePoint = P1 * (1 - u - v) + P2 * u + P3 * v;
		RayCastV.X = TracePoint.X;
		RayCastV.Y = TracePoint.Y;
		RayCastV.Z = (TracePoint - TraceStart).Length();
		RayCastV.Color = V1.Color * (1 - u - v) + V2.Color * u + V3.Color * v;
		RayCastV.Tex_X = V1.Tex_X * (1 - u - v) + V2.Tex_X * u + V3.Tex_X * v;
		RayCastV.Tex_Y = V1.Tex_Y * (1 - u - v) + V2.Tex_Y * u + V3.Tex_Y * v;
	}

	return bTrace;
}

void Render::Update()
{
	//Draw back ground
	for (int i = 0; i < ViewWidth * ViewHeight; i++)
	{
		ColorBuffer[i] = 0x334c4c;
		ZBuffer[i] = 1000000.f;
	}

	static RawBox* Box = new RawBox();
	static Camera* DefaultCamera = new Camera();
	Box->Update();
	DefaultCamera->Update();

	if (RenderStyle == ERenderStyle::Front)
	{
		//TODO:遮挡剔除和背面消隐
		//Coordinate transform
		Matrix44 WroldMatrix = Box->GetWorldMatrix();
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
				OutTriangle.Vertexs[j] = RawVertex(ScreenX, ScreenY, Depth, SrcTri.Vertexs[j].Tex_X, SrcTri.Vertexs[j].Tex_Y, SrcTri.Vertexs[j].Color);
			}
			ScreenTriangles.push_back(OutTriangle);
		}

		//Rasterization
		for (auto Tri : ScreenTriangles)
		{
			DrawTriangle(Tri.Vertexs[0], Tri.Vertexs[1], Tri.Vertexs[2]);
		}
	}
	else if (RenderStyle == ERenderStyle::RayTrace)
	{
		RayTracingDrawTriangles(DefaultCamera, Box);
	}


	BitBlt(ScreenDC, 0, 0, ViewWidth, ViewHeight, BufferDC, 0, 0, SRCCOPY);

	Sleep(1);
}