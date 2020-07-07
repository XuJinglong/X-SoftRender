#pragma once

#include <iostream>

class Texture 
{
public:
	Texture() {}
	~Texture();

	Texture(const char* ImagePath);

public:
	void LoadImage(const char* ImagePath);

	uint32_t GetColorNum(float Tex_x, float Tex_y);

private:
	int Width = 0;
	int Height = 0;
	int NChannels = 0;
	unsigned char* Data = nullptr;
};