#pragma once

#include <unordered_map>
#include <functional>

#include "nodedef.h"

#define REGISTER(CLASS, NAME) static bool CLASS##OK = NodeRegistry::global()->add<CLASS>(NAME);

namespace libsignal
{

	class Node;

	template <typename T>
	Node *create()
	{
		return new T;
	}

	class NodeRegistry
	{
		public:
			NodeRegistry();

			static NodeRegistry *global();

			Node *create(std::string name);
			Node *create(NodeDefinition definition);

			/*------------------------------------------------------------------------
			 * Function template implementations must be in .h file.
			 *-----------------------------------------------------------------------*/
			template <class T>
			bool add(std::string name)
			{
				classes[name] = &::libsignal::create<T>;
				return true;
			}

			std::unordered_map <std::string, std::function <Node *()> > classes;
	};

}
