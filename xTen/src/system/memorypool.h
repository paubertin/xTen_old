#pragma once

#include <string>

namespace xten {

	//--------------------------------------------------------------------------------------------------
	// This class represents a single memory pool.  A memory pool is pool of memory that's split into 
	// chunks of equal size, each with a 4-byte header.  The header is treated as a pointer that points
	// to the next chunk, making the pool a singly-linked list of memory chunks.
	// 
	// When the pool is first initialized (via the Init() function), you must pass in a chunk size and
	// the number of chunks you want created.  These two values are immutable unless you destroy and
	// reinitialize the entire pool.  The chunk size is the size of each chunk, minus the header, in 
	// bytes.  The memory pool will allocate the appropriate amount of memory and set up the data
	// structure in the Init() call.  Thus, total memory usage will be N * (S + 4) + O, where N is the
	// number of chunks, S is the size of each chunk, and O is the overhead for the class (currently 
	// 18 + (number of reallocations * 4).
	// 
	// Call the Alloc() function to retrieve a chunk from the memory pool.  The Alloc() function removes
	// the head of the linked list, sets the new head to the next chunk, and returns a pointer to the 
	// data section of the old head.  If there aren't anymore chunks left when Alloc() is called, it 
	// will allocate another block of N chunks, where N is the number of chunks you passed into Init().
	// While Alloc() is typically a very fast function, this reallocation will certainly cost you so 
	// choose your initial sizes carefully.
	// 
	// Call the Free() function to release a chunk of memory back into the memory pool for reuse.  This
	// will cause the chunk to the inserted to the front of the list, ready for the next bit.
	//--------------------------------------------------------------------------------------------------
	class xMemoryPool
	{
		unsigned char** m_ppRawMemoryArray;  // an array of memory blocks, each split up into chunks and connected
		unsigned char* m_pHead;  // the front of the memory chunk linked list
		unsigned int m_chunkSize, m_numChunks;  // the size of each chunk and number of chunks per array, respectively
		unsigned int m_memArraySize;  // the number elements in the memory array
		bool m_toAllowResize;  // true if we resize the memory pool when it fills up

							   // tracking variables we only care about for debug
#ifdef _DEBUG
		std::string m_debugName;
		unsigned long m_allocPeak, m_numAllocs;
#endif

	public:
		// construction
		xMemoryPool(void);
		~xMemoryPool(void);
		bool init(unsigned int chunkSize, unsigned int numChunks);
		void destroy(void);

		// allocation functions
		void* xAlloc(void);
		void xFree(void* pMem);
		unsigned int getChunkSize(void) const { return m_chunkSize; }

		// settings
		void setAllowResize(bool toAllowResize) { m_toAllowResize = toAllowResize; }

		// debug functions
#ifdef _DEBUG
		void setDebugName(const char* debugName) { m_debugName = debugName; }
		std::string getDebugName(void) const { return m_debugName; }
#else
		void setDebugName(const char* debugName) { }
		//std::string GetDebugName(void) const { return (std::string("<No Name>")); }
		const char* getDebugName(void) const { return "<No Name>"; }
#endif

	private:
		// resets internal vars
		void reset(void);

		// internal memory allocation helpers
		bool growMemoryArray(void);
		unsigned char* allocateNewMemoryBlock(void);

		// internal linked list management
		unsigned char* getNext(unsigned char* pBlock);
		void setNext(unsigned char* pBlockToChange, unsigned char* pNewNext);

		// don't allow copy constructor
		xMemoryPool(const xMemoryPool& memPool) {}
	};


}