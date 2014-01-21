#pragma once

namespace pdh
{
	class TypedDataBase
	{
	public:
		virtual const type_info& getTypeID() const = 0;
		virtual TypedDataBase* clone() const = 0;
	};
}