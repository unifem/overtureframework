#include "Ogmg.h"
#include "display.h"
#include "Ogen.h"
#include "ParallelUtility.h"

// extern CompositeGrid *cgGlobal;  // *******************

int checkGrid( CompositeGrid & cg, GenericGraphicsInterface *ps =0, int debug=0 );

int 
displayMaskLaTeX( const intArray & mask, 
		  const aString & label =nullString,
		  FILE *file = NULL ,
		  const DisplayParameters *displayParameters = NULL );

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

//\begin{>>OgmgInclude.tex}{\subsection{buildExtraLevels}}
int Ogmg::
buildExtraLevels(CompositeGrid & mg)
// ========================================================================================
//  /Description: 
//       Build extra multigrid levels. This routine will create coarser levels automatically.
// The tricky part is to determine how to interpolate on the new coarser levels.
// After a grid is coarsened it may no longer have enough interpolation points. We add new interpolation
// points to fill in the gaps. The width of the interpolation stencil is reduced, on a point by point
// basis, if necessary.
//
// /mg (input/output): 
//\end{OgmgInclude.tex} 
// =========================================================================================
{


  #ifdef USE_PPP
    // parallel version:
    return buildExtraLevelsNew( mg );
  #endif

  real time0=getCPU();
  CompositeGrid & mgcg = multigridCompositeGrid();
  
//  debug=7;  // ***
  int debugb=debug; // 7; // debug

  // printF("\n ********************** buildExtraLevels debugb=%i **************************\n",debugb);

  // --- The CompositeGrid mg is supplied by the USER and we do not want to change it.
  //      Instead we build another copy, mgcg that will contain the extra multigrid levels.

  if( parameters.saveGridCheckFile )
  {
    printF(" buildExtraLevelsNew: parameters.saveGridCheckFile=%i\n",(int)parameters.saveGridCheckFile);
    // save a check file containing information about the coarse grid levels
    if( gridCheckFile==NULL )
    {
      aString name="ogmg";
      if( numberOfInstances>1 )
      {
	// Give different names to the grid check file if we have more than 1 instance of Ogmg:
	sPrintF(name,"ogmg%i",numberOfInstances);
      }
      aString gridCheckFileName;
      sPrintF(gridCheckFileName,"%s.%s.coarseGrids.check",(const char*)name,(const char*)gridName);
      gridCheckFile = fopen((const char*)gridCheckFileName,"w" );     
      printF("Saving the check file %s\n",(const char*)gridCheckFileName);
    }
  }


  // *NOTE* For moving grid do the following; otherwise the statement "mgcg=mg" seems to over-write
  //    the MappedGrids in the version of "mg" that was used in the previous call!
  // mgcg.destroy(CompositeGrid::EVERYTHING);  // trouble with this
  if( mgcg.numberOfComponentGrids()>0 )
  {
    for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
    {
      mgcg[grid].breakReference();
    }
  }
  

  int grid;
  if( false )
  {
    
    mgcg=mg;  // deep copy for now *******************************************************************************

  }
  else if( true )
  {
    // mgcg.reference(mg);  // problems with this, mg seems to get changed below

    // mgcg.destroy(CompositeGrid::EVERYTHING);  // *wdh* 040831 -- added to fix bug with moving grids ---
    
    mgcg=mg;
    // now reference the grids so we don't keep two copies of the big arrays.
    
    for( grid=0; grid<mg.numberOfComponentGrids(); grid++ )
    {
      mgcg[grid].reference(mg[grid]);
    }
  
    // we could also reference interpolation arrays.

    mgcg.updateReferences();

    if( false )
    {
      mg.interpolationStartEndIndex.display("buildExtraLevels: mg.interpolationStartEndIndex");
      mgcg.interpolationStartEndIndex.display("buildExtraLevels: mgcg.interpolationStartEndIndex");
    }
    
  }
  else
  { 
    // we need to reference MappedGrids and Interpolation info.


    // this does not work
    // mgcg.setNumberOfDimensionsAndGrids(mg.numberOfDimensions(),mg.numberOfComponentGrids());

    for( grid=0; grid<mg.numberOfComponentGrids(); grid++ )
    {
      // mgcg[grid].reference(mg[grid].mapping());
      // mgcg[grid].reference(mg[grid]);
      mgcg.add(mg[grid]);           // add will make a reference
    }
    mgcg.updateReferences();
    // ** mgcg.update(MappedGrid::THEcenter);
  
    // mgcg.update(CompositeGrid::THElists | CompositeGrid::THEmultigridLevel);
    // mgcg.update(CompositeGrid::THElists | CompositeGrid::THEmultigridLevel);
  }
  mgcg.update(CompositeGrid::THEmultigridLevel);



  if( false )
    mgcg.multigridLevel[0].interpolationStartEndIndex.display("buildExtraLevels: after mgcg.update(CompositeGrid::THEmultigridLevel): mgcg.multigridLevel[0].interpolationStartEndIndex");
  
  // ***** fix this in CompositeGrid ****
  mgcg.multigridLevel[0].interpolationStartEndIndex=mgcg.interpolationStartEndIndex;
  

  const int numberOfDimensions = mgcg.numberOfDimensions();
  int axis;
  IntegerArray factor(3);
  factor=2;

  const int level0=mg.numberOfMultigridLevels()-1;
  int l, level;
  Range Rx=mgcg.numberOfDimensions();

  for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
  {
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	if( mgcg[grid].boundaryCondition()(side,axis) > 0 && mgcg[grid].numberOfGhostPoints()(side,axis)<2 )
	{
	  printF("Ogmg::buildExtraLevels:ERROR: The grid must be made with numberOfGhostPoints>=2 "
                  "on all physical boundaries\n");
          Overture::abort();
	}
      }
    }
  }



  // --- determine the maximum number of levels we can add. For now we must be able to coarsen by
  //     a factor of two along each axis.

  IntegerArray maxLevels(mgcg.numberOfComponentGrids()); // number of levels we can add to each grid
  maxLevels=0;
  const int minimumNumberOfPointsOnCoarseGrid=orderOfAccuracy/2;
  for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
  {
    const IntegerArray & gridIndexRange = mgcg.multigridLevel[level0][grid].gridIndexRange();

    // display((gridIndexRange(End,Rx)-gridIndexRange(Start,Rx)),"gridIndexRange(End,Rx)-gridIndexRange(Start,Rx)");

    for( int m=0; m<parameters.maximumNumberOfExtraLevels; m++ )
    {
      const int pow2 = (int)pow(2,m+1);

      // printF("pow2 = %i\n",pow2);
      
//       display(((gridIndexRange(End,Rx)-gridIndexRange(Start,Rx)) % pow2),
//            "(gridIndexRange(End,Rx)-gridIndexRange(Start,Rx)) % pow2 )");
      
      bool powerOfTwo=TRUE;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	if( (((gridIndexRange(End,axis)-gridIndexRange(Start,axis)) % pow2 )!=0)  || // not divisible by 2
	    (((gridIndexRange(End,axis)-gridIndexRange(Start,axis))/pow2) 
                            <=minimumNumberOfPointsOnCoarseGrid ) )// at least this many points on coarse grid
	{
	  powerOfTwo=FALSE;
          break;
	}
      }
      if( !powerOfTwo )
        break;

// A++ bug:
//       if( (max( (gridIndexRange(End,Rx)-gridIndexRange(Start,Rx)) % pow2 )!=0)  || // divisible by 2
//           (min( (gridIndexRange(End,Rx)-gridIndexRange(Start,Rx))/pow2) <2 ) ) // at least 1 points on coarse grid
//       {
// 	// this grid cannot be coarsened anymore
// 	break;
//       }
      maxLevels(grid)=m+1;
    }
    
  }
  // display(maxLevels,"Maximum number of extra levels per grid");
  if( debugb & 2 )
  {
    for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
    {
      printF("Ogmg:INFO: %i extra multigrid levels could be built on grid %i (%s).\n",maxLevels(grid),grid,
	     (const char*)mgcg[grid].getName());
    }
  }
  
  numberOfExtraLevels=min(maxLevels);

  if( debugb & 2 )
  {
    printF("**** Ogmg::buildExtraLevels: this grid supports %i extra MG levels (numberOfExtraLevels). *****\n"
           "**** At most %i levels will be made (maximumNumberOfExtraLevels).                         *****\n",
         numberOfExtraLevels, parameters.maximumNumberOfExtraLevels);
    fPrintF(debugFile,
           "**** Ogmg::buildExtraLevels: this grid supports %i extra MG levels (numberOfExtraLevels). *****\n"
           "**** At most %i levels will be made (maximumNumberOfExtraLevels).                         *****\n",
         numberOfExtraLevels, parameters.maximumNumberOfExtraLevels);
  }
  
  // --- Here we add the grids for the extra levels
  for( l=level0; l<level0+numberOfExtraLevels; l++ )
  {
    level=l+1;
    for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      mgcg.addMultigridCoarsening(factor,level,grid);
  }
  
  // mgcg.update(CompositeGrid::THElists | CompositeGrid::THEmultigridLevel);
  // mgcg.update();
    //  Tell the CompositeGrid that the interpolation data have been computed:

  // *wdh* 061123
  // mgcg.makeCompleteMultigridLevels();
  

// *wdh* 061123  
  bool newWay=true;  // *wdh* 061123  -- make sure the interp data lists are consistent between mgcg and levels
  if( newWay )
  {
    // update the MG level and make all interpolation arrays the correct length
    if( mgcg.numberOfBaseGrids()>1 )
    {
      // trouble here if there are no interp pts: (--fix this---)
      mgcg.update( CompositeGrid::THEmultigridLevel | 
		   CompositeGrid::THEinterpolationPoint       |
		   CompositeGrid::THEinterpoleeGrid           |
		   CompositeGrid::THEinterpoleeLocation       |
		   CompositeGrid::THEinterpolationCoordinates);

      if( Ogmg::debug & 2 )
	printF("@@@ Ogmg:BuildExtraLevels: after adding extra grids for MG levels : mgcg.numberOfGrids()=%i, "
	       "mgcg.numberOfComponentGrids()=%i, "
	       "mgcg.interpolationPoint.getLength=%i\n",
	       mgcg.numberOfGrids(),mgcg.numberOfComponentGrids(),mgcg.interpolationPoint.getLength());
    }
    else
    {
      mgcg.update( CompositeGrid::THEmultigridLevel );
    }
    
  }
  else
  {
    mgcg.update( CompositeGrid::THEmultigridLevel);
  }
  

  // Assign work-loads and load balance the CompositeGrid and all multigrid levels.
  loadBalance( mg, mgcg );


  mgcg.update( MappedGrid::THEmask ); // ********** no need to build center for rectangular grids
  // mgcg.update( MappedGrid::THEmask | MappedGrid::THEcenter | CompositeGrid::THEinterpolationPoint );

  if( true )
  {
    // For coarser level grids we do NOT share the vertex array in the grid with the mapping grid
    // used by the inverse (Otherwise the mapping inverse may get an extremely coarse grid) *wdh* 100424
    for( int level=1; level<mgcg.numberOfMultigridLevels(); level++ )
    {
      CompositeGrid & cg = mgcg.multigridLevel[level]; 
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	cg[grid].setShareGridWithMapping(false);
      }
    }
  }
  


  if( mgcg.numberOfComponentGrids()<=1 ||
      mgcg.numberOfMultigridLevels()==1 )
  {
    // ****** return here if there is only one component grid  or one MG level ******

    if( FALSE && level0==0 )
    {
      // now test out the validity of the newly created levels.
      for( l=level0; l<level0+numberOfExtraLevels; l++ )
      {
	level=l+1;
	printf("\n\n **************** check extra level %i ******************* \n\n",level);
	checkGrid( mgcg.multigridLevel[level],ps,debugb );
      }
    }

    tm[timeForBuildExtraLevels]+=getCPU()-time0;
    return 0;
  }
  
  Range Rg=mgcg.numberOfComponentGrids();

  // update interpolation data on multigrid level 0  -- why is this needed ?
  if( newWay )
  {

    //    CompositeGrid & cg = mgcg.multigridLevel[0];
    //     ::display(mgcg.numberOfInterpolationPoints,"buildExtra: mgcg.numberOfInterpolationPoints");
    //     ::display(cg.numberOfInterpolationPoints,"buildExtra: cg.numberOfInterpolationPoints (level=0)");
    //     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    //     {
    //       ::display(cg.interpoleeGrid[grid],"cg.interpoleeGrid[grid]");
    //       ::display(cg.variableInterpolationWidth[grid],"cg.variableInterpolationWidth[grid]");
    //       ::display(cg.interpolationPoint[grid],"cg.interpolationPoint[grid]");        
    //       ::display(cg.interpoleeLocation[grid],"cg.interpoleeLocation[grid]");        
    //       ::display(cg.interpolationCoordinates[grid],"cg.interpolationCoordinates[grid]");  
    //     }
  }
  else // try turning this off 061123
  {
    CompositeGrid & cg = mgcg.multigridLevel[0];

    //     ::display(mgcg.numberOfInterpolationPoints,"buildExtra: mgcg.numberOfInterpolationPoints");
    //     ::display(cg.numberOfInterpolationPoints,"buildExtra: cg.numberOfInterpolationPoints (level=0)");
      

    cg.numberOfInterpolationPoints(Rg)=mgcg.numberOfInterpolationPoints(Rg);
  
    cg.update(
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeGrid           |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpolationCoordinates ,
      CompositeGrid::COMPUTEnothing);

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      cg.interpoleeGrid[grid]            =mgcg.interpoleeGrid[grid];
      cg.variableInterpolationWidth[grid]=mgcg.variableInterpolationWidth[grid];
      cg.interpolationPoint[grid]        =mgcg.interpolationPoint[grid];
      cg.interpoleeLocation[grid]        =mgcg.interpoleeLocation[grid];
      cg.interpolationCoordinates[grid]  =mgcg.interpolationCoordinates[grid];
    }

    //  Tell the CompositeGrid that the interpolation data have been computed:
    mgcg->computedGeometry |=
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpoleeGrid;

    cg->computedGeometry |=
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpoleeGrid;

    // we also need to mark each MappedGrid (or else we lose the mask if we put/get to a file)
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid]->computedGeometry |= MappedGrid::THEmask;     // added 030829 *wdh*
  }
  
  // mgcg.update();
  // display(mgcg.interpolationPoint[0],"mgcg.interpolationPoint[0]");
  // displayMask(mgcg.multigridLevel[0][0].mask(),"mgcg.multigridLevel[0][0].mask");
  // display(mgcg.multigridLevel[0].interpolationPoint[0],"mgcg.multigridLevel[level].interpolationPoint[0]");
  
  
   CompositeGrid & cg00 = mgcg.multigridLevel[0];
   // cg00.update();
   if( debugb & 8 )
   {
     for( grid=0; grid<cg00.numberOfComponentGrids(); grid++ )
     {
       display(mgcg.variableInterpolationWidth[grid],"mgcg.variableInterpolationWidth[grid]",debugFile);
       display(cg00.variableInterpolationWidth[grid],"cg00.variableInterpolationWidth[grid]",debugFile);
     }
   }
   
   
   // **TESTING *** build vertex on grids so that the grid used by the mapping inverse comes from
   //               the fine grid
   if( false  ) 
   {
     for( int l=level0+numberOfExtraLevels; l>=level0; l-- )
     {
       int level=l;
       CompositeGrid & cg = l==0 ? mgcg : mgcg.multigridLevel[l]; 

       for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
       {
	 MappedGrid & mg = cg[grid];
	 if( !mg.isRectangular() )
	 {
	   printF("Ogmg:buildExtraLevels: Build vertex for the mapping inverse, level=%i, grid=%i\n",level,grid);
	 
	   mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
       
	   if( true || level==0 )
	   {
	     Mapping & map = mg.mapping().getMapping();
	     map.approximateGlobalInverse->reinitialize();  // -- this is needed -- fix me 
	     map.getBoundingBox();  // this will init the inverse 
	     if( map.approximateGlobalInverse!=NULL )
	     {
	       const RealArray & mgrid = map.approximateGlobalInverse->getGrid();
	  
	       printF("buildExtraLevels: level=%i grid=%i approximateGlobalInverse:\n",level,grid);
	       mg.indexRange().display("mg.indexRange");
	       printF("buildExtraLevels: approximateGlobalInverse: grid=[%i,%i]x[%i,%i] \n",
		      mgrid.getBase(0),mgrid.getBound(0),mgrid.getBase(1),mgrid.getBound(1));

	     }
	     else
	     {
	       printF("level=%i grid=%i  approximateGlobalInverse=NULL\n",level,grid);
	     }
	   }
	   
	 }
       
       }
     }
   }




  intSerialArray *iaA = new intSerialArray [mgcg.numberOfComponentGrids()];                 
  intSerialArray *interpoleeGridA = new intSerialArray [mgcg.numberOfComponentGrids()];     
  intSerialArray *interpolationPointA = new intSerialArray [mgcg.numberOfComponentGrids()]; 
  intSerialArray *interpoleeLocationA = new intSerialArray [mgcg.numberOfComponentGrids()]; 
  intSerialArray *variableInterpolationWidthA = new intSerialArray [mgcg.numberOfComponentGrids()]; 
  realSerialArray *interpolationCoordinatesA = new realSerialArray [mgcg.numberOfComponentGrids()]; 


  // --------------------------------------------------------------------------------
  // ---- Now update the mask and interpolation equations for the extra levels. -----
  // --------------------------------------------------------------------------------

  real timeForBuildMask=0., timeForBuildInterpolation=0., timeForValidStencil=0.;

  for( l=level0; l<level0+numberOfExtraLevels; l++ )
  {
    CompositeGrid & cg0 = l==0 ? mgcg : mgcg.multigridLevel[l];  // finer grid  ***********
    level=l+1;
    CompositeGrid & cg1 = mgcg.multigridLevel[level]; // coarser grid
    
    // cg1.update(MappedGrid::THEmask | MappedGrid::THEcenter);   // *********
    cg1.update(MappedGrid::THEmask );
    
    IntegerArray numberOfInterpolationPoints(cg1.numberOfComponentGrids());
    numberOfInterpolationPoints=0;
    
    
    if( debugb & 2 )
    {
      printF("=========================== build level %i ================================\n",level);
      fPrintF(debugFile,"=========================== build level %i ================================\n",level);
    }

    int I1Base,I1Bound, I2Base,I2Bound, I3Base,I3Bound;
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

    real timea=getCPU();
    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
    {

      const IntegerArray & coarseningRatio = mgcg.multigridCoarseningRatio(Range(0,2),grid,level0);
      int cf[3], &cf1=cf[0], &cf2=cf[1], &cf3=cf[2];
      cf1=coarseningRatio(axis1);  // coarsening factor
      cf2=coarseningRatio(axis2);
      cf3=coarseningRatio(axis3);  
      assert(cf1==2 && (cf2==2 || numberOfDimensions<2) && (cf3==2 || numberOfDimensions<3));

      MappedGrid & mg0 = cg0[grid];
      intArray & mask0g = mg0.mask();

      MappedGrid & mg1 = cg1[grid];
      intArray & mask1g = mg1.mask();

      #ifdef USE_PPP
        intSerialArray mask0; getLocalArrayWithGhostBoundaries(mask0g,mask0);
        intSerialArray mask1; getLocalArrayWithGhostBoundaries(mask1g,mask1);
      #else
        intSerialArray & mask0 = mask0g;
        intSerialArray & mask1 = mask1g;
      #endif


      int * mask1p = mask1.Array_Descriptor.Array_View_Pointer2;
      const int mask1Dim0=mask1.getRawDataSize(0);
      const int mask1Dim1=mask1.getRawDataSize(1);
#define MASK1(i0,i1,i2) mask1p[i0+mask1Dim0*(i1+mask1Dim1*(i2))]	
      
//        printF("------------mg0 from level %i, mg1 from level %i\n",l,level);
//        ::display(mgcg[grid].boundaryCondition(),"mgcg[grid].boundaryCondition()");
//        ::display(mg0.boundaryCondition(),"mg0.boundaryCondition()");
//        ::display(mg1.boundaryCondition(),"mg1.boundaryCondition()");
      


//       if( false && !mg1.isRectangular() ) // for debugging "valve" *wdh* 091130 ***************************************************
//       {
// 	Mapping & map = mg1.mapping().getMapping();
//         // mg1.update(MappedGrid::THEvertex | MappedGrid::THEcenter ); // this will replace map.AGI->grid with the vertex

// 	// map.approximateGlobalInverse->reinitialize();  // -- this is needed -- fix me 
	
//         map.getBoundingBox();  // this will init the inverse 
// 	if( map.approximateGlobalInverse!=NULL )
// 	{
//           const IntegerArray & indexRange = map.approximateGlobalInverse->indexRange;
//           const RealArray & mgrid = map.approximateGlobalInverse->grid;
	  
// 	  printF("buildExtraLevels: level=%i grid=%i approximateGlobalInverse:\n",level,grid);
//           mg1.indexRange().display("mg1.indexRange");
//           indexRange.display("indexRange");
//  	  printF("buildExtraLevels: approximateGlobalInverse: grid=[%i,%i]x[%i,%i], indexRange =[%i,%i][%i,%i] \n",
//  		 mgrid.getBase(0),mgrid.getBound(0),mgrid.getBase(1),mgrid.getBound(1),
//  		 indexRange(0,0),indexRange(1,0),indexRange(0,1),indexRange(1,1));

// 	}
// 	else
// 	{
// 	  printF("level=%i grid=%i  approximateGlobalInverse=NULL\n",level,grid);
// 	}
//       }
      


      if( debugb & 4 )
      {
        displayMask(mask0,sPrintF(buff,"Ogmg::buildExtraLevels: mask0 from level %i grid %i ",level-1,grid),pDebugFile);
	displayMask(mask1,sPrintF("Ogmg::buildExtraLevels: mask1 for level %i (BEFORE)",level),pDebugFile);
      }
      if( debugb & 16 )
      {
        Index J1,J2,J3;
        getIndex(mg0.gridIndexRange(),J1,J2,J3);   // Index's for coarse grid, one ghost line.
        displayMaskLaTeX(mask0g(J1,J2,J3),sPrintF(buff,"Ogmg::buildExtraLevels: mask0 from level %i grid %i ",
                    level-1,grid),pDebugFile);

        getIndex(mg1.gridIndexRange(),J1,J2,J3);   // Index's for coarse grid, one ghost line.
        displayMaskLaTeX(mask1g(J1,J2,J3),sPrintF("Ogmg::buildExtraLevels: mask1 for level %i (BEFORE)",level),pDebugFile);
      }
      
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      Index Ig1,Ig2,Ig3;
      
      // **** here I need 2 ghost lines **** is this really necessary ??
      const int nGhostCoarse = orderOfAccuracy/4;
      const int nGhostFine   = nGhostCoarse*2;

      getIndex(mg0.gridIndexRange(),I1,I2,I3,nGhostFine);   // Index's for fine grid -- include 2 ghost lines
      I1=IndexBB(I1,cf[0]);  I2=IndexBB(I2,cf[1]);  I3=IndexBB(I3,cf[2]);  // set stride
  
      Index J1,J2,J3;
      getIndex(mg1.gridIndexRange(),J1,J2,J3,nGhostCoarse);   // Index's for coarse grid, one ghost line.

      // ***************************************************************
      // ******** Copy fine mask to coarse mask ************************
      // ***************************************************************

      mask1(J1,J2,J3)=mask0(I1,I2,I3);


      if( debugb & 4 )
        displayMask(mask1,"Ogmg::buildExtraLevels: mask1=mask0 (stride 2)",pDebugFile);
      if( debugb & 16 )
      {
        Index J1,J2,J3;
        getIndex(mg1.gridIndexRange(),J1,J2,J3);   // Index's for coarse grid, one ghost line.
        displayMaskLaTeX(mask1g(J1,J2,J3),"Ogmg::buildExtraLevels: mask1=mask0 (stride 2)",pDebugFile);
      }
      
      MappedGrid & c = mg1;

    
      // We are going to mark new interpolation points as those with mask==0 but neighbours mask>0
      // We do not want the ghost values to play a role in this marking so set mask==0 at ghost points.
      //        set mask==0 at on both ghost lines outside bc!=0 boundaries
      // ****** set mask==0 at 2nd ghost line outside interp boundaries.!
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( c.boundaryCondition(side,axis)==0 )
	  {
            if( false )
	    {
	      // on interpolation boundaries we allow interpolation points to extend to
	      // ghost line 1 (orderOfAccuracy==2) or ghost line 2 (orderOfAccuracy==4)
	      // Mark the next ghost line outside of that (2 or 3) with zeroes
	      getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3,2,2); // ghost line 2, 2 extra
	      mask1(I1,I2,I3)=0;
	      if( orderOfAccuracy==2 )
	      {
		getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3,1,2); // ghost line 1, 2 extra
		mask1(I1,I2,I3)=0;
	      }
	    }
	    else
	    {
	      // *new* way : explicitly mark ghost points  -- now we don't need extra ghost points
              if( orderOfAccuracy==2 )
	      {
                // If the point on the boundary is a discretization point then we need to mark the
                // the first ghost line as interpolation 
                if( false )
		{
  		  getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3,1); // boundary, 1 or 2 extra
  		  getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,1);  // ghost line, 1 or 2 extra
		  where( mask1(I1,I2,I3)>0 )
		  {
		    mask1(Ig1,Ig2,Ig3)=MappedGrid::ISinterpolationPoint;
		  }
		  otherwise()
		  {
		    mask1(Ig1,Ig2,Ig3)=0;
		  }
		}
		else
		{
                  const IntegerArray & gir = c.gridIndexRange();
  		  getBoundaryIndex(gir,side,axis,I1,I2,I3); // boundary
                  int i1,i2,i3;
                  is1=is2=is3=0;
		  isv[axis]=1-2*side;
		  FOR_3(i1,i2,i3,I1,I2,I3)
		  {
                    if( MASK1(i1,i2,i3)>0 )
                      MASK1(i1-is1,i2-is2,i3-is3)=MappedGrid::ISinterpolationPoint;
		    else
                      MASK1(i1-is1,i2-is2,i3-is3)=0;
		  }

                  // **** Now mark corners and edges **** 030829
                  //      Some points are marked twice
                  // (we cannot include this in the above loop since the mask is zero at ghost points)

                  if( true ) // ************
		  {
                  const int axisp1 = (axis+1) % numberOfDimensions;
		  for( int side2=0; side2<=1; side2++ )
		  {
                    // corner in 2D, or edge 3D
		    Iv[axisp1]= gir(side2,axisp1);
                    isv[axisp1]=1-2*side2;
		    FOR_3(i1,i2,i3,I1,I2,I3)
		    {
		      if( MASK1(i1,i2,i3)>0 )
			MASK1(i1-is1,i2-is2,i3-is3)=MappedGrid::ISinterpolationPoint;
		      else
			MASK1(i1-is1,i2-is2,i3-is3)=0;
		    }
		    if( numberOfDimensions==3 )
		    {
                      // corners in 3D
		      const int axisp2=(axis+2) % numberOfDimensions;
                      for( int side3=0; side3<=1; side3++ )
		      {
			Iv[axisp2]= gir(side3,axisp2);
			isv[axisp2]=1-2*side3;
			FOR_3(i1,i2,i3,I1,I2,I3)
			{
			  if( MASK1(i1,i2,i3)>0 )
			    MASK1(i1-is1,i2-is2,i3-is3)=MappedGrid::ISinterpolationPoint;
			  else
			    MASK1(i1-is1,i2-is2,i3-is3)=0;
			}
		      }
		    }
		  }
                  if( numberOfDimensions==3 )
		  {
                    // other edge in 3D
		    Iv[axisp1]=Range(gir(0,axisp1),gir(1,axisp1)); // reset
                    isv[axisp1]=0;
		    
		    const int axisp2=(axis+2) % numberOfDimensions;
		    for( int side3=0; side3<=1; side3++ )
		    {
		      Iv[axisp2]= gir(side3,axisp2);
		      isv[axisp2]=1-2*side3;
		      FOR_3(i1,i2,i3,I1,I2,I3)
		      {
			if( MASK1(i1,i2,i3)>0 )
			  MASK1(i1-is1,i2-is2,i3-is3)=MappedGrid::ISinterpolationPoint;
			else
			  MASK1(i1-is1,i2-is2,i3-is3)=0;
		      }
		    }
		    
		  }
		  }
		  
		}
	      }
	      else if( orderOfAccuracy==4 )
	      {
                // If the point on the 1st ghost line is a discretization point then we need to mark the
                // the second ghost line as interpolation 
		getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3,1,2); // ghost line 1, 2 extra
		getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,2,2);  // ghost line 2, 2 extra

		where( mask1(I1,I2,I3)!=0 )
		{
		  mask1(Ig1,Ig2,Ig3)=MappedGrid::ISinterpolationPoint;
		}
                otherwise()
		{
		  mask1(Ig1,Ig2,Ig3)=0;
		}

                // *wdh* added 030614

		// If a point on the boundary is interpolation then we need to mark the first ghost line
                // as interpolation
		getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3,2); // boundary, 1 or 2 extra
		getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,2);  // ghost line, 1 or 2 extra
		where( mask1(I1,I2,I3)<0 )
		{
		  mask1(Ig1,Ig2,Ig3)=MappedGrid::ISinterpolationPoint;
		}

	      }
	      
//                const int line= orderOfAccuracy/2;
//                const int extra=line;
//                assert( line==1 || line==2 );
//  	      getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3,extra); // boundary, 1 or 2 extra
//  	      getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,line,extra);  // ghost line, 1 or 2 extra
//  	      where( mask1(I1,I2,I3)>0 )
//  	      {
//  		mask1(Ig1,Ig2,Ig3)=MappedGrid::ISinterpolationPoint;
//  	      }

	    }
	  }
	  else
	  {
            getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3,2,2); // ghost line 2, 2 extra
            mask1(I1,I2,I3)=0;
            getGhostIndex(c.gridIndexRange(),side,axis,I1,I2,I3,1,2); // ghost line 1, 2 extra
            mask1(I1,I2,I3)=0;
	  }
	}
      }
      
      if( debugb & 4 )
        displayMask(mask1,sPrintF(buff,"Ogmg::buildExtraLevels: mask1 for level %i grid %i (Before marking extra)",
             level,grid),pDebugFile);

      // ***********************************************************
      // ***  mark any extra interpolation points on coarse grid  **
      // ***********************************************************
      // Mark a point with mask==0 but with a neighbour mask>0
      // The number of neighbours we check depends on how wide the discretization width is.

      // getIndex(c.extendedIndexRange(),I1,I2,I3);   // includes the ghost point on bc==0 sides.
      getIndex(c.gridIndexRange(),I1,I2,I3);  // *wdh* 030202
      // **wdh* We need to include ghost points on BC=0 sides! (sib) 030802
      // getIndex(c.extendedIndexRange(),I1,I2,I3); 

      halfWidth1 = c.discretizationWidth(axis1)/2;
      halfWidth2 = numberOfDimensions>1 ? c.discretizationWidth(axis2)/2 : 0;
      halfWidth3 = numberOfDimensions>2 ? c.discretizationWidth(axis3)/2 : 0;
      const int hw[3] = { halfWidth1,halfWidth2,halfWidth3};  
    
      const IntegerArray & dimension = c.dimension();
      // int axis;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	if( min(c.numberOfGhostPoints()(Range(0,1),axis))<hw[axis] )
	{
	  printF("Ogen::ERROR: the number of ghost points must be at least %i if the discretization width =%i\n",
		 hw[axis],c.discretizationWidth(axis));
	  throw "error";
	}
//  	Iv[axis] = Range(max(Iv[axis].getBase() ,dimension(Start,axis)+hw[axis]),
//  			 min(Iv[axis].getBound(),dimension(End  ,axis)-hw[axis]));
      }
    
      // *new* Mark points needed for discretization
      IntegerArray eir;
      eir = c.extendedIndexRange();  // indexRange plus 1 or 2  extra at interpolation boundaries
      // We should not count "interp" points at periodic images 
//        int axis;
//        for( axis=0; axis<c.numberOfDimensions(); axis++ )  // not needed, already ok
//        {
//  	if( (bool)c.isPeriodic(axis) )
//  	{
//  	  eir(0,axis)=c.indexRange(0,axis);
//  	  eir(1,axis)=c.indexRange(1,axis);
//  	}
//        }

      if( debugb & 4 )
	fprintf(pDebugFile,">>>>level=%i grid=%i isPeriodic=%i,%i,%i eir=[%i,%i][%i,%i][%i,%i]\n",
		level,grid,c.isPeriodic(0),c.isPeriodic(1),c.isPeriodic(2),eir(0,0),eir(1,0),
		eir(0,1),eir(1,1),eir(0,2),eir(1,2));
      
      const int *eirp = eir.Array_Descriptor.Array_View_Pointer1;
      const int eirDim0=eir.getRawDataSize(0);
#undef EIR
#define EIR(i0,i1) eirp[i0+eirDim0*(i1)]

      int i1,i2,i3,j1,j2,j3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( MASK1(i1,i2,i3)>0 ) // discretization point
	{
	  for( int m3=-halfWidth3; m3<=halfWidth3; m3++ )      
	  {
	    for( int m2=-halfWidth2; m2<=halfWidth2; m2++ )
	    {
	      for( int m1=-halfWidth1; m1<=halfWidth1; m1++ )
	      {
		if( MASK1(i1+m1,i2+m2,i3+m3)==0 )
		{
                  j1=i1+m1;
		  j2=i2+m2;
		  j3=i3+m3;
		  if( j1>=EIR(0,0) && j1<=EIR(1,0) &&
                      j2>=EIR(0,1) && j2<=EIR(1,1) &&
                      j3>=EIR(0,2) && j3<=EIR(1,2) )
		  {
		    MASK1(i1+m1,i2+m2,i3+m3)= MappedGrid::ISinterpolationPoint;
		  }
		}
	      }
	    }
	  }
	}
      }

      mg1.mask().periodicUpdate(); // *wdh* 021006 needed for stir.mg.hdf for example

      if( debugb & 4 )
        displayMask(mask1,sPrintF(buff,"Ogmg::buildExtraLevels: mask1 for level %i grid %i (AFTER)",level,grid),
                    pDebugFile);
      if( debugb & 16 )
      {
        Index J1,J2,J3;
        getIndex(mg1.gridIndexRange(),J1,J2,J3);   // Index's for coarse grid, one ghost line.
        displayMaskLaTeX(mask1g(J1,J2,J3),
              sPrintF(buff,"Ogmg::buildExtraLevels: mask1 for level %i grid %i (AFTER)",level,grid),pDebugFile);
      }
      
    }

    real timeb=getCPU();
    timeForBuildMask+=timeb-timea;

    // ***********************************************************************
    // Interpolation points have now been marked on the coarse grid
    // Determine the interpolation information for each interpolation point.
    // ***********************************************************************


    realSerialArray r(1,3),x(1,3);
    real *xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
#define X(i0,i1) xp[i0+xDim0*(i1)]
    real *rp = r.Array_Descriptor.Array_View_Pointer1;
    int rDim0=r.getRawDataSize(0);
#define R(i0,i1) rp[i0+rDim0*(i1)]

    r=-1.; x=0.;

    // arrays for rectangular grids:
    real dx0[3],xab0[2][3];
    real dx1[3],xab1[2][3];
    int iv0[3], iv1[3];
#define CENTER00(i0,i1,i2) (xab0[0][0]+dx0[0]*(i0-iv0[0]))
#define CENTER01(i0,i1,i2) (xab0[0][1]+dx0[1]*(i1-iv0[1]))
#define CENTER02(i0,i1,i2) (xab0[0][2]+dx0[2]*(i2-iv0[2]))

#define CENTER10(i0,i1,i2) (xab1[0][0]+dx1[0]*(i0-iv1[0]))
#define CENTER11(i0,i1,i2) (xab1[0][1]+dx1[1]*(i1-iv1[1]))
#define CENTER12(i0,i1,i2) (xab1[0][2]+dx1[2]*(i2-iv1[2]))

    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
    {
      // only create the center (vertices) for non-rectangular grids
      if( !cg1[grid].isRectangular() )
      {
        if( l==level0 ) cg0[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter );	
        cg1[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter );
      }
    }
    
    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
    {

      // ** Parallel: todo: 
      //   Get a list of interp pts that are on this processor 
      //   getLocalInterpolationData( ... )


      const IntegerArray & numberOfInterpolationPoints0 =cg0.numberOfInterpolationPoints;
      #ifdef USE_PPP
        intSerialArray interpoleeGrid0; getLocalArrayWithGhostBoundaries(cg0.interpoleeGrid[grid],interpoleeGrid0);
        intSerialArray interpolationPoint0; getLocalArrayWithGhostBoundaries(cg0.interpolationPoint[grid],interpolationPoint0);
        realSerialArray interpolationCoordinates0; getLocalArrayWithGhostBoundaries(cg0.interpolationCoordinates[grid],
                                                                                    interpolationCoordinates0);
      #else
        const intArray & interpoleeGrid0 =cg0.interpoleeGrid[grid];  // ************ should use cg0 ****
        const intArray & interpolationPoint0 =cg0.interpolationPoint[grid];
        const realArray & interpolationCoordinates0 =cg0.interpolationCoordinates[grid];
      #endif    

      const int * interpoleeGrid0p = interpoleeGrid0.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID0(i0) interpoleeGrid0p[i0]

      const int *interpolationPoint0p = interpolationPoint0.Array_Descriptor.Array_View_Pointer1;
      const int interpolationPoint0Dim0=interpolationPoint0.getRawDataSize(0);
#define INTERPOLATIONPOINT0(i0,i1) interpolationPoint0p[i0+interpolationPoint0Dim0*(i1)]

      const real *interpolationCoordinates0p = interpolationCoordinates0.Array_Descriptor.Array_View_Pointer1;
      const int interpolationCoordinates0Dim0=interpolationCoordinates0.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES0(i0,i1) interpolationCoordinates0p[i0+interpolationCoordinates0Dim0*(i1)]

      MappedGrid & mg0 = cg0[grid];
      intArray & mask0g = mg0.mask();

      MappedGrid & mg1 = cg1[grid];
      MappedGrid & c = mg1;
      const bool isRectangular=mg1.isRectangular();

      intArray & mask1g = mg1.mask();


      #ifdef USE_PPP
        intSerialArray mask0; getLocalArrayWithGhostBoundaries(mask0g,mask0);
        intSerialArray mask1; getLocalArrayWithGhostBoundaries(mask1g,mask1);
        realSerialArray center;  if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg1.center(),center);
      #else
        intSerialArray & mask0 = mask0g;
        intSerialArray & mask1 = mask1g;
        const realArray & center = mg1.center();
      #endif

      const int * mask1p = mask1.Array_Descriptor.Array_View_Pointer2;
      const int mask1Dim0=mask1.getRawDataSize(0);
      const int mask1Dim1=mask1.getRawDataSize(1);

      real *centerp;
      int centerDim0,centerDim1,centerDim2;
      if( !isRectangular )
      {
	centerp = center.Array_Descriptor.Array_View_Pointer3;
	centerDim0=center.getRawDataSize(0);
	centerDim1=center.getRawDataSize(1);
	centerDim2=center.getRawDataSize(2);
      }
      
#define CENTER(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]	

      if( isRectangular )
      {
	mg0.getRectangularGridParameters( dx0, xab0 );
	mg1.getRectangularGridParameters( dx1, xab1 );
	iv0[0]=mg0.gridIndexRange(0,0);
	iv0[1]=mg0.gridIndexRange(0,1);
	iv0[2]=mg0.gridIndexRange(0,2);
	iv1[0]=mg1.gridIndexRange(0,0);
	iv1[1]=mg1.gridIndexRange(0,1);
	iv1[2]=mg1.gridIndexRange(0,2);
	if( mg1.isAllCellCentered() )
	{
	  xab0[0][0]+=dx0[0]*.5;
	  xab0[0][1]+=dx0[1]*.5;
	  xab0[0][2]+=dx0[2]*.5;

	  xab1[0][0]+=dx1[0]*.5;
	  xab1[0][1]+=dx1[1]*.5;
	  xab1[0][2]+=dx1[2]*.5;
	}     
      }
      
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

      IntegerArray eir;
      eir = c.extendedIndexRange();  // indexRange plus 1 or 2  extra at interpolation boundaries
//        for( axis=0; axis<c.numberOfDimensions(); axis++ ) // not needed
//        {
//  	if( (bool)c.isPeriodic(axis) )
//  	{
//  	  eir(0,axis)=c.indexRange(0,axis);
//  	  eir(1,axis)=c.indexRange(1,axis);
//  	}
//        }

      const int *eirp = eir.Array_Descriptor.Array_View_Pointer1;
      const int eirDim0=eir.getRawDataSize(0);
#undef EIR
#define EIR(i0,i1) eirp[i0+eirDim0*(i1)]

//        if( orderOfAccuracy==4 )
//        {
//          // we need to extend to 2 ghost points on interpolation boundaries for 4th order
//          int numberOfGhostPoints=orderOfAccuracy/2;
//  	for( axis=0; axis<numberOfDimensions; axis++ )
//  	{
//  	  for( int side=0; side<=1; side++ )
//  	  {
//              if( c.boundaryCondition(side,axis)==0 )
//  	    {
//  	      eir(side,axis)=c.gridIndexRange(side,axis)-numberOfGhostPoints*(1-2*side);
//  	    }
//  	  }
//  	}
//        }

      const IntegerArray & indexRange0 = mg0.indexRange();  // fine grid
      const int *indexRange0p = indexRange0.Array_Descriptor.Array_View_Pointer1;
      const int indexRange0Dim0=indexRange0.getRawDataSize(0);
#define INDEXRANGE0(i0,i1) indexRange0p[i0+indexRange0Dim0*(i1)]

      const IntegerArray & indexRange = mg1.indexRange();  // coarse grid 
      const int *indexRangep = indexRange.Array_Descriptor.Array_View_Pointer1;
      const int indexRangeDim0=indexRange.getRawDataSize(0);
#define INDEXRANGE(i0,i1) indexRangep[i0+indexRangeDim0*(i1)]

      getIndex(eir,I1,I2,I3,1); 
      intSerialArray inverseGrid(I1,I2,I3);  // allocate here to include extra on periodic edges 030617

      // const int extra=orderOfAccuracy==2 ? 1 : 0; // need one extra (ellipsoid)
      const int extra=0;  // *wdh* turn off 030830 -- no longer needed after other changes.
      getIndex(eir,I1,I2,I3,extra);   


//        printF(">>>>> grid=%i gid=%i %i eir+1=%i %i \n",grid,c.gridIndexRange(0,0),c.gridIndexRange(1,0),
//  	     I1.getBase(),I1.getBound());

      // We should not count "interp" points at periodic images *wdh* 021006
      // This is needed after we added the mask.periodicUpdate above
      int axis;
      for( axis=0; axis<c.numberOfDimensions(); axis++ )
      {
	if( INDEXRANGE(0,axis)!=0 )  // we assume this for now!
	{
	  printF("Ogmg::buildExtraLevels:WARNING: grid=%i : indexRange(0,axis)!=0 ! FIX ME BILL!\n",grid);
	  // OV_ABORT("ERROR");
	}
	
	if( c.isPeriodic(axis) )
	  Iv[axis]=Range(INDEXRANGE(0,axis),INDEXRANGE(1,axis));
      }

      // printF("*** I1,I2,I3 for counting interp: [%i,%i][%i,%i][%i,%i]\n",
      //      I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
      

      // ****** set mask==0 at 2nd ghost line outside interp boundaries.!

      // make a list of interpolation points.
      int maxNumberOfInterpolationPoints=cg0.numberOfInterpolationPoints(grid)*2+100;
      Range R=maxNumberOfInterpolationPoints;
      intSerialArray & ia =iaA[grid]; ia.redim(R,3);
      int *iap = ia.Array_Descriptor.Array_View_Pointer1;
      const int iaDim0=ia.getRawDataSize(0);
#define IA(i0,i1) iap[i0+iaDim0*(i1)]


      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];      
      int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];      
      int kpv[3], &kp1=kpv[0], &kp2=kpv[1], &kp3=kpv[2];      
      int I1Base,I2Base,I3Base;
      int I1Bound,I2Bound,I3Bound;
      int i=0;
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	if( MASK1(i1,i2,i3) & MappedGrid::ISinterpolationPoint )
	{                                
	  IA(i,0)=i1;
	  IA(i,1)=i2;
	  IA(i,2)=i3;
	  i++;
	}
      }

      int ni=i;
      numberOfInterpolationPoints(grid)=ni;
      if( debugb & 4 )
      {
        printF("*** numberOfInterpolationPoints(%i) =%i \n",grid,ni);
        fprintf(pDebugFile,"*** level=%i, grid=%i, numberOfInterpolationPoints(%i) =%i \n",level,grid,grid,ni);
      }
      
      R=ni;
// **      intSerialArray inverseGrid(I1,I2,I3);
      int * inverseGridp = inverseGrid.Array_Descriptor.Array_View_Pointer2;
      const int inverseGridDim0=inverseGrid.getRawDataSize(0);
      const int inverseGridDim1=inverseGrid.getRawDataSize(1);
#define INVERSEGRID(i0,i1,i2) inverseGridp[i0+inverseGridDim0*(i1+inverseGridDim1*(i2))]	

      inverseGrid=-1;
      realSerialArray inverseCoordinates(I1,I2,I3,Rx);
      
      real * inverseCoordinatesp = inverseCoordinates.Array_Descriptor.Array_View_Pointer3;
      const int inverseCoordinatesDim0=inverseCoordinates.getRawDataSize(0);
      const int inverseCoordinatesDim1=inverseCoordinates.getRawDataSize(1);
      const int inverseCoordinatesDim2=inverseCoordinates.getRawDataSize(2);
#define INVERSECOORDINATES(i0,i1,i2,i3) inverseCoordinatesp[i0+inverseCoordinatesDim0*(i1+inverseCoordinatesDim1*(i2+inverseCoordinatesDim2*(i3)))]	


      // --- mark all coarse grid interpolation points that match a fine grid interpolation point
      i3=0;// assumes base 0 ---- fix ---
      j3=0;
      k3=0;
      ni=0; // reset -- we will recount
      const int numInterp=numberOfInterpolationPoints0(grid);
      for( i=0; i<numInterp; i++ )
      {
        for( axis=0; axis<numberOfDimensions; axis++ )
	{
          // wdh* 2012/03/12
  	  // iv[axis]=INTERPOLATIONPOINT0(i,axis);
          // jv[axis]=(iv[axis] - INDEXRANGE(0,axis))/2;  // coarse grid point

  	  iv[axis]=INTERPOLATIONPOINT0(i,axis)- INDEXRANGE0(0,axis);  // fine grid OFFSET point
          jv[axis]=iv[axis]/2 + INDEXRANGE(0,axis);  // coarse grid point
	}
        if( (i1%2)==0 && (i2%2)==0 && (i3%2)==0 )  // ******************* %2 should be % cf
	{  // fine grid interp pt matches coarse grid interp pt.
	  assert( MASK1(j1,j2,j3) & MappedGrid::ISinterpolationPoint );
          ni++;
          assert( INTERPOLEEGRID0(i)>=0 );
	  INVERSEGRID(j1,j2,j3)=INTERPOLEEGRID0(i);
          for( axis=0; axis<numberOfDimensions; axis++ )
	    INVERSECOORDINATES(j1,j2,j3,axis)=INTERPOLATIONCOORDINATES0(i,axis); 
	}
	
      }

      // if( debugb & 4 )
      // {
      //   fprintf(pDebugFile,"grid=%i: number of coarse grid interp that match fine=%i \n",grid,ni);
      // }

      // --- For each coarse grid interp point that matches a fine grid interp point (and has already
      //  been marked), mark any neighbours that are interpolation points and are not marked yet.
      int jb=0;  // counts new interpolation points
      int maxNumberOfExtraInterpolationPoints=numberOfInterpolationPoints0(grid)*numberOfDimensions;
                   
      IntegerArray ib(maxNumberOfExtraInterpolationPoints,4);
      int *ibp = ib.Array_Descriptor.Array_View_Pointer1;
      const int ibDim0=ib.getRawDataSize(0);
#define IB(i0,i1) ibp[i0+ibDim0*(i1)]

      IntegerArray gridsToCheck(cg1.numberOfComponentGrids());
      gridsToCheck=0;
      
      for( i=0; i<numberOfInterpolationPoints0(grid); i++ )
      {

        for( axis=0; axis<numberOfDimensions; axis++ )
	{
          // *wdh* 2012/03/12 :
  	  // iv[axis]=INTERPOLATIONPOINT0(i,axis);
          // kv[axis]=(iv[axis] - INDEXRANGE(0,axis) + 8 )/2 -4;
  	  iv[axis]=INTERPOLATIONPOINT0(i,axis)- INDEXRANGE0(0,axis); // fine grid OFFSET point
          kv[axis]=(iv[axis] + 8 )/2 -4 + INDEXRANGE(0,axis);
	}
        if( (i1%2)==0 && (i2%2)==0 && (i3%2)==0 
	    && orderOfAccuracy==2 ) // ******************************* 030202
	{  // fine grid interp pt matches coarse grid interp pt.
	}
	else
	{

	  // fprintf(pDebugFile," level=%i grid=%i ni=%i i=%i fineInterpPoint=(%i,%i,%i) coarse=(%i,%i,%i) \n",
          //         level,grid,ni,i,i1,i2,i3, k1,k2,k3);

          // assert( numberOfDimensions<3 );
          const int m3Start=numberOfDimensions<3 ? 0 : -1;
          const int m3End  =numberOfDimensions<3 ? 0 : +1;
          for( int m3=m3Start; m3<=m3End; m3++ )
	  {
	    j3=k3+m3;
	    for( int m2=-1; m2<=1; m2++ )
	    {
	      j2=k2+m2;
	      for( int m1=-1; m1<=1; m1++ )
	      {
		j1=k1+m1;

		// fprintf(pDebugFile," check (j1,j2,j3)=(%i,%i,%i) mask1=%i inverseGrid=%i\n", // *************** TEMP
                //         j1,j2,j3,MASK1(j1,j2,j3),INVERSEGRID(j1,j2,j3));
		
		if( (MASK1(j1,j2,j3) & MappedGrid::ISinterpolationPoint) && INVERSEGRID(j1,j2,j3)==-1 )
		{
                  // this neighbour has not been marked yet.
                  // skip points outside (these could be periodic images of interp points) *wdh* 021006
		  // if( debugb & 4 ) // TEMP 2012/03/11
		  // {
		  //   fprintf(pDebugFile," grid=%i ni=%i i=(%i,%i,%i) k=(%i,%i,%i) check j=(%i,%i,%i) mask1(j)=%i "
                  //           "inverseGrid=%i eir=[%i,%i][%i,%i][%i,%i]\n",
		  // 	    grid,ni,i1,i2,i3,k1,k2,k3,j1,j2,j3,MASK1(j1,j2,j3),INVERSEGRID(j1,j2,j3),
		  // 	    EIR(0,0),EIR(1,0),EIR(0,1),EIR(1,1),EIR(0,2),EIR(1,2));
		  // }


                  if( j1<EIR(0,0) || j1>EIR(1,0) ) continue;
                  if( j2<EIR(0,1) || j2>EIR(1,1) ) continue;
                  if( numberOfDimensions==3 && (j3<EIR(0,2) || j3>EIR(1,2)) ) continue;
		  
		  
		  ni++;
                  // by default we choose the same interpolee grid as point i  -- this may
                  // be changed later
                  assert( INTERPOLEEGRID0(i)>=0 );
                  int grid2=INTERPOLEEGRID0(i); // **** this is a first guess , could try to take closest one 
		  INVERSEGRID(j1,j2,j3)=grid2;
                  gridsToCheck(grid2)=1;

                  IB(jb,0)=i;
                  IB(jb,1)=j1;
                  IB(jb,2)=j2;
                  IB(jb,3)=j3;
		  
		  jb++;
		  if( jb>=maxNumberOfExtraInterpolationPoints )
		  {
                    maxNumberOfExtraInterpolationPoints=int(maxNumberOfExtraInterpolationPoints*1.5+1);
		    
                    printF("INFO: increasing maxNumberOfExtraInterpolationPoints to %i\n",
			   maxNumberOfExtraInterpolationPoints);
		    ib.resize(maxNumberOfExtraInterpolationPoints,ib.getLength(1));
		  }
		  
		  if( false )
		  {
		    getInterpolationCoordinates(cg0,cg1,i,grid,iv,jv,r, isRectangular,iv0,dx0,xab0,iv1,dx1,xab1);
//                      printF("getInterpolationCoordinates:  i=%i grid=%i iv=[%i,%i] jv=[%i,%i] r=[%8.2e,%8.2e]\n",
//   			 i,grid,iv[0],iv[1], jv[0],jv[1],r(0,0),r(0,1));
		  
		    for( axis=0; axis<numberOfDimensions; axis++ )
		      INVERSECOORDINATES(j1,j2,j3,axis)=r(0,axis);
		  }
		  
		  
		} // end if (MASK1
		// j1-=m1;
	      } // end for m1
	      // j2-=m2;
	    }
	    // j3-=m3;
	  } // end for m3
	}
	
      }  // end for i
      

      // now compute a first guess at the interpolation coords for all the extra interp points
      const int numberOfExtraInterpolationPoints=jb;
      if( numberOfExtraInterpolationPoints>0 )
      {
	r.redim(numberOfExtraInterpolationPoints,3);
	rp = r.Array_Descriptor.Array_View_Pointer1;
	rDim0=r.getRawDataSize(0);

	Range Ri=numberOfExtraInterpolationPoints, all;
	
	getInterpolationCoordinates(cg0,cg1,ib(Ri,all),grid,gridsToCheck, r,isRectangular,iv0,dx0,xab0,iv1,dx1,xab1);
      }
      
      if( numberOfDimensions==2 )
      {
	for( i=0; i<numberOfExtraInterpolationPoints; i++ )
	{
          j1=IB(i,1); j2=IB(i,2); j3=IB(i,3);
//            real diff=max( fabs(INVERSECOORDINATES(j1,j2,j3,axis1)-R(i,0)),
//                           fabs(INVERSECOORDINATES(j1,j2,j3,axis2)-R(i,1)) );
//  	  if( diff>1.e-10 )
//  	  {
//  	    printF("****getInterpolationCoordinates:ERROR  i=%i OLD: r=[%8.2e,%8.2e] new: r=[%8.2e,%8.2e]\n",
//  		   i,INVERSECOORDINATES(j1,j2,j3,axis1),INVERSECOORDINATES(j1,j2,j3,axis2),R(i,0),R(i,1));
	    
//  	  }
    	  INVERSECOORDINATES(j1,j2,j3,axis1)=R(i,axis1);
    	  INVERSECOORDINATES(j1,j2,j3,axis2)=R(i,axis2);
	}
      }
      else
      {
	for( i=0; i<numberOfExtraInterpolationPoints; i++ )
	{
          j1=IB(i,1); j2=IB(i,2); j3=IB(i,3);
//            real diff=max( fabs(INVERSECOORDINATES(j1,j2,j3,axis1)-R(i,0)),
//                           fabs(INVERSECOORDINATES(j1,j2,j3,axis2)-R(i,1)),
//                           fabs(INVERSECOORDINATES(j1,j2,j3,axis3)-R(i,2)) );
//  	  if( diff>1.e-10 )
//  	  {
//  	    printF("****getInterpolationCoordinates:ERROR i=%i OLD: r=[%8.2e,%8.2e,%8.2e] new r=[%8.2e,%8.2e,%8.2e]\n",
//  		   i,INVERSECOORDINATES(j1,j2,j3,axis1),INVERSECOORDINATES(j1,j2,j3,axis2),
//                        INVERSECOORDINATES(j1,j2,j3,axis3),R(i,0),R(i,1),R(i,2));
	    
//  	  }
	  INVERSECOORDINATES(j1,j2,j3,axis1)=R(i,axis1);
	  INVERSECOORDINATES(j1,j2,j3,axis2)=R(i,axis2);
	  INVERSECOORDINATES(j1,j2,j3,axis3)=R(i,axis3);
	}
      }
      

      if( debugb & 4 )
        printF(" *** number of interpolation points assigned, ni=%i, level=%i grid=%i "
               "numberOfInterpolationPoints=%i\n",ni,level,grid,numberOfInterpolationPoints(grid));
      assert( ni==numberOfInterpolationPoints(grid) );
      
      
      if( debugb & 4 )
        display(inverseGrid,sPrintF(buff,"inverseGrid level=%i grid=%i",level,grid),pDebugFile);
      
      // ****************************************************
      // ----- find a valid stencil to interpolate from -----
      // ****************************************************

      real timea=getCPU();

      intSerialArray & interpoleeGrid = interpoleeGridA[grid];                  interpoleeGrid.redim(R);
      intSerialArray & interpolationPoint = interpolationPointA[grid];          interpolationPoint.redim(R,Rx);
      intSerialArray & interpoleeLocation = interpoleeLocationA[grid];          interpoleeLocation.redim(R,Rx);
      realSerialArray & interpolationCoordinates = interpolationCoordinatesA[grid]; interpolationCoordinates.redim(R,Rx);

      intSerialArray & variableInterpolationWidth = variableInterpolationWidthA[grid]; 
      variableInterpolationWidth.redim(R);
      variableInterpolationWidth=0;
      
      if( parameters.coarseGridInterpolationWidth>0 || orderOfAccuracy==4 )
      {
        // Interpolate to lower order accuracy for fourth-order discretizations -- this assumes the averaged operator
        // on the coarse grid is only second-order anyway.
	Range all;
        Range Rx=mgcg.numberOfDimensions();

        const int defaultInterpWidth = orderOfAccuracy==2 ? 3 : 3;
	
        int interpWidth = (parameters.coarseGridInterpolationWidth<0 ? defaultInterpWidth : 
                           parameters.coarseGridInterpolationWidth);

//      Need to set: mgcg.interpolationWidth(Rx,grid,grid2,l)
  	mgcg.interpolationWidth(Rx,all,all,level)=interpWidth; 
  	cg1.interpolationWidth(Rx,all,all,all)=interpWidth; 

//	mgcg.interpolationWidth(Rx,all,all,level)=5;
//	cg1.interpolationWidth(Rx,all,all,all)=5; 
      }
      
      for( i=0; i<numberOfInterpolationPoints(grid); i++ )
      {
	i1=IA(i,0); i2=IA(i,1); i3=IA(i,2);
        assert( MASK1(i1,i2,i3) & MappedGrid::ISinterpolationPoint );

        if( debugb & 4 ) 
          fprintf(pDebugFile,"grid=%i: pt i=%i : find a valid stencil for i=(%i,%i,%i), interpolee grid guess=%i, "
                 " ri=(%4.2f,%4.2f,%4.2f)..\n",grid,i,i1,i2,i3,inverseGrid(i1,i2,i3),inverseCoordinates(i1,i2,i3,0),
                inverseCoordinates(i1,i2,i3,1),(numberOfDimensions==2 ? 0. : inverseCoordinates(i1,i2,i3,2)) );

	InterpolationQualityEnum interpolationQuality;
	// *wdh* 030324 interpolationQuality=getInterpolationStencil(cg0,cg1,i,iv,grid,l,
	interpolationQuality=getInterpolationStencil(cg0,cg1,i,iv,grid,l+1,
						inverseGrid,
						interpoleeGrid,
						interpoleeLocation,
						interpolationPoint,
						variableInterpolationWidth,
						interpolationCoordinates,
						inverseCoordinates );

	if( (int)interpolationQuality >= (int)canInterpolateWithExtrapolation )
	{
	  // look for another possible interpolee grid by searching for some nearby fine grid interpolation
          // points that interpolate from some other grid. (This could fail in some cases!)

          if( debugb & 4 ) fprintf(pDebugFile," --> look for another grid to interpolate from..\n");

          if( isRectangular )
	  {
            X(0,0)=CENTER10(i1,i2,i3);
            X(0,1)=CENTER11(i1,i2,i3);
            X(0,2)=CENTER12(i1,i2,i3);
	  }
	  else
	  {
  	    for( axis=0; axis<numberOfDimensions; axis++ )
	      X(0,axis)=CENTER(i1,i2,i3,axis);
	  }
	  
	  int grid2=interpoleeGrid(i);
	  bool notDone=TRUE;
          const int width=1;
	  const int m3Start=numberOfDimensions<3 ? 0 : -width;
	  const int m3End  =numberOfDimensions<3 ? 0 : +width;
	  j1=i1; j2=i2; j3=i3; //
	  for( int m3=m3Start; m3<=m3End && notDone; m3++ )
	  {
	    j3+=m3;
	    for( int m2=-width; m2<=width && notDone; m2++ )
	    {
	      j2+=m2;
	      for( int m1=-width; m1<=width && notDone ; m1++ )
	      {
		j1+=m1;

		// skip points outside (these could be periodic images of interp points) *wdh* 021006
		if( j1<EIR(0,0) || j1>EIR(1,0) ) continue;
		if( j2<EIR(0,1) || j2>EIR(1,1) ) continue;
		if( numberOfDimensions==3 && (j3<EIR(0,2) || j3>EIR(1,2)) ) continue;

                if( debugb & 4 ) 
		{
		  if( (MASK1(j1,j2,j3) & MappedGrid::ISinterpolationPoint) )
		    fprintf(pDebugFile," ...nearby pt interps from grid=%i\n",INVERSEGRID(j1,j2,j3));
		}
		if( (MASK1(j1,j2,j3) & MappedGrid::ISinterpolationPoint) && INVERSEGRID(j1,j2,j3)!=grid2 )
		{
		  if( debugb & 4 ) 
                    fprintf(pDebugFile," ... grid %i is another possible interpolee grid\n",INVERSEGRID(j1,j2,j3));
		

		  int gridI=INVERSEGRID(j1,j2,j3);
                  checkForBetterQualityInterpolation( x,gridI,interpolationQuality,
						      cg0,cg1,i,iv,grid,l,
						      inverseGrid,
						      interpoleeGrid,
						      interpoleeLocation,
						      interpolationPoint,
						      variableInterpolationWidth,
						      interpolationCoordinates,
						      inverseCoordinates );
		  

		  if( (int)interpolationQuality < (int)canInterpolateWithExtrapolation )
		  {
		    notDone=false;
		    break;
		  }


		} // end for m1
		j1-=m1;
	      }
	      j2-=m2;
	    }
	    j3-=m3;
	  } // end for m3
	  
          if( (int)interpolationQuality >= (int)canInterpolateQualityBad )  // ***** is thsi check ok?
	  {
            fprintf(pDebugFile," ***WARNING: unable to find a better grid to interpolate from --"
                      " do a more careful search now...\n");

	    if( isRectangular )
	    {
	      X(0,0)=CENTER10(i1,i2,i3);
	      X(0,1)=CENTER11(i1,i2,i3);
	      X(0,2)=CENTER12(i1,i2,i3);
	    }
	    else
	    {
	      for( axis=0; axis<numberOfDimensions; axis++ )
		X(0,axis)=CENTER(i1,i2,i3,axis);
	    }
	    
            for( int g=cg1.numberOfComponentGrids()-1; g>=0; g-- )
	    {
	      // check to see if we can interpolate from grid g -- should check bounding box?
	      
              if( g==grid )
                continue;
	      
              int gridI=g;

	      checkForBetterQualityInterpolation( x,gridI,interpolationQuality,
						  cg0,cg1,i,iv,grid,l,
						  inverseGrid,
						  interpoleeGrid,
						  interpoleeLocation,
						  interpolationPoint,
						  variableInterpolationWidth,
						  interpolationCoordinates,
						  inverseCoordinates );

	      if( (int)interpolationQuality < (int)canInterpolateWithExtrapolation )
	      {
		notDone=false;
		break;
	      }

	    } // end for g
	    
	      
	    if( (int)interpolationQuality >= (int)canInterpolateQualityBad )
	    {
	      printF(" ***Ogmg:ERROR:buildExtraLevels: level=%i unable to find a better grid to interpolate from!\n",
                    level);
              printF("    grid=%i (%s) iv=(%i,%i,%i) interpoleeGrid=%i (%s) width=%i r=(%f,%f,%f) interpQuality=%i\n",
		     grid,(const char*)cg0[grid].getName(),iv[0],iv[1],iv[2],
                      interpoleeGrid(i),
                     (const char*)cg0[interpoleeGrid(i)].getName(),variableInterpolationWidth(i),
		     INVERSECOORDINATES(iv[0],iv[1],iv[2],0),INVERSECOORDINATES(iv[0],iv[1],iv[2],1),
                     (numberOfDimensions==3 ? INVERSECOORDINATES(iv[0],iv[1],iv[2],2) : 0.),
                      interpolationQuality);

	      fprintf(pDebugFile," ***Ogmg:ERROR:buildExtraLevels: level=%i unable to find a better grid to interpolate from!\n",
                    level);
              fprintf(pDebugFile,"    grid=%i (%s) iv=(%i,%i,%i) interpoleeGrid=%i (%s) width=%i r=(%f,%f,%f) interpQuality=%i\n",
		     grid,(const char*)cg0[grid].getName(),iv[0],iv[1],iv[2],
                      interpoleeGrid(i),
                     (const char*)cg0[interpoleeGrid(i)].getName(),variableInterpolationWidth(i),
		     INVERSECOORDINATES(iv[0],iv[1],iv[2],0),INVERSECOORDINATES(iv[0],iv[1],iv[2],1),
                     (numberOfDimensions==3 ? INVERSECOORDINATES(iv[0],iv[1],iv[2],2) : 0.),
                      interpolationQuality);
	      
	    }

	  }
	

	}  // if quality bad
	
        if( debugb & 4 ) fprintf(pDebugFile," ---< pt i=%i : interpolate from grid %i location=(%i,%i,%i)..\n",i,interpoleeGrid(i),
				 interpoleeLocation(i,0),interpoleeLocation(i,1),(numberOfDimensions==2 ? 0 : interpoleeLocation(i,2)) );
	
      }  // end for( i )

      timeForValidStencil+=getCPU()-timea;
      
      if( debugb & 4 )
      {
	display(interpolationPoint,sPrintF(buff,"interpolationPoint, level=%i grid=%i",level,grid),pDebugFile);
	display(interpoleeLocation,sPrintF(buff,"interpoleeLocation, level=%i grid=%i",level,grid),pDebugFile);
	display(interpolationCoordinates,sPrintF(buff,"interpolationCoordinates, level=%i grid=%i",level,grid),pDebugFile);
	display(variableInterpolationWidth,sPrintF(buff,"variableInterpolationWidth, level=%i grid=%i",level,grid),pDebugFile);
      }
      
    }  // end for grid
    timeForBuildInterpolation+=getCPU()-timeb;
    
    markGhostPoints( cg1 );
    if( debugb & 4 )
      for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
	displayMask(cg1[grid].mask(),"Ogmg::buildExtraLevels: mask1 after markGhostPoints",pDebugFile);

    // display(cg1.numberOfInterpolationPoints,"cg1.numberOfInterpolationPoints");
    
    cg1.numberOfInterpolationPoints(Rg)=numberOfInterpolationPoints(Rg);

    // cg1.update();
    
    if( debugb & 4 )
      display(cg1.numberOfInterpolationPoints,"cg1.numberOfInterpolationPoints",debugFile);

    // *wdh* 091201 -- for parallel see classify.C l. 3879 ---


  // now we know how many interpolation points there are so we can create the arrays in the cg.
    cg1.update(
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeGrid           |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpolationCoordinates,
      CompositeGrid::COMPUTEnothing);


    // Fill in the interpolation info.

//        printF("*** mgcg.interpolationIsAllExplicit()=%i\n",mgcg.interpolationIsAllExplicit());
//        printF("*** cg0.interpolationIsAllExplicit()=%i\n",cg0.interpolationIsAllExplicit());
//        printF("*** cg1.interpolationIsAllExplicit()=%i\n",cg1.interpolationIsAllExplicit());
// 	cg1.interpolationIsImplicit.display("cg1.interpolationIsImplicit");

    IntegerArray gridStart(cg1.numberOfComponentGrids()), ng(cg1.numberOfComponentGrids());
    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
    {

      if( numberOfInterpolationPoints(grid) > 0 )
      {
        #ifdef USE_PPP
        intSerialArray interpoleeGrid1; getLocalArrayWithGhostBoundaries(cg1.interpoleeGrid[grid],interpoleeGrid1);
        intSerialArray interpolationPoint1; getLocalArrayWithGhostBoundaries(cg1.interpolationPoint[grid],interpolationPoint1);
        intSerialArray interpoleeLocation1; getLocalArrayWithGhostBoundaries(cg1.interpoleeLocation[grid],interpoleeLocation1);
        intSerialArray variableInterpolationWidth1; 
                       getLocalArrayWithGhostBoundaries(cg1.variableInterpolationWidth[grid],variableInterpolationWidth1);
        realSerialArray interpolationCoordinates1; 
                       getLocalArrayWithGhostBoundaries(cg1.interpolationCoordinates[grid],interpolationCoordinates1);
        #else
        const intArray & interpoleeGrid1            = cg1.interpoleeGrid[grid]; 
        const intArray & interpolationPoint1        = cg1.interpolationPoint[grid];
	intArray & interpoleeLocation1              = cg1.interpoleeLocation[grid];
        intArray & variableInterpolationWidth1      = cg1.variableInterpolationWidth[grid];
        const realArray & interpolationCoordinates1 = cg1.interpolationCoordinates[grid];
        #endif  

	// intArray & interpoleeGrid1            = cg1.interpoleeGrid[grid];
	// intArray & interpolationPoint1        = cg1.interpolationPoint[grid];
	// intArray & interpoleeLocation1        = cg1.interpoleeLocation[grid];
        // intArray & variableInterpolationWidth1= cg1.variableInterpolationWidth[grid];
	// realArray    & interpolationCoordinates1  = cg1.interpolationCoordinates[grid];

	intSerialArray & interpoleeGrid            = interpoleeGridA[grid];
	intSerialArray & interpolationPoint        = interpolationPointA[grid];
	intSerialArray & interpoleeLocation        = interpoleeLocationA[grid];
        intSerialArray & variableInterpolationWidth=variableInterpolationWidthA[grid];
	realSerialArray & interpolationCoordinates = interpolationCoordinatesA[grid]; 

      const int *interpoleeLocationp = interpoleeLocation.Array_Descriptor.Array_View_Pointer1;
      const int interpoleeLocationDim0=interpoleeLocation.getRawDataSize(0);
#define INTERPOLEELOCATION(i0,i1) interpoleeLocationp[i0+interpoleeLocationDim0*(i1)]
      int *interpoleeLocation1p = interpoleeLocation1.Array_Descriptor.Array_View_Pointer1;
      const int interpoleeLocation1Dim0=interpoleeLocation1.getRawDataSize(0);
#define INTERPOLEELOCATION1(i0,i1) interpoleeLocation1p[i0+interpoleeLocation1Dim0*(i1)]

      const int *interpolationPointp = interpolationPoint.Array_Descriptor.Array_View_Pointer1;
      const int interpolationPointDim0=interpolationPoint.getRawDataSize(0);
#define INTERPOLATIONPOINT(i0,i1) interpolationPointp[i0+interpolationPointDim0*(i1)]
      int *interpolationPoint1p = interpolationPoint1.Array_Descriptor.Array_View_Pointer1;
      const int interpolationPoint1Dim0=interpolationPoint1.getRawDataSize(0);
#define INTERPOLATIONPOINT1(i0,i1) interpolationPoint1p[i0+interpolationPoint1Dim0*(i1)]

      const real *interpolationCoordinatesp = interpolationCoordinates.Array_Descriptor.Array_View_Pointer1;
      const int interpolationCoordinatesDim0=interpolationCoordinates.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES(i0,i1) interpolationCoordinatesp[i0+interpolationCoordinatesDim0*(i1)]
      real *interpolationCoordinates1p = interpolationCoordinates1.Array_Descriptor.Array_View_Pointer1;
      const int interpolationCoordinates1Dim0=interpolationCoordinates1.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES1(i0,i1) interpolationCoordinates1p[i0+interpolationCoordinates1Dim0*(i1)]


      int * interpoleeGridp = interpoleeGrid.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID(i0) interpoleeGridp[i0]
      int * interpoleeGrid1p = interpoleeGrid1.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID1(i0) interpoleeGrid1p[i0]
      int * ngp = ng.Array_Descriptor.Array_View_Pointer0;
#define NG(i0) ngp[i0]

      int * gridStartp = gridStart.Array_Descriptor.Array_View_Pointer0;
#define GRIDSTART(i0) gridStartp[i0]
      const int * variableInterpolationWidthp = variableInterpolationWidth.Array_Descriptor.Array_View_Pointer0;
#define VARIABLEINTERPOLATIONWIDTH(i0) variableInterpolationWidthp[i0]
      int * variableInterpolationWidth1p = variableInterpolationWidth1.Array_Descriptor.Array_View_Pointer0;
#define VARIABLEINTERPOLATIONWIDTH1(i0) variableInterpolationWidth1p[i0]

        // order the interpolation points by interpolee grid.
	ng=0;
        int i;
        const int nig=numberOfInterpolationPoints(grid);
	for( i=0; i<nig; i++ )
	  NG(INTERPOLEEGRID(i))++;

	GRIDSTART(0)=0;
        int grid2;
	for( grid2=1; grid2<cg1.numberOfComponentGrids(); grid2++ )
	  GRIDSTART(grid2)=GRIDSTART(grid2-1)+NG(grid2-1);
	
        // ***** we need to assign the interpolationStartEndIndex 
        // **** this needs to be set on multigridLevel[0] too ********

        // for now we assume that the interpolation is implicit on coarser levels *** fix this ***

        cg1.interpolationIsAllExplicit()=false;
        cg1.interpolationIsAllImplicit()=true;
	
	for( grid2=0; grid2<cg1.numberOfComponentGrids(); grid2++ )
	{

	  if( NG(grid2)>0 )
	  {
            mgcg.interpolationIsImplicit(grid,grid2,level)=true;
  	    cg1.interpolationIsImplicit(grid,grid2,0)=true;

	    cg1.interpolationStartEndIndex(0,grid,grid2)=GRIDSTART(grid2);              // start value
	    cg1.interpolationStartEndIndex(1,grid,grid2)=GRIDSTART(grid2)+NG(grid2)-1;  // end value
	    if( true || cg1.interpolationIsImplicit(grid,grid2,0) )
	      cg1.interpolationStartEndIndex(2,grid,grid2)= cg1.interpolationStartEndIndex(1,grid,grid2);
           // fix this: put any implicit points first
// 	   else if( ngi(grid2)>0 )
// 	     cg1.interpolationStartEndIndex(2,grid,grid2)=GRIDSTART(grid2)+ngi(grid2)-1; // end value for implicit pts.
	  }
	}


        if( numberOfDimensions==2 )
	{
	  for( i=0; i<nig; i++ )
	  {
	    grid2=INTERPOLEEGRID(i);
	    int j=GRIDSTART(grid2);
	    INTERPOLEEGRID1(j)=grid2;
	    INTERPOLATIONPOINT1(j,0)=INTERPOLATIONPOINT(i,0);
	    INTERPOLATIONPOINT1(j,1)=INTERPOLATIONPOINT(i,1);
	    INTERPOLEELOCATION1(j,0)=INTERPOLEELOCATION(i,0);
	    INTERPOLEELOCATION1(j,1)=INTERPOLEELOCATION(i,1);
	    INTERPOLATIONCOORDINATES1(j,0)=INTERPOLATIONCOORDINATES(i,0);
	    INTERPOLATIONCOORDINATES1(j,1)=INTERPOLATIONCOORDINATES(i,1);
	    variableInterpolationWidth1(j)=variableInterpolationWidth(i);
	  
	    GRIDSTART(grid2)++;
	  }
	}
	else
	{
	  for( i=0; i<nig; i++ )
	  {
	    grid2=INTERPOLEEGRID(i);
	    int j=GRIDSTART(grid2);
	    interpoleeGrid1(j)=grid2;
	    INTERPOLATIONPOINT1(j,0)=INTERPOLATIONPOINT(i,0);
	    INTERPOLATIONPOINT1(j,1)=INTERPOLATIONPOINT(i,1);
	    INTERPOLATIONPOINT1(j,2)=INTERPOLATIONPOINT(i,2);
	    INTERPOLEELOCATION1(j,0)=INTERPOLEELOCATION(i,0);
	    INTERPOLEELOCATION1(j,1)=INTERPOLEELOCATION(i,1);
	    INTERPOLEELOCATION1(j,2)=INTERPOLEELOCATION(i,2);
	    INTERPOLATIONCOORDINATES1(j,0)=INTERPOLATIONCOORDINATES(i,0);
	    INTERPOLATIONCOORDINATES1(j,1)=INTERPOLATIONCOORDINATES(i,1);
	    INTERPOLATIONCOORDINATES1(j,2)=INTERPOLATIONCOORDINATES(i,2);
	    VARIABLEINTERPOLATIONWIDTH1(j)=VARIABLEINTERPOLATIONWIDTH(i);
	  
	    GRIDSTART(grid2)++;
	  }
	}
	
      }
      // cg.numberOfInterpolationPoints(grid)=numberOfInterpolationPoints(grid);

    }
  
//  Tell the CompositeGrid that the interpolation data have been computed:
    cg1->computedGeometry |=
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpoleeGrid;

    // we also need to mark each MappedGrid (or else we lose the mask if we put/get to a file)
    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
      cg1[grid]->computedGeometry |= MappedGrid::THEmask;     // added 030829 *wdh*

    if( debugb & 16 && ps!=NULL )
    {
      ps->erase();
      PlotIt::plot(*ps,mgcg);
      PlotIt::plot(*ps,cg1);
    }
    
    
  } // end for l 
  
  // *wdh* 061123 -- now update the collections so that the main 
  // mgcg.update( CompositeGrid::THEmultigridLevel);

  //  *wdh* 061123 Tell the CompositeGrid that the interpolation data have been computed:
  // fill in number of interpolation pts from all levels
  if( newWay )
  {
    for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )
    {
      Range Rl = Rg + mgcg.numberOfComponentGrids()*l;
      mgcg.numberOfInterpolationPoints(Rl)=mgcg.multigridLevel[l].numberOfInterpolationPoints(Rg);
    }
    int gridl=mgcg.numberOfComponentGrids();
    for( int l=1; l<mgcg.numberOfMultigridLevels(); l++ )
    {
      CompositeGrid & cgl = mgcg.multigridLevel[l];
      for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
      {
	mgcg->interpolationPoint[gridl].reference(cgl->interpolationPoint[grid]);
	mgcg->interpoleeGrid[gridl].reference(cgl->interpoleeGrid[grid]);
	mgcg->interpoleeLocation[gridl].reference(cgl->interpoleeLocation[grid]);
	mgcg->variableInterpolationWidth[gridl].reference(cgl->variableInterpolationWidth[grid]);
	mgcg->interpolationCoordinates[gridl].reference(cgl->interpolationCoordinates[grid]);
        gridl++;
      }

      for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
      {
	for( int grid2=0; grid2<cgl.numberOfComponentGrids(); grid2++ )
	{
	  int g=grid+l*mgcg.numberOfComponentGrids();
	  int g2=grid2+l*mgcg.numberOfComponentGrids();
	  mgcg.interpolationStartEndIndex(Range(0,2),g,g2)=cgl.interpolationStartEndIndex(Range(0,2),grid,grid2);
	}
      }
      
    }
    mgcg.updateReferences();
    
    mgcg->computedGeometry |=
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpoleeGrid;

    // ::display(mgcg.numberOfInterpolationPoints,"buildExtra: mgcg.numberOfInterpolationPoints");    

  }
  

//   for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )
//   {
//     mgcg.multigridLevel[l]->computedGeometry |=
//       CompositeGrid::THEmask                     |
//       CompositeGrid::THEinterpolationCoordinates |
//       CompositeGrid::THEinterpolationPoint       |
//       CompositeGrid::THEinterpoleeLocation       |
//       CompositeGrid::THEinterpoleeGrid;
//   }
  

  delete [] iaA; // *wdh* added 040820
  delete [] interpoleeGridA;
  delete [] interpolationPointA;
  delete [] interpoleeLocationA;
  delete [] variableInterpolationWidthA;
  delete [] interpolationCoordinatesA;

//   for( l=level0; l<level0+numberOfExtraLevels; l++ )
//   {
//     mgcg.multigridLevel[level]->computedGeometry |=
//       CompositeGrid::THEmask                     |
//       CompositeGrid::THEinterpolationCoordinates |
//       CompositeGrid::THEinterpolationPoint       |
//       CompositeGrid::THEinterpoleeLocation       |
//       CompositeGrid::THEinterpoleeGrid;
//   }

  // printF(" **** debug=%i, debug & 1 = %i,  debug & 2 = %i,  debug & 4 = %i \n",debug,debug&1,debug&2,debug&4);
  
  if( debugb & 2 || debugb & 4 )
  {
    // check the grids
    Ogen ogen;
    for( l=0; l<mgcg.numberOfMultigridLevels(); l++ )
    {
      int level=l;
      CompositeGrid & cg0 = l==0 ? mgcg : mgcg.multigridLevel[l];
      int rt=checkOverlappingGrid(cg0);
      if( rt==0 )
        printF("$$$$ checkOverlappingGrid: level %i is OK\n",l);
      else
        printF("$$$$ checkOverlappingGrid: level %i is NOT OK ****ERROR****\n",l);

      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      int numErrors=0;
      IntegerArray errorsPerGrid(cg0.numberOfComponentGrids());
      errorsPerGrid=0;
      const bool checkOneSidedAtBoundaries=false;
      for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      {
        printF(" check: level=%i grid=%i numberOfInterpolationPoints=%i\n",
                       l,grid,cg0.numberOfInterpolationPoints(grid));
	
	MappedGrid & mg = cg0[grid];
        const intArray & mask = mg.mask();
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( mask(i1,i2,i3)>0 )
	  {
	    if( !ogen.canDiscretize( mg,iv,checkOneSidedAtBoundaries ) ) // Note: iv==(i1,i2,i3)
	    {
	      numErrors++;
	      errorsPerGrid(grid)++;
	      printF("ERROR: level=%i grid=%i Discretization point (%i,%i,%i) cannot be discretized\n",
		     l,grid,i1,i2,i3);
	      fprintf(pDebugFile,"ERROR: level=%i grid=%i Discretization point (%i,%i,%i) cannot be discretized\n",
		     l,grid,i1,i2,i3);

              if( numberOfDimensions==3 )
	      {
		Range J1(i1-1,i1+1),J2(i2-1,i2+1),J3(i3-1,i3+1);
		displayMask(mask(J1,J2,J3),"mask near i1,i2,i3",pDebugFile);
	      }
	      
	    }
	  }
	}
        //
        // Now we need to check that there are no discretization points in the ghost point region of
        // interpolation boundaries. (This is a mistake that can be made in the above algorithm)
        const IntegerArray & gir = mg.gridIndexRange();
        I3=gir(0,2);
        const int numGhostLines=orderOfAccuracy/2; // check this many ghost lines
	for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
	{
	  for( int dir=0; dir<cg0.numberOfDimensions(); dir++ )
	  {
	    if( dir!=axis )
	    {
	      int na=gir(0,dir);
	      int nb=gir(1,dir);
              // check adjacent ghost lines (thus corners) if the adjacent boundary is interpolation
	      if( mg.boundaryCondition(0,dir)==0 ) na-=numGhostLines;
	      if( mg.boundaryCondition(1,dir)==0 ) nb+=numGhostLines;
	      Iv[dir]=Range(na,nb);
	    }
	  }
	  for( int side=0; side<=1; side++ )
	  {
            if( mg.boundaryCondition(side,axis)==0 )
	    {
	      const int is=1-2*side;
	      const int ghost=1;
	      // check "numGhostLines" different ghost lines :

	      Iv[axis]=Range(mg.gridIndexRange(side,axis)-is,mg.gridIndexRange(side,axis)-is*numGhostLines);

	      FOR_3(i1,i2,i3,I1,I2,I3)
	      {
		if( mask(i1,i2,i3)>0 )
		{
		  numErrors++;
  	          errorsPerGrid(grid)++;
		  printF("ERROR: level=%i, grid=%i, ghost point (%i,%i,%i) (side,axis)=(%i,%i) mask=%i, "
                         "is a discretization pt!\n", l,grid,i1,i2,i3,side,axis,mask(i1,i2,i3));
		  fprintf(pDebugFile,"ERROR: level=%i, grid=%i, ghost point (%i,%i,%i) (side,axis)=(%i,%i) mask=%i, "
                         "is a discretization pt!\n", l,grid,i1,i2,i3,side,axis,mask(i1,i2,i3));
		}
	      }
	    }
	  }
	}
      }
      if( numErrors>0 )
      {
	printf("$$$$ check points on level %i **ERROR** there were %i invalid points.\n",
	       l,numErrors);
        if( true )
	{
          fprintf(pDebugFile,
                  "\n*****************************************************************************\n"
                  "$$$$ check points on level %i **ERROR** there were %i invalid points.\n",
	          l,numErrors);
	  for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
	  {
	    if( errorsPerGrid(grid)>0 )
	    {
	      displayMask(cg0[grid].mask(),sPrintF(buff,"Ogmg::buildExtraLevels: level=%i grid=%i errors=%i, mask:",
						   l,grid,errorsPerGrid(grid)),pDebugFile);
	    }
	  }
	}
	
	Overture::abort("ERROR");
      }
      else
      {
	printf("$$$$ check points on level %i **PASSED***\n",l);
      }

    }
  }
  



  if( (debugb & 8) && level0==0 && ( mgcg.numberOfInterpolationPoints(0)>0)  )
  {
    // now test out the validity of the newly created levels.

    for( l=level0; l<level0+numberOfExtraLevels; l++ )
    {
      level=l+1;
      
      printF("\n\n **************** check extra level %i ******************* \n\n",level);
      
      checkGrid( mgcg.multigridLevel[level],ps,debugb );
      
    }

    // exit(0);
  }
  

  


  if( false )
  {
   mgcg.interpolationStartEndIndex.display("buildExtraLevels: END:mgcg.interpolationStartEndIndex");
   mgcg.multigridLevel[0].interpolationStartEndIndex.display("buildExtraLevels:END: mgcg.multigridLevel[0].interpolationStartEndIndex");
  }
  

  if( gridCheckFile!=NULL )
  {
    fclose(gridCheckFile);
  }


  tm[timeForBuildExtraLevels]+=getCPU()-time0;
  if( debugb & 4 )
    printF(" ****tm[timeForBuildExtraLevels]=%8.2e buildMask=%8.2e buildInterp=%8.2e validStencil=%8.2e ****\n",
           tm[timeForBuildExtraLevels],timeForBuildMask,timeForBuildInterpolation,timeForValidStencil);
  
  return 0;
}


//! Check to see if a point x can interpolate from gridI with a better quality interpolation
/*!
 /param gridI (input): try interpolating from this grid. 
 /param interpolationQuality (input/output) : on input the current quality, on output the new quality
 /param inverseGrid, inverseCoordinates (input/output) :
*/
int Ogmg::
checkForBetterQualityInterpolation( realSerialArray & x, int gridI, InterpolationQualityEnum & interpolationQuality,
				    CompositeGrid & cg0,
				    CompositeGrid & cg1,
				    int i,
				    int iv[3],
				    int grid,
				    int l,
				    intSerialArray & inverseGrid,
				    intSerialArray & interpoleeGrid,
				    intSerialArray & interpoleeLocation,
				    intSerialArray & interpolationPoint,
				    intSerialArray & variableInterpolationWidth,
				    realSerialArray & interpolationCoordinates,
				    realSerialArray & inverseCoordinates )
{
  assert( gridI>=0 && gridI<cg1.numberOfComponentGrids());
		
  // invert mapping to get coordinates of the new interpolation pt. 
  MappedGrid & gI = cg1[gridI];
  realSerialArray r(1,3);
  r=-1.;
  #ifdef USE_PPP
    gI.mapping().getMapping().inverseMapS(x,r);
  #else
    gI.mapping().getMapping().inverseMap(x,r);
  #endif

//     if( true ) // *** TEMP ***
//     {
//       fprintf(pDebugFile,"checkForBetter: grid=%i donor=%i x=(%8.2e,%8.2e,%8.2e) r=(%8.2e,%8.2e,%8.2e)\n",
// 	     grid,gridI,x(0,0),x(0,1),x(0,2),r(0,0),r(0,1),r(0,2));
//     }
    

  if( max(fabs(r-.5))<2. ) // we allow extrapolation if necessary
  {
    int axis;
    const int numberOfDimensions=cg0.numberOfDimensions();
    int &i1=iv[0], &i2=iv[1], &i3=iv[2];
    
    const IntegerArray & extended = gI.extendedIndexRange(); 
    const real offset = parameters.allowExtrapolationOfInterpolationPoints ? 1. : 0.; // *wdh* 040903
    bool extrapolated=false;
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      // normally limit r to [0,1] unless the boundary is interpolation, then limit by .25*Delta r
      // from the last interpolation point.
      const real dr = gI.gridSpacing(axis);
      real rMin = gI.boundaryCondition(Start,axis)!=0  ? -dr*offset :
	                                            (extended(Start,axis)+.25)*dr; // move in .25*Dr from the end
      real rMax = gI.boundaryCondition(End  ,axis)!=0  ? 1.+dr*offset :
	                                              (extended(End,  axis)-.25)*dr;
		      
      if( r(0,axis)<rMin-.01 || r(0,axis)>rMax+.01 )  // *** what should these be ???
      {
	extrapolated=true;
      }
      
      r(0,axis)=max(rMin,min(rMax,r(0,axis)));
    }

    // we save the current best guess for which grid to interp from.
    int inverseGridSave=inverseGrid(i1,i2,i3);
    inverseGrid(i1,i2,i3)=gridI;
    realSerialArray rSave(1,3);
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      rSave(0,axis)=inverseCoordinates(i1,i2,i3,axis);
      inverseCoordinates(i1,i2,i3,axis)=r(0,axis);
    }

    InterpolationQualityEnum newInterpQuality;
    // *wdh* 030324 newInterpQuality=getInterpolationStencil(cg0,cg1,i,iv,grid,l,
    newInterpQuality=getInterpolationStencil(cg0,cg1,i,iv,grid,l+1,
					     inverseGrid,
					     interpoleeGrid,
					     interpoleeLocation,
					     interpolationPoint,
					     variableInterpolationWidth,
					     interpolationCoordinates,
					     inverseCoordinates );

    if( extrapolated )
    {
      // change the quality if we extrapolated to be at best canInterpolateWithExtrapolation
      newInterpQuality = (InterpolationQualityEnum) max( (int)canInterpolateWithExtrapolation,(int)newInterpQuality);
    }
    
    if( (int)newInterpQuality < (int)interpolationQuality )
    {
      // we have found a better quality interpolation
      interpolationQuality=newInterpQuality;
      if( (int)interpolationQuality >= (int)canInterpolateWithExtrapolation )
      {
	if( Ogmg::debug & 4 ) fprintf(pDebugFile," +++++ a better quality interpolee grid found: gridI=%i, "
		"but keep looking..\n",gridI);
      }
      else
      {
	if( Ogmg::debug & 4 ) fprintf(pDebugFile," +++++ new interpolee grid found: gridI=%i\n",gridI);
	// notDone=FALSE;
	// break;
      }
		      
    }
    else
    {
      // reset to previous best guess
      inverseGrid(i1,i2,i3)=inverseGridSave;
      for( axis=0; axis<numberOfDimensions; axis++ )
	inverseCoordinates(i1,i2,i3,axis)=rSave(0,axis);
                     
    }
    
  }

  return 0;
}

#define FOR_IJ() \
  for( i3=I3Base, j3=J3Base; i3<=I3Bound; i3++,j3++ ) \
  for( i2=I2Base, j2=J2Base; i2<=I2Bound; i2++,j2++ ) \
  for( i1=I1Base, j1=J1Base; i1<=I1Bound; i1++,j1++ )


int Ogmg::  // should be used by ogen too
markGhostPoints( CompositeGrid & cg )
// =========================================================================================
//  Mark ghost points values next to boundaries.
// =========================================================================================
{
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    //
    // For each used point on the boundary, mark its ghost points.
    //
    MappedGrid & g = cg[grid];
    intArray & maskd = g.mask();
    #ifdef USE_PPP
      intSerialArray mask; getLocalArrayWithGhostBoundaries(maskd,mask);
    #else
      const intArray & mask = maskd;
    #endif

    int * maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=mask.getRawDataSize(0);
    const int maskDim1=mask.getRawDataSize(1);
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]	


    // g.mask().display("Mask before marking ghost values");
    // if( debug & 8 ) displayMask(mask,"Mask before marking ghost values",logFile);

    Index I[3], &I1=I[0], &I2=I[1], &I3=I[2];
    Index J[3], &J1=J[0], &J2=J[1], &J3=J[2];
    int i1,i2,i3,j1,j2,j3;
    
    for( int axis=0; axis<3; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
        getBoundaryIndex(g.dimension(),side,axis,I1,I2,I3);
        // *wdh* 980626 I[axis]=g.indexRange(side,axis);
        I[axis]=g.gridIndexRange(side,axis);
	
        getGhostIndex(g.dimension(),side,axis,J1,J2,J3);

	int includeGhost=1; // include parallel ghost 
        bool ok;
	ok = ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3,includeGhost);
	ok = ParallelUtility::getLocalArrayBounds(maskd,mask,J1,J2,J3,includeGhost);

	if( !ok ) continue;
	 
	const Integer pm1 = 2 * side - 1;
	// if( g.boundaryCondition(side,axis)!=0 ) // do not change periodic edges, these may be needed for interp.
        int lastGhost= g.extendedIndexRange(side,axis);
	  
        const int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();
        const int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();

	if( g.boundaryFlag(side,axis)==MappedGrid::physicalBoundary )
	{
	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
	  { 
	    J[axis] = k; 
            const int J1Base=J1.getBase(), J2Base=J2.getBase(), J3Base=J3.getBase();
	    FOR_IJ()
            {
              if( MASK(i1,i2,i3) )
	      {
                MASK(j1,j2,j3) = MappedGrid::ISghostPoint;
	      }
	      else
	      {
                MASK(j1,j2,j3)=0;
	      }
	      
	    }
	  }
	}
	else if( g.boundaryCondition()(side,axis)==0 )
	{
	  // set ghost lines outside interpolation edges to zero.
	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
	  { 
	    J[axis] = k; 
	    mask(J1,J2,J3) = 0;
	  }
	}
	else if( g.boundaryFlag(side,axis)==MappedGrid::mixedPhysicalInterpolationBoundary )
	{
          lastGhost=g.gridIndexRange(side,axis);
	  for (Integer k=g.dimension(side,axis); k!=lastGhost; k-=pm1)
	  { 
	    J[axis] = k; 
            const int J1Base=J1.getBase(), J2Base=J2.getBase(), J3Base=J3.getBase();

	    FOR_IJ()
            {
              if( MASK(i1,i2,i3) && MASK(j1,j2,j3)>0)
	      {
                MASK(j1,j2,j3) = MappedGrid::ISghostPoint;
	      }
	      else if( MASK(i1,i2,i3)==0 )
	      {
                MASK(j1,j2,j3)=0;
	      }
	      
	    }
	  }
	}

      }
    }
    g.mask().periodicUpdate();
    // if( debug& 8 ) displayMask(g.mask(),"Mask afer marking ghost values",logFile);
  }

  return 0;
}



  

int Ogmg::
getInterpolationCoordinates( CompositeGrid & cg0, // finer grid
                             CompositeGrid & cg1, // new coarser grid
                             const IntegerArray & ib,     // check these points...
			     const int grid,            // ..on this grid
                             const IntegerArray & gridsToCheck, // ..from these grids
                             realSerialArray & rb,      // return these values
                             const bool isRectangular,
                             int iv0[3], real dx0[3], real xab0[2][3],   // these are used by Macros!
                             int iv1[3], real dx1[3], real xab1[2][3] )
// ==================================================================================================
// /Description: ** new version that does many points at a time
//     Given a new interpolation point on a coarse grid, determine the r coordinates on the 
//   interpolee grid. 
//
// /ib (input) : ib(i,4) : (i,j1,j2,j3) 
// ==================================================================================================
{
  const int numberOfDimensions=cg0.numberOfDimensions();

  #ifdef USE_PPP
    intSerialArray interpoleeGrid0; getLocalArrayWithGhostBoundaries(cg0.interpoleeGrid[grid],interpoleeGrid0);
    intSerialArray interpolationPoint0; getLocalArrayWithGhostBoundaries(cg0.interpolationPoint[grid],interpolationPoint0);
    realSerialArray interpolationCoordinates0; getLocalArrayWithGhostBoundaries(cg0.interpolationCoordinates[grid],
                                                                                interpolationCoordinates0);
  #else
    const intArray & interpoleeGrid0 =cg0.interpoleeGrid[grid];  // ************ should use cg0 ****
    const intArray & interpolationPoint0 =cg0.interpolationPoint[grid];
    const realArray & interpolationCoordinates0 =cg0.interpolationCoordinates[grid];
  #endif    


  // const intArray & interpoleeGrid0 =cg0.interpoleeGrid[grid];  // ************ should use cg0 ****
  // const realArray & interpolationCoordinates0 =cg0.interpolationCoordinates[grid];
  const real *interpolationCoordinates0p = interpolationCoordinates0.Array_Descriptor.Array_View_Pointer1;
  const int interpolationCoordinates0Dim0=interpolationCoordinates0.getRawDataSize(0);

  const int * interpoleeGrid0p = interpoleeGrid0.Array_Descriptor.Array_View_Pointer0;

  // const intArray & interpolationPoint0 =cg0.interpolationPoint[grid];
  const int *interpolationPoint0p = interpolationPoint0.Array_Descriptor.Array_View_Pointer1;
  const int interpolationPoint0Dim0=interpolationPoint0.getRawDataSize(0);

  MappedGrid & mg0 = cg0[grid];
  const realArray & center0 = mg0.center();
  int center0Dim0,center0Dim1,center0Dim2;
  real *center0p;  // defined below
#define CENTER0(i0,i1,i2,i3) center0p[i0+center0Dim0*(i1+center0Dim1*(i2+center0Dim2*(i3)))]	

  MappedGrid & mg1 = cg1[grid];
  #ifdef USE_PPP
    realSerialArray center;  if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg1.center(),center);
  #else
    const realArray & center = mg1.center();
  #endif

  real *centerp;
  int centerDim0,centerDim1,centerDim2;
  if( !isRectangular )
  {
    centerp = center.Array_Descriptor.Array_View_Pointer3;
    centerDim0=center.getRawDataSize(0);
    centerDim1=center.getRawDataSize(1);
    centerDim2=center.getRawDataSize(2);

    center0p = center0.Array_Descriptor.Array_View_Pointer3;
    center0Dim0=center0.getRawDataSize(0);
    center0Dim1=center0.getRawDataSize(1);
    center0Dim2=center0.getRawDataSize(2);

  }
      
  const int maxNumToInterp = ib.getLength(0);
  if( maxNumToInterp==0 ) return 0;

  realSerialArray x(maxNumToInterp,3), r(maxNumToInterp,3);
  IntegerArray ia(maxNumToInterp);
  
  real *xp = x.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x.getRawDataSize(0);
  real *rp = r.Array_Descriptor.Array_View_Pointer1;
  const int rDim0=r.getRawDataSize(0);

  int *iap = ia.Array_Descriptor.Array_View_Pointer0;
#undef IA
#define IA(i0) iap[i0]

  real *rbp = rb.Array_Descriptor.Array_View_Pointer1;
  const int rbDim0=rb.getRawDataSize(0);
#define RB(i0,i1) rbp[i0+rbDim0*(i1)]

  int *ibp = ib.Array_Descriptor.Array_View_Pointer1;
  const int ibDim0=ib.getRawDataSize(0);

  int ii,axis;
  int j1,j2,j3;
  int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];      
  int kpv[3], &kp1=kpv[0], &kp2=kpv[1], &kp3=kpv[2];      
  
  for( int grid2=0; grid2<cg1.numberOfComponentGrids(); grid2++ )
  {
    if( gridsToCheck(grid2)==0 ) continue;  // no need to check this grid

    // collect up all new interpolation points that interpolate from grid2
    int jb=0;
    if( isRectangular )
    {
      if( numberOfDimensions==2 )
      {
	for( int i=0; i<maxNumToInterp; i++ )
	{
          ii=IB(i,0);  // index into original interpolation point arrays
	  if( INTERPOLEEGRID0(ii)==grid2 )
	  {
	    j1=IB(i,1); j2=IB(i,2); j3=IB(i,3); 

	    IA(jb)=i;  // save i
	    X(jb,0)=CENTER10(j1,j2,j3);
	    X(jb,1)=CENTER11(j1,j2,j3);
	    R(jb,0)=INTERPOLATIONCOORDINATES0(ii,0);
	    R(jb,1)=INTERPOLATIONCOORDINATES0(ii,1);
	    R(jb,2)=0.;
	    jb++;
	  }
      
	}
      }
      else
      {
	for( int i=0; i<maxNumToInterp; i++ )
	{
          ii=IB(i,0);  // index into orginal interpolation point arrays
	  if( INTERPOLEEGRID0(ii)==grid2 )
	  {
	    j1=IB(i,1); j2=IB(i,2); j3=IB(i,3); 

	    IA(jb)=i;
	    X(jb,0)=CENTER10(j1,j2,j3);
	    X(jb,1)=CENTER11(j1,j2,j3);
	    X(jb,2)=CENTER12(j1,j2,j3);
	    R(jb,0)=INTERPOLATIONCOORDINATES0(ii,0);
	    R(jb,1)=INTERPOLATIONCOORDINATES0(ii,1);
	    R(jb,2)=INTERPOLATIONCOORDINATES0(ii,2);
	    jb++;
	  }
	}
      }
    }
    else
    {
      if( numberOfDimensions==2 )
      {
	for( int i=0; i<maxNumToInterp; i++ )
	{
          ii=IB(i,0);  // index into orginal interpolation point arrays
	  if( INTERPOLEEGRID0(ii)==grid2 )
	  {
	    j1=IB(i,1); j2=IB(i,2); j3=IB(i,3); 

	    IA(jb)=i;
	    X(jb,0)=CENTER(j1,j2,j3,0);
	    X(jb,1)=CENTER(j1,j2,j3,1);
	    R(jb,0)=INTERPOLATIONCOORDINATES0(ii,0);
	    R(jb,1)=INTERPOLATIONCOORDINATES0(ii,1);
	    R(jb,2)=0.;
	    jb++;
	  }
	}
      }
      else
      {
	for( int i=0; i<maxNumToInterp; i++ )
	{
          ii=IB(i,0);  // index into orginal interpolation point arrays
	  if( INTERPOLEEGRID0(ii)==grid2 )
	  {
	    j1=IB(i,1); j2=IB(i,2); j3=IB(i,3); 

	    IA(jb)=i;
	    X(jb,0)=CENTER(j1,j2,j3,0);
	    X(jb,1)=CENTER(j1,j2,j3,1);
	    X(jb,2)=CENTER(j1,j2,j3,2);
	    R(jb,0)=INTERPOLATIONCOORDINATES0(ii,0);
	    R(jb,1)=INTERPOLATIONCOORDINATES0(ii,1);
	    R(jb,2)=INTERPOLATIONCOORDINATES0(ii,2);
	    jb++;
	  }
	}
      }
    }
    const int numToInterp=jb;
    if( numToInterp==0 ) continue;  // **** no points to interpolate
    
    // printF(" grid=%i : interpolate %i extra points from grid2=%i\n",grid,numToInterp,grid2);

    MappedGrid & g2 = cg1[grid2];
    const IntegerArray & extended = g2.extendedIndexRange(); 
    const realArray & center2 = g2.center();

    const bool g2IsRectangular=g2.isRectangular();
    real dx2[3], xra[3][3];
    if( g2IsRectangular )
    {
      g2.getDeltaX(dx2);
    }
    const real offset = parameters.allowExtrapolationOfInterpolationPoints ? 1. : 0.; // *wdh* 040903
    real rMin[3], rMax[3];
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      // normally limit r to [0,1] unless the boundary is interpolation, then limit by .25*Delta r
      // from the last interpolation point.
      const real dr = g2.gridSpacing(axis);
      rMin[axis] = g2.boundaryCondition(Start,axis)!=0  ? -dr*offset :
	                                                  (extended(Start,axis)+.25)*dr; // move in .25*Dr from the end
      rMax[axis] = g2.boundaryCondition(End  ,axis)!=0  ? 1.+dr*offset :(extended(End,  axis)-.25)*dr;
    }
      
    Range R=numToInterp,Rx=numberOfDimensions;
    
    // invert mapping to get coordinates of the new interpolation pt.
    Mapping & map2 = g2.mapping().getMapping();
    map2.useRobustInverse(true);
    r=-1.;
    #ifdef USE_PPP
      map2.inverseMapS(x(R,Rx),r);  // ** here is the expensive part **
    #else
      map2.inverseMap(x(R,Rx),r);  // ** here is the expensive part **
    #endif

    real dx[3], dr[3];
    real dr2[3]={g2.gridSpacing(0),g2.gridSpacing(1),g2.gridSpacing(2)}; //
    realSerialArray xx(1,3), rr(1,3);
    for( jb=0; jb<numToInterp; jb++ )
    {
      const int i=IA(jb);
      if( R(jb,0)!=R(jb,0) || R(jb,1)!=R(jb,1) || R(jb,2)!=R(jb,2) )
      {
        printf("Ogmg:getInterpolationCoordinates:WARNING: Something is wrong here! nan's ?\n"
               " grid=%i, donor=%i, jb=%i x=(%e,%e,%e) -> r=(%e,%e,%e) ... will try to fix...\n",
               grid,grid2,jb,x(jb,0),x(jb,1),
	       (numberOfDimensions==2 ? 0. : x(jb,2)),R(jb,0), R(jb,1),R(jb,2));
        // do this for now:
        R(jb,0)=Mapping::bogus;
        R(jb,1)=Mapping::bogus;
        R(jb,2)=Mapping::bogus;
	// OV_ABORT("error");
      }
      
      if( max(fabs(R(jb,0)),fabs(R(jb,1)),fabs(R(jb,2))) > 5. )
      {
	// pt was not invertible -- estimate it's inverse location

        const int ii=IB(i,0);
	
  	for( axis=0; axis<numberOfDimensions; axis++ )   // *wdh* 021203
  	  R(jb,axis)=INTERPOLATIONCOORDINATES0(ii,axis);	


	// correct inverseCoordinates
	// dx = distance from nearby interpolation pt to the new interpolation point.
	// add dr = (dr/dx)_2 * dx 
        int i1=INTERPOLATIONPOINT0(ii,0), i2=INTERPOLATIONPOINT0(ii,1);
        int i3=numberOfDimensions==2 ? 0 : INTERPOLATIONPOINT0(ii,2);
        int j1=IB(i,1), j2=IB(i,2), j3=IB(i,3);
	if( isRectangular )
	{
	  dx[0]=CENTER10(j1,j2,j3)-CENTER00(i1,i2,i3);
	  dx[1]=CENTER11(j1,j2,j3)-CENTER01(i1,i2,i3);
	  dx[2]=CENTER12(j1,j2,j3)-CENTER02(i1,i2,i3);

	}
	else
	{
	  for( axis=0; axis<numberOfDimensions; axis++ )
	    dx[axis]=CENTER(j1,j2,j3,axis)-CENTER0(i1,i2,i3,axis);
	}
	if( g2IsRectangular )
	{
	  dr[0]=dx[0]/dx2[0]*dr2[0];
	  dr[1]=dx[1]/dx2[1]*dr2[1];
	  dr[2]=dx[2]/dx2[2]*dr2[2];
	}    
	else
	{

	  Mapping & map2 = g2.mapping().getMapping();
	  rr=-1.;
	  xx(0,Rx)=x(jb,Rx);
          #ifdef USE_PPP
	    OV_ABORT("Ogmg::getInterpolationCoordinates:ERROR: finish this");
  	  #else
  	    map2.approximateGlobalInverse->findNearestGridPoint(0,0,xx,rr);
          #endif

          if( debug & 4 )
	    printF(" Ogmg:non-invertible pt: map2.usingRobustInverse()=%i, nearest: rr=(%8.2e,%8.2e,%8.2e)"
		   " -> i=(%i,%i,%i)\n",
		   (int)map2.usingRobustInverse(),rr(0,0),rr(0,1),rr(0,2),
		   int(rr(0,0)/g2.gridSpacing(0)),int(rr(0,1)/g2.gridSpacing(1)),int(rr(0,2)/g2.gridSpacing(2)));


	  // kv=(k1,k2,k3) = closest point on grid2 (use dr/dx from near this point)
                 
	  k3=g2.dimension(Start,axis3);
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    // dx[axis]=center(j1,j2,j3,axis)-center0(i1,i2,i3,axis);
	    kv[axis]=int( R(jb,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis) );
	    kpv[axis]=kv[axis]+1;
	    if( kpv[axis] > g2.dimension(End,axis) )
	    {
	      kpv[axis]=kv[axis]-1;
	      assert( kpv[axis] >= g2.dimension(Start,axis) );
	    }
	  }


#define XR(m,n) xra[n][m]
	  int ax;
	  if( numberOfDimensions==2 )
	  {
	    // estimate (dx/dr) on grid 2 by differences (to avoid building xr)
	    for(ax=0; ax<numberOfDimensions; ax++ ) 
	    {
	      xra[0][ax]=(kp1-k1)*(center2(kp1,k2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(0);
	      xra[1][ax]=(kp2-k2)*(center2(k1,kp2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(1);
	    }
	    real det = XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0);
	    if( det!=0. )
	    {
	      det=1./det;
	      dr[0]=(  XR(1,1)*dx[0]-XR(0,1)*dx[1] )*det;
	      dr[1]=( -XR(1,0)*dx[0]+XR(0,0)*dx[1] )*det;
	    }
	    else
	    { // if the jacobian is singular
	      if( debug & 1 )
	      {
		printf("Ogmg:WARNING: non-invertible point and jacobian=0. for estimating location\n");
	      }
	      dr[0]=dr[1]=0.;
	    }
	    dr[2]=0.;
            if( debug & 4 )
  	      printF("Ogmg:buildExtraLevels: non-invertible pt, dr from diff.: dx=(%e,%e) r=(%e,%e) dr=(%e,%e) \n",
		     dx[0],dx[1],r(0,0),r(0,1),dr[0],dr[1]);
	  }
	  else
	  {
	    for(ax=0; ax<numberOfDimensions; ax++ ) 
	    {
	      xra[0][ax]=(kp1-k1)*(center2(kp1,k2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(0);  // opt these *****
	      xra[1][ax]=(kp2-k2)*(center2(k1,kp2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(1);
	      xra[2][ax]=(kp3-k3)*(center2(k1,k2,kp3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(2);
	    }
	    real det =((XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0))*XR(2,2) +
		       (XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1))*XR(2,0) +
		       (XR(0,2)*XR(1,0)-XR(0,0)*XR(1,2))*XR(2,1));
	    if( det!=0. )
	    {
	      det=1./det;
	      dr[0]=( (XR(1,1)*XR(2,2)-XR(2,1)*XR(1,2))*dx[0]+
		      (XR(2,1)*XR(0,2)-XR(0,1)*XR(2,2))*dx[1]+
		      (XR(0,1)*XR(1,2)-XR(1,1)*XR(0,2))*dx[2] )*det;
		      
	      dr[1]=( (XR(1,2)*XR(2,0)-XR(2,2)*XR(1,0))*dx[0]+
		      (XR(2,2)*XR(0,0)-XR(0,2)*XR(2,0))*dx[1]+
		      (XR(0,2)*XR(1,0)-XR(1,2)*XR(0,0))*dx[2] )*det;
		      
	      dr[2]=( (XR(1,0)*XR(2,1)-XR(2,0)*XR(1,1))*dx[0]+
		      (XR(2,0)*XR(0,1)-XR(0,0)*XR(2,1))*dx[1]+
		      (XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1))*dx[2] )*det;

              if( debug & 4 )
              printF(" Ogmg:non-invertible pt: interp=(%i,%i,%i) j=(%i,%i,%i) R(jb=%i,.)=(%8.2e,%8.2e,%8.2e) \n"
                     "       x=(%8.2e,%8.2e,%8.2e) kv=[%i,%i,%i] kpv=[%i,%i,%i]\n"
                     "       dx=(%8.2e,%8.2e,%8.2e) dr=(%8.2e,%8.2e,%8.2e) gridSpacing=(%8.2e,%8.2e,%8.2e)\n"
                     " xr(.,0)=(%8.2e,%8.2e,%8.2e) \n"
                     " xr(.,1)=(%8.2e,%8.2e,%8.2e)\n"
                     " xr(.,2)=(%8.2e,%8.2e,%8.2e) \n",
		     i1,i2,i3,j1,j2,j3,jb,R(jb,0),R(jb,1),R(jb,2),x(jb,0),x(jb,1),x(jb,2),
                     k1,k2,k3,kp1,kp2,kp3,
                     dx[0],dx[1],dx[2],dr[0],dr[1],dr[2],
                     g2.gridSpacing(0),g2.gridSpacing(1),g2.gridSpacing(2),
                     XR(0,0),XR(1,0),XR(2,0),XR(0,1),XR(1,1),XR(2,1),XR(0,2),XR(1,2),XR(2,2));  
	      
              for(ax=0; ax<numberOfDimensions; ax++ )
	      {
                dr[ax]=min(g2.gridSpacing(ax),max(-g2.gridSpacing(ax),dr[ax]));
	      }
	      
	    }
	    else
	    { // if the jacobian is singular
	      if( debug & 1 )
		printf("Ogmg:WARNING: non-invertible point and jacobian=0. for estimating location\n"
                       " grid=%i, grid2=%i : kv=[%i,%i,%i] kpv=[%i,%i,%i] \n"
                       " center2(kv) = (%8.2e,%8.2e,%8.2e) center2(kpv)=(%8.2e,%8.2e,%8.2e) \n"
                       " xra[.][0]=(%8.2e,%8.2e,%8.2e) \n"
                       " xra[.][1]=(%8.2e,%8.2e,%8.2e)\n"
                       " xra[.][2]=(%8.2e,%8.2e,%8.2e) \n",grid,grid2,
                       k1,k2,k3,kp1,kp2,kp3,center2(k1,k2,k3,0),center2(k1,k2,k3,1),center2(k1,k2,k3,2),
                       center2(kp1,k2,k3,0),center2(k1,kp2,k3,1),center2(k1,k2,kp3,2),
                       XR(0,0),XR(1,0),XR(2,0),XR(0,1),XR(1,1),XR(2,1),XR(0,2),XR(1,2),XR(2,2));
	      dr[0]=dr[1]=dr[2]=0.;
	    }
	  }
	}
#undef XR
		    
	for( axis=0; axis<numberOfDimensions; axis++ )
	{
	  R(jb,axis)+=dr[axis];
	}
      }  // end if point not invertible
    
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	// limit r to [0,1] unless the boundary is interpolation, then limit by .25*Delta r
	// from the last interpolation point.
	RB(i,axis)=max(rMin[axis],min(rMax[axis],R(jb,axis)));
      }

    } // end for jb
  }  // end for grid2


  return 0;
}


int Ogmg::
getInterpolationCoordinates( CompositeGrid & cg0, // finer grid
                             CompositeGrid & cg1, // new coarser grid
                             int i,               // check this point
			     int grid,
                             int iv[],
                             int jv[],
                             realSerialArray & r,
                             bool isRectangular,
                             int iv0[3], real dx0[3], real xab0[2][3], int iv1[3], real dx1[3], real xab1[2][3] )
// ==================================================================================================
// /Description: *** old version where we did one point at a time ****
//     Given a new interpolation point on a coarse grid, determine the r coordinates on the 
//   interpolee grid. 
// ==================================================================================================
{
  const int numberOfDimensions=cg0.numberOfDimensions();
  int &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int &j1=jv[0], &j2=jv[1], &j3=jv[2];      
  int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];      
  int kpv[3], &kp1=kpv[0], &kp2=kpv[1], &kp3=kpv[2];      

  const intArray & interpoleeGrid0 =cg0.interpoleeGrid[grid];  // ************ should use cg0 ****

  const realArray & interpolationCoordinates0 =cg0.interpolationCoordinates[grid];
  const real *interpolationCoordinates0p = interpolationCoordinates0.Array_Descriptor.Array_View_Pointer1;
  const int interpolationCoordinates0Dim0=interpolationCoordinates0.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES0(i0,i1) interpolationCoordinates0p[i0+interpolationCoordinates0Dim0*(i1)]

  MappedGrid & mg0 = cg0[grid];
//intArray & mask0 = mg0.mask();
  const realArray & center0 = mg0.center();

  MappedGrid & mg1 = cg1[grid];
//MappedGrid & c = mg1;
//intArray & mask1 = mg1.mask();
  const realArray & center = mg1.center();
  real *centerp;
  int centerDim0,centerDim1,centerDim2;
  if( !isRectangular )
  {
    centerp = center.Array_Descriptor.Array_View_Pointer3;
    centerDim0=center.getRawDataSize(0);
    centerDim1=center.getRawDataSize(1);
    centerDim2=center.getRawDataSize(2);
  }
      
  realSerialArray x(1,3);
  real *xp = x.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=x.getRawDataSize(0);
  real *rp = r.Array_Descriptor.Array_View_Pointer1;
  const int rDim0=r.getRawDataSize(0);

  int axis;
  if( isRectangular )
  {
    X(0,0)=CENTER10(j1,j2,j3);
    X(0,1)=CENTER11(j1,j2,j3);
    X(0,2)=CENTER12(j1,j2,j3);
  }
  else
  {
    for( axis=0; axis<numberOfDimensions; axis++ )
      X(0,axis)=CENTER(j1,j2,j3,axis);
  }

  r(0,2)=0.;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    R(0,axis)=INTERPOLATIONCOORDINATES0(i,axis);	
  }
  int grid2=interpoleeGrid0(i);
  assert( grid2>=0 && grid2<cg1.numberOfComponentGrids());
  MappedGrid & g2 = cg1[grid2];
		
  // invert mapping to get coordinates of the new interpolation pt.
  #ifdef USE_PPP
    g2.mapping().getMapping().inverseMapS(x,r);
  #else
    g2.mapping().getMapping().inverseMap(x,r);
  #endif
  // assert( max(fabs(r)) <5. );

  if( max(fabs(R(0,0)),fabs(R(0,1)),fabs(R(0,2))) > 5. )
  {
    // pt was not invertible -- estimate it's inverse location
    for( axis=0; axis<numberOfDimensions; axis++ )
      R(0,axis)=INTERPOLATIONCOORDINATES0(i,axis);	

    // correct inverseCoordinates
    // dx = distance from nearby interpolation pt to the new interpolation point.
    // add dr = (dr/dx)_2 * dx 
    real dx[3], dr[3];
    if( isRectangular )
    {
      dx[0]=CENTER10(j1,j2,j3)-CENTER00(i1,i2,i3);
      dx[1]=CENTER11(j1,j2,j3)-CENTER01(i1,i2,i3);
      dx[2]=CENTER12(j1,j2,j3)-CENTER02(i1,i2,i3);

    }
    else
    {
      for( axis=0; axis<numberOfDimensions; axis++ )
	dx[axis]=CENTER(j1,j2,j3,axis)-center0(i1,i2,i3,axis);
    }
    if( g2.isRectangular() )
    {
      real dx2[3];
      g2.getDeltaX(dx2);
      
      dr[0]=dx[0]/dx2[0]*g2.gridSpacing(0);
      dr[1]=dx[1]/dx2[1]*g2.gridSpacing(1);
      dr[2]=dx[2]/dx2[2]*g2.gridSpacing(2);
    }    
    else
    {
      // kv=(k1,k2,k3) = closest point on grid2 (use dr/dx from near this point)
                 
      const RealMappedGridFunction & center2 = g2.center();
      k3=g2.dimension(Start,axis3);
//       for( axis=0; axis<numberOfDimensions; axis++ )
// 	dx[axis]=center(j1,j2,j3,axis)-center0(i1,i2,i3,axis);
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	// dx[axis]=center(j1,j2,j3,axis)-center0(i1,i2,i3,axis);
	kv[axis]=int( R(0,axis)/g2.gridSpacing(axis) + g2.indexRange(0,axis) );
	kpv[axis]=kv[axis]+1;
	if( kpv[axis] > g2.dimension(End,axis) )
	{
	  kpv[axis]=kv[axis]-1;
	  assert( kpv[axis] >= g2.dimension(Start,axis) );
	}
      }

#define XR(m,n) xra[n][m]
      int ax;
      real xra[3][3];
      if( numberOfDimensions==2 )
      {
	// estimate (dx/dr) on grid 2 by differences (to avoid building xr)
	for(ax=0; ax<numberOfDimensions; ax++ ) 
	{
	  xra[0][ax]=(center2(kp1,k2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(0);
	  xra[1][ax]=(center2(k1,kp2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(1);
	}
	real det = XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0);
	if( det!=0. )
	{
	  det=1./det;
	  dr[0]=(  XR(1,1)*dx[0]-XR(0,1)*dx[1] )*det;
	  dr[1]=( -XR(1,0)*dx[0]+XR(0,0)*dx[1] )*det;
	}
	else
	{ // if the jacobian is singular
	  if( debug & 1 )
	  {
	    printF("Ogmg:WARNING: non-invertible point and jacobian=0. for estimating location\n");
	  }
	  dr[0]=dr[1]=0.;
	}
	dr[2]=0.;
        if( debug & 4 )
	printf("Ogmg:buildExtraLevels: non-invertible pt, dr from diff.: dx=(%e,%e) r=(%e,%e) dr=(%e,%e) \n",
	       dx[0],dx[1],r(0,0),r(0,1),dr[0],dr[1]);
      }
      else
      {
	for(ax=0; ax<numberOfDimensions; ax++ ) 
	{
	  xra[0][ax]=(center2(kp1,k2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(0);
	  xra[1][ax]=(center2(k1,kp2,k3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(1);
	  xra[2][ax]=(center2(k1,k2,kp3,ax)-center2(k1,k2,k3,ax))/g2.gridSpacing(2);
	}
	real det =((XR(0,0)*XR(1,1)-XR(0,1)*XR(1,0))*XR(2,2) +
		   (XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1))*XR(2,0) +
		   (XR(0,2)*XR(1,0)-XR(0,0)*XR(1,2))*XR(2,1));
	if( det!=0. )
	{
	  det=1./det;
	  dr[0]=( (XR(1,1)*XR(2,2)-XR(2,1)*XR(1,2))*dx[0]+
		  (XR(2,1)*XR(0,2)-XR(0,1)*XR(2,2))*dx[1]+
		  (XR(0,1)*XR(1,2)-XR(1,1)*XR(0,2))*dx[2] )*det;
		      
	  dr[1]=( (XR(1,2)*XR(2,0)-XR(2,2)*XR(1,0))*dx[0]+
		  (XR(2,2)*XR(0,0)-XR(0,2)*XR(2,0))*dx[1]+
		  (XR(0,2)*XR(1,0)-XR(1,2)*XR(0,0))*dx[2] )*det;
		      
	  dr[2]=( (XR(1,0)*XR(2,1)-XR(2,0)*XR(1,1))*dx[0]+
		  (XR(2,0)*XR(0,1)-XR(0,0)*XR(2,1))*dx[1]+
		  (XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1))*dx[2] )*det;
	}
	else
	{ // if the jacobian is singular
	  if( debug & 1 )
	    printF("Ogmg:WARNING: non-invertible point and jacobian=0. for estimating location\n");
	  dr[0]=dr[1]=dr[2]=0.;
	}
      }
    }
#undef XR
		    
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      R(0,axis)+=dr[axis];
    }
  }

  const IntegerArray & extended = g2.extendedIndexRange(); 
  const real offset = parameters.allowExtrapolationOfInterpolationPoints ? 1. : 0.; // *wdh* 040903
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    // normally limit r to [0,1] unless the boundary is interpolation, then limit by .25*Delta r
    // from the last interpolation point.
    const real dr = g2.gridSpacing(axis);
    real rMin = g2.boundaryCondition(Start,axis)!=0  ? -offset*dr :
                                      (extended(Start,axis)+.25)*dr; // move in .25*Dr from the end
    real rMax = g2.boundaryCondition(End  ,axis)!=0  ? 1.+offset*dr :
                                      (extended(End,  axis)-.25)*dr;
		      
    R(0,axis)=max(rMin,min(rMax,R(0,axis)));
  }
  return 0;
}



Ogmg::InterpolationQualityEnum Ogmg::
getInterpolationStencil(CompositeGrid & cg0,
                        CompositeGrid & cg1,
                        int i,
                        int iv[3],
                        int grid,
			int l,
                        intSerialArray & inverseGrid,
                        intSerialArray & interpoleeGrid,
                        intSerialArray & interpoleeLocation,
                        intSerialArray & interpolationPoint,
                        intSerialArray & variableInterpolationWidth,
                        realSerialArray & interpolationCoordinates,
                        realSerialArray & inverseCoordinates )

{
// =============================================================================
//   /Description:
//      Determine an interpolation stencil that can be used for interpolation point i.
//  The stencil width will be reduced to try to obtain a valid set of points to interpolate
// from. The smallest width is 1. A non-zero return value indicates that no stencil could be found.
// /i (input) : interpolation point number.
// /iv (input) : interpolation point
// /grid (input) : interpolation pt is on this grid.
// /l (input) : multigrid level we are working on.
// /inverseGrid (input):
// /interpoleeLocation, variableInterpolationWidth (output):
// /Return value:
//   One of the InterpolationQualityEnum values.
// =============================================================================
  
  int debugi=0; // 7;
  
  CompositeGrid & mgcg = multigridCompositeGrid();
  InterpolationQualityEnum status=canNotInterpolate;
  
  const int numberOfDimensions=cg0.numberOfDimensions();
  int &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];      
  int kv[3], &k1=kv[0], &k2=kv[1], &k3=kv[2];      


  interpoleeGrid(i)=inverseGrid(i1,i2,i3);
  int grid2=interpoleeGrid(i);
  assert( grid2>=0 && grid2<cg1.numberOfComponentGrids() );

  Range Rx=numberOfDimensions;
  const IntegerArray & interpolationWidth = mgcg.interpolationWidth(Rx,grid,grid2,l); // *** fix this use variableWidth **

  MappedGrid & g2 = cg1[grid2];
  const int * indexRangep = g2.indexRange().Array_Descriptor.Array_View_Pointer1;
  const int indexRangeDim0=g2.indexRange().getRawDataSize(0);
#define indexRange(i0,i1) indexRangep[i0+indexRangeDim0*(i1)]
  const int * extendedIndexRangep = g2.extendedIndexRange().Array_Descriptor.Array_View_Pointer1;
  const int extendedIndexRangeDim0=g2.extendedIndexRange().getRawDataSize(0);
#define extendedIndexRange(i0,i1) extendedIndexRangep[i0+extendedIndexRangeDim0*(i1)]
  const int * boundaryConditionp = g2.boundaryCondition().Array_Descriptor.Array_View_Pointer1;
  const int boundaryConditionDim0=g2.boundaryCondition().getRawDataSize(0);
#define boundaryCondition(i0,i1) boundaryConditionp[i0+boundaryConditionDim0*(i1)]

  const real * gridSpacingp = g2.gridSpacing().Array_Descriptor.Array_View_Pointer0;
#define gridSpacing(i0) gridSpacingp[i0]

  int axis;
  j3=indexRange(Start,axis3);

  real rI[3]={0.,0.,0.};
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    rI[axis]=inverseCoordinates(i1,i2,i3,axis);

    interpolationPoint(i,axis)=iv[axis];
    interpolationCoordinates(i,axis)=rI[axis];
    // closest point on grid2:
    jv[axis]=int( rI[axis]/gridSpacing(axis) + indexRange(0,axis) );

  }
  k3=j3;
  int width=interpolationWidth(axis1); // note

//  int width=min(3,interpolationWidth(axis1)); 

  #ifdef USE_PPP
    intSerialArray mask2; getLocalArrayWithGhostBoundaries(g2.mask(),mask2);
  #else
    const intArray & mask2 = g2.mask();
  #endif
  const int * mask2p = mask2.Array_Descriptor.Array_View_Pointer2;
  const int mask2Dim0=mask2.getRawDataSize(0);
  const int mask2Dim1=mask2.getRawDataSize(1);
#define MASK2(i0,i1,i2) mask2p[i0+mask2Dim0*(i1+mask2Dim1*(i2))]	

  // check for a valid interpolation stencil. If the stencil is invalid, reduce the interpolation
  // width and try again.
  bool notDone=TRUE;
  while( notDone && width>0 )
  {
    bool extrapolate=false;
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      // Get the lower-left corner of the interpolation cube.
      int intLoc=int(floor(rI[axis]/gridSpacing(axis) + indexRange(0,axis) -
			   .5 * width + (g2.isCellCentered(axis) ? .5 : 1.)));
      if (!g2.isPeriodic(axis)) 
      {
	//  Check for points close to a BC side.  One-sided interpolation used.
	if( intLoc < extendedIndexRange(0,axis) )
	{
  	  intLoc = extendedIndexRange(0,axis);
          if( boundaryCondition(Start,axis)>0 && rI[axis]>=-.1*gridSpacing(axis) )
	  {
	  }
	  else
	  {
	    extrapolate=true;
	  }
        }
      
	if( intLoc + width- 1 > extendedIndexRange(1,axis) ) 
	{
	  intLoc = extendedIndexRange(1,axis) - width + 1;
          if( boundaryCondition(End,axis)>0 && rI[axis]<=1.+.1*gridSpacing(axis) )
	  {
	  }
	  else
	  {
            extrapolate=true;
	  }
	}
      } 
      kv[axis]=intLoc;

    } 
        
#define MASK2D(k1,k2,k3) (MASK2(k1,k2,k3)>0 ? 1 : MASK2(k1,k2,k3)==0 ? 0 : -1) 
    if( debugi & 4 )
      fprintf(pDebugFile,"getInterp: l=%i grid=%i i=%i grid2=%i i=(%i,%i,%i) interpolee=(%i,%i) width=%i r=(%4.2f,%4.2f,%4.2f) : \n",
                l,grid,i,grid2,i1,i2,i3,k1,k2, width, rI[0],rI[1],rI[2] );
    if( width==3 )
    {
      if( numberOfDimensions==2 )
      {
	if( MASK2(k1  ,k2  ,k3)!=0 && MASK2(k1+1,k2  ,k3)!=0 && MASK2(k1+2,k2  ,k3)!=0 && 
	    MASK2(k1  ,k2+1,k3)!=0 && MASK2(k1+1,k2+1,k3)!=0 && MASK2(k1+2,k2+1,k3)!=0 && 
	    MASK2(k1  ,k2+2,k3)!=0 && MASK2(k1+1,k2+2,k3)!=0 && MASK2(k1+2,k2+2,k3)!=0 )
	{
	  notDone=FALSE;
          status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality1;
	}
	if( debugi & 4 )
	  fprintf(pDebugFile,"mask =[%i,%i,%i]x[%i,%i,%i]x[%i,%i,%i]\n",
		  MASK2D(k1,k2  ,k3),MASK2D(k1+1,k2  ,k3),MASK2D(k1+2,k2  ,k3),
		  MASK2D(k1,k2+1,k3),MASK2D(k1+1,k2+1,k3),MASK2D(k1+2,k2+1,k3),
		  MASK2D(k1,k2+2,k3),MASK2D(k1+1,k2+2,k3),MASK2D(k1+2,k2+2,k3));
      }
      else
      {
	if( MASK2(k1  ,k2  ,k3  )!=0 && MASK2(k1+1,k2  ,k3  )!=0 && MASK2(k1+2,k2  ,k3  )!=0 && 
	    MASK2(k1  ,k2+1,k3  )!=0 && MASK2(k1+1,k2+1,k3  )!=0 && MASK2(k1+2,k2+1,k3  )!=0 && 
	    MASK2(k1  ,k2+2,k3  )!=0 && MASK2(k1+1,k2+2,k3  )!=0 && MASK2(k1+2,k2+2,k3  )!=0 &&
	    MASK2(k1  ,k2  ,k3+1)!=0 && MASK2(k1+1,k2  ,k3+1)!=0 && MASK2(k1+2,k2  ,k3+1)!=0 && 
	    MASK2(k1  ,k2+1,k3+1)!=0 && MASK2(k1+1,k2+1,k3+1)!=0 && MASK2(k1+2,k2+1,k3+1)!=0 && 
	    MASK2(k1  ,k2+2,k3+1)!=0 && MASK2(k1+1,k2+2,k3+1)!=0 && MASK2(k1+2,k2+2,k3+1)!=0 &&
	    MASK2(k1  ,k2  ,k3+2)!=0 && MASK2(k1+1,k2  ,k3+2)!=0 && MASK2(k1+2,k2  ,k3+2)!=0 && 
	    MASK2(k1  ,k2+1,k3+2)!=0 && MASK2(k1+1,k2+1,k3+2)!=0 && MASK2(k1+2,k2+1,k3+2)!=0 && 
	    MASK2(k1  ,k2+2,k3+2)!=0 && MASK2(k1+1,k2+2,k3+2)!=0 && MASK2(k1+2,k2+2,k3+2)!=0   
                                                                                       )
	{
	  notDone=FALSE;
          status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality1;
	}
      }
      
    }
    else if( width==2 )
    {
      if( numberOfDimensions==2 )
      {
	if( MASK2(k1  ,k2  ,k3)!=0 && MASK2(k1+1,k2  ,k3)!=0 && 
	    MASK2(k1  ,k2+1,k3)!=0 && MASK2(k1+1,k2+1,k3)!=0 )
	{
	  status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality2;
	  notDone=FALSE;
	}
	if( debugi & 4 )
	  fprintf(pDebugFile,"mask =[%i,%i]x[%i,%i]\n",
		  MASK2D(k1,k2  ,k3),MASK2D(k1+1,k2  ,k3),
		  MASK2D(k1,k2+1,k3),MASK2D(k1+1,k2+1,k3));
      }
      else
      {
	if( MASK2(k1  ,k2  ,k3  )!=0 && MASK2(k1+1,k2  ,k3  )!=0 && 
	    MASK2(k1  ,k2+1,k3  )!=0 && MASK2(k1+1,k2+1,k3  )!=0 && 
	    MASK2(k1  ,k2  ,k3+1)!=0 && MASK2(k1+1,k2  ,k3+1)!=0 && 
	    MASK2(k1  ,k2+1,k3+1)!=0 && MASK2(k1+1,k2+1,k3+1)!=0 )
	{
	  status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality2;
	  notDone=FALSE;
	}
      }
      
    }
    else if( width==1 )
    {
      if( debug & 4 )
	fprintf(pDebugFile, "mask =[%i]\n",MASK2D(k1,k2,k3));

      if( MASK2(k1  ,k2  ,k3)!=0 )
      {
	if( MASK2(k1  ,k2  ,k3)>0 )
	{
	  if( !extrapolate )
	    status=canInterpolateQuality3;
	  else
	    status=canInterpolateWithExtrapolation;

	  printF("INFO: one point interpolation, pt=(%i,(from a discretization pt\n");
  	  printF("    : l=%i, grid=%i i=%i : grid2=%i interp:i=(%i,%i,%i) interpolee:kv=(%i,%i,%i)\n",
		    l,grid,i,grid2,i1,i2,i3,k1,k2,k3,MASK2D(k1,k2,k3));
	  if( Ogmg::debug & 2 )
	  {
	    fprintf(pDebugFile,"INFO: one point interpolation, pt=(%i,(from a discretization pt\n",i);
	    fprintf(pDebugFile,"    : l=%i, grid=%i i=%i : grid2=%i interp:i=(%i,%i,%i) interpolee:kv=(%i,%i,%i)\n"
		    "    : mask2 = %i %i %i %i\n",
		    l,grid,i,grid2,i1,i2,i3,k1,k2,k3,MASK2D(k1,k2,k3),MASK2D(k1+1,k2,k3),MASK2D(k1,k2+1,k3),
                     MASK2D(k1+1,k2+1,k3));
	  }
	  
	}
	else
	{
          status=canInterpolateQualityBad;

          if( Ogmg::debug & 4 )
  	    fprintf(pDebugFile,"Ogmg:WARNING: l=%i, grid=%i i=%i : grid2=%i i=(%i,%i,%i) kv=(%i,%i) "
		    "one pt interp. from interp pt, mask =[%i]\n",
		    l,grid,i,grid2,i1,i2,i3,k1,k2,MASK2D(k1,k2,k3));

	}
	notDone=FALSE;
      }
      else
      {
	// closest point could not be used for interpolation,
	// find a nearby point.                      **** do better here ****
        if( Ogmg::debug & 4 )
	{
	  fprintf(pDebugFile,"  ... unable to interpolate from nearest point! Now look for a valid neigbour\n");
	  printF("  ... unable to interpolate from nearest point! Now look for a valid neigbour\n");
	}
	
	const int m3Start=numberOfDimensions<3 ? 0 : -1;
	const int m3End  =numberOfDimensions<3 ? 0 : +1;
	for( int m3=m3Start; m3<=m3End && notDone; m3++ )
	{
	  for( int m2=-1; m2<=1 && notDone ; m2++ )
	  {
	    for( int m1=-1; m1<=1 && notDone ; m1++ )
	    {
	      if( MASK2(k1+m1,k2+m2,k3+m3)!=0 )
	      {
		k1+=m1;
		k2+=m2;
		k3+=m3;

                fprintf(pDebugFile,"  ... can interpolate badly from a nearby point. Try to fix..\n");
                status=canInterpolateQualityBad;
		notDone=FALSE;
		break;
	      }
	    }
	  }
	}
	if( notDone )
	{
	  if( Ogmg::debug & 4 )
	  {
	    fprintf(pDebugFile,"  ... unable to interpolate from nearest point and there is no valid neighbour\n");
	    printF("  ... unable to interpolate from nearest point and there is no valid neighbour\n");
	  }
	  
          status=canNotInterpolate;

	  return status;
	  
	}
      }
    }
    else if( width==5 )
    {
#define MASK2_W5A(k1,k2,k3)\
  MASK2(k1,k2,k3)!=0 && MASK2(k1+1,k2,k3)!=0 && MASK2(k1+2,k2,k3)!=0 && MASK2(k1+3,k2,k3)!=0 && MASK2(k1+4,k2,k3)!=0

#define MASK2_W5B(k1,k2,k3)\
  MASK2_W5A(k1,k2,k3) && MASK2_W5A(k1,k2+1,k3) && MASK2_W5A(k1,k2+2,k3) && \
  MASK2_W5A(k1,k2+3,k3) && MASK2_W5A(k1,k2+4,k3)  

#define MASK2_W5C(k1,k2,k3)\
  MASK2_W5B(k1,k2,k3) && MASK2_W5B(k1,k2,k3+1) && MASK2_W5B(k1,k2,k3+2) && \
  MASK2_W5B(k1,k2,k3+3) && MASK2_W5B(k1,k2,k3+4)  

      if( numberOfDimensions==2 )
      {
	if( MASK2_W5B(k1,k2,k3) )
	{
	  notDone=FALSE;
          status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality1;
	}
	if( debug & 4 )
	  fprintf(pDebugFile,"mask =[%i,%i,%i]x[%i,%i,%i]x[%i,%i,%i]\n",
		  MASK2D(k1,k2  ,k3),MASK2D(k1+1,k2  ,k3),MASK2D(k1+2,k2  ,k3),
		  MASK2D(k1,k2+1,k3),MASK2D(k1+1,k2+1,k3),MASK2D(k1+2,k2+1,k3),
		  MASK2D(k1,k2+2,k3),MASK2D(k1+1,k2+2,k3),MASK2D(k1+2,k2+2,k3));
      }
      else
      {
	if( MASK2_W5C(k1,k2,k3) )
	{
	  notDone=FALSE;
          status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality1;
	}
      }
      
    }
    else if( width==4 )
    {
#define MASK2_W4A(k1,k2,k3)\
  MASK2(k1,k2,k3)!=0 && MASK2(k1+1,k2,k3)!=0 && MASK2(k1+2,k2,k3)!=0 && MASK2(k1+3,k2,k3)!=0

#define MASK2_W4B(k1,k2,k3)\
  MASK2_W4A(k1,k2,k3) && MASK2_W4A(k1,k2+1,k3) && MASK2_W4A(k1,k2+2,k3) && MASK2_W4A(k1,k2+3,k3) 

#define MASK2_W4C(k1,k2,k3)\
  MASK2_W4B(k1,k2,k3) && MASK2_W4B(k1,k2,k3+1) && MASK2_W4B(k1,k2,k3+2) && MASK2_W4B(k1,k2,k3+3) 

      if( numberOfDimensions==2 )
      {
	if( MASK2_W4B(k1,k2,k3) )
	{
	  notDone=FALSE;
          status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality1;
	}
	if( debug & 4 )
	  fprintf(pDebugFile,"mask =[%i,%i,%i]x[%i,%i,%i]x[%i,%i,%i]\n",
		  MASK2D(k1,k2  ,k3),MASK2D(k1+1,k2  ,k3),MASK2D(k1+2,k2  ,k3),
		  MASK2D(k1,k2+1,k3),MASK2D(k1+1,k2+1,k3),MASK2D(k1+2,k2+1,k3),
		  MASK2D(k1,k2+2,k3),MASK2D(k1+1,k2+2,k3),MASK2D(k1+2,k2+2,k3));
      }
      else
      {
	if( MASK2_W4C(k1,k2,k3) )
	{
	  notDone=FALSE;
          status=extrapolate ? canInterpolateWithExtrapolation : canInterpolateQuality1;
	}
      }
      
    }
    else
    {
      printF("getInterpolationStencil:ERROR: unexpected width of interpolation = %i\n",width);
      Overture::abort();
    }
    
    width--;

  }  // end while notDone and width>0
  

  variableInterpolationWidth(i)=width+1;
  real rDist=0.;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    real dr=interpolationCoordinates(i,axis)/gridSpacing(axis) + indexRange(0,axis) -jv[axis];
    rDist+=SQR(dr);
	  
    interpoleeLocation(i,axis) = kv[axis];
    // interpolationCoordinates(i,axis)=inverseCoordinates(i1,i2,i3,axis);
  }
  rDist=sqrt(rDist);
  if( (MASK2(j1,j2,j3) & MappedGrid::ISinterpolationPoint) && rDist < .05 )
  {
    if( Ogmg::debug & 4 )
      fprintf(pDebugFile,"buildExtraLevels:INFO: interpolation stencil is too implicit: rDist=%6.2e\n",rDist);

    status=canNotInterpolate;
  }

  return status;
}

