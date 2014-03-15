#include "Regrid.h"
#include "Overture.h"  
#include "PlotStuff.h"
#include "BoxLib.H"
#include "Box.H"
#include "BoxList.H"
#include "display.h"
#include "conversion.h"
#include "SquareMapping.h"
#include "MatrixTransform.h"

#include "RotatedBox.h"
#include "ListOfRotatedBox.h"
#include "GenericDataBase.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)



static bool limitBoxSize=true;  // put this here for now

// void printInfo( GridCollection & cg, int option=0 );

// static int piab[6]={0,0,0,0,0,0};

#define iab(side,axis) piab[(side)+2*(axis)]


// If the base-grid index-space cannot be evenly divided by indexCoarseningFactor, then
// we add add any extra cells to the first interval of the coarse index-space.
// There will be at most (indexCoarseningFactor-1) extra cells 
// 
// *** indexCoarseningFactor=2 ***
//  0        1     2     3     4     5     6     7 
//  +--------+-----+-----+-----+-----+-----+-----+   coarse index-space 
//  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   index-space
//  0  1  2  3  4  5  ...                        N=15
//  X  * iab(0,axis)=1
//  X=iab(1,axis)
//
// *** indexCoarseningFactor=4 ***
//  0                    1           2           3 
//  +--------------------+-----------+-----------+   coarse index-space 
//  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   index-space
// -1  0  1  2  3  4  5  6  7  8  9 10           N=14
//  X        * iab(0,axis)=2         
//  X=iab(1,axis)


inline int Regrid::coarsenIndexLower( int i, int axis ) const 
// convert a lower bound index to the coarsened index-space
{
  if( !useCoarsenedIndexSpace ) 
    return i;
  else
    return max(0, (i - iab(0,axis))/indexCoarseningFactor);
}

inline int Regrid::coarsenIndexUpper( int i, int axis ) const 
// convert an upper bound index to the coarsened index-space
{
  if( !useCoarsenedIndexSpace ) 
    return i;
  else if( i>iab(1,axis) )
    return max( 1, (i - iab(0,axis)+indexCoarseningFactor-1)/indexCoarseningFactor);
  else
    return 0;
}

inline int Regrid::refineIndex( int i, int axis ) const 
// Convert a coarse index-space value to the normal index-space.
// NOTE: i=0 is a special case 
{
  if( !useCoarsenedIndexSpace ) 
    return i;
  else if( i>0 )
    return i*indexCoarseningFactor + iab(0,axis);
  else
    return iab(1,axis);
}



static bool splitBoxesAtBranchCut=true;

//\begin{>RegridInclude.tex}{\subsection{Constructor}} 
Regrid::
Regrid()
//=========================================================================================
// /Description:
//     Use this class to build adaptive mesh refinement grids.
//
//\end{RegridInclude.tex} 
//=========================================================================================
{
  myid= max(0,Communication_Manager::My_Process_Number);

  efficiency=.7;
  refinementRatio=2;
  defaultNumberOfRefinementLevels=2;
  
  // numberOfBufferZones : minimum distance between error-points and edge of the refinement grid 
  numberOfBufferZones=1;  
  // widthOfProperNesting : number of coarse grid cells separating the boundary of `level' from `level-1'
  widthOfProperNesting=1;

  // build amr grids on an index space that is coarsened by this amount: this will increase the size of the
  // smallest possible refinement grid.
  indexCoarseningFactor=1;
  useCoarsenedIndexSpace=false;
  
  iab(0,0)=iab(1,0)=iab(0,1)=iab(1,1)=iab(0,2)=iab(1,2)=0;
  
  minimumBoxSize=100; // *wdh* 030818  = 16;
  minimumBoxWidth=5;  // This does work if we rebuild all levels at once. 
  
  useSmartBisection=true;
  gridAdditionOption=addGridsAsRefinementGrids;
  gridAlgorithmOption=aligned; // rotated; // aligned;
  
  properNestingDomain=NULL;
  complementOfProperNestingDomain=NULL;
  mergeBoxes=true;
  maximumNumberOfSplits=INT_MAX;
  
  timeForRegrid=0.;
  timeForBuildGrids=0.;
  timeForBuildTaggedCells=0.;

  numberOfDimensions=0;

  loadBalance=true;  // *wdh* 070706 load-balancing is now on by default
   
  debug=0;
}

Regrid::
~Regrid()
{
}

//\begin{>>RegridInclude.tex}{\subsection{getDefaultNumberOfRefinementLevels}}
int Regrid::
getDefaultNumberOfRefinementLevels() const
//=========================================================================================
// /Description:
//    Return the default number of refinement levels.
//\end{RegridInclude.tex} 
//=========================================================================================
{
  return defaultNumberOfRefinementLevels;
}

//\begin{>>RegridInclude.tex}{\subsection{getRefinementRatio}}
int Regrid::
getRefinementRatio() const
//=========================================================================================
// /Description:
//    Return the refinement ratio.
//\end{RegridInclude.tex} 
//=========================================================================================
{
  return refinementRatio;
}

//\begin{>>RegridInclude.tex}{\subsection{loadBalancingIsOn}} 
bool Regrid::
loadBalancingIsOn() const
// =======================================================================================
// /Description:
//    Return true is load balancing is turned on
//\end{RegridInclude.tex} 
{
  return loadBalance;
}

//\begin{>>RegridInclude.tex}{\subsection{outputRefinementInfo}} 
int Regrid::
outputRefinementInfo( GridCollection & gc, 
                      const aString & gridFileName, 
		      const aString & fileName )
// =======================================================================================
// /Description:
//   This function will output a command file for the "refine" test code.
// /gc(input) : name of the grid.
// /refinementRatio (input) : refinement ratio.
// /gridFileName (input) : grid file name, such as "cic.hdf". This is not essential,
//    but then you will have to edit the comamnd file to add the correct name.
// /fileName (input) : name of the output command file, such as "bug.cmd"
// The output will be a file of the form
// \begin{verbatim}
//  * Add a refinement grid using values: ([P0,P1] = processor range)
//  *  baseGrid level i1a i1b i2a i2b i3a i3b ratio P0 P1
// choose a grid
//   cic.hdf
// add a refinement
//   0 1 4 10 12 15 2
// add a refinement
//   0 1 3 10 15 19 2
// add a refinement
//   1 1 12 16 0 7  2
// add a refinement
//   1 1 16 20 3 7  2
// \end{verbatim}
//\end{RegridInclude.tex} 
// ========================================================================================
{
  printF("*** outputing a command file %s for refine ****\n",(const char*)fileName);
  
  FILE *file=fopen(fileName,"w");
  fprintf(file,"choose a grid\n"
	  " %s \n",(const char*)gridFileName);

  const int np= max(1,Communication_Manager::numberOfProcessors());
  fprintf(file,"* grid created with np=%i\n",np);
  
  int refinementRatio=2;
  if( gc.numberOfRefinementLevels()>1 )
    refinementRatio=gc.refinementLevel[1].refinementFactor(0,0);
  
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>0 )
    {
      MappedGrid & mg = gc[grid];
      Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
      const intSerialArray & processorSet = partition.getProcessorSet();

      fprintf(file,"add a refinement\n"
              " %i %i  %i %i %i %i %i %i %i %i %i\n",gc.baseGridNumber(grid),gc.refinementLevelNumber(grid),
              mg.gridIndexRange(0,0)/refinementRatio,mg.gridIndexRange(1,0)/refinementRatio,
              mg.gridIndexRange(0,1)/refinementRatio,mg.gridIndexRange(1,1)/refinementRatio,
              mg.gridIndexRange(0,2)/refinementRatio,mg.gridIndexRange(1,2)/refinementRatio,
              refinementRatio,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)) );
    }
  }
  fclose(file);
  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{setEfficiency}} 
void Regrid::
setEfficiency(real efficiency_ )
//=========================================================================================
// /Description:
//    Set the regridding efficiency, the ratio of tagged to un-tagged points.
// /efficiency\_ (input) : regridding efficiency.
//\end{RegridInclude.tex} 
//=========================================================================================
{
  efficiency=efficiency_;
  
}

//\begin{>>RegridInclude.tex}{\subsection{setIndexCoarseningFactor}} 
void Regrid::
setIndexCoarseningFactor(int factor)
//=========================================================================================
// /Description:
//   Build amr grids on an index space that is coarsened by this amount: this will increase the size of the
// smallest possible refinement grid. The smallest possible grid will have a width of factor*refinementRatio cells. 
// 
// /factor (input) : coarsening factor. 1=no coarsening. 2=coarsen grid by a factor of 2, 4=coarsen by a factor of 4.
//\end{RegridInclude.tex} 
//=========================================================================================
{
  indexCoarseningFactor=factor;
  if( true || indexCoarseningFactor!=1 ) // do this for now for testing
    useCoarsenedIndexSpace=true;   
  
}


//\begin{>>RegridInclude.tex}{\subsection{setGridAdditionOption}} 
void Regrid::
setGridAdditionOption( GridAdditionOption gridAdditionOption_ )
// ===================================================================================
//  /Description:
//    New grids can be added as refinement grids or as additional base grids.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  gridAdditionOption=gridAdditionOption_;
}

//\begin{>>RegridInclude.tex}{\subsection{getGridAdditionOption}} 
Regrid::GridAdditionOption Regrid::
getGridAdditionOption() const
// ===================================================================================
//  /Description:
//    Return the grid addition option.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  return gridAdditionOption;
}



//\begin{>>RegridInclude.tex}{\subsection{setGridAlgorithmOption}} 
void Regrid::
setGridAlgorithmOption( GridAlgorithmOption gridAlgorithmOption_ )
// ===================================================================================
//  /Description:
//    Specify the algorithm to use.
// /gridAlgorithmOption\_ (input) : one of {\tt aligned} or {\tt rotated}
//\end{RegridInclude.tex} 
// ===================================================================================
{
  gridAlgorithmOption=gridAlgorithmOption_;
}

void Regrid::
setMaximumNumberOfSplits( int num )
{
  maximumNumberOfSplits=num;
}



//\begin{>>RegridInclude.tex}{\subsection{setMergeBoxes}} 
void Regrid::
setMergeBoxes( bool trueOrFalse /* =true */ )
// ===================================================================================
//  /Description:
//     Indicate whether boxes should be merged.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  mergeBoxes=trueOrFalse;
}



//! Specify the minimum number of grid points on a refinement grid
/*!
   /param numberOfGridPoints: the minimum (total) number of grid points on a refinement grid
 */
void Regrid::
setMinimumBoxSize(int numberOfGridPoints)
{
  minimumBoxSize=numberOfGridPoints;
}

//! Specify the minimum width of a box (in coarse grid cells). The actual minimum width will be width*ratio. 
/*!
   /param width:
 */
void Regrid::
setMinimumBoxWidth(int width)
{
  minimumBoxWidth=width;  
}



//\begin{>>RegridInclude.tex}{\subsection{setNumberOfBufferZones}} 
void Regrid::
setNumberOfBufferZones( int numberOfBufferZones_ )
// =====================================================================================
// /Description: 
//    Specify the number of buffer zones to increase the tagged area by. The boundary
//  of refinement grids will be this number of {\bf coarse grid cells} away from the
//  boundary of the next coarser level.
//   Note that numberOfBufferZones>=1 since we always transfer node centred errors
//   to surrounding cells.
// /numberOfBufferZones\_ (input) :
//\end{RegridInclude.tex} 
// =====================================================================================
{
  if( numberOfBufferZones_<0 )
  {
    printF("Regrid::setNumberOfBufferZones:ERROR: improper value for numberOfBufferZones=%i\n",
	   numberOfBufferZones_);
  }
  else
    numberOfBufferZones=numberOfBufferZones_;
}

//\begin{>>RegridInclude.tex}{\subsection{setWidthOfProperNesting}} 
void Regrid::
setWidthOfProperNesting( int widthOfProperNesting_ )
// =====================================================================================
// /Description: 
//    Specify the number of buffer zones between grids on a refinement level and
//   grids on the next coarser level. The value for widthOfProperNesting should 
// be greater than or equal to zero.
//
//\end{RegridInclude.tex} 
// =====================================================================================
{
  if( widthOfProperNesting_<0 )
  {
    printF("Regrid::setwidthOfProperNesting:ERROR: improper value for widthOfProperNesting=%i\n",
	   widthOfProperNesting_);
  }
  else
    widthOfProperNesting=widthOfProperNesting_;
}

//\begin{>>RegridInclude.tex}{\subsection{setRefinementRatio}} 
void Regrid::
setRefinementRatio( int refinementRatio_ )
// ===================================================================================
//  /Description:
//     Set the refinement ratio.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  refinementRatio=refinementRatio_;
}


//\begin{>>RegridInclude.tex}{\subsection{setUseSmartBisection}} 
void Regrid::
setUseSmartBisection( bool trueOrFalse /* =true */ )
// ===================================================================================
//  /Description:
//     Indicate whether the smart biscection routine should be used.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  useSmartBisection=trueOrFalse;
}


//\begin{>>RegridInclude.tex}{\subsection{turnOnLoadBalacing}} 
void Regrid::
turnOnLoadBalacing( bool trueOrFalse /* =true */ ) 
// ===================================================================================
//  /Description:
//     Turn load balancing on or off. The grids are load balanced at the regrid stage.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  loadBalance=trueOrFalse;
}

//\begin{>>RegridInclude.tex}{\subsection{getLoadBalancer}} 
LoadBalancer & Regrid::
getLoadBalancer()
// ===================================================================================
//  /Description:
//     Return the Loadbalancer used by Regrid. You can change the parameters in this
//  object in order to adjust the load-balancing.  You should also call turnOnLoadBalacing.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  return loadBalancer;
}


// static const int MINOFF = 2;      // min width for a grid (grid cells)?
static const int CUT_THRESH = 2;  // ??

//\begin{>>RegridInclude.tex}{\subsection{findCut}} 
int Regrid::
findCut(int *hist, int lo, int hi, CutStatus &status)
// ===================================================================================
//  /Description:
//    Code taken from HAMR from LBL.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  const int MINOFF = minimumBoxWidth;

  status = invalidCut;
  int len = hi - lo + 1;

  // check validity of histogram
  if (len <= 1) return lo;

  // first find centermost point where hist == 0 (if any)
  int mid = len/2;
  int cutpoint = -1;
  int i;
//  for (  i = 0; i < len; i++) 
  for (  i = 0+MINOFF; i < len-MINOFF; i++) // *wdh* add this 070514
  {
    if (hist[i] == 0) 
    {
      status = holeCut;
      if (abs(cutpoint-mid) > abs(i-mid))
      {
	cutpoint = i;
	if (i > mid) break;
      };
    };
  };
  if (status == holeCut)
    return lo+cutpoint;

   // if we got here, there was no obvious cutpoint, try
   // finding place where change in second derivative is max
  int iprev = hist[0];
  int icur, inext;
  for (i = 1; i < len-1; i++)
  {
    icur = hist[i];
    inext = hist[i+1];
    hist[i] = inext - 2*icur + iprev;
    iprev = icur;
  };

  int locmax = -1;
  for(i = 0+MINOFF; i < len-MINOFF; i++) 
  {
    iprev = hist[i-1];
    icur = hist[i];
    int locdif = abs(iprev-icur);
    if ( (iprev*icur < 0) && (locdif >= locmax) ) 
    {
      if (locdif > locmax) 
      {
	status = steepCut;
	cutpoint = i;
	locmax = locdif;
      } else {
	// select location nearest center of range
	if (abs(i-mid) < abs(cutpoint-mid)) cutpoint = i;
      };
    };
  };

  if (locmax <= CUT_THRESH)
  {
    // just recommend a bisect cut
    cutpoint = mid;
    status = bisectCut;
  };

  return lo + cutpoint;
}



//\begin{>>RegridInclude.tex}{\subsection{getBox}} 
BOX Regrid::
getBox( const intArray & ia )
// ===================================================================================
//  /Description:
//     Build the smallest box that covers a list of tagged cells.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  // const int numberOfDimensions=ia.getLength(1);
  assert( numberOfDimensions>0 );
  
  int iva[3]={0,0,0}; //
  int ivb[3]={0,0,0}; //

  Range R=ia.getLength(0);

  int axis;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    iva[axis]=min(ia(R,axis));
    ivb[axis]=max(ia(R,axis));
  }
//   if( indexCoarseningFactor!=1 )
//   {
//     for( axis=0; axis<numberOfDimensions; axis++ )
//     {
//       iva[axis]=coarsenIndexLower(iva[axis],axis);
//       ivb[axis]=coarsenIndexUpper(ivb[axis],axis);
//     }
//   }
  
  // IndexType centering (D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE));
  IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));

  return BOX(INTVECT(iva[0],iva[1],iva[2]),INTVECT(ivb[0],ivb[1],ivb[2]),centering); 
}

#ifdef USE_PPP
//\begin{>>RegridInclude.tex}{\subsection{getBox}} 
BOX Regrid::
getBox( const intSerialArray & ia )
// ===================================================================================
//  /Description:
//     Build the smallest box that covers a list of tagged cells.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  // const int numberOfDimensions=ia.getLength(1);
  assert( numberOfDimensions>0 );
  
  int iva[3]={0,0,0}; //
  int ivb[3]={0,0,0}; //

  const int num=ia.getLength(0);
  Range R=num;

  int axis;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    if( num>0 )
    {
      iva[axis]=min(ia(R,axis));
      ivb[axis]=max(ia(R,axis));
    }
    else
    {
      iva[axis]= INT_MAX/2;
      ivb[axis]=-INT_MAX/2;
    }
    iva[axis]=ParallelUtility::getMinValue(iva[axis]);
    ivb[axis]=ParallelUtility::getMaxValue(ivb[axis]);
  }

//   if( indexCoarseningFactor!=1 )
//   {
//     for( axis=0; axis<numberOfDimensions; axis++ )
//     {
//       iva[axis]=coarsenIndexLower(iva[axis],axis);
//       ivb[axis]=coarsenIndexUpper(ivb[axis],axis);
//     }
//   }   

  if( debug & 2 ) 
     printF(" getBox: iva=[%i,%i,%i] ivb=[%i,%i,%i]\n",iva[0],iva[1],iva[2],ivb[0],ivb[1],ivb[2]);

  // IndexType centering (D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE));
  IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));

  return BOX(INTVECT(iva[0],iva[1],iva[2]),INTVECT(ivb[0],ivb[1],ivb[2]),centering); 
}

#endif

//\begin{>>RegridInclude.tex}{\subsection{buildBox}} 
BOX Regrid::
buildBox(Index Iv[3] )
//=========================================================================================
// /Description:
//    Build a box from 3 Index objects.
//\end{RegridInclude.tex} 
//=========================================================================================
{
  BOX box;
  box.convert(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));
  for( int axis=0; axis<3; axis++ )
  {
    box.setSmall(axis,coarsenIndexLower(Iv[axis].getBase(),axis));
    // To coarsen a cell-centered box:  (1) Add one to make node centered, (2) coarsen, (3) subtract 1
    box.setBig(axis,coarsenIndexUpper(Iv[axis].getBound()+1,axis)-1);
  }
  return box;
}

//\begin{>>RegridInclude.tex}{\subsection{getBoundedBox}} 
BOX Regrid::
getBoundedBox( const intSerialArray & ia, const Box & boundingBox ) 
// =================================================================================================
//  /Description:
//     Build the smallest box that covers a list of tagged cells 
//   BUT that is also
//      at least minimumBoxWidth points wide in each direction and sits inside boundingBox
//\end{RegridInclude.tex} 
// =================================================================================================
{
  // const int numberOfDimensions=ia.getLength(1);
  assert( numberOfDimensions>0 );
  
  Box box = getBox(ia);
  // *wdh* 110529 if( box.isEmpty() ) box;
  if( box.isEmpty() ) return box;
  
  // Make sure the box remains at least minimumBoxWidth cells in each direction AND that the box remains
  // inside the boundingBox.
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    int base=box.smallEnd(axis);
    int bound=box.bigEnd(axis);
    if( bound-base+1 < minimumBoxWidth )
    {
      int diff=minimumBoxWidth - (bound-base+1);
      base=base  - diff/2;
      bound=bound+(diff+1)/2;
      int ba=boundingBox.smallEnd(axis), bb=boundingBox.bigEnd(axis);
      if( base<ba )  // prevent box from extending outside
      {
  	base=ba; 
  	bound= min(bb,base+minimumBoxWidth);
      }
      else if( bound>bb )
      {
  	bound=bb;
  	base=max(ba,bound-minimumBoxWidth);
      }

      box.setSmall(axis,base);
      box.setBig(axis,bound);
    }
  }
  if( debug & 2 )
  {
    printF(" Regrid::getBoundedBox: created box=[%i,%i][%i,%i] widths=[%i][%i], "
           " boundingBox=[%i,%i][%i,%i] widths=[%i][%i]",
	   box.smallEnd(0),box.bigEnd(0),
	   box.smallEnd(1),box.bigEnd(1),
	   box.bigEnd(0)-box.smallEnd(0)+1,
           box.bigEnd(1)-box.smallEnd(1)+1,
	   boundingBox.smallEnd(0),boundingBox.bigEnd(0),
	   boundingBox.smallEnd(1),boundingBox.bigEnd(1),
	   boundingBox.bigEnd(0)-boundingBox.smallEnd(0)+1,
           boundingBox.bigEnd(1)-boundingBox.smallEnd(1)+1);
    if( (box.bigEnd(0)-box.smallEnd(0)+1) < minimumBoxWidth ||
        (box.bigEnd(1)-box.smallEnd(1)+1) < minimumBoxWidth )
    {
      printF(" ******TOO SMALL ****\n");
    }
    else
    {
      printF("\n");
    }
    
  }
  
  return box;
}


//\begin{>>RegridInclude.tex}{\subsection{getEfficiency}} 
real Regrid::
getEfficiency(const intSerialArray & ia, const BOX & box )
// ===================================================================================
//  /Description:
//     return the efficiency of a box, the ratio of tagged cells to non-tagged cells.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  real numBoxPoints = max(1.,real(box.numPts()));
//   if( indexCoarseningFactor>1 )
//   {
//     numBoxPoints*=pow(indexCoarseningFactor,numberOfDimensions);
//   }
  
  #ifdef USE_PPP
    return ParallelUtility::getSum(ia.getLength(0))/numBoxPoints;
  #else
    return ia.getLength(0)/numBoxPoints;
  #endif
}

// *  ---- not needed anymore
// * //\begin{>>RegridInclude.tex}{\subsection{splitBox}} 
// * int Regrid::
// * fixPeriodicBox( MappedGrid & mg, BOX & box, const intArray & ia, int level )
// * // =======================================================================================
// * // /Description:
// * //    Fix a box that is used for a periodic grid such as an Annulus.
// * //
// * // Find an appropriate place to split the box along the periodic direction and then
// * // shift the tagged points in the array ia that one side of the cutPoint so the other
// * // side of the branch cut. When the box is split, this will allow refinement patches
// * // to cross the branch cut.
// * //
// * //\end{RegridInclude.tex} 
// * // ========================================================================================
// * {
// *   int numberOfPeriodicDirections=0;
// *   int axis;
// *   for( axis=0; axis<mg.numberOfDimensions(); axis++ )
// *     numberOfPeriodicDirections += mg.isPeriodic(axis)!=0;
// *   
// *   if( numberOfPeriodicDirections==0 )
// *     return numberOfPeriodicDirections;
// *   
// * 
// *   if( getEfficiency(ia,box) >= efficiency || box.numPts()<=minimumBoxSize*2 )
// *   {
// *     return 0;
// *   }
// *   else
// *   {
// *     for( axis=0; axis<mg.numberOfDimensions(); axis++ )
// *     {
// *       if( mg.isPeriodic(axis) )
// *       {
// * 	int longDirection=axis;
// * 	
// * 	int cutPoint;
// * 	findCutPoint( box,ia,longDirection,cutPoint );
// * 
// *         int period = mg.gridIndexRange(End,axis)-mg.gridIndexRange(Start,axis);
// *         if( level>1 )
// *           period*= int(pow( refinementRatio, level-1 )+.5);
// * 	
// *         Range R=ia.dimension(0);
// *         if( true ||  cutPoint < (mg.gridIndexRange(Start,axis)+mg.gridIndexRange(End,axis))/2 )
// * 	{
// * 	  where( ia(R,axis)>cutPoint )
// * 	  {
// * 	    ia(R,axis)-=period;
// * 	  }
// * 	}
// * 	else
// * 	{
// * 	  where( ia(R,axis)<cutPoint )
// * 	  {
// * 	    ia(R,axis)+=period;
// * 	  }
// * 	}
// *       }
// *     }
// *     if( numberOfPeriodicDirections>0 )
// *     {
// *       if( debug & 2 )
// *         cout << "fixPeriodicBox: old box: " << box << endl;
// * 
// *       box = getBox( ia ); // rebuild the box 
// * 
// *       if( debug & 2 )
// *         cout << "fixPeriodicBox: new box: " << box << endl;
// *     }
// *   }
// * 
// * 
// *   return 0;
// * }
// * 

//\begin{>>RegridInclude.tex}{\subsection{findCut}} 
int Regrid::
findCutPoint( BOX & box, const intSerialArray & ia, int & cutDirection, int & cutPoint )
// ===================================================================================
//  /Description:
//     Find the best place to split the box
//
// /box (input) : box to possibly split
// /ia (input) : array of tagged cells.
// /cutDirection (intput/output): on input: if $>0$, cut the box in this direction, otherwise
//   choose a direction to cut the box. On output: box was cut in this direction
// /cutPoint (output): box was cut at this point.
//\end{RegridInclude.tex} 
// ===================================================================================
{
  // *** here we bisect the box ****
  Range R=ia.getLength(0);
  // const int numberOfDimensions=ia.getLength(1);
  assert( numberOfDimensions>0 );

  if( cutDirection<0 )
  {
    box.longside(cutDirection);
    if( debug & 2)
      printF("findCutPoint: cutDirection = %i\n",cutDirection);
  }
  
  int boxa = box.smallEnd(cutDirection);
  int boxb = box.bigEnd(cutDirection);

  int ivm; // mid point in the cut direction
  ivm=int( ( (boxa+boxb) +1.5)/2. );   // add 1.5 for cell centred

  cutPoint=ivm;  // 
    
  CutStatus status;
  if( useSmartBisection )
  {

    intSerialArray histogram(Range(boxa,boxb));

    histogram=0;
    int i;
    for( i=0; i<=R.getBound(); i++ )
      histogram(ia(i,cutDirection))+=1;
    
    #ifdef USE_PPP
    for( i=histogram.getBase(0); i<=histogram.getBound(0); i++ )
    {
      histogram(i)=ParallelUtility::getSum(histogram(i));  // *** this may not be efficient ***
    }
    #endif    

    if( debug & 8 )
    {
      display(ia,"ia","%3i");
      display(histogram,"histogram","%4i");
    }
      
    cutPoint=findCut(histogram.getDataPointer(),boxa,boxb,status);


  }
  
  if( debug & 2)
    printF(" cutPoint=%i, midPoint=%i, cutStatus=%i, [boxa,boxb]=[%i,%i]\n", cutPoint,ivm,status,boxa,boxb);
  

  return 0;
}




//\begin{>>RegridInclude.tex}{\subsection{splitBox}} 
int Regrid::
splitBox( BOX & box, const intSerialArray & ia, BoxList & boxList, int refinementLevel )
// ===================================================================================
//  /Description:
//     Split a box into two if it does not satisfy the efficiency criterion.
//   This function then calls itself recursively.
//
// /box (input) : box to possibly split
// /ia (input) : array of tagged cells.
// /boxList (input/output) :
// /refinementLevel (input) :
//\end{RegridInclude.tex} 
// ===================================================================================
{

  const real minEfficiency=.25;  // *wdh* added 060904 -- prevent nearly empty boxes on periodic grids
  real boxEfficiency=getEfficiency(ia,box);

  const int indexCoarseningFactorFactor 
    = (numberOfDimensions==2 ? indexCoarseningFactor*indexCoarseningFactor : 
       numberOfDimensions==3 ? indexCoarseningFactor*indexCoarseningFactor*indexCoarseningFactor : indexCoarseningFactor);
  
  const int numBoxPoints = box.numPts();
  const int actualNumBoxPoints=numBoxPoints*indexCoarseningFactorFactor;

  int maxBoxWidth=0;
  for( int axis=0; axis<numberOfDimensions; axis++ ) 
    maxBoxWidth=max(maxBoxWidth,box.bigEnd(axis)-box.smallEnd(axis)+1);

  if( (boxEfficiency >= efficiency) || 
      ( maxBoxWidth < minimumBoxWidth*2 ) || 
      ( actualNumBoxPoints <minimumBoxSize*2  && boxEfficiency>minEfficiency)  ||  
      splitNumber>=maximumNumberOfSplits  )
  {
    if( box.numPts()>0 )
    {
      if( debug & 2 )
      {
        int numia = ParallelUtility::getSum(ia.getLength(0));
	printF("splitBox:add box to list: box=[%i,%i][%i,%i][%i,%i] numPts=%i num(ia)=%i eff=%5.2f target=%5.2f "
               " minimumBoxSize*2=%i\n",
               box.smallEnd(0),box.bigEnd(0),
               box.smallEnd(1),box.bigEnd(1),
               box.smallEnd(2),box.bigEnd(2),box.numPts(),numia,boxEfficiency,efficiency,minimumBoxSize*2);
      }
	
      if( debug & 8 )
        display(ia,"ia","%3i");
      
      if( false || refinementLevel==1 || properNestingDomain[refinementLevel-1].contains(box) )
      {
	boxList.add( box );
      }
      else
      {
	// box is efficient but does not properly nest -- split it up
        if( debug & 2)
	{
	  printF("splitBox:A box is efficient but does not properly nest level=%i box=[%i,%i][%i,%i][%i,%i]\n",
		 refinementLevel,
		 box.smallEnd(0),box.bigEnd(0),
		 box.smallEnd(1),box.bigEnd(1),
		 box.smallEnd(2),box.bigEnd(2));
	}
	
        BoxList insideList(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));
	insideList=intersect(properNestingDomain[refinementLevel-1],box);
 	insideList.simplify();
	for( BoxListIterator bli(insideList); bli; ++bli)
	{
          Box & newBox = insideList[bli];
          boxList.add( newBox );
          if( debug & 2)
            printF("splitBox: --> replace by box=[%i,%i][%i,%i][%i,%i]\n",
		 newBox.smallEnd(0),newBox.bigEnd(0),
		 newBox.smallEnd(1),newBox.bigEnd(1),
		 newBox.smallEnd(2),newBox.bigEnd(2));
	}
      }
    }
  }
  else
  {
    // split the box into two
    splitNumber++;

    if( debug & 2  )
    {
      const real eff = getEfficiency(ia,box);
      if( myid==0 )
	cout << "splitBox: Split the box = " << box << ", numPts=" << box.numPts() << 
	  " efficiency=" << eff << endl;
    }
    
    Range R=ia.getLength(0);
    // const int numberOfDimensions=ia.getLength(1);
    assert( numberOfDimensions>0 );
    
    // *** here we split the box ****
    int longDirection=-1;
    int cutPoint;
    findCutPoint( box,ia,longDirection,cutPoint );


    intSerialArray iad; 
    if( R.length()>0 )
    {
      iad.redim(R);
      iad=ia(R,longDirection); // make a copy or indexMap doesn't work below
    }
    for( int dir=0; dir<=1; dir++ )
    {
      intSerialArray ib;
      if( R.length()>0 )
      {
	if( dir==0 )
	  ib= (iad <  cutPoint ).indexMap();     // use <= if vertex centered
	else
	  ib= (iad >= cutPoint ).indexMap();
      }
      
      // display(ib,"ib","%3i");
      
      int numInBox=ib.getLength(0);
      #ifdef USE_PPP
        numInBox=ParallelUtility::getMaxValue(numInBox);
      #endif
      if( numInBox>0 )
      {
	intSerialArray ia1;
        if( ib.getLength(0)>0 )
	{
  	  Range R2=ib.getLength(0);
	  ia1.redim(R2,numberOfDimensions);
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    ia1(R2,axis)=ia(ib,axis);
	}
	
        // we could restrict the new box to have a certain width here:
        // For dir==0 : use "left-half of the box", dir=1 use right-half box
	Box box1;
        if( limitBoxSize )
	{
          Box boundingBox=box;
          if( dir==0 )
	    boundingBox.setBig(longDirection,cutPoint-1);
	  else
            boundingBox.setSmall(longDirection,cutPoint);
	  box1=getBoundedBox(ia1,boundingBox);  
	}
	else
	{
	  box1=getBox(ia1);  
	}
	
	splitBox(box1,ia1,boxList,refinementLevel);
	
      }
      else
      {
        if( debug & 2 && myid==0 )
	  printF("box%i is empty\n",dir);
      }
      
    }
  }

  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{buildTaggedCells}} 
int Regrid::
buildTaggedCells( MappedGrid & mg, 
		  intMappedGridFunction & tag,
		  const realArray & error, 
		  real errorThreshhold,
                  bool useErrorFunction,
                  bool cellCentred /* = true */ )
// =========================================================================================================
// /Description: 
//     Build the integer flag array (tags) of points that need to be refined on a single grid.
//   Increase the region covered by tagged by the specified buffer zone.
// /mg (input) : build tags for this grid.
// /tag (output) : tagged cells go on this grid.
// /error (input): user defined error function (only used if useErrorFunction==true).
// /errorThreshhold (input) : tag cells where the error is larger than this value.
// /useErrorFunction (input) : if false, the tag array is already set and we ignore the error array.
//\end{RegridInclude.tex} 
// =========================================================================================================
{
  real time0=getCPU();
  
  // const int numberOfDimensions = mg.numberOfDimensions();
  assert( numberOfDimensions>0 );

  #ifdef USE_PPP
    intSerialArray tagLocal; getLocalArrayWithGhostBoundaries(tag,tagLocal);
  #else
    intSerialArray & tagLocal = tag;
  #endif

  if( useErrorFunction )
  {
    // compute the tag array
    #ifdef USE_PPP
      realSerialArray errorLocal; getLocalArrayWithGhostBoundaries(error,errorLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
    #else
      const realSerialArray & errorLocal = error;
      const intSerialArray & maskLocal = mg.mask();
    #endif

      // If error has updated ghost boundaries then so should tag after the next operations:
    tagLocal=0;
    where( errorLocal>errorThreshhold )
    {
      tagLocal=1;
    }
    where( maskLocal==0 )
    {
      tagLocal=0;
    }
  }
  
  Index I1,I2,I3;
  //  getIndex(mg.extendedIndexRange(),I1,I2,I3);
  getIndex(extendedGridIndexRange(mg),I1,I2,I3);

  #ifdef USE_PPP 
    int i1a=max(I1.getBase() ,tagLocal.getBase(0) +tag.getGhostBoundaryWidth(0)), 
        i1b=min(I1.getBound(),tagLocal.getBound(0)-tag.getGhostBoundaryWidth(0));
    int i2a=max(I2.getBase(), tagLocal.getBase(1) +tag.getGhostBoundaryWidth(1)), 
        i2b=min(I2.getBound(),tagLocal.getBound(1)-tag.getGhostBoundaryWidth(1));
    int i3a=max(I3.getBase(), tagLocal.getBase(2) +tag.getGhostBoundaryWidth(2)), 
        i3b=min(I3.getBound(),tagLocal.getBound(2)-tag.getGhostBoundaryWidth(2));

    const bool localArrayIsNotNull = i1a<=i1b && i2a<=i2b && i3a<=i3b;
    if( localArrayIsNotNull )
    {
      I1=Range(i1a,i1b);
      I2=Range(i2a,i2b);
      I3=Range(i3a,i3b);
    }
  #else
    const bool localArrayIsNotNull = true;
  #endif


  // *** mark all cells around the error vertex -- we count this as a growth factor of 1.
  if( cellCentred )
  {
    if( localArrayIsNotNull )
    {
      if( numberOfDimensions==2 )
      {
	tagLocal(I1,I2,I3)= min(1, (tagLocal(I1,I2,I3)+tagLocal(I1+1,I2,I3)+
                                    tagLocal(I1,I2+1,I3)+tagLocal(I1+1,I2+1,I3)) );
      }
      else if( numberOfDimensions==1 )
      {
	tagLocal(I1,I2,I3)= min(1, (tagLocal(I1,I2,I3)+tagLocal(I1+1,I2,I3)));
      }
      else
      {
	tagLocal(I1,I2,I3)= min(1, (tagLocal(I1,I2,I3)+tagLocal(I1+1,I2,I3)+tagLocal(I1,I2+1,I3)+tagLocal(I1,I2,I3+1)+
			            tagLocal(I1+1,I2+1,I3)+tagLocal(I1+1,I2,I3+1)+tagLocal(I1,I2+1,I3+1)+
                                    tagLocal(I1+1,I2+1,I3+1)) );
      }
    }
    tag.periodicUpdate();
    tag.updateGhostBoundaries();
  }
  
  // int growStart= cellCentred ? 1 : 0;
  for( int grow=1; grow<numberOfBufferZones; grow++ )
  {
    if( localArrayIsNotNull )
    {
      if( numberOfDimensions==2 )
      {
        tagLocal(I1,I2,I3)= min(1,
  			 (tagLocal(I1-1,I2-1,I3)+tagLocal(I1  ,I2-1,I3)+tagLocal(I1+1,I2-1,I3)+
  			  tagLocal(I1-1,I2  ,I3)+tagLocal(I1  ,I2  ,I3)+tagLocal(I1+1,I2  ,I3)+
  			  tagLocal(I1-1,I2+1,I3)+tagLocal(I1  ,I2+1,I3)+tagLocal(I1+1,I2+1,I3)));
      }
      else if( numberOfDimensions==1 )
      {
        tagLocal(I1,I2,I3)= min(1,tagLocal(I1-1,I2  ,I3)+tagLocal(I1  ,I2  ,I3)+tagLocal(I1+1,I2  ,I3));
      }
      else
      {
        tagLocal(I1,I2,I3)= min(1,
  			 (tagLocal(I1-1,I2-1,I3-1)+tagLocal(I1  ,I2-1,I3-1)+tagLocal(I1+1,I2-1,I3-1)+
  			  tagLocal(I1-1,I2  ,I3-1)+tagLocal(I1  ,I2  ,I3-1)+tagLocal(I1+1,I2  ,I3-1)+
  			  tagLocal(I1-1,I2+1,I3-1)+tagLocal(I1  ,I2+1,I3-1)+tagLocal(I1+1,I2+1,I3-1)+
  			  tagLocal(I1-1,I2-1,I3  )+tagLocal(I1  ,I2-1,I3  )+tagLocal(I1+1,I2-1,I3  )+
  			  tagLocal(I1-1,I2  ,I3  )+tagLocal(I1  ,I2  ,I3  )+tagLocal(I1+1,I2  ,I3  )+
  			  tagLocal(I1-1,I2+1,I3  )+tagLocal(I1  ,I2+1,I3  )+tagLocal(I1+1,I2+1,I3  )+
  			  tagLocal(I1-1,I2-1,I3+1)+tagLocal(I1  ,I2-1,I3-1)+tagLocal(I1+1,I2-1,I3+1)+
  			  tagLocal(I1-1,I2  ,I3+1)+tagLocal(I1  ,I2  ,I3-1)+tagLocal(I1+1,I2  ,I3+1)+
  			  tagLocal(I1-1,I2+1,I3+1)+tagLocal(I1  ,I2+1,I3-1)+tagLocal(I1+1,I2+1,I3+1) 
                                              ));
      }
    }
    tag.periodicUpdate();
    tag.updateGhostBoundaries();
  }
  timeForBuildTaggedCells+=getCPU()-time0;
  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{cellCenteredBox}} 
Box Regrid::
cellCenteredBox( MappedGrid & mg, int ratio /* =1 */ )
// ================================================================================================
// /Description:
//   Build a cell centered box from a MappedGrid.
//\end{RegridInclude.tex} 
// ===============================================================================================
{
  Box box = mg.box();      // we could keep a list for below
  

  box.convert(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));  // build a cell centered box

  if( ratio!=1 )
    box.refine( ratio );
  
  if( useCoarsenedIndexSpace )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      box.setSmall(axis,coarsenIndexLower(box.smallEnd(axis),axis));
      // To coarsen a cell-centered box:  (1) Add one to make node centered, (2) coarsen, (3) subtract 1
      box.setBig(axis,coarsenIndexUpper(box.bigEnd(axis)+1,axis)-1); 
    }
  }

  for( int axis=mg.numberOfDimensions(); axis<3; axis++ )
  { 
    box.setSmall(axis,0); 
    box.setBig(axis,0);   
 }
  return box;
}

//\begin{>>RegridInclude.tex}{\subsection{cellCenteredBox}} 
Box Regrid::
cellCenteredBaseBox( MappedGrid & mg )
// ================================================================================================
// /Description:
//   Build a cell centered box from a MappedGrid on level=0. 
//
// We expand the box on the base level to include ghost points on interpolation boundaries,
// since we need to allow refinement patches to extend into the interpolation region.
//
// On a periodic grid we extend the box in the periodic direction since we should
// never need to restrict refinements in this direction
//
//\end{RegridInclude.tex} 
// ===============================================================================================
{
  Box box = mg.box();      // we could keep a list for below
  box.convert(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));  // build a cell centered box

  int axis;
  for( axis=mg.numberOfDimensions(); axis<3; axis++ )
  {
    box.setSmall(axis,0); 
    box.setBig(axis,0);   
  }

  // we expand the box on the base level to include ghost points on interpolation boundaries,
  // since we need to allow refinement patches to extend into the interpolation region.
  const IntegerArray & extendedIndexRange = mg.extendedIndexRange();
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    // *wdh* 000705 box.setSmall(axis,extendedIndexRange(0,axis)); 
    // *wdh* 000705 box.setBig(axis,extendedIndexRange(1,axis));
    if( !mg.isPeriodic(axis) )
    {
      box.setSmall(axis,extendedIndexRange(0,axis)); 
      box.setBig(axis,extendedIndexRange(1,axis)-1); // note -1 since cell centred
    }
    else // if( !splitBoxesAtBranchCut )
    {
      // on a periodic grid we extend the box in the periodic direction since we should
      // never need to restrict refinements in this direction
      int period=(extendedIndexRange(1,axis)-extendedIndexRange(0,axis));
      box.setSmall(axis,extendedIndexRange(0,axis)-2*period); 
      box.setBig  (axis,extendedIndexRange(1,axis)+2*period); 
    }

  }

  if( useCoarsenedIndexSpace )
  {
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      box.setSmall(axis,coarsenIndexLower(box.smallEnd(axis),axis));
      // To coarsen a cell-centered box:  (1) Add one to make node centered, (2) coarsen, (3) subtract 1
      box.setBig(axis,coarsenIndexUpper(box.bigEnd(axis)+1,axis)-1); 
    }
  }


  return box;
}


//\begin{>>RegridInclude.tex}{\subsection{buildProperNestingDomains}} 
int Regrid::
buildProperNestingDomains(GridCollection & gc, 
                          int baseGrid,
			  int refinementLevel,
			  int baseLevel,
                          int numberOfRefinementLevels  )
// ============================================================================================
// /Description:
//   Build a list of boxes that covers the portition of the domain where we are
//   allowed to add refinement grids, in order to ensure proper nesting of grids.
//
// NOTE: The proper nesting domain is ONLY built for the baseLevel grids, i.e. the grids that
//   do not change. We build fine grid versions of this proper nesting domain.
//  The newer levels are properly nested automatically since the fine grid tagged
//  cells are added to the coarse grid tagged cells. 
// 
// /properNestingDomain[level] : a list of boxes defining the allowable region where
//   refinement grids at level+1 can be added. The allowable region will be {\tt widthOfProperNesting}
//   inside the refinement grids at level.
// /complementOfProperNestingDomain[level] : the set complement of properNestingDomain[level].
//
//\end{RegridInclude.tex} 
// ==========================================================================================
{
  if( numberOfRefinementLevels>1 )
  {
    GridCollection & rl = gc.refinementLevel[baseLevel];
    IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));

    // Set some parameters if we use a coarse index space for building grids
    setupCoarseIndexSpace(gc,baseGrid,baseLevel+1);

    // ** this doesn't work: intersects requires all directions to be the same centering
    // IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));
    // if( numberOfDimensions==2 )
    //  centering =IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::NODE));

    BoxList boxList1(centering), boxList2(centering);
    const int ratio=int(pow(refinementRatio,baseLevel)+.5);

    // find all refinement grids on this level that have the same base grid
    for( int gr=0; gr<rl.numberOfComponentGrids(); gr++ )
    {
      if( rl.baseGridNumber(gr)==baseGrid )
      {
	Box box= baseLevel!=0 ? cellCenteredBox(gc[rl.gridNumber(gr)]) :    // we could keep a list for below
	  cellCenteredBaseBox(gc[rl.gridNumber(gr)]);
      
	if( debug & 2 ) 
	{
          printF("buildProperNesting:level=%i Add grid %i to to boxList1 ---> box =[%i,%i][%i,%i][%i,%i]\n",
             refinementLevel,rl.gridNumber(gr),
		 box.smallEnd(0),box.bigEnd(0),
		 box.smallEnd(1),box.bigEnd(1),
		 box.smallEnd(2),box.bigEnd(2));
	}
	
	boxList1.add(box); 
      }
    }
    // Method: first build the complement of the boxList1, then expand the complement.
    // Build the proper nesting domain as the complement of the complement.

    // could be a box that covers all flagged points:
    // on the base level we may have to treat periodic boxes 
    Box baseBox= baseLevel!=0 ? cellCenteredBox(gc[baseGrid],ratio) : cellCenteredBaseBox(gc[baseGrid]);

    if( debug & 2 )
    {
      printF("buildProperNesting:level=%i baseBox=[%i,%i][%i,%i][%i,%i]\n",refinementLevel,
	     baseBox.smallEnd(0),baseBox.bigEnd(0),
	     baseBox.smallEnd(1),baseBox.bigEnd(1),
	     baseBox.smallEnd(2),baseBox.bigEnd(2));
    }

    boxList2=complementIn(baseBox,boxList1);

    if( debug & 2 && myid==0 )
    {
      cout << " boxList1: " << boxList1 << endl;
      cout << " complement of boxList1, boxList2: " << boxList2 << endl;
    }
    
//    assert( (widthOfProperNesting % indexCoarseningFactor) == 0 );
//    const int bufferWidth = widthOfProperNesting/indexCoarseningFactor;
    const int bufferWidth = widthOfProperNesting;
    
    complementOfProperNestingDomain[baseLevel]=accrete(boxList2,bufferWidth);
    complementOfProperNestingDomain[baseLevel].intersect(baseBox);
    complementOfProperNestingDomain[baseLevel].simplify();
    properNestingDomain[baseLevel]=complementIn(baseBox,complementOfProperNestingDomain[baseLevel]);
    properNestingDomain[baseLevel].simplify();
    boxList1.clear();
    boxList2.clear();

    if( debug & 2 && myid==0 )
      cout << " proper nesting domain for baseLevel=" << baseLevel << ": " << properNestingDomain[baseLevel] << endl;

    // Now build proper nesting lists for higher refinement levels. These are just refined versions of the
    // the baseLevel proper nesting domain (i.e. the same region but for the fine grid index space)
    for( int lv=baseLevel+1; lv<numberOfRefinementLevels; lv++ )
    {
      BoxList tmp;
      tmp=refine(complementOfProperNestingDomain[lv-1],refinementRatio); // refinementRatio(lv-1)
      complementOfProperNestingDomain[lv]=accrete(tmp,bufferWidth);

      // *wdh* 000626 Box levelBaseBox = cellCenteredBox(gc[baseGrid],pow(refinementRatio,lv));
      Box levelBaseBox = baseBox;
      levelBaseBox.refine((int)pow(refinementRatio,lv));
      
      complementOfProperNestingDomain[lv].intersect(levelBaseBox); 

      properNestingDomain[lv]=complementIn(levelBaseBox,complementOfProperNestingDomain[lv]);
      properNestingDomain[lv].simplify();

      if( debug & 2 && myid==0 )
        cout << " proper nesting domain for level=" << lv << ": " << properNestingDomain[lv] << endl;

    }

  }
  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{printStatistics}} 
int Regrid::
printStatistics( GridCollection & gc, FILE *file /* = NULL */, 
		       int *numberOfGridPoints /* =NULL */ )
// =================================================================================================
// /Description:
//    Print statistics about the adaptive grid such as the number of grids and grid points
// at each refinement level.
//
// /file (input): write to this file (if specified)
// /numberOfGridPoints (input) : return the total number of grid points (if specified)
//\end{RegridInclude.tex} 
// =================================================================================================
{
  FILE *f = file!=NULL ? file : stdout;

  IntegerArray numberOfPoints(gc.numberOfRefinementLevels());
  numberOfPoints=0;
  int l;
  for( l=0; l<gc.numberOfRefinementLevels(); l++ )
  {
    GridCollection & rl = gc.numberOfRefinementLevels()==1 ? gc : gc.refinementLevel[l];
    for( int g=0; g<rl.numberOfGrids(); g++ )
    {
      int grid=rl.gridNumber(g);
      const IntegerArray & d=gc[grid].dimension();

      numberOfPoints(l)+=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);

    }
  }
  int totalNumberOfPoints=sum(numberOfPoints);
  if( myid==0 )
  {
    fprintf(f,"  level grids  grid points percentage\n");
    for( l=0; l<gc.numberOfRefinementLevels(); l++ )
    {
      GridCollection & rl = gc.numberOfRefinementLevels()==1 ? gc : gc.refinementLevel[l];
      fprintf(f," %4i %6i  %10i   %6.2f%% \n",l,
	      rl.numberOfComponentGrids(),numberOfPoints(l),
	      100.*real(numberOfPoints(l))/totalNumberOfPoints);
    }
    fprintf(f," total%6i  %10i   %6.2f%% \n",
            gc.numberOfComponentGrids(),totalNumberOfPoints,
	    100.*real(totalNumberOfPoints)/totalNumberOfPoints);
  
  }
  
  if( numberOfGridPoints!=NULL )
    *numberOfGridPoints=totalNumberOfPoints;

  if( myid==0 )
  {
    real totalTime=max(REAL_MIN,timeForRegrid);
    fprintf(f,"(last regrid) CPU: regrid=%8.1e (%6.2f%%), buildGrids=%8.1e (%6.2f%%), buildTagged=%8.1e (%6.2f%%)\n",
	    timeForRegrid,100.*timeForRegrid/totalTime,
	    timeForBuildGrids,100.*timeForBuildGrids/totalTime,
	    timeForBuildTaggedCells,100.*timeForBuildTaggedCells/totalTime);
  }
  
  return 0;
}


//\begin{>>RegridInclude.tex}{\subsection{buildGrids}} 
int Regrid::
buildGrids( GridCollection & gcOld, 
            GridCollection & gc, 
            int baseGrid, 
	    int baseLevel, 
	    int refinementLevel, 
	    BoxList *refinementBoxList,
            IntegerArray **gridInfo )
// =================================================================================================
// /Description:
//    Add grids to a GridCollection that can be found in the list of boxes for each refinement level.
//
// /gcOld (input) : old grid
// /gc (output) : new grid
// /baseGrid (input) : the base grid is gcOld[baseGrid]
// /baseLevel (input) : this level and below have not been changed.
// /refinementBoxList (input) : lists of boxes to be added as grids.
// /gridInfo (input) : holds list of boxes for the new optimized method 
//     gridInfo holds
//        0..5 : [n1a,n1b][n2a,n2b][n3a,n3b]
//        6..8 : refinement factor[axis] 
//        9..10 : processor range for the parallel distribution of this grid
//
//\end{RegridInclude.tex} 
// =================================================================================================
{
  real time0=getCPU();

  // int debug=3;
  
  if( debug & 2 && myid==0 )
  {
    for( int level=baseLevel+1; level<=refinementLevel; level++ )
    {    
      BoxList & boxList = refinementBoxList[level-baseLevel-1];
      cout << "*** buildGrids:: refinementBoxList[" << level << "] :" << boxList << endl;
    }
  }
  
  numberOfDimensions=gcOld.numberOfDimensions();
  assert( numberOfDimensions>0 && numberOfDimensions<=3 );
  Range Rx=numberOfDimensions;
  
  if( gc.numberOfComponentGrids()==0 || 
           gc.numberOfRefinementLevels() < gcOld.numberOfRefinementLevels() )
  {
    // if gc is empty we first make a copy of the base grids -- 
    if( debug & 2) printF("+++++++++Regrid::buildGrids: copy base grids\n");
    // gc.destroy();
    // gc=gcOld; // .refinementLevel[0];  // ***** fix this, no need to copy all grids ****
    gc.setNumberOfGrids(0);
    gc=gcOld; // we need the interpolation data
    if( debug & 2 )
    {
      for( int ll=0; ll<gc.numberOfRefinementLevels(); ll++ )
      {
	printF("+++Regrid::buildGrids: level %i gc.refinementLevel[ll].numberOfComponentGrids()=%i \n",
	       ll,gc.refinementLevel[ll].numberOfComponentGrids());
      }
    }
//     for( int grid=0; grid<gcOld.numberOfComponentGrids(); grid++ )
//     {
//       if( gcOld.baseGridNumber(grid)==grid )
//       {
// 	printf(" Add base grid %s\n",(const char *)(gcOld[grid].getName()));
      
// 	gc.add( gcOld[grid] );
//       }
//     }
  }
  gc.update(GridCollection::THErefinementLevel);

  if( true && gridAdditionOption==addGridsAsRefinementGrids )
  {
    // **** new method: add and delete grids at the same time *****
    // It is much quicker to reuse an existing grid rather than rebuild it
    Range all;

    // Now make a list of the locations of all grids to be added
    int level;
    for( level=baseLevel+1; level<=refinementLevel; level++ )
    {
      IntegerArray & info = gridInfo[baseGrid][level-baseLevel-1];
      int num=refinementBoxList[level-baseLevel-1].length();
      // printF(" **** buildGrids: add %i grids to level %i (baseGrid=%i)\n",num,level,baseGrid);
      
      // gridInfo holds
      //    0..5 : [n1a,n1b][n2a,n2b][n3a,n3b]
      //    6..8 : refinement factor[axis] 
      //    9..10 : processor range for the parallel distribution of this grid
      info.redim(11,num); // we may need to add more later for grids crossing periodic boundaries.
      info=0;
      info(10,all)=-1;  // processor range of [0,-1] means use all processors
    }

    IntegerArray extended;
    IntegerArray factor(3);
    
    for( level=baseLevel+1; level<=refinementLevel; level++ )
    {
      BoxList & boxList = refinementBoxList[level-baseLevel-1];
      int numToAdd=boxList.length();
    
      // Set some parameters if we use a coarse index space for building grids
      setupCoarseIndexSpace(gc,baseGrid,level);


      // printF(" *** Regrid:buildGrids: baseGrid=%i level=%i numToAdd=%i numberToDelete=%i\n",
      //      baseGrid,level,numToAdd,numberToDelete);

      IntegerArray & info = gridInfo[baseGrid][level-baseLevel-1];

      int numberAdded=0;

      if( numToAdd>0 )
      {
	bool baseMappingIsPeriodic=false;
	int axis;
	for( axis=0; axis<numberOfDimensions; axis++ )
	{
	  if( gc[baseGrid].isPeriodic(axis)==Mapping::notPeriodic )
	  {
	    baseMappingIsPeriodic=true;
	    break;
	  }
	}
	
	// extended: do not let boxes extend outside the base grid extendedIndexRange
	int refinementFactor=int(pow(refinementRatio,level-1)+.5);
	extended = extendedGridIndexRange(gc[baseGrid])*refinementFactor;

	factor=refinementRatio;

	// add new grids as refinements.
	for( BoxListIterator bli(boxList); bli; ++bli)
	{
	  if( baseMappingIsPeriodic && numberAdded>info.getBound(1) )
	  {
	    info.resize(info.getLength(0),numberAdded+20); // add more entries
	  }
	  else
	  {
	    assert( numberAdded<=info.getBound(1) );
	  }
	
	  bool emptyPeriodicBox=false;
	  int splitPeriodicBox=0;
	  int splitAxis;
          int minSize=INT_MAX;
          info(0+2*axis3,numberAdded)=0;
          info(1+2*axis3,numberAdded)=0;
	  
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    const Box & box = boxList[bli];
	    info(0+2*axis,numberAdded)=refineIndex(box.smallEnd(axis),axis);
	    info(1+2*axis,numberAdded)=refineIndex(box.bigEnd(axis)+1,axis);  // add one since we create node centered grids
	
	    if( !baseMappingIsPeriodic || gc[baseGrid].isPeriodic(axis)==Mapping::notPeriodic )
	    {
	      info(1+2*axis,numberAdded)=min(info(1+2*axis,numberAdded),extended(End,axis)); 
	    }
	    else if( splitBoxesAtBranchCut )
	    {
	      // grids should not cross the periodic branch cut.
	      info(0+2*axis,numberAdded)=max(info(0+2*axis,numberAdded),extended(Start,axis));
	      info(1+2*axis,numberAdded)=min(info(1+2*axis,numberAdded),extended(End ,axis));
	      if( info(0+2*axis,numberAdded)==info(1+2*axis,numberAdded) )
		emptyPeriodicBox=true;  // we sometimes get an empty box on a periodic boundary, this is ok
	      if( info(0+2*axis,numberAdded)==extended(0,axis) && info(1+2*axis,numberAdded)==extended(1,axis) )
	      {
		splitPeriodicBox++;
		splitAxis=axis;
	      }
	    }
	    minSize=min(minSize,info(1+2*axis,numberAdded)-info(0+2*axis,numberAdded));
	  }
	  if( debug & 1 )
	    printF("add grid to level %i, (base=%i)  [%i,%i]x[%i,%i]\n",level,baseGrid,
		   info(0+2*0,numberAdded),info(1+2*0,numberAdded),info(0+2*1,numberAdded),info(1+2*1,numberAdded));

	  if( minSize>=1  )
	  {
            info(6,numberAdded)=factor(0);
            info(7,numberAdded)=factor(1);
            info(8,numberAdded)=factor(2);
	    
	    numberAdded++;
	  
	    if( splitPeriodicBox )
	    {
	      // split a periodic box -- 
	      if( numberAdded>info.getBound(1) )
	      {
		 info.resize(info.getLength(0),numberAdded+20); // add more entries
	      }

	      assert( splitPeriodicBox==1 );  // only 1 periodic direction assumed, fix for 2.
	      if( debug & 1 )
		printF("split grid on a periodic axis\n");
	    
	      // IntegerArray rp=range;

	      info(all,numberAdded)=info(all,numberAdded-1);

	      info(1+2*splitAxis,numberAdded-1)=(info(0+2*splitAxis,numberAdded)+info(1+2*splitAxis,numberAdded))/2;

	      // gc.addRefinement(rp, factor, level, baseGrid); 

	      info(0+2*splitAxis,numberAdded)=info(1+2*splitAxis,numberAdded-1);
	      // info(1+2*splitAxis,numberAdded)=info(1+2*splitAxis,numberAdded);

	      numberAdded++;
	    
	      // gc.addRefinement(rp, factor, level, baseGrid); 
	    }
	  }
	  else if( !emptyPeriodicBox )
	  {
	    for( axis=0; axis<numberOfDimensions; axis++ )

	      printF("Regrid::regrid:ERROR: an invalid box was made! [%i,%i]x[%i,%i]\n",
		     info(0+2*0,numberAdded),info(1+2*0,numberAdded),info(0+2*1,numberAdded),info(1+2*1,numberAdded));
	  }
	} // end loop over boxes
	
      }

      // printF(" **** buildGrids: numberAdded= %i (level=%i, baseGrid=%i)\n",numberAdded,level,baseGrid);

      if( info.getLength(1)!=numberAdded )
	info.resize(info.getLength(0),numberAdded);

      // printF("****BuildGrids (new way): added %i grids to level %i\n",numberAdded,level);
      // info.display("info");

    }  // end for level
    
  }
  else
  {
    // **** old way *****

    // first delete any existing refinement grids.
    int level,grid;
    for( level=min(refinementLevel,gc.numberOfRefinementLevels()-1); level>=baseLevel+1; level-- )
    {
      GridCollection & rl = gc.refinementLevel[level];
      for( int gr=rl.numberOfComponentGrids()-1; gr>=0; gr-- )
      {
	if( rl.baseGridNumber(gr)==baseGrid )
	{
	  grid=rl.gridNumber(gr);
	  if( debug & 2 )
	    printF("delete grid %i from level %i\n",grid,level);
	  
	  gc.deleteRefinement(grid);
	}
      }
    }
    // gc.deleteRefinement(gridsToDelete);
  

  // now add the grids into the grid collection.
    for( level=baseLevel+1; level<=refinementLevel; level++ )
    {    
      BoxList & boxList = refinementBoxList[level-baseLevel-1];
      if( boxList.length()==0 )
	break;
    
      // extended: do not let boxes extend outside the base grid extendedIndexRange
      IntegerArray extended;
      int refinementFactor=int(pow(refinementRatio,level-1)+.5);
      extended = extendedGridIndexRange(gc[baseGrid])*refinementFactor;

      if( gridAdditionOption==addGridsAsRefinementGrids )
      {
	// add new grids as refinements.

	IntegerArray range(2,3), factor(3);
	range=0;
	factor=refinementRatio;
	for( BoxListIterator bli(boxList); bli; ++bli)
	{
	  bool emptyPeriodicBox=false;
	  int splitPeriodicBox=0;
	  int axis, splitAxis;
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    const Box & box = boxList[bli];
	    range(0,axis)=refineIndex(box.smallEnd(axis),axis);
	    range(1,axis)=refineIndex(box.bigEnd(axis)+1,axis);  // add one since we create node centered grids
	
	    if( gc[baseGrid].isPeriodic(axis)==Mapping::notPeriodic )
	      range(1,axis)=min(range(1,axis),extended(End,axis)); 
	    else if( splitBoxesAtBranchCut )
	    {
	      // grids should not cross the periodic branch cut.
	      range(0,axis)=max(range(0,axis),extended(Start,axis));
	      range(1,axis)=min(range(1,axis),extended(End ,axis));
	      if( range(0,axis)==range(1,axis) )
		emptyPeriodicBox=true;  // we sometimes get an empty box on a periodic boundary, this is ok
	      if( range(0,axis)==extended(0,axis) && range(1,axis)==extended(1,axis) )
	      {
		splitPeriodicBox++;
		splitAxis=axis;
	      }
	    }
	  
	  }
	  if( debug & 1 )
	    printF("add grid to level %i, (base=%i)  [%i,%i]x[%i,%i]\n",level,baseGrid,
		   range(0,0),range(1,0),range(0,1),range(1,1));

	  if( min(range(1,Rx)-range(0,Rx))>=1  )
	  {
	    if( !splitPeriodicBox )
	      gc.addRefinement(range, factor, level, baseGrid); 
	    else
	    {
	      // split a periodic box -- 

	      assert( splitPeriodicBox==1 );  // only 1 periodic direction assumed, fix for 2.
	      if( debug & 1 )
		printf("split grid on a periodic axis\n");
	    
	      IntegerArray rp=range;
	      rp(1,splitAxis)=(range(0,splitAxis)+range(1,splitAxis))/2;
	      gc.addRefinement(rp, factor, level, baseGrid); 
	      rp(0,splitAxis)=rp(1,splitAxis);
	      rp(1,splitAxis)=range(1,splitAxis);
	      gc.addRefinement(rp, factor, level, baseGrid); 
	    }
	  }
	  else if( !emptyPeriodicBox )
	  {
	    for( axis=0; axis<numberOfDimensions; axis++ )

	      printF("Regrid::regrid:ERROR: an invalid box was made! [%i,%i]x[%i,%i]\n",
		     range(0,0),range(1,0),range(0,1),range(1,1));
	  }
	}
      }
      else
      {
        // ****************************************************************
	// ***** add new grids directly as new overlapping grids. *********
        // ****************************************************************
	MappedGrid & gb = gc[baseGrid];

	const IntegerArray & gid = gb.gridIndexRange();
        real pra[6];
	#define ra(side,axis) pra[(side)+2*(axis)]
	int g=0; // counts new grids added.
	for( BoxListIterator bli(boxList); bli; ++bli)
	{
	  const Box & box = boxList[bli];
	  bool ok=true;
	  int axis;
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    ra(0,axis)=(refineIndex(box.smallEnd(axis),axis)  -gid(Start,axis))/real(max(1,gid(End,axis)-gid(Start,axis)));
	    ra(1,axis)=(refineIndex(box.bigEnd(axis)+1,axis)  -gid(Start,axis))/real(max(1,gid(End,axis)-gid(Start,axis)));
            ok= ok && ra(1,axis)>ra(0,axis);
	  }
          for( axis=numberOfDimensions; axis<3; axis++ )
	  {
            ra(0,axis)=0.;
            ra(1,axis)=1.;
	  }
	  
	  if( ok )
	  {
	
	    ReparameterizationTransform & refine = 
	      *new ReparameterizationTransform (gb.mapping(),ReparameterizationTransform::restriction);

	    refine.incrementReferenceCount();
	    char buff[80];
	    refine.setName(Mapping::mappingName,sPrintF(buff,"grid%i-%i",baseGrid,g));
	    refine.setBounds(ra(0,0),ra(1,0),ra(0,1),ra(1,1),ra(0,2),ra(1,2));
	    // refine.scaleBounds(ra(0,0),ra(1,0),ra(0,1),ra(1,1),ra(0,2),ra(1,2));
            // refine.setBoundsForMulitpleReparameterizations(pra);
	
	    printf("Add a refinement grid as a base grid : g=%i baseGrid=%i bounds=[%g,%g][%g,%g][%g,%g]\n",
		   g,baseGrid,ra(0,0),ra(1,0),ra(0,1),ra(1,1),ra(0,2),ra(1,2));

	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
	      refine.setGridDimensions(axis,(refineIndex(box.bigEnd(axis)+1,axis)-
                                             refineIndex(box.smallEnd(axis),axis))*refinementRatio+1);
              // refine.setGridIndexRange(0,axis, box.smallEnd(axis)*refinementRatio);
              // refine.setGridIndexRange(1,axis, (box.bigEnd(axis)+1)*refinementRatio);
	      
             
	      if( refineIndex(box.smallEnd(axis),axis)==gid(Start,axis) && gb.boundaryCondition(Start,axis)>0 )
		refine.setBoundaryCondition(Start,axis,gb.boundaryCondition(Start,axis));
	      else
		refine.setBoundaryCondition(Start,axis,0);
            
	      if( refineIndex(box.bigEnd(axis)+1,axis)==gid(End,axis) && gb.boundaryCondition(End,axis)>0 )
		refine.setBoundaryCondition(End,axis,gb.boundaryCondition(End,axis));
	      else
		refine.setBoundaryCondition(End  ,axis,0);

	      for( int side=0; side<=1; side++ )
	      {
                refine.setNumberOfGhostPoints(side,axis,gb.numberOfGhostPoints(side,axis));
	      }
	      
	    }
	  
            MappedGrid mg(refine);
	    // set ghost lines 
	    for( int axis=0; axis<3; axis++ )
	    {
	      for( int side=0; side<=1; side++ )
	      {
		mg.numberOfGhostPoints()(side,axis)=gb.numberOfGhostPoints(side,axis);
	      
	      }
	    }

// 	    for( axis=0; axis<numberOfDimensions; axis++ )
// 	    {
// 	      // refine.setGridDimensions(axis,(box.bigEnd(axis)+1-box.smallEnd(axis))*refinementRatio+1);
//               mg.gridIndexRange()(0,axis)=box.smallEnd(axis)*refinementRatio;
//               mg.gridIndexRange()(1,axis)=(box.bigEnd(axis)+1)*refinementRatio;
// 	    }
	    

	    gc.add( mg );    // Add a new component grid, built from this Mapping.

	    // display(gc[gc.numberOfComponentGrids()-1].boundaryCondition(),"bc for grid added");
	  
	    refine.decrementReferenceCount();
	    g++;
	  }
	  else
	  {
	    printF("Regrid::regrid:ERROR: an invalid box was made! [%e,%e]x[%e,%e]\n",
		   ra(0,0),ra(1,0),ra(0,1),ra(1,1));
	  }
	}
      
      }
    }
  }
  
  timeForBuildGrids+=getCPU()-time0;
  return 0;
}
#undef ra

void Regrid::
setupCoarseIndexSpace(GridCollection & gc, int baseGrid, int level )
// ==================================================================================
//  /Description:
//     Assign the parameters used hwne building grids on a coarse index-space
// ==================================================================================
{
  if( !useCoarsenedIndexSpace )
  {
    iab(0,0)=iab(1,0)=iab(0,1)=iab(1,1)=iab(0,2)=iab(1,2)=0;
  }
  else
  {
    // If the base-grid index-space cannot be evenly divided by indexCoarseningFactor, then
    // we add add any extra cells to the first interval of the coarse index-space.
    // There will be at most (indexCoarseningFactor-1) extra cells 
    // 
    // *** indexCoarseningFactor=2 ***
    //  0        1     2     3     4     5     6     7 
    //  +--------+-----+-----+-----+-----+-----+-----+   coarse index-space 
    //  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   index-space
    //  0  1  2  3  4  5  ...                        N=15
    //  X  * iab(0,axis)=1
    //  X=iab(1,axis)
    //
    // *** indexCoarseningFactor=4 ***
    //  0                    1           2           3 
    //  +--------------------+-----------+-----------+   coarse index-space 
    //  +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+   index-space
    // -1  0  1  2  3  4  5  6  7  8  9 10           N=14
    //  X        * iab(0,axis)=2         
    //  X=iab(1,axis)
     
    MappedGrid & mg = gc[baseGrid];
    const int ratio=int(pow(refinementRatio,level-1)+.5);

    const IntegerArray & gid = mg.gridIndexRange();
    // The base level index-space includes ghost points on interpolation boundaries,
    // since we need to allow refinement patches to extend into the interpolation region.
    const IntegerArray & eid = mg.extendedIndexRange();
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      if( !mg.isPeriodic(axis) )
      {
	int numCells = eid(1,axis)-eid(0,axis);
	iab(0,axis) = (numCells % indexCoarseningFactor) + eid(0,axis);
	iab(1,axis) = eid(0,axis); // save the starting index 
      }
      else
      {
	// on a periodic grid we extend the index-space in the periodic direction since we should
	// never need to restrict refinements in this direction. 

	// int period=(extendedIndexRange(1,axis)-extendedIndexRange(0,axis));

	int numCells = eid(1,axis)-eid(0,axis);
	// iab(0,axis) = (numCells % indexCoarseningFactor) + eid(0,axis) -2*indexCoarseningFactor;
	// iab(1,axis)=eid(0,axis) -2*indexCoarseningFactor; // save the starting index 
	iab(0,axis) = (numCells % indexCoarseningFactor) + eid(0,axis);
	iab(1,axis) = eid(0,axis); // save the starting index 

      }
      if( ratio>1 )
      {
	iab(0,axis) *= ratio;
	iab(1,axis) *= ratio;
      }
      

      if( debug & 2 )
	printF("!!!!! setupCoarseIndexSpace: baseGrid=%i level=%i ratio=%i axis=%i: iab=[%i,%i] isPeriodic=%i "
               "gid=[%i,%i] eid=[%i,%i]\n",
	       baseGrid,level,ratio,axis,iab(0,axis),iab(1,axis),(int)mg.isPeriodic(axis),gid(0,axis),gid(1,axis),
	       eid(0,axis),eid(1,axis));
	
    }
      
  }
}


//\begin{>>RegridInclude.tex}{\subsection{regrid}} 
int Regrid::
regrid( GridCollection & gc, 
        GridCollection & gcNew,
	realGridCollectionFunction & error,
	real errorThreshhold,
        int refinementLevel /* = 1 */,
        int baseLevel /* = -1 */  )
// ===================================================================================
// /Description:
//   Build refinement grids to cover the "tagged points" where the error is
// greater than an errorThreshold.
//
//  Use a bisection approach to build the grids.
// 
// /gc (input) : old grid
// /gcOld (output) : new grid (must be a different object from gc).
// /error (input): user defined error function
// /errorThreshhold (input) : tag cells where the error is larger than this value.
// /refinementLevel (input) : highest refinement level to build, 
//       build refinement levels from baseLevel+1,..,refinementLevel
// /baseLevel (input) : this level and below stays fixed, by default baseLevel=refinementLevel-1 so that
//   only one level is rebuilt.
//      
//\end{RegridInclude.tex} 
// ===================================================================================
{
  if( gc.rcData == gcNew.rcData )
  {
    printF("Regrid::regrid:ERROR: gc and gcNew appear to be the same GridCollection! "
           "Supply regrid with two different GridCollections\n");
    return 1;
  }
//   if( ((numberOfBufferZones % indexCoarseningFactor) !=0) || 
//       ((widthOfProperNesting % indexCoarseningFactor) !=0) )
//   {
//     printF("Regrid::regrid:ERROR: numberOfBufferZones=%i and widthOfProperNesting=%i must be a multiple of"
// 	   " the indexCoarseningFactor=%i\n",numberOfBufferZones,widthOfProperNesting,indexCoarseningFactor);
//     Overture::abort("Regrid::regrid:error");
//   }

  numberOfDimensions=gc.numberOfDimensions();

  bool useErrorFunction=true;
  intGridCollectionFunction tagCollection(gc);

  if( gridAlgorithmOption==aligned )
  {
    return regridAligned(gc,gcNew,useErrorFunction,&error,errorThreshhold,tagCollection,refinementLevel,baseLevel);
  }
  else
  {
   return regridRotated(gc,gcNew,useErrorFunction,&error,errorThreshhold,tagCollection,refinementLevel,baseLevel);
  }
  
}

//\begin{>>RegridInclude.tex}{\subsection{regrid}} 
int Regrid::
regrid( GridCollection & gc,            
	GridCollection & gcNew,        
	intGridCollectionFunction & errorMask,
	int refinementLevel /* = 1 */, 
	int baseLevel  /* = -1 */)  
// ================================================================================================
// /Description:
//    Regrid based on an error mask.
// /gc (input) :   grid to regrid.
// /gcNew (input) : put new grid here (must be different from gc)
// /errorMask (input/output) : $\ne 0$ at points to refine, 0 otherwise. Note: this function may be changed on
// output. It will reflect the actual points refined, taking into account buffer zones and proper nesting.
//  NOTE: On an overlapping grid you should normally set the errorMask to zero at unused points.
// /refinementLevel (input) : highest level to refine
// /baseLevel (input) :  keep this level and below fixed, by default baseLevel=refinementLevel-1.
//\end{RegridInclude.tex} 
// ================================================================================================
{
  bool useErrorFunction=false;
  numberOfDimensions=gc.numberOfDimensions();

  regridAligned( gc,gcNew,useErrorFunction,NULL,0.,errorMask,refinementLevel,baseLevel);

  return 0;
}



//\begin{>>RegridInclude.tex}{\subsection{regridAligned}} 
int Regrid::
regridAligned( GridCollection & gc,  
               GridCollection & gcNew,
               bool useErrorFunction,
	       realGridCollectionFunction *pError,
	       real errorThreshhold,
               intGridCollectionFunction & tagCollection,
	       int refinementLevel /* = 1 */,
	       int baseLevel /* = -1 */  )
// ===========================================================================
// /Access: protected.
// /Description:
//   Build refinement grids to cover the "tagged points" where the error is
// greater than an errorThreshold.
//
//  Use a bisection approach to build the grids.
// 
// /refinementLevel (input) : highest refinement level to build, 
//       build refinement levels from baseLevel+1,..,refinementLevel
// /baseLevel (input) : this level and below stays fixed, by default baseLevel=refinementLevel-1 so that
//   only one level is rebuilt.
//
//\end{RegridInclude.tex} 
// ==========================================================================
{
  real time0=getCPU();

  // debug = 7;  // ***

  timeForRegrid=0.;
  timeForBuildGrids=0.;
  timeForBuildTaggedCells=0.;

  if( useErrorFunction )
  {
    assert( pError!=NULL );
  }

  Overture::checkMemoryUsage("Regrid::regridAligned (start)");  
  
  numberOfDimensions = gc.numberOfDimensions();
  Range Rx= numberOfDimensions;
  
  // we can only add at most one additional refinement level
  refinementLevel = min(refinementLevel,gc.numberOfRefinementLevels());
  int numberOfRefinementLevels=refinementLevel;
  
  // start refining from this level
  if( baseLevel<0 )
    baseLevel= refinementLevel-1;

  assert( baseLevel>=0 && baseLevel<gc.numberOfRefinementLevels() );
  
//   IndexType centering (D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE));
  IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  if( gridAdditionOption==addGridsAsRefinementGrids )
  {
    gc.update(GridCollection::THErefinementLevel);
    // gcNew.update(GridCollection::THErefinementLevel);
  }
  
  const int numberOfBaseGrids=gc.numberOfBaseGrids();

  assert( properNestingDomain==NULL && complementOfProperNestingDomain==NULL );
  properNestingDomain = new BoxList [numberOfRefinementLevels];
  complementOfProperNestingDomain = new BoxList [numberOfRefinementLevels];
  
//   assert( (widthOfProperNesting % indexCoarseningFactor) == 0 );
//   const int bufferWidth = widthOfProperNesting/indexCoarseningFactor;
  const int bufferWidth = widthOfProperNesting;

  int grid;
//   IntegerArray oldNumberOfRefinementLevels(numberOfBaseGrids);
//   oldNumberOfRefinementLevels=0;
//   for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
//   {
//     oldNumberOfRefinementLevels(gc.baseGridNumber(grid))=max(oldNumberOfRefinementLevels(gc.baseGridNumber(grid)),
// 							     gc.refinementLevelNumber(grid));
//   }

//   display(oldNumberOfRefinementLevels,"oldNumberOfRefinementLevels");
  
  // gridInfo[baseGrid][level0] holds the list of boxes to build on each grid and level
  // For efficiency we first build these lists and then replace all refinement levels at once.
  const int numberOfExtraLevels=numberOfRefinementLevels-baseLevel;
  // printF("***** numberOfExtraLevels=%i *******\n",numberOfExtraLevels);
  
//     gridInfo[baseGrid][level-baseLevel](0..10,g) for each refinement grid g on level, holds
//        0..5 : [n1a,n1b][n2a,n2b][n3a,n3b]
//        6..8 : refinement factor[axis] 
//        9..10 : processor range for the parallel distribution of this grid

  IntegerArray **gridInfo = new IntegerArray * [numberOfBaseGrids]; 
  int gb;
  for( gb=0; gb<numberOfBaseGrids; gb++ )
    gridInfo[gb] = new IntegerArray [numberOfExtraLevels];

  // *** Loop over the base grids and apply the refinement algorithm to each one independently ****
  int axis;
  for( gb=0; gb<numberOfBaseGrids; gb++ )
  {
    // the maximum number of refinement levels can only increase by one from the old grid.
    // refinementLevel = min(refinementLevel_,oldNumberOfRefinementLevels(gb)+1);

    int baseGrid=gc.baseGridNumber(gb);

    // printF("**** regridAligned baseGrid=%i **** \n",baseGrid);
    // printInfo( gc );

    // To build lists of boxes used to ensure proper nesting of grid.
    // NOTE: The proper nesting domain is ONLY built for the baseLevel grids, i.e. the grids that
    //   do not change. We build fine grid versions of this proper nesting domain.
    //  The newer levels are properly nested automatically since the fine grid tagged
    //  cells are added to the coarse grid tagged cells. 
    buildProperNestingDomains(gc,baseGrid,refinementLevel,baseLevel,numberOfRefinementLevels );

    // here are the lists that will hold the boxes for each level we build.
    BoxList *refinementBoxList = new BoxList [refinementLevel-baseLevel];

    // Build a list of tagged cells. These are saved as an indirect addressing array:
    //   (ia(i,0),ia(i,1),ia(i,2))  = (i1,i2,i3) : this point is tagged (in the global index space
    //     of this base grid.)

    int level;
    // We build the finest level first. These grids must sit within the current coarser level.
    // We then rebuild the next coarser level and so on...
    for( level=refinementLevel; level>baseLevel; level-- )
    {

      // Set some parameters if we use a coarse index space for building grids
      setupCoarseIndexSpace(gc,baseGrid,level);

      int coarserLevel=level-1;

      GridCollection & rl = gridAdditionOption==addGridsAsRefinementGrids ? gc.refinementLevel[coarserLevel] : gc;

      IntegerArray ia;  // holds indirect addressing list of all tagged cells 

      // find all refinement grids on this level that have the same base grid
      for( int gr=0; gr<rl.numberOfComponentGrids(); gr++ )
      {
	if( rl.baseGridNumber(gr)==baseGrid )
	{
	  grid=rl.gridNumber(gr);

	  MappedGrid & mg = gc[grid];

	  intMappedGridFunction & tag = tagCollection[grid];
    
          // printF(" grid=%i : tag [%i,%i]x[%i,%i] dimension=[%i,%i]x[%i,%i]\n",
	  //	 grid,tag.getBase(0),tag.getBound(0),tag.getBase(1),tag.getBound(1),
          //       mg.dimension(0,0),mg.dimension(1,0),mg.dimension(0,1),mg.dimension(1,1));

          const realArray & gridError = useErrorFunction ? (*pError)[grid] : Overture::nullRealDistributedArray();
  	  buildTaggedCells( mg,tag,gridError,errorThreshhold,useErrorFunction );

	  // Enforce proper nesting 
	  if( refinementLevel>1 && baseLevel>0 )
	  {
	    // enforce proper nesting
	    // where mg.box() intersects complementOfProperNestingDomain[coarserLevel]
	    // set tag to zero
	  
	    Box box = cellCenteredBox(mg);
	    BoxList intersectionList = intersect(complementOfProperNestingDomain[coarserLevel],box);

	    for( BoxListIterator bli(intersectionList); bli; ++bli)
	    {
	      Box & box = intersectionList[bli];
	      for( axis=0; axis<numberOfDimensions; axis++ )
		Iv[axis]=Range(refineIndex(box.smallEnd(axis),axis),
                               refineIndex(box.bigEnd(axis)+1,axis)-1);  // *check* this 

              if( debug & 2 )
	        printF(" Enforcing proper nesting on grid=%i : remove cells [%i,%i]x[%i,%i]\n",
		       grid,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
	    
	      tag(I1,I2,I3)=0;
	    }
      
	  }

          // look at cell centers only *** include interp pts on base grids
	  getIndex(mg.extendedIndexRange(),I1,I2,I3);  // *wdh* 000626

	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    if( mg.boundaryCondition(End,axis)>0 )
	      Iv[axis]=Range(Iv[axis].getBase(),mg.indexRange(End,axis)-1);
            else if( mg.boundaryCondition(End,axis)<0 ) // *wdh* 000811for annulus
              Iv[axis]=Range(Iv[axis].getBase()-1,mg.indexRange(End,axis)+1);
	  }


          if( level<refinementLevel )
	  {
	    // if new grids at a finer level have been built on top of this one, tag points
            // on the coarser level to ensure proper nesting.

            BoxList & finerBoxList = refinementBoxList[level-baseLevel];
            BoxList finerBoxListCoarsened = coarsen(finerBoxList,refinementRatio);

	    Box box = cellCenteredBox(mg);
            if( coarserLevel==0 )
	    {
              for( axis=0; axis<numberOfDimensions; axis++ )
	      {
                if( mg.boundaryCondition(Start,axis)==0 )
                  box.setSmall(axis,refineIndex(box.smallEnd(axis),axis)-1);
                if( mg.boundaryCondition(End,axis)==0 )
                  box.setBig(axis,refineIndex(box.bigEnd(axis)+1,axis));  // check this 
		
	      }
	    }

            // getIndex(extendedGridIndexRange(mg),J1,J2,J3);
	    // Box box = buildBox( Jv );
	    
	    BoxList intersectionList = intersect(finerBoxListCoarsened,box);

	    if( debug & 2 && myid==0 )
	    {
	        printF(" --Marking coarser level grid=%i (coarserLevel=%i) from finer levels? box:\n",coarserLevel,
                        grid);
		cout << box << endl;
		cout << " finerBoxList: " << finerBoxList << endl;
                printF(" refinementRatio = %i\n",refinementRatio);
		
		cout << " finerBoxListCoarsened: " << finerBoxListCoarsened << endl;
                printF("Here are the boxes we need to mark on the coarser level:\n");
		cout << "intersectionList: " << intersectionList << endl;
	    }


	    for( BoxListIterator bli(intersectionList); bli; ++bli)
	    {
	      Box & box = intersectionList[bli];
	      for( axis=0; axis<numberOfDimensions; axis++ )
		Jv[axis]=Range(max(Iv[axis].getBase(), refineIndex(box.smallEnd(axis),axis)  -bufferWidth),  
                               min(Iv[axis].getBound(),refineIndex(box.bigEnd(axis)+1,axis)-1+bufferWidth));
              if( debug & 2 )
	        printF(" --Marking coarser level grid=%i from finer levels: add cells [%i,%i]x[%i,%i] "
                       "bufferWidth=%i \n",
		       grid,J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),bufferWidth);
	    
	      tag(J1,J2,J3)=1;
	    }
            tag.periodicUpdate();
            if( debug & 8 )
              display(tag,"Here is tag after periodic update","%2i");
	    
	  }

          #ifdef USE_PPP
	    intSerialArray tagLocal; getLocalArrayWithGhostBoundaries(tag,tagLocal);
            int n1a = max(I1.getBase(),  tagLocal.getBase(0)+tag.getGhostBoundaryWidth(0));
	    int n1b = min(I1.getBound(),tagLocal.getBound(0)-tag.getGhostBoundaryWidth(0));
            int n2a = max(I2.getBase(),  tagLocal.getBase(1)+tag.getGhostBoundaryWidth(1));
	    int n2b = min(I2.getBound(),tagLocal.getBound(1)-tag.getGhostBoundaryWidth(1));
            int n3a = max(I3.getBase(),  tagLocal.getBase(2)+tag.getGhostBoundaryWidth(2));
	    int n3b = min(I3.getBound(),tagLocal.getBound(2)-tag.getGhostBoundaryWidth(2));

            if( n1a>n1b || n2a>n2b || n3a>n3b )
	    {
              continue;
 	    }
	    
            I1=Range(n1a,n1b);
            I2=Range(n2a,n2b);
            I3=Range(n3a,n3b);
	    
	  #else
	    const intSerialArray & tagLocal = tag;
          #endif
	  IntegerArray tag0(I1,I2,I3);

	  tag0 = tagLocal(I1,I2,I3);
	  //     ia = tag(I1,I2,I3).indexMap(); // A++ bug
	  if( debug & 8 )
	  {
	    char buff[80];
	    display(tag0,sPrintF(buff,"tag0 for grid %i",grid),"%2i");
	  }
	  
	  IntegerArray ia2;
	  if( !useCoarsenedIndexSpace )
	  {
	    ia2 = tag0.indexMap();
	  }
	  else
	  {
            // make a list of the tagged cells on the coarsened index space

            // --> maybe just make a coarsen tag array 
            Index J1,J2,J3;
	    
            J1=Range(coarsenIndexLower(I1.getBase(),0),coarsenIndexLower(I1.getBound(),0));
            J2=Range(coarsenIndexLower(I2.getBase(),1),coarsenIndexLower(I2.getBound(),1));
            J3=Range(coarsenIndexLower(I3.getBase(),2),coarsenIndexLower(I3.getBound(),2));
	    
            // mark cell-centers on the coarsened array tagc
            IntegerArray tagc(J1,J2,J3);
	    tagc=0;
            int i1,i2,i3;
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( tag0(i1,i2,i3)!=0 )
	      {
		int j1=coarsenIndexLower(i1,0), j2=coarsenIndexLower(i2,1), j3=coarsenIndexLower(i3,2);
		tagc(j1,j2,j3)=1;
	      }
	    }
	    
	    ia2 = tagc.indexMap();

	  }
		
        
	  if(ia2.getLength(0)>0 )
	  {
	    Range R2=ia2.getLength(0);
	    // if( min(ia2(R2,axis2))<I2.getBase() )

#ifdef USE_OLD_APP
            if( numberOfDimensions>1 )
	      ia2(R2,axis2)+=I2.getBase(); // **** A++ bug ****
#endif
	    // append ia2 to ia
	    int numberOfTaggedCells=ia.getLength(0);
	    int newNumberOfTaggedCells=ia2.getLength(0);
	  
	    Range R = numberOfTaggedCells+newNumberOfTaggedCells;
	  
	    ia.resize(R,Rx); // maybe should allocate more to begin with ??
	    Range R0(numberOfTaggedCells,numberOfTaggedCells+newNumberOfTaggedCells-1);
	  
	    ia(R0,Rx)=ia2;
	  }
	
	}
      } // end for gr
      if( debug & 8 )
      {
        printF("List of tagged cells: \n");
	for( int i=0; i<ia.getLength(0); i++ )
	{
	  printF("(%i,%i) ",ia(i,0),ia(i,1));
	  if( (i+1)%10 == 0 ) printf("\n");
	}
	printF("\n");
        // display(ia,"List of tagged cells: ia","%3i");
      }
      
      BoxList & boxList = refinementBoxList[level-baseLevel-1];

      int numberOfTaggedCells=ia.getLength(0);
      #ifdef USE_PPP
        numberOfTaggedCells=ParallelUtility::getSum(numberOfTaggedCells);
      #endif

      if( numberOfTaggedCells == 0 )
      {
        boxList.clear();
        if( debug & 2 )
          printF(" *** level = %i : there are no refinement grids at this level\n",level);

	continue;  // there are no tagged points on this base grid, at this level
      }
      // BoxList boxList(centering);  // here is the list that will hold the boxes for this level

      if( debug & 2)
	printF("regridAligned: total number of tagged cells = %i \n",numberOfTaggedCells);

      Range R=ia.getLength(0);
    
      Box mainBox;  
      if( limitBoxSize )
      {
        // is this correct ? 
	Box boundingBox=baseLevel!=0 ? cellCenteredBox(gc[baseGrid],refinementRatio) : cellCenteredBaseBox(gc[baseGrid]); 
        mainBox=getBoundedBox(ia,boundingBox);  
      }
      else
      {
	mainBox= getBox( ia );
      }
	
      // try this: Box mainBox = getBoundedBox( ia );

      // adjust a periodic grid
      // we can only allow a periodic grid on the finest level
//        if( false && level==1 ) 
//          fixPeriodicBox( gc[baseGrid], mainBox,ia, level );

      // *** recursively split the main box ****
      splitNumber=0;
      splitBox( mainBox, ia, boxList, level );


      // merge boxes if possible
      if( debug & 2 && myid==0 )
	cout << "*** BEFORE merge, boxlist:" << boxList << endl;

      int numberMerged = boxList.simplify();
      if( debug & 2 )
        printF(" *** level=%i, number merged= %i\n",level,numberMerged);

      if( numberMerged>0 ) // ** We seem to need to do this, at least in 3D *** *wdh* 030627
      {
	// NOTE: BoxList::simplify() does not check to see if a merged box can be itself merged
        //   with a previous box in the list
        const int maxIteration=10;
        int iteration=0;
	while( numberMerged>0 && iteration<maxIteration )
	{
         numberMerged = boxList.simplify();
         iteration++;
         if( debug & 2 && numberMerged>0 ) 
	   printF("**** Regrid:INFO %i boxes were merged on additional call %i to simplify ****\n",
		  iteration,numberMerged);
	}
	if( iteration==maxIteration )
	{
	  printF("**** Regrid:WARNING: %i merge operations were performed. Not all boxes may be merged\n",
		 maxIteration);
	}
	
      }
      
    
      if( debug & 2 && myid==0 )
	cout << "*** final boxList (number merged=" << numberMerged << "):" << boxList << endl;
    

    }  // end for level
  
    // Add new grids corresponding to the boxes we have created.
    buildGrids(gc,gcNew,baseGrid,baseLevel,refinementLevel,refinementBoxList,gridInfo);
    
    delete [] refinementBoxList;

  } // end for base grid

  if( debug & 2 )
  {
    for( int ll=0; ll<gcNew.numberOfRefinementLevels(); ll++ )
    {
      printF("+++Regrid::regrid:Before replaceRefinementLevels:  "
	     "level %i gcNew.refinementLevel[ll].numberOfComponentGrids()=%i \n",
	     ll,gcNew.refinementLevel[ll].numberOfComponentGrids());
    }
  }

  // parallel load balancing of the new grids
  // buildGrids above does not make any new MappedGrids, it only creates the gridInfo

  const int np = max(1,Communication_Manager::numberOfProcessors());
  if( loadBalance && (np>1 || true) )
  {
    if( false ) // for testing:
      loadBalancer.setLoadBalancer(LoadBalancer::sequentialAssignment);

    GridDistributionList & gridDistributionListOld = gc->gridDistributionList;
    GridDistributionList & gridDistributionList = gcNew->gridDistributionList;

    // loadBalancer.assignWorkLoads( cg,gridDistributionList ); // don't do this

    // **** assign work-loads based on gridInfo: ****

    int gNew=-1;  // This will be the first new grid that we will add
    for( int g=0; g<gc.numberOfGrids(); g++ )
    {
      if( gc.refinementLevelNumber(g)>baseLevel )
      { // find the first grid that is on a level>baseLevel -- this will be the first grid replaced
	gNew=g;
	break;
      }
      // Grid g is on a level<=baseLevel and will keep the same work-load etc. 
      if( g<gridDistributionListOld.size() )
      {
	if( g >= gridDistributionList.size() )
	  gridDistributionList.push_back(gridDistributionListOld[g]);  
	else
	  gridDistributionList[g]=gridDistributionListOld[g];          
      }
      else
      {
        // The old GridCollection does not have a GridDistribution  -- so we create one
	printF("Regrid:WARNING: Making a GridDistribution for grid %i on the old GridCollection gc.\n",g);
	
        GridDistribution gridDistribution;
        gridDistribution.setGridAndRefinementLevel(g,gc.refinementLevelNumber(g));
	
        MappedGrid & mg = gc[g];
        // set the work load equal to the number of grid points
	const IntegerArray & d = mg.dimension();
	real workLoad = (d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);
        // gridDistribution.setWorkLoad(workLoad);
        int gridPoints[3];
	for( int dir=0; dir<3; dir++ )
	  gridPoints[dir]=d(1,dir)-d(0,dir)+1;
        gridDistribution.setWorkLoadAndGridPoints(workLoad,gridPoints);

        // find which processors this grid is distributed over:
	Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
	const intSerialArray & processorSet = partition.getProcessorSet();  // this should be a const fn
        int pStart=processorSet.getBase(0), pEnd=processorSet.getBound(0);
	gridDistribution.setProcessors(pStart,pEnd);
	
	if( g >= gridDistributionList.size() )
	  gridDistributionList.push_back(gridDistribution);  
	else
	  gridDistributionList[g]=gridDistribution;
      }
    }
    if( gNew==-1 ) gNew=gc.numberOfGrids();
    
    IntegerArray range(2,3), factor(3);
    int gridPoints[3];
    for( int level=baseLevel+1; level<=refinementLevel; level++ ) // loop over new levels
    {
      // assign grids on refinementLevel = "level"
      for( int bg=0; bg<numberOfBaseGrids; bg++ ) // loop over base grids
      {
	const IntegerArray & info = gridInfo[bg][level-(baseLevel+1)];
	int ngrl=info.getLength(1);  // number of refinement grids for this base grid and level
	for( int rg=0; rg<ngrl; rg++ )
	{
	  range(0,0)=info(0,rg); 
	  range(1,0)=info(1,rg);
	  range(0,1)=info(2,rg);
	  range(1,1)=info(3,rg);
	  range(0,2)=info(4,rg);
	  range(1,2)=info(5,rg);
	  factor(0) =info(6,rg);
	  factor(1) =info(7,rg);
	  factor(2) =info(8,rg);

          real workLoad= (range(1,0)-range(0,0)+1)*(range(1,1)-range(0,1)+1)*(range(1,2)-range(0,2)+1)*
                         factor(0)*factor(1)*factor(2);
          if( gNew >= gridDistributionList.size() )
	    gridDistributionList.push_back(GridDistribution());

          gridDistributionList[gNew].setGridAndRefinementLevel(gNew,level);
          // gridDistributionList[gNew].setWorkLoad(workLoad);
          for( int d=0; d<3; d++ )
            gridPoints[d]=range(1,d)-range(0,d)+1;
          gridDistributionList[gNew].setWorkLoadAndGridPoints(workLoad,gridPoints);
          if( debug & 2 )
	  {
	    printF("Regrid: assign a workLoad of %8.2e to grid=%i (level=%i) bounds=[%i,%i][%i,%i][%i,%i]"
		   " factor=[%i,%i,%i]\n",
		   workLoad,gNew,level,range(0,0),range(1,0),range(0,1),range(1,1),range(0,2),range(1,2),
		   factor(0),factor(1),factor(2));
	  }
          gNew++;
	}
      }
    }
    int newNumberOfGrids=gNew;
    while( gridDistributionList.size()>newNumberOfGrids )  // remove any extra entries in the list
      gridDistributionList.erase(gridDistributionList.end());

    // load balance the refinement level grids, keeping the previous levels unchanged. 
    loadBalancer.determineLoadBalance( gridDistributionList,baseLevel+1 );
  }

  real timeA=getCPU();
  if( gridAdditionOption==addGridsAsRefinementGrids )
  {
    gcNew.replaceRefinementLevels( baseLevel+1,numberOfRefinementLevels+1,gridInfo );
  }
  else
  {
    // Add grids as base grids 
    // addRefinementsAsBaseGrids( gcNew, baseLevel+1,numberOfRefinementLevels+1,gridInfo ); 
  }
  
  real timeB=getCPU()-timeA;
  if( debug & 2 ) printF(" time for replaceRefinements: cpu=%8.2e\n",timeB);
  timeForBuildGrids+=timeB;

  if( gridAdditionOption==addGridsAsRefinementGrids )
    gcNew.update(GridCollection::THErefinementLevel);
  
  if( debug & 2 )
  {
    for( int ll=0; ll<gcNew.numberOfRefinementLevels(); ll++ )
    {
      printF("+++Regrid::regrid:After replaceRefinementLevels:  "
	     "level %i gcNew.refinementLevel[ll].numberOfComponentGrids()=%i \n",
	     ll,gcNew.refinementLevel[ll].numberOfComponentGrids());
    }
  }

  for( gb=0; gb<numberOfBaseGrids; gb++ )
    delete [] gridInfo[gb];
  delete [] gridInfo;

  delete [] properNestingDomain;
  properNestingDomain=NULL;
  delete [] complementOfProperNestingDomain;
  complementOfProperNestingDomain=NULL;
  
  Overture::checkMemoryUsage("Regrid::regridAligned (end)");  

  timeForRegrid+=getCPU()-time0;
  return 0;
}

// int Regrid::
// addRefinementsAsBaseGrids(GridCollection & gc, int level0, int numberOfRefinementLevels0, IntegerArray **gridInfo )
// {
//   const int numberOfDimensions=gc.numberOfDimensions();
//   const int numberOfBaseGrids0 = gc.numberOfBaseGrids(); // initial number of base grids 
  
//   IntegerArray range(2,3), factor(3), gridIndexRange(2,3);
//   RealArray parameterBounds(2,3);
//   for( int level=level0; level<numberOfRefinementLevels0; level++ ) // loop over new levels
//   {
//     // assign grids on refinementLevel = "level"
//     for( int bg=0; bg<numberOfBaseGrids0; bg++ ) // loop over base grids
//     {
//       MappedGrid & gb = gc[bg];  // base grid
//       const IntegerArray & gid = gb.gridIndexRange();

//       const IntegerArray & info = gridInfo[bg][level-level0];
//       int ngrl=info.getLength(1);  // number of refinement grids for this base grid and level
//       for( int rg=0; rg<ngrl; rg++ )
//       {
// 	range(0,0)=info(0,rg); 
// 	range(1,0)=info(1,rg);
// 	range(0,1)=info(2,rg);
// 	range(1,1)=info(3,rg);
// 	range(0,2)=info(4,rg);
// 	range(1,2)=info(5,rg);
// 	factor(0) =info(6,rg);
// 	factor(1) =info(7,rg);
// 	factor(2) =info(8,rg);


// 	ReparameterizationTransform &refine = *new ReparameterizationTransform
// 	  (*gb.mapping().mapPointer, ReparameterizationTransform::restriction);
// 	refine.incrementReferenceCount();

//         // See also Regrid.020502.C  line 1346


// 	// set bc, share, gridIndexRange
// 	for( int axis=0; axis<3; axis++ )
// 	{
// 	  if( axis<numberOfDimensions )
// 	  {
// 	    for( int side=0; side<=1; side++ )
// 	    {
// 	      gridIndexRange(side,axis) = factor(axis) * range(side,axis);

// 	      parameterBounds(side,axis)=(range(side,axis)-gid(0,axis))/real(max(1,gid(1,axis)-gid(0,axis)));

// 	      if( range(side,axis)==gid(0,axis) && gb.boundaryCondition(side,axis)>0 )
// 	      {
// 		refine.setBoundaryCondition(side,axis,gb.boundaryCondition(side,axis));
// 		refine.setShare(side,axis,gb.sharedBoundaryFlag(side,axis));
// 	      }
// 	      else
// 	      {
// 		refine.setBoundaryCondition(side,axis,0); 
// 	      }
		
// 	    }
// 	    refine.setGridIndexRange(0,axis, 0);
// 	    refine.setGridIndexRange(1,axis, gridIndexRange(1,axis)-gridIndexRange(0,axis) );

// 	  }
// 	  else
// 	  {
// 	    parameterBounds(0,axis)=0.;
// 	    parameterBounds(1,axis)=1.;
// 	  }
// 	}
	
// 	refine.setBounds(parameterBounds(0,0), parameterBounds(1,0),
// 			 parameterBounds(0,1), parameterBounds(1,1),
// 			 parameterBounds(0,2), parameterBounds(1,2));

// 	printf("Add a refinement grid as base grid : bounds=[%g,%g][%g,%g][%g,%g]\n",
// 	       parameterBounds(0,0), parameterBounds(1,0),
// 	       parameterBounds(0,1), parameterBounds(1,1),
// 	       parameterBounds(0,2), parameterBounds(1,2));

// 	MappedGrid mg(refine);
	  
// 	// set ghost lines 
// 	for( int axis=0; axis<3; axis++ )
// 	{
// 	  for( int side=0; side<=1; side++ )
// 	  {
// 	    mg.numberOfGhostPoints()(side,axis)=gb.numberOfGhostPoints(side,axis);
	      
// 	  }
// 	}
	  
// 	gc.add(mg);
	  
// 	if( refine.decrementReferenceCount() == 0 ) delete &refine;
//       }
//     }
//   }
//   gc.updateReferences();
  
//   return 0;
// }



static int numberOfSplits=0;  // ************************************* fix this ***********************

//\begin{>>RegridInclude.tex}{\subsection{splitBoxRotated}} 
int Regrid::
splitBoxRotated( RotatedBox & box, ListOfRotatedBox & boxList, 
                 realArray & xa, int refinementLevel )
// ===========================================================================================
// /Description: Build possibly rotated boxes
//\end{RegridInclude.tex} 
// =========================================================================================
{
  box.setPoints(xa);

  if( numberOfSplits>200 || box.getEfficiency() >= efficiency || 
      box.numberOfPoints()<minimumBoxSize*2 )
  {
    if( box.numberOfPoints() > 0 )
    {
      if( true || debug & 2)
      {
	cout << "add box to list: box" << endl;
	box.display();
        cout << ", numPts=" << box.numberOfPoints() << ", num(xa)=" << xa.getLength(0) << 
	  ", efficiency=" << box.getEfficiency() << ", efficiency bound=" << efficiency << endl;
      }
      if( debug & 8 )
        display(xa,"xa","%3i");
      
      boxList.addElement(box);
      
    }
  }
  else
  {
    // *** here we bisect the box ****

    numberOfSplits++;
    // split the box into two
    if( true || debug & 2)
    {
      cout << "Split box = " << endl;
      box.display();
      cout << ", numPts=" << box.numberOfPoints() << "efficiency=" << box.getEfficiency() << endl;
    }
    
    Range R=xa.getLength(0);
    // const int numberOfDimensions=xa.getLength(1)-1;
    assert( numberOfDimensions>0 );
    
    int longAxis = box.halfAxesLength[0] >= box.halfAxesLength[1] ? 0 : 1;
    
/* -----
    box1(0)=box(0)-.5*box(3+3*longAxis  );     // mean[0];
    box1(1)=box(1)-.5*box(3+3*longAxis+1);     // mean[1];
    box1(2)=box(2);
    box1(3)=longAxis==0 ? .5*box(3) : box(3); // ev(0,0);
    box1(4)=longAxis==0 ? .5*box(4) : box(4); // ev(1,0);
    box1(5)=box(5);
    box1(6)=longAxis==1 ? .5*box(6) : box(6);  // ev(0,1);
    box1(7)=longAxis==1 ? .5*box(7) : box(7); // ev(1,1);
    box1(8)=box(8); // 0.;
    box1(9)=longAxis==0 ? box(9)*.5 : box(9); // ax[0];
    box1(10)=longAxis==1 ? box(10)*.5 : box(10); // ax[1];
    box1(11)=box(11); 

    printF("***split box : box1: longAxis=%i ***\n",longAxis);
    displayBox(box1);

    box2=box1;
    box2(0)+=box(3+3*longAxis  ); 
    box2(1)+=box(3+3*longAxis+1); 

    printF("***split box : box2:***\n");
    displayBox(box2);
----- */

    realArray xa2;
    intArray ia2;

    // int lap1 = (longAxis+1)%numberOfDimensions;

    realArray xDot(R);
    xDot=(xa(R,0)-box.centre[0])*box.axisVector[0][longAxis]+(xa(R,1)-box.centre[1])*box.axisVector[1][longAxis];

    real xDotMin=min(xDot);
    real xDotMax=max(xDot);
    // real xDotAve=.5*(xDotMin+xDotMax);
    
    real cutValue=0.;
    if( !useSmartBisection && xDotMax-xDotMin>0. )
    {
      // build a histogram so we can do a smart bisection
      // The array xDot holds the "longAxis" coordinate of each point -- we need to 
      // put these values into a discrete set of bins

      int ha=0, hb=20;   // 20 points in the histogram
      real hLength=hb-ha;
    
      realArray h(R);
      h= (xDot+(ha-xDotMin))*(hLength/(xDotMax-xDotMin));  // scale to [0,hLength];

      
      intArray histogramIndex(R);
      equals(histogramIndex,evaluate(h+.5));  // find closest integer

      assert( max(histogramIndex)<=hb && min(histogramIndex)>=ha );
      
      IntegerArray histogram(Range(ha,hb));
      histogram=0;
      for( int i=0; i<=R.getBound(); i++ )
      {
	histogram(histogramIndex(i))+=1;
      }

      CutStatus status;
      int cutPoint=findCut(histogram.getDataPointer(),ha,hb,status);

      cutValue=xDotMin + (xDotMax-xDotMin)*real(cutPoint-ha)/(hb-ha);

      if( true || debug & 2)
        printF(" cutPoint=%i, cutStatus=%i, [%i,%i], cutValue=%e in [%8.2e,%8.2e] \n", 
                  cutPoint,status,ha,hb,cutValue,xDotMin,xDotMax);

    }


    intArray mask;
    mask=xDot <= cutValue;
    ia2=mask.indexMap();
    if( ia2.getLength(0)>0 )
    {
      Range Rxa=numberOfDimensions+1;
      Range R2=ia2.getLength(0);
      
      xa2.redim(R2,Rxa);
      for( int axis=0; axis<=numberOfDimensions; axis++ )
        xa2(R2,axis)=xa(ia2,axis);

      // display(xa2,"xa2 for box1");
      
      RotatedBox & box1 = * new RotatedBox (numberOfDimensions);
      splitBoxRotated(box1, boxList, xa2,refinementLevel);
    }
    
    ia2.redim(0);
    xa2.redim(0);
    ia2=(mask==0).indexMap();
    if( ia2.getLength(0)>0 )
    {
      Range Rxa=numberOfDimensions+1;
      Range R2=ia2.getLength(0);
      
      xa2.redim(R2,Rxa);
      for( int axis=0; axis<=numberOfDimensions; axis++ )
        xa2(R2,axis)=xa(ia2,axis);

      RotatedBox & box2 = * new RotatedBox (numberOfDimensions);
      splitBoxRotated(box2,boxList, xa2,refinementLevel);
    }

  }

  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{merge}} 
int Regrid::
merge( ListOfRotatedBox & boxList )
// ==================================================================================
// /Description:
//   Attempt to merge rotated boxes.
//\end{RegridInclude.tex} 
// ==================================================================================
{
  
  int numberOfBoxes=boxList.getLength();
  int numberMerged=0;

  real distance =.1;  // boxes this close are said to intersect
  

  for( int b=0; b<boxList.getLength(); b++ )
  {
    RotatedBox & box = boxList[b];
    
    for( int b2=b+1; b2<boxList.getLength(); b2++ )
    {
      if( b2!=b )
      {
        RotatedBox & box2 = boxList[b2];

	// attempt to merge box and box2
        if( box.intersects(box2,distance) )
	{
          // build a box about all the points in box and box2
	  RotatedBox & newBox = * new RotatedBox(box.numberOfDimensions);

          // we need to merge the two sets of tagged points.
          Range R=box.xa.getLength(0)+box2.xa.getLength(0);
	  
          Range Rxa=box.xa.dimension(1);
          realArray xa(R,Rxa);

	  Range R0=box.xa.dimension(0);
          xa(R0,Rxa)=box.xa;

          R0=box2.xa.dimension(0)+box.xa.getLength(0);
          xa(R0,Rxa)=box2.xa;
	  
	  newBox.setPoints( xa );

	  if( newBox.getEfficiency() >= efficiency*.9 ) // **** reduce efficiency here ??
	  {
	    // keep this merge
            numberMerged++;
            printF(" box %i (eff=%8.2e)  and box %i (eff=%8.2e) were merged (eff=%8.2e) c=(%8.2e,%8.2e). \n",
               b,box.getEfficiency(),b2,box2.getEfficiency(),newBox.getEfficiency(),newBox.centre[0],
                   newBox.centre[1]);

            boxList.setElementPtr(&newBox,b);
	    delete &box;
	    boxList.deleteElement(b2);

            b--;  // We need to redo box b since it has been replaced
            break;
	  }
	  else
	  {
	    printF(" box %i (eff=%8.2e)  and box %i (eff=%8.2e) were NOT merged (eff=%8.2e). \n",b,
                   box.getEfficiency(),b2,box2.getEfficiency(),newBox.getEfficiency());
	    delete &newBox;
	  }

	}
      }
    }
  }

  return numberMerged;
}

//\begin{>>RegridInclude.tex}{\subsection{regridRotated}} 
int Regrid::
regridRotated( GridCollection & gc,  
               GridCollection & gcNew,
	       bool useErrorFunction,
	       realGridCollectionFunction *pError,
	       real errorThreshhold,
	       intGridCollectionFunction & tagCollection,
	       int refinementLevel /* = 1 */,
	       int baseLevel /* = -1 */  )
// ===========================================================================
// /Description:
//   Build refinement grids to cover the "tagged points" where the error is
// greater than an errorThreshold.
//
//  Use a bisection approach allowing rotated grids.
// 
// /refinementLevel (input) : highest refinement level to build, 
//       build refinement levels from baseLevel+1,..,refinementLevel
// /baseLevel (input) : this level and below stays fixed, by default baseLevel=refinementLevel-1 so that
//   only one level is rebuilt.
//
//\end{RegridInclude.tex} 
// ==========================================================================
{
  numberOfDimensions = gc.numberOfDimensions();
  Range Rx= numberOfDimensions;
  
  // const int numberOfRefinementLevels=refinementLevel;

  // start refining from this level
  if( baseLevel<0 )
    baseLevel= refinementLevel-1;

  assert( baseLevel>=0 && baseLevel<gc.numberOfRefinementLevels() );
  

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3]; //  &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  if( gridAdditionOption==addGridsAsRefinementGrids )
    gcNew.update(GridCollection::THErefinementLevel);
  
  const int numberOfBaseGrids=gc.numberOfBaseGrids();
  

  // intGridCollectionFunction tagCollection(gc);  // holds tagged points.

  // *** Loop over the base grids and apply the refinement algorithm to each one independently ****
  int gb,grid,axis;
  for( gb=0; gb<numberOfBaseGrids; gb++ )
  {
    int baseGrid=gc.baseGridNumber(gb);
    
    // here are the lists that will hold the boxes for each level we build.
    ListOfRotatedBox *refinementBoxList = new ListOfRotatedBox [refinementLevel-baseLevel];

    // Build a list of tagged cells. These are saved as an indirect addressing array:
    //   (ia(i,0),ia(i,1),ia(i,2))  = (i1,i2,i3) : this point is tagged (in the global index space
    //     of this base grid.)

    int level;
    for( level=refinementLevel; level>baseLevel; level-- )
    {
      int coarserLevel=level-1;

      GridCollection & rl = gridAdditionOption==addGridsAsRefinementGrids ? gc.refinementLevel[coarserLevel] : gc;

      intArray ia;  // holds indirect addressing list of all tagged cells 
      realArray xa; // holds indirect addressing list of all tagged cells verticies
      

      // find all refinement grids on this level that have the same base grid
      for( int gr=0; gr<rl.numberOfComponentGrids(); gr++ )
      {
	if( rl.baseGridNumber(gr)==baseGrid )
	{
	  grid=rl.gridNumber(gr);

	  MappedGrid & mg = gc[grid];

	  intMappedGridFunction & tag = tagCollection[grid];
    
	  // buildTaggedCells( mg,tag,error[grid],errorThreshhold,false );
          const realArray & gridError = useErrorFunction ? (*pError)[grid] : Overture::nullRealDistributedArray();
  	  buildTaggedCells( mg,tag,gridError,errorThreshhold,useErrorFunction,false );


	  getIndex(mg.extendedIndexRange(),I1,I2,I3);   // *wdh* 000626

// 	  for( axis=0; axis<numberOfDimensions; axis++ )
// 	  {
// 	    if( mg.boundaryCondition(End,axis)>0 )
// 	      Iv[axis]=Range(Iv[axis].getBase(),mg.indexRange(End,axis)-1);
// 	  }


	  intArray tag0(I1,I2,I3);
	  tag0 = tag(I1,I2,I3);
	  //     ia = tag(I1,I2,I3).indexMap(); // A++ bug
	  if( debug & 4 )
	    display(tag0,"tag0","%2i");

	  intArray ia2;
	  ia2 = tag0.indexMap();

	  if(ia2.getLength(0)>0 )
	  {
	    Range R2=ia2.getLength(0);
	    // if( min(ia2(R2,axis2))<I2.getBase() )

#ifdef USE_OLD_APP
            if( numberOfDimensions>1 )
	      ia2(R2,axis2)+=I2.getBase(); // **** A++ bug ****
#endif
	    // append ia2 to ia
	    int numberOfTaggedCells=ia.getLength(0);
	    int newNumberOfTaggedCells=ia2.getLength(0);
	  
	    Range R = numberOfTaggedCells+newNumberOfTaggedCells;
	  
	    ia.resize(R,Rx); // maybe should allocate more to begin with ??
	    Range R0(numberOfTaggedCells,numberOfTaggedCells+newNumberOfTaggedCells-1);
	  
	    ia(R0,Rx)=ia2;

            const realArray & vertex = mg.vertex();
            Range Rxa=numberOfDimensions+1;
            xa.resize(R,Rxa);           
            int axis;
            int i3=mg.gridIndexRange(Start,axis3);
	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
	      if( numberOfDimensions==2 )
		xa(R0,axis)=vertex(ia2(R2,0),ia2(R2,1),i3,axis);
	      else if( numberOfDimensions==3 )
		xa(R0,axis)=vertex(ia2(R2,0),ia2(R2,1),ia2(R2,2),axis);
	      else 
		xa(R0,axis)=vertex(ia2(R2,0),mg.gridIndexRange(0,1),i3,axis);
	    }
            // save the cell area
            if( numberOfDimensions==2 )
	    {
	      xa(R,numberOfDimensions)=abs(
		(vertex(ia2(R2,0)+1,ia2(R2,1)  ,i3,axis1)-vertex(ia2(R2,0),ia2(R2,1),i3,axis1))*
		(vertex(ia2(R2,0)  ,ia2(R2,1)+1,i3,axis2)-vertex(ia2(R2,0),ia2(R2,1),i3,axis2)) );
	    }
	    else if( numberOfDimensions==3 )
	    {
	      Overture::abort("error");
	    }
	    else
	    {
	      xa(R,numberOfDimensions)=abs(
		(vertex(ia2(R2,0)+1,ia2(R2,1)  ,i3,axis1)-vertex(ia2(R2,0),ia2(R2,1),i3,axis1)) );
	    }
	    
	    
	  }
	
	}
      }
      // display(ia,"ia","%3i");

      if( ia.getLength(0) == 0 )
	continue;  // there are no tagged points on this base grid.

      // BoxList boxList(centering);  // here is the list that will hold the boxes for this level

      ListOfRotatedBox & boxList = refinementBoxList[level-baseLevel-1];

      if( debug & 2)
	printf("Number of tagged cells = %i \n",ia.getLength(0));
      Range R=ia.getLength(0);
    
      RotatedBox & mainBox = * new RotatedBox(numberOfDimensions);  // *** need to delete if appropriate
      
      splitBoxRotated( mainBox, boxList, xa, level );

      if( mergeBoxes )
        merge( boxList );


    }  // end for level
  
    // first delete any existing refinement grids.
    // for( level=baseLevel+1; level<=min(refinementLevel,gc.numberOfRefinementLevels()-1); level++ )
    for( level=min(refinementLevel,gc.numberOfRefinementLevels()-1); level>=baseLevel+1; level-- )
    {
      GridCollection & rl = gc.refinementLevel[level];
      for( int gr=rl.numberOfComponentGrids()-1; gr>=0; gr-- )
      {
	if( rl.baseGridNumber(gr)==baseGrid )
	{
	  grid=rl.gridNumber(gr);
          printF("delete grid %i from level %i\n",grid,level);
	  
          gc.deleteRefinement(grid);
	}
      }
    }




    Range all;
    // now add the grids into the grid collection.
    for( level=baseLevel+1; level<=refinementLevel; level++ )
    {    
      ListOfRotatedBox & boxList =refinementBoxList[level-baseLevel-1];
      printF("****** number of rotated boxes = % i ***** \n",boxList.getLength());
      int numberOfRotatedBoxes=boxList.getLength();

      if( false && gridAdditionOption==addGridsAsRefinementGrids )
      {
/* -------
	// add new grids as refinements.

	IntegerArray range(2,3), factor(3);
	range=0;
	factor=refinementRatio;
	for( int b=0; b<numberOfRotatedBoxes; b++ )
	{
	  int axis;
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    const realArray & box = boxList(all,b);
	    range(0,axis)=box(0);  // this is wrong
	    range(1,axis)=box(1);
	
	  }
          printF("add grid to level %i,  [%i,%i]x[%i,%i]\n",level,range(0,0),range(1,0),range(0,1),range(1,1));

	  if( min(range(1,Rx)-range(0,Rx))>=0  )
	    gc.addRefinement(range, factor, level, baseGrid); 
	  else
	  {
	    printF("Regrid::regrid:ERROR: an invalid box was made! [%i,%i]x[%i,%i]\n",
		   range(0,0),range(1,0),range(0,1),range(1,1));
	  }
	}
------- */
      }
      else
      {
 	// add new grids directly as new overlapping grids.
 	MappedGrid & mg = gcNew[baseGrid];

 	const IntegerArray & gid = mg.gridIndexRange();
        Range all;
 	int g=0; // counts new grids added.
        
	for( int b=0; b<numberOfRotatedBoxes; b++ )
	{
          RotatedBox & box = boxList[b];

          real scale1 = 1.; 
          real scale2 = 1.;

          real xc=box.centre[0]*scale1, yc=box.centre[1]*scale2;  // centre of the box

          real dx = max( .05, box.halfAxesLength[0] );
	  real dy = max( .05, box.halfAxesLength[1] );

          real xa=box.centre[0]-dx;
          real xb=box.centre[0]+dx;
 
          real ya=box.centre[1]-dy;
          real yb=box.centre[1]+dy;
  
          Mapping & map = * new SquareMapping(xa,xb,ya,yb);
          map.incrementReferenceCount();
	  
       

          MatrixTransform & rotate = *new MatrixTransform(map);
          rotate.incrementReferenceCount();
	  
          int numberOfGridPoints=21;
          real aspectRatio=SQRT(dx/dy);
          rotate.setGridDimensions(axis1,max(4,int(numberOfGridPoints*aspectRatio+.5)));
          rotate.setGridDimensions(axis2,max(4,int(numberOfGridPoints/aspectRatio+.5)));
	    

          real angle = atan2((double)box.axisVector[1][0],(double)box.axisVector[0][0]);
          printF(" Add grid %i: (%8.2e,%8.2e)x(%8.2e,%8.2e) centre=(%8.2e,%8.2e) angle=%e degrees\n",
               g,xa,xb,ya,yb,xc,yc,angle*180./Pi);
	  
          rotate.shift( -xc,-yc );
	  rotate.rotate( axis3,angle );
          rotate.shift( xc,yc );

	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
            rotate.setBoundaryCondition(Start,axis,0);
            rotate.setBoundaryCondition(End  ,axis,0);
	  }

	  gcNew.add( rotate);    // Add a new component grid, built from this Mapping.

	  map.decrementReferenceCount();
	  rotate.decrementReferenceCount();
	  g++;

	}
    
      }
    }

    delete [] refinementBoxList;

  } // end for base grid

  if( gridAdditionOption==addGridsAsRefinementGrids )
    gcNew.update(GridCollection::THErefinementLevel);
  

  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{displayParameters}} 
int Regrid:: 
displayParameters(FILE *file /* = stdout */ ) const
// ===========================================================================
// /Description:
//   Display parameters.
// 
// /file (input) : display to this file.
//
//\end{RegridInclude.tex} 
// ==========================================================================
{
  fprintf(file,
	  "Regrid:: parameters:\n"
          "  default number of refinement levels=%i\n"
          "  grid efficiency= %f  (in the range (0,1))\n"
          "  refinement ratio = %i \n"
          "  number of buffer zones = %i \n",
	  defaultNumberOfRefinementLevels,efficiency,refinementRatio,numberOfBufferZones);

  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{get}}
int Regrid::
get( const GenericDataBase & dir, const aString & name)
// ===========================================================================
// /Description:
//   Get from a data base file.
//\end{RegridInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Regrid");

  aString className;
  subDir.get( className,"className" ); 
  if( className != "Regrid" )
  {
    cout << "Regrid::get ERROR in className!" << endl;
  }

  subDir.get(defaultNumberOfRefinementLevels,"defaultNumberOfRefinementLevels");
  subDir.get(efficiency,"efficiency");
  subDir.get(refinementRatio,"refinementRatio");
  subDir.get(numberOfBufferZones,"numberOfBufferZones");
  subDir.get(widthOfProperNesting,"widthOfProperNesting");
  subDir.get(maximumNumberOfSplits,"maximumNumberOfSplits");
  
  subDir.get(minimumBoxSize,"minimumBoxSize");

  subDir.get(useSmartBisection,"useSmartBisection");
  subDir.get(mergeBoxes,"mergeBoxes");
  int temp;
  subDir.get(temp,"gridAdditionOption");  gridAdditionOption=(GridAdditionOption)temp;
  subDir.get(temp,"gridAlgorithmOption"); gridAlgorithmOption=(GridAlgorithmOption)temp;

  delete &subDir;
  return 0;
}

//\begin{>>RegridInclude.tex}{\subsection{put}}
int Regrid::
put( GenericDataBase & dir, const aString & name) const
// ===========================================================================
// /Description:
//   Put to a data base file.
//\end{RegridInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Regrid");                   // create a sub-directory 

  subDir.put( "Regrid","className" );

  subDir.put(defaultNumberOfRefinementLevels,"defaultNumberOfRefinementLevels");
  subDir.put(efficiency,"efficiency");
  subDir.put(refinementRatio,"refinementRatio");
  subDir.put(numberOfBufferZones,"numberOfBufferZones");
  subDir.put(widthOfProperNesting,"widthOfProperNesting");
  subDir.put(maximumNumberOfSplits,"maximumNumberOfSplits");
  
  subDir.put(minimumBoxSize,"minimumBoxSize");

  subDir.put(useSmartBisection,"useSmartBisection");
  subDir.put(mergeBoxes,"mergeBoxes");

  subDir.put((int)gridAdditionOption,"gridAdditionOption");
  subDir.put((int)gridAlgorithmOption,"gridAlgorithmOption");

  delete &subDir;
  return 0;
}




//\begin{>>RegridInclude.tex}{\subsection{update}} 
int Regrid::
update( GenericGraphicsInterface & gi )
// ===========================================================================
// /Description:
//   Change parameters interactively.
// 
// /gi (input) : 
// /par (input) : 
//
//\end{RegridInclude.tex} 
// ==========================================================================
{
  aString menu[]=
  {
    "!Regrid parameters",
    "display parameters",
    "default number of refinement levels",
    "refinement ratio",
    "grid efficiency",
    "number of buffer zones",
    "width of proper nesting",
    "index coarsening factor",
    "minimum box width",
    "minimum box size",  
    "turn on load balancer",
    "turn off load balancer",
    "change load balancer",
    "exit",
    ""
  };

//\begin{>>RegridInclude.tex}{}
//\no function header:
//
// \begin{description} \index{show file!options}
//  \item[number of refinement levels] : 
//  \item[grid efficiency] : a number between 0 and 1, normally about .7
//  \item[number of buffer zones] :
//  \item[number of refinement levels] : 
// \end{description}
//\end{RegridInclude.tex}

  aString answer,answer2;
  char buff[100];

  gi.appendToTheDefaultPrompt("Regrid>");  
  for(;;)
  {
    gi.getMenuItem(menu,answer,"choose a menu item");
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="display parameters" )
    {
      displayParameters();
    }
    else if( answer=="default number of refinement levels" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the number of refinement levels (current=%i)",
                             defaultNumberOfRefinementLevels));
      sScanF(answer,"%i",&defaultNumberOfRefinementLevels);
      printF("set defaultNumberOfRefinementLevels=%i\n",defaultNumberOfRefinementLevels);
      
    }
    else if( answer=="grid efficiency" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the grid efficiency 0< eff < 1 (current=%8.2e)",efficiency));
      sScanF(answer,"%e",&efficiency);
      printF("set efficiency=%e\n",efficiency);

    }
    else if( answer=="refinement ratio" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the refinement ratio (current=%i)",refinementRatio));
      sScanF(answer,"%i",&refinementRatio);

      printF("set refinementRatio=%i\n",refinementRatio);

//       regrid.setRefinementRatio(refinementRatio);
//       IntegerArray ratio(3);
//       ratio=refinementRatio;
//       interp.setRefinementRatio( ratio );
    }
    else if( answer=="number of buffer zones" )
    {
      printF("INFO:number of buffer zones: increase tagged cells by this many zones.\n");
      gi.inputString(answer,sPrintF(buff,"Enter the number of buffer zones (current=%i)",numberOfBufferZones));
      sScanF(answer,"%i",&numberOfBufferZones);
      printF("set numberOfBufferZones=%i\n",numberOfBufferZones);
      
      setNumberOfBufferZones(numberOfBufferZones); 
    }
    else if( answer=="width of proper nesting" )
    {
      printF("INFO:the width of proper nesting is the number of cells between grids at level l with those at l-1\n");
      gi.inputString(answer,sPrintF(buff,"Enter the width of proper nesting (current=%i)",widthOfProperNesting));
      sScanF(answer,"%i",&widthOfProperNesting);
      printF("set width of proper nesting=%i\n",widthOfProperNesting);
      
      setWidthOfProperNesting(widthOfProperNesting); // distance between levels
    }
    else if( answer=="minimum box size" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the minimum number of points in a refinement grid (current=%i)",
            minimumBoxSize));
      sScanF(answer,"%i",&minimumBoxSize);
      printF("set minimumBoxSize\n",minimumBoxSize);
      
    }
    else if( answer=="minimum box width" )
    {
      printF("The minimum box width is the minimum width for a refinement grid \n"
             " measured in the coarse grid index space. The actual minimum width will be minWidth*ratio\n"
             " This option will only really work if all refinement levels are regenerated at the same time\n");
      gi.inputString(answer,sPrintF(buff,"Enter the minimum width of a a refinement grid (current=%i)",
            minimumBoxWidth));
      sScanF(answer,"%i",&minimumBoxWidth);
      printF("set minimumBoxWidth\n",minimumBoxWidth);
      
    }
    else if( answer=="index coarsening factor" )
    {
      printF("INFO: index coarsening factor: Build amr grids on an index space that is coarsened by this amount.\n"
             " This will increase the size of the smallest possible refinement grid. \n"
             " The smallest possible grid will have a width of (index coarsening factor)*refinementRatio cells.\n");
      int factor=indexCoarseningFactor;
      
      gi.inputString(answer,sPrintF(buff,"Enter the index coarsening factor (current=%i)",factor));
      sScanF(answer,"%i",&factor);
      printF("set index coarsening factor=%i\n",factor);
      
      setIndexCoarseningFactor(factor);
    }
    else if( answer=="turn on load balancer" ||
             answer=="turn off load balancer" )
    {
      loadBalance = answer=="turn on load balancer" ? true : false;
    }
    else if( answer=="change load balancer" )
    {
      // change load balancer options
      loadBalancer.update(gi);
    }
    else
    {
      cout << "Unknown response: [" << answer2 << "]\n";
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();



  return 0;
}




