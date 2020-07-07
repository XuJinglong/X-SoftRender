#pragma once

#include "MathLib.h"
#include <math.h>
#include "InputManager.h"
#include "Matrix44.h"

class Camera 
{
public:
	Camera() {}
	~Camera() {}

	void Update() 
	{
		auto InputMng = InputManager::GetInstance();
		Location.X += InputMng->FrontDelta * ControlMoveSpeed;
		Location.Y += InputMng->RightDelta * ControlMoveSpeed;
		Location.Z += InputMng->UpDelta * ControlMoveSpeed;

		/*Rotation.Pitch += InputMng->DeltaMove.Y * ControlRotSpeed;
		Rotation.Yaw += InputMng->DeltaMove.X * ControlRotSpeed;*/
	}

	Matrix44 GetPerspectiveMatrix()
	{
		Matrix44 Ret = Matrix44::GetIdentity();
		Ret[0][3] = 1.f / ViewDistance;
		Ret[3][3] = 0.f;
		return Ret;
	}

	Matrix44 GetCameraMatrix() 
	{
		Matrix44 Ret = Matrix44::GetIdentity();
		Ret[3][0] = -Location.X;
		Ret[3][1] = -Location.Y;
		Ret[3][2] = -Location.Z;

		Ret = Ret * Rotation.GetMatrix();
		return Ret;
	}

	void TransViewToScreen(float ScreenWidth, float ScreenHeight, float& ScreenX, float& ScreenY)
	{
		float HalfViewHeight = ViewDistance * tan(TO_RADIAN(FOV / 2));
		float HalfViewWidth = HalfViewHeight * WidthHeightRatio;
		
		ScreenX = (ScreenX + HalfViewWidth ) * ScreenWidth / (HalfViewHeight * 2);
		ScreenY = (ScreenY + HalfViewHeight) * ScreenHeight / (HalfViewHeight * 2);
	}

public:
	float FOV = 90.f;//Along Z axis, Height
	float ViewDistance = 1.f;
	float WidthHeightRatio = 1.f;
	float NearPlane = 10.f;
	float FarPlane = 3000.f;


	Vector3D Location{-500, 0, 0};
	Rotator Rotation{0, 0, 0};

	float ControlMoveSpeed = 10.f;
	float ControlRotSpeed = 0.02f;
};