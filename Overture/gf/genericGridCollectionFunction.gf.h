#ifndef GENERIC_GRID_COLLECTION_FUNCTION
#define GENERIC_GRID_COLLECTION_FUNCTION "GenericGridCollectionFunction.h"

#include "GenericGridFunction.h" 
#include "ReferenceCounting.h"
#include "ListOfGenericGridFunction.h"

class GenericGridCollection;  // forward declaration

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class GenericGridCollectionFunction : public ReferenceCounting
{
 public:
  
  ListOfGenericGridFunction genericGridFunctionList;
  GenericGridCollection *gridList;

  GenericGridCollectionFunction(){}
  GenericGridCollectionFunction(const GenericGridCollectionFunction& ,
                                      const CopyType =DEEP){}
  virtual ~GenericGridCollectionFunction(){}
  GenericGridCollectionFunction( GenericGridCollection *gridList );
  GenericGridCollectionFunction & operator= ( const GenericGridCollectionFunction & X );
  GenericGridFunction & operator[]( const int grid );
  void reference( const GenericGridCollectionFunction & ){};
  virtual void breakReference(){};
 private:
  virtual ReferenceCounting& operator=( const ReferenceCounting & x)
    { return operator=( (GenericGridCollectionFunction &) x ); }
  virtual void reference( const ReferenceCounting & x)
    { reference( (GenericGridCollectionFunction &) x ); }
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new GenericGridCollectionFunction(*this, ct); }
};  



#endif 
