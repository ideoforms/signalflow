#pragma once

#include "node.h"
#include "nodedef.h"

namespace libsignal
{
	class Structure
	{
		public:
			Structure(std::string name);
			
			/*----------------------------------------------------------------------------------
			 * Methods for creating a Structure from live Node objects.
			 *---------------------------------------------------------------------------------*/
			NodeRef add_input(std::string name, sample default_value = 0);
			NodeRef add_node(NodeRef node);
			void set_output(const NodeRef &out);

			/*----------------------------------------------------------------------------------
			 * Methods for creating a Structure from NodeDefinitions.
			 *---------------------------------------------------------------------------------*/
			void add_node_def(NodeDefinition def);
			void set_output(NodeDefinition def);
			NodeDefinition *get_node_def(int id);

			/*----------------------------------------------------------------------------------
			 * Save a Structure to disk.
			 *---------------------------------------------------------------------------------*/
			void save(std::string filename);

			/*----------------------------------------------------------------------------------
			 * Load a Structure from disk.
			 *---------------------------------------------------------------------------------*/
			void load(std::string filename);

			/*----------------------------------------------------------------------------------
			 * Parse a template from live Node objects to create a network of NodeDefs
			 *---------------------------------------------------------------------------------*/
			void parse();
			NodeDefinition get_root();

			/*----------------------------------------------------------------------------------
			 * Store a Structure to the global SynthRegistry so that it can be
			 * instantiated by name.
			 *---------------------------------------------------------------------------------*/
			void store();

			/**----------------------------------------------------------------------------------
			 * Returns true if this Structure is ready to be played.
			 *---------------------------------------------------------------------------------*/
			bool is_ready();

			std::string name;

		private:

			NodeDefinition output_def;
			NodeRef output = nullptr;
			bool parsed = false;
			int last_id = 0;

			std::string get_input_name(const NodeRef &node);
			NodeDefinition parse_root(const NodeRef &node);

			std::unordered_map <std::string, Node *> inputs;
			std::unordered_map <int, NodeDefinition> nodedefs;
	};

    class StructRef : public std::shared_ptr<Structure>
    {
        public:
            using std::shared_ptr<Structure>::shared_ptr;

            StructRef() : std::shared_ptr<Structure>(nullptr) { }
            StructRef(Structure *ptr) : std::shared_ptr<Structure>(ptr) { }
    };
}
