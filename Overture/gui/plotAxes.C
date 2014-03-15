#include "GL_GraphicsInterface.h"
#ifdef NO_APP
using GUITypes::real;
#endif

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{plotAxes}} 
void GL_GraphicsInterface:: 
plotAxes(const RealArray & xBound_, 
	 const int numberOfDimensions,
         GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	 int win_number /* = -1 */)
//=========================================================================================
//  /Description:
//    This routines generates the display list for plotting axes along the specified bounds, 
//    for the given number of space dimensions. The actual plotting is done by the display
//    call-back function, which is activated by calling the function redraw().
//  /xBound(0:1,0:2) (input): Bounds to use for the axes. {\ff xBound(Start,axis1)},
//  {\ff xBound(End,axis1)}, ...
//  /numberOfDimensions (input): Number of space dimensions. This determines how many
//    axes to draw.
//  /parameters (input): Specification of the graphics parameters (line width, etc.)
//  /win\_number (input): The number of the window where the axes should be plotted. If win\_number==-1,
//                       or it is omitted, the axes will be plotted in the currentWindow.
//  /Return Values: none.
//  
//  /Author: WDH \& AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  

  if ( win_number < 0 || win_number >= moglGetNWindows() )
    win_number = currentWindow;
  
  real xScale[3] = {parameters.xScaleFactor,parameters.yScaleFactor,parameters.zScaleFactor};
  
  RealArray xBound;
  xBound=xBound_;
  
  bool scaleAxes = xScale[0]!=1. || xScale[1]!=1. || xScale[2]!=1.;

  // printf("********* plotAxes scaleAxes=%i ************\n",scaleAxes);

  if( scaleAxes )
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      xBound(0,axis)*=xScale[axis];
      xBound(1,axis)*=xScale[axis];
    }
  }

  if( axesOriginOption[win_number]==1 )
  {
    axesOrigin[win_number](0)=rotationCenter[win_number][0];
    axesOrigin[win_number](1)=rotationCenter[win_number][1];
    axesOrigin[win_number](2)=rotationCenter[win_number][2];
  }
  else
  {
    axesOrigin[win_number]=(real)defaultOrigin;
  }

  real & xAxesOrigin  = axesOrigin[win_number](0);
  real & yAxesOrigin  = axesOrigin[win_number](1);
  real & zAxesOrigin  = axesOrigin[win_number](2);
//   plotBackGroundGrid = parameters.plotBackGroundGrid;
  // printf("plotAxes: plotBackGroundGrid=%i\n",plotBackGroundGrid);
  

  // Do not rotate the y-axis labels if rotateAxisLabels==FALSE
  const bool rotateAxisLabels=FALSE;  // if true we rotate the axis labels to be parallel to the axis

// *wdh* 991111
//   bool scalesEqual= 
//     (numberOfDimensions==1                                  ) ||
//     (numberOfDimensions==2 && windowScaleFactor[0]==windowScaleFactor[1]) ||
//     (numberOfDimensions==3 && windowScaleFactor[0]==windowScaleFactor[2] && 
//                               windowScaleFactor[1]==windowScaleFactor[2]) ;

//  bool scalesEqual= fabs(aspectRatio[win_number]-1.)<1.e-3;
  bool scalesEqual= keepAspectRatio && fabs(aspectRatio[win_number]-1.)<1.e-3;
// AP: scalesEqual==true produces strange axes!!!
  scalesEqual = false;

  int ii;
  real xMin[3], xMax[3], xOrigin[3];

// AP fixing thread problem
  real maxLength;
  if (numberOfDimensions == 3)
    maxLength = max(xBound(End,0)-xBound(Start,0),
		    xBound(End,1)-xBound(Start,1),
		    xBound(End,2)-xBound(Start,2));
  else if (numberOfDimensions == 2)
    maxLength = max(xBound(End,0)-xBound(Start,0),
		    xBound(End,1)-xBound(Start,1));
  else
    maxLength = xBound(End,0)-xBound(Start,0);
  
  int axis;
  for( axis=axis1; axis<numberOfDimensions; axis++ )
  { // keep the axes at least as long as factor times the longest axis
    real factor= scalesEqual ? .5 : 0.; // *** .5;
    if( xBound(End,axis)-xBound(Start,axis) > maxLength*factor ) 
    {
      xMin[axis]=xBound(Start,axis);
      xMax[axis]=xBound(End  ,axis);
    }
    else
    {
      xMin[axis]=xBound(Start,axis)-.5*factor*(maxLength-(xBound(End,axis)-xBound(Start,axis)));
      xMax[axis]=xBound(End  ,axis)+.5*factor*(maxLength-(xBound(End,axis)-xBound(Start,axis)));
    }
  }    
// the remaining axes (if any) are unit length
  for( axis=numberOfDimensions; axis<3; axis++ )
  {
    xMin[axis]=0.;
    xMax[axis]=1.;
  }
  
  int numberOfd0=7;
  real d0[] = {.025,.05,.1,.2,.5,1.,2. };
  char format[40],number[40];

  real xxMin[3],xxMax[3],dx[3],xd[3],xv[3], dxShift[3];
  for (ii=0; ii<3; ii++)
  {
    xxMin[ii]=0.; xxMax[ii]=1.; xd[ii]=0.; xv[ii]=0.; dxShift[ii]=0.;
  }
  
  int nx[3];
  for (ii=0; ii<3; ii++)
    nx[ii]=1;
  for (ii=0; ii<3; ii++)
    dx[ii]=(xMax[ii]-xMin[ii]);

  // Set the origin
  for (ii=0; ii<3; ii++)
    xOrigin[ii]=xMin[ii];

  if( numberOfDimensions==1 )
    xOrigin[1]-=(xMax[0]-xMin[0])*.05;
  
  if( xAxesOrigin!=(real)defaultOrigin )
    xOrigin[0]=xAxesOrigin;
  if( yAxesOrigin!=(real)defaultOrigin )
    xOrigin[1]=yAxesOrigin;
  if( zAxesOrigin!=(real)defaultOrigin )
    xOrigin[2]=zAxesOrigin;
  

  // tick marks are a fraction of this length: dxShift
  real dxmax = max(dx[0],dx[1],dx[2]);
  if( !scalesEqual )
  {
    for (ii=0; ii<3; ii++)
      dxShift[ii]=dx[ii];  // tick marks all different lengths if scale factors not equal
  }
  else
  {
    for (ii=0; ii<3; ii++)    
      dxShift[ii]=dxmax;     // all tick marks the same length if scale factors equal
  }
  

  //  Compute the tick locations.
  //                 ---*-----------------*---
  //                  xMin               xMax
  //                    |<----- dx ------>|
  // if   dx = 10**power then
  //   then we try to choose the tick interval xd to be about 
  //         d0[j]**power * 10**power
  //  const real magFactor = min(10.,magnificationFactor[win_number]);  // don't draw too many extra
  const real magFactor = magnificationFactor[win_number];
  real scale=1;
  if( magFactor>1. )
    scale=int(magFactor+.5);
  else if( magFactor>.001 )
    scale=1./(int(1./magFactor+.5));
  
  //  printf("magFactor=%e, scale=%e\n", magFactor, scale);

  int maximumNumberOfLabels[3];
  // AP: Scale the max number of labels with the size of each axis
  for (ii=0; ii<3; ii++)
     maximumNumberOfLabels[ii] = int(scale*dx[ii]/dxmax + 0.5) * (numberOfDimensions==2 ? 8 : 6);

  for (ii=0; ii<3; ii++)
    maximumNumberOfLabels[ii]=max(1,maximumNumberOfLabels[ii]); // at least one

  for( axis=axis1; axis<numberOfDimensions; axis++ )
  {
    int power = int(log10( max(dx[axis],REAL_EPSILON) ));
    power = power < 0 ? power+1 : power;
    power = power==0 ? 1 : power; // trouble if power=0
    for( int j=0; j<numberOfd0; j++ )
    {
      xd[axis]=pow( (double) d0[j]*10./scale, (double) power);
      if(xMin[axis]>=0.)
	xxMin[axis]=xMin[axis]-fmod(xMin[axis]-.001*dx[axis],xd[axis])-.001*dx[axis];
      else
	xxMin[axis]=xMin[axis]+fmod(xd[axis]-xMin[axis]+.001*dx[axis],xd[axis])-.001*dx[axis];
      if(xxMin[axis]<xMin[axis]-.002*dx[axis])
	xxMin[axis]=xxMin[axis]+xd[axis];
      
      if(xMax[axis]>=0.)
	xxMax[axis]=xMax[axis]-fmod(xMax[axis]+.001*dx[axis],xd[axis])+.001*dx[axis];
      else
	xxMax[axis]=xMax[axis]+fmod(xd[axis]-xMax[axis]-.001*dx[axis],xd[axis])+.001*dx[axis];
      if(xxMax[axis]>xMax[axis]+.002*dx[axis])
        xxMax[axis]=xxMax[axis]-xd[axis];

      nx[axis]=int( (xxMax[axis]-xxMin[axis])/xd[axis]+.5 );

      if(nx[axis]<=maximumNumberOfLabels[axis] && nx[axis]>1 ) break;
    }

    if( nx[axis]<2 || nx[axis] > maximumNumberOfLabels[axis] )
    {
      nx[axis]= nx[axis]<2 ? 1 : maximumNumberOfLabels[axis];   // *wdh =4;
      xd[axis]=(xMax[axis]-xMin[axis])/nx[axis];
      xxMin[axis]=xMin[axis];
      xxMax[axis]=xMax[axis];
    }
  }


  for (ii=0; ii<3; ii++)
  {
    dx[ii]/=magnificationFactor[win_number];
    dxShift[ii]/=magnificationFactor[win_number];
  }
  dxmax /= magnificationFactor[win_number]; 

  glLineWidth(parameters.size(lineWidth)*lineWidthScaleFactor[win_number]); // reset
  setColour(textColour);
  for( axis=axis1; axis<numberOfDimensions; axis++ )
  {
    glBegin(GL_LINES);

    //  Set the character right and up vectors.
    real xp[3],xRight[3],xUp[3];
    for (ii=0; ii<3; ii++)
    {
      xp[ii]=0.;
      xRight[ii]=0.;   
      xUp[ii]=0.;
    }
    xRight[axis]=1.;  // characters are tangent to this vector
    
    if( axis==axis1  )
    {
      xRight[axis] = 0;
      xRight[1] = 1;
      xUp[0]=-1.;
    }
    else if( axis==axis2 )
    {
      xRight[axis] = 0;
      xRight[0] = 1;
      xUp[1] = 1;
    }
    else
    {
      xRight[axis] = 0;
      xRight[1] = 1;
      xUp[2] = 1;
    }
    
    // ==== draw axes ======
    for (ii=0; ii<3; ii++)
      xv[ii]=xOrigin[ii];
    xv[axis]=xMin[axis];
    glVertex3(xv[0],xv[1],xv[2]);
    for (ii=0; ii<3; ii++)
      xv[ii]=xOrigin[ii];
    xv[axis]=xMax[axis];
    glVertex3(xv[0],xv[1],xv[2]);
    
    // Draw the ticks.
    int axisp1 = (axis +1) % max(2,numberOfDimensions);
    int axisp2 = (axis +2) % max(2,numberOfDimensions);
    int i;
//     if( numberOfDimensions==2 && plotBackGroundGrid )
//     {
//       printf("plotAxes: plotBackGroundGrid: numberOfDimensions=%i, plotBackGroundGrid=%i\n",
// 	     numberOfDimensions,plotBackGroundGrid);
//     }
//     else
//     {
//       printf("plotAxes: do NOT plotBackGroundGrid: numberOfDimensions=%i, plotBackGroundGrid=%i\n",
// 	     numberOfDimensions,plotBackGroundGrid);
//     }
    
//    printf("nx[%i]=%i\n", axis, nx[axis]);
    
    for( i=-1; i<=nx[axis]; i++ )
    {
      // Draw smaller tick marks to sub-divide each interval
      int numberOfSubIntervals=5;
      for( int j=0; j<=numberOfSubIntervals; j++ )
      {
	for (ii=0; ii<3; ii++)
	  xv[ii]=xOrigin[ii];
	xv[axis]=xxMin[axis]+(i+real(j)/numberOfSubIntervals)*xd[axis];
	
        if( xv[axis]>=xMin[axis]-.001*xd[axis] && xv[axis]<=xMax[axis]+.001*xd[axis] )
	{
	  glVertex3(xv[0],xv[1],xv[2]);
// these parameters can be different in different windows, which is a problem if an inactive
// window is replotted
	  real tickSize = ((j%numberOfSubIntervals==0) ? parameters.size(GraphicsParameters::axisMajorTickSize)
                                                       : parameters.size(GraphicsParameters::axisMinorTickSize) );
	  if( numberOfDimensions==1 )
	  {
	    xv[axisp1]+=tickSize*dxShift[0];   // tick mark direction
          }
	  else if( numberOfDimensions==2 )
	  {
	    xv[axisp1]+=tickSize*dxShift[axisp1];   // tick mark direction
          }
	  else
	  {
// AP
//  	    xv[axisp1]+=tickSize*dxShift[axisp1]*fabs(xUp[axisp1]);     // tick mark direction
//  	    xv[axisp2]+=tickSize*dxShift[axisp2]*fabs(xUp[axisp2]);     // tick mark direction
// AP wants ticmarks with the same length in all directions
	    xv[axisp1]+=tickSize*dxmax*fabs(xRight[axisp1]);     // tick mark direction
	    xv[axisp2]+=tickSize*dxmax*fabs(xRight[axisp2]);     // tick mark direction
	  }
	  
	  glVertex3(xv[0],xv[1],xv[2]);

          // ******** plot the back ground grid ************
          if( numberOfDimensions==2 && plotBackGroundGrid[win_number] && j<numberOfSubIntervals )
	  {
            glEnd();
            if( j==0 )
              glColor3(.8,.8,.8);          // light gray
            else
              glColor3(.9,.9,.9);          // lighter gray
            glBegin(GL_LINES);
            // glEnable(GL_LINE_STIPPLE);
	    // glLineStipple(2,0xAAAA);  // dash pattern - - - - - - -
            // xv=xOrigin;
	    for (ii=0; ii<3; ii++)
	      xv[ii]=xMin[ii];
            xv[2]=xOrigin[2];  // set z-level
            xv[axis]=xxMin[axis]+(i+real(j)/numberOfSubIntervals)*xd[axis];
            glVertex3(xv[0],xv[1],xv[2]-1.e-2); // ****
            xv[axisp1]=xMax[axisp1];
  	    glVertex3(xv[0],xv[1],xv[2]-1.e-2);
            // glDisable(GL_LINE_STIPPLE);
            glEnd();
            setColour(textColour); 
            glBegin(GL_LINES);
	  }
	}
      }	
    } // end for i=-1,...,nx[axis]
    

    glEnd();

    //  Set the character height.
    
    real size = parameters.size(axisNumberSize); // AP: This number can be different in different windows!!!
    if( numberOfDimensions==3 )
      size*=1.25;  // make labels bigger in 3d
    
    real height=size/1.25;

    // **** Determine how to shift the axis number labels away from the axis ****
    // **** xp(.) is the position of the label
    for (ii=0; ii<3; ii++)
      xp[ii]=xOrigin[ii];
    real shiftFromAxis= ( rotateAxisLabels || (axis==axis1 || axis==axis3) ) ? 1.2 : .25;   // 1.25
    if(numberOfDimensions==1)
      xp[1]=xOrigin[1]-height*shiftFromAxis*dxShift[0];   // dx(axisp1); 
    else if(numberOfDimensions==2)
      xp[axisp1]=xOrigin[axisp1]-height*shiftFromAxis*dxShift[axisp1];   // dx(axisp1); 
    else
    {
      xp[axisp1]=xOrigin[axisp1]-height*shiftFromAxis*dxmax*fabs(xUp[axisp1]);
      xp[axisp2]=xOrigin[axisp2]-height*shiftFromAxis*dxmax*fabs(xUp[axisp2]);
    }    
    int nx1=int(
           log10( max(max(fabs(xxMin[axis]),fabs(xxMax[axis])),xxMax[axis]-xxMin[axis])*1.001*xScale[axis] )
               +100.)-100;
    int nx2;
    
    if (xxMax[axis]-xxMin[axis] < REAL_MIN)
      nx2 = nx1-4;
    else
      nx2 = max(nx1-4,int(log10((real)(xxMax[axis]-xxMin[axis])*1.001*xScale[axis])+100.)-100);

    nx1=max(-5,min(5,nx1));  // **** wdh ****
    nx2=max(-5,min(5,nx2));
    

    int axisNumberWidth;
    if(nx1>=-4 && nx1<=0)
    {
      axisNumberWidth=5-nx2;
      sPrintF(format,"%%%i.%if",axisNumberWidth,2-nx2);
    }
    else if(nx1>0 && nx1<=3)
    {
      axisNumberWidth=3+nx1+max(0,2-nx2);
      sPrintF(format,"%%%i.%if", axisNumberWidth,max(0,2-nx2));
    }
    else
    {
      axisNumberWidth=8+nx1-nx2;
      sPrintF(format,"%%%i.%ie",axisNumberWidth,1+nx1-nx2);
    }
    
    
    int centering=0;
    for(i=0; i<=nx[axis]; i++ )
    {
      xv[axis]=xxMin[axis]+i*xd[axis];  // ***
      xp[axis]=xv[axis];

      sPrintF(number,format,xv[axis]/xScale[axis]);

      if (axis == axis3 || axis == axis1 || axis == axis2) 
	centering = 1;
      
      xLabel(number,xp,size,centering,xRight,xUp,parameters, win_number);

    } // end for i=0,...,nx[axis]
    

    // Label the axis with "x-axis" or "y-axis" or "z-axis"
    real shiftForLabels=(rotateAxisLabels || (axis==axis1 || axis==axis3)) ? 2.5 
                                             : min(8,axisNumberWidth)*.8;  // .8 = width/height
    xp[axis]=xxMin[axis]+.95*nx[axis]*xd[axis];   // put label .95 of the way along the axis
    if(numberOfDimensions < 3)
      xp[axisp1]=xOrigin[axisp1]-height*shiftForLabels*dxShift[axisp1]; 
    else
    {
      xp[axisp1]=xOrigin[axisp1]+parameters.size(GraphicsParameters::axisMajorTickSize)*dxmax*fabs(xRight[axisp1]);
      xp[axisp2]=xOrigin[axisp2]+parameters.size(GraphicsParameters::axisMajorTickSize)*dxmax*fabs(xRight[axisp2]);
    }    
    aString axisLabel;
    if( axis==axis1 )
      axisLabel = xAxisLabel[win_number]==blankString ? "X" : (const char*)xAxisLabel[win_number].c_str();
    else if( axis==axis2 )
      axisLabel = yAxisLabel[win_number]==blankString ? "Y" : (const char*)yAxisLabel[win_number].c_str();
    else
      axisLabel = zAxisLabel[win_number]==blankString ? "Z" : (const char*)zAxisLabel[win_number].c_str();
	 
    // plot the label on the other side of the axes
    centering = -1;
    xLabel(axisLabel,xp,size,centering,xRight,xUp,parameters, win_number);

    
  } // end for axis...  
  
//  glEndList();
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{eraseAxes}} 
void GL_GraphicsInterface:: 
eraseAxes(int win_number)
//=========================================================================================
//  /Description:
//    Erase the display lists holding the axes in window `win\_number'.
//  /Author: WDH \& AP
//\end{GL_GraphicsInterfaceInclude.tex} 
// =======================================================================================
{
  glDeleteLists(getAxesDL(win_number),1);  // clear the axes if they are there.
}
