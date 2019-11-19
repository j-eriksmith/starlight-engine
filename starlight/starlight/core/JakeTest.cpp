#include <iostream>
#include <thread>
#include <ctime>
#include <array>
#include "math/vec3.h"
#include "math/matrix3.h"
#include "math/transform.h"
#include "FileIO/FileIO.h"
#include "MemMgr/MemMgr.h"
#include "DataStructures/RingBuffer.h"
#include "ResourceMgr/ResourceMgr.h"
#include "Time/Clock.h"
#include "../Engine/Engine.h"

void WorkFinished(FileRequest* request)
{
	std::cout << "Finished mah work with this" << std::endl;
}

int main()
{
	// Engine Startup
	const double S_PER_UPDATE = 0.0167;
	Clock startupClock;
	MemMgr m = MemMgr(102400);
	ResourceMgr rm;
	Engine e;
	std::thread ioThread(FileIO::WaitForRequests);

	// Fixed Timestep Game Loop
	double LastLoopTime = startupClock.GetTimeSinceStartup();
	double AccumulatedLag = 0.0;

	for (;;)
	{
		static std::vector<int> test;
		double CurrentLoopTime = startupClock.GetTimeSinceStartup();
		double DeltaTime =  CurrentLoopTime - LastLoopTime;
		LastLoopTime = CurrentLoopTime;
		AccumulatedLag += DeltaTime;

		while (AccumulatedLag >= S_PER_UPDATE)
		{
			/* Imaginary work
			for (int i = 0; i < 1000000; ++i)
			{
				test.push_back(100000);
			}
			*/
			e.Update();
			AccumulatedLag -= S_PER_UPDATE;
		}
		// test.clear();
	}

	return 0;
}

void EngineUpdate()
{
	// Determine usual system order update for ECS

}

int TextureMain()
{
	// Engine Startup

	MemMgr m = MemMgr(102400);
	ResourceMgr rm;
	std::thread ioThread(FileIO::WaitForRequests);
	std::array<std::shared_ptr<Texture>, 100> TextureArray;

	// Imaginary work 

	for (int i = 0; i < 100; i++)
	{
		/* Read/Write Testing
		byte* word_as_byte = reinterpret_cast<byte*>("hello");
		WriteRequest* Write = new WriteRequest(Resources::Get("uhh.txt"), word_as_byte, sizeof(word_as_byte), WorkFinished);
		FileIO::SubmitRequest(Write);

		ReadRequest* Read = new ReadRequest(Resources::Get("uhh.txt"), new byte[65536], 65536, WorkFinished);
		FileIO::SubmitRequest(Read);
		*/

		/*
		auto Future = rm.Load(Resources::Get("grass_texture.jpg"));
		std::shared_ptr<Texture> Texture = Future.get();
		GrassArray[i] = Texture;
		*/
	}

	auto GrassFuture = rm.Load("grass_texture.jpg");
	TextureArray[0] = GrassFuture.get();
	auto SquareFuture = rm.Load("square.png");
	rm.Unload("grass_texture.jpg");
	TextureArray[1] = SquareFuture.get();
	auto StoneFuture = rm.Load("stone.png");
	TextureArray[2] = StoneFuture.get();
	rm.Unload("stone.png");
	rm.Unload("square.png");

	// Engine Shutdown 
	ioThread.join();

	return 0;
}