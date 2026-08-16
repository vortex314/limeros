#ifndef CIRCBUF_H_
#define CIRCBUF_H_

// #include "base.h"
#include <stdint.h>
#include <errno.h>

#define POW 4
#define CIRCBUF_SIZE (1 << POW)
#define CIRCBUF_MASK (CIRCBUF_SIZE - 1)

template <typename T>
class CircBuf
{
private:
	T *_start;
	uint64_t _readPos;
	uint64_t _writePos;
	uint32_t _limit;

public:
	CircBuf(uint32_t buf_size)
	{
		_start = new T[buf_size];
		clear();
		_limit = buf_size;
	}

	void clear()
	{
		_readPos = 0;
		_writePos = 0;
	}

	~CircBuf() { delete[] _start; }

	int write(T *b, uint32_t len)
	{
		if (space() >= len)
		{
			for (uint32_t i = 0; i < len; i++)
			{
				_start[_writePos++ % _limit] = b[i];
			}
			return 0;
		}
		return -ENOSPC;
	}

	int write(T b)
	{
		if (space() > 1)
		{
			_start[_writePos++ % _limit] = b;
			return 0;
		}
		return -ENOSPC;
	}

	T read()
	{
		if (hasData())
			return _start[_readPos++ % _limit];
		// PANIC("CircBuf underflow");
		return T();
	}

	uint32_t size() const { return _writePos - _readPos; }
	uint32_t space() const { return _limit - size(); }
	bool hasSpace(uint32_t sz = 1) const { return space() >= sz; }
	bool hasData() const { return size() > 0; }
	bool isFull() const { return space() == 0; }
	bool isEmpty() const { return size() == 0; }
};

#endif /* CIRCBUF_H_ */