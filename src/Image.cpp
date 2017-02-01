#include "Image.h"
#include "Util.h"

namespace RE
{
	void LoadFreeImage()
	{
		FreeImage_Initialise();
	}

	void UnloadFreeImage()
	{
		FreeImage_DeInitialise();
	}

	


	Image::Image()
	{
		pData = nullptr;
	}

	Image::Image(int width, int height, int bpp, const float* raw)
	{
		// The size of the byte format array
		size_t pitch = width * bpp / 8;
		size_t size = pitch * height;

		// Allocate raw array for byte format
		BYTE * rawByte = new BYTE[size];

		// Convert float format to byte format
		for (size_t i = 0; i < size; i++)
			rawByte[i] = (BYTE)(raw[i] * 255.0f);

		// Create image from byte format
		pData = FreeImage_ConvertFromRawBits(rawByte, width, height, pitch, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

		// Delete byte format
		delete[] rawByte;
	}

	Image::Image(const Image& other)
	{
		Copy(other);
	}

	Image::Image(Image&& other)
	{
		pData = other.pData;

		other.pData = nullptr;
	}

	Image::~Image()
	{
		Destroy();
	}


	void Image::Copy(const Image& other)
	{
		pData = FreeImage_Clone(other.pData);
	}

	void Image::Destroy()
	{
		RE_SAFE_FREE_F(pData, FreeImage_Unload);
	}


	Image& Image::operator=(const Image& other)
	{
		if (this != &other)
		{
			Destroy();
			Copy(other);
		}

		return *this;
	}

	Image& Image::operator=(Image&& other)
	{
		if (this != &other)
		{
			Destroy();

			pData = other.pData;

			other.pData = nullptr;
		}

		return *this;
	}


	void Image::SaveToFile(const std::string& file)const
	{
		FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(file.c_str());
		FreeImage_Save(format, pData, file.c_str());
	}
}