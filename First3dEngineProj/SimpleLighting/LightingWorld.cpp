#include "LightingWorld.h"
#include <Mesh.h>
#include <Renderer.h>
#include <Material.h>
#include <CommonEngineIncludes.h>
#include <Model.h>
#include <ShaderProgram.h>
#include <Font.h>
#include <FrameBufferObject.h>

namespace pdh
{
	LightingWorld::LightingWorld( double deltaTime, int width, int height, float fov,  const char* windowTitle )
		:	GameWorld( deltaTime, width, height, fov, windowTitle )
		,	m_lightPos( 5.0f, 5.0f, 15.0f )
		,	m_unlit( false )
		,	m_renderTangentSpace( true )
		,	m_enableSpecular( true )
		,	m_enableFog( true )
		,	m_enableDiffuse( true )
		,	m_enableEmissive( true )
		,	m_enableParallax( true )
		,	m_intertiaMatrix( mat4f::identity() )
		,	m_c( 1.0f )
		,	m_k( 1.0f )
		,	m_cubeState( vec3f( 0.0f, 0.0f, 0.0f ), vec3f(), mat4f::identity(), vec3f() )
		,	m_springAttachedLoc( 5.0f, 5.0f, 5.0f )
	{
		m_material = m_renderer->getMaterial( "cobblestone" );
		m_fontMaterial = m_renderer->getMaterial( "mainFontFiery_72" );
		m_fieryFont = m_renderer->getFont( "mainFontFiery_72" );

		m_fontMaterial->addTexture(  "uTexels", new Texture( "../data/fonts/MainFont_EN_00_72.png" ) );
		m_fontMaterial->addTexture( "uNoiseTexture", m_renderer->getTexture( "NoiseTexture4" ) );
		
		Mesh* cube = Mesh::generateCube( 10.0f );
		m_cube = new Actor( "cube" );
		m_cube->setModel( new Model( cube ) );
		m_actorManager.addActor( m_cube );
		Actor* cubeActor;
		int count = 0;
		for( int i = 0; i < 5; ++i, ++count )
		{
			for( int j = 0; j < 5; ++j, ++count )
			{
				cube->setMaterial( "cobblestone" );
				std::stringstream ss;
				std::string id;
				ss << "cube_" << count;
				ss >> id;
				cubeActor = new Actor( id );
				cubeActor->setModel( new Model( cube ) );
				cubeActor->setPosition( vec3f( i*20.0f, 0.0f, j*20.0f ) );
				m_actorManager.addActor( cubeActor );
			}
		}
		createLights();

		//m_renderer->pushFBO( m_screenWidth, m_screenHeight, "OutlineMaterial" );
		m_renderer->pushFBO( m_screenWidth, m_screenHeight, "GlowMapGenMaterial" );
		m_renderer->pushFBO( m_screenWidth, m_screenHeight, "BlurMaterialX" );
		m_renderer->pushFBO( m_screenWidth, m_screenHeight, "BlurMaterialY" );
		//m_renderer->pushFBO( m_screenWidth, m_screenHeight, "BlurMaterial" );
		//m_renderer->pushFBO( m_screenWidth, m_screenHeight, "BlurMaterial" );
		m_renderer->pushFBO( m_screenWidth, m_screenHeight, "GlowShaderMaterial" );
		m_renderer->pushFBO( m_screenWidth, m_screenHeight, "FireishMaterial" );

	}

	LightingWorld::~LightingWorld()
	{}

	bool LightingWorld::onKeyDown( unsigned char keyCode, int x, int y )
	{
		bool used = false;

		used = GameWorld::onKeyDown( keyCode, x, y );
		if( !used )
		{
			keyCode = toupper( keyCode );
			switch( keyCode )
			{
			case 'L':
				m_unlit = !m_unlit;
				m_material->updateUniform( "unlit", (int)m_unlit );
				consolePrintColorf( "Unlit: %d", color::GREEN, m_unlit );
				used = true;
				break;
			case 'T':
				m_renderTangentSpace = !m_renderTangentSpace; 
				m_material->updateUniform( "renderTangentSpace", (int)m_renderTangentSpace );
				consolePrintColorf( "Tangent Space: %d", color::GREEN, m_renderTangentSpace );
				used = true;
				break;
			case 'R':
				m_enableSpecular = !m_enableSpecular;
				m_material->updateUniform( "enableSpecular", (int)m_enableSpecular );
				consolePrintColorf( "Specular: %d", color::GREEN, m_enableSpecular );
				used = true;
				break;
			case 'F':
				m_enableFog = !m_enableFog;
				m_material->updateUniform( "enableFog", (int)m_enableFog );
				consolePrintColorf( "Fog: %d", color::GREEN, m_enableFog );
				used = true;
				break;
			case 'U':
				m_enableDiffuse = !m_enableDiffuse;
				m_material->updateUniform( "enableDiffuse", (int)m_enableDiffuse );
				consolePrintColorf( "Diffuse: %d", color::GREEN, m_enableDiffuse );
				used = true;
				break;
			case 'M':
				m_enableEmissive = !m_enableEmissive;
				m_material->updateUniform( "enableEmissive", (int)m_enableEmissive );
				consolePrintColorf( "Emissive: %d", color::GREEN, m_enableEmissive );
				used = true;
				break;
			case 'P':
				m_enableParallax = !m_enableParallax;
				m_material->updateUniform( "enableParallax", (int)m_enableParallax );
				consolePrintColorf( "Parallax: %d", color::GREEN, m_enableParallax );
				used = true;
				break;
			case '1':
				createPointGDO( 10.0f, vec3f(), m_time + 10.0f, color::YELLOW );
				createActorGDO( "cube", m_time + 10.0f, color::GREEN );
				createTextGDO( "cube", "pos", m_time + 10.0f, color::BLUE );
				used = true;
				break;	
			}
		}

		return used;
	}

	void LightingWorld::updateSimulation()
	{
		GameWorld::updateSimulation();
		updateLightPositions();
		m_lightPos.x = 15.0f * std::sin( m_time );
		m_lightPos.y = 15.0f * std::cos( m_time );
		m_material->updateUniform( "lightPos", m_lightPos );
		m_material->updateUniform( "time", m_time );
		m_fontMaterial->updateUniform( "time", m_time );
		m_material->updateUniform( "uLights[0]", m_lights[0] );
		for( int i = 0; i < MAX_NUM_LIGHTS; ++i )
		{
			createPointGDO( 1.0f, m_lights[i].position, 0.0f, m_lights[i].color );
			createArrowGDO( m_lights[i].direction, 1.0f, m_lights[i].position );
		}
		if( m_camera )
		{
			m_material->updateUniform( "cameraPos", m_camera->getPosition() );
		}
		//createPointGDO( 1.0f, m_lightPos, 0.0f, color::YELLOW );
		//createSphereGDO( 1.0f, 5, 5, m_lightPos, 0.0f, color::DARK_GREEN );
		//mat4f rot(	std::cos( m_time ), std::sin( m_time ), 0.0f, 0.0f,
		//			-std::sin( m_time ), std::cos( m_time ), 0.0f, 0.0f,
		//			0.0f, 0.0f, 1.0f, 0.0f,
		//			0.0f, 0.0f, 0.0f, 1.0f );
		//createOrientationGDO( 3.0f, m_lightPos, rot, 0.0f );
		//vec3f dir = m_lightPos;
		//dir.normalize();
		//createArrowGDO( dir, 3.0f, vec3f() );
		//updateRigidSimulation();
		//m_cube->setPosition( m_cubeState.CoM );
		//m_cube->setRotation( m_cubeState.Rotation );
	}

	void LightingWorld::updateDisplay()
	{			
		glClearColor( 0.5f, 0.2f, 0.2f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		m_renderer->bindFBOQueue();
		GameWorld::updateDisplay();
		matStackf& modelStack = m_renderer->getModelStack();
		modelStack.push();
		modelStack.loadIdentity();
		m_renderer->renderAxis( 20.0f );
		modelStack.pop();

		m_renderer->renderFBOQueue();
		
		GameWorld::renderConsole();
		GameWorld::renderDebugHUD();
		glutSwapBuffers();
	}

	void LightingWorld::updateRigidSimulation()
	{

		float dt = static_cast< float >( DELTA_TIME );

		// translation
		vec3f worldSpringAttached = m_cubeState.CoM + transformVector( m_springAttachedLoc, m_cubeState.Rotation );
		
		vec3f forces = vec3f( 0.0f, -9.81f, 0.0f ) + worldSpringAttached * ( -m_k ) + m_cubeState.VelCoM * ( -m_c );
		
		m_cubeState.CoM = m_cubeState.CoM + m_cubeState.VelCoM * dt;
		m_cubeState.VelCoM = m_cubeState.VelCoM + forces * dt;
		
		// Rotation
		vec3f moment = ( worldSpringAttached - m_cubeState.CoM ).cross( worldSpringAttached * ( -m_k ) ) + m_omega * ( -m_c );
		
		mat4f dRot = calculateDerivativeOfRot();
		m_cubeState.AngularMomCoM = m_cubeState.AngularMomCoM + moment * dt;
		m_cubeState.Rotation = m_cubeState.Rotation + dRot * dt;
		orthoNormalize( m_cubeState.Rotation );
	}

	mat4f LightingWorld::calculateDerivativeOfRot()
	{
		m_omega = transformVector( m_cubeState.AngularMomCoM, (m_cubeState.Rotation.getTranspose() * m_intertiaMatrix * m_cubeState.Rotation) );

		mat4f matOmega( 0, -m_omega.z, m_omega.y, 0,
						m_omega.z, 0, -m_omega.x, 0,
						-m_omega.y, m_omega.x, 0, 0,
						0, 0, 0, 0 );

		mat4f dRot = matOmega * m_cubeState.Rotation;
		return dRot;
	}

	void LightingWorld::createLights()
	{
		m_lights[0].position = vec3f( 0.0f, 10.0f, 0.0f);
		m_lights[0].direction = vec3f( -1.0f, -1.0f, -1.0f );
		m_lights[0].color = color::WHITE;
		m_lights[0].attenuation.start = 15.0f;
		m_lights[0].attenuation.end = 20.0f;
		m_lights[0].aperture.innerConeDot = cos( degreesToRadians( 10.0f ) );
		m_lights[0].aperture.outerConeDot = cos( degreesToRadians( 15.0f ) );
		m_lights[0].attenuationType.linear = 1;
		m_lights[0].ambientFactor = 0.0f;
		m_material->addUniform( "uLights[0]", m_lights[0] );
		
		m_lights[1].position = vec3f( 0.0f, 10.0f, 0.0f);
		m_lights[1].direction = vec3f( 0.0f, -51.0f, 0.0f );
		m_lights[1].color = color::WHITE;
		m_lights[1].color.a = 0.0f;				// Indicates directinoal light
		m_lights[1].attenuation.start = 10.0f;
		m_lights[1].attenuation.end = 20.0f;
		m_lights[1].aperture.innerConeDot = -1.0f;//cos( degreesToRadians( 10.0f ) );
		m_lights[1].aperture.outerConeDot = -1.0f;//cos( degreesToRadians( 20.0f ) );
		m_lights[1].attenuationType.linear = 1;
		m_material->addUniform( "uLights[1]", m_lights[1] );

		m_lights[2].position = vec3f( 30.0f, -5.0f, 10.0f);
		m_lights[2].direction = vec3f( 1.0f, -1.0f, 1.0f );
		m_lights[2].color = color::RED;
		m_lights[2].attenuation.start = 10.0f;
		m_lights[2].attenuation.end = 15.0f;
		m_lights[2].aperture.innerConeDot = -1.0f;//cos( degreesToRadians( 10.0f ) );
		m_lights[2].aperture.outerConeDot = -1.0f;//cos( degreesToRadians( 20.0f ) );
		m_lights[2].attenuationType.inverse = 1;
		m_material->addUniform( "uLights[2]", m_lights[2] );
		
		for( int i = 3; i < MAX_NUM_LIGHTS; ++i )
		{
			m_lights[i].position = vec3f( 10.0f, 5.0f, 10.0f );
			m_lights[i].direction = vec3f( 0.0f, 1.0f, 0.0f );
			m_lights[i].color = color::BLUE;
			m_lights[i].attenuation.start = 19.0f;
			m_lights[i].attenuation.end = 25.0f;
			m_lights[i].attenuationType.inverse = 1;
			m_lights[i].aperture.innerConeDot = cos( degreesToRadians( 10.0f ) );
			m_lights[i].aperture.outerConeDot = cos( degreesToRadians( 15.0f ) );
			std::stringstream ss;
			ss << "uLights[" << i << "]";
			std::string str;
			ss >> str;
			m_material->addUniform( str.c_str(), m_lights[i] );
		}
	}

	void LightingWorld::updateLightPositions()
	{
		m_lights[0].direction = vec3f( sin( m_time ), -1.0f, sin( m_time ) );
		m_material->updateUniform( "uLights[0]", m_lights[0] );

		for( int i = 3; i < 5; ++i )
		{
			std::stringstream ss;
			ss << "uLights[" << i << "]";
			std::string str;
			ss >> str;
			m_lights[i].position = vec3f( 17.0f, 0.0f, 17.0f ) + vec3f( cos( m_time - i ), 5.0f, sin( m_time - i ) ) * 5.0f;
			m_lights[i].direction = vec3f( 0.0f, -1.0f, 0.0f );
			m_material->updateUniform( str.c_str(), m_lights[i] );
		}
		for( int i = 5; i < MAX_NUM_LIGHTS; ++i )
		{
			std::stringstream ss;
			ss << "uLights[" << i << "]";
			std::string str;
			ss >> str;
			m_lights[i].position = vec3f( cos( m_time - i ), -5.0f, sin( m_time - i ) ) * 5.0f;
			m_material->updateUniform( str.c_str(), m_lights[i] );
		}
	}
}