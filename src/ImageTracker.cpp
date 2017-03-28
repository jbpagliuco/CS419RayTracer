#include <ImageCV.h>

#define WINDOW_NAME "DEBUG DISPLAY"

namespace RE
{
	ImageTracker::ImageTracker()
	{
		buffer = nullptr;

		lastIndex = 0;

#ifndef HIDE_OPENCV
		cv::namedWindow(WINDOW_NAME);
#endif
	}

	ImageTracker::~ImageTracker()
	{
#ifndef HIDE_OPENCV
		cv::destroyWindow(WINDOW_NAME);
#endif
	}

	void ImageTracker::SetBuffer(ColorBuffer2D * pBuffer)
	{
		buffer = pBuffer;

#ifndef HIDE_OPENCV
		img = cv::Mat(buffer->GetHeight(), buffer->GetWidth(), CV_8UC4, cv::Scalar(0, 0, 0, 255));
#endif
	}


	void ImageTracker::Update(U32 startRow, U32 startCol, U32 endRow, U32 endCol)
	{
		assert(buffer);

		U32 width = buffer->GetWidth();
		I32 startIndex = startRow * width + startCol;
		I32 endIndex = endRow * width + endCol;
		for (I32 i = startIndex; i < endIndex; i++)
		{
			ColorU8 cu8 = buffer->operator()(i).ToColorU8();
#ifndef HIDE_OPENCV
			cv::Vec4b v(cu8.b, cu8.g, cu8.r, cu8.a);

			U32 r = i / width;
			U32 c = i % width;
			img.at<cv::Vec4b>(cv::Point(c, r)) = v;
#endif
		}
	}


	void ImageTracker::Update(U32 row, U32 col)
	{
		assert(buffer);

		U32 width = buffer->GetWidth();
		I32 newIndex = row * width + col;
		for (I32 i = lastIndex; i <= newIndex; i++)
		{
			ColorU8 cu8 = buffer->operator()(i).ToColorU8();
#ifndef HIDE_OPENCV
			cv::Vec4b v(cu8.b, cu8.g, cu8.r, cu8.a);

			U32 r = i / width;
			U32 c = i % width;
			img.at<cv::Vec4b>(cv::Point(c, r)) = v;
#endif
		}
		lastIndex = newIndex;

#ifndef HIDE_OPENCV
		cv::imshow(WINDOW_NAME, img);
		cv::waitKey(1);
#endif
	}






	void ImageTracker::Show()
	{
#ifndef HIDE_OPENCV
		cv::imshow(WINDOW_NAME, img);
		cv::waitKey(1);
#endif
	}
}