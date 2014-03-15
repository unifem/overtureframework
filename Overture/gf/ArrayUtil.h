#ifndef ARRAY_UTIL_INCLUDE_FILE
#define ARRAY_UTIL_INCLUDE_FILE

// -----
#include "Overture.h"
#include "A++.h"
#include "OGFunction.h"
// -----

class ArrayUtil
{
public:

static void turnOnAutomaticCommunication();

static void turnOffAutomaticCommunication();


static bool getAutomaticCommunication();


static int getSent();

static int getReceived();

static void printMessageInfo( const char* msg, FILE *file=stdout );

// Put this here for now -- should be moved to OGFunction
static int
assignGridFunction( OGFunction & exact, 
		    realMappedGridFunction & u, 
                    const Index &I1, const Index&I2, const Index&I3, const Index & N, const real t);

static void assign( realGridCollectionFunction & u, const realGridCollectionFunction & v );

static void assign( realGridCollectionFunction & u, real value );

static void assign( realArray & u, const realArray & v );

static void assign( realArray & u, const realArray & v, 
        const Index & I1, const Index & I2, const Index & I3, const Index & I4 );

// u(I1,I2,I3,I4)=v(J1,J2,J3,J4)
static void assign( realArray & u, const Index & I1, const Index & I2, const Index & I3, const Index & I4,
             const realArray & v, const Index & J1, const Index & J2, const Index & J3, const Index & J4 );

static void assign( realArray & u, real value, 
        const Index & I1=nullIndex, const Index & I2=nullIndex, 
        const Index & I3=nullIndex, const Index & I4=nullIndex );

// return the base of the local array that includes ghost points
static inline int getFullBase( const doubleSerialArray & u, int axis)
        {  return u.getFullRange(axis).getBase();  }

// return the bound of the local array that includes ghost points
static inline int getFullBound( const doubleSerialArray & u, int axis)
        {  return u.getFullRange(axis).getBound();  }


static void checkArrayIDs(const aString & label, bool printNumber = false );

// real getSignForJacobian( MappedGrid & mg );
};


#endif
