#pragma once

#include "MathLib.h"
#include <math.h>

class Camera 
{
public:
	Camera() {}
	~Camera() {}

	Matrix44 GetPerspectiveMatrix()
	{
		Matrix44 Ret = Matrix44::GetIdentity();
		Ret[0][3] = 1.f / ViewDistance;
		return Ret;
	}

	Matrix44 GetCameraMatrix() 
	{
		//TODO:Rotaton
		Matrix44 Ret = Matrix44::GetIdentity();
		Ret[3][0] = Location.X;
		Ret[3][1] = Location.Y;
		Ret[3][2] = Location.Z;
		return Ret;
	}

	void TransViewToScreen(float ScreenWidth, float ScreenHeight, float& ScreenX, float& ScreenY)
	{
		float HalfViewHeight = ViewDistance * tan(FOV / 2 / 57.3);
		float HalfViewWidth = HalfViewHeight * WidthHeightRatio;
		
		ScreenX = (ScreenX + HalfViewWidth ) * ScreenWidth / (HalfViewHeight * 2);
		ScreenY = (ScreenY + HalfViewHeight) * ScreenHeight / (HalfViewHeight * 2);
	}

public:
	float FOV = 90.f;//Along Z axis, Height
	float ViewDistance = 1.f;
	float WidthHeightRatio = 1.f;
	float NearPlane = 1.f;
	float FarPlane = 3000.f;


	Vector3D Location{-400, 0, 150};
	Rotator Rotation;
};