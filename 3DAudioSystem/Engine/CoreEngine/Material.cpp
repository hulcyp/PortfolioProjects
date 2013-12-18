#ifdef _WIN32
#include <glew.h>
#endif
#include "Material.h"
#include "ShaderProgram.h"
#include <Renderer.h>

namespace Monky
{
	std::map< std::string, Material* > Material::sm_materials;
	Material* Material::sm_currentlyActiveMaterial = nullptr;
	std::vector< Material::Occlusion > Material::m_occlusionValues;
	std::map< std::string, Material::MaterialType > Material::m_stringToMaterialTypeMap;

	Material::Material( ShaderProgram* program, int srcBlendFunc, int destBlendFunc )
		:	m_program( program )
		,	m_blendFuncSrcFactor( srcBlendFunc )
		,	m_blendFuncDestFactor( destBlendFunc )
		,	m_isActive( false )
		,	m_directOcclusion( 0.0f )
		,	m_reverbOcclusion( 0.0f )
		,	m_matType( NONE )
	{
		if( m_occlusionValues.empty() )
		{
			m_occlusionValues.push_back( Occlusion( 0.0f, 0.0f ) );
			m_occlusionValues.push_back( Occlusion( 0.0f, 0.0f ) );
			m_occlusionValues.push_back( Occlusion( 1.0f, 1.0f ) );
			m_occlusionValues.push_back( Occlusion( 0.5f, 0.5f ) );
			m_occlusionValues.push_back( Occlusion( 0.3f, 0.4f ) );
			m_occlusionValues.push_back( Occlusion( 0.8f, 0.8f ) );
		}
	}
	//--------------------------------------------------------------------------
	Material::~Material()
	{}
	//--------------------------------------------------------------------------
	Material* Material::createOrGetMaterial( const std::string& materialName, const std::string& shaderProgramName )
	{
		auto iter = sm_materials.find( materialName );
		Material* newMaterial = nullptr;//sm_materials[ materialName ];
		if( iter == sm_materials.end() )
		{
			newMaterial = new Material( ShaderProgram::getShaderProgram( shaderProgramName ) );

			newMaterial->addUniform( "uProjectionMatrix", mat4f::identity() );
			newMaterial->addUniform( "uViewMatrix", mat4f::identity() );
			newMaterial->addUniform( "uModelMatrix", mat4f::identity() );
			newMaterial->addUniform( "uMVPMatrix", mat4f::identity() );
			newMaterial->addUniform( "time", 0.0f );
			newMaterial->addUniform( "uUnlit", 0 );

			sm_materials.insert( std::pair< std::string, Material* >( materialName,  newMaterial ) );
		}	
		else
			newMaterial = iter->second;
		return newMaterial;
	}
	//--------------------------------------------------------------------------
	void Material::createMaterial( const std::string& materialName, const std::string& shaderProgramName )
	{
		createOrGetMaterial( materialName, shaderProgramName );
	}
	//--------------------------------------------------------------------------
	Material* Material::getMaterial( const std::string& materialName )
	{
		Material* material = nullptr;
		if( materialName != "" && sm_materials.find( materialName ) != sm_materials.end() )
		{
			material = sm_materials[ materialName ];
		}
		return material;
	}
	//--------------------------------------------------------------------------
	void Material::useMaterial( const std::string& materialName, bool lightingEnabled )
	{
		sm_currentlyActiveMaterial;
		std::map< std::string, Material* >::iterator iter = sm_materials.find( materialName );
		if( materialName != "" && iter != sm_materials.end() )
		{
			Material* newMat = iter->second;
			if( sm_currentlyActiveMaterial == nullptr )
			{
				sm_currentlyActiveMaterial = newMat;
			}
			else if( newMat != sm_currentlyActiveMaterial )
			{
				sm_currentlyActiveMaterial->deactivateProgram();
				sm_currentlyActiveMaterial = newMat;
			}
			sm_currentlyActiveMaterial->updateUniform( "uUnlit", (int)(!lightingEnabled) );
			sm_currentlyActiveMaterial->activateProgram();
		}
	}
	//--------------------------------------------------------------------------
	Material* Material::getActiveMaterial()
	{
		return sm_currentlyActiveMaterial;
	}
	//--------------------------------------------------------------------------
	void Material::updateTimeOnMaterials( float time )
	{
		std::for_each( sm_materials.begin(), sm_materials.end(),
			[&]( std::pair< const std::string, Material* >& material )
		{
			material.second->updateUniform( "time", time );
		} );
	}
	//--------------------------------------------------------------------------
	Material::MaterialType Material::getTypeFromString( const std::string& materialType )
	{
		if( m_stringToMaterialTypeMap.empty() )
		{
			m_stringToMaterialTypeMap[ "AIR" ] = AIR;
			m_stringToMaterialTypeMap[ "STONE" ] = STONE;
			m_stringToMaterialTypeMap[ "WATER" ] = WATER;
			m_stringToMaterialTypeMap[ "GLASS" ] = GLASS;
			m_stringToMaterialTypeMap[ "PLASTIC" ] = PLASTIC;
		}
		auto iter = m_stringToMaterialTypeMap.find( materialType );
		if( iter != m_stringToMaterialTypeMap.end() )
			return iter->second;
		else
			return NONE;
	}
	//--------------------------------------------------------------------------
	void Material::activateProgram()
	{
		if( !m_isActive )
		{
			glUseProgram( m_program->getProgramID() );
			glBlendFunc( m_blendFuncSrcFactor, m_blendFuncDestFactor );		
			m_isActive = true;
		}
	}
	//--------------------------------------------------------------------------
	void Material::deactivateProgram()
	{
		if( m_isActive )
		{			
			size_t i = 0;
			std::for_each( m_textures.begin(), m_textures.end(),
				[&] ( std::pair< const std::string, std::pair< GLint, Texture* > >& texture )
			{
				glActiveTexture( GL_TEXTURE0 + i );
				glBindTexture( GL_TEXTURE_2D, 0 );
				glUniform1i( texture.second.first, -1 );
				++i;
			} );
			glDisable( GL_TEXTURE_2D );
			glUseProgram( 0 );
			m_isActive = false;
		}
	}
	//--------------------------------------------------------------------------
	void Material::updateUniforms()
	{
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

		std::for_each( m_color4fUniforms.begin(), m_color4fUniforms.end(),
			[] ( std::pair< const std::string, std::pair< GLint, Color4f > >& color4fUniform )
		{
			glUniform4f(	color4fUniform.second.first, 
							color4fUniform.second.second.r, 
							color4fUniform.second.second.g,
							color4fUniform.second.second.b,
							color4fUniform.second.second.a);
		} );

		activateLights();		
	}
	//--------------------------------------------------------------------------
	void Material::changeShaderProgram( const std::string& programName )
	{
		deactivateProgram();
		m_program = ShaderProgram::getShaderProgram( programName );
		addUniform( "uProjectionMatrix", mat4f::identity() );
		addUniform( "uViewMatrix", mat4f::identity() );
		addUniform( "uModelMatrix", mat4f::identity() );
		addUniform( "uMVPMatrix", mat4f::identity() );
		addUniform( "time", 0.0f );
		addUniform( "uUnlit", 0 );
		updateUniformLocations();
	}
	//--------------------------------------------------------------------------
	void Material::setMaterialType( MaterialType type )
	{		
		if( type != COUNT && type != NONE )
		{
			m_directOcclusion = m_occlusionValues[ type ].direct;
			m_reverbOcclusion = m_occlusionValues[ type ].reverb;
		}
		m_matType = type;
	}
	//--------------------------------------------------------------------------
	Material::MaterialType Material::getMaterialType() const
	{
		return m_matType;
	}
	//--------------------------------------------------------------------------
	void Material::setDirectOcclusion( float directOcclusion )
	{
		m_matType = CUSTOM;
		m_directOcclusion = directOcclusion;
	}
	//--------------------------------------------------------------------------
	void Material::setReverbOcclusion( float reverbOcclusion )
	{
		m_matType = CUSTOM;
		m_reverbOcclusion = reverbOcclusion;
	}
	//--------------------------------------------------------------------------
	float Material::getDirectOcclusion() const
	{
		return m_directOcclusion;
	}
	//--------------------------------------------------------------------------
	float Material::getReverbOcclusion() const
	{
		return m_reverbOcclusion;
	}

	///////////////////////////////////////////////////////
	// Members to add values to material for shader program	
	///////////////////////////////////////////////////////
	void Material::addTexture( const char* uniformName, Texture* texture )
	{	
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 && texture != nullptr )
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
	void Material::addUniform( const char* uniformName, const Light* value )
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
			
			m_lightUniforms[ uniformName ] = std::make_pair< LightUniformLocs, const Light* >( uniformLocs, value );
		}
	}
	void Material::addUniform( const char* uniformName, const Color4f& value )
	{
		GLint pLocation = glGetUniformLocation( m_program->getProgramID(), uniformName );
		
		if( pLocation != -1 )
			m_color4fUniforms[ uniformName ] = std::make_pair< GLint, Color4f >( pLocation, value );
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
	void Material::updateUniform( const std::string& uniformName, const Light* light )
	{
		if( m_lightUniforms.find( uniformName ) != m_lightUniforms.end() )
		{
			m_lightUniforms[ uniformName ].second = light;
		}
	}
	//----------------------------------------------------------------------
	void Material::cleanupMaterials()
	{
		MAP_CLEANUP( sm_materials );
	}
	//----------------------------------------------------------------------
	void Material::activateLights()
	{
		std::for_each( m_lightUniforms.begin(), m_lightUniforms.end(),
			[&]( std::pair< const std::string, std::pair< LightUniformLocs, const Light* > >& value )
		{
			std::pair< LightUniformLocs, const Light* >& light = value.second;

			glUniform3f( light.first.pos,	light.second->position.x,
											light.second->position.y,
											light.second->position.z );

			glUniform4f( light.first.color,	light.second->color.r,
											light.second->color.g,
											light.second->color.b,
											light.second->color.a );

			glUniform3f( light.first.dir,	light.second->direction.x,
											light.second->direction.y,
											light.second->direction.z );

			glUniform1f( light.first.ambient, light.second->ambientFactor );
			
			glUniform4f( light.first.attenType,	(float)light.second->attenuationType.constant,
												(float)light.second->attenuationType.linear,
												(float)light.second->attenuationType.inverse,
												(float)light.second->attenuationType.inverseSquared );

			glUniform1f( light.first.atten.start,	light.second->attenuation.start );
			glUniform1f( light.first.atten.end,		light.second->attenuation.end );

			glUniform1f( light.first.apert.inner,	light.second->aperture.innerConeDot );
			glUniform1f( light.first.apert.outer,	light.second->aperture.outerConeDot );
		} );
	}	
	void Material::updateUniform( const std::string& uniformName, const Color4f& value )
	{
		if( m_color4fUniforms.find( uniformName ) != m_color4fUniforms.end() )
		{
			m_color4fUniforms[ uniformName ].second = value;
		}
	}

	void Material::updateUniformLocations()
	{
		std::for_each( m_textures.begin(), m_textures.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, Texture* > >& texture )
		{
			texture.second.first = glGetUniformLocation( m_program->getProgramID(), texture.first.c_str() );
		} );
		std::for_each( m_intUniforms.begin(), m_intUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, int > >& intUniform )
		{
			intUniform.second.first = glGetUniformLocation( m_program->getProgramID(), intUniform.first.c_str() );
		} );

		std::for_each( m_uintUniforms.begin(), m_uintUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, unsigned int > >& uintUniform )
		{
			uintUniform.second.first = glGetUniformLocation( m_program->getProgramID(), uintUniform.first.c_str() );
		} );

		std::for_each( m_floatUniforms.begin(), m_floatUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, float > >& floatUniform )
		{
			floatUniform.second.first = glGetUniformLocation( m_program->getProgramID(), floatUniform.first.c_str() );
		} );

		std::for_each( m_vec2iUniforms.begin(), m_vec2iUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, vec2i > >& vec2iUniform )
		{
			vec2iUniform.second.first = glGetUniformLocation( m_program->getProgramID(), vec2iUniform.first.c_str() );
		} );

		std::for_each( m_vec2fUniforms.begin(), m_vec2fUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, vec2f > >& vec2fUniform )
		{
			vec2fUniform.second.first = glGetUniformLocation( m_program->getProgramID(), vec2fUniform.first.c_str() );;
		} );

		std::for_each( m_vec3iUniforms.begin(), m_vec3iUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, vec3i > >& vec3iUniform )
		{
			vec3iUniform.second.first = glGetUniformLocation( m_program->getProgramID(), vec3iUniform.first.c_str() );
		} );

		std::for_each( m_vec3fUniforms.begin(), m_vec3fUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, vec3f > >& vec3fUniform )
		{
			vec3fUniform.second.first = glGetUniformLocation( m_program->getProgramID(), vec3fUniform.first.c_str() );;
		} );

		std::for_each( m_mat4fUniforms.begin(), m_mat4fUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, mat4f > >& mat4fUniform )
		{
			mat4fUniform.second.first = glGetUniformLocation( m_program->getProgramID(), mat4fUniform.first.c_str() );
		} );

		std::for_each( m_vec2uiUniforms.begin(), m_vec2uiUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, vec2ui > >& vec2uiUniform )
		{
			vec2uiUniform.second.first = glGetUniformLocation( m_program->getProgramID(), vec2uiUniform.first.c_str() );
		} );
		
		std::for_each( m_vec3uiUniforms.begin(), m_vec3uiUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, vec3ui > >& vec3uiUniform )
		{
			vec3uiUniform.second.first = glGetUniformLocation( m_program->getProgramID(), vec3uiUniform.first.c_str() );
		} );

		std::for_each( m_color4fUniforms.begin(), m_color4fUniforms.end(),
			[&] ( std::pair< const std::string, std::pair< GLint, Color4f > >& color4fUniform )
		{
			color4fUniform.second.first = glGetUniformLocation( m_program->getProgramID(), color4fUniform.first.c_str() );
		} );
	}
}