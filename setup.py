import os
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
        cpu_count = os.cpu_count()
        build_args = ['--config', cfg, '-j', str(cpu_count)]

        print("Building signalflow version " + self.distribution.get_version())
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DCMAKE_BUILD_PYTHON=1',
                      '-DCMAKE_BUILD_TYPE=' + cfg,
                      '-DSIGNALFLOW_VERSION=' + self.distribution.get_version()]
        if 'CMAKE_OSX_ARCHITECTURES' in os.environ:
            cmake_args += ['-DCMAKE_OSX_ARCHITECTURES=%s' % os.environ['CMAKE_OSX_ARCHITECTURES']]

        env = os.environ.copy()
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

setup(
    name='signalflow',
    version='0.4.2',
    author='Daniel Jones',
    author_email='dan@erase.net',
    description='signalflow',
    long_description = open("README.md", "r").read(),
    long_description_content_type = "text/markdown",
    ext_modules=[CMakeExtension('signalflow')],
    cmdclass=dict(build_ext=CMakeBuild),
    setup_requires=['pytest-runner', 'pybind11-stubgen'],
    install_requires=['numpy'],
    tests_require=['pytest', 'numpy', 'scipy'],
    package_dir={ '': 'auxiliary/libs' },
    packages=['signalflow_midi', 'signalflow-stubs', 'signalflow_examples'],
    include_package_data=True,
    # signalflow-stubs contains type hint data in a .pyi file, per PEP 561
    package_data={"signalflow-stubs": ["*.pyi"]},
    scripts=['source/bin/signalflow']
)

