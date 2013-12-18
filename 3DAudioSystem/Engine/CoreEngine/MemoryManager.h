#pragma once
#include "CommonTypeDefines.h"
#include "Uncopyable.h"
#include "EventSystemInterface.h"
#include "Colors.h"

//Special flags for the lineNum variable
#define FREE_BLOCK				-1
#define USED_BLOCK_NO_LINE_NUM	-2

#define DEFAULT 0
#define RENDERING 1
#define AUDIO 2
#define FILE_IO 3


namespace Monky
{
	void initializeMemoryManager( size_t bytesToAllocate );
	void cleanupMemoryManager();
	void updateMemoryManagerFrame();
	void renderMemoryUsageToHUD( NamedProperties& params );
	void visualizeMemoryUsage( NamedProperties& params );
	void* allocMemoryFromPool( size_t bytes, const char* fileName, int lineNum, short memTag = DEFAULT );
	void freeMemoryFromPool( void* addr );

	class MemoryManager : public Uncopyable
	{
	public:
		friend void initializeMemoryManager( size_t bytesToAllocate );
		friend void cleanupMemoryManager();
		friend void updateMemoryManagerFrame();

		~MemoryManager();

		void* allocateMemory( size_t bytes, const char* fileName, int lineNum, short memTag = DEFAULT );
		void freeMemory( void* addr );

		void renderMemoryUsageToHUD( NamedProperties& params );
		void visualizeMemoryUsage( NamedProperties& params );
		void reportMemoryUsageToOutputConsole();

	private:
		struct BlockMetaData
		{
			byte* prevBlock;
			byte* nextBlock;
			const char* fileName;
			int blockSizeBytes;		//including BlockMetaData
			int lineNum;
			int frameNumAllocated;
			short tagType;
		};
		
		MemoryManager( size_t bytesAllocated );
		BlockMetaData* findFreeBlock( size_t bytesIncludingBMD );
		void splitBlock( BlockMetaData* firstBlock, size_t sizeOfFirstBlockIncludingMetaData );
		void mergeFreeBlocks( BlockMetaData* first, BlockMetaData* second );		
		void collectAllocStats( size_t bytes, const char* fileName, int lineNum );
		void collectFreeStats( size_t bytes );

		bool isInPool( byte* addr ) const;
		const char* getStringVersionOfTag( short tag );
		Color4f getColorFromTag( short tag );

		const int MEMORY_POOL_SIZE_BYTES;
		const int MANAGER_SIZE_BYTES;
		const int BMD_SIZE_BYTES;
		const int MIN_BLOCK_SIZE_BYTES;

		BlockMetaData* m_lastFreeBlock;

		size_t m_totalNumberOfAllocationRequested;
		size_t m_totalBytesAllocated;
		size_t m_largestAllocationRequested;
		float m_avgAllocationRequested;
		size_t m_currentNumberOfAllocations;
		size_t m_currentNumberOfBytesAllocated;
		size_t m_bytesAvailable;
		size_t m_numberOfAllocationsLastFrame;
		size_t m_currentFrame;
	};	
}