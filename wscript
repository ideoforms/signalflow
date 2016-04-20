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
import shutil

#------------------------------------------------------------------------
# Don't require a manual ./waf configure before build
#------------------------------------------------------------------------

import waflib.Configure
waflib.Configure.autoconfig = True

import waflib.Options
platform = waflib.Options.platform

def options(opt):
	opt.load('compiler_cxx')

def configure(conf):

	#------------------------------------------------------------------------
	# Use C++11 extensions and add general search paths
	#------------------------------------------------------------------------
	conf.load('compiler_cxx')
	conf.env.CXXFLAGS = ['-std=c++11', '-Wall']
	conf.env.LIBPATH = ['/usr/local/lib']
	conf.env.INCLUDES = ['/usr/local/include', 'lib']

	#------------------------------------------------------------------------
	# Check support for c++11 (required right now)
	#------------------------------------------------------------------------
	conf.check_cxx(cxxflags = [ '-std=c++11' ], mandatory = True)

	#------------------------------------------------------------------------
	# OSX platform-specific flags
	#------------------------------------------------------------------------
	if platform == "darwin":
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

	conf.env.LDFLAGS += [ '-ldl', '-lgslcblas' ]
	conf.check(lib = 'gsl', define_name = 'HAVE_GSL') 
	conf.check(lib = 'gslcblas', define_name = 'HAVE_GSLCBLAS') 

def build(bld):
	libraries = [ 'GSL', 'GSLCBLAS', 'SNDFILE', 'SOUNDIO' ]

	if bld.cmd == "dev":
		bld.env.CXXFLAGS += [ "-g" ]
		#------------------------------------------------------------------------
		# Use define rather than adding -D to build flags, as this ensures
		# that waf correctly refreshes the build when includes within #ifdef
		# are updated.
		#------------------------------------------------------------------------
		bld.define("DEBUG", 1)
	else:
		bld.env.CXXFLAGS += [ "-O3" ]

	bld.env.CXXFLAGS += [ "-Wno-unused-variable" ]

	#------------------------------------------------------------------------
	# Build every .cpp file found within signal as a shared library.
	# Set install_path = "@rpath" sets the internal install_name of the 
	# shared object, used when generating the link paths of binaries
	# compiled against this lib.
	#------------------------------------------------------------------------
	bld.shlib(
		source = bld.path.ant_glob('lib/vamp-hostsdk/*.cpp') + bld.path.ant_glob('lib/json11/json11.cpp') + bld.path.ant_glob('signal/**/*.cpp'),
		target = 'signal',
		vnum = VERSION,
		use = libraries,
		install_path = '@rpath'
	)

	build_dir = "build"

	#------------------------------------------------------------------------
	# Collate source files to build.
	# If sources are specified on the command line (./waf build foo.cpp),
	# only these will be built; otherwise, examples will be built.
	#------------------------------------------------------------------------
	source_files = []
	
	if (waflib.Options.commands):
		source_files += waflib.Options.commands
		waflib.Options.commands = []
	else:
		#------------------------------------------------------------------------
		# Collate all source files found within example folders.
		# If "dev" command is given, also include examples-dev.
		#------------------------------------------------------------------------
		example_dirs = [ "examples" ]
		if bld.cmd == "dev":
			example_dirs += [ "examples-dev" ]

		excl = []
		if platform != "darwin":
			#------------------------------------------------------------------------
			# FFT not yet supported on non-Darwin systems
			#------------------------------------------------------------------------
			excl.append("*/fft*.cpp")
		for example_dir in example_dirs:
			examples = bld.path.ant_glob(os.path.join(example_dir, "*.cpp"), excl = excl)
			for example in examples:
				example_path = os.path.join(example_dir, str(example))
				source_files.append(example_path);

	#------------------------------------------------------------------------
	# Build each source file
	#------------------------------------------------------------------------
	for source_file in source_files:
		#------------------------------------------------------------------------
		# Remove path prefixes to ensure that built binaries go directly
		# in "build".
		#------------------------------------------------------------------------
		target = os.path.basename(source_file)
		target = os.path.splitext(target)[0]

		bld.program(
			features = 'cxx cxxprogram',
			source = source_file,
			target = target,
			includes = [ ".." ],
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

class dev(waflib.Build.BuildContext):
	cmd = 'dev'
