import pytest
import subprocess
import glob
import os

root_directory = "examples"

@pytest.mark.skip
def test_example(example_basename):
    example_file = os.path.join(root_directory, example_basename)
    pipe = subprocess.Popen([example_file], stdout=open("/dev/null", "w"), stderr=open("/dev/null", "w"))
    try:
        pipe.wait(0.5)
        assert pipe.poll() == 0
    except subprocess.TimeoutExpired:
        #--------------------------------------------------------------------------------
        # Process ran OK
        #--------------------------------------------------------------------------------
        pipe.kill()

def pytest_generate_tests(metafunc):
    example_files = glob.glob(os.path.join(root_directory, "*.py"))
    if len(example_files) == 0:
        raise RuntimeError("Couldn't find any examples in directory: %s" % root_directory)

    example_basenames = [os.path.basename(file) for file in example_files]
    metafunc.parametrize("example_basename", example_basenames)

