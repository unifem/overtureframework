#ifndef PARALLEL_UTILITY_H
#define PARALLEL_UTILITY_H

#include "GenericDataBase.h"
#include "wdhdefs.h"           // some useful defines and constants
#include "mathutil.h"          // define max, min,  etc
#include "OvertureInit.h"

#include "broadCast.h"
#include "display.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
#include <vector>
#else
#include <list.h>
#include <vector>
#endif

doubleSerialArray& getLocalArrayWithGhostBoundaries( const doubleArray & u, doubleSerialArray & uLocal );
floatSerialArray& getLocalArrayWithGhostBoundaries( const floatArray & u, floatSerialArray & uLocal );
intSerialArray&    getLocalArrayWithGhostBoundaries( const intArray & u, intSerialArray & uLocal );


class ParallelUtility
{
  public:

  // broadcast argc, argv to all processors in a communicator
  static int broadCastArgs(int & argc, char **&argv, int sourceProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );
  static int broadCastArgsCleanup(int & argc, char **&argv, int sourceProcessor=0 );

  //  get max over all processors in a communicator
  static real getMaxValue(real value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);  
  static int getMaxValue(int value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);
  static doubleLengthInt getMaxValue(doubleLengthInt value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);

  // get min over all processors in a communicator
  static real getMinValue(real value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);  
  static int getMinValue(int value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);
  static doubleLengthInt getMinValue(doubleLengthInt value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);

  // get sum over all processors in a communicator 
  static real getSum(real value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);  
  static int getSum(int value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD); 
  static doubleLengthInt getSum(doubleLengthInt value, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD); 

  // get max of all array components over all processors in a communicator
  static void getMaxValues(real *value, real *maxValue, int n, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);  
  static void getMaxValues(int *value, int *maxValue, int n, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);
  // This next one conflicts with the one below
  // static void getMaxValues( const std::vector<real>& value, std::vector<real>& maxValue, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD ); 

  static void getMaxValues( const std::vector<real>& value, std::vector<real>& maxValue, MPI_Comm& comm ); 

  // get min of all array components over all processors in a communicator
  static void getMinValues(real *value, real *minValue, int n, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD); 
  static void getMinValues(int *value, int *minvalue, int n, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);

  // get sum of all array components over all processors in a communicator
  static void getSums(real *value, real *sum, int n, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);  
  static void getSums(int *value, int *sum, int n, int processor=-1, MPI_Comm comm=MPI_COMM_WORLD);  

  //  Make the assignment dest(D[0],...,D[nd-1]) = src(S[0],...S[nd-1])
  static int copy( intArray & dest, Index *D, 
	   	   const intArray & src, Index *S, int nd );

  static int copy( floatArray & dest, Index *D, 
	   	   const floatArray & src, Index *S, int nd );

  static int copy( doubleArray & dest, Index *D, 
	   	   const doubleArray & src, Index *S, int nd );


  static int getArgsFromFile(const aString & fileName, int & argc, char **&argv );

  static int deleteArgsFromFile(int & argc, char **&argv );

  static bool getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
				Index & I1, Index & I2, Index & I3, 
				int & n1a, int & n1b, int & n2a, int & n2b, int & n3a, int & n3b,
				int option = 0 );

  static bool getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
				Index & I1, Index & I2, Index & I3, 
				int option = 0 );

  static bool getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
				  Index & I1, Index & I2, Index & I3, Index &I4,
				  int & n1a, int & n1b, int & n2a, int & n2b, int & n3a, int & n3b, int & n4a, int & n4b,
				  int option = 0 );

  static bool getLocalArrayBounds(const realArray & u, const realSerialArray & uLocal,
				  Index & I1, Index & I2, Index & I3, Index & I4,
				  int option = 0 );

  static bool getLocalArrayBounds(const intArray & u, const intSerialArray & uLocal,
				Index & I1, Index & I2, Index & I3, 
				int & n1a, int & n1b, int & n2a, int & n2b, int & n3a, int & n3b,
				int option = 0 );

  static bool getLocalArrayBounds(const intArray & u, const intSerialArray & uLocal,
				Index & I1, Index & I2, Index & I3, 
				int option = 0 );

  // Build v, a copy of the array u that lives on the processors defined by the Range P
  static int redistribute(const intArray & u, intArray & v, const Range & P);
  static int redistribute(const floatArray & u, floatArray & v, const Range & P);
  static int redistribute(const doubleArray & u, doubleArray & v, const Range & P);

  // Build v, a copy of the array u that lives on the local processor
  static int redistribute( const intArray & u, intSerialArray & v );
  static int redistribute( const floatArray & u, floatSerialArray & v );
  static int redistribute( const doubleArray & u, doubleSerialArray & v );

  // added by JAFH
  static void Sync( MPI_Comm comm = MPI_COMM_WORLD );

};



const int MAX_DISTRIBUTED_DIMENSIONS=4;

class IndexBox
{
public:
IndexBox();
IndexBox(int i1a, int i1b, int i2a=0, int i2b=0, int i3a=0, int i3b=0, int i4a=0, int i4b=0);
~IndexBox();

inline int base(int d) const {return ab[d][0];} //
inline int bound(int d) const {return ab[d][1];} //
// return the total size: (number of elements in the array)
int size() const;

inline int bb(int side, int d) const {return ab[d][side];} //

void setBounds(int i1a, int i1b, int i2a=0, int i2b=0, int i3a=0, int i3b=0, int i4a=0, int i4b=0);

static bool intersect(const IndexBox & a, const IndexBox & b, IndexBox & c);

bool isEmpty() const;

int processor;  // processor number associated with this IndexBox

protected:
  int ab[MAX_DISTRIBUTED_DIMENSIONS][2];  // base and bounds

};

typedef std::vector<IndexBox> ListOfIndexBox;

class CopyArray
{
public:

// copy functions to copy distributed array to local arrays (generalized P++ copy)
static int copyArray( const floatDistributedArray & u,
		      Index *Iv, 
		      IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
		      floatSerialArray & vLocal );
static int copyArray( const doubleDistributedArray & u,
		      Index *Iv, 
		      IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
		      doubleSerialArray & vLocal );
static int copyArray( const intDistributedArray & u,
		      Index *Iv, 
		      IndexBox *vBox, // bounds of v on each processor, vBox[p] p=0,1,..,numProc-1
		      intSerialArray & vLocal );

// copy functions to replace P++ distributed to distributed copies
static int copyArray( floatArray & dest, Index *D, 
	              const floatArray &  src, Index *S, int nd=4 );
static int copyArray( doubleArray & dest, Index *D, 
	              const doubleArray &  src, Index *S, int nd=4 );
static int copyArray( intArray & dest, Index *D, 
	              const intArray &  src, Index *S, int nd=4 );


// copy serial arrays uLocal into a distributed array, v (generalized P++ copy)
static int copyArray( const floatSerialArray & uLocal,
		      const Index *Jv, 
		      const intSerialArray & uProcessorSet,
		      floatDistributedArray & v, 
                      const Index *Iv );
static int copyArray( const doubleSerialArray & uLocal,
		      const Index *Jv, 
		      const intSerialArray & uProcessorSet,
		      doubleDistributedArray & v, 
                      const Index *Iv );
static int copyArray( const intSerialArray & uLocal,
		      const Index *Jv, 
		      const intSerialArray & uProcessorSet,
		      intDistributedArray & v, 
                      const Index *Iv );

// copy a serial array from one processor to another: 
static int copyArray( floatSerialArray  & dest, int destProcessor, floatSerialArray  & src, int srcProcessor );
static int copyArray( doubleSerialArray & dest, int destProcessor, doubleSerialArray & src, int srcProcessor );
static int copyArray( intSerialArray    & dest, int destProcessor, intSerialArray    & src, int srcProcessor );


static void getLocalArrayInterval(const floatDistributedArray & u, int p, int *pv);
static void getLocalArrayInterval(const doubleDistributedArray & u, int p, int *pv);
static void getLocalArrayInterval(const intDistributedArray & u, int p, int *pv);

// get local array bounds (no ghost points):
static bool getLocalArrayBox( int p, const floatDistributedArray & u, IndexBox & uBox );
static bool getLocalArrayBox( int p, const doubleDistributedArray & u, IndexBox & uBox );
static bool getLocalArrayBox( int p, const intDistributedArray & u, IndexBox & uBox );

// get local array bounds including ghost points:
static bool getLocalArrayBoxWithGhost( int p, const floatDistributedArray & u, IndexBox & uBox );
static bool getLocalArrayBoxWithGhost( int p, const doubleDistributedArray & u, IndexBox & uBox );
static bool getLocalArrayBoxWithGhost( int p, const intDistributedArray & u, IndexBox & uBox );


static void copyCoarseToFine( const floatArray & uc, const floatArray & uf, Index *Iv, 
			      floatSerialArray & uc2, int *ratio, int *ghost);

static void copyCoarseToFine( const doubleArray & uc, const doubleArray & uf, Index *Iv, 
			      doubleSerialArray & uc2, int *ratio, int *ghost);

static void copyCoarseToFine( const intArray & uc, const intArray & uf, Index *Iv, 
			      intSerialArray & uc2, int *ratio, int *ghost);


static void getAggregateArray( floatSerialArray & u, Index *Iv, floatSerialArray & u0, int p0);
static void getAggregateArray( doubleSerialArray & u, Index *Iv, doubleSerialArray & u0, int p0);
static void getAggregateArray( intSerialArray & u, Index *Iv, intSerialArray & u0, int p0);

static int debug;

};

#endif
