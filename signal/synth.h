#include "nodedef.h"

namespace libsignal
{
	class Synth
	{
		public:
			Synth(StructRef structure);
			Synth(std::string name);
			NodeRef instantiate(NodeDefinition *nodedef);

			void set_param(std::string name, float value);
			void set_param(std::string name, NodeRef value);
	};
}
