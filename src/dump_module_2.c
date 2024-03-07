#define PY_SSIZE_T_CLEAN
#define CHUNK_SIZE 8192
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <Python.h>
#include "odbdump.h"
#include "pyspam.h"

/* ENDIANESS */
extern int ec_is_little_endian();
extern double util_walltime_();



// C wrapper 
static PyObject *pyodbDump_method(PyObject *self, PyObject *args) {
    char *database = NULL;
    char *sql_query= NULL;
    
    Py_Initialize() ;

    PyObject *st ,*dt , *ttm , *int4, *fl ; 



    if(!PyArg_ParseTuple(args, "ss", &database, &sql_query )) {
        return NULL;
    }

  //int i_am_little = ec_is_little_endian();
  char *poolmask  = NULL;
  char *varvalue  = NULL;
  //char *outfile   = NULL;
  char *queryfile = NULL;
  Bool print_mdi  = true; /* by default prints "NULL", not value of the NULL */
  //Bool debug_on   = true;  to use later !
  Bool raw        = true;
  //const char dummydb[] = "$ODB_SYSDBPATH/DUMMY";
  int maxlines = -1;
  //char *dbl_fmt   = NULL;   /to be used later 
  void *h         = NULL;
  int maxcols     = 0;
  int rc          = 0;
  //double wlast;
  extern int optind;
 
  Bool get_header=true ; 

  if (maxlines == 0) return PyLong_FromLong(rc);

  //printf ("Opening the odb  :  %s \n",   database  ) ;
  printf ("Executing query  :  %s \n",   sql_query ) ;


  //wlast = util_walltime_(); to use later for time ellapsed !

  // open the odb 
  h     = odbdump_open(database, sql_query, queryfile, poolmask, varvalue, &maxcols);
  
  if (h && maxcols > 0) {

    PyObject*  py_hdr    = PyList_New(maxcols) ;  
    PyObject*  py_row    = PyList_New(0) ;
    Py_ssize_t irow      = -1  ;

    printf( "Number of feteched columns : %d \n" , maxcols  )  ; 

    int new_dataset = 0   ;
    colinfo_t *ci   = NULL;
    int nci = 0           ; // NUMBER OF COLUMNS IN QUERY 
    double *d = NULL;       // DATA VALUES C VARIABLE 
    int nd          ; 
    ll_t nrows =   0;
    ll_t nrtot =   0;
    //int  ncols =   0;  / to delete  
    Bool packed = false;
    int (*nextrow)(void *, void *, int, int *) = 
      packed ? odbdump_nextrow_packed : odbdump_nextrow;
    int dlen = packed ? maxcols * sizeof(*d) : maxcols;

    ALLOCX(d, maxcols);
    
    while ( (nd = nextrow(h, d, dlen, &new_dataset)) > 0) {
      Py_ssize_t i ;
      PyObject*  py_col    = PyList_New(maxcols) ;
      if (new_dataset) {
	/* New query ? */
	//CLEAN STRUCTURES 
	ci = odbdump_destroy_colinfo(ci, nci);
	ci = odbdump_create_colinfo(h, &nci);

	// get_header=true ; 
       if (get_header ) {	 
             for (i=0; i<nd; i++) {
              colinfo_t *pci = &ci[i];
	  PyObject *hdr ; 
          //printf(  "%s\n"  ,  pci->name ) ; 
	  if ( pci->nickname !=NULL  ) {
	  hdr = PyUnicode_FromFormat (pci->nickname)  ; 
	  if (hdr == NULL ) {
                    return NULL   ;
	  }

	  PyList_SetItem( py_hdr , i , hdr  ) ;
	  }else {
	  hdr = PyUnicode_FromFormat (pci->name)  ;
	  if (hdr  == NULL ) {
                    return NULL   ;
	  }

	  PyList_SetItem( py_hdr , i , hdr  ) ;

	  }
	  Py_DECREF (hdr ) ;
	 }
       }
       new_dataset = 0;
       nrows = 0;
      
      }  /* if  NEW DATASET */

      if(raw){
	Py_ssize_t  icol= -1 ; 

	for (Py_ssize_t i=0; i<nd; i++) {
            colinfo_t *pci = &ci[i];
	  if (print_mdi && pci->dtnum != DATATYPE_STRING && ABS(d[i]) == mdi) {
             printf ( "%s\n returned NULL " ,  "NULL" ) ;
             //return NULL   ;
	  }
	  else {

	    switch (pci->dtnum) {

	    case DATATYPE_STRING:
	      {
		long unsigned int js  ;
		char cc[sizeof(double)+1];
		char *scc = cc ;
		union {
		  char s[sizeof(double)] ;
		  double d;
		} u;
	        
                u.d = d[i];

                for (js=0; js<sizeof(double); js++) {
                  char c = u.s[js];

               *scc++ = isprint(c) ? c : '8' ; /* unprintables as blanks  REPLACE BY 8 arbitrary choice , ( juste for the col source@hdr)    */
                } /* for (js=0; js<sizeof(double); js++) */
                *scc = '\0';
		icol ++  ;  

		if (PyLong_Check( PyLong_FromLong(icol) ) ){
			;
		}; 

	        st=PyUnicode_FromFormat (cc )  ; 
		if ( PyUnicode_Check(st) ) {
			;
		}
                if (st == NULL ) {  
		   // return PyUnicode_FromFormat ( "NULL" )  ;
		   return NULL ; 
		} 
		PyList_SetItem ( py_col , icol , st  ) ;
		if (py_col == NULL ) {
                  Py_DECREF(py_col);
                  Py_DECREF(st);
                  return NULL ;
                  }

	      }
	      break;
         case DATATYPE_YYYYMMDD:
               icol++  ;
               if (PyLong_Check( PyLong_FromLong(icol) ) ){
                        ;
               };
	       dt= PyLong_FromLong((int)d[i])     ; 
               if (dt == NULL ) {
                   return PyUnicode_FromFormat ( "NULL" )  ;
                 }
	       PyList_SetItem(py_col, icol , dt  ) ;
              if (py_col == NULL ) {
                  // ERROR!
                  Py_DECREF(py_col);
		  Py_DECREF(dt);
                  return NULL;
                  }
	       break;

	  case DATATYPE_HHMMSS:
	       icol++  ;
	       if (PyLong_Check( PyLong_FromLong(icol) ) ){
                        ;
               };
	       ttm= PyLong_FromLong( (int) d[i] )     ;
               if ( ttm  == NULL ) {
                   return PyUnicode_FromFormat ( "NULL" )  ;
                }
	       PyList_SetItem(py_col, icol , ttm )    ;
	       if (py_col == NULL ) {
                  Py_DECREF(py_col);
                  Py_DECREF(ttm);
                  return NULL;
                  }

               break; 
          case DATATYPE_INT4:
              icol++  ;
              int4 = PyLong_FromLong(d[i] );
	      if (int4 == NULL ) {  
	         return NULL   ;
	      }
              PyList_SetItem(py_col , icol , int4 )  ;
              if (py_col == NULL ) {
                  Py_DECREF(py_col);
                  Py_DECREF(int4);
                  return NULL;
                  }
              break;
	  default:
              icol++ ;

              fl = PyFloat_FromDouble( d[i] )  ;
              if (fl == NULL ) {
                 //return PyUnicode_FromFormat ( "NULL" )  ;
		 return NULL ; 
              }
              PyList_SetItem(py_col , icol ,fl ) ;
              if (py_col == NULL ) {
                  Py_DECREF(py_col);
                  Py_DECREF(fl);
                  return NULL;
              }
              break;

	    } /* switch (pci->dtnum) */
	  }
	} /* for (i=0; i<nd; i++) */
	
       } /* if (!raw_binary)*/

    //next_please:
      ++nrows;
      ++irow ; 

     PyList_Append(py_row , py_col   ) ;
     Py_ssize_t nbcols = PyList_Size(py_col);
     // reset list references !
     py_col=list_reset (&py_col, nbcols ) ;  

     if ( nrows  > 20  )   break ; 
     if (maxlines > 0 && ++nrtot >= maxlines) break; /* while (...) */
     Py_DECREF ( nrows) ; 
    } /* while (...) */

    ci = odbdump_destroy_colinfo(ci, nci);
    rc = odbdump_close(h);

    FREEX(d);
    return    py_row   ;

    Py_Finalize() ;
  }    /* if (h && maxcols > 0) ... */
  else {
    rc = -1;
  }
     return PyLong_FromLong(0) ; 
     Py_Finalize() ;
}




static PyMethodDef module_methods[] = {
    {"pyodbDump",  (PyCFunction)(void(*)(void))   pyodbDump_method ,
     METH_VARARGS | METH_KEYWORDS,   "dump a given  ODB database  "},

};


// Modules definition
static struct PyModuleDef   odbmodule = {
    PyModuleDef_HEAD_INIT,
    "pyodb",
    "Python interface for odb C ",
    -1,
    module_methods ,
};


// Called first during python call
PyMODINIT_FUNC PyInit_pyodb(void) {
    return PyModule_Create(&odbmodule);
}





