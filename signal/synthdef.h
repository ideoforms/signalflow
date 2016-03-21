#include "node.h"
#include "nodedef.h"

namespace libsignal
{
	class SynthDef
	{
		public:
			SynthDef(std::string name);
			
			/*----------------------------------------------------------------------------------
			 * Methods for creating a SynthDef from live Node objects.
			 *---------------------------------------------------------------------------------*/
			NodeRef add_input(std::string name, sample default_value);
			NodeRef add_node(NodeRef node);
			void set_output(const NodeRef &out);

			/*----------------------------------------------------------------------------------
			 * Save a SynthDef to disk.
			 *---------------------------------------------------------------------------------*/
			void save(std::string filename);

			/*----------------------------------------------------------------------------------
			 * 
			 *---------------------------------------------------------------------------------*/
			NodeDefinition read_structure();

			std::string name;
			NodeRef output = nullptr;

		private:
			std::string get_input_name(const NodeRef &node);
			NodeDefinition read_structure(const NodeRef &node);
			std::unordered_map <std::string, Node *> inputs;
	};
}
