#pragma once
#include "Vector2s.h"

const int MAX_HEAP_SIZE = 10001;
typedef Vector2s MapTile;

class AStarMinHeap
{
public:
	struct HeapElement
	{
		float F;
		MapTile mapTile;
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
	HeapError pushElement( float F, MapTile mapTile );
	HeapError updateElement( float oldF, float newF, MapTile mapTile );

	void resetHeap();

protected:
	void swapChildWithParent( int parentIndex, int childIndex );
	void swapParentWithChild( int parentIndex, int childIndex );
	bool findAndUpdateElement( int elementIndex, float oldF, float newF, MapTile mapTile );
		
private:
	HeapElement m_heapElements[ MAX_HEAP_SIZE ];
	int m_nextElementIndex;
};