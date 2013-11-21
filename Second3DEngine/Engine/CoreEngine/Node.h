#pragma once
#include <CommonEngineIncludes.h>
#include <Timer.h>

//----------------------------------------------------
// NOTE: I really need to remove the animation stuff from this so that a node does not have
//		to have "frames" for animation. And have the animation system be seperate.
//----------------------------------------------------
namespace Monky
{	
	class BoundingSphere;

	class Node
	{
	public:

		struct Frame
		{
			Frame()
			{}
			Frame( const vec3f& pos, const mat3f& rot, const vec3f& scale )
				:	position( pos )
				,	rotation( rot )
				,	scale( scale )
			{}
			vec3f position;
			mat3f rotation;
			vec3f scale;
		};


		Node( const std::string& id );
		virtual ~Node();

		void addChild( Node* node );
		void removeChild( Node* node );
		bool isLeaf() const;
		const std::string& getID() { return m_id; }
		std::list< Node* >& getChildren() { return m_children; }
		Node* getParent() { return m_parent; }

		virtual vec3f getWorldPosition() const;
		virtual mat4f getWorldTransformation() const;

		virtual const vec3f& getLocalPosition() const;
		virtual const mat3f& getLocalRotation() const;
		virtual const vec3f& getLocalScale() const;
		virtual mat4f getLocalTransformation() const;
		
		virtual void setPosition( const vec3f& pos, int frame = 0 );
		virtual void setRotation( const mat3f& rot, int frame = 0 );
		virtual void setScale( const vec3f& scale, int frame = 0 );

		void setFrame( const Frame& frame, int frameIndex );
		void addFrame( const Frame& frame );
		void setFrameCount( int numFrames );
		
		void setFrameRate( float frameRate );
		void goToFrame( int frame );
		int getCurrentFrameNum() { return m_currentFrame; }
		int getMaxNumberOfFrames() { return m_frames.size(); }
		void play();
		void pause();
		void stop();

		virtual void update( double deltaTime );
		
		virtual BoundingSphere* getBoundingSphere();
	
	protected:
		void deleteChildren();

		Node* m_parent;
		std::list< Node* > m_children;
		bool m_isCulled;

		int m_currentFrame;
		float m_frameRate;
		bool m_isPlaying;
		Timer m_frameTimer;

		std::vector< Frame > m_frames;

		std::string m_id;
	};
}