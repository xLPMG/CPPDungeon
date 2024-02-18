##
# @author Luca-Philipp Grumbach
#
# # Description
# Entry-point for builds.
##
import SCons
import platform
import os

print('running build script')

#####################
#      GET OS       #
#####################
OS = platform.system()

#####################
#  READ ARGUMENTS   #
#####################
vars = Variables()

vars.AddVariables(
  EnumVariable( 'mode',
                'compile modes, option \'san\' enables address and undefined behavior sanitizers',
                'release',
                allowed_values=('release', 'debug', 'release+san', 'debug+san', 'benchmark')
              ),
  EnumVariable( 'opt',
                'optimization flag',
                '-O2',
                allowed_values=('-O0', 
                                '-O1', 
                                '-O2', 
                                '-O3', 
                                '-Ofast')
              )
)

# exit in the case of unknown variables
if vars.UnknownVariables():
  print( "build configuration corrupted, don't know what to do with: " + str(vars.UnknownVariables().keys()) )
  exit(1)

#####################
#  SET ENVIRONMENT  #
#####################
env = Environment( variables = vars )

# set local env
env['ENV'] = os.environ

#####################
#  COMPILER OPTION  #
#####################
if 'CXX' in os.environ:
  env['CXX'] = os.environ['CXX']
print("Using ", env['CXX'], " compiler.")

#####################
#  LOAD LIBRARIES   #
#####################

# macOS magic for finding libraries with gcc
if OS == "Darwin":
  env.Append( CXXFLAGS = [  '-I/usr/local/include/' ] )
  env.Append ( LIBPATH = [ '/usr/local/lib' ])

conf = Configure(env)

if OS == "Linux":
  if not conf.CheckLib('X11'):
    print ('Did not find the X11 library!')
    exit(1)
  elif not conf.CheckLib('GL'):
    print ('Did not find the GL library!')
    exit(1)
  elif not conf.CheckLib('png'):
    print ('Did not find the png library!')
    exit(1)
elif OS == "Windows":
  if not conf.CheckLib('user32'):
    print ('Did not find the user32 library!')
    exit(1)
  elif not conf.CheckLib('gdi32'):
    print ('Did not find the gdi32 library!')
    exit(1)
  elif not conf.CheckLib('opengl32'):
    print ('Did not find the opengl32 library!')
    exit(1)
  elif not conf.CheckLib('gdiplus'):
    print ('Did not find the gdiplus library!')
    exit(1)
  elif not conf.CheckLib('Shlwapi'):
    print ('Did not find the Shlwapi library!')
    exit(1)
  elif not conf.CheckLib('dwmapi'):
    print ('Did not find the dwmapi library!')
    exit(1)
  elif not conf.CheckLib('png'):
    print ('Did not find the png library!')
    exit(1)
elif OS == "Darwin":
    if not conf.CheckLib('png'):
        print ('Did not find the png library!')
        exit(1)
    env.AppendUnique(FRAMEWORKS=Split('OpenGL GLUT Carbon CoreGraphics Foundation'))

env.Append( CXXFLAGS = [ '-isystem', 'lib' ] )

env = conf.Finish()

# generate help message
Help( vars.GenerateHelpText( env ) )

####################
#  ENABLE THREADS  #
####################
env.Append( LINKFLAGS = [ '-lpthread' ] )
if OS != "Darwin": 
  env.Append( CXXFLAGS = [ '-pthread' ] )

#####################
#   DEFAULT FLAGS   #
#####################
if OS == "Darwin":
  env.Append( CXXFLAGS = [ '-std=c++20',
                           '-Wall',
                           '-Wextra',
                           '-g' ] )
else:
  env.Append( CXXFLAGS = [ '-std=c++20',
                           '-Wall',
                           '-Wextra',
                           '-Werror',
                           '-Wpedantic',
                           '-g' ] )

#####################
# OPTIMIZATION MODE #
#####################
if 'debug' in env['mode']:
  env.Append( CXXFLAGS = [ '-g',
                           '-O0' ] )
else:
  env.Append( CXXFLAGS = [ env['opt'] ] )

#####################
#    SANITIZERS    #
#####################
if 'san' in  env['mode']:
  env.Append( CXXFLAGS =  [ '-g',
                            '-fsanitize=float-divide-by-zero',
                            '-fsanitize=bounds',
                            '-fsanitize=address',
                            '-fsanitize=undefined',
                            '-fno-omit-frame-pointer' ] )
  env.Append( LINKFLAGS = [ '-g',
                            '-fsanitize=address',
                            '-fsanitize=undefined' ] )

#####################
# GET SOURCE FILES  #
#####################
VariantDir( variant_dir = 'build/src',
            src_dir     = 'src' )

env.sources = []

Export('env')
SConscript( 'build/src/SConscript' )
Import('env')

#####################
#  SPECIFY TARGETS  #
#####################

env.Program( target = 'build/dungeon',
               source = env.sources + env.standalone )