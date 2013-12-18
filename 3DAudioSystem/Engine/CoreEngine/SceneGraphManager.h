#pragma once
#include "StdLibraryIncludes.h"

namespace Monky
{
	class Node;

	typedef std::map< std::string, Node* >::iterator nIter;

	class SceneGraphManager
	{
	public:
		virtual ~SceneGraphManager();

		void addNode( Node* node );
		void addNode( const std::string& parentID, Node* child );
		void removeNode( const std::string& id );
		void removeNodeAndChildren( const std::string& id );
		Node* getNode( const std::string& id );
		void listNodes();

	private:
		Node* m_root;

		std::map< std::string, Node* > m_nodes;
	};
}