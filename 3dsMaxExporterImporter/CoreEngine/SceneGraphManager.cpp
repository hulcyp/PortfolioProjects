#include "SceneGraphManager.h"
#include "CommonEngineIncludes.h"
#include "Node.h"
#include <algorithm>

namespace pdh
{
	SceneGraphManager::~SceneGraphManager()
	{
		MAP_CLEANUP( m_nodes );
	}
	//-------------------------------------------------------------------
	void SceneGraphManager::addNode( Node* node )
	{
		Node*& tempNode = m_nodes[ node->getID() ];
		if( tempNode != nullptr )
		{
			tempNode = node;
			m_root->addChild( node );
		}
		else
		{
			consolePrintColorf( "The Node %s, already exists", color::YELLOW, node->getID().c_str() );
		}
	}
	//-------------------------------------------------------------------
	void SceneGraphManager::addNode( const std::string& parentID, Node* child )
	{
		nIter iter = m_nodes.find( parentID );

		if( iter != m_nodes.end() && child != nullptr )
		{
			iter->second->addChild( child );
		}
		else
		{
			//m_root->addChild( child );
			consolePrintColorf( "The parent Node %s, does not exist. Failed to make %s a child of specified Node", color::YELLOW, parentID.c_str(), child->getID().c_str() );
		}
	}
	//-------------------------------------------------------------------
	void SceneGraphManager::removeNode( const std::string& id )
	{
		nIter iter = m_nodes.find( id );
		if( iter != m_nodes.end() )
		{
			std::list< Node* > children = iter->second->getChildren();
			std::for_each( children.begin(), children.end(),
				[&]( Node* child )
				{
					iter->second->getParent()->addChild( child );		
				} );
			m_nodes.erase( iter );
		}
	}
	//-------------------------------------------------------------------
	void SceneGraphManager::removeNodeAndChildren( const std::string& id )
	{
		nIter iter = m_nodes.find( id );
		if( iter != m_nodes.end() )
		{
			std::list< Node* > children = iter->second->getChildren();
			std::for_each( children.begin(), children.end(),
				[&]( Node* child )
				{
					iter->second->getParent()->addChild( child );		
				} );
			SAFE_DELETE( iter->second );
			m_nodes.erase( iter );
		}
	}
	//-------------------------------------------------------------------
	Node* SceneGraphManager::getNode( const std::string& id )
	{
		nIter iter = m_nodes.find( id );
		if( iter != m_nodes.end() )
		{
			return iter->second;
		}
		else
		{
			consolePrintColorf( "The Node %s, does not exist", color::YELLOW, id.c_str() );
			return nullptr;
		}
	}
	//-------------------------------------------------------------------
	void SceneGraphManager::listNodes()
	{
		for( nIter iter = m_nodes.begin(); iter != m_nodes.end(); ++iter )
		{
			consolePrintColorf( iter->first.c_str(), color::YELLOW );
		}
	}
}