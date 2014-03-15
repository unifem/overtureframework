#include "HyperbolicMapping.h"
#include "CompositeSurface.h"
#include "GL_GraphicsInterface.h"
#include "CompositeTopology.h"
#include "UnstructuredMapping.h"
#include "ReductionMapping.h"
#include "arrayGetIndex.h"
#include "MatchingCurve.h"
#include "display.h"

// Declare and define base and bounds, perform loop
#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// Perform loop
#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )


static inline 
double
tetVolume6(real *p1, real*p2, real *p3, real *p4 )
{
  // Rteurn 6 times the volume of the tetrahedra
  // (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
  // 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
  return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) ;
	  
}

inline 
bool
hexIsBad( real *v000, real *v100, real *v010, real *v110, real *v001, real *v101, real *v011, real *v111, const real orientation=1. )
// =====================================================================================================
// Return true if the hex defined by the vertices v000,v100,... has any tetrahedra that are negative.
// =====================================================================================================
{
//    real vol1=tetVolume6(v000,v100,v010, v001);
//    real vol2=tetVolume6(v110,v010,v100, v111);
//    real vol3=tetVolume6(v101,v001,v111, v100);
//    real vol4=tetVolume6(v011,v111,v001, v010);
  
//    printf(" tetVolume = %8.2e,%8.2e,%8.2e,%8.2e,\n",vol1,vol2,vol3,vol4);
  
  // *** for now we just check the volumes of 4 tetrahedra
  if( tetVolume6(v000,v100,v010, v001)*orientation <=0. ) return true;
  if( tetVolume6(v110,v010,v100, v111)*orientation <=0. ) return true;
  if( tetVolume6(v101,v001,v111, v100)*orientation <=0. ) return true;
  if( tetVolume6(v011,v111,v001, v010)*orientation <=0. ) return true;

  return false;
}

int HyperbolicMapping::
drawReferenceSurface(GenericGraphicsInterface & gi, 
                     GraphicsParameters & referenceSurfaceParameters,
                     const real & surfaceOffset,
                     const aString & referenceSurfaceColour,
                     const aString & edgeCurveColour )
// ==============================================================================================
// /Description:
//    Auxillary routine for plotting the reference surface.
// ==============================================================================================
{
  referenceSurfaceHasChanged=false;
  if( !plotHyperbolicSurface )
    referenceSurfaceParameters.set(GI_TOP_LABEL,getName(mappingName)+" (reference surface)");
  else
    referenceSurfaceParameters.set(GI_TOP_LABEL,"" );
	
//        referenceSurfaceParameters.set(GI_SURFACE_OFFSET, initialOffset);  
  referenceSurfaceParameters.set(GI_SURFACE_OFFSET,surfaceOffset);  
  // referenceSurfaceParameters.get(GI_SURFACE_OFFSET, offSet);  
  // printf("hypeUpdate: offset for reference surface: %f\n", offSet);
	

  referenceSurfaceParameters.set(GI_MAPPING_COLOUR,referenceSurfaceColour);
  // in 2D draw thicker lines:
  real oldCurveLineWidth;
  referenceSurfaceParameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,2.);

  bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
  if( // useTriangulation &&    // always plot edge curves from CompositeTopology of they are there
      isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL )
  {
    // use the triangulation that sits with the reference surface

    CompositeSurface & cs = (CompositeSurface&)(*surface);
    CompositeTopology & compositeTopology = *cs.getCompositeTopology();
    UnstructuredMapping *uns=compositeTopology.getTriangulation();
    assert( uns!=NULL );
	  

    // *wdh* Always plot original surface in case we need to pick points

    // turn off patch edges since we will plot edges separately
    referenceSurfaceParameters.set(GI_PLOT_MAPPING_EDGES,false);
    
    PlotIt::plot(gi,*surface,referenceSurfaceParameters);   

    if( plotTriangulation )
    {
      // ** plot the unstructured surface ****
      // only plot lines on the triangulation
      GraphicsParameters triangulationParameters;
      triangulationParameters.set(GI_PLOT_UNS_FACES,false);
      triangulationParameters.set(GI_PLOT_UNS_EDGES,true);
      triangulationParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      PlotIt::plot(gi,*uns,triangulationParameters);
    }
    

    // **** plot edge curves, these can be picked as starting curves ****

    referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,2.);
    referenceSurfaceParameters.set(GI_MAPPING_COLOUR,edgeCurveColour);
    referenceSurfaceParameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);
    referenceSurfaceParameters.set(GI_POINT_SIZE,(real)3.);
    referenceSurfaceParameters.set(GI_PLOT_END_POINTS_ON_CURVES,true);

    int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
    for( int e=0; e<numberOfEdgeCurves; e++ )
    {
      // printf(" edge=%i status=%i\n",e,int(compositeTopology.getEdgeCurveStatus(e)));
      // if( int(compositeTopology.getEdgeCurveStatus(e)) <= 2 )
      Mapping & edge = compositeTopology.getEdgeCurve(e);
	    
	    
      if( compositeTopology.getEdgeCurveStatus(e)==CompositeTopology::edgeCurveIsMerged )
	PlotIt::plot(gi,compositeTopology.getEdgeCurve(e),referenceSurfaceParameters);
    }
	  


  }
  else
    PlotIt::plot(gi,*surface,referenceSurfaceParameters);  

  //        referenceSurfaceParameters.set(GI_SURFACE_OFFSET,initialOffset);  // reset
  referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);

  return 0;
}

int HyperbolicMapping::
drawReferenceSurfaceEdges(GenericGraphicsInterface & gi,
                          GraphicsParameters & parameters,
                          const aString *boundaryColour)
// ==============================================================================================
// /Description:
//    Auxillary routine for plotting edges of the referenceSurface so we can tell 
//     which boundaries are left/right/bottom/top
// ==============================================================================================
{
      
  if( !surfaceGrid && surface!=NULL )
  {
    // plot the edges of the referenceSurface so we can tell which boundaries are left/right/bottom/top

    real oldCurveLineWidth;
    parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GraphicsParameters::curveLineWidth,5.);

    if( domainDimension==3 )
    {
      for( int axis=0; axis<domainDimension-1; axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  ReductionMapping edge(*surface,axis,(real)side);

	  parameters.set(GI_MAPPING_COLOUR,boundaryColour[side+2*axis]);
	  PlotIt::plot(gi,edge,parameters);
	}
      }
    }
    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  }
  return 0;
}

int HyperbolicMapping::
drawHyperbolicGrid(GenericGraphicsInterface & gi, 
                   GraphicsParameters & parameters, 
		   bool plotNonPhysicalBoundaries,
                   const real & initialOffset, 
                   const aString & hyperbolicMappingColour )
// ==============================================================================================
// /Description:
//    Auxillary routine to plot the hyperbolic grid.
// ==============================================================================================
{

  if( plotHyperbolicSurface && dpm!=NULL )
  {
    // plot hyperbolic surface

    parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE );
    if( plotNonPhysicalBoundaries )
      parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,TRUE);
    parameters.set(GI_MAPPING_COLOUR,hyperbolicMappingColour);
    parameters.set(GI_PLOT_BLOCK_BOUNDARIES,false);
    parameters.set(GI_PLOT_MAPPING_EDGES,false);
	
    aString name;
    parameters.set(GI_TOP_LABEL,sPrintF(name,"%s: vs=%i eps=%4.3f imp=%3.2f",
					(const char*)getName(mappingName),
					numberOfVolumeSmoothingIterations,
					uniformDissipationCoefficient,
					implicitCoefficient));
    parameters.set(GI_TOP_LABEL_SUB_1,sPrintF("cs=%3.2f uw=%3.2f eq=%3.2f",
					      curvatureSpeedCoefficient,
					      upwindDissipationCoefficient,equidistributionWeight));

    //        parameters.set(GI_SURFACE_OFFSET,(real)3.);  // offset the surface so we can see it better
    parameters.set(GI_SURFACE_OFFSET,(real).5);  // offset the surface so we can see it better
    //        parameters.set(GI_SURFACE_OFFSET,initialOffset);  // reset
    // parameters.get(GI_SURFACE_OFFSET, offSet);  
    // printf("offset for hyperbolic grid: %f\n", offSet);

    if( plotGhostPoints )
    {
      parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,numberOfGhostLinesToPlot); 
    }
    else
      parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,0); 

    printf("drawHyperbolicGrid: plot grid\n");
    
    PlotIt::plot(gi,*this,parameters);  

    parameters.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,0);   // make this an option


    if( plotNegativeCells )
    {
      // Draw cells with a negative jacobian in red
      // We draw the edges of the cell in redn

      realArray & x = xHyper;
    
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      ::getIndex(indexRange,I1,I2,I3); 

      int axis;
      for( axis=0; axis<domainDimension; axis++ )
	Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);  // only compute at cell centres.

     
      int maxNumberOfNegativeCells=1000;  // This will be increased below if needed.
      realArray line(maxNumberOfNegativeCells,3,2);

      const bool growBothDirections = fabs(growthOption) > 1;
      const int growthDirection = (growthOption==1 || growBothDirections) ? 0 : 1;
      const real orientation= 1.-2*growthDirection;  

      int numberOfBadCells=0;
      if( domainDimension==2 && rangeDimension==2 )
      {
	
      }
      else if( false &&   // **** finish this ****
               domainDimension==2 && rangeDimension==3 )
      {
        Iv[axis3]=Range(Iv[axis3].getBase(),Iv[axis3].getBound()-1);  // only compute at cell centres.

        Range Rx=rangeDimension;
	
	realArray a(I1,I2,I3,Rx),b(I1,I2,I3,Rx);
	a=x(I1+1,I2  ,I3  ,Rx)-x(I1,I2,I3,Rx);
	realArray cc(I1,I2,I3,Rx);
	b=x(I1  ,I2  ,I3+1,Rx)-x(I1,I2,I3,Rx);

	cc(I1,I2,I3,0)=a(I1,I2,I3,1)*b(I1,I2,I3,2)-a(I1,I2,I3,2)*b(I1,I2,I3,1);
	cc(I1,I2,I3,1)=a(I1,I2,I3,2)*b(I1,I2,I3,0)-a(I1,I2,I3,0)*b(I1,I2,I3,2);
	cc(I1,I2,I3,2)=a(I1,I2,I3,0)*b(I1,I2,I3,1)-a(I1,I2,I3,1)*b(I1,I2,I3,0);
    
	a=x(I1+1,I2  ,I3+1,Rx)-x(I1,I2,I3+1,Rx);

	realArray vol(I1,I2,I3);
	vol=(( a(I1,I2,I3,1)*b(I1,I2,I3,2)-a(I1,I2,I3,2)*b(I1,I2,I3,1) )*cc(I1,I2,I3,0)+
	     ( a(I1,I2,I3,2)*b(I1,I2,I3,0)-a(I1,I2,I3,0)*b(I1,I2,I3,2) )*cc(I1,I2,I3,1)+
	     ( a(I1,I2,I3,0)*b(I1,I2,I3,1)-a(I1,I2,I3,1)*b(I1,I2,I3,0) )*cc(I1,I2,I3,2));

	const int m1a[4] = {0,0,0,1}; //
	const int m2a[4] = {0,1,0,0}; //
	const int m1b[4] = {1,1,0,1}; //
	const int m2b[4] = {0,1,1,1}; //

        printf("Checking for bad quads on the surface: [%i,%i][%i,%i][%i,%i] orientation=%8.2e\n",I1.getBase(),I1.getBound(),
	       I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),orientation);
	
        real v[2][2][3];  // hold the 4 vertices of the quad

	int j=0;
        int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          // printf(" i=(%i,%i,%i)",i1,i2,i3);
	  if( vol(i1,i2,i3)<= 0. )
	  {
	    for( int axis=0; axis<3; axis++ )
	    {
	      v[0][0][axis]=x(i1  ,i2  ,i3  ,axis);
	      v[1][0][axis]=x(i1+1,i2  ,i3  ,axis);
	      v[0][1][axis]=x(i1  ,i2  ,i3+1,axis);
	      v[1][1][axis]=x(i1+1,i2  ,i3+1,axis);
	    }
	    // make a list of line-segments for the 4 edges of this quad
            for( int m=0; m<12; m++ )
	    {
	      line(j,0,0)=v[m1a[m]][m2a[m]][0];
	      line(j,1,0)=v[m1a[m]][m2a[m]][1];
	      line(j,2,0)=v[m1a[m]][m2a[m]][2];
	      line(j,0,1)=v[m1b[m]][m2b[m]][0];
	      line(j,1,1)=v[m1b[m]][m2b[m]][1];
	      line(j,2,1)=v[m1b[m]][m2b[m]][2];
	      j++;
	    }
            if( j>= maxNumberOfNegativeCells-12 )
	    {
              maxNumberOfNegativeCells=int(maxNumberOfNegativeCells*1.5);
	      line.resize(maxNumberOfNegativeCells,3,2);
	    }
	  }
	}
        numberOfBadCells=j;
      }
      else if( domainDimension==3 && rangeDimension==3 )
      {
	
	const int m1a[12] = {0,0,0,1, 0,0,0,1, 0,1,0,1}; //
	const int m2a[12] = {0,1,0,0, 0,1,0,0, 0,0,1,1}; //
	const int m3a[12] = {0,0,0,0, 1,1,1,1, 0,0,0,0}; //

	const int m1b[12] = {1,1,0,1, 1,1,0,1, 0,1,0,1}; //
	const int m2b[12] = {0,1,1,1, 0,1,1,1, 0,0,1,1}; //
	const int m3b[12] = {0,0,0,0, 1,1,1,1, 1,1,1,1}; //

        printf("Checking for bad cells: [%i,%i][%i,%i][%i,%i] orientation=%8.2e\n",I1.getBase(),I1.getBound(),
	       I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),orientation);
	
        real v[2][2][2][3];  // hold the 8 vertices of the hex
	
        int j=0;
        int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          // printf(" i=(%i,%i,%i)",i1,i2,i3);
	  
	  for( int axis=0; axis<3; axis++ )
	  {
	    v[0][0][0][axis]=x(i1  ,i2  ,i3  ,axis);
	    v[1][0][0][axis]=x(i1+1,i2  ,i3  ,axis);
	    v[0][1][0][axis]=x(i1  ,i2+1,i3  ,axis);
	    v[1][1][0][axis]=x(i1+1,i2+1,i3  ,axis);
	    v[0][0][1][axis]=x(i1  ,i2  ,i3+1,axis);
	    v[1][0][1][axis]=x(i1+1,i2  ,i3+1,axis);
	    v[0][1][1][axis]=x(i1  ,i2+1,i3+1,axis);
	    v[1][1][1][axis]=x(i1+1,i2+1,i3+1,axis);
	  }
	  if( hexIsBad(v[0][0][0],v[1][0][0],v[0][1][0],v[1][1][0],
                       v[0][0][1],v[1][0][1],v[0][1][1],v[1][1][1], orientation) )
	  {
	    // make a list of line-segments for the 12 edges of this hex
            

            for( int m=0; m<12; m++ )
	    {
	      line(j,0,0)=v[m1a[m]][m2a[m]][m3a[m]][0];
	      line(j,1,0)=v[m1a[m]][m2a[m]][m3a[m]][1];
	      line(j,2,0)=v[m1a[m]][m2a[m]][m3a[m]][2];
	      line(j,0,1)=v[m1b[m]][m2b[m]][m3b[m]][0];
	      line(j,1,1)=v[m1b[m]][m2b[m]][m3b[m]][1];
	      line(j,2,1)=v[m1b[m]][m2b[m]][m3b[m]][2];
	      j++;
	    }
            if( j>= maxNumberOfNegativeCells-12 )
	    {
              maxNumberOfNegativeCells=int(maxNumberOfNegativeCells*1.5);
	      line.resize(maxNumberOfNegativeCells,3,2);
	    }
	  }
	}
        numberOfBadCells=j;
      }
      if( numberOfBadCells>0 )
      {
	printf("*** Plotting %i cells with negative volumes\n",numberOfBadCells/12);
	Range R=numberOfBadCells;
	Range all;
      
	// add line colour, line width
	parameters.set(GI_LINE_COLOUR,"red");
	real oldCurveLineWidth;
	parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	parameters.set(GraphicsParameters::curveLineWidth,4.);

	gi.plotLines(line(R,all,all),parameters);

	parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
      }
      
    }

    parameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,FALSE);
    parameters.set(GI_SURFACE_OFFSET,initialOffset);  // reset
  }

  return 0;
}



int HyperbolicMapping::
drawBoundariesAndCurves(GenericGraphicsInterface & gi, 
			GraphicsParameters & parameters, 
			GraphicsParameters & referenceSurfaceParameters, 
                        const real & surfaceOffset, 
                        const real & initialOffset,
			const aString & boundaryConditionMappingColour,
                        const aString & referenceSurfaceColour,
                        const aString & edgeCurveColour,
                        const aString & buildCurveColour,
                        const aString *boundaryColour )
// ==============================================================================================
// /Description:
//    Auxillary routine for plotting various curves, edges and boundaries.
// ==============================================================================================
{

  // plot any mappings used for boundary conditions
  if( plotBoundaryConditionMappings && !surfaceGrid )
  {
    parameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,FALSE);
    parameters.set(GI_SURFACE_OFFSET,surfaceOffset);  // offset the surface so we can see it better
    // parameters.get(GI_SURFACE_OFFSET, offSet);  
    // printf("offset for boundary condition mappings: %f\n", offSet);

    parameters.set(GI_MAPPING_COLOUR,boundaryConditionMappingColour);
    for( int axis=0; axis<domainDimension-1; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	if( boundaryCondition(side,axis)==matchToMapping  && boundaryConditionMapping[side][axis]!=NULL  )
	{
	  PlotIt::plot(gi,*boundaryConditionMapping[side][axis],parameters);
	}
      }
    }
    parameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,TRUE);
    //        parameters.set(GI_SURFACE_OFFSET,(real)3.);  // reset
    parameters.set(GI_SURFACE_OFFSET, initialOffset);  // reset
  }
  if( surfaceGrid  && startCurve!=NULL && !(plotHyperbolicSurface && dpm!=NULL) )
  { 
    // plot the initial curve
    parameters.set(GI_MAPPING_COLOUR,"green");
    real oldCurveLineWidth;
    parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GraphicsParameters::curveLineWidth,3.);

    // parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,plotGridPointsOnStartCurve);  // no longer since arrows show this
    // parameters.set(GI_POINT_SIZE,(real)5.);
    
    PlotIt::plot(gi,*startCurve,parameters);  
    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    // parameters.set(GI_USE_PLOT_BOUNDS,TRUE); 
  }
  if( plotDirectionArrowsOnInitialCurve && surfaceGrid && startCurve!=NULL  )
  {
    // plot arrows in the marching direction
    // printf("plot direction arrows on start curve\n");
    parameters.set(GI_MAPPING_COLOUR,"green");
    plotDirectionArrows(gi, parameters);
  }

  real oldCurveLineWidth;
  parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  if( surfaceGrid && numberOfBoundaryCurves>0 && plotBoundaryCurves )
  {
    parameters.set(GraphicsParameters::curveLineWidth,4.);
    // parameters.set(GI_MAPPING_COLOUR,"green");

    parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);
    int col=0;
    for( int b=0; b<numberOfBoundaryCurves; b++ )
    {
      if( (growthOption>=1 && (  // forward
          (boundaryCondition(0,0)==matchToABoundaryCurve && boundaryConditionMapping[0][0]==boundaryCurves[b]) || 
	  (boundaryCondition(1,0)==matchToABoundaryCurve && boundaryConditionMapping[1][0]==boundaryCurves[b]) )) ||
          (growthOption!=1 && (  // backward
	  (boundaryCondition(0,1)==matchToABoundaryCurve && boundaryConditionMapping[0][1]==boundaryCurves[b]) ||
	  (boundaryCondition(1,1)==matchToABoundaryCurve && boundaryConditionMapping[1][1]==boundaryCurves[b]) )) )
      {
        // this is a boundary curve that is used as a BC mapping.
	parameters.set(GI_MAPPING_COLOUR,boundaryConditionMappingColour);
	parameters.set(GraphicsParameters::curveLineWidth,5.);
      }
      else
      {
	aString colour=((GL_GraphicsInterface &)gi).colourNames[col % (GL_GraphicsInterface::numberOfColourNames-1)];
	while( colour==referenceSurfaceColour || colour==edgeCurveColour ||
               colour==boundaryConditionMappingColour || colour==buildCurveColour ) // skip these colours
	{
	  col++;
	  colour=((GL_GraphicsInterface &)gi).colourNames[col % (GL_GraphicsInterface::numberOfColourNames-1)];
	}
	parameters.set(GI_MAPPING_COLOUR,colour);
      }
      // printf("draw boundary curve b=%i\n",b);

      PlotIt::plot(gi,*boundaryCurves[b],parameters);
      col++;
    }

    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    // parameters.set(GI_MAPPING_COLOUR,"red");
    parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	
  }

    
  if( surface!=NULL && plotBoundaryCurves )
  {
    bool isCompositeSurface = surface->getClassName()=="CompositeSurface";
    if( surfaceGrid && isCompositeSurface && ((CompositeSurface*)surface)->getCompositeTopology()!=NULL )
    {
      // plot any edge curves that are used for boundary conditions.
      CompositeSurface & cs = (CompositeSurface&)(*surface);
      CompositeTopology & compositeTopology = *cs.getCompositeTopology();
      referenceSurfaceParameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);

      int numberOfEdgeCurves=compositeTopology.getNumberOfEdgeCurves();
      for( int e=0; e<numberOfEdgeCurves; e++ )
      {
	Mapping & edge = compositeTopology.getEdgeCurve(e);
	if( (boundaryCondition(Start,0)==matchToABoundaryCurve && boundaryConditionMapping[Start][0]==&edge) || 
	    (boundaryCondition(Start,1)==matchToABoundaryCurve && boundaryConditionMapping[Start][1]==&edge) ||
	    (boundaryCondition(End  ,0)==matchToABoundaryCurve && boundaryConditionMapping[End  ][0]==&edge) ||
	    (boundaryCondition(End  ,1)==matchToABoundaryCurve && boundaryConditionMapping[End  ][1]==&edge) )
	{
	  referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,4.);
	  referenceSurfaceParameters.set(GI_MAPPING_COLOUR,boundaryConditionMappingColour);

	  PlotIt::plot(gi,compositeTopology.getEdgeCurve(e),referenceSurfaceParameters);
	}
	parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);

      }
    }

    // Plot any curves used for interior matching curves
    const int numberOfMatchingCurves=matchingCurves.size();
    if( numberOfMatchingCurves>0 )
    {
      aString matchingCurveColour="PALEGREEN"; 
      
      referenceSurfaceParameters.set(GraphicsParameters::curveLineWidth,5.);
      referenceSurfaceParameters.set(GI_MAPPING_COLOUR,matchingCurveColour);

      for( int i=0; i<numberOfMatchingCurves; i++ )
      {
        MatchingCurve & match = matchingCurves[i];
        if( match.curve!=NULL )
	  PlotIt::plot(gi,*match.curve,referenceSurfaceParameters);
      }
      parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    }
  }

  
  if( ((surfaceGrid && surface!=NULL && plotHyperbolicSurface && !plotDirectionArrowsOnInitialCurve) ||
        (domainDimension==2 && rangeDimension==2 && !plotDirectionArrowsOnInitialCurve ) ) && 
      dpm!=NULL )
  {
    // On a surface grid plot the boundary lines so we know where the ghost points are
    // On a 2D grid plot the boundaries to show where the ghost lines are
    real oldCurveLineWidth;
    parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
    parameters.set(GraphicsParameters::curveLineWidth,5.);

    for( int axis=0; axis<domainDimension; axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
         // if the grid fails with a negative volume there may be only on grid line
        if( dpm->getGridDimensions((axis+1)%domainDimension) > 1 ) 
	{
	  ReductionMapping edge(*dpm,axis,(real)side);

	  parameters.set(GI_MAPPING_COLOUR,boundaryColour[side+2*axis]);
	  PlotIt::plot(gi,edge,parameters);
	}
      }
    }
    parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
  }
  return 0;
}


void HyperbolicMapping::
plotDirectionArrows(GenericGraphicsInterface & gi, GraphicsParameters & params)
// =========================================================================
// Plot arrows on the initial curve in the direction in which the hyperbolic
//  surface grid will be grown
// =========================================================================
{

// ***wdh030219   if( !gi.graphicsIsOn() )
// ***wdh030219    return;

  if( startCurve==NULL || surface==NULL )
    return;


  Range xAxes(0,rangeDimension-1);

//    // compute the arrow vector
//    int numberOfArrows=max(5,numberOfPointsOnStartCurve);
//    realArray arrow(numberOfArrows,3,2);
//    realArray r(numberOfArrows,1), x(numberOfArrows,3), xr(numberOfArrows,3,2);
  
//    real dr=1./max(1,numberOfArrows-1);
//    r.seqAdd(0,dr);  // r =0, dr, 2*dr, ...

//    startCurve->map(r,x,xr);

//    // project curve and get normals
//    x.reshape(numberOfArrows,1,1,3);
//    xr.reshape(numberOfArrows,1,1,3,2);

  
  int numberOfArrows=numberOfPointsOnStartCurve;

  // We need to assign the indexRange and dimension arrays since they have not been set yet
  indexRange.redim(2,3);
  indexRange=0;
  indexRange(1,0)=numberOfArrows-1;
  dimension.redim(2,3);
  dimension=indexRange;
  gridIndexRange.redim(2,3);
  gridIndexRange=indexRange;
  
  Index D1,D2,D3;
  ::getIndex(dimension,D1,D2,D3);

  realArray x(D1,1,1,rangeDimension),xr(D1,1,1,rangeDimension,2);
  realArray arrow(numberOfArrows,3,2);
  xr=1.;  // to avoid UMR

  Range all;
  evaluateStartCurve( x );

  bool setBoundaryConditions=FALSE;

  const int marchingDirection=growthOption>0 ? 1 : -1; 
  bool initialStep=true;

  project( x,marchingDirection,xr,setBoundaryConditions,initialStep );  // this will also fill in xr

  x.reshape(numberOfArrows,3);
  xr.reshape(numberOfArrows,3,2);


  Index I(0,numberOfArrows);
  // real lengthOfArrow=-sign(growthOption)*.025;
  const real arrowLengthScalingFactor=.05;
  real lengthOfArrow=-sign(growthOption)*arrowLengthScalingFactor;
  // printf("plotDirectionArrows: lengthOfArrow=%e \n",lengthOfArrow);
    
  // ::display(xr,"plotArrows: xr","%8.2e ");

  // direction = normal X tangent  
  // starting points
  realArray normal(I,3);
  normal(I,0)=(xr(I,1,1)*xr(I,2,0)-xr(I,2,1)*xr(I,1,0));
  normal(I,1)=(xr(I,2,1)*xr(I,0,0)-xr(I,0,1)*xr(I,2,0));
  normal(I,2)=(xr(I,0,1)*xr(I,1,0)-xr(I,1,1)*xr(I,0,0));

  // scale by the average length
  realArray norm; 
  norm= SQRT( SQR(normal(I,0))+SQR(normal(I,1))+SQR(normal(I,2)) );
  RealArray xBound;
  xBound = gi.getGlobalBound();
  real scale = max(xBound(End,xAxes)-xBound(Start,xAxes));

  real startCurveLength=startCurve->getArcLength();
  if( startCurveLength<0. ) // this means it hasn't been computed yet
  {
    startCurveLength=0.;
    for( int i=0; i<numberOfArrows-1; i++ )
      startCurveLength+=sqrt( SQR(x(i+1,0)-x(i,0))+SQR(x(i+1,1)-x(i,1))+SQR(x(i+1,2)-x(i,2)) );
    startCurve->setArcLength(startCurveLength);
  }
//  printf(" Plot arrows: model scale=%8.2e, startCurveLength=%8.2e\n",scale,startCurveLength);
  
  real normalScale=min(scale,startCurveLength*.25/arrowLengthScalingFactor); // sum(norm)/numberOfArrows;
  
//   printf(" > arrows: normal r=0: normal=(%8.2e,%8.2e,%8.2e)\n",
//              normal(0,0)/norm(0),normal(0,1)/norm(0),normal(0,2)/norm(0));
//   printf(" > arrows: normal r=1: normal=(%8.2e,%8.2e,%8.2e)\n",
//              normal(numberOfArrows-1,0)/norm(numberOfArrows-1),normal(numberOfArrows-1,1)/norm(numberOfArrows-1),
//              normal(numberOfArrows-1,2)/norm(numberOfArrows-1));

  norm= lengthOfArrow*normalScale/max(REAL_MIN*100.,norm);
  for( int axis=0; axis<3; axis++ )
    normal(I,axis)*=norm;
  
  if( abs(growthOption)==1 )
    arrow(I,xAxes,0) = x(I,xAxes);
  else
    arrow(I,xAxes,0) = x(I,xAxes) - normal(I,xAxes);  // arrows go in both directions
  // ending points
  arrow(I,xAxes,1) = x(I,xAxes) + normal(I,xAxes);


  if( !gi.isGraphicsWindowOpen() )
   return;

//    int list= gi.generateNewDisplayList(); // get a new display list to use
//    assert(list!=0);

  GraphicsParameters gp;
  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
   // use twice as thick lines as everywhere else
  real lineWidth;
  gp.set(GraphicsParameters::curveLineWidth, params.get(GraphicsParameters::lineWidth,lineWidth)*2.);
  aString mappingColour;
  // params.get(GI_MAPPING_COLOUR,mappingColour); 
  // gp.set(GI_POINT_COLOUR, mappingColour);
  gp.set(GI_POINT_COLOUR, gi.getColour(GenericGraphicsInterface::textColour));
  
  gi.plotLines(arrow, gp);

  // draw coloured squares to indicate the direction of increasing r

  Range Ig(0,int( numberOfArrows/2.+.5)-1);
  Range Ir(int( numberOfArrows/2.+.5),numberOfArrows-1);
  Range C(0,2);
  
  gp.set(GI_POINT_SIZE, (real) 9.0);
  gp.set(GI_POINT_COLOUR, "green");
  gi.plotPoints(x(Ig,C), gp);
  gp.set(GI_POINT_COLOUR, "red");
  gi.plotPoints(x(Ir,C),   gp);
  
  gi.redraw( gi.getCurrentWindow() );
}



int HyperbolicMapping::
plotCellQuality(GenericGraphicsInterface & gi, 
		GraphicsParameters & parameters)
// =============================================================================
// 
// =============================================================================
{

  // find any cells will negative volumes.
  Index I1,I2,I3;
  ::getIndex(indexRange,I1,I2,I3);  
  I3=Range(I3.getBase(),I3.getBound()-1); // include a ghost cell on left edge

  I1=Range(I1.getBase()-1,I1.getBound()); // include a ghost cell on left edge
  if( domainDimension==3 )
    I2=Range(I2.getBase()-1,I2.getBound()); // include a ghost cell on bottom.
    
  realArray & x = xHyper;

  real dSign = growthOption==1 ? 1. : -1.;

  Range Rx(0,rangeDimension-1);
  realArray vol(I1,I2,I3);

  if( domainDimension==3 && rangeDimension==3 )  // **** finish this ********************
  {
    realArray a(I1,I2,I3,Rx),b(I1,I2,I3,Rx);
    a=x(I1+1,I2  ,I3  ,Rx)-x(I1,I2,I3,Rx);
    b=x(I1  ,I2+1,I3  ,Rx)-x(I1,I2,I3,Rx);
    realArray cc(I1,I2,I3,Rx);
    cc=x(I1  ,I2  ,I3+1,Rx)-x(I1,I2,I3,Rx);
    vol=(a(I1,I2,I3,0)*(b(I1,I2,I3,1)*cc(I1,I2,I3,2)-b(I1,I2,I3,2)*cc(I1,I2,I3,1))+
	 a(I1,I2,I3,1)*(b(I1,I2,I3,2)*cc(I1,I2,I3,0)-b(I1,I2,I3,0)*cc(I1,I2,I3,2))+
	 a(I1,I2,I3,2)*(b(I1,I2,I3,0)*cc(I1,I2,I3,1)-b(I1,I2,I3,1)*cc(I1,I2,I3,0)))*dSign;
    a=x(I1+1,I2  ,I3+1,Rx)-x(I1,I2,I3+1,Rx);
    b=x(I1  ,I2+1,I3+1,Rx)-x(I1,I2,I3+1,Rx);
    vol=min(vol,
	    (a(I1,I2,I3,0)*(b(I1,I2,I3,1)*cc(I1,I2,I3,2)-b(I1,I2,I3,2)*cc(I1,I2,I3,1))+
	     a(I1,I2,I3,1)*(b(I1,I2,I3,2)*cc(I1,I2,I3,0)-b(I1,I2,I3,0)*cc(I1,I2,I3,2))+
	     a(I1,I2,I3,2)*(b(I1,I2,I3,0)*cc(I1,I2,I3,1)-b(I1,I2,I3,1)*cc(I1,I2,I3,0)))*dSign );
    
  }
  else if( domainDimension==2 && rangeDimension==2 )
  {
    realArray a0(I1,I2,I3,Rx),a1(I1,I2,I3,Rx),b(I1,I2,I3,Rx);
    a0=x(I1+1,I2  ,I3  ,Rx)-x(I1,I2,I3+1,Rx);
    a1=x(I1+1,I2  ,I3+1,Rx)-x(I1,I2,I3+1,Rx);
    b =x(I1  ,I2  ,I3+1,Rx)-x(I1,I2,I3,Rx);
    vol=min( (a0(I1,I2,I3,1)*b(I1,I2,I3,0)-a0(I1,I2,I3,0)*b(I1,I2,I3,1))*dSign,
             (a1(I1,I2,I3,1)*b(I1,I2,I3,0)-a1(I1,I2,I3,0)*b(I1,I2,I3,1))*dSign );

  }
  else if( domainDimension==2 && rangeDimension==3 ) 
  {
    // (a0 X b) . ( a1 X b )
    realArray a(I1,I2,I3,Rx),b(I1,I2,I3,Rx);
    a=x(I1+1,I2  ,I3  ,Rx)-x(I1,I2,I3,Rx);
    realArray cc(I1,I2,I3,Rx);
    b=x(I1  ,I2  ,I3+1,Rx)-x(I1,I2,I3,Rx);

    cc(I1,I2,I3,0)=a(I1,I2,I3,1)*b(I1,I2,I3,2)-a(I1,I2,I3,2)*b(I1,I2,I3,1);
    cc(I1,I2,I3,1)=a(I1,I2,I3,2)*b(I1,I2,I3,0)-a(I1,I2,I3,0)*b(I1,I2,I3,2);
    cc(I1,I2,I3,2)=a(I1,I2,I3,0)*b(I1,I2,I3,1)-a(I1,I2,I3,1)*b(I1,I2,I3,0);
    
    a=x(I1+1,I2  ,I3+1,Rx)-x(I1,I2,I3+1,Rx);

    vol=(( a(I1,I2,I3,1)*b(I1,I2,I3,2)-a(I1,I2,I3,2)*b(I1,I2,I3,1) )*cc(I1,I2,I3,0)+
	 ( a(I1,I2,I3,2)*b(I1,I2,I3,0)-a(I1,I2,I3,0)*b(I1,I2,I3,2) )*cc(I1,I2,I3,1)+
	 ( a(I1,I2,I3,0)*b(I1,I2,I3,1)-a(I1,I2,I3,1)*b(I1,I2,I3,0) )*cc(I1,I2,I3,2));
  }
  else
  {
    {throw "error";}
  }
  
  
  int maximumNumberToPlot=100;
  realArray point(maximumNumberToPlot,rangeDimension);
  
  int numberOfCells=0,axis,i1,i2,i3;
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++)
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++)
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++)
  {
    if( vol(i1,i2,i3)<0. )
    {
      for( axis=0; axis<rangeDimension; axis++ )
        point(numberOfCells,axis)=x(i1,i2,i3,axis);
      numberOfCells++;
      if( numberOfCells>=maximumNumberToPlot )
      {
	maximumNumberToPlot*=2;
	point.resize(maximumNumberToPlot,rangeDimension);
      }
    }
  }

  printf("Number of negative volumes=%i, min(vol)=%9.2e, max(vol)=%9.2e \n",numberOfCells,min(vol),max(vol));
  
  if( numberOfCells>0 )
  {
    Range I(0,numberOfCells-1);

    parameters.set(GI_POINT_SIZE,(real)5.);      // point size in pixels
    parameters.set(GI_POINT_COLOUR,"yellow"); 

    gi.plotPoints(point(I,Rx),parameters);

    parameters.set(GI_POINT_SIZE,(real)3.);      // reset

  }
  return 0;
}

