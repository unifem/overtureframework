#include "ReparameterizationTransform.h"
#include "MappingInformation.h"
#include "OrthographicTransform.h"
#include "MappingRC.h"
#include "RestrictionMapping.h"
#include "ReorientMapping.h"
#include "SplineMapping.h"
#include "display.h"
#include "SquareMapping.h"
#include "BoxMapping.h"

// *NOTE* The static variables localParams and localParamsAreBeingUsed are set in initStaticMappingVariables


int
equidistribute( const realArray & w, realArray & r );


ReparameterizationTransform::
ReparameterizationTransform() : ComposeMapping()
//===========================================================================
/// \brief  Default Constructor
///     The {\tt ReparameterizationTransform} can reparameterize a given Mapping
///  in one of the following ways:
///    <ul>
///       <li>[orthographic:] Remove a polar singularity by using a orthographic projection
///          to define a new patch over the singularity.
///       <li>[restriction:] restrict the parameter space to a sub-rectangle of the
///           original parameter space. Use this, for example, to define a refined patch in an
///           adaptive grid.
///       <li>[equidistribution:] reparameterize a curve in 2D or 3D so as to equi-distribute
///          a weighted sum of arclength and curvature.   
///    </ul>   
//===========================================================================
{ 
  reparameterizationType=orthographic;
  reparameterize = NULL;

  ReparameterizationTransform::className="ReparameterizationTransform";
  setName( Mapping::mappingName,"Transform");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );

  coordinateType=spherical;
  arcLengthWeight=1.;
  curvatureWeight=0.;
  numberOfEquidistributionSmooths=3;
  equidistributionInitialized=false;
// for multiple compositions of restriction mappings -- keep scaling to original
  mr[0]=0.; mr[1]=1.; mr[2]=0.; mr[3]=1.; mr[4]=0.; mr[5]=1.;
  
  mappingHasChanged();
}


ReparameterizationTransform::
ReparameterizationTransform(Mapping & map, 
                            const ReparameterizationTypes type /* = defaultReparameterization */) 
: ComposeMapping()
// =================================================================================
/// \details  Constructor for a Reparameterization. 
/// \param map (input) : mapping to reparameterize.
/// \param type (input) : 
// ================================================================================
{ 
  constructor(map,type);
}

ReparameterizationTransform::
ReparameterizationTransform(MappingRC & mapRC, 
                            const ReparameterizationTypes type /* = defaultReparameterization */) 
: ComposeMapping()
// =================================================================================
/// \details  Constructor for a Reparameterization. 
///     See the comments in the constructor member function
// ================================================================================
{
  assert(mapRC.mapPointer!=NULL);
  constructor(*mapRC.mapPointer,type);
}

void ReparameterizationTransform::
constructor(Mapping & map, const ReparameterizationTypes type)
// =================================================================================
/// \details  This is a protected routine, used internally.
///     Constructor for a Reparameterization. This constructor will
///    check to see if you are trying to reparameterize a Mapping that is already
///    the same type of reparameterization of another mapping. For example you may
///    be making a sub-mapping (restriction) of a sub-mapping. In this case this
///    constructor will eliminate the multiple restriction operations and replace
///    it by a single restriction. You should then use the scaleBounds member function
///    to define a new restriction. This function will scale the bounds found in map.
// ================================================================================
{
  ReparameterizationTransform::className="ReparameterizationTransform";
  setName( Mapping::mappingName,"Transform");

  coordinateType=spherical;
  arcLengthWeight=1.;
  curvatureWeight=0.;
  numberOfEquidistributionSmooths=3;
  equidistributionInitialized=false;

  // for multiple compositions of restriction mappings -- keep scaling to original
  mr[0]=0.; mr[1]=1.; mr[2]=0.; mr[3]=1.; mr[4]=0.; mr[5]=1.;
  if( type==defaultReparameterization )
  {
    // choose orthographic if the mapping has a singularity 
    reparameterizationType=restriction;
    for( int axis=0; axis<map.getDomainDimension(); axis++ )
      for( int side=Start; side<=End; side++ )
	if( map.getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
	  reparameterizationType=orthographic;
  }
  else
    reparameterizationType=type;

  if( map.getClassName()==ReparameterizationTransform::className &&
      ((ReparameterizationTransform &)map).reparameterizationType==reparameterizationType )
  {
    constructorForMultipleReparams( (ReparameterizationTransform &)map );
  }
  else
  {
    
    if( reparameterizationType==orthographic )
      reparameterize = new OrthographicTransform();
    else if( reparameterizationType==restriction )
      reparameterize = new RestrictionMapping();
    else if( reparameterizationType==equidistribution )
    {
      reparameterize = new SplineMapping();
      reparameterize->setRangeSpace(parameterSpace);
    }
    else if( reparameterizationType==reorientDomainCoordinates )
    {
      reparameterize = new ReorientMapping();
    }
    else
    {
      printf("ReparameterizationTransform::ERROR: unknown reparameterization type\n");
      {throw "error: unknown reparameterization type";}
    }
    

    reparameterize->incrementReferenceCount();   // this says the mapping was newed
    
    // set dimensions of reparameterization mapping to match
    reparameterize->setDomainDimension(map.getDomainDimension()); 
    reparameterize->setRangeDimension(map.getDomainDimension());
    // compose the mappings:
    setMappings(*reparameterize,map);
    reparameterize->decrementReferenceCount(); // setMappings will also increment ref count so we decrement here
    setName(mappingName,aString("reparameterized-")+map.getName(mappingName));

    setMappingProperties(&map);
  }
  mappingHasChanged();
}

void ReparameterizationTransform::
constructorForMultipleReparams(ReparameterizationTransform & rtMap )
// ==========================================================================================
/// \details  **This is a protected routine**
///    If you want to reparameterize a mapping that is already Reparameterized then use this
///    constructor.  It will replace multiple reparams of the same type with just one reparam
/// \param Notes:
///    
// ==========================================================================================
{ 

  if( Mapping::debug & 4 ) 
    printF("constructorForMultipleReparams called! \n");
  
  // this is the mapping we actually reparameterize
  assert( rtMap.map2.mapPointer!=NULL );
  Mapping *map = rtMap.reparameterizationType==reparameterizationType ? rtMap.map2.mapPointer : &rtMap; 

  if( reparameterizationType==orthographic )
    reparameterize = new OrthographicTransform();
  else if( reparameterizationType==restriction )
  {
    // if we are replacing the restriction mapping, then copy current values
    if( rtMap.reparameterizationType==reparameterizationType )
    {
      // Save the bounds of the original restriction.
      rtMap.getBounds(mr[0],mr[1],mr[2],mr[3],mr[4],mr[5]);  // use these I think

//        printf("constructorForMultipleReparams: mr=%8.2e,%8.2e,%8.2e,%8.2e,%8.2e,%8.2e\n",
//  	     mr[0],mr[1],mr[2],mr[3],mr[4],mr[5]);
      
      RestrictionMapping & baseRestriction=(RestrictionMapping &)*rtMap.reparameterize;
      reparameterize = new RestrictionMapping(baseRestriction);
      // reparameterize = new RestrictionMapping((RestrictionMapping &)*rtMap.reparameterize);
    }
    else
      reparameterize = new RestrictionMapping();

    //  (RestrictionMapping &)*reparameterize= (RestrictionMapping &)*rtMap.reparameterize; 
  }
  else 
  {
    reparameterize = new SplineMapping();
    reparameterize->setRangeSpace(parameterSpace);
  }
  
  reparameterize->incrementReferenceCount();   // this says the mapping was newed

  // compose the mappings:
  setMappings(*reparameterize,*map);
  setName(mappingName,aString("reparameterized-")+map->getName(mappingName));
  reparameterize->decrementReferenceCount(); // setMappings will also increment ref count so we decrement here

  setMappingProperties(map);
  mappingHasChanged();
}




// Copy constructor is deep by default
ReparameterizationTransform::
ReparameterizationTransform( const ReparameterizationTransform & map, const CopyType copyType )
{
  ReparameterizationTransform::className="ReparameterizationTransform";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    printF("ReparameterizationTransform:: sorry no shallow copy constructor, doing a deep! \n");
    *this=map;
  }
}

ReparameterizationTransform::
~ReparameterizationTransform()
{ 
}

ReparameterizationTransform & ReparameterizationTransform::
operator=( const ReparameterizationTransform & X )
//
// /Description:  Deep Copy
//
{
  if( Mapping::debug & 4 ) 
    printF("ReparameterizationTransform::operator= called \n");
  if( ReparameterizationTransform::className != X.getClassName() )
  {
    printF("ReparameterizationTransform::operator= ERROR trying to set a ReparameterizationTransform = to a %s\n",
	   (const char*)X.getClassName());
    return *this;
  }
  // The next = operator will just copy the pointers to the composed mappings
  this->ComposeMapping::operator=(X);            // call = for derivee class
  coordinateType=X.coordinateType;
  reparameterizationType=X.reparameterizationType;

  reparameterize=map1.mapPointer;
  arcLengthWeight=X.arcLengthWeight;
  curvatureWeight=X.curvatureWeight;
  numberOfEquidistributionSmooths=X.numberOfEquidistributionSmooths;
  equidistributionInitialized=X.equidistributionInitialized;
  for( int i=0; i<6; i++ )
    mr[i]=X.mr[i];

  return *this;
}

// Copy like constructor that makes a deep copy of all but the transformed Mapping which is replaced by "map"
// This function is used by the GridCollection equals operator to build AMR grids 
ReparameterizationTransform::
ReparameterizationTransform( const ReparameterizationTransform & x, MappingRC & map )
{
  ReparameterizationTransform::className="ReparameterizationTransform";

  // **** this->ComposeMapping::operator=(x);            // call = for derivee class
  // ------------ start ComposeMapping operator= --------------------
  //   We should really make a separate equals function for the ComposeMapping *** fix this ***
  // For now this was copied from ComposeMapping.C

  // ****Are these next lines are correct if map is itself a ReparameterizationTransform ???
  map1=x.map1;
  // *** map2=x.map2;
  if( map.getClassName()!="ReparameterizationTransform" )
  {
    map2.reference(map);   // use this Mapping instead of the one in x
  }
  else
  {
    // If map is itself a restriction ReparameterizationTransform we need to use the original
    //  un-Reparameterized Mapping. This case occurs when we have made an AMR refinement 
    // grid on top of a base Mapping which is itself a ReparameterizationTransform.
    ReparameterizationTransform & rtMap = (ReparameterizationTransform&)map.getMapping();
    if( rtMap.reparameterizationType==restriction )
    {

      if( debug & 16 ) 
        printf(" ************* ReparameterizationTransform: copy constructor for AMR -- map is a restriction!! *****\n");
      
      // *wdh* 060428: remove this next assert -- rtMap.map2 could itself be a ReparamTransform.
      // assert( rtMap.map2.getClassName()!="ReparameterizationTransform" );
      map2.reference(rtMap.map2);
    }
    else
    {
      map2.reference(map);
    }
    
  }
  

  if( map1.mapPointer && map2.mapPointer )
    setMappings(*map1.mapPointer,*map2.mapPointer);

  useDefaultInverse=x.useDefaultInverse;

  this->Mapping::operator=(x);            // call = for base class
  // -------------- end ComposeMapping operator= ----------------

  coordinateType=x.coordinateType;
  reparameterizationType=x.reparameterizationType;

  reparameterize=map1.mapPointer;
  arcLengthWeight=x.arcLengthWeight;
  curvatureWeight=x.curvatureWeight;
  numberOfEquidistributionSmooths=x.numberOfEquidistributionSmooths;
  equidistributionInitialized=x.equidistributionInitialized;
  for( int i=0; i<6; i++ )
    mr[i]=x.mr[i];              // for multiple reparam's, this is the scaling to the original

}


int ReparameterizationTransform::
scaleBounds(const real ra, /* =0. */
	    const real rb, /* =1. */ 
	    const real sa, /* =0. */
	    const real sb, /* =1. */
	    const real ta, /* =0. */
	    const real tb  /* =1. */ )
// ==========================================================================================
/// \details  
///     Scale the current bounds for a restriction Mapping. See the documentation for the
///    {\tt RestrictionMapping} for further details.
/// \param ra,rb,sa,sb,ta,tb (input): 
// ==========================================================================================
{
  if( reparameterizationType==restriction )
  {
    assert(reparameterize!=NULL);
    RestrictionMapping & rm = (RestrictionMapping &)*reparameterize;
    mappingHasChanged();
    // We may need to scale if we have composed restriction's
    real dr=mr[1]-mr[0],ds=mr[3]-mr[2],dt=mr[5]-mr[4];
    return rm.scaleBounds( mr[0]+dr*ra,mr[0]+dr*rb,mr[2]+ds*sa,mr[2]+ds*sb,mr[4]+dt*ta,mr[4]+dt*tb );
    // return rm.scaleBounds( ra,rb,sa,sb,ta,tb );
  }
  else
  {
    printF("ReparameterizationTransform:ERROR: scaleBounds: this is not a restriction mapping!\n");
    return 1;
  }
}
int ReparameterizationTransform::
getBounds(real & ra, real & rb, real & sa, real & sb, real & ta, real & tb ) const
// ==========================================================================================
/// \details 
///   Get the bounds for a restriction mapping.
///    {\tt RestrictionMapping} for further details.
/// \param ra,rb,sa,sb,ta,tb (output): 
// ==========================================================================================
{
  if( reparameterizationType==restriction )
  {
    assert(reparameterize!=NULL);
    RestrictionMapping & rm = (RestrictionMapping &) *reparameterize;
    real ra1,rb1,sa1,sb1,ta1,tb1;
    int returnValue=rm.getBounds( ra1,rb1,sa1,sb1,ta1,tb1 );

    real dr=mr[1]-mr[0],ds=mr[3]-mr[2],dt=mr[5]-mr[4];
    assert( dr!=0. && ds!=0. && dt!=0. );
    ra=(ra1-mr[0])/dr; rb=(rb1-mr[0])/dr; 
    sa=(sa1-mr[2])/ds; sb=(sb1-mr[2])/ds; 
    ta=(ta1-mr[4])/dt; tb=(tb1-mr[4])/dt;

    return returnValue;
  }
  else
  {
    printF("ReparameterizationTransform:ERROR: getBounds: this is not a restriction mapping!\n");
    return 1;
  }

}


int ReparameterizationTransform::
setBounds(const real ra, /* =0. */ 
	  const real rb, /* =1. */ 
	  const real sa, /* =0. */
	  const real sb, /* =1. */
	  const real ta, /* =0. */
	  const real tb  /* =1. */ )
// ==========================================================================================
/// \details 
///   Set absolute bounds. See the documentation for the
///    {\tt RestrictionMapping} for further details.
/// \param ra,rb,sa,sb,ta,tb (input): 
// ==========================================================================================
{
  mappingHasChanged();
  if( reparameterizationType==restriction )
  {
    assert(reparameterize!=NULL);
    RestrictionMapping & rm = (RestrictionMapping &) *reparameterize;
    // We may need to scale if we have composed restriction's
    real dr=mr[1]-mr[0],ds=mr[3]-mr[2],dt=mr[5]-mr[4];
    int rt=rm.setBounds( mr[0]+dr*ra,mr[0]+dr*rb,mr[2]+ds*sa,mr[2]+ds*sb,mr[4]+dt*ta,mr[4]+dt*tb );

    setMappingProperties(map2.mapPointer);
    return rt;

//  return rm.setBounds( ra,rb,sa,sb,ta,tb );
  }
  else
  {
    printF("ReparameterizationTransform:ERROR: setBounds: this is not a restriction mapping!\n");
    return 1;
  }
}

int ReparameterizationTransform::
getBoundsForMultipleReparameterizations(real & ra, real & rb, real & sa, real & sb, real & ta, real & tb ) const
// ==========================================================================================
// /Description:
//  Get the bounds for multiple reparameterizations. 
//  These are the scale factors to the original underlying mapping.
// 
// /ra,rb,sa,sb,ta,tb (output): 
//\end{ReparameterizationTransformInclude.tex}
// ==========================================================================================
{
  if( reparameterizationType==restriction )
  {
    assert(reparameterize!=NULL);
    RestrictionMapping & rm = (RestrictionMapping &) *reparameterize;
    return rm.getBounds(ra,rb,sa,sb,ta,tb);
  }
  else
  {
    ra=0.; rb=1.; sa=0.; sb=1.; ta=0.; tb=1.;
    return 0;
  }
}



int 
ReparameterizationTransform::
getBoundsForMultipleReparameterizations( real mrBounds[6] ) const
// ==========================================================================================
/// \details 
///   Get the bounds for multiple reparameterizations. This routine will usually only be
///  called by the Grid class.
/// \param mrBounds (output): 
// ==========================================================================================
{
  for( int i=0; i<6; i++ )
    mrBounds[i]=mr[i];
  return 0;
}



int 
ReparameterizationTransform::
setBoundsForMultipleReparameterizations( real mrBounds[6] )
// ==========================================================================================
/// \details 
///   Set the bounds for multiple reparameterizations. This routine will usually only be
///  called by the Grid class.
/// \param mrBounds (input): 
// ==========================================================================================
{
  for( int i=0; i<6; i++ )
    mr[i]=mrBounds[i];
  return 0;
}

RealArray ReparameterizationTransform::
getBoundingBox( const int & side /* =-1 */, const int & axis /* =-1 */ ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box for the Mapping (if side<0 and axis<0) or the bounding
//   box for a particular side.
//   /side, axis (input): indicates the side of the mapping, side=(0,1) (or side=(Start,End)) 
//     and axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
// =====================================================================================
{
  if( reparameterizationType==restriction && getMappingCoordinateSystem()==rectangular )
  { 
    real xa[3], xb[3];
    if( map2.getClassName()=="SquareMapping" )
    {
      SquareMapping & sq = (SquareMapping&)map2.getMapping();
      xa[2]=sq.getVertices(xa[0],xb[0], xa[1],xb[1] );
      xb[2]=xa[2];
    }
    else if( map2.getClassName()=="BoxMapping" )
    {
      BoxMapping & box = (BoxMapping&)map2.getMapping();
      box.getVertices(xa[0],xb[0], xa[1],xb[1], xa[2],xb[2] );
    }
    else
    {
      printF("ReparameterizationTransform::getBoundingBox:ERROR: mapping is Cartesian but unknown\n"
             "   className=%s\n",(const char*)map2.getClassName());
    }
    real rab[6];
    // RestrictionMapping & rm = (RestrictionMapping &)*reparameterize;
    getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );

    RealArray bb(2,3);
    for( int dir=0; dir<3; dir++ )
    { 
      real xa0=xa[dir];
      real xba=xb[dir]-xa[dir];
      bb(0,dir)=xa0 + rab[0+2*(dir)]*xba;
      bb(1,dir)=xa0 + rab[1+2*(dir)]*xba;
    }  
    if( side<0 && axis<0 )
    {
      return bb;
    }
  
    if( !validSide( side ) || !validAxis( axis ) )
    {
      printF(" ReparameterizationTransform::getBoundingBox:ERROR: Invalid arguments\n");
      Overture::abort("error");
    }

    bb(0,axis)=bb(side,axis);
    bb(1,axis)=bb(side,axis);
    return bb;
  }
  else
  {
    return Mapping::getBoundingBox(side,axis);
  }
  
}

int ReparameterizationTransform::
getBoundingBox( const IntegerArray & indexRange, const IntegerArray & gridIndexRange_,
                RealArray & xBounds, bool local /* =false */ ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box, xBounds, for the set of grid points spanned by 
//   indexRange. 
//
// /indexRange(0:1,0:2) (input) : range of indicies, i\_m=indexRange(0,m),...,indexRange(1,m)
// /gridIndexRange\_(0:1,0:2) (input) : Normally these should match the gridIndexRange of the Mapping.
//    This argument is used to double check that this is true.
// /xBounds(0:1,0:2) : bounds
// /local (input) : if local=true then only compute the min and max over points on this processor, otherwise
//                  compute the min and max over all points on all processors
//
// /Return values: 0=success, 1=indexRange values are invalid.
// =====================================================================================
{
  if( reparameterizationType==restriction && getMappingCoordinateSystem()==rectangular )
  {
    real xa[3], xb[3];
    if( map2.getClassName()=="SquareMapping" )
    {
      SquareMapping & sq = (SquareMapping&)map2.getMapping();
      xa[2]=sq.getVertices(xa[0],xb[0], xa[1],xb[1] );
      xb[2]=xa[2];
    }
    else if( map2.getClassName()=="BoxMapping" )
    {
      BoxMapping & box = (BoxMapping&)map2.getMapping();
      box.getVertices(xa[0],xb[0], xa[1],xb[1], xa[2],xb[2] );
    }
    else
    {
      printF("ReparameterizationTransform::getBoundingBox:ERROR: mapping is Cartesian but unknown\n"
             "   className=%s\n",(const char*)map2.getClassName());
    }
    real rab[6];
    // RestrictionMapping & rm = (RestrictionMapping &)*reparameterize;
    // getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );
    getBoundsForMultipleReparameterizations(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );

    if( false )
    {
      printF("+++ReparameterizationTransform::getBoundingBox: rab=[%g,%g][%g,%g][%g,%g]\n"
	     "     xab=[%g,%g][%g,%g][%g,%g]\n"
             " indexRange=[%i,%i][%i,%i][%i,%i]\n"
             " gridIndexRange=[%i,%i][%i,%i][%i,%i]\n",
	     rab[0],rab[1], rab[2],rab[3], rab[4],rab[5],
	     xa[0],xb[0],xa[1],xb[1],xa[2],xb[2],
             indexRange(0,0),indexRange(1,0),
             indexRange(0,1),indexRange(1,1),
             indexRange(0,2),indexRange(1,2),
             gridIndexRange(0,0),gridIndexRange(1,0),
             gridIndexRange(0,1),gridIndexRange(1,1),
             gridIndexRange(0,2),gridIndexRange(1,2)
                );
    }
    
    for( int dir=0; dir<3; dir++ )
    { 
      real xa0=xa[dir];
      real xba=xb[dir]-xa[dir];
      xa[dir]= xa0 + rab[0+2*(dir)]*xba;
      xb[dir]= xa0 + rab[1+2*(dir)]*xba;
    } 

    for( int dir=0; dir<3; dir++ )
    {
      real dx = (xb[dir]-xa[dir])/max(1,gridIndexRange_(1,dir)-gridIndexRange_(0,dir));
      xBounds(0,dir)=xa[dir]+ (indexRange(0,dir)-gridIndexRange_(0,dir))*dx;
      xBounds(1,dir)=xb[dir]+ (indexRange(1,dir)-gridIndexRange_(1,dir))*dx;
    }  
    return 0;
  }
  else
  {
    return Mapping::getBoundingBox(indexRange,gridIndexRange_,xBounds,local);
  }
  
}


int ReparameterizationTransform::
getBoundingBox( const RealArray & rBounds, RealArray & xBounds ) const
// =====================================================================================
// 
// Specialized version. Supplying this functions means we can avoid building the grid in
// order to determine the bounding box for intersect etc.
// 
// /Description:
//   Return the bounding box, xBounds, for the range space that corresponds to the
//   bounding box, rBounds, in the domain space. 
// =====================================================================================
{
  return Mapping::getBoundingBox(rBounds,xBounds);
}



int ReparameterizationTransform::
setEquidistributionParameters(const real & arcLengthWeight_ /* =1.*/, 
			      const real & curvatureWeight_ /* =0.*/,
                              const int & numberOfSmooths /* = 3 */ )
//=====================================================================================
/// \details 
///    Set the `arclength' parameterization parameters. The parameterization is chosen to
///  redistribute the points to resolve the arclength and/or the curvature of the curve.
///  By default the curve is parameterized by arclength only. To resolve regions of high
///  curvature choose the recommended values of {\tt arcLengthWeight\_=1.} and
///   {\tt curvatureWeight\_=1.}.
/// 
///   To determine the parameterization we equidistribute the weight function 
///   \[
///      w(r) =      {\rm arcLengthWeight} {s(r)\over |s|_\infty}  
///                + {\rm curvatureWeight} {c(r)\over |c|_\infty}
///   \]
///   where $s(r)$ is the local arclength and $c(r)$ is the curvature. Note that we normalize
///  $s$ and $c$ by their maximum values.
///   
///  \[
///       c = |x_{ss}| = { | x_{rr} | \over  |x_r|^2 }
///  \]
///  
/// \param arcLengthWeight_ (input): A weight for arclength. A negative value may give undefined results.
/// \param curvatureWeight_ (input): A weight for curvature. A negative value may give undefined results.
/// \param numberOfSmooths (input): Number of times to smooth the equidistribution weight function.
/// 
//=====================================================================================
{
  equidistributionInitialized=false;

  arcLengthWeight=arcLengthWeight_;
  curvatureWeight=curvatureWeight_;
  numberOfEquidistributionSmooths=numberOfSmooths;
  
  return 0;
}


int ReparameterizationTransform::
initializeEquidistribution(const bool & useOriginalMapping /* =true */)
// ===========================================================================================
//  /Description:
//    Determine the mapping that will equidistribute the specified combination or arclength 
//  and curvature.
// /useOriginalMapping (input): If true, base equidistribution on the grid points from the
//   orignal mapping to be reparameterized. Otherwise use the grid points from the 
//   current reparameterized mapping. These will not in general give the same results.
// ===========================================================================================
{
  equidistributionInitialized=true;
  
  // compute the parameterization based on arclength and curvature
  assert( domainDimension==1 );
  
  
  const int n=useOriginalMapping ? map2.getGridDimensions(axis1)-1 : getGridDimensions(axis1)-1;
  realArray s(n+1), weight(n+1), curvature(n+1);
  Range R(1,n-1);

  const realArray & x = useOriginalMapping ? map2.getMapping().getGrid() : getGrid();

  s(0)=0.;
  if( rangeDimension==2 )
  {
    for( int i=1; i<=n; i++ )
      s(i)=s(i-1)+SQRT( SQR(x(i,0,0,axis1)-x(i-1,0,0,axis1))+
                        SQR(x(i,0,0,axis2)-x(i-1,0,0,axis2)) );
  }
  else
  {
    for( int i=1; i<=n; i++ )
      s(i)=s(i-1)+SQRT( SQR(x(i,0,0,axis1)-x(i-1,0,0,axis1))+
                        SQR(x(i,0,0,axis2)-x(i-1,0,0,axis2))+
                        SQR(x(i,0,0,axis3)-x(i-1,0,0,axis3)) );
  }
  
  s*=1./s(n);

  weight(R)=s(R+1)-s(R-1);  // 2 times the delta arclength

  if( getIsPeriodic(axis1)==functionPeriodic )
  {
    weight(0)=s(1)-s(0)+s(n)-s(n-1);
    weight(n)=weight(0);
  }
  else
  { // use one sided approx.
    weight(0)=s(2)-s(0);
    weight(n)=s(n)-s(n-2);
  }

  real minWeight=min(weight);
//  printf("min(weight)=%e, max(weight)=%e \n",minWeight,max(weight));
  if( minWeight<=0. )
  {
    printf("ReparameterizationTransform::initializeEquidistribution:ERROR: min(weight)=%e <=0 !!\n",minWeight);
    real maxWeight=max(weight);
    if( maxWeight>0. )
    {
      printf("  ...Setting minimum value to max(weight)/10. =%e \n",maxWeight*.1);
      weight=max(maxWeight*.1,weight);
    }
    else
    {
      printf("  ...ERROR: max(weight)=%e <=0 !!\n",maxWeight); 
      printf("  ...Setting weight==1 and continuing\n");
      weight=1.;
    }
  }
  if( rangeDimension==2 )
  {
    curvature(R)=(fabs(x(R+1,0,0,0)-2.*x(R,0,0,0)+x(R-1,0,0,0))+
		  fabs(x(R+1,0,0,1)-2.*x(R,0,0,1)+x(R-1,0,0,1)))/SQR(weight(R));
    if( getIsPeriodic(axis1)==functionPeriodic )
    {
      curvature(0)=(fabs(x(1,0,0,0)-2.*x(0,0,0,0)+x(n-1,0,0,0))+
		    fabs(x(1,0,0,1)-2.*x(0,0,0,1)+x(n-1,0,0,1)))/SQR(weight(0));
      curvature(n)=curvature(0);
    }
    else
    { // use one sided approx.
      curvature(0)=curvature(1);
      curvature(n)=curvature(n-1);
    }
  }
  else
  {
    curvature(R)=(fabs(x(R+1,0,0,0)-2.*x(R,0,0,0)+x(R-1,0,0,0))+
		  fabs(x(R+1,0,0,1)-2.*x(R,0,0,1)+x(R-1,0,0,1))+
		  fabs(x(R+1,0,0,2)-2.*x(R,0,0,2)+x(R-1,0,0,2)))/SQR(weight(R));
    if( getIsPeriodic(axis1)==functionPeriodic )
    {
      curvature(0)=(fabs(x(1,0,0,0)-2.*x(0,0,0,0)+x(n-1,0,0,0))+
		    fabs(x(1,0,0,1)-2.*x(0,0,0,1)+x(n-1,0,0,1))+
		    fabs(x(1,0,0,2)-2.*x(0,0,0,2)+x(n-1,0,0,2)))/SQR(weight(0));
      curvature(n)=curvature(0);
    }
    else
    { // use one sided approx.
      curvature(0)=curvature(1);
      curvature(n)=curvature(n-1);
    }
  }
  // ::display(x,"Here is the grid points x");
  //   ::display(s,"Here is the arclength s");
  //   ::display(weight,"Here is the arclength weight");
  //   ::display(curvature,"Here is the curvature weight ");


  real sMax=max(weight);
  real cMax=max(curvature);
  cMax=max(sMax*REAL_EPSILON,cMax);

  // weight=weight*(arcLengthWeight/sMax)+curvature*(curvatureWeight/cMax);

  weight=weight*( (arcLengthWeight/sMax)+curvature*(curvatureWeight/(sMax*cMax)) );

  // smooth the weight function
  const real omega=.5;
  for( int it=0; it<numberOfEquidistributionSmooths; it++ )
  {
    weight(R)=(1.-omega)*weight(R) + omega*.5*(weight(R+1)+weight(R-1));
    if( getIsPeriodic(axis1) )
    {
      weight(0)=(1.-omega)*weight(0) + omega*.5*(weight(1)+weight(n-1));
      weight(n)=weight(0);
    }
  }
  
  if( debug & 4 ) ::display(weight,"Here is the weight function","%8.1e");
	
  equidistribute(weight,s);

  if( debug & 4 ) ::display(s,"Here is the equidistributed parameter","%9.2e");

  SplineMapping & equiSpline = (SplineMapping &) *reparameterize;
  if( useOriginalMapping )
  {
    equiSpline.setPoints(s);
  }
  else
  {
    // In this case we need to evaluate the current spline to get the correct positions.
    realArray ss(n+1);
    ss=s;
    equiSpline.map(s,ss);
    equiSpline.setPoints(ss);
    printf(" max(weight)=%g, min(weight)=%g \n",max(weight),min(weight));
  }
  if( false )
  { // s should converge to be equaly spaced.
    ::display(weight,"Here is weight  after smoothing");
    ::display(s,"Here is s ");
  }
    
  if( false )
  {
    realArray r(n+1);
    r.seqAdd(0.,1./n);
    equiSpline.inverseMap(r,s);
    ::display(s,"Here is s after inverting the equidistributed parameter");
  }
  
  mappingHasChanged();
  return 0;
}

void ReparameterizationTransform::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params_ )
{
  // Evaluate the mapping derivatives in spherical/cylindrical coordinates
  if( reparameterizationType==equidistribution && !equidistributionInitialized )
    initializeEquidistribution();

  // we don't want to change the default parameters so make a new one if necessary
  // We keep a stack of localParams[] in case the ReparameterizationTransform is called recursively
  int lp=0;
  while( lp<maximumNumberOfRecursionLevels && localParamsAreBeingUsed[lp] )
  {
    lp++;
  }
  if( lp==maximumNumberOfRecursionLevels )
  {
    printf("ReparameterizationTransform::ERROR: too many levels of recursion lp=%i."
           "Get Bill to fix this.\n",lp);
    Overture::abort();
  }
  
  localParamsAreBeingUsed[lp]=1;
  if( params_.isNull && localParams[lp]==NULL )
    localParams[lp] = new MappingParameters;
  
  MappingParameters & params = params_.isNull ? *localParams[lp] : params_;
  params.coordinateType=coordinateType;
  ComposeMapping::map( r,x,xr,params );
  params.coordinateType=cartesian;   // reset
  localParamsAreBeingUsed[lp]=0;     // reset
  
}    

void ReparameterizationTransform::
inverseMap( const realArray & x, realArray & r, realArray & rx,  MappingParameters & params )
{
  if( useDefaultInverse )
  {
    // use default inverse from base class (this is NOT the default and probably slower)
    Mapping::inverseMap(x,r,rx,params );
    return;
  }

  // be careful not to change params if it is the static variable: Overture::nullMappingParameters()
  // We keep a stack of localParams[] in case the ReparameterizationTransform is called recursively
  int lp=0;
  while( lp<maximumNumberOfRecursionLevels && localParamsAreBeingUsed[lp] )
  {
    lp++;
  }
  if( lp==maximumNumberOfRecursionLevels )
  {
    printf("ReparameterizationTransform::ERROR: too many levels of recursion lp=%i."
           "Get Bill to fix this.\n",lp);
    Overture::abort();
  }

  localParamsAreBeingUsed[lp]=1;
  if( params.isNull && localParams[lp]==NULL )
    localParams[lp] = new MappingParameters;

  MappingParameters & params0 = params.isNull ? *localParams[lp] : params; 
  const coordinateSystem coordinateTypeSave=(coordinateSystem)params0.coordinateType;

  params0.coordinateType=coordinateType;
  if( coordinateType==cylindrical )
  {
    // If the mapping is in cylindrical coordinates and doesn't have a basicInverse then
    // we must call the base class inverseMap because the ComposeMapping::inverseMap will 
    // call the inverse for the cylindrical map and Netwon won't converge at the singularity
    Mapping::inverseMap( x,r,rx,params0 );       
  }    
  else // ComposeMapping::inverseMap will work ok for a spherical singularity 
  {
    ComposeMapping::inverseMap( x,r,rx,params0 );
  }
  
  params0.coordinateType=coordinateTypeSave;  // reset
  localParamsAreBeingUsed[lp]=0;     // reset
}
 
void ReparameterizationTransform::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
// *wdh* 060415 -- we need a basicInverse to overload ComposeMapping::bascInverse so that we can
//      set the coordinateType (for evaluation of OrthographicTransform for e.g.)
{
  // be careful not to change params if it is the static variable: Overture::nullMappingParameters()
  // We keep a stack of localParams[] in case the ReparameterizationTransform is called recursively
  int lp=0;
  while( lp<maximumNumberOfRecursionLevels && localParamsAreBeingUsed[lp] )
  {
    lp++;
  }
  if( lp==maximumNumberOfRecursionLevels )
  {
    printf("ReparameterizationTransform::ERROR: too many levels of recursion lp=%i."
           "Get Bill to fix this.\n",lp);
    Overture::abort();
  }

  localParamsAreBeingUsed[lp]=1;
  if( params.isNull && localParams[lp]==NULL )
    localParams[lp] = new MappingParameters;

  MappingParameters & params0 = params.isNull ? *localParams[lp] : params; 
  const coordinateSystem coordinateTypeSave=(coordinateSystem)params0.coordinateType;
  
  params0.coordinateType=coordinateType;


  ComposeMapping::basicInverse( x,r,rx,params0 );

  params0.coordinateType=coordinateTypeSave;  // reset
  localParamsAreBeingUsed[lp]=0;     // reset

}



void ReparameterizationTransform::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params_ )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  // Evaluate the mapping derivatives in spherical/cylindrical coordinates
  if( reparameterizationType==equidistribution && !equidistributionInitialized )
    initializeEquidistribution();

  // we don't want to change the default parameters so make a new one if necessary
  // We keep a stack of localParams[] in case the ReparameterizationTransform is called recursively
  int lp=0;
  while( lp<maximumNumberOfRecursionLevels && localParamsAreBeingUsed[lp] )
  {
    lp++;
  }
  if( lp==maximumNumberOfRecursionLevels )
  {
    printf("ReparameterizationTransform::ERROR: too many levels of recursion lp=%i."
           "Get Bill to fix this.\n",lp);
    Overture::abort();
  }
  
  localParamsAreBeingUsed[lp]=1;
  if( params_.isNull && localParams[lp]==NULL )
    localParams[lp] = new MappingParameters;
  
  MappingParameters & params = params_.isNull ? *localParams[lp] : params_;
  params.coordinateType=coordinateType;
  ComposeMapping::mapS( r,x,xr,params );
  params.coordinateType=cartesian;   // reset
  localParamsAreBeingUsed[lp]=0;     // reset
  
}    

void ReparameterizationTransform::
inverseMapS( const RealArray & x, RealArray & r, RealArray & rx,  MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  if( useDefaultInverse )
  {
    // use default inverse from base class (this is NOT the default and probably slower)
    Mapping::inverseMapS(x,r,rx,params );
    return;
  }

  // be careful not to change params if it is the static variable: Overture::nullMappingParameters()
  // We keep a stack of localParams[] in case the ReparameterizationTransform is called recursively
  int lp=0;
  while( lp<maximumNumberOfRecursionLevels && localParamsAreBeingUsed[lp] )
  {
    lp++;
  }
  if( lp==maximumNumberOfRecursionLevels )
  {
    printf("ReparameterizationTransform::ERROR: too many levels of recursion lp=%i."
           "Get Bill to fix this.\n",lp);
    Overture::abort();
  }

  localParamsAreBeingUsed[lp]=1;
  if( params.isNull && localParams[lp]==NULL )
    localParams[lp] = new MappingParameters;

  MappingParameters & params0 = params.isNull ? *localParams[lp] : params; 
  const coordinateSystem coordinateTypeSave=(coordinateSystem)params0.coordinateType;

  params0.coordinateType=coordinateType;
  if( coordinateType==cylindrical )
  {
    // If the mapping is in cylindrical coordinates and doesn't have a basicInverse then
    // we must call the base class inverseMap because the ComposeMapping::inverseMap will 
    // call the inverse for the cylindrical map and Netwon won't converge at the singularity
    Mapping::inverseMapS( x,r,rx,params0 );       
  }    
  else // ComposeMapping::inverseMap will work ok for a spherical singularity 
  {
    ComposeMapping::inverseMapS( x,r,rx,params0 );
  }
  
  params0.coordinateType=coordinateTypeSave;  // reset
  localParamsAreBeingUsed[lp]=0;     // reset
}
 
void ReparameterizationTransform::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
// *wdh* 060415 -- we need a basicInverse to overload ComposeMapping::bascInverse so that we can
//      set the coordinateType (for evaluation of OrthographicTransform for e.g.)
{
  // be careful not to change params if it is the static variable: Overture::nullMappingParameters()
  // We keep a stack of localParams[] in case the ReparameterizationTransform is called recursively
  int lp=0;
  while( lp<maximumNumberOfRecursionLevels && localParamsAreBeingUsed[lp] )
  {
    lp++;
  }
  if( lp==maximumNumberOfRecursionLevels )
  {
    printf("ReparameterizationTransform::ERROR: too many levels of recursion lp=%i."
           "Get Bill to fix this.\n",lp);
    Overture::abort();
  }

  localParamsAreBeingUsed[lp]=1;
  if( params.isNull && localParams[lp]==NULL )
    localParams[lp] = new MappingParameters;

  MappingParameters & params0 = params.isNull ? *localParams[lp] : params; 
  const coordinateSystem coordinateTypeSave=(coordinateSystem)params0.coordinateType;
  
  params0.coordinateType=coordinateType;


  ComposeMapping::basicInverseS( x,r,rx,params0 );

  params0.coordinateType=coordinateTypeSave;  // reset
  localParamsAreBeingUsed[lp]=0;     // reset

}






//=================================================================================
// get a mapping from the database
//=================================================================================
int ReparameterizationTransform::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  subDir.setMode(GenericDataBase::streamInputMode);
  if( debug & 4 )
    printF("Entering ReparameterizationTransform::get\n");

  subDir.get( ReparameterizationTransform::className,"className" ); 
  if( ReparameterizationTransform::className != "ReparameterizationTransform" )
  {
    printF("ReparameterizationTransform::get ERROR in className!\n");
  }
  subDir.get(coordinateType,"coordinateType");
  int temp;
  subDir.get(temp,"reparameterizationType"); reparameterizationType=(ReparameterizationTypes&)temp;
  subDir.get( arcLengthWeight,"arcLengthWeight");
  subDir.get( curvatureWeight,"curvatureWeight");
  subDir.get( numberOfEquidistributionSmooths,"numberOfEquidistributionSmooths" );
  subDir.get( equidistributionInitialized,"equidistributionInitialized");
  subDir.get( mr,"mr",6);

  ComposeMapping::get(subDir,"ComposeMapping"); 
  reparameterize=map1.mapPointer;     
  mappingHasChanged();
  delete &subDir;
  return 0;
}
int ReparameterizationTransform::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( ReparameterizationTransform::className,"className" );
  subDir.put(coordinateType,"coordinateType");
  subDir.put((int)reparameterizationType,"reparameterizationType");
  subDir.put( arcLengthWeight,"arcLengthWeight");
  subDir.put( curvatureWeight,"curvatureWeight");
  subDir.put( numberOfEquidistributionSmooths,"numberOfEquidistributionSmooths" );
  subDir.put( equidistributionInitialized,"equidistributionInitialized");
  subDir.put( mr,"mr",6);

  ComposeMapping::put( subDir,"ComposeMapping" );
  delete &subDir;
  return 0;
}

Mapping *ReparameterizationTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==ReparameterizationTransform::className )
    retval = new ReparameterizationTransform();
  return retval;
}

    
void ReparameterizationTransform::
setMappingProperties( Mapping *mapPointer )
// =================================================================================================
// /Description:
// Set properties of the mapping.
// =================================================================================================
{
  if( reparameterizationType==orthographic )
  {
    int tAxis=axis2;  // theta axis
    if( mapPointer->getIsPeriodic(axis2) )
      tAxis=axis2;
    else if(  mapPointer->getIsPeriodic(axis1) )
      tAxis=axis1;
    else
    {
      printF("ReparameterizationTransform:setMappingProperties:ERROR: The orthographic transforms expects the\n"
	     " mapping to be periodic along axis1 or axis2, but it is not so ! \n");
    }
    const int sAxis=1-tAxis;  // phi axis
    ((OrthographicTransform*)reparameterize)->setAngularAxis(tAxis);
    // The reparameterized sphere or cylinder is no longer periodic in the axis2 direction
    setBoundaryCondition(Start,sAxis,0);
    setBoundaryCondition(End  ,sAxis,0);
    if( true ) // || getIsPeriodic(tAxis) )
    {
      setIsPeriodic(tAxis,notPeriodic);
      setBoundaryCondition(Start,tAxis,0);
      setBoundaryCondition(End  ,tAxis,0);
    }
    // The reparameterized sphere or cylinder no longer has a coordinate singularity
    for( int side=Start; side<=End; side++ )
    {
      if( mapPointer->getTypeOfCoordinateSingularity(side,sAxis)==polarSingularity )
        setTypeOfCoordinateSingularity(side,sAxis,noCoordinateSingularity);
    }
    if( mapPointer->getCoordinateEvaluationType(spherical) )
      coordinateType=spherical;
    else if( mapPointer->getCoordinateEvaluationType(cylindrical) )
      coordinateType=cylindrical;
    else
    {
      coordinateType=cartesian;
      printF("ReparameterizationTransform:setMappingProperties:ERROR: The given mapping can not be evaluated\n"
	     " in spherical or cylindrical coordinates! \n");
    }
  }
  else if( reparameterizationType==restriction )
  { 
    coordinateType=cartesian;
     // For a restriction mapping the new mapping is cartesian if the original one is (for AMR)
    setMappingCoordinateSystem(mapPointer->getMappingCoordinateSystem()); // *wdh* 020220

    RestrictionMapping & restrictMap = (RestrictionMapping &)(*reparameterize);
    RealArray abrs(2,3); 
    abrs(0,0)=restrictMap.ra; abrs(1,0)=restrictMap.rb;
    abrs(0,1)=restrictMap.sa; abrs(1,1)=restrictMap.sb;
    abrs(0,2)=restrictMap.ta; abrs(1,2)=restrictMap.tb;
    
    int axis;
    for( axis=0; axis<domainDimension; axis++ )
    {
      // *wdh* 001026 -- fixed some problems with periodic boundaries

      if( mapPointer->getIsPeriodic(axis)==functionPeriodic )
        restrictMap.setSpaceIsPeriodic(axis);            // *wdh* 001105

      if( abrs(0,axis)==0. && abrs(1,axis)==1. && mapPointer->getIsPeriodic(axis)==functionPeriodic )
	setIsPeriodic(axis,functionPeriodic);
      else
      {
	setIsPeriodic(axis,notPeriodic);
      }
      
      // if the this mapping is periodic, the reparameterize mapping should have derivativePeriodic
      if( getIsPeriodic(axis) )
        reparameterize->setIsPeriodic(axis,derivativePeriodic);
      else
        reparameterize->setIsPeriodic(axis,notPeriodic);
    }
    
    for( axis=0; axis<domainDimension; axis++ )
    {
      int numGridPoints=mapPointer->getGridDimensions(axis);
      if( true ) // *wdh* 070403  -- turn this off -- check this 
      {
	setGridDimensions(axis,int((numGridPoints-1)*(abrs(1,axis)-abrs(0,axis))+1+.5));
      }
      else  // *wdh* 070403 -- set the gridIndexRange to use non-zero base
      {
	int newNumberOfGridPoints = int((numGridPoints-1)*(abrs(1,axis)-abrs(0,axis))+1+.5);
      
	// *wdh* 070403 -- set the gridIndexRange to use non-zero base
	int ma = mapPointer->getGridIndexRange(0,axis);
	int mb = mapPointer->getGridIndexRange(1,axis);
      
	int na = int( ma + (numGridPoints-1)*abrs(0,axis) + .5 );
	int nb = na + newNumberOfGridPoints -1;

	setGridIndexRange(0,axis,na);
	setGridIndexRange(1,axis,nb);
      }
      
      for( int side=Start; side<=End; side++ )
	if( mapPointer->getTypeOfCoordinateSingularity(side,axis)==polarSingularity
	   && abrs(side,axis)==side )
	  setTypeOfCoordinateSingularity(side,axis,polarSingularity);
	else
	  setTypeOfCoordinateSingularity(side,axis,noCoordinateSingularity);
    }
  }
  else if( reparameterizationType==equidistribution )
  {
    coordinateType=cartesian;
    if( map2.getIsPeriodic(axis1) )
    {
      reparameterize->setIsPeriodic(axis1,derivativePeriodic);
      setIsPeriodic(axis1,functionPeriodic);
    }
  }
  else if( reparameterizationType==reorientDomainCoordinates )
  {
    coordinateType=cartesian;
    // *wdh* no: For a this transformation the new mapping is rectangular if the original one is (for AMR)
    // setMappingCoordinateSystem(mapPointer->getMappingCoordinateSystem()); 
    // *wdh* 050613 -- use a general coordinate system since the rectangular case assumes x=xa+r*(xb-xa) 
    setMappingCoordinateSystem(general);
    

    int axis;
    ReorientMapping & reorient = (ReorientMapping &)*reparameterize;
    int dir1,dir2,dir3;
    reorient.getOrientation(dir1,dir2,dir3);
    const int newAxis[3]={dir1,dir2,dir3}; //
    for( axis=0; axis<domainDimension; axis++ )
    {
      setGridDimensions(newAxis[axis],mapPointer->getGridDimensions(axis));
      for( int side=Start; side<=End; side++ )
        setBoundaryCondition(side,newAxis[axis],mapPointer->getBoundaryCondition(side,axis));
      setIsPeriodic(newAxis[axis],mapPointer->getIsPeriodic(axis));
    }
    
    for( axis=0; axis<domainDimension; axis++ )
    {
      for( int side=Start; side<=End; side++ )
        setTypeOfCoordinateSingularity(side,newAxis[axis],mapPointer->getTypeOfCoordinateSingularity(side,axis));
    }
  }
  else
  {
    printf(" ReparameterizationTransform::setMappingProperties:ERROR: unknown reparameterizationType\n");
    {throw "error";}
  }
  

  mappingHasChanged();
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int ReparameterizationTransform::
update( MappingInformation & mapInfo ) 
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!ReparameterizationTransform",
      "transform which mapping?",
      "orthographic",
      "restrict parameter space",
      ">restriction parameters",
        "set corners",
      "<equidistribution",
      ">equidistribution parameters",
        "arclength weight",
        "curvature weight",
        "number of smooths",
        "re-evaluate equidistribution",
      "<reorient domain coordinates",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "use robust inverse",
      "do not use robust inverse",
      "check inverse",
      "check",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "    Reparameterize a Mapping in Various Possible Ways",
      "transform which mapping? : choose the mapping to transform",
      "orthographic       : reomve a spherical polar singularity with an orthographic transform",
      "restrict parameter space: restrict to a sub-rectangle",
      "set corners        : set unit square bounds for a restriction",
      "arclength weight : weighting factor for arclength in computing the parameterization",
      "curvature weight : weighting factor for curvature in computing the parameterization",
      " ",
      "reset              : reset the transformation to be the identity",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 
  bool plotObject=true;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  bool mappingChosen= reparameterize!=NULL;
  Mapping *mapPointer;
  int pole=1;

  // By default transform the last mapping in the list (if this mapping is unitialized, mappingChosen==false)
  // and if the Mapping is not already a ReparameterizationTransform
  if( !mappingChosen )
  {
    mappingHasChanged();
    int number= mapInfo.mappingList.getLength();
    if( number > 0 ) 
    {
      for( int i=number-1; i>=0; i-- )
      {
        mapPointer=mapInfo.mappingList[i].mapPointer;
        if( mapPointer->getClassName() != "ReparameterizationTransform" )
          break;  // use this one
      }
      reparameterizationType=restriction;
      // choose orthographic if the mapping has a singularity 
      for( int axis=0; axis<mapPointer->getDomainDimension(); axis++ )
	for( int side=Start; side<=End; side++ )
	  if( mapPointer->getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
	  {
	    pole=2*side-1;   // choose north pole, pole==1 by default
	    reparameterizationType=orthographic;
	  }

      printF("ReparameterizationTransfrom:reparameterize mapping %s with %s by default\n",
	     (const char*)mapPointer->getName(mappingName),(reparameterizationType==restriction ?
							    "restriction" : "orthographic"));
      
    }
    else
    {
      printF("ReparameterizationTransfrom:ERROR: no mappings to transform!! \n");
      return 1;
    }
  }
  else
  {
    mapPointer=map2.mapPointer;
    assert( mapPointer!=NULL );
  }
  

  gi.appendToTheDefaultPrompt("ReparameterizationTransform>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 
    if( answer=="transform which mapping?" )
    { 
      reparameterizationType=restriction;  // set default *wdh* 070614

      // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+1];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(mappingName);
      menu2[num]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      // Here is the mapping to be rotated/scaled etc.
      if( mapInfo.mappingList[mapNumber].mapPointer==this )
      {
	printF("ReparameterizationTransform::ERROR: you cannot transform this mapping, this would be recursive!\n");
        continue;
      }
      else
        mapPointer=mapInfo.mappingList[mapNumber].mapPointer;

      // eliminate multiple levels of reparamterizations *****************************
      //  if( mapPointer->getClassName() != "ReparameterizationTransform" )
      //    break;  // use this one

      mappingHasChanged();
    }
    
    if( !mappingChosen || answer=="transform which mapping?")
    {
      constructor(*mapPointer,reparameterizationType);

      setBounds(0.,1.,0.,1.,0.,1. );  // is this needed for multiple reparams?
      
//       if( reparameterize==NULL )
//       {
// 	if( reparameterizationType==orthographic )
// 	{
//   	  reparameterize=new OrthographicTransform();
//           ((OrthographicTransform*)reparameterize)->setPole(pole==1 ? OrthographicTransform::northPole :
// 							    OrthographicTransform::southPole );
// 	}
//         else if( reparameterizationType==restriction )
// 	  reparameterize=new RestrictionMapping();
// 	else
// 	{
// 	  reparameterize=new SplineMapping();
//           reparameterize->setRangeSpace(parameterSpace);
// 	}
	
//         reparameterize->incrementReferenceCount();  // this says mapping was newed
//         reparameterize->setDomainDimension(mapPointer->getDomainDimension()); 
//         reparameterize->setRangeDimension(mapPointer->getDomainDimension());
//         setMappings(*reparameterize,*mapPointer);
//         reparameterize->decrementReferenceCount();
//       }
//       else
//       {
//         // set dimensions of reparameterization mapping to match
//         reparameterize->setDomainDimension(mapPointer->getDomainDimension()); 
//         reparameterize->setRangeDimension(mapPointer->getDomainDimension());
//         // compose the mappings:
//         setMappings(*reparameterize,*mapPointer);
//         // setName(mappingName,aString("reparameterized-")+mapPointer->getName(mappingName));
//       }
      
//       setMappingProperties(mapPointer);

//       equidistributionInitialized=false;
//      mappingHasChanged();
      mappingChosen=true;
      plotObject=true;
    }
    else if( answer=="reorient domain coordinates" )
    {
      int dir1=0, dir2=1, dir3=2;
      if( reparameterizationType==reorientDomainCoordinates && reparameterize!=NULL )
      {
        ((ReorientMapping*) reparameterize)->getOrientation(dir1,dir2,dir3);
      }
      
      if( domainDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter dir1,dir2 (current=(%i,%i), permutation of (0,1)): ",dir1,dir2));
        if( line!="" ) sScanF(line,"%i %i",&dir1,&dir2);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter dir1,dir2,dir3 (current=(%i,%i,%i), permutation of (0,1,2)): ",
              dir1,dir2,dir3));
        if( line!="" ) sScanF(line,"%i %i %i",&dir1,&dir2,&dir3);
      }

      if( reparameterizationType!=reorientDomainCoordinates )
      {
        reparameterizationType=reorientDomainCoordinates;
	reparameterize=new ReorientMapping();
        reparameterize->incrementReferenceCount();

        reparameterize->setDomainDimension(mapPointer->getDomainDimension()); 
        reparameterize->setRangeDimension(mapPointer->getDomainDimension());

        setMappings(*reparameterize,*mapPointer);
        reparameterize->decrementReferenceCount();
      }
      ((ReorientMapping*) reparameterize)->setOrientation(dir1,dir2,dir3);
      setMappingProperties(mapPointer);

      mappingHasChanged();
    }
    else if( answer=="orthographic" ) 
    { // choose orthographic and change parameters
      if( reparameterizationType!=orthographic )
      {
        // not needed: setMappings will delete as appropriate!
        // if( reparameterize!=0 && reparameterize->decrementReferenceCount()==0 )
  	//   delete reparameterize;
        reparameterizationType=orthographic;
	reparameterize=new OrthographicTransform();
        reparameterize->incrementReferenceCount();
        reparameterize->setDomainDimension(mapPointer->getDomainDimension()); 
        reparameterize->setRangeDimension(mapPointer->getDomainDimension());
        setMappings(*reparameterize,*mapPointer);
        reparameterize->decrementReferenceCount();
      }
      reparameterize->update(mapInfo);
      setMappingProperties(mapPointer);
      mappingHasChanged();
    }
    else if( answer=="restrict parameter space" ) 
    { // choose restriction and change parameters
      if( reparameterizationType!=restriction )
      {
        // not needed: setMappings will delete as appropriate!
        // if( reparameterize!=0 && reparameterize->decrementReferenceCount()==0 )
  	//   delete reparameterize;
        reparameterizationType=restriction;
	reparameterize=new RestrictionMapping();
        reparameterize->incrementReferenceCount();
        reparameterize->setDomainDimension(mapPointer->getDomainDimension()); 
        reparameterize->setRangeDimension(mapPointer->getRangeDimension());
        setMappings(*reparameterize,*mapPointer);
        reparameterize->decrementReferenceCount();
      }
      // *wdh* 070415 -- force user to use 'set corners' from this menu so it works for multiple reparams
      reparameterize->update(mapInfo); 
      setMappingProperties(mapPointer);
      mappingHasChanged();
    }
    else if( answer=="equidistribution" )
    {
      if( domainDimension!=1 )
      {
	printf("ReparameterizationTransfrom:ERROR: equidistribution only implemented for domainDimension==1\n");
      }
      else
      {
	if( reparameterizationType!=equidistribution )
	{
          // not needed: setMappings will delete as appropriate!
	  // if( reparameterize!=0 && reparameterize->decrementReferenceCount()==0 )
	  //  delete reparameterize;
	  reparameterizationType=equidistribution;
	  reparameterize=new SplineMapping();
	  reparameterize->incrementReferenceCount();
	  reparameterize->setDomainDimension(mapPointer->getDomainDimension()); 
	  reparameterize->setRangeDimension(mapPointer->getDomainDimension());
          reparameterize->setRangeSpace(parameterSpace);

	  setMappings(*reparameterize,*mapPointer);
	  reparameterize->decrementReferenceCount();
	}
	setMappingProperties(mapPointer);

	mappingHasChanged();
      }
    }
    else if( answer=="set corners" || answer=="specify corners" )
    {
      if( reparameterizationType==restriction )
      {
        real ra=0., rb=1., sa=0., sb=1., ta=0., tb=1.;
	if( domainDimension==1 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter ra,rb (default=(%e,%e)): ",ra,rb));
	  if( line!="" ) sScanF(line,"%e %e ",&ra,&rb);
	}
	else if( domainDimension==2 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter ra,rb, sa,rb (default=[%e,%e]x[%e,%e]): ",
				      ra,rb,sa,sb));
	  if( line!="" ) sScanF(line,"%e %e %e %e ",&ra,&rb,&sa,&sb);
	}
	else
	{
	  gi.inputString(line,sPrintF(buff,"Enter ra,rb, sa,sb, ta,tb (default=[%e,%e]x[%e,%e]x[%e,%e]): ",
				      ra,rb,sa,sb,ta,tb));
	  if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&ra,&rb,&sa,&sb,&ta,&tb);
	}

// 	real rab[6];
// 	getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );
//         RestrictionMapping & rm = (RestrictionMapping &)*reparameterize;

// 	printF("+++ReparameterizationTransform::set corners:BEFORE: getBounds=[%g,%g][%g,%g][%g,%g]\n"
// 	       " mr=[%g,%g][%g,%g][%g,%g], ",
// 	       rab[0],rab[1], rab[2],rab[3], rab[4],rab[5],
//                mr[0],mr[1], mr[2],mr[3], mr[4],mr[5]);
//         rm.getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );
// 	printF("restrict-bounds=[%g,%g][%g,%g][%g,%g]\n",rab[0],rab[1], rab[2],rab[3], rab[4],rab[5]);


        setBounds(ra,rb,sa,sb,ta,tb);  
        // scaleBounds(ra,rb,sa,sb,ta,tb);

// 	getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );

// 	printF("+++ReparameterizationTransform::set corners:AFTER: getBounds=[%g,%g][%g,%g][%g,%g]\n"
// 	       " mr=[%g,%g][%g,%g][%g,%g], ",
// 	       rab[0],rab[1], rab[2],rab[3], rab[4],rab[5],
//                mr[0],mr[1], mr[2],mr[3], mr[4],mr[5]);

//         rm.getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );
// 	printF("restrict-bounds=[%g,%g][%g,%g][%g,%g]\n",rab[0],rab[1], rab[2],rab[3], rab[4],rab[5]);
	
//        setMappingProperties(mapPointer);

//         getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );
// 	printF("+++ReparameterizationTransform::set corners:AFTER(2): getBounds=[%g,%g][%g,%g][%g,%g]\n"
// 	       " mr=[%g,%g][%g,%g][%g,%g], ",
// 	       rab[0],rab[1], rab[2],rab[3], rab[4],rab[5],
//                mr[0],mr[1], mr[2],mr[3], mr[4],mr[5]);
//         rm.getBounds(rab[0],rab[1], rab[2],rab[3], rab[4],rab[5] );
// 	printF("restrict-bounds=[%g,%g][%g,%g][%g,%g]\n",rab[0],rab[1], rab[2],rab[3], rab[4],rab[5]);

      }
      else
        gi.outputString("Error: you can only set bounds for a restriction");
      mappingHasChanged();
    }
    else if( answer=="set bounds" ) // this will be phased out
    {
      if( reparameterizationType==restriction )
      {
        real ra=0., rb=1., sa=0., sb=1., ta=0., tb=1.;
	if( domainDimension==1 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter ra,rb (default=(%e,%e)): ",ra,rb));
	  if( line!="" ) sScanF(line,"%e %e ",&ra,&rb);
	}
	else if( domainDimension==2 )
	{
	  gi.inputString(line,sPrintF(buff,"Enter ra,sa,rb,sb (default=(%e,%e,%e,%e)): ",
				       ra,sa,rb,sb));
	  if( line!="" ) sScanF(line,"%e %e %e %e ",&ra,&sa,&rb,&sb);
	}
	else
	{
	  gi.inputString(line,sPrintF(buff,"Enter ra,sa,ta,rb,sb,tb (default=(%e,%e,%e,%e,%e,%e)): ",
				       ra,sa,ta,rb,sb,tb));
	  if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&ra,&sa,&ta,&rb,&sb,&tb);
	}
        setBounds(ra,rb,sa,sb,ta,tb);  // *wdh* 070415 -- scale bonunds in case of multiple reparams
        // scaleBounds(ra,rb,sa,sb,ta,tb);
        // setMappingProperties(mapPointer);
      }
//      else if( reparameterizationType==orthographic )
//      {
//      }
      else
        gi.outputString("Error: you can only set bounds for a restriction");
      mappingHasChanged();
    }
    else if( answer=="arclength weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the arclength weight (>=0., default=%6.2e) :",arcLengthWeight));
      if( line!="" )
      {
        sScanF(line,"%e",&arcLengthWeight);
	printf("New arcLengthWeight=%e  \n",arcLengthWeight);
        equidistributionInitialized=false;
	mappingHasChanged();
      }
    }
    else if( answer=="curvature weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the curvature weight (>=0., default=%6.2e) :",curvatureWeight));
      if( line!="" )
      {
        sScanF(line,"%e",&curvatureWeight);
	printf("New curvatureWeight=%e  \n",curvatureWeight);
        equidistributionInitialized=false;
	mappingHasChanged();
      }
    }
    else if( answer=="number of smooths" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the number of times to smooth the weight function, (default=%i) :",
          numberOfEquidistributionSmooths));
      if( line!="" )
      {
        sScanF(line,"%i",&numberOfEquidistributionSmooths);
	mappingHasChanged();
      }
    }
    else if( answer=="re-evaluate equidistribution" )
    {
      if( reparameterizationType==equidistribution )
        initializeEquidistribution(!equidistributionInitialized);
    }
    else if( answer=="show parameters" )
    {
      printf(" arcLengthWeight = %6.2e \n"
             " curvatureWeight = %6.2e \n",
             arcLengthWeight,curvatureWeight );
      display();
      printf("\n ***Here is the Mapping used to reparameterize ***\n");
      reparameterize->display();
    }
    else if( answer=="plot" )
    {
      if( !mappingChosen )
      {
	gi.outputString("you must first choose a mapping to transform");
	continue;
      }
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="test" )
    {
      const int debugOld=Mapping::debug;
      Mapping::debug=15;

      realArray x(2,3),r(1,3),xx(1,3),xr(1,3,3);
      r=-1.;
      x=.1;  // raise up to plot
      Range Rx(0,rangeDimension-1);
      aString answer;
      aString menu[]=
      {
	"enter a point",
	"enter multiple points",
	"enter an r value",
	"use robust inverse",
	"do not use robust inverse",
	"done",
	""
      };
      for( int i=0;;i++ )
      {
	gi.inputString(answer,"Enter a point r");

	sScanF(answer,"%e %e %e",&r(0,0),&r(0,1),&r(0,2));
	mapC(r,x(0,Rx),xr);
	printf(" r=(%12.8e,%12.8e,%12.8e), x=(%12.8e,%12.8e,%12.8e)\n",
	       r(0,0),r(0,1),(domainDimension==2 ? 0. : r(0,2)), 
	       x(0,0),x(0,1),(rangeDimension==2  ? 0. : x(0,2)));

	if( domainDimension==3 && rangeDimension==3 )
	{
	  real tripleProduct=( (xr(0,0,0)*xr(0,1,1)-xr(0,0,1)*xr(0,1,0))*xr(0,2,2) +
			       (xr(0,0,1)*xr(0,1,2)-xr(0,0,2)*xr(0,1,1))*xr(0,2,0) +
			       (xr(0,0,2)*xr(0,1,0)-xr(0,0,0)*xr(0,1,2))*xr(0,2,1) );

	  printf(" xr = (%8.2e,%8.2e,%8.2e), xs=(%8.2e,%8.2e,%8.2e) xt=(%8.2e,%8.2e,%8.2e)\n"
		 " xr X xs o xt = %8.2e  signForJacobian = %f \n",
		 xr(0,0,0),xr(0,1,0),xr(0,2,0),
		 xr(0,0,1),xr(0,1,1),xr(0,2,1),
		 xr(0,0,2),xr(0,1,2),xr(0,2,2),tripleProduct,getSignForJacobian());
	}
      }

      Mapping::debug=debugOld;
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
             answer=="check" ||
             answer=="check inverse"   || 
             answer=="use robust inverse" || 
             answer=="do not use robust inverse"  )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=true;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject && mappingChosen )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
