#include "AStarMinHeap.h"

namespace pdh
{
	AStarMinHeap::AStarMinHeap()
		:	m_nextElementIndex( 0 )
	{}
	//-----------------------------------------------------
	AStarMinHeap::HeapError AStarMinHeap::popSmallestElement( HeapElement& minElement )
	{
		if( m_nextElementIndex == 0 )
			return EMPTY;
		
		minElement = m_heapElements[0];

		m_heapElements[0] = m_heapElements[ --m_nextElementIndex ];

		if( m_nextElementIndex > 2 )
		{
			if( m_heapElements[0].F > m_heapElements[1].F )
				swapParentWithChild( 0, 1 );
			else if( m_heapElements[0].F > m_heapElements[2].F )
				swapParentWithChild( 0, 2 );
		}
		else if( m_nextElementIndex > 1 )
		{
			if( m_heapElements[0].F > m_heapElements[1].F )
				swapParentWithChild( 0, 1 );	
		}
		
		return SUCCESS;
	}
	
	//-----------------------------------------------------
	AStarMinHeap::HeapError AStarMinHeap::pushElement( int F, Map::MapTile* mapTile )
	{
		if( m_nextElementIndex + 1 == MAX_HEAP_SIZE )
			return OUT_OF_MEMORY;

		m_heapElements[ m_nextElementIndex ].F = F;
		m_heapElements[ m_nextElementIndex ].mapTile = mapTile;

		if( m_nextElementIndex > 0 )
		{
			int parentIndex =  ( m_nextElementIndex - 1 ) >> 1;
			if( m_heapElements[ m_nextElementIndex ].F < m_heapElements[ parentIndex ].F )
			{
				swapChildWithParent( parentIndex, m_nextElementIndex );
			}
		}
		++m_nextElementIndex;
		return SUCCESS;
	}
	//-----------------------------------------------------
	AStarMinHeap::HeapError AStarMinHeap::updateElement( int oldF, int newF, Map::MapTile* mapTile )
	{
		if( m_nextElementIndex == 0 )
			return EMPTY;

		if( m_heapElements[0].F == oldF )
		{
			if( mapTile == m_heapElements[0].mapTile )
			{
				m_heapElements[0].mapTile = mapTile;
				return SUCCESS;
			}
		}

		if( oldF >= m_heapElements[1].F )
		{
			if( findAndUpdateElement( 1, oldF, newF, mapTile ) )
				return SUCCESS;
		}
		if( oldF >= m_heapElements[2].F )
		{
			if( findAndUpdateElement( 2, oldF, newF, mapTile ) )
				return SUCCESS;
		}

		return ELEMENT_NOT_FOUND;
	}
	//-----------------------------------------------------
	void AStarMinHeap::resetHeap()
	{
		m_nextElementIndex = 0;
	}
	//-----------------------------------------------------
	// Protected member functions
	//-----------------------------------------------------
	void AStarMinHeap::swapChildWithParent( int parentIndex, int childIndex )
	{
		int swapIndex = m_nextElementIndex + 1;

		m_heapElements[ swapIndex ].F = m_heapElements[ parentIndex ].F;
		m_heapElements[ swapIndex ].mapTile = m_heapElements[ parentIndex ].mapTile;

		m_heapElements[ parentIndex ].F = m_heapElements[ childIndex ].F;
		m_heapElements[ parentIndex ].mapTile = m_heapElements[ childIndex ].mapTile;

		m_heapElements[ childIndex ].F = m_heapElements[ swapIndex ].F;
		m_heapElements[ childIndex ].mapTile = m_heapElements[ swapIndex ].mapTile;

		int nextParentIndex = ( parentIndex - 1 ) >> 1;
		if( nextParentIndex >= 0 )
		{
			if( m_heapElements[ nextParentIndex ].F > m_heapElements[ parentIndex ].F )
				swapChildWithParent( nextParentIndex, parentIndex );
		}				
	}
	//-----------------------------------------------------
	void AStarMinHeap::swapParentWithChild( int parentIndex, int childIndex )
	{

		int swapIndex = m_nextElementIndex + 1;
		m_heapElements[ swapIndex ].F = m_heapElements[ parentIndex ].F;
		m_heapElements[ swapIndex ].mapTile = m_heapElements[ parentIndex ].mapTile;

		m_heapElements[ parentIndex ].F = m_heapElements[ childIndex ].F;
		m_heapElements[ parentIndex ].mapTile = m_heapElements[ childIndex ].mapTile;

		m_heapElements[ childIndex ].F = m_heapElements[ swapIndex ].F;
		m_heapElements[ childIndex ].mapTile = m_heapElements[ swapIndex ].mapTile;

		int child1Index = 2 * childIndex + 1;
		int child2Index = child1Index + 1;

		if( m_nextElementIndex > child2Index )
		{
			if( m_heapElements[childIndex].F > m_heapElements[child1Index].F )
				swapParentWithChild( childIndex, child1Index );
			else if( m_heapElements[childIndex].F > m_heapElements[child2Index].F )
				swapParentWithChild( childIndex, child2Index );
		}
		else if( m_nextElementIndex > child1Index )
		{
			if( m_heapElements[childIndex].F > m_heapElements[child1Index].F )
				swapParentWithChild( childIndex, child1Index );
		}
	}
	//-----------------------------------------------------
	bool AStarMinHeap::findAndUpdateElement( int elementIndex, int oldF, int newF, Map::MapTile* mapTile )
	{
		if( m_heapElements[ elementIndex ].mapTile == mapTile )
		{
			m_heapElements[ elementIndex ].F = newF;
			int parentIndex = ( elementIndex - 1 ) >> 1;
			if( m_heapElements[ parentIndex ].F > m_heapElements[ elementIndex ].F )
				swapChildWithParent( parentIndex, elementIndex );
			return true;
		}

		int child1Index = 2 * elementIndex + 1;
		int child2Index = child1Index + 1;

		if( m_nextElementIndex > child2Index )
		{
			if( oldF >= m_heapElements[ child1Index ].F )
			{
				if( findAndUpdateElement( child1Index, oldF, newF, mapTile ) )
					return true;
			}
			if( oldF >= m_heapElements[ child2Index ].F )
			{
				if( findAndUpdateElement( child2Index, oldF, newF, mapTile ) )
					return true;
			}
		}
		else if( m_nextElementIndex > child1Index )
		{
			if( oldF >= m_heapElements[ child2Index ].F )
			{
				if( findAndUpdateElement( child2Index, oldF, newF, mapTile ) )
					return true;
			}
		}

		return false;
	}
}