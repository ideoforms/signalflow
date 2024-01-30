import os
import numpy as np
import pandas as pd
from signalflow import AudioGraph, Buffer, BufferPlayer, Node

class AudioFeatureBuffer (Buffer):
    @classmethod
    def from_buffer(cls,
                    audio_buffer: Buffer,
                    segment_by: str = "block",
                    block_size: int = 8192,
                    hop_size: int = None,
                    onset_times: list[float] = None,
                    do_window: bool = True,
                    write_segments: bool = False,
                    features: list[Node] = None,
                    labels: list[str] = None):
        #--------------------------------------------------------------------------------
        # Create an AudioFeatureBuffer from an existing audio buffer, generating one
        # frame of N-dimensional feature data for each segment in the buffer.
        #
        # This needs to be flexible enough to support different input paradigms:
        #  - fixed-length blocks (i.e., segmented by fixed block size)
        #    vs variable-length blocks (i.e., segmented by onsets)
        #  - blocks with overlap
        #  - single or multiple feature extractors (for N-dimensional feature space)
        #--------------------------------------------------------------------------------
        if audio_buffer.num_channels > 1:
            print("WARNING: AudioFeatureBuffer.from_buffer currently only supports mono input audio, only analysing the first channel")
            audio_buffer = audio_buffer[0]
        if hop_size is None:
            hop_size = block_size
        if features is None or len(features) == 0:
            raise ValueError("Must specify one or more features to extract")
        num_features = len(features)

        if not labels:
            labels = "xyzpqrstuvw"[num_features]
        if len(labels) != num_features:
            raise ValueError("Number of feature labels does not match number of features")

        if segment_by == "block":
            if len(audio_buffer) < block_size:
                raise ValueError("Buffer cannot be shorter than block size")

            num_segments = 1 + (len(audio_buffer) - block_size) // hop_size
            feature_buffer = AudioFeatureBuffer(num_features, num_segments)
            feature_buffer.segment_offsets = [hop_size * block_index for block_index in range(num_segments)]
            feature_buffer.segment_lengths = [block_size for _ in feature_buffer.segment_offsets]

            #--------------------------------------------------------------------------------
            # These derived properties simply convert sample units -> seconds, so can
            # be performed agnostically to the segmentation method.
            #--------------------------------------------------------------------------------
            feature_buffer.segment_durations = [segment_length / audio_buffer.sample_rate for segment_length in feature_buffer.segment_lengths]
            feature_buffer.segment_times = [segment_offset / audio_buffer.sample_rate for segment_offset in feature_buffer.segment_offsets]
        elif segment_by == "onset_times":
            if onset_times is None or len(onset_times) == 0:
                raise ValueError("No onset times specified")
            num_segments = len(onset_times)
            feature_buffer = AudioFeatureBuffer(num_features, num_segments)
            feature_buffer.segment_times = onset_times
            feature_buffer.segment_durations = []
            for onset_index, onset_time in enumerate(onset_times):
                if onset_index < len(onset_times) - 1:
                    segment_duration = onset_times[onset_index + 1] - onset_time
                else:
                    segment_duration = audio_buffer.duration - onset_time
                # TODO: max duration
                if segment_duration > 2:
                    segment_duration = 2
                feature_buffer.segment_durations.append(segment_duration)
            feature_buffer.segment_offsets = [int(round(segment_time * audio_buffer.sample_rate)) for segment_time in feature_buffer.segment_times]
            feature_buffer.segment_lengths = [int(round(segment_duration * audio_buffer.sample_rate)) for segment_duration in feature_buffer.segment_durations]
        else:
            raise ValueError("Invalid segment mode: %s" % segment_by)

        feature_buffer.labels = labels

        #--------------------------------------------------------------------------------
        # For each segment, extract the samples to a buffer
        #--------------------------------------------------------------------------------
        for segment_index, segment_offset in enumerate(feature_buffer.segment_offsets):
            segment_length = feature_buffer.segment_lengths[segment_index]
            segment_samples = audio_buffer.data[:, segment_offset:segment_offset+segment_length]
            if do_window:
                segment_samples = segment_samples * np.hanning(len(segment_samples))
            segment_buffer = Buffer(segment_samples)
            if write_segments:
                segment_dir = "audio/segments"
                os.makedirs(segment_dir, exist_ok=True)
                segment_buffer.save("%s/segment-%d.wav" % (segment_dir, segment_index))

            player = BufferPlayer(segment_buffer)
            graph = AudioGraph.get_shared_graph()
            segment_block_length = graph.output_buffer_size
            segment_block_count = segment_length // segment_block_length
            segment_block_values_per_feature = [[] for _ in range(num_features)]
            feature_output_buffer = Buffer(1, segment_block_length)
            for segment_block_index in range(segment_block_count):
                #--------------------------------------------------------------------------------
                # For each feature, run Vamp on the audio segment and output feature values
                # into a mono buffer. Because Vamp only extracts one feature value per
                # block, this buffer will just include the same value repeatedly.
                #
                # NOTE: Some vamp features don't seem to provide features for small blocks
                #       (<1024) - need to overcome this
                #--------------------------------------------------------------------------------
                player.process(segment_block_length)
                for feature_index, feature in enumerate(features):
                    feature.input = player
                    feature.process(feature_output_buffer)
                    feature_value = feature_output_buffer.data[0][0]
                    segment_block_values_per_feature[feature_index].append(feature_value)
            for feature_index in range(len(features)):
                average_feature_value = np.median(segment_block_values_per_feature[feature_index])
                feature_buffer.data[feature_index][segment_index] = average_feature_value
        return feature_buffer

    def to_records(self):
        data = []
        for index in range(len(self)):
            record = {
                "index": index,
                "timestamp": self.segment_times[index],
                "duration": np.log10(self.segment_durations[index])
            }
            for feature_index, label in enumerate(self.labels):
                record[label] = np.abs(self.data[feature_index][index])
            data.append(record)

        return data

    def to_dataframe(self):
        return pd.DataFrame.from_records(self.to_records())
