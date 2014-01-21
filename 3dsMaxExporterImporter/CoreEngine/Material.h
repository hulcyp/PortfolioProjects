#pragma once
#ifdef _WIN32
#include <glew.h>
#endif
#include <glut.h>
#include <map>
#include <vector>
#include "CommonEngineIncludes.h"
#include <string>
#include "Texture.h"

namespace pdh
{
	const int MAX_NUM_LIGHTS = 8;

	class ShaderProgram;

	class Material
	{
	public:

		struct AttenuationType
		{
			AttenuationType()
				:	constant( 0 )
				,	linear( 1 )
				,	inverse( 0 )
				,	inverseSquared( 0 ) 
			{}
			int constant;
			int linear;
			int inverse;
			int inverseSquared;
		};

		struct Attenuation
		{
			Attenuation()
				:	start( 0.0f )
				,	end( 10.0f )
			{}
			float start;
			float end;
		};

		
		struct Aperture
		{
			Aperture()
				:	innerConeDot( -1.0f )
				,	outerConeDot( -1.0f )
			{}
			float innerConeDot;
			float outerConeDot;
		};
		
		struct Light
		{
			Light()
				:	ambientFactor( 0.0f )
			{}
			vec3f position;
			Color4f color;
			vec3f direction;
			float ambientFactor;
			Attenuation attenuation;
			Aperture aperture;
			AttenuationType attenuationType;
		};

		struct AttenuationUniformLocs
		{
			GLint start;
			GLint end;
		};

		struct ApertureUniformLocs
		{
			GLint inner;
			GLint outer;
		};

		struct LightUniformLocs
		{
			GLint pos;
			GLint color;
			GLint dir;
			GLint ambient;
			GLint attenType;
			AttenuationUniformLocs atten;
			ApertureUniformLocs apert;
		};


		Material( ShaderProgram* program, int srcBlendFunc = GL_SRC_ALPHA, int destBlendFunc = GL_ONE_MINUS_SRC_ALPHA );
		~Material();

		void activateProgram();
		void deactivateProgram();
		void updateUniforms();
		ShaderProgram* getShaderProgram() { return m_program; }
		const bool& isActive() const { return m_isActive; }
		void changeShaderProgram( const std::string& programName );
	
		void addTexture( const char* uniformName, Texture* texture );
		void addUniform( const char* uniformName, int value );
		void addUniform( const char* uniformName, unsigned int value );
		void addUniform( const char* uniformName, float value );
		void addUniform( const char* uniformName, const vec2i& value );
		void addUniform( const char* uniformName, const vec2f& value );
		void addUniform( const char* uniformName, const vec3i& value );
		void addUniform( const char* uniformName, const vec3f& value );
		void addUniform( const char* uniformName, const mat4f& value );
		void addUniform( const char* uniformName, const vec2ui& value );
		void addUniform( const char* uniformName, const vec3ui& value );
		void addUniform( const char* uniformName, const Light* value );	
		void addUniform( const char* uniformName, const Color4f& value );

		void updateUniform( const std::string& uniformName, Texture* texture ); 
		void updateUniform( const std::string& uniformName, int value );
		void updateUniform( const std::string& uniformName, unsigned int value );
		void updateUniform( const std::string& uniformName, float value );
		void updateUniform( const std::string& uniformName, const vec2i& value );
		void updateUniform( const std::string& uniformName, const vec2f& value );
		void updateUniform( const std::string& uniformName, const vec3i& value );
		void updateUniform( const std::string& uniformName, const vec3f& value );
		void updateUniform( const std::string& uniformName, const mat4f& value );
		void updateUniform( const std::string& uniformName, const vec2ui& value );
		void updateUniform( const std::string& uniformName, const vec3ui& value );
		void updateUniform( const std::string& uniformName, const Light* value );
		void updateUniform( const std::string& uniformName, const Color4f& value );

	protected:
		void activateLights();
		void updateUniformLocations();
		
	private:
		ShaderProgram* m_program;
		std::map< std::string, std::pair< GLint, Texture* > > m_textures;
		std::map< std::string, std::pair< GLint, int > > m_intUniforms;
		std::map< std::string, std::pair< GLint, unsigned int > > m_uintUniforms;
		std::map< std::string, std::pair< GLint, float > > m_floatUniforms;
		std::map< std::string, std::pair< GLint, vec2i > > m_vec2iUniforms;
		std::map< std::string, std::pair< GLint, vec2f > > m_vec2fUniforms;
		std::map< std::string, std::pair< GLint, vec3i > > m_vec3iUniforms;
		std::map< std::string, std::pair< GLint, vec3f > > m_vec3fUniforms;
		std::map< std::string, std::pair< GLint, mat4f > > m_mat4fUniforms;
		std::map< std::string, std::pair< GLint, vec2ui > > m_vec2uiUniforms;
		std::map< std::string, std::pair< GLint, vec3ui > > m_vec3uiUniforms;
		std::map< std::string, std::pair< LightUniformLocs,  const Light* > > m_lightUniforms;
		std::map< std::string, std::pair< GLint, Color4f > > m_color4fUniforms;


		int m_blendFuncSrcFactor;
		int m_blendFuncDestFactor;	

		bool m_isActive;
	};

}