#include "CompositeSurface.h"
#include "MappingProjectionParameters.h"
#include "MappingRC.h"
#include "TrimmedMapping.h"
#include "MappingInformation.h"
#include "NurbsMapping.h"
#include "ComposeMapping.h"
#include "display.h"
#include "CompositeTopology.h"

//#include "GL_GraphicsInterface.h"
#include "GenericGraphicsInterface.h"
#include "UnstructuredMapping.h"

// *wdh* include <GL/gl.h>

#define SC (char *)(const char *)

static int totalNumberOfProjections=0;
static int totalNumberOfProjectInverses=0;


CompositeSurface::
CompositeSurface()
: Mapping(2,3,parameterSpace,cartesianSpace) 
//===========================================================================
/// \brief  Default Constructor
//===========================================================================
{
  CompositeSurface::className="CompositeSurface";
  setName( Mapping::mappingName,"compositeSurface");

  numberOfSurfaces=0;
  topologyDetermined=FALSE;
  surfaceColour=NULL;
  compositeTopology=NULL;
  tolerance=0.;
  plotGhostLines=false;  // normally we do NOT plot ghost lines (takes too long for trimmed surafces)
  
  for( int i=0; i<numberOfTimings; i++ )
    timing[i]=0.;
}


// Copy constructor is deep by default
CompositeSurface::
CompositeSurface( const CompositeSurface & X0, const CopyType copyType /* =DEEP */ )
{
  CompositeSurface::className="CompositeSurface";
  (*this)=X0;
}

CompositeSurface::
~CompositeSurface()
{
  delete [] surfaceColour;
  delete compositeTopology;
}


CompositeSurface & CompositeSurface::
operator =( const CompositeSurface & X0 )
//===========================================================================
/// \brief  operator equal is a deep copy
//===========================================================================
{
  numberOfSurfaces=X0.numberOfSurfaces;
  surfaces=X0.surfaces;
  signForNormal.redim(0);
  signForNormal=X0.signForNormal;
  topologyDetermined=X0.topologyDetermined;
  visible=X0.visible;
  surfaceIdentifier.redim(0);
  surfaceIdentifier=X0.surfaceIdentifier;
  delete [] surfaceColour;
  surfaceColour=NULL;
  tolerance=X0.tolerance;
  if( numberOfSurfaces>0 )
  {
    surfaceColour=new aString [numberOfSurfaces];
    for( int s=0; s<numberOfSurfaces; s++ )
      surfaceColour[s]=X0.surfaceColour[s];
  }
// display lists stuff
  dList = X0.dList;
  
  if( compositeTopology!=NULL && X0.compositeTopology!=NULL )
  {
    *compositeTopology=*X0.compositeTopology;
  }
  else if( X0.compositeTopology!=NULL )
  {
    compositeTopology = new CompositeTopology(*this);
    *compositeTopology=*X0.compositeTopology;
  }
  else
  {
    delete compositeTopology;
    compositeTopology=NULL;
  }
  

  return *this;
}

CompositeTopology* 
CompositeSurface::
getCompositeTopology(bool alloc /*=false*/) 
{// kkc allocate a new one if it is not there compositeTopology; 
  return compositeTopology? (compositeTopology) : (alloc ? ((compositeTopology=new CompositeTopology( *this ) )):0 );
}

void CompositeSurface::
initialize()
{
  // make sure the bounding boxes and the grid are made (even for mappings with analytic inverses)
  for( int s=0; s<numberOfSurfaces; s++ )
  {
    Mapping *map = &(*this)[s];
    printf("CompositeSurface::initialize inverse of sub surface s=%i, class name = %s \n",s,
          (const char *)map->getClassName());
    // map->approximateGlobalInverse->initialize(); 
    (*this)[s].approximateGlobalInverse->initialize(); 
    // printf("CompositeSurface::initialize: bounding box for subsurface %i \n",s);
    // (*this)[s].approximateGlobalInverse->getBoundingBox().display("bounding box");
  }
/* ----  don't overwrite values from get!
  visible.redim(numberOfSurfaces);
  visible=TRUE;
  surfaceIdentifier.redim(numberOfSurfaces);
  surfaceIdentifier=-1;
---- */
}

int CompositeSurface::
add( Mapping & surface,
     const int & surfaceID /* = -1 */)
//===========================================================================
/// \brief  
///     Add a surface to the composite surface
/// \param surface (input): add this mapping to the composite surface.
/// \param surfaceID (input): optional surface identification number. This could identify
///    the surface in a CAD file, for example.
//===========================================================================
{

//kkc 070305
  if ( signForNormal.getLength(0)<=numberOfSurfaces )
  {
      int nAdd = 100;
      signForNormal.resize(numberOfSurfaces+nAdd);
      Range R(numberOfSurfaces, numberOfSurfaces+nAdd-1 );
      signForNormal(R) = 1;
  }

  numberOfSurfaces++;
  MappingRC surfaceRC(surface);   // make a reference counted surface
  surfaces.addElement(surfaceRC);

  if( visible.getLength(0) < numberOfSurfaces || surfaceIdentifier.getLength(0) < numberOfSurfaces ||
      dList.getLength(1) < numberOfSurfaces )
  {
    visible.resize(numberOfSurfaces+20);
    surfaceIdentifier.resize(numberOfSurfaces+20);
    aString *temp=new aString [numberOfSurfaces+20];
    for( int s=0; s<numberOfSurfaces-1; s++ )
      temp[s]=surfaceColour[s];
    delete [] surfaceColour;
    surfaceColour=temp;
// display list stuff
    dList.resize(numberOfDLProperties,numberOfSurfaces+20);
  }
  visible(numberOfSurfaces-1)=TRUE;
  surfaceIdentifier(numberOfSurfaces-1)=surfaceID;
  surfaceColour[numberOfSurfaces-1]="blue";
// display list stuff
  Range dlRange(boundary,numberOfDLProperties-1);
  dList(dlRange,numberOfSurfaces-1) = 0; // initialize to zero
  
  int axis;
// compute the bounding box by constructing the grid that will
// be used for plotting and inverting the mapping
  if( surface.getClassName() == "TrimmedMapping" )
  {
    int side, axis;
    TrimmedMapping & trim = (TrimmedMapping &)surface;
    if(trim.trimmingIsValid()  )
    {
      UnstructuredMapping & um = trim.getTriangulation();
      for (side=0; side<=1; side++)
	for(axis=0; axis<rangeDimension; axis++ )
	  trim.setRangeBound(side, axis, (real) um.getRangeBound(side, axis));
      
    }
    else // get the grid for the untrimmed surface
    {
      Mapping &untrim = *trim.surface;
      untrim.getGrid();
      for (side=0; side<=1; side++)
	for (axis=0; axis<untrim.getRangeDimension(); axis++)
	  trim.setRangeBound(side, axis, untrim.getRangeBound(side, axis));
    }
// AP debug
//      for (axis=0; axis<rangeDimension; axis++)
//        printf("Trimmed %s surface, axis=%i, bound=[%e, %e]\n", 
//  	     (trim.trimmingIsValid()? "valid": "INvalid"), axis, 
//  	     (real) trim.getRangeBound(Start, axis), (real) trim.getRangeBound(End, axis));
  }
  else
  {

    if( surface.getClassName() != "UnstructuredMapping" )
      surface.getGrid();

// AP debug
      for (axis=0; axis<rangeDimension; axis++)
        printf("Untrimmed surface, axis=%i, bound=[%e, %e]\n", 
  	     axis, (real) surface.getRangeBound(Start, axis), (real) surface.getRangeBound(End, axis));
  }
  
  
  Bound bs, be, bsCS, beCS, nullBound;
  
  for( axis=0; axis<rangeDimension; axis++ )
  {
    bs = surface.getRangeBound(Start,axis);
    be = surface.getRangeBound(End,axis);
    bsCS = getRangeBound(Start,axis);
    beCS = getRangeBound(End  ,axis);

// AP debug
//      printf("CS::add %i: current bounds axis=%i [%e,%e] new surface bounds=[%e,%e]\n",numberOfSurfaces-1,
//    	   axis,(real)bsCS,(real)beCS,(real)bs,(real)be);
    
    if (bs.isFinite())
    {
      if (bsCS.isFinite())
	setRangeBound(Start,axis,min((real)getRangeBound(Start,axis), (real)bs));
      else
	setRangeBound(Start, axis, (real)bs);
    }
    
    if (be.isFinite())
    {
      if (beCS.isFinite())
	setRangeBound(End  ,axis,max((real)getRangeBound(End  ,axis), (real)be));	
      else
	setRangeBound(End  , axis, (real)be);
    }
    
// AP debug
//      printf("CS::add %i: new composite surface bounds axis=%i [%e, %e]\n",numberOfSurfaces-1,
//    	   axis, (real) getRangeBound(Start, axis), (real) getRangeBound(End, axis));
  }


  topologyDetermined=FALSE;
  mappingHasChanged();
  return 0;
}

int CompositeSurface::
isVisible(const int & surfaceNumber) const
//===========================================================================
/// \details 
///    Query whether a sub-surface is visible.
/// \param surfaceNumber : sub-surface index from 0 to numberOfSubSurfaces()-1
//===========================================================================
{
  if( surfaceNumber<0 || surfaceNumber>=numberOfSurfaces )
  {
    printf("CompositeSurface::isVisible:ERROR: invalid surface number %i\n",surfaceNumber);
    return 0;
  }
  return visible(surfaceNumber);
}

int CompositeSurface::
setIsVisible(const int & surfaceNumber, 
             const bool & trueOrFalse /* =TRUE */)
//===========================================================================
/// \details 
///    Set the visibity of a sub-surface. Invisible sub-surfaces are NOT considered
///  by the project function.
/// \param surfaceNumber : sub-surface index from 0 to numberOfSubSurfaces()-1
/// \param trueOrFalse (input) : true if visible, else invisible.
//===========================================================================
{
  if( surfaceNumber<0 || surfaceNumber>=numberOfSurfaces )
  {
    printf("CompositeSurface::setIsVisible:ERROR: invalid surface number %i\n",surfaceNumber);
    return 1;
  }
  visible(surfaceNumber)=trueOrFalse;
  return 0;

}


int CompositeSurface::
findOutwardTangent( Mapping & map, const realArray & r, const realArray & x, realArray & outwardTangent )
//===========================================================================
/// \param Access: This is a {\bf protected} routine.
/// \brief  
///     Determine the outward tangent at point r on the edge of a (trimmed) sub-surface.
///    If r is on the boundary of the unit square then it is easy to get the outward tangent.
///    If r is near the the boundary of a trimmed surface then we find which trimming curve we
///    on on and use the normal to the trimming curve (which is in r space) to get the outward
///    tangent.
/// \param map (input): find the outward tangent of this Mapping.
/// \param r(0,0:1) (input) : unit square coordinates on the surface.
/// \param x(0,0:2) (input) : surface coordinates x=map(r)
/// \param outwardTangent(0,0:2) : outward tangent (if return value==0)
/// \return  0 on success, 1 for failure.
//===========================================================================
{
  // find other outward tangent:
 //  if not a trimmed surface then must be near r=0 or r=1
 // if on a trimmed surface then near r=0,r=1 or near a trimming curve

  real eps = .001;  
  real drShiftFactor=.05; 

  realArray inwardDeltaR(1,2);
  inwardDeltaR=0.;
  if( fabs(r(0,0))<eps )
    inwardDeltaR(0,0)=1.;
  else if(  fabs(r(0,0)-1.)<eps  )
    inwardDeltaR(0,0)=-1.;
  else if( fabs(r(0,1))<eps )
    inwardDeltaR(0,1)=1.;
  else if(  fabs(r(0,1)-1.)<eps  )
    inwardDeltaR(0,1)=-1.;
  else
  {
    // we must be on a trimmed surface
    if( map.getClassName()=="TrimmedMapping" )
    {
      intArray cMin(1);
      realArray rC(1,2), xC(1,2),dist(1), rCr(1,2,1);
      rC=0.;
      xC=0.;
      cMin=-1;
      dist=1.;
      TrimmedMapping & trim = (TrimmedMapping &) map;
      trim.findClosestCurve(r,cMin, rC, xC, dist );
      
      int c=cMin(0);
#ifdef OLDSTUFF
      Mapping & curve = c==1 ? *trim.outerCurve : *(trim.innerCurve[c-2]);
#else
      Mapping & curve = * trim.trimCurves[c];
#endif

      curve.map(rC,xC,rCr); // determine the tangent to the curve

      inwardDeltaR(0,0)=-rCr(0,1,0);   // normal to curve (in r coordinates)
      inwardDeltaR(0,1)= rCr(0,0,0);
      real norm = SQR(inwardDeltaR(0,0)) + SQR(inwardDeltaR(0,1));
      if( norm==0. )
      {
	cout << "findOutwardTangent:ERROR: tangent to curve is zero ! \n";
	throw "error";
      }
      inwardDeltaR*=1./SQRT(norm);

      // make sure the normal is inward **** could be wrong
      const int inOut1 = trim.insideOrOutside( evaluate(r+inwardDeltaR*drShiftFactor), c  );  // was .01
      if( inOut1==-1 )
      {
        const int inOut2 = trim.insideOrOutside( evaluate(r-inwardDeltaR*drShiftFactor), c );
        if( inOut2==+1 )
  	  inwardDeltaR=-inwardDeltaR;
	else
	{
          const int numberOfTries=4;
          for( int itry=0; itry<numberOfTries; itry++ )
	  {
	    printf("***CompositeSurface:findOutwardTangent:WARNING: itry=%i, inOut1(r+%5.2e)=%i, inOut2(r-%5.2e)=%i "
		   "are both outside! Trying with a smaller shift...\n",itry,drShiftFactor,inOut1,drShiftFactor,inOut2);
	    // **wdh* 990407 drShiftFactor+=.05;
	    drShiftFactor*=.1;
	    const int inOut3 = trim.insideOrOutside( r, c );
	    printf(" insideOrOutside(r)=%i, nearest curve c=%i (1=outer) \n",inOut3,c);
	    const int inOut4 = trim.insideOrOutside( evaluate(r-inwardDeltaR*drShiftFactor), c );
	    const int inOut5 = trim.insideOrOutside( evaluate(r+inwardDeltaR*drShiftFactor), c );
	    if( inOut5==-1 )
	    {
	      if( inOut4==+1 )
	      {
		inwardDeltaR=-inwardDeltaR;
		break;
	      }
	      else if( itry==(numberOfTries-1) )
	      {
		printf(" insideOrOutside(r-dr*%5.2e)=%i, insideOrOutside(r+dr*%5.2e)=%i \n",
                       drShiftFactor,inOut4,drShiftFactor,inOut5);
		printf("***ERROR something is wrong here. Maybe the surfaces don't match. Continuing...\n");
		return 1;
	      }
	    }
	  }
	  
	}
      }
    }
    else
    {
      // this is not a trimmed mapping after all. The surfaces could be overlapped
      // choose the closest boundary.
      if( min(fabs(r(0,0)),fabs(r(0,1))) < min(fabs(r(0,0)-1.),fabs(r(0,1)-1.)) )
      {
	if( fabs(r(0,0)) < fabs(r(0,1)) )
          inwardDeltaR(0,0)=1.;  // we are closest to r_0=0
        else	    
          inwardDeltaR(0,1)=1.;  // we are closest to r_1=0
      }
      else
      {
	if( fabs(r(0,0)-1.) < fabs(r(0,1)-1.) )
          inwardDeltaR(0,0)=-1.;
        else	    
          inwardDeltaR(0,1)=-1.;
      }
    }
  }
  
  map.map(evaluate(r+inwardDeltaR*drShiftFactor),outwardTangent); 
  if( debug & 4 )
  {
    printf("  findOutwardTangent: r=(%g,%g), inwardDeltaR=(%g,%g) \n",r(0,0),r(0,1),
            inwardDeltaR(0,0),inwardDeltaR(0,1));
    printf(" map(r+deltaR) = (%g,%g,%g) \n",outwardTangent(0,0),outwardTangent(0,1),outwardTangent(0,2));
  }
  
  outwardTangent=x-outwardTangent;
  real norm=SQRT( SQR(outwardTangent(0,0)) + SQR(outwardTangent(0,1)) + SQR(outwardTangent(0,2)) );
  if( norm==0. )
  {
    printf("  findOutwardTangent:ERROR: norm of outward tangent is ZERO \n");
    return 1;
  }
  else
    outwardTangent*=1./norm;
  
  if( debug & 4 )
    printf("  findOutwardTangent: outwardTangent=(%g,%g,%g) \n",
         outwardTangent(0,0),outwardTangent(0,1),outwardTangent(0,2));
  
  return 0;
  
}


void CompositeSurface::
findNearbySurfaces(const int & s, 
                   realArray & r,
                   const bool & doubleCheck,
                   IntegerArray & consistent,
                   IntegerArray & inconsistent )
// =====================================================================================
/// \param visibility:
///    This is a private routine.
/// \details 
///    Given a point r on the boundary of a surface, find any nearby surfaces to this
///   point and set the signForNormal array
///    
// =====================================================================================
{


  Mapping & map = (*this)[s];

  realArray x(1,3),xr(1,3,2),normal(1,3), rP(1,2), xP(1,3), xrP(1,3,2),normalP(1,3);
  intArray subSurfaceIndex(1), ignoreThisSubSurface(1);

  map.map(r,x,xr);  // find point on the subsurface
  subSurfaceIndex(0)=s;
  if( getNormals(subSurfaceIndex,xr,normal)!=0 )
    return;

  normal*= signForNormal(s) > 0 ? 1. : -1; // reset to original normal direction

  if( TRUE || Mapping::debug & 2 )
    printf("\n -----------------------------------------------------------------------------------\n"
           " findNearbySurfaces: project pt r=(%5.1e,%5.1e) x=(%6.2e,%6.2e,%6.2e) from sub-surface %i "
           "onto composite surface\n",r(0,0),r(0,1),x(0,0),x(0,1),x(0,2),s);

  // project this point onto the composite surface, avoiding the current surface
  subSurfaceIndex(0)=-1;
  ignoreThisSubSurface(0)=s;
  xP=0.;
  rP=-1.; // initial guess -- -1 => let routine decide a better one
  project(subSurfaceIndex,x,rP,xP,xrP,normalP,ignoreThisSubSurface);   // *********** use the new interface ****
  int nearbySurface=subSurfaceIndex(0);

  Range Rx(0,rangeDimension-1);
  // Here is the tolerance we use to decide if two surfaces meet at a boundary.   *****************
  // This could be wrong if there are two nearby parallel surfaces that are not joined.
  const RealArray & boundingBox = map.approximateGlobalInverse->getBoundingBox();
  real tol=.001*max(boundingBox(End,Rx)-boundingBox(Start,Rx));

  
  //  if( max(fabs(xP(0,Rx)-x(0,Rx))) > .01*max(boundingBox(End,Rx)-boundingBox(Start,Rx)) ) // ************
  if( max(fabs(xP(0,Rx)-x(0,Rx))) > tol )
  {
    if( TRUE || Mapping::debug & 2 )
      printf("  projected pt is xP=(%6.2e,%6.2e,%6.2e) of surface %i is not near any other subsurface\n",
           xP(0,0),xP(0,1),xP(0,2),s);
    return;
  }
  else if( max(fabs(xP(0,Rx)-x(0,Rx))) > .1*tol )
  {
    if( TRUE || Mapping::debug & 2 )
      printf("  >>>findNearbySurfaces:WARNING:Closest pt not so close: |x-XP| = %e, boundinBox=%e ratio=%6.2e \n",
	   max(fabs(xP(0,Rx)-x(0,Rx))),max(boundingBox(End,Rx)-boundingBox(Start,Rx)),
	   max(fabs(xP(0,Rx)-x(0,Rx)))/max(boundingBox(End,Rx)-boundingBox(Start,Rx)) );
    printf("  >>>findNearbySurfaces:WARNING: I could be making a mistake in assuming the surfaces are connected\n");
  }

  normalP*= signForNormal(nearbySurface) > 0 ? 1. : -1; // reset to original normal direction

  if( TRUE || Mapping::debug & 2 )
    printf("  projected pt, xP=(%6.2e,%6.2e,%6.2e), from %i hits subsurface = %i \n",
	 xP(0,0),xP(0,1),xP(0,2),s,nearbySurface);
	      
  if( abs(signForNormal(s)) != abs(signForNormal(nearbySurface)) || doubleCheck )
  {
    // these surfaces are newly connected
    printf("  normal[s=%i]=(%5.3f,%5.3f,%5.3f), normal[s=%i]=(%5.3f,%5.3f,%5.3f) \n",s,
	   normal(0,0),normal(0,1),normal(0,2),nearbySurface,normalP(0,0),normalP(0,1),normalP(0,2));
    printf("  ++++++ subsurface %i is connected to subsurface %i ",s,nearbySurface);
		
    real nDotN = normal(0,0)*normalP(0,0)+normal(0,1)*normalP(0,1)+normal(0,2)*normalP(0,2);

    int nSign= nDotN > 0. ? 1 : -1;

    // Since a corner can be very sharp we must ALWAYS do the more sophisticated check.
    // if( fabs(nDotN)<.1 ) // *wdh* 990301
    if( fabs(nDotN) < .8 )
//    if( TRUE || fabs(nDotN) < .99 )
    {
      if( debug & 2 )
        printf("  INFO: checking orientation from outward tangent vectors! \n");
      // compute dot product of outward "tangent" from surface wih normalP
      realArray outwardTangent1(1,3), outwardTangent2(1,3);

      // find the outward tangents to the two surfaces. Together with the normals we use this
      // info to determine which direction we should march along the surface.
      if( findOutwardTangent( (*this)[s],             r,x,  outwardTangent1 )!=0 ||
          findOutwardTangent( (*this)[nearbySurface], rP,xP,outwardTangent2 )!=0 )
      {
	// findOutwardTangent failed!
        printf("  outward tangent computation failed, skipping this case\n");
	return; 
      }
      
      // we should have that n2 and t2 are nearly in the same plane as n1 and t1.
      // this means   fabs( (n1 X t1).(n2 X t2 ) ) approx 1
      // **** otherwise we could be at a corner and picking up the wrong outward tangent
      real n1Xt1DotN2Xt2=((normal (0,1)*outwardTangent1(0,2)-normal (0,2)*outwardTangent1(0,1))*
			  (normalP(0,1)*outwardTangent2(0,2)-normalP(0,2)*outwardTangent2(0,1))+
			  (normal (0,2)*outwardTangent1(0,0)-normal (0,0)*outwardTangent1(0,2))*
			  (normalP(0,2)*outwardTangent2(0,0)-normalP(0,0)*outwardTangent2(0,2))+
			  (normal (0,0)*outwardTangent1(0,1)-normal (0,1)*outwardTangent1(0,0))*
			  (normalP(0,0)*outwardTangent2(0,1)-normalP(0,1)*outwardTangent2(0,0)) );

      if( fabs(n1Xt1DotN2Xt2)<.1 )
      {
        printf("\n ** fabs( (n1 X t1).(n2 X t2 ) )=%e, should be near 1. outward tangent computation failed\n",
	       fabs(n1Xt1DotN2Xt2));
	return;
      }
      

      if( fabs(nDotN) > .2 )
      {
        // If the normals are NOT nearly orthogonal then we check that (t1.t2)*(n1.n2) < 0
        //            
        //
        //                   ^ n1            ^ n2
        //                   |               |
        //                   --->t1    t2 <---
        //       -------------------X------------------
        //
        
	real outwardTangent1DotOutwardTangent2 = (outwardTangent1(0,0)*outwardTangent2(0,0)+
						  outwardTangent1(0,1)*outwardTangent2(0,1)+
						  outwardTangent1(0,2)*outwardTangent2(0,2));

        nSign = outwardTangent1DotOutwardTangent2*nDotN <0. ? 1 : -1;
        if( debug & 2 )
	  printf("  nSign = %i, outwardTangent1*outwardTangent2 = %g  (<0 means normals in same direction)\n",
                  nSign,outwardTangent1DotOutwardTangent2);
      }
      else
      {
        // if the normals are nearly orthogonal then we check that (t1.n2)*(t2.n1) > 0
        //
        //
        //                   ^ n1    
        //                   |       
        //                   --->t1  
        //     ---------------------X
        //                          | ^ t2
        //                          | |
        //                          | |----> n2
        //                          |
        //

	real outwardTangent1DotN2 = (outwardTangent1(0,0)*normalP(0,0)+
				     outwardTangent1(0,1)*normalP(0,1)+
				     outwardTangent1(0,2)*normalP(0,2));

	real outwardTangent2DotN1 = (outwardTangent2(0,0)*normal(0,0)+
				     outwardTangent2(0,1)*normal(0,1)+
				     outwardTangent2(0,2)*normal(0,2));

        nSign = outwardTangent1DotN2*outwardTangent2DotN1 >0. ? 1 : -1;
        if( debug & 2 )
	{
	  printf("  nSign = %i, outwardTangent1=(%g,%g,%g), normalP=(%g,%g,%g) \n",nSign,
		 outwardTangent1(0,0),outwardTangent1(0,1),outwardTangent1(0,2),
		 normalP(0,0),normalP(0,1),normalP(0,2));
	  printf("  outwardTangent2=(%g,%g,%g), normal=(%g,%g,%g) \n",
		 outwardTangent2(0,0),outwardTangent2(0,1),outwardTangent2(0,2),
		 normal(0,0),normal(0,1),normal(0,2));
	}
      }
    }
    

    printf("  with %s sign= %i \n",(nSign>0 ? "same" : "opposite"),nSign);
      
    int sNearby=signForNormal(nearbySurface);
    int sn=signForNormal(s);
    if( abs(sn) == abs(sNearby) )
    {
      if( sn*nSign != sNearby )
      {
        printf("**ERROR** Consistency error for normals on surface %i and surface %i \n",s,nearbySurface);
        inconsistent(s,nearbySurface)++;
        inconsistent(nearbySurface,s)++;
      }
      else
      {
        printf("**INFO** Consistency check for normals on surface %i and surface %i \n",s,nearbySurface);
        consistent(s,nearbySurface)++;
        consistent(nearbySurface,s)++;
      }
    }
    else
    {
      // change all subsurfaces with signForNormal(s) to +/- signForNormal(nearbySurface)
      for( int ss=0; ss<numberOfSurfaces; ss++ )
      {
	if( abs(signForNormal(ss))==abs(sn) )
	  signForNormal(ss)= signForNormal(ss)==sn ? nSign*sNearby : -nSign*sNearby;
      }
    }
    
  }
}


void CompositeSurface::
determineTopology()
// =====================================================================================
/// \brief  This is a private function.
///     Determine some topology info about the composite surface:
///   Determine the sign for each normal so that the normals of all surfaces are consistent.
/// 
/// \param Algorithm:
///    We want to assign a value of +1 or -1 to each surface (signForNormal(s)) to indicate if
///    we need to reverse the normal of the surface or not.
/// 
///  <ul>  
///    <li> surface zero is arbitrarily given a sign of +1. All other surfaces are given a
///      unique positive number to identity the surface
///    <li> We now try to link surfaces together. If two surfaces are connected at a boundary then
///      we assign the same number to them (actually plus or minus the same number depending on
///      whether the normals need to be reversed). If the two surfaces are already connected to
///      other surfaces then all connected surfaces get (+/-) the same value.
///    <li> If one of the surfaces is numbered +/- 1 (then it must be connected to surface zero)
///      then all connected surfaces will get a value of +/- 1
///    <li> stop checking when all surfaces have a value of +/- 1
///  </ul>  
///    
// =====================================================================================
{

  real time0=getCPU();
  
  topologyDetermined=TRUE; // this must be at the start since project checks this.
  printf("+++++++++++++++ determineTopology ++++++++++++++++ \n");
  initialize();
  
  const bool doubleCheck=TRUE;
  IntegerArray consistent(numberOfSurfaces,numberOfSurfaces); 
  consistent=1;
  IntegerArray inconsistent(numberOfSurfaces,numberOfSurfaces);   
  inconsistent=0;

  
  if( signForNormal.getLength(0) < numberOfSurfaces )
  {
    signForNormal.resize(numberOfSurfaces+25);
  }
  int s;
  for( s=0; s<numberOfSurfaces; s++ )
    signForNormal(s)=numberOfSurfaces*10 + s;   // give each surface a unique id
    
  signForNormal(0)=+1;  // sub surface 0 gets sign=+1

  if( numberOfSurfaces<=1 )
    return;
  
  realArray r(1,2),x(1,3),xr(1,3,2),normal(1,3), rP(1,2), xP(1,3), xrP(1,3,2),normalP(1,3);
  realArray rc(1,2),rcr(1,2,1);
  
  IntegerArray subSurfaceIndex(1), ignoreThisSubSurface(1);
  
  s=-1;
  // bool done=FALSE;
  for( int iteration=0; iteration<numberOfSurfaces*2; iteration++ )
  {
    s = (s+1) % numberOfSurfaces;
    if( iteration==0 || fabs(signForNormal(s))!=1 )
    {
      // Find other surfaces connected to this surface, 

      Mapping & map = (*this)[s];
      if( map.getClassName()=="TrimmedMapping" )
      {
        TrimmedMapping & trim = (TrimmedMapping &) map;
	// place some points on each trimming curve
        // find any nearby sub-surface

        int numberOfPointsToTryPerSide;
#ifdef OLDSTUFF
	int cStart= trim.outerCurve==NULL ? 2 : 1;
	for( int c=cStart; c<trim.numberOfInnerCurves+2; c++ )
	{
	  Mapping & curve = c==1 ? *trim.outerCurve : *(trim.innerCurve[c-2]);
#else
	for ( int c=0; c<trim.getNumberOfTrimCurves(); c++ )
	{
	  Mapping & curve = * (trim.getTrimCurve(c));
#endif
	  if( curve.getClassName()=="NurbsMapping" )
	  {
            // Normally (in proE ) a trimming curve consists of subCurves, each subCurve
            // matches to one other surface. Thus we only check the midpoint of each sub-curve -- unless
            // there is only one sub-curve, then we check more points.
	    NurbsMapping & nurb = (NurbsMapping&)curve;
	    // printf("trimCurve is a nurb, number of subCurves = %i \n",nurb.numberOfSubCurves());
            numberOfPointsToTryPerSide=nurb.numberOfSubCurves()==1 ? 2 : 1;
	    for( int subCurve=0; subCurve<nurb.numberOfSubCurves(); subCurve++ )
	    {
              printf("Check the mid point of sub-curve %i\n",subCurve);
	      
              // test a point at the mid-point of the sub-curve
	      NurbsMapping & subTrimCurve = nurb.subCurve(subCurve);
	      for( int n=0; n<numberOfPointsToTryPerSide; n++ )
	      {
		r(0,0)=(n+.5)/numberOfPointsToTryPerSide;
		subTrimCurve.map(r,rc);

		findNearbySurfaces( s,rc,doubleCheck,consistent,inconsistent );

		if( fabs(signForNormal(s))==1 ) // this means we are connected to the first surface.
		  break;
	      }
	    }
	  }
	  else
	  {
            numberOfPointsToTryPerSide=2;
	    for( int n=0; n<numberOfPointsToTryPerSide; n++ )
	    {
	      r(0,0)=(n+.5)/numberOfPointsToTryPerSide;
	      curve.map(r,rc); // get the unit square coordinates for the sub-surface

	      findNearbySurfaces( s,rc,doubleCheck,consistent,inconsistent );

	      if( fabs(signForNormal(s))==1 ) // this means we are connected to the first surface.
		break;
	    }
	  }
          // if( fabs(signForNormal(s))==1 )
          //  break;
	}
      }
      else
      {
        // place somes points on each of the boundaries and find which
        // subsurface these points are projected onto
        int numberOfPointsToTryPerSide=1;
        for( int axis=0; axis<=1; axis++ )
	{
	  for( int side=Start; side<=End; side++ )
	  {
	    for( int n=0; n<numberOfPointsToTryPerSide; n++ )
	    {
              int axisp1 = (axis+1) % 2;
	      r(0,axisp1)=(n+.5)/numberOfPointsToTryPerSide;
	      r(0,axis)=side; 

              findNearbySurfaces( s,r,doubleCheck,consistent,inconsistent  );

	      if( fabs(signForNormal(s))==1 )
		break;
	    }
	    // if( fabs(signForNormal(s))==1 )
	    //  break;
	  }
	  // if( fabs(signForNormal(s))==1 )
	  //  break;
	}
      }

    }
    if( max(abs(signForNormal(Range(0,numberOfSurfaces-1))))==min(abs(signForNormal(Range(0,numberOfSurfaces-1)))) )
    {
      printf(" ++++++++++ all sub surfaces classified ++++++ \n");
      // normalize
      signForNormal(Range(0,numberOfSurfaces-1))/=abs(signForNormal(Range(0,numberOfSurfaces-1)));
      signForNormal(Range(0,numberOfSurfaces-1)).display("signForNormal");
      break;
    }
  }
  if( max(abs(signForNormal(Range(0,numberOfSurfaces-1))))!=1 )
  {
    printf("CompositeSurface::WARNING: not all surfaces have been connected to one another. This could be\n"
           "                  an error or it could be that some surfaces are disjoint from others\n");
    signForNormal(Range(0,numberOfSurfaces-1)).display("signForNormal");
    // normalize
    signForNormal(Range(0,numberOfSurfaces-1))/=abs(signForNormal(Range(0,numberOfSurfaces-1)));
  }
  int numberOfInconsistencies=sum(inconsistent);
  int numberOfConsistencies=sum(consistent);
  if( numberOfInconsistencies>0  )
  {
    printf("CompositeSurface::WARNING: There were inconsistencies found, number=%i\n",numberOfInconsistencies);
  }
  printf(" numberOfConsistencies/numberOfSurfaces^2 = %e \n",numberOfConsistencies/real(SQR(numberOfSurfaces)));
  printf("\n +++CompositeSurface::INFO: time to determine topology = %e\n\n",getCPU()-time0);
    
}


int  CompositeSurface::
numberOfSubSurfaces() const
//===========================================================================
/// \brief  
///  return the total number of sub-surfaces that make up this composite surface
//===========================================================================
{
  return numberOfSurfaces;
}




Mapping & CompositeSurface::
operator []( const int & subSurfaceIndex )
//===========================================================================
/// \brief  
///  return the Mapping that represents a subSurface
//===========================================================================
{
  if( subSurfaceIndex<0 || subSurfaceIndex>=numberOfSurfaces )
  {
    cout << "CompositeSurface::operator []: ERROR subSurface index is out of range \n";
    {throw "error";}
  }
  return surfaces[subSurfaceIndex].getMapping();
}


int CompositeSurface::
printStatistics(FILE *file /* =stdout */ )
//===========================================================================
/// \brief  
///    Print some statistics about the CompositeSurface. Currently only some timing
///  statistics for the project function are presented.
//===========================================================================
{
  fprintf(file,"  ========== Statistics for CompositeSurface =================\n"
          " number of projections=%i, number of project inverses=%i \n"
	  "   Timings:                      seconds     %%   \n",
          totalNumberOfProjections,totalNumberOfProjectInverses);
  
  timing[totalTime]=max(timing[0],REAL_MIN*10.);
  

  aString timingName[numberOfTimings]=
  {
    "total time",
    "timeForProject",
    "timeToProjectInvertMapping",
    "timeToProjectEvaluateMapping"
  };

  int nSpace=30;
  aString dots="...................................................................";
  int i;
  if( timing[totalTime]==0. )
    timing[totalTime]=REAL_MIN;
  for( i=0; i<numberOfTimings; i++ )
    if( timingName[i]!="" )    
      fprintf(file,"%s%s%10.2e  %7.3f\n",(const char*)timingName[i],
         (const char*)dots(0,max(0,nSpace-timingName[i].length())),timing[i],100.*timing[i]/timing[totalTime]);



  if( compositeTopology!=NULL )
  {
    // use the triangulation associated with the reference surface.
    UnstructuredMapping *uns=compositeTopology->getTriangulation();
    if( uns!=NULL )
    {
      uns->printStatistics(file);
      
    }
  }

  return 0;
}



int CompositeSurface::
remove( const int & surfaceNumber )
//===========================================================================
/// \brief  
///     Remove a sub-surface from the composite surface
/// \param surfaceNumber (input): remove this surface.
//===========================================================================
{
  if( surfaceNumber<0 || surfaceNumber>=numberOfSurfaces )
  {
    printf("CompositeSurface::remove:ERROR: unable to remove surface number %i, this does not exist!\n",
	   surfaceNumber);
    return -1;
  }
  surfaces.deleteElement(surfaceNumber);
  Range S(surfaceNumber,numberOfSurfaces-2); // AP: What happens when numberOfSurfaces < 2???
  visible(S)=visible(S+1);
  surfaceIdentifier(S)=surfaceIdentifier(S+1);
  int s;
  for( s=S.getBase(); s<=S.getBound(); s++ )
  {
    surfaceColour[s]=surfaceColour[s+1];
    if( s>=signForNormal.getBase(0) && s<signForNormal.getBound(0) )
      signForNormal(s)=signForNormal(s+1);
  }
// display list stuff
  Range dlRange(boundary,numberOfDLProperties-1);
  dList(dlRange,S) = dList(dlRange,S+1);

  numberOfSurfaces--;
  mappingHasChanged();

  return 0;
}

void CompositeSurface::
recomputeBoundingBox()
//===========================================================================
/// \brief  
///   Recompute the bounding box of the CompositeSUrface by querying all subsurfaces
///   (visible and invisible) of their bounding boxes. Use this routine sparingly,
///   since changing the bounding box will make the plot translate on the screen.
/// \param Author: AP
//===========================================================================
{
// recompute the bounding box
  int axis, side;
  RealArray xBound(2,3);
// initialize
  for (axis=0; axis<rangeDimension; axis++)
  {
    xBound(Start,axis)= REAL_MAX*.1;
    xBound(End  ,axis)=-REAL_MAX*.1;
  }
  
  Bound bs, be;
// loop over all surfaces
  int s;
  for (s=0; s<numberOfSurfaces; s++)
  {
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      bs = surfaces[s].getMapping().getRangeBound(Start,axis);
      be = surfaces[s].getMapping().getRangeBound(End,axis);

      if (bs.isFinite())
	xBound(Start,axis) = min(xBound(Start,axis), (real) bs);
    
      if (be.isFinite())
	xBound(End,axis) = max(xBound(End,axis), (real) be);

// AP debug
//        printf("CS::recomputeBB: surface=%i, axis=%i, bounds=[%e, %e]\n", s, axis,
//    	     (real) bs, (real) be);
    } // end for axis
  } // end for numberOfSurfaces

// assign the new bounds to the composite surface
  for (axis=0; axis<rangeDimension; axis++)
  {
    setRangeBound(Start, axis, xBound(Start,axis));
    setRangeBound(End,   axis, xBound(End,axis));
// AP debug
//      printf("CS::recomputeBB: new composite surface bounds axis=%i [%e, %e]\n",axis, 
//    	   xBound(Start,axis), xBound(End,axis));
  }
  
}

aString CompositeSurface::
getColour( const int & surfaceNumber ) const
//===========================================================================
/// \brief  
///     Get the colour of a sub-surface.
/// \param surfaceNumber (input): sub-surface to set.
/// \param Return value : the name of the colour.
//===========================================================================
{
  if( surfaceNumber<0 || surfaceNumber>=numberOfSurfaces )
  {
    printf("CompositeSurface::getColour:ERROR: invalid surface number %i, this does not exist!\n",
	   surfaceNumber);
    return 0;
  }
  return surfaceColour[surfaceNumber];
}

int CompositeSurface::
setColour( const int & surfaceNumber, const aString & colour )
//===========================================================================
/// \brief  
///     Set the colour for a sub-surface.
/// \param surfaceNumber (input): sub-surface to set.
/// \param colour (input) : the name of the colour such as "red", "green",...
//===========================================================================
{
  if( surfaceNumber<0 || surfaceNumber>=numberOfSurfaces )
  {
    printf("CompositeSurface::setColour:ERROR: invalid surface number %i, this does not exist!\n",
	   surfaceNumber);
    return -1;
  }
  // printf("CompositeSurface::setColour: surfaceColour[%i]=%s\n",surfaceNumber,(const char*)colour);
  
  surfaceColour[surfaceNumber]=colour;
  return 0;
}




int CompositeSurface::
getNormals(const intArray & subSurfaceIndex, const realArray & xr, realArray & normal) const
// ==============================================================================================
//
// /Description: Given the derivatives xr, compute the normal and multiply by signForNormal(subSurfaceIndex(i))
//   provided subSurfaceIndex(i)>=0
//
//
// ==============================================================================================
{
  Range R(xr.getBase(0),xr.getBound(0));
  int rBase=R.getBase(), rBound=R.getBound();

  realArray l2Norm(R);  // ********** fix *****
    
  normal(R,0)=xr(R,1,0)*xr(R,2,1)-xr(R,2,0)*xr(R,1,1);
  normal(R,1)=xr(R,2,0)*xr(R,0,1)-xr(R,0,0)*xr(R,2,1);
  normal(R,2)=xr(R,0,0)*xr(R,1,1)-xr(R,1,0)*xr(R,0,1);

  l2Norm=SQRT(SQR(normal(R,0))+SQR(normal(R,1))+SQR(normal(R,2)));
  for( int i=rBase; i<=rBound; i++ )
  {
    if( l2Norm(i)==0. )
    {
      // this must be a singularity, choose a nearby normal
      cout << "CompositeSurface::getNormals: zero normal \n";
      int in = i+1 < rBound ? i+1 : max(rBase,i-1);
      l2Norm(i)=l2Norm(in);
      normal(i,Range(0,2))=normal(in,Range(0,2));
      if( l2Norm(i)==0. )
      {
        if( rBase!=rBound )
  	  cout << "CompositeSurface::getNormal:ERROR: the normal for the neighbour is zero too! \n";
	return 1;
      }
    }
    if( subSurfaceIndex(i)>=0 )
      l2Norm(i)*= signForNormal(subSurfaceIndex(i)) > 0 ? 1. : -1; // ***** scale normal by +1 or -1
  }
  
  l2Norm=1./l2Norm;
  normal(R,0)*=l2Norm;
  normal(R,1)*=l2Norm;
  normal(R,2)*=l2Norm;
  return 0;
}


int CompositeSurface::
insideOrOutside( realArray & x, IntegerArray & inside )
//===========================================================================
/// \details 
///    Determine whether the points x(i,0:2) are inside the surface triangulation using ray tracing.
///  Count the number of times a ray crosses the triangulation.
///  The surface triangulation is assumed to be water-tight. 
/// 
/// \param x (input) : check these points these points.
/// \param inside (output) : inside(i) = true if point x(i,0:2) is inside 
/// \return  If successful, the return value is inside(0). Return -1 if there was an error.
///  
//===========================================================================
{
  if( compositeTopology==NULL )
  {
    printf("CompositeSurface::insideOrOutside:ERROR: the topology has not been built.\n");
    return -1;
  }
  UnstructuredMapping *uns = compositeTopology->getTriangulation();
  if( uns==NULL )
  {
    printf("CompositeSurface::insideOrOutside:ERROR: there is no triangulation in the topology.\n");
    return -1;
  }
  return uns->insideOrOutside(x,inside);
}


int CompositeSurface::
project( realArray & x, 
	 MappingProjectionParameters & mpParams )
//===========================================================================
/// \brief  
///    Project the points x(i,0:2) onto the surface. Also return the sub-surface index
/// 
/// \param subSurfaceIndex (input/output) : The index of the sub-surface that the point
///    is closest to. On input this is the index of the previous point (if >= 0)
/// \param elementIndex (input/output) : if the CompositeSurface has an associated triangulation then
///     this will be the closest element on the triangulation. On input this is a guess to the
///     closest triangulation ( if >=0 ).
/// \param x (input) : project these points onto the surface.
/// \param rProject (input/output) : sub-surface coordinates. On input these are an initial
///     guess. On output they are the actual unit square coordinates.
/// \param xProject (input/output) : on input these are the projected points from the previous
///      step (if subSurfaceIndex>=0 on input). On output these are the projected points.
/// \param xrProject (output) : the derivative of the mapping at xProject
/// \param normal (input/output) : on input this is the normal to the surface at the old point. On output
///    this array then it will hold the normal to the
///     surface, normal(i,0:2). The normal vector will be chosen so that it is consistent
///     across all sub-surfaces
/// \param ignoreThisSubSurface(i) (input) : Optional. Do not consider this sub-surface when
///    projecting point x(i,0:2).
//===========================================================================
{
  typedef MappingProjectionParameters MPP;

  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  intArray & elementIndex = mpParams.getIntArray(MPP::elementIndex);
  intArray & ignoreThisSubSurface = mpParams.getIntArray(MPP::ignoreThisSubSurface);

  realArray & rProject  = mpParams.getRealArray(MPP::r);
  realArray & xOld      = mpParams.getRealArray(MPP::x);
  realArray & xrProject = mpParams.getRealArray(MPP::xr);
  realArray & normal    = mpParams.getRealArray(MPP::normal);

  const int xBase = x.getBase(0);
  const int xBound = x.getBound(0);
  Range R(xBase,xBound);
  if( subSurfaceIndex.getBase(0)>xBase || subSurfaceIndex.getBound(0)<xBound )
  {
    subSurfaceIndex.redim(R);
    subSurfaceIndex=-1;  // this means we have no guess at the previous surface.
    elementIndex.redim(R);
    elementIndex=-1;  // this means we have no guess at the previous element number
    rProject.redim(R,domainDimension); rProject=.5;
    xOld.redim(R,rangeDimension);   xOld=0.;
        xrProject.redim(R,rangeDimension,domainDimension);

    if( normal.getBase(0)>xBase || normal.getBound(0)<xBound )
      normal.redim(R,rangeDimension);
  }
  
//   IntegerArray & subSurfaceIndex0 = (IntegerArray &)subSurfaceIndex.getLocalArray();
//   IntegerArray & ignoreThisSubSurface0 = (IntegerArray &)ignoreThisSubSurface.getLocalArray();
//   realArray & rProject0  = (realArray&)rProject.getLocalArray(); // these should not be redimensioned
//   realArray & xOld0      = (realArray&)xOld.getLocalArray();
//   realArray & xrProject0 = (realArray&)xrProject.getLocalArray();
//   realArray & normal0    = (realArray&)normal.getLocalArray();

  bool invertUntrimmedSurface=false;
  if( compositeTopology!=NULL && compositeTopology->getTriangulation()!=NULL )
  {
    UnstructuredMapping *uns=compositeTopology->getTriangulation();
    invertUntrimmedSurface=true;  // no need to invert full TrimmedMapping
      
    if( debug & 2 )
      printF("CompositeSurface::project onto the unstructured surface. \n");
	
    if( !mpParams.projectOntoReferenceSurface() )
    {
      // do NOT project onto the reference surface *wdh* 090710

      if( true )
	printF("CompositeSurface::project onto the unstructured surface ONLY. \n"); 

      // Is this correct: ? *wdh* 090710
      xOld=x;
      uns->project(xOld,mpParams);

    }
    else 
    {
      // ------------------------------------------
      // --- project onto the reference surface ---
      // ------------------------------------------

      realArray x2;
      x2=x;
      uns->project(x2,mpParams);

      if( mpParams.getMatchNormals() )
      {
	// When this option is on we should check to see if the closest element is next
	// to a corner in the surface in which case we should choose the closest element
	// to be the one whose normal best matches the input normal.
      }



      const intArray & elementSurface = uns->getTags();
      const int numberOfElements = uns->getNumberOfElements();
     
      // project the points lying on the triangulation onto the reference surface
      CompositeSurface & cs = *this;
    
      Range R3=3;
      realArray x0, r0, xr0;
      for( int i=xBase; i<=xBound; i++ )
      {
	int e = elementIndex(i);  // this is the element we are in!
	assert( e>=0 && e<numberOfElements );

	// fillin the subSurfaceIndex from the elementIndex.
	const int s=elementSurface(e);
	subSurfaceIndex(i)=s;  
	  
	if(  debug & 2 ) 
	  printf("CompositeSurface::project point i=%i projected onto element %i, subSurface =%i\n",i,e,s);

	if( s>=0 && s<numberOfSubSurfaces() )
	{
	  // collect up all consecutive points that belong to this surface
	  int iStart=i, iEnd=i;
	  for( int j=i+1; j<=xBound; j++ )
	  {
	    int e2=elementIndex(j);
	    assert( e2>=0 && e2<numberOfElements );
	    if( elementSurface(e2)==s )
	    {
	      iEnd=j;
	      i++;
	    }
	    else
	    {
	      break;
	    }
	  }
	  if( debug & 4 ) printf("CS:project:project points [%i,%i] onto surface %i (i=%i)\n",iStart,iEnd,s,i);
	    
	  Range J(iStart,iEnd), J0=iEnd-iStart+1;
	  x0.redim(J0,3);
	  r0.redim(J0,2);
	  xr0.redim(J0,3,2);

	  x0(J0,0)=x(J,0); x0(J0,1)=x(J,1); x0(J0,2)=x(J,2); 

	  r0=-1; // We could get an initial guess if we knew the r coordinates of the triangulation
              
	  Mapping *mapPointer = &cs[s];
	  if( cs[s].getClassName()=="TrimmedMapping" )
	    mapPointer = ((TrimmedMapping&)cs[s]).untrimmedSurface();

	  Mapping & subSurface = *mapPointer;

	  subSurface.inverseMap(x0,r0);

	  if( max(fabs(r0)) < 2. )
	  {
	    subSurface.map(r0,x0,xr0);

	    xOld(J,R3)=x0(J0,R3); 

	    // *** we need to compute normals too ****

	    normal(J,axis1)=xr0(J0,1,0)*xr0(J0,2,1)-xr0(J0,2,0)*xr0(J0,1,1);
	    normal(J,axis2)=xr0(J0,2,0)*xr0(J0,0,1)-xr0(J0,0,0)*xr0(J0,2,1);
	    normal(J,axis3)=xr0(J0,0,0)*xr0(J0,1,1)-xr0(J0,1,0)*xr0(J0,0,1);

	    realArray norm;
	    norm = SQRT( SQR(normal(J,0))+SQR(normal(J,1))+SQR(normal(J,2))  ) ;

	    norm=cs.getSignForNormal(s)/max(REAL_MIN,norm);
    
	    int dir;
	    for( dir=0; dir<rangeDimension; dir++ ) 
	      normal(J,dir)*=norm;
	  }
	  else
	  {
	    printf("***CS:project:ERROR: inverting sub-surface %i\n",s);
	  }
	}
	else
	{
	  printf("***CS:project:ERROR: invalid sub-surface, s=%i, numberOfSubSurfaces=%i\n",s,numberOfSubSurfaces());
	}
      } // end for i 
      
    } // end project onto the reference surface
    
  }
  else
  {
    // invert when we don't have a triangulation:
    project( subSurfaceIndex,
	     x,
	     rProject,
	     xOld,
	     xrProject,
	     normal,
	     ignoreThisSubSurface );
  }
  
  x=xOld;
  return 0;
}


void CompositeSurface::
project( intArray & subSurfaceIndex,
	 realArray & x, 
	 realArray & rProject, 
	 realArray & xProject,
         realArray & xrProject,
         realArray & normal /* = Overture::nullRealDistributedArray() */,
         const intArray & ignoreThisSubSurface /* = Overture::nullIntArray() */,
         bool invertUntrimmedSurface /* = false */ )
//===========================================================================
/// \brief  
///    Project the points x(i,0:2) onto the surface. Also return the sub-surface index
///  NOTE: invisible surfaces are ignored when projecting.
/// 
/// \param subSurfaceIndex (input/output) : The index of the sub-surface that the point
///    is closest to. On input this is the index of the previous point (if >= 0)
/// \param x (input) : project these points onto the surface.
/// \param rProject (input/output) : sub-surface coordinates. On input these are an initial
///     guess. On output they are the actual unit square coordinates.
/// \param xProject (input/output) : on input these are the projected points from the previous
///      step (if subSurfaceIndex>=0 on input). On output these are the projected points. These should
///  always have some valid values on input to prevent purify UMR problems.
/// \param xrProject (output) : the derivative of the mapping at xProject
/// \param normal (input/output) : on input this is the normal to the surface at the old point. On output
///    this array then it will hold the normal to the
///     surface, normal(i,0:2). The normal vector will be chosen so that it is consistent
///     across all sub-surfaces
/// \param ignoreThisSubSurface(i) (input) : Optional. Do not consider this sub-surface when
///    projecting point x(i,0:2).
/// \param invertUntrimmedSurface: if tru only invert the untrimmed surface of a trimmed mapping. Use this option
///    if the triangulation has already been used to find the closest sub-surface.
//===========================================================================
{
// AP: testing
  if( !topologyDetermined )
  {
    // determineTopology();
    //kkc 070305 add now resizes signForNormal if( signForNormal.getLength(0)!=numberOfSurfaces )
    if( signForNormal.getLength(0)==0 )
    {
      signForNormal.redim(numberOfSurfaces);
      signForNormal=1;
    }
  }
  
  
  real time0=getCPU();
  
  Range R(x.getBase(0),x.getBound(0));
  int rBase=R.getBase(), rBound=R.getBound();
  realArray rP(R,2), r2(R,2), xx(R,3), xr(R,3,2), xSave(R,3);
  Range Axes(0,1), xAxes(0,2);
  bool ignoreSomeSurfaces = ignoreThisSubSurface.getLength(0) > 0;

  IntegerArray possibleSurface(R), corner(R); 
  possibleSurface=0;
  corner=FALSE;       // corner(i)=TRUE if we have crossed a corner between two sub-surface.

  MappingParameters mapParams;  // use these to get the mask for Trimmed surfaces
  intArray & mask = mapParams.mask;

  IntegerArray startingSurface(R); startingSurface=0;
  intArray initialGuessForSubSurface(R);
  initialGuessForSubSurface=subSurfaceIndex;
  
  IntegerArray index(R);
  index=-1;

  realArray dist(R);

  //
  // found(i) = -1 : found but outside
  //             0 : no possible surfaces found yet
  //             1 : at least one possible surface found
  //             2 : surface has been found
  //             3 : unable to find any surface
  // 
  enum
  {
    foundButOutside=-1,    
    notFound=0,
    maybeFound=1,
    surfaceFound=2,
    unableToFind=3
  };
  enum
  {
    outsideSurface=-1,
    insideSurface=+1
  };
  

  IntegerArray found(R);
  found=notFound; // 0=not found yet, 1=found inside, -1=found but outside, 2=unable to find

  realArray distMin(R);  distMin=REAL_MAX;

  if( Mapping::debug & 2 )
  {
    printf("\n *************CompositeSurface::project points ***********\n");
    if( Mapping::debug & 64 )
      printf("CompositeSurface: found: foundButOutside=%i, notFound=%i, maybeFound=%i, unableToFind=%i \n",
	   foundButOutside,notFound,maybeFound,unableToFind);
    for( int i=rBase; i<=rBound; i++ )
      printf("CS: i=%4i, old x=(%7.2e,%7.2e,%7.2e) new=(%7.2e,%7.2e,%7.2e) tangent=(%7.2e,%7.2e,%7.2e) dist=%6.2e\n",
	     i,xProject(i,0),xProject(i,1),xProject(i,2),x(i,0),x(i,1),x(i,2),
             x(i,0)-xProject(i,0),x(i,1)-xProject(i,1),x(i,2)-xProject(i,2),
             SQRT( SQR(x(i,0)-xProject(i,0))+SQR(x(i,1)-xProject(i,1))+SQR(x(i,2)-xProject(i,2))) );
  }
  
  totalNumberOfProjections+=R.getLength();  // for statistics

  bool cornerFound=FALSE;
  int i;
  for( int s=0; s<numberOfSurfaces; s++ ) // possibly check all sub-surfaces 
  {
    if( Mapping::debug & 2 )
      printf("\n *************CompositeSurface iteration (s=) %i (iterate to check different sub-surfaces) \n ",s);

    if( Mapping::debug & 4 )
      for( int i=rBase; i<=rBound; i++ )
        printf("CompositeSurface: i=%i, found(i)=%i, subSurfaceIndex(i)=%i \n", i,found(i),subSurfaceIndex(i));
    
    bool done=TRUE;
    for( i=rBase; i<=rBound; i++ )
    {
      if( found(i)>maybeFound ) // this point has already been found, or cannot be found, skip it
	continue;
     

      // if no guess is given, choose surface found for the previous point ** not in vector version ***
      //if( initialGuessForSubSurface(i)<0 && i>rBase && subSurfaceIndex(i-1)>0 )
      //  initialGuessForSubSurface(i)=subSurfaceIndex(i-1);

      int subSurfaceToIgnore = ignoreSomeSurfaces ? ignoreThisSubSurface(i) : -1;

      // first check the old sub-surface
      if( s==0 )
	index(i) = initialGuessForSubSurface(i)!=subSurfaceToIgnore ? initialGuessForSubSurface(i) : -1;
      else
	index(i)=-1;

      if( index(i)<0 )
      {
	// look for closest surface based on the bounding boxes
	real minimumDistanceToABox=distMin(i);
	for( int ss=startingSurface(i); ss<numberOfSurfaces; ss++ )
	{
	  if( ss!=subSurfaceToIgnore && ss!=initialGuessForSubSurface(i) && isVisible(ss) )
	  {
	    // check the distance to the bounding box 
	    // const RealArray & boundingBox = surfaces[ss].getMapping().getBoundingBox();
            Mapping & map = surfaces[ss].getMapping();
	    real boxDim=0., distanceToBox=0.;
	    for( int dir=0; dir<rangeDimension; dir++ )
	    {
              real xa=(real)map.getRangeBound(Start,dir), xb=(real)map.getRangeBound(End,dir);
	      
              boxDim=max(boxDim,xb-xa);
	      real dist= max(max(xa-x(i,dir),x(i,dir)-xb),0.);
	      distanceToBox+=SQR(dist);
	    }
	    if( distanceToBox < SQR(boxDim)*.01 )
	    { // point is inside this bounding box
	      index(i)=ss;
	      minimumDistanceToABox=0.;
	      break;
	    }
	    else if( distanceToBox<minimumDistanceToABox )
	    {
	      index(i)=ss;
	      minimumDistanceToABox=distanceToBox;
	    }
	  }
	}
        startingSurface(i)=index(i)>=0 ? index(i)+1 : numberOfSurfaces;  // start search here next time
      }

      if( index(i)<0 )
      { // no more surfaces to check
        if( found(i)==notFound )
          found(i)=unableToFind;     // unable to find a point on the surface
      }
      else
        done=FALSE;
    }
    
    if( done )
      break;
    // now we have defined index(i) to be the next surface to try
  
    int iNext=rBase;
    while( iNext<=rBound )
    {
  
      // collect a contiguous set of points that need to be inverted from the SAME surface
      while( iNext<=rBound && (found(iNext)>maybeFound || index(iNext)<0) )  // skip points we have found already
	iNext++;
      if( iNext>rBound )
	break;
      
      int iStart=iNext, iEnd=iNext;
      int surf=index(iStart);
      while( iEnd<rBound && found(iEnd+1)<=maybeFound && index(iEnd+1)==surf )
	iEnd++;

      iNext=iEnd+1;  // here is where we start the next search

      // Here are the range of points to look at:
      Range Ri(iStart,iEnd);

      if( Mapping::debug & 2 )
	printf("*** try to invert sub-surface %i for points (%i,%i) \n",surf,iStart,iEnd);

      bool trimmedSurface = surfaces[surf].getClassName()=="TrimmedMapping";

      // invert the surface mapping.
      rP(Ri,Axes)=rProject(Ri,Axes);  // initial guess for inverse.
      real time1=getCPU();
      if( trimmedSurface )
      {
	if( invertUntrimmedSurface )
	{
          TrimmedMapping & trim=(TrimmedMapping&)(surfaces[surf].getMapping());
	  trim.untrimmedSurface()->inverseMapC(x(Ri,xAxes),rP(Ri,Axes));
	}
	else
	  surfaces[surf].inverseMapC(x(Ri,xAxes),rP(Ri,Axes),Overture::nullRealDistributedArray(),mapParams);  
      }
      else
      {
        surfaces[surf].inverseMapC(x(Ri,xAxes),rP(Ri,Axes));
      }
      timing[timeToProjectInvertMapping]+=getCPU()-time1;
      
      // evaluate the mapping to find the projected point
      // first make sure the r values are in [0,1]
      r2(Ri,Axes)=max(0.,min(1.,rP(Ri,Axes)));  

      totalNumberOfProjectInverses+=Ri.getLength();  // for statistics
      
      time1=getCPU();
      if( trimmedSurface )
      {
	// for a TrimmedMapping we evaluate the untrimmed surface since the point r has 
	// already been projected onto the valid part of the TrimmedMapping
	TrimmedMapping & trim =(TrimmedMapping &) surfaces[surf].getMapping();
	trim.untrimmedSurface()->mapC(r2(Ri,Axes),xx(Ri,xAxes),xr(Ri,xAxes,Axes));
      }
      else
	surfaces[surf].mapC(r2(Ri,Axes),xx(Ri,xAxes),xr(Ri,xAxes,Axes));

      timing[timeToProjectEvaluateMapping]+=getCPU()-time1;
        // ***** where ***
      for( i=iStart; i<=iEnd; i++ )
      {
	if( trimmedSurface && !invertUntrimmedSurface )
	{
	  possibleSurface(i)=mask(i); // mask returned from inverseMap, -1=outside, 1=inside
	}
	else
	  possibleSurface(i) = fabs(rP(i,0)-.5)<=.5 && fabs(rP(i,1)-.5)<=.5 ?  insideSurface : outsideSurface;
      }
	
      // dist(i) = distance from x to it's projected value.
      dist(Ri) = SQR(xx(Ri,0)-x(Ri,0))+SQR(xx(Ri,1)-x(Ri,1))+SQR(xx(Ri,2)-x(Ri,2));

      if( s==0  ) // *wdh* 980915 
      {
	// If we have moved off the previous surface we were on then increase the distance to this
        // previous surface a bit so that we prefer to move onto another nearby surface. 
        // This helps us go around corners.
	// Add on the distance between the current point and the previous point

        
        // stepLength = distance from last position to the next
	const realArray & stepLength=evaluate(SQR(xProject(Ri,0)-x(Ri,0))+
					      SQR(xProject(Ri,1)-x(Ri,1))+
					      SQR(xProject(Ri,2)-x(Ri,2)));

	  /* ----
	  where( initialGuessForSubSurface(Ri)>=0 && possibleSurface(Ri)==outsideSurface )
	  {
	    dist(Ri) += stepLength(Ri);
	  }
         --- */
        if( TRUE )
	{
	  // check for corners. If we have gone around a sharp corner then total step length may
          // have decreased significantly. We need to correct this.
	  for( i=iStart; i<=iEnd; i++ )
	  {
	    if(	possibleSurface(i)==outsideSurface && initialGuessForSubSurface(i)>=0 && subSurfaceIndex(i)>=0 &&
		rP(i,0)!=Mapping::bogus )
	    {
	      const real dist1=SQR(xProject(i,0)-xx(i,0))+SQR(xProject(i,1)-xx(i,1))+SQR(xProject(i,2)-xx(i,2));
	      // *** if(  dist1<.75*dist2 || dist2<.75*dist1)  // note that the distances are squared .75*.75=
	      if(  dist1<.75*stepLength(i) )
	      {
                // ***** need to check distance for actual point found on the composite surface****
		if( Mapping::debug & 2 )
		{
		  printf("***CS: point %4i moved off previous surface, maybe a corner. stepLength=%e, "
			 "ds_new=%e, xx=(%8.3e,%8.3e,%8.3e)\n",i,SQRT(stepLength(i)),SQRT(dist1),
			 xx(i,0),xx(i,1),xx(i,2));
		}
		
                dist(i)+=stepLength(i);  // increase this so we prefer to move around the corner.
		
		corner(i)=TRUE;            // this is a corner.
		cornerFound=TRUE;
		xSave(i,xAxes)=x(i,xAxes); // save orginal x
                // replace x by it's projected value on the boundary 
		// ***no*** x(i,xAxes)=xx(i,xAxes);   // ******************** this could be wrong ***********

	      }
	      else
	      {
		printf("***CS: point %4i has moved off the previous surface, NO corner? dist1=%e, stepLength=%e\n",
		       i,dist1,SQRT(stepLength(i)));
	      }
	    }
	  }
	}
	
      }

      if( Mapping::debug & 2 )
	for( i=iStart; i<=iEnd; i++ )
	  printf("CS: surface %i : point i=%4i, rP=(%8.3e,%8.3e), dist=%7.2e, possibleSurface=%s,"
		 " best so far=%i \n",
		 surf,i,rP(i,0),rP(i,1),dist(i),possibleSurface(i)==1 ? "inside " : "outside",
		 subSurfaceIndex(i));
      
    }
    
    for( i=rBase; i<=rBound; i++ )
    {
      int surf=index(i);
      if( surf>=0 && possibleSurface(i) && found(i)!=surfaceFound )
      {
        if( possibleSurface(i)==insideSurface && surf==initialGuessForSubSurface(i) )
	{
	  if( fabs(rP(i,0)-.5)<=.5 && fabs(rP(i,1)-.5)<=.5 ) // ****** need ?? ******** is this correct? ******
  	  { // we are inside the same surface as the last step, assume this is good if we are close
	    // compute distance from the last point
	    real stepLength=SQR(xx(i,0)-xProject(i,0))+SQR(xx(i,1)-xProject(i,1))+SQR(xx(i,2)-xProject(i,2));

	    distMin(i)=dist(i);
	    subSurfaceIndex(i)=surf;
	    rProject(i,Axes)=rP(i,Axes);
	    xProject(i,xAxes)=xx(i,xAxes);
	    xrProject(i,xAxes,Axes)=xr(i,xAxes,Axes);

            const real stepLengthFactor=.25;  // =1.

             // subSurfaceIndex(i)<0 on the first time thru
	    if( dist(i)<=stepLengthFactor*stepLength || subSurfaceIndex(i)<0 )
	      found(i)=surfaceFound;
	    else
	    {  // not close, call it "outside"
	      found(i)=foundButOutside;
              if( debug & 2 )
  	        printf("*** CS:WARNING pt %3i inside same surface %i, as before, but not close?? "
                       "dist=%e > stepLength=%e*** \n", i,surf,SQRT(dist(i)),SQRT(stepLength));
	    }
	  }
	  else
	  {
            if( Mapping::debug & 2 )
              printf("*** CS:point has crossed the boundary of surface %i, maybe moving to a new surface \n",surf);
	  }
	}
        // **** if not marching: if both surfaces are close, choose the one that keeps the normal continuous
/* -------
        if( !marching && i>rBase && dist(i)< eps )
	{
	  newNormal(0,xAxes) = 0;
	  if( newNormal.normal(i-1) > normal(i).normal(i-1) )
	  {
            // prefer this new point.
	  }
	}
----- */
        if( dist(i)<distMin(i) )
	{
          // this surface is closer than any previous
          found(i)=possibleSurface(i)==outsideSurface ? foundButOutside : maybeFound;  
          distMin(i)=dist(i);
	  subSurfaceIndex(i)=surf;
          rProject(i,Axes)=rP(i,Axes);
          xProject(i,xAxes)=xx(i,xAxes);
          xrProject(i,xAxes,Axes)=xr(i,xAxes,Axes);
	}
      }
    }
    
    if( min(found)>maybeFound)   // all points have been treated 
      break;

  }  // end for (s)
  // now save the results
  bool determineNormals = normal.getBase(0) <= rBase && normal.getBound(0) >= rBound  && normal.getLength(1) >=3 ;

  if( !cornerFound )
  {
    if( determineNormals )
      getNormals(subSurfaceIndex, xrProject, normal);
  }
  else
  {
    realArray oldNormal,tangent(1,3),b(1,3);
    oldNormal=normal;
    
    assert( determineNormals );
    getNormals(subSurfaceIndex, xrProject, normal);
    const real epsilon=REAL_EPSILON*100.;
    for( i=rBase; i<=rBound; i++ )
    {
      if( corner(i) )
      {
	// we have gone over a corner -- re-adjust the position x, which was previously changed to be
	// the intersection point with the corner, to be approximately the correct distance along the new surface
        //
        //                   xOld   xP       xSave
        //       =============O=====O-------->O
        //                           \
        //                             \
        //                              \
        //                               X <------- rotate tangent around to here

	tangent(0,xAxes)=xSave(i,xAxes)-xProject(i,xAxes);

	// we need to determine a new tangent vector 
	// rotate the tangent vector around the corner by an angle theta.
	// theta is the angle between the old normal and the new normal.
	const real cosTheta = sum(oldNormal(i,xAxes)*normal(i,xAxes));
           
        if( cosTheta<.75 ) // no need to adjust if cos(theta) is near +1.
	{
	  // b = the vector orthogonal to oldNormal and in the plane of oldNormal and normal
	  // (b is not the tangent since the tangent may cross the corner at an angle).
	  b(0,xAxes)=normal(i,xAxes)-cosTheta*oldNormal(i,xAxes);
	  real normB =sum(b*b);
	  if( fabs(normB)>epsilon )
	  {
	    b*=1./SQRT(normB);
	  
	    const real sinTheta = sum(b*normal(i,xAxes));

	    // tangent = alpha*a() + beta*b() + gamma*c()
	    //       a() = old normal
	    //       c() = new Normal X old Normal (normalized)
	    //       b() = a() X c() =  normal() - cosTheta*oldNormal()  (normalized)
	    // new tangent = (alpha*cos-gamma*sin) a() + (+alpha*sin+beta*cos) b() + gamma c()
	    //             = oldTangent + (alpha*(cos-1)+gamma*sin) a() + (-alpha*sin+beta*(cos-1)) b()
	    real alpha=sum(tangent*oldNormal(i,xAxes));
	    real beta=sum(tangent*b);
	    tangent(0,xAxes)+=(alpha*(cosTheta-1.)-beta*sinTheta)*oldNormal(i,xAxes) +
	      (alpha*sinTheta+beta*(cosTheta-1.))*b(0,xAxes);
      
      
	    // findOutwardTangent( Mapping & map, const realArray & r, const realArray & x, realArray & outwardTangent );
      
	    xProject(i,xAxes)+=tangent(0,xAxes);
	    printf("CS: Moving pt %4i around the corner n=(%4.1f,%4.1f,%4.1f), t=(%4.1f,%4.1f,%4.1f), cos=%6.2e"
		   " sin=%6.2e b=(%4.1f,%4.1f,%4.1f), n_old=(%4.1f,%4.1f,%4.1f)\n",i,
		   normal(i,0),normal(i,1),normal(i,2), tangent(0,0),tangent(0,1),tangent(0,2),cosTheta,sinTheta,
		   b(0,0),b(0,1),b(0,2), oldNormal(i,0),oldNormal(i,1),oldNormal(i,2));
	  }
	  else if( cosTheta<0. )
	  {
	    // this must be a 180 degree turn! really need to get the tangent to the edge in this case
	    printf("CompositeSurface:project:WARNING: the corner has apparently rotated by 180 degrees, cos(theta)=%e\n"
		   "Setting t -> -t , this case should be handled in a better way",cosTheta);
	  
	    xProject(i,xAxes)-=tangent(0,xAxes);  // reverse the tangent
	  }
	
	  // re-project this point
	  if( TRUE )   // ***** fix this *** duplicate code from above
	  {
	    // invert the surface mapping.
	    int surf=subSurfaceIndex(i);
	    const bool trimmedSurface = surfaces[surf].getClassName()=="TrimmedMapping";

	    rP(i,Axes)=rProject(i,Axes);
	    if( trimmedSurface )
	    {
	      if( invertUntrimmedSurface )
	      {
		TrimmedMapping & trim=(TrimmedMapping&)(surfaces[surf].getMapping());
		trim.untrimmedSurface()->inverseMapC(xProject(i,xAxes),rP(i,Axes));
	      }
	      else
		surfaces[surf].inverseMapC(xProject(i,xAxes),rP(i,Axes),
					   Overture::nullRealDistributedArray(),mapParams);  
	    }
	    else
	    {
	      surfaces[surf].inverseMapC(xProject(i,xAxes),rP(i,Axes));
	    }
      
	    // evaluate the mapping to find the projected point
	    // first make sure the r values are in [0,1]
	    r2(i,Axes)=max(0.,min(1.,rP(i,Axes)));  
	    if( trimmedSurface )
	    {
	      // for a TrimmedMapping we evaluate the untrimmed surface since the point r has 
	      // already been projected onto the valid part of the TrimmedMapping
	      TrimmedMapping & trim =(TrimmedMapping &) surfaces[surf].getMapping();
	      trim.untrimmedSurface()->mapC(r2(i,Axes),xx(i,xAxes),xr(i,xAxes,Axes));
	    }
	    else
	      surfaces[surf].mapC(r2(i,Axes),xx(i,xAxes),xr(i,xAxes,Axes));
	  
	    rProject(i,Axes)=r2(i,Axes);
	    xProject(i,xAxes)=xx(i,xAxes);
	    xrProject(i,xAxes,Axes)=xr(i,xAxes,Axes);
	  }
	}

	x(i,xAxes)=xSave(i,xAxes); // restore original point
	
      }
      
    }
    // **** recompute the normals *****
    if( determineNormals )
      getNormals(subSurfaceIndex, xrProject, normal);
    
  }  // end else corner found

  
  if( Mapping::debug & 2 )
    printf("\n --------------- CS: done projecting. Here are the results: ---------------------\n");
    
  for( i=rBase; i<=rBound; i++ )
  {
    if( Mapping::debug & 4 )
      printf("CompositeSurface: i=%5i, found(i)=%i, subSurfaceIndex(i)=%i \n", i,found(i),subSurfaceIndex(i));
    
    if( found(i)==notFound || found(i)==unableToFind )
    {
      if( Mapping::debug & 2 )
	printf("***CompositeSurface::project:ERROR: point i=%5i, x=(%e,%e,%e) : unable to project! \n",
	       i,x(i,0),x(i,1),x(i,2));
      subSurfaceIndex(i)=-1;
      xrProject(i,Range(0,2),Range(0,1))=0.;
      xrProject(i,0,0)=1.;
      xrProject(i,1,1)=1.;
    }
    else if( found(i)==foundButOutside )
    {
      if( Mapping::debug & 2 )
	printf("***CS::project: i=%5i, xProject=(%10.3e,%10.3e,%10.3e) n=(%4.2f,%4.2f,%4.2f) (subSurf %i): outside but close! \n",
	       i,x(i,0),x(i,1),x(i,2),normal(i,0),normal(i,1),normal(i,2),subSurfaceIndex(i));
    }
    else
    {
      if( Mapping::debug & 2 )
	printf("***CS::project: i=%5i, xProject=(%10.3e,%10.3e,%10.3e) n=(%4.2f,%4.2f,%4.2f)  (subSurf %i) \n",
	       i,xProject(i,0),xProject(i,1),xProject(i,2),normal(i,0),normal(i,1),normal(i,2),subSurfaceIndex(i));
    }
  }
  
  timing[timeToProject]+=getCPU()-time0;
  timing[totalTime]+=getCPU()-time0;
}

void CompositeSurface::
oldProject( intArray & subSurfaceIndex,
	    realArray & x, 
	    realArray & rProject, 
	    realArray & xProject,
	    realArray & xrProject,
	    realArray & normal /* = Overture::nullRealDistributedArray() */,
	    const intArray & ignoreThisSubSurface /* = Overture::nullIntArray() */ )
//===========================================================================
// /Purpose: 
//   Project the points x(i,0:2) onto the surface. Also return the sub-surface index
//
// /subSurfaceIndex (input/output) : The index of the sub-surface that the point
//   is closest to. On input this is the index of the previous point (if >= 0)
// /rProject (input/output) : sub-surface coordinates. On input these are an initial
//    guess. On output they are the actual unit square coordinates.
// /xProject (output) : the projected points.
// /xrProject (output) : the derivative of the mapping at xProject
// /normal (output) : if there is space in this array then it will hold the normal to the
//    surface, normal(i,0:2). The normal vector will be chosen so that it is consistent
//    across all sub-surfaces
// /ignoreThisSubSurface(i) (input) : Optional. Do not consider this sub-surface when
//   projecting point x(i,0:2).
//===========================================================================
{
  if( !topologyDetermined )
    determineTopology();
  

  Range R(x.getBase(0),x.getBound(0));
  int rBase=R.getBase(), rBound=R.getBound();
  realArray rr(1,2), xx(1,3), xr(1,3,2);
  Range Axes(0,1), xAxes(0,2);
  bool ignoreSomeSurfaces = ignoreThisSubSurface.getLength(0) > 0;

  MappingParameters mapParams;  // use these to get the mask for Trimmed surfaces
  intArray & mask = mapParams.mask;

  IntegerArray surfaceWasChecked(numberOfSurfaces);
  

  real dist;
  for( int i=rBase; i<=rBound; i++ )
  {
    if( Mapping::debug & 2 )
      printf("\n *************CompositeSurface::project point i=%i x=(%e,%e,%e)***********\n ",i,
         x(i,0),x(i,1),x(i,2));

    int found=0; // 0=not found, 1=found inside, 2=found but outside
    surfaceWasChecked=FALSE;
    
    int initialGuessForSubSurface=subSurfaceIndex(i);
    // if no guess is given, choose surface found for the previous point
    if( initialGuessForSubSurface<0 && i>rBase && subSurfaceIndex(i-1)>0 )
      initialGuessForSubSurface=subSurfaceIndex(i-1);
    int index;
    real distMin=REAL_MAX;
    for( int s=0; s<numberOfSurfaces; s++ ) // check all subsurfaces if necessary
    {
      int possibleSurface=0;
      int subSurfaceToIgnore = ignoreSomeSurfaces ? ignoreThisSubSurface(i) : -1;
      // first check the old sub-surface
      if( s==0 )
        
        index = initialGuessForSubSurface!=subSurfaceToIgnore ? initialGuessForSubSurface : -1;
      else
        index=-1;

      if( index<0 )
      {
        // look for closest surface based on the bounding boxes
        real minimumDistanceToABox=distMin;
        for( int ss=0; ss<numberOfSurfaces; ss++ )
	{
	  if( !surfaceWasChecked(ss) && ss!=subSurfaceToIgnore )
	  {
            // check the distance to the bounding box 
            const RealArray & boundingBox = surfaces[ss].getMapping().approximateGlobalInverse->getBoundingBox();
	    real distanceToBox=0.;
	    for( int dir=0; dir<rangeDimension; dir++ )
	    {
	      real dist= max(max(boundingBox(Start,dir)-x(i,dir),x(i,dir)-boundingBox(End,dir)),0.);
	      distanceToBox+=SQR(dist);
	    }
            if( distanceToBox == 0. )
	    { // point is inside this bounding box
	      index=ss;
              minimumDistanceToABox=0.;
	      break;
	    }
            else if( distanceToBox<minimumDistanceToABox )
	    {
	      index=ss;
	      minimumDistanceToABox=distanceToBox;
	    }
	  }
	}
      }

      if( index<0 )
        break;      // no more surfaces to check
      
      surfaceWasChecked(index)=TRUE;

      if( Mapping::debug & 2 )
        printf(" try to invert sub-surface %i \n",index);

      if( surfaces[index].getClassName()!="TrimmedMapping" )
      {
        // invert non-trimmed surface
        surfaces[index].inverseMapC(x(i,xAxes),rProject(i,Axes));

        if( Mapping::debug & 2 )
          printf(" non-trimmed surface : rProject=(%e,%e)",rProject(i,0),rProject(i,1));
        if( fabs(rProject(i,0)-.5)<=.51 && fabs(rProject(i,1)-.5)<=.51 )
	{ // inside or close
          rr(0,Axes)=max(0.,min(1.,rProject(i,Axes)));  // **** wdh
          surfaces[index].map(rr,xx,xr);

          possibleSurface=fabs(rProject(i,0)-.5)<=.5 && fabs(rProject(i,1)-.5)<=.5 ?  1 : 2;
	  dist = SQR(xx(0,0)-x(i,0))+SQR(xx(0,1)-x(i,1))+SQR(xx(0,2)-x(i,2));
	}
      }
      else
      {
	const TrimmedMapping & trim = (TrimmedMapping &) surfaces[index].getMapping();
	
        // invert trimmed surface
        trim.surface->inverseMapC(x(i,xAxes),rProject(i,Axes));

        if( Mapping::debug & 2 )
          printf(" trimmed surface: rProject=(%e,%e) \n",rProject(i,0),rProject(i,1));
        // evaluate the trimmed surface to see if the point is inside
        rr(0,Axes)=rProject(i,Axes);
        surfaces[index].map(rr,xx,xr,mapParams);

        if( Mapping::debug & 2 )
          printf(" after evaluating trimmed surface, mask=%i",mask(0,0));

        if( mask(0,0)>0 || trim.distanceToBoundary(0)<1.e-2 )  // ***
	{
          possibleSurface= mask(0,0)>0 ? 1 : 2;  // 1=inside, 2 = outside but may be close to trimmed boundary
          if( Mapping::debug & 2 && mask(0,0)<=0 )
            printf("*** close to a boundary of a trimmed surface *** ");
          dist = SQR(xx(0,0)-x(i,0))+SQR(xx(0,1)-x(i,1))+SQR(xx(0,2)-x(i,2));
          if( initialGuessForSubSurface>=0 && possibleSurface== 2 )
	  { // this is approximate *** ***** xProject may not be defined the first time ****
            dist += (SQR(xProject(i,0)-x(i,0))+SQR(xProject(i,1)-x(i,1))+SQR(xProject(i,2)-x(i,2)));
	  }
	}
        else
	{
          if( Mapping::debug & 2 )
            printf("*** from trimmed surface: trim.distanceToBoundary(i)=%e \n",trim.distanceToBoundary(0));
	}
      }
      if( possibleSurface )
      {
        if( Mapping::debug & 2 )
          printf(", distance=%e \n",dist);
        if( possibleSurface==1 && index==initialGuessForSubSurface )
	{
	  if( fabs(rProject(i,0)-.5)<=.5 && fabs(rProject(i,1)-.5)<=.5 )
  	  { // we are inside the same surface as the last step, assume this is good if we are close
	    // compute distance from the last point
	    real stepLength=SQR(xx(0,0)-xProject(i,0))+SQR(xx(0,1)-xProject(i,1))+SQR(xx(0,2)-xProject(i,2));
	    if( dist<=stepLength )
	    {
              found=possibleSurface;
	      distMin=dist;
	      subSurfaceIndex(i)=index;
	      xProject(i,xAxes)=xx(0,xAxes);
	      xrProject(i,xAxes,Axes)=xr(0,xAxes,Axes);
	      break;
	    }
            else
	    {
	      if( Mapping::debug & 2 )
		printf(" *** INFO inside initial surface %i, but not close?? *** \n",index);
	    }
	  }
	  else
	  {
            if( Mapping::debug & 2 )
              printf(" *** point has crossed the boundary of surface %i, maybe moving to a new surface \n",index);
	  }
	}
        if( dist<distMin )
	{
          found=possibleSurface;
          distMin=dist;
	  subSurfaceIndex(i)=index;
          xProject(i,xAxes)=xx(0,xAxes);
          xrProject(i,xAxes,Axes)=xr(0,xAxes,Axes);
	}
      }
      else
      {
        if( Mapping::debug & 2 )
          printf("\n");
      }
    }
    if( found==0 )
    {
      if( Mapping::debug & 2 )
        printf("***CompositeSurface::project: point i=%i, x=(%e,%e,%e) : unable to project! \n",i,x(i,0),x(i,1),x(i,2));
      subSurfaceIndex(i)=-1;
      xrProject(i,Range(0,2),Range(0,1))=0.;
      xrProject(i,0,0)=1.;
      xrProject(i,1,1)=1.;
    }
    else if( found==2 )
    {
      if( Mapping::debug & 2 )
        printf("***CompositeSurface::project: point i=%i, x=(%e,%e,%e) (from subSurface %i): outside but close! \n",
                i,x(i,0),x(i,1),x(i,2),subSurfaceIndex(i));
    }
  }
  bool determineNormals = normal.getBase(0) <= rBase && normal.getBound(0) >= rBound  && normal.getLength(1) >=3 ;
  
  if( determineNormals )
    getNormals(subSurfaceIndex, xrProject, normal);

}







void CompositeSurface::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  This routine should not normally be called
//=====================================================================================
{
  cout <<" CompositeSurface:ERROR: map called \n";
  // evaluate the first sub surface for something to do
  if( numberOfSurfaces>0 )
    surfaces[0].map(r,x,xr,params);

}

int & CompositeSurface::
getSignForNormal(int s) const
// ===================================================================================================
/// \details 
///      Return the sign of the normal for sub-surface s, either +1 or -1;  In order to orient the
///  normals to the sub-surfaces in the same direction it may be necessary to reverse the normals
///  of some sub-surfaces.
///  kkc 070305 changed to reference so we can change the sign from outside the code
// ===================================================================================================
{
  return signForNormal(s);
}

int CompositeSurface::
setTolerance(real tol)
// ===================================================================================================
/// \details 
///  Set the tolerance for how well the surfaces match (may come from the CAD file)
// ===================================================================================================
{
  tolerance=tol;
  return 0;
}

  // get the tolerance
real CompositeSurface::
getTolerance() const
// ===================================================================================================
/// \details 
///  Get the tolerance for how well the surfaces match (may come from the CAD file)
// ===================================================================================================
{
  return tolerance;
}


void CompositeSurface::
eraseCompositeSurface(GenericGraphicsInterface &gi, int surface /* = -1 */)
// ==============================================================================
/// \details 
///    purge all display lists if surface = -1, otherwise, just purge one list
/// \param surface (input): purge the display lists for this surface. By default purge all lists.
// ==============================================================================
{
  if( !gi.isGraphicsWindowOpen() )
   return;

  int sStart, sEnd;
  if (surface == -1)
  {
    sStart = 0;
    sEnd = numberOfSubSurfaces();
  }
  else if (surface >= 0 && surface < numberOfSubSurfaces())
  {
    sStart = surface;
    sEnd = surface+1;
  }
  else
  {
    printf("eraseCompositeSurface: surface = %i out of bounds\n",surface);
    return;
  }
  int s;
  for( s=sStart; s<sEnd; s++)
  {
    gi.deleteList(dList(boundary, s));
    gi.deleteList(dList(gridLines, s));
    gi.deleteList(dList(shadedSurface, s));
//      if (glIsList(dList(boundary, s))) glDeleteLists(dList(boundary, s), 1);
//      if (glIsList(dList(gridLines, s))) glDeleteLists(dList(gridLines, s), 1);
//      if (glIsList(dList(shadedSurface, s))) glDeleteLists(dList(shadedSurface, s), 1);
    dList(boundary, s) = 0;
    dList(gridLines, s) = 0;
    dList(shadedSurface, s) = 0;
  }
  gi.redraw(); // redraw all display lists
};



int CompositeSurface::
findBoundaryCurves(int & numberOfBoundaryCurves, Mapping **& boundaryCurves )
// ===================================================================================================
/// \details 
///      Locate boundary curves on a CompositeSurface. Merge boundary edge curves that form a 
///    smooth portion of the boundary.
/// \param numberOfBoundaryCurves (output) : number of boundary curves found.
/// \param boundaryCurves (output) : Boundary curves.
// ===================================================================================================
{
  if( compositeTopology!=NULL )
  {
    return compositeTopology->findBoundaryCurves(numberOfBoundaryCurves,boundaryCurves);
  }
  else
  {
    numberOfBoundaryCurves=0;
  }

  return 0;
}


int CompositeSurface::
get( const GenericDataBase & dir, const aString & name)    // get from a database file
// ========================================================================================
// /Description:
//   get from a data base file.
// ========================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
//  dir.find(subDir,name,"Mapping");
// AP: First check if the directory exists, and if not return with an error instead of throwing an error
// inside find!!!

  int retCode = dir.locate(subDir,name,"Mapping");
  if ( retCode!=0 )
  {
    printf("Warning: CompositeSurface::get could not find directory `%s'\n", SC name);
    return retCode;
  }
  subDir.setMode(GenericDataBase::streamInputMode);

  aString surfaceClassName;

  subDir.get( CompositeSurface::className,"className" ); 
  if( CompositeSurface::className != "CompositeSurface" )
  {
    cout << "CompositeSurface::get ERROR in className!, got=[" 
	 << (const char *) CompositeSurface::className 
         << "]" << endl;
  }

  subDir.get(numberOfSurfaces,"numberOfSurfaces");
//  printf("CompositeSurface::get: numberOfSurfaces=%i \n",numberOfSurfaces);
  
// display list stuff
  dList.resize(numberOfDLProperties,numberOfSurfaces+20); // same size as in add()
  Range dlRange(boundary,numberOfDLProperties-1);
  Range allSurfaces(0,numberOfSurfaces-1);
  
  dList(dlRange, allSurfaces) = 0; // initialize to zero

  char buff[80];
  int i;
  for( i=0; i<numberOfSurfaces; i++ )
  {
    sprintf(buff,"surfaceClassName%4.4i",i);
    subDir.get(surfaceClassName,buff);
//    cout << "sub surface " << i << ", className = " << surfaceClassName << endl;
    
    // check the most likely candidates first, for speed
    Mapping *mapPointer;
    if( surfaceClassName=="NurbsMapping" )
      mapPointer=new NurbsMapping;
    else if( surfaceClassName=="TrimmedMapping" )
      mapPointer=new TrimmedMapping;
    else if( surfaceClassName=="ComposeMapping" )
      mapPointer=new ComposeMapping;
    else
      mapPointer= Mapping::makeMapping( surfaceClassName ); 
    if( mapPointer==NULL )
    {
      cout << "CompositeSurface::get:ERROR unable to make sub-surface number " << i << endl;
      return 1;
    }
// AP: Incrementing the reference count for mapPointer
    mapPointer->incrementReferenceCount();

    sprintf(buff,"surface%4.4i",i);
    surfaces.addElement(*mapPointer); // this causes the following calls:
// 1. MappingRC(Mapping &) to make a MappingRC out of a Mapping&
// 2. ListOfMappingRC::addElement( MapppingRC& ) which calls
// 3. MappingRC default constructor, which calls
// 4. Mapping default constructor
// 5. Upon exit of addElement, the MappingRC object goes out of scope and ~MappingRC is called
// 6. which calls ~Mapping for the mapping created in step 4
// Unbelievable!
    surfaces[i].get(subDir,buff);
    if (mapPointer->decrementReferenceCount() == 0)
      delete mapPointer;
    
    
  }
  subDir.get(visible,"visible");
  subDir.get(surfaceIdentifier,"surfaceIdentifier");
// AP: surfaceColour must always have the same length as visible, otherwise the code
// can crash when adding new mappings in add()
  const int surfaceColourDimension=visible.getLength(0);
  if( surfaceColourDimension>0 )
  {
    surfaceColour = new aString [surfaceColourDimension];
  }
  else
    surfaceColour=NULL;
  subDir.get(surfaceColour,"surfaceColour", numberOfSurfaces);
  subDir.get(topologyDetermined,"topologyDetermined");
  subDir.get(signForNormal,"signForNormal");

// set bounds and initialize display lists
// we need the bounds before we call compositeTopology->get()
  if( numberOfSurfaces>=1 )
  {
    dList.redim(numberOfDLProperties,numberOfSurfaces+20);
    dList=0;
    
    Bound b;
    realArray xBound(2,3); xBound=0.;
    for( i=0; i<numberOfSurfaces; i++)
    {
      Mapping & map = (*this)[i];
      for( int axis=0; axis<map.getRangeDimension(); axis++ )
      {
	if( i==0 )
	{
	  xBound(Start,axis)= REAL_MAX;
	  xBound(End  ,axis)=-REAL_MAX;
	}
	b = map.getRangeBound(Start,axis);
	if( b.isFinite() )
	  xBound(Start,axis)=min(xBound(Start,axis),(real)b);
	b = map.getRangeBound(End,axis);
	if( b.isFinite() )
	  xBound(End,axis)=max(xBound(End,axis),(real)b);
      }
    }
    for( int axis=0; axis<getRangeDimension(); axis++ )
      for( int side=Start; side<=End; side++ )
	setRangeBound(side,axis,xBound(side,axis));
  }
  

  int compositeTopologySaved;
  subDir.get(compositeTopologySaved,"compositeTopologySaved");
  if( compositeTopologySaved )
  {
    if( compositeTopology==NULL )
      compositeTopology = new CompositeTopology(*this);
    compositeTopology->get(subDir,"compositeTopology");
  }

  subDir.get(tolerance,"tolerance");
  
  Mapping::get( subDir, "Mapping" );

//  initialize();

  
  delete &subDir;
  return 0;
}


int CompositeSurface::
put( GenericDataBase & dir, const aString & name) const    // put to a database file
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  // output this class in streaming mode to be efficient
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put(CompositeSurface::className,"className");
  subDir.put(numberOfSurfaces,"numberOfSurfaces");
  char buff[80];
  for( int i=0; i<numberOfSurfaces; i++ )
  {
    real time0=getCPU();
    
    sprintf(buff,"surfaceClassName%4.4i",i);
    aString surfaceClassName=surfaces[i].getClassName();
    // cout << "CompositeSurface::put: surfaceClassName =[" << surfaceClassName << "]\n";
    
    subDir.put(surfaceClassName,buff);
    sprintf(buff,"surface%4.4i",i);
    surfaces[i].put(subDir,buff);

//    if( numberOfSurfaces>100 &&  i>100 )
//    {
//      printf("CompositeSurface::put surface %i, cpu time=%e \n",i,getCPU()-time0);
//      dir.printStatistics();
//    }
  }

  subDir.put(visible,"visible");
  subDir.put(surfaceIdentifier,"surfaceIdentifier");
  subDir.put(surfaceColour,"surfaceColour", numberOfSurfaces); 
  subDir.put(topologyDetermined,"topologyDetermined");
  subDir.put(signForNormal,"signForNormal");

  int compositeTopologySaved=compositeTopology !=NULL;
  subDir.put(compositeTopologySaved,"compositeTopologySaved");
  if( compositeTopologySaved )
    compositeTopology->put(subDir,"compositeTopology");

  subDir.put(tolerance,"tolerance");

  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}


Mapping* CompositeSurface::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==CompositeSurface::className )
    retval = new CompositeSurface();
  return retval;
}

static bool
addPrefix(aString cmd[], const aString & prefix)
// ==============================================================================================
// /Description:
//    Add a prefix string to the start of every command.
// /cmd (input/output) : null terminated array of strings.
// /prefix (input) : all this string as a prefix.
// ==============================================================================================
{
    
  int i;
  for( i=0; cmd[i]!=""; i++ )
    cmd[i]=prefix+cmd[i];

  return true;
}

bool 
CompositeSurface::
computeTopology(GenericGraphicsInterface &gi)
//===========================================================================
/// \details 
///    Attempt to determine the topology automatically. You should set variables
///  such as the arclength and maximum area (that control the topology global triangulation)
///  before calling this function. To set these variables use getCompositeTopology() to access
///  the CompositeToplogy object and then use the appropriate member functions of that class.
//===========================================================================
{       
  if ( !compositeTopology )
    compositeTopology = new CompositeTopology(*this);

  compositeTopology->invalidateTopology();
  topologyDetermined = compositeTopology->computeTopology(gi);
  if( topologyDetermined )
  {
    topologyDetermined=true;
    signForNormal.redim(0);
    signForNormal=compositeTopology->getSignForNormal();
  }

  return topologyDetermined;
}

void CompositeSurface::
updateTopology()
//===========================================================================
/// \details 
///     Update the topology interactively.
//===========================================================================
{
  if( compositeTopology==NULL )
    compositeTopology= new CompositeTopology(*this);
  compositeTopology->update();
  if( compositeTopology->topologyDetermined() )
  {
    topologyDetermined=true;
    signForNormal.redim(0);
    signForNormal=compositeTopology->getSignForNormal();
  }
}

// =========================================================================================
/// \brief Refine the grid spacing or triangulation of a sub-surface. This is 
//    normally used when plotting the surface.
/// \param s (input) : sub-surface to refine
// =========================================================================================
int CompositeSurface::
refineSubSurface( const int s )
{
  if( s<0 || s>numberOfSubSurfaces() )
  {
    printF("CompositeSurface::refineSubSurface: invalid sub-surface s=%i. There are %i sub-surfaces\n",
	   s,numberOfSubSurfaces() );
    return 1;
  }

  const int maxNumberOfTriangles=500000;

  Mapping & map = (*this)[s];
  if( map.getClassName()=="TrimmedMapping" )
  {
    TrimmedMapping & trim = (TrimmedMapping &)map;
    if( trim.trimmingIsValid() )
    {
      int numberOfTriangles=trim.getTriangulation().getNumberOfElements();
      if( numberOfTriangles<maxNumberOfTriangles/2 )
      {
	real maxArea,minAngle,elementDensity;
	trim.getTriangulationParameters( maxArea, minAngle, elementDensity );
	if( elementDensity<=0. )
	  elementDensity=.05;
	else
	  elementDensity/=sqrt(2.);
	if( maxArea<=0. )
	{
	  maxArea=SQR(.05);
	  // Scale the area by the jacobian -- 
	  realArray rg(1,2),xg(1,3),dxdr(1,3,2);
	  rg(0,0) = .5;
	  rg(0,1) = .5;
	  trim.untrimmedSurface()->map(rg,xg,dxdr);

	  maxArea *= (.5*sqrt( SQR(dxdr(0,1,0)*dxdr(0,2,1)-dxdr(0,1,1)-dxdr(0,2,0)) + 
			       SQR(dxdr(0,0,0)*dxdr(0,2,1)-dxdr(0,0,1)-dxdr(0,2,0)) +
			       SQR(dxdr(0,0,0)*dxdr(0,1,1)-dxdr(0,0,1)-dxdr(0,1,0)) ));
	}
	else
	  maxArea/=2.;


	trim.setMaxAreaForTriangulation( maxArea );
	trim.setElementDensityToleranceForTriangulation( elementDensity );
	printf("Surface %i: triangulate with: maxArea=%e elementDensity=%e\n",s,maxArea,elementDensity);
		  
	trim.triangulate();
      }
      else
      {
	printf("Surface %i already has %i triangles. I will not refine this any more\n",s,numberOfTriangles);
      }
    }
    else
    { // for a surface with invalid trim curves the untrimmedSurface is plotted
      Mapping & refSurface = *trim.untrimmedSurface();
      for( int axis=0; axis<domainDimension; axis++ )
	refSurface.setGridDimensions(axis, 2*refSurface.getGridDimensions(axis)); 
    }
  }
  else
  {
    for( int axis=0; axis<map.getDomainDimension(); axis++ )
      map.setGridDimensions(axis, 2*map.getGridDimensions(axis)); 

  }

  return 0;
}




int CompositeSurface::
update( MappingInformation & mapInfo )
// =================================================================================================
// /Description:
//    Interactive update.
//
// ================================================================================================
{
  aString prefix="CSUP:"; // prefix for all commands

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  const bool executeCommand = (mapInfo.commandOption == MappingInformation::readOneCommand && 
			       mapInfo.command!=NULL);

  aString command="";
  SelectionInfo localSelect, *selectPtr;
  if( executeCommand )
  {
    command=mapInfo.command[0];
    selectPtr = (mapInfo.selectPtr)? mapInfo.selectPtr: &localSelect;
  }
  else
    selectPtr = &localSelect;

  SelectionInfo & select = *selectPtr;

  if( executeCommand && command(0,prefix.length()-1)!=prefix && command != "build dialog" 
      && command != "update dialog" && select.nSelect == 0)
    return 1;

  static int mappingToExamine=0, mappingToHide=0, mappingToShow=0, mappingToDelete=0, mappingToFlip=0;

  int returnValue=0;

  GUIState gui;
  gui.setWindowTitle("Composite Surface");
  gui.setExitCommand("exit", "Exit");

  DialogData & interface = (mapInfo.commandOption == MappingInformation::readOneCommand && 
			    mapInfo.interface!=NULL) ? *mapInfo.interface : (DialogData &)gui;

  char buff[180];  // buffer for sprintf

  aString answer, line, answer2, buf; 

  bool plotObject;
  
  enum SelectEnum { noOp=0, flipNormals, hideSurface, showSurface, deleteSurface, 
                    examineSurface, querySurface,
                    refinePlot, pickToQueryPoint, numberOfSelections };
  static SelectEnum selectFunction=noOp;

// setup graphics parameters... (This should only be done if there is no global object to use)
  GraphicsParameters par;
  GraphicsParameters & params = (mapInfo.gp_ != NULL)? *mapInfo.gp_ : par;
  
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  params.set(GI_TOP_LABEL,getName(mappingName));

  GraphicsParameters::ColourOptions gridLineColourOption;
  int plotShadedMappingBoundaries;
  int plotLinesOnMappingBoundaries;
  int plotTitleLabels;
  int labelGridsAndBoundaries;
  int plotMappingNormals;
  bool pickClosest=false;  // if true only use the closest selection (for hiding for example).

  int temp;
  params.get(GI_GRID_LINE_COLOUR_OPTION, temp); gridLineColourOption=(GraphicsParameters::ColourOptions)temp;
  params.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedMappingBoundaries);
  params.get(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnMappingBoundaries);
  params.get(GI_PLOT_LABELS, plotTitleLabels);
  params.get(GI_LABEL_GRIDS_AND_BOUNDARIES,labelGridsAndBoundaries);
  params.get(GI_PLOT_MAPPING_NORMALS, plotMappingNormals);

  int plotMappingEdges;
  params.get(GI_PLOT_MAPPING_EDGES, plotMappingEdges);

// ordering of the text labels
  static int mnIndex = 0, examineIndex = 1, hideIndex=2, showIndex=3, deleteIndex=4, flipIndex=5; 
  static OptionMenu *selectOption_=NULL;
  
  if( mapInfo.interface==NULL || (executeCommand && command =="build dialog") )
  {
// define layout of option menus
    interface.setOptionMenuColumns(1);
// first option menu
    aString opCommand1[] = {"colour grid lines by grid number", "colour grid lines black", ""};
    aString opLabel1[] = {"by surface colour", "black", "" };

// initial choice: BC number
    int initialChoice = (gridLineColourOption == GraphicsParameters::colourByGrid)? 0 : 1;
    addPrefix(opCommand1, prefix);
    interface.addOptionMenu( "Colour grid lines", opCommand1, opLabel1, initialChoice); 

// second option menu
    aString opCommand2[] = {"colour surface by grid number", "set surface colour", ""};
    aString opLabel2[] = {"by grid number", "choose colour...", "" };

// initial choice: BC number
    addPrefix(opCommand2, prefix);
    interface.addOptionMenu( "Colour surface", opCommand2, opLabel2, 0); 

// third option menu: add a mapping
    int num=mapInfo.mappingList.getLength();
    aString *opCommand3 = new aString[num+2];
    aString *opLabel3 = new aString[num+2];
    int j=0, i;
    for(i=0; i<num; i++ )
    {
      MappingRC & map = mapInfo.mappingList[i];
      if( map.getDomainDimension()==2 &&
	  map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this &&
	  map.getClassName() != "CompositeSurface")
      {
	// sPrintF(opCommand3[j], "%d", i);  // *wdh* 010314 : use names instead, more robust
        opCommand3[j]=map.getName(mappingName);
	opLabel3[j++]=map.getName(mappingName);
      }
    }
    int noneIndex = j;
    opCommand3[j]="none"; 
    opLabel3[j++]="None"; 

    opCommand3[j]="";   // null string terminates the menu
    opLabel3[j]="";   // null string terminates the menu

// add "add a mapping " in front of all labels to get the commands
    addPrefix(opCommand3,"add a mapping ");

// initial choice: BC number
    addPrefix(opCommand3, prefix);
    interface.addOptionMenu( "Add mapping", opCommand3, opLabel3, noneIndex); // none is the default
    delete [] opCommand3;
    delete [] opLabel3;

// fourth option menu
    aString opCommand4[] = {"selection function 0", "selection function 1", "selection function 2",
			    "selection function 3", "selection function 4", "selection function 5", 
                            "selection function 6", "refine plot", "query point", ""};
    aString opLabel4[] = {"No Operation", "Flip Normal", "Hide Surface", 
			  "Show Surface", "Delete Surface", "Examine Surface", 
                          "Query Surface", "refine plot", "query point", "" };

// initial choice: BC number
    addPrefix(opCommand4, prefix);
    interface.addOptionMenu( "Selection function", opCommand4, opLabel4, selectFunction); 
    selectOption_ = &interface.getOptionMenu(3);

// toggle buttons
    aString tbLabels[] = {"Shade", "Grid", "Boundary", /* "Labels", "Squares", "Axes", */
                          "Normals", "pick closest", ""};
    aString tbCommands[] = {"plot shaded surfaces (3D) toggle",
			    "plot grid lines on boundaries (3D) toggle",
			    "plot sub-surface boundaries (toggle)",
//  			    "plot labels (toggle)",
//  			    "plot number labels (toggle)",
//  			    "plot axes (toggle)",
			    "plot normals (toggle)",
                            "pick closest",
			    ""};
    int tbState[] = {plotShadedMappingBoundaries, 
		     plotLinesOnMappingBoundaries, 
		     plotMappingEdges, 
//  		     plotTitleLabels,
//  		     labelGridsAndBoundaries,
//  		     gi.getPlotTheAxes(),
		     plotMappingNormals,
                     pickClosest};
    
    addPrefix(tbCommands, prefix);
    int numberOfColumns=4;
    interface.setToggleButtons(tbCommands, tbLabels, tbState, numberOfColumns); // organize in 4 columns
// done defining toggle buttons

// setup a user defined menu and some user defined buttons
    aString buttonCommands[] = {"plotObject", 
				"erase",
				"flip normals (toggle)", 
				"determine topology",
				"unhide all sub-surfaces", 
				"hide all sub-surfaces", 
				"delete hidden sub-surfaces", 
                                "delete unhidden",
				"print parameters",
                                "recompute bounds",
                                "show broken surfaces",
				""};
    aString buttonLabels[] = {"Replot",
			      "Erase",
			      "Flip normals", 
			      "Topology", 
			      "Show all",
			      "Hide all",
			      "Delete hidden",
                              "delete unhidden",
			      "print parameters",
                              "recompute bounds",
                              "show broken surfaces",
			      ""};
  
    addPrefix(buttonCommands, prefix);
    interface.setPushButtons(buttonCommands, buttonLabels, 4); // 4 rows

// setup textlabels
    aString textCmd[] = {"mappingName", "examine a sub-surface", "hide sub-surfaces",
			 "unhide sub-surfaces", "delete sub-surfaces", "change the sign of a normal",
                         "debug",
			 ""};
    aString textLbl[] = {"Name:", "Examine sub-surface #", "Hide sub-surface #", 
			 "Show sub-surface #", "Delete sub-surface #", "Flip normal on sub-surface #",
                         "debug",
			 ""};
    aString textInit[8];
    int cnt=0;
    mnIndex = cnt;      textInit[cnt++] = getName(mappingName);           // mapping name
    examineIndex = cnt; sPrintF(textInit[cnt++], "%i", mappingToExamine); // subsurface to examine
    hideIndex = cnt;    sPrintF(textInit[cnt++], "%i", mappingToHide);    // subsurface to hide
    showIndex = cnt;    sPrintF(textInit[cnt++], "%i", mappingToShow);    // subsurface to show (un-hide)
    deleteIndex = cnt;  sPrintF(textInit[cnt++], "%i", mappingToDelete);  // subsurface to delete
    flipIndex = cnt;    sPrintF(textInit[cnt++], "%i", mappingToFlip);    // subsurface to flip the normal
                        sPrintF(textInit[cnt++], "%i", Mapping::debug); 
    textInit[cnt++] = "";
    

    addPrefix(textCmd, prefix);
    interface.setTextBoxes(textCmd, textLbl, textInit); 

    if( executeCommand ) return 0;
  }
  
  if (executeCommand && command == "update dialog")
  {
// set the state of all toggle buttons
    int q=0;
    interface.setToggleState(q++, plotShadedMappingBoundaries);
    interface.setToggleState(q++, plotLinesOnMappingBoundaries);
    interface.setToggleState(q++, plotMappingEdges);
//      interface.setToggleState(q++, plotTitleLabels);
//      interface.setToggleState(q++, labelGridsAndBoundaries);
//      interface.setToggleState(q++, gi.getPlotTheAxes());
    interface.setToggleState(q++, plotMappingNormals);
    interface.setToggleState(q++, pickClosest);
    
// redo the third option menu: add a mapping
    int num=mapInfo.mappingList.getLength();
    aString *opCommand3 = new aString[num+2];
    aString *opLabel3 = new aString[num+2];
    int j=0, i;
    for(i=0; i<num; i++ )
    {
      MappingRC & map = mapInfo.mappingList[i];
      if( map.getDomainDimension()==2 &&
	  map.getDomainDimension()== (map.getRangeDimension()-1) &&
	  map.getClassName() != "CompositeSurface")
      {
	sPrintF(opCommand3[j], "%d", i);
	opLabel3[j++]=map.getName(Mapping::mappingName);
      }
    }
    int noneIndex = j;
    opCommand3[j]="none"; 
    opLabel3[j++]="None"; 

    opCommand3[j]="";   // null string terminates the menu
    opLabel3[j]="";   // null string terminates the menu

// add "add a mapping " in front of all labels to get the commands
    addPrefix(opCommand3,"add a mapping ");

// initial choice: BC number
    addPrefix(opCommand3, prefix);
    interface.changeOptionMenu( 2, opCommand3, opLabel3, noneIndex); // #2 is the add mapping option
    delete [] opCommand3;
    delete [] opLabel3;

    OptionMenu &selectOption = interface.getOptionMenu(3); // the select function option menu is #3.
    selectOption.optionList[1].setSensitive(topologyDetermined); // menu item 1 is flip normals
// set the sensitivity of the flipping of the normal
    interface.setSensitive(topologyDetermined, DialogData::textBoxWidget, flipIndex);
    return 0;
  }
  


  if (!executeCommand)
  {
    aString menu[] = 
    {
      "!CompositeSurface",
//      "add a mapping",
        "add all mappings",
        "project a point",
//      ">hide",
//          "hide sub-surfaces",
//          "unhide sub-surfaces",
//          "unhide all sub-surfaces",
//          "hide sub-surfaces with mouse",
//          "unhide sub-surfaces with mouse",
//        "<>delete",
//        "delete sub-surfaces",
//          "delete sub-surfaces with mouse",
//        "delete hidden sub-surfaces",
//        "<>query",
//          "query sub-surfaces with mouse",
//        "examine a sub-surface",
//      "<determine topology",
//        "<>normals",
//        "plot normals (toggle)",
//      "flip normals (toggle)",
//        "flip normals with mouse",
//      "change the sign of a normal",
//        "<>plot options",
//          "plot shaded surfaces (3D) toggle",
//          "plot grid lines on boundaries (3D) toggle",
//          "plot sub-surface boundaries",
//          "do not plot sub-surface boundaries",
//          "colour grid lines by grid number",
//          "colour grid lines black",
//          "colour surface by grid number",
//          "set surface colour",
//          "plot number labels (toggle)",
//          "plot axes (toggle)",
//      "< ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
//      "print parameters",
//      "plotter",
      "help",
//      "exit", 
      "" 
    };

    gui.buildPopup(menu);
//    gi.appendToTheDefaultPrompt("CompositeSurface>"); // set the default prompt
    gi.appendToTheDefaultPrompt(""); // set the default prompt
    gi.pushGUI(gui);
  }
  
  aString help[] = 
  {
    "add a mapping      : add a new mapping to the composite surface",
    "add all mappings   : add all valid mappings"
    "hide sub-surfaces  : ",
    "delete sub-surfaces: ",
    "determine topology : determine outward normals to all sub-surfaces",
    " plot normals (toggle) : plot the outward normals on each surface",
    "examine a sub-surface : call update for one of the sub-surfaces that makes up "
    "the composite surface",
    "plot shaded surfaces (3D) toggle",
    "plot grid lines on boundaries (3D) toggle",
    "plot sub-surface boundaries",
    "colour grid lines by grid number",
    "colour grid lines black",
//    "plot number labels (toggle) : plot colured squares"
    "lines              : specify number of grid lines",
    "boundary conditions: specify boundary conditions",
    "share              : specify share values for sides",
    "mappingName        : specify the name of this mapping",
    "periodicity        : specify periodicity in each direction",
    "print parameters   : print current values for parameters",
    "plot               : enter plot menu (for changing ploting options)",
    "help               : Print this list",
    "exit               : Finished with changes",
    "" 
  };

  OptionMenu &selectOption = interface.getOptionMenu(3); // the select function option menu is #3.
  int len=0;
  
  for( int it=0;; it++ )
  {
    plotObject=numberOfSubSurfaces()>0;

// set the sensitivity of the flipping of the normal
    interface.setSensitive(topologyDetermined, DialogData::textBoxWidget, flipIndex);
    selectOption.optionList[1].setSensitive(topologyDetermined); // menu item 1 is flip normals

    if( !executeCommand )
    {
      if( it==0 && plotObject )
        answer="plotObject";
      else
      {
	gi.savePickCommands(false); // temporarily turn off saving of pick commands.  

	gi.getAnswer(answer, "", select);

        gi.savePickCommands(true); // turn back on
      }
      
    }
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }

// take off the prefix
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);
 

//                     01234567890123456789
    if( answer(0,17)=="selection function" )
    {
      SelectEnum s = noOp;
      sScanF(answer(19,answer.length()-1),"%d",&s);
      if ( noOp <= s && s < numberOfSelections )
      {
	selectFunction = s;
	selectOption_->setCurrentChoice(s);
	gi.outputString(sPrintF(buf, "Selection function: %d", s));
      }
      else
      {
	gi.outputString(sPrintF(buf, "Error: Bad selection function: %d", s));
      }
      plotObject = false;
    }
    else if( answer=="refine plot" )
    {
      selectFunction=refinePlot;
      selectOption_->setCurrentChoice(selectFunction);
      gi.outputString(sPrintF(buf, "Selection function: %d", selectFunction));
      plotObject = false;
    }
    else if( answer=="query point" )
    {
      selectFunction=pickToQueryPoint;
      selectOption_->setCurrentChoice(selectFunction);
    }
    else if( (len=answer.matches("add a mapping")) )
    {
      aString rest = answer(len+1,answer.length()-1);
      if (rest == "none")
      {
	gi.outputString("Adding NO mapping");
	continue;
      }
      else
      {
	int mapNumber = -1;
	// sScanF(rest,"%d",&mapNumber);
	int num=mapInfo.mappingList.getLength();
	for( int i=0; i<num; i++ )
	{
	  if( mapInfo.mappingList[i].getName(mappingName)==rest )
	  {
	    mapNumber=i;
	    break;
	  }
	}
	if ( 0 <= mapNumber )
	{
	  MappingRC & map = mapInfo.mappingList[mapNumber];
	  add( *map.mapPointer, mapNumber );
	  setColour(numberOfSubSurfaces()-1, gi.getColourName(numberOfSubSurfaces()-1));
	  gi.outputString(sPrintF(buf, "Adding mapping `%s'", SC map.getName(mappingName)));

	}
	else
	{
	  gi.outputString(sPrintF(buf, "Error: Bad mapping name: %s", SC rest));
          gi.stopReadingCommandFile();
	}
	
      }
    }
    else if( answer=="add all mappings" )
    {
      int num=mapInfo.mappingList.getLength();
      for( int i=0; i<num; i++ )
      {
  	MappingRC & map = mapInfo.mappingList[i];
  	if( map.getDomainDimension()==2 &&
	    map.getDomainDimension()== (map.getRangeDimension()-1) && map.mapPointer!=this )
  	{
	  bool alreadyInTheList=FALSE;
	  for( int s=0; s<numberOfSubSurfaces(); s++ )
  	  {
	    if( map.mapPointer==&(*this)[s] )
  	    {
  	      alreadyInTheList=TRUE;
  	      break;
  	    }
  	  }
	  if( !	alreadyInTheList )  
  	  {
	    add( *(map.mapPointer),i );
  	    setColour(i,gi.getColourName(i));
  	  }
  	}
      }
      plotObject=TRUE;
    }//                     01234567890123456789
    else if( answer(0,16)=="hide sub-surfaces" )
    {
      int s=-1;
      sScanF(answer(18,answer.length()-1),"%d",&s);
      if (0 <= s && s<numberOfSubSurfaces())
      {
	mappingToHide = s;
	gi.outputString(sPrintF(buf, "Hide sub-surface %i", s));
	visible(s)=FALSE;
      }
      else
      {
//	gi.createMessageDialog(sPrintF(buf, "Invalid sub-surface %i",s), errorDialog);
	gi.outputString(sPrintF(buf, "Invalid sub-surface %i",s));
	interface.setTextLabel(hideIndex, sPrintF(buf, "%i", mappingToHide)); // (re)set the textlabel
      }

    }//                     01234567890123456789
    else if( answer(0,18)=="unhide sub-surfaces" )
    {
      int s=-1;
      sScanF(answer(20,answer.length()-1),"%d",&s);
      if (0 <= s && s<numberOfSubSurfaces())
      {
	mappingToShow = s;
	gi.outputString(sPrintF(buf, "Show sub-surface %i", s));
	visible(s)=TRUE;
      }
      else
      {
//	gi.createMessageDialog(sPrintF(buf, "Invalid sub-surface %i",s), errorDialog);
	gi.outputString(sPrintF(buf, "Invalid sub-surface %i",s));
	interface.setTextLabel(showIndex, sPrintF(buf, "%i", mappingToShow)); // (re)set the textlabel
      }

    }//                     012345678901234567890123456789
    else if( answer(0,20)=="hide all sub-surfaces" )
    {
      visible=FALSE;
    }//                     01234567890123456789
    else if( answer=="unhide all sub-surfaces" )
    {
      visible=TRUE;
    }//                     01234567890123456789
    else if( answer(0,18)=="delete sub-surfaces" )
    {
      int s=-1;
      sScanF(answer(20,answer.length()-1),"%d",&s);
      if (0 <= s && s<numberOfSubSurfaces())
      {
	mappingToDelete = s;
	gi.outputString(sPrintF(buf, "Deleting sub-surface %i", s));
// delete the display lists for surface # s
	eraseCompositeSurface(gi, s);
  	remove(s);
      }
      else
      {
//	gi.createMessageDialog(sPrintF(buf, "Invalid sub-surface %i",s), errorDialog);
	gi.outputString(sPrintF(buf, "Invalid sub-surface %i",s));
	interface.setTextLabel(deleteIndex, sPrintF(buf, "%i", mappingToDelete)); // (re)set the textlabel
      }

    }
    else if( select.nSelect > 0 && selectFunction == deleteSurface )
    {
      // delete the selected object(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      if( pickClosest || singleSelect )
      {
	select.nSelect = 1;
	select.selection(0,0) = select.globalID;
      }
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<numberOfSurfaces; s++ )
	{
	  if( select.selection(i,0) == surfaces[s].getMapping().getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i will be deleted",s));
// erase the display lists for surface #s
	    eraseCompositeSurface(gi, s);
	    remove(s);
	  }
	}
      }
    }
    else if( (len=answer.matches("refine surface")) )
    {
      int s=-1;
      sScanF(answer(len,answer.length()-1),"%i",&s);

      refineSubSurface( s );

      plotObject=true;
      eraseCompositeSurface(gi,s);  // This will erase the display list and redraw
    }
    else if( select.nSelect > 0 && selectFunction == refinePlot )
    {
      // refine the surface plot of the selected surfaces
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      if( pickClosest || singleSelect)
      {
	select.nSelect = 1;
	select.selection(0,0) = select.globalID;
      }
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<numberOfSurfaces; s++ )
	{
	  if( select.selection(i,0) == surfaces[s].getMapping().getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "refine the surface plot of sub-surface %i",s));
            gi.outputToCommandFile(sPrintF("refine surface %i\n",s));   // * finish me **

            refineSubSurface( s );
//             Mapping & map = (*this)[s];
// 	    if( map.getClassName()=="TrimmedMapping" )
// 	    {
//               TrimmedMapping & trim = (TrimmedMapping &)map;
//               if( trim.trimmingIsValid() )
// 	      {
//                 int numberOfTriangles=trim.getTriangulation().getNumberOfElements();
//                 const int maxNumberOfTriangles=100000;
// 		if( numberOfTriangles<maxNumberOfTriangles/2 )
// 		{
// 		  real maxArea,minAngle,elementDensity;
// 		  trim.getTriangulationParameters( maxArea, minAngle, elementDensity );
// 		  if( elementDensity<=0. )
// 		    elementDensity=.05;
// 		  else
// 		    elementDensity/=sqrt(2.);
// 		  if( maxArea<=0. )
// 		  {
// 		    maxArea=SQR(.05);
// 		    // Scale the area by the jacobian -- 
// 		    realArray rg(1,2),xg(1,3),dxdr(1,3,2);
// 		    rg(0,0) = .5;
// 		    rg(0,1) = .5;
// 		    trim.untrimmedSurface()->map(rg,xg,dxdr);
// 
// 		    maxArea *= (.5*sqrt( SQR(dxdr(0,1,0)*dxdr(0,2,1)-dxdr(0,1,1)-dxdr(0,2,0)) + 
// 					 SQR(dxdr(0,0,0)*dxdr(0,2,1)-dxdr(0,0,1)-dxdr(0,2,0)) +
// 					 SQR(dxdr(0,0,0)*dxdr(0,1,1)-dxdr(0,0,1)-dxdr(0,1,0)) ));
// 		  }
// 		  else
// 		    maxArea/=2.;
// 
// 
// 		  trim.setMaxAreaForTriangulation( maxArea );
// 		  trim.setElementDensityToleranceForTriangulation( elementDensity );
//                   printf("Surface %i: triangulate with: maxArea=%e elementDensity=%e\n",s,maxArea,elementDensity);
// 		  
// 		  trim.triangulate();
// 		}
// 		else
// 		{
// 		  printf("Surface %i already has %i triangles. I will not refine this any more\n",s,numberOfTriangles);
// 		}
// 	      }
// 	      else
// 	      { // for a surface with invalid trim curves the untrimmedSurface is plotted
// 		Mapping & refSurface = *trim.untrimmedSurface();
// 		for( int axis=0; axis<domainDimension; axis++ )
// 		  refSurface.setGridDimensions(axis, 2*refSurface.getGridDimensions(axis)); 
// 	      }
// 	    }
// 	    else
// 	    {
// 	      for( int axis=0; axis<map.getDomainDimension(); axis++ )
// 		map.setGridDimensions(axis, 2*map.getGridDimensions(axis)); 
// 
// 	    }
// 
            plotObject=true;
	    eraseCompositeSurface(gi,s);  // This will erase the display list and redraw

	  }


	}
      }
    }    else if( select.nSelect > 0 && selectFunction == noOp )
    {
      continue;
    }
    else if( answer=="delete hidden sub-surfaces" )
    {
      for( int s=numberOfSurfaces-1; s>=0; s-- )
      {
        if( !visible(s) )
  	  remove(s);
      }
    }
    else if( answer=="delete unhidden" )
    {
      printf("Delete un-hidden surfaces and make hidden surfaces visible\n");
      for( int s=numberOfSurfaces-1; s>=0; s-- )
      {
        if( visible(s) )
  	  remove(s);
        else
          visible(s)=true;
	
      }
    }
    else if( select.nSelect > 0 && selectFunction == showSurface )
    {
// show the selected sub-surface(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      if( pickClosest ||singleSelect)
      {
	select.nSelect = 1;
	select.selection(0,0) = select.globalID;
      }
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<numberOfSurfaces; s++ )
	{
	  if( select.selection(i,0)==surfaces[s].getMapping().getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i will be shown",s));
	    visible(s)=TRUE;

            gi.outputToCommandFile(sPrintF("unhide sub-surfaces %i\n",s)); 

	  }
	}
      }
    }
    else if( select.nSelect > 0 && selectFunction == hideSurface )
    {
// hide the selected sub-surface(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      if( pickClosest || singleSelect )
      {
	select.nSelect = 1;
	select.selection(0,0) = select.globalID;
      }
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<numberOfSurfaces; s++ )
	{
	  if( select.selection(i,0) == surfaces[s].getMapping().getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i will be hidden",s));
	    visible(s)=FALSE;

            gi.outputToCommandFile(sPrintF("hide sub-surfaces %i\n",s)); 

	  }
	}
      }
    }
    else if( select.nSelect > 0 && selectFunction == querySurface )
    {
      // query the surfaces -- print info about each surface chosen.
      printf("Surface numbers from CAD file:\n");
      int numSelected=0;
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<numberOfSurfaces; s++ )
	{
	  if( select.selection(i,0) == surfaces[s].getMapping().getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf,"sub-surface %4i: name=%s file-ID=%i colour=%s",
				    s,(const char*)(*this)[s].getName(mappingName),
				    surfaceIdentifier(s),(const char*)surfaceColour[s]));
            
            if( select.globalID==select.selection(i,0) )
              gi.outputString(sPrintF(buf,"*closest sub-surface: %4i: name=%s file-ID=%i colour=%s",
				    s,(const char*)(*this)[s].getName(mappingName),
				    surfaceIdentifier(s),(const char*)surfaceColour[s]));
            numSelected++;
	  }
	}
      }
    }
    else if( select.nSelect > 0 && selectFunction == examineSurface )
    {
// examine the closest sub-surface
      for( int s=0; s<numberOfSurfaces; s++ )
      {
	if( select.globalID == surfaces[s].getMapping().getGlobalID() )
	{
          gi.outputToCommandFile(sPrintF("examine a sub-surface %i\n",s)); 

	  gi.outputString(sPrintF(buf, "Sub-surface %i was selected. Visible=%i, ID=%i, "
				  "signForNormal=%i, name=%s", s, visible(s), surfaceIdentifier(s), 
				  (signForNormal.getLength(0)>s ? signForNormal(s) : 0),
				  SC (*this)[s].getName(mappingName)));

	  mappingToExamine = s;
	  gi.outputString(sPrintF(buf, "Examine sub-surface %i", s));
	  gi.erase();
	  params.set(GI_TOP_LABEL,sPrintF(buff,"sub-surface %i", s));
	  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  Mapping *map = &(*this)[s];  // make a pointer so virtual function call works.
	  map->update( mapInfo );
// delete the display lists so that sub-surface s will get replotted properly
	  eraseCompositeSurface(gi, s);
	
	  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	  params.set(GI_TOP_LABEL,getName(mappingName));
	  plotObject=true;
	}
      }
    }
    else if( (select.nSelect > 0 && selectFunction == pickToQueryPoint) || 
             answer.matches("query surface point") )
    {
      // find the selected point 
      if( (len=answer.matches("query surface point")) )
      {
        // reading from a command file:
	sScanF(answer(len,answer.length()-1)," %i %e %e %e\n",&select.globalID,
	       &select.x[0],&select.x[1],&select.x[2]);
      }
      for( int s=0; s<numberOfSurfaces; s++ )
      {
	if( select.globalID == surfaces[s].getMapping().getGlobalID() )
	{
          gi.outputToCommandFile(sPrintF("query surface point %i %e %e %e\n",select.globalID,
                      select.x[0],select.x[1],select.x[2])); 

          printF(">>>query surface point x=(%e %e %e)\n",select.x[0],select.x[1],select.x[2]); 
	  gi.outputString(sPrintF(buf, "The point was found on surface %i. ID=%i, "
				  "signForNormal=%i, name=%s, colour=%s", s, surfaceIdentifier(s), 
				  (signForNormal.getLength(0)>s ? signForNormal(s) : 0),
				  SC (*this)[s].getName(mappingName),(const char*)surfaceColour[s]));

          const bool isTrimmedMapping = (*this)[s].getClassName()=="TrimmedMapping";
          Mapping & map = (!isTrimmedMapping) ? (*this)[s] : 
                          *((TrimmedMapping&)(*this)[s]).untrimmedSurface();

          // printF(" surface s=%i : isTrimmedMapping=%i\n",s,(int)isTrimmedMapping);

	  realArray x(1,3),r(1,3),xr(1,3,2);
          x(0,0)=select.x[0];
          x(0,1)=select.x[1];
          x(0,2)=select.x[2];
	  r=-1;
   	  map.inverseMap(x,r);

          map.map(r,x,xr);

          real xDist=sqrt( SQR(x(0,0)-select.x[0])+SQR(x(0,1)-select.x[1])+SQR(x(0,2)-select.x[2]) );
	  printF(" ...closest point on surface: x=(%9.3e,%9.3e,%9.3e) r=(%9.3e,%9.3e) (dist to picked pt=%8.2e)\n",
                 x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),xDist);

          if( false )
	  {
	    printF(" check inverse again with an initial guess\n");
	    map.inverseMap(x,r);
	  }
	  
	  real nv[3];
	  nv[0] = xr(0,1,0)*xr(0,2,1)-xr(0,2,0)*xr(0,1,1);
	  nv[1] = xr(0,2,0)*xr(0,0,1)-xr(0,0,0)*xr(0,2,1);
	  nv[2] = xr(0,0,0)*xr(0,1,1)-xr(0,1,0)*xr(0,0,1);
	  real anorm =max(REAL_MIN*100.,sqrt(SQR(nv[0])+SQR(nv[1])+SQR(nv[2]))); 
	  nv[0]/=anorm; nv[1]/=anorm; nv[2]/=anorm;
	 
	  printF(" ...parametric derivatives: xr=(%9.3e,%9.3e,%9.3e), xs=(%9.3e,%9.3e,%9.3e)\n",
                 xr(0,0,0),xr(0,1,0),xr(0,2,0),
                 xr(0,0,1),xr(0,1,1),xr(0,2,1));
	  printF(" ...normal to surface = (%9.3e,%9.3e,%9.3e)\n",nv[0],nv[1],nv[2]); 


	}
      }
    }
    else if( answer=="determine topology" )
    {
      if( false )
      {
        determineTopology();
      }
      else
      {
	updateTopology();
      }
      
    }//                     012345678901234567890123456789
    else if( answer(0,20)=="plot normals (toggle)" )
    {
      plotMappingNormals=!plotMappingNormals;
      params.set(GI_PLOT_MAPPING_NORMALS, plotMappingNormals);
    }//                     012345678901234567890123456789
    else if( answer(0,20)=="flip normals (toggle)" )
    {
      signForNormal(Range(0,numberOfSurfaces-1))=-signForNormal(Range(0,numberOfSurfaces-1));
    }//                     012345678901234567890123456789
    else if( interface.getToggleValue(answer,"pick closest",pickClosest) ){}//
    else if( answer(0,20)=="examine a sub-surface" )
    {
      int s=-1;
      sScanF(answer(22,answer.length()-1),"%d",&s);
      if (0 <= s && s<numberOfSubSurfaces())
      {
	mappingToExamine = s;
	gi.outputString(sPrintF(buf, "Examine sub-surface %i", s));
	gi.erase();
	params.set(GI_TOP_LABEL,sPrintF(buff,"sub-surface %i", s));
	params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	Mapping *map = &(*this)[s];  // make a pointer so virtual function call works.
	map->update( mapInfo );
// delete the display lists so that sub-surface s will get replotted properly
	eraseCompositeSurface(gi, s);
	
	params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	params.set(GI_TOP_LABEL,getName(mappingName));
      }
      else
      {
//	gi.createMessageDialog(sPrintF(buf, "Invalid sub-surface %i",s), errorDialog);
	gi.outputString(sPrintF(buf, "Invalid sub-surface %i",s));
	interface.setTextLabel(examineIndex, sPrintF(buf, "%i", mappingToExamine)); // (re)set the textlabel
      }
      
    }//                                          012345678901234567890123456789
    else if( (len=answer.matches("debug")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&Mapping::debug);
      printF("Setting debug=%i\n",debug);
    }
    else if(topologyDetermined && answer(0,26)=="change the sign of a normal" )
    {
      int s=-1;
      sScanF(answer(28,answer.length()-1),"%d",&s);
      if (0 <= s && s<numberOfSubSurfaces())
      {
	mappingToFlip = s;
	gi.outputString(sPrintF(buf, "Flip the normal on sub-surface %i", s));
	signForNormal(s)=-signForNormal(s);
      }
      else
      {
//	gi.createMessageDialog(sPrintF(buf, "Invalid sub-surface %i",s), errorDialog);
	gi.outputString(sPrintF(buf, "Invalid sub-surface %i",s));
	interface.setTextLabel(flipIndex, sPrintF(buf, "%i", mappingToFlip)); // (re)set the textlabel
      }
      
    }
    else if( topologyDetermined && select.nSelect > 0 && selectFunction == flipNormals )
    {
// flip normals of the closest object
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      if (singleSelect)
      {
	select.nSelect = 1;
	select.selection(0,0) = select.globalID;
      }
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<numberOfSurfaces; s++ )
	{
	  if( select.selection(i,0) == surfaces[s].getMapping().getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Reversing the normal of sub surface %i",s));
	    signForNormal(s)=-signForNormal(s);
	  }
	}
      }
    }//                     0123456789012345678901234567890123456789
    else if( answer(0,31)=="plot shaded surfaces (3D) toggle" ) // add these
    {
      plotShadedMappingBoundaries = !plotShadedMappingBoundaries;   
//        gridLineColourOption=plotShadedMappingBoundaries ? GraphicsParameters::defaultColour :
//  	GraphicsParameters::colourByGrid;
      params.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotShadedMappingBoundaries);
//        params.set(GI_GRID_LINE_COLOUR_OPTION,gridLineColourOption);
    }//                     01234567890123456789012345678901234567890
    else if( answer(0,40)=="plot grid lines on boundaries (3D) toggle" )
    {
      plotLinesOnMappingBoundaries= !plotLinesOnMappingBoundaries;
      params.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,plotLinesOnMappingBoundaries);
    }
    else if( answer=="plot sub-surface boundaries" )
    {
      params.set(GI_PLOT_MAPPING_EDGES,TRUE);
    }
    else if( answer=="do not plot sub-surface boundaries" )
    {
      params.set(GI_PLOT_MAPPING_EDGES,FALSE);
    }//                     0123456789012345678901234567890123456789
    else if( answer(0,35)=="plot sub-surface boundaries (toggle)" )
    {
      int pssb;
      params.get(GI_PLOT_MAPPING_EDGES,pssb);
      params.set(GI_PLOT_MAPPING_EDGES,(pssb)? FALSE: TRUE); // reverse the flag
    }//                     012345678901234567890123456789
    else if( answer=="colour grid lines by grid number" )
    {
      gridLineColourOption=GraphicsParameters::colourByGrid;
      params.set(GI_GRID_LINE_COLOUR_OPTION,gridLineColourOption);
      eraseCompositeSurface(gi);
    }
    else if( answer=="colour grid lines black" )
    {
      gridLineColourOption=GraphicsParameters::defaultColour;
      params.set(GI_GRID_LINE_COLOUR_OPTION,gridLineColourOption);
      eraseCompositeSurface(gi);
    }
    else if( answer=="colour surface by grid number" )
    {
      for( int s=0; s<numberOfSubSurfaces(); s++ )
	setColour(s,gi.getColourName(s));
      eraseCompositeSurface(gi);
    }
    else if( answer=="set surface colour" )
    {
      int ss=-1;
      gi.inputString(answer,"Change the colour for which sub-surface? (-1=all)");
      sScanF(answer,"%i",&ss);
      
      aString answer2 = gi.chooseAColour();
      if( answer2!="no change" )
      {
        if( ss<0 || ss>numberOfSubSurfaces() )
	{
	  for( int s=0; s<numberOfSubSurfaces(); s++ )
	    setColour(s,answer2);
          printf("Change all surfaces to be %s\n",(const char *)answer2);
	}
	else
	{
          setColour(ss,answer2);
          printf("Change surface %i to be %s\n",ss,(const char *)answer2);
	}
      }
      eraseCompositeSurface(gi);
      
    }
    else if( answer=="print parameters" || 
             answer=="show parameters" /* for backward compatibility */ )
    {
      gi.outputString(sPrintF(buf,"Number of sub-surfaces = %i",numberOfSubSurfaces()));
      int totalTriangles=0;
      int maxTriangles=0, maxTrianglesSurface=0;
      for( int s=0; s<numberOfSubSurfaces(); s++ )
      {
        Mapping & map = (*this)[s];
	
	printf("sub-surface %4i: visible=%i",s,visible(s));

        int numTriangles=0;
        if( map.getClassName()=="TrimmedMapping" )
	{
          TrimmedMapping & trim = (TrimmedMapping &)map;
	  if( trim.hasTriangulation() )
	  {
            numTriangles=trim.getTriangulation().getNumberOfElements();
	  }
	}
        printf(" triangles=%7i,",numTriangles);
	totalTriangles+=numTriangles;
        if( numTriangles>maxTriangles )
	{
	  maxTriangles=numTriangles;
	  maxTrianglesSurface=s;
	}
	
	printf(" name=%.30s, ID=%i colour=%s",
	       (const char*)map.getName(mappingName),surfaceIdentifier(s),(const char*)surfaceColour[s]);

        printf("\n");
	
      }
      printf(" Total triangles for plotting all surfaces=%i\n",totalTriangles);
      printf(" Surface %i (ID=%i) had the most triangles with %7i.\n",
                  maxTrianglesSurface,surfaceIdentifier(maxTrianglesSurface),maxTriangles);
      
      printf("List of surface ID's: (can be used to read in just these surfaces)\n");
      for( int s=0; s<numberOfSubSurfaces(); s++ )
      {
        printf("%i ",surfaceIdentifier(s));
        if( (s % 30) == 29 ) printf("\n");
      }
      printf("\n");
      
      plotObject = false;
    }
    else if( answer=="recompute bounds" )
    {
      real xBound[2][3]={REAL_MAX,REAL_MAX,REAL_MAX,-(REAL_MAX/4),-(REAL_MAX/4),-(REAL_MAX/4)};
      for( int s=0; s<numberOfSurfaces; s++ )
      {
	Mapping & surface = (*this)[s];
	for( int axis=0; axis<rangeDimension; axis++ )
	{
	  xBound[Start][axis]=min(xBound[Start][axis],(real)surface.getRangeBound(Start,axis));
	  xBound[End  ][axis]=max(xBound[End  ][axis],(real)surface.getRangeBound(End  ,axis));
	}
      }
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	setRangeBound(Start, axis,xBound[Start][axis]);
	setRangeBound(End  , axis,xBound[End  ][axis]);
      }

    }
    else if( answer=="show broken surfaces" )
    {
      int numBroken=0;
      for( int s=0; s<numberOfSubSurfaces(); s++ )
      {
        if( (*this)[s].getClassName()=="TrimmedMapping" )
	{
	  TrimmedMapping & trim = (TrimmedMapping&)((*this)[s]);
	  if( !trim.trimmingIsValid() )
	  {
            printf("Surface %i (ID=%i) has invalid trimming curves\n",s,surfaceIdentifier(s));
            numBroken++;
	  }
	}
      }
      if( numBroken==0 )
        gi.outputString("There are no broken surfaces");
      else
      {
	printf("There were %i broken surfaces\n",numBroken);
      }
    }
    
//      else if( answer=="plotter" )
//      {
//        params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
//        params.set(GI_TOP_LABEL,getName(mappingName));
//        gi.erase();
//        PlotIt::plot(gi,*this,params); 
//        params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
//      }
    else if( answer=="project a point" )
    {
      MappingProjectionParameters mpParams;
      intArray & subSurfaceIndex = mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);	

      realArray x(1,3), x0(1,3), x2(2,3);
      Range all;
      while(1) 
      {
        gi.inputString(line,"Enter a point to project x,y,z (hit enter to finish)");
	if( line!="" )
	{
	  sScanF(line,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
	}
	else
	{
	  break;
	}
	
	x0=x;
	
        project(x,mpParams);

        printf(" Point x=(%9.3e,%9.3e,%9.3e) projected to x(%9.3e,%9.3e,%9.3e) subSurface %i\n",
	       x0(0,0),x0(0,1),x0(0,2), x(0,0),x(0,1),x(0,2), subSurfaceIndex(0));

	params.set(GI_POINT_SIZE,(real)6.);
        x2(0,all)=x0;
	x2(1,all)=x;
	gi.plotPoints(x2,params);

      }
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
      plotObject = false;
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }//                     01234567890123456789
    else if( answer(0,10)=="mappingName" ) // read the name off answer
    {
      aString newName = "";
      if (answer.length() > 12)
	newName = answer(12,answer.length()-1);

      if (newName != "" && newName != " ")
	{
	  setName(mappingName, newName);
	  params.set(GI_TOP_LABEL,getName(mappingName));
	}
      else
	gi.outputString("Invalid name");
      aString name = getName(mappingName); // gcc warning, setTextLabel should take const aString &
      interface.setTextLabel(mnIndex,name); // (re)set the textlabel
    }
    else if( answer=="exit" || answer=="done"  )
      break;
    else if( answer=="plotObject" )
    {
// AP: don't have to do anything!
    }
    else if( answer=="erase" )
    {
// purge all display lists
      eraseCompositeSurface(gi);
      gi.erase(); // remove/hide all display lists
      plotObject = false;
    }
    else 
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	gi.outputString( sPrintF(buff,"Unknown response=%s", (const char*)answer) );
	gi.stopReadingCommandFile();
	plotObject=FALSE;
      }

    }

    if( plotObject )
    {
      gi.setAxesDimension(rangeDimension);
  
//        params.set(GI_TOP_LABEL,getName(mappingName));
//        params.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotShadedMappingBoundaries);
//        params.set(GI_GRID_LINE_COLOUR_OPTION,gridLineColourOption);
//        params.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,plotLinesOnMappingBoundaries);

      gi.erase();
      PlotIt::plot(gi,*this,params);  
// moved to ogshow/plotCompositeSurface.C
//      if( plotNormals )
//	gi.plotSubSurfaceNormals(*this,params);

    }
  }
  
  if (!executeCommand)
  {
    gi.popGUI();
//  gi.erase(); // AP: why always erase?
    gi.unAppendTheDefaultPrompt();  // reset prompt
  }
  
  return returnValue;
}

