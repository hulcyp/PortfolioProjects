#pragma once
#ifdef _WIN32
#include <glew.h>
#endif
#include <Gl/GL.h>
#include "Texture.h"
#include "StdLibraryIncludes.h"
#include "CommonTypeDefines.h"
#include "Colors.h"

namespace Monky
{
	const int MAX_NUM_LIGHTS = 8;

	class ShaderProgram;
	class Renderer;

	class Material
	{
	public:
		friend Renderer;

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


		enum MaterialType
		{
			NONE,
			AIR,
			STONE,
			WATER,
			GLASS,
			PLASTIC,
			CUSTOM,
			COUNT
		};

		struct Occlusion
		{
			Occlusion( float direct, float reverb )
				:	direct( direct )
				,	reverb( reverb )
			{}
			float direct;
			float reverb;
		};

		Material( ShaderProgram* program, int srcBlendFunc = GL_SRC_ALPHA, int destBlendFunc = GL_ONE_MINUS_SRC_ALPHA );
		~Material();


		static Material* createOrGetMaterial( const std::string& materialName, const std::string& shaderProgramName );
		static void createMaterial( const std::string& materialName, const std::string& shaderProgramName );
		static Material* getMaterial( const std::string& materialName );
		static void useMaterial( const std::string& materialName, bool lightingEnabled );
		static Material* getActiveMaterial();
		static void updateTimeOnMaterials( float time );
		static MaterialType getTypeFromString( const std::string& materialType );

		void activateProgram();
		void deactivateProgram();
		void updateUniforms();
		ShaderProgram* getShaderProgram() { return m_program; }
		const bool& isActive() const { return m_isActive; }
		void changeShaderProgram( const std::string& programName );
		
		void setMaterialType( MaterialType type );
		MaterialType getMaterialType() const;
		void setDirectOcclusion( float directOcclusion );
		void setReverbOcclusion( float reverbOcclusion );
		float getDirectOcclusion() const;
		float getReverbOcclusion() const;
	
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
		static void cleanupMaterials();
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

		float m_directOcclusion;
		float m_reverbOcclusion;
		MaterialType m_matType;

		bool m_isActive;

		static std::map< std::string, Material* > sm_materials;
		static Material* sm_currentlyActiveMaterial;
		static std::vector< Occlusion > m_occlusionValues;
		static std::map< std::string, MaterialType > m_stringToMaterialTypeMap;
	};

}