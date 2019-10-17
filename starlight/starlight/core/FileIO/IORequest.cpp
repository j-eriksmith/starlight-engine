#include "FileIO.h"
#include <iostream>

void ReadRequest::Fulfill()
{
	HANDLE FileHandle = CreateFileA(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesRead;

		if (ReadFile(FileHandle, Buf, BufSize, &bytesRead, 0))
		{
			std::cout << "Read the file!" << Buf << std::endl;
		}
		else
		{
			std::cout << "Did not read the file!" << GetLastError() << FileName << std::endl;
		}
	}
	else
	{
		std::cout << "Did not open the file!" << GetLastError() << FileName << std::endl;
	}
	CloseHandle(FileHandle);
}

void WriteRequest::Fulfill()
{
	HANDLE fileHandle = CreateFileA(FileName.c_str() , GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWrote;

		if (WriteFile(fileHandle, Buf, BufSize, &bytesWrote, 0))
		{
			std::cout << "Wrote to the file!" << Buf << std::endl;
		}
		else
		{
			std::cout << "Could not write to the file!" << GetLastError() << FileName << std::endl;
		}
	}
	else
	{
		std::cout << "Did not open the file!" << GetLastError() << FileName << std::endl;
	}
	CloseHandle(fileHandle);
}