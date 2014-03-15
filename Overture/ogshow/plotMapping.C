#include "GL_GraphicsInterface.h" // GL include files needed for glVertex, etc.
#include "CompositeSurface.h"
#include "UnstructuredMapping.h"
#include "TrimmedMapping.h"
#include "arrayGetIndex.h"
#include "NurbsMapping.h"
#include "PlotIt.h"
#include "xColours.h"
#include "ParallelUtility.h"

// AP: The new routines plotTrimmedMapping and plotNurbsMapping are still pretty unstable,
// so we'll use the old style plotting for now.
#define USE_PLOTSTRUCTURED
//  #ifdef OLD_NURBS
//  #define USE_PLOTSTRUCTURED
//  #else
//  #undef USE_PLOTSTRUCTURED
//  #endif

//  #ifdef OV_USE_MESA
//  #define USE_PLOTSTRUCTURED
//  #endif
 
extern "C" void OV_gluCallback ( GLenum err );

#define ForBoundary(side,axis)   for( axis=0; axis<domainDimension; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define FOR_2(i1,i2,I1,I2) \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

//\begin{>>PlotItInclude.tex}{\subsection{Plot a Mapping}} 
void PlotIt:: 
plot(GenericGraphicsInterface &gi, Mapping& map, 
     GraphicsParameters & parameters /* = nullGraphicsParameters */,
     int dList /* = 0 */, bool lit /* = 0 */)
//----------------------------------------------------------------------
// /Description:
//   Plot a mapping. Plot curves, surfaces and volumes in 1,2 or 3
//     space dimensions. Plot grid lines. In 3D plot shaded surfaces
//     and grid lines.
// 
// /map (input): Mapping to plot.
// /parameters (input/output): supply optional parameters to change
//    plotting characteristics.

// /Return Values: none.
//
//  /Author: WDH \& AP
//\end{PlotItInclude.tex} 
//----------------------------------------------------------------------
{
  if( !gi.graphicsIsOn() ) return;

  const int processorForGraphics = gi.getProcessorForGraphics();
  
  if( map.getClassName()=="CompositeSurface" )
  {
    plotCompositeSurface(gi, (CompositeSurface &)map, parameters );
  }
  else if( map.getClassName()=="UnstructuredMapping" )
  {
    plotUM( gi, (UnstructuredMapping &)map, parameters, dList, lit );
  }
  else if ( map.getClassName()=="TrimmedMapping" )
  {
    TrimmedMapping & trim = (TrimmedMapping &)map;
    if( true && trim.trimmingIsValid() ) // AP: false avoids the triangulation until it gets fixed
    {
      UnstructuredMapping *um_ = NULL;
      um_ = & trim.getTriangulation();

      // plot the TrimmedMapping with it's related UnstructuredMapping
      // We need to adjust the globalID so we can still pick the original Mapping.

      //UnstructuredMapping & um = trim.getTriangulation();
      UnstructuredMapping & um = *um_;
      int id = um.getGlobalID();
      um.setGlobalID(map.getGlobalID());
      plotUM(gi, um, parameters, dList, lit ); 
      um.setGlobalID(id);  // reset
    }
    else // plot the untrimmed surface
    {
      if( Mapping::debug & 1 )
         printf("Plotting the untrimmed surface of an invalid trimmed surface! name=%s\n",
	     (const char*)trim.getName(Mapping::mappingName));
      // should also plot the incomplete trimcurves
      Mapping &untrim = *trim.surface;
      int id = untrim.getGlobalID();
      untrim.setGlobalID(map.getGlobalID());
      plot(gi, untrim, parameters, dList, lit );
      untrim.setGlobalID(id);  // reset
    }
    
  }
  else if ( map.getClassName()=="NurbsMapping")
  {
    plotNurbsMapping(gi, (NurbsMapping &)map, parameters, dList, lit );
  }
  else
  {
    // only 1 processor actually plots 
//     RealArray *grid=NULL;
//     if( gi.isGraphicsWindowOpen() && map.usesDistributedMap() )
//     { // If the Mapping requires communication to evaluate itself then we must pre-evaluate in parallel
//       realArray & grid = map.getGrid();
//       // make a copy on the processorForGraphics:

//     }
    
    //if( Communication_Manager::localProcessNumber()==processorForGraphics )
    // {

    plotStructured(gi, map, parameters, dList, lit);

      //}
    // Here we broadcast parameters that should be known on all processors:
    broadCast(parameters.objectWasPlotted,processorForGraphics); 

  }
  return;
  
}


void PlotIt::
plotMappingBoundaries(GenericGraphicsInterface &gi,
		      const Mapping & mapping,
                      const RealArray & vertex,
                      const int colourOption, 
                      const real zRaise,
                      GraphicsParameters & parameters )
//--------------------------------------------------------------------------------------
//  Plot Mapping boundaries in 2D
// /colourOption : 0=black, 1= choose from parameters
//---------------------------------------------------------------------------------------
{
  int side,axis;
  int i1,i2,i3;
  Index I1a,I2a,I3a;

  int rangeDimension=mapping.getRangeDimension();
  int domainDimension=mapping.getDomainDimension();

//  Index I1=Range(vertex.getBase(0),vertex.getBound(0)),  // *wdh* 971029
//        I2=Range(vertex.getBase(1),vertex.getBound(1)),
//        I3=Range(vertex.getBase(2),vertex.getBound(2));
  Index I1=Range(0,mapping.getGridDimensions(0)-1);
  Index I2=domainDimension>1 ? Range(0,mapping.getGridDimensions(1)-1) : Range(0,0);
  Index I3=domainDimension>2 ? Range(0,mapping.getGridDimensions(2)-1) : Range(0,0);

  ForBoundary(side,axis)
  {
    glLineWidth(parameters.size(GraphicsParameters::lineWidth)*3.*
		gi.getLineWidthScaleFactor());   // make coloured lines 3 times normal
    if( colourOption==0 || mapping.getBoundaryCondition(side,axis)<0 )
    {
      gi.setColour(GenericGraphicsInterface::textColour); 
      glLineWidth(parameters.size(GraphicsParameters::lineWidth)*2.*
		  gi.getLineWidthScaleFactor());      // boundary line is twice normal
    }
    else if( parameters.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition  || 
             parameters.boundaryColourOption==GraphicsParameters::defaultColour )
      setXColour( gi.getColourName(mapping.getBoundaryCondition(side,axis)) );
    else if( parameters.boundaryColourOption==GraphicsParameters::colourByShare )
      setXColour( gi.getColourName(mapping.getShare(side,axis)) );
    else if( parameters.boundaryColourOption==GraphicsParameters::colourByGrid )
      setXColour(parameters.mappingColour);
    else 
      setXColour(GenericGraphicsInterface::textColour);

    int is1 = axis==axis1 ? 0 : 1;
    int is2 = axis==axis2 ? 0 : 1;
    if( axis==axis1 )
    {
      I1a= side==Start ? Range(I1.getBase(),I1.getBase()) : Range(I1.getBound(),I1.getBound());
      I2a=Range(I2.getBase(),I2.getBound()-is2);
    }
    else
    {
      I1a=Range(I1.getBase(),I1.getBound()-is1);
      I2a= side==Start ? Range(I2.getBase(),I2.getBase()) : Range(I2.getBound(),I2.getBound());
    }

    glBegin(GL_LINES);
    FOR_3(i1,i2,i3,I1a,I2a,I3)
    {
      glVertex3( vertex(i1    ,i2    ,i3,0),vertex(i1    ,i2    ,i3,1),zRaise );
      glVertex3( vertex(i1+is1,i2+is2,i3,0),vertex(i1+is1,i2+is2,i3,1),zRaise );
    }
    glEnd();
  }
  glLineWidth(parameters.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor() );  // reset
}

//=======================================================================================================
// Return the normal at a point
// 
// Input -
//   xr      : array of derivatives 
//   iv(0:2) : get normal at this point
//   axis    : normal is in this "direction"
// Output -
//   normal(0:2) : the normal vector
//======================================================================================================
void 
getNormal(const realArray & x, 
	  const int iv[3],
          const int axis,
	  real normal[3],
          const int & recursion=TRUE )
{
  const int & i1 = iv[0];
  const int & i2 = iv[1];
  const int & i3 = iv[2];

  int ap1=(axis+1) % 3;
  int ap2=(axis+2) % 3;

  int ip[3] = { i1,i2,i3 };
  int im[3] = { i1,i2,i3 };
  
  ip[ap1]= min( ip[ap1]+1,x.getBound(ap1) );
  im[ap1]= max( im[ap1]-1,x.getBase(ap1) );
    
  real xr1[3];
  int dir;
  for( dir=0; dir<3; dir++ )
    xr1[dir]=x(ip[0],ip[1],ip[2],dir)-x(im[0],im[1],im[2],dir);
  
  ip[ap1]=iv[ap1];
  im[ap1]=ip[ap1];
  
  ip[ap2]= min( ip[ap2]+1,x.getBound(ap2) );
  im[ap2]= max( im[ap2]-1,x.getBase(ap2) );
    
  real xr2[3];
  for( dir=0; dir<3; dir++ )
    xr2[dir]=x(ip[0],ip[1],ip[2],dir)-x(im[0],im[1],im[2],dir);

  normal[axis1]=xr1[1]*xr2[2]-xr1[2]*xr2[1];
  normal[axis2]=xr1[2]*xr2[0]-xr1[0]*xr2[2];
  normal[axis3]=xr1[0]*xr2[1]-xr1[1]*xr2[0];

  real l2Norm=sqrt(SQR(normal[axis1])+SQR(normal[axis2])+SQR(normal[axis3]));
  if( l2Norm==0. )
  {
    // cout << "getNormalFromVertex::WARNING: normal has length zero!\n";
    if( recursion )
    {
      // try a nearby point
      int iv2[3] = { iv[0], iv[1], iv[2]  };
      iv2[ap1] = iv[ap1] < x.getBound(ap1) ? iv[ap1]+1 : iv[ap1]-1;
      iv2[ap2] = iv[ap2] < x.getBound(ap2) ? iv[ap2]+1 : iv[ap2]-1;
      getNormal( x,iv2,axis,normal,FALSE);
      // cout << "getNormalFromVertex::ERROR: normal had zero length, after recursion:\n";
      // printf(" i1=%i,i2=%i,i3=%i normal=(%e,%e,%e)\n",i1,i2,i3,normal[0],normal[1],normal[2]);
    }
    else
    {
      // cout << "getNormalFromVertex::ERROR: normal still has ength zero!\n";
      // printf(" i1=%i,i2=%i,i3=%i \n",i1,i2,i3);
      l2Norm=1.;
      normal[0]=0.;   normal[1]=0.;       normal[2]=1.;
    }
  }
  else
  {
    l2Norm=1./l2Norm;
    // normalize the normal to have length 1
    for(dir=axis1; dir<3; dir++ )
      normal[dir]*=l2Norm;
  }
}





#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

void 
getNormal(const real *xp, int xDim0, int xDim1, int xDim2, int *xBase, int *xBound,
	  const int iv[3],
          const int axis, const int ap1, const int ap2,
	  real normal[3],
          const int & recursion=TRUE )
// optimized getNormal
{
  const int & i1 = iv[0];
  const int & i2 = iv[1];
  const int & i3 = iv[2];

//   int ap1=(axis+1) % 3;
//   int ap2=(axis+2) % 3;

  int ip[3] = { i1,i2,i3 };
  int im[3] = { i1,i2,i3 };
  
  ip[ap1]= min( ip[ap1]+1,xBound[ap1] );
  im[ap1]= max( im[ap1]-1,xBase[ap1] );
    
  real xr1[3];
  int dir;
  for( dir=0; dir<3; dir++ )
    xr1[dir]=X(ip[0],ip[1],ip[2],dir)-X(im[0],im[1],im[2],dir);
  
  ip[ap1]=iv[ap1];
  im[ap1]=ip[ap1];
  
  ip[ap2]= min( ip[ap2]+1,xBound[ap2] );
  im[ap2]= max( im[ap2]-1,xBase[ap2] );
    
  real xr2[3];
  for( dir=0; dir<3; dir++ )
    xr2[dir]=X(ip[0],ip[1],ip[2],dir)-X(im[0],im[1],im[2],dir);

  normal[axis1]=xr1[1]*xr2[2]-xr1[2]*xr2[1];
  normal[axis2]=xr1[2]*xr2[0]-xr1[0]*xr2[2];
  normal[axis3]=xr1[0]*xr2[1]-xr1[1]*xr2[0];

  real l2Norm=sqrt(SQR(normal[axis1])+SQR(normal[axis2])+SQR(normal[axis3]));
  if( l2Norm==0. )
  {
    // cout << "getNormalFromVertex::WARNING: normal has length zero!\n";
    if( recursion )
    {
      // try a nearby point
      int iv2[3] = { iv[0], iv[1], iv[2]  };
      iv2[ap1] = iv[ap1] < xBound[ap1] ? iv[ap1]+1 : iv[ap1]-1;
      iv2[ap2] = iv[ap2] < xBound[ap2] ? iv[ap2]+1 : iv[ap2]-1;
      getNormal( xp, xDim0, xDim1, xDim2, xBase,xBound, iv2,axis,ap1,ap2,normal,FALSE);
      // cout << "getNormalFromVertex::ERROR: normal had zero length, after recursion:\n";
      // printf(" i1=%i,i2=%i,i3=%i normal=(%e,%e,%e)\n",i1,i2,i3,normal[0],normal[1],normal[2]);
    }
    else
    {
      // cout << "getNormalFromVertex::ERROR: normal still has ength zero!\n";
      // printf(" i1=%i,i2=%i,i3=%i \n",i1,i2,i3);
      l2Norm=1.;
      normal[0]=0.;   normal[1]=0.;       normal[2]=1.;
    }
  }
  else
  {
    l2Norm=1./l2Norm;
    // normalize the normal to have length 1
    for(dir=axis1; dir<3; dir++ )
      normal[dir]*=l2Norm;
  }
}

void PlotIt::
plotShadedFace(GenericGraphicsInterface &gi,
	       const RealArray & x, 
	       const Index & I1, 
	       const Index & I2, 
	       const Index & I3, 
	       const int axis,
	       const int side,
	       const int domainDimension,
	       const int rangeDimension,
	       const RealArray & rgb,
               const intArray & mask )
{

  int is1= axis==axis1 ? 0 : 1;
  int is2= axis==axis2 ? 0 : 1;
  int is3= axis==axis3 ? 0 : 1;

  int maskPoints = mask.getLength(0)>0;

  Index I1a=Range(I1.getBase(),I1.getBound()-is1); 
  Index I2a=Range(I2.getBase(),I2.getBound()-is2); 
  Index I3a=Range(I3.getBase(),I3.getBound()-is3); 

  const real *xp = x.Array_Descriptor.Array_View_Pointer3;
  const int xDim0=x.getRawDataSize(0);
  const int xDim1=x.getRawDataSize(1);
  const int xDim2=x.getRawDataSize(2);
  int xBase[3]={x.getBase(0),x.getBase(1),x.getBase(2)};  //
  int xBound[3]={x.getBound(0),x.getBound(1),x.getBound(2)};  //

  real normal[3];
  int iv3[3],iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  int axisp1=(axis+1) % 3;
  int axisp2=(axis+2) % 3;

  FOR_3(i1,i2,i3,I1a,I2a,I3a)
  {  // ---plot a face that is parallel to axis
    if( maskPoints && (mask(i1,i2,i3)==0 || mask(i1+1,i2,i3)==0 || mask(i1,i2+1,i3)==0 ||mask(i1+1,i2+1,i3)==0)  )
      continue;
    
    //  ....loop around the 4 vertices of the face, direction axis is fixed
    glBegin(GL_POLYGON);
    for( int i=0; i<4; i++ )
    {
      iv3[axis  ]=iv[axis];
      iv3[axisp1]=iv[axisp1]+ ( ((i+1)/2) % 2 );
      iv3[axisp2]=iv[axisp2]+ ( ((i  )/2) % 2);

      // getNormal(x,iv3,axis,normal);
      getNormal(xp, xDim0, xDim1, xDim2, xBase,xBound,iv3,axis,axisp1,axisp2,normal);
      glNormal3v(normal);

      // printf("%i %e %e %e %e %e %e\n",i,x(iv3[0],iv3[1],iv3[2],0),x(iv3[0],iv3[1],iv3[2],1),
      //      x(iv3[0],iv3[1],iv3[2],2),normal[0],normal[1],normal[2]);
      
      glVertex3(X(iv3[0],iv3[1],iv3[2],0),
		X(iv3[0],iv3[1],iv3[2],1),
		X(iv3[0],iv3[1],iv3[2],2));
    }
    glEnd();  // GL_POLYGON
  }
}

void PlotIt::
plotLinesOnSurface(GenericGraphicsInterface &gi,
		   const RealArray & x, 
		   const Index & I1, 
		   const Index & I2, 
		   const Index & I3, 
		   const int axis,
		   const bool offsetLines_,
                   const real eps,
                   GraphicsParameters & parameters,
                   const intArray & mask )
// ============================================================================================
// /Description:
//    Plot lines on a surface
// /offsetLines Input): if TRUE draw lines offset from the surface in both directions
// /eps (input): offset amount
// ============================================================================================
{
  // RealArray normal(3);
  // IntegerArray iv(3);
  // int & i1 = iv(0);
  // int & i2 = iv(1);
  // int & i3 = iv(2);
  const real *xp = x.Array_Descriptor.Array_View_Pointer3;
  const int xDim0=x.getRawDataSize(0);
  const int xDim1=x.getRawDataSize(1);
  const int xDim2=x.getRawDataSize(2);

  int i1,i2,i3;
  int maskPoints = mask.getLength(0)>0;

  if( parameters.gridLineColourOption==GraphicsParameters::defaultColour )
    gi.setColour(GenericGraphicsInterface::textColour);
  else if( parameters.gridLineColourOption==GraphicsParameters::colourByGrid )
    gi.setColour(parameters.mappingColour);
  else if( parameters.gridLineColourOption==GraphicsParameters::colourByValue )
  {
    gi.setColour( gi.getColourName(min(max(0,parameters.gridLineColourValue),
				       GenericGraphicsInterface::numberOfColourNames-1)) );
  }
  else 
  {
    gi.setColour(GenericGraphicsInterface::textColour);
  }

  glLineWidth( parameters.size(GraphicsParameters::lineWidth)*
	       gi.getLineWidthScaleFactor());
  glBegin(GL_LINES);
  for( int dir=1; dir<=2; dir++ )
  {
    int axisp = (axis+dir) % 3;    // plot lines parallel to axis=axisp
    int is1= axisp==axis1 ? 1 : 0;
    int is2= axisp==axis2 ? 1 : 0;
    int is3= axisp==axis3 ? 1 : 0;
    Index I1a=Range(I1.getBase(),I1.getBound()-is1); 
    Index I2a=Range(I2.getBase(),I2.getBound()-is2); 
    Index I3a=Range(I3.getBase(),I3.getBound()-is3); 
    FOR_3(i1,i2,i3,I1a,I2a,I3a)
    {
      if( maskPoints && (mask(i1,i2,i3)==0 || mask(i1+is1,i2+is2,i3)==0)  )
        continue;
      glVertex3(X(i1    ,i2    ,i3    ,0),
		X(i1    ,i2    ,i3    ,1),
		X(i1    ,i2    ,i3    ,2));
      glVertex3(X(i1+is1,i2+is2,i3+is3,0),
		X(i1+is1,i2+is2,i3+is3,1),
		X(i1+is1,i2+is2,i3+is3,2));
    }
  }
  glEnd();     // GL_LINES

}

#undef X

void PlotIt::
plotMappingEdges(GenericGraphicsInterface &gi,
		 const RealArray & x, 
		 const IntegerArray & gridIndexRange,
		 GraphicsParameters & parameters,
                 const intArray & mask,
                 int grid /* =0 */ )
// ============================================================================================
// /Description:
//    Plot lines on a surface
// /offsetLines Input): if TRUE draw lines offset from the surface in both directions
// /eps (input): offset amount
// ============================================================================================
{
  const int numberOfDimensions = x.getLength(3);
  if( numberOfDimensions!=3 )
    return;
  
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int imv[3], &i1m=imv[0], &i2m=imv[1], &i3m=imv[2];
  int ipv[3], &i1p=ipv[0], &i2p=ipv[1], &i3p=ipv[2];
  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  int maskPoints = mask.getLength(0)>0;

  if( parameters.blockBoundaryColourOption==GraphicsParameters::colourByGrid )
  {
    gi.setColour( gi.getColourName(grid) ); // colour by grid number
  }
  else if( parameters.blockBoundaryColourOption==GraphicsParameters::defaultColour )
  {
    // Make the default colour for block boundaries the mappingColour *wdh* 2011/0830
    gi.setColour( parameters.mappingColour );
  }
  else
  {
    gi.setColour(GenericGraphicsInterface::textColour);
  }
  

  glLineWidth(parameters.size(GraphicsParameters::lineWidth)*2.*gi.getLineWidthScaleFactor());
 
  if( !maskPoints )
  {
    int axis;
    for(axis=axis1; axis< numberOfDimensions; axis++ ) // plot lines parallel to axis
    {
      int axisp1 = (axis+1) % 3;
      int axisp2 = (axis+2) % 3;
      for( int i=0; i<=1; i++ )   // there are 4 lines parallel
	for( int j=0; j<=1; j++ )   // to this axis
	{
	  for(int dir=axis1; dir<numberOfDimensions; dir++ )
	    iv[dir]=gridIndexRange(Start,dir);
	  iv[axisp1]=gridIndexRange(i,axisp1);
	  iv[axisp2]=gridIndexRange(j,axisp2);
	  // note in the next loop: iv[0:2] is aliased to (i1,i2,i3)
	  glBegin(GL_LINE_STRIP);
	  for( iv[axis] =gridIndexRange(Start,axis); 
	       iv[axis]<=gridIndexRange(End,axis); iv[axis]++)
	  {
	    glVertex3( x(i1,i2,i3,axis1),x(i1,i2,i3,axis2),x(i1,i2,i3,axis3) );
	  }
	  glEnd();     // GL_LINE_STRIP
	}
    }
  }
  else
  {
    // some points are masked -- we need to plot the boundary of the masked region
    glBegin(GL_LINES);
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

    getIndex(gridIndexRange,I1,I2,I3);

    int domainDimension = I3.getBase()==I3.getBound() ? 2 : 3;
    int normalDir=axis3;
    int axis;
    for(int dir=0; dir<domainDimension; dir++ ) // plot two or three sets of lines
    {
      axis = (normalDir+dir+1) % 3;  // plot lines parallel to axis
      int axisp1 = (axis+dir+1) % 3;   
      is1=is2=is3=0;
      isv[axis]=1;

      Index I1a=Range(I1.getBase(),I1.getBound()-is1); 
      Index I2a=Range(I2.getBase(),I2.getBound()-is2); 
      Index I3a=Range(I3.getBase(),I3.getBound()-is3); 
      FOR_3(i1,i2,i3,I1a,I2a,I3a)
      {
	if( (mask(i1,i2,i3)==0 || mask(i1+is1,i2+is2,i3+is3)==0)  ) // don't plot this line segment
	  continue;
        bool onBoundary = (iv[axisp1]==Iv[axisp1].getBase() || iv[axisp1]==Iv[axisp1].getBound() );
	
        i1m=i1; i2m=i2; i3m=i3;
	imv[axisp1]=iv[axisp1]-1;
        i1p=i1; i2p=i2; i3p=i3;
	ipv[axisp1]=iv[axisp1]+1;
	
        // plot this line segment if we are on the boundary OR a neighbouring point
        // in the tangential direction is masked
        if( onBoundary || 
	    mask(i1m,i2 ,i3 )==0 || mask(i1p,i2 ,i3 )==0  ||
	    mask(i1 ,i2m,i3 )==0 || mask(i1 ,i2p,i3 )==0  ||
	    mask(i1 ,i2 ,i3m)==0 || mask(i1 ,i2 ,i3p)==0  ||
	    mask(i1m+is1,i2 +is2,i3 +is3)==0 || mask(i1p+is1,i2 +is2,i3 +is3)==0  ||
	    mask(i1 +is1,i2m+is2,i3 +is3)==0 || mask(i1 +is1,i2p+is2,i3 +is3)==0  ||
	    mask(i1 +is1,i2 +is2,i3m+is3)==0 || mask(i1 +is1,i2 +is2,i3p+is3)==0  )
	{
	  glVertex3(x(i1    ,i2    ,i3    ,0),
		    x(i1    ,i2    ,i3    ,1),
		    x(i1    ,i2    ,i3    ,2));
	  glVertex3(x(i1+is1,i2+is2,i3+is3,0),
		    x(i1+is1,i2+is2,i3+is3,1),
		    x(i1+is1,i2+is2,i3+is3,2));
	}
      }
    }
    glEnd();     // GL_LINES
  }
  
}

//
// plot a trimmed nurbs mapping using openGL trimmed nurbs support
// note that MESA does not support trimming yet.
//

static void nurbsCallback ( GLenum err )//( GLenum err )
{
  aString errst;
  errst = (char *)gluErrorString(err);
  cout<<"GLUnurbs ERROR :  "<<errst<<endl;
  
}

void PlotIt::
plotNurbsMapping( GenericGraphicsInterface &gi,
		  NurbsMapping &map,
		  GraphicsParameters & params,
		  int dList, bool lit)
{
// check if the NURBS is initialized, otherwise there is nothing to plot...
  if (!map.isInitialized())
    return;

// if this is a curve, plot all the subcurves to make sure all corners get resolved
  if( params.plotNurbsCurvesAsSubCurves &&    // *wdh* 021002
      map.getDomainDimension()==1 && map.numberOfSubCurves()>1 )
  {
    int i;
// don't plot the points for the subcurves
    int gridPoints = params.plotGridPointsOnCurves;
    params.plotGridPointsOnCurves = 0;
// AP test: Don't plot endpoints on the sub-curves
    int endPoints;
    params.get(GI_PLOT_END_POINTS_ON_CURVES,endPoints);
    params.set(GI_PLOT_END_POINTS_ON_CURVES,false); 
// don't plot labels either
    int plotLabels = params.labelGridsAndBoundaries;
    params.labelGridsAndBoundaries = 0;
    
    int plotObjectAndExit;
    params.get(GI_PLOT_THE_OBJECT_AND_EXIT,plotObjectAndExit);
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

    for (i=0; i<map.numberOfSubCurves(); i++)
    {
// temporarily change the global id of the subcurve to make it selectable
      int id = map.subCurve(i).getGlobalID();
      map.subCurve(i).setGlobalID(map.getGlobalID());
// call the structured plotter for each subcurve
      plotStructured( gi, map.subCurve(i), params, dList, lit );
      map.subCurve(i).setGlobalID(id);  // reset
    }

    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,plotObjectAndExit); // reset

// use the merged nurbs curve to draw labels and grid points, but not lines
    int plotShadedMappingBoundaries = params.plotShadedMappingBoundaries;
    params.plotShadedMappingBoundaries = 0;
    params.plotGridPointsOnCurves = gridPoints;
    params.labelGridsAndBoundaries = plotLabels;
// AP: And endpoints
    params.set(GI_PLOT_END_POINTS_ON_CURVES,endPoints);
// call the structured plotter for the merged curve    
    plotStructured( gi, map, params, dList, lit );
// reset params
    params.plotShadedMappingBoundaries = plotShadedMappingBoundaries;
  }
  else
    plotStructured( gi, map, params, dList, lit );
}

void PlotIt::
plotTrimmedMapping(GenericGraphicsInterface &gi,
		   TrimmedMapping & map,
		   GraphicsParameters & params,
		   int dList, bool lit)
{

#ifdef USE_PLOTSTRUCTURED
  plotStructured( gi, (Mapping &)map, params, dList, lit );
#else

  // if the mapping does not consist of all Nurbs entities, then use
  // the structured mapping plotter
  if ( ! map.isAllNurbs() )
    {
      plotStructured((Mapping &)map, params, dList, lit );
      return;
    }

  //... it is all nurbs, plot using gl

  //
  // parameters determining the plotting behavior
  //
  bool plotObject = params.plotObject;
  bool plotObjectAndExit = params.plotObjectAndExit;
  bool & plotShaded = params.plotShadedMappingBoundaries;
  bool & plotLines  = params.plotLinesOnMappingBoundaries;
  bool & plotEdges  = params.plotMappingEdges;
  bool & plotAxes   = params.plotTheAxes;
  //
  // set up the GUIState
  //
  GUIState interface;
  
  aString userButtons[][2] = { {"exit", "exit"}, {"plot edges", "plot edges"}, 
			       {"plot lines", "plot lines"}, {"shaded", "shaded"},
			       {"",""} };
  interface.setUserButtons(userButtons);

  if ( !plotObjectAndExit )
    pushGUI(interface);

  //
  // gather the nurbs mappings for the untrimmed surface and the trimming curves
  //
  NurbsMapping &surface = (NurbsMapping &) *map.untrimmedSurface();

#ifdef OLDSTUFF
  int numberOfInnerCurves = map.getNumberOfInnerCurves();
  int numberOfBoundaryCurves = map.getNumberOfBoundaryCurves(); // should be 0 or 1
#endif

  //
  // create display lists if neccessary
  //
  int lightedList;
  int unlitList;
  if( gi.isGraphicsWindowOpen() && dList == 0 )
  {
    lightedList = generateNewDisplayList(1);
    unlitList = generateNewDisplayList(0);
  }

  //
  // set the display bounds
  //
  Bound b;
  RealArray xBound(2,3); xBound = 0;

  if ( params.usePlotBounds ) // use existing plot bounds
    xBound = params.plotBound;
  else
  { // determine plot bounds from the mapping bounding box
    Index xAxes(0, map.getRangeDimension());
    for ( int axis=0; axis<map.getRangeDimension(); axis++ )
    {
      b=map.getRangeBound(Start,axis);
      if ( b.isFinite() )
	xBound(Start,axis) = (real)b;
      b=map.getRangeBound(End,axis);
      if ( b.isFinite() )
	xBound(End,axis) = (real)b;
    }
    if ( params.usePlotBoundsOrLarger ) // use larger of either the mapping existing boxes
    {
      real relativeBoundsChange = max(fabs(params.plotBound-xBound))
	/max(params.plotBound(End,xAxes)-params.plotBound(Start,xAxes));

      if( relativeBoundsChange>params.relativeChangeForPlotBounds ) 
      {
	for( int axis=0; axis<3; axis++ )
	{
	  xBound(Start,axis)=min(xBound(Start,axis),params.plotBound(Start,axis));
	  xBound(End  ,axis)=max(xBound(End  ,axis),params.plotBound(End  ,axis));
	}
      }
    }
    if( !params.isDefault() )
    { // set plot bounds in params to be the result of usePlotBoundsOrLarger
      params.plotBound=xBound;
      params.plotBoundsChanged=TRUE;
    }
  }
  
  setGlobalBound(xBound);
  //setModelViewMatrix();

  //
  // main event loop
  //
  aString answer;
  answer = "plot";
  int it=0;
  while (1)
  {
    if ( it==0 && ( plotObject || plotObjectAndExit ) )
      answer = "plot";
    else if ( it==1 && plotObjectAndExit )
      answer = "exit";
    else
      getAnswer(answer,"");
      
    if ( answer == "exit" )
      break;
    else if ( answer == "plot edges" )
    {
      plotEdges = !plotEdges;
    }
    else if ( answer == "plot lines")
    {
      plotLines = !plotLines;
    }
    else if ( answer == "shaded" )
    {
      plotShaded = !plotShaded;
    }
    else if ( answer == "plot" )
    {
      plotObject = true;
    }
    else if ( answer == "exit ogen" )
      exit(0);

    if ( plotObject )
    {
      if ( dList==0 )
	glNewList(lightedList, GL_COMPILE);
	
      if ( lit )
      {
	glPushName(map.getGlobalID());
	renderTrimmedNurbsMapping( map, params, lit );
	glPopName();
      }
      if ( dList==0 )
	glEndList();

      if ( dList==0 )
	glNewList(unlitList, GL_COMPILE);
	  
      if ( !lit || dList == 0)
      {
	glPushName(map.getGlobalID());
	renderTrimmedNurbsMapping( map, params, false );
	glPopName();
      }

      if ( dList==0 )
	glEndList();

      redraw();
    }

    it++;
  }

  if ( !plotObjectAndExit )
    popGUI();

#endif

}

void PlotIt:: 
renderTrimmedNurbsMapping( GenericGraphicsInterface &gi,
			   TrimmedMapping &trimmedNurb, GraphicsParameters &parameters, bool lit )
{

  glEnable(GL_AUTO_NORMAL);

  bool & plotShaded = parameters.plotShadedMappingBoundaries;
  bool & plotLines  = parameters.plotLinesOnMappingBoundaries;
  bool & plotEdges  = parameters.plotMappingEdges;

  if ( plotShaded && lit)
    {
      gi.setColour(parameters.mappingColour);
      renderTrimmedNurbsByMode( gi, trimmedNurb, GLU_FILL );
    }
  if ( plotLines &&  (!lit) )
    {
      if( parameters.gridLineColourOption==GraphicsParameters::colourByGrid )
	gi.setColour(parameters.mappingColour);
      else
	gi.setColour(GenericGraphicsInterface::textColour);
      renderTrimmedNurbsByMode( gi, trimmedNurb, GLU_OUTLINE_POLYGON );
    }
  if ( plotEdges && (!lit))
    {
      if( parameters.gridLineColourOption==GraphicsParameters::colourByGrid )
	gi.setColour(parameters.mappingColour);
      else
	gi.setColour(GenericGraphicsInterface::textColour);
      renderTrimmedNurbsByMode( gi, trimmedNurb, GLU_OUTLINE_PATCH );
    }

}

static GLUnurbsObj *
getGLUnurbsObj(const GLfloat &type)
// ---------------------------------------------------------------------------------
// /Description:
//   Obtain a pointer to a GLUnurbs object for nurbs rendering in GL
// /Returns :
//   Either a pointer to the requested nurbs renderer or NULL if an error occurred.
//  /Author: KKC
// --------------------------------------------------------------------------------
{

// GLU objects
// GLUnurbsObj, make an array of 3 for the different modes : 
//              GLU_FILL, GLU_OUTLINE_POLYGON and GLU_OUTLINE_PATCH
  static GLUnurbsObj *OV_GLUnurbsArray[3] = {0,0,0}; 

  int i_mode;
  if (type==GLfloat(GLU_FILL))
    i_mode = 0;
  else if (type==GLfloat(GLU_OUTLINE_POLYGON))
    i_mode = 1;
  else if (type==GLfloat(GLU_OUTLINE_PATCH))
    i_mode = 2;
  else
    return NULL;

  if ( OV_GLUnurbsArray[i_mode] == NULL ) 
    {
      OV_GLUnurbsArray[i_mode] = gluNewNurbsRenderer();
      gluNurbsProperty(OV_GLUnurbsArray[i_mode], (GLenum) GLU_DISPLAY_MODE, type);
      // mesa does not seem to understand GLU_NURBS_ERROR, perhaps only the current version...
      // sgi irix doesn't have GLU_NURBS_ERROR either
      //#ifndef OV_USE_MESA  //old fix
      //..We assume GLU_NURBS_ERROR is a MACRO (if const etc, could do something else here)  **pf
#ifdef GLU_NURBS_ERROR
      gluNurbsCallback(OV_GLUnurbsArray[i_mode], GLU_NURBS_ERROR, ( void (*)())OV_gluCallback);
#endif
      gluNurbsProperty(OV_GLUnurbsArray[i_mode], (GLenum) GLU_SAMPLING_TOLERANCE, 75.0);

      // alternate modes for tesselation creation:
      // use error in pixels between tesselation and surface for polygon creation
      //gluNurbsProperty(OV_GLUnurbsArray[i_mode], GLU_SAMPLING_METHOD, GLU_PARAMETRIC_ERROR);
      //gluNurbsProperty(OV_GLUnurbsArray[i_mode], GLU_PARAMETRIC_TOLERANCE, 50.0);
      // specify the samplings in each direction
      //gluNurbsProperty(OV_GLUnurbsArray[i_mode], GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
      //gluNurbsProperty(OV_GLUnurbsArray[i_mode], GLU_U_STEP, 10);
      //gluNurbsProperty(OV_GLUnurbsArray[i_mode], GLU_V_STEP, 10);
    }

  return OV_GLUnurbsArray[i_mode];
}

void PlotIt::
renderTrimmedNurbsByMode(GenericGraphicsInterface &gi, TrimmedMapping &map, GLfloat mode)
{

  GLUnurbsObj *nurbsRenderer = getGLUnurbsObj(mode);

  gluBeginSurface(nurbsRenderer);

  renderNurbsSurface( gi, (NurbsMapping &) *map.untrimmedSurface(), mode );

#ifdef OLDSTUFF
  gluBeginTrim(nurbsRenderer);
  renderNurbsCurve( (NurbsMapping &) *map.getOuterCurve(), mode, GLU_MAP1_TRIM_3 );
  gluEndTrim(nurbsRenderer);

  for ( int i=0; i<map.getNumberOfInnerCurves(); i++ )
    {
      gluBeginTrim(nurbsRenderer);
      renderNurbsCurve( (NurbsMapping &) *map.getInnerCurve(i), mode, GLU_MAP1_TRIM_3 );
      gluEndTrim(nurbsRenderer);
    }
  
#else
  for ( int i=0; i<map.getNumberOfTrimCurves() && map.trimmingIsValid(); i++ )
    {
      gluBeginTrim(nurbsRenderer);
      renderNurbsCurve( gi, (NurbsMapping &) *map.getTrimCurve(i), mode, (GLenum)GLU_MAP1_TRIM_3 );
      gluEndTrim(nurbsRenderer);
    }
#endif


  gluEndSurface(nurbsRenderer);

}

void PlotIt::
renderNurbsSurface(GenericGraphicsInterface &gi, NurbsMapping &map, GLfloat mode)
{
#if 0
  GLfloat *uknots, *vknots, *ctlarray;
  int n_uknots = map.m1+1; 
  int n_vknots = map.m2+1; 
  
  uknots = new GLfloat[n_uknots];
  vknots = new GLfloat[n_vknots];

  ctlarray = new GLfloat[map.cPoint.getLength(0)*
			map.cPoint.getLength(1)*
			(map.getRangeDimension()+1)];

  int u,v;

  for ( u=0; u<n_uknots; u++ )
    uknots[u] = GLfloat(map.uKnot(u));

  for ( v=0; v<n_vknots; v++ )
    vknots[v] = GLfloat(map.vKnot(v));

  int idx = 0;
  for ( u=0; u<map.cPoint.getLength(0); u++ )
    for ( v=0; v<map.cPoint.getLength(1); v++ )
      for ( int a=0; a<(map.getRangeDimension()+1); a++ )
	ctlarray[idx++] = GLfloat(map.cPoint(u,v,a));

  int u_stride = map.cPoint.getLength(1)*(map.getRangeDimension()+1);
  int v_stride = map.getRangeDimension()+1;

  int uorder = map.p1 + 1;
  int vorder = map.p2 + 1;

  gluNurbsSurface(getGLUnurbsObj(mode),
		  n_uknots,
		  uknots,
		  n_vknots,
		  vknots,
		  u_stride,
		  v_stride,
		  ctlarray,
		  uorder,
		  vorder,
		  GL_MAP2_VERTEX_4);

  delete [] uknots;
  delete [] vknots;
  delete [] ctlarray;
#endif
}

void PlotIt::
renderNurbsCurve(GenericGraphicsInterface &gi,NurbsMapping &map, float mode, int type_)
{
#if 0
  // treat all curves as 3d, adjusting z if needed
  GLenum type = (GLenum) type_;

  GLfloat *knots, *ctlarray;

  int stride = map.getRangeDimension()+1;
  
  int n_knots = map.m1+1;
  if ( type == (GLenum)GLU_MAP1_TRIM_3 )
    stride = 3;
  else 
    stride = 4;
  int n_ctl = (map.n1+1) * stride;
  
  //  int n_ctl = (map.n1+1) * (map.getRangeDimension()+1);
  
  knots = new GLfloat[n_knots];
  ctlarray = new GLfloat[n_ctl];

  for ( int k=0; k<n_knots; k++ )
    knots[k] = GLfloat(map.uKnot(k));
  
  int idx=0;
  if ( type == (GLenum) GLU_MAP1_TRIM_3 )
    for ( int u=0; u<(map.n1+1); u++ )
	for ( int a=0; a<(map.getRangeDimension()+1); a++ )
	  ctlarray[idx++] = GLfloat(map.cPoint(u,a));
  else if ( map.getRangeDimension()==3 )
    {
      stride = 4;
      for ( int u=0; u<(map.n1+1); u++ )
	for ( int a=0; a<(map.getRangeDimension()+1); a++ )
	  ctlarray[idx++] = GLfloat(map.cPoint(u,a));
    }
  else
    for ( int u=0; u<(map.n1+1); u++ )
      {
	for ( int a=0; a<(map.getRangeDimension()); a++ )
	  ctlarray[idx++] = GLfloat(map.cPoint(u,a));
	ctlarray[idx++] = 0.;
	ctlarray[idx++] = GLfloat(map.cPoint(u,map.getRangeDimension()));
      }

  int order = map.p1 + 1;

  gluNurbsCurve(getGLUnurbsObj(mode), n_knots, knots, stride, ctlarray, order, type);
  
  delete [] knots;
  delete [] ctlarray;
#endif
}
