/*
	Author: Jake Smith
	Date: July 2019
*/
#pragma once
#include <windows.h>
#include <fileapi.h>
#include <synchapi.h>
#include <string>
#include <future>
#include "ResourceMgr/Texture.h"
#include "DataStructures/RingBuffer.h"

struct FileRequest
{
	FileRequest (const std::string& file, std::function<void(FileRequest* request)> callback)
		: FileName(file), 
			Callback(callback) {}
	virtual void Fulfill() = 0; // I'd like this to be pure virtual, but then can't default construct an array
	virtual ~FileRequest();

	std::string FileName;
	std::function<void(FileRequest*)> Callback;
};

struct TextureRequest : public FileRequest
{
	// Gets data, width, height, etc. for a texture
	TextureRequest(const std::string& file, std::function<void(FileRequest* request)> callback)
		: FileRequest(file, callback),
		LoadedTexture(nullptr),
		TexturePromise() {}

	void Fulfill() override;
	Texture* LoadedTexture;
	std::promise<std::shared_ptr<Texture>> TexturePromise;
};

struct ReadRequest : public FileRequest
{
	explicit ReadRequest()
		: FileRequest("", 0),
		Buf(0),
		BufSize(0) {}

	ReadRequest(const std::string& file, byte* buf, size_t bufSize, std::function<void(FileRequest* request)> callback)
		: FileRequest(file, callback),
		Buf(buf),
		BufSize(bufSize) {}

	void Fulfill() override;

	byte* Buf;
	size_t BufSize;
};

struct WriteRequest : public FileRequest
{
	explicit WriteRequest()
		: FileRequest("", 0),
		Buf(0),
		BufSize(0) {}

	WriteRequest(const std::string& file, byte* buf, size_t bufSize, std::function<void(FileRequest* request)> callback)
		: FileRequest(file, callback),
		Buf(buf),
		BufSize(bufSize) {}

	void Fulfill() override;

	byte* Buf;
	size_t BufSize;
};

class FileIO
{
public:
	static void WaitForRequests();
	static void SubmitRequest(FileRequest* request);
	static FileRequest* GetRequest();

	// For the IO Thread to call - make it a friend class instead of these public?
	static void PerformRead(std::string fileName, byte* buffer, DWORD maxBytes);
	static void PerformReadAsync(std::string fileName, byte* buffer, DWORD maxBytes, LPOVERLAPPED overlap, LPOVERLAPPED_COMPLETION_ROUTINE onFinish);
	static void PerformWrite(std::string fileName, byte* buffer, DWORD bytes);
private:
	static RingBuffer<FileRequest*> IOQueue;
};
