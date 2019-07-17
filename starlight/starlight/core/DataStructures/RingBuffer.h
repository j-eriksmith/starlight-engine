#pragma once
#include <utility>
#include <mutex>
#include <condition_variable>

template <typename T>
class RingBuffer 
{
public:
	inline RingBuffer(unsigned int size)
		: head(0),
		tail(0),
		capacity(size),
		full(false)
	{
		// Todo: figure out how to use the memory mger instead of this
		buffer = new T[capacity];
	}

	inline RingBuffer(const RingBuffer<T>& rhs)
		: head(rhs.head),
		tail(rhs.tail),
		capacity(rhs.capacity),
		full(rhs.full)
	{
		buffer = new T[capacity];
		for (int i = 0; i < capacity; ++i)
		{
			buffer[i] = rhs.buffer[i];
		}
	}

	inline void Enqueue(T&& item)
	{
		mutex.lock();
		if (head == tail && full)
		{
			std::cout << "Ring buffer is full!" << std::endl;
			mutex.unlock();
			return;
		}

		buffer[head] = item;
		head = Advanced(head);
		std::cout << "New head:" << head << std::endl;
	
		full = head == tail;
		mutex.unlock();
	}

	inline T&& Dequeue()
	{
		mutex.lock();
		if (isEmpty())
		{
			mutex.unlock();
			throw "Ring buffer is empty on dequeue!";
		}
		else
		{
			T&& item = std::move(buffer[tail]);
			tail = Advanced(tail);
			full = false;
			mutex.unlock();
			return std::move(item);
		}

	}

	inline bool isEmpty()
	{
		return head == tail && !full;
	}

	inline bool isFull()
	{
		return head == tail && full;
	}

	unsigned int capacity;
private:
	T* buffer;
	unsigned int head;
	unsigned int tail;
	bool full; // frees up one block for us
	std::mutex mutex;
	inline unsigned int Advanced(unsigned int i)
	{
		// should be inlined by compiler
		return (i + 1) % capacity;
	}
};

