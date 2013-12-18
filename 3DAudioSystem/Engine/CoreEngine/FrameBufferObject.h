#pragma once
#include "Texture.h"
#include "Mesh.h"

namespace Monky
{
	class Renderer;

	class FrameBufferObject
	{
	public:
		FrameBufferObject( int width, int height, float aspectRatio, const std::string& materialName, Renderer* renderer );
		~FrameBufferObject();

		void bindFBO();
		void unBindFBO();
		void render( Renderer* renderer );
		Texture* getColorTexture() { return m_colorTexture; }
		Texture* getDepthTexture() { return m_depthTexture; }
		const std::string& getMaterial() { return m_mesh->getMaterial( 0 ); }

	private:
		unsigned int m_FBOId;
		Texture* m_colorTexture;
		int m_currentTextureIndex;
		Texture* m_depthTexture;
		int m_width;
		int m_height;
		float m_aspectRatio;
		Mesh* m_mesh;
	};
}