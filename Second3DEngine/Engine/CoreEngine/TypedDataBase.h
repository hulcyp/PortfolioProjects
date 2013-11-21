#pragma once

namespace Monky
{
	class TypedDataBase
	{
	public:
		virtual ~TypedDataBase() {}

		virtual const type_info& getTypeID() const = 0;
		virtual TypedDataBase* clone() const = 0;

	};
}