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

#if 0
  // A.view("A");
  // A.reshape(1,1,1,10);

     intArray xx(Range(0,1),Range(0,1),Range(0,1),Range(2,3));
     Partitioning_Type* partition = new Partitioning_Type();
     int ghost_boundary_width = 0;
     partition->partitionAlongAxis(0,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(1,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(2,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(3,TRUE,1);
     xx.partition(*partition);
  // xx.view("xx");

#if 0
  // Error checking!
     if ( Communication_Manager::Number_Of_Processors == 1 )
        {
          for (int i=0; i < MAX_ARRAY_DIMENSION; i++)
             {
            // printf ("Offset_For_Ghost_Boundary_Width = %d \n",Offset_For_Ghost_Boundary_Width);
               printf ("getLength(%d) = %d  != Array_Descriptor.SerialArray->getLength(i) = %d \n",
                    i,xx.getLength(i),xx.Array_Descriptor.SerialArray->getLength(i));
               printf ("getInternalGhostCellWidth(%d) = %d \n",i,xx.getInternalGhostCellWidth(i));
             }
        }
#endif

#if 1
     xx=3.;
     xx.reshape(Range(0,3),Range(0,0),Range(0,1),Range(2,3));
  // xx.view("xx");
#endif
#endif

  // A.view("A");

  // Build a NULL array object (added to this simple test: (8/7/2000))
     intArray B(0);

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");

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
