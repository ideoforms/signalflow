import libsignal

def test_graph():
    graph = libsignal.AudioGraph()
    assert graph
    del graph
    graph = None
    import gc
    gc.collect()