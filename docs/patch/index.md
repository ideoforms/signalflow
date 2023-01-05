# Patch

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

A `Patch` represents a connected group of `Nodes`, analogous to a synthesizer. Defining patches makes it easy to create higher-level structures, which can then be reused and instantiated with a single line of code, in much the same way as a Node.

Behind the scenes, the structure of a `Patch` is encapsulated by a `PatchSpec`, a template which can be instantiated or serialised to a JSON file for later use.  

- [A Patch structure is defined](defining) either by declaring a Patch subclass or with a JSON specification file
- [Play and stop a Patch](playback) by connecting it to the AudioGraph or the input of another Patch or Node 
- Similar to nodes, a Patch can be [modulated by audio-rate inputs](inputs#audio-rate-inputs), [triggered by trigger inputs](inputs#triggers), and [access sample data via buffer inputs](inputs#buffer-inputs) 
- The outputs of Patches can be altered or combined by [normal Python operators](operators)
- The status of a Patch can be queried via its [properties](properties) 
- Patches can be [exported and imported to JSON](exporting)
- The [auto-free mechanism](auto-free) allows Patches to automatically stop and free their memory after playback is complete 

---

[→ Next: Defining a Patch](defining.md)
