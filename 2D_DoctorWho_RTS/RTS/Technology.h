#pragma once
#include <string>

namespace pdh
{
	class TechnologyBlueprint;

	class Technology
	{
		friend TechnologyBlueprint;

		Technology( const std::string& name );
		~Technology();

		struct Cost
		{
			int time;
			int mineral;
			int gas;
		};

	private:
		const std::string& m_name;
		Cost m_cost;
	};
}