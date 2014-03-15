// This file automatically generated from smooth.bC with bpp.
#include "Ogmg.h"
#include "display.h"
#include "App.h"
#include "ParallelUtility.h"

#define smoothRedBlackOpt EXTERN_C_NAME(smoothredblackopt)
#define smRedBlack EXTERN_C_NAME(smredblack)
#define smoothJacobiOpt EXTERN_C_NAME(smoothjacobiopt)

extern "C"
{

    void smoothJacobiOpt( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                                              const int &nd3a, const int &nd3b,
                   		       const int &n1a, const int &n1b, const int &n1c,
                                              const int &n2a, const int &n2b, const int &n2c,
                                              const int &n3a, const int &n3b, const int &n3c,
                                              const int &ndc, const real & f, const real & c,
                                              const real & u, const real & v, const int & mask, const int & option, 
                                              const int & order, const int & sparseStencil,
                                              const real & cc, const real & varCoeff, const real & dx, const real & omega, const int & bc,
                                              const int &np, const int &ndip, const int &ip, const int & ipar );

    void smoothRedBlackOpt( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
                                              const int &nd3a, const int &nd3b,
                   		       const int &n1a, const int &n1b, const int &n1c,
                                              const int &n2a, const int &n2b, const int &n2c,
                                              const int &n3a, const int &n3b, const int &n3c, 
                                              const int &ndc, const real & f, const real & c,
                                              const real & u, const real & v, const int & mask, const int & option, 
                                              const int & order, const int & sparseStencil,
                                              const real & cc, const real & varCoeff, const real & dx, const real & omega,
                                              const int & useLocallyOptimalOmega, const real & variableOmegaScaleFactor, 
                                              const int & ipar, const real & rpar );

  // new opt version for parallel:
    void smRedBlack( const int &nd,  const int & nd1a, const int &nd1b, const int &nd2a, const int &nd2b,
               		   const int &nd3a, const int &nd3b,
               		   const int &n1a, const int &n1b, const int &n1c,
               		   const int &n2a, const int &n2b, const int &n2c,
               		   const int &n3a, const int &n3b, const int &n3c, 
               		   const int &ndc, const real & f, const real & c,
               		   const real & u, const real & v, const int & mask, const int & option, 
               		   const int & order, const int & sparseStencil,
               		   const real & cc, const real & varCoeff, const real & dx, const real & omega,
               		   const int & useLocallyOptimalOmega, const real & variableOmegaScaleFactor, 
               		   const int & ipar, const real & rpar );

}

//\begin{>>OgmgInclude.tex}{\subsection{computeDefectRatios}}
void Ogmg::
computeDefectRatios( int level )
// ====================================================================================
// /Description:
//  Compute the defect ratios needed for the automatic sub-smoothing algorithm
//  (We actually just compute the l2-norm of the defect here -- the ratio is computed in smooth)
//\end{OgmgInclude.tex}
// ====================================================================================
{
    CompositeGrid & mgcg = multigridCompositeGrid();

    if( parameters.useNewAutoSubSmooth && parameters.autoSubSmoothDetermination && mgcg.numberOfComponentGrids()>1 )
    {
        for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
        {
            defectRatio(grid,level)=l2Norm(defectMG.multigridLevel[level][grid]); 
        }
    }
}



//\begin{>>OgmgInclude.tex}{\subsection{smooth}}
void Ogmg::
smooth(const int & level, int numberOfSmoothingSteps, int cycleNumber )
// ======================================================================================
//   /Description:
//     This is the "composite" smooth routine. Smooth on all grids using a possibly different
//  smoother for each grid.
//\end{OgmgInclude.tex} 
// ======================================================================================
{
    real time=getCPU();
    CompositeGrid & mgcg = multigridCompositeGrid();

    bool initialSmooths= cycleNumber==0 && level==0 && parameters.minimumNumberOfInitialSmooths>0;
    if( initialSmooths && debug & 2 )
        printF("Ogmg:smooth:INFO:perform %i initial smooths...\n",parameters.minimumNumberOfInitialSmooths);
    
    
    for( int iteration=0; iteration<numberOfSmoothingSteps; iteration++ )
    {
        real maximumDefectOld=0., maximumDefectNew=0.;
        Range all;
        bool computeDefectRatios=true; // iteration==0;
        
        if( computeDefectRatios && 
                parameters.autoSubSmoothDetermination && mgcg.numberOfComponentGrids()>1 )
        {
      // *** these require a correct defect *****
            if( computeDefectRatios || parameters.showSmoothingRates )
            {
                if( false && level==0 && parameters.useNewAutoSubSmooth && defectRatio(0,level)>=0. )
      	{
      	}
      	else
      	{
        	  real time0=getCPU();

        	  if( debug & 4 ) printF("Ogmg:smooth:INFO:compute defect for auto-smooth: cycleNumber=%i level=%i\n",cycleNumber,level);
        	  
          // **** should compute defect and l2norms on a sub-set of points for speed.
                    if( false )
        	  {
          	    defect(level);  // we could probably avoid this sometimes *****************************************

          	    real time=getCPU()-time0;
          	    tm[timeForDefectInSmooth]+=time;
          	    tm[timeForDefect]-=time;         // don't count this time here

	    // printF(" smooth: compute defect for autosmooth: iteration=%i level=%i \n",iteration,level);
        	  
	  // **workUnits(level)+=1.;  // *wdh* added 030711 
          	    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
          	    {
            	      defectRatio(grid,level)=l2Norm(defectMG.multigridLevel[level][grid]);

            	      real norm=defectNorm(level,grid);
            	      printF("level=%i grid=%i: old=%8.2e new=%8.2e Error in norm=%8.2e\n",
                                          level,grid,defectRatio(grid,level),norm,fabs(norm-defectRatio(grid,level)));
           	     
          	    }
        	  }
        	  else
        	  {
          	    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
          	    {
            	      defectRatio(grid,level)=defectNorm(level,grid);
            	      
          	    }
        	  }
        	  

      	}
            }
            

      // *wdh* 2012/03/17 -- this is now down in Ogmg::updateToMatchGrid

      // --- Determine which grid to use as a "reference" grid for variable sub smooths ---
      //  The reference grid uses 1 sub-smooth (usually) and normally should be a Cartesian
      //  with the most grid points. 

//       // == We could compute gridMax once and for all in an initialization step ==
//       int gridMax=0;         // grid with the most grid-points
//       int maxGridPoints=0;   // number of grid-points in gridMax
//       int gridMaxCartesian=-1;         // Cartesian grid with most points
//       int maxGridPointsCartesian=0;   // number of grid-points in gridMaxCartesian
//       for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
//       {
//         MappedGrid & mg = mgcg.multigridLevel[level][grid];
//         const IntegerArray & gid = mg.gridIndexRange();
// 	int numGridPoints=(gid(1,0)-gid(0,0)+1)*(gid(1,1)-gid(0,1)+1)*(gid(1,2)-gid(0,2)+1);
// 	if( numGridPoints>maxGridPoints )
// 	{
// 	  maxGridPoints=numGridPoints;
// 	  gridMax=grid;
// 	}
// 	if( mg.isRectangular() && numGridPoints>maxGridPointsCartesian )
// 	{
// 	  maxGridPointsCartesian=numGridPoints;
// 	  gridMaxCartesian=grid;
// 	}
//       }
//       int defectReferenceGrid=gridMax;
//       if( gridMaxCartesian>=0 && maxGridPointsCartesian > .25*maxGridPoints )
//       {
//         defectReferenceGrid=gridMaxCartesian;
//       }
              
            if( debug & 4 )
            {
      	fPrintF(debugFile,"auto subSmooth: iteration=%i (grid,L2 defect,max defect) on level=%i : ",iteration,level);
      	for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
        	  fPrintF(debugFile,"(%i : %6.2e, %6.2e) ",grid,defectRatio(grid,level),
                                    maxNorm(defectMG.multigridLevel[level][grid]));
      	fPrintF(debugFile,"\n");
            }

            real minDefect;
            if( false && level==0 )
            {
                where( active )
        	  minDefect=min( defectRatio(all,level) );   // only count active grids.
            }
            else
            {
        // if there is one grid with many more points than the others then we force this large grid to 
        // have 1 sub-smooth
      	if( false ) // **wdh* 2012/03/06 -- TEST -- turn this back on
      	{
          // *wdh* 030425: With this choice there could be a small grid with a small defect that 
          //               would cause a very large grid to get many sub-smooths -- this is bad
        	  minDefect=min( defectRatio(all,level) );  
      	}
      	else
      	{
          // make the grid with the most grid points have one sub-smooth
          //  -- we could relax this if there are a number of grids nearly the same size.
                    minDefect=defectRatio(subSmoothReferenceGrid,level);  
      	}
      	
            }

            defectRatio(all,level)/=max(REAL_MIN*100., minDefect );

            IntegerArray & numberOfSubSmooths = parameters.numberOfSubSmooths;
      // numberOfSubSmooths.display("numberOfSubSmooths");
            real maxDefectRatio=0.;
            
            for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
            {
      	const OgmgParameters::SmootherTypeEnum & type = 
                          OgmgParameters::SmootherTypeEnum(parameters.smootherType(grid,level));
      	if( true ||
                        type==OgmgParameters::GaussSeidel || 
                        type==OgmgParameters::Jacobi || 
                        type==OgmgParameters::redBlack ||
                        type==OgmgParameters::redBlackJacobi )
      	{
          // we use a different rule for line smoothers since they are more expensive
                    bool lineSmooth=!(type==OgmgParameters::GaussSeidel || 
                                                        type==OgmgParameters::Jacobi || 
                                                        type==OgmgParameters::redBlack || 
                                                        type==OgmgParameters::redBlackJacobi );

                    const int maxNumberOfSubSmooths=lineSmooth ? parameters.maximumNumberOfLineSubSmooths : 
                                                                                                              parameters.maximumNumberOfSubSmooths;

// *wdh* 030717    const real defectRatioLowerBound=lineSmooth ? 1.5 : 1.01;
 // *wdh*          const real defectRatioUpperBound=lineSmooth ? 3.0 : 2.0;
                    real defectRatioLowerBound, defectRatioUpperBound; 
                    if( lineSmooth )
        	  { // use the value specified in the parameters, if >0
                        defectRatioLowerBound=parameters.defectRatioLowerBoundLineSmooth>0. ? 
                                                                    parameters.defectRatioLowerBoundLineSmooth : .5;
                        defectRatioUpperBound=parameters.defectRatioUpperBoundLineSmooth>0. ? 
                                                                    parameters.defectRatioUpperBoundLineSmooth : 2.;
        	  }
        	  else
        	  {
                        defectRatioLowerBound=parameters.defectRatioLowerBound>0. ? parameters.defectRatioLowerBound : .5;
                        defectRatioUpperBound=parameters.defectRatioUpperBound>0. ? parameters.defectRatioUpperBound : 2.;
        	  }
        	  
                    const real nu =1./max(1,numberOfSubSmooths(grid,level));
                    defectRatioLowerBound=pow(defectRatioLowerBound,nu);
                    defectRatioUpperBound=pow(defectRatioUpperBound,nu);
        	  
                    maxDefectRatio=max(maxDefectRatio,defectRatio(grid,level));
        	  

        	  const int minSubSmooths=1;
        	  if( numberOfSubSmooths(grid,level)>minSubSmooths && defectRatio(grid,level)<defectRatioLowerBound )
          	    numberOfSubSmooths(grid,level)--;
        	  else if( numberOfSubSmooths(grid,level)<maxNumberOfSubSmooths && defectRatio(grid,level)>defectRatioUpperBound )
        	  {
          	    numberOfSubSmooths(grid,level)++;
        	  }
        	  

        	  if( debug & 2 )
        	  {
          	    fPrintF(debugFile," level=%i grid=%i numberOfSubSmooths=%i defectRatio=%g defRatUpperBound=%g" 
                                                    " maxSubSmooths=%i\n",
                		    level,grid,numberOfSubSmooths(grid,level),defectRatio(grid,level),defectRatioUpperBound,
                                        maxNumberOfSubSmooths);
        	  }


      	}
            } // end for grid
            
      // *wdh* 2012/03/16 -- if grid with most points has the largest defect then we need to increase
      // the number of smooths on it  ** THIS DOESN'T WORK CORRECTLY so turn OFF: 
            if( false && maxDefectRatio<1.01 && 
                    numberOfSubSmooths(subSmoothReferenceGrid,level)<parameters.maximumNumberOfSubSmooths ) 
            {
      	numberOfSubSmooths(subSmoothReferenceGrid,level)++;
            }


//      // If all sub smooths are greater than 1 then decrease all by 1
//        Range G=mgcg.multigridLevel[level].numberOfComponentGrids();
//        if( min(numberOfSubSmooths(G,level))>1 )
//        {
//  	numberOfSubSmooths(G,level)--;
//        }
        
            if( (parameters.showSmoothingRates || (debug & 2 && level==0)) && debugFile!=NULL )
            {
      	fPrintF(debugFile,"(grid,defectRatio,subSmooth) on level=%i : ",level);
      	for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
        	  fPrintF(debugFile,"(%i : %6.2e :%i) ",grid,defectRatio(grid,level),numberOfSubSmooths(grid,level));
      	fPrintF(debugFile,"\n");
            }
        }
        
        if( parameters.showSmoothingRates )
        {
            real time0=getCPU();      
      // if( !(parameters.autoSubSmoothDetermination && mgcg.numberOfComponentGrids()>1 && iteration==0) )  
            if( true ) // we need to make sure defectMG.multigridLevel is defined for maxNorm below *wdh* 110321
                defect(level);

            real time=getCPU()-time0;
            tm[timeForDefectInSmooth]+=time;
            tm[timeForDefect]-=time;         // don't count this time here

            maximumDefectOld=maxNorm(defectMG.multigridLevel[level]);

            if( Ogmg::debug & 8 )
                printF("%*.1s Ogmg:smooth, level = %i, BEFORE iteration=%i, defect=%e \n",
                          level*4,"  ",level,iteration,maximumDefectOld);

        }
        if( Ogmg::debug & 16 )
            uMG.multigridLevel[level].display(sPrintF(buff,"Ogmg::smooth:level=%i Here is the solution before the smooth %i",
                            level,iteration),debugFile,"%9.1e");
        if( iteration==0 && Ogmg::debug & 16 )
            fMG.multigridLevel[level].display(sPrintF(buff,"Ogmg::smooth:level=%i Here is f before the smooth",level),debugFile,"%10.2e");

      
        bool newInterpolationMethod=false;  // 

    // gridOrderingForSmooth:  0= 1...ng 
    //                         1= ng...1
    //                         2= alternate
        int gridStart=0, gridEnd  =mgcg.multigridLevel[level].numberOfComponentGrids()-1, gridStride=1;
        if( parameters.gridOrderingForSmooth==1 ) // reverse order
        {
            gridEnd  =0, gridStart=mgcg.multigridLevel[level].numberOfComponentGrids()-1, gridStride=-1;
        }
        else if( parameters.gridOrderingForSmooth==2 && cycleNumber % 2 == 1 )
        {
            gridEnd  =0, gridStart=mgcg.multigridLevel[level].numberOfComponentGrids()-1, gridStride=-1;
        }
        
          
        for( int grid=gridStart; grid!=gridEnd+gridStride; grid+=gridStride )
        {

            const OgmgParameters::SmootherTypeEnum smootherType =  
                                      OgmgParameters::SmootherTypeEnum(parameters.smootherType(grid,level));
            
            if( false && !active(grid) && cycleNumber>0 && level==0 )
            {
      	interpolate(uMG.multigridLevel[level],grid,level);
                continue;   // skip this grid if it is not active and it is not the first cycle.
            }
            
            if( grid!=gridStart || ( newInterpolationMethod && iteration>0 ) || !parameters.interpolateAfterSmoothing)
            {
                if( smootherType!=OgmgParameters::ogesSmoother )
                    interpolate(uMG.multigridLevel[level],grid,level); // interpolate grid ** this does more pts than necessary
        // interpolate(uMG.multigridLevel[level]);
            }

            switch (smootherType)
            {
            case OgmgParameters::GaussSeidel:
      	smoothGaussSeidel(level,grid);
      	break;
            case OgmgParameters::Jacobi:
      	smoothJacobi(level,grid);
      	break;
            case OgmgParameters::redBlack:
            case OgmgParameters::redBlackJacobi:
      	smoothRedBlack(level,grid);
      	break;
            case OgmgParameters::lineJacobiInDirection1:
      	smoothLine(level,grid,axis1,false);
      	break;
            case OgmgParameters::lineJacobiInDirection2:
      	smoothLine(level,grid,axis2,false);
      	break;
            case OgmgParameters::lineJacobiInDirection3:
      	smoothLine(level,grid,axis3,false);
      	break;
            case OgmgParameters::lineZebraInDirection1:
      	smoothLine(level,grid,axis1);
      	break;
            case OgmgParameters::lineZebraInDirection2:
      	smoothLine(level,grid,axis2);
      	break;
            case OgmgParameters::lineZebraInDirection3:
      	smoothLine(level,grid,axis3);
      	break;
            case OgmgParameters::alternatingLineJacobi:
      	alternatingLineSmooth(level,grid,false);
      	break;
            case OgmgParameters::alternatingLineZebra:
      	alternatingLineSmooth(level,grid);
      	break;
            case OgmgParameters::ogesSmoother:

                applyOgesSmoother(level,grid);
                break;
            default :
      	printF("Unknown smoother type! \n");
            }

            if( parameters.combineSmoothsWithIBS==1 && 
        	  parameters.numberOfInterpolationLayersToSmooth>=1 && parameters.numberOfInterpolationSmoothIterations>=1 &&
        	  parameters.numberOfIBSIterations>0 && level<parameters.numberOfLevelsForInterpolationSmoothing )
            {
	// *************IBS: Interpolation Boundary Smoothing**********************
        // --- Here we merge the IBS smoothing with the regular smooth ---
      	smoothInterpolationNeighbours(level, grid );
            }
      // interpolate(uMG.multigridLevel[level]);
        } // end for grid 

    // printF("smooth: interpolate level=%i\n",level);
        
        if( Ogmg::debug & 16 )
            uMG.multigridLevel[level].display(sPrintF(buff,"Ogmg::smooth:level=%i Here is the solution before interp, smooth %i",
                            level,iteration),debugFile,"%9.1e");

        if( !newInterpolationMethod && parameters.interpolateAfterSmoothing )
            interpolate(uMG.multigridLevel[level],-1,level);  // **** this is over-kill


        if( parameters.combineSmoothsWithIBS==0 && 
                parameters.numberOfInterpolationLayersToSmooth>=1 && parameters.numberOfInterpolationSmoothIterations>=1 &&
                parameters.numberOfIBSIterations>0 && level<parameters.numberOfLevelsForInterpolationSmoothing )
        {
      // ************************************************************************
      // *************IBS: Interpolation Boundary Smoothing**********************
      // ************************************************************************
        // --- Here we apply the IBS smoothing after the regular smooth ---
            for( int it=0; it<parameters.numberOfIBSIterations; it++ )
            {
	// optionally smooth neighbours of interpolation points where the defect can get large after 
        // the final interpolation above.
      	for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
        	  smoothInterpolationNeighbours(level, grid );

      	interpolate(uMG.multigridLevel[level],-1,level); 
            }
            
        }


        if( Ogmg::debug & 16 )
            uMG.multigridLevel[level].display(sPrintF(buff,"Ogmg::smooth:level=%i Here is the solution AFTER interp, smooth %i",
                            level,iteration),debugFile,"%9.1e");
        if( parameters.showSmoothingRates || Ogmg::debug & 4 )
        {
            defect(level);

            if( parameters.showSmoothingRates )
                printF("%*.1s Ogmg:smooth, level = %i, iteration=%i, defect: ",level*4,"  ",level,iteration);
            if( Ogmg::debug & 4 )
                fPrintF(debugFile,"%*.1s Ogmg:smooth, level = %i, iteration=%i, defect: ",level*4,"  ",level,iteration); 
            maximumDefectNew=0.;
            for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
            {
                real maxDefect=maxNorm(defectMG.multigridLevel[level][grid]);
                maximumDefectNew=max(maximumDefectNew,maxDefect);
                if( parameters.showSmoothingRates ) printF("(%i=%8.3e),",grid,maxDefect);
                if( Ogmg::debug & 4) fPrintF(debugFile,"(%i=%8.3e),",grid,maxDefect);
            }
            const real rate = min(10.,maximumDefectNew/max(REAL_MIN,maximumDefectOld));
            if( parameters.showSmoothingRates ) 
                printF("max= %8.2e, [rate = %6.4f],\n",maximumDefectNew,rate);
            if( Ogmg::debug & 4)
                fPrintF(debugFile,"max= %8.2e, [rate = %6.4f],\n",maximumDefectNew,rate);
            if( Ogmg::debug & 4 )
            {
      	for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
      	{
        	  MappedGrid & mg = mgcg.multigridLevel[level][grid];
          // note: do not display a view uu(J1,J2,J3) since this can cause trouble in parallel -- instead pass Jv
                    Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2]; Jv[3]=Range(0,0); // Jv[3]= components
                    int extra=1; getIndex(mg.gridIndexRange(),J1,J2,J3,extra);
        	  ::display(defectMG.multigridLevel[level][grid],
                		    sPrintF(buff,"defect after smooth, level=%i grid=%i iteration=%i",
                      			    level,grid,iteration),debugFile,"%8.2e ",Jv);         
      	}
            }
        }

        if( parameters.totalNumberOfSubSmooths.getLength(0)<mgcg.multigridLevel[level].numberOfComponentGrids() ||
                parameters.numberOfSubSmooths.getLength(0)<mgcg.multigridLevel[level].numberOfComponentGrids() ||
                parameters.totalNumberOfSmoothsPerLevel.getLength(0)<mgcg.numberOfMultigridLevels() )
        {
            printF("smooth:ERROR: parameters.totalNumberOfSubSmooths.getLength(0)=%i \n"
                          "              parameters.numberOfSubSmooths.getLength(0)=%i\n"
                          "              parameters.totalNumberOfSmoothsPerLevel.getLength(0)=%i\n",
           	     parameters.totalNumberOfSubSmooths.getLength(0),
                          parameters.numberOfSubSmooths.getLength(0),
                          parameters.totalNumberOfSmoothsPerLevel.getLength(0));

            Overture::abort("error");
        }
        for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
            parameters.totalNumberOfSubSmooths(grid,level)+=parameters.numberOfSubSmooths(grid,level);

        parameters.totalNumberOfSmooths++;  // counts total number of smooths
        parameters.totalNumberOfSmoothsPerLevel(level)++;

    } // end loop over multiple smoothing steps
    

    if( (Ogmg::debug & 4) && (Ogmg::debug & 16) )
    {
        defect(level);
        uMG.multigridLevel[level].display(sPrintF(buff,"Here is the solution after smooths level=%i, cycle=%i",level,
                          numberOfCycles), debugFile,"%10.2e");
        defectMG.multigridLevel[level].display(sPrintF(buff,"Here is the defect after smooths level=%i, cycle=%i",level,
                          numberOfCycles),debugFile,"%10.2e");
    }
    
    tm[timeForSmooth]+=getCPU()-time;

}

static bool firstSolve[50]; // 50=maxLevels, fix this ***

//\begin{>>OgmgInclude.tex}{\subsection{applyOgesSmoother}}
void Ogmg::    
applyOgesSmoother(const int level, const int grid)
//---------------------------------------------------------------------------------------------
// /Description:
//    Jacobi or Gauss-Seidel Smoother.
//
// /smootherChoice (input) : 0=jacobi, 1=gauss-seidel
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  // printF("Entering applyOgesSmoother: level=%i grid=%i\n",level,grid);

    if( grid!=parameters.activeGrids(0) )  // This smooth does all grids in the activeGrids list at once.
        return; 

    CompositeGrid & mgcg = multigridCompositeGrid();
    realCompositeGridFunction & u = uMG.multigridLevel[level];
    realCompositeGridFunction & f = fMG.multigridLevel[level];


    real time0=getCPU();
    if( ogesSmoother==NULL )
    {
        ogesSmoother = new Oges[mgcg.numberOfMultigridLevels()-1];
        int l;
        for( int l=0; l<mgcg.numberOfMultigridLevels()-1; l++ )
        {
            CompositeGrid & cg = mgcg.multigridLevel[l];
            ogesSmoother[l].updateToMatchGrid( cg );
            ogesSmoother[l].setOgesParameters(*parameters.ogesSmoothParameters);

            ogesSmoother[l].setGridsToUse(parameters.activeGrids);

            realCompositeGridFunction & coeff =  cMG.multigridLevel[l];
      // the coeff array needs to have the interpolation equations set for the active grids.
      // ** we need the correct corner boundary conditions here ***
      // printF("***applyOgesSmoother:before finishBC coeff.getIsACoefficientMatrix()=%i\n",coeff.getIsACoefficientMatrix());

            CompositeGridOperators & op = operatorsForExtraLevels[l];
            op.finishBoundaryConditions(coeff,bcParams,nullRange,parameters.activeGrids);
            
            ogesSmoother[l].setCoefficientArray(coeff);

        }
        for( l=0; l<mgcg.numberOfMultigridLevels(); l++ )
            firstSolve[l]=true;
    }

  // we really only need to interp these grids from others
    interpolate(u,-1,level);  // we should interp all grids in the ogesSmoother

  // interpolate( u,parameters.activeGrids);

//    int num=
//    IntegerArray gridsToInterpolateFrom(mgcg.numberOfComponentGrids()-
//    interpolate( u,parameters.activeGrids, gridsToInterpolateFrom );

    Oges & solver = ogesSmoother[level];

  // The maximumNumberOfIterations = numberOfSubSmooths*( default max number of Oges iterations )
    int oldMaxNumberOfIterations;
    solver.get(OgesParameters::THEmaximumNumberOfIterations,oldMaxNumberOfIterations);
    const int maxNumberOfIterations=parameters.numberOfSubSmooths(grid,level)*oldMaxNumberOfIterations;

  // printF("ogesSmoother:level=%i, grid=%i  setting maxNumberOfIterations=%i\n",level,grid,maxNumberOfIterations);

    solver.set(OgesParameters::THEmaximumNumberOfIterations,maxNumberOfIterations);

    solver.solve(u,f);

    solver.set(OgesParameters::THEmaximumNumberOfIterations,oldMaxNumberOfIterations); // reset

    if( firstSolve[level] )
    {
    // the first solve we count as initialization since this includes the setup time
        firstSolve[level]=false;
        real time=getCPU()-time0;
        tm[timeForOgesSmootherInit]+=time;
        tm[timeForInitialize]+=time;

        tm[timeForSmooth]-=time;   // do not count as part of smoother
    }
    
    

    intArray & mask = mgcg.multigridLevel[level][grid].mask();
  // need an estimate for the work units: ************************** finish this *****
    int iluLevels; // scale work by number of ilu levels
    solver.get(OgesParameters::THEnumberOfIncompleteLULevels,iluLevels);
    workUnits(level)+=(3+iluLevels)*solver.getNumberOfIterations()*mask.elementCount()/real(numberOfGridPoints);

    if( debug & 4 )
    {
    // printF("applyOgesSmoother: Solver: %s\n",(const char*)solver.parameters.getSolverName());
        printF("applyOgesSmoother: level=%i grid=%i max residual=%8.2e (iterations=%i) ***\n",level,grid,
                        solver.getMaximumResidual(),
                        solver.getNumberOfIterations());
    }
    
}



#undef C
#undef M123
#define M123(m1,m2,m3) (m1+halfWidth1+width1*(m2+halfWidth2+width2*(m3+halfWidth3)))
// define C(m1,m2,m3,I1,I2,I3) c(I1,I2,I3,M123(m1,m2,m3))
#define C(m1,m2,m3,I1,I2,I3) c(M123(m1,m2,m3),I1,I2,I3)


//\begin{>>OgmgInclude.tex}{\subsection{smoothJacobi}}
void Ogmg::
smoothJacobi(const int & level, const int & grid, int smootherChoice /* = 0 */ )
//---------------------------------------------------------------------------------------------
// /Description:
//    Jacobi or Gauss-Seidel Smoother.
//
// /smootherChoice (input) : 0=jacobi, 1=gauss-seidel
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
    realMappedGridFunction & u = uMG.multigridLevel[level][grid];
    realMappedGridFunction & f = fMG.multigridLevel[level][grid];
    realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
    realArray & defect = defectMG.multigridLevel[level][grid];

    CompositeGrid & mgcg = multigridCompositeGrid();

    MappedGrid & mg = mgcg.multigridLevel[level][grid];  
    const int numberOfDimensions = mg.numberOfDimensions();
    const intArray & mask = mg.mask();
    const IntegerArray & bc = mg.boundaryCondition();
    
    realArray & uu = u;
    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  // ---- Determine Index's for interior points        ----
  // ---- include periodic edges in smooth computation ---
    getIndex(mg.gridIndexRange(),I1,I2,I3); // *wdh* 020205 no need to include ghost pts
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        if( (bool)mg.isPeriodic(axis) )
            Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);
        else if( equationToSolve!=OgesParameters::userDefined && mg.isRectangular() )
        {
      // do NOT smooth on the boundary for the predefine equations with dirichlet BC's
//        printF(" )))))grid=%i axis=%i boundaryCondition=[%i,%i] I1a=[%i,%i]->",grid,axis,boundaryCondition(0,axis,grid),
//               boundaryCondition(1,axis,grid),I1a.getBase(),I1a.getBound());
            if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate && 
                    boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate )
      	Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound()-1);
            else if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate  )
      	Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound());
            else if( boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate  )
      	Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);
      // printF("  I1a=[%i,%i]\n",I1a.getBase(),I1a.getBound());
        }
    }

  // ---- optimal omega for smoothing ----
  //   **** should be 1/( 1 + cMin/2)    cMin=.5 for 2D laplace, cMin=1/3 for 3D laplace
    real omega=numberOfDimensions==2 ? 4./5. : 6./7.; 
    if( parameters.omegaJacobi > 0 )
        omega=parameters.omegaJacobi; // use user supplied value
    if( smootherChoice==1 )
        omega=parameters.omegaGaussSeidel<0. ? 1. : parameters.omegaGaussSeidel;  // Gauss-Seidel -- check this ---
    
    if( parameters.useOptimizedVersion )
    {
        const IntegerArray & d = mg.dimension();
        const int ndc=c.getLength(0);

        const bool rectangular=(*c.getOperators()).isRectangular() &&
                                                ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 

    // const int general=0, sparse=1, constantCoeff=2, sparseConstantCoefficients=3;
      
        real dx[3]={1.,1.,1.};
        int sparseStencil=general;
        if( mg.isRectangular() ) // ***
        {
            if( equationToSolve!=OgesParameters::userDefined ) // ***  defect is used on the boundary
            {
                if( equationToSolve==OgesParameters::divScalarGradOperator ||
                        equationToSolve==OgesParameters::variableHeatEquationOperator ||
                        equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
      	{
        	  sparseStencil=level==0 ? sparseVariableCoefficients : variableCoefficients;
      	}
      	else
      	{
        	  sparseStencil=level==0 ? sparseConstantCoefficients : constantCoeff;
      	}
            }
            else if( rectangular && assumeSparseStencilForRectangularGrids )
                sparseStencil=sparse;

            mg.getDeltaX(dx);
        }

    // hw[axis] = discretization stencil half-width: 
    // numExtraParallelGhost = parallelGhost - hw[axis] 
        int hw[3]={0,0,0}; // 
        int numExtraParallelGhost=INT_MAX;
        for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
        {
            hw[axis]=orderOfAccuracy/2;
            numExtraParallelGhost = min(numExtraParallelGhost,mask.getGhostBoundaryWidth(axis)-hw[axis]);
        }

        int n1a = I1.getBase(); 
        int n1b = I1.getBound();
        int n1c = 1;
        int n2a = I2.getBase();
        int n2b = I2.getBound();
        int n2c = 1;
        int n3a = I3.getBase();
        int n3b = I3.getBound();
        int n3c = 1;

        #ifdef USE_PPP
            const realSerialArray & defectLocal = defect.getLocalArrayWithGhostBoundaries();
            const realSerialArray & uLocal = u.getLocalArrayWithGhostBoundaries();
            const realSerialArray & fLocal = f.getLocalArrayWithGhostBoundaries();
            const realSerialArray & cLocal = c.getLocalArrayWithGhostBoundaries();
            const intSerialArray & maskLocal = mask.getLocalArrayWithGhostBoundaries();

            realArray & varCoeffd = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid] : u;
            const realSerialArray & varCoeffLocal = varCoeffd.getLocalArrayWithGhostBoundaries(); 
            real *vcp = getDataPointer(varCoeffLocal);

      // n1a = max(n1a,maskLocal.getBase(0)  +mask.getGhostBoundaryWidth(0));
      // n1b = min(n1b,maskLocal.getBound(0) -mask.getGhostBoundaryWidth(0));
      // n2a = max(n2a,maskLocal.getBase(1)  +mask.getGhostBoundaryWidth(1));
      // n2b = min(n2b,maskLocal.getBound(1) -mask.getGhostBoundaryWidth(1));
      // n3a = max(n3a,maskLocal.getBase(2)  +mask.getGhostBoundaryWidth(2));
      // n3b = min(n3b,maskLocal.getBound(2) -mask.getGhostBoundaryWidth(2));
            n1a = max(n1a,maskLocal.getBase(0)  +hw[0]);
            n1b = min(n1b,maskLocal.getBound(0) -hw[0]);
            n2a = max(n2a,maskLocal.getBase(1)  +hw[1]);
            n2b = min(n2b,maskLocal.getBound(1) -hw[1]);
            n3a = max(n3a,maskLocal.getBase(2)  +hw[2]);
            n3b = min(n3b,maskLocal.getBound(2) -hw[2]);

        #else
            const realSerialArray & defectLocal = defect;
            const realSerialArray & uLocal = u;
            const realSerialArray & fLocal = f;
            const realSerialArray & cLocal = c;
            const intSerialArray & maskLocal = mask;

            realArray & varCoeffd = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid] : u;
            const realSerialArray & varCoeffLocal = varCoeffd.getLocalArrayWithGhostBoundaries(); 
            real *vcp = getDataPointer(varCoeffLocal);
        #endif


        real *vp = getDataPointer(defectLocal); // temp space for jacobi
        real *up=getDataPointer(uLocal);
        const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : vp;
    // real *vcp = varCoeff!=NULL ? getDataPointer((*varCoeff).multigridLevel[level][grid]) : up;

    // const int boundaryLayers=2; // only used for boundary smoothing
        int ipar[10];
        ipar[0]=parameters.numberOfBoundaryLayersToSmooth; 
        const int np=0,ndip=1,ip=0;
        
        const int option=smootherChoice==0 ? 0 : 1 ; // 0=Jacobi 1=GS
        for( int iteration=0; iteration<parameters.numberOfSubSmooths(grid,level); iteration++ )
        {
            real time0=getCPU();


            if( smootherChoice!=0 && parameters.alternateSmoothingDirections )
            {
        // Alternate the ordering of the points in the Gauss-Seidel sweep (has no effect on Jacobi)
	// Two possibilities for now:
      	int numMod = parameters.totalNumberOfSubSmooths(grid,level)+iteration % 2;
      	if( numMod == 1 )
      	{
	  // reverse the order of the smoother
        	  int temp;
        	  temp=n1b; n1b=n1a; n1a=temp; n1c=-n1c;
        	  temp=n2b; n2b=n2a; n2a=temp; n2c=-n2c;
        	  temp=n3b; n3b=n3a; n3a=temp; n3c=-n3c;

      	}
            }
      // :::display(u,"smoothJacobi: u before new smooth",debugFile,"%7.1e ");
      // ::display(f,"smoothJacobi: f before new smooth",debugFile,"%7.1e ");

        // *** no need to smooth the boundary if dirichlet ***

            smoothJacobiOpt( mg.numberOfDimensions(), 
                                              maskLocal.getBase(0),maskLocal.getBound(0),
                   		       maskLocal.getBase(1),maskLocal.getBound(1),
                   		       maskLocal.getBase(2),maskLocal.getBound(2),
                   		       n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, 
                   		       *getDataPointer(fLocal),
                   		       *getDataPointer(cLocal),
                   		       *up, *vp,
                   		       *getDataPointer(maskLocal), 
                   		       option, orderOfAccuracy, sparseStencil, *pcc, *vcp, dx[0], omega,
                                              bc(0,0),np,ndip,ip, ipar[0] );

            tm[timeForRelaxInSmooth]+=getCPU()-time0;
            
	// ::display(u,"smoothJacobi: u after smooth",debugFile,"%5.1f ");
            applyBoundaryConditions( level,grid,u,f );  // *** wdh 991121 for periodic and Neumann BC's
      // ::display(u,"smoothJacobi: u after applyBoundaryConditions",debugFile,"%5.1f ");

            workUnits(level)+=mask.elementCount()/real(numberOfGridPoints);
        }

    }
    else
    {
        assert( smootherChoice==0 );
        
        getIndex(mg.dimension(),I1,I2,I3);
//  realArray defect(I1,I2,I3);
//  defect=0.;                                       // *************

        realArray omegaOverC0(1,I1,I2,I3);
        where( C(0,0,0,I1,I2,I3)!=0. )
            omegaOverC0 = omega/C(0,0,0,I1,I2,I3);
        otherwise( )
            omegaOverC0 = 0.;
        
        omegaOverC0.reshape(I1,I2,I3);

    
        real maximumDefect;
        IntegerArray & numberOfSubSmooths = parameters.numberOfSubSmooths;
        for( int iteration=0; iteration<numberOfSubSmooths(grid,level); iteration++ )
        {
            real time0=getCPU();
            getDefect(level,grid,f,u,I1,I2,I3,defect);
            tm[timeForDefectInSmooth]+=getCPU()-time0;

            if( Ogmg::debug & 32 )
            {
      	maximumDefect=maxNorm(defectMG.multigridLevel[level][grid]);
      	cout << "smoothJacobi: iteration = " << iteration << ", maximumDefect = " << maximumDefect << endl;
            }
            if( Ogmg::debug & 4 )
            {
      	display(u,"smoothJacobi: Here is u",debugFile);
                real maxDefect=maxNorm(defectMG.multigridLevel[level][grid]);
      	display(defect,sPrintF(buff,"smoothJacobi: Here is the defect, grid=%i, max=%e",grid,maxDefect),debugFile);
            }

/* ---
      where( mg.mask()(I1,I2,I3)==0 )  // ******************************************8
      uu(I1,I2,I3)=1.e9;
      ---- */

//    u.reshape(1,u.dimension(0),u.dimension(1),u.dimension(2));
//    defect.reshape(1,defect.dimension(0),defect.dimension(1),defect.dimension(2));
            if( false && numberOfSubSmooths(grid,level)==1 )
            {
      	uu(I1,I2,I3)+=defect(I1,I2,I3)*omegaOverC0;
            }
            else
            { // do not change the interpolation point values:
      	where( mask(I1,I2,I3)>0 )
        	  uu(I1,I2,I3)+=defect(I1,I2,I3)*omegaOverC0;
            }
            if( debug & 16 )
      	fPrintF(debugFile," **** smooth jacobi level=%i grid=%i defect =%e \n",level,grid,
                                          maxNorm(defectMG.multigridLevel[level][grid]));

//    u.reshape(u.dimension(1),u.dimension(2),u.dimension(3));
//    defect.reshape(defect.dimension(1),defect.dimension(2),defect.dimension(3));

      // Boundary points:
            applyBoundaryConditions( level,grid,u,f );

      //  workUnits(level)+=1./mgcg.multigridLevel[level].numberOfComponentGrids();  // should weight by number of pts
            workUnits(level)+=mask.elementCount()/real(numberOfGridPoints);
        }
    }
    
}

//\begin{>>OgmgInclude.tex}{\subsection{smoothGaussSeidel}}
void Ogmg::
smoothGaussSeidel(const int & level, const int & grid)
//---------------------------------------------------------------------------------------------
// /Description:
//    Gauss Seidel Smoother. NOT implemented yet.
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
    smoothJacobi(level,grid,1);
}




//\begin{>>OgmgInclude.tex}{\subsection{smoothRedBlack}}
void Ogmg::
smoothRedBlack(const int & level, const int & grid)
//---------------------------------------------------------------------------------------------
// /Description:
//    Red-Black Smoother
//
//  First smooth "red" points, then black points
//
// \begin{verbatim}
//  Two-dimensions:
//        shift1  shift2 
//          0       0
//          1       1
//          1       0
//          0       1
//  Three-dimensions:
//        shift1  shift2  shift3 
//          0       0      0
//          1       1      0
//          1       0      1
//          0       1      1
//          1       0      0
//          0       1      0
//          0       0      1
//          1       1      1
// \end{verbatim}
//\end{OgmgInclude.tex} 
//---------------------------------------------------------------------------------------------
{
    realMappedGridFunction & u = uMG.multigridLevel[level][grid];
    realMappedGridFunction & f = fMG.multigridLevel[level][grid];
    realMappedGridFunction & c =  level==0 ? cMG[grid] : cMG.multigridLevel[level][grid];
    realArray & defect = defectMG.multigridLevel[level][grid];
    CompositeGrid & mgcg = multigridCompositeGrid();
    MappedGrid & mg = mgcg.multigridLevel[level][grid];  
    const intArray & mask = mg.mask();

    const OgmgParameters::SmootherTypeEnum smootherType =  
        OgmgParameters::SmootherTypeEnum(parameters.smootherType(grid,level));

    bool useJacobiRedBlack= smootherType == OgmgParameters::redBlackJacobi;

  // const aString fmt = "%8.4f ";
    const aString fmt = "%16.12f ";

    if( Ogmg::debug & 4 ) 
    {
        fPrintF(debugFile," --- Entering smoothRedBlack: level=%i grid=%i cycle=%i ---\n"
          	    "     useJacobiRedBlack=%i useNewSmoother=%i, alternateSmoothingDirections=%i\n",
          	    level,grid,numberOfCycles,(int)useJacobiRedBlack,
                        (int)parameters.useNewRedBlackSmoother,(int)parameters.alternateSmoothingDirections );
        if( debugFile!=pDebugFile )
            fprintf(pDebugFile," --- Entering smoothRedBlack: level=%i grid=%i cycle=%i ---\n"
          	    "     useJacobiRedBlack=%i useNewSmoother=%i, alternateSmoothingDirections=%i\n",
          	    level,grid,numberOfCycles,(int)useJacobiRedBlack,
                        (int)parameters.useNewRedBlackSmoother,(int)parameters.alternateSmoothingDirections );

    }

//  realMappedGridFunction defect0(mg,nullRange,nullRange,nullRange);               // *************
//  realArray & defect = defect0;
    
  // ---- Determine Index's for interior points        ----
  // ---- include periodic edges in smooth computation ---
    Index Iav[3], &I1a=Iav[0], &I2a=Iav[1], &I3a=Iav[2];
    Index I1,I2,I3;
  // getIndex(mg.extendedIndexRange(),I1a,I2a,I3a); // *wdh* 020204 Include boundary points *****
    getIndex(mg.gridIndexRange(),I1a,I2a,I3a); // *wdh* 020205 no need to include ghost pts
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        if( (bool)mg.isPeriodic(axis) )
            Iav[axis]=Range(Iav[axis].getBase(),Iav[axis].getBound()-1);
//    else if( equationToSolve!=OgesParameters::userDefined && mg.isRectangular() )
// ***> switch back 030607     else if( true  )
        else if( true  )
        {
      // do NOT smooth on the boundary for the predefine equations with dirichlet BC's
//        printF(" )))))grid=%i axis=%i boundaryCondition=[%i,%i] I1a=[%i,%i]->",grid,axis,boundaryCondition(0,axis,grid),
//               boundaryCondition(1,axis,grid),I1a.getBase(),I1a.getBound());
            if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate && mg.boundaryCondition(0,axis)>0 && 
                    boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate && mg.boundaryCondition(1,axis)>0 )
      	Iav[axis]=Range(Iav[axis].getBase()+1,Iav[axis].getBound()-1);
            else if( boundaryCondition(0,axis,grid)==OgmgParameters::extrapolate && mg.boundaryCondition(0,axis)>0 )
      	Iav[axis]=Range(Iav[axis].getBase()+1,Iav[axis].getBound());
            else if( boundaryCondition(1,axis,grid)==OgmgParameters::extrapolate && mg.boundaryCondition(1,axis)>0 )
      	Iav[axis]=Range(Iav[axis].getBase(),Iav[axis].getBound()-1);
      // printF("  I1a=[%i,%i]\n",I1a.getBase(),I1a.getBound());
        }
    }
    
  // --- compute the stencil half-width and determine if we have excess ghost points ---
  // hw[axis] = discretization stencil half-width: 
  // numExtraParallelGhost = parallelGhost - hw[axis] 
    int hw[3]={0,0,0}; // 
    int numExtraParallelGhost=INT_MAX;
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        hw[axis]=orderOfAccuracy/2;
        numExtraParallelGhost = min(numExtraParallelGhost,mask.getGhostBoundaryWidth(axis)-hw[axis]);
    }

  // printF(" **** Ogmg::redBlack:smooth: numExtraParallelGhost =%i\n",numExtraParallelGhost);

    bool applyBoundaryConditionsAtEverySubStep=true; // set to false for testing non-symmetry neumann BC's
    if( useJacobiRedBlack ) 
    {
    // For red-black Jacobi we do not need to apply the boundary conditions after the first "red" stage
    // if we have extra parallel ghost points *wdh* 100927 
        applyBoundaryConditionsAtEverySubStep = numExtraParallelGhost <= 0;

    // applyBoundaryConditionsAtEverySubStep = false; // with RB-jacobi we can turn this off *wdh* 100109  -- this did not work for order=4 parallel
    }
    

    if( parameters.useOptimizedVersion )
    {
        const IntegerArray & d = mg.dimension();
        const int ndc=c.getLength(0);

        const bool rectangular=(*c.getOperators()).isRectangular() &&
                                                ( level < mgcg.numberOfMultigridLevels()-numberOfExtraLevels ); 

    // const int general=0, sparse=1, constantCoeff=2, sparseConstantCoefficients=3;
      
        real dx[3]={1.,1.,1.};
        int sparseStencil=general;
        if( mg.isRectangular() ) // ***
        {
            if( equationToSolve!=OgesParameters::userDefined ) // ***  defect is used on the boundary
            {
                if( equationToSolve==OgesParameters::divScalarGradOperator ||
                        equationToSolve==OgesParameters::variableHeatEquationOperator ||
                        equationToSolve==OgesParameters::divScalarGradHeatEquationOperator )
      	{
        	  sparseStencil=level==0 ? sparseVariableCoefficients : variableCoefficients;
      	}
      	else
      	{
        	  sparseStencil=level==0 ? sparseConstantCoefficients : constantCoeff;
      	}
            }
            else if( rectangular && assumeSparseStencilForRectangularGrids )
                sparseStencil=sparse;

//        if( equationToSolve!=OgesParameters::userDefined ) // && level==0 ) 
//  	sparseStencil=level==0 ? sparseConstantCoefficients : constantCoeff;
//        else if( rectangular && assumeSparseStencilForRectangularGrids )
//          sparseStencil=sparse;

            mg.getDeltaX(dx);
        }


        #ifdef USE_PPP
            realSerialArray defectLocal; getLocalArrayWithGhostBoundaries(defect,defectLocal);
            realSerialArray uLocal;      getLocalArrayWithGhostBoundaries(u,uLocal);
            realSerialArray fLocal;      getLocalArrayWithGhostBoundaries(f,fLocal);
            realSerialArray cLocal;      getLocalArrayWithGhostBoundaries(c,cLocal);
            intSerialArray maskLocal;    getLocalArrayWithGhostBoundaries(mask,maskLocal);

            realArray & varCoeffd = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid] : u;
            realSerialArray varCoeffLocal; getLocalArrayWithGhostBoundaries(varCoeffd,varCoeffLocal); 
            real *vcp = getDataPointer(varCoeffLocal);


        #else
            realSerialArray & defectLocal = defect;
            realSerialArray & uLocal = u;
            const realSerialArray & fLocal = f;
            const realSerialArray & cLocal = c;
            const intSerialArray & maskLocal = mask;

            realArray & varCoeffd = varCoeff!=NULL ? (*varCoeff).multigridLevel[level][grid] : u;
            const realSerialArray & varCoeffLocal = varCoeffd;  // this has the same dataPointer
            real *vcp = getDataPointer(varCoeffLocal);
        #endif



        real *up=getDataPointer(uLocal);
    // realMappedGridFunction & vv = defectMG.multigridLevel[level][grid]; // temp space for Red-Black jacobi
    // real *vp = getDataPointer(defectLocal);  // temp space for Red-Black jacobi
    // real *vcp = varCoeff!=NULL ? getDataPointer((*varCoeff).multigridLevel[level][grid]) : up;

        realSerialArray & vTemp =defectLocal; // temp space for Red-Black jacobi
    // RealArray vTemp;
    // vTemp=uLocal;
        real *vp=vTemp.getDataPointer();
        const real *pcc = constantCoefficients.getBound(2)>=level ? &constantCoefficients(0,grid,level) : vp;

        bool prePreSmooth=false; 
        bool preSmooth=false;
        bool postSmooth=false;
        bool postPostSmooth=true; // parameters.totalNumberOfSmooths %2 == 0; true;

        if( prePreSmooth   )
        { // pre-pre-smooth
      // smoothAllBoundaries();
            if( parameters.numberOfBoundaryLayersToSmooth>0 )
            {
                int bc[6]={1,1,1,1,1,1};
                int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth; //  + (numberOfLinesSolves%3)-1;
                smoothBoundary(level,grid,bc,numberOfLayers,parameters.numberOfBoundarySmoothIterations );
            }
        }

        int ipar[10]={0,0,0,0,0,0,0,0,0,0}; //
        real rpar[10]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.}; //

    // Here is the total number of smooths that we apply to this grid per cycle:
        const int numberOfPreAndPostSmooths=parameters.numberOfSubSmooths(grid,level)*
                              (parameters.numberOfSmooths(0,level)+parameters.numberOfSmooths(1,level));
        const int cycleType=OgmgParameters::cycleTypeF ? 0 : parameters.numberOfCycles(level);
        ipar[4]=numberOfPreAndPostSmooths;
        ipar[5]=cycleType;

        Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2]; Jv[3]=Range(0,0); // Jv[3]= components

        

        if( false && mg.numberOfDimensions()==3 )
        {
      // This option can give too large values in 3D -- cf ellipsoidFixed.cmd with RB for curvilinear grids ***
            parameters.useLocallyOptimalOmega=false;   // ************************* turn this off in 3D *****
        }
        
        for( int iteration=0; iteration<parameters.numberOfSubSmooths(grid,level); iteration++ )
        {

            if( preSmooth )
            {
      // 	smoothAllBoundaries();
            if( parameters.numberOfBoundaryLayersToSmooth>0 )
            {
                int bc[6]={1,1,1,1,1,1};
                int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth; //  + (numberOfLinesSolves%3)-1;
                smoothBoundary(level,grid,bc,numberOfLayers,parameters.numberOfBoundarySmoothIterations );
            }
            }
            for( int rb=0; rb<=1; rb++ )  // rb==0 : red points, rb==1 : black points
            {
                real time0=getCPU();

                real *u1p=up, *u2p=useJacobiRedBlack ? vp : up;
        // if( false && useJacobiRedBlack )
	// {
        //   // Jacobi-RB : First time update v from u, second time update u from v.
	//   u1p= rb==0 ? up : vp;
	//   u2p= rb==0 ? vp : up;
	// }
      	
                int n1a,n1b,n1c=1,n2a,n2b,n2c=1,n3a,n3b,n3c=1,shift1=0,shift2=0,shift3=0;
      	
                int redBlackOption = rb;

                #ifdef USE_PPP
      	if( Ogmg::debug & 8 ) 
                    ::display(uLocal,sPrintF(buff,"smoothRedBlack: uLocal at start of smooth grid=%i rb=%i it=%i cycle=%i jacobiRB=%i, bcEveryStep=%i",
                           				   grid,rb,iteration,numberOfCycles,(int)useJacobiRedBlack,(int)applyBoundaryConditionsAtEverySubStep),pDebugFile,fmt);
                #endif


      	if( !parameters.useNewRedBlackSmoother )
      	{
	  // old way
        	  shift1=rb;
        	  shift2=0;
        	  shift3=0;
      	
        	  n1a = I1a.getBase()+shift1;
        	  n1b = I1a.getBound()+shift1;
        	  n1c = 2;
        	  n2a = I2a.getBase()+shift2;
        	  n2b = I2a.getBound();
        	  n2c = 2;
        	  n3a = mg.numberOfDimensions()==2 ? I3a.getBase() : I3a.getBase()+shift3;
        	  n3b = mg.numberOfDimensions()==2 ? I3a.getBound() : I3a.getBound()+shift3;
        	  n3c = 2;
      	}
      	else
      	{ // new way -- no need to adjust bounds for red vs. black
        	  n1a = I1a.getBase(); n1b=I1a.getBound(); n1c=1;
        	  n2a = I2a.getBase(); n2b=I2a.getBound(); n2c=1;
        	  n3a = I3a.getBase(); n3b=I3a.getBound(); n3c=1;

                    redBlackOption = (n1a +rb+1 +128) % 2;  // match to old "red" points
      	}
      	const int nab[] = {n1a,n1b,n1c, n2a,n2b,n2c, n3a,n3b,n3c}; //  save for printing in parallel
      	
                #ifdef USE_PPP

      	if( debug & 16 )
      	{
                    fprintf(pDebugFile,"\n********** smoothRedBlack: grid=%i level=%i rb=%i\n",grid,level,rb);
                    fprintf(pDebugFile," Before: [n1a,n1b]=[%i,%i] [n2a,n2b]=[%i,%i] \n",n1a,n1b,n2a,n2b);
                    fprintf(pDebugFile," Before: mask = [%i,%i]x[%i,%i] \n",mask.getBase(0),mask.getBound(0),
              		  mask.getBase(1),mask.getBound(1));
                    fprintf(pDebugFile," Before: maskLocal = [%i,%i]x[%i,%i] \n",maskLocal.getBase(0),maskLocal.getBound(0),
              		  maskLocal.getBase(1),maskLocal.getBound(1));
                    fprintf(pDebugFile," Before: uLocal = [%i,%i]x[%i,%i] \n",uLocal.getBase(0),uLocal.getBound(0),
              		  uLocal.getBase(1),uLocal.getBound(1));
                    fflush(pDebugFile);
        	  Communication_Manager::Sync(); // *************
      	}

      	if( !parameters.useNewRedBlackSmoother )
      	{
  	  // int m1a = max(n1a,maskLocal.getBase(0)  +mask.getGhostBoundaryWidth(0));  
          // int m1b = min(n1b,maskLocal.getBound(0) -mask.getGhostBoundaryWidth(0));
          // int m2a = max(n2a,maskLocal.getBase(1)  +mask.getGhostBoundaryWidth(1));
          // int m2b = min(n2b,maskLocal.getBound(1) -mask.getGhostBoundaryWidth(1));
          // int m3a = max(n3a,maskLocal.getBase(2)  +mask.getGhostBoundaryWidth(2));
          // int m3b = min(n3b,maskLocal.getBound(2) -mask.getGhostBoundaryWidth(2));
            	  int m1a = max(n1a,maskLocal.getBase(0)  +hw[0]);
                    int m1b = min(n1b,maskLocal.getBound(0) -hw[0]);
                    int m2a = max(n2a,maskLocal.getBase(1)  +hw[1]);
                    int m2b = min(n2b,maskLocal.getBound(1) -hw[1]);
                    int m3a = max(n3a,maskLocal.getBase(2)  +hw[2]);
                    int m3b = min(n3b,maskLocal.getBound(2) -hw[2]);

          // adjust new n1a to be on the same red/black colour
                    n1b=m1b - (n1b-m1b)%2;
                    n1a=m1a + (m1a-n1a)%2;

                    n2b=m2b - (n2b-m2b)%2;
                    n2a=m2a + (m2a-n2a)%2;

                    n3b=m3b - (n3b-m3b)%2;
                    n3a=m3a + (m3a-n3a)%2;

                    if( n1a>m1a )
        	  {
            // ** not quite -- misses pts on far right
	    // **wdh* 061118  redBlackOption=1-redBlackOption;  // flip so we get all the points
        	  }
      	}
                else
      	{
  	  // n1a = max(n1a,maskLocal.getBase(0)  +mask.getGhostBoundaryWidth(0));  
          // n1b = min(n1b,maskLocal.getBound(0) -mask.getGhostBoundaryWidth(0));
          // n2a = max(n2a,maskLocal.getBase(1)  +mask.getGhostBoundaryWidth(1));
          // n2b = min(n2b,maskLocal.getBound(1) -mask.getGhostBoundaryWidth(1));
          // n3a = max(n3a,maskLocal.getBase(2)  +mask.getGhostBoundaryWidth(2));
          // n3b = min(n3b,maskLocal.getBound(2) -mask.getGhostBoundaryWidth(2));
            	  n1a = max(n1a,maskLocal.getBase(0)  +hw[0]);
                    n1b = min(n1b,maskLocal.getBound(0) -hw[0]);
                    n2a = max(n2a,maskLocal.getBase(1)  +hw[1]);
                    n2b = min(n2b,maskLocal.getBound(1) -hw[1]);
                    n3a = max(n3a,maskLocal.getBase(2)  +hw[2]);
                    n3b = min(n3b,maskLocal.getBound(2) -hw[2]);
      	}
                if( debug & 4 ) fprintf(pDebugFile," After: [n1a,n1b]=[%i,%i] [n2a,n2b]=[%i,%i] \n",n1a,n1b,n2a,n2b);

                #endif

                if( parameters.alternateSmoothingDirections )
      	{
          // Two possibilities for now:
                    if( parameters.alternateSmoothingDirections==1 )
        	  {
          	    int numMod = parameters.totalNumberOfSubSmooths(grid,level)+iteration % 2;
          	    if( numMod == 1 )
          	    {
	      // reverse the order of the smoother
            	      int temp;
            	      temp=n1b; n1b=n1a; n1a=temp; n1c=-n1c;
            	      temp=n2b; n2b=n2a; n2a=temp; n2c=-n2c;
            	      temp=n3b; n3b=n3a; n3a=temp; n3c=-n3c;

          	    }
        	  }
        	  else
        	  {
                        const int numberOfAlternatives=mg.numberOfDimensions()==2 ? 4 : 8;
          	    int numMod = (parameters.totalNumberOfSubSmooths(grid,level)+iteration) % numberOfAlternatives;
          	    int temp;
                        if( numMod % 2 == 1 )
          	    { // switch i1 every other sweep
            	      temp=n1b; n1b=n1a; n1a=temp; n1c=-n1c;
          	    }
          	    if( (numMod/2) % 2 == 1 )
          	    { // switch i2 every (2-3)th sweep
            	      temp=n2b; n2b=n2a; n2a=temp; n2c=-n2c;
          	    }
          	    if( (numMod/4) % 2 == 1 )
          	    { // switch i3 every (4-7)th sweep
            	      temp=n3b; n3b=n3a; n3a=temp; n3c=-n3c;
          	    }
        	  }
      	}

        // printF(" l=%i g=%i ts=%i it=%i rb=%i n1=[%i,%i,%i] n2=[%i,%i,%i] n3=[%i,%i,%i] \n",level,grid,
        //   parameters.totalNumberOfSubSmooths(grid,level)+iteration,iteration,rb,n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c);
        //printf("       d=[%i,%i][%i,%i][%i,%i] \n",d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2));
      	
      	
	// :::display(u,"smoothRedBlack: u before new smooth",debugFile,"%7.1e ");
	// ::display(f,"smoothRedBlack: f before new smooth",debugFile,"%7.1e ");

        // *** no need to smooth the boundary if dirichlet ***

//      real *uptr = u.Array_Descriptor.Array_View_Pointer3;
//      int shift=d(0,0)+(d(1,0)-d(0,0)+1)*( d(0,1)+(d(1,1)-d(0,1)+1)*( d(0,2) ) );
//        printF(" smooth: uptr=%i dp=%i diff=%i \n",uptr,u.getDataPointer(),u.getDataPointer()-uptr);
//	uptr += d(0,0)+(d(1,0)-d(0,0)+1)*( d(0,1)+(d(1,1)-d(0,1)+1)*( d(0,2) ) );
//	printf(" shift=%i uptr=%i\n",d(0,0)+(d(1,0)-d(0,0)+1)*( d(0,1)+(d(1,1)-d(0,1)+1)*( d(0,2) ) ),uptr );
//	int shiftc=c.getBase(0)+c.getRawDataSize(0)*(c.getBase(1)+c.getRawDataSize(1)*(c.getBase(2)+
//			          +c.getRawDataSize(2)*(c.getBase(3))));
      	
//    printF(" c.numberOfDimensions()=%i u.numberOfDimensions()=%i\n",c.numberOfDimensions(),u.numberOfDimensions());
      	
        // u.updateGhostBoundaries();  // ************************************************************** try this 091230


      	if( Ogmg::debug & 32 ) ::display(c,sPrintF(buff,"smoothRedBlack: c before smooth grid=%i rb=%i it=%i cycle=%i",
                                                  grid,rb,iteration,numberOfCycles),debugFile,"%5.1f ");
      	if( Ogmg::debug & 8 ) 
                    ::display(u,sPrintF(buff,"smoothRedBlack: u before smooth level=%i grid=%i rb=%i it=%i cycle=%i "
                                      "[%i,%i,%i][%i,%i,%i][%i,%i,%i]",level,grid,rb,iteration,numberOfCycles,
                                        nab[0],nab[1],nab[2],nab[3],nab[4],nab[5],nab[6],nab[7],nab[8]),debugFile,fmt);


      	if( useJacobiRedBlack )
      	{ 
	  // defect=u;
          // assign(defect,u);
                    vTemp=uLocal;
      	}
      	if( !parameters.useNewRedBlackSmoother )
      	{
        	  smoothRedBlackOpt( mg.numberOfDimensions(), 
                       			     maskLocal.getBase(0),maskLocal.getBound(0),
                       			     maskLocal.getBase(1),maskLocal.getBound(1),
                       			     maskLocal.getBase(2),maskLocal.getBound(2),
                       			     n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, 
                       			     *getDataPointer(fLocal),
                       			     *getDataPointer(cLocal),
                       			     *u1p, *u2p,
                       			     *getDataPointer(maskLocal), 
                       			     redBlackOption, orderOfAccuracy, sparseStencil, 
                       			     *pcc, *vcp, dx[0],
                       			     parameters.omegaRedBlack, (int)parameters.useLocallyOptimalOmega,
                       			     parameters.variableOmegaScaleFactor, ipar[0], rpar[0] );
      	}
      	else
      	{
        	  smRedBlack( mg.numberOfDimensions(), 
                  		      maskLocal.getBase(0),maskLocal.getBound(0),
                  		      maskLocal.getBase(1),maskLocal.getBound(1),
                  		      maskLocal.getBase(2),maskLocal.getBound(2),
                  		      n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c, ndc, 
                  		      *getDataPointer(fLocal),
                  		      *getDataPointer(cLocal),
                  		      *u1p, *u2p,
                  		      *getDataPointer(maskLocal), 
                  		      redBlackOption, orderOfAccuracy, sparseStencil, 
                  		      *pcc, *vcp, dx[0],
                  		      parameters.omegaRedBlack, (int)parameters.useLocallyOptimalOmega,
                  		      parameters.variableOmegaScaleFactor, ipar[0], rpar[0] );
      	}
      	
      	tm[timeForRelaxInSmooth]+=getCPU()-time0;
            
                realMappedGridFunction & uu = u; // !useJacobiRedBlack ? u : rb==0 ? vv : u;
      	if( useJacobiRedBlack )
      	{
//          realSerialArray & unc = (realSerialArray &)uLocal;  // remove const property
//	  unc=defectLocal;
	  // assign( uu,defect );
                    uLocal=vTemp;
        	}

      	if( Ogmg::debug & 8 )
      	{
          // note: do not display a view uu(J1,J2,J3) since this can cause trouble in parallel -- instead pass Jv
        	  int extra=1; getIndex(mg.gridIndexRange(),J1,J2,J3,extra);
                      ::display(uu,sPrintF(buff,"smoothRedBlack: u after smooth level=%i grid=%i rb=%i it=%i cycle=%i",
                        				level,grid,rb,iteration,numberOfCycles),debugFile,fmt,Jv);

                    #ifdef USE_PPP
        	  if( Ogmg::debug & 8 ) 
                        ::display(uLocal,sPrintF(buff,"smoothRedBlack: uLocal after smooth (before BC) grid=%i rb=%i it=%i cycle=%i jacobiRB=%i, bcEveryStep=%i",
                           				   grid,rb,iteration,numberOfCycles,(int)useJacobiRedBlack,(int)applyBoundaryConditionsAtEverySubStep),pDebugFile,fmt);
                    #endif
      	}
      	
                if( applyBoundaryConditionsAtEverySubStep )
      	{
        	  if( numExtraParallelGhost<=0 )
        	  {
	    // if there are no extra parallel ghost then we need to do a ghost boundary update
                        #ifdef USE_PPP
           	     if( debug & 4 )
             	       printF("smoothRedBlack:INFO: update parallel ghost boundaries (increase the ghost boundary with to avoid this)\n");
           	     uu.updateGhostBoundaries();
                        #endif
        	  }

            	  applyBoundaryConditions( level,grid,uu,f );  // *** wdh 991121 for periodic and Neumann BC's
      	}
      	
      	if( Ogmg::debug & 8 ) 
      	{
        	  int extra=1; getIndex(mg.gridIndexRange(),J1,J2,J3,extra);
                    ::display(uu,sPrintF(buff,
                                        "smoothRedBlack: u after applyBoundaryConditions level=%i grid=%i rb=%i it=%i cycle=%i",
                         			       level,grid,rb,iteration,numberOfCycles),debugFile,fmt,Jv);
      	}
      	
                #ifdef USE_PPP
      	if( Ogmg::debug & 8 ) 
                    ::display(uLocal,sPrintF(buff,"smoothRedBlack: uLocal after applyBoundaryConditions grid=%i rb=%i it=%i cycle=%i",
                                                  grid,rb,iteration,numberOfCycles),pDebugFile,fmt);
                #endif

            } // end for rb
            if( !applyBoundaryConditionsAtEverySubStep )
            {
      	applyBoundaryConditions( level,grid,u,f );  
            }

            #ifdef USE_PPP
      	if( Ogmg::debug & 8 ) 
                    ::display(uLocal,sPrintF(buff,"smoothRedBlack: uLocal after smooth and BC grid=%i it=%i cycle=%i jacobiRB=%i, bcEveryStep=%i",
               		   grid,iteration,numberOfCycles,(int)useJacobiRedBlack,(int)applyBoundaryConditionsAtEverySubStep),pDebugFile,fmt);
            #endif
            
            if( postSmooth )
            {
      // 	smoothAllBoundaries();
            if( parameters.numberOfBoundaryLayersToSmooth>0 )
            {
                int bc[6]={1,1,1,1,1,1};
                int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth; //  + (numberOfLinesSolves%3)-1;
                smoothBoundary(level,grid,bc,numberOfLayers,parameters.numberOfBoundarySmoothIterations );
            }
            }
            workUnits(level)+=mask.elementCount()/real(numberOfGridPoints);
            

        }  // end for interation
        

        if( postPostSmooth )
        {
      // smoothAllBoundaries();
            if( parameters.numberOfBoundaryLayersToSmooth>0 )
            {
                int bc[6]={1,1,1,1,1,1};
                int numberOfLayers=parameters.numberOfBoundaryLayersToSmooth; //  + (numberOfLinesSolves%3)-1;
                smoothBoundary(level,grid,bc,numberOfLayers,parameters.numberOfBoundarySmoothIterations );
            }
        }

    }
    else
    {

        realArray & uu = u;

        real time0=getCPU();

        getIndex(mg.dimension(),I1,I2,I3);
    // realArray defect(I1,I2,I3);    // removed 011129
    // defect=0.;                     // removed 011129                 

        realArray c0Inverse(1,I1,I2,I3); 

  // @PD realArray4[c,c0Inverse] Range[I1,I2,I3] 
        where( C(0,0,0,I1,I2,I3)!=0. )
            c0Inverse = 1./c(M123(0,0,0),I1,I2,I3);  // @PAW                 // this could be saved and reused     
        otherwise( )
            c0Inverse=0.;
    
        c0Inverse.reshape(I1,I2,I3);
        if( Ogmg::debug & 64 )
        {
            display(C(0,0,0,I1,I2,I3),"smoothRedBlack: C(0,0,0,I1,I2,I3) ",debugFile,"%6.2e ");
            display(c,"smoothRedBlack: c ",debugFile,"%6.2e ");
            display(c0Inverse,"smoothRedBlack: c0Inverse ",debugFile,"%6.2e ");
        }
        
        tm[timeForRelaxInSmooth]+=getCPU()-time0;

        const int shift2d1[] = {0,1, 1,0}; // red points followed by black points
        const int shift2d2[] = {0,1, 0,1}; // red points: shift1+shift2=even

        const int shift3d1[] = {0,1,1,0, 1,0,0,1}; // red points: shift1+shift2+shift3=even
        const int shift3d2[] = {0,1,0,1, 0,1,0,1}; // 
        const int shift3d3[] = {0,0,1,1, 0,0,1,1}; // 

        const int numberOfSweeps = mg.numberOfDimensions()==2 ? 2 : 4;  // number of sweeps to get all red/black points
        const int *shiftd1 = mg.numberOfDimensions()==2 ? shift2d1 : shift3d1;
        const int *shiftd2 = mg.numberOfDimensions()==2 ? shift2d2 : shift3d2;
        const int *shiftd3 = shift3d3;
    

        for( int iteration=0; iteration<parameters.numberOfSubSmooths(grid,level); iteration++ )
        {
      // ****this is not an efficient way to do red-black : fix so we do all red points a once then all black ***
      //  *** we also need a special defect to compute al red (or black) points ***

            for( int rb=0; rb<=1; rb++ )  // rb==0 : red points, rb==1 : black points
            {
      	for( int sweep=0; sweep<numberOfSweeps; sweep++ ) // red or black points take this many sweeps to get them all
      	{
        	  const int m=sweep+rb*numberOfSweeps;
        	  const int shift1= shiftd1[m];
        	  const int shift2= shiftd2[m];
        	  const int shift3= mg.numberOfDimensions()==3 ? shiftd3[m] : 0;

        	  I1=IndexBB(I1a.getBase()+shift1,I1a.getBound(),2);  // stride 2
        	  I2=IndexBB(I2a.getBase()+shift2,I2a.getBound(),2);  // stride 2
        	  I3= mg.numberOfDimensions()==3 ? IndexBB(I3a.getBase()+shift3,I3a.getBound(),2) : I3a;

        	  real time1=getCPU();

	  // ***** compute the defect at the red or black points *******
        	  getDefect(level,grid,f,u,I1,I2,I3,defect);

        	  time0=getCPU();
        	  tm[timeForDefectInSmooth]+=time0-time1;

// 	  if( parameters.problemIsSingular )
// 	  {
// 	    real rDotU=sum(u(I1,I2,I3)*rightNullVector.multigridLevel[level][grid](I1,I2,I3)); // this is not correct.
// 	    defect(I1,I2,I3)+=10.*rDotU*rightNullVector.multigridLevel[level][grid](I1,I2,I3);
// 	  }

        	  if( true || debug & 16 )
        	  {
                        const IntegerArray & gid=mg.gridIndexRange();
                        const IntegerArray & eir=mg.extendedIndexRange();
                        fPrintF(debugFile,"gridIndexRange    =[%i,%i][%i,%i]\n",gid(0,0),gid(1,0),gid(0,1),gid(1,1));
                        fPrintF(debugFile,"extendedIndexRange=[%i,%i][%i,%i]\n",eir(0,0),eir(1,0),eir(0,1),eir(1,1));

          	    fPrintF(debugFile,"redBlack: shift1=%i, shift2=%i, shift3=%i I1=[%i,%i,%i] I2=[%i,%i,%i] I3=[%i,%i,%i]\n",
                		    shift1,shift2,shift3,I1.getBase(),I1.getBound(),I1.getStride(),
                		    I2.getBase(),I2.getBound(),I2.getStride(),I3.getBase(),I3.getBound(),I3.getStride());
        	  }
        	  if( debug & 64 )
          	    displayMask(mask,sPrintF(buff,"mask in smooth redBlack, level=%i",level),debugFile);
            

	  // @PD realArray3[c0Inverse,uu,defect] Range[I1,I2,I3] 
	  // if( parameters.numberOfSubSmooths(grid,level)==1 )
          // **** never change interpolation points for iterative interpolation*****
        	  if( false && iteration == parameters.numberOfSubSmooths(grid,level)-1 )  // no mask needed on last iteration
        	  {
          	    uu(I1,I2,I3)+=defect(I1,I2,I3)*c0Inverse(I1,I2,I3);  // @PA
        	  }
        	  else
        	  { // do not change the interpolation point values: ** for implicit interpolation too ***
          	    where( mask(I1,I2,I3)>0 )
            	      uu(I1,I2,I3)+=defect(I1,I2,I3)*c0Inverse(I1,I2,I3);  // @PAW
        	  }

        	  tm[timeForRelaxInSmooth]+=getCPU()-time0;
//      uu.reshape(uu.dimension(1),uu.dimension(2),uu.dimension(3));
//      defect.reshape(defect.dimension(1),defect.dimension(2),defect.dimension(3));
        	  if( Ogmg::debug & 32 )
        	  {
          	    display(defect,sPrintF(buff,"smoothRedBlack: defect (shift1,shift2,shift3)=(%i,%i,%i)",
                           				   shift1,shift2,shift3),debugFile);
          	    display(u,sPrintF(buff,"smoothRedBlack: Here is u: (shift1,shift2,shift3)=(%i,%i,%i)",
                        			      shift1,shift2,shift3),debugFile);
        	  }
        	  if( debug & 16 )
          	    fPrintF(debugFile," **** smooth RB level=%i grid=%i defect =%e \n",level,grid,
                                    maxNorm(defectMG.multigridLevel[level][grid]));
      	
      	} // end sweep (end of red points or black points)

	// u.periodicUpdate();  // *** wdh 991121
      	applyBoundaryConditions( level,grid,u,f );  // *** wdh 991121 for periodic and Neumann BC's

        // ::display(u,"u after old smooth","%7.1e ");      

	// interpolate(uMG.multigridLevel[level]);
            
            } // for(rb)

            if( FALSE )
            {
      	realMappedGridFunction v;
      	v=u;
      	applyBoundaryConditions( level,grid,u,f ); // this make a difference for mixed ??
      	v=u-v;
      	real diff=maxNorm(v);
      	fPrintF(stdout,"max difference =%e, level=%i\n",maxNorm(v),level);
      	fPrintF(debugFile,"max difference =%e, level=%i \n",diff,level);
      	if( diff > 1.e-5 )
      	{
        	  display(v,"Difference in u after second applyBC",debugFile,"%9.2e");
        	  display(f,"f",debugFile,"%9.2e");
        	  display(c,"c",debugFile,"%9.2e");
        	  throw "error";
      	}
            }
        
      // applyBoundaryConditions( level,grid,u,f );

    // workUnits(level)+=1./mgcg.multigridLevel[level].numberOfComponentGrids();  // should weight by number of pts
            workUnits(level)+=mask.elementCount()/real(numberOfGridPoints);

        } // end for interation
    }

}


#undef M123
#undef C
