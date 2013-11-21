#pragma once

#include "CommonEngineIncludes.h"
#include "Colors.h"
#include <MatrixStack.h>
#include "Mesh.h"
#include <Font.h>
#include "Camera.h"
#include "XMLParser.h"


namespace Monky
{
	class Font;
	class Texture;
	class Material;
	class ShaderProgram;
	class Shader;
	class FrameBufferObject;

	class Renderer
	{
	public:

		Renderer( int screenWidth, int screenHeight, float fov, float zNear, float zFar );
		~Renderer();

		void loadRendereringDataFiles();
		

		int getScreenWidth() const { return m_screenWidth; }
		int getScreenHeight() const { return m_screenHeight; }
		
		/////////////////////////////////////////////////////
		// Render functions
		/////////////////////////////////////////////////////
		void renderMeshEvent( NamedProperties& params );
		void renderMesh( Mesh* mesh );
		void renderDebugMeshEvent( NamedProperties& params );
		void renderDebugMesh( Mesh* mesh );
		
		void renderTextEvent( NamedProperties& params );
		void renderText( const std::string& fontName, const std::string& text, const mat4f& transformation, float fontHeight, const Color4f& color = color::WHITE, const vec2f& posRel = Font::BOTLEFT, bool drawWithShadow = false, bool fixedWidth = false );
		
		/////////////////////////////////////////////////////
		// Render FBO functions
		/////////////////////////////////////////////////////
		void pushFBO( int width, int height, const std::string& material );
		void popFBO();
		void bindFBOQueue();
		void renderFBOQueue();

		//////////////////////////////////////////////////
		// Matrix stack functions
		//////////////////////////////////////////////////
		matStackf& getModelStack() { return m_modelStack; }
		const mat4f& getViewMatrix() const;
		
		//////////////////////////////////////////////
		// Projection rendering
		//////////////////////////////////////////////
		void setProjectionMode( Camera::ProjectionMode mode );
		
		////////////////////////////////////////////
		// Set Viewport
		////////////////////////////////////////////
		void setViewport( int x, int y, int width, int height );

		/////////////////////////////////////////////////////////
		// Camera functions
		/////////////////////////////////////////////////////////
		void pushCamera( Camera* camera );
		void popCamera();
		Camera* getTopCamera() { return m_cameraStack.top(); }
				
		/////////////////////////////////////////////////////////
		// Utility functions
		/////////////////////////////////////////////////////////
		void renderAxisEvent( NamedProperties& params );
		void renderAxis( float length );
		void toggleWireFrame();
		void setWireFrame( bool renderWireFrame );
		bool getWireFrameState();
		void setLineWidth( float lineWidth );
		void setBackFaceCulling( bool enabled );
		bool getBackFaceCulling() const;
		
		/////////////////////////////////////////////////////////
		// Debug functions
		/////////////////////////////////////////////////////////
		void enableLighting( bool enable ) { m_lightingEnabled = enable; }


	protected:
		void cleanupRendereringData();
		void updateMaterialUniforms();
		void loadShaders( const std::string& fileName, const std::string& filePath );
		void loadMaterials( const std::string& fileName, const std::string& filePath );
		void addUniform( XMLNode* element, Material* material );
		void loadFonts( const std::string& fileName, const std::string& filePath );
		void loadTextures( const std::string& fileName, const std::string& filePath );

		
	private:		
		int m_screenWidth;
		int m_screenHeight;

		std::vector< FrameBufferObject* > m_fboQueue;

		matStackf m_modelStack;
		mat4f m_viewMatrix;
		Camera::ProjectionMode m_projectionMode;
		std::stack< Camera* > m_cameraStack;

		bool m_renderWireFrame;		
		float m_lineWidth;
		bool m_backFaceCullingEnabled;

		// Debugging stuff
		bool m_lightingEnabled;
	};
}