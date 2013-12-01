#include "Material.h"
#include "ShaderProgram.h"
#include <algorithm>

namespace pdh
{
	Material::Material( ShaderProgram* program, int srcBlendFunc, int destBlendFunc )
		:	m_program( program )
		,	m_blendFuncSrcFactor( srcBlendFunc )
		,	m_blendFuncDestFactor( destBlendFunc )
	{}

	Material::~Material()
	{}

	void Material::activateProgram()
	{
		glUseProgram( m_program->getProgramID() );
		glBlendFunc( m_blendFuncSrcFactor, m_blendFuncDestFactor );
		size_t i = 0;
		std::for_each( m_textures.begin(), m_textures.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, Texture* > >& texture )
		{
			texture.second.second->BindTexture( GL_TEXTURE0 + i );
			glUniform1i( texture.second.first, i );
			++i;
		} );
		

		std::for_each( m_intUniforms.begin(), m_intUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, int > >& intUniform )
		{
			glUniform1i( intUniform.second.first, intUniform.second.second );
		} );

		std::for_each( m_uintUniforms.begin(), m_uintUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, unsigned int > >& uintUniform )
		{
			glUniform1i( uintUniform.second.first, uintUniform.second.second );
		} );

		std::for_each( m_floatUniforms.begin(), m_floatUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, float > >& floatUniform )
		{
			glUniform1f( floatUniform.second.first, floatUniform.second.second );
		} );

		std::for_each( m_vec2iUniforms.begin(), m_vec2iUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, vec2i > >& vec2iUniform )
		{
			glUniform2i(	vec2iUniform.second.first, 
							vec2iUniform.second.second.x, 
							vec2iUniform.second.second.y );
		} );

		std::for_each( m_vec2fUniforms.begin(), m_vec2fUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, vec2f > >& vec2fUniform )
		{
			glUniform2f(	vec2fUniform.second.first, 
							vec2fUniform.second.second.x, 
							vec2fUniform.second.second.y );
		} );

		std::for_each( m_vec3iUniforms.begin(), m_vec3iUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, vec3i > >& vec3iUniform )
		{
			glUniform3i(	vec3iUniform.second.first, 
							vec3iUniform.second.second.x, 
							vec3iUniform.second.second.y, 
							vec3iUniform.second.second.z );
		} );

		std::for_each( m_vec3fUniforms.begin(), m_vec3fUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, vec3f > >& vec3fUniform )
		{
			glUniform3f(	vec3fUniform.second.first, 
							vec3fUniform.second.second.x, 
							vec3fUniform.second.second.y, 
							vec3fUniform.second.second.z );
		} );

		std::for_each( m_mat4fUniforms.begin(), m_mat4fUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, mat4f > >& mat4fUniform )
		{
			glUniformMatrix4fv( mat4fUniform .second.first,
								1,
								GL_FALSE,
								mat4fUniform.second.second.getRawMatrix() );
		} );

		std::for_each( m_vec2uiUniforms.begin(), m_vec2uiUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, vec2ui > >& vec2uiUniform )
		{
			glUniform2ui(	vec2uiUniform.second.first, 
							vec2uiUniform.second.second.x, 
							vec2uiUniform.second.second.y );
		} );
		
		std::for_each( m_vec3uiUniforms.begin(), m_vec3uiUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, vec3ui > >& vec3uiUniform )
		{
			glUniform3ui(	vec3uiUniform.second.first, 
							vec3uiUniform.second.second.x, 
							vec3uiUniform.second.second.y, 
							vec3uiUniform.second.second.z );
		} );

		activateLights();
	}

	///////////////////////////////////////////////////////
	// Members to add values to material for shader program	
	///////////////////////////////////////////////////////
	void Material::addTexture( const char* uniformName, Texture* texture )
	{	
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_textures[ uniformName ] = std::make_pair< GLint, Texture* >( pLocation, texture );
	}
	void Material::addUniform( const char* uniformName, int value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_intUniforms[ uniformName ] = std::make_pair< GLint, int >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, unsigned int value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_intUniforms[ uniformName ] = std::make_pair< GLint, unsigned int >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName,float value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_floatUniforms[ uniformName ] = std::make_pair< GLint, float >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const vec2i& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
			
		if( pLocation != -1 )
			m_vec2iUniforms[ uniformName ] = std::make_pair< GLint, vec2i >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const vec2f& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_vec2fUniforms[ uniformName ] = std::make_pair< GLint, vec2f >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const vec3i& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_vec3iUniforms[ uniformName ] = std::make_pair< GLint, vec3i >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const vec3f& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_vec3fUniforms[ uniformName ] = std::make_pair< GLint, vec3f >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const mat4f& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
				
		if( pLocation != -1 )
			m_mat4fUniforms[ uniformName ] = std::make_pair< GLint, mat4f >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const vec2ui& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_vec2uiUniforms[ uniformName ] = std::make_pair< GLint, vec2ui >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const vec3ui& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_vec3uiUniforms[ uniformName ] = std::make_pair< GLint, vec3ui >( pLocation, value );
	}
	void Material::addUniform( const char* uniformName, const Light& value )
	{
		if( m_lightUniforms.size() < MAX_NUM_LIGHTS )
		{
			std::string uName = uniformName;
			LightUniformLocs uniformLocs;
			GLuint programID = m_program->getProgramID();

			uniformLocs.pos				= glGetUniformLocation( programID, (uName + ".position").c_str() );
			uniformLocs.color			= glGetUniformLocation( programID, (uName + ".color").c_str() );
			uniformLocs.dir				= glGetUniformLocation( programID, (uName + ".direction").c_str() );
			uniformLocs.ambient			= glGetUniformLocation( programID, (uName + ".ambientFactor" ).c_str() );
			uniformLocs.attenType		= glGetUniformLocation( programID, (uName + ".attenuationType").c_str() );

			uniformLocs.atten.start		= glGetUniformLocation( programID, (uName + ".attenuation.start").c_str() );
			uniformLocs.atten.end		= glGetUniformLocation( programID, (uName + ".attenuation.end").c_str() );
			
			uniformLocs.apert.inner		= glGetUniformLocation( programID, (uName + ".aperture.innerConeDot").c_str() );
			uniformLocs.apert.outer		= glGetUniformLocation( programID, (uName + ".aperture.outerConeDot").c_str() );			
			
			m_lightUniforms[ uniformName ] = std::make_pair< LightUniformLocs, Light >( uniformLocs, value );
		}
	}

	///////////////////////////////////////////////////////
	// Members to update uniforms
	///////////////////////////////////////////////////////
	void Material::updateUniform( const std::string& uniformName, Texture* texture )
	{
		if( m_textures.find( uniformName ) != m_textures.end() )
		{
			m_textures[ uniformName ].second = texture;
		}
	}
	void Material::updateUniform( const std::string& uniformName, int value )
	{
		if( m_intUniforms.find( uniformName ) != m_intUniforms.end() )
		{
			m_intUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, float value )
	{
		if( m_floatUniforms.find( uniformName ) != m_floatUniforms.end() )
		{
			m_floatUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const vec2i& value )
	{
		if( m_vec2iUniforms.find( uniformName ) != m_vec2iUniforms.end() )
		{
			m_vec2iUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const vec2f& value )
	{
		if( m_vec2fUniforms.find( uniformName ) != m_vec2fUniforms.end() )
		{
			m_vec2fUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const vec3i& value )
	{
		if( m_vec3iUniforms.find( uniformName ) != m_vec3iUniforms.end() )
		{
			m_vec3iUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const vec3f& value )
	{
		if( m_vec3fUniforms.find( uniformName ) != m_vec3fUniforms.end() )
		{
			m_vec3fUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const mat4f& value )
	{
		if( m_mat4fUniforms.find( uniformName ) != m_mat4fUniforms.end() )
		{
			m_mat4fUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const vec2ui& value )
	{
		if( m_vec2uiUniforms.find( uniformName ) != m_vec2uiUniforms.end() )
		{
			m_vec2uiUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const vec3ui& value )
	{
		if( m_vec3uiUniforms.find( uniformName ) != m_vec3uiUniforms.end() )
		{
			m_vec3uiUniforms[ uniformName ].second = value;
		}
	}
	void Material::updateUniform( const std::string& uniformName, const Light& light )
	{
		if( m_lightUniforms.find( uniformName ) != m_lightUniforms.end() )
		{
			m_lightUniforms[ uniformName ].second = light;
		}
	}

	void Material::activateLights()
	{
		std::for_each( m_lightUniforms.begin(), m_lightUniforms.end(),
			[&]( std::pair< const std::string, std::pair< LightUniformLocs, Light > >& value )
		{
			std::pair< LightUniformLocs, Light >& light = value.second;

			glUniform3f( light.first.pos,	light.second.position.x,
											light.second.position.y,
											light.second.position.z );

			glUniform4f( light.first.color,	light.second.color.r,
											light.second.color.g,
											light.second.color.b,
											light.second.color.a );

			glUniform3f( light.first.dir,	light.second.direction.x,
											light.second.direction.y,
											light.second.direction.z );

			glUniform1f( light.first.ambient, light.second.ambientFactor );
			
			glUniform4f( light.first.attenType,	(float)light.second.attenuationType.constant,
												(float)light.second.attenuationType.linear,
												(float)light.second.attenuationType.inverse,
												(float)light.second.attenuationType.inverseSquared );

			glUniform1f( light.first.atten.start,	light.second.attenuation.start );
			glUniform1f( light.first.atten.end,		light.second.attenuation.end );

			glUniform1f( light.first.apert.inner,	light.second.aperture.innerConeDot );
			glUniform1f( light.first.apert.outer,	light.second.aperture.outerConeDot );
		} );
	}	
}