import os
import sys
import sysconfig  
import itertools
from ctypes import cdll 
from math import pi
#sys.path.append(  "./module")
sys.path.append("/home/cvah/pkg/src/odb/pyodb/build/lib.linux-x86_64-cpython-310")



binpath="/home/cvah/pkg/bin"
libpath="/home/cvah/pkg/lib"
cdll.LoadLibrary(libpath+'/libodb.so' ) 

from   pyodb_io     import odbCheck    , odbConnect  ,createDca
from   pyodb_info   import pyodbTables , pyodbVarno  
from   pyodb        import pyodbDump

# DB and sql query 
db ="CCMA"
sql="SELECT obstype ,statid,  varno , date, time, obsvalue , an_depar,fg_depar from  hdr, body"

#sql="select distinct  obstype,varno, fg_depar,an_depar,lat, lon ,statid ,date,time, obsvalue ,final_obs_error from hdr , body, errstat"

# BINARIES 
odb_bin={  "ODB_FEBINPATH"  :binpath,
           "ODB_SYSPATH"    :binpath,
           "ODB_BEBINPATH"  :binpath}

for k , v in odb_bin.items(): os.environ[k]=v 


# CONNECT 
#if os.path.isdir(   db + "/dca" ):
#   print( "The DCA files have been already generated")
#else:


# CHECK THE PATH FIRST 
if odbCheck ( db ) ==0  :
   print( "ODB path %s exists " % db )
   h = odbConnect ( db )
   if h > 0:
      print("Successfully connected to ", db , ". ODB handler  h= ",  h )
else:
   sys.exit()



# ADD SOME OPTIONAL ARGS 
# WHEN set degrees ( lat), degrees(lon ) , the é next columns despear !
# CONVERSION LAT LON TO DEGREES ( OPTIONAL)
# FLOAT FORMATING  ( OPTIONAL)
# VERBOSITY        ( OPTIONAL)
# REQUEST FROM INFILE (OPTIONAL)
infile =None
lheader=False
lverbos=False
lstat  =False 
dbl_fmt=None 
ldegree=None 

rows=pyodbDump ( db , sql  , lheader ,   lverbos ,  lstat  )
i=-1
for row in rows:
    i=i+1 
    print( row , i)
#odbClose ( db  )

quit()




quit()

latlon2degree=False
real_fmt     ="%.10f"
fromInfile   =False

if fromInfile:
   nargv = len(sys.argv)
   if nargv > 1 :
      query_file = sys.argv[1]
      if not os.path.exists(query_file) :
         print("File " + query_file + " not found.")
         exit(1)
      else :
         print("Usage:")
         print("> sql query needs t be read from input file \n")
         exit(1)

