#define PY_SSIZE_T_CLEAN
#include  <stdio.h> 	
#include  <sys/stat.h>
#include  "Python.h"



/*extern PyObject* pyodb_Error;
extern PyObject* pyodb_Warning;
extern PyObject* pyodb_InterfaceError;
extern PyObject* pyodb_DatabaseError;
extern PyObject* pyodb_InternalError;
extern PyObject* pyodb_OperationalError;
extern PyObject* pyodb_ProgrammingError;
extern PyObject* pyidb_IntegrityError;
extern PyObject* pyodb_DataError;
extern PyObject* pyodb_NotSupportedError;
*/


#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>


char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    //return PyString_FromString(result) ;
    return result ; 
}



//RESET LIST ITEM MEMORY ADRESSES 
static PyObject*  list_reset(PyObject* *list_col , Py_ssize_t ncols )
{
    *list_col = PyList_New (ncols) ;
    
     return *list_col ;
}

int  check_path( const char* path )
{
  
    struct stat sb;

    if (stat( path , &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return 0 ;
    } else {
        return -1 ;
    }
}

