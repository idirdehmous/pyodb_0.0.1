#define PY_SSIZE_T_CLEAN
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <Python.h>
#include "odbdump.h"
//#include "pyspam.h"
//#include "exceptions.h"


/* ENDIANESS */
extern int ec_is_little_endian();
extern double util_walltime_();

// C wrapper
static PyObject *odbConnect_method(PyObject *self, PyObject *args) {
    char *database = NULL ;

    Py_Initialize() ;
    if(!PyArg_ParseTuple(args, "s", &database )) {
        return NULL;
    }
  int  ODB_OK           ; 
  char *varvalue  = NULL;
  char *poolmask  = NULL;
  char *sql_query = NULL; 
  char *queryfile = NULL;
  int  maxlines   = -1  ;
  void *h         = NULL;
  int maxcols     = 0   ;
  int rc          = 0   ;


 if (maxlines == 0) return PyLong_FromLong(rc);
 ODB_OK = h ; 
 sql_query="select * from hdr, body" ;
 printf ("Opening the odb  :  %s \n",   database  ) ;
 printf (  "%s\n" , "Generating DCA files ..." ) ;
 //PyObject_Print( PyObjectString_FromString("Generating DCA files ... ") , stdout  , 0 );
 h     = odbdump_open(database, sql_query, queryfile, poolmask, varvalue, &maxcols);
 if ( h ) {
         return  PyLong_FromLong( h )  ;
 }else {
	 PyErr_SetString(PyExc_ConnectionError  , "Internal connection error " ) ; 
         //return  PyLong_FromLong( -1 ) ;
      }
}
 




static PyObject *createDca_method(PyObject *self, PyObject *args) {
    char *database = NULL ;

    Py_Initialize() ;
    if(!PyArg_ParseTuple(args, "s", &database )) {
        return NULL;
    }
  char *varvalue  = NULL;
  char *poolmask  = NULL;
  char *sql_query = NULL;
  char *queryfile = NULL;
  void *h         = NULL;
  int maxlines    = -1  ;
  int maxcols     = 0   ;
  int rc          = 0   ;
 if (maxlines == 0) return PyLong_FromLong(rc);
 sql_query="select * from hdr, body" ;

 h     = odbdump_open(database, sql_query, queryfile, poolmask, varvalue, &maxcols);
 if ( h) {
         return  PyLong_FromLong( h )  ;
 }else {
	 // CHECK THE dcagen process  ( later ! )
         PyErr_SetString(PyExc_ConnectionAbortedError   , "Failed to create DCA files " ) ;
         //return  PyLong_FromLong( -1 ) ;
      }
}


static PyObject *odbClose_method(PyObject *self, PyObject *args) {
    char *database = NULL;
    // TO CLOSE THE ODB HARD CODE A GIVEN SQL QUERY 
    // SUPPRESS THE handler  h 
    char *sql_query= "select * from hdr";

    Py_Initialize() ;
    if(!PyArg_ParseTuple(args, "s", &database )) {
        return NULL;
    }
  char *poolmask  = NULL;
  char *varvalue  = NULL;
  char *queryfile = NULL;
  void *h         = NULL;
  int maxcols     = 0;
  int rc  ;  
  h     = odbdump_open(database, sql_query, queryfile, poolmask, varvalue, &maxcols);
  if ( h)  {
    colinfo_t *ci   = NULL;
    int nci = 0           ; 
    ci = odbdump_destroy_colinfo(ci, nci);
    rc = odbdump_close(h);
    Py_Finalize() ;
  }    
  else {
    rc = -1;
  }
     return PyLong_FromLong(0) ;
     Py_Finalize() ;
}






static PyMethodDef module_methods[] = {
    {"odbConnect",  (PyCFunction)(void(*)(void))   odbConnect_method ,
     METH_VARARGS | METH_KEYWORDS,   "Create odb connection   "},
    {"createDca" ,  (PyCFunction)(void(*)(void))    createDca_method  ,
     METH_VARARGS | METH_KEYWORDS,   "Create Direct Columns Access files "},
    {"odbClose"  ,  (PyCFunction)(void(*)(void))    odbClose_method  ,
     METH_VARARGS | METH_KEYWORDS,   "Create Direct Columns Access files "},


};


// Modules definition
static struct PyModuleDef   odbmodule = {
    PyModuleDef_HEAD_INIT,
    "pyodb_io",
    "Python interface for odb C ",
    -1,
    module_methods ,
};


// Called first during python call
PyMODINIT_FUNC PyInit_pyodb_io (void) {
    return PyModule_Create(&odbmodule);
}

