#pragma once

#include "MathLib.h"
#include "Matrix44.h"
#include "InputManager.h"

struct RawColor 
{
	float R = 1.f;
	float G = 1.f;
	float B = 1.f;
};

struct RawVertex 
{
	RawVertex() {}
	RawVertex(float XIn, float YIn, float ZIn) : X{ XIn }, Y{ YIn }, Z{ZIn} {}

	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
};

struct RawTriangle 
{
	RawTriangle() {}
	RawTriangle(RawVertex V1, RawVertex V2, RawVertex V3){
		Vertexs[0] = V1;
		Vertexs[1] = V2;
		Vertexs[2] = V3;
	}
	RawVertex Vertexs[3];
};

class RawBox 
{
public:
	RawBox() 
	{
	}
	~RawBox() {}

public:
	Rotator Rotation{ 0,0,0 };
	Vector3D Location{ 0, 0, 0 };
	Matrix44 GetWorldMatrix() 
	{
		auto Ret = Matrix44::GetIdentity();
		Ret = Location.GetMatrix() * Rotation.GetMatrix();
		return Ret;
	}

	void Update() 
	{
		auto InputMng = InputManager::GetInstance();
		Rotation.Pitch += InputMng->DeltaMove.Y * 0.05;
		Rotation.Yaw += InputMng->DeltaMove.X * 0.05;
	}
public:
	RawTriangle TriList[12] =
	{
		//front
		{{-100, 100, 100}, {100, 100, 100}, {100, 100, -100}},
		{{-100, 100, 100}, {100, 100, -100}, {-100, 100, -100}},

		//back
		{{100, -100, 100}, {-100, -100, 100}, {-100, -100, -100}},
		{{100, -100, 100}, {-100, -100, -100}, {100, -100, -100}},

		//up
		{{-100, -100, 100}, {100, -100, 100}, {100, 100, 100}},
		{{-100, -100, 100}, {100, 100, 100}, {-100, 100, 100}},

		//down
		{{-100, -100, -100}, {-100, 100, -100}, {100, 100, -100}},
		{{-100, -100, -100}, {100, 100, -100}, {100, -100, -100}},

		//left
		{{-100, -100, 100}, {-100, 100, 100}, {-100, 100, -100}},
		{{-100, -100, 100}, {-100, 100, -100}, {-100, -100, -100}},

		//right
		{{100, 100, 100}, {100, -100, 100}, {100, -100, -100}},
		{{100, 100, 100}, {100, -100, -100}, {100, 100, -100}}
	};
};