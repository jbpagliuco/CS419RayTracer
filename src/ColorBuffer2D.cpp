#include <Buffer.h>

#include <ImageCV.h>

namespace RE
{
	void ColorBuffer2D::SaveToImage(const std::string& file)const
	{
		// Create the image
		ImageCV img(width, height, this);

		// Save the image
		img.SaveToFile(file);
	}
}