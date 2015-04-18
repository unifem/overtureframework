#include "Ogmg.h"
#include "Oges.h"
#include "SparseRep.h"
#include "Integrate.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"

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

// ======================================================================================
// \brief Compute the parameter alpha needed to project the righ-hand-side for singular
///  problems: 
///     alpha(level) = (leftNullVector,f)/(leftNullVector,rightNullVector)
///
// ======================================================================================
int Ogmg::
getSingularParameter(int level)
{
  bool useOpt=true; // use new optimized version

  CompositeGrid & mgcg = multigridCompositeGrid();
  if( alpha.getLength(0)<mgcg.numberOfMultigridLevels() )
  {
    alpha.redim(mgcg.numberOfMultigridLevels());
    alpha=0.;
  }
  
  CompositeGrid & cg = mgcg.multigridLevel[level];
  realCompositeGridFunction & f = fMG.multigridLevel[level];
  realCompositeGridFunction & leftNull = (*leftNullVector).multigridLevel[level];
  realCompositeGridFunction & rightNull = rightNullVector.multigridLevel[level];

 

  Index I1,I2,I3;
  alpha(level)=0.;
  real leftDotF=0., leftDotRight=0.;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(cg[grid].dimension(),I1,I2,I3);

    if( debug & 8 )
      ::display(f[grid],"getSingularParameter: compute l.f, here is f","%6.2f ");

    if( useOpt )
    {
      OV_GET_SERIAL_ARRAY(real,f[grid],fLocal);
      int includeGhost=0; // do NOT include parallel ghost points
      bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1,I2,I3,includeGhost);
      if( ok )
      {
	OV_GET_SERIAL_ARRAY(real,leftNull[grid],leftNullLocal) ;
	OV_GET_SERIAL_ARRAY(real,rightNull[grid],rightNullLocal) ;

 	real *leftNullp = leftNullLocal.Array_Descriptor.Array_View_Pointer2;
 	const int leftNullDim0=leftNullLocal.getRawDataSize(0);
 	const int leftNullDim1=leftNullLocal.getRawDataSize(1);
        #define LEFTNULL(i0,i1,i2) leftNullp[i0+leftNullDim0*(i1+leftNullDim1*(i2))]

	real *rightNullp = rightNullLocal.Array_Descriptor.Array_View_Pointer2;
	const int rightNullDim0=rightNullLocal.getRawDataSize(0);
	const int rightNullDim1=rightNullLocal.getRawDataSize(1);
        #define RIGHTNULL(i0,i1,i2) rightNullp[i0+rightNullDim0*(i1+rightNullDim1*(i2))]

	real *fp = fLocal.Array_Descriptor.Array_View_Pointer2;
	const int fDim0=fLocal.getRawDataSize(0);
	const int fDim1=fLocal.getRawDataSize(1);
        #define F(i0,i1,i2) fp[i0+fDim0*(i1+fDim1*(i2))]
	int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
          leftDotF+= LEFTNULL(i1,i2,i3)*F(i1,i2,i3);
          leftDotRight+= LEFTNULL(i1,i2,i3)*RIGHTNULL(i1,i2,i3);
	}
      }
    
    }
    else
    {
      leftDotF+=sum(leftNull[grid](I1,I2,I3)*f[grid](I1,I2,I3));
      leftDotRight+=sum(leftNull[grid](I1,I2,I3)*rightNull[grid](I1,I2,I3));  // could scale so this is 1.
    }
    
  }
  // sum leftDotF and leftDotRight over all processors
  real val[2]={leftDotF, leftDotRight}, valSum[2]; // 
  ParallelUtility::getSums(val,valSum,2);
  leftDotF=valSum[0]; 
  leftDotRight=valSum[1];
  alpha(level)= leftDotF/leftDotRight;
  
  if( debug & 4 )
    printF("%*.1s **Compatibility value: level=%i alpha = l.f/l.r = %8.2e **\n",level*4," ",level,alpha(level));

  return 0;
}

#undef LEFT
#undef RIGHT
#undef F


int Ogmg::
saveLeftNullVector()
// ======================================================================================
// /Description:
//    Save the left-null-vector in a file.
// ======================================================================================
{
  assert( leftNullVector!=NULL );
  CompositeGrid & mgcg = multigridCompositeGrid();
  
  HDF_DataBase dataFile;
  dataFile.mount(parameters.nullVectorFileName,"I");

  int streamMode=0;  // 1=save in compressed form.
  dataFile.put(streamMode,"streamMode");
  if( !streamMode )
    dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
  else
  {
    dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
  }

  // *** trouble using this next line ***
  //  leftNullVector->put(dataFile,"leftNullVector");

  int numLevels = parameters.useDirectSolverOnCoarseGrid ? mgcg.numberOfMultigridLevels()-1 : 
    mgcg.numberOfMultigridLevels();
  
  for( int level=0; level<numLevels; level++ )  
  {
    RealCompositeGridFunction & nullVector= (*leftNullVector).multigridLevel[level];

    nullVector.put(dataFile,sPrintF("leftNullVectorLevel%i",level));
  }
  

  dataFile.unmount();

  if( false && Ogmg::debug & 1 )
    printF("Ogmg::saveLeftNullVector:left null vector was saved in the file [%s]\n",
	   (const char*)parameters.nullVectorFileName);

  return 0;
}

int Ogmg::
readLeftNullVector()
// ======================================================================================
// /Description:
//    Attempt to read the left null vector from a file.
//
// /return value: 0=success, 1=file was not found
// ======================================================================================
{
  if( !(parameters.nullVectorOption==OgmgParameters::readOrComputeNullVector ||
        parameters.nullVectorOption==OgmgParameters::readOrComputeAndSaveNullVector) )
  {
    // we cannot read the null vector in this case
    return 1;  
  }
  
  CompositeGrid & mgcg = multigridCompositeGrid();
  if( leftNullVector==NULL )
    leftNullVector=new realCompositeGridFunction(mgcg);
  
  if( leftNullVectorIsComputed ) return 0;  // null vector has already been read or computed directly


  FILE *file = fopen(parameters.nullVectorFileName,"r");
  if( file==0 )
  {
   if( Ogmg::debug & 1 ) 
     printF("Ogmg::readLeftNullVector:left null vector was NOT found."
	    " Unable to open file [%s].\n",(const char*)parameters.nullVectorFileName);
    return 1;  // file was not found
  }
  fclose(file);
   
  HDF_DataBase dataFile;

  bool found = dataFile.mount(parameters.nullVectorFileName, "R")==0;
  assert( found );

  // check to see if this file was saved in streamMode (compressed)
  int streamMode=true; // use this as a default since this is the old way.
  dataFile.get(streamMode,"streamMode");

  if( !streamMode )
    dataFile.setMode(GenericDataBase::noStreamMode);

  // *** trouble using this next line ***
  // leftNullVector->get(dataFile,"leftNullVector");

  int numLevels = parameters.useDirectSolverOnCoarseGrid ? mgcg.numberOfMultigridLevels()-1 : 
    mgcg.numberOfMultigridLevels();
  
  for( int level=0; level<numLevels; level++ )  
  {
    RealCompositeGridFunction & nullVector= (*leftNullVector).multigridLevel[level];

    nullVector.get(dataFile,sPrintF("leftNullVectorLevel%i",level));

    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
      nullVector[grid].updateGhostBoundaries();  // ******** TEMP *********** try this 
    }
    
  }

  dataFile.unmount();

  if( Ogmg::debug & 1 ) 
    printF("Ogmg::readLeftNullVector:left null vector was successfully read from the file [%s]\n",
	   (const char*)parameters.nullVectorFileName);

  if(  Ogmg::debug & 8 )
  {
    for( int level=0; level<numLevels; level++ )  
    {
      RealCompositeGridFunction & nullVector= (*leftNullVector).multigridLevel[level];

      nullVector.display(sPrintF("Here is the leftNullVector level=%i",level),"%5.1e ");
    }
  }
  
  leftNullVectorIsComputed=true;

  return 0;
}



int Ogmg::
computeLeftNullVector()
// ======================================================================================
// /Description:
//    Compute the left-null-vector for a singular problem
// ======================================================================================
{
  if( Ogmg::debug & 1 ) 
    printF("Ogmg::computeLeftNullVector: compute the left null vector (or read it from a file)...\n");

  CompositeGrid & mgcg = multigridCompositeGrid();
  if( leftNullVector==NULL )
    leftNullVector=new realCompositeGridFunction(mgcg);

  // attempt to read the null vector from a file
  bool found = readLeftNullVector()==0;
  if( found ) 
  {
    leftNullVectorIsComputed=true;
    return 0;
  }

  int numLevels = parameters.useDirectSolverOnCoarseGrid ? mgcg.numberOfMultigridLevels()-1 : 
              mgcg.numberOfMultigridLevels();
  
  for( int level=0; level<numLevels; level++ )  
  {
    Oges solver;
    if( parameters.nullVectorParameters!=NULL )
    {
      solver.setOgesParameters(*parameters.nullVectorParameters);
    }
     CompositeGrid & cg = mgcg.multigridLevel[level];
     RealCompositeGridFunction & coeff = cMG.multigridLevel[level];
     RealCompositeGridFunction & nullVector= (*leftNullVector).multigridLevel[level];

     if( false  )
     {
      Integrate integrate(cg);
      RealCompositeGridFunction & leftNull = integrate.leftNullVector();
      nullVector=leftNull;
      
     }
     else
     {
       RealCompositeGridFunction f(cg);   // **** could probably use some other work space

       const int numberOfDimensions = cg.numberOfDimensions();
       int grid;

       Range all;

       // *** we need to finish BC's --- could do this elsewhere
//      int stencilSize=10, orderOfAccuracy=2;
//      CompositeGridOperators op(cg);                            // create some differential operators
//      op.setStencilSize(stencilSize);
//      op.setOrderOfAccuracy(orderOfAccuracy);
//      coeff.setOperators(op);

//      op.finishBoundaryConditions(coeff);

//      coeff.finishBoundaryConditions();
     
       solver.updateToMatchGrid(cg);
  
       // count the total number of grid points.
       int numberOfGridPoints=0;
       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	 numberOfGridPoints+=cg[grid].mask().elementCount();

       Index I1,I2,I3;
       bool useIterativeSolver= false || numberOfDimensions==3;
       if( useIterativeSolver )
       {
	 solver.set(OgesParameters::THEbestIterativeSolver);
	 // solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
	 // solver.setConjugateGradientPreconditioner(Oges::diagonal);
	 // Oges::debug=7;
	 solver.set(OgesParameters::THEtolerance,REAL_EPSILON*numberOfGridPoints);
       }    

       const int maxit=1000;  // this should depend on the number of points on the coarse grid!
       solver.set(OgesParameters::THEmaximumNumberOfIterations,maxit);

       // *wdh* 2012/12/23
       solver.set(OgesParameters::THEnumberOfIncompleteLULevels,5);

       solver.set(OgesParameters::THEsolveForTranspose,true); // solve the transpose system (we want the left null vector)
       solver.set(OgesParameters::THEfixupRightHandSide,false);     // no need to zero out equations at special points.

       solver.set(OgesParameters::THErescaleRowNorms,false);  // *wdh* 2014/12/22 do NOT rescale rows! fix for parallel

       bool solveSingularProblem=true;


     // assign the rhs: f=0 except for the rhs to the compatibility equation which we set to the numberOfGridPoints.
     // (this will cause the sum of the interior weights to be numberOfGridPoints so that each value should be about 1)
       f=0.;
       solver.setCoefficientArray( coeff );     // supply coefficients
       if( solveSingularProblem )
	 solver.set(OgesParameters::THEcompatibilityConstraint,true); // system is singular so add an extra equation
       solver.initialize();

       // find the equation where the compatibility constraint is put (some unused point)
       // int n,i1e,i2e,i3e,gride;
       // solver.equationToIndex(solver.extraEquationNumber(0),n,i1e,i2e,i3e,gride);
       // f[gride](i1e,i2e,i3e,n)=numberOfGridPoints;

       // printF(" ***Extra equation: n,i1e,i2e,i3e,gride=%i %i %i %i %i \n",n,i1e,i2e,i3e,gride);
     
       real value=numberOfGridPoints;
       solver.setExtraEquationValues( f,&value );  

       // nullVector.updateToMatchGrid(cg);
       if( solver.isSolverIterative() )
	 printF(" $$$$$$$$$$$$ iterative solve for null vector on level=%i $$$$$$$$$$$$\n",level);
       if( true || solver.isSolverIterative() )
       {
	 
	 nullVector=1.;
	 if( false )
	 {
	   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	   {
	     MappedGrid & c = cg[grid];
	     const IntegerDistributedArray & classify = coeff[grid].sparse->classify;

	     getIndex(c.dimension(),I1,I2,I3);
	     where( classify(I1,I2,I3)!=(int)SparseRepForMGF::interior && 
		    classify(I1,I2,I3)!=(int)SparseRepForMGF::boundary &&
		    classify(I1,I2,I3)!=(int)SparseRepForMGF::ghost1 )
	     {
	       nullVector[grid](I1,I2,I3)=0.;
	     }
	   }
	 }
	 
       }
       
	 
       if( debug & 4 )
	 displayCoeff(coeff[0],"coeff[0]",stdout,"%5.1f ");
	 
       // coeff.display("coeff");

       // Oges::debug=63;
     
       printF("**** Solve for leftNullVector on level=%i *******\n",level);
       real time=getCPU();
       solver.solve( nullVector,f );   // solve for the (unscaled weights)
       time=getCPU()-time;
       printF("**** ...done cpu=%8.2e (s)\n",time);

       if( Ogmg::debug & 1 && solver.isSolverIterative() )
       {
	 real absoluteTolerance,relativeTolerance;
	 solver.get(OgesParameters::THEabsoluteTolerance,absoluteTolerance);
	 solver.get(OgesParameters::THErelativeTolerance,relativeTolerance);
	 real maxResidual=solver.getMaximumResidual();
    
	 printF("Ogmg::computeLeftNullVector: iter's to solve = %i (max res=%8.2e "
		"rel-tol=%7.1e, abs-tol=%7.1e)\n",
		solver.getNumberOfIterations(),maxResidual,relativeTolerance,absoluteTolerance);
       }
       
       // zero out the nullVector at unused points. 
       // nullVector values are non-zero at interpolation points -- the RHS should be zero at these
       // points anyway.
       const bool useOpt=true;
       
       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
       {
	 MappedGrid & c = cg[grid];
	 const IntegerDistributedArray & classify = coeff[grid].sparse->classify;

	 if( debug & 4 ) // && orderOfAccuracy==4 )
	 {
	   ::display(classify,"Here is the classify array","%3i ");
	   ::display(nullVector[grid],"Here is the (unscaled) nullVector from solve","%5.2f ");
	 }

	 getIndex(c.dimension(),I1,I2,I3);
	 if( useOpt )
	 {
	    OV_GET_SERIAL_ARRAY(real,nullVector[grid],nullVectorLocal);
            bool ok = ParallelUtility::getLocalArrayBounds(nullVector[grid],nullVectorLocal,I1,I2,I3,1);

	    // if( orderOfAccuracy==4 ) continue; // ******************* TEMP *********************
	    if( ok )
	    {
	      real *nullVectorp = nullVectorLocal.Array_Descriptor.Array_View_Pointer2;
  	      const int nullVectorDim0=nullVectorLocal.getRawDataSize(0);
	      const int nullVectorDim1=nullVectorLocal.getRawDataSize(1);
              #define NULLVECTOR(i0,i1,i2) nullVectorp[i0+nullVectorDim0*(i1+nullVectorDim1*(i2))]

  	      OV_GET_SERIAL_ARRAY_CONST(int,classify,classifyLocal);
	      int *classifyp = classifyLocal.Array_Descriptor.Array_View_Pointer2;
  	      const int classifyDim0=classifyLocal.getRawDataSize(0);
	      const int classifyDim1=classifyLocal.getRawDataSize(1);
              #define CLASSIFY(i0,i1,i2) classifyp[i0+classifyDim0*(i1+classifyDim1*(i2))]
  	      int i1,i2,i3;
	      if( level==0 )
	      {
		FOR_3D(i1,i2,i3,I1,I2,I3)
		{
		  if( CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::interior && 
		      CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::boundary &&
		      CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::ghost1   &&
		      CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::ghost2 )
		  {
		    NULLVECTOR(i1,i2,i3)=0.;
		  }
		}
	      }
	      else
	      {
		// *wdh* 2013/11/30 -- fix for 4th-order, level>0, ghost2 was not set to zero
		FOR_3D(i1,i2,i3,I1,I2,I3)
		{
		  if( CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::interior && 
		      CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::boundary &&
		      CLASSIFY(i1,i2,i3)!=(int)SparseRepForMGF::ghost1  )
		  {
		    NULLVECTOR(i1,i2,i3)=0.;
		  }
		}
	      }
	    }
	    
            #undef NULLVECTOR	    
            #undef CLASSIFY
	 }
	 else
	 { // non-opt way 
	   where( classify(I1,I2,I3)!=(int)SparseRepForMGF::interior && 
		  classify(I1,I2,I3)!=(int)SparseRepForMGF::boundary &&
		  classify(I1,I2,I3)!=(int)SparseRepForMGF::ghost1 )
	   {
	     nullVector[grid](I1,I2,I3)=0.;
	   }
	 }
	 
       }
       if( debug & 4 ) // true && orderOfAccuracy==4  )
	 nullVector.display(sPrintF("Here is the (scaled) leftNullVector level=%i",level),"%5.2f ");
     
     }
     
  }

  if( parameters.nullVectorOption==OgmgParameters::computeAndSaveNullVector ||
      parameters.nullVectorOption==OgmgParameters::readOrComputeAndSaveNullVector )
  {
    // save the null vector in a file (so it can be used in future computations)
    saveLeftNullVector();
  }

  leftNullVectorIsComputed=true;

  return 0;
}


//! Adjust the right-hand-side for a singular problem
int Ogmg::
addAdjustmentForSingularProblem(int level, int iteration )
{

  if( !parameters.problemIsSingular || !parameters.projectRightHandSideForSingularProblem )
    return 0;

  // fMG.multigridLevel[level]+=alpha(level)*rightNullVector.multigridLevel[level];

  bool useOpt=true; // use new optimized version

  CompositeGrid & mgcg = multigridCompositeGrid();
  if( true )
  {
    // determine alpha for the equation  
    //           L u + alpha*r = f
    //           r.u = g

    if( parameters.useDirectSolverOnCoarseGrid && level==mgcg.numberOfMultigridLevels()-1 )
      return 0;


    if( !leftNullVectorIsComputed )
    {
      computeLeftNullVector();
    }

    if( level>0 || iteration==0 )
    {
      // compute alpha(level) = (leftNullVector,f)/(leftNullVector,rightNullVector) :
      getSingularParameter(level);
        
      if( useOpt )
      {
	CompositeGrid & cg = mgcg.multigridLevel[level];
	realCompositeGridFunction & f = fMG.multigridLevel[level];
	realCompositeGridFunction & rightNull = rightNullVector.multigridLevel[level];
	const real alphal = alpha(level);
	Index I1,I2,I3;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].dimension(),I1,I2,I3);
	  OV_GET_SERIAL_ARRAY(real,f[grid],fLocal) ;
	  bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1,I2,I3,1);
	  if( ok )
	  {
	    OV_GET_SERIAL_ARRAY(real,rightNull[grid],rightNullLocal) ;
	    real *rightNullp = rightNullLocal.Array_Descriptor.Array_View_Pointer2;
  	    const int rightNullDim0=rightNullLocal.getRawDataSize(0);
	    const int rightNullDim1=rightNullLocal.getRawDataSize(1);
            #define RIGHTNULL(i0,i1,i2) rightNullp[i0+rightNullDim0*(i1+rightNullDim1*(i2))]

	    real *fp = fLocal.Array_Descriptor.Array_View_Pointer2;
	    const int fDim0=fLocal.getRawDataSize(0);
	    const int fDim1=fLocal.getRawDataSize(1);
            #define F(i0,i1,i2) fp[i0+fDim0*(i1+fDim1*(i2))]

  	    int i1,i2,i3;
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      F(i1,i2,i3) -= alphal*RIGHTNULL(i1,i2,i3);
	    }
	  }

          f[grid].updateGhostBoundaries();  // ************* TEMP -- try this 
	}
      }
      else
      { // old way 
	fMG.multigridLevel[level]-=alpha(level)*rightNullVector.multigridLevel[level];
      }
      
	
    }
  }
  else
  {
    // Here we play around with different ways to compute alpha      

//     if( !( (level==0 && (debug & 2)) || debug & 4) )
//     {
//       defect(level);   
//       defectOld= max(fabs(defectMG.multigridLevel[level]));
//     }
      

    // real alphaOld=0.;
//     if( true || defectOld<.1 )
//     {
//       // alphaOld=alpha(level);
//       alpha(level)=rightNullVectorDotU( level, defectMG.multigridLevel[level] );
//     }
    
//     real rDotU = rightNullVectorDotU( level, uMG.multigridLevel[level] );
//     printf(" *** level=%i : defectOld=%8.2e, alpha=%8.2e, rDotU=%8.2e \n",level,defectOld,alpha(level),rDotU);
    
//  const real beta=1.;
//  alpha(level)-=rDotU*beta;

    // fMG.multigridLevel[level]+=1.*rightNullVector.multigridLevel[level]; // shift for testing
    
    if( iteration==0 )
    {
      for( int it=0; it<200; it++ )
      {

	smooth(level,parameters.numberOfSmooths(0,level),it);

	real rDotU=parameters.meanValueForSingularProblem-rightNullVectorDotU( level, uMG.multigridLevel[level] );
	real rDotV=rightNullVectorDotU(level,*v);
	real beta = rDotU/rDotV;
    
	alpha(level)+=beta;
	uMG.multigridLevel[level]+=beta*(*v);
    
      
	fMG.multigridLevel[level]-=beta*rightNullVector.multigridLevel[level];

	if( it % 5 == 0 )
	  printf(" >>>>level=%i: it=%i: rDotU=%8.2e, rDotV=%8.2e, beta=%8.2e, alpha=%8.2e \n",level,
		 it,rDotU,rDotV,beta,alpha(level));

      }
      uMG.multigridLevel[level]=0.;
      
    }

//     real rDotU=parameters.meanValueForSingularProblem-rightNullVectorDotU( level, uMG.multigridLevel[level] );
//     real rDotV=rightNullVectorDotU(level,*v);
//     real beta = rDotU/rDotV;
    
//     alpha(level)+=beta;
//     uMG.multigridLevel[level]+=beta*(*v);
    
      
//     fMG.multigridLevel[level]-=beta*rightNullVector.multigridLevel[level];

//     printf(" >>>>level=%i: it=%i: rDotU=%8.2e, rDotV=%8.2e, beta=%8.2e, alpha=%8.2e \n",level,
// 	   iteration,rDotU,rDotV,beta,alpha(level));
      


    // fMG.multigridLevel[level]-=alpha(level)*rightNullVector.multigridLevel[level];

//     rDotU=parameters.meanValueForSingularProblem-rightNullVectorDotU( level, uMG.multigridLevel[level] );
//     printf(">>>>AFTER:  rDotU=%8.2e (=0 ?) \n", rDotU);
      
  }

  return 0;
}
#undef F
#undef RIGHTNULL


/// ==================================================================================
/// \brief Remove the adjustment to the right-handside for a singular problem
// ==================================================================================
int Ogmg::
removeAdjustmentForSingularProblem(int level, int iteration )
{
  if( !parameters.problemIsSingular )
    return 0;
   
  bool useOpt=true; // use new optimized version
  if( level==0 ) 
  {
    // reset the right hand side
    if( alpha(level) != 0. )
    {
      if( useOpt )
      {
        CompositeGrid & mgcg = multigridCompositeGrid();
	CompositeGrid & cg = mgcg.multigridLevel[level];
	realCompositeGridFunction & f = fMG.multigridLevel[level];
	realCompositeGridFunction & rightNull = rightNullVector.multigridLevel[level];
	const real alphal = alpha(level);
	Index I1,I2,I3;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  getIndex(cg[grid].dimension(),I1,I2,I3);
	  OV_GET_SERIAL_ARRAY(real,f[grid],fLocal) ;
	  bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1,I2,I3,1);
	  if( ok )
	  {
	    OV_GET_SERIAL_ARRAY(real,rightNull[grid],rightNullLocal) ;
	    real *rightNullp = rightNullLocal.Array_Descriptor.Array_View_Pointer2;
  	    const int rightNullDim0=rightNullLocal.getRawDataSize(0);
	    const int rightNullDim1=rightNullLocal.getRawDataSize(1);
            #define RIGHTNULL(i0,i1,i2) rightNullp[i0+rightNullDim0*(i1+rightNullDim1*(i2))]

	    real *fp = fLocal.Array_Descriptor.Array_View_Pointer2;
	    const int fDim0=fLocal.getRawDataSize(0);
	    const int fDim1=fLocal.getRawDataSize(1);
            #define F(i0,i1,i2) fp[i0+fDim0*(i1+fDim1*(i2))]

  	    int i1,i2,i3;
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      F(i1,i2,i3) += alphal*RIGHTNULL(i1,i2,i3);
	    }
	  }

          f[grid].updateGhostBoundaries();  // ************* TEMP -- try this 

	}
      }
      else
      { // old way 

	fMG.multigridLevel[level]+=alpha(level)*rightNullVector.multigridLevel[level];
      }
    }
  }

  return 0;
}
#undef F
#undef RIGHTNULL
