#pragma once

#include <unordered_map>
#include <functional>

#include "nodedef.h"

#define REGISTER(CLASS, NAME) static bool CLASS##OK = NodeRegistry::global()->add<CLASS>(NAME);

namespace libsignal
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
				classes[name] = &::libsignal::create<T>;
				return true;
			}

			std::unordered_map <std::string, std::function <Unit *()> > classes;
	};

}
