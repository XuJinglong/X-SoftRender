#pragma once


class InputManager 
{
public:
	InputManager() {}
	~InputManager() {}

	static InputManager* GetInstance() 
	{
		static InputManager* Instance = new InputManager();
		return Instance;
	}

public:
	float FrontDelta = 0.f;
	float RightDelta = 0.f;
	float UpDelta = 0.f;

	Vector2D LastMousePos;
	Vector2D DeltaMove;
	bool bStartTouchMove = false;
};