//-----------------------------------------------------------------------------------------------
// Graphics.cpp
//-----------------------------------------------------------------------------------------------
#include "Graphics.hpp"
#include "IntVector2.hpp"

#define ARENA // excludes certain DLL-only portions of the CommonInterface from us
#include "../../include/CommonInterface.hpp"


//-----------------------------------------------------------------------------------------------
// Globals
//
const int NUM_CIRCLE_SIDES = 24;
const int NUM_DIAMOND_SIDES = 4;
const float LINE_WIDTH = 1.0f;
const int MAIN_FONT_HEIGHT = 12;
Vector2 g_circlePoints[ NUM_CIRCLE_SIDES ];
Vector2 g_diamondPoints[ NUM_DIAMOND_SIDES ];
GLuint g_fontDisplayListBase = 0;


//-----------------------------------------------------------------------------------------------
void InitGraphics( HDC graphicsDeviceDisplayContext )
{
	ComputeCirclePoints();
	CreateSimpleOpenGLBitmapFonts( graphicsDeviceDisplayContext );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glShadeModel( GL_SMOOTH );
}


//-----------------------------------------------------------------------------------------------
void CreateSimpleOpenGLBitmapFonts( HDC graphicsDeviceDisplayContext )
{
	g_fontDisplayListBase = glGenLists( 255 );
	if( g_fontDisplayListBase == 0 )
	{
		DebuggerPrintf( "ERROR: CreateSimpleOpenGLBitmapFonts() failed; glGenLists() returned a font display list base of 0.\n" );
		return;
	}

	HFONT newFont = ::CreateFontA(
		-MAIN_FONT_HEIGHT,			// Character height (negatives are character height; positives are cell height)
		0,							// Character width (0 is default, computed from height using default aspect)
		0,							// Escapement
		0,							// Orientation
		0,							// Weight (0=default, 300=light, 400=normal, 700=bold, 1000=max)
		FALSE,						// is italics?
		FALSE,						// is underlined?
		FALSE,						// is strikeout?
		ANSI_CHARSET,				// character set
		OUT_TT_PRECIS,				// use a TrueType font if available
		CLIP_DEFAULT_PRECIS,		// use default character clipping scheme
		ANTIALIASED_QUALITY,		// quality
		FF_SWISS|DEFAULT_PITCH,		// pitch and family
		"Tahoma" );					// font name

	if( newFont == NULL )
	{
		DebuggerPrintf( "ERROR: CreateSimpleOpenGLBitmapFonts() failed; CreateFontA() returned a NULL font.\n" );
		return;
	}

	HFONT previousFont = (HFONT) SelectObject( graphicsDeviceDisplayContext, newFont );
	wglUseFontBitmaps( graphicsDeviceDisplayContext, 0, 255, g_fontDisplayListBase );
	SelectObject( graphicsDeviceDisplayContext, previousFont );
//	DeleteObject( newFont );
}


//-----------------------------------------------------------------------------------------------
void ComputeCirclePoints()
{
	for( int i = 0; i < NUM_CIRCLE_SIDES; ++ i )
	{
		float degrees = 360.f * ((float) i / (float) NUM_CIRCLE_SIDES);
		g_circlePoints[ i ].SetUnitLengthAndYawDegrees( degrees );
	}

	for( int i = 0; i < NUM_DIAMOND_SIDES; ++ i )
	{
		float degrees = 360.f * ((float) i / (float) NUM_DIAMOND_SIDES);
		g_diamondPoints[ i ].SetUnitLengthAndYawDegrees( degrees );
	}
}


//-----------------------------------------------------------------------------------------------
void SetColor( const Rgba& color, float alpha )
{
	Rgba colorWithAlphaApplied = color;
	colorWithAlphaApplied.ScaleAlpha( alpha );
	glColor4ub( colorWithAlphaApplied.r, colorWithAlphaApplied.g, colorWithAlphaApplied.b, colorWithAlphaApplied.a );
}


//-----------------------------------------------------------------------------------------------
void Draw2DText( const std::string& text, const IntVector2& drawPos, const Rgba& textColor, float alpha )
{
//	glDisable( GL_TEXTURE_2D );
	SetColor( textColor, alpha );
	glRasterPos2i( drawPos.x, drawPos.y );
	glPushAttrib( GL_LIST_BIT );
	glListBase( g_fontDisplayListBase );
	glCallLists( (GLsizei)text.size(), GL_UNSIGNED_BYTE, (const GLubyte *)text.c_str() );
	glPopAttrib();
//	glEnable( GL_TEXTURE_2D );
}


//-----------------------------------------------------------------------------------------------
void DrawShadedLineSegment( const Vector2& start, const Vector2& end, const Rgba& colorAtStart, const Rgba& colorAtEnd, float alphaAtStart, float alphaAtEnd )
{
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINES );
	{
		SetColor( colorAtStart, alphaAtStart );
		glVertex2f( start.x, start.y );
		SetColor( colorAtEnd, alphaAtEnd );
		glVertex2f( end.x, end.y );
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledCircle( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glBegin( GL_TRIANGLE_FAN );
	{
		for( int i = 0; i < NUM_CIRCLE_SIDES; ++ i )
		{
			Vector2 point = center + (radius * g_circlePoints[ i ]);
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
}


//-----------------------------------------------------------------------------------------------
void DrawFilledDiamond( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glBegin( GL_TRIANGLE_FAN );
	{
		for( int i = 0; i < NUM_DIAMOND_SIDES; ++ i )
		{
			Vector2 point = center + (radius * g_diamondPoints[ i ]);
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
}


//-----------------------------------------------------------------------------------------------
void DrawOutlinedCircle( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINE_LOOP );
	{
		for( int i = 0; i < NUM_CIRCLE_SIDES; ++ i )
		{
			Vector2 point = center + (radius * g_circlePoints[ i ]);
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawOutlinedDiamond( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINE_LOOP );
	{
		for( int i = 0; i < NUM_DIAMOND_SIDES; ++ i )
		{
			Vector2 point = center + (radius * g_diamondPoints[ i ]);
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawX( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINES );
	{
		Vector2 TL = center + Vector2( -radius, -radius );
		Vector2 BR = center + Vector2( radius, radius );
		Vector2 TR = center + Vector2( radius, -radius );
		Vector2 BL = center + Vector2( -radius, radius );

		glVertex2f( TL.x, TL.y );
		glVertex2f( BR.x, BR.y );

		glVertex2f( TR.x, TR.y );
		glVertex2f( BL.x, BL.y );
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawPlus( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINES );
	{
		Vector2 T = center + Vector2( 0.f, -radius );
		Vector2 B = center + Vector2( 0.f, radius );
		Vector2 R = center + Vector2( radius, 0.f );
		Vector2 L = center + Vector2( -radius, 0.f );

		glVertex2f( T.x, T.y );
		glVertex2f( B.x, B.y );

		glVertex2f( R.x, R.y );
		glVertex2f( L.x, L.y );
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledOutlinedCircle( const Vector2& center, float radius, const Rgba& fillColor, const Rgba& edgeColor, float alpha )
{
	DrawFilledCircle( center, radius, fillColor, alpha );
	DrawOutlinedCircle( center, radius, edgeColor, alpha );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledPiePiece( const Vector2& center, float radius, float startDegrees, float endDegrees, float unitsAwayFromCenter, const Rgba& color, float alpha )
{
	const int NUM_SIDES_FOR_WHOLE_PIE = 40;

	float spanDegrees = endDegrees - startDegrees;
	float circleFraction = spanDegrees / 360.f;
	float centerOfSpanDegrees = startDegrees + (0.5f * spanDegrees);
	Vector2 piePivotDisplacement;
	piePivotDisplacement.SetLengthAndYawDegrees( unitsAwayFromCenter, centerOfSpanDegrees );
	Vector2 piePivot( center + piePivotDisplacement );

	int numOuterVerts = RoundToNearestInt( circleFraction * (float)NUM_SIDES_FOR_WHOLE_PIE );
	numOuterVerts = ClampInt( numOuterVerts, 2, NUM_SIDES_FOR_WHOLE_PIE );

	float degreesPerVert = spanDegrees / (float) numOuterVerts;

	SetColor( color, alpha );
	glBegin( GL_TRIANGLE_FAN );
	{
		glVertex2f( piePivot.x, piePivot.y );
		for( int i = 0; i <= numOuterVerts; ++ i )
		{
			Vector2 displacement;
			displacement.SetLengthAndYawDegrees( radius, startDegrees + degreesPerVert * (float)i );
			Vector2 point = piePivot + displacement;
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
}


//-----------------------------------------------------------------------------------------------
void DrawFilledArea( const AABB2& area, const Rgba& color, float alpha, bool alreadyInBeginEndBlock )
{
	SetColor( color, alpha );
	if( !alreadyInBeginEndBlock )
		glBegin( GL_QUADS );

	{
		glVertex2f( area.mins.x, area.mins.y );
		glVertex2f( area.maxs.x, area.mins.y );
		glVertex2f( area.maxs.x, area.maxs.y );
		glVertex2f( area.mins.x, area.maxs.y );
	}

	if( !alreadyInBeginEndBlock )
		glEnd();
}


//-----------------------------------------------------------------------------------------------
void DrawOutlinedArea( const AABB2& area, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINE_LOOP );
	{
		glVertex2f( area.mins.x, area.mins.y );
		glVertex2f( area.maxs.x, area.mins.y );
		glVertex2f( area.maxs.x, area.maxs.y );
		glVertex2f( area.mins.x, area.maxs.y );
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledOutlinedArea( const AABB2& area, const Rgba& fillColor, const Rgba& edgeColor, float alpha )
{
	DrawFilledArea( area, fillColor, alpha );
	DrawOutlinedArea( area, edgeColor, alpha );
}

