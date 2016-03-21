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
			 * Methods for creating a SynthDef from NodeDefinitions.
			 *---------------------------------------------------------------------------------*/
			void add_node_def(NodeDefinition def);
			void set_output(NodeDefinition def);
			NodeDefinition *get_node_def(int id);

			/*----------------------------------------------------------------------------------
			 * Save a SynthDef to disk.
			 *---------------------------------------------------------------------------------*/
			void save(std::string filename);

			/*----------------------------------------------------------------------------------
			 * 
			 *---------------------------------------------------------------------------------*/
			void read_structure();
			NodeDefinition get_root();

			std::string name;

		private:

			NodeDefinition output_def;
			NodeRef output = nullptr;
			bool parsed = false;
			int last_id = 0;

			std::string get_input_name(const NodeRef &node);
			NodeDefinition read_structure(const NodeRef &node);

			std::unordered_map <std::string, Node *> inputs;
			std::unordered_map <int, NodeDefinition> nodedefs;
	};
}
