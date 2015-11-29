#pragma once

#include <unordered_map>
// #include "unit.h"
#include "nodedef.h"

// #define REGISTER(class, name) (::signum::NodeRegistry::global->add<class>(name))

// #define REGISTER(class, name) static NodeRegistry *reg = NodeRegistry::global(); static bool ok = reg->add<class>(name);
#define REGISTER(class, name) static bool class##OK = NodeRegistry::global()->add<class>(name);


namespace signum
{

	class Unit;

	template <typename T>
	Unit *create()
	{
		return new T;
	}

	class NodeRegistry
	{
		public:
			NodeRegistry();

			static NodeRegistry *global();

			Unit *create(std::string name);
			Unit *create(NodeDefinition definition);

			/*------------------------------------------------------------------------
			 * Function template implementations must be in .h file.
			 *-----------------------------------------------------------------------*/
			template <class T>
			bool add(std::string name)
			{
				classes[name] = &::signum::create<T>;
				return true;
			}

			std::unordered_map <std::string, std::function <Unit *()> > classes;
	};

}
