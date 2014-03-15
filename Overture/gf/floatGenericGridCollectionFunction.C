#include "floatGenericGridCollectionFunction.h"
#include "GenericGridCollection.h"

floatGenericGridCollectionFunction::floatGenericGridCollectionFunction( 
     GenericGridCollection *gridList0 )
{
  gridList=gridList0;
    
  for( int i=0; i<gridList->grid.getLength(); i++ )
  {
    floatGenericGridFunction ggf( &(*gridList)[i] );
    genericGridFunctionList.addElement( ggf );
  }
}

floatGenericGridCollectionFunction & floatGenericGridCollectionFunction::operator= 
  ( const floatGenericGridCollectionFunction & X )
{
  gridList=X.gridList;
  genericGridFunctionList=X.genericGridFunctionList;    
  return *this;
}  

floatGenericGridFunction & floatGenericGridCollectionFunction::operator[]( const int grid )
{
  return genericGridFunctionList[grid];
}

