#pragma once
#include <windows.h>
#include <fileapi.h>
#include <synchapi.h>

class FileIO
{
public:
	static void Read(char* fileName, char* buffer, DWORD maxBytes);
	static void ReadAsync(char* fileName, char* buffer, DWORD maxBytes, LPOVERLAPPED overlap, LPOVERLAPPED_COMPLETION_ROUTINE onFinish);
	static void Write(char* fileName, char* buffer, DWORD bytes);
};
