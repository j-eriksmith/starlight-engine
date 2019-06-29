#include <iostream>
#include <thread>
#include "math/vec3.h"
#include "math/matrix3.h"
#include "math/transform.h"
#include "FileIO/FileIO.h"
#include "DataStructures/RingBuffer.h"

void ThreadDoesThing (char* file, char* buffer, DWORD size, OVERLAPPED* o)
{
	FileIO::ReadAsync(file, buffer, size, o,
		[](DWORD e, DWORD c, OVERLAPPED * o)
		{ std::cout << "Read the damn thing" << std::endl; });
	SleepEx(2000, 1);
}

int main()
{
	/*
	Vector3::RunTests();
	Matrix3::RunTests();
	Transform::RunTests();
	*/
	/*
	const DWORD bytes = 256;
	char buffer[bytes] = { '\0' }; // prevents garbage for now
	char* file = "C:\\Games\\Starlight-Engine\\starlight\\starlight\\core\\saves.txt";
	FileIO::Read(file, buffer, sizeof(buffer));

	char message[bytes] = { "Hello World" };
	FileIO::Write(file, message, sizeof(message));

	// Reset
	FileIO::Read(file, buffer, sizeof(buffer));

	// Start async testing
	char buf2[bytes] = { '\0' }; 
	OVERLAPPED o = {};
	std::thread fileThread(ThreadDoesThing, file, buf2, sizeof(buf2), &o);
	fileThread.join();
	*/
	RingBuffer<int> rb(10);
	for (int i = 0; i < 15; i++)
	{
		rb.Enqueue(std::move(i));
	}
	for (int i = 0; i < 15; i++)
	{
		std::cout << rb.Dequeue() << std::endl;
	}
	return 0;
}