#include "GL_GraphicsInterface.h"
#include "GridCollection.h"
#include "DataPointMapping.h"
#include "SquareMapping.h"
#include "display.h"
#include "PlotIt.h"

//\begin{>>PlotItInclude.tex}{\subsection{plot: 1D line plots}} 
void PlotIt::
plot(GenericGraphicsInterface &gi,
     const realArray & t, 
     const realArray & x, 
     const aString & title /* = nullString */, 
     const aString & tName /* = nullString */,
     const aString *xName        /* =NULL */,
     GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ ) 
//=============================================================================
// /Description:
//    Make some 1D line plots. 
// /t (input) : t(0:n-1) - values along the horizontal axis
// /x (input) : x(0:n-1,0:nv-1) - values to plot, nv components
// /title (input):
// /tName (input): name for horizontal axis
// /xName[nv] (input): names of components/
//\end{PlotItInclude.tex}  
//=============================================================================
{
  if( !gi.isGraphicsWindowOpen() )
    return;
 
  // ::display(t,"PlotIt::plot (line) t");

 int n=t.getLength(0);
  Range I(t.getBase(0),t.getBound(0));
  // Make a 1D grid and grid function
  DataPointMapping line;
  int i;
  line.setDataPoints(t,1,1);  // *wdh* 000819 1=position of coordinates, 1=domain dimension

  MappedGrid c(line);   // a grid
  int numGhost=0;       // remove ghost points since contour1d may plot these
  c.setNumberOfGhostPoints(0,axis1,numGhost);
  c.setNumberOfGhostPoints(1,axis1,numGhost);
  c.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );

  // realArray & vertex = c.vertex();
  // ::display(vertex,"PlotIt::plot array t -> grid -> vertex");
  // ::display(c.mask(),"PlotIt::plot array mask");
  // ::display(c.gridIndexRange(),"PlotIt::plot array gridIndexRange");

  Range all;
  int nv=x.getLength(1); // number of components to plot
  realMappedGridFunction u(c,all,all,all,nv);
  if( xName!=NULL )
  {
    for( i=0; i<nv; i++ )
      u.setName(xName[i],i);
  }
  Range I1(c.indexRange()(Start,axis1),c.indexRange()(End,axis1));
  for( i=0; i<nv; i++ )
    u(I1,all,all,i)=x(I,i);
  GraphicsParameters localParameters;
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

  // save old values
  aString oldTitle=psp.topLabel;
  bool colourLineContours=psp.colourLineContours;
  psp.set(GI_TOP_LABEL,title);
  psp.set(GI_COLOUR_LINE_CONTOURS,TRUE);

  if( tName!="" )
    gi.setAxesLabels(tName);
  //  psp.set(GI_X_AXIS_LABEL,tName);

  contour(gi,u,psp);

  // reset
  psp.set(GI_TOP_LABEL,oldTitle);
  psp.set(GI_COLOUR_LINE_CONTOURS,colourLineContours);

}

//\begin{>>PlotItInclude.tex}{\subsection{plot: surface plots}\label{timeSequence}}
void PlotIt::
plot(GenericGraphicsInterface &gi,
     const realArray & x, 
     const realArray & t,
     const realArray & u_, 
     GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
//=============================================================================
// /Description:
//    Make a surface plot of a sequence of 1D functions
// /x (input) : t(0:n-1) - values along the horizontal axis ("x-axis")
// /t (input) : t(0:nv-1) - times corresponding to the different components of u ("y-axis")
// /u (input) : u(0:n-1,0:nv-1) - values to plot, nv components  ("z-axis")
// 
// /parameters: Use these parameters to set the title, sub-titles and axis labels etc.
//\end{PlotItInclude.tex}  
//=============================================================================
{
  if( !gi.isGraphicsWindowOpen() )
    return;
  
// save the current window number 
  int startWindow = gi.getCurrentWindow();

  char buff[80];
  aString answer,answer2;
  aString menu[]= { "plot",
                   "set vertical scale factor",
		   "plot shaded surface (toggle)",
                   "set viewing angle",
		   " ",
		   "set origin for axes",
		   "erase",
		   "erase and exit",
		   "exit this menu",
		   "" };


  int list=gi.generateNewDisplayList(); // get a new display list to use
  assert(list!=0);

  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(TRUE);  // TRUE means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

  // int  & component             = psp.componentForContours;
  bool & plotShadedSurface        = psp.plotShadedSurface;
// AP  axesOrigin[currentWindow]       = psp.axesOrigin;

  real uScaleFactor=.4;  // .75;    // scale height by this factor (relative to a height of 1)
  
  real xAngle=-60., yAngle=-10., zAngle=0.;  // viewing angles in degrees
  
  plotShadedSurface=FALSE;
  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

//    if( lighting )
//    {
//      // When lighting is on we need to turn on material properties
//      glColorMaterial(GL_FRONT,GL_DIFFUSE);   // this causes the grids to reflect according to their colour
//      glColorMaterial(GL_FRONT,GL_AMBIENT); 
//      glEnable(GL_COLOR_MATERIAL);
//    }

  // set default prompt
  gi.appendToTheDefaultPrompt("plot>");

  int nv=u_.getLength(1); // number of components to plot
  Range N(0,nv-1);
  int n=x.getLength(0);
  Range I(x.getBase(0),x.getBound(0));
  
  real uDiff=max(u_(I,N))-min(u_(I,N));
  if( uDiff==0. )
    uDiff=1.;
	


  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
      gi.getMenuItem(menu,answer);

// make sure that the currentWindow is the same as startWindow! (It might get changed 
// interactively by the user)
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if( answer=="plot shaded surface (toggle)" )
    {
      plotShadedSurface= !plotShadedSurface;
    }
    else if( answer=="set vertical scale factor" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter scale factor for surface height (default=%f)",uScaleFactor)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e",&uScaleFactor);
      }
    }
    else if( answer=="set viewing angle" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter xAngle,yAngle,zAngle (rotations about each axis) (current=%f,%f,%f)",
            xAngle,yAngle,zAngle)); 
      if( answer2!="" )
      {
        xAngle=yAngle=zAngle=0.;
	sScanF(answer2,"%e %e %e",&xAngle,&yAngle,&zAngle);
      }
    }
    else if( answer=="erase" )
    {
      plotObject=FALSE;
      glDeleteLists(list,1);
      gi.redraw();
    }
    else if( answer=="exit this menu" )
    {
      break;
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
      glDeleteLists(list,1);
      gi.redraw();
      break;
    }
    else if( answer=="set origin for axes" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter origin for axes (x,y,z) (enter return for default)")); 

      if( answer2 !="" && answer2!=" ")
      {
	real xo = GenericGraphicsInterface::defaultOrigin, yo = GenericGraphicsInterface::defaultOrigin, 
	  zo = GenericGraphicsInterface::defaultOrigin;
	sScanF(answer2,"%e %e %e",&xo, &yo, &zo);
	gi.setAxesOrigin(xo, yo, zo);
      }
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else
    {
      cout << "Unknown response = " << answer << endl;
    }
    if( plotObject )
    {
      if( !plotShadedSurface )
      {
	// plot multiple lines


	glDeleteLists(list,1);  // clear the plot

      // get Bounds on the grids
	RealArray xBound(2,3); 

        realArray u;
	u=u_*uScaleFactor/uDiff;
	
	xBound(Start,0)=min(x(I));
	xBound(End  ,0)=max(x(I));
	xBound(Start,1)=min(t(N));
	xBound(End  ,1)=max(t(N));
	xBound(Start,2)=min(u(I,N));
	xBound(End  ,2)=max(u(I,N));

	real deltaU = xBound(End,2)-xBound(Start,2);
	if( deltaU==0. )
	{
	  xBound(End,2)+=.5;
	  xBound(Start,2)-=.5;
	}

	Range Axes(0,1);  
	real scale = max(xBound(End,Axes)-xBound(Start,Axes));   // *** ??
//	real fractionOfScreen=.75;                                  // scale to [fOS,fOS]
        // keep aspect ratio in x-y plane
// AP: This should not be done here anymore. See the display() function
//  	windowScaleFactor[currentWindow][0]=fractionOfScreen[currentWindow]*2./scale;                
//  	windowScaleFactor[currentWindow][1]=fractionOfScreen[currentWindow]*2./scale;                 
//  	windowScaleFactor[currentWindow][2]=uScaleFactor*fractionOfScreen[currentWindow]*
//  	  2./(xBound(End,2)-xBound(Start,2)); 

/* ----
	// ** do not keep the aspect ratio ???
	real scale1 = xBound(End,0)-xBound(Start,0);
	real scale2 = xBound(End,1)-xBound(Start,1);
	real scale3 = xBound(End,2)-xBound(Start,2);
        
	windowScaleFactor[0]=fractionOfScreen*2./scale1;                 
	windowScaleFactor[1]=fractionOfScreen*2./scale2;                 
	windowScaleFactor[2]=.5*fractionOfScreen*2./scale3;                 
---- */
	gi.resetGlobalBound(gi.getCurrentWindow());
	gi.setKeepAspectRatio(false); 
	gi.setGlobalBound(xBound);
  	// ::display(xBound,"plot(x,t,u) : xBound");
	
	gi.setAxesDimension(3); // plot axes as if for a 3D grid.

	// plot labels on top and bottom
	gi.plotLabels( psp );

	glNewList(list,GL_COMPILE); 

// the following lines could probably be removed if we uncommented the above resetGlobalBound...
	// Scale the picture to fit in [-1,1]
//  	glMatrixMode(GL_MODELVIEW);
//  	glPushMatrix();

	// scale:
//  	glScalef(windowScaleFactor[currentWindow][0], windowScaleFactor[currentWindow][1],
//  		 windowScaleFactor[currentWindow][2]);                  

//  	glTranslatef(-(xBound(Start,0)+xBound(End,0))*.5,
//  		     -(xBound(Start,1)+xBound(End,1))*.5,
//  		     -(xBound(Start,2)+xBound(End,2))*.5);     // centre object


// end remove

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);  // Is this needed?

	gi.setColour("blue");
//	glLineWidth(contourLineWidth);

	for( int j=N.getBase(); j<=N.getBound(); j++)
	{
	  glBegin(GL_LINE_STRIP); 
	  for( int i=I.getBase(); i<=I.getBound(); i++)
	    glVertex3f(x(i),t(j),uScaleFactor*u(i,j));
	  glEnd();
	}

	// glPopMatrix();
	glEndList();

// AP: What is this?
	gi.setView(GenericGraphicsInterface::xAxisAngle, xAngle);
	gi.setView(GenericGraphicsInterface::yAxisAngle, yAngle);
	gi.setView(GenericGraphicsInterface::zAxisAngle, zAngle);

	gi.redraw();
      }
      else
      {

	glDeleteLists(list,1);

        realArray u;
	u=u_*uScaleFactor/uDiff;
	
	// plot a surface
	real xa=min(x(I));
	real xb=max(x(I));
	real ta=min(t(N));
	real tb=max(t(N));

	SquareMapping square(xa,xb,ta,tb);
	square.setGridDimensions(axis1,n); 
	square.setGridDimensions(axis2,nv);
	MappedGrid mg(square);             
	mg.update();                       


	Range all;
	realMappedGridFunction uGF(mg,all,all,all);
	uGF=0.;
	Index I1,I2,I3;
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	uGF(I1,I2)=u(I,N);

	GraphicsParameters localParameters;
	GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

	psp.set(GI_PLOT_CONTOUR_LINES,FALSE);
//	psp.set(GI_PLOT_WIRE_FRAME,TRUE);

        gi.setView(GenericGraphicsInterface::xAxisAngle, xAngle);
        gi.setView(GenericGraphicsInterface::yAxisAngle, yAngle);
        gi.setView(GenericGraphicsInterface::zAxisAngle, zAngle);
/* ----
        aString ans="reset";
        processSpecialMenuItems(ans);
        ans="x-r";
        processSpecialMenuItems(ans);
        processSpecialMenuItems(ans);
--- */  
	contour(gi, uGF, psp);


      }
    }
  }
//    if( lighting )
//      glDisable(GL_COLOR_MATERIAL);

// AP: This is now taken care of in display()
//    windowScaleFactor[currentWindow][0]=1.;
//    windowScaleFactor[currentWindow][1]=1.;
//    windowScaleFactor[currentWindow][2]=1.;

  gi.unAppendTheDefaultPrompt();  // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  // this indicates that the object appears on the screen (not erased)
}



