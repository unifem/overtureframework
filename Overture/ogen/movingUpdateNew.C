// This file automatically generated from movingUpdateNew.bC with bpp.
// =============================================================================================
/// The new moving grid generator (parallel version)
// =============================================================================================

#include "Ogen.h"
#include "PlotStuff.h"
#include "MappingRC.h"
#include "conversion.h"
#include "display.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"

// we need to define these here for gcc
// typedef CompositeGridData_BoundaryAdjustment       BoundaryAdjustment;
typedef TrivialArray<BoundaryAdjustment,Range>     BoundaryAdjustmentArray;
typedef TrivialArray<BoundaryAdjustmentArray,Range>BoundaryAdjustmentArray2;

#ifdef USE_PPP
  #define GET_LOCAL(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
  #define GET_LOCAL_CONST(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
  #define GET_LOCAL(type,xd,xs)type ## SerialArray & xs = xd
  #define GET_LOCAL_CONST(type,xd,xs)const type ## SerialArray & xs = xd
#endif


#define FORJP(k1,k2,k3,j1,j2,j3) 	    for( k3=j3Min; k3<=j3Max; k3++ )  	    { j3 = periodic[2] ? (k3+period[2])%period[2] : k3; 	      for( k2=j2Min; k2<=j2Max; k2++ ) 	      { j2 = periodic[1] ? (k2+period[1])%period[1] : k2; 		for( k1=j1Min; k1<=j1Max; k1++ ) 		{ j1 = periodic[0] ? (k1+period[0])%period[0] : k1;

#define  ENDFORJP }}}

#define FORJP_STENCIL(k1,k2,k3,j1,j2,j3) 	    for( k3=stencil[2][0]; k3<=stencil[2][1]; k3++ )  	    { j3 = periodic2[2] ? (k3+period2[2])%period2[2] : k3; 	      for( k2=stencil[1][0]; k2<=stencil[1][1]; k2++ ) 	      { j2 = periodic2[1] ? (k2+period2[1])%period2[1] : k2; 		for( k1=stencil[0][0]; k1<=stencil[0][1]; k1++ ) 		{ j1 = periodic2[0] ? (k1+period2[0])%period2[0] : k1;

#define  ENDFORJP_STENCIL }}}


  // dimension new serial array interpolation data arrays
#define adjustSizeMacro(x,n)while( x.getLength() < n )x.addElement();while( x.getLength() > n )x.deleteElement()


// =============================================================================================
/// \brief The new moving grid generator (parallel version)
// =============================================================================================
int Ogen::
movingUpdateNew(CompositeGrid & cg, 
            	      CompositeGrid & cgOld, 
            	      const LogicalArray & hasMoved, 
            	      const MovingGridOption & option /* =useOptimalAlgorithm */ )
// ========================================================================================
// /Access: Protected.
//    
// /Description:
//    Determine an overlapping grid when one or more grids has moved.
//   {\bf NOTE:} If the number of grid points changes then you should use the 
//   {\tt useFullAlgorithm} option.
// 
// /cg (input) : grid to update
// /cgOld (input) : for grids that have not moved, share data with this CompositeGrid.
// /hasMoved (input): specify which grids have moved with hasMoved(grid)=true
// /option (input) : An option from one of:
// {\footnotesize
// \begin{verbatim}
//   enum MovingGridOption
//   {
//     useOptimalAlgorithm=0,
//     minimizeOverlap=1,
//     useFullAlgorithm
//   };
// \end{verbatim}
// }
//  The {\tt useOptimalAlgorithm} may result in the overlap increasing as the grid is moved.
//
// /Return value: 0=success, otherwise the number of errors encountered.
// 
//\end{ogenUpdateInclude.tex}
// ========================================================================================
{
//  #ifdef USE_PPP
//   printF("Ogen::movingUpdateNew: This routine is not ready yet for parallel. Use the full update algorithm instead\n");
//   OV_ABORT("error");
//  #endif

    info=3;
    debug=3;
    if( debug & 2 )
        printF("Ogen::movingUpdateNew: -- parallel moving grid generator ---\n");
    
    isMovingGridProblem=true;
    

// cg->localInterpolationDataState: 
//     enum LocalInterpolationDataEnum
//     {
//       noLocalInterpolationData,
//       localInterpolationDataForAMR,
//       localInterpolationDataForAll
//     };

    if( debug & 4 )
    {
        printF("cg->localInterpolationDataState=%i\n",(int)cg->localInterpolationDataState);
        
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            printF(" grid=%i, numberOfInterpolationPoints=%i\n",grid,cg.numberOfInterpolationPoints(grid));
            for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
            {
      	printf("movingUpdateNew: myid=%i cg.interpolationStartEndIndex(0:1,grid=%i,grid2=%i)=%i %i \n",
             	       myid,
             	       grid,grid2,
             	       cg.interpolationStartEndIndex(0,grid,grid2),
             	       cg.interpolationStartEndIndex(1,grid,grid2));
            }
        }
    }
    if( cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
    {
    // --- Do this for now: call the full grid generator if we do not have local interp data.
    // This can happen at the start of a moving grid simulation.

        printF("Ogen::movingUpdateNew: call the full algorithm...\n");
        debug=0; info=0;  // reset these
          
        int returnValue= updateOverlap(cg);
        return returnValue;
        
    }


//   if( false )
//   {
//     outputMovingGrids(cg,cgOld,hasMoved);
//   }
    

    if( debug>0 )
    {
        if( plotOrphanPoints.getLength(0)<cg.numberOfComponentGrids() )
        {
            plotOrphanPoints.redim(cg.numberOfComponentGrids());
            plotOrphanPoints=true;
        }
    }

    int returnValue=0;
    const int numberOfBaseGrids=cg.numberOfBaseGrids();
    const int numberOfComponentGrids=cg.numberOfComponentGrids();

    const int numberOfDimensions = cg.numberOfDimensions();
    
    const int computedGeometry0=cg->computedGeometry;

  //      Resize the interpolation data based on the old grid.

  // ***ERROR: cannot redim since referenced to letter: cg.numberOfInterpolationPoints.redim(0);
    cg.numberOfInterpolationPoints = cgOld.numberOfInterpolationPoints;

    cg->numberOfInterpolationPointsLocal = cgOld->numberOfInterpolationPointsLocal;
    
  // printF(" movingUpdate: start; cg.numberOfInterpolationPoints(0)=%i\n",cg.numberOfInterpolationPoints(0));
    
  // *********************** should not need to update AMR grids here -- fix this **************

  // NOTE: after setting the numberOfInterpolationPoints the follow arrays will be recomputed
    cg.update(
        CompositeGrid::THEmask                     |
        CompositeGrid::THEinterpolationCoordinates |
        CompositeGrid::THEinterpoleeGrid           |
        CompositeGrid::THEinterpoleeLocation       |
        CompositeGrid::THEinterpolationPoint       |
    // THEinterpolationCondition   |
    // theLists                    ,
        CompositeGrid::COMPUTEnothing              );

  // printF(" movingUpdate(1): start; cg.numberOfInterpolationPoints(0)=%i\n",cg.numberOfInterpolationPoints(0));

    cg.interpolationStartEndIndex=cgOld.interpolationStartEndIndex;

    #ifdef USE_PPP
        IntegerArray & numberOfInterpolationPointsLocal = cg->numberOfInterpolationPointsLocal;
        if( numberOfInterpolationPointsLocal.getLength(0)==0 )
            numberOfInterpolationPointsLocal.redim(cg.numberOfComponentGrids());
    #endif


    for( int grid=0; grid<numberOfBaseGrids; grid++ )
    {
        MappedGrid & g = cg[grid];
        MappedGrid & gOld = cgOld[grid];

        GET_LOCAL(int,g.mask(),maskLocal);
        GET_LOCAL(int,gOld.mask(),maskOldLocal);

    // --- Copy the mask from the old grid ---
        maskLocal = maskOldLocal;
    // displayMask(g.mask(),"g.mask()");
        
        g->computedGeometry |= CompositeGrid::THEmask;

    // -----------------------------------------------------
    // --- Copy the interpolation data from the old grid ---
    // -----------------------------------------------------

    // tell the CompositeGrid that we are storing the interp data in a local serial form:
        cg->localInterpolationDataState=CompositeGridData::localInterpolationDataForAll;

        const int numberOfGrids=cg.numberOfGrids();
    
        adjustSizeMacro(cg->interpolationPointLocal,numberOfGrids);
        adjustSizeMacro(cg->interpoleeGridLocal,numberOfGrids);
        adjustSizeMacro(cg->variableInterpolationWidthLocal,numberOfGrids);
        adjustSizeMacro(cg->interpoleeLocationLocal,numberOfGrids);
        adjustSizeMacro(cg->interpolationCoordinatesLocal,numberOfGrids);

    // *NOTE* In parallel Ogen assigns both cg.numberOfInterpolationPoints and cg->numberOfInterpolationPointsLocal

        if( cg.numberOfInterpolationPoints(grid) )
        {
            #ifdef USE_PPP
       // printF(" cgOld->localInterpolationDataState = %i\n",(int)cgOld->localInterpolationDataState);
            
              cg->interpolationCoordinatesLocal[grid].redim(0);
              cg->interpoleeGridLocal[grid].redim(0);
              cg->interpoleeLocationLocal[grid].redim(0);
              cg->interpolationPointLocal[grid].redim(0);
              cg->variableInterpolationWidthLocal[grid].redim(0);
              if( cgOld->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
              {
         // This grid was not built in parallel, get interp info from global arrays
                  if( debug & 4 )
                 	   printF("Ogen::movingUpdateNew: Copy interp data from global arrays... \n");
       	 
                  cg->interpolationCoordinatesLocal[grid]  =cgOld.interpolationCoordinates[grid].getLocalArray();
                  cg->interpoleeGridLocal[grid]            =cgOld.interpoleeGrid[grid].getLocalArray();
                  cg->interpoleeLocationLocal[grid]        =cgOld.interpoleeLocation[grid].getLocalArray();
                  cg->interpolationPointLocal[grid]        =cgOld.interpolationPoint[grid].getLocalArray();
                  cg->variableInterpolationWidthLocal[grid]=cgOld.variableInterpolationWidth[grid].getLocalArray();
              }
              else
              {
                  if( debug & 4 )
         	   printF("Ogen::movingUpdateNew: Copy interp data from local arrays...\n");

                cg->interpolationCoordinatesLocal[grid]   = cgOld->interpolationCoordinatesLocal[grid];
                cg->interpoleeGridLocal[grid]             = cgOld->interpoleeGridLocal[grid];
                cg->interpoleeLocationLocal[grid]         = cgOld->interpoleeLocationLocal[grid];
                cg->interpolationPointLocal[grid]         = cgOld->interpolationPointLocal[grid];
                cg->variableInterpolationWidthLocal[grid] = cgOld->variableInterpolationWidthLocal[grid];
              }
            #else
                cg.interpolationCoordinates[grid]   = cgOld.interpolationCoordinates[grid];
                cg.interpoleeGrid[grid]             = cgOld.interpoleeGrid[grid];
                cg.interpoleeLocation[grid]         = cgOld.interpoleeLocation[grid];
                cg.interpolationPoint[grid]         = cgOld.interpolationPoint[grid];
                cg.variableInterpolationWidth[grid] = cgOld.variableInterpolationWidth[grid];
            #endif

      // *wdh* 040808 -- we need to reset the USESbackupRules flag at interpolation points
      // **** we should add usesBackupRules(grid) to CompositeGrid so we would know whether this must be done ***

            const int numberOfInterpolationPoints= cg.numberOfInterpolationPoints(grid);
            #ifdef USE_PPP
                int numInterpLocal;
                const intSerialArray & ip = cg->interpolationPointLocal[grid];
                if( cgOld->localInterpolationDataState==CompositeGridData::noLocalInterpolationData )
      	{
                    numInterpLocal=ip.getLength(0);
                    cg->numberOfInterpolationPointsLocal(grid)=numInterpLocal;
      	}
                else
                    numInterpLocal= cg->numberOfInterpolationPointsLocal(grid); 
            #else
                const int numInterpLocal= cg.numberOfInterpolationPoints(grid);
                const intSerialArray & ip = cg.interpolationPoint[grid];
            #endif
      // const intArray & mask = g.mask();
      	if( false )
      	{
        	  display(cgOld.interpolationPoint[grid],sPrintF(" cgOld.interpolationPoint for grid %i\n",grid));
        	  display(cgOld->interpolationPointLocal[grid],sPrintF(" cgOld.interpolationPointLocal for grid %i\n",grid));
        	  display(ip,sPrintF(" interpolationPointLocal for grid %i\n",grid));
      	}

            assert( ip.getLength(0)==numInterpLocal );
            if( cg.numberOfDimensions()==2 )
            {
      	const int i3=g.dimension(0,2);
      	for( int i=0; i<numInterpLocal; i++ )
      	{
        	  if( maskLocal(ip(i,0),ip(i,1),i3) & MappedGrid::USESbackupRules ) 
        	  {
          	    maskLocal(ip(i,0),ip(i,1),i3) ^= MappedGrid::USESbackupRules;  // exclusive or to turn off USESbackupRules
        	  }
      	}
            }
            else
            {
      	for( int i=0; i<numInterpLocal; i++ )
      	{
        	  if( maskLocal(ip(i,0),ip(i,1),ip(i,2)) & MappedGrid::USESbackupRules ) 
        	  {
          	    maskLocal(ip(i,0),ip(i,1),ip(i,2)) ^= MappedGrid::USESbackupRules;  // exclusive or to turn off USESbackupRules
        	  }
      	}
            }
            

        }
    }
  //      The interpolationPoint and interpoleeGrid in c
  //      should be marked up-to-date if they were in c0.
    cg->computedGeometry &= ~(
        CompositeGrid::THEinterpolationPoint          |
        CompositeGrid::THEinterpoleeGrid              );
    cg->computedGeometry |= cgOld.computedGeometry() & (
        CompositeGrid::THEinterpolationPoint          |
        CompositeGrid::THEinterpoleeGrid              );


  //      Try to recompute the rest of the interpolation data from
  //      interpoleeGrid and interpolationPoint.
/* -----
    Integer update = cg.update(
        CompositeGrid::THEinterpolationCoordinates |
        CompositeGrid::THEinterpoleeLocation       |
    // theLists                    ,
        CompositeGrid::COMPUTEgeometry             );
---- */

  // reduce the minimum overlap to .25 
    Range Rx=numberOfDimensions, G=numberOfComponentGrids;
  // save existing values
    const int level =0;  // *** multigrid level **** fix this
    IntegerArray interpolationWidth(Rx,G,G); interpolationWidth=cg.interpolationWidth(Rx,G,G,level);
    RealArray interpolationOverlap(Rx,G,G); interpolationOverlap=cg.interpolationOverlap(Rx,G,G,0);
            
    if( false )
    {
    // need to fix up interpoleeLocation if ov<.5 !
        for( int grid=0; grid<numberOfComponentGrids; grid++ )
        {
            for( int grid2=0; grid2<numberOfComponentGrids; grid2++ )
            {
      	cg.interpolationOverlap(Rx,grid,grid2,0)=max(.25,cg.interpolationOverlap(Rx,grid,grid2,0)-.25);
            }
        }
    }
    
  // printF(" movingUpdate(2): start; cg.numberOfInterpolationPoints(0)=%i\n",cg.numberOfInterpolationPoints(0));

    checkForOneSided=true;
    Range Axes(0,cg.numberOfDimensions()-1);
    #ifdef USE_PPP
    // ::display(cg->numberOfInterpolationPointsLocal,"cg->numberOfInterpolationPointsLocal");
    // ::display(cg->numberOfInterpolationPoints,"cg->numberOfInterpolationPoints");

      const int maxInterp=max(cg->numberOfInterpolationPointsLocal);

   // *FIX me: 
   // if( cg->numberOfInterpolationPointsLocal.getLength(0)>0 )
   //   maxNumberOfInterpolationPoints=max(cg->numberOfInterpolationPointsLocal);
   // else if( cg.numberOfInterpolationPoints.getLength(0)>0 )
   //   maxNumberOfInterpolationPoints=max(cg.numberOfInterpolationPoints);
   // 
   //  const int maxInterp = maxNumberOfInterpolationPoints*3+100;
    #else
        const int maxInterp = max(cg.numberOfInterpolationPoints)*3+100;
    #endif
    
    realSerialArray r(maxInterp,cg.numberOfDimensions()), x(maxInterp,cg.numberOfDimensions());
    r=-1.;

    intSerialArray *iInterp = new intSerialArray [ numberOfBaseGrids ];
    intSerialArray *interpNew = new intSerialArray [ numberOfBaseGrids ];

    intSerialArray ia2(maxInterp,3); // *** fix this
    ia2=0;
    intSerialArray useBackupRules(maxInterp);  

    intSerialArray interpolates(maxInterp);
    useBackupRules=false;
    
  // These next lines also appear in classifyPoints
    assert( backupValues==NULL );
    backupValues = new intSerialArray [ numberOfBaseGrids ];  // to hold backup values
  // backupValuesUsed(grid) = true is some backup values have been used
    backupValuesUsed.redim(numberOfBaseGrids);
    backupValuesUsed=false;


  // cg.interpolationOverlap.display("cg.interpolationOverlap");
    
    IntegerArray numToFix(numberOfBaseGrids);
    numToFix=0;
    
    numberOfOrphanPoints=0;

//bool useBoundaryAdjustment=true;
    IntegerArray sidesShare(2,3,2,3); 
    if( isNew.getLength(0)!=numberOfBaseGrids )
    {
        isNew.redim(numberOfBaseGrids);  // this grid is in the list of new grids 
        isNew=true;
    }

    useBoundaryAdjustment=true;
    
// **  const int numberOfBaseGrids = cg.numberOfBaseGrids();
    BoundaryAdjustmentArray2 & boundaryAdjustment = cg.rcData->boundaryAdjustment;
    bool computeBoundaryAdjustment=false;
    if( useBoundaryAdjustment &&
            (boundaryAdjustment.getBound(0)-boundaryAdjustment.getBase(0)+1) <numberOfBaseGrids )
    {
        computeBoundaryAdjustment=true;
        boundaryAdjustment.redim(numberOfBaseGrids,numberOfBaseGrids);
    }
    
  // *wdh* 040719 NB: When marking new potential interpolation points we should be sure
  //         to use a stencil with an odd number of points.
    const bool useOneSidedAtBoundaries=true;
    const bool useOddInterpolationWidth=true;

  // ************************************************************************
  //  Stage I:
  //     (1) update boundary adjustment
  //     (2) check if old interp pts are still valid
  //     (3) make a list of invalid interp pts.
  // ***********************************************************************

    bool ok=true;
    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //

    for( int grid=0; grid<numberOfBaseGrids; grid++ )
    {
        MappedGrid & c = cg[grid];

        const bool isRectangular = c.isRectangular();
        if( isRectangular )
        {
            c.getRectangularGridParameters( dvx, xab );
            for( int dir=0; dir<c.numberOfDimensions(); dir++ )
            {
      	iv0[dir]=c.gridIndexRange(0,dir);
      	if( c.isAllCellCentered() )
        	  xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
            }
            		
        }
        #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

        if( !isRectangular ) // *wdh* 110727 -- fixed : only build the vertex if not rectangular
          c.update( MappedGrid::THEvertex | MappedGrid::THEcenter );
        realArray & center = c.center();

        intArray & mask = c.mask();
        intSerialArray & ia = iInterp[grid];
        #ifdef USE_PPP
            GET_LOCAL(int,mask,maskLocal);
        #else
            intSerialArray & maskLocal = mask;
        #endif

        OV_GET_SERIAL_ARRAY_CONDITIONAL(real,center,centerLocal,!isRectangular);
        #ifdef USE_PPP
            intSerialArray & interpolationPoint = cg->interpolationPointLocal[grid];
            intSerialArray & interpoleeGrid = cg->interpoleeGridLocal[grid];
            intSerialArray & interpoleeLocation = cg->interpoleeLocationLocal[grid];
            realSerialArray & interpolationCoordinates = cg->interpolationCoordinatesLocal[grid];
        #else
            intSerialArray & interpolationPoint = cg.interpolationPoint[grid];
            intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
            intSerialArray & interpoleeLocation = cg.interpoleeLocation[grid];
            realSerialArray & interpolationCoordinates = cg.interpolationCoordinates[grid];
        #endif    
          

        int startIndex, endIndex=0;
        const int & numberOfInterpolationPoints = cg.numberOfInterpolationPoints(grid);
        i3 = c.extendedIndexRange(Start,axis3);
        
        for( int grid2=0; grid2<numberOfBaseGrids; grid2++ )
        {
            if( cg.interpolationStartEndIndex(0,grid,grid2)<0 ) continue;
            
            if( useBoundaryAdjustment &&
                    ( computeBoundaryAdjustment || hasMoved(grid) || hasMoved(grid2) ) )
            {
        // **** this is being done too often ***** need to know when shared side info
	// should be recomputed --- > 
        //       use hasMoved(grid) : bit flag?
        //  hasMoved(grid,grid2) 
        //             & 2  : recompute shared sides with all other grids ???
        //             & 4  : no new shared sides have appeared
        //             & 8  : no need to recompute shared sides with grid2 


      	intSerialArray iag[6];   // could *new* to be 2*nd
      	realSerialArray rg[6], xg[6];
                if( debug & 4 ) printF("updateBoundaryAdjustment for grid=%i grid2=%i\n",grid,grid2);
      	updateBoundaryAdjustment(cg,grid,grid2,iag,rg,xg,sidesShare);
            }



      // update points on c that interpolate from c2
            startIndex=cg.interpolationStartEndIndex(0,grid,grid2);
            endIndex  =cg.interpolationStartEndIndex(1,grid,grid2);

            if( endIndex>=cg.numberOfInterpolationPoints(grid) )
            {
                printF("movingUpdate:ERROR: grid=%i grid2=%i startIndex=%i endIndex=%i is >= numberOfInterpolationPoints=%i\n",
             	       grid,grid2,startIndex,endIndex,cg.numberOfInterpolationPoints(grid));
      	
                printF("  (old grid numberOfInterpolationPoints=%i\n",cgOld.numberOfInterpolationPoints(grid));
      	
      	assert( endIndex<cg.numberOfInterpolationPoints(grid) );
            }
            
            
//    startIndex=endIndex;
//       while( endIndex<numberOfInterpolationPoints && interpoleeGrid(endIndex)==grid2 )
// 	endIndex++;

            if( endIndex>=startIndex && (hasMoved(grid) || hasMoved(grid2)) )
            {
      	MappedGrid & c2 = cg[grid2];

	// -- make a list of interpolation points (points are ordered by grid2) --
      	Range Ra(startIndex,endIndex);
      	Range R(0,endIndex-startIndex);  // base 0 version of Ra
                
      	ia2(R,Axes)=interpolationPoint(Ra,Axes);
                if( numberOfDimensions==2 )
                    ia2(R,axis3)=c.gridIndexRange(Start,axis3);
      	
        // -- compute the x coordinates of the points --
      	Mapping & map2 = c2.mapping().getMapping();
      	if( isRectangular ) // *wdh* 110727
      	{
        	  for( int i=R.getBase(); i<=R.getBound(); i++ )
        	  {
          	    i1=ia2(i,0); i2=ia2(i,1); i3=ia2(i,2);
          	    for( int axis=0; axis<numberOfDimensions; axis++ )
          	    {
            	      x(i,axis)=XC(iv,axis);
          	    }
        	  }
      	}
      	else // curvilinear
      	{ 
          // -- fix me : call map.map(r,x)
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    if( numberOfDimensions==2 )
            	      x(R,axis)=centerLocal(ia2(R,0),ia2(R,1),i3,axis);
          	    else
            	      x(R,axis)=centerLocal(ia2(R,0),ia2(R,1),ia2(R,2),axis);
        	  }
      	}

      	if( info & 4 )
        	  printF("Ogen: moving grid update: re-interpolate points (%i,%i) of grid %i from grid %i\n",startIndex,
             		 endIndex,grid,grid2);
      	
                if( useBoundaryAdjustment )
        	  adjustBoundary(cg,grid,grid2,ia2(R,Axes),x(R,Axes)); 

        // *************************************************************
        // ****** find the *new* locations of the old interp pts *******
        // *************************************************************
                #ifdef USE_PPP
        	  map2.inverseMapS(x(R,Axes),r);
                #else
        	  map2.inverseMap(x(R,Axes),r);
                #endif

	// r.display("Here are the inverse coordinates");
	// cgOld.interpolationCoordinates[grid].display("cgOld.interpolationCoordinates");
	// interpoleeLocation.display("interpoleeLocation");
      	

      	interpolates(R)=true;  // could do at the start

                #ifdef USE_PPP
        	  realSerialArray rr(R,Axes); rr=r(R,Axes);  // fix me 
        	  bool gridOk =checkCanInterpolate(cg,grid,grid2, rr, interpolates,useBackupRules);
                #else
            	  bool gridOk = cg.rcData->canInterpolate(grid,grid2, r(R,Axes), interpolates,useBackupRules,checkForOneSided);
                #endif

                ok=ok && gridOk;
      	if( !gridOk ) 
      	{
        	  if( info & 2 )
          	    printF("Ogen: moving update: unable to re-interp old interp pts of  grid %i from grid %i\n",grid,grid2);
	  // interpolates.display("interpolates?");
        	  
        	  if( debug & 2 )
        	  {
          	    for( int i=R.getBase(); i<=R.getBound(); i++ )
          	    {
            	      if( !interpolates(i) )
            	      {
            		printF(" pt (%i,%i,%i) from grid %i could NOT interp from grid %i r=(%6.2e,%6.2e)\n",
                   		       ia2(i,0),ia2(i,1),ia2(i,2),grid,grid2,r(i,0),r(i,1));
            	      }
            	      else
            	      {
            		printF(" pt (%i,%i,%i) from grid %i could interp from grid %i r=(%6.2e,%6.2e)\n",
                   		       ia2(i,0),ia2(i,1),ia2(i,2),grid,grid2,r(i,0),r(i,1));
            	      }
          	    }
        	  }
        	  
	  // *** make a list of invalid interpolee points
        	  intSerialArray ib;
                    intSerialArray m; m = interpolates(R)==0;   // ***** fix this ****
        	  
	  // ib=m.indexMap();
	  // ib=(interpolates(R)==0).indexMap();   // this works but has a leak
                    int i,j,n;
                    n=sum(m);
                    ib.redim(sum(m));
                    j=0;
        	  for( int i=R.getBase(); i<=R.getBound(); i++ )
        	  {
          	    if( m(i) )
          	    {
            	      ib(j)=i;
            	      j++;
          	    }
        	  }
        	  n=ib.getLength(0);
            	      
        	  Range I(numToFix(grid),numToFix(grid)+n-1);
        	  numToFix(grid)+=n;

                    if( ia.getLength(0) < numToFix(grid) )
                  	    ia.resize(Range(0,numToFix(grid)-1),3);
        	  for( int axis=0; axis<3; axis++ )
          	    ia(I,axis)=ia2(ib,axis);
          	    
        	  if( debug & 1) 
        	  {
            	      
          	    I=Range(numberOfOrphanPoints,numberOfOrphanPoints+n-1);

          	    numberOfOrphanPoints+=n;
          	    orphanPoint.resize(Range(0,numberOfOrphanPoints-1),numberOfDimensions+1);
          	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            	      orphanPoint(I,axis)=x(ib,axis);
          	    orphanPoint(I,cg.numberOfDimensions())=grid;
          	    
          	    if( debug & 1 ) printF("check old interp pts for grid=%i: numberOfOrphanPoints=%i\n",
                                                                      grid,numberOfOrphanPoints);
	    // display(orphanPoint,"orphanPoint");
        	  }
        	  

	  // break;
      	}
	// fill-in the new interpolation values
      	interpolationCoordinates(Ra,Axes)=r(R,Axes);
	// ... finish .....	
            }

        } // end for grid2
    } // end for grid
    
    if( debug & 2 )
    {
        if( ok )
            printF("Ogen::movingUpdateNew: We *could* just use the old interpolation points ---\n");
        else
            printF("Ogen::movingUpdateNew: We could *NOT* just use the old interpolation points ---\n");
    }
    

  // *** For now we revert to the full algorithm here ***
    bool fullAlgorithmUsed=false;
    if( true && !ok )
    {
        fullAlgorithmUsed=true;

        printF("Ogen::movingUpdateNew: reverting to the full algorithm...\n");
        debug=0; info=0;  // reset these
          
        returnValue= updateOverlap(cg);
    }


    if( debug & 4 && numberOfOrphanPoints>0 )
    {
        psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
        psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);
    // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
        plot( "orphan points from original",cg);
    }
    
    bool doubleCheck=false;  // set to true if we should double check the grid is valid at the end.

    if( ok && !fullAlgorithmUsed  )
    {
    // ======================================================================
    // ======== we could use the same interpolation points. =================
    // ======================================================================

        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
            MappedGrid & c = cg[grid];

      // Fill in the interpoleeLocation -- lower left corner of the stencil
            int l=0;  // multigrid level
            #ifdef USE_PPP
                intSerialArray & interpolationPoint = cg->interpolationPointLocal[grid];
                intSerialArray & interpoleeLoc = cg->interpoleeLocationLocal[grid];
                const intSerialArray & interpoleeGrid = cg->interpoleeGridLocal[grid];
                const realSerialArray & interpolationCoord = cg->interpolationCoordinatesLocal[grid];
            #else
                intSerialArray & interpolationPoint = cg->interpolationPoint[grid];
                intSerialArray & interpoleeLoc = cg.interpoleeLocation[grid];
                const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
                const realSerialArray & interpolationCoord = cg.interpolationCoordinates[grid];
            #endif

            cg.interpolationStartEndIndex=cgOld.interpolationStartEndIndex;
      // cg.variableInterpolationWidth=cgOld.variableInterpolationWidth; // this is now done above 
        
      // ** could vectorize this I think if we put gridSpacing into an array: spacing(grid,axis)   
            for( int i=interpolationPoint.getBase(0); i<=interpolationPoint.getBound(0); i++ )
            {
      	int grid2=interpoleeGrid(i);
      	const IntegerArray & interpolationWidth = cg.interpolationWidth(Axes,grid,grid2,l);
      	MappedGrid & g2 = cg[grid2];
      	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      	{
	  // Get the lower-left corner of the interpolation cube.
        	  int intLoc=int(floor(interpolationCoord(i,axis)/g2.gridSpacing()(axis) + g2.indexRange()(0,axis) -
                         			       .5 * interpolationWidth(axis) + (g2.isCellCentered()(axis) ? .5 : 1.)));
        	  if (!g2.isPeriodic()(axis)) 
        	  {
          	    if( (intLoc < g2.extendedIndexRange()(0,axis)) && (g2.boundaryCondition()(Start,axis)>0) )
          	    {
	      //                        Point is close to a BC side.
	      //                        One-sided interpolation used.
            	      intLoc = g2.extendedIndexRange()(0,axis);
          	    }
          	    if( (intLoc + interpolationWidth(axis) - 1 > g2.extendedIndexRange()(1,axis))
            		&& (g2.boundaryCondition()(End,axis)>0) )
          	    {
	      //                        Point is close to a BC side.
	      //                        One-sided interpolation used.
            	      intLoc = g2.extendedIndexRange()(1,axis) - interpolationWidth(axis) + 1;
          	    }
        	  } // end if
        	  interpoleeLoc(i,axis) = intLoc;
      	} 
            }
        } // end for grid 
        

    //  Tell the CompositeGrid that the interpolation data have been computed:   *wdh* added 040516
        cg->computedGeometry |=
            CompositeGrid::THEmask                     |
            CompositeGrid::THEinterpolationCoordinates |
            CompositeGrid::THEinterpolationPoint       |
            CompositeGrid::THEinterpoleeLocation       |
            CompositeGrid::THEinterpoleeGrid;

    // printF("Ogen:updateOverlap for moving grids, using same interpolation points\n");
        if( info & 4 )
            printF(" time to update the geometry..............................%e (total=%e)\n",
           	     timeUpdateGeometry,timeUpdateGeometry);

        totalTime=getCPU()-totalTime;
        if( info & 2 )
            printF("Ogen::updateOverlap: Time to compute overlap = %8.2e (+ update geom=%8.2e) "
           	     "(using same interp pts)\n",
           	     totalTime, totalTime+timeUpdateGeometry);
    }
    else if( !ok && !fullAlgorithmUsed )
    {
    // ***************************************************************
    // *********** Attempt to locally fixup interpolation ************
    // ***************************************************************
        

        IntegerArray tryThisGrid(numberOfBaseGrids);

        int  jv[3], &j1 = jv[0],  &j2= jv[1], &j3 = jv[2];
        int  kv[3], &k1 = kv[0],  &k2= kv[1], &k3 = kv[2];

        int jvMax[3], &j1Max=jvMax[0], &j2Max=jvMax[1], &j3Max=jvMax[2];
        int jvMin[3], &j1Min=jvMin[0], &j2Min=jvMin[1], &j3Min=jvMin[2];

        int periodic[3]={0,0,0};
        int periodic2[3]={0,0,0};
        int period[3]={0,0,0};
        int period2[3]={0,0,0};


    // build the arrays inverseCoordinates, inverseGrid and inverseCondition to be used by the overlap algorithm
        cg.update(CompositeGrid::THEinverseMap, CompositeGrid::COMPUTEnothing);

        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
      // We need to recompute the bounding boxes -- we could optimize this for the MatrixTransform
            if( hasMoved(grid) && cg[grid].mapping().getMapping().approximateGlobalInverse!=NULL )
      	cg[grid].mapping().getMapping().approximateGlobalInverse->reinitialize(); 
        }

    // not all of the original interpolation points could be interpolated.
        IntegerArray newNum(numberOfBaseGrids);
        newNum=0;
            
    // **** fill in the inverseGrid and inverseCooridnates arrays ****
        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
            interpNew[grid].redim(maxInterp,3);   // ********************* fix ****
            MappedGrid & c = cg[grid];
            
            #ifdef USE_PPP
              const intSerialArray & ip = cg->interpolationPointLocal[grid];
              const intSerialArray & interpoleeGrid = cg->interpoleeGridLocal[grid];
              const realSerialArray & interpolationCoordinates = cg->interpolationCoordinatesLocal[grid];
              GET_LOCAL(real,cg.inverseCoordinates[grid],rI);
              GET_LOCAL(int,cg.inverseGrid[grid],inverseGrid);
            #else
              const intSerialArray & ip = cg.interpolationPoint[grid];
              const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
              const realSerialArray & interpolationCoordinates = cg.interpolationCoordinates[grid];
              realSerialArray & rI = cg.inverseCoordinates[grid];
              intSerialArray & inverseGrid = cg.inverseGrid[grid];
            #endif
            
            inverseGrid=-1;

            if( ip.getLength(0)>0 ) // *wdh* 040516
            {
      	Range R=ip.getLength(0);
      	if( numberOfDimensions==2 )
      	{
        	  i3=c.gridIndexRange(Start,axis3);
        	  inverseGrid(ip(R,0),ip(R,1),i3)=interpoleeGrid;
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
          	    rI(ip(R,0),ip(R,1),i3,axis)=interpolationCoordinates(R,axis);
      	}
      	else if( numberOfDimensions==3 )
      	{
        	  inverseGrid(ip(R,0),ip(R,1),ip(R,2))=interpoleeGrid;
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
          	    rI(ip(R,0),ip(R,1),ip(R,2),axis)=interpolationCoordinates(R,axis);
      	}
      	else
      	{
        	  Overture::abort("ERROR: unexpected numberOfDimensions");
      	}
            }
        }
        
        
        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
            MappedGrid & mg = cg[grid];
            intArray & inverseGridd = cg.inverseGrid[grid];
            realArray & rId = cg.inverseCoordinates[grid];

            const bool isRectangular = mg.isRectangular();
            if( isRectangular )
            {
      	mg.getRectangularGridParameters( dvx, xab );
      	for( int dir=0; dir<numberOfDimensions; dir++ )
      	{
        	  iv0[dir]=mg.gridIndexRange(0,dir);
        	  if( mg.isAllCellCentered() )
          	    xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      	}
            }

            intArray & maskd = mg.mask();
            intSerialArray & ia = iInterp[grid]; 
            intSerialArray & ian = interpNew[grid]; 

            OV_GET_SERIAL_ARRAY_CONDITIONAL(real,mg.center(),center,!isRectangular);
            #ifdef USE_PPP
                GET_LOCAL(int,maskd,mask);
                GET_LOCAL(int,inverseGridd,inverseGrid);
                GET_LOCAL(real,rId,rI);
            #else
      	intSerialArray & mask=maskd;
      	intSerialArray & inverseGrid=inverseGridd;
      	realSerialArray & rI=rId;
            #endif
            
            for( int axis=0; axis<numberOfDimensions; axis++ )
            {
      	periodic[axis]=mg.isPeriodic(axis);
                period[axis]=mg.gridIndexRange(End,axis)-mg.gridIndexRange(Start,axis);
            }

            real rr[3]={0.,0.,0.};
            int stencil[3][2]={0,0,0,0,0,0}; // *wdh* 040719 -- gives values to [2][0..1] in 2D
                    
            if( debug & 2 )
            {
      	for( int i=0; i<numToFix(grid); i++ )
      	{
        	  printF(" grid=%i, orphan pt %i is (%i,%i,%i)\n",grid,i,ia(i,0),ia(i,1),ia(i,2));
      	}
            }
            

            int i;
            for( i=0; i<numToFix(grid); i++ )
            {
                i1=ia(i,0); i2=ia(i,1); i3=ia(i,2);
                int grid2=inverseGrid(i1,i2,i3);
                assert( grid2>=0 && grid2<numberOfBaseGrids);
      	
	// --------------------------------------------------------------------------------
        // The strategy is to add new pts onto the higher priority grid or remove
        // pts from a lower priority grid
	// --------------------------------------------------------------------------------

      	for( int axis=0; axis<numberOfDimensions; axis++ )
        	  rr[axis]=rI(i1,i2,i3,axis);

      	MappedGrid & g2 = cg[grid2];
      	for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  periodic2[axis]=g2.isPeriodic(axis);
        	  period2[axis]=g2.gridIndexRange(End,axis)-g2.gridIndexRange(Start,axis);
      	}

	// first determine if we are outside the unit-cube of grid 2
                bool outsideGrid2 = false;
                for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  if( !periodic2[axis] )
        	  {
                          outsideGrid2 = 
                                  rI(i1,i2,i3,axis) <    (g2.extendedRange(0,axis)-g2.indexRange(0,axis)+.5)*g2.gridSpacing(axis) ||
               	         rI(i1,i2,i3,axis) > 1.+(g2.extendedRange(1,axis)-g2.indexRange(1,axis)-.5)*g2.gridSpacing(axis);
                          if( outsideGrid2 ) break;
        	  }
      	}
      	
                if( debug & 1 ) 
                    printF(" pt (%i,%i,%i) grid %i could not interp from grid2=%i r=(%7.2e,%7.2e,%7.2e) outside=%i\n",
                                i1,i2,i3,grid,grid2,
                                rI(i1,i2,i3,0),rI(i1,i2,i3,1),(numberOfDimensions==2 ? 0. : rI(i1,i2,i3,2)),outsideGrid2 );
      	


	// determine the interpolation stencil for interpolating from grid2
      	
      	computeInterpolationStencil(cg,grid,grid2,rr,stencil,useOneSidedAtBoundaries,useOddInterpolationWidth);
      	
      	for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
                    if( !periodic2[axis] )
        	  {
          	    stencil[axis][0]=max(stencil[axis][0],g2.extendedIndexRange(Start,axis));
          	    stencil[axis][1]=min(stencil[axis][1],g2.extendedIndexRange(End  ,axis));
        	  }
                    else
        	  {
          	    stencil[axis][0]=max(stencil[axis][0],g2.dimension(Start,axis));
          	    stencil[axis][1]=min(stencil[axis][1],g2.dimension(End  ,axis));
        	  }
        	  
      	}
      	
                bool ptok=false;
        // ===================================================
        // ==== Try to interpolate from a different grid =====
        // ===================================================
        // *wdh* 040719 if( true || (grid2>grid && grid2<numberOfBaseGrids) )
                if( numberOfBaseGrids>2 )  // no need to check for alternatives if we only have 2 grids
      	{
        	  tryThisGrid=true;
        	  tryThisGrid(grid)=false;  // should allow for c grids?
        	  tryThisGrid(grid2)=false;

                    #ifdef USE_PPP
         	   GET_LOCAL(int,cg.inverseGrid[grid2],inverseGrid2);
                    #else
                      intArray & inverseGrid2 = cg.inverseGrid[grid2];
                    #endif
        	  
          // loop over neighbouring points in the interpolation stencil
          // Find a different interpolee grid that we might be able to interpolate from.
        	  FORJP_STENCIL(k1,k2,k3,j1,j2,j3) // loop over k1,k2,k3 -> periodic correction (j1,j2,j3)
        	  {
//                 if( debug & 2 )
//                   fprintf(logFile,"pt (%i,%i,%i) grid %i : grid2=%i stencil: j=(%i,%i,%i) inverseGrid=%i\n",
//                        i1,i2,i3,grid,grid2,j1,j2,j3,inverseGrid2(j1,j2,j3));
            		

          	    int gridI=inverseGrid2(j1,j2,j3);  // fix me for parallel 

                        if( debug & 1 && gridI>=0 && gridI<numberOfBaseGrids )
                            printF("found possible gridI=%i\n",gridI);
            	      
          	    if( gridI>=0 && gridI<numberOfBaseGrids && tryThisGrid(gridI) )
          	    {
	      // a point in the interp stencil interpolates from another grid
	      // attempt to interpolate from this other grid
	      // assert( gridI<numberOfBaseGrids );
              		  
            	      if( debug & 1 ) printF("try gridI=%i,",gridI);
              		  
            	      Mapping & mapI = cg[gridI].mapping().getMapping();
                  
            	      realSerialArray r(1,3),x(1,3);

            	      ia2(0,Axes)=ia(i,Axes);
            	      if( isRectangular )
            	      {
                                i1=ia(i,0); i2=ia(i,1); i3=ia(i,2);
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  x(0,axis)=XC(iv,axis);
            	      }
            	      else
            	      {
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  x(0,axis)=center(ia(i,0),ia(i,1),ia(i,2),axis);
            	      }

            	      if( useBoundaryAdjustment )
            		adjustBoundary(cg,grid,gridI,ia2(0,Axes),x(0,Axes)); 

            	      r=-1;
                            #ifdef USE_PPP
             	       mapI.inverseMapS(x,r);
                            #else
             	       mapI.inverseMap(x,r);
                            #endif

            	      interpolates(0)=true;  
                            #ifdef USE_PPP
              	        ptok = checkCanInterpolate(cg,grid,gridI, r, interpolates,useBackupRules );
                            #else
              	        ptok = cg.rcData->canInterpolate(grid,gridI, r, interpolates,useBackupRules,checkForOneSided );
                            #endif
            	      if( ptok )
            	      {
            		if( debug & 1 ) printF(" ...ok, can interp from a different grid: %i \n",gridI);
            		if( debug & 1 ) printF(" ...-> mask(%i,%i,%i)=%i\n",i1,i2,i3,mask(i1,i2,i3));
            		
            		inverseGrid(ia(i,0),ia(i,1),ia(i,2))=gridI;
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  rI(i1,i2,i3,axis)=r(0,axis);

            		k2=stencil[1][1]+1;  // this will cause us to break out of the triple loop
            		k3=stencil[2][1]+1;
            		break;
            	      }
            	      else
            	      {
            		if( debug & 1 ) printF(" ...not ok\n");
            		tryThisGrid(gridI)=false;
                                if( outsideGrid2 )
            		{
              		  MappedGrid & gI = cg[gridI];
              		  bool outsideGridI = false;
              		  for( int axis=0; axis<numberOfDimensions; axis++ )
              		  {
                		    if( !gI.isPeriodic(axis) )
                		    {
                  		      outsideGridI = 
                  			r(0,axis) <    (gI.extendedRange(0,axis)-gI.indexRange(0,axis)+.5)*gI.gridSpacing(axis) ||
                  			r(0,axis) > 1.+(gI.extendedRange(1,axis)-gI.indexRange(1,axis)-.5)*gI.gridSpacing(axis);
                  		      if( outsideGridI ) break;
                		    }
              		  }
              		  if( !outsideGridI )
              		  {
                                        if( debug & 1 ) printF("...not ok but inside. Use this as new best guess\n");
                		    inverseGrid(ia(i,0),ia(i,1),ia(i,2))=gridI;
                		    for( int axis=0; axis<numberOfDimensions; axis++ )
                  		      rI(i1,i2,i3,axis)=r(0,axis);
              		  }
            		}
            	      }
          	    }
        	  }
                    ENDFORJP_STENCIL
      	}
                if( ptok )
                    continue;

        // grid2 may have changed
                if( grid2!=inverseGrid(i1,i2,i3) )
      	{
        	  grid2=inverseGrid(i1,i2,i3); // **** watch out! g2 is nolonger valid
        	  assert( grid2>=0 && grid2<numberOfBaseGrids);
                    MappedGrid & g2New = cg[grid2];
        	  
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
          	    rr[axis]=rI(i1,i2,i3,axis);

        	  for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    periodic2[axis]=g2New.isPeriodic(axis);
          	    period2[axis]=g2New.gridIndexRange(End,axis)-g2New.gridIndexRange(Start,axis);
        	  }

        	  computeInterpolationStencil(cg,grid,grid2,rr,stencil,useOneSidedAtBoundaries,useOddInterpolationWidth ); 
          
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
        	  {
          	    if( !periodic2[axis] )
          	    {
            	      stencil[axis][0]=max(stencil[axis][0],g2New.extendedIndexRange(Start,axis));
            	      stencil[axis][1]=min(stencil[axis][1],g2New.extendedIndexRange(End  ,axis));
          	    }
          	    else
          	    {
            	      stencil[axis][0]=max(stencil[axis][0],g2New.dimension(Start,axis));
            	      stencil[axis][1]=min(stencil[axis][1],g2New.dimension(End  ,axis));
          	    }
        	  
        	  }
      	}
      	

        // **** if outside an interp boundary then increase overlap,
        //      if outside a physical boundary then decrease overlap
      	int expand=true;
      	int sideb=-1,axisb=-1;
      	for( int axis=0; axis<numberOfDimensions; axis++ )
      	{
        	  if( rI(i1,i2,i3,axis) < -.0001 ) // -boundaryEps*100. )
        	  {
          	    sideb=0; axisb=axis; break;
        	  }
        	  else if( rI(i1,i2,i3,axis)>1.0001 ) // 1.+boundaryEps*100.  )
        	  {
          	    sideb=1; axisb=axis; break;
        	  }
      	}
      	if( sideb>=0 )
      	{
        	  if( cg[grid2].boundaryCondition(sideb,axisb)>0 )
        	  {
          	    expand=false; // outside a physical boundary
        	  }
      	}

                for( int axis=0; axis<3; axis++ )
      	{
                    int width=cg.interpolationWidth(axis,grid,grid2)/2;
        	  if( !periodic[axis] )
        	  {
          	    jvMin[axis]=max(iv[axis]-width,mg.extendedIndexRange(Start,axis));
          	    jvMax[axis]=min(iv[axis]+width,mg.extendedIndexRange(End  ,axis));
        	  }
        	  else
        	  {
          	    jvMin[axis]=max(iv[axis]-width,mg.dimension(Start,axis));
          	    jvMax[axis]=min(iv[axis]+width,mg.dimension(End  ,axis));
        	  }
        	  
      	}
      	

                if( grid2>=grid ) // && !canExpandHigherPriorityGrid )
      	{
	  // interpolation failed from a higher priority grid
      	

                    if( expand )
        	  {
	    // ---- increase the amount of overlap ------
	    // move interpolation boundary back -- try to turn pt into a discretization pt.
    
          	    FORJP(k1,k2,k3,j1,j2,j3)
          	    {
            	      if( !mask(j1,j2,j3) )
            	      {
		// make this an interpolation point
            		mask(j1,j2,j3)=MappedGrid::ISinterpolationPoint;
            		if( debug & 1 ) 
              		  printF("   make pt (%i,%i,%i) grid=%i a new interp pt (increase)\n",j1,j2,j3,grid);
              		  
            		int j=newNum(grid);
            		ian(j,axis1)=j1;
            		ian(j,axis2)=j2;
            		ian(j,axis3)=j3;
            		inverseGrid(j1,j2,j3)=grid2;         // a guess
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  rI(j1,j2,j3,axis)=rI(i1,i2,i3,axis); // a guess
            		newNum(grid)++;

            	      }
          	    }
                        ENDFORJP
        	  }
        	  else
        	  {
	    // decrease the amount of overlap **** this is dangerous *** could invalidate other points.
                        doubleCheck=true;
          	    
          	    FORJP(k1,k2,k3,j1,j2,j3)
          	    {
            	      if( mask(j1,j2,j3) > 0 ) // turn discretization points into interpolation
            	      {
		// make this an interpolation point
            		mask(j1,j2,j3)=MappedGrid::ISinterpolationPoint;
            		if( debug & 1 ) 
              		  printF("   make pt (%i,%i,%i) grid=%i a new interp pt (decrease).\n",j1,j2,j3,grid);
              		  
            		int j=newNum(grid);
            		ian(j,axis1)=j1;
            		ian(j,axis2)=j2;
            		ian(j,axis3)=j3;
            		inverseGrid(j1,j2,j3)=grid2;         // a guess
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  rI(j1,j2,j3,axis)=rI(i1,i2,i3,axis); // a guess
            		newNum(grid)++;

            	      }
                        }
          	    ENDFORJP      
          	    mask(i1,i2,i3)=0;  // remove this point
                        if( debug & 1 ) printF("  remove pt (%i,%i,%i) on grid %i\n",i1,i2,i3,grid);
          	    
        	  }
        	  
      	}
      	else // grid2 < grid 
      	{
	  // ========== interpolation failed from a lower priority grid  =============
          //    --> fill in points into the interpolation stencil to make it valid.
          // or --> remove points from the higher priority grid

        	  if( expand )
        	  {
          	    int j=newNum(grid);
          	    for( int axis=0; axis<3; axis++ )
            	      ian(j,axis)=ia(i,axis);
          	    newNum(grid)++;

	    // computeInterpolationStencil(cg,grid,grid2,rr,stencil,true ); // recompute with one-sided on at bndries

	    // bool ptAdded=false;

            // *********** fix me for parallel ***********

          	    intSerialArray & ian2 = interpNew[grid2];
                        #ifdef USE_PPP
            	      GET_LOCAL(int,cg[grid2].mask(),mask2);
            	      GET_LOCAL(int,cg.inverseGrid[grid2],inverseGrid2);
                        #else
            	      intSerialArray & mask2 = cg[grid2].mask();
            	      intSerialArray & inverseGrid2 = cg.inverseGrid[grid2];
                        #endif

                        if( debug & 4 )
                              printF(" ...expand interp pts: stencil=[%i,%i][%i,%i][%i,%i]\n",stencil[0][0],stencil[0][1],
                                stencil[1][0],stencil[1][1],stencil[2][0],stencil[2][1]);
          	    
          	    FORJP_STENCIL(k1,k2,k3,j1,j2,j3)
          	    {
            	      if( !mask2(j1,j2,j3) )
            	      {
		// make this an interpolation point
		// ptAdded=true;
            		mask2(j1,j2,j3)=MappedGrid::ISinterpolationPoint;
            		if( debug & 1 ) 
              		  printF("   make pt (%i,%i,%i) grid2=%i a new interp pt (grid=%i)\n",j1,j2,j3,grid2,grid);
            		int j=newNum(grid2);
            		for( int axis=0; axis<3; axis++ )
              		  ian2(j,axis)=jv[axis];

            		inverseGrid2(j1,j2,j3)=grid;         // a guess
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  cg.inverseCoordinates[grid2](j1,j2,j3,axis)=0.;     // a guess

            		newNum(grid2)++;
            	      }
          	    }
          	    ENDFORJP_STENCIL;


        	  }
        	  else
        	  {
            // this section is copied from above -- fix --
	    // decrease the amount of overlap **** this is dangerous *** could invalidate other points.
                        doubleCheck=true;
          	    
          	    FORJP(k1,k2,k3,j1,j2,j3)
          	    {
            	      if( mask(j1,j2,j3) > 0 ) // turn discretization points into interpolation
            	      {
		// make this an interpolation point
            		mask(j1,j2,j3)=MappedGrid::ISinterpolationPoint;
            		if( debug & 1 ) 
              		  printF("   make pt (%i,%i,%i) grid=%i a new interp pt (decrease).\n",j1,j2,j3,grid);
              		  
            		int j=newNum(grid);
            		ian(j,axis1)=j1;
            		ian(j,axis2)=j2;
            		ian(j,axis3)=j3;
            		inverseGrid(j1,j2,j3)=grid2;         // a guess
            		for( int axis=0; axis<numberOfDimensions; axis++ )
              		  rI(j1,j2,j3,axis)=rI(i1,i2,i3,axis); // a guess
            		newNum(grid)++;

            	      }
          	    } // for( j3
          	    ENDFORJP      
          	    mask(i1,i2,i3)=0;  // remove this point
                        if( debug & 1 ) printF("  remove pt (%i,%i,%i) on grid %i\n",i1,i2,i3,grid);

        	  }
        	  
      	}
      	
            }  // for( i )

      // **** problem is here : why do we do this *****
            for( int i=0; i<numToFix(grid); i++ )
            {
                if( inverseGrid(ia(i,0),ia(i,1),ia(i,2)) > grid && mask(ia(i,0),ia(i,1),ia(i,2)) )
      	{
        	  i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
        	  if( canDiscretize(cg[grid], iv) ) // *wdh* 040806 
        	  {
          	    mask(ia(i,0),ia(i,1),ia(i,2))=MappedGrid::ISdiscretizationPoint;
          	    if( debug & 1 ) printF("  make a discr. pt: (%i,%i,%i) on grid %i\n",ia(i,0),ia(i,1),ia(i,2),grid);  
        	  }
        	  
      	}
            }
            

            
        }  // for grid

        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
            cg[grid].mask().periodicUpdate();
        }

    // *****************************************************
    // *** now check the points that remain in the list ****
    // *****************************************************

        realSerialArray r(1,3),x(1,3);
        ok=true;
        int numberOfExtraInterpolationPoints=0;
        numberOfOrphanPoints=0;
        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
      // displayMask(cg[grid].mask(),"mask",logFile);
            
            intSerialArray & ia = interpNew[grid];

            MappedGrid & c = cg[grid];
            const bool isRectangular = c.isRectangular();
            if( isRectangular )
            {
      	c.getRectangularGridParameters( dvx, xab );
      	for( int dir=0; dir<c.numberOfDimensions(); dir++ )
      	{
        	  iv0[dir]=c.gridIndexRange(0,dir);
        	  if( c.isAllCellCentered() )
          	    xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      	}
            		
            }
            OV_GET_SERIAL_ARRAY_CONDITIONAL(real,c.center(),center,!isRectangular);
            #ifdef USE_PPP      
                GET_LOCAL(int,cg[grid].mask(),mask);
                GET_LOCAL(int,cg.inverseGrid[grid],inverseGrid);
                GET_LOCAL(real,cg.inverseCoordinates[grid],rI);
            #else
                intSerialArray & mask = cg[grid].mask();
                intSerialArray & inverseGrid = cg.inverseGrid[grid];
                realSerialArray & rI = cg.inverseCoordinates[grid];
            #endif

            for( int i=0; i<newNum(grid); i++ )
            {
                int grid2=inverseGrid(ia(i,0),ia(i,1),ia(i,2));
      	
                if( debug & 1 ) 
                    printF(" New pts to check: grid=%i pt=(%i,%i,%i) grid2=%i",grid,ia(i,0),ia(i,1),ia(i,2),grid2);

                assert( grid2>=0 && grid2<numberOfBaseGrids);
      	
        // check canInterpolate
      	Mapping & map2 = cg[grid2].mapping().getMapping();

      	if( isRectangular )
      	{
        	  i1=ia(i,0); i2=ia(i,1); i3=ia(i,2);
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
          	    x(0,axis)=XC(iv,axis);
      	}
      	else
      	{
        	  for( int axis=0; axis<numberOfDimensions; axis++ )
          	    x(0,axis)=center(ia(i,0),ia(i,1),ia(i,2),axis);
      	}

                ia2(0,Axes)=ia(i,Axes);
                if( useBoundaryAdjustment )
        	  adjustBoundary(cg,grid,grid2,ia2(0,Axes),x(0,Axes)); 

      	r=-1;
                #ifdef USE_PPP
       	 map2.inverseMapS(x,r);
                #else
       	 map2.inverseMap(x,r);
      	#endif

      	interpolates(0)=true;  

                #ifdef USE_PPP
        	  int ptok = checkCanInterpolate(cg,grid,grid2, r, interpolates, useBackupRules );
                #else
        	  int ptok = cg.rcData->canInterpolate(grid,grid2, r, interpolates, useBackupRules, checkForOneSided );
                #endif
                assert( ptok==interpolates(0) );
      	
                if( ptok )
      	{
                    if( debug & 1 ) printF("...ok\n");
                    for( int axis=0; axis<numberOfDimensions; axis++ )
          	    rI(ia(i,0),ia(i,1),ia(i,2),axis)=r(0,axis);
      	}
      	else
      	{
        	  if( debug & 1 ) printF("...not ok x=(%8.2e,%8.2e) r=(%8.2e,%8.2e), ",x(0,0),x(0,1),r(0,0),r(0,1));
          // try to interpolate from another grid
                    for( int gridI=0; gridI<numberOfBaseGrids; gridI++ )
        	  {
          	    if( gridI==grid || gridI==grid2 ) continue;
          	    Mapping & mapI = cg[gridI].mapping().getMapping();
                  
                        ia2(0,Axes)=ia(i,Axes);
          	    if( useBoundaryAdjustment )
            	      adjustBoundary(cg,grid,gridI,ia2(0,Axes),x(0,Axes)); 

          	    r=-1;
                        #ifdef USE_PPP
            	      mapI.inverseMapS(x,r);
                        #else
            	      mapI.inverseMap(x,r);
                        #endif
          	    interpolates(0)=true;  
                        #ifdef USE_PPP
            	      ptok = checkCanInterpolate(cg,grid,gridI, r, interpolates, useBackupRules );
                        #else
            	      ptok = cg.rcData->canInterpolate(grid,gridI, r, interpolates, useBackupRules, checkForOneSided );
                        #endif
                        if( ptok )
          	    {
            	      if( debug & 1 ) printF("...ok, can interp from a different grid: %i \n",gridI);
                            inverseGrid(ia(i,0),ia(i,1),ia(i,2))=gridI;
            	      for( int axis=0; axis<numberOfDimensions; axis++ )
            		rI(ia(i,0),ia(i,1),ia(i,2),axis)=r(0,axis);

                            numberOfExtraInterpolationPoints++;
            	      
            	      break;
          	    }
        	  }
        	  if( !ptok )
        	  {
          	    if( debug & 1 ) printF("...not ok x=(%8.2e,%8.2e) r=(%8.2e,%8.2e)\n",x(0,0),x(0,1),r(0,0),r(0,1));
  
          	    if( orphanPoint.getLength(0)<numberOfOrphanPoints+1 )
            	      orphanPoint.resize(Range(0,numberOfOrphanPoints+20),Axes);
          	    for( int axis=0; axis<numberOfDimensions; axis++ )
            	      orphanPoint(numberOfOrphanPoints,axis)=x(0,axis);
          	    numberOfOrphanPoints++;
        	  }
      	}

                i1=ia(i,0), i2=ia(i,1), i3=ia(i,2);
      	
                if( !ptok && canDiscretize(cg[grid], iv) )
      	{
        	  if( debug & 1 ) 
          	    printF(" ...pt (%i,%i,%i) grid=%i can be used as a discretization pt\n",i1,i2,i3,grid);
                    ptok=true;
                    mask(ia(i,0),ia(i,1),ia(i,2))=MappedGrid::ISdiscretizationPoint;
        	  
      	}


                ok = ok && ptok;
      	
            }  // end for i

        }  // for for grid
        
        if( ok )
        {
            numberOfOrphanPoints=0;
            IntegerArray numberOfInterpolationPoints(numberOfBaseGrids);
            for( int grid=0; grid<numberOfBaseGrids; grid++ )
            {
                numberOfInterpolationPoints(grid)=cg.numberOfInterpolationPoints(grid)+
                            newNum(grid)-numToFix(grid)+numberOfExtraInterpolationPoints;
        // *wdh* 080531 -- we need extra for some reason, maybe periodic boundaries ??  ---- fix this --------
      	const int extraInterp = cg.numberOfDimensions()==2 ? 100 : 1000; 
                iInterp[grid].redim(numberOfInterpolationPoints(grid)+extraInterp,3); // why are extra needed ???
            }
            
            if( debug & 1 )
            {
                display(numberOfInterpolationPoints,"numberOfInterpolationPoints");
        // for( grid=0; grid<numberOfBaseGrids; grid++ )
        //  displayMask(cg[grid].mask(),"mask");
            }
            
            generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );

      // --- For each used point on the boundary, mark its ghost points. ---
      // *wdh* 090804 -- we need to mark the mask at ghost points (e.g. cgins slide example)
      // We could be more selective here but do this for now:
            markMaskAtGhost( cg );


      // cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");

      // printF("Ogen:updateOverlap for moving grids, using same interpolation points\n");
            if( info & 2 )
      	printF(" time to update the geometry..............................%e (total=%e)\n",
             	       timeUpdateGeometry,timeUpdateGeometry);

            totalTime=getCPU()-totalTime;
            if( info & 2 )
      	printF("Ogen::updateOverlap: Time to compute overlap = %8.2e (+ update geom.=%8.2e) "
             	       "(using optimized algorithm)\n",
             	       totalTime, totalTime+timeUpdateGeometry);


            if( debug & 2 ) // ******* double check *****
            {
      	int numberOfErrors=checkOverlappingGrid(cg);
      	if( true || debug & 2 || numberOfErrors!=0 )
      	{
        	  printF("movingUpdate::after optimized algorithm: check grid... numberOfErrors=%i \n",numberOfErrors);
      	}
      	
            }

            if( debug & 4 && ok )
            {
        // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

      	psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
                psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);
      	plot( "Optimized overlap a success!",cg);
            }
            
        }
        if( !ok )
        {
      // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
            
            if( debug & 2 )
            {
      	psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
      	psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);
      	plot( "Optimized overlap algorithm failed",cg);
      	aString answer;
	// ps->inputString(answer,"prompt");
            }
            printF("Ogen::optimized overlap algorithm failed. Will retry with full algorithm\n");

            cg.interpolationWidth(Rx,G,G,level)=interpolationWidth;  // reset
            cg.interpolationOverlap(Rx,G,G,0)=interpolationOverlap;

            returnValue= updateOverlap(cg);

            
        }

        
    } // !ok 
    

    
  // reset values
    cg.interpolationWidth(Rx,G,G,level)=interpolationWidth;
    cg.interpolationOverlap(Rx,G,G,0)=interpolationOverlap;


    if( true || doubleCheck || debug & 2 )
    {
        int numberOfErrors=checkOverlappingGrid(cg);
        if( debug & 2 || numberOfErrors!=0 )
            printF("movingUpdate::check grid... (doubleCheck=%i) numberOfErrors=%i \n",doubleCheck,numberOfErrors);
        if( numberOfErrors!=0 )
        {

            if( debug & 2 )
            {
                debug=3;
      	psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
      	psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);
      	plot( "Optimized overlap algorithm failed",cg);
            }

            if( debug & 2 )
            {
      	for( int grid=0; grid<numberOfBaseGrids; grid++ )
      	{
        	  displayMask(cg[grid].mask(),"mask");
      	}
            }

            printF("Ogen::optimized overlap algorithm failed in checkOverlappingGrid. Will retry with full algorithm\n");

            returnValue= updateOverlap(cg);


        }
      	
    }

  // *wdh* 040504 -- don't forget we have refinement levels
    if( computedGeometry0 & CompositeGrid::THErefinementLevel ) 
        cg->computedGeometry |= CompositeGrid::THErefinementLevel;

    delete [] iInterp;   iInterp=NULL;
    delete [] interpNew; interpNew=NULL;
    delete [] backupValues; backupValues=NULL;

    debug=0; info=0;  // reset these *******************************

    return returnValue;  

// #endif
}
