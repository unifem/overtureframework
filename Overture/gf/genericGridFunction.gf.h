#ifndef GENERIC_GRID_FUNCTION_H
#define GENERIC_GRID_FUNCTION_H "GenericGridFunction.h"

#include "ReferenceCounting.h"
#include "OvertureTypes.h"

class GenericGrid;  // forward declaration

//---------------------------------------------------------------------------
//  This is a generic grid function from which other types of grid functions
//  can be derived. 
//---------------------------------------------------------------------------
class GenericGridFunction : public ReferenceCounting
{
 public:
  GenericGrid *grid;

  GenericGridFunction(){ grid=NULL; }
  GenericGridFunction( const GenericGridFunction & ,
                             const CopyType=DEEP ) { }
  ~GenericGridFunction (){}

  GenericGridFunction ( GenericGrid *grid0 ){ grid=grid0; }

  GenericGridFunction & operator= ( const GenericGridFunction & X )
    { grid = X.grid; return *this; }
  void reference( const GenericGridFunction & ){}
  virtual void breakReference(){}
 private:
  virtual ReferenceCounting& operator=( const ReferenceCounting & x)
    { return GenericGridFunction::operator=( (GenericGridFunction &) x ); }
  virtual void reference( const ReferenceCounting & x)
    { reference( (GenericGridFunction &) x ); }
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new GenericGridFunction(*this, ct); }
  
};
#endif // GenericGridFunction.h
