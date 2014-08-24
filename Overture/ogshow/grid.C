#include "GL_GraphicsInterface.h"
#include "conversion.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "CompositeGrid.h"
#include "PlotIt.h"
#include "xColours.h"
#include "Regrid.h"
#include "GridStatistics.h"
#include "MappingInformation.h"
#include "Ogen.h"
#include "DataPointMapping.h"

// add to class: 
static real boundaryLineWidth=3.;  // scale factor for drawing the lines on the boundary
static real boundaryVerticalOffset=0.;  // offset boundary in zed direction by this amount



// This is a local version that we can set to 0 if we do not want to plot interior boundary points
static int ISinteriorBoundaryPoint=0;

// // here is a computational point but not a refined point
// #define MASK_CNR(i1,i2,i3) (mask(i1,i2,i3) && !(mask(i1,i2,i3) & MappedGrid::IShiddenByRefinement))
// // here is a discretization point but not a refined point
// #define MASK_DNR(i1,i2,i3) (mask(i1,i2,i3)>0 && !(mask(i1,i2,i3) & MappedGrid::IShiddenByRefinement))
// // here is a discretization point
// #define MASK_D(i1,i2,i3) (mask(i1,i2,i3)>0)
// // here is a discretization point that is not an interior boundary point
#define MASK_DNIB(i1,i2,i3) (mask(i1,i2,i3)>0 && !(mask(i1,i2,i3) & ISinteriorBoundaryPoint) )
// here is a discretization point or interpolation point that is not an interior boundary point
#define MASK_DINIB(i1,i2,i3) (mask(i1,i2,i3)!=0 && !(mask(i1,i2,i3) & ISinteriorBoundaryPoint) )


void setXColour( const aString & xColourName );

#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

static inline int 
decode(int mask )
// decode the mask ->  1=interior, 2=ghost, -2,3=interiorBoundaryPoint,  4=hidden by refinement, <0 =interp 
{
  int m=0;
  if( (mask & MappedGrid::ISdiscretizationPoint) && (mask & MappedGrid::ISinteriorBoundaryPoint) )
    m=3;
  else if( mask & MappedGrid::ISdiscretizationPoint )
    m=1;
  if( mask<0 && mask>-100 )
    m=mask;
  else if( mask<0 )
    m=-1;
  else if( mask & MappedGrid::ISghostPoint )
    m=2;

  if( mask<0 && (mask & MappedGrid::ISinteriorBoundaryPoint) )
    m=-2;
  if( mask & MappedGrid::IShiddenByRefinement )
    m=4;

  return m;
}


int PlotIt::
plotParallelGridDistribution(GridCollection & cg, GenericGraphicsInterface & gi, GraphicsParameters & par )
// ================================================================================================
// /Description:
//    Plot the parallel distribution for a grid by plotting a contour plot of a grid function
//  where the value of the grid function is equal to the processor number on which it lives. 
// ================================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  realGridCollectionFunction pd(cg);
  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.dimension(),I1,I2,I3);

#ifdef USE_PPP
    realSerialArray pdLocal; getLocalArrayWithGhostBoundaries(pd[grid],pdLocal);
    bool ok=ParallelUtility::getLocalArrayBounds(pd[grid],pdLocal,I1,I2,I3);
    if( !ok ) continue;
#else
    realSerialArray & pdLocal = pd[grid];
#endif
    pdLocal=myid;
  }
	      
//  ps.erase();
//  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
//  psp.set(GI_TOP_LABEL,sPrintF(buff,"Parallel distribution at t=%8.2e",t));
  PlotIt::contour(gi,pd,par);
//  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  return 0;
}


int PlotIt::
buildColourDialog(DialogData & dialog)
// =================================================================
// Build a sibling dialog that can be used to choose a colour
// ==================================================================
{
  const aString prefix="PIC:";

  const aString *colourNames = getAllColourNames();
  const int maximumNumberOfColours=110;
  aString colourCommand[maximumNumberOfColours];
  GUIState::addPrefix(colourNames,prefix,colourCommand,maximumNumberOfColours);

  int blue=4;
  int numberOfColumns=5;
  dialog.addRadioBox("Colour choices:", colourCommand,colourNames,blue,numberOfColumns);


  return 0;
}

bool PlotIt::
getColour( const aString & answer_, DialogData & dialog, aString & colour )
// ====================================================================================
//  Look for answers that choose a colour from the sibling dialog built with buildColourDialog
// Return value: true if an answer was processed, false if no answer processed.
// ===================================================================================
{
  const aString prefix="PIC:";
  aString answer=answer_;
  // take off the prefix
  // printF("PlotIt::getColour answer=[%s] [%s]\n",(const char*)answer,(const char*)answer(0,prefix.length()-1));
  
  if( answer(0,prefix.length()-1)==prefix )
    answer=answer(prefix.length(),answer.length()-1);
  else
  {
    return false;
  }
  const int maximumNumberOfColours=110;
  const aString *colourNames = getAllColourNames();
  colour="";
  for( int i=0; i<maximumNumberOfColours; i++ )
  {
    if( colourNames[i]=="" ) break;
    if( colourNames[i]==answer )
    {
      colour=answer;
      break;
    }
  }
  if( colour=="" )
  {
    printF("PlotIt::getColour:ERROR: unknown colour: [%s]. Choosing `blue' instead.\n",(const char*)answer);
    colour="blue";
  }
  return true;
}



aString PlotIt::
getGridColour( int item, int side, int axis, 
               int grid, const GridCollection & gc, GenericGraphicsInterface & gi, GraphicsParameters & par )
// ====================================================================================
//  /Description:
//     Return the colour of boundaries(item==0), grid lines (item==1), or block boundaries(item==2) 
// /item (input) : 0,1,or 2 -- return the colour for boundaries, grid lines or block boundaries.
// /grid (input) : the grid number
// /par (input) : current graphics parameters.
// /Return value: the name of a colour.
// ====================================================================================
{
  int option = item==0 ? par.boundaryColourOption : item==1 ? par.gridLineColourOption : par.blockBoundaryColourOption;

  aString col;
  if( option==GraphicsParameters::colourByGrid ||
      ( (item==0 || item==2 || gc.numberOfDimensions()<=2 ) && option==GraphicsParameters::defaultColour) )
  {
    col=gi.getColourName((grid % GenericGraphicsInterface::numberOfColourNames));
  }
  else if( option==GraphicsParameters::colourByRefinementLevel )
  {
    col=gi.getColourName((gc.refinementLevelNumber(grid) % GenericGraphicsInterface::numberOfColourNames) );
  }
  else if( option==GraphicsParameters::colourBlack || (item==1 && option==GraphicsParameters::defaultColour) )
  {
    col=gi.getColour(GenericGraphicsInterface::textColour); 
  }
  else if( option==GraphicsParameters::colourByIndex )
  {
    int index = item==0 ? side+2*axis : item==1 ? 6 : 7;
    col=getXColour(par.gridColours(grid,index));
  }
  else if( option==GraphicsParameters::colourByValue )
  {
    int value=item==0 ? par.boundaryColourValue : item==1 ? par.gridLineColourValue : par.blockBoundaryColourValue;
    col=gi.getColourName( (max(0,value) % GenericGraphicsInterface::numberOfColourNames) );
  }
  else if( option==GraphicsParameters::colourByDomain )
  {
    col=gi.getColourName((gc.domainNumber(grid) % GenericGraphicsInterface::numberOfColourNames) );
  }
  else
  {
    col=gi.getColour(GenericGraphicsInterface::textColour);
  }
  
  return col;
}


void PlotIt::
getGridBounds(const GridCollection & gc, GraphicsParameters & params, RealArray & xBound)
// =================================================================================================
// /Description:
//    Return the bounds on the grid. 
// =================================================================================================
{
  const int numberOfGrids = gc.numberOfComponentGrids();
  // get Bounds on the grids that we plot
  xBound=0.;
  Index I[3];
  Index & I1 = I[0];
  Index & I2 = I[1];
  Index & I3 = I[2];
  for( int grid=0; grid<numberOfGrids; grid++ )
  {
    if( gc[grid].getGridType()==MappedGrid::unstructuredGrid )
    {
      const MappingRC & map = gc[grid].mapping();
      
      Bound b;
      for( int axis=0; axis<map.getRangeDimension(); axis++ )
      {
	b = map.getRangeBound(Start,axis);
	if( b.isFinite() )
	  xBound(Start,axis)=grid==0 ? (real)b : min( (real)b, xBound(Start,axis));
        // printF("getGridBounds: unstructured grid: min(x%i)=%e",axis,(real)b);
	b = map.getRangeBound(End,axis);
        // printF(" max(x%i)=%e \n",axis,(real)b);
	if( b.isFinite() )
	  xBound(End,axis)=grid==0 ? (real)b : max( (real)b, xBound(End,axis));
      }
      // xBound.display("GL_GraphicsInterface::xBound for unstructured");
      
    }
    else
    {
      // in P++ we assume that the grid functions only live on one processor
      MappedGrid & mg = (MappedGrid &)gc[grid];
      mg.update( MappedGrid::THEmask );

      const IntegerArray & mask = gc[grid].mask().getLocalArray();
      getIndex(gc[grid].gridIndexRange(),I1,I2,I3,params.numberOfGhostLinesToPlot);

      // *wdh* 061105 -- limit bounds (when large numbers of ghost lines are plotted)
      for( int dir=0; dir<gc.numberOfDimensions(); dir++ )
	I[dir]=Range(max(I[dir].getBase(),mg.dimension(0,dir)),min(I[dir].getBound(),mg.dimension(1,dir)));

       // *wdh* 020406  : do this so we don't need the vertex array for rectangular grids.
      // -- could also check mask to reduce size.
      if( gc[grid].isRectangular() )
      {
	real dx[3],xab[2][3];
	gc[grid].getRectangularGridParameters( dx, xab );
// 	printf(" getGridBounds: rectangular: xab=%e,%e,%e,%e ghost=%i\n",xab[0][0],xab[1][0],xab[0][1],xab[1][1],
//            params.numberOfGhostLinesToPlot );
	
        int axis;
	for( axis=0; axis<gc.numberOfDimensions(); axis++ )
	{
	  real xmin=xab[0][axis]-dx[axis]*params.numberOfGhostLinesToPlot;
	  real xmax=xab[1][axis]+dx[axis]*params.numberOfGhostLinesToPlot;

	  xBound(0,axis)=grid==0 ? xmin : min(xBound(0,axis),xmin);
	  xBound(1,axis)=grid==0 ? xmax : max(xBound(1,axis),xmax);
	}
      }
      else
      {
	// non-rectagular 
        MappedGrid & mg = (MappedGrid &)gc[grid];
	
        mg.update( MappedGrid::THEvertex | MappedGrid::THEmask );
	
	const RealArray & vertex = gc[grid].vertex().getLocalArray();

	if( int(gc[grid].isAllVertexCentered()))
	{

// 	  I1=Range(max(I1.getBase(),gc[grid].dimension(Start,0)),min(I1.getBound(),gc[grid].dimension(End,0)));
// 	  I2=Range(max(I2.getBase(),gc[grid].dimension(Start,1)),min(I2.getBound(),gc[grid].dimension(End,1)));
// 	  I3=Range(max(I3.getBase(),gc[grid].dimension(Start,2)),min(I3.getBound(),gc[grid].dimension(End,2)));

	  const int includeGhost=0;
	  bool ok=ParallelUtility::getLocalArrayBounds(gc[grid].vertex(),vertex,I1,I2,I3,includeGhost);
	  if( ok )
	  {
	    where( mask(I1,I2,I3)!=0 )
	    {
	      for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	      {
		xBound(Start,axis)= grid==0 ? min(vertex(I1,I2,I3,axis))
		  : min(min(vertex(I1,I2,I3,axis)),xBound(Start,axis));
		xBound(End,axis)= grid==0 ? max(vertex(I1,I2,I3,axis))
		  : max(max(vertex(I1,I2,I3,axis)),xBound(End,axis));
	      } 
	    }
	  }
	  
	}
	else // cell centre
	{

	  Index J[3];
	  Index & J1 = J[0];
	  Index & J2 = J[1];
	  Index & J3 = J[2];
	  for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	  {
	    int is1 = axis==0 ? 1 : 0; 
	    int is2 = axis==1 ? 1 : 0; 
	    int is3 = axis==2 ? 1 : 0; 
	    J1=Range(max(I1.getBase(),gc[grid].dimension(Start,0)+is1),min(I1.getBound(),gc[grid].dimension(End,0)));
	    J2=Range(max(I2.getBase(),gc[grid].dimension(Start,1)+is2),min(I2.getBound(),gc[grid].dimension(End,1)));
	    J3=Range(max(I3.getBase(),gc[grid].dimension(Start,2)+is3),min(I3.getBound(),gc[grid].dimension(End,2)));

	    const int includeGhost=0;
	    bool ok=ParallelUtility::getLocalArrayBounds(gc[grid].vertex(),vertex,J1,J2,J3,includeGhost);
	    if( ok )
	    {

	      where( mask(J1,J2,J3)!=0 || mask(J1-is1,J2-is2,J3-is3)!=0)
	      {
		xBound(Start,axis)=grid==0 ? min(vertex(J1,J2,J3,axis)) 
		  : min(min(vertex(J1,J2,J3,axis)),xBound(Start,axis));
		xBound(End,axis)=grid==0 ? max(vertex(J1,J2,J3,axis)) 
		  : max(max(vertex(J1,J2,J3,axis)),xBound(End,axis));
	      } 
	      if( I[axis].getBase()<=gc[grid].dimension()(Start,axis) )
	      { // we missed the left end
		J[axis]=gc[grid].dimension()(Start,axis);
		where( mask(J1,J2,J3)!=0 )
		{
		  xBound(Start,axis)= min(min(vertex(J1,J2,J3,axis)),xBound(Start,axis));
		  xBound(End,axis)=   max(max(vertex(J1,J2,J3,axis)),xBound(End,axis));
		} 
	  
	      }
	    }
	  }
	}
      }
    }
  }
  
  for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
  {
    // A null grid is set to bounds [0,1]
    if( xBound(Start,axis)>xBound(End,axis) )
    {
      xBound(Start,axis)=0.;
      xBound(End,axis)=1.;
    }

    if( PlotIt::parallelPlottingOption==1 )
    {
      xBound(Start,axis)=ParallelUtility::getMinValue(xBound(Start,axis));
      xBound(End  ,axis)=ParallelUtility::getMaxValue(xBound(End  ,axis));
    }
    
  }
}
//----------------------------------------------------------------------------------------
//  Determine plotting bounds -- 
//---------------------------------------------------------------------------------------
void PlotIt::
getPlotBounds(const GridCollection & gc, GraphicsParameters & params, RealArray & xBound)
{
  if( params.usePlotBounds )
    xBound=params.plotBound;
  else
  {
    getGridBounds(gc,params,xBound);
    if( params.usePlotBoundsOrLarger )
    {
      for( int axis=0; axis<3; axis++ )
      {
	xBound(Start,axis)=min(xBound(Start,axis),params.plotBound(Start,axis));
	xBound(End  ,axis)=max(xBound(End  ,axis),params.plotBound(End  ,axis));
      }
    }
    params.plotBound=xBound;
  }
}  

void PlotIt:: 
plotGridBoundaries(GenericGraphicsInterface &gi, const GridCollection & gc,
		   IntegerArray & boundaryConditionList, int numberOfBoundaryConditions,
		   const int colourOption,     
		   const real zRaise,
                   GraphicsParameters & parameters)
//----------------------------------------------------------------------------------------
//  Plot the boundaries of a grid (in 2D)
//
//  colourOption = 0 : plot grids boundaries as black
//                 1 : plot grids boundaries according to parameters
//  zRaise : raise the grid lies by this amount in the zed direction
//----------------------------------------------------------------------------------------
{
  if( gc.numberOfDimensions()!=2 )
    return;

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int graphicsProcessor = gi.getProcessorForGraphics();
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();


  int side,axis;
  Index I1,I2,I3;
  int i1,i2,i3;

  const int numberOfGrids = parameters.plotRefinementGrids ? gc.numberOfComponentGrids() : gc.numberOfBaseGrids();
  
#ifdef USE_PPP
  // build a partition for arrays that just lives on the graphicsProcessor.
  Partitioning_Type partition; 
  partition.SpecifyProcessorRange(Range(graphicsProcessor,graphicsProcessor)); 
  for( int axis=0; axis<4; axis++ )
  {
    int ghost=0; // uPartition.getGhostBoundaryWidth(axis);
    if( ghost>0 )
      partition.partitionAlongAxis(axis, true , ghost);
    else
      partition.partitionAlongAxis(axis, false, 0);
  }
#endif


  // --- Now plot the boundaries. Base the colour on the boundary condition, the share value or the grid number ---

  int grid;
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    if( !(parameters.gridsToPlot(grid) & GraphicsParameters::toggleGrids) )
      continue;

    // *************** Loop over processors ******************
    for( int p=0; p<np; p++ )
    {

      if( gc[grid].getGridType()==MappedGrid::unstructuredGrid )
      {
      }
      else
      {
	if( parameters.gridOptions(grid) & GraphicsParameters::plotInteriorBoundary )
	  ISinteriorBoundaryPoint=MappedGrid::ISinteriorBoundaryPoint;
	else
	  ISinteriorBoundaryPoint=0; 
      

	// use the vertex array if it is there (for plotting the displacement)
	const bool plotRectangular = gc[grid].isRectangular() && !(gc[grid]->computedGeometry & MappedGrid::THEvertex);
        #ifdef USE_PPP

         intSerialArray mask; 
         realSerialArray vertex; 

         realArray vertexd; // distributed version that lives on the graphicsProcessor
	 intArray maskd;

	 if( p==graphicsProcessor )
	 {
           getLocalArrayWithGhostBoundaries(gc[grid].mask(),mask);
           if( !plotRectangular ) getLocalArrayWithGhostBoundaries(gc[grid].vertex(),vertex);
	 }
	 else
	 {
           // copy data from processor p to the graphicsProcessor
           
           IndexBox pBox;
           const int nd=4;
           Index Jv[nd];

	   // CopyArray::getLocalArrayBoxWithGhost( p, u, pBox ); // get local bounds of the array on processor p 
	   CopyArray::getLocalArrayBox( p, gc[grid].mask(), pBox ); // get local bounds of the array on processor p 

           if( pBox.isEmpty() ) continue;
	   
	   for( int d=0; d<3; d++ )	     
	   {
	     int ja=pBox.base(d), jb=pBox.bound(d);
	     // copy an extra line on internal ghost boundaries to avoid a gap
	     // if( ja>gridIndexRange(0,d) ) ja--;
	     if( jb<gc[grid].gridIndexRange(1,d) ) jb++;
	     Jv[d]=Range(ja,jb);
	   }

	   if( !plotRectangular )
	   {
	     Jv[3]=Range(0,gc.numberOfDimensions()-1); // copy (x,y,z)
	     vertexd.partition(partition);
	     vertexd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
	     ParallelUtility::copy(vertexd,Jv,gc[grid].vertex(),Jv,nd); // copy data from processor p to graphics processor
	     getLocalArrayWithGhostBoundaries(vertexd,vertex);
	   }
           
	   Jv[3]=Range(0,0);
	   maskd.partition(partition);
	   maskd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
	   ParallelUtility::copy(maskd,Jv,gc[grid].mask(),Jv,nd); // copy data from processor p to graphics processor
	   getLocalArrayWithGhostBoundaries(maskd,mask);


	 }

        #else
          const intSerialArray & mask = gc[grid].mask();
          const realSerialArray & vertex = plotRectangular ? Overture::nullRealDistributedArray() : 
                                              (RealDistributedArray &)gc[grid].vertex();
        #endif
        if( plotOnThisProcessor )
        {
	  
	  ForBoundary(side,axis)
	  {
	    int thisBC=gc[grid].boundaryCondition(side,axis);
	    int bcIndex =bcNumber(thisBC, boundaryConditionList, numberOfBoundaryConditions);
	
	    //printf("plotGridBoundaries: grid=%i side,axis=%i,%i thisBC=%i bcIndex=%i\n",
	    //       grid,side,axis,thisBC,bcIndex);

	    if( (thisBC>0 && parameters.gridBoundaryConditionOptions(bcIndex) & 1) || 
		(parameters.plotNonPhysicalBoundaries && gc[grid].boundaryCondition(side,axis)==0) ||
		(parameters.plotBranchCuts            && gc[grid].boundaryCondition(side,axis)<0) )
	    {
	      glLineWidth(parameters.size(GraphicsParameters::lineWidth)*boundaryLineWidth*
			  gi.getLineWidthScaleFactor());  // make lines wider so we can see them
	      if( colourOption==0 || parameters.boundaryColourOption==GraphicsParameters::colourBlack )
	      {
		gi.setColour(GenericGraphicsInterface::textColour); 
		glLineWidth(parameters.size(GraphicsParameters::lineWidth)*boundaryLineWidth*
			    gi.getLineWidthScaleFactor());      // boundaries are twice as thick as other lines
	      }
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition || 
		       parameters.boundaryColourOption==GraphicsParameters::defaultColour )
		setXColour( gi.getColourName( min(max(0,gc[grid].boundaryCondition(side,axis)),
						  GenericGraphicsInterface::numberOfColourNames-1)) );
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByShare )
		setXColour( gi.getColourName( min(gc[grid].sharedBoundaryFlag(side,axis),
						  GenericGraphicsInterface::numberOfColourNames-1)) );
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByGrid )
		setXColour( gi.getColourName( min(grid,GenericGraphicsInterface::numberOfColourNames-1)) );
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByRefinementLevel )
		setXColour( gi.getColourName( min(gc.refinementLevelNumber(grid),
						  GenericGraphicsInterface::numberOfColourNames-1)) );
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByDomain )
		setXColour( gi.getColourName( min(gc.domainNumber(grid),
						  GenericGraphicsInterface::numberOfColourNames-1)) );
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByValue )
		setXColour( gi.getColourName( min(max(0,parameters.boundaryColourValue),
						  GenericGraphicsInterface::numberOfColourNames-1)) );
	      else if( parameters.boundaryColourOption==GraphicsParameters::colourByIndex )
	      {
		int item=0;  // boundary
		int index = item==0 ? side+2*axis : item==1 ? 6 : 7;
		setXColour( getXColour(parameters.gridColours(grid,index)) );
	      }	 
	      else
	      {
		printF("GL_GraphicsInterface::plotGridBoundaries:ERROR: unknown value of parameters.boundaryColourOption \n");
		setXColour( gi.getColourName( min(grid,GenericGraphicsInterface::numberOfColourNames-1)) );
	      }
	
	      int is1 = axis==axis1 ? 0 : 1;
	      int is2 = axis==axis2 ? 0 : 1;
	      if( parameters.plotNonPhysicalBoundaries && gc.refinementLevelNumber(grid)>0 )
		getBoundaryIndex(gc[grid].gridIndexRange(),side,axis,I1,I2,I3);
	      else
		getBoundaryIndex(extendedGridIndexRange(gc[grid]),side,axis,I1,I2,I3);

	      const int includeGhost=1;
	      bool ok = ParallelUtility::getLocalArrayBounds(gc[grid].mask(),mask,I1,I2,I3,includeGhost);
	      if( !ok ) continue;

	      I1=Range(I1.getBase(),I1.getBound()-is1);
	      I2=Range(I2.getBase(),I2.getBound()-is2);

	      real dx[3]={0.,0.,0.}, xab[2][3]={0.,0.,0.,0.,0.,0.};
	      if( gc[grid].isRectangular() )
		gc[grid].getRectangularGridParameters( dx, xab );

	      const int i0a=gc[grid].gridIndexRange(0,0);
	      const int i1a=gc[grid].gridIndexRange(0,1);

	      const real xa=xab[0][0], dx0=dx[0];
	      const real ya=xab[0][1], dy0=dx[1];

#define XSCALE(x) (parameters.xScaleFactor*(x))
#define YSCALE(y) (parameters.yScaleFactor*(y))
#define ZSCALE(z) (parameters.zScaleFactor*(z))
	
#define COORD0(i0,i1,i2) XSCALE(xa+dx0*(i0-i0a))
#define COORD1(i0,i1,i2) YSCALE(ya+dy0*(i1-i1a))

	      glBegin(GL_LINES); // *wdh* 100325 -- bug fixed (for parallel)
	      if( int(gc[grid].isAllVertexCentered()) )
	      {
		if( plotRectangular )
		{
		  FOR_3(i1,i2,i3,I1,I2,I3)
		  {
		    if( MASK_DINIB(i1,i2,i3) &&  MASK_DINIB(i1+is1,i2+is2,i3) )
		    {
		      glVertex3( COORD0(i1    ,i2    ,i3),COORD1(i1    ,i2    ,i3),zRaise );
		      glVertex3( COORD0(i1+is1,i2+is2,i3),COORD1(i1+is1,i2+is2,i3),zRaise );
		    }
		  }
		}
		else
		{
		  FOR_3(i1,i2,i3,I1,I2,I3)
		  {
		    if( MASK_DINIB(i1,i2,i3) &&  MASK_DINIB(i1+is1,i2+is2,i3) )
		    {
		      glVertex3( XSCALE(vertex(i1    ,i2    ,i3,axis1)),YSCALE(vertex(i1    ,i2    ,i3,axis2)),zRaise );
		      glVertex3( XSCALE(vertex(i1+is1,i2+is2,i3,axis1)),YSCALE(vertex(i1+is1,i2+is2,i3,axis2)),zRaise );
		    }
		  }
		}
	    
	      }
	      else 
	      {
		if( plotRectangular )
		{
		  FOR_3(i1,i2,i3,I1,I2,I3)
		  {
		    if( MASK_DNIB(i1,i2,i3) )
		    {
		      glVertex3( COORD0(i1    ,i2    ,i3),COORD1(i1    ,i2    ,i3),zRaise );
		      glVertex3( COORD0(i1+is1,i2+is2,i3),COORD1(i1+is1,i2+is2,i3),zRaise );
		    }
		  }
		}
		else
		{
		  FOR_3(i1,i2,i3,I1,I2,I3)
		  {
		    if( MASK_DNIB(i1,i2,i3) )
		    {
		      glVertex3( XSCALE(vertex(i1    ,i2    ,i3,axis1)),YSCALE(vertex(i1    ,i2    ,i3,axis2)),zRaise);
		      glVertex3( XSCALE(vertex(i1+is1,i2+is2,i3,axis1)),YSCALE(vertex(i1+is1,i2+is2,i3,axis2)),zRaise);
		    }
		  }
		}
	      }
	      glEnd();
	    }
	  }
	  glLineWidth(parameters.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor() );
	  
	} // end if plotOnThisProcessor
      }

    } // for p (processors)
    
  } // end for grid 
  
  
}
#undef COORD0
#undef COORD1

// static int numberOfBoundaryConditions;
// static IntegerArray boundaryConditionList;

int PlotIt::
bcNumber( const int bc, IntegerArray & boundaryConditionList, int numberOfBoundaryConditions )
// Return the index in boundaryConditionList of the element that matches bc
{
  for( int i=0; i<numberOfBoundaryConditions; i++ ) // check if it is already in the list
    if( bc==boundaryConditionList(i) )
      return i;
  cout << "PlotIt::bcNumber():internal ERROR: boundary condition not in list! \n";
  return 0;
}

//\begin{>PlotItInclude.tex}{\subsection{Plot a MappedGrid}} 
void PlotIt::
plot(GenericGraphicsInterface &gi, MappedGrid & mg, 
     GraphicsParameters & parameters /* = Overture::defaultGraphicsParameters() */ )
//===================================================================================
// /Description:
//   Plot a MappedGrid and optionally supply parameters that define the plot characteristics.
//   In two-dimensions, grid-lines are plotted. 
//   In three dimensions, by default only the block boundaries of the grid are plotted.
//   You may also plot grid lines on boundaries and/or plot shaded boundary surfaces.
//
//   Grids and boundary conditions are plotted with different colours. Grids are numbered
//   and boundary conditions are numbered. For each number there corresponds a colour.
//   The colour associated with each number is plotted in the lower right corner.
// 
//
// /mg (input): MappedGrid to plot.
// /parameters (input/output): Supply optional parameters to alter plot characteristics.
//
// /Author: WDH
//
//\end{PlotItInclude.tex}  
//===================================================================================
{
  if( !gi.graphicsIsOn() ) return;

  GridCollection gc(mg.numberOfDimensions(),1);  // make a collection with 1 component grid
  gc[0].reference(mg);
  gc.updateReferences();
  
  plot(gi, gc,parameters );    
}

//\begin{>>PlotItInclude.tex}{\subsection{Plot a GridCollection (CompositeGrid)}} 
void PlotIt::
plot(GenericGraphicsInterface &gi, GridCollection & gc0, 
     GraphicsParameters & parameters /* = Overture::defaultGraphicsParameters() */)
//===================================================================================
// /Description:
//   Plot a CompositeGrid and optionally supply parameters that define the plot characteristics.
//   In two dimensions, grid-lines are plotted. Interpolation points can be plotted with
//   small circles.
//   In three dimensions, by default only the block boundaries of the grid are plotted.
//   You may also plot grid lines on boundaries and/or plot shaded boundary surfaces.
//   
//   Grids and boundary conditions are plotted with different colours. Grids are numbered
//   and boundary conditions are numbered. For each number there corresponds a colour.
//   The colour associated with each number is plotted in the lower right corner.
//
// /gc (input): CompositeGrid to plot.
// /parameters (input/output): Supply optional parameters to alter plot characteristics.
//
// /Author: WDH \& AP
//
//\end{PlotItInclude.tex}  
//===================================================================================
{
  if( !gi.graphicsIsOn() ) return;
  
  // In parallel: make a new grid that only lives on one processor
  const int processorForGraphics = gi.getProcessorForGraphics();

#ifndef USE_PPP
  GridCollection & gc = gc0;
#else

  GridCollection *gcp=&gc0;  // default
  bool multiProcessorGrid=false;
  
  if( PlotIt::parallelPlottingOption==0 )
  { 
     // *old way* copy all data to the processorForGraphics

    // Check whether this GridCollection already lives on the processor used for graphics.
    for( int grid=0; grid<gc0.numberOfComponentGrids(); grid++ )
    {
      Partitioning_Type & partition = (Partitioning_Type &)gc0[grid].getPartition();
      const intSerialArray & processorSet = partition.getProcessorSet();
      if( processorSet.getLength(0)!=1 || processorSet(0)!=processorForGraphics )
      {
	multiProcessorGrid=true;
	break;
      }
    }
  
    gcp=&gc0; // use this if we are already on the graphics processor.
    if( multiProcessorGrid )
    {
      if( gc0.getClassName()=="CompositeGrid" )
      {
	CompositeGrid & cg = *new CompositeGrid();
	ParallelGridUtility::redistribute( (CompositeGrid &)gc0, cg, Range(processorForGraphics,processorForGraphics) );
	gcp=&cg;
      }
      else
      {
	gcp = new GridCollection();
	ParallelGridUtility::redistribute( gc0, *gcp, Range(processorForGraphics,processorForGraphics) );

      }
    }
    else
    {
      // In this case the input grid already lives entirely on the processor used for graphics

      // copy interpolation data from AMR refinement grids -- *wdh* 060524
      // The interp. data for the AMR grids is stored in separate arrays that are local to each processor.
      // --> this should be moved into a separate function.
      if( gc0.getClassName()=="CompositeGrid" )
      {
	CompositeGrid & cg = (CompositeGrid &)gc0;
	Partitioning_Type & partition = cg->partition;
	const int numberOfDimensions = cg.numberOfDimensions();
	intSerialArray & numberOfInterpolationPointsLocal = cg->numberOfInterpolationPointsLocal;
	if( numberOfInterpolationPointsLocal.getLength(0)>0 )
	{

	  int gStart = cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAll ? 0 : 
	    cg.numberOfBaseGrids();

	  printF("plot(grid): copy local interp data, gStart=%i\n",gStart);

	  for( int g=gStart; g<cg.numberOfComponentGrids(); g++ )
	  {
	    const int ni = cg->numberOfInterpolationPointsLocal(g); 
	    // if( ni==0 || cg.numberOfInterpolationPoints(g)==ni ) continue;
	    if( ni==0 ) continue;

	    cg.numberOfInterpolationPoints(g)=ni; 
	
	    cg->interpolationPoint[g].partition(partition);
	    cg->interpolationPoint[g].redim(ni,numberOfDimensions);

	    cg->interpoleeGrid[g].partition(partition);
	    cg->interpoleeGrid[g].redim(ni);

	    cg->interpolationCoordinates[g].partition(partition);
	    cg->interpolationCoordinates[g].redim(ni,numberOfDimensions);

	    cg->interpoleeLocation[g].partition(partition);
	    cg->interpoleeLocation[g].redim(ni,numberOfDimensions);

	    cg->variableInterpolationWidth[g].partition(partition);
	    cg->variableInterpolationWidth[g].redim(ni);

	    intSerialArray ip;  getLocalArrayWithGhostBoundaries(cg->interpolationPoint[g],ip);
	    intSerialArray il;  getLocalArrayWithGhostBoundaries(cg->interpoleeLocation[g],il);
	    intSerialArray vw;  getLocalArrayWithGhostBoundaries(cg->variableInterpolationWidth[g],vw);
	    intSerialArray ig;  getLocalArrayWithGhostBoundaries(cg->interpoleeGrid[g],ig);
	    realSerialArray ci; getLocalArrayWithGhostBoundaries(cg->interpolationCoordinates[g],ci);

	    // This did not work:
	    // ip.reference(cg->interpolationPointLocal[g]);   
	    // ig.reference(cg->interpoleeGridLocal[g]);
	    // ci.reference(cg->interpolationCoordinatesLocal[g]);
	    ip=cg->interpolationPointLocal[g];
	    il=cg->interpoleeLocationLocal[g];
	    vw=cg->variableInterpolationWidthLocal[g];
	    ig=cg->interpoleeGridLocal[g];
	    ci=cg->interpolationCoordinatesLocal[g];

	    // now update references to the cg.rcData (accessed through cg-> )
	    cg.interpolationPoint[g].reference(cg->interpolationPoint[g]);
	    cg.interpoleeLocation[g].reference(cg->interpoleeLocation[g]);
	    cg.variableInterpolationWidth[g].reference(cg->variableInterpolationWidth[g]);
	    cg.interpoleeGrid[g].reference(cg->interpoleeGrid[g]);
	    cg.interpolationCoordinates[g].reference(cg->interpolationCoordinates[g]);
	  
	    if( false )
	    {
	      printF("*** gridPlot:Grid=%i, ni=%i\n",g,ni);
	      ::display(ip," gridPlot: Here is ip");
	      ::display(cg.interpolationPoint[g]," gridPlot: Here is cg.interpolationPoint[g]");
	    }
	  
	  }
	}
      }
    }
    
  }
  
  GridCollection & gc = *gcp;


#endif
  // these geometry arrays are needed:
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc[grid].isRectangular() )  // *wdh* 020406 : no need for vertex array now, if rectangular.
      gc[grid].update(MappedGrid::THEmask ); 
    else
      gc[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
  }

  // this must be here for P++ (only 1 processor actually plots stuff)
  if( Communication_Manager::localProcessNumber()==processorForGraphics ||
      PlotIt::parallelPlottingOption==1 )
  {
    if( PlotIt::parallelPlottingOption==0 )
    {  
      gi.setSingleProcessorGraphicsMode(true);   // on parallel machines we only plot on one processor
      Optimization_Manager::setOptimizedScalarIndexing(On);   // stop communication for scalar indexing
    }
    

    plotGrid(gi,gc, parameters);

    if( PlotIt::parallelPlottingOption==0 )
    {    
      gi.setSingleProcessorGraphicsMode(false);
      Optimization_Manager::setOptimizedScalarIndexing(Off);   // turn communication back on
    }
  }

  if( PlotIt::parallelPlottingOption==0 )
  {
    // Here we broadcast parameters that should be known on all processors:
    broadCast(parameters.objectWasPlotted,processorForGraphics); 
  }
  
#ifdef USE_PPP
  if( multiProcessorGrid && PlotIt::parallelPlottingOption==0 )
  {
    delete &gc;
  }
#endif

  return;
}

// ====================================================================================================
///  \brief Adjust the grid to include the displacement. 
///  \details For solid-mechanics problems, the actual grid may be the sum of the vertex array
///     and a displacement (or just the "displacement" itself). This routine changes the vertex
///     array to match the actual grid. Call unAdjustGrid to undo this operation.
/// 
/// \param gc,u,parameters (input) :
/// \xSave (input/ouput) : xSave==NULL the first time this routine is called. 
///                        Keep this pointer for calling unAdjustGrid. 
/// \displacementScaleFactor (input) : scale the displacement by this factor
// ====================================================================================================
int PlotIt::
adjustGrid( GridCollection & gc, const realGridCollectionFunction & v, GraphicsParameters & parameters,
            realSerialArray *& xSave, real displacementScaleFactor  )
{
  const real dScale = displacementScaleFactor;
  // For now if the displacementScaleFactor < 0 then we just plot the displacement by itself
  const bool plotDisplacementOnly = dScale<0.;

  const bool adjustMappingForDisplacement=parameters.dbase.get<bool>("adjustMappingForDisplacement");

  // printF("adjustGrid: psp.displacementScaleFactor=%9.3e plotDisplacementOnly=%i\n",dScale,(int)plotDisplacementOnly);

  // printF("adjustGrid: using displacement components = [%i,%i,%i]\n",parameters.displacementComponent[0],
  //   parameters.displacementComponent[1],parameters.displacementComponent[2]);
  


  if( plotDisplacementOnly && xSave==NULL )
    xSave = new realSerialArray[gc.numberOfComponentGrids()];
    
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = gc[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // for now build the vertex on all grids -- to fix
#ifdef USE_PPP
    realSerialArray vLocal;  getLocalArrayWithGhostBoundaries(v[grid],vLocal);
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
#else
    const realSerialArray & vLocal  =  v[grid];
    const realSerialArray & xLocal  =  mg.vertex();
#endif

    if( adjustMappingForDisplacement && !plotDisplacementOnly )
    {
      // keep track if the Mapping has be replaced 
      DataBase & db = mg->dbase;
      if( !db.has_key("mappingAdjustedForDisplacement") )
      {
	db.put<bool>("mappingAdjustedForDisplacement");
	db.get<bool>("mappingAdjustedForDisplacement")=false;
      }
    }



    Index I1,I2,I3;
    getIndex(mg.dimension(),I1,I2,I3);
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost);

    if( plotDisplacementOnly )
    {
      if( !ok ) continue;

      xSave[grid]=xLocal;  // save the grid pts 
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
	xLocal(I1,I2,I3,dir)=vLocal(I1,I2,I3,parameters.displacementComponent[dir]);
      }
    }
    else
    {
      if( adjustMappingForDisplacement &&  mg->dbase.get<bool>("mappingAdjustedForDisplacement") )
      {
	printF("adjustGrid::INFO: Mapping has already been adjusted for the displacement\n");
        continue;
      }
      
      // --- just over-write the vertex array ---
      if( ok )
      {
	for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
	  xLocal(I1,I2,I3,dir)+=vLocal(I1,I2,I3,parameters.displacementComponent[dir])*dScale;
      }
      
      if( adjustMappingForDisplacement )
      {
        // Replace the mapping in the component grid
        mg->dbase.get<bool>("mappingAdjustedForDisplacement")=true;

	printF("adjustGrid::INFO: replace the Mapping to adjust the grid for displacement\n");
        DataPointMapping & dpm = *new DataPointMapping; 
        dpm.incrementReferenceCount();

	const IntegerArray & dim = mg.dimension();
	const IntegerArray & gid = mg.gridIndexRange();
        int numberOfGhostLinesInData[2][3]={0,0,0,0,0,0};
	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  numberOfGhostLinesInData[0][axis]=gid(0,axis)-dim(0,axis);
	  numberOfGhostLinesInData[1][axis]=dim(1,axis)-gid(1,axis);
	}

        dpm.setDataPoints(mg.vertex(),3,mg.numberOfDimensions(),numberOfGhostLinesInData,gid );
 
        // we need to keep the mask to use with the new grid
        intArray mask; 	mask = mg.mask();

        // -- set BC's etc --
	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  dpm.setIsPeriodic(axis,mg.mapping().getIsPeriodic(axis));
	  for( int side=0; side<=1; side++ )
	  {
	    dpm.setBoundaryCondition(side,axis,mg.boundaryCondition(side,axis));
	  }
	}

        mg.reference(dpm); 
        mg.update(MappedGrid::THEmask);
        mg.mask()=mask;
	mg.updateReferences();

        dpm.decrementReferenceCount();

        // gc.updateReferences();
        // gc[grid].destroy(MappedGrid::THEcenter | MappedGrid::THEvertex );
        // gc[grid].update(MappedGrid::THEcenter | MappedGrid::THEvertex );
      }

    }
  } // end for grid

  if( adjustMappingForDisplacement )
    gc.updateReferences();

  return 0;
}

// ====================================================================================================
///  \brief Un-adjust the grid to remove the displacement. 
///  \details For solid-mechanics problems, the actual grid may be the sum of the vertex array
///     and a displacement (or just the "displacement" itself). This routine changes the vertex
///     array to match the actual grid. Call unAdjustGrid to undo this operation.
///
/// \param gc,u,parameters (input) :
/// \xSave (input) : pointer used in calling adjustGrid. 
// ====================================================================================================
int PlotIt::
unAdjustGrid( GridCollection & gc, const realGridCollectionFunction & v, GraphicsParameters & parameters,
	      realSerialArray *& xSave, real displacementScaleFactor )
{
  const real dScale = displacementScaleFactor;
  // For now if the displacementScaleFactor < 0 then we just plot the displacement by itself
  const bool plotDisplacementOnly = dScale<0.;
  const bool adjustMappingForDisplacement=parameters.dbase.get<bool>("adjustMappingForDisplacement");

  // printF("unAdjustGrid: psp.displacementScaleFactor=%9.3e plotDisplacementOnly=%i\n",dScale,(int)plotDisplacementOnly);

  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = gc[grid];
#ifdef USE_PPP
    realSerialArray vLocal;  getLocalArrayWithGhostBoundaries(v[grid],vLocal);
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
#else
    const realSerialArray & vLocal  =  v[grid];
    realSerialArray & xLocal  =  mg.vertex();
#endif

    Index I1,I2,I3;
    getIndex(mg.dimension(),I1,I2,I3);
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost);
    if( !ok ) continue;

    if( plotDisplacementOnly )
    {
      assert( xSave!=NULL );
      xLocal=xSave[grid]; // restore the grid pts
    }
    else
    {
      if( !mg->dbase.has_key("mappingAdjustedForDisplacement") || !mg->dbase.get<bool>("mappingAdjustedForDisplacement") )
      {
        // -- for now we do NOT return the Mapping to the originial (to save computation) -- this could be an option
	for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
	  xLocal(I1,I2,I3,dir)-=vLocal(I1,I2,I3,parameters.displacementComponent[dir])*dScale;
      }
    }
    
  }
  if( xSave!=NULL )
  {
    delete [] xSave;
    xSave=NULL;
  }
  
  return 0;
}


//\begin{>>PlotItInclude.tex}{\subsection{Plot the displacement for a solid mechanics problem}} 
void PlotIt::
displacement(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	     GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
//===================================================================================
// /Description:
// Plot the "displacement" for a solid mechanics problem,  d = x + u, where x are the grid vertices
// and u is the provided solution. 
// 
// /u (input): the pertubation to the grid points.
// /parameters (input/output): Supply optional parameters to alter plot characteristics.
//
// /Author: WDH \& AP
//
//\end{PlotItInclude.tex}  
//===================================================================================
{
  if( !gi.graphicsIsOn() ) return;

  bool multiProcessorGrid=false;

#ifndef USE_PPP
  const realGridCollectionFunction & v = u;
  GridCollection & gc = *u.getGridCollection();

#else

//    realGridCollectionFunction v;
//    GridCollection gc;
//    int processorForGraphics = gi.getProcessorForGraphics();
//    redistribute( u, gc,v,Range(processorForGraphics,processorForGraphics) );

  // In parallel: make a new grid and gridfunction that only live on one processor
  const int processorForGraphics = gi.getProcessorForGraphics();

  GridCollection & gc0 = *u.getGridCollection();
  GridCollection *gcp=&gc0; 
  realGridCollectionFunction *vp= (realGridCollectionFunction*)(&u); 

  if( PlotIt::parallelPlottingOption==0 )
  { 
     // *old way* copy all data to the processorForGraphics

    // Check whether this GridCollection already lives on the processor used for graphics.
    for( int grid=0; grid<gc0.numberOfComponentGrids(); grid++ )
    {
      Partitioning_Type & partition = (Partitioning_Type &)gc0[grid].getPartition();
      const intSerialArray & processorSet = partition.getProcessorSet();
      if( processorSet.getLength(0)!=1 || processorSet(0)!=processorForGraphics )
      {
	multiProcessorGrid=true;
	break;
      }
    }

    if( multiProcessorGrid )
    {
      if( u.getGridCollection()->getClassName()=="CompositeGrid" )
      {
	CompositeGrid & cg = *new CompositeGrid();
	realCompositeGridFunction & vcg = *new realCompositeGridFunction();

	ParallelGridUtility::redistribute( (realCompositeGridFunction &)u, cg,vcg,
					   Range(processorForGraphics,processorForGraphics) );

	gcp=&cg;
	vp=&vcg;

	// *** do this for now until we fix -- I don't think this is needed 060227 ****
	// cg.destroy(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now *** need to fix redistr.

      }
      else
      {
	gcp = new GridCollection();
	vp  = new realGridCollectionFunction();

	ParallelGridUtility::redistribute( u, *gcp,*vp,Range(processorForGraphics,processorForGraphics) );

      }
    }
  }
  

  GridCollection & gc = *gcp;
  const realGridCollectionFunction & v = *vp;

#endif


  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( !gc[grid].isRectangular() )
    {
      gc[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask);
    }
    else
    {
      gc[grid].update(MappedGrid::THEmask);
    }
  }

  
  // this must be here for P++ (only 1 processor actually plots stuff)
  if( Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics() ||
      PlotIt::parallelPlottingOption==1 )
  {

    if( PlotIt::parallelPlottingOption==0 )
    {  
      gi.setSingleProcessorGraphicsMode(true);   // on parallel machines we only plot on one processor
      Optimization_Manager::setOptimizedScalarIndexing(On);   // stop communication for scalar indexing
    }
    
    // Here we change the grid: 
    realSerialArray *xSave=NULL; 
    // We need to save the current displacementScaleFactor in case it is changed in the grid plotter
    const real displacementScaleFactorSave = parameters.displacementScaleFactor;
    adjustGrid( gc,v,parameters,xSave,displacementScaleFactorSave );
 
//     // *** Do this for now -- alter the grid points using u ***
//     const real dScale = parameters.displacementScaleFactor;

//     // For now if the displacementScaleFactor < 0 then we just plot the displacement by itself
//     const bool plotDisplacementOnly = dScale<0.;
//     realSerialArray *xSave=NULL; 
//     if( plotDisplacementOnly )
//       xSave = new realSerialArray[gc.numberOfComponentGrids()];
    
//     for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
//     {
//       MappedGrid & mg = gc[grid];
//       mg.update(MappedGrid::THEvertex );  // for now build the vertex on all grids -- to fix
//       #ifdef USE_PPP
//        realSerialArray vLocal;  getLocalArrayWithGhostBoundaries(v[grid],vLocal);
//        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
//       #else
//        const realSerialArray & vLocal  =  v[grid];
//        const realSerialArray & xLocal  =  mg.vertex();
//       #endif

//       Index I1,I2,I3;
//       getIndex(mg.dimension(),I1,I2,I3);
//       int includeGhost=1;
//       bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost);
//       if( !ok ) continue;

//       if( plotDisplacementOnly )
//       {
// 	xSave[grid]=xLocal;  // save the grid pts 
// 	for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
// 	{
//           xLocal(I1,I2,I3,dir)=vLocal(I1,I2,I3,parameters.displacementComponent[dir]);
          
// 	}
//       }
//       else
//       {
// 	for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
// 	  xLocal(I1,I2,I3,dir)+=vLocal(I1,I2,I3,parameters.displacementComponent[dir])*dScale;
//       }
      
//     }

    plotGrid(gi,gc, parameters, &v);

    // reset...  (make sure we use the same scale factor that was used to adjust the grid
    unAdjustGrid( gc,v,parameters,xSave,displacementScaleFactorSave );

//     for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
//     {
//       MappedGrid & mg = gc[grid];
//       #ifdef USE_PPP
//        realSerialArray vLocal;  getLocalArrayWithGhostBoundaries(v[grid],vLocal);
//        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
//       #else
//        const realSerialArray & vLocal  =  v[grid];
//        realSerialArray & xLocal  =  mg.vertex();
//       #endif

//       Index I1,I2,I3;
//       getIndex(mg.dimension(),I1,I2,I3);
//       int includeGhost=1;
//       bool ok = ParallelUtility::getLocalArrayBounds(v[grid],vLocal,I1,I2,I3,includeGhost);
//       if( !ok ) continue;

//       if( plotDisplacementOnly )
//       {
// 	xLocal=xSave[grid]; // restore the grid pts
//       }
//       else
//       {
// 	for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
// 	  xLocal(I1,I2,I3,dir)-=vLocal(I1,I2,I3,parameters.displacementComponent[dir])*dScale;
//       }
//     }
//     if( plotDisplacementOnly )
//       delete [] xSave;

    if( PlotIt::parallelPlottingOption==0 )
    {    
      gi.setSingleProcessorGraphicsMode(false);
      Optimization_Manager::setOptimizedScalarIndexing(Off);   // turn communication back on
    }
    

  }

#ifdef USE_PPP
  if( multiProcessorGrid && PlotIt::parallelPlottingOption==0 )
  {
    delete gcp;
    delete vp;
  }
#endif


}



void PlotIt::
plotGrid(GenericGraphicsInterface &gi, GridCollection & gc, 
         GraphicsParameters & parameters /* = Overture::defaultGraphicsParameters() */,
         const realGridCollectionFunction *v /* = NULL */
         )
//===================================================================================
// /Description:
//   Protected routine to plot a grid collection. This function assumes that the
// GridCollection is distributed over 1 processor only.
// 
// /Author: WDH \& AP
//
//===================================================================================
{

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();

  if( false && PlotIt::parallelPlottingOption==1 )
  {
#ifdef USE_PPP
    printF("*** plotGrid: myid=%i distributed plotting is ON *****\n",myid);
    fflush(0);
    MPI_Barrier(Overture::OV_COMM);
#endif
  }

  int numberOfGrids = gc.numberOfComponentGrids();
  const int numberOfDimensions = gc.numberOfDimensions();

  int grid,side,axis;


  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(TRUE);  // TRUE means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;


//
// AP: Need to make psp be aware of multiple windows. In particular, the currentWindow might
// change while inside this routine!!!
//

  int list=0;
  int lightList=0;

  bool branchCutsPresent=false;

  IntegerArray numberList(numberOfGrids*(1+6+6));  // keep a list of all numbers used for grid's bc's
  
//   int maximumNumberOfClippingPlanes=6;
//   int maximumNumberOfClippingPlanes;
//   glGetIntegerv(GL_MAX_CLIP_PLANES,&maximumNumberOfClippingPlanes);
//   GLenum clip[] = { GL_CLIP_PLANE0, GL_CLIP_PLANE1, GL_CLIP_PLANE2, GL_CLIP_PLANE3, GL_CLIP_PLANE4, 
// 		    GL_CLIP_PLANE5};
//   int numberOfClippingPlanes=0;
//   // clipping plane is defined by Ax+By+Cz+D
//   doubleSerialArray clippingPlaneEquation(4,maximumNumberOfClippingPlanes); 
//   bool clippingPlanes=FALSE;   // TRUE if clipping planes are defined

  IntegerArray & gridsToPlot          = psp.gridsToPlot;
  bool & plotInterpolationPoints      = psp.plotInterpolationPoints;
  bool & plotBackupInterpolationPoints= psp.plotBackupInterpolationPoints;
  bool & labelBoundaries              = psp.labelBoundaries;
  bool & plotBranchCuts               = psp.plotBranchCuts;
  int  & boundaryColourOption         = psp.boundaryColourOption;
  int  & gridLineColourOption         = psp.gridLineColourOption;
  int  & blockBoundaryColourOption    = psp.blockBoundaryColourOption;
  bool plotGridLines = psp.plotGridLines;  // this is not a reference, lhs=bool, rhs=int

// gridOptions is a reference to psp.gridOptions, which is passed to grid3d, for example.
  IntegerArray & gridOptions          = psp.gridOptions; 
  IntegerArray & gridBoundaryConditionOptions= psp.gridBoundaryConditionOptions;
  real & zLevelFor2DGrids             = psp.zLevelFor2DGrids;                    // level for a 2D grid
  real & yLevelFor1DGrids             = psp.yLevelFor1DGrids;
  bool & labelGridsAndBoundaries      = psp.labelGridsAndBoundaries;
  bool & plotInterpolationCells       = psp.plotInterpolationCells;
  bool & plotNonPhysicalBoundaries    = psp.plotNonPhysicalBoundaries;

  int & numberOfGhostLinesToPlot = psp.numberOfGhostLinesToPlot;
  int & numberOfGridCoordinatePlanes= psp.numberOfGridCoordinatePlanes; 
  IntegerArray & gridCoordinatePlane    = psp.gridCoordinatePlane;


  if( gc.numberOfDimensions() == 3 )
  {
// sync the shadedsurfacegrid to the unstructured faces
    psp.plotShadedSurfaceGrids = psp.plotUnsFaces;     
// sync the unstructured boundary edges and  the boundary grid lines to the grid lines 
    psp.plotUnsBoundaryEdges =  psp.plotLinesOnGridBoundaries = psp.plotGridLines;
  }
  else // in 2-D, the unstructured faces give the grid lines
  {
    psp.plotUnsFaces =  psp.plotLinesOnGridBoundaries = psp.plotGridLines;
  }
  
  // A hybrid surface grid may be built (for integration)
  const bool hybridSurfaceGridExists=gc.getClassName()=="CompositeGrid" && 
    ((CompositeGrid&)gc).getSurfaceStitching()!=NULL;
  bool plotHybridGrid=false;

  bool & plotNodes = psp.plotUnsNodes;
  bool & plotFaces = psp.plotUnsFaces; 

  bool & plotEdges = psp.plotUnsEdges;
  bool & plotBoundaryEdges = psp.plotUnsBoundaryEdges;

// AP: Not relevant anymore  axesOrigin[currentWindow] = psp.axesOrigin;

  bool plotGrid=TRUE;
  
  gi.setKeepAspectRatio(psp.keepAspectRatio); 

//  if( numberOfGhostLinesToPlot> 0 )
//    setMaskAtGhostPoints(gc,numberOfGhostLinesToPlot); // set values

  int multigridLevelToPlot=0;

  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

  if( psp.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    gridsToPlot.redim(numberOfGrids);  
    gridsToPlot=GraphicsParameters::toggleSum;  // by default plot all grids, contours, etc.
    gridOptions.redim(numberOfGrids);  
    gridOptions=GraphicsParameters::plotGrid | GraphicsParameters::plotBlockBoundaries | 
                GraphicsParameters::plotInteriorBoundary | GraphicsParameters::plotInterpolation |
                GraphicsParameters::plotBoundaryGridLines | GraphicsParameters::plotShadedSurfaces;

    gridBoundaryConditionOptions.redim(numberOfGrids*gc.numberOfDimensions()*2+1);
    gridBoundaryConditionOptions=0;
    
    plotObject=TRUE;
    psp.plotObjectAndExit=FALSE;
    plotInterpolationPoints= gc.numberOfDimensions()==2;
    labelBoundaries        = FALSE;
    psp.plotBranchCuts         = FALSE; // gc.numberOfDimensions()==2;
  }
  else
  {
    if( gridsToPlot.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      gridsToPlot.redim(numberOfGrids);  
      gridsToPlot=GraphicsParameters::toggleSum;  // by default plot all grids, contours, etc.
    }    
    if( gridOptions.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      int size=gridOptions.getLength(0);
      gridOptions.resize(numberOfGrids); 
      gridBoundaryConditionOptions.resize(numberOfGrids*gc.numberOfDimensions()*2+1);
      // assign parameters for the new grids:
      for( grid=size; grid<numberOfGrids; grid++ )
      {
        gridOptions(grid)=GraphicsParameters::plotGrid;
        gridOptions(grid)|=GraphicsParameters::plotInterpolation;

	if( psp.plotGridBlockBoundaries )
	  gridOptions(grid)|=GraphicsParameters::plotBlockBoundaries;
	if( psp.plotLinesOnGridBoundaries )
	  gridOptions(grid)|=GraphicsParameters::plotBoundaryGridLines;
	if( psp.plotShadedSurfaceGrids )
	  gridOptions(grid)|=GraphicsParameters::plotShadedSurfaces;
      	if( psp.plotBackupInterpolationPoints )
	  gridOptions(grid)|=GraphicsParameters::plotBackupInterpolation;
        if( psp.plotInteriorBoundaryPoints )
	  gridOptions(grid)|=GraphicsParameters::plotInteriorBoundary;

        gridOptions(grid)|=GraphicsParameters::plotInteriorGridLines;

	gridBoundaryConditionOptions(grid)=0;
      }
    }    
  }

  // make a list of all the distinct boundary condition numbers, i=0,1,2,...,numberOfBoundaryConditions
  // gridBoundaryConditionOption(i) & 1 == TRUE : plot this bc
  int numberOfBoundaryConditions=0;

  IntegerArray boundaryConditionList;
  boundaryConditionList.redim(numberOfGrids*gc.numberOfDimensions()*2+1);

  boundaryConditionList(numberOfBoundaryConditions)=psp.plotNonPhysicalBoundaries ? 1 : 0;
  boundaryConditionList(numberOfBoundaryConditions++)=0;  // first entry in the list is for bc==0 
  
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    ForBoundary(side,axis)
    { // by default, add to the list:

      gridBoundaryConditionOptions(numberOfBoundaryConditions)=gc[grid].boundaryCondition(side,axis)>0;
      // no: && map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity;
    
      boundaryConditionList(numberOfBoundaryConditions++)=gc[grid].boundaryCondition(side,axis);
      for( int i=0; i<numberOfBoundaryConditions-1; i++ ) // check if it is already in the list
      {
	if(boundaryConditionList(i)==gc[grid].boundaryCondition()(side,axis) )
	{
	  numberOfBoundaryConditions--;  // remove from the list
	  break;
	}
      }
    }
  }
  
  aString buff;
  aString answer;
  aString menu[] = {"!grid plotter",
                   "erase and exit",
		   " ",
  		   "reset all grid options",
                   ">interpolation point options",
                     "colour interpolation points",
		     "plot interpolation cells (toggle)",
                     "plot backup interpolation points (toggle)",
                   "<>line options",
                       "plot grid lines on boundaries (3D) toggle",
		     "plot grid lines on coordinate planes",
                   "<>unstructured",
		     "plot nodes",
		     "do not plot nodes",
    		     "plot faces",
    		     "do not plot faces",
		     "plot edges",
		     "do not plot edges",
    		     "plot boundary edges",
    		     "do not plot boundary edges",
                   "<>coordinate planes",
                     "plot grid lines on coordinate planes",
                     "plot next coordinate plane",
                     "plot previous coordinate plane",
  		     "delete all coordinate planes", 
                   "<>query",
  		     "mark a point",
		     "edit the mapping of a grid",
                   "<>options",
		     "keep aspect ratio",
                     "do not keep aspect ratio",
 		     "point size",
 		     "boundary line width",
                     "boundary vertical offset",
                     "change colours",
		     "output amr debug file",
		     "set plot bounds",
		     "reset plot bounds",
  		     "save grid to a file",
//                     "lighting (toggle)",
                   "<",
//                   "erase",
//                   "exit this menu",
                   "" };

  GUIState gui;
  DialogData & dialog = gui;
  
  aString *pdCommand1 = new aString[numberOfGrids+2];
  aString *pdLabel1 = new aString[numberOfGrids+2];
  int *initState1 = new int[numberOfGrids+2];

  aString *pdCommand2 = new aString[numberOfGrids+1];
  aString *pdLabel2 = new aString[numberOfGrids+1];
  int *initState2 = new int[numberOfGrids+1];

  aString *pdCommand3 = new aString[numberOfBoundaryConditions+1];
  aString *pdLabel3 = new aString[numberOfBoundaryConditions+1];
  int *initState3 = new int[numberOfBoundaryConditions+1];

  enum PickingOptionEnum
  {
    pickingOff,
    pickToToggleGrids,
    pickToColourGrids,
    pickToToggleBoundaries,
    pickToToggleBoundaryGridLines, //   *** finish this ****
    pickToColourBoundaries,
    pickToQueryGridPoint,
    pickToExamineMapping,
    pickToAddCoordinatePlane1,
    pickToAddCoordinatePlane2,
    pickToAddCoordinatePlane3,
  } pickingOption=pickToToggleGrids;

  int pickColourIndex=getXColour("aquamarine");  // index of the colour used for pick to colour grids
  bool pickClosest=false;  // if true only pick the closest of the chosen items, other use all

  // --- Build the sibling dialog for colour dialog ---
  DialogData & colourDialog = gui.getDialogSibling();
  colourDialog.setWindowTitle("Pick colour");
  colourDialog.setExitCommand("close colour choices", "close");



  if( !psp.plotObjectAndExit )
  {
    dialog.setWindowTitle("Grid Plotter");
    dialog.setExitCommand("exit this menu", "Exit");

    PlotIt::buildColourDialog(colourDialog);

    // setup a pulldown menu for toggling grids on/off
    for( grid=0; grid<numberOfGrids; grid++ )
    {
      sPrintF(pdCommand1[grid],"toggle grid %i", grid);
      sPrintF(pdLabel1[grid],"%s", (const char*)gc[grid].mapping().getName(Mapping::mappingName));
      initState1[grid] = (psp.gridsToPlot(grid) & GraphicsParameters::toggleGrids)? 1:0;
    }
    pdCommand1[numberOfGrids]="";   // null string terminates the commands
    pdLabel1[numberOfGrids]="";   // null string terminates the labels
    dialog.addPulldownMenu("View", pdCommand1, pdLabel1, GI_TOGGLEBUTTON, initState1);

    // setup a pulldown menu for toggling shaded surfaces on/off
    if (gc.numberOfDimensions() == 3)
    {
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	sPrintF(pdCommand2[grid],"toggle shaded surfaces %i", grid);
	sPrintF(pdLabel2[grid],"%s", (const char*)gc[grid].mapping().getName(Mapping::mappingName));
	initState2[grid] = (psp.gridOptions(grid) & GraphicsParameters::plotShadedSurfaces)? 1:0;
      }
      pdCommand2[numberOfGrids]="";   // null string terminates the commands
      pdLabel2[numberOfGrids]="";   // null string terminates the labels
      dialog.addPulldownMenu("3D-Shade", pdCommand2, pdLabel2, GI_TOGGLEBUTTON, initState2);
    }

    // setup a pulldown menu for toggling boundary conditions on/off
    for(int bcI=0; bcI<numberOfBoundaryConditions; bcI++ )
    {
      sPrintF(pdCommand3[bcI],"plot boundary condition (toggle) %i",boundaryConditionList(bcI));
      sPrintF(pdLabel3[bcI],"boundary condition #%i",boundaryConditionList(bcI));
      initState3[bcI] = gridBoundaryConditionOptions(bcI)? 1: 0;
      if (boundaryConditionList(bcI) == -1)
      {
	branchCutsPresent = true;
      }
    }
    pdCommand3[numberOfBoundaryConditions]="";   // null string terminates the commands
    pdLabel3[numberOfBoundaryConditions]="";   // null string terminates the labels
    dialog.addPulldownMenu("Plot BC", pdCommand3, pdLabel3, GI_TOGGLEBUTTON, initState3);
    
    // setup a pulldown for toggling interpolation points
    for( grid=0; grid<numberOfGrids; grid++ )
    {
      sPrintF(pdCommand1[grid],"toggle interpolation %i", grid);
      sPrintF(pdLabel1[grid],"%s", (const char*)gc[grid].mapping().getName(Mapping::mappingName));
      initState1[grid] = (psp.gridsToPlot(grid) & GraphicsParameters::plotInterpolation)? 1:0;
    }
    pdCommand1[numberOfGrids]="toggle interpolation all";
    pdLabel1[numberOfGrids]="toggle interpolation all";   
    pdCommand1[numberOfGrids+1]="";   // null string terminates the commands
    pdLabel1[numberOfGrids+1]="";   // null string terminates the labels
    dialog.addPulldownMenu("Interp", pdCommand1, pdLabel1, GI_TOGGLEBUTTON, initState1);


    // done setting up pulldown

    // define layout of option menus
    dialog.setOptionMenuColumns(1);
    // first option menu
    aString opCommand1[] = {"colour boundaries by bc number",
			    "colour boundaries by grid number",
                            "colour boundaries by chosen name",
			    "colour boundaries by refinement level number",
			    "colour boundaries by share value",
			    "colour boundaries black",
			    "colour boundaries by domain",
			    "colour boundaries in the default way",
			    ""};
    
    aString opLabel1[] = {"BC number", "Grid number", "Chosen name", "Refinement", "Share", "Black", "Domain", 
                          "Default", "" };

  
    // initial choice: BC number
    int initialChoice;
    initialChoice=(boundaryColourOption==GraphicsParameters::colourByBoundaryCondition ? 0 :
		   boundaryColourOption==GraphicsParameters::colourByGrid ? 1 :
		   boundaryColourOption==GraphicsParameters::colourByIndex ? 2 :
		   boundaryColourOption==GraphicsParameters::colourByRefinementLevel ? 3 :
		   boundaryColourOption==GraphicsParameters::colourByShare ? 4 : 
		   boundaryColourOption==GraphicsParameters::colourBlack ? 5 :
		   boundaryColourOption==GraphicsParameters::colourByDomain ? 6 :
		   0);  // default is by BC
    dialog.addOptionMenu( "Colour boundaries by", opCommand1, opLabel1, initialChoice); 

    // second option menu
    aString opCommand2[] = {"colour grid lines by grid number",
                            "colour grid lines from chosen name",
			    "colour grid lines by refinement level number",
			    "colour grid lines black",
                            "colour grid lines by domain",
			    "colour grid lines in the default way",""};
    
    aString opLabel2[] = {"Grid number", "Chosen name", "Refinement", "Black", "Domain", "Default", "" };

  
    // initial choice: 
    initialChoice=(gridLineColourOption==GraphicsParameters::colourByGrid ? 0 :
		   gridLineColourOption==GraphicsParameters::colourByIndex ? 1 :
		   gridLineColourOption==GraphicsParameters::colourByRefinementLevel ? 2 :
		   gridLineColourOption==GraphicsParameters::colourBlack ? 3 : 
		   boundaryColourOption==GraphicsParameters::colourByDomain ? 4 :
		   numberOfDimensions<=2 ? 0 : 5);  // default choice depends on 2D or 3D
             
    dialog.addOptionMenu( "Colour grid lines by", opCommand2, opLabel2, initialChoice); 

    // third option menu (only in 3D)
    if (gc.numberOfDimensions() == 3)
    {
      aString opCommand3[] = {"colour block boundaries by grid number",
                              "colour block boundaries by chosen name",
			      "colour block boundaries by refinement level number",
			      "colour block boundaries black",
			      "colour block boundaries by domain",
			      "colour block boundaries in the default way",
			      ""};
    
      aString opLabel3[] = {"Grid number", "Chosen name", "Refinement", "Black", "Domain", "Default", "" };


      initialChoice=(blockBoundaryColourOption==GraphicsParameters::colourByGrid ? 0 :
		     blockBoundaryColourOption==GraphicsParameters::colourByIndex ? 1 :
		     blockBoundaryColourOption==GraphicsParameters::colourByRefinementLevel ? 2 :
		     blockBoundaryColourOption==GraphicsParameters::colourBlack ? 3 : 
		     blockBoundaryColourOption==GraphicsParameters::colourByDomain ? 4 : 5 );
             
      dialog.addOptionMenu( "Colour block bndry by", opCommand3, opLabel3, initialChoice); 
    }
    

    aString opcmd[] = {"pick off","pick to toggle grids",
                       "pick to colour grids",
                       "pick to toggle boundaries",
                       "pick to toggle grid lines",
                       "pick to colour boundaries", 
                       "pick to query grid point",
		       "pick to examine mapping",
                       "pick to add coordinate plane1",
                       "pick to add coordinate plane2",
                       "pick to add coordinate plane3",
                       ""};
    aString opLabel[] = {"off",
                         "toggle grids",
                         "colour grids",
                         "toggle boundaries",
                         "toggle grid lines",
                         "colour boundaries",
                         "query grid point",
	 		 "examine mapping",
                         "add coordinate plane1",
                         "add coordinate plane2",
                         "add coordinate plane3",
                         ""};
    dialog.addOptionMenu("Pick to:", opcmd,opLabel,pickingOption);


//      const aString *colourNames = getAllColourNames();
//      const int maximumNumberOfColours=110;
//      aString pickColourCommand[maximumNumberOfColours];
//      GUIState::addPrefix(colourNames,"pick colour ",pickColourCommand,maximumNumberOfColours);
    
//      dialog.addOptionMenu("Pick colour",pickColourCommand,colourNames,pickColourIndex); 

    // done setting up option menus


    // *** specify toggle buttons ***
    const int numberOfToggleButtons=19;
    aString tbCommands[numberOfToggleButtons], tbLabels[numberOfToggleButtons];
    int tbState[numberOfToggleButtons];
    
    int i=0;
    tbCommands[i] = "plot interpolation points"; 
    tbLabels[i] = "plot interpolation points"; 
    tbState[i] = psp.plotInterpolationPoints; i++;

    tbCommands[i] = "colour interpolation points"; 
    tbLabels[i] = "colour interp points"; 
    tbState[i] = psp.plotInterpolationPoints; i++;

    tbCommands[i] = "plot block boundaries"; 
    tbLabels[i] = "plot block boundaries"; 
    tbState[i] = psp.plotGridBlockBoundaries;  i++;

    tbCommands[i] = "plot grid lines"; 
    tbLabels[i] = "plot grid lines"; 
    tbState[i] = psp.plotGridLines;  i++;

    tbCommands[i] = "plot non-physical boundaries"; 
    tbLabels[i] = "plot non-physical boundaries"; 
    tbState[i] = psp.plotNonPhysicalBoundaries;  i++;

    tbCommands[i] = "plot branch cuts";  
    tbLabels[i] = "plot branch cuts"; 
    tbState[i] = psp.plotBranchCuts;  i++;

    tbCommands[i] = "plot nodes"; 
    tbLabels[i] = "plot nodes"; 
    tbState[i] = psp.plotUnsNodes;   i++;             

    tbCommands[i] = "plot edges"; 
    tbLabels[i] = "plot edges"; 
    tbState[i] = psp.plotUnsEdges;  i++; 

    tbCommands[i] = "interior boundary points"; 
    tbLabels[i] = "interior boundary points"; 
    tbState[i] = psp.plotInteriorBoundaryPoints; i++; 

    if (gc.numberOfDimensions() == 3 )
    {
      tbCommands[i] = "plot shaded surfaces (3D)"; 
      tbLabels[i] = "plot shaded surfaces"; 
      tbState[i] = psp.plotShadedSurfaceGrids;  i++;
    }
    tbCommands[i] = "pick closest"; 
    tbLabels[i] = "pick closest"; 
    tbState[i] = pickClosest;  i++; 

    tbCommands[i] = "plot refinement grids"; 
    tbLabels[i] = "plot refinement grids"; 
    tbState[i]=psp.plotRefinementGrids;  i++;

    if( hybridSurfaceGridExists )
    {
      tbCommands[i] = "plot hybrid grid"; 
      tbLabels[i] = "plot hybrid grid"; 
      tbState[i]=plotHybridGrid ;  i++;
    }
    tbCommands[i] = "plot backup interpolation points"; 
    tbLabels[i] = tbCommands[i]; 
    tbState[i]=plotBackupInterpolationPoints;  i++;

    tbCommands[i] = "plot hidden refinement points"; 
    tbLabels[i] = tbCommands[i]; 
    tbState[i]=psp.plotHiddenRefinementPoints;  i++;
    
    tbCommands[i] = "compute coarsening factor"; 
    tbLabels[i] = tbCommands[i]; 
    tbState[i]=psp.computeCoarseningFactor;  i++;

    tbCommands[i] = "";
    tbLabels[i] = "";
    tbState[i]=0;

    assert( i<numberOfToggleButtons );

    const int numColumns=2;
    dialog.setToggleButtons(tbCommands, tbLabels, tbState, numColumns); 
// done defining toggle buttons

// text boxes
    const int maxNumberOfTextBoxes=20;
    aString textCommands[maxNumberOfTextBoxes+1];
    aString textLabels[maxNumberOfTextBoxes+1];
    aString textStrings[maxNumberOfTextBoxes+1];

    int nt=0;
    
    if (gc.numberOfDimensions() == 2)
    {
      textCommands[nt] = "raise the grid by this amount (2D)";
      textLabels[nt] = "offset grid (2D)";
      sPrintF(textStrings[nt], "%g", psp.zLevelFor2DGrids);
      nt++;
    }
    
    textCommands[nt] = "plot ghost lines";
    textLabels[nt]   = "ghost lines";
    sPrintF(textStrings[nt], "%i", psp.numberOfGhostLinesToPlot);
    nt++;

    textCommands[nt] = "plot a multigrid level";
    sPrintF(textLabels[nt],"Multigrid level (0,...,%i)", gc.numberOfMultigridLevels()-1); 
    sPrintF(textStrings[nt], "%i", multigridLevelToPlot);
    nt++;

    textCommands[nt] = "point size";
    textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt],"%3.0f pixels",psp.pointSize);
    nt++;

    textCommands[nt]=textLabels[nt] ="coarsening factor";
    sPrintF(textStrings[nt], "%i",gi.gridCoarseningFactor);
    nt++;

    textCommands[nt]=textLabels[nt] = "xScale, yScale, zScale";
    sPrintF(textStrings[nt], "%g %g %g",psp.xScaleFactor,psp.yScaleFactor,psp.zScaleFactor);
    nt++;


    textCommands[nt]=textLabels[nt] = "displacement scale factor";
    sPrintF(textStrings[nt], "%g",psp.displacementScaleFactor);
    nt++;

//      textCommands[nt] = "pick colour";
//      textLabels[nt]=textCommands[nt];
//      textStrings[nt]=getXColour(pickColourIndex);
//      nt++;

    assert( nt<maxNumberOfTextBoxes );
    textCommands[nt] = "";
    textLabels[nt] = "";
    textStrings[nt] = ""; 

    dialog.setTextBoxes(textCommands, textLabels, textStrings);
// done defining text boxes


    gui.buildPopup(menu);

// setup a user defined menu and some user defined buttons
    aString buttonCommands[] = {"plot", "erase", "erase and exit","pick colour...",
                                "refinement grid colours", "show all grids", "show all faces",
                                "print grid statistics", "plot grid quality", ""};
    aString buttonLabels[] = {"Plot", "Erase", "erase and exit", "pick colour...", 
                              "refinement grid colours", "show all grids", "show all faces", 
                              "print grid statistics", "plot grid quality",""};
 
    int numberOfRows=3;
    dialog.setPushButtons(buttonCommands, buttonLabels, numberOfRows);
    
    gi.pushGUI( gui );
  }
  
  // ---------------------------------------------------------------------------------------------
  // set the grid coarsening factor: for very fine grids we do not plot at the highest resolution
  // For 3D grids we only plot grids on the boundaries.
  const int maxPlotablePoints=500000; // maximum number of points we plot at the highest resolution
  int numberOfGridPoints=0;
  int numberOfBoundaryPoints=0;
  Index I1,I2,I3;
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    getIndex(gc[grid].gridIndexRange(),I1,I2,I3);
    numberOfGridPoints+=I1.getLength()*I2.getLength()*I3.getLength();
    if( numberOfDimensions==2 )
      numberOfBoundaryPoints+=2*(I1.getLength()+I2.getLength());
    else if( numberOfDimensions==3 )
      numberOfBoundaryPoints+=2*(I1.getLength()*I2.getLength() + 
                                 I1.getLength()*I3.getLength() + I2.getLength()*I3.getLength());
    else
      numberOfBoundaryPoints=2;
  }
  // printF("grid plotter:INFO:numberOfGridPoints=%i \n",numberOfGridPoints);
  
  int numberOfPointsToPlot=numberOfDimensions<3 ? numberOfGridPoints : numberOfBoundaryPoints;
  if( psp.computeCoarseningFactor && numberOfPointsToPlot>maxPlotablePoints )
  {
    gi.gridCoarseningFactor=int(numberOfPointsToPlot/real(maxPlotablePoints)+.5);
    if( false )
      printF("contour:INFO: Setting  gi.gridCoarseningFactor = %i since there are %i grid points to plot\n",
	   gi.gridCoarseningFactor,  numberOfPointsToPlot);
    if( !psp.plotObjectAndExit && gi.isGraphicsWindowOpen() )
      dialog.setTextLabel("coarsening factor",sPrintF(answer,"%i",gi.gridCoarseningFactor));
  }
  // ---------------------------------------------------------------------------------------------


  RealArray xBound(2,3);
  const GraphicsParameters::GridOptions 
    faceToToggle[6]={GraphicsParameters::doNotPlotFace00,GraphicsParameters::doNotPlotFace10,
		     GraphicsParameters::doNotPlotFace01,GraphicsParameters::doNotPlotFace11,
		     GraphicsParameters::doNotPlotFace02,GraphicsParameters::doNotPlotFace12}; //

  const GraphicsParameters::GridOptions 
    toggleGridLinesOnFace[6]={
                     GraphicsParameters::doNotPlotGridLinesOnFace00,GraphicsParameters::doNotPlotGridLinesOnFace10,
		     GraphicsParameters::doNotPlotGridLinesOnFace01,GraphicsParameters::doNotPlotGridLinesOnFace11,
		     GraphicsParameters::doNotPlotGridLinesOnFace02,GraphicsParameters::doNotPlotGridLinesOnFace12}; //

  // set default prompt
  aString answer2,pickColour;
  gi.appendToTheDefaultPrompt("Gridcollection>");
  
  int i,len=0;
  SelectionInfo select; select.nSelect=0;
  Range all;
  
  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
      gi.getAnswer(answer,"", select);
      gi.savePickCommands(true); // turn back on
    }
    
    // printF("grid plot: it=%i answer=%s\n",it,(const char*)answer);
    

    if( answer=="pick to query value" || 
	answer=="pick to toggle grids" || 
	answer=="pick to colour grids" || 
	answer=="pick to toggle boundaries" ||
	answer=="pick to toggle grid lines" ||
	answer=="pick to colour boundaries" ||
	answer=="pick to query grid point" ||
	answer=="pick to examine mapping" ||
	answer=="pick to add coordinate plane1" ||
	answer=="pick to add coordinate plane2" ||
	answer=="pick to add coordinate plane3" ||
	answer=="pick off" )
    {
      pickingOption= (answer=="pick to toggle grids"  ? pickToToggleGrids :
		      answer=="pick to colour grids" ? pickToColourGrids :
		      answer=="pick to toggle boundaries" ? pickToToggleBoundaries :
                      answer=="pick to toggle grid lines" ? pickToToggleBoundaryGridLines :
                      answer=="pick to colour boundaries" ? pickToColourBoundaries : 
                      answer=="pick to query grid point" ? pickToQueryGridPoint :  
		      answer=="pick to examine mapping" ? pickToExamineMapping : 
                      answer=="pick to add coordinate plane1" ? pickToAddCoordinatePlane1 :
                      answer=="pick to add coordinate plane2" ? pickToAddCoordinatePlane2 :
                      answer=="pick to add coordinate plane3" ? pickToAddCoordinatePlane3 :
                      pickingOff);
      
      gui.getOptionMenu("Pick to:").setCurrentChoice((int)pickingOption);
    }
    else if( answer(0,32)=="plot interpolation cells (toggle)" )
    {
      if( plotInterpolationCells )
        printF("plot interpolation cells.\n");
      else
        printF("do NOT plot interpolation cells.\n");
      plotInterpolationCells= !plotInterpolationCells;
    }
    // toggle entries:
    else if( dialog.getToggleValue(answer,"plot interpolation points",plotInterpolationPoints) ){}
    // for backward compatibility:
    else if( dialog.getToggleValue(answer,"plot interpolation points (toggle)",plotInterpolationPoints) ){}
    else if( dialog.getToggleValue(answer,"colour interpolation points",psp.colourInterpolationPoints) ){}
    else if( dialog.getToggleValue(answer,"compute coarsening factor",psp.computeCoarseningFactor) ){} //
    else if( dialog.getToggleValue(answer,"plot block boundaries",psp.plotGridBlockBoundaries) )
    {
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	if( psp.plotGridBlockBoundaries )
	{
	  gridOptions(grid) |=GraphicsParameters::plotBlockBoundaries;
	}
	else
	{
	  gridOptions(grid) &= ~GraphicsParameters::plotBlockBoundaries;
	}
      }
    }
    // else if( (dialog.getToggleValue(answer,"plot grid lines",plotGridLines) ||
    //         answer(0,23)=="plot grid lines (toggle)") &&
    //       (answer.length()<15 || answer(16,17)!="on") ) //kkc 050104, earlier version prevented coord plane plotting
    else if( dialog.getToggleValue(answer,"plot grid lines",plotGridLines) )
    {
      psp.plotGridLines=plotGridLines;  // rhs is a bool
      // we might as well do the 3-D here also
      psp.plotLinesOnGridBoundaries=psp.plotGridLines; 
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	if( plotGridLines )
	{
	  gridOptions(grid) |= GraphicsParameters::plotInteriorGridLines;  
	  gridOptions(grid) |= GraphicsParameters::plotBoundaryGridLines;  
	}
	else
	{
	  gridOptions(grid) &= ~GraphicsParameters::plotInteriorGridLines;  
	  gridOptions(grid) &= ~GraphicsParameters::plotBoundaryGridLines;  
	}
      }
      
      // and the unstructured boundary edges in 2-D, unstructured faces in 3-D
      if (gc.numberOfDimensions() == 3)
	plotBoundaryEdges=psp.plotGridLines;
      else
	plotFaces=psp.plotGridLines;
    }
    else if( dialog.getToggleValue(answer,"plot non-physical boundaries",plotNonPhysicalBoundaries) )
    {
      gridBoundaryConditionOptions(0)= plotNonPhysicalBoundaries ? 1 : 0;   
    }
    else if( dialog.getToggleValue(answer,"plot branch cuts",plotBranchCuts) )
    {
      if( branchCutsPresent )
      {
	int bcIndex =bcNumber(-1, boundaryConditionList, numberOfBoundaryConditions);
        gridBoundaryConditionOptions(bcIndex) = plotBranchCuts;
      }
    }
    else if( dialog.getToggleValue(answer,"plot nodes",plotNodes) ){}
    else if( dialog.getToggleValue(answer,"plot nodes (toggle)",plotNodes) ){}  // for backward compat

    else if( dialog.getToggleValue(answer,"plot edges",plotEdges) ){}
    else if( dialog.getToggleValue(answer,"plot edges (toggle)",plotEdges) ){}  // for backward compat
    else if( dialog.getToggleValue(answer,"interior boundary points",psp.plotInteriorBoundaryPoints) ){}
    else if( dialog.getToggleValue(answer,"plot hidden refinement points",psp.plotHiddenRefinementPoints) ){}
    else if( dialog.getToggleValue(answer,"plot shaded surfaces (3D)",psp.plotShadedSurfaceGrids) )
    {
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	if( psp.plotShadedSurfaceGrids )
	  gridOptions(grid) |= GraphicsParameters::plotShadedSurfaces;  
	else
	  gridOptions(grid) &= ~GraphicsParameters::plotShadedSurfaces;
      }
      
      // might as well do the unstructured faces if we are in 3-D
      if (gc.numberOfDimensions() == 3)
	plotFaces=psp.plotShadedSurfaceGrids; 
    }
    else if( answer=="set plot bounds" )
    {
      RealArray & pb = parameters.plotBound;
      parameters.usePlotBounds=true;
      gi.inputString(answer,"Enter plot bounds to use xa,xb, ya,yb, za,zb");
      sScanF(answer,"%e %e %e %e %e %e\n",&pb(0,0),&pb(1,0),&pb(0,1),&pb(1,1),&pb(0,2),&pb(1,2));
      printF(" Using plot bounds = [%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
	     pb(0,0),pb(1,0),pb(0,1),pb(1,1),pb(0,2),pb(1,2));
    }
    else if( answer=="reset plot bounds" )
    {
      parameters.usePlotBounds=false;
    }
    else if( dialog.getToggleValue(answer,"pick closest",pickClosest) ){}//

    else if( dialog.getToggleValue(answer,"plot refinement grids",psp.plotRefinementGrids) ){}
    else if( dialog.getToggleValue(answer,"plot hybrid grid",plotHybridGrid) )
    {
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	if( plotHybridGrid )
          gridOptions(grid) |=GraphicsParameters::plotInteriorBoundary;
        else
          gridOptions(grid) &= ~GraphicsParameters::plotInteriorBoundary;
      }
    }
    else if( dialog.getToggleValue(answer,"plot backup interpolation points",plotBackupInterpolationPoints) )
    {
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	if( plotBackupInterpolationPoints )
          gridOptions(grid) |= GraphicsParameters::plotBackupInterpolation;
        else
          gridOptions(grid) &= ~GraphicsParameters::plotBackupInterpolation;
      }
    }
    else if( answer(0,24)=="label boundaries (toggle)" )
    {
      labelBoundaries= !labelBoundaries;
      if( labelBoundaries )
        printF("label boundaries.\n");
      else
        printF("do NOT label boundaries.\n");
    }
    else if( answer=="colour boundaries by bc number" )
      boundaryColourOption=GraphicsParameters::colourByBoundaryCondition;
    else if( answer=="colour boundaries by grid number" )
      boundaryColourOption=GraphicsParameters::colourByGrid;
    else if( answer=="colour boundaries by refinement level number" )
      boundaryColourOption=GraphicsParameters::colourByRefinementLevel;
    else if( answer=="colour boundaries by share value" )
      boundaryColourOption=GraphicsParameters::colourByShare;
    else if( answer=="colour boundaries black" )
      boundaryColourOption=GraphicsParameters::colourBlack;
    else if( answer=="colour boundaries by domain" )
      boundaryColourOption=GraphicsParameters::colourByDomain;
    else if( answer=="colour boundaries in the default way" )
      boundaryColourOption=GraphicsParameters::defaultColour;

    else if( answer=="colour grid lines by grid number" )
      gridLineColourOption=GraphicsParameters::colourByGrid;
    else if( answer=="colour grid lines by domain" )
      gridLineColourOption=GraphicsParameters::colourByDomain;
    else if( answer=="colour boundaries by chosen name"  ||
             answer=="colour grid lines from chosen name" ||
             answer=="colour block boundaries by chosen name" )
    {
      if( answer=="colour boundaries by chosen name" )
	boundaryColourOption=GraphicsParameters::colourByIndex;
      else if( answer=="colour grid lines from chosen name" )
	gridLineColourOption=GraphicsParameters::colourByIndex;
      else 
        blockBoundaryColourOption=GraphicsParameters::colourByIndex;
      // Set initial names for all grids
      if( gc.numberOfComponentGrids()>psp.gridColours.getLength(0) )
      {
        Range all;
	int gStart=psp.gridColours.getLength(0);
	psp.gridColours.resize(gc.numberOfComponentGrids(),8);
	for( int g=gStart; g<gc.numberOfComponentGrids(); g++ )
	  psp.gridColours(g,all)=getXColour(gi.getColourName(g % GenericGraphicsInterface::numberOfColourNames));

      }
    }
    else if( answer=="colour grid lines by refinement level number" )
      gridLineColourOption=GraphicsParameters::colourByRefinementLevel;
    else if( answer=="colour grid lines black" )
      gridLineColourOption=GraphicsParameters::colourBlack;
    else if( answer=="colour grid lines in the default way" )
      gridLineColourOption=GraphicsParameters::defaultColour;

    else if( answer=="colour block boundaries by grid number" )
      blockBoundaryColourOption=GraphicsParameters::colourByGrid;
    else if( answer=="colour block boundaries by domain" )
      blockBoundaryColourOption=GraphicsParameters::colourByDomain;
    else if( answer=="colour block boundaries by refinement level number" )
      blockBoundaryColourOption=GraphicsParameters::colourByRefinementLevel;
    else if( answer=="colour block boundaries black" )
      blockBoundaryColourOption=GraphicsParameters::colourBlack;
    else if( answer=="colour block boundaries in the default way" )
      blockBoundaryColourOption=GraphicsParameters::defaultColour;

    else if( answer=="save grid to a file" )
    {
      aString gridFileName="myGrid.hdf", gridName="myGrid";
      gi.inputString(gridFileName,"Enter the file name for the grid");

      printF("Saving the current grid in the file %s\n",(const char*)gridFileName);
      printF("You can use ogen to re-generate this grid using the commands: \n"
	     "ogen\ngenerate an overlapping grid\n read in an old grid\n  %s\n  reset grid"
	     "\n display intermediate results\n compute overlap\n",
	     (const char*)gridFileName); 
      Ogen::saveGridToAFile( (CompositeGrid&)gc, gridFileName,gridName );
    }

//                          01234567890123456789
    else if( answer=="delete all coordinate planes" )
    {
      numberOfGridCoordinatePlanes=0;
    }
    else if( (len=answer.matches("add coordinate plane")) )
    {
      int grid=-1,dir=0,index=0;
      sScanF(answer(len,answer.length()-1),"%i %i %i",&grid,&dir,&index);
      if( grid>=0 && grid<gc.numberOfComponentGrids() && dir>=0 && dir<numberOfDimensions &&
          index>=gc[grid].dimension(0,dir) && index<=gc[grid].dimension(1,dir) )
      {
	if( numberOfGridCoordinatePlanes>=gridCoordinatePlane.getLength(1) )
	  gridCoordinatePlane.resize(3,gridCoordinatePlane.getLength(1)*2+5);

	gridCoordinatePlane(0,numberOfGridCoordinatePlanes)=grid;
	gridCoordinatePlane(1,numberOfGridCoordinatePlanes)=dir; 
	
	gridCoordinatePlane(2,numberOfGridCoordinatePlanes)=index; 
	numberOfGridCoordinatePlanes++;
      }
      else
      {
	printF("ERROR: invalid values for a coordinate plane, grid=%i, dir=%i, index=%i\n",grid,dir,index);
	gi.stopReadingCommandFile();
      }
    }
    else if( pickingOption==pickToExamineMapping && 
	     (select.active || select.nSelect ) )
    {
      printF("Look for the closest item picked...\n");
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	if( gc[grid].getGlobalID()==select.globalID )
	{
	  printF("Examine grid %i (%s) \n",grid,(const char*)gc[grid].getName());

	  // PlotIt::plot(gi,gc[grid].mapping().getMapping());
          gc[grid].mapping().getMapping().interactiveUpdate(gi); // *wdh* 090705
	  break;
	}
      }
    }
    else if( pickingOption==pickToToggleGrids && 
	     (select.active || select.nSelect ) )
    {
      if( pickClosest )
      {
        printF("Look for the closest item picked...(toggle `pick closest' to choose all items picked)\n");
	for( int grid=0; grid<numberOfGrids; grid++ )
	{
	  if( gc[grid].getGlobalID()==select.globalID )
	  {
	    printF("Toggle grid %i (%s) \n",grid,(const char*)gc[grid].getName());
	    gridsToPlot(grid)^=GraphicsParameters::toggleGrids;

	    int value=gridsToPlot(grid)&GraphicsParameters::toggleGrids;
	    dialog.getPulldownMenu("View").setToggleState(grid,value);
	    gi.outputToCommandFile(sPrintF(answer,"toggle grid %i %i\n",grid,value));
	    break;
	  }
	}
      }
      else
      {
        printF("Look for the all items picked... (toggle `pick closest' to only choose the closest)\n");
	for( int grid=0; grid<numberOfGrids; grid++ )
	{
	  for( int i=0; i<select.nSelect; i++ )
	  {
	    if( gc[grid].getGlobalID()==select.selection(i,0) )
	    {
	      printF("Toggle grid %i (%s) \n",grid,(const char*)gc[grid].getName());
	      gridsToPlot(grid)^=GraphicsParameters::toggleGrids;

	      int value=gridsToPlot(grid)&GraphicsParameters::toggleGrids;
	      dialog.getPulldownMenu("View").setToggleState(grid,value);
	      gi.outputToCommandFile(sPrintF(answer,"toggle grid %i %i\n",grid,value));
	      break;
	    }
	  }
	}
      }
    }
    else if( (pickingOption==pickToToggleBoundaries || 
              pickingOption==pickToToggleBoundaryGridLines || 
              pickingOption==pickToColourBoundaries ||
              pickingOption==pickToQueryGridPoint ||
              pickingOption==pickToAddCoordinatePlane1 ||
              pickingOption==pickToAddCoordinatePlane2 ||
              pickingOption==pickToAddCoordinatePlane3
             ) && 
	     (select.active || select.nSelect ) )
    {
      if( pickClosest )
      {
        printF("Look for the closest item picked...(toggle `pick closest' to choose all items picked)\n");
      }
      else
      {
        printF("Look for the all items picked... (toggle `pick closest' to only choose the closest)\n");
      }

      GridCollection & cg = multigridLevelToPlot== 0 ? gc : gc.multigridLevel[multigridLevelToPlot];
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	for( int i=0; i<(pickClosest ? 1 : select.nSelect); i++ )
	{
	  if( (pickClosest && cg[grid].getGlobalID()==select.globalID) ||
              (!pickClosest && cg[grid].getGlobalID()==select.selection(i,0)) )
	  {
            // Find the closest grid face
            int side=0,axis=0;
	    realSerialArray x(1,3),r(1,3);
	    x(0,0)=select.x[0];
	    x(0,1)=select.x[1];
	    x(0,2)=select.x[2];
	    r=-1.;
            #ifndef USE_PPP
	      cg[grid].mapping().getMapping().inverseMap(x,r);   // inverseMapS may not be defined for all ??
            #else
	      cg[grid].mapping().getMapping().inverseMapS(x,r);
            #endif
	    
            const real minDist=min( min(fabs(r)), min(fabs(r-1.)) ); // distance to closest boundary
            int dir;
            for( dir=0; dir<numberOfDimensions; dir++ )
	    {
	      if( fabs(r(0,dir)) <= minDist )
	      {
		side=0, axis=dir; 
                break;
	      }
	      else if( fabs(r(0,dir)-1.) <= minDist )
	      {
                side=1, axis=dir;
                break;
	      }
	    }
//  	    printF("selection: x=(%8.2e,%8.2e,%8.2e) grid=%i r=(%8.2e,%8.2e,%8.2e)\n",
//  		   x(0,0),x(0,1),x(0,2),grid,r(0,0),r(0,1),r(0,2));
	    
            if( pickingOption==pickToAddCoordinatePlane1 ||
		pickingOption==pickToAddCoordinatePlane2 ||
		pickingOption==pickToAddCoordinatePlane3 )
	    {
              MappedGrid & mg = cg[grid];
              // find the closest grid point
              int iv[3], &i1 = iv[0], &i2=iv[1], &i3=iv[2];
              i3=0;
              for( dir=0; dir<numberOfDimensions; dir++ )
	      {
		real dr = mg.gridSpacing(dir);
                iv[dir]=int(r(0,dir)/dr+20.5)-20+ mg.gridIndexRange(0,dir); // watch out for negative r
                iv[dir]=max(mg.dimension(0,dir),min(mg.dimension(1,dir),iv[dir]));
	      }

              int coordinateDirection = (pickingOption==pickToAddCoordinatePlane1 ? 0 :
					 pickingOption==pickToAddCoordinatePlane2 ? 1 : 2);

	      if( numberOfGridCoordinatePlanes>=gridCoordinatePlane.getLength(1) )
		gridCoordinatePlane.resize(3,gridCoordinatePlane.getLength(1)*2+5);

	      gridCoordinatePlane(0,numberOfGridCoordinatePlanes)=grid;
	      gridCoordinatePlane(1,numberOfGridCoordinatePlanes)=coordinateDirection;
	      gridCoordinatePlane(2,numberOfGridCoordinatePlanes)=iv[coordinateDirection];
	      numberOfGridCoordinatePlanes++;

              gi.outputToCommandFile(sPrintF(answer,"add coordinate plane %i %i %i (grid dir index)\n",grid,
                      coordinateDirection,iv[coordinateDirection]));

              break;
	    }
	    else if( pickingOption==pickToQueryGridPoint )
	    {
              printF("\n ----------------------------------------------------------------------------------\n");
              printF(" Point x=(%9.3e,%9.3e,%9.3e) lies on grid=%i (name=%s) at r=(%9.3e,%9.3e,%9.3e)\n",
		     x(0,0),x(0,1),x(0,2),grid,(const char*)cg[grid].getName(),r(0,0),r(0,1),r(0,2) );
	      
              MappedGrid & mg = cg[grid];
              // find the closest grid point
              int iv[3], &i1 = iv[0], &i2=iv[1], &i3=iv[2];
              i3=0;
              for( dir=0; dir<numberOfDimensions; dir++ )
	      {
		real dr = mg.gridSpacing(dir);
                iv[dir]=int(r(0,dir)/dr+20.5)-20+ mg.gridIndexRange(0,dir); // watch out for negative r
                iv[dir]=max(mg.dimension(0,dir),min(mg.dimension(1,dir),iv[dir]));
	      }
              const intArray & mask = mg.mask();
              // printF(" The closest grid point is (i1,i2,i3)=(%i,%i,%i) with mask=%i\n",i1,i2,i3,mask(i1,i2,i3));
              printF(" The closest grid point is (i1,i2,i3)=(%i,%i,%i)\n",i1,i2,i3);
	      fflush(0);

              if( gc.getClassName()=="CompositeGrid" && gc.numberOfComponentGrids()>1 )
	      {
		// Check to see if this is an interpolation point
         	CompositeGrid & cg0 = multigridLevelToPlot==0 ? (CompositeGrid &)gc :  
                                      ((CompositeGrid &)gc).multigridLevel[multigridLevelToPlot];
                if( cg0.numberOfInterpolationPoints(grid)>0 )
		{

		  bool useLocal = !( 
		    (grid<cg.numberOfBaseGrids() && 
		     cg0->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
		    cg0->localInterpolationDataState==CompositeGridData::noLocalInterpolationData );

		  intSerialArray ip,il,ig,vWidth;
                  realSerialArray ci;
		  if( !useLocal )
		  {
		    ip.reference(cg0.interpolationPoint[grid].getLocalArray());
		    il.reference(cg0.interpoleeLocation[grid].getLocalArray());
		    ig.reference(cg0.interpoleeGrid[grid].getLocalArray());
		    ci.reference(cg0.interpolationCoordinates[grid].getLocalArray());
		    vWidth.reference(cg0.variableInterpolationWidth[grid].getLocalArray());
		  }
		  else
		  {
		    ip.reference(cg0->interpolationPointLocal[grid]);
		    il.reference(cg0->interpoleeLocationLocal[grid]);
		    ig.reference(cg0->interpoleeGridLocal[grid]);
		    ci.reference(cg0->interpolationCoordinatesLocal[grid]);
		    vWidth.reference(cg0->variableInterpolationWidthLocal[grid]);
		  }

		  int i0=-1;
                  for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
		  {
		    if( ip(i,0)==i1 && ip(i,1)==i2 && (numberOfDimensions==2 || ip(i,2)==i3 ) )
		    {
		      i0=i;
		      break;
		    }
		  }
		  if( i0>=0 )
		  {
		    printf(" Point (%i,%i,%i) on grid %i interpolates from grid %i (%s) at r=(%9.3e,%9.3e,%9.3e)\n",
			   i1,i2,i3,grid,ig(i0),(const char*)cg0[ig(i0)].getName(),
                           ci(i0,0),ci(i0,1),(numberOfDimensions==2 ? 0. : ci(i0,2)));
		    printf(" Interpolation pt %i, width=%i, lower left corner of interp stencil=(%i,%i,%i)\n",
			   i0,vWidth(i0),il(i0,0),il(i0,1),(numberOfDimensions==2 ? 0 : il(i0,2)));
		  }

		  
                  // -- OLD ---

// 		  const intArray & ip =cg0.interpolationPoint[grid];
// 		  const intArray & interpoleeGrid =cg0.interpoleeGrid[grid];
// 		  const int ni=cg0.numberOfInterpolationPoints(grid);
// 		  int i0=-1;
// 		  for( int i=0; i<ni; i++ )
// 		  {
// 		    if( ip(i,0)==i1 && ip(i,1)==i2 && (numberOfDimensions==2 || ip(i,2)==i3 ) )
// 		    {
// 		      i0=i;
// 		      break;
// 		    }
// 		  }
// 		  if( i0>=0 )
// 		  {
// 		    const realArray & ci = cg0.interpolationCoordinates[grid];
// 		    const intArray & il = cg0.interpoleeLocation[grid];
// 		    const intArray & vWidth = cg0.variableInterpolationWidth[grid];
// 		    printF(" Point (%i,%i,%i) on grid %i interpolates from grid %i at r=(%9.3e,%9.3e,%9.3e)\n",
// 			   i1,i2,i3,grid,interpoleeGrid(i0),ci(i0,0),ci(i0,1),(numberOfDimensions==2 ? 0. : ci(i0,2)));
// 		    printF(" Interpolation pt %i, width=%i, lower left corner of interp stencil=(%i,%i,%i)\n",
// 			   i0,vWidth(i0),il(i0,0),il(i0,1),(numberOfDimensions==2 ? 0 : il(i0,2)));
// 		  }

		}
	      } // end if ( gc.getClassName()=="CompositeGrid" ..
	      

              mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
              const realArray & vertex = mg.vertex();
              fflush(0);
	      printF(".....................nearby mask values ..............................\n");
	      printF(" mask: 1=interior, 2=ghost, -2,3=interiorBoundaryPoint, 4=hidden-by-refinement <0 =interp \n");
              fflush(0);
	  
	      if( i1>=mg.dimension(0,0) && i1<=mg.dimension(1,0) &&
		  i2>=mg.dimension(0,1) && i2<=mg.dimension(1,1) &&
		  i3>=mg.dimension(0,2) && i3<=mg.dimension(1,2) )
	      {
		// -- In parallel we print from the processor that owns the point ---
		OV_GET_SERIAL_ARRAY_CONST(real,vertex,vertexLocal);
		OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
	
                #ifdef USE_PPP
		 const int proc = mask.Array_Descriptor.findProcNum( iv );  // point this on this processor
                #else
		 const int proc = 0;
                #endif

		if( myid==proc )
		{
		  const int dw = mg.discretizationWidth(0); // discretization width 
		  const int hw = (dw-1)/2;  // half width 

		  printf(" grid=%i : %s, (i1,i2,i3)=(%i,%i,%i) x=(%11.5e,%11.5e,%11.5e), mask=%i decode=%i\n",
			 grid,(const char*)mg.getName(),
			 i1,i2,i3,
			 vertexLocal(i1,i2,i3,0),vertexLocal(i1,i2,i3,1),
			 (mg.numberOfDimensions()>2 ? vertexLocal(i1,i2,i3,2) : 0.),maskLocal(i1,i2,i3),
			 decode(maskLocal(i1,i2,i3)));

		  for( int j3=i3-hw; j3<=i3+hw; j3++ )
		  {
		    if( j3<mg.dimension(0,2) || j3>mg.dimension(1,2) )
		      continue;
		    for( int j2=i2-hw; j2<=i2+hw; j2++ )
		    {
		      if( j2<mg.dimension(0,1) || j2>mg.dimension(1,1) )
			continue;
		      printf("   mask(%i:%i,%i,%i) =",i1-hw,i1+hw,j2,j3);

		      for( int j1=i1-hw; j1<=i1+hw; j1++ )
		      {
			if( j1>=mg.dimension(0,0) && j1<=mg.dimension(1,0) )
			{
			  printf(" %3i ",decode(maskLocal(j1,j2,j3)));
			}
		      }
		      printf("\n");
		    }
		  }
		  fflush(0);
		}
	      }
	      
// 	      if( i1>=mg.dimension(0,0) && i1<=mg.dimension(1,0) &&
// 		  i2>=mg.dimension(0,1) && i2<=mg.dimension(1,1) &&
// 		  i3>=mg.dimension(0,2) && i3<=mg.dimension(1,2) )
// 	      {
// 		printf(" grid=%i : %s, (i1,i2,i3)=(%i,%i,%i) x=(%11.5e,%11.5e,%11.5e), mask=%i decode=%i\n",
// 		       grid,(const char*)mg.getName(),
// 		       i1,i2,i3,
// 		       vertex(i1,i2,i3,0),vertex(i1,i2,i3,1),
// 		       (mg.numberOfDimensions()>2 ? vertex(i1,i2,i3,2) : 0.),mask(i1,i2,i3),decode(mask(i1,i2,i3)));
// 		for( int j3=i3-1; j3<=i3+1; j3++ )
// 		{
// 		  if( j3<mg.dimension(0,2) || j3>mg.dimension(1,2) )
// 		    continue;
// 		  for( int j2=i2-1; j2<=i2+1; j2++ )
// 		  {
// 		    if( j2<mg.dimension(0,1) || j2>mg.dimension(1,1) )
// 		      continue;
// 		    printF(" mask(%i:%i,%i,%i) =",i1-1,i1+1,j2,j3);

// 		    for( int j1=i1-1; j1<=i1+1; j1++ )
// 		    {
// 		      if( j1>=mg.dimension(0,0) && j1<=mg.dimension(1,0) )
// 		      {
// 			printf(" %3i ",decode(mask(j1,j2,j3)));
// 		      }
// 		    }
// 		    printF("\n");
// 		  }
// 		}
// 	      }

              fflush(0);
              printF(" ----------------------------------------------------------------------------------\n");
	    }
	    else if( pickingOption==pickToToggleBoundaries )
	    {
	      printF("Toggle (side,axis)=(%i,%i) of grid %i (%s) \n",side,axis,grid,(const char*)cg[grid].getName());
	      gridOptions(grid)^=faceToToggle[side+2*axis];
	      int faceIsShown = !bool(gridOptions(grid)&faceToToggle[side+2*axis]);
	      gi.outputToCommandFile(sPrintF(answer,"toggle boundary %i %i %i %i\n",side,axis,grid,faceIsShown));
	    }
            else if( pickingOption==pickToToggleBoundaryGridLines )
	    {
	      printF("Toggle grid lines on boundary (side,axis)=(%i,%i) of grid %i (%s) \n",
                        side,axis,grid,(const char*)cg[grid].getName());
	      gridOptions(grid)^= toggleGridLinesOnFace[side+2*axis];
	      int faceIsShown = !bool(gridOptions(grid)&toggleGridLinesOnFace[side+2*axis]);
	      gi.outputToCommandFile(sPrintF(answer,"toggle grid lines on boundary %i %i %i %i\n",
                        side,axis,grid,faceIsShown));
	    }
	    else
	    {

              const aString pickColour=getXColour(pickColourIndex);

	      printF("Colour (side,axis)=(%i,%i) of grid %i (%s) to be %s\n",
                      side,axis,grid,(const char*)cg[grid].getName(),(const char*)pickColour);

	      gi.outputToCommandFile(sPrintF(answer,"grid boundary colour (side,axis,grid,colour): %i %i %i %s\n",
					     side,axis,grid,(const char*)pickColour));
	      if( numberOfGrids>psp.gridColours.getLength(0) )
	      {
		int gStart=max(0,psp.gridColours.getLength(0));
		psp.gridColours.resize(cg.numberOfComponentGrids(),8);
		// Set initial names for all grids
		for( int g=gStart; g<cg.numberOfComponentGrids(); g++ )
		  psp.gridColours(g,all)=getXColour(gi.getColourName((g % GenericGraphicsInterface::numberOfColourNames)));
	      }

              psp.gridColours(grid,side+2*(axis))=pickColourIndex;
	    }
	  }
	}
      }
    }
    else if( (len=answer.matches("toggle boundary")) )
    {
      int faceIsShown, gridToToggle=-1, side=-1, axis=-1;
      sScanF(&answer[len],"%i %i %i %i", &side, &axis, &gridToToggle, &faceIsShown );
      if( gridToToggle>=0 && gridToToggle<numberOfGrids && side>=0 && side<=1 && axis>=0 && axis<numberOfDimensions )
      {
        if( (!(gridOptions(gridToToggle)&faceToToggle[side+2*axis])) != faceIsShown )
  	  gridOptions(gridToToggle) ^= faceToToggle[side+2*axis];
      }
      else
      {
	printF("toggle boundary: Invalid values for one of grid=%i or side=%i or axis=%i\n",gridToToggle,side,axis);
	gi.stopReadingCommandFile();
      }
    }
    else if( (len=answer.matches("toggle grid lines on boundary")) )
    {
      int faceIsShown, gridToToggle=-1, side=-1, axis=-1;
      sScanF(&answer[len],"%i %i %i %i", &side, &axis, &gridToToggle, &faceIsShown );

//       printF("toggle grid lines on boundary: grid=%i side=%i axis=%i\n",
//                gridToToggle,side,axis);

      if( gridToToggle>=0 && gridToToggle<numberOfGrids && side>=0 && side<=1 && axis>=0 && axis<numberOfDimensions )
      {
        if( (!(gridOptions(gridToToggle)&toggleGridLinesOnFace[side+2*axis])) != faceIsShown )
  	  gridOptions(gridToToggle) ^= toggleGridLinesOnFace[side+2*axis];
      }
      else
      {
	printF("toggle grid lines on boundary: Invalid values for one of grid=%i or side=%i or axis=%i\n",
               gridToToggle,side,axis);
	gi.stopReadingCommandFile();
      }
    }
    else if( (len=answer.matches("toggle grid")) )
    {
      // this needs to go after "toggle grid lines on boundary"

      int onOff, gridToToggle=-1;
      sScanF(&answer[len],"%i %i", &gridToToggle, &onOff);

      // printF("toggle grid: gridToToggle=%i onOff=%i\n",gridToToggle,onOff);

      if( gridToToggle>=0 && gridToToggle<numberOfGrids )
      {
	gridsToPlot(gridToToggle) ^=GraphicsParameters::toggleGrids;
	int value=gridsToPlot(gridToToggle)&GraphicsParameters::toggleGrids;
	dialog.getPulldownMenu("View").setToggleState(grid,value);
      }
    }
    else if( answer(0,21)=="toggle shaded surfaces" )
    {
      // the syntax is toggle shaded surfaces grid# state
      int onOff, gridToToggle=-1;
      sScanF(&answer[22],"%i %i", &gridToToggle, &onOff);

      if( gridToToggle>=0 && gridToToggle<numberOfGrids )
      {
	gridOptions(gridToToggle) ^=GraphicsParameters::plotShadedSurfaces;
      }
    }
    else if( answer=="toggle interpolation all" )
    {
      for(int grid=0; grid<numberOfGrids; grid++ )
        gridOptions(grid) ^=GraphicsParameters::plotInterpolation;
    }
    else if( (len=answer.matches("toggle interpolation")) )
    {
      int onOff, gridToToggle=-1;
      sScanF(&answer[len],"%i %i", &gridToToggle, &onOff);

      if( gridToToggle>=0 && gridToToggle<numberOfGrids )
      {
	gridOptions(gridToToggle) ^=GraphicsParameters::plotInterpolation;
      }
    }
    else if( answer=="plot grid lines on coordinate planes" )
    {
      printF(" grid   coordinate   starting      ending       grid\n");
      printF("        direction    grid index    grid index   name\n");
      for( grid=0; grid<numberOfGrids; grid++ )
      {
        for( int axis=0; axis<3; axis++ )
	{
	  printF(" %6i    %1i      %5i       %7i        %s \n",grid,axis,gc[grid].indexRange()(Start,axis),
                 gc[grid].indexRange()(End,axis),(const char *)gc[grid].mapping().getName(Mapping::mappingName));
	}
      }
      numberOfGridCoordinatePlanes=0;
      gridCoordinatePlane=0;
      for(;;)
      {
	gi.inputString(answer2,sPrintF(buff,"Enter grid, coordinate direction and grid index (enter -1 finish)"));
	if( answer2 !="" && answer2!=" ")
	{
          int dir,index;
	  sScanF(answer2,"%i %i %i",&grid,&dir,&index);
	  if( grid<0 )
            break;
	  if( grid>=numberOfGrids )
	  {
	    printF("Error, the grid number=%i must be in the range [0,%i]\n",grid,numberOfGrids-1);
	  }
          else if( dir<0 || dir>2 )
	  {
	    printF("Error, the coordinate direction=%i must be 0,1, or 2 \n",dir);
	  }
	  else if( index<gc[grid].indexRange()(Start,dir) || index>gc[grid].indexRange()(End,dir) )
	  {
	    printF("Error, index=%i should be in the range [%i,%i] \n",index,gc[grid].indexRange()(Start,dir),
		   gc[grid].indexRange()(End,dir));
	  }
	  else
	  {
            if( numberOfGridCoordinatePlanes>=gridCoordinatePlane.getLength(1) )
	    {
	      gridCoordinatePlane.resize(3,gridCoordinatePlane.getLength(1)*2+5);
	    }
	    gridCoordinatePlane(0,numberOfGridCoordinatePlanes)=grid;
	    gridCoordinatePlane(1,numberOfGridCoordinatePlanes)=dir; 
	    gridCoordinatePlane(2,numberOfGridCoordinatePlanes)=index;
            numberOfGridCoordinatePlanes++;
	  }
	}
        else
	  break;
      }
    }
    else if( answer=="mark a point" )
    {
      if( gi.isGraphicsWindowOpen() )
      {
	
	int pointList=gi.generateNewDisplayList();  // get a new display list to use
	assert(pointList!=0);

	getPlotBounds(gc,psp,xBound);
	gi.setGlobalBound(xBound);
	const real zLevel=zLevelFor2DGrids+.001*max(xBound(End,Range(0,1))-xBound(Start,Range(0,1)));
	for( ;; )
	{
	  int grid=-1, i1=0, i2=0, i3=0;
	  gi.inputString(answer,"Enter grid,i1,i2,i3 of a point to mark (grid<0 to end)");
	  glDeleteLists(pointList,1); // clear 

	  if( answer!="" )
	  {
	    sScanF(answer,"%i %i %i %i\n",&grid,&i1,&i2,&i3);
	    if( grid>=0 && grid<gc.numberOfGrids() )
	    {
	      MappedGrid & c = gc[grid];

              c.update(MappedGrid::THEcenter);  // could handle rectangular grids *****

	      const IntegerArray & d = c.dimension();
	      const realArray & center = c.center();
	    
	      i1=max(d(0,0),min(d(1,0),i1));
	      i2=max(d(0,1),min(d(1,1),i2));
	      i3=max(d(0,2),min(d(1,2),i3));
	      printF("Marking point (%i,%i,%i) on grid %i (%s) as a black mark\n",i1,i2,i3,grid,
		     (const char *)c.getName());
	    
	      glNewList(pointList,GL_COMPILE);
	      gi.setColour(GenericGraphicsInterface::textColour);
	      glPointSize(5.*gi.getLineWidthScaleFactor());   
	      glBegin(GL_POINTS);  

	      if( gc.numberOfDimensions()==2 )
		glVertex3(center(i1,i2,i3,axis1),
			  center(i1,i2,i3,axis2),zLevel);  // raise up above other interp points.
	      else
		glVertex3(center(i1,i2,i3,axis1),
			  center(i1,i2,i3,axis2),center(i1,i2,i3,axis3));
	      glEnd();
	      glEndList(); 

	      gi.redraw();
	    }
	    else
	    {
	      break;
	    }
	  }
	}
      }
    }
//                          0123456789012345678901234567890123
    else if( answer=="plot nodes" )
    {
      plotNodes=TRUE;
    }
    else if( answer=="do not plot nodes" )
    {
      plotNodes=FALSE;
    }
//                          0123456789012345678901234567890123
    else if( answer=="plot edges" )
    {
      plotEdges=TRUE;
    }
    else if( answer=="do not plot edges" )
    {
      plotEdges=FALSE;
    }
    else if( answer=="plot next coordinate plane" || answer=="plot previous coordinate plane" )
    {
      // shift all coordinate planes by +1 or -1
      // periodic wrap at the ends 
      int increment = answer=="plot next coordinate plane" ? 1 : -1;
      for( int plane=0; plane<numberOfGridCoordinatePlanes; plane++ )
      {
        int grid=gridCoordinatePlane(0,plane);
	int axis=gridCoordinatePlane(1,plane);
        if( grid>=0 && grid<numberOfGrids && axis>=0 && axis<gc.numberOfDimensions() )
	{
          gridCoordinatePlane(2,plane)+=increment;
	  if( gridCoordinatePlane(2,plane) < gc[grid].dimension(Start,axis) )
            gridCoordinatePlane(2,plane)=gc[grid].dimension(End,axis);
	  else if( gridCoordinatePlane(2,plane)>gc[grid].dimension(End,axis) )
            gridCoordinatePlane(2,plane)=gc[grid].dimension(Start,axis);
	}
      }
    }
//                          0123456789012345678901234567890123456789
    else if( answer(0,31)=="plot boundary condition (toggle)" )
    {
      int bcI, bcState;
      sScanF(&answer[32], "%i %i", &bcI, &bcState);
      
      int bcIndex =bcNumber(bcI, boundaryConditionList, numberOfBoundaryConditions);
      gridBoundaryConditionOptions(bcIndex)=bcState;

      if (bcI == 0)
      {
	plotNonPhysicalBoundaries= bcState;
	dialog.setToggleState("plot non-physical boundaries",plotNonPhysicalBoundaries);
      }
      
      if (bcI == -1)
      {
	plotBranchCuts= bcState;
	dialog.setToggleState("plot branch cuts",plotBranchCuts);
      }

    }
//                          0123456789012345678901234567890123
//                          012345678901234567890123456789012345678901
    else if( answer(0,40)=="plot grid lines on boundaries (3D) toggle" )
    {
      psp.plotLinesOnGridBoundaries=!psp.plotLinesOnGridBoundaries;      
      for( grid=0; grid<numberOfGrids; grid++ )
        gridOptions(grid)^=GraphicsParameters::plotBoundaryGridLines;   // exclusive or
    }
//                          0123456789012345678901234567890123
    else if( answer(0,33) =="raise the grid by this amount (2D)" )
    {
      real newZLevel;
      int nRead = sScanF(&answer[34],"%e",&newZLevel);
      if (nRead < 1)
      {
// couldn't read a real number
	gi.outputString("ERROR: invalid input");
// write back the default value in the text string
	sPrintF(buff, "%g", psp.zLevelFor2DGrids);
	dialog.setTextLabel(0, buff); // This is text label # 0
      }
      else
      {
	psp.zLevelFor2DGrids = newZLevel;
      }
    }
//                           01234567890123456789
    else if( answer(0,15) =="plot ghost lines" )
    {
      int newNGhost;
      int nRead = sScanF(&answer[16],"%i",&newNGhost);
      if (nRead < 1 || newNGhost < 0)
      {
        // couldn't read a real number
	gi.outputString("ERROR: invalid input");
        // write back the default value in the text string
	sPrintF(buff, "%i", psp.numberOfGhostLinesToPlot);
	dialog.setTextLabel("plot ghost lines", buff); // This is text label # 0
      }
      else
      {
	psp.numberOfGhostLinesToPlot = newNGhost;
      }
    }
    else if( answer(0,21) =="plot a multigrid level" )
    {
      int newMGL=-1;
      int nRead = sScanF(&answer[22], "%i", &newMGL);
      if (nRead < 1 || newMGL <0 || newMGL >= gc.numberOfMultigridLevels())
      {
        // couldn't read a real number
	gi.outputString("ERROR: invalid input");
        // write back the default value in the text string
	sPrintF(buff, "%i", multigridLevelToPlot);
	dialog.setTextLabel("plot a multigrid level", buff); // This is text label # multiGridIndex
      }
      else
      {
	multigridLevelToPlot = newMGL;
        if( multigridLevelToPlot!=0 )
	{
	  // these geometry arrays are needed:
	  GridCollection & cg = gc.multigridLevel[multigridLevelToPlot];
	  // these geometry arrays are needed:
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    if( cg[grid].isRectangular() )  // *wdh* 020406 : no need for vertex array now, if rectangular.
	      cg[grid].update(MappedGrid::THEmask ); 
	    else
	      cg[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
	  }
	}
      }
    }
    else if( answer=="reset all grid options" )
    {
      // reset grid options
      if( gc.numberOfDimensions() == 3 )
      {
        // sync the shadedsurfacegrid to the unstructured faces
	psp.plotShadedSurfaceGrids = psp.plotUnsFaces = true;     
        // sync the unstructured boundary edges and  the boundary grid lines to the grid lines 
	psp.plotUnsBoundaryEdges =  psp.plotLinesOnGridBoundaries = psp.plotGridLines =true;
      }
      else // in 2-D, the unstructured faces give the grid lines
      {
	psp.plotUnsFaces =  psp.plotLinesOnGridBoundaries = psp.plotGridLines =true;
      }      

      gridsToPlot=GraphicsParameters::toggleSum; 

      for( grid=0; grid<numberOfGrids; grid++ )
      {
        gridOptions(grid)=GraphicsParameters::plotGrid;
        gridOptions(grid)|=GraphicsParameters::plotInterpolation;

	if( psp.plotGridBlockBoundaries )
	  gridOptions(grid)|=GraphicsParameters::plotBlockBoundaries;
	if( psp.plotLinesOnGridBoundaries )
	  gridOptions(grid)|=GraphicsParameters::plotBoundaryGridLines;
	if( psp.plotShadedSurfaceGrids )
	  gridOptions(grid)|=GraphicsParameters::plotShadedSurfaces;
      	if( psp.plotBackupInterpolationPoints )
	  gridOptions(grid)|=GraphicsParameters::plotBackupInterpolation;
        if( psp.plotInteriorBoundaryPoints )
	  gridOptions(grid)|=GraphicsParameters::plotInteriorBoundary;

        gridOptions(grid)|=GraphicsParameters::plotInteriorGridLines;


	int value=gridsToPlot(grid)&GraphicsParameters::toggleGrids;
	dialog.getPulldownMenu("View").setToggleState(grid,value);

	value=gridsToPlot(grid)&GraphicsParameters::plotShadedSurfaces; 
	dialog.getPulldownMenu("3D-Shade").setToggleState(grid,value);
      }

      for( int i=0; i<numberOfBoundaryConditions; i++ )
      {
	if( boundaryConditionList(i)>0 )
	  gridBoundaryConditionOptions(i)=1;
	else 
	  gridBoundaryConditionOptions(i)=0;
        dialog.getPulldownMenu("3D-Shade").setToggleState(i,gridBoundaryConditionOptions(i));
      }
      
    }
    else if( answer=="keep aspect ratio" )
    {
      psp.keepAspectRatio=true;
      gi.setKeepAspectRatio(psp.keepAspectRatio); 
    }
    else if( answer=="do not keep aspect ratio" )
    {
      psp.keepAspectRatio=false;
      gi.setKeepAspectRatio(psp.keepAspectRatio); 
    }
    else if( (len=answer.matches("point size")) )
    {
      real value;
      sScanF(answer(len,answer.length()-1),"%f",&value);
      psp.pointSize=value;
      dialog.setTextLabel("point size",sPrintF(buff, "%3.0f pixels",psp.pointSize));
    }
    else if( answer== "point size" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the point size (current = %f pixels)",psp.pointSize)); 
      if( answer2!="" )
        sScanF(answer2,"%e",&psp.pointSize);
    }
    else if( answer=="boundary line width" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the width of boundary lines (current = %f pixels)",
                     boundaryLineWidth)); 
      if( answer2!="" )
        sScanF(answer2,"%e",&boundaryLineWidth);
    }
    else if( answer=="boundary vertical offset" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the vertical offset of the boundary lines (current = %f)",
                     boundaryVerticalOffset)); 
      if( answer2!="" )
        sScanF(answer2,"%e",&boundaryVerticalOffset);
    }
    else if( (len=answer.matches("coarsening factor")) ) 
    {
      sScanF(answer(len,answer.length()-1),"%i",&gi.gridCoarseningFactor);
      dialog.setTextLabel("coarsening factor",sPrintF(answer,"%i",gi.gridCoarseningFactor));

      printF("=================================INFO====================================================\n"
             "The coarsening factor is used to plot grid lines at a reduced resolution,\n"
             "which may be required for very fine grids to avoid using too much memory.\n"
             "The coarsening factor may be any positive or negative integer.\n"
             " o Setting this coarsening-factor to `2', for example, will cause the plotter\n"
             "   to use every other grid point (except near boundaries or interpolation points).\n"
             "   Choosing a value of `3' will use every third grid point. \n"
             "=========================================================================================\n");

      printF("NOTE: turn on `compute coarsening factor' if you want to recompute the coarsening factor.\n");
      psp.computeCoarseningFactor=false;  // do not recompute the coarsening factor
      dialog.setToggleState("compute coarsening factor",(int)psp.computeCoarseningFactor);
    }
    else if( (len=answer.matches("xScale, yScale, zScale")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&psp.xScaleFactor,&psp.yScaleFactor,&psp.zScaleFactor);
      printF("New values are xScale = %g, yScale = %g, zScale = %g \n",psp.xScaleFactor,psp.yScaleFactor,
              psp.zScaleFactor);
      dialog.setTextLabel("xScale, yScale, zScale",sPrintF(answer,"%g %g %g",psp.xScaleFactor,
                          psp.yScaleFactor,psp.zScaleFactor));
    }
    else if( dialog.getTextValue(answer,"displacement scale factor","%e",psp.displacementScaleFactor) )
    {
      printF("INFO: The displacement scale factor applies only when the `displacement' is plotted.\n"
             "      You will have to exit this menu and replot to see the new displacement scale factor"
             " take effect. displacementScaleFactor=%9.3e\n",psp.displacementScaleFactor);
    }
    else if( answer=="change colours" )
    { 
      // AP: is this ever used? WDH: yes
      // change the colours that appear in the colourNames array
      aString *menu2 = new aString[GenericGraphicsInterface::numberOfColourNames+2];
      for(;;)
      {
  	for( i=0; i<GenericGraphicsInterface::numberOfColourNames; i++ )
  	{
 	  menu2[i]=sPrintF(buff,"%i : (%s)",i, (const char*) gi.getColourName(i) );
  	}
  	menu2[GenericGraphicsInterface::numberOfColourNames]="exit this menu";
  	menu2[GenericGraphicsInterface::numberOfColourNames+1]="";   // null string terminates the menu
	int colourNumber=gi.getMenuItem(menu2, answer2, "change which colour?");
	if( answer2=="exit this menu" )
	  break;
  	else 
  	{
	  aString answer3 = gi.chooseAColour();
	  if( answer3!="no change" )
	    gi.setColourName(colourNumber, answer3);
  	}
      }
      delete [] menu2;

    }
    else if( answer=="output amr debug file" )
    {
      aString gridName="gridFromPlotStuff";
      Regrid::outputRefinementInfo( gc, gridName,"gridAMRDebug.cmd" );
      printF("Wrote file gridAMRDebug.cmd which can be used as input to the `refine' test program\n");
    }
    else if( answer=="lighting (toggle)" )
    {
      gi.outputString("This function is obsolete and is replaced by the Set View Characteristics dialog");
      
    }
    else if( answer=="edit the mapping of a grid" )
    {
      int gridChoice=-1;
      gi.inputString(answer2,"Edit the mapping with which grid?");
      if( answer2!="" )
        sScanF(answer2,"%i",&gridChoice);
      if( gridChoice>=0 && gridChoice <gc.numberOfComponentGrids() )
      {
	MappingRC & map = gc[gridChoice].mapping();
        MappingInformation mapInfo;
	mapInfo.graphXInterface=&gi;
        map.update(mapInfo);
      }
      else
      {
        printF("ERROR: invalid grid=%i, valid values are in [0,%i]\n",
               gridChoice,gc.numberOfComponentGrids()-1);
        gi.stopReadingCommandFile();
      }
    }
    else if( answer=="exit this menu" || answer=="exit" )
    {
      break;
    }
    else if( answer=="erase" )
    {
      plotObject=FALSE;
      if( plotOnThisProcessor )
      {
	if (glIsList(list)) glDeleteLists(list,1);
	if (glIsList(lightList)) glDeleteLists(lightList,1);
      }
//        if( labelGridsAndBoundaries && glIsList(getColouredSquaresDL(currentWindow)) )
//          glDeleteLists(getColouredSquaresDL(currentWindow),1);  // clear the squares        
      gi.redraw();
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
      if( plotOnThisProcessor )
      {
	if (glIsList(list)) glDeleteLists(list,1);
	if (glIsList(lightList)) glDeleteLists(lightList,1);
      }
//        if( labelGridsAndBoundaries && glIsList(getColouredSquaresDL(currentWindow)) )
//          glDeleteLists(getColouredSquaresDL(currentWindow),1);  // clear the squares        
      gi.redraw();
      break;
    }
    else if( answer=="pick colour..." )
    {
      colourDialog.showSibling();
      if( pickingOption!=pickToColourGrids && pickingOption!=pickToColourBoundaries )
      {
	printF("INFO: setting picking option to `pick to colour grids'.\n");
	pickingOption=pickToColourGrids;
	gui.getOptionMenu("Pick to:").setCurrentChoice((int)pickingOption);
      }
    }
    else if( answer=="close colour choices" )
    {
      colourDialog.hideSibling();
    }
    else if( PlotIt::getColour( answer,colourDialog,pickColour ) )
    {
      printF("answer=%s was processed by the colourDialog\n",(const char*)answer);
      pickColourIndex=getXColour(pickColour);
      if( pickColourIndex==0 )
      {
	printF(" ERROR: colour=[%s] not recognized! using aquamarine instead\n",(const char*)pickColour);
	pickColourIndex=getXColour("aquamarine");
      }
    }
    else if( pickingOption==pickToColourGrids &&  (select.active || select.nSelect ) )
    {
      if( boundaryColourOption!=GraphicsParameters::colourByIndex )
      {
	printF("INFO: changing `Colour boundaries by' option to colour by `Chosen name'\n");
      }
      boundaryColourOption=GraphicsParameters::colourByIndex;
      dialog.getOptionMenu(0).setCurrentChoice(2);

      const aString pickColour=getXColour(pickColourIndex);
      if( numberOfGrids>psp.gridColours.getLength(0) )
      {
	int gStart=max(0,psp.gridColours.getLength(0));
	psp.gridColours.resize(gc.numberOfComponentGrids(),8);
	// Set initial names for all grids
	for( int g=gStart; g<gc.numberOfComponentGrids(); g++ )
	  psp.gridColours(g,all)=getXColour(gi.getColourName((g % GenericGraphicsInterface::numberOfColourNames)));
      }
      if( pickClosest )
      {
        printF("Look for the closest item picked...(toggle `pick closest' to choose all items picked)\n");
	for( int grid=0; grid<numberOfGrids; grid++ )
	{
	  if( gc[grid].getGlobalID()==select.globalID )
	  {
	    printF("Colour grid %i (%s) to %s\n",grid,(const char*)gc[grid].getName(),(const char*)pickColour);
	    psp.gridColours(grid,all)=pickColourIndex;
	    
	    gi.outputToCommandFile(sPrintF(answer,"grid colour %i %s\n",grid,(const char*)pickColour));
	    break;
	  }
	}
      }
      else
      {
        printF("Look for the all items picked... (toggle `pick closest' to only choose the closest)\n");
	for( int grid=0; grid<numberOfGrids; grid++ )
	{
	  for( int i=0; i<select.nSelect; i++ )
	  {
	    if( gc[grid].getGlobalID()==select.selection(i,0) )
	    {
	      printF("Colour grid %i (%s) to %s\n",grid,(const char*)gc[grid].getName(),(const char*)pickColour);
	      psp.gridColours(grid,all)=pickColourIndex;
	    
	      gi.outputToCommandFile(sPrintF(answer,"grid colour %i %s\n",grid,(const char*)pickColour));
	      break;
	    }
	  }
	}
      }
    }
//      else if( (len=answer.matches("pick colour")) )
//      {
//        aString colour;
//        colour=answer(len,answer.length()-1);
//        pickColourIndex=getXColour(colour);
//        if( pickColourIndex==0 )
//        {
//  	printf(" ERROR: colour=[%s] not recognized! using aquamarine instead\n",(const char*)colour);
//  	pickColourIndex=getXColour("aquamarine");
//        }
//        dialog.getOptionMenu("Pick colour").setCurrentChoice(pickColourIndex);
//      }
    else if( answer.matches("grid colour") || answer.matches("grid boundary colour (side,axis,grid,colour):") )
    {
      gi.outputString("Choose colours for grid. Select `colour grids by chosen name' to use these colours");

      boundaryColourOption=GraphicsParameters::colourByIndex;
      dialog.getOptionMenu(0).setCurrentChoice(2);

      int side=-1, axis=-1, grid=-100;
      char colour[100];
      if( (len=answer.matches("grid colour")) )
      {
	sScanF(answer(len,answer.length()-1),"%i %s",&grid,colour);
      }
      else if( (len=answer.matches("grid boundary colour (side,axis,grid,colour)):")) )
      {
        sScanF( &answer[len],"%i %i %i %s",&side,&axis,&grid,colour);
	if( side<0 || side>1 || axis<0 || axis>=gc.numberOfDimensions() )
	{
	  printF("ERROR:grid boundary colour: Invalid value for (side,axis)=(%i,%i)\n",side,axis);
          printF("answer=[%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	  continue;
	}
      }
      else
      {
	Overture::abort("Unknown answer!");
      }
      
      int index=getXColour(colour);
      if( index==0 )
      {
	printF(" ERROR: colour=[%s] not recognized! using blue instead\n",(const char*)colour);
	index=getXColour("blue");
      }
      printF(" Setting grid=%i colour=%s (X colour index=%i)\n",grid,(const char*)colour,index);

      if( grid<-1 || grid>=gc.numberOfComponentGrids() )
      {
	printF("Invalid grid = %i. Should be -1 for all or between 0...%i\n",grid,gc.numberOfComponentGrids()-1);
	gi.stopReadingCommandFile();
	continue;
      }
     
      Range G(grid,grid);
      if( grid==-1 )
      {
	G=gc.numberOfComponentGrids();
	grid=gc.numberOfComponentGrids()-1;
      }
      
      if( grid>=psp.gridColours.getLength(0) )
      {
	int gStart=max(0,psp.gridColours.getLength(0));
	psp.gridColours.resize(gc.numberOfComponentGrids(),8);
	// Set initial names for all grids
	for( int g=gStart; g<gc.numberOfComponentGrids(); g++ )
	  psp.gridColours(g,all)=getXColour(gi.getColourName((g % GenericGraphicsInterface::numberOfColourNames)));
      }
      if( side==-1 )
	psp.gridColours(G,all)=index;
      else
       psp.gridColours(G,side+2*(axis))=index;
    }
    else if( answer=="show all grids" )
    {
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	int gridIsOn=gridsToPlot(grid)&GraphicsParameters::toggleGrids;
	dialog.getPulldownMenu("View").setToggleState(grid,gridIsOn);
        if( !gridIsOn )
          gridsToPlot(grid)^=GraphicsParameters::toggleGrids;
	
      }
    }
    else if( answer=="show all faces" )
    {
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
	    if( gridOptions(grid)&faceToToggle[side+2*axis] ) // this means the face is NOT plotted.
	      gridOptions(grid) ^= faceToToggle[side+2*axis];        
	  }
	}
      }
      
    }
    else if( answer=="print grid statistics" )
    {
      if( gc.getClassName()=="CompositeGrid" )
      {
	CompositeGrid & cg0 = (CompositeGrid &)gc;
	GridStatistics::printGridStatistics(cg0);
      }
      else
      {
	GridStatistics::printGridStatistics(gc);
      }
    }
    else if( answer=="plot grid quality" )
    {
      printF("plot the grid quality for grid=0 **** fix this ***");
      PlotIt::plotGridQuality(gi,gc);
    }
    else if( answer=="refinement grid colours" )
    {
      gi.outputString("Choose colours for refinement grids. Select `colour grids by chosen name' to use these colours");
      for( ;; )
      {
	gi.inputString(answer,"Enter base grid number, refinement level and colour name. Example `0 2 green'. "
                              "(Enter -1 for all grids or all levels) Enter `done' when finished");
	if( answer=="done" || answer=="exit" )
	{
	  break;
	}
	else
	{
          int grid=-1;
          int refinementLevel=-1;
	  char colour[100];
	  sScanF(answer,"%i %i %s",&grid,&refinementLevel,colour);
          const int index=getXColour(colour);

          if( grid>=gc.numberOfComponentGrids() )
	  {
            printF("Invalid grid number = %i. Should be between 0...%i\n",grid,gc.numberOfComponentGrids()-1);
	    gi.stopReadingCommandFile();
	    continue;
	  }
          if( refinementLevel>=gc.numberOfRefinementLevels() )
	  {
            printF("Invalid refinementLevel = %i. Should be between 0...%i\n",grid,refinementLevel,
                   gc.numberOfRefinementLevels()-1);
	    gi.stopReadingCommandFile();
	    continue;
	  }
	  if( gc.numberOfComponentGrids()>=psp.gridColours.getLength(0) )
  	  {
	    int gStart=max(0,psp.gridColours.getLength(0));  // done change existing names
	    psp.gridColours.resize(gc.numberOfComponentGrids(),8);
  	    // Set initial names for all grids
  	    for( int g=gStart; g<gc.numberOfComponentGrids(); g++ )
    	      psp.gridColours(g,all)=getXColour(gi.getColourName((g % GenericGraphicsInterface::numberOfColourNames)));
  	  }
          for( int g=0; g<gc.numberOfGrids(); g++ )
	  {
	    if( ( grid<0 || gc.baseGridNumber(g)==gc.baseGridNumber(grid)) &&
                ( refinementLevel<0 || gc.refinementLevelNumber(g)==refinementLevel) )
	    {
	      printF(" Setting grid=%i refinementLevel=%i to colour=%s (X colour index=%i)\n",
		     g,gc.refinementLevelNumber(g),(const char*)colour,index);
	      psp.gridColours(g,all)=index;
	    }
	  }
	}
      }
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else
    {
      #ifndef USE_PPP
        printF("plot grid: unknown response = %s\n",(const char*)answer);
      #else
        printF("plot grid: myid=%i, Unknown response = %s\n",myid,(const char*)answer);
        fflush(0);
      #endif
      gi.stopReadingCommandFile();
    }

//    GLenum errCode;
//    const GLubyte *errString;
  
    // *wdh* 070910 if( plotObject && (gi.isGraphicsWindowOpen() || PlotIt::parallelPlottingOption==1 ) ) // *wdh* only plot if window is open
    if( plotObject && gi.isInteractiveGraphicsOn() )
    {
      gi.setAxesDimension(gc.numberOfDimensions());

      if( plotGrid )
      {
// AP: We need to make new lists every time, since the user might be plotting in a different window
// and the display lists are not shared between windows.
	if( plotOnThisProcessor )
	{
	  if (glIsList(list)) glDeleteLists(list,1);
	  list=gi.generateNewDisplayList(0);  // get a new display list without lighting

	  if (glIsList(lightList)) glDeleteLists(lightList,1);
	  lightList=gi.generateNewDisplayList(1);  // get a new display list with lighting
	}
	
	numberList=-1;
	int number=-1;

	Index I1,I2,I3;
	int i1,i2,i3;

        // get Bounds on the grids that we plot
        getPlotBounds(gc,psp,xBound);
	gi.setGlobalBound(xBound);

	if( gc.numberOfDimensions()==3 )
	{ // Plot a 3D grid
//  	  if( multigridLevelToPlot==0 )
//  	    grid3d(gi, gc, psp, boundaryConditionList, numberOfBoundaryConditions,
//  		   numberList, number, list, lightList);
//  	  else
//  	    grid3d(gi, gc.multigridLevel[multigridLevelToPlot], psp, 
//  		   boundaryConditionList, numberOfBoundaryConditions, numberList, number, list, lightList);

	  if ( gc.numberOfGrids()>0 && gc[0].domainDimension()==2 )
	    surfaceGrid3d(gi, gc, psp, boundaryConditionList, numberOfBoundaryConditions, 
			  numberList, number,list, lightList);
	  else if( multigridLevelToPlot==0 )
	    grid3d(gi, gc, psp, boundaryConditionList, numberOfBoundaryConditions, 
		   numberList, number, list, lightList);
	  else 
	    grid3d(gi, gc.multigridLevel[multigridLevelToPlot], psp, boundaryConditionList, 
                   numberOfBoundaryConditions, numberList, number, list, lightList);

          if( hybridSurfaceGridExists && plotHybridGrid )
	  {
	    printF(" *** plot the hybrid surface grid ***\n");
	    UnstructuredMapping & hybridSurfaceGrid = *((CompositeGrid&)gc).getSurfaceStitching();
	    int plotAndExit;
	    psp.get(GI_PLOT_THE_OBJECT_AND_EXIT,plotAndExit);
	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

	    psp.set(GI_PLOT_UNS_FACES,true);  // plot shaded faces
	    psp.set(GI_PLOT_UNS_EDGES,true);  // plot triangle edges
	    
	    PlotIt::plotUM(gi, hybridSurfaceGrid, psp);

	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,plotAndExit); // reset
	     
	  }
	  
	}
	else
	{ // Plot a 1D or 2D grid. No lighting in this case => Everything goes into the unlit display list.
	  if( plotOnThisProcessor )
            glNewList(list,GL_COMPILE);

          // **** plot2dGrid( ... );
          plotGrid2d(gi,gc,psp,xBound,multigridLevelToPlot,numberList, number);

          GridCollection & gcl = multigridLevelToPlot==0 ? gc : gc.multigridLevel[multigridLevelToPlot];
          numberOfGrids = gcl.numberOfComponentGrids();


	  // Now plot the boundaries. Base the colour on the boundary condition, the share value
	  // or the grid number
	  if (psp.plotGridBlockBoundaries)
	  {
	    plotGridBoundaries(gi, gcl, boundaryConditionList, numberOfBoundaryConditions, 
			       1, zLevelFor2DGrids+boundaryVerticalOffset, psp);
	  }
	  
	  for( grid=0; grid<numberOfGrids; grid++ )
	  {
	    int thisBC;
	    ForBoundary(side,axis)
	    {
	      thisBC = gcl[grid].boundaryCondition()(side,axis);
	      if( (thisBC== 0 && plotNonPhysicalBoundaries) ||
		  thisBC>0)
	      {
		if( boundaryColourOption==GraphicsParameters::colourByBoundaryCondition ||
                    boundaryColourOption==GraphicsParameters::defaultColour )
		  numberList(++number)=gcl[grid].boundaryCondition()(side,axis);
                else if( boundaryColourOption==GraphicsParameters::colourByShare )
		  numberList(++number)=gcl[grid].sharedBoundaryFlag()(side,axis);
		else if( boundaryColourOption==GraphicsParameters::colourByRefinementLevel )
		  numberList(++number)=gcl.refinementLevelNumber(grid);
		else if( boundaryColourOption==GraphicsParameters::colourByDomain )
		  numberList(++number)=gcl.domainNumber(grid);
		else if( boundaryColourOption==GraphicsParameters::colourByGrid )
		  numberList(++number)=grid;
	      }
	    }
	  }

	  if( plotOnThisProcessor ) glEndList(); 
	} // end plot 1D or 2D
	
      } // end if plotGrid...

      // plot labels on top and bottom
      if( psp.plotTitleLabels && plotOnThisProcessor )
      {
	gi.plotLabels( psp );
      }

      // -------------------------Label Colours------------------------------------
      // Draw a coloured square with the number inside it for each of the colours
      // shown on the plot 
      if( plotOnThisProcessor ) // *wdh* always plot these 080423
      {
	gi.drawColouredSquares(numberList,psp);
      }

      gi.redraw();
      plotGrid=true;
    }  // end plotObject
  }

//  if( numberOfGhostLinesToPlot> 0 )
//    setMaskAtGhostPoints(gc,numberOfGhostLinesToPlot,1); // reset values

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  // this indicates that the object appears on the screen (not erased)

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }
  delete [] pdCommand1;
  delete [] pdLabel1;
  delete [] initState1;

  delete [] pdCommand2;
  delete [] pdLabel2;
  delete [] initState2;

  delete [] pdCommand3;
  delete [] pdLabel3;
  delete [] initState3;
  
} // end plot(GridCollection)

    
#undef FOR_3
