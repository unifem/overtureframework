
/* void print_out ( int* Descriptor ); */

/* GNU will build intances of all objects in the header file if this
// is not specified.  The result is very large object files (too many symbols)
// so we can significantly reduce the size of the object files which will
// build the library (factor of 5-10).
*/
#ifdef __GNUC__
#pragma implementation "constants.h"
#pragma implementation "machine.h"
#endif


#define MDI_MAIN_FUNCTION
#include "constants.h"
#include <math.h>
#include <limits.h>
#include "machine.h"

extern int APP_DEBUG;

/* This makes sure it is expanded only once! */
void print_out ( int* Descriptor )
   {
     int i;
     printf ("\n");
     printf ("Print Out Descriptor -- \n");
     for (i=0; i <= 18; i++)
        {
          printf (" %d ",Descriptor[i]);
        }
     printf ("\n");
     printf ("\n");
   }


double asinh(double x);
double acosh(double x);
double atanh(double x);

#ifdef CRAY
/* These functions are not available on the crays! */
/* These could be made into macros to allow for vectorication! */

double asinh(double x)
   {
     return ( log( x + sqrt( x*x+1.0 ) ) );
   }

double acosh(double x)
   {
     return ( log( x + sqrt( x*x-1.0 ) ) );
   }

double atanh(double x)
   {
     return ( 0.5 * log( (1+x) / (1-x) ) );
   }
#endif

static int* Static_Map = NULL;

int*
MDI_Get_Default_Index_Map_Data (int Local_Min_Base, int Local_Max_Bound)
   {
  /* This function returns an integer map that can be used to replace
     the constant stride index information that is not readily implemented
     in the MDI loops for indirect addressing.  In the multithreaded
     environment this array represents a shared resource.
  */

  /* The Minimum_Base and Maximum_Bound determine the size of the 
     Static_Map and the Index_Size tells us what the dimension lengths 
     must be (in order to be conformable with the other Index arrays 
     in the case where they are multi dimensional).  The Minimum_Base 
     also tells us where in the Static_Map we are to posision the 
     pointer.  The Stride must be set to that of the arrays view which 
     we are indexing -- but is constant in each dimension of the 
     multidimensional part of the index array. Opps, it is only non 
     unitary in the first dimension (if stride > 1) 
  */

     static int Minimum_Base  = INT_MAX;
     static int Maximum_Bound = INT_MIN;
     static int Map_Length    = 0;
     int i;

  /* allocate memory for Static_Map if necessary */

  /* printf ("Local_Min_Base = %d Local_Max_Bound = %d \n",Local_Min_Base,Local_Max_Bound); */

     if (Local_Min_Base  < Minimum_Base )
          Minimum_Base  = Local_Min_Base;
     if (Local_Max_Bound > Maximum_Bound)
          Maximum_Bound = Local_Max_Bound;

     MDI_ASSERT (Maximum_Bound >= Minimum_Base);

  /* printf ("Minimum_Base = %d  Maximum_Bound = %d \n",Minimum_Base,Maximum_Bound); */

     if (Maximum_Bound - Minimum_Base >= Map_Length)
        {
          Map_Length = (Maximum_Bound - Minimum_Base) + 1;

       /* printf ("Map_Length = %d \n",Map_Length); */

          if (Static_Map == NULL)
             {
	    /* printf ("Allocating a new indirection array \n"); */
               Static_Map = (int*) malloc ( Map_Length * sizeof(int*) );
             }
            else
             {
            /* printf ("REALLOCATING an indirection array \n"); */
               Static_Map = (int*) realloc ( (char*) Static_Map , Map_Length * sizeof(int*) );
             }

          if (Static_Map == NULL)
             {
               printf ("ERROR: In MDI_Get_Default_Index_Map_Data -- Static_Map == NULL \n");
               exit(1);
             }

       /* Make sure this vectorizes if possible! */
          for (i=Minimum_Base; i <= Maximum_Bound; i++)
             {
               Static_Map [i-Minimum_Base] = i;
            /* printf ("Static_Map [i-Minimum_Base] = %d \n",Static_Map [i-Minimum_Base]); */
             }
        }

     MDI_ASSERT(&(Static_Map [-Minimum_Base]) != NULL);

     return &(Static_Map [-Minimum_Base]);
   }

void
cleanup_after_MDI()
   {
     printf("Cleaning up MDI arrays\n");
     if (Static_Map != NULL) 
        {
          free (Static_Map);
          Static_Map = NULL;
        }
   }

/* MDI Support for error handling */
void
MDI_ABORT()
   {
     abort();
   }

/* MDI Support for assertion handling */
void
MDI_Assertion_Support ( char* Source_File_With_Error, unsigned Line_Number_In_File )
   {
     fflush(stdout);
     fprintf(stderr, "\n\nMDI Assertion failed: %s, line %u \n",Source_File_With_Error,Line_Number_In_File);
     fflush(stderr);
     MDI_ABORT();
   }








