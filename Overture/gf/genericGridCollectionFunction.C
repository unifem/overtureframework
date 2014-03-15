#include "GenericGridCollectionFunction.h"
#include "GenericGridCollection.h"

GenericGridCollectionFunction::GenericGridCollectionFunction( 
     GenericGridCollection *gridList0 )
{
  gridList=gridList0;
    
  for( int i=0; i<gridList->grid.getLength(); i++ )
  {
    GenericGridFunction ggf( &(*gridList)[i] );
    genericGridFunctionList.addElement( ggf );
  }
}

GenericGridCollectionFunction & GenericGridCollectionFunction::operator= 
  ( const GenericGridCollectionFunction & X )
{
  gridList=X.gridList;
  genericGridFunctionList=X.genericGridFunctionList;    
  return *this;
}  

GenericGridFunction & GenericGridCollectionFunction::operator[]( const int grid )
{
  return genericGridFunctionList[grid];
}

