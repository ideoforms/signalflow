#! /usr/bin/env python

#------------------------------------------------------------------------
# waf build script for signum.
# To run:
#
#   ./waf
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
	# Use C++1z extensions and add general search paths
	#------------------------------------------------------------------------
	conf.load('compiler_cxx')
	conf.env.CXXFLAGS = ['-std=c++1z', '-Wall', '-g']
	conf.env.LIBPATH = ['/usr/local/lib']
	conf.env.INCLUDES = ['/usr/local/include']

	#------------------------------------------------------------------------
	# Check support for c++1z (required right now)
	#------------------------------------------------------------------------
	conf.check_cxx(cxxflags = [ '-std=c++1z' ], mandatory = True)

	#------------------------------------------------------------------------
	# For test applications (before installing in system paths), instruct
	# binaries to search in their current directory for shared libs.
	#------------------------------------------------------------------------
	if platform == "darwin":
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
	example_dir = "examples"
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
		shutil.copytree(os.path.join(example_dir, "audio"), os.path.join(build_dir, "audio"))

