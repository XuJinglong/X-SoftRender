#pragma once

#define PI 3.1415926535897932f

#define TO_RADIAN(Degree) (Degree * PI / 180.f)

#include <math.h>

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

	float Length() 
	{
		return sqrtf(X*X + Y*Y + Z*Z);
	}

	Vector3D Normalize() 
	{
		float Size = Length();
		return Vector3D(X/Size, Y/Size, Z/Size);
	}

	Vector3D operator- (const Vector3D& Other) const
	{
		return Vector3D(X - Other.X, Y - Other.Y, Z - Other.Z);
	}

	Vector3D operator- () const
	{
		return Vector3D(-X, -Y, -Z);
	}

	Vector3D operator+ (const Vector3D& Other) const
	{
		return Vector3D(X + Other.X, Y + Other.Y, Z + Other.Z);
	}

	float Dot(const Vector3D& Other) const
	{
		return X * Other.X + Y * Other.Y + Z * Other.Z;
	}

	Vector3D Cross(const Vector3D& Other) const
	{
		return Vector3D(
			Y * Other.Z - Other.Y * Z,
			Other.X * Z - X * Other.Z,
			X * Other.Y - Other.X * Y
		);
	}

	Vector3D operator* (float Num) const
	{
		return Vector3D(X * Num, Y * Num, Z * Num);
	}
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
	class Matrix44 GetInverseMatrix();
public:
	float Pitch = 0.f;
	float Yaw = 0.f;
	float Roll = 0.f;
};

class MathUtils 
{
public:
	static float LineLerp(float Low, float High, float Alpha) 
	{
		Alpha = Alpha < 0 ? 0 : Alpha > 1 ? 1 : Alpha;
		return Low + Alpha * (High - Low);
	}

	static float Clamp(float InVar, float Min, float Max) 
	{
		return InVar < Min ? Min : InVar > Max ? Max : InVar;
	}
};