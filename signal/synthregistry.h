#pragma once

#include <unordered_map>
#include "structure.h"

namespace libsignal
{
	class SynthRegistry
	{
		public:
			SynthRegistry();
			static SynthRegistry *global();

			Synth *create(std::string name);
			void add(std::string name, StructRef structure);
			StructRef get(std::string name);

			std::unordered_map <std::string, StructRef> structures;
	};
}
