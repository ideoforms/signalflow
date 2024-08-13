from signalflow import Buffer, WavetableBuffer, WaveShaperBuffer, EnvelopeBuffer
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def plot_buffer(buffer: Buffer,
                ax: matplotlib.axis.Axis = None,
                title: str = None):
    axis_passed = (ax is not None)

    #--------------------------------------------------------------------------------
    # In the case that no matplotlib axis is given, create a new high-resolution
    # plot and axis
    #--------------------------------------------------------------------------------
    if not axis_passed:
        _, ax = plt.subplots(figsize=(16, 4), dpi=300)

    ax.grid(linestyle=":")

    if isinstance(buffer, WavetableBuffer) or isinstance(buffer, WaveShaperBuffer) or isinstance(buffer, EnvelopeBuffer):
        #--------------------------------------------------------------------------------
        # Plot envelope and wavetable/shaper buffers by sample
        #--------------------------------------------------------------------------------
        ax.plot(np.arange(len(buffer)), buffer.data[0])
    else:
        #--------------------------------------------------------------------------------
        # Plot all other buffers by time, in seconds
        #--------------------------------------------------------------------------------
        ax.plot(np.arange(len(buffer)) / buffer.sample_rate, buffer.data[0])

    #--------------------------------------------------------------------------------
    # For wavetable/shaper buffers, highlight the y=0 line.
    #--------------------------------------------------------------------------------
    if isinstance(buffer, WavetableBuffer) or isinstance(buffer, WaveShaperBuffer):
        plt.axhline(y=0, color="red", linestyle=":", linewidth=0.5)

    #--------------------------------------------------------------------------------
    # Plot title
    #--------------------------------------------------------------------------------
    if title is not None:
        ax.set_title(title)

    #--------------------------------------------------------------------------------
    # If this is a new axis, render it to screen
    #--------------------------------------------------------------------------------
    if not axis_passed:
        plt.plot()
        plt.show()