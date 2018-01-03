// Problem report 0-03-09-11-51-09

#include <A++.h>

// Error when stdarg.h is included after A++.h (now fixed)
#include <stdarg.h>

int
main( int argc, char *argv[])
   {
#if 1
     Index::setBoundsCheck(on);
#endif

     int Number_Of_Processors = 0;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     intArray A(10);
     A = 1;

     printf ("Program Terminated Normally! \n");

  // MPI_Finalize();
     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }

#if 0
	Bug reported in the inclusion of stdarg.h after A++.h in P++
	application.  Fixed problem by commenting out the definition of
	VA_DCL and VA_START which are not used in PARTI (at least not
	any more).
	
     #if 0
     /* Modified by Dan Quinlan: removed since these are not used */
     /* #ifdef __STDC__ */
     #ifdef STD_COMPLIANT_COMPILER
     #  include <stdarg.h>
     #  define VA_DCL(type,var)              (type var,...)
     #  define VA_START(list,var,type)       ((va_start(list,var)) , (var))
     #else
     #  include <varargs.h>
     #  define VA_DCL(type,var)               (va_alist) va_dcl
     #  define VA_START(list,var,type)        ((va_start(list)), va_arg(list,type))
     #endif
     #endif
#endif
