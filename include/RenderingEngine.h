#pragma once

#include <string>
#include <World.h>
#include <Camera.h>
#include <ThreadUtil.h>

namespace RE
{
	class RenderingEngine
	{
	public:
		RenderingEngine();

		// Default destructor.
		virtual ~RenderingEngine() = default;

		// Initializes the rendering engine and loads any necessary components.
		// @param configFile - The filename of the configuration file to read from.
		void Initialize(const std::string& configFile);

		// Get a reference to the world to draw.
		// @param pWorld - A pointer to the world.
		void SetWorld(World* pWorld);

		// Run the ray tracer on the world.
		void RunRayTracer();

		// Unload the rendering engine components.
		void Unload();

	private:
		void RenderScene();
		void RenderScene_Single();
		void RenderScene_Multi();

		struct ThreadData
		{
			ThreadData() {}
			ThreadData(U32 startRow, U32 startCol, U32 endRow, U32 endCol) :
				startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol) {}

			U32 startRow, startCol;
			U32 endRow, endCol;
		};
		void RenderScene_ThreadFunc(U32 idx, World* pWorld, ThreadOutput* out, TaskQueue<ThreadData>* q);

	public:
		static U32 NumSamples;
		static SamplerGenerator SamplerFunc;

	private:
		ColorBuffer2D renderBuffer;
		Buffer2D<F32> depthBuffer;
		ImageTracker imageTracker;

		Camera *pCamera;
		Color bkgColor;
		Viewport vp;

		World* pWorld;

		std::string outputFolder;
		std::string outputFilename;

		bool bUseMultiThreading;
		U8 numThreads;
	};
}