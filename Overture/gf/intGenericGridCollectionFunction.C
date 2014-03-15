#include "intGenericGridCollectionFunction.h"
#include "GenericGridCollection.h"

intGenericGridCollectionFunction::intGenericGridCollectionFunction( 
     GenericGridCollection *gridList0 )
{
  gridList=gridList0;
    
  for( int i=0; i<gridList->grid.getLength(); i++ )
  {
    intGenericGridFunction ggf( &(*gridList)[i] );
    genericGridFunctionList.addElement( ggf );
  }
}

intGenericGridCollectionFunction & intGenericGridCollectionFunction::operator= 
  ( const intGenericGridCollectionFunction & X )
{
  gridList=X.gridList;
  genericGridFunctionList=X.genericGridFunctionList;    
  return *this;
}  

intGenericGridFunction & intGenericGridCollectionFunction::operator[]( const int grid )
{
  return genericGridFunctionList[grid];
}

