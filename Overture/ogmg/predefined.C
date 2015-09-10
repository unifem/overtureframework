#include "Ogmg.h"
#include "SparseRep.h"
#include "ParallelUtility.h"

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )


//! Specify the (predefined) equation to solve.  **this is not finished yet **
/*!
  \param bc(0:1,0:2,numberOfComponentGrids) (input): boundary conditions, Ogmg::dirichlet, neumann or mixed.
  \param bcData(0:1,0:1,0:2,numberOfComponentGrids) (input): For a neumann BC, a(0:1)=bcData(0:1,side,axis,grid) 
     are the coefficients of u and du/dn :  a(0)*u + a(1)*u.n   
  \param constantCoeff (input) : For equation\_==heatEquationOperator we solve
        constantCoeff(0,grid)*I + constantCoeff(1,grid)*Laplacian

  Notes: updateToMatchGrid should be called before this function. It is assumed that the extra grid levels
         have already been generated. When calling MG through Oges, the MultigridEquationSolver: function
         setEquationAndBoundaryConditions will call Ogmg::updateToMatchGrid before calling this function.

 */

int Ogmg::
setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation_, 
                                  CompositeGridOperators & op,
                                  const IntegerArray & bc_,
				  const RealArray & bcData,
                                  const RealArray & constantCoeff,
				  realCompositeGridFunction *variableCoeff /* =NULL */ )
{
  equationToSolve=equation_;

//    if( true )
//    {
//      printf("+++++ Ogmg:: setEquationAndBC: &mgcg[0].vertex() = %i \n",&mgcg[0].vertex());
//      printf("+++++ Ogmg:: setEquationAndBC: &mgcg.multigridLevel[0][0].vertex() = %i \n",
//                     &mgcg.multigridLevel[0][0].vertex());
//      Range all;
//      display(mgcg[0].vertex()(all,all,all,0),"  +++++ mgcg[0].vertex()","%7.4f ");

//      if( cgGlobal!=NULL )
//        display( (*cgGlobal)[0].vertex()(all,all,all,0),"  +++++ (*cgGlobal)[0].vertex()","%7.4f ");
//    }
  

  int grid,level=0;
  
  CompositeGrid & mgcg = multigridCompositeGrid();
  const int numberOfComponentGrids=mgcg.numberOfComponentGrids();

  // ********************************************************
  // ***** Some equations use the variableCoeff argument ****
  // ********************************************************
  if( equationToSolve==OgesParameters::divScalarGradOperator ||
      equationToSolve==OgesParameters::variableHeatEquationOperator ||
      equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
  {
    if( variableCoeff==NULL )
    {
      Overture::abort(" Ogmg::setEquationAndBoundaryConditions:ERROR: This equation requires variableCoeff");
    }
    // Build the grid function of variable coefficients
    // Make a reference to the input variableCoeff,
    // we will average the coefficients on coarser levels.
    if( varCoeff==NULL )
      varCoeff=new realCompositeGridFunction(mgcg);

    realCompositeGridFunction & var=*varCoeff;
    for( grid=0; grid<numberOfComponentGrids; grid++ )
    {
      var[grid].reference((*variableCoeff)[grid]);
      var.multigridLevel[0][grid].reference((*variableCoeff)[grid]);  // need to do this too
    }

    
  }

  equationCoefficients.redim(0);
  equationCoefficients=constantCoeff;
  if( equationToSolve==OgesParameters::heatEquationOperator )
  {
    assert( equationCoefficients.getLength(0)>=2 && 
	    equationCoefficients.getLength(1)>=mgcg.numberOfComponentGrids() );
  }
  
  for( grid=0; grid<numberOfComponentGrids; grid++ )
  {
    active(grid)=true;
    if( equationToSolve==OgesParameters::heatEquationOperator )
    {
      if( fabs(equationCoefficients(0,grid)-1.)<REAL_EPSILON*10. && equationCoefficients(1,grid)==0. )
      {
	if( Ogmg::debug & 2 )
	  printF("Ogmg:INFO: solving for the identity operator on grid %i (%s)\n",grid,
		 (const char*) mgcg[grid].getName());
	active(grid)=false;
      }
    }
      

  }
  
  setBoundaryConditions(bc_,bcData);
  
  return buildPredefinedEquations(op);
}


//! build the predefined equations
int Ogmg::
buildPredefinedEquations(CompositeGridOperators & cgop)
// ============================================================================================
// ============================================================================================
{
  real time0=getCPU();

  const int width = orderOfAccuracy+1;  // 3 or 5
  const int numberOfGhostLines=(width-1)/2;
  assert( numberOfGhostLines==1 || numberOfGhostLines==2 );

  Range all;
  CompositeGrid & mgcg = multigridCompositeGrid();
  const int stencilSize=int(pow(width,mgcg.numberOfDimensions())+1);
  int grid;


  // dataAllocationOption==2 : do not allocate on rectangular grids, except the coarsest level
  //                     ==1 : do not allocate on rectangular grids
  int dataAllocationOption = parameters.useDirectSolverOnCoarseGrid ? 2 : 1;

  if( numberOfExtraLevels==0 && parameters.useDirectSolverForOneLevel )
  {
    dataAllocationOption=0;  // only one level -- we need to form the coeff matrix 
  }
  // printf(" *********** dataAllocationOption=%i \n",dataAllocationOption);
  

  // ******** this is needed for some reason or updateToMatchGrid will fail is this function is called twice
  cMG.destroy(); 
  

  cMG.setDataAllocationOption(dataAllocationOption);  
  // cMG.display("cMG");

  // printf(" ***BEFORE: cMG.sizeOf()=%12.0f\n",cMG.sizeOf());

  cMG.updateToMatchGrid(mgcg,stencilSize,all,all,all);

//  printf(">>>>>buildPredefinedEquations: mgcg.numberOfMultigridLevels()=%i\n",mgcg.numberOfMultigridLevels());

//    if( true ) 
//    {
//      cMG=0.;
//      cMG.display("cMG");
//      cMG.multigridLevel[0].display("cMG.multigridLevel[0]");
//    }
  
  
//    if( true )
//    {
//      for( int l=0; l<numberOfExtraLevels; l++ )
//      {
//        int level=l;

//        // we need operators to apply boundary conditions.
//        RealCompositeGridFunction & cl = cMG.multigridLevel[level+1];
//        CompositeGrid & cgl = *cl.getCompositeGrid();
//        printf(">>>>>buildPredefinedEquations: level+1=%i cgl.rcData=%i \n",level+1,cgl.rcData);
      
//      }
//    }
  // printf(" ***AFTER: cMG.sizeOf()=%12.0f\n",cMG.sizeOf());
  //  cMG.display("cMG");

  cMG.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines); 
  // printf(">>>: 1-> cMG.getIsACoefficientMatrix()=%i\n",cMG.getIsACoefficientMatrix());
  cgop.setStencilSize(stencilSize);
  cgop.setOrderOfAccuracy(orderOfAccuracy);

  realCompositeGridFunction & coeff = cMG.multigridLevel[0];

  // printf(">>>: cMG.getIsACoefficientMatrix()=%i\n",cMG.getIsACoefficientMatrix());
  // printf(">>>: cMG.multigridLevel[0].getIsACoefficientMatrix()=%i\n",cMG.multigridLevel[0].getIsACoefficientMatrix());

  if( numberOfExtraLevels==0 )
    cMG.setOperators(cgop);
  else
    coeff.setOperators(cgop);
  for( grid=0; grid<coeff.numberOfComponentGrids(); grid++ )
    cMG[grid].setOperators(*coeff[grid].getOperators());

  // build equations on level 0 but do not build on rectangular grids.
  bool buildRectanguar=false, buildCurvilinear=true;

  if( numberOfExtraLevels==0 && parameters.useDirectSolverForOneLevel )
  {
    buildRectanguar=true;
    initializeConstantCoefficients();    // we need this here -- also done in buildCoefficientArrays() below
  }
  
  buildPredefinedCoefficientMatrix( 0,buildRectanguar,buildCurvilinear );

  tm[timeForBuildPredefinedEquations]=getCPU()-time0;
  tm[timeForInitialize]+=tm[timeForBuildPredefinedEquations];

  buildCoefficientArrays();

  return 0;
}


// ========================================================================================================
//! Build the coefficient matrix for the predefined equations on a given level
/*!
    \param buildRectangular (input) : if true build coeff matrices for curvilinear grids.
             If true build matrices for rectangular ONLY. This is normally
                only done on the coarsest level if we have to call a direct solver.
    \param buildCurvilinear (input) if true, build coefficients for curvilinear grids.
 */
// =======================================================================================================
int Ogmg::
buildPredefinedCoefficientMatrix( int level, bool buildRectangular, bool buildCurvilinear )
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  assert( level>=0 && level<mgcg.numberOfMultigridLevels() );
  
  if( debug & 8 )
    printF(" **** buildPredefinedCoefficientMatrix: level=%i buildRectangular=%i buildCurvilinear=%i\n",
        level,buildRectangular,buildCurvilinear);

  realCompositeGridFunction & coefficients = level==0 ? cMG : cMG.multigridLevel[level];
  CompositeGrid & cg = level==0 ? mgcg : mgcg.multigridLevel[level];
  Range all;
  Index I1,I2,I3;
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], J3=Jv[2];
  Index Ig1,Ig2,Ig3;

  const int width = orderOfAccuracy+1;  // 3 or 5
  const int stencilSize=int(pow(width,mgcg.numberOfDimensions())+1);
  
  int md; // diagonal term
  if( cg.numberOfDimensions()==2 )
    md=(width*width)/2; // 4 or 12 ;
  else if( cg.numberOfDimensions()==3 )
    md=(width*width*width)/2; // 13 or 62;
  else
    md=width/2; // 1

//    int m222; // diagonal term
//    if( cg.numberOfDimensions()==2 )
//      m222=4;
//    else if( cg.numberOfDimensions()==3 )
//      m222=13;
//    else
//      m222=1;

  // printf("***: coefficients.getIsACoefficientMatrix()=%i\n",coefficients.getIsACoefficientMatrix());

  for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    
    // we either build the coeff for all grids, just curvilinear grids, or just rectangular grids.
    // *** for now operator averaging does not apply to fourth order grids so we build curvilinear grids too
    bool buildThisGrid = ( mg.isRectangular() && buildRectangular) ||  
                         (!mg.isRectangular() && buildCurvilinear);
    if( !buildThisGrid )
      continue;

    realMappedGridFunction & coeff = coefficients[grid];

    intArray & maskd = mg.mask();
    #ifdef USE_PPP
      realSerialArray coeffa; getLocalArrayWithGhostBoundaries(coeff,coeffa);
      intSerialArray mask;  getLocalArrayWithGhostBoundaries(maskd,mask);
    #else
      intSerialArray & mask = maskd;
      realArray & coeffa = coeff;
    #endif


    MappedGridOperators & op = * coeff.getOperators();
    op.setStencilSize(stencilSize);
    op.setOrderOfAccuracy(orderOfAccuracy);

    const bool isRectangular=mg.isRectangular();
    
//      if( true )
//      {
//        // *****
//        coeff.display("coeff in buildPredefinedCoefficientMatrix");
//      }
    

    coeffa=0; // **

    if( isRectangular &&
	(equationToSolve==OgesParameters::laplaceEquation || 
	 equationToSolve==OgesParameters::heatEquationOperator) )
    {
      // special case for rectangular grids and constant coefficient equations
      const RealArray & cc = constantCoefficients(all,grid,level);  

      getIndex(mg.gridIndexRange(),I1,I2,I3);

      const int stencilSize=coeffa.getLength(0)-1;

      #ifdef USE_PPP
        int includeGhost=1;  // include ghost 
        bool ok = ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3,includeGhost);
        if( !ok ) continue;
	
//         const int n1a = max(I1.getBase() ,mask.getBase(0) +maskd.getGhostBoundaryWidth(0));
//         const int n1b = min(I1.getBound(),mask.getBound(0)-maskd.getGhostBoundaryWidth(0));
//         const int n2a = max(I2.getBase() ,mask.getBase(1) +maskd.getGhostBoundaryWidth(1));
//         const int n2b = min(I2.getBound(),mask.getBound(1)-maskd.getGhostBoundaryWidth(1));
//         const int n3a = max(I3.getBase() ,mask.getBase(2) +maskd.getGhostBoundaryWidth(2));
//         const int n3b = min(I3.getBound(),mask.getBound(2)-maskd.getGhostBoundaryWidth(2));
        
//         if( n1a>n1b || n2a>n2b || n3a>n3b ) continue;

// 	I1=Range(n1a,n1b);
// 	I2=Range(n2a,n2b);
// 	I3=Range(n3a,n3b);
	
      #endif

      real * coeffap = coeffa.Array_Descriptor.Array_View_Pointer3;
      const int coeffaDim0=coeffa.getRawDataSize(0);
      const int coeffaDim1=coeffa.getRawDataSize(1);
      const int coeffaDim2=coeffa.getRawDataSize(2);
#define COEFFA(i0,i1,i2,i3) coeffap[i0+coeffaDim0*(i1+coeffaDim1*(i2+coeffaDim2*(i3)))]	
      const real *ccp = cc.Array_Descriptor.Array_View_Pointer0;
#define CONC(i0) ccp[i0]

      int I1Base,I2Base,I3Base;
      int I1Bound,I2Bound,I3Bound;
      int i1,i2,i3;
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	for( int m=0; m<stencilSize; m++ )
	  COEFFA(m,i1,i2,i3)=CONC(m);
      }

    
    }
    else if( equationToSolve==OgesParameters::laplaceEquation )
    {
      // printf("***before assign to laplace: coeff.getIsACoefficientMatrix()=%i\n",coeff.getIsACoefficientMatrix());

      op.coefficients(MappedGridOperators::laplacianOperator,coeff); // efficient version

      // printf("***after  assign to laplace: coeff.getIsACoefficientMatrix()=%i\n",coeff.getIsACoefficientMatrix());

    }
    else if( equationToSolve==OgesParameters::heatEquationOperator )
    {
      op.coefficients(MappedGridOperators::laplacianOperator,coeff); // efficient version

      assert( equationCoefficients.getLength(0)>=2 && 
              equationCoefficients.getLength(1)>=mgcg.numberOfComponentGrids() );
      real cI=equationCoefficients(0,grid);
      real cLap=equationCoefficients(1,grid);
      assert( fabs(cI)+fabs(cLap) > 0. );

      coeffa*=cLap;

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      int includeGhost=1;  // include ghost 
      bool ok = ParallelUtility::getLocalArrayBounds(maskd,mask,I1,I2,I3,includeGhost);
      if( !ok ) continue;

      coeffa(md,I1,I2,I3)+=cI;

//  	realArray identity;  // ***** fix this : avoid allocating an array ***** write a loop
//  	identity.redim(coeff);
//  	op.assignCoefficients(MappedGridOperators::identityOperator,identity);
	
//  	identity*=cI;
//  	coeff+=identity;
    }
    else if( equationToSolve==OgesParameters::divScalarGradOperator )
    {
      assert( varCoeff!=NULL );
      realMappedGridFunction & variableCoeff = (*varCoeff).multigridLevel[level][grid];
      op.coefficients(MappedGridOperators::divergenceScalarGradient,coeff,variableCoeff);
    }
    else if( equationToSolve==OgesParameters::variableHeatEquationOperator )
    {
      // I + s(x)*Delta
      assert( varCoeff!=NULL );
      realMappedGridFunction & variableCoeff = (*varCoeff).multigridLevel[level][grid];
      const realArray & var = variableCoeff;

      op.coefficients(MappedGridOperators::laplacianOperator,coeff,variableCoeff ); 

      multiply(coeff,variableCoeff);
//          getIndex(mg.gridIndexRange(),I1,I2,I3);
//  	const int stencilSize=coeffa.getLength(0)-1;
//  	for( int m=0; m<stencilSize; m++ )
//  	  coeffa(m,I1,I2,I3)*=var(I1,I2,I3);
	
      coeffa(md,I1,I2,I3)+=1;  // add the Identity.

    }
    else if( equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
    {
      // I + div( s(x) grad )
      assert( varCoeff!=NULL );
      realMappedGridFunction & variableCoeff = (*varCoeff).multigridLevel[level][grid];
      op.coefficients(MappedGridOperators::divergenceScalarGradient,coeff,variableCoeff);

      coeffa(md,all,all,all)+=1; // add the Identity.

    }
    else
    {
      printf("Ogmg::buildPredefinedCoefficientMatrix:ERROR: unknown equationToSolve=%i\n",
	     (int)equationToSolve);
      Overture::abort();
    }
    
    if( isRectangular && level<mgcg.numberOfMultigridLevels()-1 )
    {
      // Rectangular grid but not the coarsest level,
      // no need to fill in the BC's in this case
      continue;
    }

    // ***** we may not need to fill in BC's for predefined equations in some cases ****
    assignBoundaryConditionCoefficients( coeff, grid, level );

    // updateGhostBoundaries is now performed in finishBoundaryConditions but I guess we do not call that here

    coeff.updateGhostBoundaries();  // *wdh* 091217 -- need values on parallel ghost for operator averaging


//     else // **** old way ***
//     {
      

//       // display(coeffa.boundaryCondition(),"mgCoarse.boundaryConditions");
//       RealArray & a = bcParams.a;
//       a.redim(2);

//       const int orderOfExtrapolation= orderOfAccuracy==2 ? 3 : 4;  // 5 **** use extrap order 4 for 4th order
    
//       BoundaryConditionParameters extrapParams;
//       extrapParams.orderOfExtrapolation=orderOfExtrapolation; // orderOfAccuracy+1; 

// //      if( orderOfAccuracy==4 )
// //      {  
// //        // **** do this for now **** fix for Neumann.
// //        // extrap 2nd ghost line 
// //        extrapParams.ghostLineToAssign=2;
// //        coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,extrapParams); 
// //        extrapParams.ghostLineToAssign=1;
	
// //      }

//     // const IntegerArray & bc = mg.boundaryCondition();
//       int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
//       for( int axis=0; axis<mgcg.numberOfDimensions(); axis++ )
//       {
// 	for( int side=0; side<=1; side++ )
// 	{
// 	  if( mg.boundaryCondition(side,axis)<=0 )
// 	    continue;
	
	
// 	  if( bc(side,axis,grid)==OgmgParameters::dirichlet )
// 	  {
// //            if( orderOfAccuracy==2 || 
// //  	      // *wdh* 030521 level==(mgcg.numberOfMultigridLevels()-1) ||
// //                 parameters.fourthOrderBoundaryConditionOption==0 ||
// //                 (orderOfAccuracy==4 && level!=0 && !parameters.useEquationForDirichletOnLowerLevels) )

// 	    bool useEquationOnGhost = useEquationOnGhostLineForDirichletBC(mg,level);

// 	    if( orderOfAccuracy==2 || !useEquationOnGhost )
// 	    {
// 	      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,
// 						       extrapParams);
// 	    }
// 	    else
// 	    {
// 	      // === Use the equation to 2nd order on the boundary ===

// 	      if( equationToSolve==OgesParameters::laplaceEquation )
// 	      {
// 		getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
// 		op.setOrderOfAccuracy(2);

// 		// ***this is wrong*** --> corners
// 		// ** op.coefficients(MappedGridOperators::laplacianOperator,coeff,I1,I2,I3); // efficient version

           
// 		// ***** 030606: to fix: what if there are interp points on the edge --> we need to extrap points
// 		// adjacent to them *****


// 		realArray tempCoeff(coeff.dimension(0),I1,I2,I3);
// 		op.assignCoefficients(MappedGridOperators::laplacianOperator,tempCoeff,I1,I2,I3); // efficient version
// 		op.setOrderOfAccuracy(4);
// 		getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);

              
// 		const int m3b= mg.numberOfDimensions()==2 ? -1 : 1;
// 		const int ee=0; 
// 		int I1Base,I2Base,I3Base;
// 		int I1Bound,I2Bound,I3Bound;
// 		int i1,i2,i3;
// 		is1=is2=is3=0;
// 		isv[axis]=1-2*side;
	      
// 		for( int m3=-1; m3<=m3b; m3++ )
// 		  for( int m2=-1; m2<=1; m2++ )
// 		    for( int m1=-1; m1<=1; m1++ )
// 		    {
// 		      // copy the second order equation into the correct positions of the 4th order stencil
// 		      const int index2=(m1+1)+3*(m2+1+3*(m3+1));  // stencil width=3

// 		      int index4=mg.numberOfDimensions()==2 ? (m1+2)+5*(m2+2)          :        // stencil width==4
// 			(m1+2)+5*(m2+2+5*(m3+2));
// 		      // remember we are shifted to the ghost line :
// 		      index4 += axis==0 ? 1-2*side : axis==1 ? 5*(1-2*side) : 25*(1-2*side);
		
// 		      FOR_3(i1,i2,i3,I1,I2,I3)
// 		      {
// 			int ig1=i1-is1, ig2=i2-is2, ig3=i3-is3;
// 			coeff(index4,ig1,ig2,ig3)=tempCoeff(index2,i1,i2,i3);
// 			coeff.sparse->setClassify(SparseRepForMGF::ghost1,ig1,ig2,ig3,ee);
// 		      }
		
// 		    }

// 		if( true )
// 		{
// 		  printF("\n>>>>>>>>>>>>>buildPredefinedCoefficientMatrix: \n"
// 			 " fill in 2nd-order equation on the ghost points : level=%i, grid=%i side=%i "
// 			 "axis=%i\n",level,grid,side,axis);
// 		  // coeff(all,Ig1,Ig2,Ig3).display("Eqn to 2nd order on the boundary");
// 		}
	      
// 		// **** end points on dirichlet sides: use extrapolation (otherwise the same eqn appears twice!)

// 		for( int dir=0; dir<mg.numberOfDimensions()-1; dir++ )
// 		{
// 		  const int axisp = (axis+dir+1) % mg.numberOfDimensions(); // adjacent side
// 		  for( int side2=0; side2<=1; side2++ )
// 		  {
// 		    if( bc(side2,axisp,grid)==OgmgParameters::dirichlet )
// 		    {
// 		      if( mg.boundaryCondition(side2,axisp)<=0 )
// 		      {
// 			display(bc,"bc");
// 			display(mg.boundaryCondition(),"mg.boundaryCondition()");
// 			Overture::abort("Unepected Error");
// 		      }
		    
// 		      J1=Ig1, J2=Ig2, J3=Ig3;
// 		      Jv[axisp]= side2==0 ? Jv[axisp].getBase() : Jv[axisp].getBound();
// 		      // extrapolate points  coeff(.,J1,J2,J3) in the direction axis
// 		      op.setExtrapolationCoefficients(coeff,ee,J1,J2,J3,orderOfExtrapolation); // in GenericMGOP
// 		    }
// 		  }
// 		}
	      

// 	      }
// 	      else
// 	      {
// 		Overture::abort();
// 	      }
	    
// 	    }

// 	    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::boundary1+side+2*axis);
// 	    if( orderOfAccuracy==4 )
// 	    {
// 	      extrapParams.ghostLineToAssign=2;
// 	      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,
// 						       extrapParams); 
// 	      extrapParams.ghostLineToAssign=1;
// 	    }
// 	  }
// 	  else if( bc(side,axis,grid)==OgmgParameters::neumann )
// 	  {
// 	    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,BCTypes::boundary1+side+2*axis);

// 	    if( orderOfAccuracy==4 )
// 	    {
// 	      bool useEquationOnGhost = useEquationOnGhostLineForNeumannBC(mg,level);
// 	      if( useEquationOnGhost )
// 	      {
// 		printf(" ******buildPredefinedCoefficientMatrix: WARNING: not using eqn on ghost for neumann BC"
// 		       " at level =%i ******\n",level);


// 	      }

// 	      extrapParams.ghostLineToAssign=2;
// 	      if( false )
// 	      {
// 		coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,
// 							 extrapParams); 
// 	      }
// 	      else
// 	      {

// 		coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,BCTypes::boundary1+side+2*axis,
// 							 extrapParams); 
// 	      }
// 	    }
	  
// 	    extrapParams.ghostLineToAssign=1;

// 	    // getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);

// 	    // Range all;
// 	    // display(coeff(all,I1,I2,I3),"coeff on ghost line after adding a neumann BC",debugFile,"%8.2e ");
	  
// 	  }
// 	  else if( bc(side,axis,grid)==OgmgParameters::mixed )
// 	  {
// 	    a(0)=boundaryConditionData(0,side,axis,grid);  // coeff of u
// 	    a(1)=boundaryConditionData(1,side,axis,grid);  // coeff of du/dn
// 	    // printf(" predefined: mixed BC: a0=%e a1=%e \n",a(0),a(1));
	  
// 	    coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::mixed,BCTypes::boundary1+side+2*axis,bcParams);

// 	    if( orderOfAccuracy==4 )
// 	    {
// 	      bool useEquationOnGhost = useEquationOnGhostLineForNeumannBC(mg,level);
// 	      if( useEquationOnGhost )
// 	      {
// 		// *** fix this *** use mixedToSecondOrder instead of the neumann condition above and the symmetry below

// 		printf(" ******buildPredefinedCoefficientMatrix: WARNING: not using eqn on ghost for mixed BC"
// 		       " at level =%i ******\n",level);
// 	      }

// 	      extrapParams.ghostLineToAssign=2;
// 	      if( level==0 || !parameters.useSymmetryForNeumannOnLowerLevels ) 
// 	      {
// 		extrapParams.orderOfExtrapolation=4;
// 		coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::boundary1+side+2*axis,
// 							 extrapParams); 
// 	      }
// 	      else
// 	      {
// 		coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::evenSymmetry,BCTypes::boundary1+side+2*axis,
// 							 extrapParams); 
// 	      }
// 	    }
// 	    extrapParams.ghostLineToAssign=1;
	  
// 	  }
// 	  else if( bc(side,axis,grid)>0 )
// 	  {
// 	    printf("Ogmg::buildPredefinedCoefficientMatrix:ERROR: unknown bc=%i for grid=%i side=%i axis=%i\n",
// 		   bc(side,axis,grid),grid,side,axis);
// 	    throw "error";
// 	  }
// 	}
//       }
//     } // end if old way 
    
    
    if( (Ogmg::debug & 64 && ( level==mgcg.numberOfMultigridLevels()-1)) )
      displayCoeff(coeff,sPrintF(buff,"buildPredefinedCoefficientMatrix:coeff on coarsest level=%i "
			    "orderOfAccuracy=%i",level,orderOfAccuracy),debugFile,"%8.2e ");
    

  } // end for grid
  
  
  return 0;
}


int Ogmg::
buildPredefinedVariableCoefficients( RealCompositeGridFunction & coeff, const int level )
// ===================================================================================================
// /Description:
//    Average variable coefficients on level+1 from those on level
//
//    For predefined equations with variable coefficients we need to compute the averaged coefficients
// for the coarse levels.
// ===================================================================================================
{
  // *** Average the variable coeff to coarser levels ***
  CompositeGrid & mgcg = multigridCompositeGrid();
  printf("Ogmg:INFO: average variable coefficients to the coarser grids on level=%i...\n",level+1);
    
  Range all;
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Interpolate::InterpolateOptionEnum interpOption=  mgcg.numberOfDimensions()==1 ? Interpolate::fullWeighting100 :
    mgcg.numberOfDimensions()==2 ? Interpolate::fullWeighting110 : Interpolate::fullWeighting111;

  assert( level>=0 && level<mgcg.numberOfMultigridLevels()-1 );

  CompositeGridOperators & op = *( coeff.multigridLevel[level+1].getOperators() );
  
  assert( varCoeff!=NULL );
  realCompositeGridFunction & varFine   =(*varCoeff).multigridLevel[level];
  realCompositeGridFunction & varCoarse =(*varCoeff).multigridLevel[level+1];

  int grid;
  for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mgCoarse = mgcg.multigridLevel[level+1][grid];
	
    const IntegerArray & ratio = mgcg.multigridCoarseningRatio(all,grid,level+1);
    getIndex(mgCoarse.gridIndexRange(),I1,I2,I3); 
    varCoarse[grid]=1.;
    int update=0; 
    interp.interpolateCoarseFromFine(varCoarse[grid],Iv,varFine[grid], ratio,interpOption,update);
        
  }

  // *** fix boundaries ***
//    BoundaryConditionParameters extrapParams;
//    extrapParams.orderOfExtrapolation=2;
  
  op.applyBoundaryCondition(varCoarse,0,BCTypes::extrapolate,BCTypes::allBoundaries);
  varCoarse.interpolate();
  op.finishBoundaryConditions(varCoarse);
  
  if( Ogmg::debug & 32 )
    varCoarse.display(sPrintF(buff,"variables coefficients averaged to level %i\n",level+1),"%8.2e ");
  
  return 0;
}


int Ogmg::
initializeConstantCoefficients()
// ==========================================================================================
// /Description:
//    Determine whether the elements in the matrix are constant in which case we 
//  can use an optimized defect computation.
// ==========================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();



  if( equationToSolve==OgesParameters::laplaceEquation || equationToSolve==OgesParameters::heatEquationOperator )
  {
    // **new way ***
    // Form the operator cI*I + cLap*Delta

    real cI=0.;   
    real cLap=1.;
    if( equationToSolve==OgesParameters::heatEquationOperator )
    {
      assert( equationCoefficients.getLength(0)>=2 && 
              equationCoefficients.getLength(1)>=mgcg.numberOfComponentGrids() );
    }
    
    const int width = orderOfAccuracy+1;  // 3 or 5
    const int stencilSize=int( pow(width,mgcg.numberOfDimensions()) );
    constantCoefficients.redim(stencilSize,mgcg.numberOfComponentGrids(),mgcg.numberOfMultigridLevels());
    constantCoefficients=0.;
    
    for( int level=0; level<mgcg.numberOfMultigridLevels(); level++ )
    {

      // *wdh* 2015/09/05 -- added option NOT to average equations on the coarsest level (e.g. for Hypre AMG solver)
      const bool & averageEquationsOnCoarsestGrid = parameters.dbase.get<bool>("averageEquationsOnCoarsestGrid");  
      const bool isCoarsestLevel=level == mgcg.numberOfMultigridLevels()-1; 
      const bool doNotAverageEquations =
	(level==0 || 
	 parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations ||
	 ( isCoarsestLevel && !averageEquationsOnCoarsestGrid )
	  );
      

      for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
      {

	MappedGrid & mg = mgcg.multigridLevel[level][grid];

	if( equationToSolve==OgesParameters::heatEquationOperator )
	{
	  cI=equationCoefficients(0,grid);
	  cLap=equationCoefficients(1,grid);
	  // printf(" ======= Ogmg::initializeConstantCoefficients: heat: grid=%i cI=%8.2e cLap=%8.2e\n",grid,cI,cLap);

	  assert( fabs(cI)+fabs(cLap) > 0. );
	}


	if( mg.isRectangular() )
	{
          Range all;
//          RealArray & cc = constantCoefficients(all,grid,level); // not allowed on some machines
#define CC(m) constantCoefficients(m,grid,level)

	  real dx[3];
	  mg.getDeltaX(dx);
	  if( mg.numberOfDimensions()==2 )
	  {

            if( orderOfAccuracy==2 )
	    {
  	      // printf(">>>>>>>>>>>>>>>>>>>>>Setting constant coefficients for grid %i, 2nd order\n",grid);

              // ***** 2nd order ******
	      const int m11=0;                   // MCE(-1,-1, 0)
	      const int m21=1;                   // MCE( 0,-1, 0)
	      const int m31=2;                   // MCE(+1,-1, 0)
	      const int m12=3;                   // MCE(-1, 0, 0)
	      const int m22=4;                   // MCE( 0, 0, 0)
	      const int m32=5;                   // MCE(+1, 0, 0)
	      const int m13=6;                   // MCE(-1,+1, 0)
	      const int m23=7;                   // MCE( 0,+1, 0)
	      const int m33=8;                   // MCE(+1,+1, 0)
          
	      real dxsqi=1./(dx[0]*dx[0]), dysqi=1./(dx[1]*dx[1]);
	    
	      // if( level==0 || 
              //     (parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations ) )
	      if( doNotAverageEquations ) // *wdh* 2015/09/05
	      {
		// 5 point stencil
		CC(m21)=   cLap*(            dysqi  );
		CC(m12)=   cLap*(      dxsqi        );
		CC(m22)=cI+cLap*( -2.*(dxsqi+dysqi) );
		CC(m32)=   cLap*(      dxsqi        );
		CC(m23)=   cLap*(            dysqi  );
	      }
	      else if( level==1 )
	      {
		// 9 point averaged operator
		// these are computed with galerkin.m
		CC(m11)=   cLap*( 1./8.*(dxsqi+dysqi) );
		CC(m21)=   cLap*( 3./4*dysqi-dxsqi/4 );
		CC(m31)=   cLap*( 1./8.*(dxsqi+dysqi) );

		CC(m12)=   cLap*( 3./4*dxsqi-dysqi/4 );
		CC(m22)=cI+cLap*( -3./2.*(dxsqi+dysqi) );
		CC(m32)=   cLap*( 3./4*dxsqi-dysqi/4 );

		CC(m13)=   cLap*( 1./8.*(dxsqi+dysqi) );
		CC(m23)=   cLap*( 3./4*dysqi-dxsqi/4 );
		CC(m33)=   cLap*( 1./8.*(dxsqi+dysqi) );         
	      }
	      else if( level==2 )
	      {
		// these are computed with galerkin.m

		CC(m11)=   cLap*( 5./32.*(dxsqi+dysqi) );
		CC(m21)=   cLap*( 11./16*dysqi-5./16.*dxsqi );
		CC(m31)=   cLap*( 5./32.*(dxsqi+dysqi) );

		CC(m12)=   cLap*( 11./16*dxsqi-5./16.*dysqi );
		CC(m22)=cI+cLap*( -11./8.*(dxsqi+dysqi) );
		CC(m32)=   cLap*( 11./16*dxsqi-5./16.*dysqi );

		CC(m13)=   cLap*( 5./32.*(dxsqi+dysqi) );
		CC(m23)=   cLap*( 11./16*dysqi-5./16.*dxsqi );
		CC(m33)=   cLap*( 5./32.*(dxsqi+dysqi) );         

		// cc.display("++++++++++=cc on level>0+++++++++++++++++");
	      
	      }
	      else
	      {
//   A_\infty &=
//   \left[\begin{matrix} 
//           {1\over6}(\hxx+\hyy) & {2\over3}\hyy-{1\over3}\hxx & {1\over6}(\hxx+\hyy)         \\
//    {2\over3}\hxx-{1\over3}\hyy & -{4\over3}( \hxx+\hyy)      & {2\over3}\hxx-{1\over3}\hyy      \\
//          {1\over6}(\hxx+\hyy)  & {2\over3}\hyy-{1\over3}\hxx & {1\over6}(\hxx+\hyy) 
		real c22=cI+cLap*( -4./3.*(dxsqi+dysqi) );
		real c11=   cLap*( 1./6.*(dxsqi+dysqi) );
		real c21=   cLap*( 2./3*dysqi-1./3.*dxsqi );
		real c12=   cLap*( 2./3*dxsqi-1./3.*dysqi );

		CC(m11)=c11;
		CC(m21)=c21;
		CC(m31)=c11;
		CC(m12)=c12;
		CC(m22)=c22;
		CC(m32)=c12;
		CC(m13)=c11;
		CC(m23)=c21;
		CC(m33)=c11;



	      }
	    }
	    else if( orderOfAccuracy==4 )
	    {
	      // printF(">>>>>>>>>>>>>>>>>>>>>Setting constant coefficients for grid %i, 4th order\n",grid);
              // ***** 4th order ******
	      const int m11=0;
	      const int m21=1;
	      const int m31=2;
	      const int m41=3;
	      const int m51=4;
	      const int m12=5;
	      const int m22=6;
	      const int m32=7;
	      const int m42=8;
	      const int m52=9;
	      const int m13=10;
	      const int m23=11;
	      const int m33=12;
	      const int m43=13;
	      const int m53=14;
	      const int m14=15;
	      const int m24=16;
	      const int m34=17;
	      const int m44=18;
	      const int m54=19;
	      const int m15=20;
	      const int m25=21;
	      const int m35=22;
	      const int m45=23;
	      const int m55=24;

//        urr(i1,i2,i3,kd)=
//       & ( -30.*u(i1,i2,i3,kd)
//       &  +16.*(u(i1+1,i2,i3,kd)+u(i1-1,i2,i3,kd))
//       &      -(u(i1+2,i2,i3,kd)+u(i1-2,i2,i3,kd)) )*d24(1)

  	      const real dxi=1./(12.*dx[0]*dx[0]);
  	      const real dyi=1./(12.*dx[1]*dx[1]);
      
	      real dxsqi=1./(dx[0]*dx[0]);
	      real dysqi=1./(dx[1]*dx[1]);
		

	      // if( level==0 || parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations )
	      if( doNotAverageEquations ) // *wdh* 2015/09/05
	      {
		CC(m11)=0.;
		CC(m21)=0.;
		CC(m31)=   cLap*(     -dyi       );
		CC(m41)=0.;
		CC(m51)=0.;
		CC(m12)=0.;
		CC(m22)=0.;
		CC(m32)=   cLap*(  16.*dyi       );
		CC(m42)=0.;
		CC(m52)=0.;
		CC(m13)=   cLap*(     -dxi       );
		CC(m23)=   cLap*(  16.*dxi       );
		CC(m33)=cI+cLap*( -30.*(dxi+dyi) );
		CC(m43)=   cLap*(  16.* dxi      );
		CC(m53)=   cLap*(     -dxi       );
		CC(m14)=0.;
		CC(m24)=0.;
		CC(m34)=   cLap*(  16.*dyi       );
		CC(m44)=0.;
		CC(m54)=0.;
		CC(m15)=0.;
		CC(m25)=0.;
		CC(m35)=   cLap*(     -dyi       );
		CC(m45)=0.;
		CC(m55)=0.;
	      }
	      else if( level==1 )
	      {
		// averaged operator from the matlab code galerkin4.m

		CC(m11)=0.;
		CC(m21)=cLap*(-0.5208333333333333E-2*dysqi);
		CC(m31)=cLap*(-0.3125E-1*dysqi);
		CC(m41)=cLap*(-0.5208333333333333E-2*dysqi);
		CC(m51)=0.;

		CC(m12)=cLap*(-0.5208333333333333E-2*dxsqi);
		CC(m22)=cLap*(0.1458333333333333*dysqi+0.1458333333333333*dxsqi);
		CC(m32)=cLap*(0.875*dysqi-0.28125*dxsqi);
		CC(m42)=cLap*(0.1458333333333333*dysqi+0.1458333333333333*dxsqi);
		CC(m52)=cLap*(-0.5208333333333333E-2*dxsqi);

		CC(m13)=   cLap*(-0.3125E-1*dxsqi);
		CC(m23)=   cLap*( -0.28125*dysqi+0.875*dxsqi);
		CC(m33)=cI+cLap*( -0.16875E1*dysqi-0.16875E1*dxsqi );
		CC(m43)=   cLap*(-0.28125*dysqi+0.875*dxsqi );
		CC(m53)=   cLap*( -0.3125E-1*dxsqi);

		CC(m14)=cLap*(-0.5208333333333333E-2*dxsqi);
		CC(m24)=cLap*(0.1458333333333333*dysqi+0.1458333333333333*dxsqi);
		CC(m34)=cLap*(0.875*dysqi-0.28125*dxsqi  );
		CC(m44)=cLap*(0.1458333333333333*dysqi+0.1458333333333333*dxsqi);
		CC(m54)=cLap*(-0.5208333333333333E-2*dxsqi);

		CC(m15)=0.;
		CC(m25)=cLap*(-0.5208333333333333E-2*dysqi);
		CC(m35)=cLap*(-0.3125E-1*dysqi   );
		CC(m45)=cLap*(-0.5208333333333333E-2*dysqi);
		CC(m55)=0.;
	      }
	      else if( level==2 )
	      {
		CC(m11)=  cLap*( 0.0);
		CC(m12)=  cLap*( -0.3255208333333333E-2*dxsqi);
		CC(m13)=  cLap*( -0.1432291666666667E-1*dxsqi);
		CC(m14)=  cLap*( -0.3255208333333333E-2*dxsqi);
		CC(m15)=  cLap*( 0.0);
		CC(m21)=  cLap*( -0.3255208333333333E-2*dysqi);
		CC(m22)=  cLap*( 0.1692708333333333*dysqi+0.1692708333333333*dxsqi);
		CC(m23)=  cLap*( -0.33203125*dysqi+0.7447916666666667*dxsqi);
		CC(m24)=  cLap*( 0.1692708333333333*dysqi+0.1692708333333333*dxsqi);
		CC(m25)=  cLap*( -0.3255208333333333E-2*dysqi);
		CC(m31)=  cLap*( -0.1432291666666667E-1*dysqi);
		CC(m32)=  cLap*( 0.7447916666666667*dysqi-0.33203125*dxsqi);
		CC(m33)=cI+cLap*( -0.14609375E1*dysqi-0.14609375E1*dxsqi);
		CC(m34)=  cLap*( 0.7447916666666667*dysqi-0.33203125*dxsqi);
		CC(m35)=  cLap*( -0.1432291666666667E-1*dysqi);
		CC(m41)=  cLap*( -0.3255208333333333E-2*dysqi);
		CC(m42)=  cLap*( 0.1692708333333333*dysqi+0.1692708333333333*dxsqi);
		CC(m43)=  cLap*( -0.33203125*dysqi+0.7447916666666667*dxsqi);
		CC(m44)=  cLap*( 0.1692708333333333*dysqi+0.1692708333333333*dxsqi);
		CC(m45)=  cLap*( -0.3255208333333333E-2*dysqi);
		CC(m51)=  cLap*( 0.0);
		CC(m52)=  cLap*( -0.3255208333333333E-2*dxsqi);
		CC(m53)=  cLap*( -0.1432291666666667E-1*dxsqi);
		CC(m54)=  cLap*( -0.3255208333333333E-2*dxsqi);
		CC(m55)=  cLap*( 0.0);
	      }
	      else if( level==3 )
	      {
		CC(m11)=  cLap*( 0.0);
		CC(m12)=  cLap*( -0.1708984375E-2*dxsqi);
		CC(m13)=  cLap*( -0.6998697916666667E-2*dxsqi);
		CC(m14)=  cLap*( -0.1708984375E-2*dxsqi);
		CC(m15)=  cLap*( 0.0);
		CC(m21)=  cLap*( -0.1708984375E-2*dysqi);
		CC(m22)=  cLap*( 0.1708984375*dysqi+0.1708984375*dxsqi);
		CC(m23)=  cLap*( -0.33837890625*dysqi+0.6998697916666667*dxsqi);
		CC(m24)=  cLap*( 0.1708984375*dysqi+0.1708984375*dxsqi);
		CC(m25)=  cLap*( -0.1708984375E-2*dysqi);
		CC(m31)=  cLap*( -0.6998697916666667E-2*dysqi);
		CC(m32)=  cLap*( 0.6998697916666667*dysqi-0.33837890625*dxsqi);
		CC(m33)=  cLap*( -0.13857421875E1*dysqi-0.13857421875E1*dxsqi);
		CC(m34)=  cLap*( 0.6998697916666667*dysqi-0.33837890625*dxsqi);
		CC(m35)=  cLap*( -0.6998697916666667E-2*dysqi);
		CC(m41)=  cLap*( -0.1708984375E-2*dysqi);
		CC(m42)=  cLap*( 0.1708984375*dysqi+0.1708984375*dxsqi);
		CC(m43)=  cLap*( -0.33837890625*dysqi+0.6998697916666667*dxsqi);
		CC(m44)=  cLap*( 0.1708984375*dysqi+0.1708984375*dxsqi);
		CC(m45)=  cLap*( -0.1708984375E-2*dysqi);
		CC(m51)=  cLap*( 0.0);
		CC(m52)=  cLap*( -0.1708984375E-2*dxsqi);
		CC(m53)=  cLap*( -0.6998697916666667E-2*dxsqi);
		CC(m54)=  cLap*( -0.1708984375E-2*dxsqi);
		CC(m55)=  cLap*( 0.0);
	      }
	      else
	      { // level==10
		CC(m11)=  cLap*( 0.0);
		CC(m12)=  cLap*( -0.135633551205198E-4*dxsqi);
		CC(m13)=  cLap*( -0.5425349809229374E-4*dxsqi);
		CC(m14)=  cLap*( -0.135633551205198E-4*dxsqi);
		CC(m15)=  cLap*( 0.0);
		CC(m21)=  cLap*( -0.135633551205198E-4*dysqi);
		CC(m22)=  cLap*( 0.1667207611414293*dysqi+0.1667207611414293*dxsqi);
		CC(m23)=  cLap*( -0.3334143955726177*dysqi+0.6668839985504746*dxsqi);
		CC(m24)=  cLap*( 0.1667207611414293*dysqi+0.1667207611414293*dxsqi);
		CC(m25)=  cLap*( -0.135633551205198E-4*dysqi);
		CC(m31)=  cLap*( -0.5425349809229374E-4*dysqi);
		CC(m32)=  cLap*( 0.6668839985504746*dysqi-0.3334143955726177*dxsqi);
		CC(m33)=cI+cLap*( -0.1333659490104765E1*dysqi-0.1333659490104765E1*dxsqi);
		CC(m34)=  cLap*( 0.6668839985504746*dysqi-0.3334143955726177*dxsqi);
		CC(m35)=  cLap*( -0.5425349809229374E-4*dysqi);
		CC(m41)=  cLap*( -0.135633551205198E-4*dysqi);
		CC(m42)=  cLap*( 0.1667207611414293*dysqi+0.1667207611414293*dxsqi);
		CC(m43)=  cLap*( -0.3334143955726177*dysqi+0.6668839985504746*dxsqi);
		CC(m44)=  cLap*( 0.1667207611414293*dysqi+0.1667207611414293*dxsqi);
		CC(m45)=  cLap*( -0.135633551205198E-4*dysqi);
		CC(m51)=  cLap*( 0.0);
		CC(m52)=  cLap*( -0.135633551205198E-4*dxsqi);
		CC(m53)=  cLap*( -0.5425349809229374E-4*dxsqi);
		CC(m54)=  cLap*( -0.135633551205198E-4*dxsqi);
		CC(m55)=  cLap*( 0.0);

	      }
	      


	    }
            else
	    {
	      Overture::abort("ERROR: invalid orderOfAccuacy");
	    }

	  }
	  else if( mg.numberOfDimensions()==3 )
	  {
            // *************************************************************
            // ****************** three dimensions *************************
            // *************************************************************
            if( orderOfAccuracy==2 )
	    {
              // ***** 2nd order ******
	      const int m111=0;
	      const int m211=1;
	      const int m311=2;
	      const int m121=3;
	      const int m221=4;
	      const int m321=5;
	      const int m131=6;
	      const int m231=7;
	      const int m331=8;
	      const int m112=9;
	      const int m212=10;
	      const int m312=11;
	      const int m122=12;
	      const int m222=13;
	      const int m322=14;
	      const int m132=15;
	      const int m232=16;
	      const int m332=17;
	      const int m113=18;
	      const int m213=19;
	      const int m313=20;
	      const int m123=21;
	      const int m223=22;
	      const int m323=23;
	      const int m133=24;
	      const int m233=25;
	      const int m333=26;

	      real dxsqi=1./(dx[0]*dx[0]), dysqi=1./(dx[1]*dx[1]), dzsqi=1./(dx[2]*dx[2]);
	    
	      // if( level==0 || parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations )
	      if( doNotAverageEquations ) // *wdh* 2015/09/05
	      {
		// 7 point stencil
		CC(m221)=   cLap*( dzsqi );

		CC(m212)=   cLap*( dysqi );

		CC(m122)=   cLap*( dxsqi );
		CC(m222)=cI+cLap*( -2.*(dxsqi+dysqi+dzsqi) );
		CC(m322)=   cLap*( dxsqi );

		CC(m232)=   cLap*( dysqi );
		CC(m223)=   cLap*( dzsqi );
	      }
	      else if( level==1 )
	      {
		// 27 point averaged operator
		// these are computed with galerkin.m
		CC(m111)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m121)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dxsqi-dysqi/32.0); 
		CC(m131)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m211)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dysqi-dxsqi/32.0); 
		CC(m221)=    cLap*( 9.0/16.0*dzsqi-3.0/16.0*dxsqi-3.0/16.0*dysqi); 
		CC(m231)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dysqi-dxsqi/32.0); 
		CC(m311)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m321)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dxsqi-dysqi/32.0); 
		CC(m331)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m112)=    cLap*( 3.0/32.0*dysqi+3.0/32.0*dxsqi-dzsqi/32.0); 
		CC(m122)=    cLap*( 9.0/16.0*dxsqi-3.0/16.0*dysqi-3.0/16.0*dzsqi); 
		CC(m132)=    cLap*( 3.0/32.0*dysqi+3.0/32.0*dxsqi-dzsqi/32.0); 
		CC(m212)=    cLap*( 9.0/16.0*dysqi-3.0/16.0*dxsqi-3.0/16.0*dzsqi); 
		CC(m222)=    cLap*( -9.0/8.0*dxsqi-9.0/8.0*dysqi-9.0/8.0*dzsqi); 
		CC(m232)=    cLap*( 9.0/16.0*dysqi-3.0/16.0*dxsqi-3.0/16.0*dzsqi); 
		CC(m312)=    cLap*( 3.0/32.0*dysqi+3.0/32.0*dxsqi-dzsqi/32.0); 
		CC(m322)=    cLap*( 9.0/16.0*dxsqi-3.0/16.0*dysqi-3.0/16.0*dzsqi); 
		CC(m332)=    cLap*( 3.0/32.0*dysqi+3.0/32.0*dxsqi-dzsqi/32.0); 
		CC(m113)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m123)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dxsqi-dysqi/32.0); 
		CC(m133)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m213)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dysqi-dxsqi/32.0); 
		CC(m223)=    cLap*( 9.0/16.0*dzsqi-3.0/16.0*dxsqi-3.0/16.0*dysqi); 
		CC(m233)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dysqi-dxsqi/32.0); 
		CC(m313)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 
		CC(m323)=    cLap*( 3.0/32.0*dzsqi+3.0/32.0*dxsqi-dysqi/32.0); 
		CC(m333)=    cLap*( dzsqi/64.0+dysqi/64.0+dxsqi/64.0); 


	      }
	      else if( level==2 )
	      {
		// these are computed with galerkin.m
		// cc.display("++++++++++=cc on level>0+++++++++++++++++");
		CC(m111)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m121)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dxsqi-25.0/512.0*dysqi); 
		CC(m131)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m211)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dysqi-25.0/512.0*dxsqi); 
		CC(m221)=    cLap*( 121.0/256.0*dzsqi-55.0/256.0*dxsqi-55.0/256.0*dysqi); 
		CC(m231)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dysqi-25.0/512.0*dxsqi); 
		CC(m311)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m321)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dxsqi-25.0/512.0*dysqi); 
		CC(m331)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m112)=    cLap*( 55.0/512.0*dysqi+55.0/512.0*dxsqi-25.0/512.0*dzsqi); 
		CC(m122)=    cLap*( 121.0/256.0*dxsqi-55.0/256.0*dysqi-55.0/256.0*dzsqi); 
		CC(m132)=    cLap*( 55.0/512.0*dysqi+55.0/512.0*dxsqi-25.0/512.0*dzsqi); 
		CC(m212)=    cLap*( 121.0/256.0*dysqi-55.0/256.0*dxsqi-55.0/256.0*dzsqi); 
		CC(m222)=    cLap*( -121.0/128.0*dxsqi-121.0/128.0*dysqi-121.0/128.0*dzsqi); 
		CC(m232)=    cLap*( 121.0/256.0*dysqi-55.0/256.0*dxsqi-55.0/256.0*dzsqi); 
		CC(m312)=    cLap*( 55.0/512.0*dysqi+55.0/512.0*dxsqi-25.0/512.0*dzsqi); 
		CC(m322)=    cLap*( 121.0/256.0*dxsqi-55.0/256.0*dysqi-55.0/256.0*dzsqi); 
		CC(m332)=    cLap*( 55.0/512.0*dysqi+55.0/512.0*dxsqi-25.0/512.0*dzsqi); 
		CC(m113)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m123)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dxsqi-25.0/512.0*dysqi); 
		CC(m133)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m213)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dysqi-25.0/512.0*dxsqi); 
		CC(m223)=    cLap*( 121.0/256.0*dzsqi-55.0/256.0*dxsqi-55.0/256.0*dysqi); 
		CC(m233)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dysqi-25.0/512.0*dxsqi); 
		CC(m313)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
		CC(m323)=    cLap*( 55.0/512.0*dzsqi+55.0/512.0*dxsqi-25.0/512.0*dysqi); 
		CC(m333)=    cLap*( 25.0/1024.0*dzsqi+25.0/1024.0*dysqi+25.0/1024.0*dxsqi); 
	      
	      }
	      else
	      {
		// limit matrix
		real c111=   cLap*( 1./36*(dxsqi+dysqi+dzsqi) );          // 122167725625.0/4398046511104.0
		real c112=   cLap*( 1./9.*dxsqi+1./9.*dysqi-1./18.*dzsqi);
		real c121=   cLap*( 1./9.*dxsqi+1./9.*dzsqi-1./18.*dysqi);
		real c211=   cLap*( 1./9.*dzsqi+1./9.*dysqi-1./18.*dxsqi);
              
		real c212=   cLap*( 4./9.*dysqi-2./9.*dxsqi-2./9.*dzsqi); // check
		real c221=   cLap*( 4./9.*dzsqi-2./9.*dxsqi-2./9.*dysqi);
		real c122=   cLap*( 4./9.*dxsqi-2./9.*dysqi-2./9.*dzsqi);

		real c222=cI+cLap*(-8./9.*(dxsqi+dysqi+dzsqi));  // .888888
	       
		CC(m111)=c111;
		CC(m121)=c121;
		CC(m131)=c111;
		CC(m211)=c211;
		CC(m221)=c221;
		CC(m231)=c211;
		CC(m311)=c111;
		CC(m321)=c121;
		CC(m331)=c111;
		CC(m112)=c112;
		CC(m122)=c122;
		CC(m132)=c112;
		CC(m212)=c212;
		CC(m222)=c222;
		CC(m232)=c212;
		CC(m312)=c112;
		CC(m322)=c122;
		CC(m332)=c112;
		CC(m113)=c111;
		CC(m123)=c121;
		CC(m133)=c111;
		CC(m213)=c211;
		CC(m223)=c221;
		CC(m233)=c211;
		CC(m313)=c111;
		CC(m323)=c121;
		CC(m333)=c111;

	      }
	      
	    } // end orderOfAccuracy==2
	    else if( orderOfAccuracy==4 )
	    {
              // ************ 4th order *********************
	      // printf("Setting constant coefficients for grid %i, 4th order\n",grid);

	      real dxsqi=1./(dx[0]*dx[0]), dysqi=1./(dx[1]*dx[1]), dzsqi=1./(dx[2]*dx[2]);

	      const int m111=0;
	      const int m211=1;
	      const int m311=2;
	      const int m411=3;
	      const int m511=4;
	      const int m121=5;
	      const int m221=6;
	      const int m321=7;
	      const int m421=8;
	      const int m521=9;
	      const int m131=10;
	      const int m231=11;
	      const int m331=12;
	      const int m431=13;
	      const int m531=14;
	      const int m141=15;
	      const int m241=16;
	      const int m341=17;
	      const int m441=18;
	      const int m541=19;
	      const int m151=20;
	      const int m251=21;
	      const int m351=22;
	      const int m451=23;
	      const int m551=24;

	      const int m112=25;
	      const int m212=26;
	      const int m312=27;
	      const int m412=28;
	      const int m512=29;
	      const int m122=30;
	      const int m222=31;
	      const int m322=32;
	      const int m422=33;
	      const int m522=34;
	      const int m132=35;
	      const int m232=36;
	      const int m332=37;
	      const int m432=38;
	      const int m532=39;
	      const int m142=40;
	      const int m242=41;
	      const int m342=42;
	      const int m442=43;
	      const int m542=44;
	      const int m152=45;
	      const int m252=46;
	      const int m352=47;
	      const int m452=48;
	      const int m552=49;

	      const int m113=50;
	      const int m213=51;
	      const int m313=52;
	      const int m413=53;
	      const int m513=54;
	      const int m123=55;
	      const int m223=56;
	      const int m323=57;
	      const int m423=58;
	      const int m523=59;
	      const int m133=60;
	      const int m233=61;
	      const int m333=62;
	      const int m433=63;
	      const int m533=64;
	      const int m143=65;
	      const int m243=66;
	      const int m343=67;
	      const int m443=68;
	      const int m543=69;
	      const int m153=70;
	      const int m253=71;
	      const int m353=72;
	      const int m453=73;
	      const int m553=74;

	      const int m114=75;
	      const int m214=76;
	      const int m314=77;
	      const int m414=78;
	      const int m514=79;
	      const int m124=80;
	      const int m224=81;
	      const int m324=82;
	      const int m424=83;
	      const int m524=84;
	      const int m134=85;
	      const int m234=86;
	      const int m334=87;
	      const int m434=88;
	      const int m534=89;
	      const int m144=90;
	      const int m244=91;
	      const int m344=92;
	      const int m444=93;
	      const int m544=94;
	      const int m154=95;
	      const int m254=96;
	      const int m354=97;
	      const int m454=98;
	      const int m554=99;

	      const int m115=100;
	      const int m215=101;
	      const int m315=102;
	      const int m415=103;
	      const int m515=104;
	      const int m125=105;
	      const int m225=106;
	      const int m325=107;
	      const int m425=108;
	      const int m525=109;
	      const int m135=110;
	      const int m235=111;
	      const int m335=112;
	      const int m435=113;
	      const int m535=114;
	      const int m145=115;
	      const int m245=116;
	      const int m345=117;
	      const int m445=118;
	      const int m545=119;
	      const int m155=120;
	      const int m255=121;
	      const int m355=122;
	      const int m455=123;
	      const int m555=124;


  	      const real dxi=1./(12.*dx[0]*dx[0]);
  	      const real dyi=1./(12.*dx[1]*dx[1]);
  	      const real dzi=1./(12.*dx[2]*dx[2]);
	      // if( level==0 || parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations )
	      if( doNotAverageEquations ) // *wdh* 2015/09/05
	      {
		CC(m331)=   cLap*(     -dzi       );
		CC(m332)=   cLap*(  16.*dzi       );
		CC(m313)=   cLap*(     -dyi       );
		CC(m323)=   cLap*(  16.*dyi       );
		CC(m133)=   cLap*(     -dxi       );
		CC(m233)=   cLap*(  16.*dxi       );
		CC(m333)=cI+cLap*( -30.*(dxi+dyi+dzi) );
		CC(m433)=   cLap*(  16.* dxi      );
		CC(m533)=   cLap*(     -dxi       );
		CC(m343)=   cLap*(  16.*dyi       );
		CC(m353)=   cLap*(     -dyi       );
		CC(m334)=   cLap*(  16.*dzi       );
		CC(m335)=   cLap*(     -dzi       );
	      }
              else if( level==1 )
	      {
		CC(m111)=    cLap*( 0.0); 
		CC(m211)=    cLap*( 0.0); 
		CC(m311)=    cLap*( 0.0); 
		CC(m411)=    cLap*( 0.0); 
		CC(m511)=    cLap*( 0.0); 
		CC(m121)=    cLap*( 0.0); 
		CC(m221)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m321)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m421)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m521)=    cLap*( 0.0); 
		CC(m131)=    cLap*( 0.0); 
		CC(m231)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m331)=    cLap*( -0.234375E-1*dzsqi); 
		CC(m431)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m531)=    cLap*( 0.0); 
		CC(m141)=    cLap*( 0.0); 
		CC(m241)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m341)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m441)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m541)=    cLap*( 0.0); 
		CC(m151)=    cLap*( 0.0); 
		CC(m251)=    cLap*( 0.0); 
		CC(m351)=    cLap*( 0.0); 
		CC(m451)=    cLap*( 0.0); 
		CC(m551)=    cLap*( 0.0); 
		CC(m112)=    cLap*( 0.0); 
		CC(m212)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m312)=    cLap*( -0.390625E-2*dysqi); 
		CC(m412)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m512)=    cLap*( 0.0); 
		CC(m122)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m222)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m322)=    cLap*( 0.109375*dzsqi+0.109375*dysqi-0.3515625E-1*dxsqi); 
		CC(m422)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m522)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m132)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m232)=    cLap*( 0.109375*dzsqi-0.3515625E-1*dysqi+0.109375*dxsqi); 
		CC(m332)=    cLap*( 0.65625*dzsqi-0.2109375*dysqi-0.2109375*dxsqi); 
		CC(m432)=    cLap*( 0.109375*dzsqi-0.3515625E-1*dysqi+0.109375*dxsqi); 
		CC(m532)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m142)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m242)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m342)=    cLap*( 0.109375*dzsqi+0.109375*dysqi-0.3515625E-1*dxsqi); 
		CC(m442)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m542)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m152)=    cLap*( 0.0); 
		CC(m252)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m352)=    cLap*( -0.390625E-2*dysqi); 
		CC(m452)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m552)=    cLap*( 0.0); 
		CC(m113)=    cLap*( 0.0); 
		CC(m213)=    cLap*( -0.390625E-2*dysqi); 
		CC(m313)=    cLap*( -0.234375E-1*dysqi); 
		CC(m413)=    cLap*( -0.390625E-2*dysqi); 
		CC(m513)=    cLap*( 0.0); 
		CC(m123)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m223)=    cLap*( -0.3515625E-1*dzsqi+0.109375*dysqi+0.109375*dxsqi); 
		CC(m323)=    cLap*( -0.2109375*dzsqi+0.65625*dysqi-0.2109375*dxsqi); 
		CC(m423)=    cLap*( -0.3515625E-1*dzsqi+0.109375*dysqi+0.109375*dxsqi); 
		CC(m523)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m133)=    cLap*( -0.234375E-1*dxsqi); 
		CC(m233)=    cLap*( -0.2109375*dzsqi-0.2109375*dysqi+0.65625*dxsqi); 
		CC(m333)= cI+cLap*( -0.1265625E1*dzsqi-0.1265625E1*dysqi-0.1265625E1*dxsqi); 
		CC(m433)=    cLap*( -0.2109375*dzsqi-0.2109375*dysqi+0.65625*dxsqi); 
		CC(m533)=    cLap*( -0.234375E-1*dxsqi); 
		CC(m143)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m243)=    cLap*( -0.3515625E-1*dzsqi+0.109375*dysqi+0.109375*dxsqi); 
		CC(m343)=    cLap*( -0.2109375*dzsqi+0.65625*dysqi-0.2109375*dxsqi); 
		CC(m443)=    cLap*( -0.3515625E-1*dzsqi+0.109375*dysqi+0.109375*dxsqi); 
		CC(m543)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m153)=    cLap*( 0.0); 
		CC(m253)=    cLap*( -0.390625E-2*dysqi); 
		CC(m353)=    cLap*( -0.234375E-1*dysqi); 
		CC(m453)=    cLap*( -0.390625E-2*dysqi); 
		CC(m553)=    cLap*( 0.0); 
		CC(m114)=    cLap*( 0.0); 
		CC(m214)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m314)=    cLap*( -0.390625E-2*dysqi); 
		CC(m414)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m514)=    cLap*( 0.0); 
		CC(m124)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m224)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m324)=    cLap*( 0.109375*dzsqi+0.109375*dysqi-0.3515625E-1*dxsqi); 
		CC(m424)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m524)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m134)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m234)=    cLap*( 0.109375*dzsqi-0.3515625E-1*dysqi+0.109375*dxsqi); 
		CC(m334)=    cLap*( 0.65625*dzsqi-0.2109375*dysqi-0.2109375*dxsqi); 
		CC(m434)=    cLap*( 0.109375*dzsqi-0.3515625E-1*dysqi+0.109375*dxsqi); 
		CC(m534)=    cLap*( -0.390625E-2*dxsqi); 
		CC(m144)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m244)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m344)=    cLap*( 0.109375*dzsqi+0.109375*dysqi-0.3515625E-1*dxsqi); 
		CC(m444)=    cLap*( 0.1822916666666667E-1*dzsqi+0.1822916666666667E-1*dysqi+0.1822916666666667E-1*dxsqi); 
		CC(m544)=    cLap*( -0.6510416666666667E-3*dxsqi); 
		CC(m154)=    cLap*( 0.0); 
		CC(m254)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m354)=    cLap*( -0.390625E-2*dysqi); 
		CC(m454)=    cLap*( -0.6510416666666667E-3*dysqi); 
		CC(m554)=    cLap*( 0.0); 
		CC(m115)=    cLap*( 0.0); 
		CC(m215)=    cLap*( 0.0); 
		CC(m315)=    cLap*( 0.0); 
		CC(m415)=    cLap*( 0.0); 
		CC(m515)=    cLap*( 0.0); 
		CC(m125)=    cLap*( 0.0); 
		CC(m225)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m325)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m425)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m525)=    cLap*( 0.0); 
		CC(m135)=    cLap*( 0.0); 
		CC(m235)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m335)=    cLap*( -0.234375E-1*dzsqi); 
		CC(m435)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m535)=    cLap*( 0.0); 
		CC(m145)=    cLap*( 0.0); 
		CC(m245)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m345)=    cLap*( -0.390625E-2*dzsqi); 
		CC(m445)=    cLap*( -0.6510416666666667E-3*dzsqi); 
		CC(m545)=    cLap*( 0.0); 
		CC(m155)=    cLap*( 0.0); 
		CC(m255)=    cLap*( 0.0); 
		CC(m355)=    cLap*( 0.0); 
		CC(m455)=    cLap*( 0.0); 
		CC(m555)=    cLap*( 0.0); 
	      }
	      else if( level==2 )
	      {
		CC(m111)=   cLap*( 0.0); 
		CC(m211)=   cLap*( 0.0); 
		CC(m311)=   cLap*( 0.0); 
		CC(m411)=   cLap*( 0.0); 
		CC(m511)=   cLap*( 0.0); 
		CC(m121)=   cLap*( 0.0); 
		CC(m221)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m321)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m421)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m521)=   cLap*( 0.0); 
		CC(m131)=   cLap*( 0.0); 
		CC(m231)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m331)=   cLap*( -0.9847005208333333E-2*dzsqi); 
		CC(m431)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m531)=   cLap*( 0.0); 
		CC(m141)=   cLap*( 0.0); 
		CC(m241)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m341)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m441)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m541)=   cLap*( 0.0); 
		CC(m151)=   cLap*( 0.0); 
		CC(m251)=   cLap*( 0.0); 
		CC(m351)=   cLap*( 0.0); 
		CC(m451)=   cLap*( 0.0); 
		CC(m551)=   cLap*( 0.0); 
		CC(m112)=   cLap*( 0.0); 
		CC(m212)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m312)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m412)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m512)=   cLap*( 0.0); 
		CC(m122)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m222)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m322)=   cLap*( 0.1163736979166667*dzsqi+0.1163736979166667*dysqi-0.518798828125E-1*dxsqi); 
		CC(m422)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m522)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m132)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m232)=   cLap*( 0.1163736979166667*dzsqi-0.518798828125E-1*dysqi+0.1163736979166667*dxsqi); 
		CC(m332)=   cLap*( 0.5120442708333333*dzsqi-0.228271484375*dysqi-0.228271484375*dxsqi); 
		CC(m432)=   cLap*( 0.1163736979166667*dzsqi-0.518798828125E-1*dysqi+0.1163736979166667*dxsqi); 
		CC(m532)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m142)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m242)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m342)=   cLap*( 0.1163736979166667*dzsqi+0.1163736979166667*dysqi-0.518798828125E-1*dxsqi); 
		CC(m442)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m542)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m152)=   cLap*( 0.0); 
		CC(m252)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m352)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m452)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m552)=   cLap*( 0.0); 
		CC(m113)=   cLap*( 0.0); 
		CC(m213)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m313)=   cLap*( -0.9847005208333333E-2*dysqi); 
		CC(m413)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m513)=   cLap*( 0.0); 
		CC(m123)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m223)=   cLap*( -0.518798828125E-1*dzsqi+0.1163736979166667*dysqi+0.1163736979166667*dxsqi); 
		CC(m323)=   cLap*( -0.228271484375*dzsqi+0.5120442708333333*dysqi-0.228271484375*dxsqi); 
		CC(m423)=   cLap*( -0.518798828125E-1*dzsqi+0.1163736979166667*dysqi+0.1163736979166667*dxsqi); 
		CC(m523)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m133)=   cLap*( -0.9847005208333333E-2*dxsqi); 
		CC(m233)=   cLap*( -0.228271484375*dzsqi-0.228271484375*dysqi+0.5120442708333333*dxsqi); 
		CC(m333)=cI+cLap*( -0.100439453125E1*dzsqi-0.100439453125E1*dysqi-0.100439453125E1*dxsqi); 
		CC(m433)=   cLap*( -0.228271484375*dzsqi-0.228271484375*dysqi+0.5120442708333333*dxsqi); 
		CC(m533)=   cLap*( -0.9847005208333333E-2*dxsqi); 
		CC(m143)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m243)=   cLap*( -0.518798828125E-1*dzsqi+0.1163736979166667*dysqi+0.1163736979166667*dxsqi); 
		CC(m343)=   cLap*( -0.228271484375*dzsqi+0.5120442708333333*dysqi-0.228271484375*dxsqi); 
		CC(m443)=   cLap*( -0.518798828125E-1*dzsqi+0.1163736979166667*dysqi+0.1163736979166667*dxsqi); 
		CC(m543)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m153)=   cLap*( 0.0); 
		CC(m253)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m353)=   cLap*( -0.9847005208333333E-2*dysqi); 
		CC(m453)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m553)=   cLap*( 0.0); 
		CC(m114)=   cLap*( 0.0); 
		CC(m214)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m314)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m414)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m514)=   cLap*( 0.0); 
		CC(m124)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m224)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m324)=   cLap*( 0.1163736979166667*dzsqi+0.1163736979166667*dysqi-0.518798828125E-1*dxsqi); 
		CC(m424)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m524)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m134)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m234)=   cLap*( 0.1163736979166667*dzsqi-0.518798828125E-1*dysqi+0.1163736979166667*dxsqi); 
		CC(m334)=   cLap*( 0.5120442708333333*dzsqi-0.228271484375*dysqi-0.228271484375*dxsqi); 
		CC(m434)=   cLap*( 0.1163736979166667*dzsqi-0.518798828125E-1*dysqi+0.1163736979166667*dxsqi); 
		CC(m534)=   cLap*( -0.2237955729166667E-2*dxsqi); 
		CC(m144)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m244)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m344)=   cLap*( 0.1163736979166667*dzsqi+0.1163736979166667*dysqi-0.518798828125E-1*dxsqi); 
		CC(m444)=   cLap*( 0.2644856770833333E-1*dzsqi+0.2644856770833333E-1*dysqi+0.2644856770833333E-1*dxsqi); 
		CC(m544)=   cLap*( -0.5086263020833333E-3*dxsqi); 
		CC(m154)=   cLap*( 0.0); 
		CC(m254)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m354)=   cLap*( -0.2237955729166667E-2*dysqi); 
		CC(m454)=   cLap*( -0.5086263020833333E-3*dysqi); 
		CC(m554)=   cLap*( 0.0); 
		CC(m115)=   cLap*( 0.0); 
		CC(m215)=   cLap*( 0.0); 
		CC(m315)=   cLap*( 0.0); 
		CC(m415)=   cLap*( 0.0); 
		CC(m515)=   cLap*( 0.0); 
		CC(m125)=   cLap*( 0.0); 
		CC(m225)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m325)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m425)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m525)=   cLap*( 0.0); 
		CC(m135)=   cLap*( 0.0); 
		CC(m235)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m335)=   cLap*( -0.9847005208333333E-2*dzsqi); 
		CC(m435)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m535)=   cLap*( 0.0); 
		CC(m145)=   cLap*( 0.0); 
		CC(m245)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m345)=   cLap*( -0.2237955729166667E-2*dzsqi); 
		CC(m445)=   cLap*( -0.5086263020833333E-3*dzsqi); 
		CC(m545)=   cLap*( 0.0); 
		CC(m155)=   cLap*( 0.0); 
		CC(m255)=   cLap*( 0.0); 
		CC(m355)=   cLap*( 0.0); 
		CC(m455)=   cLap*( 0.0); 
		CC(m555)=   cLap*( 0.0); 

	      }
	      else if( level==3 )
	      {
		CC(m111)=   cLap*( 0.0); 
		CC(m211)=   cLap*( 0.0); 
		CC(m311)=   cLap*( 0.0); 
		CC(m411)=   cLap*( 0.0); 
		CC(m511)=   cLap*( 0.0); 
		CC(m121)=   cLap*( 0.0); 
		CC(m221)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m321)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m421)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m521)=   cLap*( 0.0); 
		CC(m131)=   cLap*( 0.0); 
		CC(m231)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m331)=   cLap*( -0.4702250162760417E-2*dzsqi); 
		CC(m431)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m531)=   cLap*( 0.0); 
		CC(m141)=   cLap*( 0.0); 
		CC(m241)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m341)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m441)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m541)=   cLap*( 0.0); 
		CC(m151)=   cLap*( 0.0); 
		CC(m251)=   cLap*( 0.0); 
		CC(m351)=   cLap*( 0.0); 
		CC(m451)=   cLap*( 0.0); 
		CC(m551)=   cLap*( 0.0); 
		CC(m112)=   cLap*( 0.0); 
		CC(m212)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m312)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m412)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m512)=   cLap*( 0.0); 
		CC(m122)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m222)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m322)=   cLap*( 0.1148223876953125*dzsqi+0.1148223876953125*dysqi-0.5551528930664063E-1*dxsqi); 
		CC(m422)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m522)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m132)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m232)=   cLap*( 0.1148223876953125*dzsqi-0.5551528930664062E-1*dysqi+0.1148223876953125*dxsqi); 
		CC(m332)=   cLap*( 0.4702250162760417*dzsqi-0.2273483276367187*dysqi-0.2273483276367188*dxsqi); 
		CC(m432)=   cLap*( 0.1148223876953125*dzsqi-0.5551528930664062E-1*dysqi+0.1148223876953125*dxsqi); 
		CC(m532)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m142)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m242)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m342)=   cLap*( 0.1148223876953125*dzsqi+0.1148223876953125*dysqi-0.5551528930664063E-1*dxsqi); 
		CC(m442)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m542)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m152)=   cLap*( 0.0); 
		CC(m252)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m352)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m452)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m552)=   cLap*( 0.0); 
		CC(m113)=   cLap*( 0.0); 
		CC(m213)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m313)=   cLap*( -0.4702250162760417E-2*dysqi); 
		CC(m413)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m513)=   cLap*( 0.0); 
		CC(m123)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m223)=   cLap*( -0.5551528930664062E-1*dzsqi+0.1148223876953125*dysqi+0.1148223876953125*dxsqi); 
		CC(m323)=   cLap*( -0.2273483276367187*dzsqi+0.4702250162760417*dysqi-0.2273483276367188*dxsqi); 
		CC(m423)=   cLap*( -0.5551528930664062E-1*dzsqi+0.1148223876953125*dysqi+0.1148223876953125*dxsqi); 
		CC(m523)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m133)=   cLap*( -0.4702250162760417E-2*dxsqi); 
		CC(m233)=   cLap*( -0.2273483276367187*dzsqi-0.2273483276367187*dysqi+0.4702250162760417*dxsqi); 
		CC(m333)=cI+cLap*( -0.9310455322265625*dzsqi-0.9310455322265625*dysqi-0.9310455322265625*dxsqi); 
		CC(m433)=   cLap*( -0.2273483276367187*dzsqi-0.2273483276367187*dysqi+0.4702250162760417*dxsqi); 
		CC(m533)=   cLap*( -0.4702250162760417E-2*dxsqi); 
		CC(m143)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m243)=   cLap*( -0.5551528930664062E-1*dzsqi+0.1148223876953125*dysqi+0.1148223876953125*dxsqi); 
		CC(m343)=   cLap*( -0.2273483276367187*dzsqi+0.4702250162760417*dysqi-0.2273483276367188*dxsqi); 
		CC(m443)=   cLap*( -0.5551528930664062E-1*dzsqi+0.1148223876953125*dysqi+0.1148223876953125*dxsqi); 
		CC(m543)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m153)=   cLap*( 0.0); 
		CC(m253)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m353)=   cLap*( -0.4702250162760417E-2*dysqi); 
		CC(m453)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m553)=   cLap*( 0.0); 
		CC(m114)=   cLap*( 0.0); 
		CC(m214)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m314)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m414)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m514)=   cLap*( 0.0); 
		CC(m124)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m224)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m324)=   cLap*( 0.1148223876953125*dzsqi+0.1148223876953125*dysqi-0.5551528930664063E-1*dxsqi); 
		CC(m424)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m524)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m134)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m234)=   cLap*( 0.1148223876953125*dzsqi-0.5551528930664062E-1*dysqi+0.1148223876953125*dxsqi); 
		CC(m334)=   cLap*( 0.4702250162760417*dzsqi-0.2273483276367187*dysqi-0.2273483276367188*dxsqi); 
		CC(m434)=   cLap*( 0.1148223876953125*dzsqi-0.5551528930664062E-1*dysqi+0.1148223876953125*dxsqi); 
		CC(m534)=   cLap*( -0.1148223876953125E-2*dxsqi); 
		CC(m144)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m244)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m344)=   cLap*( 0.1148223876953125*dzsqi+0.1148223876953125*dysqi-0.5551528930664063E-1*dxsqi); 
		CC(m444)=   cLap*( 0.2803802490234375E-1*dzsqi+0.2803802490234375E-1*dysqi+0.2803802490234375E-1*dxsqi); 
		CC(m544)=   cLap*( -0.2803802490234375E-3*dxsqi); 
		CC(m154)=   cLap*( 0.0); 
		CC(m254)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m354)=   cLap*( -0.1148223876953125E-2*dysqi); 
		CC(m454)=   cLap*( -0.2803802490234375E-3*dysqi); 
		CC(m554)=   cLap*( 0.0); 
		CC(m115)=   cLap*( 0.0); 
		CC(m215)=   cLap*( 0.0); 
		CC(m315)=   cLap*( 0.0); 
		CC(m415)=   cLap*( 0.0); 
		CC(m515)=   cLap*( 0.0); 
		CC(m125)=   cLap*( 0.0); 
		CC(m225)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m325)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m425)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m525)=   cLap*( 0.0); 
		CC(m135)=   cLap*( 0.0); 
		CC(m235)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m335)=   cLap*( -0.4702250162760417E-2*dzsqi); 
		CC(m435)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m535)=   cLap*( 0.0); 
		CC(m145)=   cLap*( 0.0); 
		CC(m245)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m345)=   cLap*( -0.1148223876953125E-2*dzsqi); 
		CC(m445)=   cLap*( -0.2803802490234375E-3*dzsqi); 
		CC(m545)=   cLap*( 0.0); 
		CC(m155)=   cLap*( 0.0); 
		CC(m255)=   cLap*( 0.0); 
		CC(m355)=   cLap*( 0.0); 
		CC(m455)=   cLap*( 0.0); 
		CC(m555)=   cLap*( 0.0); 

	      }
	      else 
	      {
                // level=10
		CC(m111)=   cLap*( 0.0); 
		CC(m211)=   cLap*( 0.0); 
		CC(m311)=   cLap*( 0.0); 
		CC(m411)=   cLap*( 0.0); 
		CC(m511)=   cLap*( 0.0); 
		CC(m121)=   cLap*( 0.0); 
		CC(m221)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m321)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m421)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m521)=   cLap*( 0.0); 
		CC(m131)=   cLap*( 0.0); 
		CC(m231)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m331)=   cLap*( -0.3616901597491839E-4*dzsqi); 
		CC(m431)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m531)=   cLap*( 0.0); 
		CC(m141)=   cLap*( 0.0); 
		CC(m241)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m341)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m441)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m541)=   cLap*( 0.0); 
		CC(m151)=   cLap*( 0.0); 
		CC(m251)=   cLap*( 0.0); 
		CC(m351)=   cLap*( 0.0); 
		CC(m451)=   cLap*( 0.0); 
		CC(m551)=   cLap*( 0.0); 
		CC(m112)=   cLap*( 0.0); 
		CC(m212)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m312)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m412)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m512)=   cLap*( 0.0); 
		CC(m122)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m222)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m322)=   cLap*( 0.1111472270933889*dzsqi+0.1111472270933889*dysqi-0.5556901293397865E-1*dxsqi); 
		CC(m422)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m522)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m132)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m232)=   cLap*( 0.1111472270933889*dzsqi-0.5556901293397865E-1*dysqi+0.1111472270933889*dxsqi); 
		CC(m332)=   cLap*( 0.4445895443636969*dzsqi-0.2222763697046604*dysqi-0.2222763697046604*dxsqi); 
		CC(m432)=   cLap*( 0.1111472270933889*dzsqi-0.5556901293397865E-1*dysqi+0.1111472270933889*dxsqi); 
		CC(m532)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m142)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m242)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m342)=   cLap*( 0.1111472270933889*dzsqi+0.1111472270933889*dysqi-0.5556901293397865E-1*dxsqi); 
		CC(m442)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m542)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m152)=   cLap*( 0.0); 
		CC(m252)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m352)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m452)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m552)=   cLap*( 0.0); 
		CC(m113)=   cLap*( 0.0); 
		CC(m213)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m313)=   cLap*( -0.3616901597491839E-4*dysqi); 
		CC(m413)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m513)=   cLap*( 0.0); 
		CC(m123)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m223)=   cLap*( -0.5556901293397865E-1*dzsqi+0.1111472270933889*dysqi+0.1111472270933889*dxsqi); 
		CC(m323)=   cLap*( -0.2222763697046604*dzsqi+0.4445895443636969*dysqi-0.2222763697046604*dxsqi); 
		CC(m423)=   cLap*( -0.5556901293397865E-1*dzsqi+0.1111472270933889*dysqi+0.1111472270933889*dxsqi); 
		CC(m523)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m133)=   cLap*( -0.3616901597491839E-4*dxsqi); 
		CC(m233)=   cLap*( -0.2222763697046604*dzsqi-0.2222763697046604*dysqi+0.4445895443636969*dxsqi); 
		CC(m333)=cI+cLap*( -0.889106750695444*dzsqi-0.889106750695444*dysqi-0.889106750695444*dxsqi); 
		CC(m433)=   cLap*( -0.2222763697046604*dzsqi-0.2222763697046604*dysqi+0.4445895443636969*dxsqi); 
		CC(m533)=   cLap*( -0.3616901597491839E-4*dxsqi); 
		CC(m143)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m243)=   cLap*( -0.5556901293397865E-1*dzsqi+0.1111472270933889*dysqi+0.1111472270933889*dxsqi); 
		CC(m343)=   cLap*( -0.2222763697046604*dzsqi+0.4445895443636969*dysqi-0.2222763697046604*dxsqi); 
		CC(m443)=   cLap*( -0.5556901293397865E-1*dzsqi+0.1111472270933889*dysqi+0.1111472270933889*dxsqi); 
		CC(m543)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m153)=   cLap*( 0.0); 
		CC(m253)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m353)=   cLap*( -0.3616901597491839E-4*dysqi); 
		CC(m453)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m553)=   cLap*( 0.0); 
		CC(m114)=   cLap*( 0.0); 
		CC(m214)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m314)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m414)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m514)=   cLap*( 0.0); 
		CC(m124)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m224)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m324)=   cLap*( 0.1111472270933889*dzsqi+0.1111472270933889*dysqi-0.5556901293397865E-1*dxsqi); 
		CC(m424)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m524)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m134)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m234)=   cLap*( 0.1111472270933889*dzsqi-0.5556901293397865E-1*dysqi+0.1111472270933889*dxsqi); 
		CC(m334)=   cLap*( 0.4445895443636969*dzsqi-0.2222763697046604*dysqi-0.2222763697046604*dxsqi); 
		CC(m434)=   cLap*( 0.1111472270933889*dzsqi-0.5556901293397865E-1*dysqi+0.1111472270933889*dxsqi); 
		CC(m534)=   cLap*( -0.9042241058687672E-5*dxsqi); 
		CC(m144)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m244)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m344)=   cLap*( 0.1111472270933889*dzsqi+0.1111472270933889*dysqi-0.5556901293397865E-1*dxsqi); 
		CC(m444)=   cLap*( 0.2778676702402024E-1*dzsqi+0.2778676702402024E-1*dysqi+0.2778676702402024E-1*dxsqi); 
		CC(m544)=   cLap*( -0.2260557030916062E-5*dxsqi); 
		CC(m154)=   cLap*( 0.0); 
		CC(m254)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m354)=   cLap*( -0.9042241058687672E-5*dysqi); 
		CC(m454)=   cLap*( -0.2260557030916062E-5*dysqi); 
		CC(m554)=   cLap*( 0.0); 
		CC(m115)=   cLap*( 0.0); 
		CC(m215)=   cLap*( 0.0); 
		CC(m315)=   cLap*( 0.0); 
		CC(m415)=   cLap*( 0.0); 
		CC(m515)=   cLap*( 0.0); 
		CC(m125)=   cLap*( 0.0); 
		CC(m225)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m325)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m425)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m525)=   cLap*( 0.0); 
		CC(m135)=   cLap*( 0.0); 
		CC(m235)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m335)=   cLap*( -0.3616901597491839E-4*dzsqi); 
		CC(m435)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m535)=   cLap*( 0.0); 
		CC(m145)=   cLap*( 0.0); 
		CC(m245)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m345)=   cLap*( -0.9042241058687672E-5*dzsqi); 
		CC(m445)=   cLap*( -0.2260557030916062E-5*dzsqi); 
		CC(m545)=   cLap*( 0.0); 
		CC(m155)=   cLap*( 0.0); 
		CC(m255)=   cLap*( 0.0); 
		CC(m355)=   cLap*( 0.0); 
		CC(m455)=   cLap*( 0.0); 
		CC(m555)=   cLap*( 0.0); 


	      }
	      
	    }
	    else
	    {
	      Overture::abort("ERROR: invalid orderOfAccuacy");
	    }

	  }
	  else
	  {
            // 1D
	    real dxsqi=1./(dx[0]*dx[0]);
	    const int m1=0;                   // MCE(-1, 0, 0)
	    const int m2=1;                   // MCE( 0, 0, 0)
	    const int m3=2;                   // MCE(+1, 0, 0)
	    CC(m1)=   cLap*( dxsqi );
	    CC(m2)=cI+cLap*( -2.*dxsqi );
	    CC(m3)=   cLap*( dxsqi );
	  }
	  
	  
	}
	if( parameters.saveGridCheckFile )
	{
	  // -- save the coarse grid equatons in the grid check file for regression tests ---
	  if( myid==0 ) 
	    assert( gridCheckFile!=NULL );
	  fPrintF(gridCheckFile,"\n");
	  fPrintF(gridCheckFile,"Averaged (constant) matrix coeffs, level=%i, grid=%i: \n"
                  " c = ",level,grid);
          for( int m=0; m<stencilSize; m++ )
            fPrintF(gridCheckFile,"%9.2e ",CC(m));
	}

      } // end for grid
    } // end for level

    return 0;
  }
  else if( equationToSolve==OgesParameters::divScalarGradOperator ||
           equationToSolve==OgesParameters::variableHeatEquationOperator ||
           equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
  {
    // nothing to do here 
    // variable coefficient array will be averaged elsewhere
    return 0;
  }
  else if( equationToSolve!=OgesParameters::userDefined )
  {
    printf("Ogmg::initializeConstantCoefficients:ERROR: unknown equationToSolve=%i\n",(int)equationToSolve);
    Overture::abort("error");
  }

#undef CC  




  // Only check the coarse grid!
  const int level=mgcg.multigridLevel[0].numberOfMultigridLevels()-1;
  
  isConstantCoefficients.redim(mgcg.multigridLevel[level].numberOfComponentGrids());
  isConstantCoefficients=FALSE;
  const int stencilSize=int( pow(3,mgcg.numberOfDimensions()) );
  constantCoefficients.redim(stencilSize,mgcg.multigridLevel[level].numberOfComponentGrids());
  constantCoefficients=0.;
  
    

  if( this )  // ******  don't do for now. Need to finish 3d
    return 0;
  
  Index I1,I2,I3;
  for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
  {

    realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
    if( c.getOperators()->isRectangular() )
    {
      isConstantCoefficients(grid)=TRUE;
      
      MappedGrid & mg = mgcg.multigridLevel[level][grid];  

      getIndex(mg.extendedIndexRange(),I1,I2,I3,-1);  // avoid boundaries
      intArray & mask = mg.mask()(I1,I2,I3) > 0 ;
      mask.reshape(1,I1,I2,I3);
      for( int m=0; m<stencilSize; m++ )
      {
        real cMax=-1., cMin=1.;
	where( mask )
	{
	  cMin=min(c(m,I1,I2,I3));
	  cMax=max(c(m,I1,I2,I3));
	}
        printf(" >>>> grid=%i cMax=%e, cMin=%e \n",grid,cMax,cMin);
	
	if( fabs(cMax-cMin)<=fabs(cMax)*REAL_EPSILON*10. )
	{
	  constantCoefficients(m,grid)=cMax;
	}
	else
	{
	  isConstantCoefficients(grid)=FALSE;
	  break;
	}
      }
      if( isConstantCoefficients(grid) )
	printf("Ogmg: Grid %i is rectangular and constant coefficients (name=%s)\n",grid,
	       (const char*)mg.mapping().getName(Mapping::mappingName));

    }
  }
  
  return 0;
}


#undef C
#undef M123
