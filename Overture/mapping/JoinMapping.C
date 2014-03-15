#include "JoinMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "LineMapping.h"
#include "TFIMapping.h"
#include "ComposeMapping.h"
#include "ReductionMapping.h"
#include "ReparameterizationTransform.h"
#include <float.h>

JoinMapping::
JoinMapping() : Mapping(2,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  
///   Define a mapping that tranforms a "source-mapping" so that
///  it intersects another "clip-surface" "exactly". For example,
///  a Mapping for a wing (source-mapping)  can be joined to a fuselage (clip-surface).
/// 
//===========================================================================
{ 
  JoinMapping::className="JoinMapping";
  setName( Mapping::mappingName,"JoinMapping");
  setGridDimensions( axis1,15 );
  joinType=nonParametric;
  intersectionFound=FALSE;
  
  intersectionToUse=0;   // use this intersection point/curve
  newCurves=TRUE;
  endOfJoin=1.;             // determines r value of the end of the joint

  curve[0]=NULL;
  curve[1]=NULL;

  line[0]=line[1]=NULL;
  tfi[0]=tfi[1]=tfi[2]=NULL;
  join=NULL;
  join1=NULL;
  join2=NULL;
  join3=NULL;
  surface1=NULL;
  surface2=NULL;
  surface3=NULL;
  
  uninitialized=TRUE;
  mappingHasChanged();
}


JoinMapping::
JoinMapping(Mapping & sourceMapping, 
	    Mapping & clipSurface)
  : Mapping(1,2,parameterSpace,cartesianSpace) 
//===========================================================================
/// \details  
///   Define a mapping that tranforms a "source-mapping" so that
///  it intersects another "clip-surface" "exactly". For example,
///  a Mapping for a wing (source-mapping)  can be joined to a fuselage (clip-surface).
///  
/// \param sourceMapping (input): defines the source-mapping. This is the Mapping
///   that will be changed. 
/// \param clipMapping : defines the clip-surface. This Mapping will clip away a
///     portion of the sourceMapping. Use  the setEndOfJoin function to specify
///     which portion of the sourceMapping to retain. 
//===========================================================================
{
  JoinMapping::className="JoinMapping";
  setName( Mapping::mappingName,"JoinMapping");

  curve[0]=&sourceMapping;
  curve[0]->uncountedReferencesMayExist();
  curve[0]->incrementReferenceCount();
  
  curve[1]=&clipSurface;
  curve[1]->uncountedReferencesMayExist();
  curve[1]->incrementReferenceCount();

  joinType=nonParametric;
  intersectionFound=FALSE;
  
  intersectionToUse=0;   // use this intersection point/curve
  newCurves=TRUE;
  endOfJoin=1.;
  
  line[0]=line[1]=NULL;
  tfi[0]=tfi[1]=tfi[2]=NULL;
  join=NULL;
  join1=NULL;
  join2=NULL;
  join3=NULL;
  surface1=NULL;
  surface2=NULL;
  surface3=NULL;

  setup();
}

void JoinMapping::
setup()
{
  for( int i=0; i<=1; i++ )
  {
    Mapping & c = *(curve[i]);
    if( c.getRangeDimension() - c.getDomainDimension() != 1 )
    {
      cout << "JoinMapping::ERROR: c" << i+1 << " is not a c in 2D or surface in 3D \n";
      cout << " c" << i+1 << " : domainDimension = " << c.getDomainDimension()
	   << ",  rangeDimension = " << c.getRangeDimension() << endl;
      {throw "error";}
    }
  }
  setDomainDimension(curve[0]->getDomainDimension());
  setRangeDimension(curve[0]->getRangeDimension());
  for( int axis=axis1; axis<domainDimension; axis++ )
    setGridDimensions( axis,21 );

  uninitialized=TRUE;
  mappingHasChanged();
}


// Copy constructor is deep by default
JoinMapping::
JoinMapping( const JoinMapping & map, const CopyType copyType )
{
  JoinMapping::className="JoinMapping";
  line[0]=line[1]=NULL;
  tfi[0]=tfi[1]=tfi[2]=NULL;
  join=NULL;
  join1=NULL;
  join2=NULL;
  join3=NULL;
  surface1=NULL;
  surface2=NULL;
  surface3=NULL;
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "JoinMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

JoinMapping::
~JoinMapping()
{ 
  if( debug & 4 )
    cout << " JoinMapping::Desctructor called" << endl;
#define DELETE_MAPPING(map) \
  if( map!=NULL && map->decrementReferenceCount()==0 ) \
    delete map;

  DELETE_MAPPING(curve[0]);
  DELETE_MAPPING(curve[1]);
  DELETE_MAPPING(line[0])
  DELETE_MAPPING(line[1])
  DELETE_MAPPING(tfi[0])
  DELETE_MAPPING(tfi[1])
  DELETE_MAPPING(tfi[2])
  DELETE_MAPPING(join)
  DELETE_MAPPING(join1)
  DELETE_MAPPING(join2)
  DELETE_MAPPING(join3)
  DELETE_MAPPING(surface1)
  DELETE_MAPPING(surface2)
  DELETE_MAPPING(surface3)
  
}

JoinMapping & JoinMapping::
operator=( const JoinMapping & X )
{
  if( JoinMapping::className != X.getClassName() )
  {
    cout << "JoinMapping::operator= ERROR trying to set a JoinMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  
  inter=X.inter;
  inter2=X.inter2;

#define EQUALS_MAPPING(map) \
  if( map!=NULL && map->decrementReferenceCount() ) \
    delete map; \
  map=X.map; \
  map->incrementReferenceCount();

  EQUALS_MAPPING(curve[0]);
  EQUALS_MAPPING(curve[1]);
  EQUALS_MAPPING(line[0])
  EQUALS_MAPPING(line[1])
  EQUALS_MAPPING(tfi[0])
  EQUALS_MAPPING(tfi[1])
  EQUALS_MAPPING(tfi[2])
  EQUALS_MAPPING(join)
  EQUALS_MAPPING(join1)
  EQUALS_MAPPING(join2)
  EQUALS_MAPPING(join3)
  EQUALS_MAPPING(surface1)
  EQUALS_MAPPING(surface2)
  EQUALS_MAPPING(surface3)
#undef EQUALS_MAPPING


  uninitialized=X.uninitialized;
  intersectionFound=X.intersectionFound;
  intersectionToUse=X.intersectionToUse;
  numberOfIntersections=X.numberOfIntersections;
  newCurves=X.newCurves;
  endOfJoin=X.endOfJoin;

  this->Mapping::operator=(X);            // call = for derivee class
  return *this;
}


int JoinMapping::
setCurves(Mapping & sourceMapping, 
	  Mapping & clipSurface )
//===========================================================================
/// \details  
///    Supply the source-mapping and clip-surface from which the join will be defined.
///  
/// \param sourceMapping (input): defines the source-mapping. This is the Mapping
///   that will be changed. 
/// \param clipMapping : defines the clip-surface. This Mapping will clip away a
///     portion of the sourceMapping. Use  the setEndOfJoin function to specify
///     which portion of the sourceMapping to retain. 
//===========================================================================
{
  if( curve[0]!=NULL && curve[0]->decrementReferenceCount()==0 )
    delete curve[0];
  curve[0]=&sourceMapping;
  curve[0]->uncountedReferencesMayExist();
  curve[0]->incrementReferenceCount();
  
  if( curve[1]!=NULL && curve[1]->decrementReferenceCount()==0 )
    delete curve[1];
  curve[1]=&clipSurface;
  curve[1]->uncountedReferencesMayExist();
  curve[1]->incrementReferenceCount();

  newCurves=TRUE;
  intersectionFound=FALSE;
  
  setup();
  return 0;
}

int JoinMapping::
setEndOfJoin( const real & endOfJoin_ )
//===========================================================================
/// \details  
///  Specify the r value for the end of the join opposite the curve
///  of intersection. Use this to specify which portion of the source-mapping to retain.
///  For example, choosing a value of $0$ or $1$ will select the portion of the
///  source-mapping that lies on one side of the clip-surface or the other side. Choosing
///  a value of $.8$, for example, will shorten the join-mapping. 
///  
/// \param endOfJoin_ (input) : a value in [0,1].
//===========================================================================
{
  endOfJoin=endOfJoin_;
  return 0;
}


void JoinMapping::
initialize()
// ======================================================================================
//    Compute the new Mapping that will exactly match to another Mapping.
// ======================================================================================
{
  uninitialized=FALSE;
  
  assert( curve[0]!=NULL && curve[1]!=NULL );
  
  if( rangeDimension==2 )
  {
    if( domainDimension==1 )
    {
      // In this case we build a new curve that matches to another curve
      int numberOfIntersectionPoints=0;
      realArray r1,r2,x;
      
      if( newCurves )
      {
	newCurves=FALSE;
	// compute the intersection points between the curves
	// Mapping *inter.rCurve1 : 
	// Mapping *inter.rCurve2 : 
	// Mapping *inter.curve   : 
	int result = inter.intersectCurves(*curve[0],*curve[1],
				     numberOfIntersectionPoints, r1,r2,x);
	if( result!=0 )
	{
	  cout << "JoinMapping:ERROR in computing the intersection between the curves \n";
	  intersectionFound=FALSE;
	}
	else
	{
	  intersectionFound=TRUE;
	}
      }
      if( intersectionFound )
      {
	intersectionFound=TRUE;
	printf("intersection point found. Now compute the new curve that will join to the intersection..\n");

      
	ReparameterizationTransform *map = new ReparameterizationTransform(*curve[0],
                          ReparameterizationTransform::restriction);
        real ra=endOfJoin, rb=endOfJoin;
	
        if( numberOfIntersectionPoints==1 )
	{
	  if( r1(0)<endOfJoin )
	    ra=r1(0);
	  else
	    rb=r1(0);
	}
	else
	{
	  ra=min(r1);
	  rb=max(r1);
	}
	map->setBounds(ra,rb);
        join = map;
      }
    }
    else if( domainDimension==2 )
    {
      // Here we build a new grid that matches to another curve.
      // In this case we need to form a volume grid that joins to curve[1]
      int numberOfIntersectionPoints1=0;
      realArray r11,r12,x1;
      int numberOfIntersectionPoints2=0;
      realArray r21,r22,x2;
      int axisT, result;  // *** need to save axisT
      if( newCurves )
      {
	newCurves=FALSE;
	// compute the intersection points between the curves
	for( axisT=axis1; axisT<=axis2; axisT++ )
	{
  	  surface1 = new ReductionMapping(*curve[0],axisT,0.);          // **** could use axis1==0 *****
          surface1->incrementReferenceCount();
	  
	  printf("\n ----- JoinMapping compute intersection 1 ---------\n");
        
	  result = inter.intersectCurves(*surface1,*curve[1],
				     numberOfIntersectionPoints1, r11,r12,x1);
          if( numberOfIntersectionPoints1>0 )
            break;
	}
	if( result!=0 )
	{
	  cout << "JoinMapping:ERROR in computing the intersection curve 1\n";
	  intersectionFound=FALSE;
	}
	else
	  intersectionFound=TRUE;

	if( intersectionFound )
	{
	  surface2 = new ReductionMapping(*curve[0],axisT,1.); 
          surface2->incrementReferenceCount();

	  printf("\n ----- JoinMapping compute intersection 2 ---------\n");

	  result = inter.intersectCurves(*surface2,*curve[1],
				       numberOfIntersectionPoints2, r21,r22,x2);
	  if( result!=0 || numberOfIntersectionPoints2<=0 )
	  {
	    cout << "JoinMapping:ERROR in computing the intersection curve 2\n";
	    intersectionFound=FALSE;
	  }
	  else 
	    intersectionFound=TRUE; 
	}
      }
      if( intersectionFound )
      {
	printf("intersection points found. Now compute the new surface that will join to the intersection..\n");

        // A multiple join occurs in the joinAnnulusToCircle for example.
        const bool multipleJoin= numberOfIntersectionPoints1==2 && numberOfIntersectionPoints2==2;
        int ia=0, ib=1;
        if( multipleJoin && r11(0)>r11(1) )
	{
	  ia=1;
	  ib=0;
	}
#define ASSIGN_MAP_POINTER(map,map2) \
   if( map!=NULL && map->decrementReferenceCount()==0 ) \
     delete map; \
     map=map2; \
     map->incrementReferenceCount();
    
        ReparameterizationTransform *map = new ReparameterizationTransform(*curve[1],
                          ReparameterizationTransform::restriction);
        ASSIGN_MAP_POINTER(join1,map);

        real ra, rb;
        ra=r12(ia);
	rb=r22(ia);
	map->setBounds(ra,rb); 
        
        map = new ReparameterizationTransform(*surface1,ReparameterizationTransform::restriction);
        ra=r11(ia);
	rb=multipleJoin ? r11(ib) : endOfJoin;
	map->setBounds(ra,rb); 
        ASSIGN_MAP_POINTER(join2,map);

        map = new ReparameterizationTransform(*surface2,ReparameterizationTransform::restriction);
        ra=r21(ia);
	rb=multipleJoin ? r21(ib) : endOfJoin;
	map->setBounds(ra,rb); 
        ASSIGN_MAP_POINTER(join3,map);

        if( multipleJoin )
	{
          map = new ReparameterizationTransform(*curve[1],ReparameterizationTransform::restriction);
          ra=r12(ib);
	  rb=r22(ib);
	  map->setBounds(ra,rb); 
          ASSIGN_MAP_POINTER(tfi[0],map);
	}
	else
	{
	  Mapping *map = new ReductionMapping(*curve[0],(axisT+1)%2,endOfJoin); 
	  ASSIGN_MAP_POINTER(tfi[0],map);
	}
        // compose the tfi in parameter space with curve[0]
	Mapping *mapPointer = new TFIMapping(join1,tfi[0],join2,join3);
        ASSIGN_MAP_POINTER(join,mapPointer);


      }
    }
  }
  else if( domainDimension==2 )
  {
    // In this case we build a surface grid in 3D that joins to another surface
    if( newCurves )
    {
      newCurves=FALSE;
      // compute the intersection points between the curves
      // Mapping *inter.rCurve1 : 
      // Mapping *inter.rCurve2 : 
      // Mapping *inter.curve   : 
      int result = inter.intersect(*curve[0],*curve[1]);
      if( result!=0 )
      {
	cout << "JoinMapping:ERROR in computing the intersection curve \n";
        intersectionFound=FALSE;
      }
      else
      {
        intersectionFound=TRUE;
        if( inter.curve->getIsPeriodic(axis1)==functionPeriodic )
	{
          // curve is periodic -> join is periodic
          setIsPeriodic(axis1,functionPeriodic);
          setBoundaryCondition(Start,axis1,-1);
          setBoundaryCondition(End  ,axis1,-1);
	}
      }
    }
    if( intersectionFound )
    {
      intersectionFound=TRUE;
      printf("intersection curve found. Now compute the new surface that will join to the intersection..\n");

      realArray r(2,1),x(2,2);
      r=0; r(1,0)=1.;
      inter.rCurve1->map(r,x);  // intersection curve may not be parameterized from [0,1]
      printf("Intersection curve in parameter space: r_0=[%e,%e], r_1=[%e,%e] \n",x(0,0),x(1,0),x(0,1),x(1,1));
      // we have two choices for the shape of the patch in the parameter space.
      
      Mapping *map;
      if( fabs(x(1,0)-x(0,0)) > fabs(x(1,1)-x(0,1)) )
        map = new LineMapping(x(0,axis1),endOfJoin, x(1,axis1),endOfJoin );  // assumes angular direction is axis1
      else
        map = new LineMapping(endOfJoin,x(0,axis2), endOfJoin,x(1,axis2) ); 
      ASSIGN_MAP_POINTER(line[0],map);

      map = new TFIMapping(0,0,inter.rCurve1,line[0]);   // tfi mapping in parameter space.
      ASSIGN_MAP_POINTER(tfi[0],map);
      tfi[0]->setRangeSpace(parameterSpace);
      
      map = new ComposeMapping(*tfi[0],*curve[0]);
      ASSIGN_MAP_POINTER(join,map);
      
    }
  }
  else if( domainDimension==3 )
  {
    // In this case we need to form a volume grid that joins to curve[1]
    if( newCurves )
    {
      newCurves=FALSE;
      // compute the intersection points between the curves

      // guess these: 
      int axisNormal=axis3;   // the join mapping expects axis3 to be the radial direction 

      Mapping *map = new ReductionMapping(*curve[0],axisNormal,0);          // **** keep these?? *****
      ASSIGN_MAP_POINTER(surface1,map);

      printf("\n ----- JoinMapping compute intersection 1 ---------\n");
      
      int result = inter.intersect(*surface1,*curve[1]);
      // This needs more work: 
//       if( result!=0 )
//       {
// 	printF("JoinMapping:ERROR in computing the intersection curve 1. Try using another face of the mapping...\n");
//         axisNormal=axis2;
// 	Mapping *map2 = new ReductionMapping(*curve[0],axisNormal,0);          // **** keep these?? *****
// 	ASSIGN_MAP_POINTER(surface1,map2);
//         result = inter.intersect(*surface1,*curve[1]);
//       }
      
      if( result!=0 )
      {
	printF("JoinMapping:ERROR in computing the intersection curve 1\n");
        intersectionFound=FALSE;
	
	GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();
	gi.stopReadingCommandFile();
	gi.erase();
        GraphicsParameters params;
        params.set(GI_TOP_LABEL,"Surface 1 (curve[1] will join to this surface)");
	params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	PlotIt::plot(gi,*surface1,params);
        params.set(GI_TOP_LABEL,"Surface 1 and curve[1]");
	params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	PlotIt::plot(gi,*curve[1],params);
	
      }
      else
      {
        intersectionFound=true;
        if( inter.curve->getIsPeriodic(axis1)==functionPeriodic )
	{
          // curve is periodic -> join is periodic
          setIsPeriodic(axis1,functionPeriodic);
          setBoundaryCondition(Start,axis1,-1);
          setBoundaryCondition(End  ,axis1,-1);
	}
      }
      if( intersectionFound )
      {
	map = new ReductionMapping(*curve[0],axisNormal,1); 
	ASSIGN_MAP_POINTER(surface2,map);

        printf("\n ----- JoinMapping compute intersection 2 ---------\n");

	int result = inter2.intersect(*surface2,*curve[1]);
	if( result!=0 )
	{
	  cout << "JoinMapping:ERROR in computing the intersection curve 2\n";
	  intersectionFound=FALSE;
	}
        else if( inter.curve->getIsPeriodic(axis1) !=inter2.curve->getIsPeriodic(axis1) )
	{
	  printf("JoinMapping::ERROR:One curve of intersection is periodic and the other is not. Something is "
                 "wrong here!\n    It could be that the surfaces do not completely intersect. \n");
	  intersectionFound=FALSE;
	}
	else
         intersectionFound=TRUE; 
      }
    }
    if( intersectionFound )
    {
      printf("intersection curves found. Now compute the new surface that will join to the intersection..\n");

      realArray r(2,1),x(2,2);
      r=0; r(1,0)=1.;

      inter.rCurve1->map(r,x);  // intersection curve may not be parameterized from [0,1]
      printf("Intersection curve1 in parameter space: r_0=[%e,%e], r_1=[%e,%e] \n",x(0,0),x(1,0),x(0,1),x(1,1));
      Mapping *map;
      // we have two choices for the shape of the patch in the parameter space.
      if( fabs(x(1,0)-x(0,0)) > fabs(x(1,1)-x(0,1)) )
        map = new LineMapping(x(0,axis1),endOfJoin, x(1,axis1),endOfJoin );  // assumes angular direction is axis1
      else
        map = new LineMapping(endOfJoin,x(0,axis2), endOfJoin,x(1,axis2) ); 

      ASSIGN_MAP_POINTER(line[0],map);

      map = new TFIMapping(0,0,inter.rCurve1,line[0]);   // tfi mapping in parameter space.
      ASSIGN_MAP_POINTER(tfi[0],map);
      tfi[0]->setRangeSpace(parameterSpace);

      map = new ComposeMapping(*tfi[0],*surface1);
      ASSIGN_MAP_POINTER(join1,map);

      inter2.rCurve1->map(r,x);  // intersection curve may not be parameterized from [0,1]
      printf("Intersection curve2 in parameter space: r_0=[%e,%e], r_1=[%e,%e] \n",x(0,0),x(1,0),x(0,1),x(1,1));
      // we have two choices for the shape of the patch in the parameter space.

      if( fabs(x(1,0)-x(0,0)) > fabs(x(1,1)-x(0,1)) )
        map = new LineMapping(x(0,axis1),endOfJoin, x(1,axis1),endOfJoin );  // assumes angular direction is axis1
      else
        map = new LineMapping(endOfJoin,x(0,axis2), endOfJoin,x(1,axis2) ); 

      ASSIGN_MAP_POINTER(line[1],map);

      map = new TFIMapping(0,0,inter2.rCurve1,line[0]);   // tfi mapping in parameter space.
      ASSIGN_MAP_POINTER(tfi[1],map);
      tfi[1]->setRangeSpace(parameterSpace);
      
      map = new ComposeMapping(*tfi[1],*surface2);
      ASSIGN_MAP_POINTER(join2,map);
      
      // Make the parameter space Mapping for the annular region between the two surfaces
      map = new TFIMapping(0,0,inter.rCurve2,inter2.rCurve2);
      ASSIGN_MAP_POINTER(tfi[2],map);
      tfi[2]->setRangeSpace(parameterSpace);

      map = new ComposeMapping(*tfi[2],*curve[1]);
      ASSIGN_MAP_POINTER(join3,map);

      map = new ReductionMapping(*curve[0],axis2,endOfJoin);
      ASSIGN_MAP_POINTER(surface3,map);

      //join = new TFIMapping(0,0,0,0,join1,join2);
      map = new TFIMapping(0,0,join3,surface3,join1,join2);
      ASSIGN_MAP_POINTER(join,map);
    }
  }
  if( join!=NULL )
  {
    for( int axis=0; axis<domainDimension; axis++ )
    {
      join->setIsPeriodic(axis,getIsPeriodic(axis));
    }
  }
  
}

void JoinMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the TFI and/or derivatives. 
//=====================================================================================
{
  if( uninitialized )
    initialize();
  if( !intersectionFound )
  {
    cout << "JoinMapping::ERROR: the join has not been created yet\n";
    return;
  }
    
  if( params.coordinateType != cartesian )
    cerr << "JoinMapping::map - coordinateType != cartesian " << endl;

  assert( join!=NULL );
  join->map(r,x,xr);
  
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int JoinMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering JoinMapping::get" << endl;
  subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( JoinMapping::className,"className" );
  subDir.get( endOfJoin,"endOfJoin" ); 
  subDir.get( uninitialized,"uninitialized" ); 
  subDir.get( intersectionFound,"intersectionFound" ); 
  subDir.get( intersectionToUse,"intersectionToUse" ); 
  subDir.get( numberOfIntersections,"numberOfIntersections" );
  subDir.get( newCurves,"newCurves" ); 
  int temp; subDir.get( temp, "joinType" ); joinType=(JoinType)temp;

  char buff[40];
  for( int i=0; i<1; i++ )  // *** only get 1 mapping for now.
  {
    Mapping *mapPointer = i==0 ? join :
                           i==1 ? curve[0] :
                           i==2 ? curve[1] :
                           i==3 ? &inter : 
                                 &inter2;
               
    int mappingExists=   mapPointer!=NULL ? 1 : 0;
    subDir.get(mappingExists,sPrintF(buff,"mapping%iExists",i));
    if( mappingExists )
    {
      aString mapClassName;
      subDir.get(mapClassName,sPrintF(buff,"mapping%iClassName",i));
      mapPointer = Mapping::makeMapping( mapClassName );
      if( mapPointer==NULL )
      {
	cout << "JoinMapping::get:ERROR unable to make the mapping with className = " 
	  << (const char *)mapClassName << endl;
        {throw "error";}
      }
      mapPointer->get( subDir,sPrintF(buff,"mapping%i",i) );
    }
    if( i==0 ) 
      join=mapPointer;
    else if( i==1 ) 
      curve[0]=mapPointer;
    else if( i==2 ) 
      curve[1]=mapPointer;
  }

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();

  delete &subDir;

  return 0;
}

int JoinMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  
  // save the mapping as a stream of data by default, this is more efficient
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( JoinMapping::className,"className" );
  subDir.put( endOfJoin,"endOfJoin" ); 
  subDir.put( uninitialized,"uninitialized" ); 
  subDir.put( intersectionFound,"intersectionFound" ); 
  subDir.put( intersectionToUse,"intersectionToUse" ); 
  subDir.put( numberOfIntersections,"numberOfIntersections" );
  subDir.put( newCurves,"newCurves" ); 
  subDir.put( (int)joinType, "joinType" );

  // put the final "join" mapping -- we could also save the surfaces and intersections??
  char buff[40];
  for( int i=0; i<1; i++ )   // ***NOTE*** we only save the join for now.
  {
    Mapping *mapPointer = i==0 ? join :
                          i==1 ? (Mapping*) curve[0] :
                          i==2 ? (Mapping*) curve[1] :
                          i==3 ? (Mapping*) &inter : 
                                 (Mapping*) &inter2;
               
    int mappingExists=   mapPointer!=NULL ? 1 : 0;
    subDir.put(mappingExists,sPrintF(buff,"mapping%iExists",i));
    if( mappingExists )
    {
      subDir.put(mapPointer->getClassName(),sPrintF(buff,"mapping%iClassName",i));
      mapPointer->put( subDir,sPrintF(buff,"mapping%i",i) );
    }
  }

  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping *JoinMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==JoinMapping::className )
    retval = new JoinMapping();
  return retval;
}

    

int JoinMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the Join mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
    {
      "!JoinMapping",
      "compute join",
      "choose curves",
      "end of join",
      "choose intersection",
      "plot surfaces (toggle)",
      "plot tfi",
      "plot join",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "compute join     : recompute join with current parameters",
      "choose curves    : choose the 'source-mapping' and 'clip-surface'",
      "end of join      : r value for the end of the join opposite the intersection curve",
      "choose intersection: choose which intersection point/curve to use",
      "plot surfaces (toggle)",
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "check              : check properties of the mapping.",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,answer2,answer3; 

  bool plotObject= curve[0]!=NULL && curve[1]!=NULL;
  newCurves= uninitialized &&  curve[0]!=NULL && curve[1]!=NULL;
  bool plotSurfaces=TRUE;   // plot the two intersecting surfaces
      
  GraphicsParameters params;
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  // plottingbounds:
  RealArray xBound(2,3);

  gi.appendToTheDefaultPrompt("JoinMapping>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="choose curves" )
    { 
      // include all curves/surfaces and all faces of volume grids with bc>0

      printF("Choose two 'curves'. The first is the 'source-mapping' and the second is the 'clip-surface'.\n"
             "The source-mapping will be truncated by the clip-surface to form the join mapping.\n"
             "Use the 'end of join' option to specify what portion of the source-mapping to retain.\n" );

      newCurves=TRUE;  // tells initialize to recompute the intersection
      // Make a menu with the Mapping names (only curves or surfaces!)
      int numberOfMaps=mapInfo.mappingList.getLength();
      int numberOfFaces=numberOfMaps*(6+1);  // up to 6 sides per grid plus grid itself
      aString *menu2 = new aString[numberOfFaces+2];
      IntegerArray subListNumbering(numberOfFaces);
      int i, j=0;
      for( i=0; i<numberOfMaps; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( (map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this) ||
            (map.getDomainDimension()==map.getRangeDimension() && map.getDomainDimension()>1 ) )
	{
          if( map.getDomainDimension()==map.getRangeDimension()-1 )
	  {
    	    subListNumbering(j)=i;
            menu2[j++]=map.getName(mappingName);
          }
	  else
	  {
    	    subListNumbering(j)=i;
            menu2[j++]=map.getName(mappingName);  
            // include all sides that are physical boundaries.
	    for( int axis=axis1; axis<map.getDomainDimension(); axis++ )
	    {
	      for( int side=Start; side<=End; side++ )
	      {
                if( map.getBoundaryCondition(side,axis)>0 )
		{
  	          subListNumbering(j)=i;
                  menu2[j++]=sPrintF(buff,"%s (side=%i,axis=%i)",(const char *)map.getName(mappingName),side,axis);
		}
	      }
	    }
          }
	}
      }
      if( j==0 )
      {
	gi.outputString("JoinMapping::WARNING: There are no appropriate curves/surfaces to choose from");
        continue;
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
	
      aString choice[] = {"choose the source-mapping (the Mapping that will be deformed to match the clip-surface)",
                          "choose the clip-surface (the mapping that will clip the source-mapping)"
                        }; 
      for( i=0; i<=1; i++ )
      {
        aString prompt = choice[i];
        int mapNumber = gi.getMenuItem(menu2,answer2,prompt);
        mapNumber=subListNumbering(mapNumber);  // map number in the original list
        if( mapInfo.mappingList[mapNumber].mapPointer==this )
        {
    	  cout << "JoinMapping::ERROR: you cannot use this mapping, this would be recursive!\n";
          continue;
        }
        Mapping & map =* mapInfo.mappingList[mapNumber].mapPointer;

	if( map.getDomainDimension()==map.getRangeDimension()-1 )
	{
          if( curve[i]!=NULL && curve[i]->decrementReferenceCount()==0 )
	    delete curve[i];
          curve[i]=mapInfo.mappingList[mapNumber].mapPointer;
          curve[i]->uncountedReferencesMayExist();
	  curve[i]->incrementReferenceCount();
	}
	else if( map.getDomainDimension()==map.getRangeDimension() )
	{
	  // we may need to build a Mapping that corresponds to a side of a volume grid.
          int side=-1, axis=-1;
          // sScanF(answer2,"(side=%i axis=%i)",&side,&axis); // remember that commas are removed
          int length=answer2.length();
	  for( int j=0; j<length-6; j++ )
	  {
	    if( answer2(j,j+5)=="(side=" ) 
	    {
              sScanF(answer2(j,length-1),"(side=%i axis=%i",&side,&axis); // remember that commas are removed
	      if( side<0 || axis<0 )
	      {
		cout << "Error getting (side,axis) from choice!\n";
		throw "error";
	      }
	      Mapping *mapPointer= new ReductionMapping(map,axis,side);  
              ASSIGN_MAP_POINTER(curve[i],mapPointer);
	      printf(" create a mapping for (side,axis)=(%i,%i) for curve[%i] \n",side,axis,i);
              break;
	    }
	  }
	  if( side<0 || axis<0 )
	  {
	    printf("Setting curve[%i] \n",i);
	    if( curve[i]!=NULL && curve[i]->decrementReferenceCount()==0 )
	      delete curve[i];
            curve[i]=mapInfo.mappingList[mapNumber].mapPointer; // build the volume grid instead.
            curve[i]->uncountedReferencesMayExist();
	    curve[i]->incrementReferenceCount();
	  }
	}
	else
	{
	  throw "error";
	}
      }
      
      delete [] menu2;
      // Define properties of this mapping
      setDomainDimension(curve[0]->getDomainDimension());
      setRangeDimension(curve[0]->getRangeDimension());
      for( int axis=0; axis<domainDimension; axis++ )
        setGridDimensions(axis,curve[0]->getGridDimensions(axis));

      uninitialized=TRUE;
      plotObject=TRUE;
    }
    else if( answer=="compute join" )
    {
      if( uninitialized )
      {
        initialize();
        mappingHasChanged(); 
      }
      else
        printf("JoinMapping:INFO: No need to recompute join\n");
    }
    else if( answer=="choose intersection" )
    {
      gi.inputString(answer3,sPrintF(buff,"Choose intersection point/curve (0,...,%i) (current=(%i)): ",
              numberOfIntersections-1,intersectionToUse));
      if( answer3!="" ) 
      {
	sScanF(answer3,"%i",&intersectionToUse);
        uninitialized=TRUE;
      }
    }
    else if( answer=="end of join" )
    {
      gi.inputString(answer3,sPrintF(buff,"Enter the r value [0,1] for the end of the join (current=(%e)): ",
				     endOfJoin));
      if( answer3!="" ) 
      {
	sScanF(answer3,"%e",&endOfJoin);
        uninitialized=TRUE;
      }
    }
    else if( answer=="plot surfaces (toggle)" )
    {
      plotSurfaces=!plotSurfaces;
    }
    else if( answer=="plot join" )
    {
      if( join!=NULL )
      {
	gi.erase();
	PlotIt::plot(gi,*join);
      }
      else
      {
	printf("The join has not been computed yet\n");
      }
    }
    else if( answer=="plot tfi" )
    {
      params.set(GI_USE_PLOT_BOUNDS,FALSE);
      if( tfi[0]!=NULL )
      {
        gi.erase();
	params.set(GI_TOP_LABEL,"curve of intersection 1 (parameter space)");
	PlotIt::plot(gi,*inter.rCurve1,params);
	gi.erase();
	params.set(GI_TOP_LABEL,"tfi Mapping 1 (parameter space)");
	PlotIt::plot(gi,*tfi[0],params);
	gi.erase();
      }
      if( tfi[1]!=NULL )
      {
        gi.erase();
	params.set(GI_TOP_LABEL,"curve of intersection 2 (parameter space)");
	PlotIt::plot(gi,*inter2.rCurve1,params);
	gi.erase();
	params.set(GI_TOP_LABEL,"tfi Mapping 2 (parameter space)");
	PlotIt::plot(gi,*tfi[1],params);
	gi.erase();

	params.set(GI_TOP_LABEL,"tfi Mapping 3 (parameter space)");
	PlotIt::plot(gi,*tfi[2],params);
	gi.erase();

	params.set(GI_TOP_LABEL,"opposite boundary (surface3)");
	PlotIt::plot(gi,*surface3,params);
	gi.erase();


      }
    }
    else if( answer=="show parameters" )
    {
      // printf(" joinWidth = %e, joinOverlap = %e \n",joinWidth,joinOverlap);
      display();
      if( join!=NULL )
        join->display("Here is the join");
	
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity"  ||
             answer=="check" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;

      if( join!=NULL )
      {
	for( int axis=0; axis<domainDimension; axis++ )
	{
	  if( answer=="periodicity" )
	    join->setIsPeriodic(axis,getIsPeriodic(axis));
          else if( answer=="boundary conditions" )
	  {
            for( int side=Start; side<=End; side++ )
  	      join->setBoundaryCondition(side,axis,getBoundaryCondition(side,axis));
	  }
	}
      }

    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    
    if( plotObject )
    {

      if( newCurves )
      {
	// determine bounds on the mappings for plotting
	Bound b;
	xBound=0.;
	for( int m=0; m<2; m++)
	{
	  Mapping & map = *curve[m];
          map.getGrid(); // do this to make sure the bounds are defined
	  for( int axis=0; axis<map.getRangeDimension(); axis++ )
	  {
	    b = map.getRangeBound(Start,axis);
	    if( b.isFinite() )
	      xBound(Start,axis)=min(xBound(Start,axis),(real)b);
	    b = map.getRangeBound(End,axis);
	    if( b.isFinite() )
	      xBound(End,axis)=max(xBound(End,axis),(real)b);
	  }
	}
	params.set(GI_PLOT_BOUNDS,xBound);
      }


      if( !plotSurfaces )
        params.set(GI_USE_PLOT_BOUNDS,FALSE); 
      else
      {
	params.set(GI_USE_PLOT_BOUNDS,TRUE); 
	params.set(GI_PLOT_BOUNDS,xBound);
      }

      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      gi.erase();

      if( intersectionFound )
      {
        printf("plot the join... \n");
	
        params.set(GI_TOP_LABEL,getName(mappingName));
        params.set(GI_MAPPING_COLOUR,"green");
        PlotIt::plot(gi,*this,params);  
      }
      
      if( plotSurfaces )
      {
	// params.set(GI_PLOT_SHADED_SURFACE,FALSE);

        params.set(GI_SURFACE_OFFSET,(real)20.);  // offset the surfaces so we see the join better
	params.set(GI_TOP_LABEL,"");
	params.set(GI_MAPPING_COLOUR,"red");
	PlotIt::plot(gi,*curve[0],params);   
	params.set(GI_MAPPING_COLOUR,"blue");
	PlotIt::plot(gi,*curve[1],params);   
        params.set(GI_SURFACE_OFFSET,(real)3.);   // reset to default

	// params.set(GI_PLOT_SHADED_SURFACE,TRUE);
      }
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      params.set(GI_USE_PLOT_BOUNDS,FALSE); 

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
