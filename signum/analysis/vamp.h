#pragma once

#include "../unit.h"

#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginLoader.h>

using namespace std;

using Vamp::Plugin;
using Vamp::PluginHostAdapter;
using Vamp::RealTime;
using Vamp::HostExt::PluginLoader;
using Vamp::HostExt::PluginWrapper;
using Vamp::HostExt::PluginInputDomainAdapter;

namespace signum
{
	class VampAnalysis : public UnaryOpUnit
	{
		public:
			VampAnalysis(UnitRef input = 0.0) :
				UnaryOpUnit(input)
			{
				this->name = "vamp";

				this->current_frame = 0;

			    string vamp_plugin_library = "vamp-example-plugins";
				string vamp_plugin_feature = "spectralcentroid";
				string vamp_plugin_output = "linearcentroid";	

				PluginLoader *loader = PluginLoader::getInstance();
				PluginLoader::PluginKey key = loader->composePluginKey(vamp_plugin_library, vamp_plugin_feature);

				this->plugin = loader->loadPlugin(key, 44100, PluginLoader::ADAPT_ALL);

				if (!this->plugin)
					fprintf(stderr, "Failed to load plugin");

				/*------------------------------------------------------------------------
				 * Get required output index
				 *-----------------------------------------------------------------------*/
				Plugin::OutputList outputs = this->plugin->getOutputDescriptors();
				this->output_index = -1;

				for (int oi = 0; oi < outputs.size(); oi++)
				{
					if (outputs[oi].identifier == vamp_plugin_output)
					{
						this->output_index = oi;
						break;
					}
				}

				printf("Loaded plugin (output index %d)\n", this->output_index);
				this->plugin->initialise(1, 512, 512);
			}

			~VampAnalysis()
			{
				// close vamp plugins
			}

			void next(sample **out, int num_frames)
			{
				printf("Reading %d frames\n", num_frames);
				RealTime rt = RealTime::frame2RealTime(this->current_frame, 44100);
				Plugin::FeatureSet features = this->plugin->process(this->input->out, rt);
				if (features[this->output_index].size())
				{
					Plugin::Feature feature = features[this->output_index][0];
					float value = feature.values[0];
					// printf("Got results (size = %d): %f\n", features[this->output_index].size(), value);

					for (int frame = 0; frame < num_frames; frame++)
					{
						for (int channel = 0; channel < this->channels_in; channel++)
						{
							out[channel][frame] = value;
						}
					}

					this->current_frame += num_frames;
				}
			}

			int current_frame;
			int output_index;
			Plugin *plugin;
	};

	REGISTER(VampAnalysis, "vamp");
}
