#include "MemoryManager.h"
#include "MonkyException.h"
#include "StdLibraryIncludes.h"
#include "MonkyNew.h"
#include "Mesh.h"
#include "ThreadUtils.h"

#ifdef new
#undef new
#endif

namespace Monky
{
	MemoryManager* g_memoryManager = nullptr;
	MONKY_CRITICAL_SECTION g_memoryManagerCS;

	void initializeMemoryManager( size_t bytesToAllocate )
	{
		if( g_memoryManager == nullptr )
		{
			void* memPoolAddr = malloc( bytesToAllocate );
			while( memPoolAddr == nullptr )
			{
				bytesToAllocate = (size_t)( bytesToAllocate * (0.6666) ); 
				memPoolAddr = malloc( bytesToAllocate );
			}
			Monky::initializeCriticalSection( g_memoryManagerCS );
			g_memoryManager = new( memPoolAddr ) MemoryManager( bytesToAllocate );		
		}
	}
	//---------------------------------------------------------
	void cleanupMemoryManager()
	{
		if( g_memoryManager != nullptr )
		{
			//Monky::enterCriticalSection( g_memoryManagerCS );
			g_memoryManager->reportMemoryUsageToOutputConsole();
			//Monky::leaveCriticalSection( g_memoryManagerCS );
			//free( g_memoryManager );
			//g_memoryManager = nullptr;
		}	
	}
	//---------------------------------------------------------
	void updateMemoryManagerFrame()
	{
		if( g_memoryManager != nullptr )
		{
			g_memoryManager->m_currentFrame++;
			g_memoryManager->m_numberOfAllocationsLastFrame = 0;
		}
	}
	//---------------------------------------------------------
	void renderMemoryUsageToHUD( NamedProperties& params )
	{
		if( g_memoryManager != nullptr )
		{
			//Monky::enterCriticalSection( g_memoryManagerCS );
			g_memoryManager->renderMemoryUsageToHUD( params );
			//Monky::leaveCriticalSection( g_memoryManagerCS );	
		}
	}
	//---------------------------------------------------------
	void visualizeMemoryUsage( NamedProperties& params )
	{
		if( g_memoryManager != nullptr )
		{
			//Monky::enterCriticalSection( g_memoryManagerCS );
			g_memoryManager->visualizeMemoryUsage( params );
			//Monky::leaveCriticalSection( g_memoryManagerCS );		
		}
	}
	//---------------------------------------------------------
	void* allocMemoryFromPool( size_t bytes, const char* fileName, int lineNum, short memTag  )
	{
		if( g_memoryManager != nullptr )
		{
			//Monky::enterCriticalSection( g_memoryManagerCS );
			return g_memoryManager->allocateMemory( bytes, fileName, lineNum, memTag );
			//Monky::leaveCriticalSection( g_memoryManagerCS );
		}
		else
			return malloc( bytes );		
	}
	//---------------------------------------------------------
	void freeMemoryFromPool( void* addr )
	{
		if( g_memoryManager != nullptr )
		{
			//Monky::enterCriticalSection( g_memoryManagerCS );
			g_memoryManager->freeMemory( addr );
			//Monky::leaveCriticalSection( g_memoryManagerCS );
		}
		else
			free( addr );		
	}
	
	//---------------------------------------------------------
	// Memory Manager member functions
	//---------------------------------------------------------
	MemoryManager::~MemoryManager()
	{}
	//---------------------------------------------------------
	void* MemoryManager::allocateMemory( size_t bytes, const char* fileName, int lineNum, short memTag )
	{
		int newBlockSize = bytes + BMD_SIZE_BYTES;
		BlockMetaData* freeBlock = findFreeBlock( newBlockSize );
		int leftOver = freeBlock->blockSizeBytes - ( newBlockSize );

		if( leftOver >= MIN_BLOCK_SIZE_BYTES )
			splitBlock( freeBlock, newBlockSize );
		else
			freeBlock->blockSizeBytes = newBlockSize;

		freeBlock->fileName = fileName;
		freeBlock->lineNum = lineNum;
		freeBlock->tagType = memTag;
		freeBlock->frameNumAllocated = m_currentFrame;
		
		byte* allocAddr = reinterpret_cast< byte* >( freeBlock ) + BMD_SIZE_BYTES;

		collectAllocStats( newBlockSize, fileName, lineNum );
		if( allocAddr == nullptr || !isInPool( allocAddr ) )
			MONKY_EXCEPTION( "Failed to allocate memory" );
		return reinterpret_cast< void* >( allocAddr );
	}
	//---------------------------------------------------------
	void MemoryManager::freeMemory( void* addr )
	{
		byte* byteAddrOfBlockToFree = reinterpret_cast< byte* >( addr );
		byte* startOfPool = reinterpret_cast< byte* >( this );
		byte* endOfPool = reinterpret_cast< byte* >( this ) + MEMORY_POOL_SIZE_BYTES;

		if( isInPool( byteAddrOfBlockToFree ) )
		{
			byteAddrOfBlockToFree -= BMD_SIZE_BYTES;

			BlockMetaData* blockToFree = reinterpret_cast< BlockMetaData* >( byteAddrOfBlockToFree );
			blockToFree->lineNum = FREE_BLOCK;
			blockToFree->fileName = nullptr;
		
			if( blockToFree->nextBlock != nullptr )
				blockToFree->blockSizeBytes = (int)( blockToFree->nextBlock - byteAddrOfBlockToFree );
			else
			{
				byte* ptrToTheEnd = reinterpret_cast< byte* >( this ) + MEMORY_POOL_SIZE_BYTES;
				blockToFree->blockSizeBytes = (int)( ptrToTheEnd - byteAddrOfBlockToFree );
			}

			collectFreeStats( blockToFree->blockSizeBytes );
			if( blockToFree->prevBlock != nullptr )
			{
				BlockMetaData* prevBlock = reinterpret_cast< BlockMetaData* >( blockToFree->prevBlock );
				if( prevBlock->lineNum == FREE_BLOCK )
				{
					mergeFreeBlocks( prevBlock, blockToFree );

					if( prevBlock->nextBlock != nullptr )
					{
						BlockMetaData* nextBlock = reinterpret_cast< BlockMetaData* >( blockToFree->nextBlock );
						if( nextBlock->lineNum == FREE_BLOCK )
							mergeFreeBlocks( prevBlock, nextBlock );
					}
				}
				m_lastFreeBlock = prevBlock;
			}
			else if( blockToFree->nextBlock != nullptr )
			{
				BlockMetaData* nextBlock = reinterpret_cast< BlockMetaData* >( blockToFree->nextBlock );
				if( nextBlock->lineNum == FREE_BLOCK )
					mergeFreeBlocks( blockToFree, nextBlock );
				m_lastFreeBlock = blockToFree;
			}
		}
		else
		{
			// in case somebody is trying to free something not in our pool
			if( addr != nullptr )
				free( addr );
		}
	}
	//---------------------------------------------------------
	void MemoryManager::renderMemoryUsageToHUD( NamedProperties& params )
	{
		byte* firstBlockAddr = reinterpret_cast< byte* >( this ) + MANAGER_SIZE_BYTES;
		BlockMetaData* block = reinterpret_cast< BlockMetaData* >( firstBlockAddr );		
		std::string memoryUsage;

		int allocations[4] = {0,0,0,0};

		while( block != nullptr )
		{
			if( block->lineNum != FREE_BLOCK )
			{
				//char msg[100];
				//sprintf_s( msg, "Memory allocated at: %s(%d) of %dB\n", block->fileName, block->lineNum, (block->blockSizeBytes - BMD_SIZE_BYTES) ); 
				//memoryUsage += msg;
				++allocations[ block->tagType ];
			}
			block = reinterpret_cast< BlockMetaData* >( block->nextBlock );
		}

		char msg[100];
		sprintf_s( msg, "Total number of allocations: %d\n", m_totalNumberOfAllocationRequested );
		memoryUsage += msg;
		sprintf_s( msg, "Total number of bytes allocated: %dB\n", m_totalBytesAllocated );
		memoryUsage += msg;
		sprintf_s( msg, "Largest allocation request: %dB\n", m_largestAllocationRequested );
		memoryUsage += msg;
		sprintf_s( msg, "Avg allocation size: %fB\n", m_avgAllocationRequested );
		memoryUsage += msg;
		sprintf_s( msg, "Current number of allocations: %d\n", m_currentNumberOfAllocations );
		memoryUsage += msg;
		sprintf_s( msg, "Current number of bytes allocated: %dB\n", m_currentNumberOfBytesAllocated );
		memoryUsage += msg;
		sprintf_s( msg, "Bytes available: %dB\n", m_bytesAvailable );
		memoryUsage += msg;
		sprintf_s( msg, "Number of allocations last frame: %d\n", m_numberOfAllocationsLastFrame );
		memoryUsage += msg;
		sprintf_s( msg, "Current allocations by tag: Default: %d, Rendering: %d, Audio: %d, File IO: %d", allocations[DEFAULT], allocations[RENDERING], allocations[AUDIO], allocations[FILE_IO] );
		memoryUsage += msg;

		params.set( "text", memoryUsage );
		fireEvent( "renderText", params );
	}
	//---------------------------------------------------------
	void MemoryManager::visualizeMemoryUsage( NamedProperties& params )
	{
		float currentY = 0.0f;
		float totalHeight = 650.0f;
		float width = 100.0f;

		byte* firstBlockAddr = reinterpret_cast< byte* >( this ) + MANAGER_SIZE_BYTES;
		BlockMetaData* block = reinterpret_cast< BlockMetaData* >( firstBlockAddr );

		std::vector< Mesh::Vertex > vertices;
		std::vector< Mesh::Vertex > lineVertices;
		std::vector< unsigned int > indices;
		std::vector< unsigned int > lineIndices;
		unsigned int currentIndex = 0;
		Color4f currentColor = color::BLACK;


		while( block != nullptr )
		{
			if( block->lineNum == FREE_BLOCK )
			{
				currentColor = color::BLACK;								
			}
			else
			{
				currentColor = getColorFromTag( block->tagType );
				if( block->frameNumAllocated == m_currentFrame || block->frameNumAllocated == m_currentFrame - 1 )
					currentColor = color::WHITE * 0.5f + currentColor * 0.5f;
				else 
				{
					float percentAwayFromCurrentFrame = (float)( m_currentFrame - block->frameNumAllocated ) / m_currentFrame;
					currentColor = currentColor * 0.5f + currentColor * ( 1.0f - percentAwayFromCurrentFrame );
				}
			}

			float percentOfHeight = (float)block->blockSizeBytes / MEMORY_POOL_SIZE_BYTES;
			

			vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, currentY ),
												vec3f(),
												currentColor,
												vec2f( 0.0f, 0.0f ) ) );
			vertices.push_back( Mesh::Vertex(	vec3f( width, currentY ),
												vec3f(),
												currentColor,
												vec2f( 1.0f, 0.0f) ) );
			lineVertices.push_back( Mesh::Vertex(	vec3f( 0.0f, currentY ),
													vec3f(),
													color::WHITE,
													vec2f( 0.0f, 0.0f ) ) );
			lineVertices.push_back( Mesh::Vertex(	vec3f( width, currentY ),
													vec3f(),
													color::WHITE,
													vec2f( 1.0f, 0.0f) ) );
			
			currentY -= percentOfHeight * totalHeight;
			
			vertices.push_back( Mesh::Vertex(	vec3f( width, currentY ),
												vec3f(),
												currentColor,
												vec2f( 1.0f, 1.0f ) ) );
			vertices.push_back( Mesh::Vertex(	vec3f( 0.0f, currentY ),
												vec3f(),
												currentColor,
												vec2f( 0.0f, 1.0f ) ) );			

			indices.push_back( currentIndex );
			indices.push_back( currentIndex + 2 );
			indices.push_back( currentIndex + 3 );
			indices.push_back( currentIndex + 2 );
			indices.push_back( currentIndex );
			indices.push_back( currentIndex + 1 );

			lineIndices.push_back( currentIndex );
			lineIndices.push_back( currentIndex + 1 );

			currentIndex += 4;

			block = reinterpret_cast< BlockMetaData* >( block->nextBlock );
		}

		Mesh* memoryMesh = new Mesh(  vertices, indices, "SimpleColorMaterial" );
		Mesh* lineMesh = new Mesh( lineVertices, lineIndices, "SimpleColorMaterial", GL_LINES );
		vertices.clear();
		lineVertices.clear();
		indices.clear();
		lineIndices.clear();
		params.set( "mesh", memoryMesh );
		params.set( "lineWidth", 1.0f );
		fireEvent( "renderMesh", params );
		params.set( "mesh", lineMesh );
		params.set( "lineWidth", 0.5f );
		fireEvent( "renderMesh", params );
		delete memoryMesh;
		delete lineMesh;
	}
	//---------------------------------------------------------
	void MemoryManager::reportMemoryUsageToOutputConsole()
	{
		byte* firstBlockAddr = reinterpret_cast< byte* >( this ) + MANAGER_SIZE_BYTES;
		BlockMetaData* block = reinterpret_cast< BlockMetaData* >( firstBlockAddr );		

		int numOfBlocks = 0;
		while( block != nullptr )
		{
			if( block->lineNum != FREE_BLOCK )
			{
				char msg[100];
				sprintf_s( msg, "%s(%d): MEMORY LEAK(%s): %dB\n", block->fileName, block->lineNum, getStringVersionOfTag( block->tagType ), (block->blockSizeBytes - BMD_SIZE_BYTES) ); 
				MonkyException::printToCompilerOutputConsole( msg );
			}
			block = reinterpret_cast< BlockMetaData* >( block->nextBlock );
			++numOfBlocks;
		}
		
		char msg[30];
		sprintf_s( msg, "Number of blocks: %d\n", numOfBlocks - 1 );
		MonkyException::printToCompilerOutputConsole( msg );
	}
	//---------------------------------------------------------
	//	Private constructor
	//---------------------------------------------------------
	MemoryManager::MemoryManager( size_t bytesAllocated )
		:	MEMORY_POOL_SIZE_BYTES( bytesAllocated )
		,	MANAGER_SIZE_BYTES( sizeof( MemoryManager ) )
		,	BMD_SIZE_BYTES( sizeof( BlockMetaData ) )
		,	MIN_BLOCK_SIZE_BYTES( sizeof( BlockMetaData ) + 8 )
		,	m_totalNumberOfAllocationRequested( 0 )
		,	m_totalBytesAllocated( 0 )
		,	m_largestAllocationRequested( 0 )
		,	m_avgAllocationRequested( 0 )
		,	m_currentNumberOfAllocations( 0 )
		,	m_currentNumberOfBytesAllocated( 0 )
		,	m_bytesAvailable( MEMORY_POOL_SIZE_BYTES - MANAGER_SIZE_BYTES - BMD_SIZE_BYTES )
		,	m_currentFrame( 0 )
	{
		byte* firstBlockAddr = reinterpret_cast< byte* >( this ) + MANAGER_SIZE_BYTES;
		BlockMetaData* firstBlock = reinterpret_cast< BlockMetaData* >( firstBlockAddr );
		firstBlock->blockSizeBytes = MEMORY_POOL_SIZE_BYTES - MANAGER_SIZE_BYTES;
		firstBlock->fileName = "";
		firstBlock->lineNum = FREE_BLOCK;
		firstBlock->nextBlock = nullptr;
		firstBlock->prevBlock = nullptr;
		m_lastFreeBlock = firstBlock;
	}
	//---------------------------------------------------------
	MemoryManager::BlockMetaData* MemoryManager::findFreeBlock( size_t bytesIncludingBMD )
	{
		BlockMetaData* block = m_lastFreeBlock;

		while( block->lineNum != FREE_BLOCK || block->blockSizeBytes < (int)bytesIncludingBMD )
		{
			block = reinterpret_cast< BlockMetaData* >( block->nextBlock );
			if( block == nullptr )
			{
				byte* byteAddr = reinterpret_cast< byte* >( this ) + MANAGER_SIZE_BYTES; 
				block = reinterpret_cast< BlockMetaData* >( byteAddr );
			}
			if( block == m_lastFreeBlock )
				MONKY_EXCEPTION( "Memory manager out of memory" );
		}

		return block;
	}
	//---------------------------------------------------------
	void MemoryManager::splitBlock( BlockMetaData* firstBlock, size_t sizeOfFirstBlockIncludingMetaData )
	{
		byte* firstBlockAddr = reinterpret_cast< byte* >( firstBlock ); 
		byte* secBlockAddr = firstBlockAddr + sizeOfFirstBlockIncludingMetaData;
		BlockMetaData* secBlock = reinterpret_cast< BlockMetaData* >( secBlockAddr );
		
		secBlock->nextBlock = firstBlock->nextBlock;
		secBlock->prevBlock = firstBlockAddr;
		firstBlock->nextBlock = secBlockAddr;
		firstBlock->blockSizeBytes = sizeOfFirstBlockIncludingMetaData;

		if( secBlock->nextBlock != nullptr )
		{
			secBlock->blockSizeBytes = secBlock->nextBlock - secBlockAddr;
			BlockMetaData* secBlockNextBlock = reinterpret_cast< BlockMetaData* >( secBlock->nextBlock );
			secBlockNextBlock->prevBlock = secBlockAddr;
		}
		else
		{
			byte* ptrToTheEnd = reinterpret_cast< byte* >( this ) + MEMORY_POOL_SIZE_BYTES;
			secBlock->blockSizeBytes = ptrToTheEnd - secBlockAddr;
		}
		secBlock->lineNum = FREE_BLOCK;
		secBlock->fileName = nullptr;
	}
	//---------------------------------------------------------
	void MemoryManager::mergeFreeBlocks( BlockMetaData* first, BlockMetaData* second )
	{
		byte* firstBlockAddr = reinterpret_cast< byte* >( first );
		BlockMetaData* secBlockNextBlock = reinterpret_cast< BlockMetaData* >( second->nextBlock );

		first->blockSizeBytes += second->blockSizeBytes;
		first->nextBlock = second->nextBlock;
		if( secBlockNextBlock != nullptr )
			secBlockNextBlock->prevBlock = firstBlockAddr;
	}
	//---------------------------------------------------------
	void MemoryManager::collectAllocStats( size_t bytes, const char* fileName, int lineNum )
	{
		++m_totalNumberOfAllocationRequested;
		m_totalBytesAllocated += bytes;
		if( bytes > m_largestAllocationRequested )
			m_largestAllocationRequested = bytes;
		m_avgAllocationRequested *= 0.9f;
		m_avgAllocationRequested += bytes * 0.1f;	
		++m_numberOfAllocationsLastFrame;
		++m_currentNumberOfAllocations;
		m_currentNumberOfBytesAllocated += bytes;
		m_bytesAvailable -= bytes;
	}
	void MemoryManager::collectFreeStats( size_t bytes )
	{
		m_bytesAvailable += bytes;
		--m_currentNumberOfAllocations;
		m_currentNumberOfBytesAllocated -= bytes;
	}
	//---------------------------------------------------------
	bool MemoryManager::isInPool( byte* addr ) const
	{
		static const byte* startOfPool = reinterpret_cast< const byte* >( this );
		static const byte* endOfPool = startOfPool + MEMORY_POOL_SIZE_BYTES;
		return addr > startOfPool && addr < endOfPool && addr != nullptr;
	}
	//---------------------------------------------------------
	const char* MemoryManager::getStringVersionOfTag( short tag )
	{
		const char* stringVersion = nullptr;
		switch( tag )
		{
		case DEFAULT:
			stringVersion = "DEFAULT";
			break;
		case RENDERING:
			stringVersion = "RENDERING";
			break;
		case AUDIO:
			stringVersion = "RENDERING";
			break;
		case FILE_IO:
			stringVersion = "FILE IO";
			break;
		default:
			stringVersion = "";
		}
		return stringVersion;
	}
	//---------------------------------------------------------
	Color4f MemoryManager::getColorFromTag( short tag )
	{
		Color4f color;
		switch( tag )
		{
		case DEFAULT:
			color = color::BLUE;
			break;
		case RENDERING:
			color = color::GREEN;
			break;
		case AUDIO:
			color = color::RED;
			break;
		case FILE_IO:
			color = color::YELLOW;
			break;
		default:
			color = color::BLUE;
		}
		return color;
	}
}