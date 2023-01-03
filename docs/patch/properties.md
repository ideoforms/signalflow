# Patch

## Patch properties

| Property | Type | Description                                               |
|----------|------|-----------------------------------------------------------|
| nodes    | list | A list of all of the Node objects that make up this Patch |
| inputs | dict | A dict of key-value pairs corresponding to all of the (audio rate) inputs within the Patch |
| state | int | The Patch's current playback state, which can be `SIGNALFLOW_PATCH_STATE_ACTIVE` or `SIGNALFLOW_PATCH_STATE_STOPPED`. |
| graph | AudioGraph | A reference to the AudioGraph that the Patch is part of |

---

[→ Next: Exporting and importing patches](exporting)