#! /usr/bin/env python

VERSION = '0.0.1'
APPNAME = 'signum'

import os

#------------------------------------------------------------------------
# Don't require a manual ./waf configure before build
#------------------------------------------------------------------------
import waflib.Configure
waflib.Configure.autoconfig = True

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
	# For test applications (before installing in system paths), instruct
	# binaries to search in their current directory for shared libs.
	#------------------------------------------------------------------------
	conf.env.LINKFLAGS = [ "-rpath", "." ]

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
	files = bld.path.ant_glob('test-*.cpp')

	for filename in files:
		target = os.path.splitext(str(filename))[0]
		bld.program(
			features = 'cxx cxxprogram',
			source = str(filename),
			target = target,
			use = libraries + [ 'signum' ]
		)
