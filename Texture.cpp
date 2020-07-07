#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture()
{
	if (Data)
	{
		stbi_image_free(Data);
	}
}

Texture::Texture(const char* ImagePath)
{
	LoadImage(ImagePath);
}

void Texture::LoadImage(const char* ImagePath)
{
	//stbi_set_flip_vertically_on_load(true);
		//(0,0)left up
	Data = stbi_load(ImagePath, &Width, &Height, &NChannels, 0);
	if (!Data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

uint32_t Texture::GetColorNum(float Tex_x, float Tex_y)
{
	uint32_t Tx = Tex_x * Width;
	uint32_t Ty = Tex_y * Height;
	if (Data)
	{
		uint32_t R = Data[Width * Tx * NChannels + Ty * NChannels + 0];
		uint32_t G = Data[Width * Tx * NChannels + Ty * NChannels + 1];
		uint32_t B = Data[Width * Tx * NChannels + Ty * NChannels + 2];
		return (R << 16) + (G << 8) + B;
	}
	return 0xffffff;
}
