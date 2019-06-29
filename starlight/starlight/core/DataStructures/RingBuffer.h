#pragma once
#include <utility>

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
		buffer = new T[size];
	}

	inline void Enqueue(T&& item)
	{
		if (head == tail && full)
		{
			std::cout << "Ring buffer is full!" << std::endl;
			return;
		}

		buffer[head] = item;
		head = Advanced(head);
		std::cout << "New head:" << head << std::endl;
	
		full = head == tail;
	}

	inline T&& Dequeue()
	{
		if (isEmpty())
		{
			std::cout << "Ring buffer is empty on dequeue!" << std::endl;
			return 1000;
		}
		else
		{
			T&& item = std::move(buffer[tail]);
			tail = Advanced(tail);
			full = false;
			return std::move(item);
		}

	}

	inline bool isEmpty()
	{
		return head == tail && !full;
	}

private:
	T* buffer;
	unsigned int head;
	unsigned int tail;
	unsigned int capacity;
	bool full; // frees up one block for us
	inline unsigned int Advanced(unsigned int i)
	{
		// should be inlined by compiler
		return (i + 1) % capacity;
	}
};

