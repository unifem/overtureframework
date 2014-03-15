#include "doubleGenericGridCollectionFunction.h"
#include "GenericGridCollection.h"

doubleGenericGridCollectionFunction::doubleGenericGridCollectionFunction( 
     GenericGridCollection *gridList0 )
{
  gridList=gridList0;
    
  for( int i=0; i<gridList->grid.getLength(); i++ )
  {
    doubleGenericGridFunction ggf( &(*gridList)[i] );
    genericGridFunctionList.addElement( ggf );
  }
}

doubleGenericGridCollectionFunction & doubleGenericGridCollectionFunction::operator= 
  ( const doubleGenericGridCollectionFunction & X )
{
  gridList=X.gridList;
  genericGridFunctionList=X.genericGridFunctionList;    
  return *this;
}  

doubleGenericGridFunction & doubleGenericGridCollectionFunction::operator[]( const int grid )
{
  return genericGridFunctionList[grid];
}

