#include "Ogmg.h"
#include "SparseRep.h"
#include "ParallelUtility.h"


#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( int i3=I3Base; i3<=I3Bound; i3++ )  \
  for( int i2=I2Base; i2<=I2Bound; i2++ )  \
  for( int i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase(),\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( int i3=I3Base; i3<=I3Bound; i3++ )  \
  for( int i2=I2Base; i2<=I2Bound; i2++ )  \
  for( int i1=I1Base; i1<=I1Bound; i1++ )

int Ogmg::
operatorAveraging(RealCompositeGridFunction & coeff, const int & level)
// =====================================================================================
// /Description:
//   Generate a coarse grid operator for level+1 by averaging a fine grid operator.
// =====================================================================================
{
  real time=getCPU();
  if( debug & 4 )
    printF("%*.1s Ogmg::operatorAveraging:level = %i \n",level*2,"  ",level);


  CompositeGrid & mgcg = multigridCompositeGrid();
  CompositeGrid & cg = *coeff.getCompositeGrid();
  for( int grid=0; grid<cg.multigridLevel[level].numberOfComponentGrids(); grid++ )
  {
    operatorAveraging(coeff.multigridLevel[level][grid],coeff.multigridLevel[level+1][grid],
		      cg.multigridCoarseningRatio(Range(0,2),grid,level+1),grid,level);
  }
  
  if( equationToSolve==OgesParameters::divScalarGradOperator ||
      equationToSolve==OgesParameters::variableHeatEquationOperator ||
      equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
  {
    // -- predefined variable coefficients: 
    //    compute averaged coefficients for level+1 
    buildPredefinedVariableCoefficients( coeff, level ); 
  }
  
  if( (equationToSolve!=OgesParameters::userDefined && 
       (level+1)==mgcg.numberOfMultigridLevels()-1 && 
       parameters.useDirectSolverOnCoarseGrid)     )
  {
    // build coarse grid equations on rectangular grids (curvilinear grids were already done by averaging)
    bool buildRectangular=true;
    bool buildCurvilinear=false;
    buildPredefinedCoefficientMatrix( level+1,buildRectangular,buildCurvilinear );
  }

  tm[timeForOperatorAveraging]+=getCPU()-time;
  return 0;
}

// Use this for indexing into coefficient matrices representing systems of equations
#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))
#define ForStencil(m1,m2,m3)   \
	for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
	for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
	for( m1=-halfWidth1; m1<=halfWidth1; m1++) 


#define averageOpt EXTERN_C_NAME(averageopt)

extern "C"
{
  void averageOpt( const int& nd, const int & nd1a,const int & nd1b,const int & nd2a,const int & nd2b,
                   const int & nd3a,const int & nd3b,
                   const int& md1a,const int& md1b,const int& md2a,const int& md2b,const int& md3a,const int& md3b, 
		   const int& j1a,const int& j1b,const int& j2a,const int& j2b,const int& j3a,const int& j3b, 
                   const int& i1a,const int& i1b,const int& i1c,const int& i2a,const int& i2b,const int& i2c,
                   const int& i3a,const int& i3b,const int& i3c, 
		   const int& i1pa,const int& i1pb,const int& i2pa,const int& i2pb,const int& i3pa,const int& i3pb,
                   const int& ndc, const real & cFine, real & cCoarse,
		   const int& ndc0, real & c0, real & c1, const int& option, const int& orderOfAccuracy,
                   const int &ipar );
}

//\begin{>>OgmgInclude.tex}{\subsection{operatorAveraging}}
int Ogmg::
operatorAveraging(RealMappedGridFunction & coeffFine,
                  RealMappedGridFunction & coeffCoarse,
                  const IntegerArray & coarseningRatio,
                  int grid /* =0 */,
                  int level /* =0 */ )
// =====================================================================================
// /Description:
//   Generate a coarse grid operator by averaging a fine grid operator.
//\end{OgmgInclude.tex} 
// =====================================================================================
{
  CompositeGrid & mgcg  = multigridCompositeGrid();
  MappedGrid & mgFine   = *coeffFine.getMappedGrid();
  MappedGrid & mgCoarse = *coeffCoarse.getMappedGrid();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int ipar[5]={0,0,0,0,0};

  const bool isRectangular = mgCoarse.isRectangular();
  
  if( equationToSolve!=OgesParameters::userDefined && isRectangular )
  {
    // **** Predefined Equation and a rectangular Grid *****
     
    if( (level+1)==mgcg.numberOfMultigridLevels()-1 && parameters.useDirectSolverOnCoarseGrid )
    {
      // the equations on the coarse level are built in the routine above
    }
    else
    {


      if( Ogmg::debug & 4 ) 
        printF("**** operatorAveraging: skip building coeffCoarse on a rectangular grid level=%i *****\n",level+1); 
    }
    
    return 0;
  }
  if( equationToSolve!=OgesParameters::userDefined && 
      (parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations ||
       parameters.averagingOption==OgmgParameters::doNotAverageCoarseCurvilinearGridEquations) )
  {
    if( Ogmg::debug & 2 )
      printF("\n**** operatorAveraging: do NOT average coeff's for grid=%i, level=%i *****\n",grid,level+1); 

    bool buildRectangular=false;
    bool buildCurvilinear=true;
    buildPredefinedCoefficientMatrix( level+1,buildRectangular,buildCurvilinear );

    return 0;
    
  }

  
  realArray & cCoarse      = coeffCoarse;
  const int & numberOfDimensions = mgFine.numberOfDimensions();

  // ************** try this : 091217
  // coeffFine.updateGhostBoundaries();   // ************** The input coeff's should have done this !

  RealMappedGridFunction localCoeffFine;
  localCoeffFine.reference(coeffFine);

  Range Rx=numberOfDimensions;
  if( max(mgFine.isPeriodic()(Rx)-Mapping::notPeriodic)!=0 )
  {
    // in the periodic case we need to replace the periodic BC's with the interior equation
    // so that we can average these values

    if( Ogmg::debug & 4 )
      printF("*** operatorAveraging: grid is periodic, need to make a copy of the coeff's***\n");
    
    // display(localCoeffFine,"localCoeffFine before periodic update");

    if( false ) // *wdh* 011107 *** do not make a copy -- we assume the user doesn't mind if we alter the coeff matrix
      localCoeffFine.breakReference(); // make a separate copy.

    localCoeffFine.periodicUpdate();

    // display(localCoeffFine,"localCoeffFine after periodic update");
  }
  
  realArray & cFine = localCoeffFine;
  

//  int cf1,cf2,cf3,cf[3];
  int cf[3];
  cf[0]=coarseningRatio(axis1);  // coarsening factor
  cf[1]=coarseningRatio(axis2);
  cf[2]=coarseningRatio(axis3);  

  assert(cf[0]==2 && (cf[1]==2 || numberOfDimensions<2) && (cf[2]==2 || numberOfDimensions<3));

  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Ipv[3], &I1p=Ipv[0], &I2p=Ipv[1], &I3p=Ipv[2];
  Index Iav[3], &I1a=Iav[0], &I2a=Iav[1], &I3a=Iav[2];

  Index Ivc[3], &I1c=Ivc[0], &I2c=Ivc[1], &I3c=Ivc[2];
  Index Jvc[3], &J1c=Jvc[0], &J2c=Jvc[1], &J3c=Jvc[2];

  // We need coefficients on the ghost points outside interpolation boundaries since
  // these will be used in the average for the equation on the interp boundary
  // Depending on the number of ghost lines we may not have computed this value for level==0.
  #ifdef USE_PPP
    realSerialArray cFineLocal;   getLocalArrayWithGhostBoundaries(cFine,cFineLocal);
    const intArray & maskFine = mgFine.mask();
    intSerialArray maskFineLocal;   getLocalArrayWithGhostBoundaries(maskFine,maskFineLocal);
    // realSerialArray cCoarseLocal; getLocalArrayWithGhostBoundaries(cCoarse,cCoarseLocal);
  #else
    realSerialArray & cFineLocal = cFine;
    const intArray & maskFine = mgFine.mask();
    const intSerialArray & maskFineLocal =maskFine; 
    // realSerialArray & cCoarseLocal = cCoarse;
  #endif


  #ifdef USE_PPP
    // In parallel we build a local coarse grid coeff array that matches the local fine grid coeff
    // (since cFine and cCoarse may not have matching distributions)
    // At the end cCoarseLocal will be copied into cCoarse
    realSerialArray cCoarseLocal;

    const IntegerArray & gidf = mgFine.gridIndexRange();
    const IntegerArray & gidc = mgCoarse.gridIndexRange();
    IntegerArray dimLocalCoarse(2,3); dimLocalCoarse=0;
    const int numGhost=orderOfAccuracy/2; // is this right ? 
    bool thisProcessorHasPoints=true;
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      // --- side=0 ---
      if( maskFineLocal.getLength(axis)==0 )
      {
	// no points on this processor
	thisProcessorHasPoints=false;
	for( int side=0; side<=1; side++ )
	{
	  dimLocalCoarse(side,axis) = -side;
	}
	continue;
      }      
      if( maskFine.getLocalBase(axis) == maskFine.getBase(axis) ) 
      {
	dimLocalCoarse(0,axis) = gidc(0,axis)-numGhost;
      }
      else
      { // this side is an internal parallel boundary
	// choose end-pt to match the coarse grid pt 
	int ia0 =  maskFine.getLocalBase(axis)+maskFine.getGhostBoundaryWidth(axis); // index-bound, no ghost
	int ja = (ia0-gidf(0,axis))/cf[axis]+gidc(0,axis);   // coarse grid pt <= ia0 
	int ia = (ja -gidc(0,axis))*cf[axis]+gidf(0,axis);                    // fine grid point to match ja
	assert( ia>=maskFine.getLocalBase(axis) );
	  
	dimLocalCoarse(0,axis) = ja; // -hw[axis];   // what should this be ? 
      }
      // --- side=1 ---
      if( maskFine.getLocalBound(axis) == maskFine.getBound(axis) ) 
      {
	dimLocalCoarse(1,axis) = gidc(1,axis)+numGhost; 
      }
      else
      { // this side is an internal parallel boundary
	int ia0 = maskFine.getLocalBound(axis)-maskFine.getGhostBoundaryWidth(axis);  // last pt (no parallel ghost)
	int ja = (ia0+(cf[axis]-1)-gidf(0,axis))/cf[axis]+gidc(0,axis);   // coarse grid pt >= ia0 
	int ia = (ja-gidc(0,axis))*cf[axis]+gidf(0,axis);                 // fine grid point to match ja 
	assert( ia<=maskFine.getLocalBound(axis) );

	dimLocalCoarse(1,axis) = ja; // +hw[axis];  // what should this be ? 
      } 
    }
    getIndex(dimLocalCoarse,J1,J2,J3);
    cCoarseLocal.redim(cCoarse.dimension(0),J1,J2,J3);

  #else
    realSerialArray & cCoarseLocal = cCoarse;
    const IntegerArray & dimLocalCoarse = mgCoarse.dimension();
  #endif


  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  int side,axis;
  for( axis=0; axis<numberOfDimensions; axis++ )
  {
    for( side=Start; side<=End; side++ )
    {
      if( mgFine.boundaryCondition(side,axis)==0 && 
          (level>0 || mgFine.numberOfGhostPoints(side,axis)< orderOfAccuracy/2+1) )
      {
        Range all;
        int extra=1;  // to get corners
        getGhostIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3,1,extra);
        int includeGhost=1;
        bool ok =ParallelUtility::getLocalArrayBounds(maskFine,maskFineLocal,I1,I2,I3,includeGhost);
	if( !ok ) continue;

        is1=is2=is3=0;
	isv[axis]=1-2*side;
        if( debug & 4 )
          printF("*** Extrap fine grid equation on interp boundary (%i,%i) of grid %i level=%i\n",side,axis,grid,level);
	
        cFineLocal(all,I1,I2,I3)=2.*cFineLocal(all,I1+is1,I2+is2,I3+is3)-cFineLocal(all,I1+2*is1,I2+2*is2,I3+2*is3);
      }
    }
  }
  
  // *not used* 030607 getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3);
  // indexRange is OK
  getIndex(mgFine.indexRange(),I1,I2,I3);                    // Index's for fine grid
  getIndex(mgFine.indexRange(),I1a,I2a,I3a);                    // Index's for fine grid, stride 1
  getIndex(mgFine.indexRange(),I1p,I2p,I3p,1);     // one bigger
  
  I1=IndexBB(I1,cf[0]);  I2=IndexBB(I2,cf[1]);  I3=IndexBB(I3,cf[2]);  // set stride
  

  getIndex(mgCoarse.indexRange(),J1,J2,J3);                  // Index's for coarse grid

  cCoarseLocal=0.; // *** fix this *** need to zero out un-assigned points
  
  //  real time=getCPU();

  //  **********************************************************************
  //  *********** average coefficients over the interior *******************
  //  **********************************************************************

  TransferTypesEnum option[3]={fullWeighting,fullWeighting,fullWeighting};

  averageCoefficients(I1,I2,I3,I1p,I2p,I3p,J1,J2,J3,option,cFineLocal,cCoarseLocal,ipar);

  if( Ogmg::debug & 4 || Ogmg::debug & 64 )
  {
    fprintf(pDebugFile,"level=%i, grid=%i : averaged operator cCoarseLocal before average BC\n",level+1,grid);
    // ::display(cFineLocal,"cFineLocal",pDebugFile,"%7.1e ");
    ::display(cCoarseLocal,"cCoarseLocal",pDebugFile,"%7.1e ");
    fflush(pDebugFile);
  }

  //  **********************************************************************
  //  ******** average coefficients on the boundary where needed ***********
  //  **********************************************************************
  // This was moved here for parallel *wdh* 100330

  const int width = orderOfAccuracy+1;  // 3 or 5
  const int stencilSize=int(pow(width,mgcg.numberOfDimensions())+1);

  MappedGridOperators & op = * coeffCoarse.getOperators();
  op.setStencilSize(stencilSize);
  op.setOrderOfAccuracy(orderOfAccuracy);

  // const int orderOfExtrapolation= orderOfAccuracy==2 ? 3 : 4;  // 5 **** use extrap order 4 for 4th order
  const int orderOfExtrapolation= getOrderOfExtrapolation(level);  // 100118 

  BoundaryConditionParameters extrapParams;
  extrapParams.orderOfExtrapolation=orderOfExtrapolation; // orderOfAccuracy+1; 

  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      // *** Check for internal ghost boundary ****
      bool internalGhost = ( (side==0 && maskFine.getLocalBase(axis)  != maskFine.getBase(axis)) ||
			     (side==1 && maskFine.getLocalBound(axis) != maskFine.getBound(axis)) );

      if( mgCoarse.boundaryCondition(side,axis)>0 && !internalGhost )
      {

	if( boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate ) 
	{
          // --- DIRICHLET -- no averaging
	}
	else 
	{
          // -- NEUMANN : average equations on the boundary

	  getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3);
	  getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1p,I2p,I3p,1);  // one extra point on each end
	  getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3);


	  // do not change the equation on the ends if the adjacent boundary is Dirichlet (probably)
	  for( int dir=0; dir<numberOfDimensions-1; dir++ )
  	  {
  	    const int axisp=(axis+dir+1)%numberOfDimensions;  // tangential direction

  	    int shift0=0, shift1=0;
	    if( boundaryCondition(Start,axisp,grid)==OgmgParameters::extrapolate )
	      shift0=1;
	    if( boundaryCondition(End,axisp,grid)==OgmgParameters::extrapolate )
	      shift1=-1;
	    if( shift0!=0 || shift1!=0 )
  	    {
  	      Jv[axisp] =Range(Jv[axisp].getBase() +shift0          ,Jv[axisp].getBound() +shift1);
  	      Iv[axisp] =Range(Iv[axisp].getBase() +shift0*cf[axisp],Iv[axisp].getBound() +shift1*cf[axisp]);
  	      Ipv[axisp]=Range(Ipv[axisp].getBase()+shift0*cf[axisp],Ipv[axisp].getBound()+shift1*cf[axisp]);
  	    }
	    
  	  }
	  
	  I1=IndexBB(I1,cf[0]);  I2=IndexBB(I2,cf[1]);  I3=IndexBB(I3,cf[2]);  // set stride

          // ------------------------------------------------------------
	  // ------- First do equations ON the boundary -----------------
          // ------------------------------------------------------------

	  if( parameters.boundaryAveragingOption[1]==OgmgParameters::partialWeighting )
	  {
	    option[axis]=restrictedFullWeighting;
            
	    
            // ipar[0]=1;  // 1= apply even symmetry condition to coefficients
            ipar[1]=side;
            ipar[2]=axis;

            //#ifdef USE_PPP
  	    //  OV_ABORT("operator averaging: BC: ERROR: finish me for parallel");
            //#endif
	    averageCoefficients(I1,I2,I3,I1p,I2p,I3p,J1,J2,J3,option,cFineLocal,cCoarseLocal,ipar);

            ipar[0]=0; // reset

	    if( false ) // for testing
	    {
	      op.coefficients(MappedGridOperators::laplacianOperator,coeffCoarse,J1,J2,J3);
	      getGhostIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3,-1); // first line in
	      op.coefficients(MappedGridOperators::laplacianOperator,coeffCoarse,J1,J2,J3);
	    }
	    

            // use restricted full weighting on ends in 2d or edges in 3D
	    for( int dir=0; dir<numberOfDimensions-1; dir++ )
	    {
	      const int axisp=(axis+dir+1)%numberOfDimensions;  // tangential direction
              if( (bool)mgCoarse.isPeriodic(axisp) ) 
                continue;   // periodic boundaries are ok on the ends 
	      
	      for( int side2=0; side2<=1; side2++ )
	      {
		if( (side2==0 && Iv[axisp].getBase() ==mgFine.gridIndexRange(0,axisp)) ||  // only do if this is really the end pt
		    (side2==1 && Iv[axisp].getBound()==mgFine.gridIndexRange(1,axisp)) )
		{
                  // (I1c,I2c,I3c) : corner or edge
		  I1c=I1; I2c=I2; I3c=I3;
		  Ivc[axisp] = side2==0 ? Iv[axisp].getBase() : Iv[axisp].getBound();

                  // (I1a,I2a,I3a) : (I1c,I2c,I3c) + 1 extra in each direction (for averaging)
                  // *030831* Next lines are wrong, need to have a buffer of cf[axis]
		  // *030831* I1a=I1p; I2a=I2p; I3a=I3p;
		  // *030831* Iav[axisp] = side2==0 ? Ipv[axisp].getBase() : Ipv[axisp].getBound();
                  // I1a=Range(I1c.getBase()-1,I1c.getBound()+1);
                  // I2a=Range(I2c.getBase()-1,I2c.getBound()+1);
                  // I3a=numberOfDimensions==2 ? Range(I3c) : Range(I3c.getBase()-1,I3c.getBound()+1);

		  I1a=I1p; I2a=I2p; I3a=I3p;
		  Iav[axisp] = side2==0 ? Ivc[axisp].getBase() : Ivc[axisp].getBound(); // this direction: injection
		  
		  J1c=J1; J2c=J2; J3c=J3;
		  Jvc[axisp] = side2==0 ? Jv[axisp].getBase() : Jv[axisp].getBound();

                  option[axisp]=restrictedFullWeighting;
		  averageCoefficients(I1c,I2c,I3c,I1a,I2a,I3a,J1c,J2c,J3c,option,cFineLocal,cCoarseLocal,ipar);

                  if( debug & 64 )
		  {
		    aString buff;
                    Range all;
		    ::display(cCoarse(all,J1c,J2c,J3c),
                       sPrintF(buff,"Neumann side=%i axis=%i side2=%i cCoarse(all,J1c,J2c,J3c)",side,axis,side2),debugFile);
		    ::display(cFine(all,I1a,I2a,I3a),
                       sPrintF(buff,"Neumann side=%i axis=%i side2=%i cFine(all,I1a,I2a,I3a)",side,axis,side2),debugFile);
		    
		  }
		  
                  if( numberOfDimensions==3 )
		  {
		    // end points of edges in 3D
                    const int axisp2= dir==0 ?  (axisp+1)%numberOfDimensions : (axis+1)%numberOfDimensions;
		    assert( axisp2!=axis && axisp2!=axisp );
		    for( int side3=0; side3<=1; side3++ )
		    {
		      Ivc[axisp2] = side3==0 ? Iv[axisp2].getBase()  : Iv[axisp2].getBound();
		      // *030831 * Iav[axisp2] = side3==0 ? Ipv[axisp2].getBase() : Ipv[axisp2].getBound();
                      // Iav[axisp2] = numberOfDimensions==2 ? Range(Ivc[axisp2]) : 
                      //               Range(Ivc[axisp2].getBase()-1,Ivc[axisp2].getBound()+1);
                      Iav[axisp2] = side3==0 ? Ivc[axisp2].getBase() : Ivc[axisp2].getBound(); // injection
		      Jvc[axisp2] = side3==0 ? Jv[axisp2].getBase()  : Jv[axisp2].getBound();

		      option[axisp2]=restrictedFullWeighting;
		      averageCoefficients(I1c,I2c,I3c,I1a,I2a,I3a,J1c,J2c,J3c,option,cFineLocal,cCoarseLocal,ipar);
		    }
		    option[axisp2]=fullWeighting; // reset
                  }
                  option[axisp]=fullWeighting; // reset
		}
	      }
	    }
	    
	    option[axis]=fullWeighting; // reset

	  }
	  else
	  {
	    printF("ERROR: unknown boundaryAveragingOption[1]=%i\n",parameters.boundaryAveragingOption[1]);
	    OV_ABORT("error");
	  }

          // ------------------------------------------------------------
	  // ------- Assign Coefficients on the GHOST line --------------
          // ------------------------------------------------------------

          Index Imv[3], &I1m=Imv[0], &I2m=Imv[1], &I3m=Imv[2];
          Index Jmv[3], &J1m=Jmv[0], &J2m=Jmv[1], &J3m=Jmv[2];

	  getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3); // recompute to be full length

	  getGhostIndex(mgFine.gridIndexRange(),side,axis,I1m,I2m,I3m);
	  getGhostIndex(mgFine.gridIndexRange(),side,axis,I1p,I2p,I3p,1,1); // one line wider
	  getGhostIndex(mgCoarse.gridIndexRange(),side,axis,J1m,J2m,J3m);

	  I1m=IndexBB(I1m,cf[0]);  I2m=IndexBB(I2m,cf[1]);  I3m=IndexBB(I3m,cf[2]);  // set stride

	  if( bc(side,axis,grid)==OgesParameters::extrapolate )
	  {
              // this is done below ...
	  }
	  else if( parameters.ghostLineAveragingOption[1]==OgmgParameters::imposeNeumann ||
                   orderOfAccuracy==4 )
	  {
            // this is done below ...
	  }
	  else if( parameters.ghostLineAveragingOption[1]==OgmgParameters::partialWeighting )
	  {
            // **** could use partialWeighting except at the end points *****

            option[axis]=restrictedFullWeighting;
	    averageCoefficients(I1m,I2m,I3m,I1p,I2p,I3p,J1m,J2m,J3m,option,cFineLocal,cCoarseLocal,ipar);

	    // use restricted full weighting on ends in 2d or edges in 3D
	    for( int dir=0; dir<numberOfDimensions-1; dir++ )
	    {
	      const int axisp=(axis+dir+1)%numberOfDimensions;  // tangential direction
	      for( int side2=0; side2<=1; side2++ )
	      {
		if( (bool)mgCoarse.isPeriodic(axisp) ) 
		  continue;   // periodic boundaries are ok on the ends 

		// *wdh* 030901 if( (side2==0 && Iv[axisp].getBase() ==mgFine.gridIndexRange(0,axisp)) ||  // only do if this is the end pt
		// *wdh* 030901     (side2==1 && Iv[axisp].getBound()==mgFine.gridIndexRange(1,axisp)) )
		if( (side2==0 && Imv[axisp].getBase() ==mgFine.gridIndexRange(0,axisp)) ||  // only do if this is the end pt
		    (side2==1 && Imv[axisp].getBound()==mgFine.gridIndexRange(1,axisp)) )
		{
		  I1c=I1m; I2c=I2m; I3c=I3m;
		  Ivc[axisp] = side2==0 ? Imv[axisp].getBase() : Imv[axisp].getBound();
                  // *030831* Next lines are wrong, need to have a buffer of cf[axis]
		  // *030831* I1a=I1p; I2a=I2p; I3a=I3p;
		  // *030831* Iav[axisp] = side2==0 ? Ipv[axisp].getBase() : Ipv[axisp].getBound();
                  // I1a=Range(I1c.getBase()-1,I1c.getBound()+1);
                  // I2a=Range(I2c.getBase()-1,I2c.getBound()+1);
                  // I3a=numberOfDimensions==2 ? Range(I3c) : Range(I3c.getBase()-1,I3c.getBound()+1);
		  // I1a=I1c; I2a=I2c; I3a=I3c;  
		  I1a=I1p; I2a=I2p; I3a=I3p;
		  Iav[axisp] = side2==0 ? Ivc[axisp].getBase() : Ivc[axisp].getBound(); // injection

		  // *wdh* 030901 J1c=J1; J2c=J2; J3c=J3;
		  J1c=J1m; J2c=J2m; J3c=J3m;  // assign ghost line at ends
		  Jvc[axisp] = side2==0 ? Jmv[axisp].getBase() : Jmv[axisp].getBound();

                  option[axisp]=restrictedFullWeighting;
		  averageCoefficients(I1c,I2c,I3c,I1a,I2a,I3a,J1c,J2c,J3c,option,cFineLocal,cCoarseLocal,ipar);
                  if( numberOfDimensions==3 )
		  {
		    // do ends of edges in 3D
                    const int axisp2= dir==0 ?  (axisp+1)%numberOfDimensions : (axis+1)%numberOfDimensions;
		    assert( axisp2!=axis && axisp2!=axisp );
		    for( int side3=0; side3<=1; side3++ )
		    {
		      Ivc[axisp2] = side3==0 ? Imv[axisp2].getBase() : Imv[axisp2].getBound();
		      // *030831 * Iav[axisp2] = side3==0 ? Ipv[axisp2].getBase() : Ipv[axisp2].getBound();
                      // Iav[axisp2] = numberOfDimensions==2 ? Range(Ivc[axisp2]) : 
                      //               Range(Ivc[axisp2].getBase()-1,Ivc[axisp2].getBound()+1);
                      Iav[axisp2] = side3==0 ? Ivc[axisp2].getBase() : Ivc[axisp2].getBound(); // injection

		      Jvc[axisp2] = side3==0 ? Jmv[axisp2].getBase() : Jmv[axisp2].getBound();

		      option[axisp2]=restrictedFullWeighting;
		      averageCoefficients(I1c,I2c,I3c,I1a,I2a,I3a,J1c,J2c,J3c,option,cFineLocal,cCoarseLocal,ipar);
		    }
		    
		    option[axisp2]=fullWeighting; // reset
		  }
                  option[axisp]=fullWeighting; // reset

		  if( debug & 64 )
		  {
		    aString buff;
                    Range all;
		    ::display(cCoarse(all,J1c,J2c,J3c),
			      sPrintF(buff,"Neumann:Ghost side=%i axis=%i side2=%i cCoarse(all,J1c,J2c,J3c)",side,axis,side2),debugFile);
		    ::display(cFine(all,I1a,I2a,I3a),
			      sPrintF(buff,"Neumann:Ghost side=%i axis=%i side2=%i cFine(all,I1a,I2a,I3a)",side,axis,side2),debugFile);
		    
		  }
		}
	      }
	    }
            option[axis]=fullWeighting; // reset
	  }
	  else
	  {
            printF("ERROR: unknown ghostLineAveragingOption[1]=%i\n",parameters.ghostLineAveragingOption[1]);
	    OV_ABORT("error");
	  }
	  

	} // end Neumann

      } // end if mgCoarse.bc > 0 

    } // end for side 
  } // end for axis 
  



  //  time=getCPU()-time;
  // printF(">>>Time for averageCoefficients=%8.2e\n",time);
  if( Ogmg::debug & 64 )
  {
    Range all;
    fPrintF(debugFile,"level=%i : cFine for operator averaging:\n",level);
    Index K1,K2,K3;
    getIndex(mgFine.indexRange(),K1,K2,K3,1); // include a ghostpoint
    display(cFine(all,K1,K2,K3),"cFine",debugFile,"%7.1e ");
    fPrintF(debugFile,"level=%i : averaged operator cCoarse after averageCoefficients\n",level);
    display(cCoarse(all,J1,J2,J3),"cCoarse",debugFile,"%7.1e ");
  }
  
  if( (Ogmg::debug & 128) && orderOfAccuracy==4 )
  {
    // -- *wdh* 100118 -- Bug fixed in averageOpt.bf 
    // check whether the averaged coefficients sum to zero 
    int length=cCoarseLocal.getLength(0);
    Index I1,I2,I3;
    getIndex(mgFine.gridIndexRange(),I1,I2,I3);
    int includeGhost=0;
    bool ok =ParallelUtility::getLocalArrayBounds(maskFine,maskFineLocal,I1,I2,I3,includeGhost);
    fprintf(pDebugFile," **** Sum of coefficients cFine on level=%i *** \n",level);
    FOR_3D(i1,i2,i3,I1,I2,I3)
    {
      real sum = 0;
      for( int m=0; m<length; m++ ) sum+= cFineLocal(m,i1,i2,i3);
      
      fprintf(pDebugFile," i=(%i,%i,%i) sum(coeffFine)=%8.2e\n",i1,i2,i3,sum);
    }
    fflush(pDebugFile);


    int extra=-2;
    getIndex(dimLocalCoarse,I1,I2,I3,extra);
    fprintf(pDebugFile," **** Sum of coefficients cCoarse on level=%i after aveCoeff interior *** \n",level+1);
    FOR_3(i1,i2,i3,I1,I2,I3)
    {
      real sum = 0;
      for( int m=0; m<length; m++ ) sum+= cCoarseLocal(m,i1,i2,i3);
      
      fprintf(pDebugFile," i=(%i,%i,%i) sum(coeffCoarse)=%8.2e\n",i1,i2,i3,sum);
    }
    fflush(pDebugFile);
  }
  
  if( Ogmg::debug & 4 || Ogmg::debug & 64 )
  {
    fprintf(pDebugFile,"level=%i, grid=%i : averaged operator cCoarse Local before copyArray\n",level+1,grid);
    // ::display(cFineLocal,"cFineLocal",pDebugFile,"%7.1e ");
    ::display(cCoarseLocal,"cCoarseLocal",pDebugFile,"%7.1e ");
    fflush(pDebugFile);
  }
  
  #ifdef USE_PPP
    // --- now copy the local coarse arrays into the distributed coarse array 

    // Note: the local arrays cCoarseLocal live on the same proc. as cFine: 
    const intSerialArray & cCoarseLocalProcessorSet = maskFine.getPartition().getProcessorSet();
    getIndex(mgCoarse.gridIndexRange(),I1,I2,I3,numGhost);  // points we need copied into 
    Range I0=cCoarse.dimension(0);
    Range J0=I0;
    if( thisProcessorHasPoints )
    {
      getIndex(dimLocalCoarse,J1,J2,J3);  // is this right?
    }
    else
    {
      Range all;
      J0=all; J1=all; J2=all; J3=all;
    }
    Index Iva[4] = {I0, I1,I2,I3};  // 
    Index Jva[4] = {J0, J1,J2,J3};  // 
    CopyArray::copyArray( cCoarseLocal,Jva,cCoarseLocalProcessorSet, cCoarse, Iva );

    // cCoarse.periodicUpdate();      // I don't think this is needed.
    // cCoarse.updateGhostBoundaries();   // *wdh* do this below after BC's 091230
  #endif

  if( Ogmg::debug & 4 || Ogmg::debug & 64 )
  {
    fPrintF(debugFile,"level=%i, grid=%i : averaged operator cCoarse AFTER copyArray\n",level+1,grid);
    displayCoeff(coeffCoarse,"cCoarse",debugFile,"%7.1e ");
    fflush(debugFile);
  }


  // -------------------------
  // ----- boundaries --------
  // -------------------------

//   if( false && orderOfAccuracy==4 )
//   {  
//     // extrap 2nd ghost line *********************** fix this *********************
//     extrapParams.ghostLineToAssign=2;
//     coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,extrapParams); 
//     extrapParams.ghostLineToAssign=1;
	
//   }

  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( mgCoarse.boundaryCondition(side,axis)>0 )
      {

	getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3);
	getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1p,I2p,I3p,1);
	getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3);

        I1=IndexBB(I1,cf[0]);  I2=IndexBB(I2,cf[1]);  I3=IndexBB(I3,cf[2]);  // set stride

	if( boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate ) 
	{
          // ****************************************************************************
          // *************************DIRICHLET******************************************
          // ****************************************************************************


          if( Ogmg::debug & 4 )
  	    printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) `extrapolate BC' \n",
		 level+1,grid,side,axis);

	  
          if(  parameters.ghostLineAveragingOption[0]==OgmgParameters::imposeExtrapolation &&
               parameters.boundaryAveragingOption[0]==OgmgParameters::imposeDirichlet )
	  {
            // New way
	    assignBoundaryConditionCoefficients( coeffCoarse, grid, level+1, side,axis );
            continue;
	  }
	  


	  bool useEquationOnGhost = useEquationOnGhostLineForDirichletBC(mgCoarse,level+1);

	  // Line solvers may need the equation on the ghost line, otherwise we only need to fill in
          // the matrix on the coarsest level since the BC's other levels will be treated by the BC routine.
          bool lineSolverIsUsed = true;  // **** can we fix this ? do we know atthis point?
	  useEquationOnGhost = (useEquationOnGhost && 
                                 ( (level+1) == mgcg.numberOfMultigridLevels()-1 ||
                                   lineSolverIsUsed ) );




          // ------ ghost line ---------
          if( parameters.ghostLineAveragingOption[0]==OgmgParameters::imposeExtrapolation )
	  {
	    // extrapParams.orderOfExtrapolation=2; // "odd symmetry u(-1)=2*u(0)-u(1)"
	    // extrapParams.orderOfExtrapolation=4; // nearly consistent with odd symmetry , 4th deriv = 0

	    extrapParams.orderOfExtrapolation=parameters.orderOfExtrapolationForDirichletOnLowerLevels;
            if( !useEquationOnGhost )
              coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary(side,axis),
                              extrapParams);
            extrapParams.orderOfExtrapolation=orderOfExtrapolation; 
          }
	  else
	  {
            printF("ERROR: unknown ghostLineAveragingOption[0]=%i\n",parameters.ghostLineAveragingOption[0]);
	    OV_ABORT("error");
	  }

           
          // ------- boundary -------
          if( parameters.boundaryAveragingOption[0]==OgmgParameters::imposeDirichlet )
	  {
            // apply dirichlet for now ***fix this*** may not be  ********************************************
	    printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) `extrapolate BC' \n"
		   " ******************* setting a Dirichlet BC FIX THIS ****************************\n",
		   level+1,grid,side,axis);

            if( Ogmg::debug & 4 )
   	      printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) apply dirichlet BC \n",
		       level+1,grid,side,axis);


            if( useEquationOnGhost )
	    {
	      if( equationToSolve==OgesParameters::laplaceEquation )
	      {
                Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
                Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
		
		
                // *** this is wrong -- see code in predefined ---
//  		getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,I1,I2,I3);
//  		op.setOrderOfAccuracy(2);
//  		op.coefficients(MappedGridOperators::laplacianOperator,coeffCoarse,I1,I2,I3); // efficient version
//  		op.setOrderOfAccuracy(4);
//  		getGhostIndex(mgCoarse.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
//                  Range all;
//  		coeffCoarse(all,Ig1,Ig2,Ig3)=coeffCoarse(all,I1,I2,I3);

                getBoundaryIndex(mgCoarse.indexRange(),side,axis,I1,I2,I3);
                getGhostIndex(mgCoarse.indexRange(),side,axis,Ig1,Ig2,Ig3);
                // do not apply on ends if the adjacent boundary is Dirichlet -- otherwise the 
                // same equation would appear twice
		for( int dir=1; dir<numberOfDimensions; dir++ )
		{
		  int axisp=(axis+dir)%numberOfDimensions;  // tangential direction

		  int bca=boundaryCondition(Start,axisp,grid);
		  int ia=Iv[axisp].getBase();
		  ia= bca==OgmgParameters::extrapolate ? ia+1 : ia;

		  int bcb=boundaryCondition(End,axisp,grid);
		  int ib=Iv[axisp].getBound();
		  ib= bcb==OgmgParameters::extrapolate ? ib-1 : ib;
		  Iv[axisp]=Range(ia,ib);
                  Igv[axisp]=Iv[axisp];
		}

                op.setOrderOfAccuracy(2);
                realArray tempCoeff(coeffCoarse.dimension(0),I1,I2,I3);
                op.assignCoefficients(MappedGridOperators::laplacianOperator,tempCoeff,I1,I2,I3); // efficient version
                op.setOrderOfAccuracy(4);

		const int ee=0; 
		int I1Base,I2Base,I3Base;
		int I1Bound,I2Bound,I3Bound;
		int i1,i2,i3;
		is1=is2=is3=0;
		isv[axis]=1-2*side;

		const int m3b= numberOfDimensions==2 ? -1 : 1;
		for( int m3=-1; m3<=m3b; m3++ )
		  for( int m2=-1; m2<=1; m2++ )
		    for( int m1=-1; m1<=1; m1++ )
		    {
		      // copy the second order equation into the correct positions of the 4th order stencil
		      const int index2=(m1+1)+3*(m2+1+3*(m3+1));  // stencil width=3

		      int index4=numberOfDimensions==2 ? (m1+2)+5*(m2+2)          :        // stencil width==5
			(m1+2)+5*(m2+2+5*(m3+2));
		      // remember we are shifted to the ghost line :
		      index4 += axis==0 ? 1-2*side : axis==1 ? 5*(1-2*side) : 25*(1-2*side);
		
		      FOR_3(i1,i2,i3,I1,I2,I3)
		      {
			int ig1=i1-is1, ig2=i2-is2, ig3=i3-is3;
			coeffCoarse(index4,ig1,ig2,ig3)=tempCoeff(index2,i1,i2,i3);
			coeffCoarse.sparse->setClassify(SparseRepForMGF::ghost1,ig1,ig2,ig3,ee);
		      }
		      //coeffCoarse(index4,Ig1,Ig2,Ig3)=tempCoeff(index2,I1,I2,I3);
		    }

		// coeffCoarse.sparse->setClassify(SparseRepForMGF::ghost1,Ig1,Ig2,Ig3,ee);

		// **** end points on dirichlet sides: use extrapolation (otherwise the same eqn appears twice!)

		getGhostIndex(mgCoarse.indexRange(),side,axis,Ig1,Ig2,Ig3);
		for( int dir=0; dir<numberOfDimensions-1; dir++ )
		{
		  const int axisp = (axis+dir+1) % numberOfDimensions; // adjacent side
		  for( int side2=0; side2<=1; side2++ )
		  {
		    if( boundaryCondition(side2,axisp,grid)==OgmgParameters::extrapolate )
		    {
		      I1=Ig1, I2=Ig2, I3=Ig3;
		      Iv[axisp]= side2==0 ? Iv[axisp].getBase() : Iv[axisp].getBound();
		      // extrapolate points  coeff(.,I1,I2,I3) in the direction axis
                      // printF(" Extrap ends: [%i,%i][%i,%i]\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound());
		      
		      op.setExtrapolationCoefficients(coeffCoarse,ee,I1,I2,I3,orderOfExtrapolation); // in GenericMGOP
		    }
		  }
		}


		printF("\n+++++++++++++Op Ave: fill in 2nd-order equation on the ghost points : level=%i\n",level+1);
		if( false )
		{
		  Range all;
		  coeffCoarse(all,Ig1,Ig2,Ig3).display("2nd-order equation on the ghost points");
		}
		
	      }
	      else
	      {
		Overture::abort("operator averaging: apply boundary conditions");
	      }
	    }
          
	    coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::boundary(side,axis));
	    
	  }
          else if( parameters.boundaryAveragingOption[0]==OgmgParameters::partialWeighting )
	  {
	    printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) `extrapolate BC' \n"
		   " ******************* partial weighting on boundary ****************************\n",
		   level+1,grid,side,axis);
	    option[axis]=restrictedFullWeighting;
            averageCoefficients(I1,I2,I3,I1p,I2p,I3p,J1,J2,J3,option,cFineLocal,cCoarseLocal,ipar);
	    option[axis]=fullWeighting;
	  }
          else if( parameters.boundaryAveragingOption[0]==OgmgParameters::lumpedPartialWeighting )
	  {
            Overture::abort("error");
//  	    printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) `extrapolate BC' \n"
//  		   " ******************* lumped partial weighting on boundary ****************************\n",
//  		   level+1,grid,side,axis);
//  	    option[axis]=restrictedFullWeighting;
//              averageCoefficients(I1,I2,I3,I1p,I2p,I3p,J1,J2,J3,lumpedPartialWeighting,cFine,cCoarse,ipar);
//  	    option[axis]=fullWeighting;

//              if( true ||  Ogmg::debug & 8 )
//  	    {
//                printF("level=%i : lumped boundary condition coefficients, side=%i, axis=%i\n",
//                      level,side,axis);
//                Range all;
//  	      cCoarse(all,J1,J2,J3).display("cCoarse");
//  	    }
	  }
//           else if( parameters.boundaryAveragingOption[0]==lumpedPartialWeighting )
// 	  {
// 	    const int diagonal=numberOfDimensions==1 ? 1 : numberOfDimensions==2 ? 4 : 13;
// 	    if(  max(fabs(cFine(diagonal,I1,I2,I3)-1.))<REAL_EPSILON*10. )
// 	    {
// 	    }
// 	  }
	  else
	  {
	    printF("ERROR: unknown boundaryAveragingOption[0]=%i\n",parameters.boundaryAveragingOption[0]);
	    OV_ABORT("error");
	  }
	  
	  if( orderOfAccuracy==4 )
	  {
            // ---- 2nd ghost line ----
	    extrapParams.ghostLineToAssign=2;
	    coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary(side,axis),extrapParams); 
	    extrapParams.ghostLineToAssign=1;
	  }
	  
	}
	else  // "equation" BC  (Neumann, mixed or extrap)
	{
          // ***************************************************************************************
          // *********************** NEUMANN, MIXED, or EXTRAP *************************************
          // ***************************************************************************************

	  if( bc(side,axis,grid)==OgesParameters::extrapolate )
	  { // *wdh* 100412 
	    // extrapParams.orderOfExtrapolation=getOrderOfExtrapolation(level); // parameters.orderOfExtrapolationForDirichletOnLowerLevels;
            extrapParams.orderOfExtrapolation= int( boundaryConditionData(0,side,axis,grid)+.5);
	    
	    coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary(side,axis),extrapParams);

	    if( orderOfAccuracy==4 )
	    {
	      extrapParams.ghostLineToAssign=2; // extrap 2nd ghost line
	      coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary(side,axis),extrapParams);
	      extrapParams.ghostLineToAssign=1; // reset 
	    }
            extrapParams.orderOfExtrapolation=orderOfExtrapolation; 	    

            continue;
	  }

          if(  parameters.ghostLineAveragingOption[1]==OgmgParameters::imposeNeumann ||
               orderOfAccuracy==4 )
	  {
            // ==============================================================
            // =================== New way ==================================
            // ==============================================================
	    assignBoundaryConditionCoefficients( coeffCoarse, grid, level+1, side,axis );

            continue;
	  }
	  
	  if( debug & 64 )
	  {
	    aString buff;
	    Range all;
	    ::display(cCoarse,sPrintF(buff,"Neumann side=%i axis=%i cCoarse after assign boundary",
                   side,axis),debugFile);
	  }


          // ------------------------------------------------------------
	  // ------- Assign Coefficients on the GHOST line --------------
          // ------------------------------------------------------------

          Index Imv[3], &I1m=Imv[0], &I2m=Imv[1], &I3m=Imv[2];
          Index Jmv[3], &J1m=Jmv[0], &J2m=Jmv[1], &J3m=Jmv[2];

	  getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3); // recompute to be full length

	  getGhostIndex(mgFine.gridIndexRange(),side,axis,I1m,I2m,I3m);
	  getGhostIndex(mgFine.gridIndexRange(),side,axis,I1p,I2p,I3p,1,1); // one line wider
	  getGhostIndex(mgCoarse.gridIndexRange(),side,axis,J1m,J2m,J3m);

	  I1m=IndexBB(I1m,cf[0]);  I2m=IndexBB(I2m,cf[1]);  I3m=IndexBB(I3m,cf[2]);  // set stride


          bool fixupClassification=true;

	  if( parameters.ghostLineAveragingOption[1]==OgmgParameters::imposeNeumann ||
              orderOfAccuracy==4 )
	  {
            // ** For fourth-order equations we do this since the second-order averaging of the Neumann BC results
            // in much slower convergence; For 2nd-order the averaged Neumann equation remains the same on a rect. grid

	    fixupClassification=false;  // no need for fixups below if we call the applyBC function
	    
	    printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) `equation BC' \n"
		   " ******************* setting a Neumann or Mixed BC  ****************************\n",
		   level+1,grid,side,axis);
       
            // For fourth order accuracy we must support different options ****

	    if( equationToSolve!=OgesParameters::userDefined )
	    {
	      if( bc(side,axis,grid)==OgmgParameters::neumann )
	      {
		coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::boundary1+side+2*axis);
	      }
	      else if( bc(side,axis,grid)==OgmgParameters::mixed )
	      {
                if( false &&
                    orderOfAccuracy==4 && (parameters.lowerLevelNeumannFirstGhostLineBC==OgmgParameters::useSymmetry ||
                         parameters.lowerLevelNeumannFirstGhostLineBC==
					   OgmgParameters::useEquationToSecondOrder) )
		{
                  // **** FIX FOR useEquationToSecondOrder ****
		  printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) apply MIXED BC -> Symmetry\n",
			 level+1,grid,side,axis);  
		  extrapParams.ghostLineToAssign=1;
		  coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,
								 BCTypes::boundary(side,axis),extrapParams); 
		}
		else
		{
		  RealArray & a = bcParams.a;
		  a.redim(2);

		  a(0)=boundaryConditionData(0,side,axis,grid);  // coeff of u
		  a(1)=boundaryConditionData(1,side,axis,grid);  // coeff of du/dn
		  printF("operatorAveraging: level=%i,grid=%i, (side,axis)=(%i,%i) apply MIXED BC a0,a1=%f,%f\n",
			 level+1,grid,side,axis,a(0),a(1));  
		  coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,
                                          BCTypes::boundary1+side+2*axis,bcParams);
		}
		
	      }
	      else if( bc(side,axis,grid)>0 )
	      {
		printF("Ogmg::operatorAveraging:ERROR: unknown bc=%i for grid=%i side=%i axis=%i\n",
		       bc(side,axis,grid),grid,side,axis);
		OV_ABORT("error");
	      }
	    }
	    else
	    {
	      coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::boundary(side,axis));
	      
	    }
	  }
	  else if( parameters.ghostLineAveragingOption[1]==OgmgParameters::partialWeighting )
	  {
// -- this next section was moved to above -- 
//             // **** could use partialWeighting except at the end points *****

//             option[axis]=restrictedFullWeighting;
// 	    averageCoefficients(I1m,I2m,I3m,I1p,I2p,I3p,J1m,J2m,J3m,option,cFineLocal,cCoarseLocal,ipar);

// 	    // use restricted full weighting on ends in 2d or edges in 3D
// 	    for( int dir=0; dir<numberOfDimensions-1; dir++ )
// 	    {
// 	      const int axisp=(axis+dir+1)%numberOfDimensions;  // tangential direction
// 	      for( int side2=0; side2<=1; side2++ )
// 	      {
// 		if( (bool)mgCoarse.isPeriodic(axisp) ) 
// 		  continue;   // periodic boundaries are ok on the ends 

// 		// *wdh* 030901 if( (side2==0 && Iv[axisp].getBase() ==mgFine.gridIndexRange(0,axisp)) ||  // only do if this is the end pt
// 		// *wdh* 030901     (side2==1 && Iv[axisp].getBound()==mgFine.gridIndexRange(1,axisp)) )
// 		if( (side2==0 && Imv[axisp].getBase() ==mgFine.gridIndexRange(0,axisp)) ||  // only do if this is the end pt
// 		    (side2==1 && Imv[axisp].getBound()==mgFine.gridIndexRange(1,axisp)) )
// 		{
// 		  I1c=I1m; I2c=I2m; I3c=I3m;
// 		  Ivc[axisp] = side2==0 ? Imv[axisp].getBase() : Imv[axisp].getBound();
//                   // *030831* Next lines are wrong, need to have a buffer of cf[axis]
// 		  // *030831* I1a=I1p; I2a=I2p; I3a=I3p;
// 		  // *030831* Iav[axisp] = side2==0 ? Ipv[axisp].getBase() : Ipv[axisp].getBound();
//                   // I1a=Range(I1c.getBase()-1,I1c.getBound()+1);
//                   // I2a=Range(I2c.getBase()-1,I2c.getBound()+1);
//                   // I3a=numberOfDimensions==2 ? Range(I3c) : Range(I3c.getBase()-1,I3c.getBound()+1);
// 		  // I1a=I1c; I2a=I2c; I3a=I3c;  
// 		  I1a=I1p; I2a=I2p; I3a=I3p;
// 		  Iav[axisp] = side2==0 ? Ivc[axisp].getBase() : Ivc[axisp].getBound(); // injection

// 		  // *wdh* 030901 J1c=J1; J2c=J2; J3c=J3;
// 		  J1c=J1m; J2c=J2m; J3c=J3m;  // assign ghost line at ends
// 		  Jvc[axisp] = side2==0 ? Jmv[axisp].getBase() : Jmv[axisp].getBound();

//                   option[axisp]=restrictedFullWeighting;
// 		  averageCoefficients(I1c,I2c,I3c,I1a,I2a,I3a,J1c,J2c,J3c,option,cFineLocal,cCoarseLocal,ipar);
//                   if( numberOfDimensions==3 )
// 		  {
// 		    // do ends of edges in 3D
//                     const int axisp2= dir==0 ?  (axisp+1)%numberOfDimensions : (axis+1)%numberOfDimensions;
// 		    assert( axisp2!=axis && axisp2!=axisp );
// 		    for( int side3=0; side3<=1; side3++ )
// 		    {
// 		      Ivc[axisp2] = side3==0 ? Imv[axisp2].getBase() : Imv[axisp2].getBound();
// 		      // *030831 * Iav[axisp2] = side3==0 ? Ipv[axisp2].getBase() : Ipv[axisp2].getBound();
//                       // Iav[axisp2] = numberOfDimensions==2 ? Range(Ivc[axisp2]) : 
//                       //               Range(Ivc[axisp2].getBase()-1,Ivc[axisp2].getBound()+1);
//                       Iav[axisp2] = side3==0 ? Ivc[axisp2].getBase() : Ivc[axisp2].getBound(); // injection

// 		      Jvc[axisp2] = side3==0 ? Jmv[axisp2].getBase() : Jmv[axisp2].getBound();

// 		      option[axisp2]=restrictedFullWeighting;
// 		      averageCoefficients(I1c,I2c,I3c,I1a,I2a,I3a,J1c,J2c,J3c,option,cFineLocal,cCoarseLocal,ipar);
// 		    }
		    
// 		    option[axisp2]=fullWeighting; // reset
// 		  }
//                   option[axisp]=fullWeighting; // reset

// 		  if( debug & 64 )
// 		  {
// 		    aString buff;
//                     Range all;
// 		    ::display(cCoarse(all,J1c,J2c,J3c),
// 			      sPrintF(buff,"Neumann:Ghost side=%i axis=%i side2=%i cCoarse(all,J1c,J2c,J3c)",side,axis,side2),debugFile);
// 		    ::display(cFine(all,I1a,I2a,I3a),
// 			      sPrintF(buff,"Neumann:Ghost side=%i axis=%i side2=%i cFine(all,I1a,I2a,I3a)",side,axis,side2),debugFile);
		    
// 		  }
// 		}
// 	      }
// 	    }
//             option[axis]=fullWeighting; // reset

	    
	  }
	  else
	  {
            printF("ERROR: unknown ghostLineAveragingOption[1]=%i\n",parameters.ghostLineAveragingOption[1]);
	    OV_ABORT("error");
	  }
	  

          // *****fix up equation numbers and classify****          
          if( fixupClassification )
	  {
	    const int numberOfComponentsForCoefficients=1;  // **** fix these ******************
	    const int width=3;
	    const int stencilSize = int( pow(width,numberOfDimensions) );
	    // const int halfWidth1= 1;
	    // const int halfWidth2= numberOfDimensions>1 ? 1 : 0;
	    // const int halfWidth3= numberOfDimensions>2 ? 1 : 0;
	
	
	    assert( coeffCoarse.sparse!=NULL );
	    int m1,m2,m3,ee,c;
	    Range E(0,0), C(0,0);
	
	    for( ee=E.getBase(); ee<=E.getBound(); ee++ )                        
	    {
	      for( c=C.getBase(); c<=C.getBound(); c++ )                        
	      {
		ForStencil(m1,m2,m3)  
		  coeffCoarse.sparse->setCoefficientIndex(M123CE(m1,m2,m3,c,ee), ee,J1m,J2m,J3m, c, (J1+m1),(J2+m2),(J3+m3) );  
	      }
	      coeffCoarse.sparse->setClassify(SparseRepForMGF::ghost1,J1m,J2m,J3m,ee);
	    }
	  }
	  
	  if( true && orderOfAccuracy==4 )
	  {
            bool useEquationOnGhost = orderOfAccuracy==4 ? useEquationOnGhostLineForNeumannBC(mgCoarse,level+1) : false;

            if( orderOfAccuracy==4 && parameters.useSymmetryForNeumannOnLowerLevels )
	    {
	      
              if( bc(side,axis,grid)==OgmgParameters::neumann && parameters.fourthOrderBoundaryConditionOption==1 )
	      {
                // This results in poor convergence for a square with mixed BC's
		printF("+++++++++++++++++Op ave: assign even symmetry to BOTH ghost lines for Neumann BC "
                       "(%i,%i) level=%i\n",side,axis,level+1);

		extrapParams.ghostLineToAssign=1;
		coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,
                                                               BCTypes::boundary(side,axis),extrapParams); 
	      }
	      
	      if( useEquationOnGhost )
	      {
		printF("\n *********WARNING: opAve: NOT using EQN BC for Neumann Boundary on level %i, "
		       " using symmetry for 2nd ghost ****************\n",level+1);
	      }
	      extrapParams.ghostLineToAssign=2;
              coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,
                                                                 BCTypes::boundary(side,axis),extrapParams); 
              extrapParams.ghostLineToAssign=1;
	    }
	    else
	    {
	      if( useEquationOnGhost )
	      {
		printF("\n *********WARNING: opAve: NOT using EQN BC for Neumann Boundary on level %i-- "
                       " Extrapolating 2nd ghost ****************\n",level+1);
	      }

	      // ---- 2nd ghost line ----
	      extrapParams.ghostLineToAssign=2;
	      //    coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary(side,axis),extrapParams); 
	      // *** fix this coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,BCTypes::boundary(side,axis),extrapParams); 
	      printF("+++++++++++++++++Op ave: extrap 2nd ghost line on Neumann BC (%i,%i) level=%i\n",side,axis,level+1);

	      extrapParams.orderOfExtrapolation=3;
	      coeffCoarse.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary(side,axis),extrapParams); 
	      extrapParams.ghostLineToAssign=1;
	    }
	    
        
	  }


	  if( boundaryCondition(side,axis,grid)!=OgmgParameters::equation ) 
	  {
            // This boundary has a mixture of equation and extrapolation points
	    // over-write extrapolation points.
/* ----
	    const IntegerArray & classifyConst = c.sparse->classify(I1m,I2m,I3m);
	    IntegerArray & classify = (IntegerArray &)classifyConst;
	    classify.reshape(1,I1m,I2m,I3m);
	    where( classify==SparseRepForMGF::extrapolation  )
	    {
	      u(0,I1m,I2m,I3m)=-(c(1,I1m,I2m,I3m)*u(0,I1m+  is1,I2m+  is2,I3m+  is3)+
				 c(2,I1m,I2m,I3m)*u(0,I1m+2*is1,I2m+2*is2,I3m+2*is3)+
				 c(3,I1m,I2m,I3m)*u(0,I1m+3*is1,I2m+3*is2,I3m+3*is3)+
				 c(4,I1m,I2m,I3m)*u(0,I1m+4*is1,I2m+4*is2,I3m+4*is3)
		)/c(0,I1m,I2m,I3m);

	    }
---- */
	  }
        
	  if( debug & 64 )
	  {
	    aString buff;
	    Range all;
	    ::display(cCoarse,sPrintF(buff,"Neumann side=%i axis=%i cCoarse at end",
                   side,axis),debugFile);
	  }

	} // end Neumann
      }
    }
  }

  

  #ifdef USE_PPP
    // Is this needed? 
    cCoarse.updateGhostBoundaries();  
  #endif


  if( Ogmg::debug & 4 || Ogmg::debug & 64 )
  {
    fPrintF(debugFile,"level=%i, grid=%i : averaged operator cCoarse at end\n",level+1,grid);
    displayCoeff(coeffCoarse,"cCoarse",debugFile,"%7.1e ");
    fflush(debugFile);

    if( parameters.saveGridCheckFile )
    {
      // -- save the coarse grid equatons in the grid check file for regression tests ---
      if( myid==0 ) 
        assert( gridCheckFile!=NULL );
      fPrintF(gridCheckFile,"\n");
      displayCoeff(coeffCoarse,sPrintF("level=%i, grid=%i, averaged coarse grid coeff matrix,  cCoarse",level+1,grid),
                   gridCheckFile,"%7.1e ");
    }
    
  }
  
  return 0;
}


// 1D full weighting along axis1==0 for the 3 coefficients
#define FW0_1D_0(F,m0,m1,m2,I1,I2,I3) (.25*(F(m0,I1-1,I2,I3)+.5*F(m1,I1-1,I2,I3)+F(m0,I1  ,I2,I3)) )
#define FW0_1D_1(F,m0,m1,m2,I1,I2,I3) (.5*F(m1,I1  ,I2,I3)+.125*(F(m1,I1-1,I2,I3)+F(m1,I1+1,I2,I3))+ \
                                     .25*(F(m2,I1-1,I2,I3)+F(m2,I1  ,I2,I3)+ \
                                          F(m0,I1  ,I2,I3)+F(m0,I1+1,I2,I3)) )
#define FW0_1D_2(F,m0,m1,m2,I1,I2,I3) (.25*(F(m2,I1  ,I2,I3)+.5*F(m1,I1+1,I2,I3)+F(m2,I1+1,I2,I3)) )

// 1D full weighting along axis2==1 for the 3 coefficients
#define FW1_1D_0(F,m0,m1,m2,I1,I2,I3) (.25*(F(m0,I1,I2-1,I3)+.5*F(m1,I1,I2-1,I3)+F(m0,I1,I2  ,I3)) )
#define FW1_1D_1(F,m0,m1,m2,I1,I2,I3) (.5*F(m1,I1  ,I2,I3)+.125*(F(m1,I1,I2-1,I3)+F(m1,I1,I2+1,I3))+ \
                                     .25*(F(m2,I1,I2-1,I3)+F(m2,I1  ,I2,I3)+ \
                                          F(m0,I1  ,I2,I3)+F(m0,I1,I2+1,I3)) )
#define FW1_1D_2(F,m0,m1,m2,I1,I2,I3) (.25*(F(m2,I1  ,I2,I3)+.5*F(m1,I1,I2+1,I3)+F(m2,I1,I2+1,I3)) )

// 1D full weighting along axis2==2 for the 3 coefficients
#define FW2_1D_0(F,m0,m1,m2,I1,I2,I3) (.25*(F(m0,I1,I2,I3-1)+.5*F(m1,I1,I2,I3-1)+F(m0,I1,I2  ,I3)) )
#define FW2_1D_1(F,m0,m1,m2,I1,I2,I3) (.5*F(m1,I1  ,I2,I3)+.125*(F(m1,I1,I2,I3-1)+F(m1,I1,I2,I3+1))+ \
                                     .25*(F(m2,I1,I2,I3-1)+F(m2,I1  ,I2,I3)+ \
                                          F(m0,I1  ,I2,I3)+F(m0,I1,I2,I3+1)) )
#define FW2_1D_2(F,m0,m1,m2,I1,I2,I3) (.25*(F(m2,I1  ,I2,I3)+.5*F(m1,I1,I2,I3+1)+F(m2,I1,I2,I3+1)) )

// 1D Injection for the 3 coefficients
// define INJECTION_1D_0(F,m0,m1,m2,I1,I2,I3) (.5*(F(m0,I1,I2,I3)))
// define INJECTION_1D_1(F,m0,m1,m2,I1,I2,I3) (.5*(F(m0,I1,I2,I3)+F(m2,I1,I2,I3))+F(m1,I1,I2,I3))
// define INJECTION_1D_2(F,m0,m1,m2,I1,I2,I3) (.5*(F(m2,I1,I2,I3)))

// **** INJECTION is defined like FW but assumes coeff's are locally constant

#define INJECTION_1D_0(F,m0,m1,m2,I1,I2,I3) (.25*(F(m0,I1,I2,I3)+.5*F(m1,I1,I2,I3)+F(m0,I1,I2,I3)) )
#define INJECTION_1D_1(F,m0,m1,m2,I1,I2,I3) (.5*F(m1,I1,I2,I3)+.125*(F(m1,I1,I2,I3)+F(m1,I1,I2,I3))+ \
                                     .25*(F(m2,I1,I2,I3)+F(m2,I1,I2,I3)+ \
                                          F(m0,I1,I2,I3)+F(m0,I1,I2,I3)) )
#define INJECTION_1D_2(F,m0,m1,m2,I1,I2,I3) (.25*(F(m2,I1,I2,I3)+.5*F(m1,I1,I2,I3)+F(m2,I1,I2,I3)) )


// define C(m,n) ((m)+width*(n))

int Ogmg::
averageCoefficients(Index & I1, Index & I2, Index & I3,
		    Index & I1p, Index & I2p, Index & I3p,
		    Index & J1, Index & J2, Index & J3,
		    TransferTypesEnum option[3],
		    const realSerialArray & cFine, 
		    realSerialArray & cCoarse, int ipar[] )
// =========================================================================================
// /Description:
//    Form a coarse grid operator by averaging from a fine grid operator.
//
// With option==fullWeighting the operator at a point is averaged with the operators of 
//  the nearest neighbours. In 1D point, for example we would average the equations at
// points $i-1$, $i$ and $i+1$
// 
//    a_{i-2} u_{i-2}  b_{i-1} u_{i-1}  c_{i  } u_{i  }
//                     a_{i-1} u_{i-1}  b_{i  } u_{i  }  c_{i+1} u_{i+1}
//                                      a_{i  } u_{i  }  b_{i+1} u_{i+1}  c_{i+2} u_{i+2}
//
//  We average the 3 equations with weights $1/4$, $1/2$ and $1/4$. The values at points 
//  $i-1$ and $i+1$ are distributed to points $i-2$, $i$ and $i+2$ to give a new operator
// only defined on $i-2$, $i$ and $i+2$.
//
//  With option==restrictedFullWeighting the operator at point $i$ is averaged as if the stencil operator
//  at points $i-1$ and $i+1$ had the same coefficients as point $i$.
//
// /option[axis] (input) : fullWeighting, restrictedFullWeighting, injection
// 
// Notes:
// Typical steps showing use of (I1,I2,I3) : fine grid stride "2"
//                              (J1,J2,J3) : coarse grid points to assign
//                              (I1p,I2p,I3p) : fine grid, no stride, width 1 bigger in each direction
// 
//    c0(*,J1,I2p,I3p)=FW0_1D_0(cFine,m0,m1,m2,I1,I2p,I3p);
//    c1(*,J1,J2,I3p)=FW1_1D_0(c0,m0,m1,m2,J1,I2,I3p); 
//    cCoarse(*,J1,J2,J3)=FW2_1D_0(c1,m0,m1,m2,J1,J2,I3);
// =========================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  const int & numberOfDimensions = mgcg.numberOfDimensions();

  if( true )  // averageOpt does not implement lumpPartialWeighting -- but not currently used ---
  {
    // ****** optimized version *****

    const int ndc=cFine.getLength(0);

    const int width = orderOfAccuracy+1; 
    const int halfWidth = (width-1)/2;  // stencilHalfWidth
    const intArray & mask = mgcg[0].mask();
    int numGhost[3]={0,0,0};
    // int numExtraParallelGhost=INT_MAX;
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      numGhost[axis]=mask.getGhostBoundaryWidth(axis);
      // numExtraParallelGhost = min(numExtraParallelGhost,mask.getGhostBoundaryWidth(axis)-halfWidth);
    }
    
    // printF("--> averageCoefficients: halfWidth=%i numGhost=%i numExtraParallelGhost=%i\n",halfWidth,mask.getGhostBoundaryWidth(0),
    //      numExtraParallelGhost);
    
    Index Iv[3]={I1,I2,I3}; //
    Index Jv[3]={J1,J2,J3}; //
    Index Ivp[3]={I1p,I2p,I3p}; //
    #ifdef USE_PPP 
      // realSerialArray cFineLocal;   getLocalArrayWithGhostBoundaries(cFine,cFineLocal);
      // realSerialArray cCoarseLocal; getLocalArrayWithGhostBoundaries(cCoarse,cCoarseLocal);
      const realSerialArray & cFineLocal = cFine;
      realSerialArray & cCoarseLocal = cCoarse;

      // bool ok =ParallelUtility::getLocalArrayBounds(maskCoarse,maskCoarseLocal,Jv[0],Jv[1],Jv[2]);
      bool ok=true;
      for( int dir=0; dir<numberOfDimensions; dir++ )
      {
        Index & I = Iv[dir];
        int na=I.getBase(), nb=I.getBound();

        if( option[dir]==restrictedFullWeighting )
	{
          // In this case we do not average in this direction -- this must be a boundary or ghost line
          assert( na==nb && Jv[dir].getBase()==Jv[dir].getBound() );
	  
          // *wdh* 2012/04/07 NOTE: na==nb here
          if( na<cFineLocal.getBase(dir+1) +numGhost[dir]-1 || 
              nb>cFineLocal.getBound(dir+1)-numGhost[dir]+1 )
	  {
	    ok=false;  // there are no pts to assign on this processor
	    break;
	  }
          continue;
	  
          // -- *wdh* 2012/04/07 OLD: this is not correct -- NOTE: na=nb 
// 	  na = max(na,cFineLocal.getBase(dir+1));
//           nb = min(nb,cFineLocal.getBound(dir+1));
// 	  if( na>nb ) 
// 	  {
// 	    ok=false;  // there are no pts to assign on this processor
// 	    break;
// 	  }
//        continue;

	}
	

        assert( ( na % 2 == 0) && ( nb % 2 == 0 ) );  // we assume this below

        int ma=na, mb=nb;  // original values

        // This seems to work but I am not exactly sure why: *wdh* 110422
	na=max(na,cFineLocal.getBase(dir+1) +numGhost[dir]-1);  // *note* dir+1 in cFineLocal since this is a coeff matrix
	nb=min(nb,cFineLocal.getBound(dir+1)-numGhost[dir]+1);  // *note* dir-1

        // *wdh* This is NOT correct but I am not exactly sure why: 
	//na=max(na,cFineLocal.getBase(dir+1) +(halfWidth+numExtraParallelGhost));  // *note* dir+1 in cFineLocal since this is a coeff matrix
	//nb=min(nb,cFineLocal.getBound(dir+1)-(halfWidth+numExtraParallelGhost));  // *note* dir-1

        // Old: 
	//na=max(na,cFineLocal.getBase(dir+1) +1);  // *note* dir+1 in cFineLocal since this is a coeff matrix
	//nb=min(nb,cFineLocal.getBound(dir+1)-1);  // *note* dir-1

	na+=(na-ma) % 2;  // na should remain even
	nb-=(mb-nb) % 2;  // nb should remain even

        if( na>nb ) 
	{
	  ok=false;  // there are no pts to assign on this processor
	  break;
	}
	
        I=Range(na,nb,I.getStride());
        Ivp[dir]=Range(na-1,nb+1);

        Index & J = Jv[dir];  // coarse grid points
	na/=2;  // for coarse grid
	nb/=2;
        // we assume the fine and coarse are distributed in the same way:
	assert( na>=J.getBase() && nb<=J.getBound() );  
	
        if( na<cCoarseLocal.getBase(dir+1) || nb>cCoarseLocal.getBound(dir+1) )
	{
	  printf("Ogmg::averageCoefficients:ERROR in computing coarse grid bounds:\n"
                 "  dir=%i, na=%i, nb=%i but cCoarseLocal=[%i,%i] \n",dir,na,nb,cCoarseLocal.getBase(dir+1),cCoarseLocal.getBound(dir+1));
	  OV_ABORT("error");
	}

        J=Range(na,nb);  // coarse grid points
      }
      if( !ok ) return 0; // no points to assign
      

    #else
      const realSerialArray & cFineLocal = cFine;
      realSerialArray & cCoarseLocal = cCoarse;
    #endif
    
    if( Ogmg::debug & 16 )
    {
      fPrintF(debugFile,"averageCoefficients: global: fine: I1,I2=[%i,%i][%i,%i] coarse: J1,J2=[%i,%i][%i,%i]\n",
              I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
              J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound());
      fPrintF(debugFile,"averageCoefficients: local: fine: I1,I2=[%i,%i][%i,%i] coarse: J1,J2=[%i,%i][%i,%i]\n",
              Iv[0].getBase(),Iv[0].getBound(),Iv[1].getBase(),Iv[1].getBound(),
              Jv[0].getBase(),Jv[0].getBound(),Jv[1].getBase(),Jv[1].getBound());

    }
      
    realSerialArray c0(ndc,Jv[0],Ivp[1],Ivp[2]), c1; // temporary space
    if( numberOfDimensions==3 )
      c1.redim(ndc,Jv[0],Jv[1],Ivp[2]);
      
    //    const int option=fullWeighting;
    // time=getCPU();
    int averageOption[3]={(int)option[0],(int)option[1],(int)option[2]}; //
    
    averageOpt( numberOfDimensions, 
                cFineLocal.getBase(1),cFineLocal.getBound(1),
                cFineLocal.getBase(2),cFineLocal.getBound(2),
                cFineLocal.getBase(3),cFineLocal.getBound(3),
                cCoarseLocal.getBase(1),cCoarseLocal.getBound(1),
                cCoarseLocal.getBase(2),cCoarseLocal.getBound(2),
                cCoarseLocal.getBase(3),cCoarseLocal.getBound(3),
		Jv[0].getBase(),Jv[0].getBound(),
                Jv[1].getBase(),Jv[1].getBound(),
                Jv[2].getBase(),Jv[2].getBound(),
		Iv[0].getBase(),Iv[0].getBound(),Iv[0].getStride(),
		Iv[1].getBase(),Iv[1].getBound(),Iv[1].getStride(),
		Iv[2].getBase(),Iv[2].getBound(),Iv[2].getStride(),
		Ivp[0].getBase(),Ivp[0].getBound(),
                Ivp[1].getBase(),Ivp[1].getBound(),
                Ivp[2].getBase(),Ivp[2].getBound(),
		ndc, 
                *getDataPointer(cFineLocal), 
                *getDataPointer(cCoarseLocal), 
		ndc, *getDataPointer(c0), *getDataPointer(c1),
		averageOption[0], orderOfAccuracy, ipar[0] );
    
//     time=getCPU()-time;
//     printF(">>>Time for averageOpt=%8.2e\n",time);

//     real err=max(fabs(cc-cCoarse));
//     printF("******  max(fabs(cc-cCoarse))=%e\n",err);

  }
  else
  {

    // ** option=injection;  

  // Average interior points using the full weighting operator
    if( numberOfDimensions==1 )
    {
      if( option[0]==fullWeighting )
      {
	cCoarse(0,J1,J2,J3)=FW0_1D_0(cFine,0,1,2,I1,I2,I3);
	cCoarse(1,J1,J2,J3)=FW0_1D_1(cFine,0,1,2,I1,I2,I3);
	cCoarse(2,J1,J2,J3)=FW0_1D_2(cFine,0,1,2,I1,I2,I3);
      }
      else
      {
	cCoarse(0,J1,J2,J3)=INJECTION_1D_0(cFine,0,1,2,I1,I2,I3);
	cCoarse(1,J1,J2,J3)=INJECTION_1D_1(cFine,0,1,2,I1,I2,I3);
	cCoarse(2,J1,J2,J3)=INJECTION_1D_2(cFine,0,1,2,I1,I2,I3);
      }
    }
    else if( numberOfDimensions==2 )
    {
      // const int width=3;
      // numbering of coefficients:
      //       6  7  8
      //       3  4  5
      //       0  1  2
      realSerialArray c0(27,J1,I2p,I3p);
      // first average in direction 0
      //  average coefficients [0 1 2] --> wide stencil
      //  average coefficients [3 4 5] --> wide stencil
      //  average coefficients [6 7 8] --> wide stencil
      int m;
      for( m=0; m<3; m++ )
      {
	int m0=3*m, m1=m0+1, m2=m0+2;
	if( option[0]==fullWeighting )
	{
	  c0(m0,J1,I2p,I3p)=FW0_1D_0(cFine,m0,m1,m2,I1,I2p,I3p);  // average coefficients m0,m1,m2 along axis1
	  c0(m1,J1,I2p,I3p)=FW0_1D_1(cFine,m0,m1,m2,I1,I2p,I3p);
	  c0(m2,J1,I2p,I3p)=FW0_1D_2(cFine,m0,m1,m2,I1,I2p,I3p);
	}
	else if( option[0]==restrictedFullWeighting )
	{
	  c0(m0,J1,I2p,I3p)=INJECTION_1D_0(cFine,m0,m1,m2,I1,I2p,I3p);  
	  c0(m1,J1,I2p,I3p)=INJECTION_1D_1(cFine,m0,m1,m2,I1,I2p,I3p);
	  c0(m2,J1,I2p,I3p)=INJECTION_1D_2(cFine,m0,m1,m2,I1,I2p,I3p);
	}
	else if( option[0]==injection )
	{
	  c0(m0,J1,I2p,I3p)=cFine(m0,I1,I2p,I3p);  
	  c0(m1,J1,I2p,I3p)=cFine(m1,I1,I2p,I3p);
	  c0(m2,J1,I2p,I3p)=cFine(m2,I1,I2p,I3p);
	}
//          else if( option[0]==lumpedPartialWeighting  )
//  	{
//            if( axis!=0 )
//  	  {
//  	    c0(m0,J1,I2p,I3p)=0.;
//  	    c0(m1,J1,I2p,I3p)=(FW0_1D_0(cFine,m0,m1,m2,I1,I2p,I3p)+
//  			       FW0_1D_1(cFine,m0,m1,m2,I1,I2p,I3p)+
//  			       FW0_1D_2(cFine,m0,m1,m2,I1,I2p,I3p));
//  	    c0(m2,J1,I2p,I3p)=0.;
//  	  }
//  	  else
//  	  {
//  	    c0(m0,J1,I2p,I3p)=0.;
//  	    c0(m1,J1,I2p,I3p)=(INJECTION_1D_0(cFine,m0,m1,m2,I1,I2p,I3p)+
//  			       INJECTION_1D_1(cFine,m0,m1,m2,I1,I2p,I3p)+
//  			       INJECTION_1D_2(cFine,m0,m1,m2,I1,I2p,I3p));
//  	    c0(m2,J1,I2p,I3p)=0.;
//  	  }
//  	}
	else
	{
	  Overture::abort("error");
	}
      }
      // display(c0,"c0","%9.1e");

    // now average in direction 1
      for( m=0; m<3; m++ )
      {
	int m0=m, m1=m0+3, m2=m0+6;
	if( option[1]==fullWeighting )
	{
	  cCoarse(m0,J1,J2,I3p)=FW1_1D_0(c0,m0,m1,m2,J1,I2,I3p); 
	  cCoarse(m1,J1,J2,I3p)=FW1_1D_1(c0,m0,m1,m2,J1,I2,I3p); 
	  cCoarse(m2,J1,J2,I3p)=FW1_1D_2(c0,m0,m1,m2,J1,I2,I3p); 
	}
	else if( option[1]==restrictedFullWeighting )
	{
	    cCoarse(m0,J1,J2,I3p)=INJECTION_1D_0(c0,m0,m1,m2,J1,I2,I3p); 
	    cCoarse(m1,J1,J2,I3p)=INJECTION_1D_1(c0,m0,m1,m2,J1,I2,I3p); 
	    cCoarse(m2,J1,J2,I3p)=INJECTION_1D_2(c0,m0,m1,m2,J1,I2,I3p); 
	}
	else if( option[1]==injection )
	{
	  cCoarse(m0,J1,J2,I3p)=c0(m0,J1,I2,I3p);
	  cCoarse(m1,J1,J2,I3p)=c0(m1,J1,I2,I3p);
	  cCoarse(m2,J1,J2,I3p)=c0(m2,J1,I2,I3p);
	}
//          else if( option[1]==lumpedPartialWeighting )
//  	{
//            if( axis!=1 )
//  	  {
//  	    cCoarse(m0,J1,J2,I3p)=0.;
//  	    cCoarse(m1,J1,J2,I3p)=(FW1_1D_0(c0,m0,m1,m2,J1,I2,I3p)+
//  				   FW1_1D_1(c0,m0,m1,m2,J1,I2,I3p)+
//  				   FW1_1D_2(c0,m0,m1,m2,J1,I2,I3p)); 
//  	    cCoarse(m2,J1,J2,I3p)=0.;
//  	  }
//  	  else
//  	  {
//  	    cCoarse(m0,J1,J2,I3p)=0.;
//  	    cCoarse(m1,J1,J2,I3p)=(INJECTION_1D_0(c0,m0,m1,m2,J1,I2,I3p)+
//  				   INJECTION_1D_1(c0,m0,m1,m2,J1,I2,I3p)+
//  				   INJECTION_1D_2(c0,m0,m1,m2,J1,I2,I3p)); 
//  	    cCoarse(m2,J1,J2,I3p)=0.;
//  	  }
//  	}
	else
	{
	  Overture::abort("error");
	}
      }
    }
    else if( numberOfDimensions==3 )
    {
      // const int width=3;

      realSerialArray c0(27,J1,I2p,I3p);
      // first average in direction 0
      // 0 1 2
      // 3 4 5
      // 6 7 8
      // 9 10 11
      // ...
      int m;
      for( m=0; m<9; m++ )
      {
	int m0=3*m, m1=m0+1, m2=m0+2;
	if( option[0]==fullWeighting )
	{
	  c0(m0,J1,I2p,I3p)=FW0_1D_0(cFine,m0,m1,m2,I1,I2p,I3p);  // average coefficients m0,m1,m2 along axis1
	  c0(m1,J1,I2p,I3p)=FW0_1D_1(cFine,m0,m1,m2,I1,I2p,I3p);
	  c0(m2,J1,I2p,I3p)=FW0_1D_2(cFine,m0,m1,m2,I1,I2p,I3p);
	}
	else if( option[0]==restrictedFullWeighting )
	{
	  c0(m0,J1,I2p,I3p)=INJECTION_1D_0(cFine,m0,m1,m2,I1,I2p,I3p);  
	  c0(m1,J1,I2p,I3p)=INJECTION_1D_1(cFine,m0,m1,m2,I1,I2p,I3p);
	  c0(m2,J1,I2p,I3p)=INJECTION_1D_2(cFine,m0,m1,m2,I1,I2p,I3p);
	}
	else
	{
	  Overture::abort("error");
	}
        
      }
      // display(c0,"c0","%9.1e");

    // now average in direction 1
    // 0 3 6
    // 1 4 7
    // 2 5 8
    // 9 12 15
    // 10 13 16
    // 11 14 17
    // 18
    // 19
    // 20 23 26 
      realSerialArray c1(27,J1,J2,I3p);
      for( m=0; m<9; m++ )
      {
	int m0=m+(m/3)*6, m1=m0+3, m2=m0+6;
	if( option[1]==fullWeighting )
	{
	  c1(m0,J1,J2,I3p)=FW1_1D_0(c0,m0,m1,m2,J1,I2,I3p); 
	  c1(m1,J1,J2,I3p)=FW1_1D_1(c0,m0,m1,m2,J1,I2,I3p); 
	  c1(m2,J1,J2,I3p)=FW1_1D_2(c0,m0,m1,m2,J1,I2,I3p); 
	}
	else if( option[1]==restrictedFullWeighting )
	{
	  c1(m0,J1,J2,I3p)=INJECTION_1D_0(c0,m0,m1,m2,J1,I2,I3p); 
	  c1(m1,J1,J2,I3p)=INJECTION_1D_1(c0,m0,m1,m2,J1,I2,I3p); 
	  c1(m2,J1,J2,I3p)=INJECTION_1D_2(c0,m0,m1,m2,J1,I2,I3p); 
	}
	else
	{
	  Overture::abort("error");
	}
      }
    
      // now average in direction 2
      // 0  9 18
      // 1 10 19
      // 2 11 20
      // 3 12 21
      for( m=0; m<9; m++ )
      {
	int m0=m,   m1=m0+9, m2=m0+18;
	if( option[2]==fullWeighting )
	{
	  cCoarse(m0,J1,J2,J3)=FW2_1D_0(c1,m0,m1,m2,J1,J2,I3); 
	  cCoarse(m1,J1,J2,J3)=FW2_1D_1(c1,m0,m1,m2,J1,J2,I3); 
	  cCoarse(m2,J1,J2,J3)=FW2_1D_2(c1,m0,m1,m2,J1,J2,I3); 
	}
	else if( option[2]==restrictedFullWeighting )
	{
	  cCoarse(m0,J1,J2,J3)=INJECTION_1D_0(c1,m0,m1,m2,J1,J2,I3); 
	  cCoarse(m1,J1,J2,J3)=INJECTION_1D_1(c1,m0,m1,m2,J1,J2,I3); 
	  cCoarse(m2,J1,J2,J3)=INJECTION_1D_2(c1,m0,m1,m2,J1,J2,I3); 
	}
	else
	{
	  Overture::abort("error");
	}
      }

    }
    else
    {
      Overture::abort();
    }
    
  }
  
    return 0;
}




#undef ForBoundary

