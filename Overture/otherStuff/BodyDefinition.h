#ifndef BODY_DEFINITION_H
#define BODY_DEFINITION_H "BodyDefinition.h"

// =======================================================================================
//    Use this class to define the relationship between a body and a grid.
// =======================================================================================

#include "Overture.h"

class BodyDefinition 
{
 public:

  BodyDefinition();
  BodyDefinition( const BodyDefinition & bd );
  
  ~BodyDefinition();
  
  BodyDefinition & operator =( const BodyDefinition & x0 );

  int defineSurface( const int & surfaceNumber, const int & numberOfFaces_, IntegerArray & boundary ); 

  // get from a database file:
  int get( const GenericDataBase & dir, const aString & name); 

  int getFace(int surfaceNumber,int face, int & side, int & axis, int & grid) const;

  int getSurface(  const int & surfaceNumber, int & numberOfFaces_, IntegerArray & boundary ) const;

  int getSurfaceNumber( const int surface ) const;

  int numberOfFacesOnASurface(int surfaceNumber) const;

  // put to a database file:
  int put( GenericDataBase & dir, const aString & name) const;    

  int totalNumberOfSurfaces() const;

 protected:

  int initialize();
  int surfaceIndex( int surfaceNumber ) const;

  aString className;         // Name of the Class

  int numberOfSurfaces,maximumNumberOfFaces;
  IntegerArray surfaceIdentifier, numberOfFaces;
  IntegerArray boundaryFaces;
  
  friend class Integrate;  // fix this
  friend class CutCells;
};

#endif
