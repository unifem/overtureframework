#include "InterpolateRefinements.h"
#include "Box.H"
#include "display.h"
#include "ParentChildSiblingInfo.h"
#include "ParallelUtility.h"
#include "Regrid.h"
#include "App.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)

// void printInfo( GridCollection & cg, int option=0 );

FILE* InterpolateRefinements::debugFile=NULL;


//\begin{>InterpolateRefinementsInclude.tex}{\subsection{Constructor}} 
InterpolateRefinements::
InterpolateRefinements(int numberOfDimensions_)
  : interpParams(numberOfDimensions_)
//=========================================================================================
// /Description:
//     Use this class to perform various interpolation operations on adaptively refined grids.
//
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  debug=0;

//    debug=3;
//    openDebugFile();
  

  myid=max(0,Communication_Manager::My_Process_Number);


  numberOfDimensions=numberOfDimensions_;
  interpParams.setInterpolateOrder(2);  // **************************
  
  interp.initialize(interpParams);

  refinementRatio.redim(3);
  refinementRatio=2;
  
  numberOfGhostLines=1;  // number of ghostlines that are used.
  
  timeForCoarseFromFine=0.;
  timeForRefinementBoundaries=0.;
  timeForRefinements=0.;
  timeForBoundaryCoarseFromFine=0.;
  
}

InterpolateRefinements::
~InterpolateRefinements()
{
  //  if( debugFile!=NULL ) fflush(debugFile);  // we cannot close the file since other instances may be using it
}

void InterpolateRefinements::
openDebugFile()
// ==================================================================================
// /Description:
//    Protected routine for opening the debug file if debug>0 -- we only want to
// open the file if we are really in debug mode.
// ==================================================================================
{
  Overture::openDebugFile();
  assert( Overture::debugFile!=NULL );
  debugFile=Overture::debugFile;
//    if( debug>0 && debugFile==NULL )
//    {
//      if( myid==0 )
//        debugFile = fopen("interpolateRefinements.debug","w" ); 
//      else
//        debugFile = fopen(sPrintF("interpolateRefinements%i.debug",myid),"w" ); 
//    }
  
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{setOrderOfInterpolation}} 
int InterpolateRefinements::
setOrderOfInterpolation( int order )
//=========================================================================================
// /Description:
//    Set the order of interpolation. The order is equal to the widht of the interpolation stencil.
//  For exmaple, order=2 will use linear interpolation, is second order, and is exact for linear
//  polynomials.
// /order (input) : the order of interpolation.
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  interpParams.setInterpolateOrder(order);  
  interp.initialize(interpParams);
  return 0;
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{setNumberOfGhostLines}} 
int InterpolateRefinements::
setNumberOfGhostLines( int number )
//=========================================================================================
// /Description:
//    Set the number of ghsot lines that are used.
// /number (input) : the number of ghsot lines 
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  numberOfGhostLines=number;
  return 0;
}

// *****************************************************************************************************
// **** Here are some generic utility routines -- these could be put somewhere else ---

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{intersects}} 
Box InterpolateRefinements::
intersects( const Box & box1, const Box & box2 )
//=========================================================================================
// /Description:
//    Protected routine for intersecting two boxes.
//
// /box1, box2 (input) : intersect these boxes.
// /return value: box defining the region of intersection.
//
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  Box box; box.convert(box1.ixType());
  for( int axis=0; axis<3; axis++ )
  {
    box.setSmall(axis,max(box1.smallEnd(axis),box2.smallEnd(axis)));
    box.setBig  (axis,min(box1.bigEnd(axis),box2.bigEnd(axis)));
  }
  return box;
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{getIndex}} 
int InterpolateRefinements::
getIndex( const BOX & box, Index Iv[3] )
//=========================================================================================
// /Description:
//    Convert a box to an array of Index's.
// /box (input):
// /Iv (output): 
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  for( int axis=0; axis<3; axis++ )
    Iv[axis]=Range(box.smallEnd(axis),box.bigEnd(axis));

  return 0;
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{getIndex}} 
int InterpolateRefinements::
getIndex( const BOX & box, int side , int axis, Index Iv[3])
//=========================================================================================
// /Description:
//    Convert a box to an array of Index's. Use this version when the box
// was created with the intersection routine -- we need to remove some of
// the intersection points
// /box (input):
// /Iv (output): 
// /side (input) : 
// /return 0 if the Index's define a positive number of points, return 1 otherwise
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  int returnValue=0;
  for( int dir=0; dir<3; dir++ )
  {
    if( dir==axis )
    {
      Iv[dir]=Range(box.smallEnd(dir)+side,box.bigEnd(dir)-1+side);
      if( Iv[dir].getBase() > Iv[dir].getBound() )
      {
	returnValue=1;
	break;
      }
    }
    else
      Iv[dir]=Range(box.smallEnd(dir),box.bigEnd(dir));

    
  }
  
  return returnValue;
}


//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{buildBox}} 
Box InterpolateRefinements::
buildBox(Index Iv[3] )
//=========================================================================================
// /Description:
//    Build a box from 3 Index objects.
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  Box box;
  box.convert(IndexType(D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE)));
  for( int axis=0; axis<3; axis++ )
  {
    box.setSmall(axis,Iv[axis].getBase());
    box.setBig(axis,Iv[axis].getBound());
  }
  return box;
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{buildBaseBox}} 
Box InterpolateRefinements::
buildBaseBox( MappedGrid & mg )
// ================================================================================================
// /Access: protected.
// /Description:
//   Build a box from a MappedGrid on level=0. 
//
// We expand the box on the base level to include ghost points on interpolation boundaries,
// since we need to allow refinement patches to extend into the interpolation region.
//
//\end{InterpolateRefinementsInclude.tex} 
// ===============================================================================================
{
  Box box = mg.box();      // we could keep a list for below
  // we expand the box on the base level to include ghost points on interpolation boundaries,
  // since we need to allow refinement patches to extend into the interpolation region.
  const IntegerArray & extendedIndexRange = mg.extendedIndexRange();
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    box.setSmall(axis,extendedIndexRange(0,axis)); 
    box.setBig  (axis,extendedIndexRange(1,axis)); 
  }
  return box;
}



//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{buildBox}} 
Box InterpolateRefinements::
buildBox(realArray & u, Index Iv[3], int processor )
//=========================================================================================
// /Description:
//    Build a box for the portion of the array u(Iv) that lives on a given processor
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  Box box;
  box.convert(IndexType(D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE)));
  const int numberOfProcessors=Communication_Manager::Number_Of_Processors;
  const int numberOAxesPartitioned = u.getLength(2)>0 ? 3 : 2;   // ***************** fix this *************
  
  for( int axis=0; axis<3; axis++ )
  {
    int na = Iv[axis].getBase();
    int nb = Iv[axis].getBound();
    
    int numberOfDivisionsAlongAxis = numberOfProcessors/numberOAxesPartitioned;
    
    int ua = u.getBase(axis) + u.getLength(axis)/numberOfDivisionsAlongAxis;
    int ub = ua;
    
    box.setSmall(axis,na);
    box.setBig(axis,nb);
  }
  return box;
}

// *****************************************************************************************************



bool InterpolateRefinements::
boxWasAdjustedInPeriodicDirection( BOX & box, GridCollection & gc, int baseGrid, int level,
            int & periodicDirection, int & periodicShift )
// ===================================================================================================
// /Access: protected.
// /Description:
//   This routine will adjust a box that crosses a branch cut of a periodic grid.
//
// /box (input/output) : adjust this box.
// /baseGrid : this is the base grid for which we check the periodicity.
// /level : refinement level for box.
// /periodicDirection (output) : 
// /periodShift (output) :
//
// /Return value:
//    Return true if the box was adjusted.   
// ==================================================================================================
{
  int returnValue=0;
  periodicDirection=-1;
  if( true || level==1 )  // *wdh* allow for periodic boxes on all levels
  {
    // *whd* 000809 const IntegerArray & gid0 = gc[baseGrid].gridIndexRange();

    IntegerArray gid(2,3);
    gid=gc[baseGrid].gridIndexRange();

    gid *= int( pow( refinementRatio(0), level ) ); // *** assumes equal refinement ratios
    int axis;
    for( axis=0; axis<gc.numberOfDimensions(); axis++ )
    {
      assert( refinementRatio(axis)==refinementRatio(0) );
      
      if( gc[baseGrid].isPeriodic(axis) &&
          ( box.smallEnd(axis) <= gid(Start,axis) || box.bigEnd(axis) >= gid(End,axis) ) )
        //  ( box.smallEnd(axis) < gid(Start,axis) || box.bigEnd(axis) > gid(End,axis) ) )
      {
	periodicDirection=axis;
	break;
      }
    }
    if( periodicDirection>=0 )
    {
      returnValue=1; // the box was adjusted.

      // shift the box by the period so we interpolate the part that is outside the fundamental period.
      periodicShift = (gid(End,periodicDirection)-gid(Start,periodicDirection));
//       periodicShift = (gid(End,periodicDirection)-gid(Start,periodicDirection))*
//                                refinementRatio(periodicDirection);
//       if( level>1 )
//         periodicShift *= pow( refinementRatio(periodicDirection), level-1 );

      // if( box.smallEnd(periodicDirection) > gid(Start,periodicDirection) )
      //  periodicShift=-periodicShift;

      if( box.smallEnd(axis) == gid(Start,axis) )
      {
	// bottom edge of box hits the branch cut --> shift in the positive direction to overlap
	// any grids that hit the other end.
      }
      else if( box.bigEnd(axis) == gid(End,axis) )
      {
	// top edge hits the branch cut, shift backwards
	periodicShift=-periodicShift;
      }
      else if( box.smallEnd(periodicDirection) >= gid(Start,periodicDirection) )
	periodicShift=-periodicShift;
      
      box.setSmall(periodicDirection,box.smallEnd(periodicDirection)+periodicShift);
      box.setBig(  periodicDirection,box.bigEnd  (periodicDirection)+periodicShift);
    }
  }
  return returnValue;
}


//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{}} 
int InterpolateRefinements::
interpolateRefinements( const realGridCollectionFunction & uOld, 
                        realGridCollectionFunction & u,
                        int baseLevel /* = 1 */ )
// ======================================================================================================
// /Description:
//    Interpolate values from the solution on one refined grid to the solution on a second
//  refined grid.
//
// /uOld (input): source values
// /u (output) : target 
// /baseLevel (input) : interpolate values for levels greater than or equal to baseLevel.
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  real time0=getCPU();

  // debug=3;  // *****************
  Overture::checkMemoryUsage("InterpolateRefinements::interpolateRefinements (start)");  

  assert( baseLevel>=1 );

  GridCollection & gcOld = *uOld.getGridCollection();
  GridCollection & gc = *u.getGridCollection();
  // const int numberOfDimensions=gc.numberOfDimensions();
  assert( numberOfDimensions==gc.numberOfDimensions() );
  
  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  assert( gc.numberOfBaseGrids() == gcOld.numberOfBaseGrids() );
  
  for( int baseGrid=0; baseGrid<gc.numberOfBaseGrids(); baseGrid++ ) 
  {
    const int bg=gc.baseGridNumber(baseGrid);
    assert( bg == gcOld.baseGridNumber(baseGrid) );
    const int nd=4; // use, Iv[] : null Index means copy all values.
    const bool sameParallelDistribution = hasSameDistribution(u[bg],uOld[bg]);
    if( sameParallelDistribution )
      assign(u[bg],uOld[bg]);
    else
      ParallelUtility::copy(u[bg],Iv,uOld[bg],Iv,nd);

    // if arrays have the same number of grid points and same partition -- just copy serial arrays
    // *wdh* 061115 u[gcOld.baseGridNumber(baseGrid)]=uOld[gc.baseGridNumber(baseGrid)];
  }
  Overture::checkMemoryUsage("InterpolateRefinements::interpolateRefinements (2)");  
  if( debug & 4 )
  {
    real mem=Overture::getCurrentMemoryUsage();
    printf("InterpolateRefinements::interpolateRefinements (2) myid=%i memory usage=%g\n",myid,mem);
  }
  
  // **** now try to interpolate all refinement grids *****

  // we assume the refinement ratio is the same for all grids
  if( gc.numberOfRefinementLevels()>1 )
    refinementRatio=gc.refinementLevel[1].refinementFactor(Range(0,2),0);
  else if( gcOld.numberOfRefinementLevels()>1 )
    refinementRatio=gcOld.refinementLevel[1].refinementFactor(Range(0,2),0);
  else
    refinementRatio=2;  // doesn't matter in this case.

  IntegerArray ratio(3);

  int axis,level;
  for( level=baseLevel; level<gc.numberOfRefinementLevels(); level++ )
  {
    GridCollection & rl = gc.refinementLevel[level];
    for( int g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      const int grid=rl.gridNumber(g);
      const int baseGrid=rl.baseGridNumber(g);
      
     
      realArray & u0 = u[grid];  // here is the grid function we want to interpolate
      #ifdef USE_PPP
        realSerialArray u0Local;  getLocalArrayWithGhostBoundaries(u0,u0Local);
      #else
        realSerialArray & u0Local = u0;
      #endif

      // const BOX & box = rl[g].box();
      BOX box = rl[g].box();  // ** here is the box for the grid we want to interpolate
      getIndex(box,Iv);
      const int numberOfPointsToInterpolate= I1.getLength()*I2.getLength()*I3.getLength();
      int numberInterpolated=0;
      bool foundAllPoints=false;

//       if( baseGrid==0 && grid==67 )
//       {
// 	debug=3;
// 	interp.debug=true;
	
// 	printf("IR: interp pts on grid=%i (baseGrid=%i) level=%i, box=[%i,%i][%i,%i]\n",grid,baseGrid,level,
// 	       box.smallEnd(0),box.bigEnd(0),box.smallEnd(1),box.bigEnd(1));
//       }
//       else
//       {
//         debug=0;
// 	interp.debug=false;
//       }

      // int periodicDirection=0,periodicShift=0;
      
	// try to interpolate from uOld
      // *** getIndex(box,Iv);
      // keep track of which points were interpolated:
      intSerialArray mask;
      mask.redim(u0Local.dimension(0),u0Local.dimension(1),u0Local.dimension(2));
      mask=0;
      
      #ifdef USE_PPP
        Index I1a=I1, I2a=I2, I3a=I3;
        bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,I1a,I2a,I3a);
      #else
	Index &I1a=I1, &I2a=I2, &I3a=I3;
        bool ok=true;
      #endif

      // Start at the highest level and keep a mask of which points were interpolated..
      const int levelStart=min(level,gcOld.numberOfRefinementLevels()-1);
      for( int l=levelStart; l>=0; l-- )
      {
	GridCollection & rlOld = gcOld.refinementLevel[l];
	    
	for( int gOld=0; gOld<rlOld.numberOfComponentGrids(); gOld++ )
	{
	  Box box0 = rlOld[gOld].box();
	  // we expand the box to include ghost points on interpolation boundaries,
	  // since we need to allow refinement patches to extend into the interpolation region.

	    // *wdh* 000806 const IntegerArray & extended = rlOld[gOld].extendedIndexRange();

	      // also include periodic edges
	  const IntegerArray & extended = extendedGridIndexRange(rlOld[gOld]);
	  for( axis=0; axis<gc.numberOfDimensions(); axis++ )
	  {
	    if( l==0 || rlOld[gOld].isPeriodic(axis) )
	    {
	      box0.setSmall(axis,extended(0,axis)); 
	      box0.setBig(axis,extended(1,axis));
	    }
	  }

	  axis=0;
	  ratio = rl.refinementFactor(axis,g)/rlOld.refinementFactor(axis,gOld);

	  // ratio = pow(refinementRatio(0),level-l);  // ***
	  if( ratio(0)!=1 )
	    box0.refine(ratio(0));

	  
	  if( rlOld.baseGridNumber(gOld)==baseGrid && box.intersects(box0) )
	  {
	    BOX box2 = intersects(box,box0);

	    for( axis=0; axis<numberOfDimensions; axis++ )
	      Jv[axis]=Range(box2.smallEnd(axis),box2.bigEnd(axis));
	    for( axis=numberOfDimensions; axis<3; axis++ )
	      Jv[axis]=rlOld[gOld].dimension(Start,axis);

	    int gridOld=rlOld.gridNumber(gOld);

	    if( debug & 2 )
	      printf("interpolate new grid %i (level=%i) (Jv=[%i,%i]x[%i,%i]) from old grid %i "
		     "(level=%i,ratio=%i) \n",grid,level,
		     Jv[0].getBase(),Jv[0].getBound(),Jv[1].getBase(),Jv[1].getBound(),gridOld,l,ratio(0));

            if( l==levelStart )
	    {
	      // interp.interpolateCoarseToFine (u0,Jv,uOld[gridOld],ratio);

              interp.interpolateFineFromCoarse(u0,Jv,uOld[gridOld],ratio);
	    }
	    else
	    {
              // avoid over-writing better values from a finer grid:

                // copy points where mask==0 
	      interp.interpolateFineFromCoarse(u0,mask,Jv,uOld[gridOld],ratio,
					       0,Interpolate::useDefaultTransferWidth,Interpolate::maskEqualZero);
	    }
	    
	    if( false && debug & 4  )
	    {
	      if( grid==6 )
		display(u0(J1,J2,J3),"u0(J1,J2,J3) after interpolateCoarseToFine","%9.2e");
	      if( gridOld==2 )
	      {
		display(uOld[gridOld],"uOld","%9.2e");
		displayMask(rlOld[gOld].mask(),"mask on uOld");
	      }
	    }
		  
	    Index J1a=Jv[0], J2a=Jv[1], J3a=Jv[2];
	    bool okj =ParallelUtility::getLocalArrayBounds(u0,u0Local,J1a,J2a,J3a);
            if( okj )
  	      mask(J1a,J2a,J3a)=1;

	    if( false && debug & 2 )
	    {
	      printF("box2==box -> %i, l=%i level=%i levelStart=%i \n",(int) (box2 == box),l,level,levelStart );
              // ::display(uOld[gridOld],"source grid: uOld[gridOld]","%5.2f ");
              ::display(u0,"u0 after interp","%5.2f ");
              // ::display(mask," Here is the mask","%2i ");
	    }
	    

	    if( box2 == box && l==level )
	    {
	      foundAllPoints=true;
	      break;
	    }

	  }
	}
      } // end for l
         
      int num = 0;
      if( ok ) num = sum(mask(I1a,I2a,I3a));
      numberInterpolated =ParallelUtility::getSum(num); 
      
      foundAllPoints = numberInterpolated >= numberOfPointsToInterpolate;
      if( !foundAllPoints )
      {
	// not all points were interpolated, go back and try again using all levels
	    
	if( true || debug & 2 )
	{
	  if( myid==0 )
	  {
	    printf(" numberInterpolated=%i, numberOfPointsToInterpolate=%i \n",numberInterpolated,
		   numberOfPointsToInterpolate);
	    cout << "box : " << box << endl;

	    printf("interpolateRefinements::INFO: grid %i cannot interp all points. \n"
		   "    baseLevel=%i (first level to interp), numberOfRefinementLevels=%i\n"
		   ,grid,baseLevel,gc.numberOfRefinementLevels());
	    
	  }
	  ::display(mask(I1a,I2a,I3a),"mask(I1a,I2a,I3a): 1=interpolated, 0=not","%2i ");
	  

	}
      }

      if( !foundAllPoints )
      {
	if( myid==0 )
	{
	  printf("interpolateRefinements::ERROR: unable to interp all pts on grid %i, baseGrid=%i \n",
		 grid,baseGrid);

	  // cout << "Here is the box we are trying to interpolate: " << box << endl;
	  // printf("*** Here is gcOld (from which we try to interpolate)\n");
	  // printInfo(gcOld);

	  printf("Saving files with amr info: amrDebugOld.cmd (gcOld) and amrDebug.cmd (gc)\n");
	  Regrid::outputRefinementInfo(gcOld,"junk.hdf","amrDebugOld.cmd"); 
	  Regrid::outputRefinementInfo(gc   ,"junk.hdf","amrDebug.cmd"); 
	  
	}
	
	Overture::abort("error");
      }

    } // end for g
  } // end for level

  Overture::checkMemoryUsage("InterpolateRefinements::interpolateRefinements (before interRefineBndry)");  
  if( debug & 4 )
  {
    fflush(0);
    Communication_Manager::Sync();
    real mem=Overture::getCurrentMemoryUsage();
    printf("InterpolateRefinements::interpolateRefinements (before interRefineBndry) myid=%i memory usage=%g\n",myid,mem);
  }
  
  // we could avoid this next interpolation if we interpolated ghost lines above -- however
  // that requires the Interpolate class not to use a full stencil when it doesn't need to.
  if( true )
  {
    // interpolateCoarseFromFine(u); // is this needed?
    
    interpolateRefinementBoundaries(u);
//    interpolateCoarseFromFine(u);

  }
  

  if( debug & 4 )
  {
    fflush(0);
    Communication_Manager::Sync();
    real mem=Overture::getCurrentMemoryUsage();
    printf("InterpolateRefinements::interpolateRefinements (after interRefineBndry) myid=%i memory usage=%g\n",myid,mem);
  }
  
  timeForRefinements+=getCPU()-time0;
  Overture::checkMemoryUsage("InterpolateRefinements::interpolateRefinements (end)");  
  return 0;
}



//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{interpolateRefinementBoundaries}} 
int InterpolateRefinements::
interpolateRefinementBoundaries( realGridCollectionFunction & u,
                                 int levelToInterpolate /* = allLevels */,
                                 const Range & C0 /* = nullRange */ )
// ======================================================================================================
// /Description:
//    Interpolate the ghost values on refinement grids.
//
// /Note: This function assumes that grids are properly nested.
// 
// /levelToInterpolate: interpolate just this level (Note: nothing to do on level 0)
// /C0 (input) : optionally specify which components to interpolate.
//
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  real time0=getCPU();

  if( levelToInterpolate==0 )
    return 0;
  
  GridCollection & gc = *u.getGridCollection();
  if( gc.numberOfRefinementLevels()<=1 )
    return 0;

  if( false )
  {
    // use the PCS info
    gc.updateParentChildSiblingInfo(); // this will only update if the info is out of date

    int returnValue= interpolateRefinementBoundaries(  *gc.getParentChildSiblingInfo(),u,levelToInterpolate,C0);
    timeForRefinementBoundaries+=getCPU()-time0;
    return returnValue;
  }
  
  bool useMask = true;

  debug = 0; // 3;  // ***************
  if( debug>0 )
    openDebugFile();

  // const int numberOfDimensions=gc.numberOfDimensions();
  assert( numberOfDimensions==gc.numberOfDimensions() );
  
  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  Range C = C0==nullRange ? Range(u.getComponentBase(0),u.getComponentBound(0)) : C0;

  int levelStart = levelToInterpolate==allLevels ? 1 : levelToInterpolate;
  int levelEnd   = levelToInterpolate==allLevels ? gc.numberOfRefinementLevels()-1 : levelToInterpolate;

  // we assume the refinement ratio is the same for all grids
  refinementRatio=gc.refinementLevel[1].refinementFactor(Range(0,2),0);

  // now try to interpolate all refinement grids
  int level;
  for( level=levelStart; level<=levelEnd; level++ )
  {
    GridCollection & rl = gc.refinementLevel[level];
    for( int g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      // interpolate this grid
      const int grid=rl.gridNumber(g);
      const int baseGrid=rl.baseGridNumber(g);
      MappedGrid & mg=gc[grid];
      const intArray & mask = mg.mask();
      #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      #else
        const intSerialArray & maskLocal=mask;
      #endif

      realArray & u0 = u[grid];
	  
      GridCollection & rlm1 = gc.refinementLevel[level-1];  // coarser level

      int c,g2;
      // for now --first interpolate all ghost boundaries from the coarser level ** fix this **
      int side,axis;
      // if( true ) // ****************************************************
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	for( side=0; side<=1; side++ )
	{
          if( mg.boundaryCondition(side,axis)==0 )
	  {
	    BOX box;
	    box.convert(IndexType(D_DECL(IndexType::NODE,IndexType::NODE,IndexType::NODE)));
            int dir;
	    for( dir=0; dir<3; dir++ )
	    {
              if( dir==axis )
	      {
                if( side==0 )
		{
		  Iv[dir]=Range(mg.indexRange(side,dir)-numberOfGhostLines,
				mg.indexRange(side,dir)-1);
		}
		else
		{
		  Iv[dir]=Range(mg.indexRange(side,dir)+1,
				mg.indexRange(side,dir)+numberOfGhostLines);
		}
		box.setSmall(dir,Iv[dir].getBase()-side);
		box.setBig  (dir,Iv[dir].getBound()+(1-side));
	      }
	      else if( dir<numberOfDimensions )
	      {
                // this will include the corner ghost point
                Iv[dir]=Range(mg.indexRange(Start,dir)-numberOfGhostLines,mg.indexRange(End,dir)+numberOfGhostLines);
		box.setSmall(dir,Iv[dir].getBase());
		box.setBig  (dir,Iv[dir].getBound());
	      }
              else // unused dimensions 
	      {
                Iv[dir]=Range(mg.indexRange(Start,dir),mg.indexRange(End,dir));
		box.setSmall(dir,Iv[dir].getBase());
		box.setBig  (dir,Iv[dir].getBound());
	      }
	    }
	    
            if( debug & 2 )
            fprintf(debugFile,
                   "interpolateRefinementBoundaries: level=%i g=%i grid=%i side=%i axis=%i\n"
                   "interp [%i,%i][%i,%i][%i,%i] "
		   "gID=[%i,%i][%i,%i][%i,%i] id=[%i,%i][%i,%i][%i,%i]\n",level,g,grid,side,axis,
                   I1.getBase(),I1.getBound(),
		   I2.getBase(),I2.getBound(),
		   I3.getBase(),I3.getBound(),
                   mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),
		   mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
		   mg.gridIndexRange(0,2),mg.gridIndexRange(1,2),
                   mg.indexRange(0,0),mg.indexRange(1,0),
		   mg.indexRange(0,1),mg.indexRange(1,1),
		   mg.indexRange(0,2),mg.indexRange(1,2));
	    
	    // If the box crosses a branch cut we try to interpolate it twice, the second time we shift
	    // the box by period so we interpolate the part that was outside the fundamental period.
	    for( int split=0; split<=1; split++ )  // 
	    {
              int periodicDirection=0, periodicShift=0;
	      if( split==1 )
	      {
		if( !boxWasAdjustedInPeriodicDirection( box, gc,baseGrid,level,periodicDirection,periodicShift ) )
		  break;
	      }

	      for( g2=0; g2<rlm1.numberOfComponentGrids(); g2++ ) // check grids on coarser level
	      {
		if( rlm1.baseGridNumber(g2)==baseGrid )
		{
                  // we build an extended box on the base level (includes ghost points)
		  BOX box2=level==1 ? buildBaseBox(rlm1[g2]) : rlm1[g2].box(); 
                  // expand the coarse grid box if periodic
                  for( dir=0; dir<numberOfDimensions; dir++ )
		  {
		    if( rlm1[g2].isPeriodic(dir) )
		    {
		      box2.setSmall(dir,box2.smallEnd(dir)-1);
		      box2.setBig(dir,box2.bigEnd(dir)+1);
		    }
		  }
		  

		  box2.refine(refinementRatio(0));   // refine coarse grid box so we can intersect it with current box
		  if( box2.intersects(box) )
		  {
                    // without Dan's "fix" to Box::intersect we also get intersections of width 1 wide -- when
                    // we remove boundary points we may end up with no intersection. *wdh* 010922

		    BOX intersection = intersects(box2,box);
		    bool ok=getIndex(intersection,side,axis,Iv)==0;
                    if( !ok ) continue;  // no points defined to interpolate.

		    int grid2=rlm1.gridNumber(g2);
		    // realArray u1(I1,I2,I3,C);

		    // interp.interpolateCoarseToFine( u1,Iv,u[grid2],refinementRatio );
                    if( split!=1 )
		    {
		      if( debug & 2 )
		      {
                        fprintf(debugFile,
                               "***interpRefBndry: Before interp from coarse, level=%i, grid=%i grid2=%i "
                               "Iv=[%i,%i][%i,%i][%i,%i]\n",
                                level,grid,grid2,Iv[0].getBase(),Iv[0].getBound(),Iv[1].getBase(),Iv[1].getBound(),
                                Iv[2].getBase(),Iv[2].getBound());
			
			display(u[grid2],"u[grid2] (coarse grid) before interp from coarse",debugFile,"%4.1f");
		      }

                    #ifdef USE_PPP
                      realSerialArray u0Local;  getLocalArrayWithGhostBoundaries(u0,u0Local);
                      if( maskLocal.dimension(0)!=u0Local.dimension(0) || maskLocal.dimension(1)!=u0Local.dimension(1) )
        	      {
        		printf("interpRefineBndrys:ERROR:myid=%i, maskLocal does not match u0Local!! grid=%i, grid2=%i \n",
                                  myid,grid,grid2);
        		printf(" maskLocal: [%i,%i][%i,%i]\n",maskLocal.getBase(0),maskLocal.getBound(0),
                               maskLocal.getBase(1),maskLocal.getBound(1));
        		printf(" u0Local: [%i,%i][%i,%i]\n",u0Local.getBase(0),u0Local.getBound(0),
                               u0Local.getBase(1),u0Local.getBound(1));
        		
                        Overture::abort("error");
        	      }
                    #endif

		      interp.interpolateFineFromCoarse( u0,maskLocal,Iv,u[grid2],refinementRatio );

		      if( debug & 2 )
		      {
			display(u0,"u0 (fine grid) after interp from coarse",debugFile,"%4.1f");
			display(u[grid2],"u[grid2] (coarse grid) after interp from coarse",debugFile,"%4.1f");
		      }
		      
		    }
		    else
		    {
                      // split==1 case -- update points on a periodic boundary
                      assert( split==1 );

                      #ifdef USE_PPP
		        // *** we should just interpolate each serial array separately, do this for now:

		        realArray u1; u1.partition(u0.getPartition());  
                        // *** fix this -- u1 needs to have enough points in each direction to match
                        // the number of ghost points -- otherwise the u1.redim below fails
                        // do this for now:
                        Range Dv[3]; Dv[0]=u0.dimension(0), Dv[1]=u0.dimension(1), Dv[2]=u0.dimension(2);
                        Dv[periodicDirection]+=periodicShift;
                        u1.redim(Dv[0],Dv[1],Dv[2],C); // u1 needs same ghost width
                        
 		        realArray u2;  u2.partition(u0.getPartition());
		        u2.redim(u0.dimension(0),u0.dimension(1),u0.dimension(2),C);
                      #else
 		        realArray u1(I1,I2,I3,C);
                        realArray & u2 = u1;
                      #endif

		      interp.interpolateFineFromCoarse( u1,Iv,u[grid2],refinementRatio );


		      J1=I1; J2=I2; J3=I3;
		      Jv[periodicDirection]-=periodicShift;  // split across a periodic boundary
		      
		      
                      #ifdef USE_PPP
                        
		      // u2(J1,J2,J3,C)=u1(I1,I2,I3,C);   // this involves communication -- use copyArray ?

           		const int numDims=4;
                        Jv[3]=C;
			Iv[3]=C;
		        ParallelUtility::copy( u2, Jv, u1, Iv, numDims);

		        const realSerialArray & u0Local = u0.getLocalArray(); 
		        const realSerialArray & u2Local = u2.getLocalArray(); 
			const intSerialArray & maskLocal = mask.getLocalArray(); 
                        bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,J1,J2,J3,1); 
                        if( !ok ) continue;  // there must be no collective operations after this

		        Index &I1a=J1, &I2a=J2, &I3a=J3;  
                      #else
                        Index &I1a=I1, &I2a=I2, &I3a=I3;
		        realSerialArray & u0Local = u0; 
		        realSerialArray & u2Local = u2; 
			const intSerialArray & maskLocal = mask; 
                      #endif

		      if( debug & 2 )
		      {
			printf("Interp bndry (side,axis)=(%i,%i) of grid %i from coarser grid %i \n"
                               "  .. split==%i, Jv=[%i,%i][%i,%i] from Iv=[%i,%i][%i,%i]\n",
			       side,axis,grid,grid2,split,
                               J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),
                               I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
		     
			// displayMask(mask(J1,J2,J3),"mask(J1,J2,J3)");
		      }
		    
		      // display(u1,"u1");

		      if( useMask )
		      {
			where( maskLocal(J1,J2,J3)>0 )
			  for( c=C.getBase(); c<=C.getBound(); c++ )
			    u0Local(J1,J2,J3,c)=u2Local(I1a,I2a,I3a,c);
		      }
		      else
		      {
			u0Local(J1,J2,J3,C)=u2Local(I1a,I2a,I3a,C);
		      }
		    }
		    
		    if( intersection == box || level==1 ) // we are done in these cases (only 1 parent on level 0)
		      break;
		  }    
		}
	      }
	    }
	  }
	}  // for side
      } // for axis
      
      
      BOX initialBox = rl[g].box();

      // If the box crosses a branch cut we try to interpolate it twice, the scond time we shift
      // the box by period so we interpolate the part that was outside the fundamental period.
      for( int split=0; split<=1; split++ )  // 
      {
        int periodicDirection=0,periodicShift=0;
	if( split==1 )
	{
	  if( !boxWasAdjustedInPeriodicDirection( initialBox, gc,baseGrid,level,periodicDirection,periodicShift ) )
	    break;
	}

	BOX box = initialBox;
	for( axis=0; axis<numberOfDimensions; axis++ )
	  box.grow(axis,numberOfGhostLines);  // grow this box to include all ghost lines to be interpolated

        // try to interpolate the ghost boundaries from the current level
	for( g2=0; g2<rl.numberOfComponentGrids(); g2++ )
	{
	  if( g2!=g && rl.baseGridNumber(g2)==baseGrid && box.intersects( rl[g2].box() ) )
	  {
	    BOX intersection = intersects(box,rl[g2].box());
	    getIndex(intersection,Iv);
	    int grid2=rl.gridNumber(g2);
            // ** we actually copy the boundary points too here -- not necessary ***
	    if( debug & 2 )
	      printf("IRB: Interp bndry of grid %i from same level grid %i I1=(%i,%i) I2=(%i,%i) (useMask=%i)\n",
		     grid,grid2,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),useMask);

	    if( split==0 )

	    {
              if( useMask ) // newer way -- avoids creation of a distributed array
	      {
		// opt version (and P++ version)
#ifdef USE_PPP
		const int numDims=4;
		Jv[0]=I1; Jv[1]=I2; Jv[2]=I3; Jv[3]=C;

		realSerialArray u0Local;  getLocalArrayWithGhostBoundaries(u0,u0Local);
		intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

                Index J1a=J1, J2a=J2, J3a=J3;
		bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,J1a,J2a,J3a,1);

	  	realSerialArray u2;  // holds a copy of pts on u0Local so we can undo changes
                if( ok )
		{
		  u2.redim(J1a,J2a,J3a,C);
                  u2(J1a,J2a,J3a,C)=u0Local(J1a,J2a,J3a,C);
		}

                      
		ParallelUtility::copy( u0, Jv, u[grid2], Jv, numDims);  // u0(Jv)=u[grid2](Jv)

		J1=J1a, J2=J2a, J3=J3a;
		if( !ok ) continue;  // there must be no collective operations after this

#else
		const realSerialArray & u0Local  =  u0;
		const realSerialArray & u2       =  u[grid2];
		const intSerialArray & maskLocal = mask;
		  
		Index &J1 = Iv[0], &J2=Iv[1], &J3=Iv[2];
#endif                   
                   
		real *u0p = u0Local.Array_Descriptor.Array_View_Pointer3;
		const int u0Dim0=u0Local.getRawDataSize(0);
		const int u0Dim1=u0Local.getRawDataSize(1);
		const int u0Dim2=u0Local.getRawDataSize(2);
#undef U0
#define U0(i0,i1,i2,i3) u0p[i0+u0Dim0*(i1+u0Dim1*(i2+u0Dim2*(i3)))]

		real *u2p = u2.Array_Descriptor.Array_View_Pointer3;
		const int u2Dim0=u2.getRawDataSize(0);
		const int u2Dim1=u2.getRawDataSize(1);
		const int u2Dim2=u2.getRawDataSize(2);
#undef U2
#define U2(i0,i1,i2,i3) u2p[i0+u2Dim0*(i1+u2Dim1*(i2+u2Dim2*(i3)))]

		const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
		const int maskDim0=maskLocal.getRawDataSize(0);
		const int maskDim1=maskLocal.getRawDataSize(1);
		const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

		const int cBase=C.getBase(), cBound=C.getBound();
		int i1,i2,i3;
              #ifdef USE_PPP
                // parallel case: restore pts
		FOR_3D(i1,i2,i3,J1,J2,J3)
		{
		  if( MASK(i1,i2,i3)<=0 )
		  {
		    // ** we actually copy the boundary points too here -- not necessary ***
		    for( int c=cBase; c<=cBound; c++ )
		    {
		      U0(i1,i2,i3,c)=U2(i1,i2,i3,c);  // restore values
		    }
		  }
		}
              #else
                // serial case: copy pts
		FOR_3D(i1,i2,i3,J1,J2,J3)
		{
		  if( MASK(i1,i2,i3)>0 )
		  {
		    // ** we actually copy the boundary points too here -- not necessary ***
		    for( int c=cBase; c<=cBound; c++ )
		    {
		      U0(i1,i2,i3,c)=U2(i1,i2,i3,c);  // copy from same refinement level
		    }
		  }
		}
              #endif
#undef U0
#undef U2
#undef MASK

	      }
	      else if( false && useMask )  // old way -- 
	      {
		// opt version (and P++ version)
#ifdef USE_PPP
		// first copy (doing any necessary communication)
		realArray u2;  u2.partition(u0.getPartition()); 
		u2.redim(u0.dimension(0),u0.dimension(1),u0.dimension(2),C);

		const int numDims=4;
		Jv[0]=I1; Jv[1]=I2; Jv[2]=I3; Jv[3]=C;
                      
		ParallelUtility::copy( u2, Jv, u[grid2], Jv, numDims);

		realSerialArray u0Local;  getLocalArrayWithGhostBoundaries(u0,u0Local);
		realSerialArray u2Local;  getLocalArrayWithGhostBoundaries(u2,u2Local);
		intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

		bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,J1,J2,J3,1); 
		if( !ok ) continue;  // there must be no collective operations after this

#else
		const realSerialArray & u0Local  =  u0;
		const realSerialArray & u2Local  =  u[grid2];
		const intSerialArray & maskLocal = mask;
		  
		Index &J1 = Iv[0], &J2=Iv[1], &J3=Iv[2];
#endif                   
                   
		real *u0p = u0Local.Array_Descriptor.Array_View_Pointer3;
		const int u0Dim0=u0Local.getRawDataSize(0);
		const int u0Dim1=u0Local.getRawDataSize(1);
		const int u0Dim2=u0Local.getRawDataSize(2);
#undef U0
#define U0(i0,i1,i2,i3) u0p[i0+u0Dim0*(i1+u0Dim1*(i2+u0Dim2*(i3)))]

		real *u2p = u2Local.Array_Descriptor.Array_View_Pointer3;
		const int u2Dim0=u2Local.getRawDataSize(0);
		const int u2Dim1=u2Local.getRawDataSize(1);
		const int u2Dim2=u2Local.getRawDataSize(2);
#undef U2
#define U2(i0,i1,i2,i3) u2p[i0+u2Dim0*(i1+u2Dim1*(i2+u2Dim2*(i3)))]

		const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
		const int maskDim0=maskLocal.getRawDataSize(0);
		const int maskDim1=maskLocal.getRawDataSize(1);
		const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

		const int cBase=C.getBase(), cBound=C.getBound();
		int i1,i2,i3;
		FOR_3D(i1,i2,i3,J1,J2,J3)
		{
		  if( MASK(i1,i2,i3)>0 )
		  {
		    // ** we actually copy the boundary points too here -- not necessary ***
	    
		    for( int c=cBase; c<=cBound; c++ )
		    {
		      U0(i1,i2,i3,c)=U2(i1,i2,i3,c);  // copy from same refinement level
		      // if( debug & 2 )
		      //  printf("IRB: Copy bndry pt: U0(%i,%i,%i,%i)=%7.3f\n",i1,i2,i3,c,U0(i1,i2,i3,c));
		    }
	    
		  }
		    
		}
#undef U0
#undef U2
#undef MASK
		  
	      }
	      else  // no mask
	      {
		u0(I1,I2,I3,C)=u[grid2](I1,I2,I3,C);  // copy from same refinement level
	      }
	      
	    }
	    else // split==1 : 
	    {
              // (I1,I2,I3) : source values
              // (J1,J2,J3) ; destination values
	      J1=I1; J2=I2; J3=I3;
	      Jv[periodicDirection]-=periodicShift;   // undo the periodic shift

	      // u0(J1,J2,J3,C)=u[grid2](I1,I2,I3,C);  // copy from same refinement level
	      if( useMask )
	      {

		if( false ) // old way
		{
		  where( mask(J1,J2,J3)>0 )
		    for( c=C.getBase(); c<=C.getBound(); c++ )
		      u0(J1,J2,J3,c)=u[grid2](I1,I2,I3,c); // copy from same refinement level
		    
		}
                else if( true ) // newer way -- avoids creation of a distributed array
		{

#ifdef USE_PPP
		  realSerialArray u0Local;  getLocalArrayWithGhostBoundaries(u0,u0Local);
		  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

                  Index J1a=J1, J2a=J2, J3a=J3;
		  bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,J1a,J2a,J3a,1);

	  	  realSerialArray u2;  // holds a copy of pts on u0Local so we can undo changes
                  if( ok )
		  {
		    u2.redim(J1a,J2a,J3a,C);
                    u2(J1a,J2a,J3a,C)=u0Local(J1a,J2a,J3a,C);
		  }

		  // now perform the copy (at all values of the mask) (doing any necessary communication)
		  const int numDims=4;
		  Jv[3]=C;
		  ParallelUtility::copy( u0, Jv, u[grid2], Iv, numDims);  // u0(Jv)=u[grid2](Iv) 


		  J1=J1a, J2=J2a, J3=J3a;
		  if( !ok ) continue;  // there must be no collective operations after this

#else
		  const realSerialArray & u0Local  =  u0;
		  const realSerialArray & u2       =  u[grid2];
		  const intSerialArray & maskLocal = mask;
#endif
		  real *u0p = u0Local.Array_Descriptor.Array_View_Pointer3;
		  const int u0Dim0=u0Local.getRawDataSize(0);
		  const int u0Dim1=u0Local.getRawDataSize(1);
		  const int u0Dim2=u0Local.getRawDataSize(2);
#undef U0
#define U0(i0,i1,i2,i3) u0p[i0+u0Dim0*(i1+u0Dim1*(i2+u0Dim2*(i3)))]

		  real *u2p = u2.Array_Descriptor.Array_View_Pointer3;
		  const int u2Dim0=u2.getRawDataSize(0);
		  const int u2Dim1=u2.getRawDataSize(1);
		  const int u2Dim2=u2.getRawDataSize(2);
#undef U2
#define U2(i0,i1,i2,i3) u2p[i0+u2Dim0*(i1+u2Dim1*(i2+u2Dim2*(i3)))]

		  const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
		  const int maskDim0=maskLocal.getRawDataSize(0);
		  const int maskDim1=maskLocal.getRawDataSize(1);
		  const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

		  const int cBase=C.getBase(), cBound=C.getBound();
                #ifdef USE_PPP
                  // in the parallel case we restore values
		  int j1,j2,j3;
		  FOR_3D(j1,j2,j3,J1,J2,J3)
		  {
		    // if( MASK(j1,j2,j3)>0 )
		    if( MASK(j1,j2,j3)<=0 )
		    {
		      for( int c=cBase; c<=cBound; c++ )
			U0(j1,j2,j3,c)=U2(j1,j2,j3,c);  // restore value
		    }
		    
		  }
                #else
                  // in the serial case we assign values
		  int i1,i2,i3,j1,j2,j3;
		  FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3)
		  {
		    if( MASK(j1,j2,j3)>0 )
		    {
		      for( int c=cBase; c<=cBound; c++ )
			U0(j1,j2,j3,c)=U2(i1,i2,i3,c);  // copy from same refinement level
		    }
		    
		  }
                #endif
#undef U0
#undef U2
#undef MASK


		}
		else // old way now .. // new way -- opt version 
		{
#ifdef USE_PPP
		  // first copy (doing any necessary communication)
	  	  realArray u2;  u2.partition(u0.getPartition()); 
		  u2.redim(u0.dimension(0),u0.dimension(1),u0.dimension(2),C);

		  const int numDims=4;
		  Jv[3]=C;
		  ParallelUtility::copy( u2, Jv, u[grid2], Iv, numDims);  // u2(Jv)=u[grid2](Iv) 

		  realSerialArray u0Local;  getLocalArrayWithGhostBoundaries(u0,u0Local);
		  realSerialArray u2Local;  getLocalArrayWithGhostBoundaries(u2,u2Local);
		  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

		  bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,J1,J2,J3,1); 
		  if( !ok ) continue;  // there must be no collective operations after this

                  I1=J1, I2=J2, I3=J3;  // The source values are now u2(J1,J2,J3)
		  
#else
		  const realSerialArray & u0Local  =  u0;
		  const realSerialArray & u2Local  =  u[grid2];
		  const intSerialArray & maskLocal = mask;
#endif
		  real *u0p = u0Local.Array_Descriptor.Array_View_Pointer3;
		  const int u0Dim0=u0Local.getRawDataSize(0);
		  const int u0Dim1=u0Local.getRawDataSize(1);
		  const int u0Dim2=u0Local.getRawDataSize(2);
#undef U0
#define U0(i0,i1,i2,i3) u0p[i0+u0Dim0*(i1+u0Dim1*(i2+u0Dim2*(i3)))]

		  real *u2p = u2Local.Array_Descriptor.Array_View_Pointer3;
		  const int u2Dim0=u2Local.getRawDataSize(0);
		  const int u2Dim1=u2Local.getRawDataSize(1);
		  const int u2Dim2=u2Local.getRawDataSize(2);
#undef U2
#define U2(i0,i1,i2,i3) u2p[i0+u2Dim0*(i1+u2Dim1*(i2+u2Dim2*(i3)))]

		  const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
		  const int maskDim0=maskLocal.getRawDataSize(0);
		  const int maskDim1=maskLocal.getRawDataSize(1);
		  const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

		  const int cBase=C.getBase(), cBound=C.getBound();
		  int i1,i2,i3,j1,j2,j3;
		  FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3)
		  {
		    if( MASK(j1,j2,j3)>0 )
		    {
		      for( int c=cBase; c<=cBound; c++ )
			U0(j1,j2,j3,c)=U2(i1,i2,i3,c);  // copy from same refinement level
		    }
		    
		  }
#undef U0
#undef U2
#undef MASK


		}
		  

	      }		
	      else
	      {
		u0(J1,J2,J3,C)=u[grid2](I1,I2,I3,C);  // copy from same refinement level
	      }
	    }
	    
 	    // could keep track of how many points are interpolated and break when done.
	  }
	}

	
      } // end for split

      #ifdef USE_PPP
        u0.updateGhostBoundaries();
      #endif
      
    } // end for g
    
  }
  
  timeForRefinementBoundaries+=getCPU()-time0;
  
  return 0;
}



//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{interpolateCoarseFromFine}} 
int InterpolateRefinements::
interpolateCoarseFromFine( realGridCollectionFunction & u,
			   int levelToInterpolate /* = allLevels */,
			   const Range & C0 /* = nullRange */ )
// ======================================================================================================
// /Description:
//    Interpolate coarse grid points that are covered by fine grid points.
// 
// /levelToInterpolate (input) : Interpolate points on this level hidden by finer grids.
// /C0 (input) : optionally specify which components to interpolate.
//
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  real time0=getCPU();
  
  GridCollection & gc = *u.getGridCollection();

  bool useMask = true;

  assert( numberOfDimensions==gc.numberOfDimensions() );
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  Range C = C0==nullRange ? Range(u.getComponentBase(0),u.getComponentBound(0)) : C0;

  // we assume the refinement ratio is the same for all grids
  refinementRatio=gc.refinementLevel[1].refinementFactor(Range(0,2),0);

  if( levelToInterpolate!=allLevels )
  {
    printf("interpolateCoarseFromFine:WARNING:The meaning of the argument levelToInterpolate was changed"
           " on 02/09/027 to actually mean what is says rather than being the finer level\n");
  }

  int levelStart = levelToInterpolate==allLevels ? 0                               : levelToInterpolate;
  int levelEnd   = levelToInterpolate==allLevels ? gc.numberOfRefinementLevels()-2 : levelToInterpolate;

  InterpolateParameters interpParams(gc.numberOfDimensions());
  Interpolate interp(interpParams);
  
  // fill in coarse grid cells that lie underneath this patch
  int level;
  // *wdh*   for( level=levelStart; level<=levelEnd; level++ ) 
  // *wdh* 020927 Go from finer to coarser so we get the correct data!
  for( level=levelEnd; level>=levelStart; level-- )  
  {
    GridCollection & rl = gc.refinementLevel[level+1];  // finer level
    for( int g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      // interpolate this grid
      const int grid=rl.gridNumber(g);
      const int baseGrid=rl.baseGridNumber(g);
      MappedGrid & mg=gc[grid];
      const intArray & mask = mg.mask();
      
      realArray & u0 = u[grid];
	  
      GridCollection & rlm1 = gc.refinementLevel[level];  // coarser level

      int ratio=refinementRatio(0); // *** fix this ***

      Box box = rl[g].box();
      Box box0 = box;
      box0.coarsen(ratio);

      int c,g2;
      for( g2=0; g2<rlm1.numberOfComponentGrids(); g2++ )
      {
	if( rlm1.baseGridNumber(g2)==baseGrid && box0.intersects( rlm1[g2].box() ) )
	{
	  BOX intersection = intersects(box0,rlm1[g2].box());
	  getIndex(intersection,Iv);
	  int grid2=rlm1.gridNumber(g2);

	  J1=Range(I1.getBase()*ratio,I1.getBound()*ratio,ratio);
	  J2=Range(I2.getBase()*ratio,I2.getBound()*ratio,ratio);
	  J3=Range(I3.getBase()*ratio,I3.getBound()*ratio,ratio);
	  
	  if( debug & 2 )
	    printf("Copy coarse grid %i (level %i) from fine grid %i (level %i) I1=(%i,%i) I2=(%i,%i)\n",
		   grid2,level,grid,level+1,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());

	  if( useMask )
	  {
            // old way:
  	    // where( gc[grid2].mask()(I1,I2,I3)>0 )
  	    //   for( c=C.getBase(); c<=C.getBound(); c++ )
  		// u[grid2](I1,I2,I3,c)=u0(J1,J2,J3,c);

            #ifdef USE_PPP
              intSerialArray mask2; getLocalArrayWithGhostBoundaries(gc[grid2].mask(),mask2);
              realSerialArray u2Local;  getLocalArrayWithGhostBoundaries(u[grid2],u2Local);
              if( mask2.dimension(0)!=u2Local.dimension(0) || mask2.dimension(1)!=u2Local.dimension(1) )
	      {
		printf("CoarseFromFine:ERROR: mask2 does not match u2Local!! grid=%i, grid2=%i \n",grid,grid2);
		printf(" mask2: [%i,%i][%i,%i]\n",mask2.getBase(0),mask2.getBound(0),
                       mask2.getBase(1),mask2.getBound(1));
		printf(" u2Local: [%i,%i][%i,%i]\n",u2Local.getBase(0),u2Local.getBound(0),
                       u2Local.getBase(1),u2Local.getBound(1));
		
                Overture::abort("error");
	      }
            #else
	      intSerialArray & mask2 = gc[grid2].mask();
            #endif

            interp.interpolateCoarseFromFine(u[grid2],mask2,Iv,u0,refinementRatio);
            //   interp.interpolateCoarseFromFine(u[grid2],gc[grid2].mask(),Iv,u0,refinementRatio);

	  }
	  else
	  {
	    u[grid2](I1,I2,I3,C)=u0(J1,J2,J3,C);
	  }

	  
	  if( intersection == box || level==0 ) // we are done in these cases (only 1 parent on level 0)
	    break;
	}
      }
    }
  }
  
  timeForCoarseFromFine+=getCPU()-time0;
  return 0;
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{get}}
int InterpolateRefinements::
get( const GenericDataBase & dir, const aString & name)
// ===========================================================================
// /Description:
//   Get from a data base file.
//\end{InterpolateRefinementsInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"InterpolateRefinements");

  aString className;
  subDir.get( className,"className" ); 
  if( className != "InterpolateRefinements" )
  {
    cout << "InterpolateRefinements::get ERROR in className!" << endl;
  }

  subDir.get(numberOfDimensions,"numberOfDimensions");
  // Interpolate interp;
  interpParams.get(subDir,"interpParams");
  subDir.get(refinementRatio,"refinementRatio");
  subDir.get(numberOfGhostLines,"numberOfGhostLines");

  delete &subDir;
  return 0;
}

//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{put}}
int InterpolateRefinements::
put( GenericDataBase & dir, const aString & name) const
// ===========================================================================
// /Description:
//   Put to a data base file.
//\end{InterpolateRefinementsInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"InterpolateRefinements");                   // create a sub-directory 

  subDir.put( "InterpolateRefinements","className" );

  subDir.put(numberOfDimensions,"numberOfDimensions");
  // Interpolate interp;
  interpParams.put(subDir,"interpParams");
  subDir.put(refinementRatio,"refinementRatio");
  subDir.put(numberOfGhostLines,"numberOfGhostLines");

  delete &subDir;
  return 0;
}



//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{interpolateRefinementBoundaries}} 
int InterpolateRefinements::
interpolateRefinementBoundaries( ListOfParentChildSiblingInfo & listOfPCSInfo,
                                 realGridCollectionFunction & u,
                                 int levelToInterpolate /* = allLevels */,
                                 const Range & C0 /* = nullRange */ )
// ======================================================================================================
// /Description:
//    Interpolate the ghost values on refinement grids.
//
// /Note: This function assumes that grids are properly nested.
// 
// /C0 (input) : optionally specify which components to interpolate.
//
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  GridCollection & gc = *u.getGridCollection();
  if( gc.numberOfRefinementLevels()<=1 )
    return 0;

  if( debug & 2 ) printf(" *** interpolateRefinementBoundaries PCS version ****\n");
  
  bool useMask = true;

  // const int numberOfDimensions=gc.numberOfDimensions();
  assert( numberOfDimensions==gc.numberOfDimensions() );
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  int c;
  Range C = C0==nullRange ? Range(u.getComponentBase(0),u.getComponentBound(0)) : C0;

  int levelStart = levelToInterpolate==allLevels ? 1 : levelToInterpolate;
  int levelEnd   = levelToInterpolate==allLevels ? gc.numberOfRefinementLevels()-1 : levelToInterpolate;

  // we assume the refinement ratio is the same for all grids
  refinementRatio=gc.refinementLevel[1].refinementFactor(Range(0,2),0);

  intSerialArray gridIndices;
  BoxList sourceBoxes;
  BoxList ghostBoxesOnCurrentGrid;
  Range ghostLines(1,numberOfGhostLines);
  IndexType iType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));  // ******************** fix ***


  // now try to interpolate all refinement grids
  int level;
  for( level=levelStart; level<=levelEnd; level++ )
  {
    GridCollection & rl = gc.refinementLevel[level];
    for( int g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      // interpolate this grid
      const int grid=rl.gridNumber(g);
      const int baseGrid=rl.baseGridNumber(g);
      MappedGrid & mg=gc[grid];
      const intArray & mask = mg.mask();
      #ifdef USE_PPP
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      #else
        const intSerialArray & maskLocal = mask;
      #endif
      realArray & u0 = u[grid];
	  

      ParentChildSiblingInfo & pcs = listOfPCSInfo[grid];


      // *********** we want to exclude BC sides *******************
      bool excludeSiblingPoints=true; // false;
      
      pcs.getParentGhostBoxes( gridIndices, 
			       sourceBoxes,
			       ghostLines,
			       gc,
			       grid,
			       iType,
			       excludeSiblingPoints );
      
      int numberOfParentBoxes=gridIndices.getLength(0);

      if( debug & 2 ) printf(" grid=%i : numberOfParentBoxes=%i\n",grid,numberOfParentBoxes);
      

      BoxListIterator bli(sourceBoxes);
      for( int p=0; bli; ++bli, p++)
      {
        int grid2=gridIndices(p);
        getIndex( sourceBoxes[bli],Iv ); // ***** fix for cell centred

        if( debug & 2 )
	{
	  printf("grid=%i interp ghost lines from parent=%i, [%i,%i]x[%i,%i]x[%i,%i]\n",grid,grid2,
		 I1.getBase(),I1.getBound(),
		 I2.getBase(),I2.getBound(),
		 I3.getBase(),I3.getBound());
	}

        if( true )
	{
	  real time1=getCPU();
	  interp.interpolateFineFromCoarse( u0,maskLocal,Iv,u[grid2],refinementRatio );
	  timeForBoundaryCoarseFromFine+=getCPU()-time1;
          
	}
	else
	{
	  realArray u1(I1,I2,I3,C);

        // *** I guess there is no need to worry about a periodic grids ??
	  real time1=getCPU();
	  interp.interpolateCoarseToFine( u1,Iv,u[grid2],refinementRatio );
	  timeForBoundaryCoarseFromFine+=getCPU()-time1;

	  if( useMask )
	  {
	    where( mask(I1,I2,I3)>0 )
	      for( c=C.getBase(); c<=C.getBound(); c++ )
		u0(I1,I2,I3,c)=u1(I1,I2,I3,c);
	  }
	  else
	  {
	    u0(I1,I2,I3,C)=u1(I1,I2,I3,C);
	  }
	}
	

      }
      

      // *******************************************************
      // ***** Interpolate ghost boundaries from siblings ******
      // *******************************************************

      pcs.getSiblingGhostBoxes( gridIndices, 
				sourceBoxes,
				ghostBoxesOnCurrentGrid,
				ghostLines,
				gc,
				grid,
				iType);
     

      int numberOfSiblingBoxes=gridIndices.getLength(0);
      if( debug & 2 ) printf(" grid=%i : numberOfSiblingBoxes=%i\n",grid,numberOfSiblingBoxes);

      BoxListIterator sbli(sourceBoxes), cbli(ghostBoxesOnCurrentGrid);
      for( int s=0;  sbli; ++sbli, ++cbli, s++)
      {
        int grid2=gridIndices(s);
        getIndex( ghostBoxesOnCurrentGrid[cbli],Iv );
	getIndex( sourceBoxes[sbli],Jv );
	
        if( debug & 2 )
	{
	  printf("grid=%i interp ghost lines from sibling=%i, [%i,%i]x[%i,%i]x[%i,%i]\n",grid,grid2,
		 I1.getBase(),I1.getBound(),
		 I2.getBase(),I2.getBound(),
		 I3.getBase(),I3.getBound());
	}

	realArray & u2 = u[grid2];
	if( useMask )
	{
	  where( mask(I1,I2,I3)>0 )
	    for( int c=C.getBase(); c<=C.getBound(); c++ )
	      u0(I1,I2,I3,c)=u2(J1,J2,J3,c);  // copy from same refinement level
	}
	else
	{
	  u0(I1,I2,I3,C)=u2(J1,J2,J3,C);  // copy from same refinement level
	}

      }
      
    }
  }
  
  return 0;
}



//\begin{>>InterpolateRefinementsInclude.tex}{\subsection{interpolateCoarseFromFine}} 
int InterpolateRefinements::
interpolateCoarseFromFine( ListOfParentChildSiblingInfo & listOfPCSInfo, 
                           realGridCollectionFunction & u,
			   int levelToInterpolate /* = allLevels */,
			   const Range & C0 /* = nullRange */ )
// ======================================================================================================
// /Description:
//    Interpolate coarse grid points that are covered by fine grid points.
// 
// /C0 (input) : optionally specify which components to interpolate.
//
//\end{InterpolateRefinementsInclude.tex} 
//=========================================================================================
{
  
  GridCollection & gc = *u.getGridCollection();

  bool useMask = true;

  assert( numberOfDimensions==gc.numberOfDimensions() );
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  Range C = C0==nullRange ? Range(u.getComponentBase(0),u.getComponentBound(0)) : C0;

  // we assume the refinement ratio is the same for all grids
  refinementRatio=gc.refinementLevel[1].refinementFactor(Range(0,2),0);

  int levelStart = levelToInterpolate==allLevels ? 1 : levelToInterpolate;
  int levelEnd   = levelToInterpolate==allLevels ? gc.numberOfRefinementLevels()-1 : levelToInterpolate;

  intSerialArray gridIndices;
  BoxList parentBoxes;
  IndexType iType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));  // ******************** fix ***

  // fill in coarse grid cells that lie underneath this patch
  int level;
  for( level=levelEnd; level>=levelStart; level-- )  // work from finest to coarsest
  {
    GridCollection & rl = gc.refinementLevel[level];
    for( int g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      // interpolate this grid
      const int grid=rl.gridNumber(g);
      const int baseGrid=rl.baseGridNumber(g);
      MappedGrid & mg=gc[grid];
      const intArray & mask = mg.mask();
      
      realArray & u0 = u[grid];
	  
      int ratio=refinementRatio(0); // *** fix this ***

      ParentChildSiblingInfo & pcs = listOfPCSInfo[grid];
      
      pcs.getParentBoxes(gridIndices, 
			 parentBoxes,
			 iType,
			 mg.numberOfDimensions() );

      BoxListIterator bli(parentBoxes);
      for( int p=0; bli; ++bli, p++)
      {
        int grid2=gridIndices(p);
        getIndex( *bli,Iv );

	J1=Range(I1.getBase()*ratio,I1.getBound()*ratio,ratio);
	J2=Range(I2.getBase()*ratio,I2.getBound()*ratio,ratio);
	J3=Range(I3.getBase()*ratio,I3.getBound()*ratio,ratio);
	  
	if( true || debug & 2 )
	  printf("Copy coarse grid %i from fine grid %i I1=(%i,%i) I2=(%i,%i)\n",
		 grid2,grid,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());

        realArray & u2 = u[grid2];
        const intArray & mask2= gc[grid2].mask();
	if( useMask )
	{
	  where( mask2(I1,I2,I3)>0 )
	    for( int c=C.getBase(); c<=C.getBound(); c++ )
	      u2(I1,I2,I3,c)=u0(J1,J2,J3,c);
	}
	else
	{
	  u2(I1,I2,I3,C)=u0(J1,J2,J3,C);
	}
      }
    }
  }
  
  return 0;
}


void InterpolateRefinements::
printStatistics( FILE *file /* =NULL */ ) const
{
  FILE *output = file!=NULL ? file : stdout;
  fPrintF(file,
         " InterpolateRefinements: cpu times\n"
         "     coarseFromFine         =%8.1e\n"
         "     refinementBoundaries   =%8.1e\n"
         "     refinements            =%8.1e\n"
         "     boundaryCoarseFromFine =%8.1e\n",
	 timeForCoarseFromFine,timeForRefinementBoundaries,timeForRefinements,timeForBoundaryCoarseFromFine);
}

