#pragma once
#include "tinyxml2.h"
#include "CommonEngineIncludes.h"
#include <RandRangeInt.h>
#include <RandRangeFloat.h>
#include <WeightedRandPool.h>

namespace pdh
{
	typedef tinyxml2::XMLDocument XMLDocument;
	typedef tinyxml2::XMLElement XMLNode;
	typedef tinyxml2::XMLAttribute XMLAttribute;
	typedef std::vector< std::string >::iterator StrVecIter;


	class XMLParser
	{
	public:
		XMLParser( const char* fileLocation );

		XMLDocument& getDocument() { return m_document; }

		float getXMLAttributeAsFloat( const XMLNode* element, const char* attributeName, float defaultValue );
		int getXMLAttributeAsInt( const XMLNode* element, const char* attributeName, int defaultValue );
		bool getXMLAttributeAsBool( const XMLNode* element, const char* attributeName, bool defaultValue );
		Color4f getXMLAttributeAsColor( const XMLNode* element, const char* attributeName, const Color4f& defaultValue );
		vec2f getXMLAttributeAsVec2( const XMLNode* element, const char* attributeName, const vec2f& defaultValue );
		vec3f getXMLAttributeAsVec3( const XMLNode* element, const char* attributeName, const vec3f& defaultValue );
		vec2i getXMLAttributeAsVec2i( const XMLNode* element, const char* attributeName, const vec2i& defaultValue );
		vec3i getXMLAttributeAsVec3i( const XMLNode* element, const char* attributeName, const vec3i& defaultValue );
		WeightedRandPool< RandRangeFloat > getXMLAttributeAsWeightedRandPoolFloatRange( const XMLNode* element, const char* attributeName );
		WeightedRandPool< RandRangeInt > getXMLAttributeAsWeightedRandPoolIntRange( const XMLNode* element, const char* attributeName );
		WeightedRandPool< std::string > getXMLAttributeAsWeightedRandPoolString( const XMLNode* element, const char* attributeName );
		std::vector< std::string > getXMLAttributeAsListOfStrings( const XMLNode* element, const char* attributeName );
		std::string getXMLAttributeAsString( const XMLNode* element, const char* attributeName, const std::string& defaultValue );
		std::string getXMLElementPCDataAsString( const XMLNode* element );

		// Validation functions
		bool validateXMLChildElements( const XMLNode* element, const char* commaSeparatedListOfRequiredChildren, const char* commaSeparatedListOfOptionalChildren );
		bool validateXMLAttributes( const XMLNode* element, const char* commaSeparatedListOfRequiredAttributes, const char* commaSeparatedListOfOptionalAttributes );

	private:
		StrVecIter findIgnoreCase( StrVecIter first, StrVecIter last, std::string val );
		void outputXMLErrorMsgToWindow( const char* format, ... );
		void outputXMLErrorMsgToConsole( const char* format, ... );
		std::string reconstructXMLTag( const XMLNode* element ); 

		std::string m_fileLocation;
		XMLDocument m_document;

		static const int MAX_MESSAGE_SIZE;
		static const char* ms_ErrorTitle;
		static const char* ms_ErrorMsg;
		static const char* ms_ErrorLocation;
		static const char* ms_caseErrorMsg;
		static const char* ms_caseErrorMsg2;
		static const char* ms_invalidDataMsg;

	};
}