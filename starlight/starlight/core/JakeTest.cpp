#include <iostream>
#include <thread>
#include <ctime>
#include "math/vec3.h"
#include "math/matrix3.h"
#include "math/transform.h"
#include "FileIO/FileIO.h"
#include "MemMgr/MemMgr.h"
#include "DataStructures/RingBuffer.h"

void WorkFinished(IORequest& request)
{
	std::cout << "Finished mah work with this: " << request.Buf << std::endl;
}

int main()
{
	// Engine Startup

	// Todo: actually decide what a healthy size for this is
	MemMgr m = MemMgr(102400);
	std::thread ioThread(FileIO::WaitForRequests);

	// Imaginary work 

	for (int i = 0; i < 10; i++)
	{
		byte* word_as_byte = reinterpret_cast<byte*>("hello");
		IORequest WriteRequest(std::string("C:\\Games\\Starlight-Engine\\starlight\\starlight\\core\\uhh.txt"),
			word_as_byte, sizeof(word_as_byte), IORequest::WRITE, WorkFinished);
		FileIO::SubmitRequest(WriteRequest);

		IORequest ReadRequest(std::string("C:\\Games\\Starlight-Engine\\starlight\\starlight\\core\\uhh.txt"),
			new byte[65536], 65536, IORequest::READ, WorkFinished);
		FileIO::SubmitRequest(ReadRequest);
	}

	// Engine Shutdown 
	ioThread.join();

	return 0;
}