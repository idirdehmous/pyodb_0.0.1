from distutils.core import setup, Extension
import os  
import sysconfig
import importlib.machinery as imp
from Cython.Distutils import build_ext
from distutils.sysconfig import customize_compiler, get_config_vars


# CLASS ( make the C extension suffix shorter .so  )
class NoSuffixBuilder(build_ext):
    def get_ext_filename(self, ext_name):
        filename = super().get_ext_filename(ext_name)
        suffix = sysconfig.get_config_var('EXT_SUFFIX')
        ext = os.path.splitext(filename)[1]
        return filename.replace(suffix, "") + ext




# WHERE TO FIND libodb.so  
odb_env="/home/cvah/pkg"

# COMPILER FLAGS 
compiler_opts = sysconfig.get_config_var('CFLAGS').split()
compiler_opts += [ "-Wall", "-Wextra", "-Wno-sign-compare" ]
#                   "-Wno-unused-variable", "-Wno-unused-function",
#                   "-Wno-unused-parameter","-Wno-return-type","-Wno-missing-field-initializers",
#                   "-Wno-unused-but-set-variable","-Wno-int-conversion"]

# MODULE SUFFIX 
sfx  =  sysconfig.get_config_var('EXT_SUFFIX')



# MODULE NAMS 
name1="pyodb_io"
name2="pyodb_info"
name3="pyodb"
pwd=os.getenv("PWD")

m1=Extension( name1, ["src/io_module.c"],
                
                     include_dirs =[ './include'],
                     extra_objects=[ odb_env+'/lib/libodb.so' ], 
                     extra_compile_args =  compiler_opts ,
                     language='c') 

m2=Extension( name2 ,      ["src/info_module.c"],
                     include_dirs =[ './include'],
                     extra_compile_args =  compiler_opts ,
                     extra_objects=[ odb_env+'/lib/libodb.so' ],
                     language='c') 


m3=Extension( name3 ,      ["src/dump_module.c"],
                     include_dirs =[ './include'],
                     extra_compile_args =  compiler_opts ,
                     extra_objects=[ odb_env+'/lib/libodb.so' ],
                     language='c') 




# CREATE THE EXTENSION !
setup( name="pyodb",
       version="0.0.1",
       description="Python interface for C odb ",
       author="blala",
       author_email="your_email@gmail.com",
       ext_modules=[  m1, m2 , m3   ], 
       package_data={'pyodb': ['libodb.so']},
       cmdclass={"build_ext": NoSuffixBuilder},
     )







