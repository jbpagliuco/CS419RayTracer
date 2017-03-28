#include <ThreadUtil.h>

#include <Windows.h>
#include <iostream>
#include <sstream>

namespace RE
{
	ThreadOutput::ThreadOutput()
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		width = info.dwMaximumWindowSize.X;
		height = info.dwMaximumWindowSize.Y;

		numCols = numThreads >> 1;
		numRows = numThreads / numCols;
		tWidth = width / numCols;
		tHeight = height / numRows;

		Clear();
	}

	ThreadOutput::~ThreadOutput()
	{

	}

	void ThreadOutput::SetMax(U32 threadIdx, U32 max)
	{
		t[threadIdx].max = max;
	}

	void ThreadOutput::UpdateStep(U32 threadIdx, U32 step)
	{
		t[threadIdx].step = step;
		Draw();
	}

	void ThreadOutput::Done(U32 threadIdx)
	{
		t[threadIdx].bDone = true;
		Draw();
	}

	void ThreadOutput::Draw()
	{
		std::unique_lock<std::mutex> lock(m);

		Clear();

		for (U32 row = 0; row < numRows; row++)
		{
			// Thread #
			for (U32 col = 0; col < numCols; col++)
			{
				U32 idx = row * numCols + col;

				std::stringstream ss;
				ss << "[Thread " << idx << "]:";
				DrawString(ss.str());
			}

			// Step
			for (U32 col = 0; col < numCols; col++)
			{
				U32 idx = row * numCols + col;

				std::stringstream ss;
				ss << t[idx].step << " of " << t[idx].max;
				DrawString(ss.str());
			}

			// Done
			for (U32 col = 0; col < numCols; col++)
			{
				U32 idx = row * numCols + col;
				if (t[idx].bDone)
					DrawString("Done.");
				else
					DrawString("");
			}

			// Empty lines
			for (U32 i = 0; i < 7; i++)
			{
				for (U32 col = 0; col < numCols; col++)
				{
					DrawString("");
				}
			}

			// Separator
			for (U32 i = 0; i < width; i++)
				std::cout << "-";
		}
	}

	void ThreadOutput::Clear()const
	{
		//system("cls");
		COORD c = { 0,0 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}

	void ThreadOutput::DrawString(const std::string& s)const
	{
		std::cout << s;
		for (I32 i = (I32)s.length(); i < (I32)tWidth - 1; i++)
			std::cout << " ";
		std::cout << "|";
	}
}