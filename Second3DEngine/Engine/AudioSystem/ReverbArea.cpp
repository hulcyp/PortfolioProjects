#include "ReverbArea.h"
#include "ReverbAreaImpl.h"
#include "AudioSystem.h"
#include "XMLParser.h"

namespace Monky
{
	std::map< std::string, ReverbArea* > ReverbArea::sm_reverbAreas;
	
	ReverbArea::ReverbArea( const std::string& name )
		:	m_impl( new ReverbAreaImpl( name ) )
	{
		AudioSystem::getInstance()->createReverbArea( *this );
	}
	//----------------------------------------------------------------------
	ReverbArea::~ReverbArea()
	{
		SAFE_DELETE( m_impl );
	}
	//----------------------------------------------------------------------
	void ReverbArea::set3DAttributes( const vec3f& pos, float min, float max )
	{
		m_impl->set3DAttributes( pos, min, max );
	}
	//----------------------------------------------------------------------
	void ReverbArea::setPosition( const vec3f& pos )
	{
		m_impl->setPosition( pos );
	}
	//----------------------------------------------------------------------
	void ReverbArea::setMinMaxDist( float min, float max )
	{
		m_impl->setMinMaxDist( min, max );
	}
	//----------------------------------------------------------------------
	void ReverbArea::setProperties( const MonkyReverbProperties& reverbProp )
	{
		m_impl->setProperties( &reverbProp );
	}
	//----------------------------------------------------------------------
	const vec3f& ReverbArea::getPosition() const
	{
		return m_impl->getPosition();
	}
	//----------------------------------------------------------------------
	void ReverbArea::getMinMaxDist( float& minOut, float& maxOut ) const
	{
		m_impl->getMinMaxDist( minOut, maxOut );
	}
	//----------------------------------------------------------------------
	void ReverbArea::renderDebugInfo( Camera* camera )
	{
		m_impl->renderDebugInfo( camera );
	}
	//----------------------------------------------------------------------
	// Static member functions
	//----------------------------------------------------------------------
	ReverbArea* ReverbArea::createOrGetReverbArea( const std::string& reverbAreaName, const MonkyReverbProperties& reverbProps )
	{
		ReverbArea*& newReverbArea = sm_reverbAreas[ reverbAreaName ];
		if( !newReverbArea )
		{
			newReverbArea = new ReverbArea( reverbAreaName );
			newReverbArea->setProperties( reverbProps );
		}
		return newReverbArea;
	}
	//----------------------------------------------------------------------
	void ReverbArea::createReverbArea( const std::string& reverbAreaName, const MonkyReverbProperties& reverbProps )
	{
		createOrGetReverbArea( reverbAreaName, reverbProps );
	}
	//----------------------------------------------------------------------
	ReverbArea* ReverbArea::getReverbArea( const std::string& reverbAreaName )
	{
		auto iter = sm_reverbAreas.find( reverbAreaName );
		if( iter != sm_reverbAreas.end() )
			return iter->second;
		else
			return nullptr;
	}
	//----------------------------------------------------------------------
	void ReverbArea::deleteReverbArea( const std::string& reverbAreaName )
	{
		auto iter = sm_reverbAreas.find( reverbAreaName );
		if( iter != sm_reverbAreas.end() )
		{
			SAFE_DELETE( iter->second );
			sm_reverbAreas.erase( iter );
		}
	}
	//----------------------------------------------------------------------
	void ReverbArea::loadReverbAreasFromXMLFile( const std::string& filePath, const std::string& file )
	{
		std::string fullFilePath = filePath + file;
		XMLParser parser( fullFilePath.c_str() );
		XMLDocument& doc = parser.getDocument();

		XMLNode* root = doc.FirstChildElement( "ReverbAreas" );
		XMLNode* reverbAreaElement = nullptr;
		for( reverbAreaElement = root->FirstChildElement( "ReverbArea" ); reverbAreaElement != nullptr; reverbAreaElement = reverbAreaElement->NextSiblingElement( "ReverbArea" ) )
		{
			parser.validateXMLAttributes( reverbAreaElement, "name,pos,minDist,maxDist", "preset" );
			std::string raName = parser.getXMLAttributeAsString( reverbAreaElement, "name", "" );
			vec3f pos = parser.getXMLAttributeAsVec3( reverbAreaElement, "pos", vec3f() );
			float minDist = parser.getXMLAttributeAsFloat( reverbAreaElement, "minDist", 0.0f );
			float maxDist = parser.getXMLAttributeAsFloat( reverbAreaElement, "maxDist", 0.1f );
			std::string preset = parser.getXMLAttributeAsString( reverbAreaElement, "preset", "MONKY_PRESET_OFF" );
			ReverbArea* ra = ReverbArea::createOrGetReverbArea( raName, getMonkyReverbPresetFromString( preset ) );
			ra->set3DAttributes( pos, minDist, maxDist );
		}
	}
	//----------------------------------------------------------------------
	void ReverbArea::cleanupReverbAreas()
	{
		MAP_CLEANUP( sm_reverbAreas );
	}
	//----------------------------------------------------------------------
	void ReverbArea::renderAllReverbAreaDebugInfo( Camera* camera )
	{
		for( auto iter = sm_reverbAreas.begin(); iter != sm_reverbAreas.end(); ++iter )
		{
			iter->second->renderDebugInfo( camera );
		}
	}

	//----------------------------------------------------------------------
	// Utility functions
	//----------------------------------------------------------------------
	MonkyReverbProperties getMonkyReverbPresetFromString( const std::string& reverbPreset )
	{
		static std::map< std::string, MonkyReverbProperties > stringToPropMapping;
		if( stringToPropMapping.empty() )
		{
			stringToPropMapping[ "MONKY_PRESET_OFF" ]			    = MONKY_PRESET_OFF              ;
			stringToPropMapping[ "MONKY_PRESET_GENERIC" ]			= MONKY_PRESET_GENERIC          ;
			stringToPropMapping[ "MONKY_PRESET_PADDEDCELL" ]		= MONKY_PRESET_PADDEDCELL       ;
			stringToPropMapping[ "MONKY_PRESET_ROOM" ]				= MONKY_PRESET_ROOM             ;
			stringToPropMapping[ "MONKY_PRESET_BATHROOM" ]			= MONKY_PRESET_BATHROOM         ;
			stringToPropMapping[ "MONKY_PRESET_LIVINGROOM" ]		= MONKY_PRESET_LIVINGROOM       ;
			stringToPropMapping[ "MONKY_PRESET_STONEROOM" ]			= MONKY_PRESET_STONEROOM        ;
			stringToPropMapping[ "MONKY_PRESET_AUDITORIUM" ]		= MONKY_PRESET_AUDITORIUM       ;
			stringToPropMapping[ "MONKY_PRESET_CONCERTHALL" ]		= MONKY_PRESET_CONCERTHALL      ;
			stringToPropMapping[ "MONKY_PRESET_CAVE" ]				= MONKY_PRESET_CAVE             ;
			stringToPropMapping[ "MONKY_PRESET_ARENA" ]				= MONKY_PRESET_ARENA            ;
			stringToPropMapping[ "MONKY_PRESET_HANGAR" ]			= MONKY_PRESET_HANGAR           ;
			stringToPropMapping[ "MONKY_PRESET_CARPETTEDHALLWAY" ]	= MONKY_PRESET_CARPETTEDHALLWAY ;
			stringToPropMapping[ "MONKY_PRESET_HALLWAY" ]			= MONKY_PRESET_HALLWAY          ;
			stringToPropMapping[ "MONKY_PRESET_STONECORRIDOR" ]		= MONKY_PRESET_STONECORRIDOR    ;
			stringToPropMapping[ "MONKY_PRESET_ALLEY" ]				= MONKY_PRESET_ALLEY            ;
			stringToPropMapping[ "MONKY_PRESET_FOREST" ]			= MONKY_PRESET_FOREST           ;	
			stringToPropMapping[ "MONKY_PRESET_CITY" ]				= MONKY_PRESET_CITY             ;
			stringToPropMapping[ "MONKY_PRESET_MOUNTAINS" ]			= MONKY_PRESET_MOUNTAINS        ;
			stringToPropMapping[ "MONKY_PRESET_QUARRY" ]			= MONKY_PRESET_QUARRY           ;
			stringToPropMapping[ "MONKY_PRESET_PLAIN" ]				= MONKY_PRESET_PLAIN            ;
			stringToPropMapping[ "MONKY_PRESET_PARKINGLOT" ]		= MONKY_PRESET_PARKINGLOT       ;
			stringToPropMapping[ "MONKY_PRESET_SEWERPIPE" ]			= MONKY_PRESET_SEWERPIPE        ;
			stringToPropMapping[ "MONKY_PRESET_UNDERWATER" ]		= MONKY_PRESET_UNDERWATER       ;				
		}

		auto iter = stringToPropMapping.find( reverbPreset );
		if( iter != stringToPropMapping.end() )
			return iter->second;
		else
			return MONKY_PRESET_OFF;
	}
}