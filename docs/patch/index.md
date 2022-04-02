# Patch

!!! warning
    This documentation is a work-in-progress and may have sections that are missing or incomplete.

A `Patch` represents a connected group of `Nodes`, analogous to a synthesizer. Defining patches makes it easy to create higher-level structures, which can then be reused and instantiated with a single line of code, in much the same way as a Node.

Behind the scenes, the structure of a `Patch` is encapsulated by a `PatchSpec`, a template which can be instantiated or serialised to a JSON file for later use.  

- [A Patch structure is defined](/patch/defining) either by declaring a Patch subclass or with a JSON specification file
- [Play and stop a Patch](/patch/playback) by connecting it to the AudioGraph or the input of another Patch or Node 
- Patch inputs
  - Audio-rate inputs
  - Buffer inputs
  - Trigger node
- Operators 
- Exporting and importing patches
- Patch properties
- Auto-free

---

[→ Next: Defining a Patch](/patch/defining)