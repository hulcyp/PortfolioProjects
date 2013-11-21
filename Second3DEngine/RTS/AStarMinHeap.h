#pragma once
#include "Map.h"

namespace pdh
{
	const int MAX_HEAP_SIZE = 10001;
	

	class AStarMinHeap
	{
	public:
		struct HeapElement
		{
			int F;
			Map::MapTile* mapTile;
			// make copy constructor
		};

		enum HeapError
		{
			SUCCESS,
			EMPTY,
			OUT_OF_MEMORY,
			ELEMENT_NOT_FOUND
		};

		AStarMinHeap();

		HeapError popSmallestElement( HeapElement& minElement );
		HeapError pushElement( int F, Map::MapTile* mapTile );
		HeapError updateElement( int oldF, int newF, Map::MapTile* mapTile );

		void resetHeap();

	protected:
		void swapChildWithParent( int parentIndex, int childIndex );
		void swapParentWithChild( int parentIndex, int childIndex );
		bool findAndUpdateElement( int elementIndex, int oldF, int newF, Map::MapTile* mapTile );
		
	private:
		HeapElement m_heapElements[ MAX_HEAP_SIZE ];
		int m_nextElementIndex;
	};
}