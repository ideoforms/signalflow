import os
import sys
import glob
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(self.build_lib)

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg, '-j', '8']

        python_prefix = subprocess.check_output([ "python3-config", "--prefix" ]).strip().decode()
        libpython_path = glob.glob("%s/lib/libpython*" % python_prefix)[0]
        include_path = glob.glob("%s/include/python**" % python_prefix)[0]

        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DCMAKE_BUILD_PYTHON=1',
                      '-DCMAKE_BUILD_TYPE=' + cfg,
                      '-DPYTHON_LIBRARY=' + libpython_path,
                      '-DPYTHON_INCLUDE_DIR=' + include_path]

        env = os.environ.copy()
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

setup(
    name='signalflow',
    version='0.0.1',
    author='Daniel Jones',
    author_email='dan@erase.net',
    description='signalflow',
    ext_modules=[CMakeExtension('signalflow')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    setup_requires=['pytest-runner'],
    tests_require=['pytest', 'numpy', 'scipy']
)
