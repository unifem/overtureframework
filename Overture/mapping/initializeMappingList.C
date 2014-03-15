//-----------------------------------------------------------------------------------------------
// Put all commonly used Mappings into the static list so that "make" works
//
//-----------------------------------------------------------------------------------------------

#include "MappingInformation.h"

#include "AirfoilMapping.h"
#include "AnnulusMapping.h"
#include "BoxMapping.h"
#include "CircleMapping.h"
#include "ComposeMapping.h"
#include "CompositeSurface.h"
#include "CrossSectionMapping.h"
#include "CylinderMapping.h"
#include "DataPointMapping.h"
#include "DepthMapping.h"
#include "FilamentMapping.h"
#include "FilletMapping.h"
#include "EllipticTransform.h"
#include "HyperbolicMapping.h"
#include "IntersectionMapping.h"
#include "JoinMapping.h"
#include "LineMapping.h"
#include "MatrixMapping.h"
#include "MatrixTransform.h"
#include "NormalMapping.h"
#include "NurbsMapping.h"
#include "OrthographicTransform.h"
#include "PlaneMapping.h"
#include "PolynomialMapping.h"
#include "QuadraticMapping.h"
#include "ReparameterizationTransform.h"
#include "ReductionMapping.h"
#include "ReorientMapping.h"
#include "RestrictionMapping.h"
#include "RevolutionMapping.h"
#include "RocketMapping.h"
#include "SmoothedPolygon.h"
#include "SphereMapping.h"
#include "SplineMapping.h"
#include "SquareMapping.h"
#include "StretchMapping.h"
#include "StretchedSquare.h"
#include "StretchTransform.h"
#include "SweepMapping.h"
#include "TrimmedMapping.h"
#include "TFIMapping.h"
#include "UnstructuredMapping.h"

#include "UserDefinedMapping1.h"

// ******* keep a reference count, only delete when ...
static int initializeMappingListReferenceCount=0;

Mapping* Mapping::
makeMapping( const aString & className )
{
  Mapping *map=NULL;
  if(      className=="AirfoilMapping" )              map = new AirfoilMapping(); 
  else if( className=="AnnulusMapping" )              map = new AnnulusMapping();
  else if( className=="BoxMapping" )                  map = new BoxMapping();
  else if( className=="CircleMapping" )               map = new CircleMapping();
  else if( className=="CompositeSurface" )            map = new CompositeSurface();
  else if( className=="ComposeMapping" )              map = new ComposeMapping();
  else if( className=="CrossSectionMapping" )         map = new CrossSectionMapping();
  else if( className=="CylinderMapping" )             map = new CylinderMapping();
  else if( className=="DataPointMapping" )            map = new DataPointMapping();
  else if( className=="DepthMapping" )                map = new DepthMapping();
  else if( className=="FilamentMapping" )             map = new FilamentMapping();
  else if( className=="FilletMapping" )               map = new FilletMapping();
  else if( className=="EllipticTransform" )           map = new EllipticTransform();
  else if( className=="HyperbolicMapping" )           map = new HyperbolicMapping();
  else if( className=="IntersectionMapping" )         map = new IntersectionMapping();
  else if( className=="JoinMapping" )                 map = new JoinMapping();
  else if( className=="LineMapping" )                 map = new LineMapping();
  else if( className=="MatrixMapping" )               map = new MatrixMapping();
  else if( className=="MatrixTransform" )             map = new MatrixTransform();
  else if( className=="NormalMapping" )               map = new NormalMapping();
  else if( className=="NurbsMapping" )                map = new NurbsMapping();
  else if( className=="OrthographicTransform" )       map = new OrthographicTransform();
  else if( className=="PlaneMapping" )                map = new PlaneMapping();
  else if( className=="PolynomialMapping" )           map = new PolynomialMapping();
  else if( className=="QuadraticMapping" )            map = new QuadraticMapping();
  else if( className=="ReorientMapping" )             map = new ReorientMapping();
  else if( className=="ReductionMapping" )            map = new ReductionMapping();
  else if( className=="ReparameterizationTransform" ) map = new ReparameterizationTransform();
  else if( className=="RestrictionMapping" )          map = new RestrictionMapping();
  else if( className=="RevolutionMapping" )           map = new RevolutionMapping();
  else if( className=="RocketMapping" )               map = new RocketMapping();
  else if( className=="SmoothedPolygon" )             map = new SmoothedPolygon();
  else if( className=="SphereMapping" )               map = new SphereMapping();
  else if( className=="SplineMapping" )               map = new SplineMapping();
  else if( className=="SquareMapping" )               map = new SquareMapping();
  else if( className=="StretchMapping" )              map = new StretchMapping();
  else if( className=="StretchedSquare" )             map = new StretchedSquare();
  else if( className=="StretchTransform" )            map = new StretchTransform();
  else if( className=="SweepMapping" )                map = new SweepMapping();
  else if( className=="TFIMapping" )                  map = new TFIMapping();
  else if( className=="TrimmedMapping" )              map = new TrimmedMapping();
  else if( className=="UnstructuredMapping" )         map = new UnstructuredMapping();
  else if( className=="UserDefinedMapping1" )         map = new UserDefinedMapping1();
  else
  {
    for( MappingItem *ptr=Mapping::staticMapList().start; ptr; ptr=ptr->next )
      if( map = ptr->val->make( className ) ) break;
  }

//  if( map!=NULL )                     // add this ** but then remove from all files!
//    map->incrementReferenceCount(); 
    
  return map;
}



int
addToMappingList(Mapping & map)
//=====================================================================================
// /Description:
//   Add a mapping to the list that is searched when Mapping's are read from a database.
//   Use this routine to add a Mapping that is not already in the list. For example,
//   add a Mapping that you have made.
// /map (input): a Mapping of a given class, the Mapping is expected to
//               exist after the call to this routine. Only a pointer to the
//               Mapping is saved.
//=====================================================================================
{
  Mapping::staticMapList().add(&map);
  return 0;
}

int
initializeMappingList()
{

  initializeMappingListReferenceCount++;
  if( initializeMappingListReferenceCount > 1  )
    return 0;
   
  return 0;
}

int
destructMappingList()
{

  initializeMappingListReferenceCount--;
  if( Mapping::debug & 2 )
    printf("destructMappingList: initializeMappingListReferenceCount=%i\n",initializeMappingListReferenceCount);

  if( initializeMappingListReferenceCount > 0 )
    return 0;
  else if( initializeMappingListReferenceCount < 0 )
  {
    cout << "destructMappingList:ERROR: destructMappingList has been called more times that"
      " initializeMappingList! My reference count is < 0 \n";
    return 1;
  }

  if( Mapping::debug & 2 )
    printf("destructMappingList: delete mappings\n");
  
  for( MappingItem *ptr=Mapping::staticMapList().start; ptr; ptr=ptr->next )
    delete ptr->val;

  
  return 0;
}
