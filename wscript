#! /usr/bin/env python

#------------------------------------------------------------------------
# waf build script for signal.
#
# To build in production mode (optimisation flags, strip debug syms):
#
#   ./waf
# 
# To build in debug mode (no optimisation):
#
#   ./waf dev
#
# To clean up:
#
#   ./waf clean
#
#------------------------------------------------------------------------

VERSION = '0.0.1'
APPNAME = 'signal'

import os
import sys
import shutil

#------------------------------------------------------------------------
# Don't require a manual ./waf configure before build
#------------------------------------------------------------------------

from waflib import TaskGen, Options
import waflib.Configure
waflib.Configure.autoconfig = True

#------------------------------------------------------------------------
# Load a C++ compiler
#------------------------------------------------------------------------
def options(opt):
    opt.load('compiler_cxx')
    opt.add_option('--shared', action='store_true', default=False,
            help='build shared library')
    opt.add_option('--debug', action='store_true', default=False,
            help='enable debug logging')

#------------------------------------------------------------------------
# Build mode 'dev' enables debugging symbols and disables optimisation.
#------------------------------------------------------------------------
class dev(waflib.Build.BuildContext):
    cmd = 'dev'

#------------------------------------------------------------------------
# Support for Objective-C++ files, required for OS X AppKit bindings.
#------------------------------------------------------------------------
@TaskGen.extension('.mm')
def m_hook(self, node):
    return self.create_compiled_task('cxx', node)

def configure(conf):
    #------------------------------------------------------------------------
    # Use C++11 extensions and add general search paths
    #------------------------------------------------------------------------
    conf.load('compiler_cxx')
    conf.env.CXXFLAGS = [
        '-std=c++11',
        '-Wall',
        '-Wno-unused-variable'
    ]
    conf.env.LIBPATH = ['/usr/local/lib']
    conf.env.INCLUDES = ['/usr/local/include', 'include', 'lib', 'lib/pybind11/include']

    #------------------------------------------------------------------------
    # Check support for c++11 (required right now)
    #------------------------------------------------------------------------
    conf.check_cxx(cxxflags = [ '-std=c++11' ], mandatory = True)

    #------------------------------------------------------------------------
    # OSX platform-specific flags
    #------------------------------------------------------------------------
    if sys.platform == "darwin":
        #------------------------------------------------------------------------
        # For test applications (before installing in system paths), instruct
        # binaries to search in their current directory for shared libs.
        #------------------------------------------------------------------------
        conf.env.LINKFLAGS = [ "-rpath", "." ]
    
        #------------------------------------------------------------------------
        # Require Accelerate framework for vectorised FFT and other
        # optimisations
        #------------------------------------------------------------------------
        conf.env.LINKFLAGS += [ "-framework", "Accelerate" ]
        conf.env.LINKFLAGS += [ "-framework", "Foundation", "-framework", "AppKit" ]

        #------------------------------------------------------------------------
        # Need to define __APPLE__ for waf to detect changes in #include within
        # platform-specified #iddef
        #------------------------------------------------------------------------
        conf.define("__APPLE__", 1)

    #------------------------------------------------------------------------
    # Setup library includes
    #------------------------------------------------------------------------
    conf.check(lib = 'sndfile', define_name = 'HAVE_SNDFILE') 
    conf.check(lib = 'soundio', define_name = 'HAVE_SOUNDIO') 

    conf.env.LDFLAGS += [ '-ldl', '-lgslcblas', '-pthread' ]
    conf.check(lib = 'gsl', define_name = 'HAVE_GSL') 
    conf.check(lib = 'gslcblas', define_name = 'HAVE_GSLCBLAS') 

def build(bld):
    libraries = [ 'GSL', 'GSLCBLAS', 'SNDFILE', 'SOUNDIO' ]

    if bld.cmd == "dev":
        bld.env.CXXFLAGS += [
            "-g",
            "-O0"
        ]

    else:
        bld.env.CXXFLAGS += [
            "-O3",
            "-funroll-loops"
        ]

    #------------------------------------------------------------------------
    # Use define rather than adding -D to build flags, as this ensures
    # that waf correctly refreshes the build when includes within #ifdef
    # are updated.
    #------------------------------------------------------------------------
    if Options.options.debug:
        bld.define("DEBUG", 1)
    
    import subprocess
    pycxxflags = subprocess.Popen([ "python3-config", "--cflags"  ], stdout = subprocess.PIPE).communicate()[0]
    pyldflags = subprocess.Popen([ "python3-config", "--ldflags", "--libs" ], stdout = subprocess.PIPE).communicate()[0]
    bld.env.CXXFLAGS += pycxxflags.split()
    bld.env.CXXFLAGS += [ "-I", "/usr/local/include/pybind11" ]
    bld.env.LDFLAGS += pyldflags.split()

    #------------------------------------------------------------------------
    # Build every .cpp file found within signal as a shared library.
    # Set install_path = "@rpath" sets the internal install_name of the 
    # shared object, used when generating the link paths of binaries
    # compiled against this lib.
    #------------------------------------------------------------------------
    source_files = []
    source_files += bld.path.ant_glob('lib/json11/json11.cpp')
    source_files += [
        'src/core.cpp',
        'src/util.cpp',
        'src/node.cpp',
        'src/monitor.cpp',
        'src/graph.cpp',
        'src/registry.cpp',
        'src/buffer.cpp',
        'src/synth.cpp',
        'src/synthregistry.cpp',
        'src/synthspec.cpp',
        'src/synthtemplate.cpp',
        'src/nodedef.cpp',
        'src/python.cpp',
    ]

    source_files += bld.path.ant_glob('src/operators/*.cpp')
    source_files += [ 'src/io/output/soundio.cpp', 'src/io/output/abstract.cpp' ]
    source_files += [ 'src/fft/fft.cpp' ]

    source_files += bld.path.ant_glob('src/chance/*.cpp')
    source_files += bld.path.ant_glob('src/control/*.cpp')
    source_files += bld.path.ant_glob('src/envelope/*.cpp')
    source_files += bld.path.ant_glob('src/fft/*.cpp')
    source_files += bld.path.ant_glob('src/filters/*.cpp')
    source_files += bld.path.ant_glob('src/io/*/*.cpp')
    source_files += bld.path.ant_glob('src/oscillators/*.cpp')
    source_files += bld.path.ant_glob('src/sequencing/*.cpp')
    if sys.platform == "darwin" or sys.platform == "ios":
        source_files += bld.path.ant_glob('src/**/*.mm')

    library_params = {
        "source" : source_files,
        "target" : "signal",
        "vnum" : VERSION,
        "use" : libraries,
        "install_path" : "@rpath"
    }

    if Options.options.shared:
        bld.shlib(**library_params)
    else:
        bld.stlib(**library_params)

    build_dir = "build"

    #------------------------------------------------------------------------
    # Collate source files to build.
    # If sources are specified on the command line (./waf build foo.cpp),
    # only these will be built; otherwise, examples will be built.
    #------------------------------------------------------------------------
    program_files = []
    
    if (Options.commands):
        program_files += Options.commands
        Options.commands = []
    else:
        #------------------------------------------------------------------------
        # Collate all source files found within example folders.
        # If "dev" command is given, also include examples-dev.
        #------------------------------------------------------------------------
        example_dirs = [ "examples" ]
        if bld.cmd == "dev":
            example_dirs += [ "examples-dev" ]

        excl = []
        if sys.platform != "darwin" and sys.platform != "ios":
            #------------------------------------------------------------------------
            # FFT not yet supported on non-Darwin systems
            #------------------------------------------------------------------------
            excl.append("*/fft*.cpp")

            #------------------------------------------------------------------------
            # Mouse control not yet supported on non-Darwin systems
            #------------------------------------------------------------------------
            excl.append("*/mouse*.cpp")
        for example_dir in example_dirs:
            program_files += bld.path.ant_glob(os.path.join(example_dir, "*.cpp"), excl = excl)
        # program_files += [ "examples/hello-world.cpp" ]

    #------------------------------------------------------------------------
    # Build each source file.
    # Need to convert to relative paths for wscript to find source files.
    #------------------------------------------------------------------------
    for program_file in program_files:
        program_file = str(program_file)
        program_file = os.path.relpath(program_file, ".")

        #------------------------------------------------------------------------
        # Remove path prefixes to ensure that built binaries go directly
        # in "build".
        #------------------------------------------------------------------------
        target = os.path.basename(program_file)
        target = os.path.splitext(target)[0]

        bld.program(
            features = 'cxx cxxprogram',
            source = program_file,
            target = target,
            includes = [ "includes", ".." ],
            use = libraries + [ 'signal' ],
        )
    
    #------------------------------------------------------------------------
    # Copy example audio to build directory.
    #------------------------------------------------------------------------
    for resource_type in [ "audio", "synths" ]:
        resource_dir = os.path.join(build_dir, resource_type)
        if os.path.exists(resource_dir):
            shutil.rmtree(resource_dir)
        shutil.copytree(os.path.join("examples", resource_type), resource_dir)
