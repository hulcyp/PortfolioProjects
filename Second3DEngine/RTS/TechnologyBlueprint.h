#pragma once
#include <XMLParser.h>
#include <map>
#include <string>

namespace pdh
{
	class Technology;

	class TechnologyBlueprint
	{
	public:
		TechnologyBlueprint( XMLParser& parser, const XMLNode* techBlueprintNode );
		~TechnologyBlueprint();

		Technology* createTechnology();
		const std::string& getName() { return m_name; }

		struct CostBP
		{
			int time;
			int mineral;
			int gas;
		};

		static TechnologyBlueprint* getBlueprint( const std::string& name );

	protected:
		void registerBP( const std::string& name );

	private:
		std::string m_name;
		CostBP m_cost;

		static std::map< std::string, TechnologyBlueprint* > sm_blueprints;
	};
}