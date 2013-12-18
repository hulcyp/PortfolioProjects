#include "XMLParser.h"
#include <algorithm>
#include "MonkyException.h"
#include "FileManager.h"
#include "GameApp.h"

namespace Monky
{
	
	const int XMLParser::MAX_MESSAGE_SIZE = 2048; 
	const char* XMLParser::ms_ErrorTitle = "XML Format Error";
	const char* XMLParser::ms_ErrorMsg = "Continue program execution?\n";
	const char* XMLParser::ms_ErrorLocation = "XML error occured at %s(0): ";
	const char* XMLParser::ms_caseErrorMsg = "Warning! XML is case sensitive. \nIncorrect case for the %s XML %s: \t\"%s\"\n";
	const char* XMLParser::ms_caseErrorMsg2 = "The correct case is: \t\t\t\t\"%s\"\n\n  > %s\n";
	const char* XMLParser::ms_invalidDataMsg = "Invalid data. Expected data of type: %s, for attribute: %s. \n  > %s";

	XMLParser::XMLParser( const char* fileLocation, bool showErrorDialog )
		: m_fileLocation( fileLocation )
	{
		int bufferSize = -1;
		char* xmlBuffer = getFileManager().readDataFromFile( fileLocation, bufferSize );
		
		int errorID = 0;
		if( bufferSize != -1 )
			errorID = m_document.Parse( xmlBuffer, bufferSize ); 
		else
			outputXMLErrorMsgToWindow( "Unable to open xml file" );

		if(  errorID != tinyxml2::XML_NO_ERROR && showErrorDialog )
		{
			outputXMLErrorMsgToWindow( m_document.GetErrorStr1() );
		}
		SAFE_DELETE( xmlBuffer );
	}

	float XMLParser::getXMLAttributeAsFloat( const XMLNode* element, const char* attributeName, float defaultValue )
	{
		float attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			if( !stringToType( text, &attrib ) )
			{
				attrib = defaultValue;
			}
		}
		return attrib;
	}

	int XMLParser::getXMLAttributeAsInt( const XMLNode* element, const char* attributeName, int defaultValue )
	{
		int attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			if( !stringToType( text, &attrib ) )
			{
				attrib = defaultValue;
			}
		}
		return attrib;
	}

	bool XMLParser::getXMLAttributeAsBool( const XMLNode* element, const char* attributeName, bool defaultValue )
	{
		bool attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			if( !stringToType( text, &attrib ) )
			{
				attrib = defaultValue;
			}
		}
		return attrib;
	}

	Color4f XMLParser::getXMLAttributeAsColor( const XMLNode* element, const char* attributeName, const Color4f& defaultValue )
	{
		Color4f attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			try
			{
				attrib = parseStringToColor4f( text, defaultValue );
			}
			catch( std::exception e )
			{
				outputXMLErrorMsgToConsole( e.what() );
				outputXMLErrorMsgToWindow( ms_invalidDataMsg, "Color4", attributeName, reconstructXMLTag( element ).c_str() );
			}
		}
		return attrib;
	}

	vec2f XMLParser::getXMLAttributeAsVec2( const XMLNode* element, const char* attributeName, const vec2f& defaultValue )
	{
		vec2f attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			try
			{
				attrib = parseStringToVec2( text, defaultValue );
			}
			catch( std::exception e )
			{
				outputXMLErrorMsgToConsole( e.what() );
				outputXMLErrorMsgToWindow( ms_invalidDataMsg, "Vector2", attributeName, reconstructXMLTag( element ).c_str() );
			}
		}
		return attrib;
	}

	vec3f XMLParser::getXMLAttributeAsVec3( const XMLNode* element, const char* attributeName, const vec3f& defaultValue )
	{
		vec3f attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			try
			{
				attrib = parseStringToVec3( text, defaultValue );
			}
			catch( std::exception e )
			{
				outputXMLErrorMsgToConsole( e.what() );
				outputXMLErrorMsgToWindow( ms_invalidDataMsg, "Vector3", attributeName, reconstructXMLTag( element ).c_str() );
			}
		}
		return attrib;
	}

	vec2i XMLParser::getXMLAttributeAsVec2i( const XMLNode* element, const char* attributeName, const vec2i& defaultValue )
	{
		vec2i attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			try
			{
				attrib = parseStringToVec2( text, defaultValue );
			}
			catch( std::exception e )
			{
				outputXMLErrorMsgToConsole( e.what() );
				outputXMLErrorMsgToWindow( ms_invalidDataMsg, "Vector2", attributeName, reconstructXMLTag( element ).c_str() );
			}
		}
		return attrib;
	}

	vec3i XMLParser::getXMLAttributeAsVec3i( const XMLNode* element, const char* attributeName, const vec3i& defaultValue )
	{
		vec3i attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			try
			{
				attrib = parseStringToVec3( text, defaultValue );
			}
			catch( std::exception e )
			{
				outputXMLErrorMsgToConsole( e.what() );
				outputXMLErrorMsgToWindow( ms_invalidDataMsg, "Vector3", attributeName, reconstructXMLTag( element ).c_str() );
			}
		}
		return attrib;
	}
		
	WeightedRandPool< RandRangeFloat > XMLParser::getXMLAttributeAsWeightedRandPoolFloatRange( const XMLNode* element, const char* attributeName )
	{
		WeightedRandPool< RandRangeFloat > pool;
		const char* text = element->Attribute( attributeName );
		
		if( text )
		{
			std::vector< std::string > poolTokens;
			stringTokenizer( text, poolTokens, "," );
			for( size_t i = 0; i < poolTokens.size(); ++i )
			{
				std::vector< std::string > weightTokens;
				stringTokenizer( poolTokens[i], weightTokens, "@" );
				if( weightTokens.size() == 1 )
				{
					pool.add( RandRangeFloat( poolTokens[i] ), 1.0f );
				}
				else
				{
					float weight = 0.0f;
					if( !stringToType( weightTokens[0], &weight ) )
					{
						weight = 0.0f;
					}
					pool.add( RandRangeFloat( weightTokens[1] ), weight );
				}
			}
		}
		return pool;
	}
	
	WeightedRandPool< RandRangeInt > XMLParser::getXMLAttributeAsWeightedRandPoolIntRange( const XMLNode* element, const char* attributeName )
	{
		WeightedRandPool< RandRangeInt > pool;
		const char* text = element->Attribute( attributeName );
		
		if( text )
		{
			std::vector< std::string > poolTokens;
			stringTokenizer( text, poolTokens, "," );
			for( size_t i = 0; i < poolTokens.size(); ++i )
			{
				std::vector< std::string > weightTokens;
				stringTokenizer( poolTokens[i], weightTokens, "@" );
				if( weightTokens.size() == 1 )
				{
					pool.add( RandRangeInt( poolTokens[i] ), 1 );
				}
				else
				{
					float weight = 0;
					if( !stringToType( weightTokens[0], &weight ) )
					{
						weight = 0;
					}
					pool.add( RandRangeInt( weightTokens[1] ), weight );
				}
			}
		}
		return pool;
	}

	WeightedRandPool< std::string > XMLParser::getXMLAttributeAsWeightedRandPoolString( const XMLNode* element, const char* attributeName )
	{
		WeightedRandPool< std::string > pool;
		const char* text = element->Attribute( attributeName );
		
		if( text )
		{
			std::vector< std::string > poolTokens;
			stringTokenizer( text, poolTokens, "," );
			for( size_t i = 0; i < poolTokens.size(); ++i )
			{
				std::vector< std::string > weightTokens;
				stringTokenizer( poolTokens[i], weightTokens, "@" );
				if( weightTokens.size() == 1 )
				{
					pool.add( poolTokens[i], 1 );
				}
				else
				{
					float weight = 0;
					if( !stringToType( weightTokens[0], &weight ) )
					{
						weight = 0;
					}
					pool.add( weightTokens[1], weight );
				}
			}
		}
		return pool;
	}

	std::vector< std::string > XMLParser::getXMLAttributeAsListOfStrings( const XMLNode* element, const char* attributeName )
	{
		std::string str = getXMLAttributeAsString( element, attributeName, "" );
		std::vector< std::string > tokens;
			
		if( str.length() > 0 )
		{
			stringTokenizer( str, tokens, "," );			
		}
		return tokens;
	}
		
	std::string XMLParser::getXMLAttributeAsString( const XMLNode* element, const char* attributeName, const std::string& defaultValue )
	{
		std::string attrib = defaultValue;
		const char* text = element->Attribute( attributeName );
		if( text )
		{
			attrib = std::string( text );
		}
		return attrib;
	}

	std::string XMLParser::getXMLElementPCDataAsString( const XMLNode* element )
	{
		std::string pcData;
		const char* text = element->GetText();
		if( text )
		{
			pcData = text;
		}
		return pcData;
	}

	// Validation functions
	bool XMLParser::validateXMLChildElements( const XMLNode* element, const char* commaSeparatedListOfRequiredChildren, const char* commaSeparatedListOfOptionalChildren )
	{
		bool valid = true;
		std::vector< std::string > requiredChildren;
		if( strlen( commaSeparatedListOfRequiredChildren ) > 0 )
			stringTokenizer( commaSeparatedListOfRequiredChildren, requiredChildren, "," );
		
		std::vector< std::string > optionalChildren;
		if( strlen( commaSeparatedListOfOptionalChildren ) > 0 )
			stringTokenizer( commaSeparatedListOfOptionalChildren, optionalChildren, "," );		

		const XMLNode* child;
		for( child = element->FirstChildElement(); child != 0; child = child->NextSiblingElement() )
		{
			StrVecIter requiredIter;
			StrVecIter optionalIter;
			requiredIter = std::find( requiredChildren.begin(), requiredChildren.end(), child->Name() );
			
			if( requiredIter == requiredChildren.end() )
			{					
				requiredIter = findIgnoreCase( requiredChildren.begin(), requiredChildren.end(), child->Name() );				
				if( requiredIter != requiredChildren.end() )
				{
					// Case problem with the specified required element
					outputXMLErrorMsgToConsole( ms_caseErrorMsg, "required", "element", child->Name() );
					std::string msg = ms_caseErrorMsg;
					msg += ms_caseErrorMsg2;
					outputXMLErrorMsgToWindow( msg.c_str(), "required", "element", child->Name(), requiredIter->c_str(), reconstructXMLTag( child ).c_str() );
					valid = false;
				}
				else
				{
					// The element was not found in the required list
					// Check the optional list
					optionalIter = std::find( optionalChildren.begin(), optionalChildren.end(), child->Name() );
					if( optionalIter == optionalChildren.end() )
					{
						optionalIter = findIgnoreCase( optionalChildren.begin(), optionalChildren.end(), child->Name() );
						if( optionalIter != optionalChildren.end() )
						{
							// Case problem with the specified element
							outputXMLErrorMsgToConsole( ms_caseErrorMsg, "optional", "element", child->Name() );
							std::string msg = ms_caseErrorMsg;
							msg += ms_caseErrorMsg2;
							outputXMLErrorMsgToWindow( msg.c_str(), "optional", "element", child->Name(), optionalIter->c_str(), reconstructXMLTag( child ).c_str() );					
							valid = false;
						}
						else
						{
							// Not an optional element or a required element so this is an element that is not allowed
							std::vector< std::string > allAllowedChildren( requiredChildren );
							allAllowedChildren.insert( allAllowedChildren.end(), optionalChildren.begin(), optionalChildren.end() );
							std::sort( allAllowedChildren.begin(), allAllowedChildren.end() );
							
							std::string listOfElements = "All allowed elements:\n";
							for( size_t i = 0; i < allAllowedChildren.size(); ++i )
							{
								listOfElements += "\t" + allAllowedChildren[i] + "\n";
							}

							outputXMLErrorMsgToConsole( "The XML element <%s> is not in the list of required or optional elements.\n", child->Name() );
							std::string msg = "The XML element <%s> is not in the list of required or optional elements.\n  > %s\nThe allowed elements are:\n" + listOfElements;
							outputXMLErrorMsgToWindow( msg.c_str(), child->Name(), reconstructXMLTag( child ).c_str() );
							valid = false;							
						}
					}
				}
			}
		}

		std::vector< int > missingElements;
		for( size_t i = 0; i < requiredChildren.size(); ++i )
		{
			if( element->FirstChildElement( requiredChildren[i].c_str() ) == 0 )
			{
				missingElements.push_back( i );				
			}			
		}

		if( missingElements.size() > 0 )
		{
			std::sort( requiredChildren.begin(), requiredChildren.end() );
			std::string listOfMissingElements;
			for( size_t c = 0; c < missingElements.size(); ++c )
			{
				listOfMissingElements += "\t" + requiredChildren[ missingElements[c] ] + "\n";
			}

			std::string listOfElements = "Required elements are:\n";
			for( size_t j = 0; j < requiredChildren.size(); ++j )
			{
				listOfElements += "\t" + requiredChildren[j] + "\n";
			}

			outputXMLErrorMsgToConsole( "Missing required XML elements.\n" );
			outputXMLErrorMsgToWindow( "The following list of required elements are missing:\n%s%s", listOfMissingElements.c_str(), listOfElements.c_str() );
			valid = false;
		}

		return valid;
	}
	
	bool XMLParser::validateXMLAttributes( const XMLNode* element, const char* commaSeparatedListOfRequiredAttributes, const char* commaSeparatedListOfOptionalAttributes )
	{
		bool valid = true;
		std::vector< std::string > requiredAttributes;

		if( strlen( commaSeparatedListOfRequiredAttributes ) > 0 )
			stringTokenizer( commaSeparatedListOfRequiredAttributes, requiredAttributes, "," );

		std::vector< std::string > optionalAttirbutes;

		if( strlen( commaSeparatedListOfOptionalAttributes ) > 0 )
				stringTokenizer( commaSeparatedListOfOptionalAttributes, optionalAttirbutes, "," );

		XMLAttribute const* attribute;
		for( attribute = element->FirstAttribute(); attribute != 0; attribute = attribute->Next() )
		{
			StrVecIter requiredIter;
			StrVecIter optionalIter;
			requiredIter = std::find( requiredAttributes.begin(), requiredAttributes.end(), attribute->Name() );

			if( requiredIter == requiredAttributes.end() )
			{
				requiredIter = findIgnoreCase( requiredAttributes.begin(), requiredAttributes.end(), attribute->Name() );				
				if( requiredIter != requiredAttributes.end() )
				{
					// Case problem with the specified required attribute
					outputXMLErrorMsgToConsole( ms_caseErrorMsg, "required", "attribute", attribute->Name() );
					std::string msg = ms_caseErrorMsg;
					msg += ms_caseErrorMsg2;
					outputXMLErrorMsgToWindow( msg.c_str(), "required", "attribute", attribute->Name(), requiredIter->c_str(), reconstructXMLTag( element ).c_str() );
					valid = false;
				}
				else
				{
					// The required attribute was not found in the required list
					// Check the optional list
					optionalIter = std::find( optionalAttirbutes.begin(), optionalAttirbutes.end(), attribute->Name() );
					if( optionalIter == optionalAttirbutes.end() )
					{
						optionalIter = findIgnoreCase( optionalAttirbutes.begin(), optionalAttirbutes.end(), attribute->Name() );
						if( optionalIter != optionalAttirbutes.end() )
						{
							// Case problem with the specified attribute
							outputXMLErrorMsgToConsole( ms_caseErrorMsg, "optional", "attribute", attribute->Name() );
							std::string msg = ms_caseErrorMsg;
							msg += ms_caseErrorMsg2;
							outputXMLErrorMsgToWindow( msg.c_str(), "optional", "attribute", attribute->Name(), optionalIter->c_str(), reconstructXMLTag( element ).c_str() );					
							valid = false;
						}
						else
						{
							// Not an optional attribute or a required attribute so this is an attribute that is not allowed
							std::string listOfAttributes = "Required Attributes:\n";
							for( size_t i = 0; i < requiredAttributes.size(); ++i )
							{
								listOfAttributes += "\t" + requiredAttributes[i] + "\n";
							}
							listOfAttributes += "Optional Attributes: \n";
							for( size_t i = 0; i < optionalAttirbutes.size(); ++i )
							{
								listOfAttributes += "\t" + optionalAttirbutes[i] + "\n";
							}
							outputXMLErrorMsgToConsole( "The XML attribute \"%s\" is not in the list of required or optional attributes.\n", attribute->Name() );
							std::string msg = "The XML attribute \"%s\" is not in the list of required or optional attributes.\n  > %s\nThe allowed attributes are:\n" + listOfAttributes;
							outputXMLErrorMsgToWindow( msg.c_str(), attribute->Name(), reconstructXMLTag( element ).c_str() );
							valid = false;							
						}
					}
				}
			}
		}

		std::vector< int > missingAttributes;
		for( size_t i = 0; i < requiredAttributes.size(); ++i )
		{
			if( element->FindAttribute( requiredAttributes[i].c_str() ) == 0 )
			{
				missingAttributes.push_back( i );		
			}			
		}

		if( missingAttributes.size() > 0 )
		{
			std::string listOfMissingAttributes;
			for( size_t c = 0; c < missingAttributes.size(); ++c )
			{
				listOfMissingAttributes += "\t" + requiredAttributes[ missingAttributes[c] ] + "\n";
			}

			std::string listOfAttributes = "Required attributes are:\n";
			for( size_t j = 0; j < requiredAttributes.size(); ++j )
			{
				listOfAttributes += "\t" + requiredAttributes[j] + "\n";
			}

			outputXMLErrorMsgToConsole( "Missing required XML attributes.\n" );
			outputXMLErrorMsgToWindow( "The following list of required attributes are missing:\n%s%s", listOfMissingAttributes.c_str(), listOfAttributes.c_str() );
			valid = false;
		}


		return valid;
	}

	StrVecIter XMLParser::findIgnoreCase( StrVecIter first, StrVecIter last, std::string val )
	{
		std::transform( val.begin(), val.end(), val.begin(), ::tolower );
		while( first != last )
		{
			std::string temp = *first;
			std::transform( temp.begin(), temp.end(), temp.begin(), ::tolower );
			if( temp == val )
			{
				return first;
			}
			++first;
		}
		return last;
	}

	void XMLParser::outputXMLErrorMsgToWindow( const char* format, ... )
	{
		char locMsg[512];
		
		sprintf_s( locMsg, ms_ErrorLocation, m_fileLocation.c_str() );

		va_list args;
		va_start( args, format );
		char errorMsg[ MAX_MESSAGE_SIZE ];
		vsprintf_s( errorMsg, format, args );
		va_end( args );

		std::string msg = std::string( locMsg ) + "\n" + std::string( errorMsg ) + "\n" + std::string( ms_ErrorMsg );		
		
		bool response = MonkyException::yesNoMessageBox( ms_ErrorTitle, msg.c_str() );

		if( !response )
		{
			GameApp::getInstance()->exitProgram( 0 );
		}
	}

	void XMLParser::outputXMLErrorMsgToConsole( const char* format, ... )
	{
		va_list args;
		va_start( args, format );
		char errorMsg[ MAX_MESSAGE_SIZE ];
		vsprintf_s( errorMsg, format, args );
		va_end( args );

		std::string msg = m_fileLocation + "(0): " + std::string( errorMsg );
	
		MonkyException::printToCompilerOutputConsole( msg.c_str() );
	}
	
	std::string XMLParser::reconstructXMLTag( const XMLNode* element )
	{
		std::string temp = "<";
		temp += element->Name();
		XMLAttribute const* attribute;
		for( attribute = element->FirstAttribute(); attribute != 0; attribute = attribute->Next() )
		{
			temp += " ";
			temp += attribute->Name();
			temp += "=\"";
			temp += attribute->Value();
			temp += "\"";
		}
		temp += ">";

		temp += getXMLElementPCDataAsString( element );
		temp += "</";
		temp += element->Name();
		temp += ">";

		return temp;
	}

}