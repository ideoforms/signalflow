import os
import sys
import shutil
import tempfile
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(self.build_lib)
        cfg = 'Debug' if self.debug else 'Release'
        cpu_count = os.cpu_count()

        #--------------------------------------------------------------------------------
        # Parallel compilation can exhaust memory on low-RAM Linux systems (e.g.
        # Raspberry Pi), so build serially if less than 8GB is available.
        #--------------------------------------------------------------------------------
        if sys.platform.startswith('linux'):
            with open('/proc/meminfo') as fd:
                mem_total_kb = int(fd.readline().split()[1])
            if mem_total_kb < 8 * 1024 * 1024:
                print("Detected %.1fGB RAM, limiting build to 1 CPU" % (mem_total_kb / 1024 ** 2))
                cpu_count = 1

        build_args = ['--config', cfg, '-j', str(cpu_count)]

        print("Building signalflow version " + self.distribution.get_version())
        print("Detected %d CPUs" % os.cpu_count())
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
            '-DCMAKE_BUILD_PYTHON=1',
            '-DCMAKE_BUILD_TYPE=' + cfg,
            '-DSIGNALFLOW_VERSION=' + self.distribution.get_version(),
            #--------------------------------------------------------------------------------
            # Build against the Python that is running this build, rather than whichever
            # Python CMake finds first. Without this, Windows builds can link against a
            # different Python version (e.g. python312.dll in a Python 3.9 wheel).
            #--------------------------------------------------------------------------------
            '-DPython3_EXECUTABLE=' + sys.executable
        ]
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
            dll_path = os.path.join(self.build_temp, cfg, libname)
            shutil.copy(dll_path, "%s/signalflow" % extdir)

        self.generate_stubs(ext)

    def generate_stubs(self, ext):
        """
        Generate type stubs for IDE type hinting and inline documentation (PEP 561),
        by running pybind11-stubgen on the newly-built module. The stubs are written
        to the signalflow-stubs package within the build directory, so that they are
        included in the wheel without being committed to the repo.

        Failure to generate stubs does not fail the build. The Build wheels workflow
        checks that each wheel includes the stubs.
        """
        extdir = os.path.abspath(self.build_lib)
        stubs_dir = os.path.join(extdir, "signalflow-stubs")
        os.makedirs(stubs_dir, exist_ok=True)

        #--------------------------------------------------------------------------------
        # On Windows, Python does not search PATH for an extension module's DLL
        # dependencies, so register the directories of the dependencies that
        # CMakeLists.txt links against (which are later bundled by delvewheel).
        #--------------------------------------------------------------------------------
        dll_dirs = []
        if os.name == "nt":
            parent_dir = os.path.dirname(ext.sourcedir)
            dll_dirs = [os.path.join(parent_dir, "libsndfile-1.2.2-win64", "bin"),
                        os.path.join(parent_dir, "fftw-3.3.5-dll64")]
        output_dir = tempfile.mkdtemp()
        script = ("import os, runpy, sys\n"
                  "for dll_dir in sys.argv[1:]:\n"
                  "    if os.path.isdir(dll_dir):\n"
                  "        os.add_dll_directory(dll_dir)\n"
                  "sys.argv = ['pybind11-stubgen', '-o', %r, 'signalflow']\n"
                  "runpy.run_module('pybind11_stubgen', run_name='__main__')\n") % output_dir

        env = os.environ.copy()
        env["PYTHONPATH"] = os.pathsep.join([extdir] + ([env["PYTHONPATH"]] if env.get("PYTHONPATH") else []))
        try:
            subprocess.check_call([sys.executable, "-c", script] + dll_dirs, cwd=extdir, env=env)

            #--------------------------------------------------------------------------------
            # On Linux and macOS, signalflow is a single extension module, and the stubs are
            # written to signalflow.pyi. On Windows, signalflow is a package containing the
            # extension module signalflow.signalflow, and the stubs are written to
            # signalflow/signalflow.pyi.
            #--------------------------------------------------------------------------------
            candidates = [os.path.join(output_dir, "signalflow.pyi"),
                          os.path.join(output_dir, "signalflow", "signalflow.pyi")]
            stub_path = next((path for path in candidates if os.path.exists(path)), None)
            if stub_path is None:
                raise RuntimeError("pybind11-stubgen did not generate signalflow.pyi")
            shutil.copy(stub_path, os.path.join(stubs_dir, "signalflow.pyi"))
        except Exception as e:
            print("Warning: Failed to generate type stubs: %s" % e)
            return
        finally:
            shutil.rmtree(output_dir, ignore_errors=True)

        #--------------------------------------------------------------------------------
        # pybind11-stubgen writes signalflow.pyi. Type checkers resolve `import signalflow`
        # to signalflow-stubs/__init__.pyi, which re-exports it.
        #--------------------------------------------------------------------------------
        with open(os.path.join(stubs_dir, "__init__.pyi"), "w") as fd:
            fd.write('"""\n'
                     'Stubs for type hinting and inline documentation.\n'
                     'See PEP 561: https://peps.python.org/pep-0561/\n'
                     '\n'
                     'Autogenerated by pybind11-stubgen during the build (see setup.py).\n'
                     '"""\n'
                     '\n'
                     'from .signalflow import *\n')


signalflow_packages = [
    'signalflow-stubs',
    'signalflow_midi',
    'signalflow_cli',
    'signalflow_vscode',
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
    packages=signalflow_packages,
    ext_modules=[CMakeExtension('signalflow')],
    cmdclass=dict(build_ext=CMakeBuild),
)
