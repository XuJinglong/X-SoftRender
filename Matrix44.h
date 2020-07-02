#pragma once

#include "MathLib.h"
#include <string>

class Matrix44
{
public:
	Matrix44()
	{
		memset(Data, 0, sizeof(Data));
	}

	Matrix44(Vector3D Location, Rotator Rotation)
	{
		memset(Data, 0, sizeof(Data));
		for (int i = 0; i < 4; i++)
		{
			Data[i][i] = 1.f;
		}
		Data[3][0] = Location.X;
		Data[3][1] = Location.Y;
		Data[3][2] = Location.Z;
	}

	float* operator[](size_t Index)
	{
		return Data[Index];
	}

	Matrix44 operator*(Matrix44 Other) 
	{
		Matrix44 Ret;
		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 4; j++) 
			{
				Ret[i][j] = Data[i][0] * Other[0][j] + Data[i][1] * Other[1][j] + Data[i][2] * Other[2][j] + Data[i][3] * Other[3][j];
			}
		}
		return Ret;
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