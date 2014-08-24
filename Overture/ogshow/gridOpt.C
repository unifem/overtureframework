// This file automatically generated from gridOpt.bC with bpp.
#include "GL_GraphicsInterface.h"
#include "CompositeGrid.h"
#include "PlotIt.h"
#include "xColours.h"
#include "ParallelUtility.h"

// local version so that we can change it: 
static int isHiddenByRefinement=MappedGrid::IShiddenByRefinement;


// here is a computational point but not a refined point
#define MASK_CNR(i1,i2,i3) (MASK(i1,i2,i3) && !(MASK(i1,i2,i3) & isHiddenByRefinement))
// here is a discretization point but not a refined point
#define MASK_DNR(i1,i2,i3) (MASK(i1,i2,i3)>0 && !(MASK(i1,i2,i3) & isHiddenByRefinement))

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  
#define FOR_3WithStride(i1,i2,i3,m1,m2,m3,I1,I2,I3) i1Bound=I1.getBound(); i2Bound=I2.getBound(); i3Bound=I3.getBound(); for( i3=I3.getBase(); i3<=i3Bound; i3+=m3 )  for( i2=I2.getBase(); i2<=i2Bound; i2+=m2 )  for( i1=I1.getBase(); i1<=i1Bound; i1+=m1 )

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )

#define XSCALE(x) (psp.xScaleFactor*(x))
#define YSCALE(y) (psp.yScaleFactor*(y))
#define ZSCALE(z) (psp.zScaleFactor*(z))


// ==============================================================================================
//   Collect the interpolation data (interpolationPoint, interpoleeGrid) that 
//  is located on the grid local to processor srcProcessor and transfer this data to destProcessor 
//  This routine is used when plotting the interpolation points.
//
// /srcProcessor (input) : find interp data that lives on the mask array that is local to processor p=srcProcessor
// /destProcessor (input) : transfer the interp data to this processor
// /interpolationPoint,interpoleeGrid (output) : all interp pts that live on portion of this grid local
//                       to p=srcProcessor.
// ============================================================================================
int
collectInterpolationData( int srcProcessor, int destProcessor, int grid, CompositeGrid & cg,
                                                    intSerialArray & interpolationPoint, intSerialArray & interpoleeGrid )
{
#ifdef USE_PPP
  // parallel case

    const int debug=0;

    const int myid = Communication_Manager::My_Process_Number;
    const int np=max(1,Communication_Manager::Number_Of_Processors);

    const int numberOfDimensions = cg.numberOfDimensions();
    
    bool useLocal = !( 
        (grid<cg.numberOfBaseGrids() && 
          cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
        cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData );

    intSerialArray ip,ig;
    if( !useLocal )
    {
        ip.reference(cg.interpolationPoint[grid].getLocalArray());
        ig.reference(cg.interpoleeGrid[grid].getLocalArray());
    }
    else
    {
        ip.reference(cg->interpolationPointLocal[grid]);
        ig.reference(cg->interpoleeGridLocal[grid]);
    }

    if( debug>0 )
        printF("\n ******** collectInterpolationData: srcProcessor=%i destProcessor=%i grid=%i ***********\n",
       	 srcProcessor,destProcessor,grid);

    const intArray & mask = cg[grid].mask();
    int iv[3]={0,0,0}; 

    const int maxInterp=ip.getLength(0); 
    int *data = new int[maxInterp*(numberOfDimensions+1)];
    int numInterp=0;           // counts pts that live on processor p
    int j=0;
    for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
    {
        for( int axis=0; axis<numberOfDimensions; axis++ )
            iv[axis]=ip(i,axis);
        
        int p = mask.Array_Descriptor.findProcNum( iv );    // index iv[] is on processor p 

        if( p==srcProcessor )
        { // save this data 
            for( int axis=0; axis<numberOfDimensions; axis++ )
            {
      	data[j]=iv[axis];
                j++;
            }
            data[j]=ig(i);
            j++;
            
            if( debug>0 )
      	printf(" collect:SEND for srcProc=%i : myid=%i i=%i ip=(%i,%i) ig=%i\n",srcProcessor,myid,i,iv[0],
                              iv[1],ig(i));

            numInterp++;
        }
    }
    assert( numInterp<=maxInterp );
    
  // int totalNum=ParallelUtility::getSum(j);
    
    if( debug>0 )
        printf(" collectInterpolationData: myid=%i, destProcessor=%i, send %i pts to srcProcessor=%i (total=%i)\n",
         	   myid,destProcessor,numInterp,srcProcessor,ip.getLength(0));

    int *nump; // nump[p] = number of interp pts we have found on proc. p
    
    MPI_Request *receiveRequest=NULL;
    int sendTag= 505808;
    if( myid==destProcessor )
    {
        nump= new int [np];
        receiveRequest = new MPI_Request [np];
        
    // ---- post receives for the number of interp values that will arrive from processor p --
        for( int p=0; p<np; p++ )
            MPI_Irecv( &nump[p],1,MPI_INT,p,sendTag+p,MPI_COMM_WORLD,&receiveRequest[p] );
    }
  // send the number of interp pts that we found:
    MPI_Request sendRequest1;
    MPI_Isend( &numInterp,1,MPI_INT,destProcessor,sendTag+myid,MPI_COMM_WORLD,&sendRequest1 );
  
    MPI_Status *receiveStatus=NULL;
    int totalNum=0;  // total number of interp pts we found on all procs
    int *rbuff=NULL; // receive buffer 
    if( myid==destProcessor )
    {
        receiveStatus = new MPI_Status [np];
        MPI_Waitall( np, receiveRequest, receiveStatus );  // wait to receive all messages

        for( int p=0; p<np; p++ )
        {
            if( debug>0 )
      	printf(" collect: myid=%i, will receive nump=%i values from p=%i\n",myid,nump[p],p);
            totalNum+=nump[p];
        }
        
    // allocate buffer space:
        if( totalNum>0 )
            rbuff = new int [totalNum*(numberOfDimensions+1)];
        else
        {
      // no interp data to send: 
            interpolationPoint.redim(0);
            interpoleeGrid.redim(0);
            delete [] data;
            data=NULL;
            return 0;
        }
        
    }
    
    sendTag=606303;
    int numToReceive=0;  // counts how many processors will send data
    if( myid==destProcessor )
    {
    // ---- post receives for the number of interp values that will arrive from processor p --
        int ii=0;
        for( int p=0; p<np; p++ )
        {
            if( nump[p]>0 )
            {
                int numData=nump[p]*(numberOfDimensions+1);
      	MPI_Irecv( &rbuff[ii],numData,MPI_INT,p,sendTag+p,MPI_COMM_WORLD,&receiveRequest[numToReceive] );
      	ii+=numData;
                numToReceive++;
            }
        }
        assert( ii==totalNum*(numberOfDimensions+1) );
    }

  // send data
    MPI_Request sendRequest;
    if( numInterp>0 )
    {
        int numData=numInterp*(numberOfDimensions+1);
        MPI_Isend( data,numData,MPI_INT,destProcessor,sendTag+myid,MPI_COMM_WORLD,&sendRequest );
    }
    
  // *********
//    fflush(0);
//    Communication_Manager::Sync();
   // *************

    if( myid==destProcessor )
    {
        assert( totalNum>0 );
        
        MPI_Waitall( numToReceive, receiveRequest, receiveStatus );  // wait to receive all messages

    // fill in interp arrays from the buffer

        interpolationPoint.redim(totalNum,numberOfDimensions);
        interpoleeGrid.redim(totalNum);
        int j=0;
        for( int i=0; i<totalNum; i++ )
        {
            for( int axis=0; axis<numberOfDimensions; axis++ )
            {
      	interpolationPoint(i,axis)=rbuff[j];
                j++;
            }
            interpoleeGrid(i)=rbuff[j];
            j++;
            if( debug>0 )
      	printf(" collect:RESULTS for destProc=%i : i=%i ip=(%i,%i) ig=%i\n",destProcessor,i,interpolationPoint(i,0),
             	       interpolationPoint(i,1),interpoleeGrid(i));
        }
        assert( j==totalNum*(numberOfDimensions+1) );
        
        delete [] nump;
        delete [] receiveRequest;
        delete [] receiveStatus;
        delete [] rbuff;
        
    }

  // wait to send data before deleting buffers
    if( numInterp>0 )
    {
        MPI_Status sendStatus;
        int numSend=1; // only 1 message sent
        MPI_Waitall( numSend, &sendRequest, &sendStatus );   
    }

    delete [] data;
    
    return 0;
#else
  // serial case
    OV_ABORT("ERROR: collectInterpolationData not expected to be called in serial");
    return 0;
#endif
}


// *****************************************************************************************
//    plotInterpolationPoints Macro 
// *****************************************************************************************


// *****************************************************************************************************************
//   plotStructured Macro
//
//  GRIDTYPE: rectangular or curvilinear
// 
// *****************************************************************************************************************

void PlotIt::
plotGrid2d(GenericGraphicsInterface &gi, GridCollection & gc, 
          GraphicsParameters & psp, RealArray & xBound,
          int multigridLevelToPlot, IntegerArray & numberList, int & number )
// =======================================================================================
//  Plot a 2d grid -- optimized version
// ======================================================================================
{
    
    const int myid=max(0,Communication_Manager::My_Process_Number);
    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int graphicsProcessor = gi.getProcessorForGraphics();
    const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();

    GridCollection & gcl = multigridLevelToPlot==0 ? gc : gc.multigridLevel[multigridLevelToPlot];
    const int numberOfGrids = psp.plotRefinementGrids ? gcl.numberOfComponentGrids() : gcl.numberOfBaseGrids();
    const int numberOfDimensions = gc.numberOfDimensions();
    
    IntegerArray & gridsToPlot          = psp.gridsToPlot;
    IntegerArray & gridOptions          = psp.gridOptions;
    bool & plotInterpolationPoints      = psp.plotInterpolationPoints;
    bool & plotBackupInterpolationPoints= psp.plotBackupInterpolationPoints;
    bool & labelBoundaries              = psp.labelBoundaries;
    bool & plotBranchCuts               = psp.plotBranchCuts;
    int  & boundaryColourOption         = psp.boundaryColourOption;
    int  & gridLineColourOption         = psp.gridLineColourOption;
    int  & blockBoundaryColourOption    = psp.blockBoundaryColourOption;
    real & zLevelFor2DGrids             = psp.zLevelFor2DGrids;                    // level for a 2D grid
    real & yLevelFor1DGrids             = psp.yLevelFor1DGrids;
    bool & labelGridsAndBoundaries      = psp.labelGridsAndBoundaries;
    bool & plotInterpolationCells       = psp.plotInterpolationCells;
    bool & plotNonPhysicalBoundaries    = psp.plotNonPhysicalBoundaries;

    isHiddenByRefinement = psp.plotHiddenRefinementPoints ? 0 : MappedGrid::IShiddenByRefinement;

    int i,i1,i2,i3,axis;
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

#ifdef USE_PPP
  // build a partition for arrays that just live on the graphicsProcessor.
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

    int grid;
    for( grid=0; grid<numberOfGrids; grid++ )
    {
        if( !(gridsToPlot(grid)&GraphicsParameters::toggleGrids) &&
        !(gridOptions(grid)&GraphicsParameters::plotInterpolation) )
        {
            if( plotInterpolationPoints )
      	numberList(++number)=grid;  // for labels
            continue; // skip this component grid altogether
        }
        else
        {
            numberList(++number)=grid;  // for labels
        }
          	    
        MappedGrid & mg = gcl[grid];
        if( plotOnThisProcessor ) 
            glPushName(mg.getGlobalID()); // assign a name for picking

        
        aString col=getGridColour( 1,0,0,grid,gcl,gi,psp );

#ifndef USE_PPP
            const intSerialArray & mask = mg.mask();
#else
            intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
#endif
            
            bool cellVertex = (bool)mg.isAllVertexCentered();
            bool cellCentre = (bool)mg.isAllCellCentered();
            
            const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
            const int maskDim0=mask.getRawDataSize(0);
            const int maskDim1=mask.getRawDataSize(1);
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
            
            const IntegerArray & dimension = mg.dimension();
            const IntegerArray & gridIndexRange = mg.gridIndexRange();
            


        if( mg.getGridType()==MappedGrid::unstructuredGrid && (gridsToPlot(grid)&GraphicsParameters::toggleGrids) )
        {

      // printf("plot: plot unstructured grid...\n");

            psp.set(GI_MAPPING_COLOUR,col);
            	      
            plotUnstructured(gi, (UnstructuredMapping&)(mg.mapping().getMapping()), psp);

            for( int p=0; p<np; p++ )  // -- finish me for parallel -- do this for now 
            {
      // 	plotInterpolationPoints(unstructured);
        // ----------------------Interpolation Points------------------------------------
                if( (plotInterpolationPoints || plotBackupInterpolationPoints ) && 
                        (gridOptions(grid)&GraphicsParameters::plotInterpolation) &&
                          numberOfGrids >= 1 && 
                          gc.getClassName()=="CompositeGrid"  )
                {
                    CompositeGrid & cg0 = (CompositeGrid &)gc;
                    CompositeGrid & cg = multigridLevelToPlot==0 ? cg0 : cg0.multigridLevel[multigridLevelToPlot];
          // RealDistributedArray & coord = (bool)cg[grid].isAllVertexCentered() ? cg[grid].vertex() : cg[grid].center(); 
          //         #If "unstructured" == "rectangular"
          //         #Else
                    cg[grid].update(MappedGrid::THEcenter);
                    #ifndef USE_PPP
                        const realSerialArray & center = cg[grid].center(); 
                    #else
                        realSerialArray center; getLocalArrayWithGhostBoundaries(cg[grid].center(),center);
                    #endif
                    i2=center.getBase(1);
                    i3=center.getBase(2);
        // printf(" gridPlot: cg.interpolationPoint.getLength()=%i\n",cg.interpolationPoint.getLength());
                if( grid>=cg.interpolationPoint.getLength() )  // *wdh* 2012/03/02 -- if a refinement has just been added
                    continue;
                  #ifndef USE_PPP
                        const intSerialArray & ip = cg.interpolationPoint[grid];
                        const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
                  #else
                  intSerialArray ip,interpoleeGrid;
         // if( !( p==1 && grid==1)  ) continue; // ********************************8
      //    fflush(0);
      //    Communication_Manager::Sync();
         // *new* way *wdh* 090808
                  collectInterpolationData( p,graphicsProcessor, grid,cg, ip,interpoleeGrid );
      //     bool useLocal = !( 
      //       (grid<cg.numberOfBaseGrids() && 
      //           cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
      //       cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData );
      //     if(  myid==p )
      //     { // We will use the interpolation data from processor p: 
      //       if( !useLocal )
      //       {
      // 	ip.reference(cg.interpolationPoint[grid].getLocalArray());
      // 	interpoleeGrid.reference(cg.interpoleeGrid[grid].getLocalArray());
      //       }
      //       else
      //       {
      // 	ip.reference(cg->interpolationPointLocal[grid]);
      // 	interpoleeGrid.reference(cg->interpoleeGridLocal[grid]);
      //       }
      //     }
                    if( p!=graphicsProcessor )
                    {
            // copy the interpolation data from processor "p" to processor "graphicsProcessor"
      //       CopyArray::copyArray( ip,graphicsProcessor, ip,p );
      //       CopyArray::copyArray( interpoleeGrid,graphicsProcessor, interpoleeGrid,p );
            // -- for vertex centered grids we can re-use the VERTEX, otherwise we need the CENTER  ** fix me **
          //             #If "unstructured" == "curvilinear"
                    }
                    if( false && myid==graphicsProcessor )
                    {
                        printF(" plotInterpolationPoints: plot data from p=%i \n",p);
                        ::display(ip,"ip","%4i ");
                        ::display(interpoleeGrid,"interpoleeGrid","%4i ");
            //             #If "unstructured" == "curvilinear"
                    }
                  #endif
                    if( ip.getLength(0)>0 )
                    {
            // ::display(ip,"gridPlot: ip");
          //       #If "unstructured" == "rectangular"
          //       #Else    
                        const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
                        const int centerDim0=center.getRawDataSize(0);
                        const int centerDim1=center.getRawDataSize(1);
                        const int centerDim2=center.getRawDataSize(2);
            #define CENTER0(i0,i1,i2) XSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(0)))])
            #define CENTER1(i0,i1,i2) YSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(1)))])
      // define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]
                        const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
                        const int ipDim0=ip.getRawDataSize(0);
            #define IP(i0,i1) ipp[i0+ipDim0*(i1)]
                        const int *interpoleeGridp = interpoleeGrid.Array_Descriptor.Array_View_Pointer0;
                        const int interpoleeGridDim0=interpoleeGrid.getRawDataSize(0);
            #define INTERPOLEEGRID(i0) interpoleeGridp[i0]
            // offset interp points on refinement grids so we see them instead of the lower levels
                        const int level = gcl.refinementLevelNumber(grid);
                        const real offset = 1.e-3;  // use this for now so refinement grid interp points appear on top
                        const real zLevelForPoints = zLevelFor2DGrids + level*offset;
                        if( plotInterpolationPoints && plotOnThisProcessor )
                        {
                  	gi.setColour(GenericGraphicsInterface::textColour);
                  	glPointSize(psp.pointSize*1.67*gi.getLineWidthScaleFactor() );   
                  	glBegin(GL_POINTS);  
      //		    glPointSize(5.*lineWidthScaleFactor[currentWindow]);   
      //              printf("plotInterpolationPoints: grid=%i num=%i ptSize=%8.2e -> %8.2e \n",
      //  		   grid,cg.numberOfInterpolationPoints(grid),
      //                     (real)psp.pointSize,psp.pointSize*1.67*gi.getLineWidthScaleFactor());
      	// printf(" gridPlot: grid=%i ni=%i\n",grid,cg.numberOfInterpolationPoints(grid));
                  	int oldInterpolationPointColour=-1;
                  	for( i=ip.getBase(0); i<=ip.getBound(0); i++ )
                  	{
      	  // printf(" gridPlot: grid=%i i=%i, ip=(%i,%i) donor=%i\n",grid,i,IP(i,axis1),IP(i,axis2),
      	  //        INTERPOLEEGRID(i));
                    	  if( psp.colourInterpolationPoints && INTERPOLEEGRID(i)!=oldInterpolationPointColour )
                    	  {
      	    // colour the interpolation the same colour as the grid it interpolates from
                      	    oldInterpolationPointColour=INTERPOLEEGRID(i);
                      	    setXColour(gi.getColourName( (oldInterpolationPointColour %
                                            					  GenericGraphicsInterface::numberOfColourNames) ));
      // 		setXColour(gi.getColourName( min(oldInterpolationPointColour,
      // 						 GenericGraphicsInterface::numberOfColourNames-1)) );
                    	  }
      	  // printf(" plot pt i=%i, level=%i, zLevelForPoints=%g\n",i,level,zLevelForPoints);
                    	  if( numberOfDimensions==2 )
                      	    glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),i3),
                              		      CENTER1(IP(i,axis1),IP(i,axis2),i3),zLevelForPoints);
                    	  else if( numberOfDimensions==3 )
                      	    glVertex3(CENTER0(IP(i,axis1),i2,i3),
                              		      CENTER1(IP(i,axis1),i2,i3),zLevelForPoints);
                    	  else 
                      	    glVertex3(CENTER0(IP(i,axis1),i2,i3),yLevelFor1DGrids,zLevelForPoints);
                  	}
                  	glEnd();
                        }
                        if( plotBackupInterpolationPoints && plotOnThisProcessor )
                        {
      	// Now plot points that use back up interpolation
            #ifndef USE_PPP
                        const intSerialArray & mask = mg.mask();
            #else
                        intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
            #endif
                        const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
                        const int maskDim0=mask.getRawDataSize(0);
                        const int maskDim1=mask.getRawDataSize(1);
            #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
      	// setXColour(colourNames[min(grid,numberOfColourNames-1)]);
                  	gi.setColour( GenericGraphicsInterface::textColour); 
                  	glPointSize(psp.pointSize*2.*gi.getLineWidthScaleFactor() );   
                  	glBegin(GL_POINTS);  
      //		    glPointSize(6.*lineWidthScaleFactor[currentWindow]);   
                  	for( i=ip.getBase(0); i<=ip.getBound(0); i++ )
                  	{
                    	  if( numberOfDimensions==2 )
                    	  {
                      	    if( MASK(IP(i,axis1),IP(i,axis2),i3) & CompositeGrid::USESbackupRules )
                      	    {
                        	      glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),i3),
                              			CENTER1(IP(i,axis1),IP(i,axis2),i3),zLevelForPoints+1.e-4);
                      	    }
                    	  }
                    	  else
                    	  {
                      	    if( MASK(IP(i,axis1),i2,i3) & CompositeGrid::USESbackupRules )
                      	    {
                        	      glVertex3(CENTER0(IP(i,axis1),i2,i3),
                              			CENTER1(IP(i,axis1),i2,i3),zLevelForPoints+1.e-4);
                      	    }
                    	  }
                  	}
                  	glEnd();
                        }
                    }
                }
            #undef CENTER0
            #undef CENTER1
            }
            
        }
        else // structured grid
        {
            	      

//        int numberOfBackupInterpolationPoints = sum(mask & CompositeGrid::USESbackupRules);
//        if( true || numberOfBackupInterpolationPoints>0 )
//  	printf("There were %i backup interpolation points on grid %i\n",numberOfBackupInterpolationPoints,grid);

      // -- use the vertex array if we are plotting with an adjustment for the "displacement"
            const bool plotRectangular = mg.isRectangular() && !psp.adjustGridForDisplacement && !(mg->computedGeometry & MappedGrid::THEvertex);
            if( plotRectangular )
            {
      // 	plotStructured(rectangular); // macro
            for( int p=0; p<np; p++ ) 
            {
            #ifndef USE_PPP
                const intSerialArray & mask = mg.mask();
            #else
                intSerialArray mask; 
                intArray maskd;  // holds distributed array that just lives on the graphicsProcessor
                IndexBox pBox;
                const int nd=4;
                Index Jv[nd];
                if( p==graphicsProcessor )
                {
                    getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                }
                else
                {
          // CopyArray::getLocalArrayBoxWithGhost( p, u, pBox ); // get local bounds of the array on processor p 
                    CopyArray::getLocalArrayBox( p, gcl[grid].mask(), pBox ); // get local bounds of the array on processor p 
                    if( pBox.isEmpty() ) continue;
                    for( int d=0; d<3; d++ )	     
                    {
                        int ja=pBox.base(d), jb=pBox.bound(d);
            // copy an extra line on internal ghost boundaries to avoid a gap
            // if( ja>gridIndexRange(0,d) ) ja--;
                        if( jb<gridIndexRange(1,d) ) jb++;
                        Jv[d]=Range(ja,jb);
                    }
                    Jv[3]=Range(0,0);
                    maskd.partition(partition);
                    maskd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
                    ParallelUtility::copy(maskd,Jv,mg.mask(),Jv,nd); // copy data from processor p to graphics processor
                    getLocalArrayWithGhostBoundaries(maskd,mask);
                }
            #endif
                const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
                const int maskDim0=mask.getRawDataSize(0);
                const int maskDim1=mask.getRawDataSize(1);
            #undef MASK
            #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
      //       #If "rectangular" == "rectangular"
                real dx[3],xab[2][3];
                gcl[grid].getRectangularGridParameters( dx, xab );
                const int i0a=gcl[grid].gridIndexRange(0,0);
                const int i1a=gcl[grid].gridIndexRange(0,1);
                const int i2a=gcl[grid].gridIndexRange(0,2);
                const real xa=xab[0][0], dx0=dx[0];
                const real ya=xab[0][1], dy0=dx[1];
                const real za=xab[0][2], dz0=dx[2];
            #define VERTEX0(i0,i1,i2) XSCALE((xa+dx0*(i0-i0a)))
            #define VERTEX1(i0,i1,i2) YSCALE((ya+dy0*(i1-i1a)))
            #define VERTEX2(i0,i1,i2) ZSCALE((za+dz0*(i2-i2a)))
                if( gridsToPlot(grid)&GraphicsParameters::toggleGrids && plotOnThisProcessor )
                {
                    gi.setColour(col);
                    glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                    if( psp.plotGridLines % 2  )
                    {
            //..................Draw Grid Lines...........................
                        glBegin(GL_LINES);
                        for( axis=axis1; axis<=axis2; axis++ )  // draw grid lines parallel to axis
                        {
      	// getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                  	if( gc.refinementLevelNumber(grid)==0 ) // plot ghost lines by default on interpolation boundaries
                    	  getIndex(extendedGridIndexRange(mg),I1,I2,I3,psp.numberOfGhostLinesToPlot);
                  	else  // do not plot ghost lines on interp. boundaries on refinement levels by default.
                    	  getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                            int isv[2], &is1=isv[0], &is2=isv[1];
                  	is1=is2=0; isv[axis]=1;
                  	if( cellVertex )
                  	{
                    	  I1=Range(max(I1.getBase(),dimension(Start,0)),min(I1.getBound()-is1,dimension(End,0)-is1));
                    	  I2=Range(max(I2.getBase(),dimension(Start,1)),min(I2.getBound()-is2,dimension(End,1)-is2));
                  	}
                  	else
                  	{
                    	  I1=Range(max(I1.getBase(),dimension(Start,0)),min(I1.getBound()-is1,dimension(End,0)-is1));
                    	  I2=Range(max(I2.getBase(),dimension(Start,1)),min(I2.getBound()-is2,dimension(End,1)-is2));
      	  // include edge of extended boundaries:
                    	  if( mg.boundaryCondition()(End  ,axis1)==0 )
                      	    I1=Range(I1.getBase(),I1.getBound()+1);
                    	  if( mg.boundaryCondition()(End  ,axis2)==0 )
                      	    I2=Range(I2.getBase(),I2.getBound()+1);
                  	}
                  	if( mg.numberOfDimensions()==2 && psp.numberOfGhostLinesToPlot==0 && psp.plotGridBlockBoundaries)
                  	{
      	  // Alter the Index's so that we do not draw lines on true boundaries, these
      	  // are done later (if the block boundaries are drawn)
                    	  int i1a = mg.boundaryFlag(Start,axis1)==MappedGrid::physicalBoundary ? is2 : 0;
                    	  int i1b = mg.boundaryFlag(End,  axis1)==MappedGrid::physicalBoundary ? is2 : 0;
                    	  int i2a = mg.boundaryFlag(Start,axis2)==MappedGrid::physicalBoundary ? is1 : 0;
                    	  int i2b = mg.boundaryFlag(End  ,axis2)==MappedGrid::physicalBoundary ? is1 : 0;
                    	  I1=Range(I1.getBase()+i1a,I1.getBound()-i1b); 
                    	  I2=Range(I2.getBase()+i2a,I2.getBound()-i2b); 
                  	}
                            #ifdef USE_PPP
      // 	const int includeGhost=1;
      // 	bool ok = ParallelUtility::getLocalArrayBounds(gc[grid].mask(),mask,I1,I2,I3,includeGhost);
      // 	if( !ok ) continue;
                            bool ok=true;
                  	for( int d=0; d<numberOfDimensions; d++ )
                  	{
                    	  int ia=max(mask.getBase(d),Iv[d].getBase()), ib=min(mask.getBound(d)-isv[d],Iv[d].getBound());
                    	  if( ia<=ib )
                    	  {
                                    Iv[d]=Range(ia,ib);
                    	  }
                    	  else
                    	  {
                      	    ok=false;
                      	    break;
                    	  }
                  	}
                  	if( !ok ) continue;
                            #endif
                  	if( mg.numberOfDimensions()==2 )
                  	{
                    	  if( cellVertex )
                    	  {
                      	    intSerialArray cMask=mask;   // use a macro for CMASK instead of making a copy ?? 
                      	    int *cMaskp = cMask.Array_Descriptor.Array_View_Pointer2;
                      	    const int cMaskDim0=cMask.getRawDataSize(0);
                      	    const int cMaskDim1=cMask.getRawDataSize(1);
            #define CMASK(i0,i1,i2) cMaskp[i0+cMaskDim0*(i1+cMaskDim1*(i2))]
                      	    if( plotInterpolationCells )
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        		CMASK(i1,i2,i3)= MASK(i1,i2,i3)!=0 && MASK(i1+is1,i2+is2,i3)!=0;
                      	    }
                      	    else
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        		CMASK(i1,i2,i3)=MASK(i1,i2,i3)>0  && MASK(i1+is1,i2+is2,i3)>0 ;
                      	    }
                      	    if( gc.numberOfRefinementLevels()>1 )
                      	    {
      	      // cMask(I1,I2,I3) = cMask(I1,I2,I3) && !( mask(I1,I2,I3)&isHiddenByRefinement || 
      	      //	mask(I1+is1,I2+is2,I3)&isHiddenByRefinement);
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		CMASK(i1,i2,i3) = CMASK(i1,i2,i3) && 
      		  !( MASK(i1,i2,i3)&isHiddenByRefinement || 
                             		     MASK(i1+is1,i2+is2,i3)&isHiddenByRefinement);
                        	      }
                      	    }
                      	    if( abs(gi.gridCoarseningFactor)==1 )
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( CMASK(i1,i2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),zLevelFor2DGrids );
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),zLevelFor2DGrids );
                        		}
                        	      }
                      	    }
                      	    else // coarsen factor >1 
                      	    {
      	      // strides: (m1,m2,m3)
                        	      const int m1=abs(gi.gridCoarseningFactor); 
                        	      const int m2=abs(gi.gridCoarseningFactor);
                        	      const int m3=1;
                        	      const int ms1=m1*is1;
                        	      const int ms2=m2*is2;
                        	      int i1Bound,i2Bound,i3Bound;
                        	      FOR_3WithStride(i1,i2,i3,m1,m2,m3,I1,I2,I3) 
                        	      {
      		// Check that all sub-lines can be plotted, only then plot the coarser line segment
      		// ** watch out for ends
                        		bool ok=true && (i1+m1-1)<=i1Bound && (i2+m2-1)<=i2Bound;
                        		int j1,j2,j3=i3;
                        		for( j2=i2; j2<=i2+m2 && ok ; j2++ )
                          		  for( j1=i1; j1<=i1+m1; j1++ )
                          		  {
                            		    if( !CMASK(j1,j2,j3) )
                            		    {
                              		      ok=false;
                              		      break;
                            		    }
                          		  }
                        		if( ok )
                        		{
      		  // plot longer line segment
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),zLevelFor2DGrids );
                          		  glVertex3( VERTEX0(i1+ms1,i2+ms2,i3),VERTEX1(i1+ms1,i2+ms2,i3),zLevelFor2DGrids );
                        		}
                        		else
                        		{
      		  // plot individual line segements
                          		  const int i2b = min(i2+m2-1,i2Bound);
                          		  const int i1b = min(i1+m1-1,i1Bound);
                          		  for( j2=i2; j2<=i2b; j2++ )
                            		    for( j1=i1; j1<=i1b; j1++ )
                            		    {
                              		      if( CMASK(j1,j2,j3) )
                              		      {
                              			glVertex3( VERTEX0(j1    ,j2    ,j3),VERTEX1(j1    ,j2    ,j3),zLevelFor2DGrids );
                              			glVertex3( VERTEX0(j1+is1,j2+is2,j3),VERTEX1(j1+is1,j2+is2,j3),zLevelFor2DGrids );
                              		      }
                            		    }
                        		}
                        	      }
                      	    } // end coarsening factor >1 
                    	  }
                    	  else
                    	  {
      	    // cell centered
                      	    i3=I3.getBase();
                      	    int i2m = max(I2.getBase()-is1,mg.dimension(Start,1));
                      	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
                      	    {
                        	      int i1m = max(I1.getBase()-is2,mg.dimension(Start,0));
                        	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
                        	      {
                        		bool plotLine = plotInterpolationCells ?
                          		  MASK_CNR(i1,i2,i3) || MASK_CNR(i1m,i2m,i3) :
                          		  MASK_DNR(i1,i2,i3) && MASK_DNR(i1m,i2m,i3);
                        		if( gc.numberOfRefinementLevels()>1 )
                          		  plotLine = plotLine && ! (mask(i1    ,i2    ,i3)&isHiddenByRefinement);
                        		if( plotLine )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),zLevelFor2DGrids );
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),zLevelFor2DGrids );
                        		}
                        		i1m=i1+1-is2;
                        	      }
                        	      i2m=i2+1-is1;
                      	    }
                    	  }
                  	}
                  	else
                  	{
      	  // ** 1D ***
                    	  if( cellVertex )
                    	  {
                      	    Index J1=Range(mg.dimension(0,0),mg.dimension(1,0)-1);
                      	    I2=Range(0,0), I3=Range(0,0);
                      	    intSerialArray cMask=mask;
                      	    int *cMaskp = cMask.Array_Descriptor.Array_View_Pointer2;
                      	    const int cMaskDim0=cMask.getRawDataSize(0);
                      	    const int cMaskDim1=cMask.getRawDataSize(1);
                      	    cMask(J1,0,0)=plotInterpolationCells ? mask(J1,I2,I3)!=0 && mask(J1+is1,I2,I3)!=0 
                        	      : mask(J1,I2,I3)>0  && mask(J1+is1,I2,I3)>0 ;
                      	    if( gc.numberOfRefinementLevels()>1 )
                        	      cMask(J1,0,0) = cMask(J1,0,0) && !( mask(J1    ,I2    ,I3)&isHiddenByRefinement || 
                                                  						  mask(J1+is1,I2    ,I3)&isHiddenByRefinement);
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( CMASK(i1,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1    ,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1+is1,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        	      }
                      	    }
      	    // mark grid points in 1D
                      	    getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                      	    real dy=(xBound(End,axis1)-xBound(Start,axis1))*.01;
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( CMASK(i1,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids-dy,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids+dy,zLevelFor2DGrids );
                        	      }
                      	    }
                    	  }
                    	  else
                    	  {
      	    // cell-centered
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( MASK_CNR(i1,i2,i3) || MASK_CNR(i1-is2,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1    ,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1+is1,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        	      }
                      	    }
      	    // mark grid points in 1D
                      	    getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                      	    real dy=(xBound(End,axis1)-xBound(Start,axis1))*.01;
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( MASK_CNR(i1,i2,i3) || MASK_CNR(i1-is2,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids-dy,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids+dy,zLevelFor2DGrids );
                        	      }
                      	    }
                    	  }
                  	}
                        }
                        glEnd();     // GL_LINES
                    }  // if( plotGridLines %2 )
                }
        // -------------------------Label Boundaries------------------------------------
                if( labelBoundaries && plotOnThisProcessor 
                        && np<=1 ) // fix for parallel
                {
                    aString buff;
                    int side,axis;
                    ForBoundary(side,axis)
                    {
                        if( mg.boundaryCondition(side,axis) > 0 )
                        {
                  	getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3);
                  	const int includeGhost=1;
                  	bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);
                  	if( !ok ) continue;
                  	i1=(int).5*(I1.getBase()+I1.getBound());
                  	i2=(int).5*(I2.getBase()+I2.getBound());
                  	i3=(int).5*(I3.getBase()+I3.getBound());
                  	gi.xLabel(sPrintF(buff,"bc=%i",mg.boundaryCondition(side,axis)),
                          		  VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),.1,0,0.,psp);
                        }
                    }
                }
        // plotInterpolationPoints(rectangular);
          // ----------------------Interpolation Points------------------------------------
                    if( (plotInterpolationPoints || plotBackupInterpolationPoints ) && 
                            (gridOptions(grid)&GraphicsParameters::plotInterpolation) &&
                              numberOfGrids >= 1 && 
                              gc.getClassName()=="CompositeGrid"  )
                    {
                        CompositeGrid & cg0 = (CompositeGrid &)gc;
                        CompositeGrid & cg = multigridLevelToPlot==0 ? cg0 : cg0.multigridLevel[multigridLevelToPlot];
            // RealDistributedArray & coord = (bool)cg[grid].isAllVertexCentered() ? cg[grid].vertex() : cg[grid].center(); 
            //           #If "rectangular" == "rectangular"
                        i2=gcl[grid].gridIndexRange(0,1); // added 030714 : i3 needs to be given a default value
                        i3=gcl[grid].gridIndexRange(0,2);
          // printf(" gridPlot: cg.interpolationPoint.getLength()=%i\n",cg.interpolationPoint.getLength());
                    if( grid>=cg.interpolationPoint.getLength() )  // *wdh* 2012/03/02 -- if a refinement has just been added
                        continue;
                      #ifndef USE_PPP
                            const intSerialArray & ip = cg.interpolationPoint[grid];
                            const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
                      #else
                      intSerialArray ip,interpoleeGrid;
           // if( !( p==1 && grid==1)  ) continue; // ********************************8
        //    fflush(0);
        //    Communication_Manager::Sync();
           // *new* way *wdh* 090808
                      collectInterpolationData( p,graphicsProcessor, grid,cg, ip,interpoleeGrid );
        //     bool useLocal = !( 
        //       (grid<cg.numberOfBaseGrids() && 
        //           cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
        //       cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData );
        //     if(  myid==p )
        //     { // We will use the interpolation data from processor p: 
        //       if( !useLocal )
        //       {
        // 	ip.reference(cg.interpolationPoint[grid].getLocalArray());
        // 	interpoleeGrid.reference(cg.interpoleeGrid[grid].getLocalArray());
        //       }
        //       else
        //       {
        // 	ip.reference(cg->interpolationPointLocal[grid]);
        // 	interpoleeGrid.reference(cg->interpoleeGridLocal[grid]);
        //       }
        //     }
                        if( p!=graphicsProcessor )
                        {
              // copy the interpolation data from processor "p" to processor "graphicsProcessor"
        //       CopyArray::copyArray( ip,graphicsProcessor, ip,p );
        //       CopyArray::copyArray( interpoleeGrid,graphicsProcessor, interpoleeGrid,p );
              // -- for vertex centered grids we can re-use the VERTEX, otherwise we need the CENTER  ** fix me **
            //               #If "rectangular" == "curvilinear"
                        }
                        if( false && myid==graphicsProcessor )
                        {
                            printF(" plotInterpolationPoints: plot data from p=%i \n",p);
                            ::display(ip,"ip","%4i ");
                            ::display(interpoleeGrid,"interpoleeGrid","%4i ");
              //               #If "rectangular" == "curvilinear"
                        }
                      #endif
                        if( ip.getLength(0)>0 )
                        {
              // ::display(ip,"gridPlot: ip");
            //         #If "rectangular" == "rectangular"
                            real dx[3],xab[2][3];
                            gcl[grid].getRectangularGridParameters( dx, xab );
                            const int i0a=gcl[grid].gridIndexRange(0,0);
                            const int i1a=gcl[grid].gridIndexRange(0,1);
                            const int i2a=gcl[grid].gridIndexRange(0,2);
                            real xa=xab[0][0], dx0=dx[0];
                            real ya=xab[0][1], dy0=dx[1];
                            if( cellCentre )
                            {
                      	xa+=dx0*.5;
                      	ya+=dy0*.5;
                            }
                #define CENTER0(i0,i1,i2) XSCALE(xa+dx0*(i0-i0a))
                #define CENTER1(i0,i1,i2) YSCALE(ya+dy0*(i1-i1a))
                            const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
                            const int ipDim0=ip.getRawDataSize(0);
                #define IP(i0,i1) ipp[i0+ipDim0*(i1)]
                            const int *interpoleeGridp = interpoleeGrid.Array_Descriptor.Array_View_Pointer0;
                            const int interpoleeGridDim0=interpoleeGrid.getRawDataSize(0);
                #define INTERPOLEEGRID(i0) interpoleeGridp[i0]
              // offset interp points on refinement grids so we see them instead of the lower levels
                            const int level = gcl.refinementLevelNumber(grid);
                            const real offset = 1.e-3;  // use this for now so refinement grid interp points appear on top
                            const real zLevelForPoints = zLevelFor2DGrids + level*offset;
                            if( plotInterpolationPoints && plotOnThisProcessor )
                            {
                      	gi.setColour(GenericGraphicsInterface::textColour);
                      	glPointSize(psp.pointSize*1.67*gi.getLineWidthScaleFactor() );   
                      	glBegin(GL_POINTS);  
        //		    glPointSize(5.*lineWidthScaleFactor[currentWindow]);   
        //              printf("plotInterpolationPoints: grid=%i num=%i ptSize=%8.2e -> %8.2e \n",
        //  		   grid,cg.numberOfInterpolationPoints(grid),
        //                     (real)psp.pointSize,psp.pointSize*1.67*gi.getLineWidthScaleFactor());
        	// printf(" gridPlot: grid=%i ni=%i\n",grid,cg.numberOfInterpolationPoints(grid));
                      	int oldInterpolationPointColour=-1;
                      	for( i=ip.getBase(0); i<=ip.getBound(0); i++ )
                      	{
        	  // printf(" gridPlot: grid=%i i=%i, ip=(%i,%i) donor=%i\n",grid,i,IP(i,axis1),IP(i,axis2),
        	  //        INTERPOLEEGRID(i));
                        	  if( psp.colourInterpolationPoints && INTERPOLEEGRID(i)!=oldInterpolationPointColour )
                        	  {
        	    // colour the interpolation the same colour as the grid it interpolates from
                          	    oldInterpolationPointColour=INTERPOLEEGRID(i);
                          	    setXColour(gi.getColourName( (oldInterpolationPointColour %
                                                					  GenericGraphicsInterface::numberOfColourNames) ));
        // 		setXColour(gi.getColourName( min(oldInterpolationPointColour,
        // 						 GenericGraphicsInterface::numberOfColourNames-1)) );
                        	  }
        	  // printf(" plot pt i=%i, level=%i, zLevelForPoints=%g\n",i,level,zLevelForPoints);
                        	  if( numberOfDimensions==2 )
                          	    glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),i3),
                                  		      CENTER1(IP(i,axis1),IP(i,axis2),i3),zLevelForPoints);
                        	  else if( numberOfDimensions==3 )
                          	    glVertex3(CENTER0(IP(i,axis1),i2,i3),
                                  		      CENTER1(IP(i,axis1),i2,i3),zLevelForPoints);
                        	  else 
                          	    glVertex3(CENTER0(IP(i,axis1),i2,i3),yLevelFor1DGrids,zLevelForPoints);
                      	}
                      	glEnd();
                            }
                            if( plotBackupInterpolationPoints && plotOnThisProcessor )
                            {
        	// Now plot points that use back up interpolation
                #ifndef USE_PPP
                            const intSerialArray & mask = mg.mask();
                #else
                            intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                #endif
                            const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
                            const int maskDim0=mask.getRawDataSize(0);
                            const int maskDim1=mask.getRawDataSize(1);
                #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
        	// setXColour(colourNames[min(grid,numberOfColourNames-1)]);
                      	gi.setColour( GenericGraphicsInterface::textColour); 
                      	glPointSize(psp.pointSize*2.*gi.getLineWidthScaleFactor() );   
                      	glBegin(GL_POINTS);  
        //		    glPointSize(6.*lineWidthScaleFactor[currentWindow]);   
                      	for( i=ip.getBase(0); i<=ip.getBound(0); i++ )
                      	{
                        	  if( numberOfDimensions==2 )
                        	  {
                          	    if( MASK(IP(i,axis1),IP(i,axis2),i3) & CompositeGrid::USESbackupRules )
                          	    {
                            	      glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),i3),
                                  			CENTER1(IP(i,axis1),IP(i,axis2),i3),zLevelForPoints+1.e-4);
                          	    }
                        	  }
                        	  else
                        	  {
                          	    if( MASK(IP(i,axis1),i2,i3) & CompositeGrid::USESbackupRules )
                          	    {
                            	      glVertex3(CENTER0(IP(i,axis1),i2,i3),
                                  			CENTER1(IP(i,axis1),i2,i3),zLevelForPoints+1.e-4);
                          	    }
                        	  }
                      	}
                      	glEnd();
                            }
                        }
                    }
                #undef CENTER0
                #undef CENTER1
            } // end for p=0... np-1
            #undef VERTEX0
            #undef VERTEX1
            #undef VERTEX2
            }
            else
            {
      // 	plotStructured(curvilinear); // macro
            for( int p=0; p<np; p++ ) 
            {
            #ifndef USE_PPP
                const intSerialArray & mask = mg.mask();
            #else
                intSerialArray mask; 
                intArray maskd;  // holds distributed array that just lives on the graphicsProcessor
                IndexBox pBox;
                const int nd=4;
                Index Jv[nd];
                if( p==graphicsProcessor )
                {
                    getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                }
                else
                {
          // CopyArray::getLocalArrayBoxWithGhost( p, u, pBox ); // get local bounds of the array on processor p 
                    CopyArray::getLocalArrayBox( p, gcl[grid].mask(), pBox ); // get local bounds of the array on processor p 
                    if( pBox.isEmpty() ) continue;
                    for( int d=0; d<3; d++ )	     
                    {
                        int ja=pBox.base(d), jb=pBox.bound(d);
            // copy an extra line on internal ghost boundaries to avoid a gap
            // if( ja>gridIndexRange(0,d) ) ja--;
                        if( jb<gridIndexRange(1,d) ) jb++;
                        Jv[d]=Range(ja,jb);
                    }
                    Jv[3]=Range(0,0);
                    maskd.partition(partition);
                    maskd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
                    ParallelUtility::copy(maskd,Jv,mg.mask(),Jv,nd); // copy data from processor p to graphics processor
                    getLocalArrayWithGhostBoundaries(maskd,mask);
                }
            #endif
                const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
                const int maskDim0=mask.getRawDataSize(0);
                const int maskDim1=mask.getRawDataSize(1);
            #undef MASK
            #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
      //       #If "curvilinear" == "rectangular"
      //       #Else    
                #ifndef USE_PPP
                    const realSerialArray & vertex = mg.vertex();
                #else
                    realSerialArray vertex; 
                    realArray vertexd; // holds distributed array that just lives on the graphicsProcessor
                    if( p==graphicsProcessor )
                    {
                        getLocalArrayWithGhostBoundaries(mg.vertex(),vertex);
                    }
                    else
                    {
                        Jv[3]=Range(0,gc.numberOfDimensions()-1); // copy (x,y,z)
                        vertexd.partition(partition);
                        vertexd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
                        ParallelUtility::copy(vertexd,Jv,mg.vertex(),Jv,nd); // copy data from processor p to graphics processor
                        getLocalArrayWithGhostBoundaries(vertexd,vertex);
                    }
                #endif
                const real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
                const int vertexDim0=vertex.getRawDataSize(0);
                const int vertexDim1=vertex.getRawDataSize(1);
                const int vertexDim2=vertex.getRawDataSize(2);
            #define VERTEX0(i0,i1,i2) XSCALE(vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(0)))])
            #define VERTEX1(i0,i1,i2) YSCALE(vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(1)))])
            #define VERTEX2(i0,i1,i2) ZSCALE(vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(2)))])
      // define VERTEX(i0,i1,i2,i3) vertexp[i0+vertexDim0*(i1+vertexDim1*(i2+vertexDim2*(i3)))]
                if( gridsToPlot(grid)&GraphicsParameters::toggleGrids && plotOnThisProcessor )
                {
                    gi.setColour(col);
                    glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                    if( psp.plotGridLines % 2  )
                    {
            //..................Draw Grid Lines...........................
                        glBegin(GL_LINES);
                        for( axis=axis1; axis<=axis2; axis++ )  // draw grid lines parallel to axis
                        {
      	// getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                  	if( gc.refinementLevelNumber(grid)==0 ) // plot ghost lines by default on interpolation boundaries
                    	  getIndex(extendedGridIndexRange(mg),I1,I2,I3,psp.numberOfGhostLinesToPlot);
                  	else  // do not plot ghost lines on interp. boundaries on refinement levels by default.
                    	  getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                            int isv[2], &is1=isv[0], &is2=isv[1];
                  	is1=is2=0; isv[axis]=1;
                  	if( cellVertex )
                  	{
                    	  I1=Range(max(I1.getBase(),dimension(Start,0)),min(I1.getBound()-is1,dimension(End,0)-is1));
                    	  I2=Range(max(I2.getBase(),dimension(Start,1)),min(I2.getBound()-is2,dimension(End,1)-is2));
                  	}
                  	else
                  	{
                    	  I1=Range(max(I1.getBase(),dimension(Start,0)),min(I1.getBound()-is1,dimension(End,0)-is1));
                    	  I2=Range(max(I2.getBase(),dimension(Start,1)),min(I2.getBound()-is2,dimension(End,1)-is2));
      	  // include edge of extended boundaries:
                    	  if( mg.boundaryCondition()(End  ,axis1)==0 )
                      	    I1=Range(I1.getBase(),I1.getBound()+1);
                    	  if( mg.boundaryCondition()(End  ,axis2)==0 )
                      	    I2=Range(I2.getBase(),I2.getBound()+1);
                  	}
                  	if( mg.numberOfDimensions()==2 && psp.numberOfGhostLinesToPlot==0 && psp.plotGridBlockBoundaries)
                  	{
      	  // Alter the Index's so that we do not draw lines on true boundaries, these
      	  // are done later (if the block boundaries are drawn)
                    	  int i1a = mg.boundaryFlag(Start,axis1)==MappedGrid::physicalBoundary ? is2 : 0;
                    	  int i1b = mg.boundaryFlag(End,  axis1)==MappedGrid::physicalBoundary ? is2 : 0;
                    	  int i2a = mg.boundaryFlag(Start,axis2)==MappedGrid::physicalBoundary ? is1 : 0;
                    	  int i2b = mg.boundaryFlag(End  ,axis2)==MappedGrid::physicalBoundary ? is1 : 0;
                    	  I1=Range(I1.getBase()+i1a,I1.getBound()-i1b); 
                    	  I2=Range(I2.getBase()+i2a,I2.getBound()-i2b); 
                  	}
                            #ifdef USE_PPP
      // 	const int includeGhost=1;
      // 	bool ok = ParallelUtility::getLocalArrayBounds(gc[grid].mask(),mask,I1,I2,I3,includeGhost);
      // 	if( !ok ) continue;
                            bool ok=true;
                  	for( int d=0; d<numberOfDimensions; d++ )
                  	{
                    	  int ia=max(mask.getBase(d),Iv[d].getBase()), ib=min(mask.getBound(d)-isv[d],Iv[d].getBound());
                    	  if( ia<=ib )
                    	  {
                                    Iv[d]=Range(ia,ib);
                    	  }
                    	  else
                    	  {
                      	    ok=false;
                      	    break;
                    	  }
                  	}
                  	if( !ok ) continue;
                            #endif
                  	if( mg.numberOfDimensions()==2 )
                  	{
                    	  if( cellVertex )
                    	  {
                      	    intSerialArray cMask=mask;   // use a macro for CMASK instead of making a copy ?? 
                      	    int *cMaskp = cMask.Array_Descriptor.Array_View_Pointer2;
                      	    const int cMaskDim0=cMask.getRawDataSize(0);
                      	    const int cMaskDim1=cMask.getRawDataSize(1);
            #define CMASK(i0,i1,i2) cMaskp[i0+cMaskDim0*(i1+cMaskDim1*(i2))]
                      	    if( plotInterpolationCells )
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        		CMASK(i1,i2,i3)= MASK(i1,i2,i3)!=0 && MASK(i1+is1,i2+is2,i3)!=0;
                      	    }
                      	    else
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        		CMASK(i1,i2,i3)=MASK(i1,i2,i3)>0  && MASK(i1+is1,i2+is2,i3)>0 ;
                      	    }
                      	    if( gc.numberOfRefinementLevels()>1 )
                      	    {
      	      // cMask(I1,I2,I3) = cMask(I1,I2,I3) && !( mask(I1,I2,I3)&isHiddenByRefinement || 
      	      //	mask(I1+is1,I2+is2,I3)&isHiddenByRefinement);
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		CMASK(i1,i2,i3) = CMASK(i1,i2,i3) && 
      		  !( MASK(i1,i2,i3)&isHiddenByRefinement || 
                             		     MASK(i1+is1,i2+is2,i3)&isHiddenByRefinement);
                        	      }
                      	    }
                      	    if( abs(gi.gridCoarseningFactor)==1 )
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( CMASK(i1,i2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),zLevelFor2DGrids );
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),zLevelFor2DGrids );
                        		}
                        	      }
                      	    }
                      	    else // coarsen factor >1 
                      	    {
      	      // strides: (m1,m2,m3)
                        	      const int m1=abs(gi.gridCoarseningFactor); 
                        	      const int m2=abs(gi.gridCoarseningFactor);
                        	      const int m3=1;
                        	      const int ms1=m1*is1;
                        	      const int ms2=m2*is2;
                        	      int i1Bound,i2Bound,i3Bound;
                        	      FOR_3WithStride(i1,i2,i3,m1,m2,m3,I1,I2,I3) 
                        	      {
      		// Check that all sub-lines can be plotted, only then plot the coarser line segment
      		// ** watch out for ends
                        		bool ok=true && (i1+m1-1)<=i1Bound && (i2+m2-1)<=i2Bound;
                        		int j1,j2,j3=i3;
                        		for( j2=i2; j2<=i2+m2 && ok ; j2++ )
                          		  for( j1=i1; j1<=i1+m1; j1++ )
                          		  {
                            		    if( !CMASK(j1,j2,j3) )
                            		    {
                              		      ok=false;
                              		      break;
                            		    }
                          		  }
                        		if( ok )
                        		{
      		  // plot longer line segment
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),zLevelFor2DGrids );
                          		  glVertex3( VERTEX0(i1+ms1,i2+ms2,i3),VERTEX1(i1+ms1,i2+ms2,i3),zLevelFor2DGrids );
                        		}
                        		else
                        		{
      		  // plot individual line segements
                          		  const int i2b = min(i2+m2-1,i2Bound);
                          		  const int i1b = min(i1+m1-1,i1Bound);
                          		  for( j2=i2; j2<=i2b; j2++ )
                            		    for( j1=i1; j1<=i1b; j1++ )
                            		    {
                              		      if( CMASK(j1,j2,j3) )
                              		      {
                              			glVertex3( VERTEX0(j1    ,j2    ,j3),VERTEX1(j1    ,j2    ,j3),zLevelFor2DGrids );
                              			glVertex3( VERTEX0(j1+is1,j2+is2,j3),VERTEX1(j1+is1,j2+is2,j3),zLevelFor2DGrids );
                              		      }
                            		    }
                        		}
                        	      }
                      	    } // end coarsening factor >1 
                    	  }
                    	  else
                    	  {
      	    // cell centered
                      	    i3=I3.getBase();
                      	    int i2m = max(I2.getBase()-is1,mg.dimension(Start,1));
                      	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
                      	    {
                        	      int i1m = max(I1.getBase()-is2,mg.dimension(Start,0));
                        	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
                        	      {
                        		bool plotLine = plotInterpolationCells ?
                          		  MASK_CNR(i1,i2,i3) || MASK_CNR(i1m,i2m,i3) :
                          		  MASK_DNR(i1,i2,i3) && MASK_DNR(i1m,i2m,i3);
                        		if( gc.numberOfRefinementLevels()>1 )
                          		  plotLine = plotLine && ! (mask(i1    ,i2    ,i3)&isHiddenByRefinement);
                        		if( plotLine )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),zLevelFor2DGrids );
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),zLevelFor2DGrids );
                        		}
                        		i1m=i1+1-is2;
                        	      }
                        	      i2m=i2+1-is1;
                      	    }
                    	  }
                  	}
                  	else
                  	{
      	  // ** 1D ***
                    	  if( cellVertex )
                    	  {
                      	    Index J1=Range(mg.dimension(0,0),mg.dimension(1,0)-1);
                      	    I2=Range(0,0), I3=Range(0,0);
                      	    intSerialArray cMask=mask;
                      	    int *cMaskp = cMask.Array_Descriptor.Array_View_Pointer2;
                      	    const int cMaskDim0=cMask.getRawDataSize(0);
                      	    const int cMaskDim1=cMask.getRawDataSize(1);
                      	    cMask(J1,0,0)=plotInterpolationCells ? mask(J1,I2,I3)!=0 && mask(J1+is1,I2,I3)!=0 
                        	      : mask(J1,I2,I3)>0  && mask(J1+is1,I2,I3)>0 ;
                      	    if( gc.numberOfRefinementLevels()>1 )
                        	      cMask(J1,0,0) = cMask(J1,0,0) && !( mask(J1    ,I2    ,I3)&isHiddenByRefinement || 
                                                  						  mask(J1+is1,I2    ,I3)&isHiddenByRefinement);
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( CMASK(i1,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1    ,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1+is1,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        	      }
                      	    }
      	    // mark grid points in 1D
                      	    getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                      	    real dy=(xBound(End,axis1)-xBound(Start,axis1))*.01;
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( CMASK(i1,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids-dy,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids+dy,zLevelFor2DGrids );
                        	      }
                      	    }
                    	  }
                    	  else
                    	  {
      	    // cell-centered
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( MASK_CNR(i1,i2,i3) || MASK_CNR(i1-is2,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1    ,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1+is1,i2,i3),yLevelFor1DGrids,zLevelFor2DGrids );
                        	      }
                      	    }
      	    // mark grid points in 1D
                      	    getIndex(gridIndexRange,I1,I2,I3,psp.numberOfGhostLinesToPlot);
                      	    real dy=(xBound(End,axis1)-xBound(Start,axis1))*.01;
                      	    FOR_3(i1,i2,i3,I1,I2,I3)
                      	    {
                        	      if( MASK_CNR(i1,i2,i3) || MASK_CNR(i1-is2,i2,i3) )
                        	      {
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids-dy,zLevelFor2DGrids );
                        		glVertex3( VERTEX0(i1,i2,i3),yLevelFor1DGrids+dy,zLevelFor2DGrids );
                        	      }
                      	    }
                    	  }
                  	}
                        }
                        glEnd();     // GL_LINES
                    }  // if( plotGridLines %2 )
                }
        // -------------------------Label Boundaries------------------------------------
                if( labelBoundaries && plotOnThisProcessor 
                        && np<=1 ) // fix for parallel
                {
                    aString buff;
                    int side,axis;
                    ForBoundary(side,axis)
                    {
                        if( mg.boundaryCondition(side,axis) > 0 )
                        {
                  	getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3);
                  	const int includeGhost=1;
                  	bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);
                  	if( !ok ) continue;
                  	i1=(int).5*(I1.getBase()+I1.getBound());
                  	i2=(int).5*(I2.getBase()+I2.getBound());
                  	i3=(int).5*(I3.getBase()+I3.getBound());
                  	gi.xLabel(sPrintF(buff,"bc=%i",mg.boundaryCondition(side,axis)),
                          		  VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),.1,0,0.,psp);
                        }
                    }
                }
        // plotInterpolationPoints(curvilinear);
          // ----------------------Interpolation Points------------------------------------
                    if( (plotInterpolationPoints || plotBackupInterpolationPoints ) && 
                            (gridOptions(grid)&GraphicsParameters::plotInterpolation) &&
                              numberOfGrids >= 1 && 
                              gc.getClassName()=="CompositeGrid"  )
                    {
                        CompositeGrid & cg0 = (CompositeGrid &)gc;
                        CompositeGrid & cg = multigridLevelToPlot==0 ? cg0 : cg0.multigridLevel[multigridLevelToPlot];
            // RealDistributedArray & coord = (bool)cg[grid].isAllVertexCentered() ? cg[grid].vertex() : cg[grid].center(); 
            //           #If "curvilinear" == "rectangular"
            //           #Else
                        cg[grid].update(MappedGrid::THEcenter);
                        #ifndef USE_PPP
                            const realSerialArray & center = cg[grid].center(); 
                        #else
                            realSerialArray center; getLocalArrayWithGhostBoundaries(cg[grid].center(),center);
                        #endif
                        i2=center.getBase(1);
                        i3=center.getBase(2);
          // printf(" gridPlot: cg.interpolationPoint.getLength()=%i\n",cg.interpolationPoint.getLength());
                    if( grid>=cg.interpolationPoint.getLength() )  // *wdh* 2012/03/02 -- if a refinement has just been added
                        continue;
                      #ifndef USE_PPP
                            const intSerialArray & ip = cg.interpolationPoint[grid];
                            const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
                      #else
                      intSerialArray ip,interpoleeGrid;
           // if( !( p==1 && grid==1)  ) continue; // ********************************8
        //    fflush(0);
        //    Communication_Manager::Sync();
           // *new* way *wdh* 090808
                      collectInterpolationData( p,graphicsProcessor, grid,cg, ip,interpoleeGrid );
        //     bool useLocal = !( 
        //       (grid<cg.numberOfBaseGrids() && 
        //           cg->localInterpolationDataState==CompositeGridData::localInterpolationDataForAMR ) || 
        //       cg->localInterpolationDataState==CompositeGridData::noLocalInterpolationData );
        //     if(  myid==p )
        //     { // We will use the interpolation data from processor p: 
        //       if( !useLocal )
        //       {
        // 	ip.reference(cg.interpolationPoint[grid].getLocalArray());
        // 	interpoleeGrid.reference(cg.interpoleeGrid[grid].getLocalArray());
        //       }
        //       else
        //       {
        // 	ip.reference(cg->interpolationPointLocal[grid]);
        // 	interpoleeGrid.reference(cg->interpoleeGridLocal[grid]);
        //       }
        //     }
                        if( p!=graphicsProcessor )
                        {
              // copy the interpolation data from processor "p" to processor "graphicsProcessor"
        //       CopyArray::copyArray( ip,graphicsProcessor, ip,p );
        //       CopyArray::copyArray( interpoleeGrid,graphicsProcessor, interpoleeGrid,p );
              // -- for vertex centered grids we can re-use the VERTEX, otherwise we need the CENTER  ** fix me **
            //               #If "curvilinear" == "curvilinear"
                                if( cellVertex )
                                { // node centered -- we can reuse the vertex array 
                            	  center.reference(vertex);
                                }
                                else
                                {
        	  // cell-centered : we need the center array which holds the coords of cell centers
                        	  CopyArray::copyArray( center,graphicsProcessor, center,p );
                                }
                                i2=center.getBase(1);
                                i3=center.getBase(2);
                        }
                        if( false && myid==graphicsProcessor )
                        {
                            printF(" plotInterpolationPoints: plot data from p=%i \n",p);
                            ::display(ip,"ip","%4i ");
                            ::display(interpoleeGrid,"interpoleeGrid","%4i ");
              //               #If "curvilinear" == "curvilinear"
                                ::display(center,"center","%5.2f ");
                        }
                      #endif
                        if( ip.getLength(0)>0 )
                        {
              // ::display(ip,"gridPlot: ip");
            //         #If "curvilinear" == "rectangular"
            //         #Else    
                            const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
                            const int centerDim0=center.getRawDataSize(0);
                            const int centerDim1=center.getRawDataSize(1);
                            const int centerDim2=center.getRawDataSize(2);
                #define CENTER0(i0,i1,i2) XSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(0)))])
                #define CENTER1(i0,i1,i2) YSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(1)))])
        // define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]
                            const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
                            const int ipDim0=ip.getRawDataSize(0);
                #define IP(i0,i1) ipp[i0+ipDim0*(i1)]
                            const int *interpoleeGridp = interpoleeGrid.Array_Descriptor.Array_View_Pointer0;
                            const int interpoleeGridDim0=interpoleeGrid.getRawDataSize(0);
                #define INTERPOLEEGRID(i0) interpoleeGridp[i0]
              // offset interp points on refinement grids so we see them instead of the lower levels
                            const int level = gcl.refinementLevelNumber(grid);
                            const real offset = 1.e-3;  // use this for now so refinement grid interp points appear on top
                            const real zLevelForPoints = zLevelFor2DGrids + level*offset;
                            if( plotInterpolationPoints && plotOnThisProcessor )
                            {
                      	gi.setColour(GenericGraphicsInterface::textColour);
                      	glPointSize(psp.pointSize*1.67*gi.getLineWidthScaleFactor() );   
                      	glBegin(GL_POINTS);  
        //		    glPointSize(5.*lineWidthScaleFactor[currentWindow]);   
        //              printf("plotInterpolationPoints: grid=%i num=%i ptSize=%8.2e -> %8.2e \n",
        //  		   grid,cg.numberOfInterpolationPoints(grid),
        //                     (real)psp.pointSize,psp.pointSize*1.67*gi.getLineWidthScaleFactor());
        	// printf(" gridPlot: grid=%i ni=%i\n",grid,cg.numberOfInterpolationPoints(grid));
                      	int oldInterpolationPointColour=-1;
                      	for( i=ip.getBase(0); i<=ip.getBound(0); i++ )
                      	{
        	  // printf(" gridPlot: grid=%i i=%i, ip=(%i,%i) donor=%i\n",grid,i,IP(i,axis1),IP(i,axis2),
        	  //        INTERPOLEEGRID(i));
                        	  if( psp.colourInterpolationPoints && INTERPOLEEGRID(i)!=oldInterpolationPointColour )
                        	  {
        	    // colour the interpolation the same colour as the grid it interpolates from
                          	    oldInterpolationPointColour=INTERPOLEEGRID(i);
                          	    setXColour(gi.getColourName( (oldInterpolationPointColour %
                                                					  GenericGraphicsInterface::numberOfColourNames) ));
        // 		setXColour(gi.getColourName( min(oldInterpolationPointColour,
        // 						 GenericGraphicsInterface::numberOfColourNames-1)) );
                        	  }
        	  // printf(" plot pt i=%i, level=%i, zLevelForPoints=%g\n",i,level,zLevelForPoints);
                        	  if( numberOfDimensions==2 )
                          	    glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),i3),
                                  		      CENTER1(IP(i,axis1),IP(i,axis2),i3),zLevelForPoints);
                        	  else if( numberOfDimensions==3 )
                          	    glVertex3(CENTER0(IP(i,axis1),i2,i3),
                                  		      CENTER1(IP(i,axis1),i2,i3),zLevelForPoints);
                        	  else 
                          	    glVertex3(CENTER0(IP(i,axis1),i2,i3),yLevelFor1DGrids,zLevelForPoints);
                      	}
                      	glEnd();
                            }
                            if( plotBackupInterpolationPoints && plotOnThisProcessor )
                            {
        	// Now plot points that use back up interpolation
                #ifndef USE_PPP
                            const intSerialArray & mask = mg.mask();
                #else
                            intSerialArray mask; getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                #endif
                            const int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
                            const int maskDim0=mask.getRawDataSize(0);
                            const int maskDim1=mask.getRawDataSize(1);
                #define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]
        	// setXColour(colourNames[min(grid,numberOfColourNames-1)]);
                      	gi.setColour( GenericGraphicsInterface::textColour); 
                      	glPointSize(psp.pointSize*2.*gi.getLineWidthScaleFactor() );   
                      	glBegin(GL_POINTS);  
        //		    glPointSize(6.*lineWidthScaleFactor[currentWindow]);   
                      	for( i=ip.getBase(0); i<=ip.getBound(0); i++ )
                      	{
                        	  if( numberOfDimensions==2 )
                        	  {
                          	    if( MASK(IP(i,axis1),IP(i,axis2),i3) & CompositeGrid::USESbackupRules )
                          	    {
                            	      glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),i3),
                                  			CENTER1(IP(i,axis1),IP(i,axis2),i3),zLevelForPoints+1.e-4);
                          	    }
                        	  }
                        	  else
                        	  {
                          	    if( MASK(IP(i,axis1),i2,i3) & CompositeGrid::USESbackupRules )
                          	    {
                            	      glVertex3(CENTER0(IP(i,axis1),i2,i3),
                                  			CENTER1(IP(i,axis1),i2,i3),zLevelForPoints+1.e-4);
                          	    }
                        	  }
                      	}
                      	glEnd();
                            }
                        }
                    }
                #undef CENTER0
                #undef CENTER1
            } // end for p=0... np-1
            #undef VERTEX0
            #undef VERTEX1
            #undef VERTEX2
            }
            

        }  // end else structured

#undef MASK

        if( plotOnThisProcessor )
            glPopName();
    }  // for grid
    
}











