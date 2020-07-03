#pragma once

#define PI 3.1415926535897932f

#define TO_RADIAN(Degree) (Degree * PI / 180.f)

class Vector3D 
{
public:
	Vector3D(float XIn, float YIn, float ZIn) :
		X(XIn),
		Y(YIn),
		Z(ZIn)
	{
	}

	class Matrix44 GetMatrix();
public:
	float X;
	float Y;
	float Z;
};
typedef Vector3D Point3D;

class Vector2D 
{
public:
	Vector2D() : Vector2D(0, 0) {}
	Vector2D(float XIn, float YIn):X(XIn), Y(YIn) {}

	bool operator==(const Vector2D& Other) 
	{
		return X == Other.X && Y == Other.Y;
	}
public:
	float X;
	float Y;
};
typedef Vector2D Point2D;

class Vector4D 
{
public:
	Vector4D(float XIn, float YIn, float ZIn, float WIn = 1) :
		X(XIn),
		Y(YIn),
		Z(ZIn),
		W(WIn)
	{
	}

	Vector4D operator* (class Matrix44 M);
public:
	float X;
	float Y;
	float Z;
	float W;
};

class Rotator 
{
public:
	Rotator() {}
	Rotator(float InPitch, float InYaw, float InRoll) :  
		Pitch(InPitch),
		Yaw(InYaw),
		Roll(InRoll)
	{}

	class Matrix44 GetMatrix();
public:
	float Pitch = 0.f;
	float Yaw = 0.f;
	float Roll = 0.f;
};