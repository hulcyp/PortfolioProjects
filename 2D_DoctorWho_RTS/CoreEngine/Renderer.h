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
#include <EventSystemInterface.h>
#include <Font.h>

namespace pdh
{
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

		enum ProjectionMode
		{
			PH_PERSPECTIVE,
			PH_ORTHOGRAPHIC
		};

		Renderer( int screenWidth, int screenHeight, float fov, float zNear, float zFar );
		~Renderer();

		///////////////////////////////////////////////////////////
		// Construction fuctions
		///////////////////////////////////////////////////////////
		static void createShaderProgram( const std::string& shaderName, const char* vertexFile, const char* fragFile );
		static Material* createMaterial( const std::string& materialName, const std::string& shaderProgramName );
		static Texture* createTexture( const std::string& textureName, const std::string&  fileName );
		static Texture* createTexture( const std::string& textureName, unsigned int textureID );
		Font* createFont( const std::string& fontName, const std::string& shaderName, const char* xmlFile, const char* bitmapFile );

		////////////////////////////////////////////////////////
		// Accessor functions
		////////////////////////////////////////////////////////
		static Material* getMaterial( const std::string& materialName );
		static ShaderProgram* getShaderProgram( const std::string programName );
		static Texture* getTexture( const std::string& textureName );
		Font* getFont( const std::string& fontName );

		int getScreenWidth() const { return m_screenWidth; }
		int getScreenHeight() const { return m_screenHeight; }
		
		/////////////////////////////////////////////////////
		// Render functions
		/////////////////////////////////////////////////////
		void renderMeshEvent( NamedProperties& params );
		void renderMesh( Mesh* mesh );
		void useMaterial( const std::string& materialName );
		void renderDebugMesh( Mesh* mesh );
		
		void renderTextEvent( NamedProperties& params );
		void renderText( const std::string& fontName, const std::string& text, vec3f pos, float fontHeight, const Color4f& color = color::WHITE, const vec2f& posRel = Font::BOTLEFT, bool fixedWidth = false );


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
		const mat4f& getViewMatrix() const;
		
		//////////////////////////////////////////////
		// Projection rendering
		//////////////////////////////////////////////
		void setProjectionMode( const ProjectionMode& mode );
		//void pushProjectionMatrix( const mat4f& projMatrix );
		//void popProjectionMatrix();
		const mat4f& getProjMatrix() const;
		

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

		/////////////////////////////////////////////////////////
		// Debug functions
		/////////////////////////////////////////////////////////
		void enableLighting( bool enable ) { m_lightingEnabled = enable; }


	protected:
		void updateMaterialUniforms();
		void loadShaders( const char* file );
		void loadMaterials( const char* file );
		void addUniform( tinyxml2::XMLElement* element, Material* material );
		void loadFonts( const char* file );
		void loadTextures( const char* file );

		
	private:
		//--------------------------------------------------------
		// Static pool members
		//--------------------------------------------------------
		static std::map< std::string, Material* > sm_materials;
		static std::map< std::string, ShaderProgram* > sm_programs;
		static std::map< std::string, Shader* > sm_shaders;
		static std::map< std::string, Texture* > sm_textures;


		std::map< std::string, Font* > m_fonts;
		
		Material* m_currentMaterial;
		int m_screenWidth;
		int m_screenHeight;

		std::vector< FrameBufferObject* > m_fboQueue;

		matStackf m_modelStack;
		mat4f m_viewMatrix;
		//std::stack< mat4f > m_perspectiveProjStack;
		//std::stack< mat4f > m_orthographicProjStack;
		ProjectionMode m_projectionMode;
		std::stack< Camera* > m_cameraStack;

		bool m_renderWireFrame;		


		// Debugging stuff
		bool m_lightingEnabled;
	};
}