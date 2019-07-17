#include <iostream>
#include <thread>
#include <ctime>
#include "math/vec3.h"
#include "math/matrix3.h"
#include "math/transform.h"
#include "FileIO/FileIO.h"
#include "MemMgr/MemMgr.h"
#include "DataStructures/RingBuffer.h"

void WaitForRequests()
{
	for (;;)
	{
		IORequest&& request = FileIO::GetRequest();

		switch (request.IOType)
		{
		case IORequest::READ:
			FileIO::PerformRead(request.FileName, request.Buf, request.BufSize);
			request.Callback(request);
			break;
		default:
			FileIO::PerformWrite(request.FileName, request.Buf, request.BufSize);
			break;
		}
	}
}

void WorkFinished(IORequest& request)
{
	// todo: This needs to get the original request back into its own hands.
	// to read back the results.
	std::cout << "Finished mah work with this: " << request.Buf << std::endl;
}

int main()
{
	/*
	Vector3::RunTests();
	Matrix3::RunTests();
	Transform::RunTests();
	*/

	// Todo: actually decide what a healthy size for this is
	MemMgr m = MemMgr(102400);
	std::thread ioThread(WaitForRequests);

	for (int i = 0; i < 1; i++)
	{
		byte* word_as_byte = reinterpret_cast<byte*>("hello");
		IORequest WriteRequest(std::string("C:\\Games\\Starlight-Engine\\starlight\\starlight\\core\\uhh.txt"),
			word_as_byte, sizeof(word_as_byte), IORequest::WRITE, WorkFinished);
		FileIO::SubmitRequest(WriteRequest);

		IORequest ReadRequest(std::string("C:\\Games\\Starlight-Engine\\starlight\\starlight\\core\\uhh.txt"),
			new byte[65536], 65536, IORequest::READ, WorkFinished);
		FileIO::SubmitRequest(ReadRequest);
	}

	// on engine shutdown:
	ioThread.join();

	return 0;
}