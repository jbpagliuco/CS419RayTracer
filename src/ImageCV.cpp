#include <ImageCV.h>

namespace RE
{
	ImageCV::ImageCV()
	{

	}

	ImageCV::ImageCV(U32 width, U32 height)
	{
#ifndef HIDE_OPENCV
		img = cv::Mat(height, width, CV_8UC3);
#endif
	}

	ImageCV::ImageCV(U32 width, U32 height, const ColorBuffer2D* colorBuffer)
	{
#ifndef HIDE_OPENCV
		img = cv::Mat(height, width, CV_8UC4);

		for (U32 row = 0; row < height; row++)
		{
			for (U32 col = 0; col < width; col++)
			{
				ColorU8 cu8 = colorBuffer->operator()(row, col).ToColorU8();
				cv::Vec4b v(cu8.b, cu8.g, cu8.r, cu8.a);
				img.at<cv::Vec4b>(cv::Point(col, row)) = v;
			}
		}
#endif
	}

	void ImageCV::SaveToFile(const std::string& file)const
	{
#ifndef HIDE_OPENCV
		// Only png right now
		std::vector<int> params = std::vector<int>();
		params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		params.push_back(7);

		cv::imwrite(file, img, params);
#endif
	}
}