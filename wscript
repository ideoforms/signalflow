#! /usr/bin/env python

#------------------------------------------------------------------------
# waf build script for signum.
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
APPNAME = 'signum'

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
	conf.env.INCLUDES = ['/usr/local/include']

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
	# Setup library includes
	#------------------------------------------------------------------------
	conf.check(lib = 'sndfile', define_name = 'HAVE_SNDFILE') 
	conf.check(lib = 'soundio', define_name = 'HAVE_SOUNDIO') 
	conf.check(lib = 'gsl', define_name = 'HAVE_GSL') 

def build(bld):
	libraries = [ 'GSL', 'SNDFILE', 'SOUNDIO' ]

	if bld.cmd == "dev":
		bld.env.CXXFLAGS += [ "-g" ]
	else:
		bld.env.CXXFLAGS += [ "-O3" ]

	#------------------------------------------------------------------------
	# Build every .cpp file found within signum as a shared library.
	# Set install_path = "@rpath" sets the internal install_name of the 
	# shared object, used when generating the link paths of binaries
	# compiled against this lib.
	#------------------------------------------------------------------------
	bld.shlib(
		source = bld.path.ant_glob('signum/**/*.cpp'),
		target = 'signum',
		vnum = VERSION,
		use = libraries,
		install_path = '@rpath'
	)

	#------------------------------------------------------------------------
	# Build example files.
	#------------------------------------------------------------------------
	build_dir = "build"

	example_dirs = [ "examples" ]
	if bld.cmd == "dev":
		example_dirs += [ "examples-dev" ]

	for example_dir in example_dirs:
		examples = bld.path.ant_glob(os.path.join(example_dir, "*.cpp"))
		for example in examples:
			example_path = os.path.join(example_dir, str(example))
			example_target = os.path.splitext(str(example))[0]
			bld.program(
				features = 'cxx cxxprogram',
				source = example_path,
				target = example_target,
				includes = [ ".." ],
				use = libraries + [ 'signum' ],
			)
		
	#------------------------------------------------------------------------
	# Copy example audio to build directory.
	#------------------------------------------------------------------------
	if not os.path.exists(os.path.join(build_dir, "audio")):
		shutil.copytree(os.path.join("examples", "audio"), os.path.join(build_dir, "audio"))

class dev(waflib.Build.BuildContext):
	cmd = 'dev'
