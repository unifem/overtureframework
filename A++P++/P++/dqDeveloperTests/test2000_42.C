/*
DATE:               14:54:32 10/03/0
VISITOR:            Brian Miller
EMAIL:              miller125@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           serious
SUMMARY:           
Binary operations on DEEPALIGNEDCOPY's don't work.
ENVIRONMENT:        
Solaris, Compass, IBM.  Recent CVS checkout.
DESCRIPTION:        
Constructing an array as a DEEPALIGNEDCOPY then performing
A binary operation with a conforming array produces unexpected results.
This happens both when copying a view and a non-view (I was wrong
about the non-view case working, sorry.)
HOW TO REPEAT:      
run the test code without PADRE.
TEST CODE:          
*/

#include "A++.h"

// Values provided by Brian
// #define EXHIBIT_ERROR 1
// #define COPY_OF_VIEW 1

#define EXHIBIT_ERROR 1
#define COPY_OF_VIEW 0

int main( int argc, char *argv[])
{
  int theNumberOfProcessors,thisProcessorNumber;

  Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

  MPI_Comm_rank( MPI_COMM_WORLD, &thisProcessorNumber );  
 
  int theJBase=-10,theJSize=14;

  Index theJIndex(theJBase,theJSize);    

  intArray J(theJIndex);
  
  J=1000;  
    
  int theLowerIncrement = 2, theUpperIncrement=1, theStride=3;

  Range theSubRange( J.getBase(0)+theLowerIncrement,
                     J.getBound(0)-theUpperIncrement,theStride);
    
#if COPY_OF_VIEW
  intArray T(J(theSubRange),DEEPALIGNEDCOPY);
#else
  intArray T(J,DEEPALIGNEDCOPY);
#endif
  
  T.setBase(APP_Global_Array_Base);  
  
  T.display("T after Assignment");

  intArray Texact(T.getLength(0));
    
  Texact = 1000;
  
  Texact.display("Texact");

  intArray theError(T.getLength(0));

#if EXHIBIT_ERROR
  theError = Texact - T;
#else
  int ii;
  for(ii=0;ii<theError.getLength(0);++ii)
  {
    int jj = T.getBase(0) + ( ii - theError.getBase(0) );
    theError(ii) = abs( T(jj) - Texact(ii) );
  }
#endif
  
  theError.display("theError");
  int theErrorSum = sum(theError);

  if( thisProcessorNumber == 0 )
  {
    cout<<" ERROR in test02("<<theJBase<<","<<theJSize<<") (should be 0): "<<theErrorSum<<endl;
  }

  Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize(); 

  return 0;
}

