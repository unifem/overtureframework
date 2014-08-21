#define BOUNDS_CHECK

#include "HyperbolicMapping.h"
//#include "PlotStuff.h"
#include "CompositeSurface.h"

#include "MappingInformation.h"
#include "TrimmedMapping.h"
#include "LineMapping.h"
#include "PlaneMapping.h"
#include "SplineMapping.h"
#include "ReductionMapping.h"
#include "NurbsMapping.h"
#include "display.h"

#include "CompositeTopology.h"
#include "ComposeMapping.h"


int HyperbolicMapping::
createCurveOnSurface( GenericGraphicsInterface & gi,
                      SelectionInfo & select, 
                      Mapping* &curve,
                      Mapping* mapPointer /* = NULL */,
                      real *xCoord /* = NULL */, 
                      real *rCoord /* = NULL */,
                      int *boundaryCurveChosen /* = NULL */,
                      bool resetBoundaryConditions /* = true */  )
// =================================================================================================
// /Description:
//  **New version***  Create a curve that can be used as a starting curve for the hyperbolic surface grid generator
// 
// /select (input) : selection info that may specify the location of a curve to use -- not used if mapPointer
//      is specified.
// /curve (input/output): The starting curve.
// /mapPointer (input) : If mapPointer is specified on input then this Mapping will be used and
//    the selection info will be ignored.
// /xCoord, rCoord (input) : 
// /resetBoundaryConditions (input) : by default we assume we are choosing an initial curve or new
//   boundary curve so we rest the boundary conditions. 
// /boundaryCurveChosen (output) : if supplied, on output this will be the number of the boundary curve chosen.
//\end{HyperbolicMappingInclude.tex}
// =================================================================================================
{
  // Selection possibilities:
  //  1. a boundary curve was chosen (one of boundaryCurves[b]). e.g. a boundary segment of an unstructured mapping.
  //  2. a point was chosen on the surface (maybe a CompositeSurface)
  //     2.1 choose a specified coordinate line
  //     2.2 choose the curve formed by the boundary of one side of the surface patch or
  //         choose a segment of the trimming curve for the trimmed surface patch.

//    debug=7;  // **********
//    if(debug>0 && debugFile==NULL )
//      debugFile = fopen("hype.debug","w" );


  int returnValue=1;
  if( (select.nSelect==0 && mapPointer==NULL) || surface==NULL )
    return returnValue;

  // Mapping *mapPointer=NULL;

  real epsx=1.e-5; // changed below
  MappingProjectionParameters mpParams;
  const IntegerDistributedArray & subSurfaceIndex = 
                                    mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);
  const RealDistributedArray & rP = mpParams.getRealArray(MappingProjectionParameters::r);
      
  RealArray xSelected(1,3), rSelected(1,3);
  if ( xCoord!=NULL )
  {
    xSelected(0,0)=xCoord[0]; xSelected(0,1)=xCoord[1]; xSelected(0,2)=xCoord[2];
  }
  if ( rCoord!=NULL )
  {
    rSelected(0,0)=rCoord[0]; rSelected(0,1)=rCoord[1]; rSelected(0,2)=rCoord[2];
  }
  

  const bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
  bool checkForEdgeCurves=isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL;
  aString line;
  real mappingBound=0.;

  if( mapPointer==NULL ) initialCurveIsABoundaryCurve=false; // *wdh* 020920

  if( initialCurveOption==initialCurveFromBoundaryCurves || initialCurveOption==initialCurveFromEdges )
  {
    bool curveFound=false;
    if( mapPointer==NULL ) // The mapPointer may be passed in
    {
      if( pickingOption==pickToChooseInitialCurve )
         printf("Selection: looking for an initial curve defined from any high-lighted edge or boundary curves\n");

      for (int i=0; i<select.nSelect && !curveFound; i++)
      {
	// printf("i=%i, ID=%i, minZ=%i, maxZ=%i\n", i,select.selection(i,0),
	//       select.selection(i,1),select.selection(i,2));

	for( int b=0; b<numberOfBoundaryCurves; b++ )
	{
	  if( boundaryCurves[b]->getGlobalID()==select.selection(i,0) )
	  {
	    printf("Boundary curve %i selected\n",b);
            if( boundaryCurveChosen!=NULL )
	      *boundaryCurveChosen=b;
	    
	    mapPointer=boundaryCurves[b];
	    initialCurveIsABoundaryCurve=true;
	    curveFound=true;

            gi.outputToCommandFile(sPrintF(line,"choose boundary curve %i\n",b));
	    break;
	  }
	} // end for b
      }
    
      if( !curveFound && initialCurveOption==initialCurveFromEdges && checkForEdgeCurves )
      {
	int zBuffMin=INT_MAX;
	int selectedCurve=-1;
	CompositeSurface & cs = (CompositeSurface&)(*surface);
	CompositeTopology & compositeTopology = *cs.getCompositeTopology();      
	int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int e=0; e<numberOfEdgeCurves; e++ )
	  {
	    // printf(" edge=%i status=%i\n",e,int(compositeTopology.getEdgeCurveStatus(e)));
	    if( compositeTopology.getEdgeCurve(e).getGlobalID()==select.selection(i,0) &&
		select.selection(i,1)<zBuffMin &&
                compositeTopology.getEdgeCurveStatus(e)!=CompositeTopology::edgeCurveIsRemoved )
	    {
	      selectedCurve=e;
	      zBuffMin=select.selection(i,1);
	      curveFound=true;
	    }
	  }
	}
	if( curveFound )
	{
	  printf("Edge curve %i selected\n",selectedCurve);
	  mapPointer=&compositeTopology.getEdgeCurve(selectedCurve);
          // we also save a point near the middle as a backup
          // (this is probably better than saving the endpoints)
	  const RealDistributedArray & xg = mapPointer->getGrid();
	  const int n=(xg.getBound(0)+xg.getBase(0))/2;
          gi.outputToCommandFile(sPrintF(line,"choose edge curve %i %e %e %e \n",selectedCurve,
					 xg(n,0,0,0),xg(n,0,0,1),xg(n,0,0,2) ));

	}
      }
    }
    else
    {
      curveFound=true;
    }
    
    if( curveFound )
    {
      if( resetBoundaryConditions )
      {
	// set default BC's
	if( mapPointer->getIsPeriodic(axis1)==Mapping::functionPeriodic )
	{
	  boundaryCondition(Start,0)=periodic; // forward
	  boundaryCondition(End  ,0)=periodic;

	  boundaryCondition(Start,1)=periodic;  // backward
	  boundaryCondition(End  ,1)=periodic;
	  // setIsPeriodic(axis1, functionPeriodic);
	      
	}
	else
	{
	  // by default look to match to a boundary curve -- this will be reset later if
	  // there is no curve to match to.
	  boundaryCondition(Start,0)=matchToABoundaryCurve; // forward
	  boundaryCondition(End  ,0)=matchToABoundaryCurve;
	  boundaryCondition(Start,1)=matchToABoundaryCurve; // backward
	  boundaryCondition(End  ,1)=matchToABoundaryCurve;
	}
	projectGhostPoints(0,0)=0;
	projectGhostPoints(1,0)=0;
      
	if( initialCurveIsABoundaryCurve )
	  growthOption=-1;   // grow in this direction ( ** is this always right  ** )

      }
      for( int dir=0; dir<mapPointer->getRangeDimension(); dir++ )
      {
	mappingBound=max(mappingBound,max(fabs((real)mapPointer->getRangeBound(End,dir)),
					  fabs((real)mapPointer->getRangeBound(Start,dir))));
      }
      epsx=edgeCurveMatchingTolerance*mappingBound;
      if( debug & 4 )
         printf("mappingBound = %e, relative tolerance =%e epsx=%e (for merging curves)\n",
              mappingBound,edgeCurveMatchingTolerance,epsx);


    }
  }
  else 
  {
    // **** look for a point selected on the surface that we will use to create a curve from *****

    // In all other cases we need to project the chosen position onto the surface
    if( mapPointer==NULL )
    {
      if( select.active != 1 )
      {
	printf("createCurveOnASurface: selected point was not on the surface\n");
	return returnValue;
      }
    
      printf("World coordinates: %e, %e, %e\n", select.x[0], select.x[1], select.x[2]);

      RealArray xOld;
      xSelected(0,0)=select.x[0]; xSelected(0,1)=select.x[1]; xSelected(0,2)=select.x[2];
      xOld=xSelected;
      
      for( int dir=0; dir<surface->getRangeDimension(); dir++ )
      {
	mappingBound=max(mappingBound,max(fabs((real)surface->getRangeBound(End,dir)),
					  fabs((real)surface->getRangeBound(Start,dir))));
      }
      epsx=edgeCurveMatchingTolerance*mappingBound;
      printf("mappingBound = %e, relative tolerance =%e, epsx=%e(for merging curves)\n",
	     mappingBound,edgeCurveMatchingTolerance,epsx);

      // project the points onto the surface 
      surface->project(xSelected,mpParams);


      real dist = SQRT( SQR(xSelected(0,0)-xOld(0,0))+SQR(xSelected(0,1)-xOld(0,1))+SQR(xSelected(0,2)-xOld(0,2)) );

      int subSurface=subSurfaceIndex.getLength(0)>0 ? subSurfaceIndex(0) : 0;
      rSelected(0,0)=rP(0,0); rSelected(0,1)=rP(0,1);

      printf("Point sits on surface %i, rSelected=(%e,%e), |xP-x|=%8.2e, epsx=%8.2e\n",subSurface,
	     rSelected(0,0),rSelected(0,1),dist,epsx);

      

      bool compositeSurface = surface->getClassName()=="CompositeSurface";

      mapPointer=surface;
      if( compositeSurface )
      {
	// If a composite surface : choose coordinate line from which sub-surface?
	CompositeSurface & cs = (CompositeSurface &)(*surface);
	assert( subSurfaceIndex(0)>=0 && subSurfaceIndex(0)<cs.numberOfSubSurfaces() );
	mapPointer = &cs[subSurfaceIndex(0)];

	gi.outputToCommandFile(sPrintF(line,"choose point on surface %i %e %e %e %e %e\n",subSurfaceIndex(0),
               xSelected(0,0),xSelected(0,1),xSelected(0,2), rSelected(0,0),rSelected(0,1)  ));
      }
      else
      {
	gi.outputToCommandFile(sPrintF(line,"choose point on surface %i %e %e %e %e %e\n",0,
               xSelected(0,0),xSelected(0,1),xSelected(0,2), rSelected(0,0),rSelected(0,1)  ));
      }
    }
    
  }
  
  
  if( mapPointer!=NULL )
  {
    // ----- A new curve was chosen : look to append this curve to any existing start curve ----
      
    Mapping & subSurface = *mapPointer;

    bool append=true;
    // we always save the start curve as a spline
    if( curve==NULL )
    {
      // first time: create the spline
      curve = new SplineMapping;
      curve->incrementReferenceCount();
      append=false;
    }
    else 
    {
      assert( curve->getClassName()=="SplineMapping" );
    }
    SplineMapping & spline = (SplineMapping&)(*curve);
    spline.setShapePreserving();


    Range R3=3;
    if( initialCurveOption==initialCurveFromCurveOnSurface )
    {
      // curve is being created by picking points on the surface
      // *** here we use the values in xSelected ****

      if( !append )
	numberOfPointsOnStartCurve=0;  // this is a global variable

      // build a curve from points specified on the surface.
      printf("adding point to a spline curve on the surface\n");
	
      RealArray xSpline; 
      if( append )
      {
	// xSpline=spline.getGrid(); *wdh* 021102 : get old knots -- otherwise curve moves off the surface
	xSpline=spline.getKnotsS();
      }
      
      numberOfPointsOnStartCurve++;
      int numberOfSplinePoints=xSpline.getLength(0);  // add one point to the existing spline.

      Range I=numberOfPointsOnStartCurve;
      if( xSpline.getLength(0)>0 )
      {
	// *wdh* 021102 xSpline.reshape(xSpline.dimension(0),xSpline.dimension(3));
	xSpline.resize(I,3);
	xSpline(numberOfPointsOnStartCurve-1,R3)=xSelected(0,R3);
      }
      else
      {
	xSpline.redim(1,3);
	xSpline=xSelected;
      }
      if( numberOfPointsOnStartCurve==1 )
      {
	I=2;
	xSpline.resize(I,3);
	xSpline(1,R3)=xSelected(0,R3)+epsx*100.; // add a nearby fake extra point, to make 2 points so we can plot it.
      }
      spline.setPoints( xSpline(I,0),xSpline(I,1),xSpline(I,2) );

      returnValue=0;
    }
    else if( initialCurveOption==initialCurveFromCoordinateLine0 ||
	     initialCurveOption==initialCurveFromCoordinateLine1 )
    {
      // curve comes from a coordinate line on a patch.
      // **** here we use the values in rSelected *****

      int numberOfPoints=numberOfPointsOnStartCurve;
      Range I=numberOfPoints;
      RealArray r(I,2), x(I,3);   // save knots for the spline in here

      real dr=1./(numberOfPoints-1);
      int axis= initialCurveOption==initialCurveFromCoordinateLine0 ? 0 : 1;
      int axisp1=(axis+1)%2;
       
      r(I,axis)=rSelected(0,axis);
      r(I,axisp1).seqAdd(0.,dr);
             
      subSurface.mapS(r,x);
      spline.setPoints( x(I,0),x(I,1),x(I,2) );

      printf("Choosing an initial curve from a coordinate line r%i=%5.2f: subSurface.getIsPeriodic(%i)=%i\n",
	     axis,rSelected(0,axis),axisp1,subSurface.getIsPeriodic(axisp1));
      
      if( (bool)subSurface.getIsPeriodic(axisp1) )
	spline.setIsPeriodic(axis1,subSurface.getIsPeriodic(axisp1));

      returnValue=0;
    }
    else if( initialCurveOption==initialCurveFromEdges ||
	     initialCurveOption==initialCurveFromBoundaryCurves )
    {
      // initial curve from edges of sub surfaces or boundary curves which are edges that have been joined.
      // ****** here we use the values in rSelected *********

      int numberOfPoints=numberOfPointsOnStartCurve;
      Range I=numberOfPoints;
      RealArray r, x(I,3);   // save knots for the spline in here
          

      if( initialCurveOption==initialCurveFromEdges && !checkForEdgeCurves )
      {
	if( subSurface.getClassName()=="TrimmedMapping" )
	{
          // A trimming curve was picked. If we are choosing edges we only want a segment of the trimming curve.
	  // find the closest segment of a trimming curve
          Mapping *mapPointer;

          TrimmedMapping & trim = (TrimmedMapping&)subSurface;
          RealArray r(1,3), r2(1,3), xx(1,3);
	  r(0,0)=rSelected(0,0); r(0,1)=rSelected(0,1);
	  int numberOfTrimCurves = trim.getNumberOfTrimCurves();
          int cMin=-1;
	  real distMin=REAL_MAX;
	  int c0;
	  for( c0=0; c0<numberOfTrimCurves; c0++ )
	  {
	    Mapping & curve = *trim.getTrimCurve(c0);

            curve.inverseMapCS(r,r2); 
	    curve.mapS(r2,xx);

            real dist = SQR(r(0,0)-xx(0,0)) + SQR(r(0,1)-xx(0,1));
            printf(" trim curve %i : dist=%e \n",c0,dist);
            if( dist<distMin) 
	    {
	      cMin=c0;
	      distMin=dist;
	    }
	  }
	  assert( cMin>=0 );
          printf("Closest trimming curve: cMin=%i, distMin=%e\n",cMin,distMin);
          if( trim.getTrimCurve(cMin)->getClassName()=="NurbsMapping" )
	  {
            NurbsMapping & nurb = (NurbsMapping &)(*trim.getTrimCurve(cMin));
	    int numberOfSubCurves=nurb.numberOfSubCurves();

	    cMin=-1;
	    distMin=REAL_MAX;
	    for( c0=0; c0<numberOfSubCurves; c0++ )
	    {
	      NurbsMapping & curve = nurb.subCurve(c0);
	      
	      curve.inverseMapCS(r,r2); 
	      curve.mapS(r2,xx);
	      real dist = SQR(r(0,0)-xx(0,0)) + SQR(r(0,1)-xx(0,1));
	      printf(" trim sub-curve %i : dist=%e \n",c0,dist);
	      if( dist<distMin) 
		{
		  cMin=c0;
		  distMin=dist;
		}
	    }
	    assert( cMin>=0 );
	    printf("Closest trimming sub-curve: cMin=%i, distMin=%e\n",cMin,distMin);
            mapPointer = &nurb.subCurve(cMin);
	  }
	  else
	  {
	    mapPointer=trim.getTrimCurve(cMin);
	  }
	  

          RealArray rr(I,2);
	  r.redim(I,1);
	  real dr = 1./max(1,(I.getBound()-I.getBase()));
	  r.seqAdd(0.,dr);
          mapPointer->mapS(r,rr);
	  // ::display(r,"r");
	  // ::display(rr,"rr");

	  x=-1;
	  trim.mapS(rr,x);
	  // ::display(x,"x");
	  
	}
	else  // not a trimmed mapping
	{
          // the side of an un-trimmed surface was chosen.
	  // evaluate points on the edge of a sub-surface
	  r.redim(I,2);
	  
	  int side,axis;
	  if( min(fabs(rSelected(0,0)),fabs(rSelected(0,0)-1.)) <  min(fabs(rSelected(0,1)),fabs(rSelected(0,1)-1.)) )
	  {
	    axis=0;
	  }
	  else
	  {
	    axis=1;
	  }
	  side=fabs(rSelected(0,axis)) < fabs(rSelected(0,axis)-1.) ? 0 : 1;
       
       
	  real dr=1./(numberOfPoints-1);
	  r(I,axis)=(real)side;
	  int axisp1=(axis+1)%2;
	  r(I,axisp1).seqAdd(0.,dr);
	}
        subSurface.mapS(r,x);

      }
      else
      {
        // a boundary curve was chosen or a curve was passed in.
	// evaluate points on a boundary curve

        if( true )
	{
          // Use twice as many grid points to represent the start curve  // *wdh* 010427
          int num = subSurface.getGridDimensions(axis1)*2;
	  if( pickingOption==pickToCreateBoundaryCurve )
	  { // for boundary curves increase the number of points to handle the case when there
            // is a sharp corner -- otherwise when we project onto the curve we could get the wrong answer
            num*=5;
	  }
	  
	  I=num;
	  r.redim(I,1);
          x.redim(I,rangeDimension);
	  real dr = 1./max(1,(I.getBound()-I.getBase()));
	  r.seqAdd(0.,dr);
	  subSurface.mapS(r,x);

          // ::display(x,"new curve x after subSurface.mapS(r,x);","%22.16e ");
	}
	else
	{
          // use the grid *wdh* 010425
          x.redim(0);
          x = subSurface.getGridSerial();
	  I=x.dimension(0);
	  x.reshape(I,x.dimension(3));
	}
	
      }
	
       
      if( append )
      {
        // ****************************************
        // **** Append the new curve to the old ***
        // ****************************************
	RealArray xSpline; 
        if( true )
	{
	  xSpline = spline.getKnotsS();  // wdh 011021 : use existing knots
	}
	else
	{
	  xSpline=spline.getGridSerial();
	  xSpline.reshape(xSpline.dimension(0),xSpline.dimension(3));
	}
	
	int numSpline=xSpline.getLength(0);

	const int xBase=x.getBase(0);
	const int xBound=x.getBound(0);
	 
	real dist00=max(fabs(x(xBase ,R3)-xSpline(0,R3))); 
	real dist10=max(fabs(x(xBound,R3)-xSpline(0,R3))); 
	real dist01=max(fabs(x(xBase ,R3)-xSpline(numSpline-1,R3))); 
	real dist11=max(fabs(x(xBound,R3)-xSpline(numSpline-1,R3))); 

	printf("append new edge : dist=(%e,%e,%e,%e) epsx=%e\n",dist00,dist10,dist01,dist11,epsx);
	if( dist00<epsx || dist11<epsx )
	{
	  // we reverse the direction of the points in these cases
	  Range R1(xBase,xBound);
	  RealArray y(R1,3);
	  for( int i=xBase; i<=xBound; i++ )
	    y(i,R3)=x(xBound-i+xBase,R3);
	  x=y;
	}

	int numberOfPointsNew=numSpline+xBound-xBase;  // new num = old + new -1
	Range I2=numberOfPointsNew;

        // ::display(x,"append new curve x","%22.16e ");	
        // ::display(xSpline,"old curve xSpline");	

	if( (dist01<epsx || dist11<epsx) && min(dist01,dist11) <= min(dist10,dist00) )
	{
	  // join as old --> new
	  Range R1(xBase+1,xBound);  // leave off the last point of x which is a duplicate.
	  xSpline.resize(I2,R3);
	  xSpline(R1+numSpline-(xBase+1),R3)=x(R1,R3);

	  spline.setPoints( xSpline(I2,0),xSpline(I2,1),xSpline(I2,2) );
	  numberOfPoints = numberOfPointsNew;
	}
	else if( dist10<epsx || dist00<epsx)
	{
	  // join as new --> old
	  Range R0(0,numSpline-1);
	  Range R1(xBase,xBound-1);
	  xSpline.resize(I2,R3);
	  xSpline(R0+xBound-xBase,R3)=xSpline(R0,R3);
	  xSpline(R1-xBase,R3)=x(R1,R3);

	  spline.setPoints( xSpline(I2,0),xSpline(I2,1),xSpline(I2,2) );
	  numberOfPoints = numberOfPointsNew;
	}
	else
	{
	  printf("ERROR: the new edge curve does not seem to match to an endpoint of the previous segment!\n"
		 "try again, or increase the edge curve tolerance (marching parameters dialog).\n");

          if( mappingBound>0. )
	  {
	    real newTol = epsx/(mappingBound*min(dist01,dist11,dist10,dist00));
	  
	    printf("   It would seem you need to increase the tolerance to %9.2e\n",newTol);
	  }
	  // ::display(x,"new curve x");	
	  // ::display(xSpline,"old curve xSpline");	
	}

        // ::display(xSpline,"new curve xSpline");	

	  
	real dist = max(fabs(xSpline(numberOfPoints-1,R3)-xSpline(0,R3)));
	if( debug & 2 )
            printf("Distance between first and last point on the initial curve =%e\n",dist);
	  
	// real scale = max(fabs(xSpline));
	if( dist < epsx ) // FLT_EPSILON*100.*scale )
	{
	  spline.setIsPeriodic(axis1,Mapping::functionPeriodic);
	  printf("I am setting the curve to be periodic. Edit the curve to change this\n");
	}
        else if( dist < epsx*100. )
	{
          printf("The curve is almost periodic, the distance between end points is %8.2e. My tolerance is %8.2e\n"
                 "Increase the edge curve tolerance (marching parameters dialog) to make this periodic\n",
		 dist,epsx);
	}
	
	if( debug & 4 ) 
	  ::display(xSpline,"xSpline after joining curves","%10.3e ");
	  
      } // end if( append )
      else 
      {
        // **** case: do not append, the curve is new ******
        spline.setShapePreserving(true);  // *wdh* 020930
	
	spline.setPoints( x(I,0),x(I,1),x(I,2) );
	if( subSurface.getIsPeriodic(0)==functionPeriodic )
	{
	  spline.setIsPeriodic(axis1,functionPeriodic);
	}
        // ** spline.setGridDimensions(axis1,numberOfPointsOnStartCurve); // why?? *wdh* 020925
	numberOfPointsOnStartCurve=I.getLength();
        
        // *** fix this for targetGridSpacing ***

        // ** do this else where *** setGridDimensions(axis1,numberOfPointsOnStartCurve);
        spline.setGridDimensions(axis1,numberOfPointsOnStartCurve);
      }
	


      returnValue=0;
       
    }
    
  } // end if mapPointer!=NULL
  
  
  return returnValue;
}





int
plotSurfaceAndEdges(GenericGraphicsInterface & gi, 
		    GraphicsParameters & params,
		    Mapping & surface, 
                    IntegerArray & subSurfaceIsActive,
		    Mapping* curve=0,
                    const int & numberOfEdgeCurves=0,
                    Mapping **edgeCurve=0,
                    int *edgeCurveWasSelected=0 )
// ==========================================================================================
/// \details 
///     Plot the reference surface, initial curve if defined and edges. 
///   Active subsurfaces of a Composite surface are plotted
///  in blue. Edges that have been chosen are in green, those not chosen are red.
/// 
/// \param surface (input): reference surface.
/// \param subSurfaceIsActive (input):
/// \param curve (input): if not NULL then this is the initial curve.
/// \param numberOfEdgeCurves,edgeCurve, edgeCurveWasSelected (input) : define the edge curves and those
///    selected.
///  
// ==========================================================================================
{
  
  bool compositeSurface = surface.getClassName()=="CompositeSurface";
  gi.erase();
  params.set(GI_MAPPING_COLOUR,"blue");
  // colour sub-surfaces by grid number.
  if( compositeSurface )
  {
    params.set(GI_GRID_LINE_COLOUR_OPTION,GraphicsParameters::colourByGrid);
    CompositeSurface & cs = (CompositeSurface&)surface;
    for(int s=0; s<cs.numberOfSubSurfaces(); s++ )
    {
      if( subSurfaceIsActive(s) )
	cs.setColour(s,"blue");
      else
	cs.setColour(s,"black");
    }
  }
  PlotIt::plot(gi,surface,params); // **** plot the surface ****

  params.set(GI_USE_PLOT_BOUNDS,TRUE);
  if( curve!=0 )
  {
    params.set(GI_MAPPING_COLOUR,"green");
    real oldCurveLineWidth;
    params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    params.set(GraphicsParameters::curveLineWidth,3.);
    PlotIt::plot(gi,*curve,params);
    params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);

  }

  params.set(GI_SURFACE_OFFSET,(real)20.);  // offset the surfaces so we see the edges better

        // params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
  for( int i=0; i<numberOfEdgeCurves; i++ )
  {
    real oldCurveLineWidth;
    params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    params.set(GraphicsParameters::curveLineWidth,3.);
    if( edgeCurveWasSelected[i] )
      params.set(GI_MAPPING_COLOUR,"green");
    else
      params.set(GI_MAPPING_COLOUR,"red");
    // printf("plot edge curve %i \n",i);
    PlotIt::plot(gi,*edgeCurve[i],params);
    params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  }
  params.set(GI_USE_PLOT_BOUNDS,FALSE);

  params.set(GI_SURFACE_OFFSET,(real)3.);

  return 0;
}


int
buildEdgeCurves( Mapping & map, 
                 const int & maximumNumberOfEdgeCurves, 
		 int & numberOfEdgeCurves, 
		 Mapping **edgeCurve,
                 int *edgeCurveWasNewed,
                 const IntegerArray & subSurfaceIsActive )
// ==========================================================================================
/// \details 
///    Build a collection of curves that represent the sides of a Mapping or CompositeSurface
/// 
/// \param subSurfaceIsActive (input) : if true, build edges for this sub-surface.
// ==========================================================================================
{
  const bool compositeSurface = map.getClassName()=="CompositeSurface";

  if( compositeSurface )
  {
    CompositeSurface & cs = (CompositeSurface &) map;
    // call this routine recursively for each sub-surface
    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
    {
      if( subSurfaceIsActive(s) )
        buildEdgeCurves( cs[s],maximumNumberOfEdgeCurves,numberOfEdgeCurves,edgeCurve,edgeCurveWasNewed,
                 subSurfaceIsActive);  
      // printf("buildEdgeCurves: after sub surface %i numberOfEdgeCurves=%i \n",s,numberOfEdgeCurves);
    }
  }
  else
  {
    const bool trimmedMapping = map.getClassName()=="TrimmedMapping";
    if( !trimmedMapping )
    {
      real inActiveAxis[3] = {-1.,-1.,-1.}; //  -1. = active
      for( int axis=0; axis<map.getDomainDimension(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
          inActiveAxis[axis]=side; // reduce the mapping to the edge r_{axis}=0. or r_{axis}=1
          assert(numberOfEdgeCurves< maximumNumberOfEdgeCurves );
          edgeCurve[numberOfEdgeCurves] = new ReductionMapping(map,inActiveAxis[0],inActiveAxis[1],inActiveAxis[2]);
          edgeCurve[numberOfEdgeCurves]->incrementReferenceCount();
	  
	  edgeCurveWasNewed[numberOfEdgeCurves]=TRUE;
          numberOfEdgeCurves++;
          inActiveAxis[axis]=-1.; // reset
	}
      }
    }
    else
    {
      // The trimming curves are in the parameter space of the surface of the TrimmedMapping.
      // We need to build a curve in cartesian space.
      TrimmedMapping & trim = (TrimmedMapping&)map;
      // printf("edge curves: TrimmedMapping: outerCurve=%i, getNumberOfInnerCurves()=%i, "
      //    "getNumberOfBoundarySubCurves()=%i \n",
      //            trim.getOuterCurve(),trim.getNumberOfInnerCurves(),trim.getNumberOfBoundarySubCurves());
#ifdef OLDSTUFF
      for( int i=-1; i<trim.getNumberOfInnerCurves(); i++ )
      {
        Mapping *c = i==-1 ? trim.getOuterCurve() : trim.getInnerCurve(i);
#else
      for ( int i=0; i<trim.getNumberOfTrimCurves(); i++ )
      {
	Mapping *c = trim.getTrimCurve(i);
#endif
	if( c!=0 )
	{
	  Mapping & trimCurve = *c;
          if( FALSE )
	  {
	    RealArray r; 
	    r = trimCurve.getGridSerial(); // .getLocalArrayWithGhostBoundaries();
	    r.reshape(r.dimension(0),2);
	    RealArray x(r.dimension(0),3);
	    assert( trim.surface!=0 );
	    trim.surface->mapS(r,x);
	    assert(numberOfEdgeCurves< maximumNumberOfEdgeCurves);
	    edgeCurve[numberOfEdgeCurves]=new NurbsMapping;
            edgeCurve[numberOfEdgeCurves]->incrementReferenceCount();

	    edgeCurveWasNewed[numberOfEdgeCurves]=TRUE;
	    ((NurbsMapping*)edgeCurve[numberOfEdgeCurves])->interpolate(x);
	    numberOfEdgeCurves++;
	  }
	  else
	  {
            // cout << "c->getClassName() = " << c->getClassName() << endl;
	    
            if( c->getClassName()=="NurbsMapping" )
	    {
              NurbsMapping & nurb = (NurbsMapping&)trimCurve;
              // printf("trimCurve is a nurb, number of subCurves = %i \n",nurb.numberOfSubCurves());
	      for( int subCurve=0; subCurve<nurb.numberOfSubCurves(); subCurve++ )
	      {
		NurbsMapping & subTrimCurve = nurb.subCurve(subCurve);
		assert(numberOfEdgeCurves< maximumNumberOfEdgeCurves);
		
		if( edgeCurveWasNewed[numberOfEdgeCurves] && 
		    edgeCurve[numberOfEdgeCurves]->decrementReferenceCount()==0 )
		  delete edgeCurve[numberOfEdgeCurves];
		
		edgeCurve[numberOfEdgeCurves]=new ComposeMapping(subTrimCurve,*trim.surface);
		edgeCurve[numberOfEdgeCurves]->incrementReferenceCount();
		
		edgeCurve[numberOfEdgeCurves]->setGridDimensions(axis1,subTrimCurve.getGridDimensions(axis1));
		edgeCurveWasNewed[numberOfEdgeCurves]=TRUE;
		numberOfEdgeCurves++;
	      }
	    }
	    else
	    {
	      assert(numberOfEdgeCurves<maximumNumberOfEdgeCurves);

	      if( edgeCurveWasNewed[numberOfEdgeCurves]  && 
                    edgeCurve[numberOfEdgeCurves]->decrementReferenceCount()==0 )
		delete edgeCurve[numberOfEdgeCurves];
		
	      edgeCurve[numberOfEdgeCurves]=new ComposeMapping(trimCurve,*trim.surface);
              edgeCurve[numberOfEdgeCurves]->incrementReferenceCount();

	      edgeCurve[numberOfEdgeCurves]->setGridDimensions(axis1,trimCurve.getGridDimensions(axis1));
	      edgeCurveWasNewed[numberOfEdgeCurves]=TRUE;
	      numberOfEdgeCurves++;
	    }
            // trimCurve.display("trimCurve");
	    // trim.surface->display("trim.surface");
            // edgeCurve[numberOfEdgeCurves]->display("Composition");
	  }
	}
      }
    }
  }
  return 0;
}



// ***** obsolete ***** but there may be some code here we still want ******


///  int HyperbolicMapping::
///  createCurveFromASurface(GenericGraphicsInterface & gi, Mapping & surface, Mapping* &curve,
///                          const aString & command /* = nullString */,
///  			DialogData *interface /* =NULL */)
// // =================================================================================================
/// \param / /Description:
///  //    Create a curve that can be used as a starting curve for the hyperbolic surface grid generator
///  // 
///  // There are a number of ways to create an initial curve.
///  // <ul>
///  //   <li>[choose an edge] : build a curve as a union of "edge curves". Edge curves are created
///  //      for each boundary of the surface, including each boundary of a composite surface of trimmed
///  //      mappings. 
///  //   <li>[coordinate line] : build a curve from a coordinate line. 
///  // </ul>
///  //
// // =================================================================================================
// {
//   int returnValue=0;

//   aString prefix = "HYPC:"; // prefix for commands to make them unique.

//   const bool executeCommand = command!=nullString;
//   if( false &&   // don't check prefix for now
//       executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
//     return 1;


//   const aString menu[] = 
//   {
//     "!create curve from surface",
//     "choose an edge",
//     ">choose active sub-surfaces",
//       "turn on all sub-surfaces",
//       "turn off all sub-surfaces",
//       "pick active sub-surfaces",
//       "specify active sub-surfaces",
//     "<coordinate line",
//     "reparameterized coordinate line",
//     "project a line",
//     "project a spline",
// //    "intersect with a plane",
//     "help",
//     "exit", 
//     "" 
//   };
//   const aString help[] = 
//   {
//     "choose an edge: create a curve from the edge of a surface",
//     "coordinate line: create a curve from a coordinate line",
//     "reparameterized coordinate line: create a curve from a reparameterized coordinate line",
//     "project a line: project a line onto the surface.",
//     "project a spline: project a spline onto the surface.",
// //    "intersect with a plane: create a curve as the intersection of a plane with the surface",

// //    "sub-surface edge: choose an edge from a particular sub-surface",
//     "help  : print this list",
//     "exit",
//     ""
//   };
//   const aString pickMenu[]=
//   {
//     "done",
//     ""
//   };
//   aString answer;

//   GUIState gui;
//   gui.setWindowTitle("Initial Curve Options");
//   gui.setExitCommand("exit", "continue");
//   DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;


//   if( interface==NULL || command=="build dialog" )
//   {
//     const int maxCommands=20;
//     aString cmd[maxCommands];

//     aString pbLabels[] = {"choose an edge",
//                           "coordinate line",
//                           "project a line",
//                           "project a spline",
// 			  ""};
//     // addPrefix(pbLabels,prefix,cmd,maxCommands);
//     int numRows=4;
//     dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

// //     aString label[] = {"polynomial","trigonometric","pulse",""}; //
// //     addPrefix(label,prefix,cmd,maxCommands);
// //     dialog.addOptionMenu("type", cmd,label, (int)twilightZoneChoice);

// //     aString tbLabel[] = {"twilight zone flow","use 2D function in 3D","compare 3D run to 2D",""};
// //     int tbState[4];
// //     tbState[0] = twilightZoneFlow;
// //     tbState[1] = dimensionOfTZFunction==2;
// //     tbState[2] = compare3Dto2D; 
// //     tbState[3]=0;
// //     addPrefix(tbLabel,prefix,cmd,maxCommands);

// //     int numColumns=1;
// //     dialog.setToggleButtons(cmd, tbLabel, tbState, numColumns); 

// //     const int numberOfTextStrings=4;
// //     aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

// //     int nt=0;
// //     textLabels[nt] = "degree in space"; 
// //     sPrintF(textStrings[nt], "%i", tzDegreeSpace); nt++; 

// //     textLabels[nt] = "degree in time"; 
// //     sPrintF(textStrings[nt], "%i", tzDegreeTime); nt++; 

// //     textLabels[nt] = "frequencies (x,y,z,t)"; 
// //     sPrintF(textStrings[nt], "%g, %g, %g, %g",omega[0],omega[1],omega[2],
// // 	    omega[3]); 
// //     nt++; 
// //     // null strings terminal list
// //     textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

// //     addPrefix(textLabels,prefix,cmd,maxCommands);
// //     dialog.setTextBoxes(cmd, textLabels, textStrings);

//     gui.buildPopup(menu);
//   }

//   if( !executeCommand  )
//   {
//     gi.pushGUI(gui);
//     gi.appendToTheDefaultPrompt("create curve>"); // set the default prompt
//   }



//   IntegerArray subSurfaceIsActive(1);
//   subSurfaceIsActive=FALSE;
//   aString *compositeSurfaceColours=NULL;  // to save original composite surface colours.
  

//   IntegerArray selection;
  
//   GraphicsParameters params;
//   bool compositeSurface = surface.getClassName()=="CompositeSurface";
  
//   // Count up the number of edges that we need to create. Note that each trimming curve
//   // could be made up of a number of sub-curves. Each sub curve is created as a separate edge.
//   int maximumNumberOfEdgeCurves=0;
//   Mapping **edgeCurve;
//   if( compositeSurface )
//   {
//     CompositeSurface & cs = (CompositeSurface &)surface;

//     subSurfaceIsActive.redim(max(1,cs.numberOfSubSurfaces()));
//     subSurfaceIsActive=FALSE;

//     compositeSurfaceColours=new aString[max(1,cs.numberOfSubSurfaces())];
//     for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
//     {
//       // save the colours on the composite surface.
//       compositeSurfaceColours[s]=cs.getColour(s);
//       if( cs[s].getClassName()=="TrimmedMapping" )
//       {
// 	maximumNumberOfEdgeCurves+=((TrimmedMapping&)cs[s]).getNumberOfBoundarySubCurves();
//       }
//       else
// 	maximumNumberOfEdgeCurves+=4;

//       // printf("after sub-surface %i maximumNumberOfEdgeCurves=%i\n",s,maximumNumberOfEdgeCurves);
//     }
//   }
//   else
//   {
//     subSurfaceIsActive.redim(1);
//     subSurfaceIsActive=TRUE;
//     if( surface.getClassName()=="TrimmedMapping" )
//       maximumNumberOfEdgeCurves+=((TrimmedMapping&)surface).getNumberOfBoundarySubCurves();
//     else
//       maximumNumberOfEdgeCurves+=4;
//   }
  
//   edgeCurve = new Mapping* [maximumNumberOfEdgeCurves];
//   int *edgeCurveWasNewed = new int [maximumNumberOfEdgeCurves];
//   int *edgeCurveWasSelected = new int [maximumNumberOfEdgeCurves];

//   int numberOfEdgeCurves=0;
//   int i;
//   for( i=0; i<maximumNumberOfEdgeCurves; i++ )
//   {
//     edgeCurve[i]=0;
//     edgeCurveWasNewed[i]=FALSE;
//     edgeCurveWasSelected[i]=0;
//   }
  
//   params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

//   bool plotShadedMappingBoundaries =FALSE;
//   // GraphicsParameters::ColourOptions gridLineColourOption=GraphicsParameters::colourByGrid;
//   bool plotLinesOnMappingBoundaries=TRUE;

//   real mappingBound=0.;
//   for( int axis=0; axis<surface.getRangeDimension(); axis++ )
//   {
//     printf(" getRangeBound(End,axis)=%e, getRangeBound(Start,axis)=%e \n",(real)surface.getRangeBound(End,axis),
// 	   (real)surface.getRangeBound(Start,axis));
//     mappingBound=max(mappingBound,max(fabs((real)surface.getRangeBound(End,axis)),
//                                       fabs((real)surface.getRangeBound(Start,axis))));
//   }
//   printf("mappingBound = %e\n",mappingBound);
  
//   if( compositeSurface )
//   {
//     params.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotShadedMappingBoundaries);
//     // params.set(GI_GRID_LINE_COLOUR_OPTION,gridLineColourOption);
//     params.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,plotLinesOnMappingBoundaries);
//     CompositeSurface & cs = (CompositeSurface&)surface;
//     for(int s=0; s<cs.numberOfSubSurfaces(); s++ )
//       cs.setColour(s,"black");
//   }

//   Range xAxes(0,surface.getRangeDimension()-1);     

//   for( int it=0;; it++ )
//   {
//     plotObject=TRUE;  // replot 
    
//     if( !executeCommand )
//     {
//       if( it==0 && plotObject )
// 	answer="plotObject";
//       else
// 	gi.getAnswer(answer,"");
//     }
//     else
//     {
//       if( it==0 ) 
//         answer=command;
//       else
//         break;
//     }
  
//     if( answer(0,prefix.length()-1)==prefix )
//       answer=answer(prefix.length(),answer.length()-1);


// //     if( it==0 && plotObject )
// //       answer="plotObject";
// //     else
// //       gi.getMenuItem(menu,answer);
 
//     if( answer=="project a line" )
//     {
//       // create a starting curve by projecting a line onto the surface.
//       real  x1=0.,y1=0.,z1=0., x2=1.,y2=0.,z2=0.;
//       int numberOfPoints=21;
//       LineMapping line;
//       line.setGridDimensions(axis1,numberOfPoints);
      
//       SplineMapping & spline = *new SplineMapping;
//       spline.setGridDimensions(axis1,numberOfPoints);

//       aString menuLine[]=
//       {
// 	"choose end points",
//         "number of points",
//         "exit",
//         ""
//       };
//       bool lineDefined=FALSE;
//       for( ;; )
//       {
// 	gi.getMenuItem(menuLine,answer,"project>");
// 	if( answer=="exit" )
// 	{
// 	  break;
// 	}
// 	else if( answer=="choose end points" )
// 	{
//           lineDefined=TRUE;
// 	  printf("Enter the end points to the line\n");
// 	  gi.inputString(answer,sPrintF(buff,"Enter x1,y1,z1, x2,y2,z2 (default=(%g,%g,%g) (%g,%g,%g))", 
// 					x1,y1,z1, x2,y2,z2));
// 	  if( answer!="" )
// 	  {
// 	    sScanF(answer,"%e %e %e %e %e %e",&x1,&y1,&z1, &x2,&y2,&z2);
// 	  }
// 	  line.setPoints(x1,y1,z1, x2,y2,z2 );
      
// 	}
// 	else if( answer=="number of points" )
// 	{
//           gi.inputString(answer,sPrintF(buff,"Enter the number of points on the line (current=%i)",numberOfPoints));
// 	  if( answer!="" )
// 	  {
//             sScanF(answer,"%i",&numberOfPoints);
//             line.setGridDimensions(axis1,numberOfPoints);
//             spline.setGridDimensions(axis1,numberOfPoints);
// 	  }
// 	}
//         else
// 	{
// 	  cout << "Unknown response=" << answer << endl;
// 	  gi.stopReadingCommandFile();
//           break;
// 	}
//         if( lineDefined )
// 	{
// 	  Range I(0,numberOfPoints-1);
// 	  // RealArray r(I,2),x;
// 	  RealArray x;
// 	  x=line.getGrid(); // .getLocalArrayWithGhostBoundaries();
// 	  x.reshape(I,3);
      
//           // project the points onto the surface 
//           MappingProjectionParameters mpParams;
//           surface.project(x,mpParams);
// 	  spline.setPoints( x(I,0),x(I,1),x(I,2) );
//           curve=&spline;

// 	  gi.erase();
// 	  params.set(GI_MAPPING_COLOUR,"blue");
// 	  // colour sub-surfaces by grid number.
//           if( compositeSurface )
// 	    params.set(GI_GRID_LINE_COLOUR_OPTION,GraphicsParameters::colourByGrid);
// 	  PlotIt::plot(gi,surface,params); // **** plot the surface ****

// 	  params.set(GI_USE_PLOT_BOUNDS,TRUE);

// 	  real oldCurveLineWidth;
// 	  params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
// 	  params.set(GraphicsParameters::curveLineWidth,3.);

// 	  params.set(GI_MAPPING_COLOUR,"red");
//           PlotIt::plot(gi,line,params);

// 	  params.set(GI_MAPPING_COLOUR,"green");
// 	  PlotIt::plot(gi,*curve,params);

// 	  params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
// 	  params.set(GI_USE_PLOT_BOUNDS,FALSE);

// 	}
//       }
//     }
//     else if( answer=="project a spline" )
//     {
//       // create a starting curve by projecting a spline onto the surface.
//       SplineMapping initialSpline(3);
      
//       MappingInformation mapInfo;
//       mapInfo.graphXInterface=&gi;
      
//       SplineMapping & spline = *new SplineMapping;
//       aString menuLine[]=
//       {
//         "!project a spline",
// 	"change the spline",
//         "change the points",
//         "change the projected spline",
//         "exit",
//         ""
//       };
//       MappingProjectionParameters mpParams;

//       for( int iter=0; ; iter++ )
//       {
//         if( iter==0 )
//           answer="change the spline";
// 	else
// 	  gi.getMenuItem(menuLine,answer,"project>");
// 	if( answer=="exit" )
// 	{
// 	  break;
// 	}
//         else if( answer=="change the spline" )
// 	{
// 	  printf("Create a spline to project\n");
// 	  gi.erase();
// 	  initialSpline.update(mapInfo);
// 	}
//         else if( answer=="change the points" )
// 	{
//           const int numberOfSplinePoints=initialSpline.getNumberOfKnots();
// 	  RealArray knots(numberOfSplinePoints,3);
// 	  for( int i=0; i<numberOfSplinePoints; i++ )
// 	  {
// 	    gi.inputString(answer,sPrintF(buff,"Enter x,y,z for point %i",i));
// 	    sScanF(answer,"%e %e %e ",&knots(i,axis1),&knots(i,axis2),&knots(i,axis3));
// 	  }
//           Range I=knots.dimension(0);
//           initialSpline.setPoints(knots(I,0),knots(I,1),knots(I,2));
// 	}
//         else if( answer=="change the projected spline" )
// 	{
// 	  gi.erase();
// 	  spline.update(mapInfo);
// 	}
//         else
// 	{
// 	  cout << "Unknown response=" << answer << endl;
// 	  gi.stopReadingCommandFile();
//           break;
// 	}
      
// 	// project the points onto the surface 
//         RealArray x; 
//         SplineMapping & sp = answer!="change the projected spline" ? initialSpline : spline;

//         x = sp.getGrid(); // .getLocalArrayWithGhostBoundaries();
//         Range I = x.dimension(0);
//         x.reshape(I,3);
// 	surface.project(x,mpParams);

// 	spline.setPoints( x(I,0),x(I,1),x(I,2) );
//         spline.setIsPeriodic(axis1,initialSpline.getIsPeriodic(axis1));
//         spline.setGridDimensions(axis1,initialSpline.getGridDimensions(axis1));
// 	curve=&spline;

// 	gi.erase();
// 	params.set(GI_MAPPING_COLOUR,"blue");
// 	// colour sub-surfaces by grid number.
// 	if( compositeSurface )
// 	  params.set(GI_GRID_LINE_COLOUR_OPTION,GraphicsParameters::colourByGrid);
// 	PlotIt::plot(gi,surface,params); // **** plot the surface ****

// 	params.set(GI_USE_PLOT_BOUNDS,TRUE);

// 	real oldCurveLineWidth;
// 	params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
// 	params.set(GraphicsParameters::curveLineWidth,3.);

// 	params.set(GI_MAPPING_COLOUR,"red");
// 	PlotIt::plot(gi,sp,params);

// 	params.set(GI_MAPPING_COLOUR,"green");
// 	PlotIt::plot(gi,*curve,params);

// 	params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
// 	params.set(GI_USE_PLOT_BOUNDS,FALSE);

//       }
//     }
// /* ----
//     else if( answer=="intersect with a plane" )
//     {
//       // ****** trouble here getting the intersection curve on a CompositeSurface **********

//       // create a curve as the intersection of a plane and the surface.
//       aString menui[]=
//       {
//         "!intersect with a plane",
// 	"define a plane",
//         "compute intersection",
//         "exit",
//         ""
//       };
//       PlaneMapping plane;
//       IntersectionMapping intersect;
      
//       real  x1=0.,y1=0.,z1=0., x2=1.,y2=0.,z2=0., x3=0.,y3=1.,z3=0.;

//       printf("A plane or rhombus is defined by 3 of it's corners\n");
//       for( ;; )
//       {
// 	gi.getMenuItem(menu,answer,"plane>");

// 	if( answer=="exit" )
// 	{
//           break;
// 	}
// 	else if( answer=="define a plane" )
// 	{
// 	  gi.inputString(line,sPrintF(buff,
// 				      "Enter x1,y1,z1, x2,y2,z2, x3,y3,z3 (default=(%6.2e,%6.2e,%6.2e)"
// 				      " ,(%6.2e,%6.2e,%6.2e),(%6.2e,%6.2e,%6.2e) ): ",
// 				      x1,y1,z1, x2,y2,z2, x3,y3,z3));
// 	  if( line!="" )
// 	  {
// 	    sScanF(line,"%e %e %e %e %e %e %e %e %e",&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3);
// 	    plane.setPoints(x1,y1,z1, x2,y2,z2, x3,y3,z3);
// 	  }

// 	}
//         else if( answer=="compute intersection" )
// 	{
// 	}
// 	else
// 	{
// 	}
//       }
//     }
// ------ */
//     else if( answer=="coordinate line" || answer=="reparameterized coordinate line" )
//     {
//       Mapping *map=NULL;
//       aString answer2;
//       if( compositeSurface )
//       {
// 	// If a composite surface : choose coordinate line from which sub-surface?
//         CompositeSurface & cs = (CompositeSurface &)surface;
//         gi.inputString(answer2,sPrintF(buff,"choose coordinate line from which sub-surface? (in the range 0..%i)",
//                        cs.numberOfSubSurfaces()-1)); 
//         if( answer2!="" )
// 	{
//           int subSurface=-1;
//           sScanF(answer2,"%i",&subSurface);
//           if( subSurface>=0 && subSurface<cs.numberOfSubSurfaces() )
// 	  {
//             map = & cs[subSurface];
// 	  }
// 	  else
// 	  {
//             printf("Invalid subSurface. Must be in the range [0,%i] \n",cs.numberOfSubSurfaces()-1);
// 	  }
// 	}
// 	else
//           continue;
//       }
//       else
//         map=&surface;
//       if( map!=NULL )
//       {
// 	// choose the coordinate line, axis=value
// 	gi.inputString(answer2,"Enter a coordinate line as `axis1=value' or `axis2=value'");
// 	int mapAxis=-1;
// 	real value=0.;
// 	sScanF(answer2,"axis%i=%e",&mapAxis,&value);
// 	if( mapAxis==1 || mapAxis==2 )
// 	{
// 	  curve= new ReductionMapping(*map,mapAxis-1,value);
//           if( answer=="reparameterized coordinate line" )
// 	  {
//             // turn the curve into a spline so that it can be reparameterized
//             SplineMapping & spline = *new SplineMapping;  // ************* need to reference count this *****
	    
//             RealArray x = curve->getGrid(); // .getLocalArrayWithGhostBoundaries();
//             Range R=Range(0,x.getLength(0)*x.getLength(1)*x.getLength(2)-1);
            
// 	    x.reshape(R,3);
	    
// 	    spline.setPoints(x(R,0),x(R,1),x(R,2));
// 	    spline.setIsPeriodic(axis1,curve->getIsPeriodic(axis1));
//             spline.setGridDimensions(axis1,R.getLength());
	    
//             delete curve;
// 	    curve=&spline;
// 	  }
// 	  break;
// 	}
// 	else
// 	{
// 	  printf("Unknown response: answer=[%s]\n",(const char*)answer);
// 	  gi.stopReadingCommandFile();
// 	}
//       }
//     }
//     else if( answer=="turn on all sub-surfaces" )
//     {
//       subSurfaceIsActive=TRUE;
//     }
//     else if( answer=="turn off all sub-surfaces" )
//     {
//       subSurfaceIsActive=FALSE;
//     }
//     else if( answer=="pick active sub-surfaces" )
//     {
//       if( compositeSurface )
//       {
//         CompositeSurface & cs = (CompositeSurface &)surface;
// // AP: rework this later...
// 	SelectionInfo select;
// 	GUIState selectInterface;
// 	selectInterface.buildPopup(pickMenu);
// 	gi.pushGUI(selectInterface);
	
// 	for(int it=0; it<100; it++)
// 	{
// 	  if( it>0 )
// 	  { 
// 	    // replot
// 	    gi.erase();
// 	    plotSurfaceAndEdges(gi,params,surface,subSurfaceIsActive);
// 	  }
// // AP: fix this
// //	  gi.getMenuItem(pickMenu,answer,"select objects with mouse",selection);
// 	  gi.getAnswer(answer, "select objects with mouse", select);
// 	  if( answer=="done" )
// 	    break;
// 	  else if( select.nSelect > 0 )
// 	  {
// 	    for( int i=0; i < select.nSelect; i++ )
// 	    {
// 	      for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
// 	      {
// 		if( select.selection(i,0)==cs[s].getGlobalID() )
// 		{
// 		  printf("sub surface %i is on. \n",s);
// 		  subSurfaceIsActive(s)=TRUE;
// 		}
// 	      }
// 	    }
// 	  }
// 	  else
// 	  {
// 	    cout << "unknown response=" << answer << endl;
// 	    gi.stopReadingCommandFile();
// 	  }
// 	}
// 	gi.popGUI();
//       }
//       plotObject=FALSE;
//     }
//     else if( answer=="specify active sub-surfaces" )
//     {
//       if( compositeSurface )
//       {
// 	// If a composite surface : choose coordinate line from which sub-surface?
//         CompositeSurface & cs = (CompositeSurface &)surface;

//         IntegerArray subSurface;
//         int numberSpecified = gi.getValues(sPrintF(buff,"Enter sub-surfaces (range 0..%i)",cs.numberOfSubSurfaces()-1),
// 					   subSurface,0,cs.numberOfSubSurfaces()-1);
// 	for( int i=0; i<numberSpecified; i++ )
//           subSurfaceIsActive(subSurface(i))=TRUE;
//       }
//       else
//       {
//         printf("INFO: This is not a CompositeSurface so there are no sub-surfaces\n");
//       }
//     }
//     else if( answer=="choose an edge" )
//     {
//       gi.appendToTheDefaultPrompt("edge>"); // set the default prompt
//       if( compositeSurface && max(subSurfaceIsActive)==0 )
//       {
// 	printf("There are no active sub surfaces for edges. Pick or select active surfaces first\n");
// 	continue;
//       }
//       // first delete any old edge curves.
//       for( i=0; i<numberOfEdgeCurves; i++ )
//       {
// 	if( edgeCurveWasNewed[i] && edgeCurve[i]->decrementReferenceCount()==0 )
// 	  delete edgeCurve[i];
//         edgeCurve[i]=NULL;
//       }

// // Build edge curves on active sub-surfaces.
//       buildEdgeCurves( surface, maximumNumberOfEdgeCurves,numberOfEdgeCurves,edgeCurve,edgeCurveWasNewed,
// 		       subSurfaceIsActive  );

//       aString *pickMenu = NULL; 
//       int numberOfEdgeCurvesSelected=0;

// // AP: temporary solution. Clean this up later
//       GUIState selectInterface;
//       SelectionInfo select;
      
//       for(int it2=0; it2<100; it2++)
//       {
// 	gi.erase();
// 	plotSurfaceAndEdges(gi, params, surface, subSurfaceIsActive, 0,
// 			    numberOfEdgeCurves, edgeCurve, edgeCurveWasSelected );

// 	delete [] pickMenu;
// 	pickMenu = new aString [3+numberOfEdgeCurves];
// 	int pick=0;
// 	pickMenu[pick++]="done";
// 	pickMenu[pick++]="specify edge curves";

// 	for( i=0; i<numberOfEdgeCurves; i++ )
// 	{
//           if( edgeCurveWasSelected[i] )
//             pickMenu[pick++]=sPrintF(buff,"curve %i (on)",i);
// 	}
// 	pickMenu[pick]="";
	
// 	selectInterface.buildPopup(pickMenu);
// 	gi.pushGUI(selectInterface);
	
// // AP: fix this
// //        gi.getMenuItem(pickMenu,answer,"select edge curves with mouse",selection);
//         gi.getAnswer(answer, "select edge curves with mouse", select);
// 	gi.popGUI();
//         if( select.nSelect > 0 )
// 	{
//           for( i=0; i<select.nSelect; i++ )
// 	  {
// 	    for( int e=0; e<numberOfEdgeCurves; e++ )
// 	    {
// 	      if( !edgeCurveWasSelected[e] && select.selection(i,0)==edgeCurve[e]->getGlobalID() )
// 	      {
// 	        printf("edge curve %i selected. \n",e);
//                 numberOfEdgeCurvesSelected++;
//                 edgeCurveWasSelected[e]=numberOfEdgeCurvesSelected;
// 	      }
// 	    }
// 	  }
//         }
//         else if( answer=="specify edge curves" )
// 	{
// 	  IntegerArray edge;
// 	  int numberSpecified = gi.getValues(sPrintF(buff,"Enter edge curves (range 0..%i)",numberOfEdgeCurves-1),
// 					     edge,0,numberOfEdgeCurves-1);
// 	  for( int i=0; i<numberSpecified; i++ )
// 	  {
//             int e = edge(i);
// 	    if( !edgeCurveWasSelected[e] )
// 	    {
// 	      numberOfEdgeCurvesSelected++;
// 	      edgeCurveWasSelected[e]=numberOfEdgeCurvesSelected;
// 	    }
// 	  }
// 	}
//         else if( answer(0,4)=="curve" )
// 	{
//           // turn off this curve
//           int edge=-1;
// 	  sScanF(answer,"curve %i",&edge);
// 	  if( edge>=0 && edge<numberOfEdgeCurves )
// 	  {
//             printf("delete edge curve %i\n",edge);
//             for( i=0; i<numberOfEdgeCurves; i++ )
// 	    {
//               if(edgeCurveWasSelected[i]>edge )
//                 edgeCurveWasSelected[i]--;
// 	    }
// 	    edgeCurveWasSelected[edge]=0;
// 	    numberOfEdgeCurvesSelected--;
// 	  }
//           else
// 	  {
//             cout << "Unknown response: " << answer << endl;
// 	  }
// 	}
//         else if( answer=="done" )
// 	{
//           // choose the first edge curve selected to be the starting curve.
      
// 	  if( numberOfEdgeCurvesSelected==1 )
// 	  {
//             for( i=0; i<numberOfEdgeCurves; i++ )
// 	    {
//               if( edgeCurveWasSelected[i] )
// 	      {
// 		curve=edgeCurve[i];
// 		break;
// 	      }
// 	    }
// 	  }
// 	  else if( numberOfEdgeCurvesSelected>1 )
// 	  {
//             // **** build a spline to hold the union of the chosen curves. ****
//             SplineMapping & spline = *(new SplineMapping);
//             spline.setShapePreserving();
//             RealArray xSpline;   // save knots for the spline in here

//             // First evaluate all chosen edge curves and save in the arrays xEdge[j]
//             RealArray *xEdge = new RealArray [numberOfEdgeCurvesSelected];
	    
// 	    for( int j=0; j<numberOfEdgeCurvesSelected; j++ )
// 	    {
// 	      for( i=0; i<numberOfEdgeCurves; i++ )
// 	      {
// 		if( edgeCurveWasSelected[i]==j+1 )
//                   break;
// 	      }
//               assert( i<numberOfEdgeCurves );
// 	      Mapping & edge = *edgeCurve[i];
//               xEdge[j]=edge.getGrid(); // .getLocalArrayWithGhostBoundaries();
// 	      xEdge[j].reshape(xEdge[j].dimension(0),3);
// 	    }

//             int numberOfPoints=xEdge[0].getLength(0);
//             Range R(0,numberOfPoints-1); 
//             xSpline=xEdge[0](R,xAxes);  // put the first edge curve into the spline.
//             intArray edgeNotAdded(numberOfEdgeCurvesSelected);
//             edgeNotAdded=TRUE;
// 	    edgeNotAdded(0)=FALSE;
	    
//             // **** Here is the tolerance for matching end points on curves ****
// 	    const real eps=FLT_EPSILON*mappingBound*100.;

//             int edgeStart=1, edgeEnd=numberOfEdgeCurvesSelected;
// 	    for( int e=0; e<numberOfEdgeCurvesSelected; e++ )
// 	    {
// 	      int newEdge=-1;
// 	      for( int j=edgeStart; j<edgeEnd; j++ ) 
// 	      {
//                 if( !edgeNotAdded(j) )
// 		{
// 		  if( j==edgeStart )
// 		    edgeStart++;
// 		  else if( j==edgeEnd )
// 		    edgeEnd--;
// 		}
// 		else
// 		{
//                   // try to add this new edge
// 	          newEdge=j;

// 		  assert( newEdge>=0 && newEdge<numberOfEdgeCurvesSelected);
// 		  RealArray & x =xEdge[newEdge];
// 		  const int xBase=x.getBase(0);
// 		  const int xBound=x.getBound(0);

// 		  real dist00=max(fabs(x(xBase ,xAxes)-xSpline(0,xAxes))); 
// 		  real dist10=max(fabs(x(xBound,xAxes)-xSpline(0,xAxes))); 
// 		  real dist01=max(fabs(x(xBase ,xAxes)-xSpline(numberOfPoints-1,xAxes))); 
// 		  real dist11=max(fabs(x(xBound,xAxes)-xSpline(numberOfPoints-1,xAxes))); 

// 		  printf("newEdge=%i : dist=(%e,%e,%e,%e)\n",newEdge,dist00,dist10,dist01,dist11);
// 		  if( dist00<eps || dist11<eps )
// 		  {
// 		    // we reverse the direction of the points in these cases
// 		    Range R1(xBase,xBound);
// 		    RealArray y(R1,3);
// 		    for( int i=xBase; i<=xBound; i++ )
// 		      y(i,xAxes)=x(xBound-i+xBase,xAxes);
// 		    x=y;
// 		  }
// 		  if( dist01<eps || dist11<eps )
// 		  {
// 		    // join as old --> new
// 		    Range R1(xBase+1,xBound);  // leave off the last point of x which is a duplicate.
// 		    xSpline.resize(numberOfPoints+xBound-xBase,xAxes);
// 		    xSpline(R1+numberOfPoints-(xBase+1),xAxes)=x(R1,xAxes);
// 		    numberOfPoints+=xBound-xBase;

// 		    edgeNotAdded(newEdge)=FALSE;
// 		  }
// 		  else if( dist10<eps || dist00<eps)
// 		  {
// 		    // join as new --> old
// 		    Range R0(0,numberOfPoints-1);
// 		    Range R1(xBase,xBound-1);
// 		    xSpline.resize(numberOfPoints+xBound-xBase,xAxes);
// 		    xSpline(R0+xBound-xBase,xAxes)=xSpline(R0,xAxes);
// 		    xSpline(R1-xBase,xAxes)=x(R1,xAxes);
// 		    numberOfPoints+=xBound-xBase;

// 		    edgeNotAdded(newEdge)=FALSE;
// 		  }
		  
// 		}
// 	      }  // end for j
// 	      if( newEdge==-1 )
//                 break;  // we are done

// 	    } // end for e
//             int numberNotAdded=sum(edgeNotAdded);
//             if( numberNotAdded>0 )
// 	    {
// 	      printf("ERROR:Unable to merge all edge curves. Number merged=%i. Number not merged=%i\n",
// 		     numberOfEdgeCurvesSelected-numberNotAdded,numberNotAdded);
// 	      // ::display(xSpline(Range(0,oldNum-1)),"Here are the spline pts up to now");
// 	      // ::display(x,"Here are the new points to be merged");
// 	    }

//             R=Range(0,numberOfPoints-1);
// 	    spline.setPoints(xSpline(R,0),xSpline(R,1),xSpline(R,2));
//             real dist = max(fabs(xSpline(numberOfPoints-1,xAxes)-xSpline(0,xAxes)));
// 	    real scale = max(fabs(xSpline));
// 	    if( dist < FLT_EPSILON*100.*scale )
// 	    {
// 	      spline.setIsPeriodic(axis1,Mapping::functionPeriodic);
// 	      printf("I am setting the curve to be periodic. Edit the curve to change this");
// 	    }
// 	    delete [] xEdge;

// /* ----
//             params.set(GI_TOP_LABEL,"spline joining edges");
//             params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
//             gi.erase();
//             PlotIt::plot(gi,spline,params);
//             params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
// ---- */
// 	    curve=&spline;
	    
// 	  }
// 	  break;
// 	}
// 	else
// 	{
// 	  cout << "createCurveFromASurface: unknown response=" << answer << endl;
// 	  gi.stopReadingCommandFile();
// 	}
//       }
//       delete [] pickMenu;
//       gi.unAppendTheDefaultPrompt();  // reset

//     }
//     else if( answer=="plotObject" )
//     { 
//       plotObject=TRUE;
//     }
//     else if( answer=="exit" )
//     {
//       break;
//     }
//     else 
//     {
//       if( executeCommand )
//       {
// 	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
//         break;
//       }
//       else
//       {
//         gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
// 	cout << "Unknown response: [" << answer << "]\n";
// 	gi.stopReadingCommandFile();
//       }
//     }

//     if( plotObject )
//     {
//       gi.erase();
//       plotSurfaceAndEdges(gi,params,surface,subSurfaceIsActive,curve);

//     }

//   }
//   params.set(GI_SURFACE_OFFSET,(real)3.);  // reset

//   // reste the colours on the composite surface.
//   if( compositeSurface )
//   {
//     CompositeSurface & cs = (CompositeSurface &)surface;
//     for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
//       cs.setColour(s,compositeSurfaceColours[s]);
//   }
//   delete [] compositeSurfaceColours;

//   for( i=0; i<numberOfEdgeCurves; i++ )
//   {
//     if( edgeCurveWasNewed[i] && curve!=edgeCurve[i] && edgeCurve[i]->decrementReferenceCount()==0 )
//       delete edgeCurve[i];
//   }
//   delete [] edgeCurve;
//   delete [] edgeCurveWasNewed;
//   delete [] edgeCurveWasSelected;



//   if( !executeCommand  )
//   {
//     gi.popGUI();
//     gi.unAppendTheDefaultPrompt();
//   }

//   return returnValue;
// }
    
