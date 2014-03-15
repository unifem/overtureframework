// This file automatically generated from boundaryConditions.bC with bpp.
#include "Ogmg.h"
#include "SparseRep.h"
#include "display.h"

#include "OGPolyFunction.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

real timeForBCWhere=0.;
real timeForNeumannBC=0.;
real timeForBC=0.;
real timeForFinishBC=0.;
real timeForGeneralNeumannBC=0.;
real timeForExtrapolationBC=0.;
real timeForSetupBC=0.;
real timeForBCOpt=0.;
real timeForBC4Extrap=0.;
real timeForBCFinal=0.;
real timeForBCUpdateGeometry=0.;


#define bcOpt EXTERN_C_NAME(bcopt)
#define gdExact EXTERN_C_NAME(gdexact)

extern "C"
{

    void bcOpt( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                                              const int &nd3a, const int &nd3b,
                   		       const int &n1a, const int &n1b,
                                              const int &n2a, const int &n2b,
                                              const int &n3a, const int &n3b,
                                              const int &ndc, const real & c,
                                              const real & u, const real & f, const int & mask, 
                                              const real & rsxy, const real & xy, 
                                              const int & bc, 
                                              const int & boundaryCondition, const int & ipar, const real & rpar );


// return a general derivative of the exact solution
    void gdExact(const int & ntd, const int & nxd, const int & nyd, const int & nzd,
             	       const real & x, const real & y, const real & z, const int & n, const real & t, real & value )
    {
        assert( Ogmg::pExactSolution!=NULL );
        value= (*Ogmg::pExactSolution).gd(ntd,nxd,nyd,nzd,x,y,z,n,t);
    }
    
}


//\begin{>>OgmgInclude.tex}{\subsection{getGhostLineBoundaryCondition}}
OgmgParameters::FourthOrderBoundaryConditionEnum Ogmg::
getGhostLineBoundaryCondition( int bc, int ghostLine, int grid, int level, 
                                                              int & orderOfExtrapolation, aString *bcName /* =NULL */ ) const
//==================================================================================
// /Description:
//   Get the boundary condition to use on a ghost line.
// /bc (input) : A boundary condition
//  /ghostLine,grid,level : get BC for this grid and face
// /orderOfExtrapolation (output) : For an extrapolation BC this is the order
// /bcName (output): If non-NULL on input, return the name of the BC.
// /Ouptut: The BC to use is from the enum in OgmgParameters
// \begin{verbatim}
//  enum FourthOrderBoundaryConditionEnum
//  {
//    useSymmetry,
//    useEquationToFourthOrder,
//    useEquationToSecondOrder,
//    useExtrapolation
//  };
// \end{verbatim}
//\end{OgmgInclude.tex} 
//==================================================================================
{
    OgmgParameters::FourthOrderBoundaryConditionEnum returnValue=OgmgParameters::useUnknown; // bogus value
    if( bcName!=NULL ) *bcName="unknown ";

    const CompositeGrid & mgcg = multigridCompositeGrid();
    MappedGrid & mg = mgcg.multigridLevel[level][grid];

    assert( ghostLine>=1 && ghostLine<=2 );
    
    if( bc==OgmgParameters::extrapolate )
    {
    // ****************************************************
    // ******************Dirichlet*************************
    // ****************************************************
        if( orderOfAccuracy==2 )
        {
            returnValue=OgmgParameters::useExtrapolation;
            orderOfExtrapolation=3; // check this
        }
        else if( orderOfAccuracy==4 )
        {
            if( level==levelZero )
            {
                orderOfExtrapolation=parameters.orderOfExtrapolationForDirichlet;  // check this
      	if( ghostLine==1 )
      	{
          // *wdh* 110223 -- The dirichletFirstGhostLineBC should now be adjusted properly by checkParameters
          // *fix me*

        	  if( equationToSolve==OgesParameters::laplaceEquation ) // *wdh* added 100414
                        returnValue=parameters.dirichletFirstGhostLineBC;
                    else
                        returnValue=OgmgParameters::useExtrapolation;
                    returnValue=parameters.dirichletFirstGhostLineBC;
      	}
      	else if( ghostLine==2 )
                    returnValue=parameters.dirichletSecondGhostLineBC;
      	else
        	  Overture::abort("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown ghost line!");
            }
            else 
            { // lower levels:
                orderOfExtrapolation=parameters.orderOfExtrapolationForDirichletOnLowerLevels;
      	if( ghostLine==1 )
      	{
        	  returnValue=parameters.lowerLevelDirichletFirstGhostLineBC;
                    if( parameters.lowerLevelDirichletFirstGhostLineBC==OgmgParameters::useExtrapolation &&
                            parameters.useEquationForDirichletOnLowerLevels==2 && 
                            mg.isRectangular() && // add for now
                            equationToSolve==OgesParameters::laplaceEquation ) // *wdh* added 100414
        	  {
                        returnValue=OgmgParameters::useEquationToSecondOrder;
        	  }
      	}
      	else if( ghostLine==2 )
        	  returnValue=parameters.lowerLevelDirichletSecondGhostLineBC;
      	else
      	{
        	  OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown ghost line!");
      	}
            }
            if(  bcName!=NULL ) 
            {
      	if( returnValue==OgmgParameters::useEquationToFourthOrder )
        	  *bcName="PDE4    ";
      	else if( returnValue==OgmgParameters::useEquationToSecondOrder )
        	  *bcName="PDE2    ";
            }
        }
        else
        {
            OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown orderOfAccuracy");
        }  


//        if( ghostLine==2 )
//        { // the 2nd ghost line is always extrapolated  -- these values are not used ---
//          orderOfExtrapolation=orderOfExtrapolationForDirichletOnLowerLevels;  // check this
//          returnValue=OgmgParameters::useExtrapolation;
//        }
//        else if( parameters.fourthOrderBoundaryConditionOption==0 )
//        {
//          returnValue=OgmgParameters::useExtrapolation;
//        }
//        else if( parameters.fourthOrderBoundaryConditionOption==1 ) 
//        {
//  	if( level==levelZero )
//  	{
//  	  returnValue=OgmgParameters::useEquationToSecondOrder;
//  	  if( bcName!=NULL ) *bcName="PDE2    ";
//  	}
//  	else // l>0 
//  	{

//  	  if( parameters.useSymmetryForDirichletOnLowerLevels )
//  	  {
//  	    returnValue=OgmgParameters::useSymmetry;
//  	  }
//            else if( parameters.useEquationForDirichletOnLowerLevels )
//  	  {
//  	    returnValue=OgmgParameters::useEquationToSecondOrder;
//  	    if( bcName!=NULL ) *bcName="PDE2    ";
//  	  }
//  	  else
//  	  {
//              returnValue=OgmgParameters::useExtrapolation;
//  	  }
//  	}
//        }
//        else
//        {
//  	Overture::abort("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown fourthOrderBoundaryConditionOption.");
//        }
//      }

    }
    else if( bc==OgmgParameters::equation )
    {
    // ****************************************************
    // ******************Neumann***************************
    // ****************************************************
        if( orderOfAccuracy==2 )
        {
            if( level==levelZero )
            {
                orderOfExtrapolation=parameters.orderOfExtrapolationForNeumann;  // *wdh* 110222
      	if( ghostLine==1 )
      	{
                    if( parameters.neumannFirstGhostLineBC==OgmgParameters::useEquationToFourthOrder ||
                            parameters.neumannFirstGhostLineBC==OgmgParameters::useEquationToSecondOrder )
          	    returnValue=OgmgParameters::useEquationToSecondOrder;

        	  if( bcName!=NULL )
        	  {
          	    if( returnValue==OgmgParameters::useEquationToSecondOrder )
            	      *bcName="N2      ";
                        else
                            *bcName="???     ";
        	  }
      	}
      	else
      	{
        	  OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown ghost line!");
      	}
            }
            else 
            { // lower levels:
                orderOfExtrapolation=parameters.orderOfExtrapolationForNeumannOnLowerLevels; // *wdh* 110222
      	if( ghostLine==1 )
      	{
          	  returnValue=parameters.lowerLevelNeumannFirstGhostLineBC; // This means mixedToSecondOrder
          // if( parameters.lowerLevelNeumannFirstGhostLineBC==OgmgParameters::useEquationToSecondOrder )
	  //   returnValue=mixedToSecondOrder;
      	}
      	else
      	{
        	  OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown ghost line!");
      	}
      	
      	if( bcName!=NULL )
      	{
        	  if( returnValue==OgmgParameters::useEquationToSecondOrder )
          	    *bcName="M2      ";  // Used a mixed-symmetry condition 
        	  else
          	    *bcName="???     ";
      	}
            }
        }
        else if( orderOfAccuracy==4 )
        {
            orderOfExtrapolation=parameters.orderOfExtrapolationForNeumann;  // *wdh* 110222
            if( level==levelZero )
            {
      	if( ghostLine==1 )
      	{
        	  returnValue=parameters.neumannFirstGhostLineBC;
        	  if( bcName!=NULL )
        	  {
          	    if( returnValue==OgmgParameters::useEquationToFourthOrder )
            	      *bcName="N4      ";
          	    else if( returnValue==OgmgParameters::useEquationToSecondOrder )
            	      *bcName="N2      ";
        	  }
      	}
      	else if( ghostLine==2 )
      	{
        	  returnValue=parameters.neumannSecondGhostLineBC;
        	  if( bcName!=NULL )
        	  {
          	    if( returnValue==OgmgParameters::useEquationToSecondOrder )
            	      *bcName="PDE.n2  ";
          	    else if( returnValue==OgmgParameters::useEquationToFourthOrder )
            	      *bcName="N4      ";
        	  }
      	}
      	else
      	{
        	  OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown ghost line!");
      	}
            }
            else 
            { // lower levels:
                orderOfExtrapolation=parameters.orderOfExtrapolationForNeumannOnLowerLevels; // *wdh* 110222
      	if( ghostLine==1 )
        	  returnValue=parameters.lowerLevelNeumannFirstGhostLineBC;
      	else if( ghostLine==2 )
        	  returnValue=parameters.lowerLevelNeumannSecondGhostLineBC;
      	else
      	{
        	  OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown ghost line!");
      	}
      	if( bcName!=NULL )
      	{
        	  if( returnValue==OgmgParameters::useEquationToFourthOrder )
          	    *bcName="N4      ";
        	  else if( returnValue==OgmgParameters::useEquationToSecondOrder )
          	    *bcName="N2      ";
      	}
            }
        }
        else
        {
            OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown orderOfAccuracy");
        } 
        
    }
    else
    {
        printf("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown boundary condition=%i \n",bc);
        OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: unknown boundary condition.");
    }

    if( returnValue==OgmgParameters::useUnknown )
    {
        printf("Ogmg:getGhostLineBoundaryCondition:ERROR: no boundary condition was assigned!\n");
        OV_ABORT("Ogmg:getGhostLineBoundaryCondition:ERROR: no boundary condition was assigned!");
    }
    

  // assign a name if requested
    if( bcName!=NULL )
    { // Here we assign the common names
        if( returnValue==OgmgParameters::useExtrapolation )
            sPrintF(*bcName,"extrap%i ",orderOfExtrapolation);
        else if( returnValue==OgmgParameters::useSymmetry )
        {
            if( bc==OgmgParameters::extrapolate )
              *bcName="odd     ";  
            else if( bc==OgmgParameters::equation )
            {
                *bcName="even    "; 
            }
        }
    }
    
    return returnValue;
}



//\begin{>>OgmgInclude.tex}{\subsection{useEquationOnGhostLineForDirichletBC}}
bool Ogmg::
useEquationOnGhostLineForDirichletBC(MappedGrid & mg, int level)
//==================================================================================
// /Description:
//    Return true if the eqn to 2nd order should be applied on the ghost line
// of a dirichlet boundary.
// /mg (input) : grid to use
//\end{OgmgInclude.tex} 
//==================================================================================
{
    
    bool useEquationOnGhost = (level==0 ||
                       			     parameters.useEquationForDirichletOnLowerLevels==1 ||
                       			     (parameters.useEquationForDirichletOnLowerLevels==2 && mg.isRectangular()) );


    useEquationOnGhost=(useEquationOnGhost && 
                  		      orderOfAccuracy==4 && 
                  		      equationToSolve!=OgesParameters::userDefined &&
                  		      parameters.fourthOrderBoundaryConditionOption==1);

    return useEquationOnGhost;
    
}

//\begin{>>OgmgInclude.tex}{\subsection{useEquationOnGhostLineForNeumannBC}}
bool Ogmg::
useEquationOnGhostLineForNeumannBC(MappedGrid & mg, int level)
//==================================================================================
// /Description:
//    Return true if the eqn to 2nd order should be applied on the ghost line
// of a Neumann or mixed boundary.
// /mg (input) : grid to use
//\end{OgmgInclude.tex} 
//==================================================================================
{
    
    bool useEquationOnGhost = (level==0 ||
                       			     parameters.useEquationForNeumannOnLowerLevels==1 ||
                       			     (parameters.useEquationForNeumannOnLowerLevels==2 && mg.isRectangular()) );
    useEquationOnGhost=(useEquationOnGhost && 
                  		      orderOfAccuracy==4 && 
                  		      equationToSolve!=OgesParameters::userDefined &&   
                  		      parameters.fourthOrderBoundaryConditionOption==1);

    return useEquationOnGhost;
    
}







//\begin{>>OgmgInclude.tex}{\subsection{initializeBoundaryConditions}}
int Ogmg::
initializeBoundaryConditions(realCompositeGridFunction & coeff)
// ==========================================================================================
// /Description:
//    Determine the type of boundary condition that is imposed on the **GHOSTLINE**
//    for each side of each grid.
// There are 3 possibilities:
// \begin{enumerate}
//   \item extrapolation : ghost point is extrapolated. This requires a special formula for the defect.
//   \item equation : an equation such as a neumann or mixed boundary condition. This uses basically the
//                same formula for the defect, but shifted to be centred on the boundary
//   \item combination : a combination of the above two appears on the boundary.
// /Notes:
//    We check the classify array to determine the type of boundary condition.
// \end{enumerate}
//\end{OgmgInclude.tex} 
// ==========================================================================================
{
    if( equationToSolve!=OgesParameters::userDefined )
    {
    // boundary conditions have already been initialized in this case
        return 0;
    }

    const int level=0;
    
    CompositeGrid & mgcg = multigridCompositeGrid();
    boundaryCondition.redim(2,3,mgcg.multigridLevel[level].numberOfComponentGrids());
    boundaryCondition=0;
    
    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = mgcg.multigridLevel[level][grid];  

        realMappedGridFunction & c = coeff[grid];
        assert( c.sparse!=0 );
        intArray & classify = c.sparse->classify;

    // **** Boundary conditions *****
        Index I1b,I2b,I3b, I1m,I2m,I3m;
        for(int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
        {
            for( int side=Start; side<=End; side++ )
            {
      	if( mg.boundaryCondition(side,axis) > 0 )
      	{
        	  getGhostIndex( mg.extendedIndexRange(),side,axis,I1b,I2b,I3b, 0); // boundary line
        	  getGhostIndex( mg.extendedIndexRange(),side,axis,I1m,I2m,I3m,+1); // first ghost line
                
        	  int extrapolate = sum( classify(I1m,I2m,I3m)==SparseRepForMGF::extrapolation );
        	  int ghost1 = sum( classify(I1m,I2m,I3m)==SparseRepForMGF::ghost1 );
        	  if( ghost1==0 )
          	    boundaryCondition(side,axis,grid)=OgmgParameters::extrapolate;
        	  else if( extrapolate==0 )
          	    boundaryCondition(side,axis,grid)=OgmgParameters::equation;
        	  else
          	    boundaryCondition(side,axis,grid)=OgmgParameters::combination;

        	  if( debug & 4 )
          	    printf("Ogmg: boundaryCondition(side=%i,axis=%i,grid=%i)=%s \n",side,axis,grid,
               		   (boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate ? "extrapolate" :
                		    boundaryCondition(side,axis,grid)==OgmgParameters::equation ? "equation" : "combination"));
      	}
      	else
      	{
        	  boundaryCondition(side,axis,grid)=0;
      	}
            }
        }
    }
    
    return 0;
}

int Ogmg::
applyInitialConditions()
// ============================================================================================
// /Description:
//   For second-order we apply the initial boundary conditions. 
//   For fourth-order we need to initially set some values at ghost points.
//    
// ============================================================================================
{
    if( true )
    { 
        int level=0;

    // uMG.multigridLevel[level]=0.; 

    // The red-black jacobi smoother relies on parallel ghost values being set in the RHS  *wdh* 100419 
        CompositeGrid & mgcg = multigridCompositeGrid();
        for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
        {
            fMG.multigridLevel[level][grid].updateGhostBoundaries();  
        }
        
    // Always apply the boundary conditions at the start *wdh* 100419 
        applyBoundaryConditions( level, uMG.multigridLevel[level], fMG.multigridLevel[level] );
    }
    


    if( orderOfAccuracy!=4 ) 
        return 0;

    real timeStart=getCPU();
    CompositeGrid & mgcg = multigridCompositeGrid();

    const int level=0;
    
    int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
    const int numberOfDimensions = mgcg.numberOfDimensions();
    Index Ivm[3],  &I1m=Ivm[0], &I2m=Ivm[1], &I3m=Ivm[2];
    const int orderOfExtrapolation=orderOfAccuracy==2 ? 3 : 4; // 5; 
    
    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
        
        MappedGrid & mg = mgcg.multigridLevel[level][grid];  
        
        #ifdef USE_PPP
          realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(uMG[grid],uLocal);
        #else
          realSerialArray & uLocal = uMG[grid];
        #endif

    // For fourth-order accuracy and dirichlet BC's we extrapolate the ghost points on the extended boundary
    //
    //               |                       |
    //               |                       |
    //            O--X---X---X---X---X---X---X--O    <------ dirichlet BC
    //           -1  0   1   2               n  n+1
    //   extrap points -1 and n+1
    //

        for(int axis=0; axis<numberOfDimensions; axis++ )
        {
            for( int side=Start; side<=End; side++ )
            {
      	if( mg.boundaryCondition(side,axis)>0 && boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate )
      	{
          // extrapolate the extended ghost line values on dirichlet walls
                    for( int direction=1; direction<numberOfDimensions; direction++ ) // tangential directions
        	  {
          	    const int axispd=(axis+direction)%numberOfDimensions;
          	    for( int side2=Start; side2<=End; side2++ )
          	    {
            	      if( !mg.isPeriodic(axispd) )
            	      {
            		getBoundaryIndex(mg.gridIndexRange(),side,axis,I1m,I2m,I3m);

            		Ivm[axispd]= side2==0 ? Ivm[axispd].getBase() : Ivm[axispd].getBound();
                                if( numberOfDimensions==3 && direction==2 )
            		{
                  // In 3D we need to get the "corners"  
              		  const int axisp1=(axis+1)%numberOfDimensions;
                                    if( !mg.isPeriodic(axisp1) )
              		  {
                		    Ivm[axisp1]=Range(Ivm[axisp1].getBase()-1,Ivm[axisp1].getBound()+1);
              		  }
            		}
                // fix this: in parallel we should check if the ghost line is on this processor and that
                //    all points in the stencil are available
            		bool ok=ParallelUtility::getLocalArrayBounds(uMG[grid],uLocal,I1m,I2m,I3m,1);
            		if( !ok ) continue;

            		is1=is2=is3=0;
            		is[axispd]=1-2*side2;
            		uLocal(I1m-is1,I2m-is2,I3m-is3)=
              		  ( 5.*(uLocal(I1m,I2m,I3m)-uLocal(I1m+3*is1,I2m+3*is2,I3m+3*is3))
                		    -10.*(uLocal(I1m+is1,I2m+is2,I3m+is3)-uLocal(I1m+2*is1,I2m+2*is2,I3m+2*is3))+
                		    uLocal(I1m+4*is1,I2m+4*is2,I3m+4*is3) );
            		
            	      }
                        }
        	  }
      	}
            }
        }
        if( Ogmg::debug & 16 ) ::display(uMG[grid],"BC: u assign extended boundaries",debugFile,"%5.1f ");
    }

    timeForBC4Extrap+=getCPU()-timeStart;
    return 0;
        
}

int Ogmg::
applyFinalConditions()
// ============================================================================================
// /Description:
//   For fourth-order we need to set some values at ghost points at the end.
//    
// ============================================================================================
{
    if( true ||  orderOfAccuracy!=4 )   // *wdh* we now extrapolate the 2nd ghost in bcOpt
        return 0;

    real timeStart=getCPU();
    CompositeGrid & mgcg = multigridCompositeGrid();

    const int level=0;
    
    int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
    const int numberOfDimensions = mgcg.numberOfDimensions();
    Index Ivm[3],  &I1m=Ivm[0], &I2m=Ivm[1], &I3m=Ivm[2];
    const int orderOfExtrapolation=orderOfAccuracy==2 ? 3 : 4; // 5; 
    
    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
        
        MappedGrid & mg = mgcg.multigridLevel[level][grid];  
        realArray & u = uMG[grid];
        
        for(int axis=0; axis<numberOfDimensions; axis++ )
        {
            for( int side=Start; side<=End; side++ )
            {
      	if( mg.boundaryCondition(side,axis)>0 && boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate )
      	{
          // extrapolate the 2nd ghost line on Dirichlet boundaries.
        	  getGhostIndex(mg.gridIndexRange(),side,axis,I1m,I2m,I3m);
        	  is1=is2=is3=0;
        	  is[axis]=1-2*side;
        	  u(I1m-is1,I2m-is2,I3m-is3)=5.*(u(I1m,I2m,I3m)-u(I1m+3*is1,I2m+3*is2,I3m+3*is3))
          	    -10.*(u(I1m+is1,I2m+is2,I3m+is3)-u(I1m+2*is1,I2m+2*is2,I3m+2*is3))+u(I1m+4*is1,I2m+4*is2,I3m+4*is3);
            		
      	}
            }
        }
    // if( true || Ogmg::debug & 16 ) ::display(u,"BC: u assign extended boundaries",debugFile,"%5.1f ");
    }

    timeForBCFinal+=getCPU()-timeStart;
    return 0;
        
}



//\begin{>>OgmgInclude.tex}{\subsection{applyBoundaryConditions(level,...)}}
int Ogmg::
applyBoundaryConditions( const int & level, RealCompositeGridFunction & u, RealCompositeGridFunction & f )
// ==========================================================================================
// 
// /Description:
//    Assign boundary conditions on the **GHOSTLINE**  for each side of each grid.
//
//\end{OgmgInclude.tex} 
// ==========================================================================================
{
            
    for( int grid=0; grid<u.numberOfComponentGrids(); grid++ )
        applyBoundaryConditions( level,grid,u[grid],f[grid] );

    return 0;
}


#undef C
#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))
// define C(m1,m2,m3,I1,I2,I3) c(I1,I2,I3,M123(m1,m2,m3))
#define C(m1,m2,m3,I1,I2,I3) c(M123(m1,m2,m3),I1,I2,I3)


// ============================================================================================
// **** set exact BC's for debugging ****
// ============================================================================================






// #define getCPUOpt() ( debug & 4 ? getCPU() : 0. )
// #define getCPUOpt() getCPU()
// turn off:
#define getCPUOpt() 0.


// =============================================================================================
/// \brief Set the boundary condition parameters for edges and corners.
// =============================================================================================
int Ogmg::
setCornerBoundaryConditions( BoundaryConditionParameters & bcParams, const int level )
{
    if( parameters.useSymmetryCornerBoundaryCondition )
    {
    // Use a corner BC based on taylor series -- only use the 4th order condition on the finest level
    // for order 4.
        BoundaryConditionParameters::CornerBoundaryConditionEnum cornerBC=
            (orderOfAccuracy==2 || level>0 ) ? BoundaryConditionParameters::taylor2ndOrderEvenCorner :
            BoundaryConditionParameters::taylor4thOrderEvenCorner;

    // cornerBC=BoundaryConditionParameters::taylor2ndOrderEvenCorner;  // *************************
    // cornerBC=BoundaryConditionParameters::taylor2ndOrder;
    // cornerBC=BoundaryConditionParameters::extrapolateCorner;
        
        bcParams.setCornerBoundaryCondition(cornerBC);  // this will set all corners and edges in finishBC below

    }

  //    const int orderOfCornerExtrapolation=5;
  //    bcParams.orderOfExtrapolation=orderOfCornerExtrapolation;
    const int orderOfExtrapolation=orderOfAccuracy==2 ? 3 : 4; 

    bcParams.orderOfExtrapolation=orderOfExtrapolation; // orderOfAccuracy+1;
    
  // only need to assign this many ghost lines at corners:
    bcParams.numberOfCornerGhostLinesToAssign=orderOfAccuracy/2; 
    return 0;
}


//\begin{>>OgmgInclude.tex}{\subsection{applyBoundaryConditions(level,grid,...)}}
int Ogmg::
applyBoundaryConditions(const int & level, 
                   			 const int & grid, 
                   			 RealMappedGridFunction & u, 
                   			 RealMappedGridFunction & f )
// ==========================================================================================
// /Description:
//    Assign boundary conditions on the **GHOSTLINE**  for each side of a grid.
//  Values on the actual boundary are assumed to be done in the smoothing step since the
// coefficient maxtrix should hold the proper equation there (a Dirichlet BC for example).
// /level,grid (input) :
// /u (input/output) : apply BC's to this grid function.
// /f (input): rhs to the equation (needed to compute the defect for non-extrapolation BC's)
//
// There are 3 possibilities:
// \begin{enumerate}
//   \item extrapolation : ghost point is extrapolated. This requires a special formula for the defect.
//   \item equation : an equation such as a neumann or mixed boundary condition. This uses basically the
//                same formula for the defect, but shifted to be centred on the boundary
//   \item combination : a combination of the above two appears on the boundary.
// \end{enumerate}
//\end{OgmgInclude.tex} 
// ==========================================================================================
{
    real time=getCPU(); 

    

    int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
    int ipar[30];
    real rpar[10];
    
    CompositeGrid & mgcg = multigridCompositeGrid();
    MappedGrid & mg = mgcg.multigridLevel[level][grid];  
    const int numberOfDimensions = mg.numberOfDimensions();
    
    const intArray & mask = mg.mask();
    realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
    realArray & defect = defectMG.multigridLevel[level][grid];
    MappedGridOperators & op = *c.getOperators();

    const int isRectangular=op.isRectangular();  // faster than mg
    if( !isRectangular )  // This is not needed if we have just Dirichlet BC's *************************** fix me ****************
    {
        mg.update(MappedGrid::THEinverseVertexDerivative);
        timeForBCUpdateGeometry+= getCPUOpt()-time;
    }

    if( false )
    {
        printF("applyBoundaryConditions: Only call op.finishBoundaryConditions...\n");
        op.finishBoundaryConditions(u,bcParams);
        return 0;
    }


    const int orderOfExtrapolation=orderOfAccuracy==2 ? 3 : 4; // 5; 
    #ifdef USE_PPP
    // We may need to first update ghost boundaries if there are too few points for extrapolation
    // of the ghost points
    //      0  1  2  3  4
    //      +--+--+--+--+
    //      +--+--X         p=0 , X=ghost
    //         X--+--+--+   p=1

    // **************** fix this ******************

        const int numProc= max(1,Communication_Manager::numberOfProcessors());
    //  Min number of points per processor:
        int nppp1 = (mg.gridIndexRange(1,0)-mg.gridIndexRange(0,0))/numProc-1;
        int nppp2 = (mg.gridIndexRange(1,1)-mg.gridIndexRange(0,1))/numProc-1;
        int nppp3 = (mg.gridIndexRange(1,2)-mg.gridIndexRange(0,2))/numProc-1;

        if( (nppp1 < orderOfExtrapolation-1) || 
                (nppp2 < orderOfExtrapolation-1) || 
                (numberOfDimensions==3 && (nppp3 < orderOfExtrapolation-1) ) )
        {
            real time0=getCPUOpt();
            u.updateGhostBoundaries();
            tm[timeForGhostBoundaryUpdate]+=getCPUOpt()-time0;
        }
    #endif


    const realArray & rsxy = isRectangular ? u : mg.inverseVertexDerivative();

    #ifdef USE_PPP
   // these next might be fairly expensive : 
   //realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
   //realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
   //realSerialArray cLocal; getLocalArrayWithGhostBoundaries(c,cLocal);
   // intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
   //realSerialArray rsxyLocal; getLocalArrayWithGhostBoundaries(rsxy,rsxyLocal);
    #else
   //const realSerialArray & uLocal = u;
   //const realSerialArray & fLocal = f;
   //const realSerialArray & cLocal = c;
   // const intSerialArray & maskLocal = mask;
   //const realSerialArray & rsxyLocal = rsxy;
    #endif

  // we must turn off twilightZoneFlow since the RHS is already set for this case. *wdh* 020804
    const int twilightZoneFlow=op.twilightZoneFlow;
    op.setTwilightZoneFlow(false);


    real *pu = u.getDataPointer();

    if( pu!=NULL )
    {

        real *pf = f.getDataPointer();
    // assert( pu==getDataPointer(uLocal) );
        real *pc = c.getDataPointer();
        int *pmask = mask.getDataPointer();
        real *prsxy = isRectangular ? rpar : rsxy.getDataPointer();
        real *pxy = prsxy;
        if( twilightZoneFlow )
        { // for debugging we build the vertex array so that bcOpt can evaluate the exact solution
            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
            pxy = mg.center().getDataPointer();
        }
    // Mv[axis] = bounds on the local mask array with ghost boundaries: 
    // *NOTE* We cannot call getLocalFullRange if the local array is NULL
        Range Mv[3],  &M1=Mv[0], &M2=Mv[1], &M3=Mv[2];
        for( int axis=0; axis<3; axis++)
            Mv[axis]=mask.getLocalFullRange(axis);

    // assert( c.sparse!=0 );
    // intArray & classify = c.sparse->classify;

        Index Ivb[3],  &I1b=Ivb[0], &I2b=Ivb[1], &I3b=Ivb[2];
        Index Ivm[3],  &I1m=Ivm[0], &I2m=Ivm[1], &I3m=Ivm[2];
        bool reshaped=false;
    
    
        const IntegerArray & extended = extendedGridIndexRange(mg); // is this needed ??
        const IntegerArray & d = mg.dimension();
        const int ndc=c.getLength(0);

    // BoundaryConditionParameters bcParams;
    
        RealArray & a = bcParams.a;
        if( a.getLength(0)!=2 )
            a.redim(2);


//  int isRectangular=false;
//  mg.update(MappedGrid::THEinverseVertexDerivative );

        const int gridType = isRectangular ? 0 : 1;
        real dx[3]={1.,1.,1.};
        if( isRectangular )
            mg.getDeltaX(dx);

        const int useForcing = level==levelZero ? 1 : 0;
        real a0=0., a1=1.;
    // if( level>0 ) orderOfExtrapolation=3;

        IntegerArray indexRangeLocal(2,3), dimLocal(2,3), bcLocal(2,3);
        ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,indexRangeLocal,dimLocal,bcLocal );
        const IntegerArray & mgbc =bcLocal;


        aString buff;
        #ifndef USE_PPP
        if( debug & 4 )
            display(u,sPrintF(buff,"applyBC: BEFORE:u level=%i grid=%i",level,grid),debugFile,"%9.2e ");
        #endif
    
    // these next values should agree with the parameters in bcOpt.bf
        const int mixedToSecondOrder=8, evenSymmetry=9, oddSymmetry=10, extrapolateTwoGhostLines=11;



        timeForSetupBC+=getCPUOpt()-time;

        for(int axis=0; axis<numberOfDimensions; axis++ )
        {
            is1=is2=is3=0;
            for( int side=Start; side<=End; side++ )
            {
      	is[axis]=1-2*side;
	// fprintf(pDebugFile,"BC: assign BC: level=%i grid=%i side=%i axis=%i\n",level,grid,side,axis);
      	
	// real timeaa=getCPUOpt();
      	
                #ifndef USE_PPP
      	if( debug & 32 )
        	  display(u,sPrintF(buff,"applyBC:level=%i grid=%i, before (side,axis)=(%i,%i)",level,grid,side,axis),
              		  debugFile,"%7.1e ");
                #endif

      // 	fillBcOptParameters();
                ipar[0]=side;
                ipar[1]=axis;
                ipar[2]=0; // useCoefficients;
                ipar[3]=orderOfExtrapolation;
                ipar[4]=gridType;
                ipar[5]=orderOfAccuracy;
                ipar[6]=0; // useForcing;
                ipar[7]=equationToSolve;
                ipar[8]=parameters.fourthOrderBoundaryConditionOption;
                ipar[9]=parameters.solveEquationWithBoundaryConditions;
                ipar[10]=level;
                ipar[11]=Ogmg::debug;
                ipar[12]=level==levelZero ? parameters.dirichletFirstGhostLineBC : parameters.lowerLevelDirichletFirstGhostLineBC;
                ipar[13]=level==levelZero ? parameters.neumannFirstGhostLineBC   : parameters.lowerLevelNeumannFirstGhostLineBC;
                ipar[14]=level==levelZero ? parameters.dirichletSecondGhostLineBC : parameters.lowerLevelDirichletSecondGhostLineBC;
                ipar[15]=level==levelZero ? parameters.neumannSecondGhostLineBC   : parameters.lowerLevelNeumannSecondGhostLineBC;
                ipar[16]=grid;
                ipar[17]=0;   // isNeumannBC
                ipar[18]=level==levelZero ? parameters.orderOfExtrapolationForNeumann : parameters.orderOfExtrapolationForNeumannOnLowerLevels;
                ipar[19]=myid;
                rpar[0]=dx[0];
                rpar[1]=dx[1];
                rpar[2]=dx[2];
                rpar[3]=0.; // a0
                rpar[4]=0.; // a1
                rpar[5]=mg.gridSpacing(0);
                rpar[6]=mg.gridSpacing(1);
                rpar[7]=mg.gridSpacing(2);
      // not needed  rpar[8]=mg.mapping().getSignForJacobian();
      	
      	bool lineSolveFilledBoundaryValues = false ;  // watch out for zebra -- which direction are we currently doing ?

      	if( boundaryCondition(side,axis,grid)==OgmgParameters::equation && mgbc(side,axis) > 0 )
      	{
	  // *************************************************************************
	  // ********************* NEUMANN, MIXED or EXTRAP **************************
	  // *************************************************************************

	  // **** NOTE *** we just assign the ghostline values here.

          // Sometimes a lineSmooth BC is not consistent with the ones here so this next line can
          // be used to turn these off for testing:
        	  if( false )
        	  {
                        printF("applyBC: skip Neumman BC...\n");
          	    continue;
        	  }
        	  


        	  real timeStart=getCPUOpt();

          // This may not be a true Neumann BC if the user has only supplied coefficients for the BC on the ghost line.
                    const bool isNeumannBC = (equationToSolve!=OgesParameters::userDefined ||	
                            				    (bcSupplied && (bc(side,axis,grid)==OgesParameters::neumann || 
                                                                                                        bc(side,axis,grid)==OgesParameters::mixed )));
                    ipar[17]=isNeumannBC;

                    if( debug & 4 )
        	  {
	    // printF("applyBC: equationToSolve=%i bcSupplied=%i isNeumannBC=%i for "
	    //    "(side,axis,grid)=(%i,%i,%i) level=%i\n",(int)equationToSolve,(int)bcSupplied,(int)isNeumannBC,
            //        side,axis,grid,level);
          	    fPrintF(debugFile,"applyBC: equationToSolve=%i bcSupplied=%i isNeumannBC=%i for "
                                        "(side,axis,grid)=(%i,%i,%i) level=%i\n",(int)equationToSolve,(int)bcSupplied,(int)isNeumannBC,
                                        side,axis,grid,level);
        	  }


                    if( bc(side,axis,grid)==OgesParameters::extrapolate )
        	  {
            // -- extrapolation BC --           *wdh* *new* 100412 

          	    int orderOfExtrapolation=-1;
          	    if( true || level==0 )
                            orderOfExtrapolation=int( boundaryConditionData(0,side,axis,grid) +.5 );
                        else
                            orderOfExtrapolation=getOrderOfExtrapolation(level);
                        if( orderOfExtrapolation<=0 || orderOfExtrapolation>100 )
          	    {
                            printF("applyBC:ERROR:orderOfExtrapolation=%i (side,axis,grid)=(%i,%i,%i) level=%i bcData=%8.2e\n",
                 		     orderOfExtrapolation,   side,axis,grid,level,   boundaryConditionData(0,side,axis,grid));
            	      OV_ABORT("error");
          	    }
          	    

          	    if( debug & 4 )
            	      fPrintF(debugFile,"<<<<<<applyBC: extrapolate:use bcOpt level,grid,side,axis=%i,%i,%i,%i, orderOfExtrapolation=%i\n",
                  		      level,grid,side,axis,orderOfExtrapolation);

	    // do not apply BC on periodic boundaries
          	    getGhostIndex( mg.extendedIndexRange(),side,axis,I1m,I2m,I3m,+1);
	    // *** do NOT extrapolate BC on a boundary if the adjacent boundary has a Dirichlet BC ***
          	    int pnr[6]={0,0,0,0,0,0};
                        #define nr(ks,kd) pnr[(ks)+2*(kd)]
          	    int dir;
          	    for( dir=0; dir<numberOfDimensions; dir++ )
          	    {
            	      nr(0,dir)=Ivm[dir].getBase();
            	      nr(1,dir)=Ivm[dir].getBound();
            	      if( dir!=axis )
            	      {
            		if( mg.boundaryCondition(0,dir)>0  && boundaryCondition(0,dir,grid)==OgmgParameters::extrapolate ) nr(0,dir)++;
            		if( mg.boundaryCondition(1,dir)>0  && boundaryCondition(1,dir,grid)==OgmgParameters::extrapolate ) nr(1,dir)--;
            	      }
          	    }
                        #ifdef USE_PPP
           	     for( dir=0; dir<numberOfDimensions; dir++ )
           	     {
             	       nr(0,dir)=max(nr(0,dir),Mv[dir].getBase()  +mask.getGhostBoundaryWidth(dir));
             	       nr(1,dir)=min(nr(1,dir),Mv[dir].getBound() -mask.getGhostBoundaryWidth(dir));
           	     }
                        #endif

                        int bcType=orderOfAccuracy==2 ? OgmgParameters::extrapolate : extrapolateTwoGhostLines;
           	     
            	    ipar[2]=0; // (do not) useCoefficients, extrapolate directly
                        ipar[3]=orderOfExtrapolation;

          	    bcOpt( numberOfDimensions, 
               		   Mv[0].getBase(),Mv[0].getBound(),
               		   Mv[1].getBase(),Mv[1].getBound(),
               		   Mv[2].getBase(),Mv[2].getBound(),
               		   nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2),
               		   ndc, *pc, *pu,*pf, *pmask, *prsxy, *pxy,
               		   bcType, mgbc(0,0), ipar[0], rpar[0] );


        	  }
        	  else if( false &&  // *wdh* 100615 this case is now done in bcOpt
                                      isNeumannBC && isRectangular && orderOfAccuracy==2 )
        	  {
	    // *******  Predefined equation -- rectangular grid, 2nd-order ***********
          // 	    neumannForPredefinedRectangularSecondOrder();
            // *******  Predefined equation -- rectangular grid ***********
            // apply a neumann or mixed BC on this side.
                        a(0)=boundaryConditionData(0,side,axis,grid);
                        a(1)=boundaryConditionData(1,side,axis,grid);
                        const int lineToAssignSave=bcParams.lineToAssign;
                        bcParams.lineToAssign=1;  // data for Neumann BC is on ghost line
                        if( a(0)==0. && fabs(a(1)-1.)<REAL_EPSILON*2. )
                        { // pure neumann BC
              // fPrintF(debugFile,"<<<<<<apply Neumann: level,grid,side,axis=%i,%i,%i,%i\n",level,grid,side,axis);
              // ::display(f(I1m,I2m,I3m),"f(I1m,I2m,I3m)",debugFile);
                            if( useForcing )
                          	op.applyBoundaryCondition(u,0,BCTypes::neumann,BCTypes::boundary1+side+2*axis,f,0.,bcParams );
                            else
                          	op.applyBoundaryCondition(u,0,BCTypes::neumann,BCTypes::boundary1+side+2*axis,0.,0.,bcParams );
                        }
                        else
                        { // mixed BC
              // fPrintF(debugFile,"<<<<<<apply Mixed: %f*u+%f*u.n level,grid,side,axis=%i,%i,%i,%i\n",a(0),a(1),level,grid,side,axis);
                            if( useForcing )
                            {
                          	op.applyBoundaryCondition(u,0,BCTypes::mixed,BCTypes::boundary1+side+2*axis,f,0.,bcParams ); 
                            }
                            else
                                op.applyBoundaryCondition(u,0,BCTypes::mixed,BCTypes::boundary1+side+2*axis,0.,0.,bcParams ); 
                        }
                        bcParams.lineToAssign=lineToAssignSave; // reset
                        timeForNeumannBC+=getCPU()-timeStart;
        	  }
        	  else
        	  {
          	    if( parameters.useOptimizedVersion && isNeumannBC )
          	    {
	      // ***** Predefined curvilinear or fourth-order ******

            	      a0=boundaryConditionData(0,side,axis,grid);  
            	      a1=boundaryConditionData(1,side,axis,grid);
            	      if( debug & 4 )
            		fPrintF(debugFile,"<<<<<<apply Neumann:use bcOpt level,grid,side,axis=%i,%i,%i,%i, a0=%8.2e, a1=%8.2e\n",
                  			level,grid,side,axis,a0,a1);

	      // Do not apply BC on periodic boundaries
              // NOTE: extendedIndexRange includes ghost points (2 for fourth order)
            	      getGhostIndex( mg.extendedIndexRange(),side,axis,I1m,I2m,I3m,+1);  // ghost line
            	      ipar[2]=1; // useCoefficients;
            	      ipar[6]=useForcing;

            	      rpar[3]=a0;
            	      rpar[4]=a1;

	      // *** do NOT apply the neumann BC on a boundary if the adjacent boundary has a Dirichlet BC ***
            	      int pnr[6]={0,0,0,0,0,0};
                            #undef nr
                            #define nr(ks,kd) pnr[(ks)+2*(kd)]
            	      int dir;
            	      for( dir=0; dir<numberOfDimensions; dir++ )
            	      {
            		nr(0,dir)=Ivm[dir].getBase();
            		nr(1,dir)=Ivm[dir].getBound();
            		if( dir!=axis )
            		{
              		  if( mg.boundaryCondition(0,dir)>0  && boundaryCondition(0,dir,grid)==OgmgParameters::extrapolate ) nr(0,dir)++;
              		  if( mg.boundaryCondition(1,dir)>0  && boundaryCondition(1,dir,grid)==OgmgParameters::extrapolate ) nr(1,dir)--;
                  // *wdh* 2012/04/29 -- reduce bounds by 1 if the adjacent side is interpolation: the
                  //  bounds already include ghost points on these sides.
                                    if( mg.boundaryCondition(0,dir)==0 )  nr(0,dir)++;
                                    if( mg.boundaryCondition(1,dir)==0 )  nr(1,dir)--;

            		}
            	      }
            	      

#ifdef USE_PPP
            	      for( dir=0; dir<numberOfDimensions; dir++ )
            	      {
            		nr(0,dir)=max(nr(0,dir),Mv[dir].getBase()  +mask.getGhostBoundaryWidth(dir));
            		nr(1,dir)=min(nr(1,dir),Mv[dir].getBound() -mask.getGhostBoundaryWidth(dir));
            	      }
#endif
	      // printf("apply BC neuman [%i,%i][%i,%i][%i,%i]\n",nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2));
            	      
            	      int bc0 = boundaryCondition(side,axis,grid);
            	      if( level>0 )
            	      {
            		bc0=(parameters.lowerLevelNeumannFirstGhostLineBC==
                 		     OgmgParameters::useSymmetry ? evenSymmetry :
                 		     parameters.lowerLevelNeumannFirstGhostLineBC==
                 		     OgmgParameters::useEquationToSecondOrder ? mixedToSecondOrder :
                 		     boundaryCondition(side,axis,grid) );
            	      }

            	      if( orderOfAccuracy==4 )
            		ipar[3]=level==0 ? parameters.orderOfExtrapolationForNeumann : parameters.orderOfExtrapolationForNeumannOnLowerLevels;

            	      if( debug & 8 )
            		printF("applyBC: Neumann BC: level=%i bc0=%i, firstGhostBC=%i lowerLevelBC=%i\n",level,bc0,
                   		       (int)parameters.neumannFirstGhostLineBC,(int)parameters.lowerLevelNeumannFirstGhostLineBC );

            	      if( false )
            	      {
                                const IntegerArray & gir = mg.gridIndexRange();
            		printf("Ogmg:apply BC neuman level=%i (side,axis)=(%i,%i) ghost line=[%i,%i][%i,%i][%i,%i]\n"
                   		       "  gir=[%i,%i][%i,%i][%i,%i], array bounds=[%i,%i][%i,%i][%i,%i]\n",
                   		       level,side,axis,nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2),
                   		       gir(0,0),gir(1,0),gir(0,1),gir(1,1),gir(0,2),gir(1,2),
                   		       Mv[0].getBase(),Mv[0].getBound(),Mv[1].getBase(),Mv[1].getBound(),
                                              Mv[2].getBase(),Mv[2].getBound());
            	      }
            	      

            	      bcOpt( numberOfDimensions, 
                 		     Mv[0].getBase(),Mv[0].getBound(),
                 		     Mv[1].getBase(),Mv[1].getBound(),
                 		     Mv[2].getBase(),Mv[2].getBound(),
                 		     nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2),
                 		     ndc, *pc, *pu,*pf, *pmask, *prsxy, *pxy,
                 		     bc0, mgbc(0,0), ipar[0], rpar[0] );
#undef nr   
            	      timeForBCOpt+=getCPUOpt()-timeStart;

            	      if( false )
            	      {
		// assign exact values for testing
            		OGFunction & e = *pExactSolution;
            		u(I1m,I2m,I3m)=e(mg,I1m,I2m,I3m,0,0.);
            		u(I1m-is1,I2m-is2,I3m-is3)=e(mg,I1m-is1,I2m-is2,I3m-is3,0,0.);
            	      }
            	      
          	    }
          	    else
          	    {
	      // userDefined equations
	      // ***** this is slow **** 
            // 	      userDefinedEquationBC();
                        #ifdef USE_PPP
            // we should remove the reshape and optimize the loop below! 
                        OV_ABORT("userDefinedEquationBC: fix me for parallel");
                        #endif
                            if( !reshaped )
                            {
                                reshaped=true;
                              	u.reshape(1,u.dimension(0),u.dimension(1),u.dimension(2));
                              	f.reshape(1,f.dimension(0),f.dimension(1),f.dimension(2));
                              	defect.reshape(1,defect.dimension(0),defect.dimension(1),defect.dimension(2));
                            }
              // printf("Ogmg::applyBC: boundaryCondition(%i,%i,%i)==equation \n",side,axis,grid);
                            getGhostIndex( extended,side,axis,I1b,I2b,I3b, 0); // boundary line
                            getGhostIndex( extended,side,axis,I1m,I2m,I3m,+1); // first ghost line
                            int numberOfIts= 1; // level==0 ? 1 : 10; // **** fix this
                            for( int it=0; it<numberOfIts; it++ )
                            {
                                evaluateTheDefectFormula(level,grid,c,u,f,defect,mg,I1m,I2m,I3m,I1b,I2b,I3b,-1);
                                u(0,I1m,I2m,I3m)=defect(0,I1m,I2m,I3m)/c(M123(-is1,-is2,-is3),I1m,I2m,I3m)+u(0,I1m,I2m,I3m); // @PA
                            }
          	    }
          	    
          	    timeForGeneralNeumannBC+=getCPUOpt()-timeStart;
        	  }
      	}
      	else if( boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate  && mgbc(side,axis) > 0  ) 
	  // *** assumed max order of extrapolation. fix!
      	{
	  // *************************************************************************
	  // *************This usually means a dirichlet BC***************************
	  // *************************************************************************

        	  if( false )
        	  {
                        printF("applyBC: skip Dirichlet BC...\n");
          	    continue;
        	  }

	  // printf("Ogmg::applyBC: boundaryCondition(%i,%i,%i)==extrapolation, isRectangular=%i "
	  //        "equationToSolve=%i\n", side,axis,grid,isRectangular,equationToSolve);

        	  real timeStart=getCPUOpt();
	  // this is still needed for some reason
	  // int useCoefficients=orderOfAccuracy==2 ? 1 : 0 ; // ************************* turn off for 4th order for now
        	  int useCoefficients=1; // 

        	  if( true  || (equationToSolve!=OgesParameters::userDefined && isRectangular) )
        	  {
	    // This MUST match the smoothing function where we may or may not smooth boundaries ***

	    // **** prededefined equation and rectangular ****
	    // For a rectangular grid we must explicitly apply the dirichlet BC (on a curvilinear
	    // grid the dirichlet BC will be in the coefficient matrix)

	    //   *************** apply a dirichlet BC.  *******************

          	    real timea=getCPUOpt();

          	    useCoefficients=0; // do not use coefficients below

          	    getGhostIndex( extended,side,axis,I1b,I2b,I3b, 0); // boundary line

          	    int n1a = max(I1b.getBase() ,Mv[0].getBase()  +mask.getGhostBoundaryWidth(0));
          	    int n1b = min(I1b.getBound(),Mv[0].getBound() -mask.getGhostBoundaryWidth(0));
          	    int n2a = max(I2b.getBase(), Mv[1].getBase()  +mask.getGhostBoundaryWidth(1));
          	    int n2b = min(I2b.getBound(),Mv[1].getBound() -mask.getGhostBoundaryWidth(1));
          	    int n3a = max(I3b.getBase(), Mv[2].getBase()  +mask.getGhostBoundaryWidth(2));
          	    int n3b = min(I3b.getBound(),Mv[2].getBound() -mask.getGhostBoundaryWidth(2));
                        
	    // We need a where statement on this next statement: 
	    // there was trouble on the candle grid since an interpolation point
	    // on the boundary would oscillate between the dirichlet BC and the interpolated value 
          	    const int bcType=OgmgParameters::dirichlet; 
          	    const int useForcing=level==levelZero || useForcingAsBoundaryConditionOnAllLevels;
            	      
	    // real *cp = useCoefficients ? pc : pu;

          	    ipar[2]=useCoefficients;
          	    ipar[6]=useForcing;
          	    bcOpt( numberOfDimensions, 
               		   Mv[0].getBase(),Mv[0].getBound(),
               		   Mv[1].getBase(),Mv[1].getBound(),
               		   Mv[2].getBase(),Mv[2].getBound(),
               		   n1a,n1b,n2a,n2b,n3a,n3b,
               		   ndc,*pc,*pu,*pf, *pmask, *prsxy, *pxy,
               		   bcType, mgbc(0,0), ipar[0], rpar[0] );
          	    
	    // timeForBCWhere+=getCPUOpt()-timea;
          	    timeForBCOpt+=getCPUOpt()-timea;
          	    
        	  }

	  // real *cp = useCoefficients ? getDataPointer(c) : getDataPointer(u);
        	  ipar[2]=useCoefficients;
        	  ipar[6]=useForcing;

        	  const int equationToSecondOrder=7; // this should agree with the value in bcOpt.bf
        	  
        	  real timea=getCPUOpt();
        	  
	  // Now extrapolate the first ghost line (or apply the eqn to 2nd-order for 4th order)
	  //      extrapolate using an explicit formula (useCoefficients=0) or use the coeff array. ***
        	  bool useEquationOnGhost= useEquationOnGhostLineForDirichletBC(mg,level);

	  // Here is the new way
        	  int bc1=boundaryCondition(side,axis,grid);
        	  int ghostLine=1, orderOfExtrapolation1;
        	  int ghostBC1 = getGhostLineBoundaryCondition(bc1,ghostLine,grid,level,orderOfExtrapolation1);

        	  if( false && !(useEquationOnGhost && ghostBC1==OgmgParameters::useEquationToSecondOrder) )
        	  {
          	    printf("ERROR: useEquationOnGhost=%i but ghostBC1=%i\n",useEquationOnGhost,ghostBC1);
          	    assert( useEquationOnGhost && ghostBC1==OgmgParameters::useEquationToSecondOrder );
        	  }
        	  
	  // if( useEquationOnGhost )
        	  if( ghostBC1==OgmgParameters::useEquationToSecondOrder )
        	  {
          	    const int bcType=equationToSecondOrder;

          	    getGhostIndex( mg.gridIndexRange(),side,axis,I1b,I2b,I3b, 0); // boundary line 

	    // ***** 030606: to fix: what if there are interp points on the edge --> we need to extrap points
	    // adjacent to them *****

	    // 1. do not assign ends if adjacent boundaries are "dirichlet" since these values
	    //    are already known.
	    // 2, if adjacent boundary is interpolation, include 2 ghost lines where we will extrapolate the ghost
          	    for( int dir=1; dir<numberOfDimensions; dir++ )
          	    {
            	      int axisp=(axis+dir)%numberOfDimensions;  // tangential direction

            	      int bca=boundaryCondition(Start,axisp,grid);
            	      int ia=Ivb[axisp].getBase();
            	      ia= bca==OgmgParameters::extrapolate ? ia+1 : mg.boundaryCondition(0,axisp)==0 ? ia-2 : ia;

            	      int bcb=boundaryCondition(End,axisp,grid);
            	      int ib=Ivb[axisp].getBound();
            	      ib= bcb==OgmgParameters::extrapolate ? ib-1 : mg.boundaryCondition(1,axisp)==0 ? ib+2 : ib;
            	      Ivb[axisp]=Range(ia,ib);
          	    }
                    
          	    int n1a = max(I1b.getBase() ,Mv[0].getBase()  +mask.getGhostBoundaryWidth(0));
          	    int n1b = min(I1b.getBound(),Mv[0].getBound() -mask.getGhostBoundaryWidth(0));
          	    int n2a = max(I2b.getBase(), Mv[1].getBase()  +mask.getGhostBoundaryWidth(1));
          	    int n2b = min(I2b.getBound(),Mv[1].getBound() -mask.getGhostBoundaryWidth(1));
          	    int n3a = max(I3b.getBase(), Mv[2].getBase()  +mask.getGhostBoundaryWidth(2));
          	    int n3b = min(I3b.getBound(),Mv[2].getBound() -mask.getGhostBoundaryWidth(2));

          	    const int useForcing= level==levelZero;
          	    ipar[6]=useForcing;
          	    bcOpt( numberOfDimensions, 
               		   Mv[0].getBase(),Mv[0].getBound(),
               		   Mv[1].getBase(),Mv[1].getBound(),
               		   Mv[2].getBase(),Mv[2].getBound(),
               		   n1a,n1b,n2a,n2b,n3a,n3b,
               		   ndc,*pc,*pu,*pf,*pmask, *prsxy, *pxy,
               		   bcType, mgbc(0,0), ipar[0], rpar[0] );

          	    timeForBCOpt+=getCPUOpt()-timea;
          	    if( false ) // warning DO NOT set to TZ for level>0 !
          	    {
            	      mg.update(MappedGrid::THEcenter);
	      // assign exact values for testing
            	      getGhostIndex( mg.gridIndexRange(),side,axis,I1m,I2m,I3m,1); 
            	      OGFunction & e = *pExactSolution;
	      // u(I1m,I2m,I3m)=e(mg,I1m,I2m,I3m,0,0.);
            	      u(I1m-is1,I2m-is2,I3m-is3)=e(mg,I1m-is1,I2m-is2,I3m-is3,0,0.);
          	    }


        	  }
        	  else
        	  {
          	    int bcType= OgmgParameters::extrapolate;
          	    getGhostIndex( extended,side,axis,I1m,I2m,I3m,+1); // first ghost line

	    // we can use explicit extrapolation if the equations are not userDefined:
          	    useCoefficients=useCoefficients && !OgesParameters::userDefined;
          	    ipar[2]=useCoefficients;
          	    ipar[3]=(orderOfAccuracy==2 ? orderOfExtrapolation : 
                 		     level==levelZero ? parameters.orderOfExtrapolationForDirichlet  :
                 		     parameters.orderOfExtrapolationForDirichletOnLowerLevels);

          	    assert( ipar[3]==orderOfExtrapolation1 );

          	    if( !useCoefficients )
          	    {
	      // --- reduce the order of extrapolation if we only have a few grid points ---
	      // this code comes from op/bc/extrapolate.C
            	      int orderOfExtrap=ipar[3];
            	      int line=1;  // ghost line to assign
#ifdef USE_PPP
              // here is how many lines we have for extrapolating in parallel:
                            const int nx=min(mg.gridIndexRange(1,axis),Mv[axis].getBound())-
            		max(mg.gridIndexRange(0,axis),Mv[axis].getBase())  +1; 
#else
                            const int nx=mg.gridIndexRange(End,axis)-mg.gridIndexRange(Start,axis)+1;
#endif
                    
            	      if( orderOfExtrap >= nx+line && nx>0 )
            	      {
		// reduce order of extrap if we only have a few grid points
		// The extrap formula uses "orderOfExtrap+1" grid points -- we should not be coupled to the ghost
		//   points on the opposite boundary
            		orderOfExtrap = nx+line-1;
            		if( debug & 8 )
            		{
#ifdef USE_PPP
              		  fprintf(pDebugFile,"Ogmg:applyBC: WARNING: p=%i reducing order of extrapolation to %i "
                    			  "since number of grid points =%i, grid=%i local bounds=[%i,%i] (for axis=%i)\n",
                    			  myid,orderOfExtrap,nx,grid,
                    			  max(mg.gridIndexRange(0,axis),Mv[axis].getBase()), 
                    			  min(mg.gridIndexRange(1,axis),Mv[axis].getBound()),axis);
#else
              		  fprintf(pDebugFile,"Ogmg:applyBC: WARNING: reducing order of extrapolation to %i "
                    			  "since number of grid points =%i, grid=%i gid=[%i,%i][%i,%i][%i,%i]\n",orderOfExtrap,nx,grid,
                    			  mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),
                    			  mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
                    			  mg.gridIndexRange(0,2),mg.gridIndexRange(1,2) );
#endif
            		}
            	      

            		ipar[3]=orderOfExtrap;
            	      }
          	    }


          	    if( ghostBC1==OgmgParameters::useExtrapolation )
          	    {
            	      bcType=orderOfAccuracy==2 ? OgmgParameters::extrapolate : extrapolateTwoGhostLines;
          	    }
          	    else if( ghostBC1==OgmgParameters::useSymmetry )  // to add this
          	    {
            	      bcType=oddSymmetry;
	      // printf(" ...odd symmetry BC...\n");
          	    }
          	    else
          	    {
            	      Overture::abort("ERROR: unexpected BC for Dirichlet ghostLine=1");
          	    }
          	    
          	    int n1a = max(I1m.getBase() ,Mv[0].getBase()  +mask.getGhostBoundaryWidth(0));
          	    int n1b = min(I1m.getBound(),Mv[0].getBound() -mask.getGhostBoundaryWidth(0));
          	    int n2a = max(I2m.getBase(), Mv[1].getBase()  +mask.getGhostBoundaryWidth(1));
          	    int n2b = min(I2m.getBound(),Mv[1].getBound() -mask.getGhostBoundaryWidth(1));
          	    int n3a = max(I3m.getBase(), Mv[2].getBase()  +mask.getGhostBoundaryWidth(2));
          	    int n3b = min(I3m.getBound(),Mv[2].getBound() -mask.getGhostBoundaryWidth(2));

          	    bcOpt( numberOfDimensions, 
               		   Mv[0].getBase(),Mv[0].getBound(),
               		   Mv[1].getBase(),Mv[1].getBound(),
               		   Mv[2].getBase(),Mv[2].getBound(),
               		   n1a,n1b,n2a,n2b,n3a,n3b,
               		   ndc,*pc,*pu,*pf, *pmask, *prsxy, *pxy,
               		   bcType, mgbc(0,0), ipar[0], rpar[0] );

          	    timeForBCOpt+=getCPUOpt()-timea;
        	  }
        	  
	  // if( Ogmg::debug & 64 ) ::display(u,"BC: u after extrap ghost line 1",debugFile,"%6.4f ");

        	  timeForExtrapolationBC+=getCPUOpt()-timeStart;
      	}
      	else if( mgbc(side,axis) > 0 )
      	{
	  // ************************************************************
	  // ************** bc is not equation or extrap ****************
	  // ************************************************************
#ifdef USE_PPP
        	  Overture::abort("Ogmg:BC: finish this");
#endif

        	  printf(">>>>Ogmg::applyBC: level=%i: boundaryCondition(%i,%i,grid=%i)==%i mgbc=%i (other) \n",
             		 level,side,axis,grid,boundaryCondition(side,axis,grid),mgbc(side,axis));
        	  getGhostIndex( extended,side,axis,I1b,I2b,I3b, 0); // boundary line
        	  getGhostIndex( extended,side,axis,I1m,I2m,I3m,+1); // first ghost line

        	  if( !reshaped )
        	  {
          	    reshaped=true;
          	    u.reshape(1,u.dimension(0),u.dimension(1),u.dimension(2));
          	    f.reshape(1,f.dimension(0),f.dimension(1),f.dimension(2));
          	    defect.reshape(1,defect.dimension(0),defect.dimension(1),defect.dimension(2));
        	  }
	  // assume most points are equation
        	  evaluateTheDefectFormula(level,grid,c,u,f,defect,mg,I1m,I2m,I3m,I1b,I2b,I3b,-1);
        	  u(0,I1m,I2m,I3m)=defect(0,I1m,I2m,I3m)/c(M123(-is1,-is2,-is3),I1m,I2m,I3m)+u(0,I1m,I2m,I3m);  // @PA

	  // now over-write extrapolation points.
        	  const intArray & classifyConst = c.sparse->classify(I1m,I2m,I3m);
        	  IntegerArray & classify = (IntegerArray &)classifyConst;
        	  classify.reshape(1,I1m,I2m,I3m);
        	  where( classify==SparseRepForMGF::extrapolation  )
        	  {
          	    u(0,I1m,I2m,I3m)=-(c(1,I1m,I2m,I3m)*u(0,I1m+  is1,I2m+  is2,I3m+  is3)+  // *fix opp PAW
                         			       c(2,I1m,I2m,I3m)*u(0,I1m+2*is1,I2m+2*is2,I3m+2*is3)+
                         			       c(3,I1m,I2m,I3m)*u(0,I1m+3*is1,I2m+3*is2,I3m+3*is3)+
                         			       c(4,I1m,I2m,I3m)*u(0,I1m+4*is1,I2m+4*is2,I3m+4*is3)
            	      )/c(0,I1m,I2m,I3m);

        	  }
      	}

                #ifndef USE_PPP
      	if( debug & 32 )
        	  display(u,sPrintF(buff,"applyBC:level=%i grid=%i, after (side,axis)=(%i,%i)",level,grid,side,axis),
              		  debugFile,"%7.1e ");
                #endif 

            }  // end for side
        } // end for axis
        if( reshaped )
        {
            real time1=getCPUOpt();
      // printf(" *** reset reshaped \n");
        
            reshaped=false;
            u.reshape(u.dimension(1),u.dimension(2),u.dimension(3));
            f.reshape(f.dimension(1),f.dimension(2),f.dimension(3));
            defect.reshape(defect.dimension(1),defect.dimension(2),defect.dimension(3));
            timeForSetupBC+=getCPUOpt()-time1;
        }
        
    } // end if pu!=NULL
    

  // ***** corners and periodicity are done here *****
    real timeStart=getCPUOpt();
    BoundaryConditionParameters bcParams;  // *wdh* 030325
  // setCornerBoundaryConditions( bcParams );
    
    if( parameters.useSymmetryCornerBoundaryCondition )
    {
    // BoundaryConditionParameters::CornerBoundaryConditionEnum cornerBC=
    //   orderOfAccuracy==2 ? BoundaryConditionParameters::taylor2ndOrderEvenCorner :
    //   BoundaryConditionParameters::taylor4thOrderEvenCorner;

    // *wdh* 100714 -- reduce order of taylor on lower levels
        BoundaryConditionParameters::CornerBoundaryConditionEnum cornerBC=
            ( orderOfAccuracy==2 || level>0) ? BoundaryConditionParameters::taylor2ndOrderEvenCorner :
            BoundaryConditionParameters::taylor4thOrderEvenCorner;

    // cornerBC=BoundaryConditionParameters::taylor2ndOrderEvenCorner;  // *************************
    // cornerBC=BoundaryConditionParameters::taylor2ndOrder;
    // cornerBC=BoundaryConditionParameters::extrapolateCorner;
        if( level>0 )
        {
      // cornerBC=BoundaryConditionParameters::evenSymmetryCorner;
      // cornerBC=BoundaryConditionParameters::oddSymmetryCorner;
      // cornerBC=BoundaryConditionParameters::taylor2ndOrderEvenCorner;
      // cornerBC=BoundaryConditionParameters::extrapolateCorner;
      // cornerBC=BoundaryConditionParameters::taylor4thOrderEvenCorner;
        }
  
        bcParams.setCornerBoundaryCondition(cornerBC);  // this will set all corners and edges in finishBC below

    // ?? we should not apply a corner BC if an adjacent side is not a physical boundary *wdh* 100610
    //  cf. sbse1.order4

    // BoundaryConditionParameters::CornerBoundaryConditionEnum interpCornerBC = BoundaryConditionParameters::doNothingCorner;
    // *wdh* 100715:  **check me**
        BoundaryConditionParameters::CornerBoundaryConditionEnum interpCornerBC = BoundaryConditionParameters::extrapolateCorner;
        

        if( numberOfDimensions==2 )
        {
            for( int side2=0; side2<=1; side2++ )for( int side1=0; side1<=1; side1++ )
      	if( mg.boundaryCondition(side1,0)<=0 || mg.boundaryCondition(side2,1)<=0 )
        	  bcParams.setCornerBoundaryCondition(interpCornerBC,side1,side2);
        }
        else
        {
      // Edges: 
            for( int side1=0; side1<=1; side1++ )for( int side2=0; side2<=1; side2++ )
      	if( mg.boundaryCondition(side1,0)<=0 || mg.boundaryCondition(side2,1)<=0 )
        	  bcParams.setCornerBoundaryCondition(interpCornerBC,side1,side2,-1);

            for( int side1=0; side1<=1; side1++ )for( int side3=0; side3<=1; side3++ )
      	if( mg.boundaryCondition(side1,0)<=0 || mg.boundaryCondition(side3,2)<=0 )
        	  bcParams.setCornerBoundaryCondition(interpCornerBC,side1,-1,side3);

            for( int side2=0; side2<=1; side2++ )for( int side3=0; side3<=1; side3++ )
      	if( mg.boundaryCondition(side2,1)<=0 || mg.boundaryCondition(side3,2)<=0 )
        	  bcParams.setCornerBoundaryCondition(interpCornerBC,-1,side2,side3);

      // Corners: 
            for( int side1=0; side1<=1; side1++ )for( int side2=0; side2<=1; side2++ )for( int side3=0; side3<=1; side3++ )
      	if( mg.boundaryCondition(side1,0)<=0 || mg.boundaryCondition(side2,1)<=0 || mg.boundaryCondition(side3,2)<=0 )
        	  bcParams.setCornerBoundaryCondition(interpCornerBC,side1,side2,side3);
        }
  
  

    }
    bcParams.orderOfExtrapolation=orderOfExtrapolation; // orderOfAccuracy+1;

  //    const int orderOfCornerExtrapolation=5;
  //    bcParams.orderOfExtrapolation=orderOfCornerExtrapolation;

  // only need to assign this many ghost lines at corners:
    bcParams.numberOfCornerGhostLinesToAssign=orderOfAccuracy/2; 

    if( true  )
    {
        if( debug & 4 )
            ::display(u,sPrintF("BC: u before finishBC, grid=%i",grid),debugFile,"%9.2e ");

    // This next call includes u.periodicUpdate and u.updateGhostBoundaries : 

        op.finishBoundaryConditions(u,bcParams);

        
        if( debug & 4 )
            ::display(u,"BC: u after finishBC",debugFile,"%9.2e ");
    }
    else
    {
          OGFunction & e = *pExactSolution;
          Index I1,I2,I3;
          getIndex( mg.dimension(),I1,I2,I3);
          realArray ee(I1,I2,I3);
          ee=e(mg,I1,I2,I3,0,0.);

        int i3=0;

        int i1=mg.gridIndexRange(0,0);
        int i2=mg.gridIndexRange(1,1);
        
        u(i1-1,i2+1,i3)=ee(i1-1,i2+1,i3); // 0;
        u(i1-2,i2+1,i3)=ee(i1-2,i2+1,i3); //0.;
        u(i1-1,i2+2,i3)=ee(i1-1,i2+2,i3); //0.;
        u(i1-2,i2+2,i3)=ee(i1-2,i2+2,i3); //0.;

          ::display(u,"BC: u before finishBC",debugFile,"%7.4f ");
          op.finishBoundaryConditions(u,bcParams);
          ::display(u,"BC: u after finishBC",debugFile,"%7.4f ");

      #ifdef USE_PPP

        real time0=getCPUOpt();
        u.updateGhostBoundaries();
        tm[timeForGhostBoundaryUpdate]+=getCPUOpt()-time0;

      #endif
    }
    
    op.setTwilightZoneFlow(twilightZoneFlow); // reset


//   if( false && orderOfAccuracy==4 && pExactSolution!=NULL )
//   {
//     setExactFourthOrderBoundaryConditions();
//   }

    real timeb = getCPU();
    timeForFinishBC+=timeb-timeStart;
    tm[timeForBoundaryConditions]+=timeb-time;
    timeForBC+=timeb-time;

    return 0;
}

