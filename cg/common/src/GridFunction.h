#ifndef GRID_FUNCTION_H
#define GRID_FUNCTION_H

#include "Overture.h"
#include "Parameters.h"

class MatrixTransform;

class GridFunction
{
 public:
  enum Forms
  {
    primitiveVariables=0,
    conservativeVariables
  };

  GridFunction(Parameters *pParameters=NULL);
  ~GridFunction();
  
  int updateToMatchGrid(CompositeGrid & cg);
 
  void updateGridVelocityArrays();  // update grid velocity arrays if the number of grids has changed.

//  OB_MappedGridFunction & operator [](const int & grid) const; // reference to a mapped grid function
  
  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  int primitiveToConservative(int gridToConvert=-1, int fixupUnsedPoints=false);
  int conservativeToPrimitive(int gridToConvert=-1, int fixupUnsedPoints=false);

  // build the gridVelocity on a specified component grid.
  realMappedGridFunction & createGridVelocity(int grid );

  // return the gridVelocity (if it is there, otherwise return a null grid function)
  realMappedGridFunction & getGridVelocity(int grid);

  int referenceGridVelocity(GridFunction & gf);

  realCompositeGridFunction & getGridVelocity(); // ***** temporary for conversion ****

  void setParameters(Parameters & parameters);

  /// the grid function is either in primitive or conservative variables.
  Forms form;                  

  /// time for grid function.
  real t;  

  /// time at which the grid velocity has been computed.
  real gridVelocityTime;  

  /// solution values.
  realCompositeGridFunction u;              

  /// number of entries in the gridVelocity array.
  int sizeOfGridVelocityArray;

  /// grid velocity for moving grid problems.
  realMappedGridFunction **gridVelocity;  // grid velocity for moving grid problems

  /// reference to the grid associated with u
  CompositeGrid cg;                         // reference to the grid associated with u

  /// array of pointers to transformations for movement.
  MatrixTransform **transform;  

  /// number of entries in the above array.
  int numberOfTransformMappings; 

  /// pointer to an associated Parameters object.
  Parameters *pParams;

  mutable DataBase dbase;  // save additional parameters here

  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

};


#endif
