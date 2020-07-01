#pragma once

class Matrix44
{
public:
	Matrix44()
	{
		memset(Data, 0, sizeof(Data));
	}

	float* operator[](size_t Index)
	{
		return Data[Index];
	}

	static Matrix44 GetIdentity()
	{
		Matrix44 Identity;
		for (int i = 0; i < 4; i++)
		{
			Identity[i][i] = 1.f;
		}
		return Identity;
	}
public:
	float Data[4][4];
};

class Vector3D 
{
public:
	Vector3D(float XIn, float YIn, float ZIn) :
		X(XIn),
		Y(YIn),
		Z(ZIn)
	{
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

	Vector4D operator* (Matrix44 M) 
	{
		return Vector4D(
			X * M[0][0] + Y * M[1][0] + Z * M[2][0] + W * M[3][0],
			X * M[0][1] + Y * M[1][1] + Z * M[2][1] + W * M[3][1],
			X * M[0][2] + Y * M[1][2] + Z * M[2][2] + W * M[3][2],
			X * M[0][3] + Y * M[1][3] + Z * M[2][3] + W * M[3][3]
		);
	}
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
public:
	float Pitch = 0.f;
	float Yaw = 0.f;
	float Roll = 0.f;
};