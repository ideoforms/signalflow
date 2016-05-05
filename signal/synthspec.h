#pragma once

#include "node.h"
#include "nodedef.h"

namespace libsignal
{
	class SynthSpec
	{
		public:
			SynthSpec(std::string name);
			
			/*----------------------------------------------------------------------------------
			 * Methods for creating a SynthSpec from NodeDefinitions.
			 *---------------------------------------------------------------------------------*/
			void add_node_def(NodeDefinition def);
			void set_output(NodeDefinition def);
			NodeDefinition *get_node_def(int id);
			NodeDefinition get_root();

			/*----------------------------------------------------------------------------------
			 * Save a SynthSpec to disk.
			 *---------------------------------------------------------------------------------*/
			void save(std::string filename);

			/*----------------------------------------------------------------------------------
			 * Load a SynthSpec from disk.
			 *---------------------------------------------------------------------------------*/
			void load(std::string filename);

			/*----------------------------------------------------------------------------------
			 * Store a SynthSpec to the global SynthRegistry so that it can be
			 * instantiated by name.
			 *---------------------------------------------------------------------------------*/
			void store();

			/**----------------------------------------------------------------------------------
			 * Returns true if this SynthSpec is ready to be played.
			 *---------------------------------------------------------------------------------*/
			bool is_ready();

			std::string name;

		// private:

			NodeDefinition output_def;
			bool parsed = false;
			int last_id = 0;

			std::unordered_map <int, NodeDefinition> nodedefs;
	};

	class SynthSpecRef : public std::shared_ptr<SynthSpec>
	{
		public:
			SynthSpecRef() : std::shared_ptr<SynthSpec>(nullptr) { }
			SynthSpecRef(SynthSpec *ptr) : std::shared_ptr<SynthSpec>(ptr) { }
	};
}
