import os
import sys
import shutil
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
        print("Detected %d CPUs" % os.cpu_count())
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

        # --------------------------------------------------------------------------------
        # On Windows, bundle the .pyd into a package.
        # This will also contain the .dlls after repairing the wheel with delvewheel.
        # --------------------------------------------------------------------------------
        if os.name == "nt":
            libname = "signalflow.pyd"
            shutil.copy(os.path.join(self.build_temp, cfg, libname), "auxiliary/libs/signalflow")


signalflow_packages = [
    'signalflow-stubs',
    'signalflow_midi',
    'signalflow_cli',
    'signalflow_examples',
    'signalflow_visualisation',
    'signalflow_analysis',
]
signalflow_package_data = []
if sys.platform == 'win32':
    # --------------------------------------------------------------------------------
    # On Linux and macOS, the bare .so file is packaged directly via ext_modules.
    # On Windows, a separate package is created, into which the .pyd file and DLLs
    # are copied.
    # --------------------------------------------------------------------------------
    signalflow_packages = ['signalflow'] + signalflow_packages
    signalflow_package_data = ['*.pyd']

setup(
    name='signalflow',
    version='0.4.6',
    author='Daniel Jones',
    author_email='dan@erase.net',
    description='SignalFlow is a sound synthesis library designed to make it quick and intuitive to explore complex sonic ideas',
    keywords=['audio', 'sound', 'synthesis', 'dsp', 'sound-synthesis'],
    classifiers=[
        'Topic :: Multimedia :: Sound/Audio',
        'Topic :: Multimedia :: Sound/Audio :: Sound Synthesis',
        'Topic :: Artistic Software',
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers'
    ],
    long_description=open("README.md", "r").read(),
    long_description_content_type="text/markdown",
    ext_modules=[CMakeExtension('signalflow')],
    cmdclass=dict(build_ext=CMakeBuild),
    python_requires=">=3.8",
    setup_requires=['pytest-runner', 'pybind11-stubgen'],
    install_requires=['numpy'],
    tests_require=['pytest', 'numpy', 'scipy'],
    package_dir={'': 'auxiliary/libs'},
    packages=signalflow_packages,
    include_package_data=True,
    # signalflow-stubs contains type hint data in a .pyi file, per PEP 561
    package_data={
        "signalflow-stubs": ["*.pyi"],
        "signalflow": signalflow_package_data
    },
    entry_points = {
        'console_scripts': [
            'signalflow = signalflow_cli:main',
        ],
    }
)
