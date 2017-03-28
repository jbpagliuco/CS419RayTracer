#pragma once

// OpenCV reports a lot of memory leaks, so use this 
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
		// Creates an empty image.
		// @param width - The width of the image.
		// @param height - The height of the image.
		ImageCV(U32 width, U32 height);

		// Creates an image and initializes it to the buffer.
		// @param width - The width of the image.
		// @param height - The height of the image.
		// @param colorBuffer - The color buffer.
		ImageCV(U32 width, U32 height, const ColorBuffer2D* colorBuffer);

		ImageCV(const std::string& filename);

		virtual ~ImageCV() = default;

		virtual Color GetColor(U32 row, U32 col)const;

		U32 Width()const;
		U32 Height()const;

		// Saves this image to a file on disk.
		// @param file - The filename.
		void SaveToFile(const std::string& file)const;

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
		void Update(U32 startRow, U32 startCol, U32 endRow, U32 endCol);
		void Show();

	private:
#ifndef HIDE_OPENCV
		cv::Mat img;
#endif
		ColorBuffer2D * buffer;

		U32 lastIndex;
	};
}