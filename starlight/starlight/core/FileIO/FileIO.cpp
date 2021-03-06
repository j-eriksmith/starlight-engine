/*
	Author: Jake Smith
	Date: July 2019
*/

#include "FileIO.h"
#include <iostream>

void FileIO::WaitForRequests()
{
	for (;;)
	{
		FileRequest* request = GetRequest();
		request->Fulfill();
		request->Callback(request);
		delete request;
	}
}

void FileIO::SubmitRequest(FileRequest* request)
{
	IOQueue.Enqueue(request);
}

FileRequest* FileIO::GetRequest()
{
	while (IOQueue.isEmpty());
	return IOQueue.Dequeue();
}

FileRequest::~FileRequest() {}

void FileIO::PerformRead(std::string fileName, byte* buffer, DWORD maxBytes)
{
	/*
	
	*/
}

void FileIO::PerformReadAsync(std::string fileName, byte* buffer, DWORD maxBytes, LPOVERLAPPED overlap, LPOVERLAPPED_COMPLETION_ROUTINE onFinish)
{
	HANDLE fileHandle = CreateFileA(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		//std::cout << "Opened the file!" << std::endl;
		if (ReadFileEx(fileHandle, buffer, maxBytes, overlap, onFinish))
		{
			//std::cout << "Started async read!" << buffer << std::endl;
		}
		else
		{
			std::cout << "Did not start async read!" << GetLastError() << fileName << std::endl;
		}
	}
	else
	{
		std::cout << "Did not open the file!" << GetLastError() << fileName << std::endl;
	}
	CloseHandle(fileHandle);
}

void FileIO::PerformWrite(std::string fileName, byte* buffer, DWORD bytes)
{
	HANDLE fileHandle = CreateFileA(fileName.c_str() , GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		std::cout << "Opened the file!" << fileName << std::endl;
		DWORD bytesWrote;

		if (WriteFile(fileHandle, buffer, bytes, &bytesWrote, 0))
		{
			std::cout << "Wrote to the file!" << buffer << std::endl;
		}
		else
		{
			std::cout << "Could not write to the file!" << GetLastError() << fileName << std::endl;
		}
	}
	else
	{
		std::cout << "Did not open the file!" << GetLastError() << fileName << std::endl;
	}
	CloseHandle(fileHandle);
}

// Static member initialization :(
RingBuffer<FileRequest*> FileIO::IOQueue = RingBuffer<FileRequest*>(10);
