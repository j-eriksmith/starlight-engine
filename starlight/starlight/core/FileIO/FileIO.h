#pragma once
#include <windows.h>
#include <fileapi.h>
#include <synchapi.h>
#include <string>
#include "../DataStructures/RingBuffer.h"

struct IORequest
{
	enum Type { READ, WRITE };
	explicit IORequest()
		: FileName(""), 
			Buf(0),
			BufSize(0),
			IOType(Type::READ),
			Callback(0) {}
	IORequest(std::string&& file, byte* buf, size_t bufSize, Type type, void (*callback)(IORequest& request))
		: FileName(file), 
			Buf(buf),
			BufSize(bufSize),
			IOType(type),
			Callback(callback) {}
	std::string FileName;
	byte* Buf;
	size_t BufSize;
	Type IOType;
	void (*Callback)(IORequest& request);
};

class FileIO
{
public:
	static void SubmitRequest(IORequest& request);
	static IORequest&& GetRequest();
	// For the IO Thread to call - make it a friend class?
	static void PerformRead(std::string fileName, byte* buffer, DWORD maxBytes);
	static void PerformReadAsync(std::string fileName, byte* buffer, DWORD maxBytes, LPOVERLAPPED overlap, LPOVERLAPPED_COMPLETION_ROUTINE onFinish);
	static void PerformWrite(std::string fileName, byte* buffer, DWORD bytes);
private:
	static RingBuffer<IORequest> IOQueue;
};
