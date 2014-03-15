#include "GL_GraphicsInterface.h"

#ifdef NO_APP
#define redim resize
#define getBound(x) size((x))-1
#define getLength size
using GUITypes::real;
#endif






//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{plotPoints}} 
void GL_GraphicsInterface::
plotPoints(const realArray & points, GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	   int dList /* = 0 */)
//----------------------------------------------------------------------
// /Description:
//   Plot points. Plot an array of points in 2D or 3D. 
// 
// /points (input) : an array of the form: points(0:n-1,0:r-1) where n is the number
//    of points and r is the range dimension. 
//
// /parameters (input/output): supply optional parameters to change
//    plotting characteristics.
// /Errors:  There are no known bugs...
// /Return Values: none.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
//
// plot points
//
{
#ifndef NO_APP
  plotPoints( points,Overture::nullRealDistributedArray(), parameters, dList );
#else
  plotPoints( points,Overture::nullRealArray(), parameters, dList );
#endif
}


// static void
// drawMark(real x,real y, real z, real dx, real dy )
// {
  
//   glBegin(GL_POLYGON);  // draw shaded filled polygons
//   glVertex3(x-dx,y-dy,z);
//   glVertex3(x   ,y-dy,z);
//   glVertex3(x   ,y   ,z);
//   glVertex3(x-dx,    ,z);
//   glEnd();   
// }



//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{plot points or spheres with individual colours (or radii)}} 
void GL_GraphicsInterface::
plotPoints(const realArray & points_, 
           const realArray & value,
           GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	   int dList /* = 0 */)
//----------------------------------------------------------------------
// /Description:
//   Plot points (or spheres) and colour each point a different colour based on the value array. 
// 
// /points (input) : an array of the form: points(0:n-1,0:r-1) where n is the number
//    of points and r is the range dimension. 
// /value (input) : an array of values, value(0:n-1,0), that will determine the colour 
//    (and optionally the sphere radius)
//    for each point (or sphere). The colour will be taken from a colour table with the colour table
//    value for point i based on the scaled quantity v(i) = (value(i,0)-min(value))/(max(value)-min(value)).
//
//    If the value array is dimensioned value(0:n-1,0:1) then value(i,1) will denote the radius of the sphere
//    that should be plotted with center given by the points array.
// 
// /parameters (input/output): supply optional parameters to change
//    plotting characteristics.
// /Errors:  Some...
// /Return Values: none.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  if( !graphicsIsOn() ) return;

  // this must be here for P++ (only 1 processor actually plots stuff)
#ifndef NO_APP
  #ifdef USE_PPP
    // copy the distributed points array to processor p=processorForGraphics
    Partitioning_Type partition;
    partition.SpecifyProcessorRange(Range(processorForGraphics,processorForGraphics));
    realArray points0;
    points0.partition(partition);
    points0.redim(points_.dimension(0),points_.dimension(1));

    points0=points_;
    const RealArray & points = points0.getLocalArray();

    if( Communication_Manager::localProcessNumber()!=processorForGraphics )
      return;
  #else
    const RealArray & points = points_;
  #endif
#else
  const RealArray & points = points_;
#endif
  
  int axis;
  aString answer, answer2;
  char buff[80];
  
  aString menu[] = {"!points plotter",
                    "plot",
                   "set colour",
                   "set size",
                   "set symbol",
                   " ",
                   "erase",
                   "erase and exit",
                   "exit",
                   "" };


  bool   plotObject               = parameters.plotObject;
  aString & pointColour            = parameters.pointColour;
  real & pointSize                = parameters.pointSize;
//  int & pointSymbol               = parameters.pointSymbol;
  bool & plotColourBar            = parameters.plotColourBar;

  real zLevelFor2DPoints = 1.e-3;
  real yLevelFor1DPoints=0.;
  
  if( parameters.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    plotObject=TRUE;
  }
  else
  {
    parameters.plotBoundsChanged=FALSE;
  }  

  int numberOfPoints=points.getLength(0);
  int rangeDimension=points.getLength(1);
  // assert( rangeDimension==2 || rangeDimension==3 );
#ifndef NO_APP
  Range I(points.getBase(0),points.getBound(0));
  Range xAxes(0,rangeDimension-1);
#endif

  // get Bounds on points
  RealArray xBound(2,3); xBound=0.;

  if( parameters.usePlotBounds )
    xBound=parameters.plotBound;
  else
  {
    for( axis=0; axis<rangeDimension; axis++ )
    {
     #ifndef NO_APP
      xBound(Start,axis)= min(points(I,axis));
      xBound(End,axis)  = max(points(I,axis));
     #else
      xBound(Start,axis) = xBound(End,axis) = points(0,axis);
      for (int ip=1; ip<points.getLength(0); ip++)
      {
       xBound(Start,axis) = min(xBound(Start,axis), points(ip,axis));
       xBound(End,axis)   = max(xBound(End,axis),   points(ip,axis));
      }
     #endif
    } 
    if( parameters.usePlotBoundsOrLarger )
    { // Use existing plot bounds unless the new bounds are larger by a certain factor
     #ifndef NO_APP
      real relativeBoundsChange = max(fabs(parameters.plotBound-xBound))
	                         /max(parameters.plotBound(End,xAxes)-parameters.plotBound(Start,xAxes));
     #else
      real maxSize = 0, maxBoundsDiff = 0;
      for (int q=0; q<3; q++)
      {
	maxSize = max(maxSize, parameters.plotBound(End,q)-parameters.plotBound(Start,q));
	for (int se=0; se<2; se++)
	  maxBoundsDiff = max(maxBoundsDiff, fabs(parameters.plotBound(se, q)-xBound(se, q)));
      }
      real relativeBoundsChange = maxBoundsDiff/maxSize;
     #endif

      if( relativeBoundsChange>parameters.relativeChangeForPlotBounds ) 
      {
	for( int axis=0; axis<3; axis++ )
	{
	  xBound(Start,axis)=min(xBound(Start,axis),parameters.plotBound(Start,axis));
	  xBound(End  ,axis)=max(xBound(End  ,axis),parameters.plotBound(End  ,axis));
	}
      }
    }
    if( !parameters.isDefault() )
    {
      parameters.plotBound=xBound;
      parameters.plotBoundsChanged=TRUE;
    }
  }

  real valueMin=0., valueMax=1., valueScale=1.;
  bool colourPointsByValue=false;
  bool plotSpheres = false;
  
 #ifdef NO_APP
  if(  value.size() && value.getBound(0)>=points.getBound(0) )
 #else
  if( value.getBase(0)<=points.getBase(0) && value.getBound(0)>=points.getBound(0) )
 #endif
  {
    colourPointsByValue=true;

#ifndef NO_APP
    plotSpheres=value.getBound(1)>0; // plot spheres if the second dimension is >1
#else
    plotSpheres=value.rank()>1; // plot spheres if the second dimension is >1
#endif
    
   #ifndef NO_APP    
    valueMin=min(value(I,0));
    valueMax=max(value(I,0));
   #else
    valueMin = valueMax = plotSpheres ? value(0,0) : value(0);
    for (int q=1; q<points.getLength(0); q++)
    {
      real v = plotSpheres ? value(q,0) : value(q);
      valueMin = min(valueMin, v);
      valueMax = max(valueMax, v);
    }
   #endif
    valueScale=valueMax-valueMin;
    if( valueScale!=0. )
      valueScale=1./valueScale;
  }
  
  int list=0;

  // set default prompt
  appendToTheDefaultPrompt("plotPoints>");

  // **** Plotting loop *****
  for(int i=0;;i++)
  {
    if( i==0 && (plotObject || parameters.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( i==1 && parameters.plotObjectAndExit )
      answer="exit";
    else
      getMenuItem(menu,answer);

    if( answer=="erase" )
    {
      plotObject=FALSE;
      eraseLabels(parameters);
      erase();
      // redraw(true);
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
      eraseLabels(parameters);
      erase();
      // redraw();
      break;
    }
    else if( answer=="set colour" )
    {
      answer2 = chooseAColour();
      if( answer2!="no change" )
        pointColour = answer2;
    }
    else if( answer=="set size" )
    {
      inputString(answer2,sPrintF(buff,"Enter the size of the points (in pixels) (default=%e)",pointSize));
      if( answer2!="" )
        sScanF(answer2,"%e",&pointSize);
    }
    else if( answer=="set symbol" )
    {
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else
    {
      char buff[100];
      outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer.c_str()) );
    }
    if( plotObject && isGraphicsWindowOpen() )
    {

      if( dList == 0 )
      {
        if( list==0 )
	{
	  if( !plotSpheres )
	    list=generateNewDisplayList();  // get a new display list to use
	  else
	    list=generateNewDisplayList(1);  // get a display list with lighting
	}
      }
      else
	list = dList;

      assert(list!=0);

      setAxesDimension(rangeDimension);
      
      // plot labels on top and bottom
      if( parameters.plotTitleLabels )
      {
        plotLabels( parameters );
      }
      
      // AP: only start a new list it it was generated locally
      if (dList == 0)
      {
        glNewList(list,GL_COMPILE);
      }
      
      setGlobalBound(xBound);
      
      real maxBoundDiff = 0;
      for (int q=0; q<3; q++)
	maxBoundDiff = max(maxBoundDiff, xBound(End,q) - xBound(Start,q));
      // real eps=.002*maxBoundDiff;

      // *** plot points ****
      #ifdef NO_APP
      const int base=0, bound=points.getBound(0); 
      #else
      const int base=points.getBase(0), bound=points.getBound(0);  
      #endif

      glPointSize(pointSize*lineWidthScaleFactor[currentWindow]);   
      setColour(pointColour);
      if( !plotSpheres ) glBegin(GL_POINTS);  
      if( rangeDimension==2 )
      {
	if( colourPointsByValue )
	{
	  for( int i=base; i<=bound; i++ )
	  {
	    setColourFromTable( (value(i,0)-valueMin)*valueScale,parameters );
	    glVertex3(points(i,0),points(i,1),zLevelFor2DPoints);
	  }
	}
	else
	{
	  for( int i=base; i<=bound; i++ )
	    glVertex3(points(i,0),points(i,1),zLevelFor2DPoints);
	}
      }
      else if( rangeDimension==3 )
      {
        if( !plotSpheres )
	{
	  if( colourPointsByValue )
	  {
	    for( int i=base; i<=bound; i++ )
	    {
	      // real val = (value(i,0)-valueMin)*valueScale;
	      // printf(" Point i=%i vale=%5.3f \n",i,val);
#ifdef NO_APP	      
	      real v = value(i);
	      setColourFromTable( (v-valueMin)*valueScale,parameters );
#else
	      setColourFromTable( (value(i,0)-valueMin)*valueScale,parameters );
#endif
	      glVertex3(points(i,0),points(i,1),points(i,2));
	    }
	  }
	  else
	  {
	    for( int i=base; i<=bound; i++ )
	      glVertex3(points(i,0),points(i,1),points(i,2));
	  }
	}
	else
	{
          // **** plot spheres ****
          GLUquadricObj *sphere; 
	  sphere =  gluNewQuadric();
          gluQuadricDrawStyle(sphere, GLU_FILL );
	  gluQuadricNormals(sphere, GLU_SMOOTH );
	  
          const GLint slices=15, stacks=10;
	  for( int i=base; i<=bound; i++ )
	  {
	    GLdouble radius=value(i,1);

            // real val = (value(i,0)-valueMin)*valueScale;
	    // printf(" Sphere i=%i vale=%5.3f \n",i,val);

	    setColourFromTable( (value(i,0)-valueMin)*valueScale,parameters );

	    glTranslate(points(i,0),points(i,1),points(i,2));
	    gluSphere( sphere, radius, slices, stacks );
            glTranslate(-points(i,0),-points(i,1),-points(i,2));
	  }

          gluDeleteQuadric(sphere);
	}
      }
      else
      {
        assert( rangeDimension==1 );
        if( colourPointsByValue )
	{
          for( int i=base; i<=bound; i++ )
	  {
            setColourFromTable( (value(i,0)-valueMin)*valueScale,parameters );
	    glVertex3(points(i,0),yLevelFor1DPoints,zLevelFor2DPoints);
	  }
	}
	else
	{
          for( int i=base; i<=bound; i++ )
	    glVertex3(points(i,0),yLevelFor1DPoints,zLevelFor2DPoints);
	}
      }
      if( !plotSpheres ) glEnd();

      // glPopMatrix();
      if (dList == 0)
	glEndList(); 

// ----------Draw the colour Bar----------------- (can't include it when dList is beeing passed in,
// since drawColourBar puts the colour bar in a different list and it is illegal to open a new display
//  list until the existing one has been closed
      if( dList == 0 && colourPointsByValue && plotColourBar )
      {
        const int numberOfColourBarLevels=11;
        drawColourBar(numberOfColourBarLevels,Overture::nullRealArray(),valueMin,valueMax,parameters);
      }

      redraw();
    }
  }
  unAppendTheDefaultPrompt(); // reset defaultPrompt
}

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{plotLines}} 
void GL_GraphicsInterface::
plotLines(const realArray & arrows, 
	  GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	  int dList /*= 0*/)
//----------------------------------------------------------------------
// /Description:
//   Plot line segments
// /arrows(input): array holding the coordinates of the start and end points for each line segment. 
// It should be dimensioned array(0:Npoints-1, 0:rangeDimension-1, 0:1), where the last index is
// 0 for the start point and 1 for the end point.
// /parameters(input): Graphics parameters controlling the plot.
// /dList(optional input): If provided, put the drawing command in this display list.
//
//  /Author: WDH
//\end{GL_GraphicsInterfaceInclude.tex} 
//----------------------------------------------------------------------
{
  // this must be here for P++ (only 1 processor actually plots stuff)
#ifndef NO_APP
  if( Communication_Manager::localProcessNumber()!=processorForGraphics )
    return;
#endif
  
  if( !graphicsWindowIsOpen )
    return;
  
  int axis, i;
  aString answer, answer2;
  // char buff[80];
  
  int list;
  if (dList == 0)
    list=generateNewDisplayList();  // get a new display list to use
  else
    list = dList;
  
  assert(list!=0);
  // cout << "grid: Generated list = " << list << endl;

  aString & lineColour = parameters.lineColour;

  real zLevelFor2DPoints = 1.e-3;
  
  if( !parameters.isDefault() )
  {
    parameters.plotBoundsChanged=FALSE;
  }  

  int numberOfPoints=arrows.getLength(0);
  int rangeDimension = arrows.getLength(1);

#ifndef NO_APP
  Range I(arrows.getBase(0), arrows.getBound(0));
  Range xAxes(0,rangeDimension-1);
  Range startEnd(0,1);
#endif

// get Bounds on arrows
  RealArray xBound(2,3); xBound=0.;

  if( parameters.usePlotBounds )
    xBound=parameters.plotBound;
  else
  {
    for( axis=0; axis<rangeDimension; axis++ )
    {
#ifndef NO_APP
      xBound(Start,axis)= min(arrows(I,axis,startEnd));
      xBound(End,axis)  = max(arrows(I,axis,startEnd));
#else
      xBound(Start,axis) = xBound(End,axis) = arrows(0,axis,0);
      for (int q=0; q<arrows.getLength(0); q++)
      {
	for (int se=0; se<2; se++)
	{
	  xBound(Start,axis)= min(xBound(Start,axis), arrows(q,axis,se));
	  xBound(End,axis)  = max(xBound(End,axis),   arrows(q,axis,se));
	}
      }
#endif
    } 
    if( parameters.usePlotBoundsOrLarger )
    { // Use existing plot bounds unless the new bounds are larger by a certain factor
#ifndef NO_APP
      real relativeBoundsChange = max(fabs(parameters.plotBound-xBound))
	                         /max(parameters.plotBound(End,xAxes)-parameters.plotBound(Start,xAxes));
#else
      real maxSize = 0, maxBoundsDiff = 0;
      for (int q=0; q<3; q++)
      {
	maxSize = max(maxSize, parameters.plotBound(End,q)-parameters.plotBound(Start,q));
	for (int se=0; se<2; se++)
	  maxBoundsDiff = max(maxBoundsDiff, fabs(parameters.plotBound(se, q)-xBound(se, q)));
      }
      real relativeBoundsChange = maxBoundsDiff/maxSize;
#endif

      if( relativeBoundsChange>parameters.relativeChangeForPlotBounds ) 
      {
	for( int axis=0; axis<3; axis++ )
	{
	  xBound(Start,axis)=min(xBound(Start,axis),parameters.plotBound(Start,axis));
	  xBound(End  ,axis)=max(xBound(End  ,axis),parameters.plotBound(End  ,axis));
	}
      }
    }
    if( !parameters.isDefault() )
    {
      parameters.plotBound=xBound;
      parameters.plotBoundsChanged=TRUE;
    }
  }

// **** Plotting lines *****
  setAxesDimension(rangeDimension);
      
  // plot labels on top and bottom
  if( parameters.plotTitleLabels )
  {
    plotLabels( parameters );
  }
      
  if (dList == 0)
    glNewList(list,GL_COMPILE);

  setGlobalBound(xBound);
      
#ifndef NO_APP
  real eps=.002*max(xBound(End,Range(0,2))-xBound(Start,Range(0,2)));
#else
  real maxBoundDiff = 0;
  for (int q=0; q<3; q++)
    maxBoundDiff = max(maxBoundDiff, xBound(End,q) - xBound(Start,q));
  
  real eps=.002*maxBoundDiff;
#endif

// plot lines
  real cw;
  parameters.get(GraphicsParameters::curveLineWidth, cw);
  glLineWidth(cw * lineWidthScaleFactor[currentWindow]);
  setColour(lineColour);

  glBegin(GL_LINES);  
  if( rangeDimension==2 )
  {
#ifdef NO_APP
    for( i=0; i<=arrows.getBound(0); i++ )
#else
    for( i=arrows.getBase(0); i<=arrows.getBound(0); i++ )
#endif
    {
      glVertex3(arrows(i,0,0),arrows(i,1,0),zLevelFor2DPoints);
      glVertex3(arrows(i,0,1),arrows(i,1,1),zLevelFor2DPoints);
    }
  }
  else if( rangeDimension==3 )
  {
#ifdef NO_APP
    for( i=0; i<=arrows.getBound(0); i++ )
#else
    for( i=arrows.getBase(0); i<=arrows.getBound(0); i++ )
#endif
    {
      glVertex3(arrows(i,0,0),arrows(i,1,0),arrows(i,2,0));
      glVertex3(arrows(i,0,1),arrows(i,1,1),arrows(i,2,1));
    }
  }
  glEnd();

  if (dList == 0)
    glEndList(); 

  redraw();
  
}

#ifdef USE_PPP
#include "ParallelUtility.h"

// 
// Version to use in parallel that take serial arrays as input and form the aggregate
// 

void GL_GraphicsInterface::
plotPoints(const RealArray & points, GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	   int dList /* = 0 */ )
// ==================================================================================================
// /Description:
//    Plot points defined in serial arrays on different processors. 
//    In parallel we need to collect up the points on all processors 
// ==================================================================================================
{

  int numPoints = ParallelUtility::getSum(points.getLength(0));
  // This is needed in case there are no points on some processor: 
  const int numberOfDimensions = ParallelUtility::getMaxValue(points.getLength(1)); 
  // printF("plotPoints: numPoints=%i, numberOfDimensions=%i\n",numPoints,numberOfDimensions);
  // ::display(points,"plotPoints: points (input serial array)");

  int p0=getProcessorForGraphics();  // copy results to this processor
  Partitioning_Type partition;
  partition.SpecifyProcessorRange(Range(p0,p0));


  realArray pts;
  pts.partition(partition);
  pts.redim(numPoints,numberOfDimensions);
  realSerialArray ptsLocal; getLocalArrayWithGhostBoundaries(pts,ptsLocal);
    
  Index Iv[2];
  Iv[0]=points.dimension(0);
  Iv[1]=Range(numberOfDimensions);
  CopyArray::getAggregateArray( (RealArray &)points, Iv, ptsLocal, p0);  // results go into ptsLocal

  // ::display(pts,"plotPoints: pts (aggregate)");
  
  plotPoints(pts,parameters,dList);

  return;
}
 
// --- serial version ----
void GL_GraphicsInterface::
plotPoints(const RealArray & points, 
           const RealArray & value,
           GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	   int dList /* = 0 */)
// ==================================================================================================
// /Description:
//    Plot points defined in serial arrays on different processors. 
//    In parallel we need to collect up the points on all processors 
// ==================================================================================================
{

  int numPoints = ParallelUtility::getSum(points.getLength(0));

  // printF("plotPoints: numPoints=%i\n",numPoints);

  int p0=getProcessorForGraphics();  // copy results to this processor
  Partitioning_Type partition;
  partition.SpecifyProcessorRange(Range(p0,p0));

  const int numberOfDimensions = ParallelUtility::getMaxValue(points.getLength(1)); 

  realArray pts;
  pts.partition(partition);
  pts.redim(numPoints,numberOfDimensions);
  realSerialArray ptsLocal; getLocalArrayWithGhostBoundaries(pts,ptsLocal);
    
  Index Iv[2];
  Iv[0]=points.dimension(0);
  Iv[1]=Range(numberOfDimensions);
  CopyArray::getAggregateArray( (RealArray &)points, Iv, ptsLocal, p0);  // results go into ptsLocal

  realArray val;
  val.partition(partition);
  val.redim(numPoints);
  realSerialArray valLocal; getLocalArrayWithGhostBoundaries(val,valLocal);
    
  Iv[0]=value.dimension(0);
  Iv[1]=1;
  CopyArray::getAggregateArray( (RealArray &)value, Iv, valLocal, p0);  // results go into valLocal

  // ::display(pts,"plotPoints: pts");
  
  plotPoints(pts,val,parameters);

}


#endif
