#------------------------------------------------------------------------
# This file is included so that pytest can find the package
# root and import the `supercollider` module from a local relative path.
#
# https://docs.pytest.org/en/latest/goodpractices.html#test-package-name
#------------------------------------------------------------------------

import os
import sys
import sysconfig

#------------------------------------------------------------------------
# Add the build path of the .so to Python's path, so that it can be
# found when running unit tests.
# https://stackoverflow.com/questions/14320220/testing-python-c-libraries-get-build-path
#------------------------------------------------------------------------
def distutils_dir_name(dir_name):
    """Returns the name of a distutils build directory"""
    f = "{dirname}.{platform}-{version[0]}.{version[1]}"
    return f.format(dirname=dir_name,
                    platform=sysconfig.get_platform(),
                    version=sys.version_info)

build_dir = os.path.join("build", distutils_dir_name("lib"))
sys.path.append(build_dir)
