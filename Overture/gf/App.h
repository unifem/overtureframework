#ifndef APP_UTILITY_INCLUDE_FILE
#define APP_UTILITY_INCLUDE_FILE


// -----
#include "Overture.h"
#include "A++.h"
#include "OGFunction.h"
// -----
void turnOnAutomaticCommunication();

void turnOffAutomaticCommunication();


bool getAutomaticCommunication();


int getSent();

int getReceived();

void printMessageInfo( const char* msg, FILE *file=stdout );

// Put this here for now -- should be moved to OGFunction
int
assignGridFunction( OGFunction & exact, 
		    realMappedGridFunction & u, 
                    const Index &I1, const Index&I2, const Index&I3, const Index & N, const real t);

// Assign values without communication (when lhs and rhs have the same distribution)
void assign( realGridCollectionFunction & u, const realGridCollectionFunction & v );
void assign( realGridCollectionFunction & u, real value);
void assign( intGridCollectionFunction & u, const intGridCollectionFunction & v );
void assign( intGridCollectionFunction & u, int value);

// Assign values without communication (when lhs and rhs have the same distribution)
void assign( realArray & u, const realArray & v );
void assign( intArray & u, const intArray & v );

// Assign values without communication (when lhs and rhs have the same distribution)
void assign( realArray & u, const realArray & v, 
             const Index & I1, const Index & I2, const Index & I3, const Index & I4 );
void assign( intArray & u, const intArray & v, 
             const Index & I1, const Index & I2, const Index & I3, const Index & I4 );

// Assign values without communication (when lhs and rhs have the same distribution)
// u(I1,I2,I3,I4)=v(J1,J2,J3,J4)
void assign( realArray & u, const Index & I1, const Index & I2, const Index & I3, const Index & I4,
             const realArray & v, const Index & J1, const Index & J2, const Index & J3, const Index & J4 );
void assign( intArray & u, const Index & I1, const Index & I2, const Index & I3, const Index & I4,
             const intArray & v, const Index & J1, const Index & J2, const Index & J3, const Index & J4 );

// Assign values without communication (when lhs and rhs have the same distribution)
void assign( realArray & u, real value, 
             const Index & I1, const Index & I2, const Index & I3, const Index & I4 );
void assign( intArray & u, int value, 
             const Index & I1, const Index & I2, const Index & I3, const Index & I4 );
void assign( realArray & u, real value );
void assign( intArray & u, int value );


// return the base of the local array that includes ghost points
inline int getFullBase( const doubleSerialArray & u, int axis)
        {  return u.getFullRange(axis).getBase();  }

// return the bound of the local array that includes ghost points
inline int getFullBound( const doubleSerialArray & u, int axis)
        {  return u.getFullRange(axis).getBound();  }


void checkArrayIDs(const aString & label, bool printNumber = false );

real getSignForJacobian( MappedGrid & mg );

// return true if the two arrays have the same parallel distribution:
bool hasSameDistribution(const intArray & u, const intArray & v );
bool hasSameDistribution(const floatArray & u, const floatArray & v );
bool hasSameDistribution(const doubleArray & u, const doubleArray & v );

// return true if the array u has a consistent distribution with partition: 
bool hasSameDistribution(const intArray & u, const Partitioning_Type & partition );
bool hasSameDistribution(const floatArray & u, const Partitioning_Type & partition );
bool hasSameDistribution(const doubleArray & u, const Partitioning_Type & partition );

bool hasSameDistribution(const Partitioning_Type & uPartition, const Partitioning_Type & vPartition );

int testConsistency( const intArray & u, const char* label );
int testConsistency( const floatArray & u, const char* label );
int testConsistency( const doubleArray & u, const char* label );

#endif
