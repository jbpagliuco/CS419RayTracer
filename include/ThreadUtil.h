#pragma once

#include <Util.h>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace RE
{
	template <typename T>
	class TaskQueue
	{
	public:
		TaskQueue();
		TaskQueue(U32 numWriters);

		virtual ~TaskQueue();

		void Push(const T& data);
		bool Pop(T& out);

		bool Empty();

		void WriterFinish();

	private:
		U32 numWriters;
		U32 numFinished;

		std::mutex m;
		std::condition_variable cv;
		std::vector<T> q;
	};

	class ThreadOutput
	{
	public:
		ThreadOutput();

		virtual ~ThreadOutput();

		void SetMax(U32 threadIdx, U32 max);
		void UpdateStep(U32 threadIdx, U32 step);
		void Done(U32 threadIdx);

	private:
		void Draw();
		void Clear()const;
		void DrawString(const std::string& s)const;

	private:
		std::mutex m;

		U32 width, height;
		U32 numRows, numCols;
		U32 tWidth, tHeight;

		static const U32 numThreads = 8;
		struct ThreadInfo
		{
			ThreadInfo() : step(0), max(0), bDone(false) {}
			U32 step, max;
			bool bDone;
		};
		ThreadInfo t[numThreads];
	};
}











namespace RE
{
	template <typename T>
	TaskQueue<T>::TaskQueue()
	{
	}

	template <typename T>
	TaskQueue<T>::TaskQueue(U32 numWriters) : numWriters(numWriters), numFinished(0)
	{

	}

	template <typename T>
	TaskQueue<T>::~TaskQueue()
	{

	}

	template <typename T>
	void TaskQueue<T>::Push(const T& data)
	{
		std::unique_lock<std::mutex> lock(m);
		q.push_back(data);
		lock.unlock();

		cv.notify_one();
	}

	template <typename T>
	bool TaskQueue<T>::Pop(T& out)
	{
		std::unique_lock<std::mutex> lock(m);
		while (q.size() == 0 && numFinished != numWriters)
			cv.wait(lock);

		if (q.size() == 0)
			return false;

		out = q[0];
		q.erase(q.begin(), q.begin() + 1);
		return true;
	}

	template <typename T>
	bool TaskQueue<T>::Empty()
	{
		std::unique_lock<std::mutex> lock(m);
		return q.size() == 0;
	}

	template <typename T>
	void TaskQueue<T>::WriterFinish()
	{
		std::unique_lock<std::mutex> lock(m);
		numFinished++;
		lock.unlock();

		cv.notify_one();
	}
}