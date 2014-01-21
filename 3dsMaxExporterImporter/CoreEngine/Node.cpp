#include "Node.h"
#include "BoundingSphere.h"
#include <algorithm>

namespace pdh
{
	Node::Node( const std::string& id )
		:	m_id( id )
		,	m_parent( nullptr )
		,	m_isCulled( false )
		,	m_currentFrame( 0 )
		,	m_isPlaying( false )
	{
		m_frames.push_back( Frame( vec3f(), mat3f::IDENTITY, vec3f::ONE ) );
	}
	//-------------------------------------------------------------------
	Node::~Node()
	{
		m_children.clear();
	}
	//-------------------------------------------------------------------
	void Node::addChild( Node* node )
	{
		if( node != this )
		{
			m_children.push_back( node );
			node->m_parent = this;
		}
	}
	//-------------------------------------------------------------------
	void Node::removeChild( Node* node )
	{
		m_children.remove( node );
	}
	//-------------------------------------------------------------------
	bool Node::isLeaf() const
	{
		return m_children.empty();
	}
	//-------------------------------------------------------------------
	vec3f Node::getWorldPosition() const
	{
		mat4f trans = getWorldTransformation();


		return vec3f( trans[12], trans[13], trans[14] );
	}
	//-------------------------------------------------------------------
	mat4f Node::getWorldTransformation() const
	{		
		vec3f pos = m_frames[ m_currentFrame ].position;
		mat3f rot = m_frames[ m_currentFrame ].rotation;
		vec3f scale = m_frames[ m_currentFrame ].scale;

		mat4f scaleTM = mat4f( scale.x,	0.0f,		0.0f,		0.0f,
								0.0f,	scale.y,	0.0f,		0.0f,
								0.0f,	0.0f,		scale.z,	0.0f, 
								0.0f,	0.0f,		0.0f,		1.0f );

		mat4f transform = scaleTM * mat4f( rot );
		transform[12] = pos.x;
		transform[13] = pos.y;
		transform[14] = pos.z;

		if( m_parent != nullptr )
		{
			transform *= m_parent->getWorldTransformation();
		}

		return transform;
	}
	//-------------------------------------------------------------------
	const vec3f& Node::getLocalPosition() const
	{
		return m_frames[ m_currentFrame ].position;
	}
	//-------------------------------------------------------------------
	const mat3f& Node::getLocalRotation() const
	{
		return m_frames[ m_currentFrame ].rotation;
	}
	//-------------------------------------------------------------------
	const vec3f& Node::getLocalScale() const
	{
		return m_frames[ m_currentFrame ].scale;
	}
	//-------------------------------------------------------------------
	mat4f Node::getLocalTransformation() const
	{
		const vec3f& scale = m_frames[ m_currentFrame ].scale;

		mat4f scaleTM = mat4f( scale.x,	0.0f,		0.0f,		0.0f,
								0.0f,		scale.y,	0.0f,		0.0f,
								0.0f,		0.0f,		scale.z,	0.0f, 
								0.0f,		0.0f,		0.0f,		1.0f );

		mat4f transform = scaleTM * mat4f( m_frames[ m_currentFrame ].rotation );
		transform[12] = m_frames[ m_currentFrame ].position.x;
		transform[13] = m_frames[ m_currentFrame ].position.y;
		transform[14] = m_frames[ m_currentFrame ].position.z;

		return transform;
	}
	//-------------------------------------------------------------------
	void Node::setPosition( const vec3f& pos, int frame )
	{
		assertion( 0 <= frame && frame < (int)m_frames.size(), "Node position frame index: %d out of bounds", frame );
		m_frames[ frame ].position = pos;
	}
	//-------------------------------------------------------------------
	void Node::setRotation( const mat3f& rot, int frame )
	{
		assertion( 0 <= frame && frame < (int)m_frames.size(), "Node rotation frame index: %d out of bounds", frame );
		m_frames[ frame ].rotation = rot;
	}
	//-------------------------------------------------------------------
	void Node::setScale( const vec3f& scale, int frame )
	{
		assertion( 0 <= frame && frame < (int)m_frames.size(), "Node scale frame sindex: %d out of bounds", frame );
		m_frames[ frame ].scale = scale;
	}
	//-------------------------------------------------------------------
	void Node::setFrame( const Frame& frame, int frameIndex )
	{
		assertion( 0 <= frameIndex && frameIndex < (int)m_frames.size(), "Node frames index: %d out of bounds", frameIndex );
		m_frames[ frameIndex ] = frame;
	}
	//-------------------------------------------------------------------
	void Node::addFrame( const Frame& frame )
	{
		m_frames.push_back( frame );
	}
	//-------------------------------------------------------------------
	void Node::setFrameCount( int numFrames )
	{
		m_frames.resize( numFrames );
	}
	//-------------------------------------------------------------------
	void Node::setFrameRate( float frameRate )
	{
		m_frameRate = frameRate;
		m_frameTimer.setTime( frameRate );
	}
	//-------------------------------------------------------------------
	void Node::goToFrame( int frame )
	{
		assertion( 0 <= frame && frame < (int)m_frames.size(), "Node frame index: %d out of bounds", frame );
		m_currentFrame = frame;
	}
	//-------------------------------------------------------------------
	void Node::play()
	{
		m_isPlaying = true;
		if( !m_frameTimer.wasStarted() )
		{
			m_frameTimer.setTime( m_frameRate );
			m_frameTimer.start();
		}
		else
		{
			m_frameTimer.unPause();
		}
	}
	//-------------------------------------------------------------------
	void Node::pause()
	{
		m_isPlaying = false;
		m_frameTimer.pause();
	}
	//-------------------------------------------------------------------
	void Node::stop()
	{
		m_isPlaying = false;
		m_currentFrame = 0;
		m_frameTimer.stop();
	}
	//-------------------------------------------------------------------
	void Node::update( double deltaTime )
	{
		if( m_isPlaying )
		{
			if( m_frameTimer.isTimeUp() )
			{
				m_currentFrame = MathFuncs<int>::wrap( m_currentFrame + 1, 0, m_frames.size() - 1 );

				m_frameTimer.start();
			}
		}
		//std::for_each( m_children.begin(), m_children.end(),
		//	[&] ( Node* node )
		//{
		//	node->update( deltaTime );
		//} );
	}
	//-------------------------------------------------------------------
	BoundingSphere* Node::getBoundingSphere()
	{
		return new BoundingSphere();
	}
	//-------------------------------------------------------------------
	// Protected Member functions
	//-------------------------------------------------------------------
	void Node::deleteChildren()
	{
		std::for_each( m_children.begin(), m_children.end(),
			[&]( Node* child )
		{
			child->deleteChildren();
		} );

		LIST_CLEANUP( m_children );
	}
}