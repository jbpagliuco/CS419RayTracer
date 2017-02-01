#pragma once


// OpenCV mistakenly (hopefully) reports a lot of memory leaks, so use this 
// flag to hide opencv to test my code for leaks
//#define HIDE_OPENCV
#ifndef HIDE_OPENCV
#include <opencv2/highgui.hpp>
#endif

#include <Util.h>
#include <Buffer.h>

namespace RE
{
	class ImageCV
	{
	public:
		ImageCV();
		ImageCV(U32 width, U32 height);
		ImageCV(U32 width, U32 height, const ColorBuffer2D* colorBuffer);

		void SaveToFile(const std::string& file)const;

		virtual ~ImageCV() = default;

	private:
#ifndef HIDE_OPENCV
		cv::Mat img;
#endif
	};


	
	// Tracks an image buffer and displays it to a window
	class ImageTracker
	{
	public:
		ImageTracker();
		virtual ~ImageTracker();

		void SetBuffer(ColorBuffer2D * pBuffer);

		void Update(U32 row, U32 col);

	private:
#ifndef HIDE_OPENCV
		cv::Mat img;
#endif
		ColorBuffer2D * buffer;

		U32 lastIndex;
	};
}