#pragma once

#include "MathLib.h"
#include "Matrix44.h"
#include "InputManager.h"

struct RawColor 
{
	float R = 1.f;
	float G = 1.f;
	float B = 1.f;

	uint32_t ToColorNum() 
	{
		uint32_t R_Num = uint32_t(R * 255) << 16;
		uint32_t G_Num = uint32_t(G * 255) << 8;
		uint32_t B_Num = B * 255;
		return R_Num + G_Num + B_Num;
	}
};

struct RawVertex 
{
	RawVertex() {}
	RawVertex(float XIn, float YIn, float ZIn, float Tx, float Ty, RawColor InColor) : 
		X{ XIn }, Y{ YIn }, Z{ZIn} ,
		Tex_X{ Tx }, Tex_Y{Ty},
		Color{ InColor }
	{}

	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	
	float Tex_X = 0.0;
	float Tex_Y = 0.0;

	RawColor Color;

	static RawVertex LineLerp(const RawVertex& Low, const RawVertex& High, float Alpha) 
	{
		RawVertex DivPoint;
		DivPoint.X = MathUtils::LineLerp(Low.X, High.X, Alpha);
		DivPoint.Y = MathUtils::LineLerp(Low.Y, High.Y, Alpha);
		DivPoint.Z = 1.f / (MathUtils::LineLerp(1.f / Low.Z, 1.f / High.Z, Alpha));

		//Í¸ÊÓÐÞÕý
		DivPoint.Tex_X = MathUtils::LineLerp(Low.Tex_X / Low.Z, High.Tex_X / High.Z, Alpha) * DivPoint.Z;
		DivPoint.Tex_Y = MathUtils::LineLerp(Low.Tex_Y / Low.Z, High.Tex_Y / High.Z, Alpha) * DivPoint.Z;

		DivPoint.Color.R = MathUtils::LineLerp(Low.Color.R / Low.Z, High.Color.R / High.Z, Alpha) * DivPoint.Z;
		DivPoint.Color.G = MathUtils::LineLerp(Low.Color.G / Low.Z, High.Color.G / High.Z, Alpha) * DivPoint.Z;
		DivPoint.Color.B = MathUtils::LineLerp(Low.Color.B / Low.Z, High.Color.B / High.Z, Alpha) * DivPoint.Z;
		return DivPoint;
	}
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
		Rotation.Pitch += InputMng->DeltaMove.Y * 0.1;
		Rotation.Yaw += InputMng->DeltaMove.X * 0.1;
	}
public:
	RawVertex VertexList[8] =
	{
		{-100, 100, 100, 0, 0, {0,0,0}},
		{100, 100, 100, 0, 1, {0,0,1}},
		{-100, 100, -100, 1, 0, {0,1,0}},
		{100, 100, -100, 1,1, {0,1,1}},
		{-100, -100, 100, 0, 0, {1,0,0}},
		{100, -100, 100, 0, 1, {1,0,1}},
		{-100, -100, -100, 1, 0, {1,1,0}},
		{100, -100, -100, 1, 1, {1,1,1}}
	};
	RawTriangle TriList[12] =
	{
		//front
		{VertexList[0], VertexList[1], VertexList[3]},
		{VertexList[0], VertexList[3], VertexList[2]},

		//back
		{VertexList[5], VertexList[4], VertexList[6]},
		{VertexList[5], VertexList[6], VertexList[7]},

		//up
		{VertexList[4], VertexList[5], VertexList[1]},
		{VertexList[4], VertexList[1], VertexList[0]},

		//down
		{VertexList[6], VertexList[2], VertexList[3]},
		{VertexList[6], VertexList[3], VertexList[7]},

		//left
		{VertexList[4], VertexList[0], VertexList[2]},
		{VertexList[4], VertexList[2], VertexList[6]},

		//right
		{VertexList[1], VertexList[5], VertexList[7]},
		{VertexList[1], VertexList[7], VertexList[3]}
	};
};