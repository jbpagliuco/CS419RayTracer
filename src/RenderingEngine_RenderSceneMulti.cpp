#include <RenderingEngine.h>

#include <Log.h>

namespace RE
{
	std::mutex m;
	U32 numDone = 0;


	void RenderingEngine::RenderScene_ThreadFunc(U32 idx, World* pWorld, ThreadOutput* out, TaskQueue<ThreadData>* q)
	{
		// Each thread updates one row before updated the debug image.
		U32 step = 0;

		U32 width = pCamera->vp.GetWidth();
		U32 height = pCamera->vp.GetHeight();
		U32 numSamples = pCamera->vp.GetSampler()->GetNumSamples();

		U32 numRuns = height / numThreads;
		out->SetMax(idx, numRuns);

		while (true)
		{
			U32 row = step * numThreads + idx;
			if (row >= height)
				break;

			for (U32 col = 0; col < width; col++)
			{
				Color finalColor(0.0f, 0.0f, 0.0f, 1.0f);
				for (U32 s = 0; s < numSamples; s++)
				{
					Color c = pCamera->RenderPixel(row, col, pWorld, idx);
					finalColor = finalColor + c;
				}

				finalColor = finalColor / (F32)vp.GetSampler()->GetNumSamples();
				finalColor.Clamp();

				renderBuffer(row, col) = finalColor;
			}

			out->UpdateStep(idx, step);
			
			m.lock();
			ThreadData d(row, 0, row, width);
			q->Push({ row, 0, row, width });
			m.unlock();

			step++;
		}

		out->Done(idx);

		std::unique_lock<std::mutex> lock(m);
		numDone++;
		q->WriterFinish();
	}

	void RenderingEngine::RenderScene_Multi()
	{
		std::vector<std::thread> threads;
		ThreadOutput out;
		TaskQueue<ThreadData> q(numThreads);

		for (U32 i = 0; i < numThreads; i++)
			threads.push_back(std::thread(&RenderingEngine::RenderScene_ThreadFunc, this, i, pWorld, &out, &q));

		while (true)
		{
			if (q.Empty() && numDone == numThreads)
				break;

			ThreadData e;
			bool b = q.Pop(e);
			if (!b)
				break;

			imageTracker.Update(e.startRow, e.startCol, e.endRow, e.endCol);
			imageTracker.Show();
		}

		for (U32 i = 0; i < numThreads; i++)
			threads[i].join();

		std::cout << std::endl;
		system("pause");
	}
}