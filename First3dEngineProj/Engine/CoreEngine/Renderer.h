#pragma once

#include "CommonEngineIncludes.h"
#include <map>
#include <vector>
#include "Colors.h"
#include <MatrixStack.h>
#include <vector>
#include "Mesh.h"
#include "tinyxml2.h"
#include <stack>

namespace pdh
{
	typedef MatrixStack<float> matStackf;
	class Font;
	class Texture;
	class Material;
	class ShaderProgram;
	class Shader;
	class Camera;
	class FrameBufferObject;

	class Renderer
	{
	public:

		Renderer( int screenWidth, int screenHeight, float fov, float zNear, float zFar );
		~Renderer();

		///////////////////////////////////////////////////////////
		// Construction fuctions
		///////////////////////////////////////////////////////////
		void createShaderProgram( const std::string& shaderName, const char* vertexFile, const char* fragFile );
		Material* createMaterial( const std::string& materialName, const std::string& shaderProgramName );
		Font* createFont( const std::string& fontName, const std::string& shaderName, const char* xmlFile, const char* bitmapFile );
		Texture* createTexture( const std::string& textureName, const std::string&  fileName );
		Texture* createTexture( const std::string& textureName, unsigned int textureID );

		////////////////////////////////////////////////////////
		// Accessor functions
		////////////////////////////////////////////////////////
		Material* getMaterial( const std::string& materialName );
		ShaderProgram* getShaderProgram( const std::string programName );
		Font* getFont( const std::string& fontName );
		Texture* getTexture( const std::string& textureName );
		
		/////////////////////////////////////////////////////
		// Render functions
		/////////////////////////////////////////////////////
		void renderMesh( Mesh* mesh );
		void useMaterial( const std::string& materialName );
		void renderDebugMesh( Mesh* mesh );
		void renderVertices( std::vector< Mesh::Vertex > vertices, const std::string& material, int mode = GL_TRIANGLES );
		
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
		void setViewMatrix( const mat4f& viewMatrix ){ m_viewMatrix = viewMatrix; }
		matStackf& getModelStack() { return m_modelStack; }
		mat4f getViewMatrix() { return m_viewMatrix; }
		
		//////////////////////////////////////////////
		// Projection rendering
		//////////////////////////////////////////////
		void enableOrthoRendering();
		void disableOrthoRendering();

		////////////////////////////////////////////
		// Set Viewport
		////////////////////////////////////////////
		void setViewport( int x, int y, int width, int height );
		
		/////////////////////////////////////////////////////////
		// Utility functions
		/////////////////////////////////////////////////////////
		void renderAxis( float length );
		void toggleWireFrame();
		void setWireFrame( bool renderWireFrame );
		bool getWireFrameState();
		void setLineWidth( float lineWidth );
		void pushCamera( Camera* camera );
		void popCamera();
		Camera* getTopCamera() { return m_cameraStack.top(); }
		void updateTimeOnMaterials( float time );


	protected:
		void enableRenderStates();
		void disableRenderStates();
		void loadShaders( const char* file );
		void loadMaterials( const char* file );
		void addUniform( tinyxml2::XMLElement* element, Material* material );
		void loadFonts( const char* file );
		void loadTextures( const char* file );

		
	private:
		std::map< std::string, Material* > m_materials;
		std::map< std::string, ShaderProgram* > m_programs;
		std::map< std::string, Shader* > m_shaders;
		std::map< std::string, Font* > m_fonts;
		std::map< std::string, Texture* > m_textures;
		Material* m_currentMaterial;
		int m_screenWidth;
		int m_screenHeight;

		std::vector< FrameBufferObject* > m_fboQueue;

		matStackf m_modelStack;
		mat4f m_viewMatrix;
		mat4f m_perspectiveProjectionMatrix;
		mat4f m_orthographicProjectionMatrix;
		bool m_orthoEnabled;
		std::stack< Camera* > m_cameraStack;

		bool m_renderWireFrame;		

		int m_positionLocation;
		int m_normalLocation;
		int m_colorLocation;
		int m_texCoordLocation;
		int m_tangentLoction;
		int m_bitangentLocation;
	};
}