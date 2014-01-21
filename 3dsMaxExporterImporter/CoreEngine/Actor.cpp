#include "Actor.h"
#include "Renderer.h"
#include "Model.h"
#include "Font.h"

namespace pdh
{
	Actor::Actor( const std::string& id, Mesh* mesh )
		:	Object( id, mesh )
	{}
}