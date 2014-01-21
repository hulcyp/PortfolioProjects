//-----------------------------------------------------------------------------------------------
// Graphics.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_Graphics__
#define __include_Graphics__

#include "AABB2.hpp"
#include "Vector2.hpp"
#include "Rgba.hpp"
#include "Clock.hpp"


extern const int MAIN_FONT_HEIGHT;


void InitGraphics( HDC graphicsDeviceDisplayContext );
void CreateSimpleOpenGLBitmapFonts( HDC graphicsDeviceDisplayContext );
void ComputeCirclePoints();
void SetColor( const Rgba& color, float alpha=1.f );
void Draw2DText( const std::string& text, const IntVector2& drawPos, const Rgba& textColor, float alpha = 1.f );
void DrawShadedLineSegment( const Vector2& start, const Vector2& end, const Rgba& colorAtStart, const Rgba& colorAtEnd, float alphaAtStart=1.f, float alphaAtEnd=1.f );
void DrawFilledCircle( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawFilledDiamond( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawOutlinedCircle( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawOutlinedDiamond( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawX( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawPlus( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawFilledOutlinedCircle( const Vector2& center, float radius, const Rgba& fillColor, const Rgba& edgeColor, float alpha=1.f );
void DrawFilledPiePiece( const Vector2& center, float radius, float startDegrees, float endDegrees, float unitsAwayFromCenter, const Rgba& color, float alpha=1.f );
void DrawFilledArea( const AABB2& area, const Rgba& color, float alpha=1.f, bool alreadyInBeginEndBlock = false );
void DrawOutlinedArea( const AABB2& area, const Rgba& color, float alpha=1.f );
void DrawFilledOutlinedArea( const AABB2& area, const Rgba& fillColor, const Rgba& edgeColor, float alpha=1.f );



#endif // __include_Graphics__
