#include "Overture.h"
#include "Ogen.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
#include "conversion.h"
#include "display.h"
#include "ParallelUtility.h"

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
 #define GET_LOCAL_CONST(type,xd,xs)\
    type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
 #define GET_LOCAL(type,xd,xs)\
    type ## SerialArray & xs = xd
 #define GET_LOCAL_CONST(type,xd,xs)\
    const type ## SerialArray & xs = xd
#endif

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )


int Ogen::
computeOverlap(CompositeGrid & cg_, 
               CompositeGrid & cgOld,
               const int & level /* =0 */,
	       const bool & movingGrids /* =FALSE */, 
	       const IntegerArray & hasMoved /* = Overture::nullIntArray() */ )
// ==============================================================================================
// /Description:
//    Compute the overlap between grids.
// /level (input) : multigrid level to compute
// /Return value: 
// \begin{itemize}
//    \item 0=succuss.
//    \item  >0 : ERROR, the number of errors encountered.
//    \item  -1 : non fatal errors occured, algorithm completed but the grid did not pass all the
//        tests in checkOverlappingGrid.
//    \item -resetTheGrid : 
// \end{itemize} : algorithm was aborted, restart.
// ==============================================================================================
{
  assert( ps!=NULL );

  GenericGraphicsInterface & gi = *ps;

  CompositeGrid & cg = cg_.numberOfMultigridLevels()==1 ? cg_ : cg_.multigridLevel[level];
  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  isMovingGridProblem= isMovingGridProblem || movingGrids;

  if( isNew.getLength(0)!=numberOfBaseGrids )
  {
    isNew.redim(numberOfBaseGrids);  // this grid is in the list of new grids 
    isNew=TRUE;
  }
  
  if( plotHolePoints.getLength(0)<numberOfBaseGrids ) // *wdh* 030620 needed if not called interactively
  {
    plotHolePoints.redim(numberOfBaseGrids);
    plotHolePoints=1;
  }

  if( info & 4 )
    printF("Find interpolation points on boundaries..\n");
  psp.set(GI_PLOT_INTERPOLATION_POINTS,true);


  // In parallel, check that we have enough parallel ghost lines.
  checkParallelGhostWidth( cg );

  int plotReturnValue;
  
  checkInterpolationOnBoundaries(cg);
  if( Ogen::debug & 1 )
  {
    intSerialArray *iInterp = new intSerialArray [ numberOfBaseGrids ];  // **** fix this ****
    IntegerArray numberOfInterpolationPoints(numberOfBaseGrids);
    for( int grid=0; grid<numberOfBaseGrids; grid++ )
    {
      const IntegerArray & extended = cg[grid].extendedRange();
      int dim=(extended(1,0)-extended(0,0)+1)*(extended(1,1)-extended(0,1)+1)*(extended(1,2)-extended(0,2)+1);
      iInterp[grid].redim(dim,3);
    }
    generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );  // need iInterp
    delete [] iInterp;

    plotReturnValue=plot( "After check interpolation on boundaries",cg );
    if( plotReturnValue!=0 )
      return  -abs(plotReturnValue);
  }

//   if( true )  // *wdh* 2012/06/18
//     return 0; 


  // if( true && numberOfBaseGrids>7 ) // TEMP
  // {
  //   int grid=7, i1=170,i2=52,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Before CUT HOLES: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }
  // if( true && numberOfBaseGrids>9 ) // TEMP
  // {
  //   int grid=9, i1=158,i2=43,i3=0;
  //   realArray & rI=cg.inverseCoordinates[grid];
  //   int donor = cg.inverseGrid[grid](i1,i2,i3); 
  //   printF("Before CUT HOLES: grid(target)=%i donor=%i rI(%i,%i,%i,0)=(%e,%e,%e) mask=%i\n",grid,donor,
  //          i1,i2,i3,rI(i1,i2,i3,0),rI(i1,i2,i3,1),rI(i1,i2,i3,2),cg[grid].mask()(i1,i2,i3));
  // }

  if( info & 4 ) printF("cut holes...\n");

  if( holeCuttingOption==0 )
    numberOfHolePoints=cutHoles(cg);  // oldest 
  else if( holeCuttingOption==1 )
    numberOfHolePoints=cutHolesNew(cg);  // current default version
  else 
    numberOfHolePoints=cutHolesNewer(cg);  // latest parallel version

  //  if( true )  // *wdh* 2012/06/18 -------------------------------------------------------------------------------
  //   return 0; 

  if( info & 4 ) printF("number Of hole points = %i \n",numberOfHolePoints);
  if( Ogen::debug & 1 )
  {
    intSerialArray *iInterp = new intSerialArray [ numberOfBaseGrids ];  // **** fix this ****
    IntegerArray numberOfInterpolationPoints(numberOfBaseGrids);
    for( int grid=0; grid<numberOfBaseGrids; grid++ )
    {
      const IntegerArray & extended = cg[grid].extendedRange();
      int dim=(extended(1,0)-extended(0,0)+1)*(extended(1,1)-extended(0,1)+1)*(extended(1,2)-extended(0,2)+1);
      iInterp[grid].redim(dim,3);
    }
    generateInterpolationArrays(cg,numberOfInterpolationPoints,iInterp );  // need iInterp
    delete [] iInterp;
    
    plotReturnValue=plot( "After cut holes",cg);
    if( plotReturnValue!=0 )
      return  -abs(plotReturnValue);
  }
  
  if( incrementalHoleSweep<=0  )
  {
    if( info & 4 ) printF("remove exterior points...\n");
    numberOfHolePoints=removeExteriorPointsNew(cg,TRUE);

    if( info & 4 ) printF("number of exterior points = %i \n",numberOfHolePoints);
    if( Ogen::debug & 1 )
    {
      plotReturnValue=plot("After remove exterior points",cg);
      if( plotReturnValue!=0 )
	return  -abs(plotReturnValue);
    }
  }
  else
  {
    int numCut=1;
    while( numCut>0 )
    {
      numCut=sweepOutHolePoints(cg);
      printF(" %i new hole points generated by the incremental sweep\n",numCut);
      if( Ogen::debug & 1 )
      {
	plotReturnValue=plot("After marking more holes",cg);
	if( plotReturnValue!=0 )
	  return  -abs(plotReturnValue);
      }
    }
    
  }
  
//  -- VERY small leak to here  maybe
//  if( true )  // *wdh* 2012/06/18 ----------------------------------------------------------------------------
//    return 0; 

  numberOfHolePoints=0;
  int numberOfErrors=classifyPoints( cg,orphanPoint,numberOfOrphanPoints,level,cg_ );
  fflush(NULL);  // flush all output streams (including the log file).

//   if( true )  // *wdh* 2012/06/18 ------------------------------------------------------------
//     return 0; 

  // finish me : int totalNumberOfErrors=ParallelUtility::getSum(numberOfErrors);
  if( numberOfErrors!=0 )
  {
    printF("=====================================================================================================\n"
	   " The overlap algorithm failed, numberOfErrors=%i. Check the file ogen.log for more info.\n"
           " There were %i `orphan points' that could not be interpolated. These are shown as larger square \n"
           " marks. Turn off the interp. pts or change the colour of the orphan pts to see them better.\n"
	   "=====================================================================================================\n",
             numberOfErrors,numberOfOrphanPoints);

    gi.stopReadingCommandFile();

    if( outputGridOnFailure )
    {
      aString gridFileName="gridFailed.hdf", gridName="gridFailed";
      printF("Saving the current grids in the file %s (option outputGridOnFailure=true)\n",(const char*)gridFileName);
      printF("To see what went wrong, use ogen to generate this grid using the commands: \n"
             "ogen\ngenerate an overlapping grid\n read in a old grid\n  %s\n  reset grid"
             "\n display intermediate results\n compute overlap\n",
             (const char*)gridFileName); 
      saveGridToAFile( cg,gridFileName,gridName );
    }
    if( abortOnAlgorithmFailure )
    {
      printF("Ogen:I am going to abort now since the option abortOnAlgorithmFailure=true\n");
      Overture::abort("error in generating a grid with Ogen");
    }

    if( isMovingGridProblem )
    {
      // **** redo this section ****


      if( false )
      { // old way, this was confusing 
	debug=1;
	printF("Ogen:computeOverlap: debug mode is on\n");
	printF(" **** I am going to redo the grid computation so you can step through the grid computation\n");
	while( numberOfErrors!=0 )
	{
	  resetGrid(cg);
          // NOTE: this is a recursive call: 
	  numberOfErrors = computeOverlap( cg,cgOld,level,isMovingGridProblem,hasMoved );  
      
	}
      }
      else
      {
        // new way: 
	printF(" **** Entering the interactive update so you can step through the grid computation\n");
        // turn these off so we don't recursively call computeOverlap:
        isMovingGridProblem=false; 
	outputGridOnFailure=false;
	MappingInformation mapInfo;
	mapInfo.graphXInterface=NULL; // This means that we do not provide Mappings 
	numberOfErrors = updateOverlap( cg, mapInfo );
      }
      
    }
    else
    {
      plotReturnValue=plot( "Overlap algorithm failed",cg);

      if( plotReturnValue!=0 )
	return  -abs(plotReturnValue);
    }
    
  }
  else
  {
    if( debug & 1 || info & 4 ) printF("Checking validity of the overlapping grid...\n");
    if( info & 2 )
      Overture::printMemoryUsage("Ogen:Before checking validity of the overlapping grid");

    int option=2; // check that discretization points are valid 
    if( doubleCheckInterpolation )  // this is for parallel 
      option |= 8;

      numberOfErrors=checkOverlappingGrid(cg,option);
    
    if( info & 2 )
      Overture::printMemoryUsage("Ogen:After checking validity of the overlapping grid");

    totalTime=getCPU()-totalTime;
    if( numberOfErrors==0 )
    {
      if( debug & 1 || info & 4 ) printF("Overlapping grid is valid.\n");
    }
    else
    {
      printF("Checking validity of the overlapping grid, Grid is not valid! Number of errors=%i\n",numberOfErrors);
      numberOfErrors=-1;

      gi.stopReadingCommandFile();
    }

    const int np= max(1,Communication_Manager::numberOfProcessors());

    Overture::checkMemoryUsage("Ogen:Before printing statistics");
    
    real mem=0., maxMem=0., minMem=0., totalMem=0., aveMem=0., maxMemRecorded=0., totalMaxMem=0.;
    real 
      aveTimeUpdateGeometry=0., 
      aveTimeInterpolateBoundaries=0., 
      aveTimeCutHoles=0., 
      aveTimeRemoveExteriorPoints=0., 
      aveTimeImproperInterpolation=0., 
      aveTimeProperInterpolation=0., 
      aveTimeAllInterpolation=0., 
      aveTimeImproveQuality=0., 
      aveTimeRemoveRedundant=0., 
      aveTotalTime=0.;
    

    if( info & 2 )
    {
      mem=Overture::getCurrentMemoryUsage();
      maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
      minMem=ParallelUtility::getMinValue(mem);  // min over all processors
      totalMem=ParallelUtility::getSum(mem);     // sum of all processors
      aveMem=totalMem/np;
      real maxMemUsage=Overture::getMaximumMemoryUsage();
      maxMemRecorded=ParallelUtility::getMaxValue(maxMemUsage);
      totalMaxMem=ParallelUtility::getSum(maxMemUsage);

      aveTimeUpdateGeometry=ParallelUtility::getSum(timeUpdateGeometry)/np;
      aveTimeInterpolateBoundaries=ParallelUtility::getSum(timeInterpolateBoundaries)/np;
      aveTimeCutHoles=ParallelUtility::getSum(timeCutHoles)/np;
      aveTimeRemoveExteriorPoints=ParallelUtility::getSum(timeRemoveExteriorPoints)/np;
      aveTimeImproperInterpolation=ParallelUtility::getSum(timeImproperInterpolation)/np;
      aveTimeProperInterpolation=ParallelUtility::getSum(timeProperInterpolation)/np;
      aveTimeAllInterpolation=ParallelUtility::getSum(timeAllInterpolation)/np;
      aveTimeImproveQuality=ParallelUtility::getSum(timeImproveQuality)/np;
      aveTimeRemoveRedundant=ParallelUtility::getSum(timeRemoveRedundant)/np;
      aveTotalTime=ParallelUtility::getSum(totalTime)/np;

      timeUpdateGeometry=ParallelUtility::getMaxValue(timeUpdateGeometry);
      timeInterpolateBoundaries=ParallelUtility::getMaxValue(timeInterpolateBoundaries);
      timeCutHoles=ParallelUtility::getMaxValue(timeCutHoles);
      timeRemoveExteriorPoints=ParallelUtility::getMaxValue(timeRemoveExteriorPoints);
      timeImproperInterpolation=ParallelUtility::getMaxValue(timeImproperInterpolation);
      timeProperInterpolation=ParallelUtility::getMaxValue(timeProperInterpolation);
      timeAllInterpolation=ParallelUtility::getMaxValue(timeAllInterpolation);
      timeImproveQuality=ParallelUtility::getMaxValue(timeImproveQuality);
      timeRemoveRedundant=ParallelUtility::getMaxValue(timeRemoveRedundant);
      totalTime=ParallelUtility::getMaxValue(totalTime);


    }
    

    for( int outfile=0; outfile<=2; outfile++ )
    { // write this info to the checkFile and to stdout if appropriate
      FILE * file;
      if( outfile==0 )
	file=checkFile;
      else if( info & 2 )
      {
	file= outfile==1 ? stdout : logFile;
      }
      else
	break;
      Index I1,I2,I3;
      doubleLengthInt totalNumberOfDiscretizationPoints=0;
      doubleLengthInt totalNumberOfInterpolationPoints=0;
      doubleLengthInt totalNumberOfPoints=0;
      doubleLengthInt totalBackup=0;
      for( int grid=0; grid<numberOfBaseGrids; grid ++ )
      {
	getIndex(extendedGridRange(cg[grid]),I1,I2,I3);
        const doubleLengthInt allPoints=(doubleLengthInt(I1.getLength())*
					 doubleLengthInt(I2.getLength())*
                                         doubleLengthInt(I3.getLength()));

        intArray & maskd =cg[grid].mask();
        GET_LOCAL_CONST(int,maskd,mask);
	bool ok=ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3);
	doubleLengthInt numberOfDiscretizationPoints=0, numberOfBackupPoints=0;
	if( ok )	
	{
	  int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
	  const int maskDim0=mask.getRawDataSize(0);
	  const int maskDim1=mask.getRawDataSize(1);
          #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
          int i1,i2,i3;
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
            if( MASK(i1,i2,i3)>0 )
	      numberOfDiscretizationPoints++;
	    if( (MASK(i1,i2,i3) & CompositeGrid::USESbackupRules)!=0 )
	      numberOfBackupPoints++;
	  }
	}
	
	numberOfDiscretizationPoints=ParallelUtility::getSum(numberOfDiscretizationPoints);
	numberOfBackupPoints=ParallelUtility::getSum(numberOfBackupPoints);
	
        if( outfile==1 && numberOfBackupPoints>0 )
          printF(" grid=%i number of backup interpolation points=%lli\n",grid,numberOfBackupPoints);
	
	fPrintF(file,"grid %3i points: discretization=%8lli, interpolation=%7i, all=%8lli, unused=%8lli, name=%s\n",
		grid,numberOfDiscretizationPoints,cg.numberOfInterpolationPoints(grid),allPoints,
                allPoints-(numberOfDiscretizationPoints+cg.numberOfInterpolationPoints(grid)),
	       (const char *)cg[grid].mapping().getName(Mapping::mappingName));
        totalNumberOfPoints+=allPoints;
        totalNumberOfDiscretizationPoints+=numberOfDiscretizationPoints;
        totalNumberOfInterpolationPoints+=cg.numberOfInterpolationPoints(grid);
        totalBackup+=numberOfBackupPoints;
      }
      fPrintF(file,"total    points: discretization=%8lli, interpolation=%7lli, all=%8lli, unused=%8lli\n",
	      totalNumberOfDiscretizationPoints,totalNumberOfInterpolationPoints,totalNumberOfPoints,
              totalNumberOfPoints-(totalNumberOfDiscretizationPoints+totalNumberOfInterpolationPoints) );
      if( outfile==1 && totalBackup>0 )
      {
        printF(" total number of backup interpolation points=%lli. \n"
               "   (use `plot backup interpolation points' in the grid plotter to see these points)\n",
                 totalBackup);
      }

      fflush(0);
      if( file!=checkFile && info & 2 )
      {
	// print statistics
	real timeSum=timeUpdateGeometry+timeInterpolateBoundaries+timeCutHoles
	  +timeRemoveExteriorPoints+timeImproperInterpolation+timeProperInterpolation
	  +timeAllInterpolation+timeRemoveRedundant;
	real aveTimeSum=aveTimeUpdateGeometry+aveTimeInterpolateBoundaries+aveTimeCutHoles
	  +aveTimeRemoveExteriorPoints+aveTimeImproperInterpolation+aveTimeProperInterpolation
	  +aveTimeAllInterpolation+aveTimeRemoveRedundant;
	fPrintF(file,
               "-----------------------------------------------------------------------------------\n"
	       "                                                        cpu (seconds)          \n"
	       "                                                    max/proc     ave    percentage   \n"
	       "update geometry.....................................%8.2e  %8.2e   %6.2f       \n"
	       "interpolate boundaries..............................%8.2e  %8.2e   %6.2f       \n"
	       "cut holes...........................................%8.2e  %8.2e   %6.2f       \n"
	       "remove exterior points..............................%8.2e  %8.2e   %6.2f       \n"
	       "improper interpolation..............................%8.2e  %8.2e   %6.2f       \n"
	       "proper interpolation................................%8.2e  %8.2e   %6.2f       \n"
	       "all interpolation...................................%8.2e  %8.2e   %6.2f       \n"
	       "improve quality of interpolation....................%8.2e  %8.2e   %6.2f       \n"
	       "remove redundant points.............................%8.2e  %8.2e   %6.2f       \n"
	       "sum of above........................................%8.2e  %8.2e   %6.2f       \n"
	       "total...............................................%8.2e  %8.2e   %6.2f       \n"
	       "-----------------------------------------------------------------------------------\n",
	       timeUpdateGeometry,aveTimeUpdateGeometry,aveTimeUpdateGeometry/aveTotalTime*100.,
	       timeInterpolateBoundaries,aveTimeInterpolateBoundaries,aveTimeInterpolateBoundaries/aveTotalTime*100.,
	       timeCutHoles,aveTimeCutHoles,aveTimeCutHoles/aveTotalTime*100.,
	       timeRemoveExteriorPoints,aveTimeRemoveExteriorPoints,aveTimeRemoveExteriorPoints/aveTotalTime*100.,
	       timeImproperInterpolation,aveTimeImproperInterpolation,aveTimeImproperInterpolation/aveTotalTime*100.,
	       timeProperInterpolation,aveTimeProperInterpolation,aveTimeProperInterpolation/aveTotalTime*100.,
	       timeAllInterpolation,aveTimeAllInterpolation,aveTimeAllInterpolation/aveTotalTime*100.,
	       timeImproveQuality,aveTimeImproveQuality,aveTimeImproveQuality/aveTotalTime*100.,
	       timeRemoveRedundant,aveTimeRemoveRedundant,aveTimeRemoveRedundant/aveTotalTime*100.,
	       timeSum,aveTimeSum,aveTimeSum/aveTotalTime*100.,
	       totalTime,aveTotalTime,aveTotalTime/aveTotalTime*100.);

        const int mgLevels=cg.numberOfPossibleMultigridLevels();
	fPrintF(file,"===== number of processors=%i, number of possible extra multigrid levels=%i. ====\n",
                np,mgLevels-1);
        fPrintF(file,"===== pts/s=%g,  interp-pts/s=%g, pts/s/proc=%g, interp-pts/s/proc=%g\n",
		totalNumberOfPoints/totalTime,totalNumberOfInterpolationPoints/totalTime,
		totalNumberOfPoints/totalTime/np,totalNumberOfInterpolationPoints/totalTime/np);
	fPrintF(file,"===== memory per-proc: [min=%g,ave=%g,max=%g](Mb), reals/pt=%g,  total-mem=%g (Mb)\n",
		minMem,aveMem,maxMem,
                totalMem*SQR(1024.)/sizeof(real)/totalNumberOfPoints,  
                totalMem);
        fPrintF(file,"===== max-mem-recorded/proc = %g (Mb), total-max-mem=%g (Mb), reals/pt=%g\n",
		maxMemRecorded,totalMaxMem,totalMaxMem*SQR(1024.)/sizeof(real)/totalNumberOfPoints);
	fflush(0);
      }
    }
//     else if( info & 2 )
//       printF("Ogen::updateOverlap: Time to compute overlap = %8.2e (including update geometry=%8.2e)"
//              " (full algorithm)\n",totalTime, totalTime+timeUpdateGeometry);
  }
  fflush(0);
  return numberOfErrors;
}

//\begin{>>ogenUpdateInclude.tex}{\subsubsection{Non-interactive updateOverlap}}
int Ogen::
updateOverlap( CompositeGrid & cg )
// ========================================================================================
// /Description:
// Build a composite grid non-interactively using the component grids found
// in cg. This function might be called if one or more grids have changed.
// /Return value: 0=success, otherwise the number of errors encountered.
// 
//\end{ogenUpdateInclude.tex}
// ========================================================================================
{
  assert( cg.numberOfComponentGrids()>0 );
  if( ps==NULL )
  {
    printf("Ogen::updateOverlap:ERROR: supply a GenericGraphicsInterface object for Ogen before calling updateOverlap\n");
    throw "error";
  }

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  // printf("Ogen::updateOverlap( CompositeGrid & cg ) : ERROR: not implemented yet\n");

//   geometryNeedsUpdating.redim(cg.numberOfComponentGrids()); 
//   geometryNeedsUpdating=TRUE; // true if the geometry needs to be updated after changes in parameters
//   numberOfGridsHasChanged=TRUE;

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  psp.set(GI_PLOT_SHADED_SURFACE_GRIDS,TRUE);
  psp.set(GI_PLOT_LINES_ON_GRID_BOUNDARIES,TRUE);
  psp.set(GI_PLOT_BLOCK_BOUNDARIES,FALSE);
  psp.set(GI_COLOUR_INTERPOLATION_POINTS,TRUE);
  if( cg.numberOfDimensions()==3 )
    psp.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByGrid); // colour boundaries by grid number in 3d


  isNew.redim(numberOfBaseGrids);  // this grid is in the list of new grids (for the incremental algorithm)
  isNew=TRUE;
//  isNew(Range(0,numberOfOldGrids-1))=FALSE;
  geometryNeedsUpdating.redim(numberOfBaseGrids); 
  geometryNeedsUpdating=TRUE; // FALSE; // *** is this correct?

  setGridParameters(cg);  // set parameters for multigrid levels
      
  totalTime=getCPU(); // starting value for totalTime

  aString answer;
  aString menu[]=
  {
    "compute overlap",
    "exit",
    "abort",
    "" 
  };
  

  int numberOfErrors = 0;
  bool done=FALSE;
  for(int it=0; !done; it++)
  {
    if( it==0 )
      answer="compute overlap";
    else
      ps->getMenuItem(menu,answer,"choose an option");
    
    if( answer=="compute overlap" )
    {

    // Compute multigrid levels -- starting from the coarsest grid
      // debug=1;
      if( info & 4 )
	printf("cg.numberOfMultigridLevels()=%i \n",cg.numberOfMultigridLevels());
      // cg[0].vertex().display(sPrintF(buff,"level=%i, cg[0][0].vertex",0));
      for( int l=cg.numberOfMultigridLevels()-1; l>=0; l-- )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  maskRatio[axis]= (int)pow(2,l);  

	CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];

	geometryNeedsUpdating=TRUE;
	numberOfGridsHasChanged=TRUE;

	real time0=getCPU();
	updateGeometry(m,m);               // build some geometry arrays used by the overlap algorithm
	timeUpdateGeometry=getCPU()-time0;

	//if( true )
        //  resetGrid(cg); // ****************** for testing amrGrid
	

	if( info & 2 )
	  printf(" time to update the geometry..............................%e \n",
		 timeUpdateGeometry);

	numberOfHolePoints=0;
	numberOfOrphanPoints=0;

	// m[0].vertex().display(sPrintF(buff,"level=%i, m[0].vertex",l));
	if( info & 4 )
	  printf("Compute the grid for multigrid level =%i \n",l);


	numberOfErrors = computeOverlap( cg,cg,l );


	if( numberOfErrors==(-resetTheGrid) )
	{
	  resetGrid(cg);
	  continue;
	}
	else if( numberOfErrors > 0  )
	{
	  if( cg.numberOfMultigridLevels()>1 )
	    printf(" ===== overlap computation failed for multigrid level = %i. Try requesting fewer levels \n",l);
	  else
	    printf(" ===== overlap computation failed ======\n");
	  continue;
	}
	else if( numberOfErrors < 0  )
	{
	  printf(" ===== The overlap computation completed but there were non-fatal errors. ======\n"
		 " It could be that backup rules were used for some points. These will appear as black\n"
		 " marks if the grid is being plotted. The resulting grid may not give good results \n"
		 " when used to solve a PDE. You can also plot the grid and turn on backup interpolation points\n"
		 " to see any questionable points. *Check* the ogen.log file for further info. \n");
	  numberOfErrors=0;
	}

      }
      done=TRUE;
	
  
      // add interpolation data from the coarse grids into the "base" CompositeGrid
      if( numberOfErrors ==0 && cg.numberOfMultigridLevels()>1 )
      {
	int l;
	for( l=0; l<cg.numberOfMultigridLevels(); l++ )
	{
	  CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
	  // fill-in numberOfInterpolationPoints from multigrid-level data back into the base CompositeGrid.
	  for( int g=0; g<m.numberOfBaseGrids(); g++ )
	  {
	    if( debug & 2 )
	      printf("l=%i: m.componentGridNumber(%i) = %i, m.gridNumber(%i)=%i, m.baseGridNumber(%i)=%i \n",
		     l,g, m.componentGridNumber(g), g,m.gridNumber(g),g,m.baseGridNumber(g));
             
	    int grid = m.gridNumber(g);
	    cg.numberOfInterpolationPoints(grid)=m.numberOfInterpolationPoints(g);
	  }
	}
	// display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
	// cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");

	// now we know how many interpolation points there are so we can create the arrays in the cg.
//	const int theLists  =
//	  (cg.numberOfRefinementLevels() == 1 ? CompositeGrid::NOTHING : CompositeGrid::THErefinementLevel) |
//	  (cg.numberOfMultigridLevels()  == 1 ? CompositeGrid::NOTHING : CompositeGrid::THEmultigridLevel );

	cg.update(
	  CompositeGrid::THEinterpolationPoint       |
	  CompositeGrid::THEinterpoleeGrid           |
	  CompositeGrid::THEinterpoleeLocation       |
	  CompositeGrid::THEinterpolationCoordinates ,
	  CompositeGrid::COMPUTEnothing);

	// fill-in  multigrid-level data back into the base CompositeGrid.
	for( l=0; l<cg.numberOfMultigridLevels(); l++ )
	{
	  CompositeGrid & m = cg.numberOfMultigridLevels()==1 ? cg : cg.multigridLevel[l];
	  for( int g=0; g<m.numberOfBaseGrids(); g++ )
	  {
	    int grid = m.gridNumber(g);
	    cg.interpoleeGrid[grid]            =m.interpoleeGrid[g];
	    cg.variableInterpolationWidth[grid]=m.variableInterpolationWidth[g];
	    cg.interpolationPoint[grid]        =m.interpolationPoint[g];
	    cg.interpoleeLocation[grid]        =m.interpoleeLocation[g];
	    cg.interpolationCoordinates[grid]  =m.interpolationCoordinates[g];

	    // printf(" m.interpolationPoint.getLength() = %i \n",m.interpolationPoint.getLength());
	    // display(m.interpolationPoint[g],sPrintF(buff," l=%i, g=%i, interpolationPoint ",l,g));
	  }
	}
  
	//  Tell the CompositeGrid that the interpolation data have been computed:
	cg->computedGeometry |=
	  CompositeGrid::THEmask                     |
	  CompositeGrid::THEinterpolationCoordinates |
	  CompositeGrid::THEinterpolationPoint       |
	  CompositeGrid::THEinterpoleeLocation       |
	  CompositeGrid::THEinterpoleeGrid           |
	  CompositeGrid::THEmultigridLevel;  // *wdh*

      }

      if( numberOfErrors==0 )
      {
	determineMinimalIndexRange(cg);
      }
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer=="abort" )
    {
      throw "error";
    }
    else
    {
      cout << "Unknown answer=[" << answer << "]\n";
      ps->stopReadingCommandFile();
    }

  } // for( it
  

  return 0;
}




//\begin{>>ogenUpdateInclude.tex}{\subsubsection{Moving Grid updateOverlap}}
int Ogen::
updateOverlap(CompositeGrid & cg, 
	      CompositeGrid & cgOld, 
	      const LogicalArray & hasMoved, 
	      const MovingGridOption & option /* =useOptimalAlgorithm */ )
// ========================================================================================
// /Description:
//    Determine an overlapping grid when one or more grids has moved.
//   {\bf NOTE:} If the number of grid points changes then you should use the 
//   {\tt useFullAlgorithm} option.
// 
// /cg (input) : grid to update
// /cgOld (input) : for grids that have not moved, share data with this CompositeGrid.
// /hasMoved (input): specify which grids have moved with hasMoved(grid)=TRUE
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
  numberOfArrays=max(numberOfArrays,GET_NUMBER_OF_ARRAYS);
  int returnValue=0;
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  info=1;
  real time0=getCPU();

  const int numberOfBaseGrids=cg.numberOfBaseGrids();
  const int numberOfComponentGrids=cg.numberOfComponentGrids();

  const int computedGeometry0=cgOld->computedGeometry;
//    printf(" >>>>> computedGeometry0 & THErefinementLevel=%i \n",
//  	   computedGeometry0 & CompositeGrid::THErefinementLevel);

  assert( hasMoved.getLength(0)>=numberOfComponentGrids );

  if( gridScale.getLength(0)<numberOfBaseGrids )
  {
    // compute gridScale: gridScale(grid) = maximum length of the bounding box for a grid
    gridScale.redim(numberOfBaseGrids);
    // warnForSharedSides : TRUE if we have warned about possible shared sides not being marked properly
    warnForSharedSides.redim(numberOfBaseGrids,6,numberOfBaseGrids,6);
    warnForSharedSides=FALSE;
  }

  buildBounds(cg); // compute rBound, boundaryEps

  if( debug & 4 )
  {
    if( computeGeometryForMovingGrids )
      printF("*** Ogen::updateOverlap: invalidate geometry for grids that move\n");
    else
      printF("** Ogen::updateOverlap: do NOT invalidate geometry for grids that move\n");
  }
  
  bool sameNumberOfGridPoints=TRUE;
  Range Rx(0,cg.numberOfDimensions()-1);
  // The next loop generates an array id leak in parallel
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & g = cg[grid];

    const bool isBaseGrid = cg.refinementLevelNumber(grid)==0;
    const int base = cg.baseGridNumber(grid);

    // (g.indexRange()-cgOld[grid].indexRange()).display("g.indexRange()-cgOld[grid].indexRange()");
    
    if( isBaseGrid )
    {
      if( max(abs(g.indexRange()-cgOld[grid].indexRange()))!=0 ||
	  max(abs(g.gridIndexRange()-cgOld[grid].gridIndexRange()))!=0 ||
	  max(abs(g.extendedIndexRange()-cgOld[grid].extendedIndexRange()))!=0 )
      {
	sameNumberOfGridPoints=FALSE;
      }
    }

    g.update(MappedGrid::THEmask);  // *wdh* 081026 -- needed for parallel for some reason --

    if( hasMoved(grid) || hasMoved(base)  )
    {
      // this next section has an array leak in parallel
      // We invalidate the geometry arrays if the grid has moved (or the base grid for an AMR grid has moved)
      if( computeGeometryForMovingGrids )
	g.geometryHasChanged(~MappedGrid::THEmask);   // **** this invalidates all geometry except the mask ***
      
      if( isBaseGrid )
      {
	// The grid generator needs the following arrays
	g.update( MappedGrid::THEcenter               |
		  MappedGrid::THEvertex               |   // we need this even for cell centred grids
		  // MappedGrid::THEvertexDerivative     |   // This was needed to prevent an error in 3D with ghost=2
		  // MappedGrid::THEcenterDerivative     |   // This was needed to prevent an error in 3D with ghost=2
		  MappedGrid::THEvertexBoundaryNormal |
		  MappedGrid::THEboundingBox      );  
	  
      }
    }
    else
    {
      // share data
      if( isBaseGrid )
      {
        // This call causes g and cgOld[grid] to share the vertex, center, vertexBoundaryNormal, ...
	if( !g.isRectangular() )
	{
	  g.update(cgOld[grid],
		   MappedGrid::THEcenter               |
		   MappedGrid::THEvertex               |   // we need this even for cell centred grids
		   MappedGrid::THEvertexBoundaryNormal |
		   MappedGrid::THEboundingBox      );  
	}
	else
	{
          // *wdh* 110630 - do NOT build center/vertex for rectangular grids
	  g.update(cgOld[grid],
		   MappedGrid::THEvertexBoundaryNormal |
		   MappedGrid::THEboundingBox      );  
	}
	
      }
      else
      {

        // share the following arrays
        // This call causes g and cgOld[grid] to share the vertex, center, vertexBoundaryNormal, ...
	if( !g.isRectangular() )
	{
	  g.update(cgOld[grid],
		   MappedGrid::THEcenter               |
		   MappedGrid::THEvertex               |   // we need this even for cell centred grids
		   MappedGrid::THEvertexBoundaryNormal |
		   MappedGrid::THEboundingBox      ); 
	}
	else
	{
          // *wdh* 110630 - do NOT build center/vertex for rectangular grids
	  g.update(cgOld[grid],
		   MappedGrid::THEvertexBoundaryNormal |
		   MappedGrid::THEboundingBox      ); 
	}
	
        // We cannot share the mask since it may change on the new grid

        // *wdh* 040322 : break the reference with the mask
        g.update(MappedGrid::THEmask);
        g.rcData->mask->breakReference();
	g.updateReferences();
	

      }
      
    }
  }
  cg->computedGeometry |=    CompositeGrid::THEmask;

  if( false )
  {
    printF("Ogen::updateOverlap:INFO:\n");
    for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
  

  timeUpdateGeometry=getCPU()-time0;
  totalTime=getCPU()-timeUpdateGeometry;

  if( useNewMovingUpdate && option==useOptimalAlgorithm && sameNumberOfGridPoints &&
      cg.numberOfComponentGrids()>1 ) // *wdh* added 040831
  {
    // **********************************************************
    // ****** optimized overlap algorithm for moving grids ******
    // **********************************************************

    returnValue = movingUpdate(cg,cgOld,hasMoved,option);


    if( returnValue==0 )
    {
      return returnValue;
    }
    else
    {
      if( true || debug & 1 )
	printf("Ogen:updateOverlap for moving grids, resort to full algorithm...\n");
    }
  }
  else
  {
    cg.update(
      CompositeGrid::THEmask                     |
      CompositeGrid::COMPUTEnothing              );
  }


  // if( true )  // *wdh* 2012/06/18
  //   return 0; 
  

  // ********************************
  // ***** Use full algorithm *******  
  // ********************************

  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    MappedGrid & g = cg[grid];
      
    // g.update(MappedGrid::THEmask, MappedGrid::COMPUTEnothing);

    // mask : 
    //  The lower order bits ( GRIDnumberBits = bits 0..22) conatin the preference for a point:
    //    either the grid we interpolate from or the grid number of the current grid.
    // By default all grids try to interpolate from the first preference:
    g.mask() = MappedGrid::ISdiscretizationPoint;  // **** should use highest priority

    g->computedGeometry |= MappedGrid::THEmask;

    gridScale(grid)=max(cg[grid].boundingBox()(End,Rx)-cg[grid].boundingBox()(Start,Rx));
    
//     if( false )
//     {
//       // cg[grid].mapping().getMapping().reinitialize(); // force a remake of the bb

//       // const RealArray & bbg =cg[grid].boundingBox();
//       // const RealArray & bbm = cg[grid].mapping().getMapping().getBoundingBox();

//       g.update(MappedGrid::THEvertex);
      
//       const realArray & xm = cg[grid].mapping().getMapping().getGrid();
//       const realArray & xg = cg[grid].vertex();
//       const realArray & xi = cg[grid].mapping().getMapping().approximateGlobalInverse->getGrid();
      
//       const RealArray & bbg =cg[grid].boundingBox();
//       const RealArray & bbm = cg[grid].mapping().getMapping().getBoundingBox();

//       Index I1,I2,I3;
//       getIndex(g.gridIndexRange(),I1,I2,I3);
//       printF("Ogen: bbg=[%9.2e,%9.2e][%9.2e,%9.2e], xg=[%9.2e,%9.2e][%9.2e,%9.2e],  grid=%i hasMoved=%i \n"
//              "      bbm=[%9.2e,%9.2e][%9.2e,%9.2e], xm=[%9.2e,%9.2e][%9.2e,%9.2e]\n"
//              "                                                      xi=[%9.2e,%9.2e][%9.2e,%9.2e]\n",
// 	     bbg(0,0),bbg(1,0),bbg(0,1),bbg(1,1),min(xg(I1,I2,I3,0)),max(xg(I1,I2,I3,0)),min(xg(I1,I2,I3,1)),max(xg(I1,I2,I3,1)),
//               grid,(int)hasMoved(grid),
// 	     bbm(0,0),bbm(1,0),bbm(0,1),bbm(1,1),min(xm(I1,I2,I3,0)),max(xm(I1,I2,I3,0)),min(xm(I1,I2,I3,1)),max(xm(I1,I2,I3,1)),
//              min(xi(I1,I2,I3,0)),max(xi(I1,I2,I3,0)),min(xi(I1,I2,I3,1)),max(xi(I1,I2,I3,1)) );
//     }
    

  }

  if( cg.numberOfComponentGrids()==0 ) // *wdh* added 040831
    return returnValue;  


  cg.numberOfInterpolationPoints=0; // *wdh* 9907014

  // build the arrays inverseCoordinates, inverseGrid and inverseCondition to be used by the overlap algorithm
  
  cg.update(CompositeGrid::THEinverseMap, CompositeGrid::COMPUTEnothing);
  // cg[0].vertexDerivative().display("vertex derivative");
  // change the shape (orignally (nd,all,all,all)
  // Range all;
  // cg.inverseCoordinates.updateToMatchGrid(cg,all,all,all,cg.numberOfDimensions());
  
  cg.inverseCoordinates=0;
  cg.inverseGrid=-1;
   
  if( info & 2 && numberOfGridsHasChanged )
  {
   gridScale.display("Ogen::updateGeometry: Here is gridScale");
  }

  timeUpdateGeometry=getCPU()-time0;
  if( info & 2 )
    printf(" time to update the geometry..............................%e (total=%e)\n",
	   timeUpdateGeometry,timeUpdateGeometry);

  totalTime=getCPU()-timeUpdateGeometry;

  numberOfOrphanPoints=0;
  numberOfHolePoints=0;
  isMovingGridProblem=true;
  // bool firstTimeToComputeOverlap=FALSE;


/* ----
  cg->computedGeometry &= ~CompositeGrid::THEboundingBox;
  cg.update(
    CompositeGrid::THEmask               |
    CompositeGrid::THEcenter             |
    CompositeGrid::THEvertex             |
    CompositeGrid::THEcenterDerivative   |
    CompositeGrid::THEvertexDerivative   |
//    CompositeGrid::THEinverseMap         |
    CompositeGrid::THEboundingBox   );
//    CompositeGrid::theLists              );
---- */


  for( grid=0; grid<numberOfBaseGrids; grid++ )
  {
    // We need to recompute the bounding boxes -- we could optimize this for the MatrixTransform
    if( hasMoved(grid) && cg[grid].mapping().getMapping().approximateGlobalInverse!=NULL )
      cg[grid].mapping().getMapping().approximateGlobalInverse->reinitialize(); 
  }

  if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
  {
    numberOfArrays=GET_NUMBER_OF_ARRAYS;
    printf("**** updateOverlap:Before computeOverlap: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

//  if( true )  // *wdh* 2012/06/18
//    return 0; 


  returnValue=computeOverlap(cg,cgOld,0,isMovingGridProblem,hasMoved);

  if( computedGeometry0 & CompositeGrid::THErefinementLevel ) // *wdh* 040504 -- don't forget we have refinement levels
    cg->computedGeometry |= CompositeGrid::THErefinementLevel;
  
  if( GET_NUMBER_OF_ARRAYS > numberOfArrays )
  {
    numberOfArrays=GET_NUMBER_OF_ARRAYS;
    printf("**** updateOverlap:After computeOverlap: number of A++ arrays = %i \n",GET_NUMBER_OF_ARRAYS);
  }

  return returnValue;
}

