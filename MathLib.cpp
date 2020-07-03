#include "MathLib.h"
#include "Matrix44.h"

Vector4D Vector4D::operator* (Matrix44 M)
{
	return Vector4D(
		X * M[0][0] + Y * M[1][0] + Z * M[2][0] + W * M[3][0],
		X * M[0][1] + Y * M[1][1] + Z * M[2][1] + W * M[3][1],
		X * M[0][2] + Y * M[1][2] + Z * M[2][2] + W * M[3][2],
		X * M[0][3] + Y * M[1][3] + Z * M[2][3] + W * M[3][3]
	);
}

Matrix44 Rotator::GetMatrix()
{
	//Yaw:Z axis
	Matrix44 YawMat = Matrix44::GetIdentity();
	YawMat[0][0] = cos(TO_RADIAN(-Yaw));
	YawMat[0][1] = sin(TO_RADIAN(-Yaw));
	YawMat[1][0] = -sin(TO_RADIAN(-Yaw));
	YawMat[1][1] = cos(TO_RADIAN(-Yaw));
	//Pitch:Y axis
	Matrix44 PitchMat = Matrix44::GetIdentity();
	PitchMat[0][0] = cos(TO_RADIAN(-Pitch));
	PitchMat[0][2] = -sin(TO_RADIAN(-Pitch));
	PitchMat[2][0] = sin(TO_RADIAN(-Pitch));
	PitchMat[2][2] = cos(TO_RADIAN(-Pitch));
	//Roll:X axis
	Matrix44 RollMat = Matrix44::GetIdentity();
	RollMat[1][1] = cos(TO_RADIAN(-Roll));
	RollMat[1][2] = sin(TO_RADIAN(-Roll));
	RollMat[2][1] = -sin(TO_RADIAN(-Roll));
	RollMat[2][2] = cos(TO_RADIAN(-Roll));

	return YawMat * PitchMat * RollMat;
}

Matrix44 Vector3D::GetMatrix()
{
	Matrix44 Ret = Matrix44::GetIdentity();
	Ret[3][0] = X;
	Ret[3][1] = Y;
	Ret[3][2] = Z;
	return Ret;
}
