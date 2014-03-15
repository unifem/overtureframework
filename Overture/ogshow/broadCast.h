#ifndef BROADCAST_H
#define BROADCAST_H

#include "OvertureTypes.h"
#include "aString.H"

#ifndef USE_PPP
  #define MPI_COMM_WORLD 0
#endif

// broadcast a value to all processors in a communicator
void broadCast( int & value, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );
void broadCast( float & value, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );
void broadCast( double & value, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );
void broadCast( bool & value, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );

// broadcast a serial array to all processors in a communicator
void broadCast( intSerialArray & buff, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );
void broadCast( floatSerialArray & buff, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );
void broadCast( doubleSerialArray & buff, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );

// broadcast a aString to all processors in a communicator
void broadCast( aString & string, const int & fromProcessor=0, MPI_Comm comm=MPI_COMM_WORLD );

#endif
