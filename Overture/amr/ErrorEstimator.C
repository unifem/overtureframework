#include "ErrorEstimator.h"
#include "PlotStuff.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "InterpolateRefinements.h"
#include "App.h"
#include "ParallelUtility.h"

#ifdef USE_PPP
#ifndef OV_USE_DOUBLE
#define MPI_Real MPI_FLOAT
#else
#define MPI_Real MPI_DOUBLE
#endif
#endif

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


//\begin{>ErrorEstimatorInclude.tex}{\subsection{Constructor}} 
ErrorEstimator::
ErrorEstimator(InterpolateRefinements & interpolateRefinements_)
//=========================================================================================
// /Description:
//     Use this class to perform various interpolation operations on adaptively refined grids.
//
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  debug=0;

  myid=max(0,Communication_Manager::My_Process_Number);

  debugFile=NULL;
  

  interpolateRefinements=&interpolateRefinements_;

  defaultNumberOfSmoothingSteps=1;
  maximumNumberOfRefinementLevels=INT_MAX;  // no need to compute errors on the finest level
  
  weightFirstDifference=1.;
  weightSecondDifference=1.;
  
  topHatCentre[0]=.35; topHatCentre[1]=.35; topHatCentre[2]=0.;
  topHatVelocity[0]=1.; topHatVelocity[1]=1.; topHatVelocity[2]=0.;
  topHatRadius=.15;  
  topHatRadiusX=topHatRadius;
  topHatRadiusY=topHatRadius;
  topHatRadiusZ=topHatRadius;
}

ErrorEstimator::
~ErrorEstimator()
{
  // if( debugFile!=NULL ) fclose(debugFile);
}


void ErrorEstimator::
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
//        debugFile = fopen("errorEstimator.debug","w" ); 
//      else
//        debugFile = fopen(sPrintF("errorEstimator%i.debug",myid),"w" ); 
//    }
  
}




//\begin{>>ErrorEstimatorInclude.tex}{\subsection{setDefaultNumberOfSmooths}} 
int ErrorEstimator::
setDefaultNumberOfSmooths( int numberOfSmooths )
//=========================================================================================
// /Description:
//   Set the default number of smoothing steps for smoothing the error.
// 
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  defaultNumberOfSmoothingSteps=numberOfSmooths;
  
  return 0;
}


//! Set the maximum number of refinement levels.
/*!
    The error is not computed on the finest level.
 */
int ErrorEstimator::
setMaximumNumberOfRefinementLevels( int maxLevels )
{
  maximumNumberOfRefinementLevels=maxLevels;
  return 0;
}



//\begin{>>ErrorEstimatorInclude.tex}{\subsection{setScaleFactor}} 
int ErrorEstimator::
setScaleFactor( RealArray & scaleFactor_ )
//=========================================================================================
// /Description:
//  Assign scale factors to scale each component of the solution when the error is computed.
// If no scale factors are specified then the a scale factor will be determined automatically.
// 
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  scaleFactor.redim(0);
  scaleFactor=scaleFactor_;
  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{setTopHatParameters}} 
int ErrorEstimator::
setTopHatParameters( real topHatCentre_[3], 
		     real topHatVelocity_[3], 
		     real topHatRadius_,
                     real topHatRadiusX_ /* =0. */, 
		     real topHatRadiusY_ /* =0. */, 
		     real topHatRadiusZ_ /* =0. */ )
//=========================================================================================
// /Description:
//   Define the parameters for the top-hat function.
//
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  for(int axis=0; axis<3; axis++ )
  {
    topHatCentre[axis]=topHatCentre_[axis];
    topHatVelocity[axis]=topHatVelocity_[axis];
  }
  topHatRadius=topHatRadius_;
  topHatRadiusX=topHatRadiusY=topHatRadiusZ=topHatRadius;
  
  if( topHatRadiusX_ >0. )
    topHatRadiusX=topHatRadiusX_;
  if( topHatRadiusY_ >0. )
    topHatRadiusY=topHatRadiusY_;
  if( topHatRadiusZ_ >0. )
    topHatRadiusZ=topHatRadiusZ_;
  
  return 0;
}

//\begin{>>ErrorEstimatorInclude.tex}{\subsection{setWeights}}
int ErrorEstimator::
setWeights( real weightFirstDifference_, real weightSecondDifference_ )
//=========================================================================================
// /Description:
//   Assign the weights in the error function.
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  weightFirstDifference=weightFirstDifference_;
  weightSecondDifference=weightSecondDifference_;
  
  return 0;
}



//\begin{>>ErrorEstimatorInclude.tex}{\subsection{computeErrorFunction}} 
int ErrorEstimator::
computeErrorFunction( realGridCollectionFunction & error, ErrorFunctionEnum type )
//=========================================================================================
// /Description:
//    Compute a pre-defined error function of a particular form. These are used to test
// the AMR grid generator.
// \begin{description}
//   \item[twoSolidCircles] : error is 1 inside two circles.
//   \item[diagonal] : error is 1 along a diagonal
//   \item[cross]: error is 1 along two diagonals
//   \item[plus] : error is 1 along a horizonal and vertical line, forming a "plus"
//   \item[hollowCircle] : error is 1 near the boundary of a circle.
// \end{description}
//
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  GridCollection & gc = *error.getGridCollection();
  
  // error = 0.; // *wdh* 030916
  
  Index I1,I2,I3,all;
  
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>=maximumNumberOfRefinementLevels-1 )
    {
//      printf("ErrorEstimator::computeErrorFunction: skip grid %i level=%i\n",grid,gc.refinementLevelNumber(grid));
      continue;  // no need to compute errors on the finest level
    }
//       printf("ErrorEstimator::computeErrorFunction: grid %i level=%i maxLevels=%i\n",grid,
//                  gc.refinementLevelNumber(grid),maximumNumberOfRefinementLevels);
    
    realSerialArray err; getLocalArrayWithGhostBoundaries(error[grid],err);

    getIndex(gc[grid].dimension(),I1,I2,I3);

    const int includeGhost=1;
    bool ok=ParallelUtility::getLocalArrayBounds(error[grid],err,I1,I2,I3,includeGhost);
    if( !ok ) continue;

    MappedGrid & mg = gc[grid];
    mg.update(MappedGrid::THEvertex );
    
    realSerialArray x;   getLocalArrayWithGhostBoundaries(gc[grid].vertex(),x);

    err(I1,I2,I3,0) = 0.;

    if( type==twoSolidCircles )
    {
      RealArray radius0(I1,I2,I3), radius1(I1,I2,I3);

      real x0=.4, y0=.4,z0=0., x1=.6,y1=.6,z1=0.;
      real rad=.125;
    
      if( gc.numberOfDimensions()==2 )
      {
	radius0(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-y0);
	radius1(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x1)+SQR(x(I1,I2,I3,1)-y1);
      }
      else if( gc.numberOfDimensions()==3 )
      {
	radius0(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-y0)+
                SQR(x(I1,I2,I3,1)-z0);
	radius1(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x1)+SQR(x(I1,I2,I3,1)-y1)+
                SQR(x(I1,I2,I3,1)-z1);
      }
      else
      {
        x0=.3, x1=.8, rad=.1;
	radius0(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x0)/topHatRadiusX;
	radius1(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x1)/topHatRadiusY;
      }
      
      where( radius0(I1,I2,I3) < SQR(rad) || radius1(I1,I2,I3)< SQR(rad) )
	err(I1,I2,I3,0)=1.;
    }
    else if( type==diagonal )
    {
      // set error to 1 where the distance to the line y=x is less than width
      real width=.05;
      RealArray dist(I1,I2,I3);

      if( gc.numberOfDimensions()>1 )
        dist(I1,I2,I3) = fabs( x(I1,I2,I3,0)-x(I1,I2,I3,1) );
      else
	dist(I1,I2,I3)= fabs( x(I1,I2,I3,0)-.5);
      

      where( dist(I1,I2,I3) < width )
	err(I1,I2,I3,0)=1.;

    }
    else if( type==cross )
    {
      // set error to 1 where the distance to the line y=x is less than width
      real width=.025; // .05;
      RealArray dist(I1,I2,I3);

      if( gc.numberOfDimensions()>1 )
	dist(I1,I2,I3)= min( fabs( x(I1,I2,I3,0)-x(I1,I2,I3,1) ), fabs( x(I1,I2,I3,0)+x(I1,I2,I3,1)-1. ) );
      else
	dist(I1,I2,I3)= fabs( x(I1,I2,I3,0)-.5);

      where( dist(I1,I2,I3) < width )
	err(I1,I2,I3,0)=1.;

    }
    else if( type==plus )
    {
      // set error to 1 where the distance to the line y=x is less than width
      const real xPlus=.5, yPlus=.5;
      // const real xPlus=-2., yPlus=.5;
     

      real width=.05001;
      RealArray dist(I1,I2,I3);

      if( gc.numberOfDimensions()>1 )
	dist(I1,I2,I3)= min( fabs( x(I1,I2,I3,0)-xPlus ), fabs( x(I1,I2,I3,1)-yPlus ) );
      else
	dist(I1,I2,I3)= fabs( x(I1,I2,I3,0)-xPlus);

      where( dist(I1,I2,I3) < width )
	err(I1,I2,I3,0)=1.;

    }
    else if( type==hollowCircle )
    {
      RealArray radius0(I1,I2,I3);

      real x0=.5, y0=.5, z0=0.;
      real rad=.35, width=.075;
      width=.005;
    
//       real x0=1., y0=.5, z0=0.;
//       real rad=.2, width=.05;
    
      if( gc.numberOfDimensions()==2 )
	radius0(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-y0);
      else if( gc.numberOfDimensions()==3 )
	radius0(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-y0)+SQR(x(I1,I2,I3,1)-z0);
      else
	radius0(I1,I2,I3)=SQR(x(I1,I2,I3,0)-x0);

      where( radius0(I1,I2,I3) > SQR(rad-width) && radius0(I1,I2,I3)<SQR(rad+width) )
	err(I1,I2,I3,0)=1.;
    }
  }
  return 0;
}

//\begin{>>ErrorEstimatorInclude.tex}{\subsection{computeFunction}}
int ErrorEstimator::
computeFunction( realGridCollectionFunction & u, FunctionEnum type, real t /* = 0. */ )
//=========================================================================================
// /Description:
//   Evaluate a function that can be used to generate nice AMR grids.
//  
// \begin{description}
//   \item[topHat] : define a top-hat function
//     \[ 
//           u = 
//     \]
// \end{description}
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  GridCollection & gc = *u.getGridCollection();
  
  Index I1,I2,I3;
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( type==topHat )
    {
      gc[grid].update( MappedGrid::THEvertex );
      getIndex(gc[grid].dimension(),I1,I2,I3);

      #ifdef USE_PPP
       realSerialArray x; getLocalArrayWithGhostBoundaries(gc[grid].vertex(),x);
       realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
       const int includeGhost=1;
       bool ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost);
       if( !ok ) continue;
      #else
       const realSerialArray & x = gc[grid].vertex();
       realSerialArray & uLocal = u[grid];
      #endif

      realSerialArray radius0(I1,I2,I3);
	
      real a = topHatVelocity[0];
      real b = gc.numberOfDimensions()>1 ? topHatVelocity[1] : 0.;
      real c = gc.numberOfDimensions()>2 ? topHatVelocity[2] : 0.;

      real x0=topHatCentre[0]+a*t, y0=topHatCentre[1]+b*t, z0= topHatCentre[2]+c*t;
      // real x0=0.0+a*t, y0=0.0+b*t, z0=0.;
      // real rad=topHatRadius;
    
      if( gc.numberOfDimensions()==2 )
	radius0=SQR(x(I1,I2,I3,0)-x0)/SQR(topHatRadiusX)+SQR(x(I1,I2,I3,1)-y0)/SQR(topHatRadiusY);
      else if( gc.numberOfDimensions()==1 )
	radius0=SQR(x(I1,I2,I3,0)-x0);
      else 
	radius0=SQR(x(I1,I2,I3,0)-x0)/SQR(topHatRadiusX)+SQR(x(I1,I2,I3,1)-y0)/SQR(topHatRadiusY)+
	  SQR(x(I1,I2,I3,2)-z0)/SQR(topHatRadiusZ);

      uLocal=0.;
      where( radius0 < 1. )
	uLocal(I1,I2,I3)=1.;

    }
  }
  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{}} 
int ErrorEstimator::
smooth( realGridCollectionFunction & error )
// ===========================================================================
// /Description:
//   Apply one smoothing step to the error.
//\end{ErrorEstimatorInclude.tex} 
// ==========================================================================
{

  GridCollection & gc = *error.getGridCollection();

  //  ---Use a Jacobi smoother, under-relaxed
  real omega0=.9;
  real omo=1.-omega0, ob4=omega0/4., ob6=omega0/6., ob2=omega0/2.;
  

  Index I1,I2,I3,all;
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>=maximumNumberOfRefinementLevels-1 )
      continue;  // no need to compute errors on the finest level

    MappedGrid & mg = gc[grid];
    const intArray & mask = mg.mask();
    realArray & e = error[grid];
      
    // getIndex( mg.gridIndexRange(),I1,I2,I3);
    getIndex( mg.dimension(),I1,I2,I3,-1);
      
    #ifdef USE_PPP
      realSerialArray eLocal;  getLocalArrayWithGhostBoundaries(e,eLocal);
      bool ok = ParallelUtility::getLocalArrayBounds(e,eLocal,I1,I2,I3); 
      if( !ok ) continue;
      intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
    #else
      const realSerialArray & eLocal  = e;
      const intSerialArray & maskLocal=mask; 
    #endif


    real *ep = eLocal.Array_Descriptor.Array_View_Pointer3;
    const int eDim0=eLocal.getRawDataSize(0);
    const int eDim1=eLocal.getRawDataSize(1);
    const int eDim2=eLocal.getRawDataSize(2);
#undef E
#define E(i0,i1,i2,i3) ep[i0+eDim0*(i1+eDim1*(i2+eDim2*(i3)))]

    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    bool useOpt=true;
    if( useOpt )
    {
      realSerialArray e2(I1,I2,I3,1);  // temp space so we perform a Jacobi iteration.

      real *e2p = e2.Array_Descriptor.Array_View_Pointer3;
      const int e2Dim0=e2.getRawDataSize(0);
      const int e2Dim1=e2.getRawDataSize(1);
      const int e2Dim2=e2.getRawDataSize(2);
      #undef E2
      #define E2(i0,i1,i2,i3) e2p[i0+e2Dim0*(i1+e2Dim1*(i2+e2Dim2*(i3)))]     

      int i1,i2,i3;
      if( gc.numberOfDimensions()==2 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)>0 )
	  {
	    E2(i1,i2,i3,0)=omo*E(i1,i2,i3,0)+ob4*(E(i1+1,i2,i3,0)+E(i1-1,i2,i3,0)+E(i1,i2+1,i3,0)+E(i1,i2-1,i3,0));
	  }
	  else
	  {
	    E2(i1,i2,i3,0)=0.;
	  }
	}
      }
      else if( gc.numberOfDimensions()==3 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)>0 )
	  {
	    E2(i1,i2,i3,0)=omo*E(i1,i2,i3,0)
	      +ob6*( E(i1+1,i2,i3,0)+E(i1-1,i2,i3,0)
		     +E(i1,i2+1,i3,0)+E(i1,i2-1,i3,0)
		     +E(i1,i2,i3+1,0)+E(i1,i2,i3-1,0) );
	  }
	  else
	  {
	    E2(i1,i2,i3,0)=0.;
	  }		    
	}
      }
      else // 1D
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  if( MASK(i1,i2,i3)>0 )
	  {
	    E2(i1,i2,i3,0)=omo*E(i1,i2,i3,0)+ob2*( E(i1+1,i2,i3,0)+E(i1-1,i2,i3,0) );
	  }
	  else
	  {
	    E2(i1,i2,i3,0)=0.;
	  }		    
	}
      }
      // copy result back to error array
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        E(i1,i2,i3,0)=E2(i1,i2,i3,0);
      }
    }
    else
    {
      if( gc.numberOfDimensions()==2 )
      {
	// where( cg[grid).mask()(I1,I2,I3) >0 ) ** add this ?
	eLocal(I1,I2,I3,0)=omo*eLocal(I1,I2,I3,0)
	  +ob4*( eLocal(I1+1,I2,I3,0)+eLocal(I1-1,I2,I3,0)
		 +eLocal(I1,I2+1,I3,0)+eLocal(I1,I2-1,I3,0));
      }
      else if( gc.numberOfDimensions()==3 )
      {
	eLocal(I1,I2,I3,0)=omo*eLocal(I1,I2,I3,0)
	  +ob6*( eLocal(I1+1,I2,I3,0)+eLocal(I1-1,I2,I3,0)
		 +eLocal(I1,I2+1,I3,0)+eLocal(I1,I2-1,I3,0)
		 +eLocal(I1,I2,I3+1,0)+eLocal(I1,I2,I3-1,0) );
      }
      else // 1D
      {
	eLocal(I1,I2,I3,0)=omo*eLocal(I1,I2,I3,0)+ob2*( eLocal(I1+1,I2,I3,0)+eLocal(I1-1,I2,I3,0) );
      }
    }
  }

#undef E
#undef E2
#undef MASK

  // *wdh* 060530
  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {  
    if( gc.refinementLevelNumber(grid)>=maximumNumberOfRefinementLevels-1 )
      continue;  // no need to compute errors on the finest level

    error[grid].updateGhostBoundaries();
  }
  
  return 0;

}

//\begin{>>ErrorEstimatorInclude.tex}{\subsection{interpolateAndApplyBoundaryConditions}}
int ErrorEstimator::
interpolateAndApplyBoundaryConditions( realCompositeGridFunction & error, CompositeGridOperators & op )
//=========================================================================================
// /Access:  protected.
// /Description:
//   Apply boundary conditions to the error. This will diffuse the error across interpolation boundaries.
// We do NOT transfer the error from fine patches to underlying coarse patches. 
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  if( true )
  {
    GridCollection & gc = *error.getGridCollection();
    CompositeGrid &cg = *error.getCompositeGrid();

    BoundaryConditionParameters bcParams;
    bcParams.orderOfExtrapolation=2;

    int grid;
    for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
      if( gc.refinementLevelNumber(grid)<maximumNumberOfRefinementLevels-1 )
      {
	op[grid].applyBoundaryCondition(error[grid],0,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams);

	if ( op.getOrderOfAccuracy()==4 || (cg[grid].discretizationWidth(0)==3 && op.getOrderOfAccuracy()==2) ) // kkc avoid for width==5, order=2
	  op[grid].applyBoundaryCondition(error[grid],0,BCTypes::extrapolateInterpolationNeighbours,
					  BCTypes::allBoundaries, 0.,0.,bcParams);
      }
    }
    

    if( debug & 2 )
    {
      error.display("interpAndApplyBC: after extrap, before interp",debugFile,"%8.1e");
    }
    
    // we do not want to interpolate hidden refinement points, since we do not want
    // to over-write the error on the coarse grid
    assert( error.getInterpolant()!=NULL );
    Interpolant & interpolant = *error.getInterpolant();
    int oldValue;
    oldValue=interpolant.getInterpolationOption(Interpolant::interpolateHiddenRefinementPoints);
    interpolant.setInterpolationOption(Interpolant::interpolateHiddenRefinementPoints,false);
    
    // *** do not apply on the finest level ***
    int oldMax=interpolant.getMaximumRefinementLevelToInterpolate();
    interpolant.setMaximumRefinementLevelToInterpolate(maximumNumberOfRefinementLevels-2);
    error.setOperators(op);

    error.interpolate(); 

    interpolant.setMaximumRefinementLevelToInterpolate(oldMax);

    interpolant.setInterpolationOption(Interpolant::interpolateHiddenRefinementPoints,oldValue);

    if( debug & 2 )
      error.display("interpAndApplyBC: after interp, before finishBC",debugFile,"%8.1e");

    for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
      if( gc.refinementLevelNumber(grid)<maximumNumberOfRefinementLevels-1 )
      {
        // periodic update
        op[grid].finishBoundaryConditions(error[grid],Overture::defaultBoundaryConditionParameters(),0);
      }
    }
    
  }
  else
  {
    interpolateRefinements->interpolateCoarseFromFine( error,InterpolateRefinements::allLevels,0 ); 

    op.applyBoundaryCondition(error,0,BCTypes::extrapolate,BCTypes::allBoundaries,0.);
    op.applyBoundaryCondition(error,0,BCTypes::extrapolateInterpolationNeighbours);

  // interpolateRefinements->interpolateCoarseFromFine( error ); 

    interpolateRefinements->interpolateRefinementBoundaries( error,InterpolateRefinements::allLevels,0 ); 
    // Here we transfer the fine grid error to the coarse grid. This prevents
    // the error from "leaking out" farther than it should.
    // interpolateRefinements->interpolateCoarseFromFine( error ); 

    error.interpolate(0); 

  
    op.finishBoundaryConditions(error,Overture::defaultBoundaryConditionParameters(),0);  // periodic update
  }
  
  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{computeErrorFunction}} 
int ErrorEstimator::
computeErrorFunction( realCompositeGridFunction & u, 
                      realCompositeGridFunction & error )
//=========================================================================================
// /Description:
//     Given a solution u defined at all discretization and interpolation points, define
// an error function that can be given to the adaptive mesh Regrid function.
//
// /u (input) : compute the error from this function.
// /error (output) : an error function that can be used to perform an AMR regrid.
//
// /Notes:
//    
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  computeErrorFunction((realGridCollectionFunction&)u,(realGridCollectionFunction&)error);

  return 0;
  
}

//\begin{>>ErrorEstimatorInclude.tex}{\subsection{computeErrorFunction}} 
int ErrorEstimator::
computeErrorFunction( realGridCollectionFunction & u, 
                      realGridCollectionFunction & error )
// =======================================================================================
// /Description:
//    Estimate errors based on un-divided differences.
//
// \begin{equation}
//      {c_2\over s_m}  \| \Delta_{+}\Delta_{-} u_{i,j} \| + {c_1\over s_m} \| \Delta_0 u_{i,j} \| 
// \end{equation}
//
//\end{ErrorEstimatorInclude.tex} 
// =======================================================================================
{
  GridCollection & gc = *error.getGridCollection();
  
  // *wdh* error = 0.;
  Index I1,I2,I3,all;
  
  Range C(u.getComponentBase(0),u.getComponentBound(0));
  RealArray meanForAllGrids(C), maxForAllGrids(C);
  meanForAllGrids=0.;
  maxForAllGrids=0.;
  
  bool useScaleFactor = scaleFactor.getBase(0)<=u.getComponentBase(0) && 
                        scaleFactor.getBound(0)>=u.getComponentBound(0);

  if( !useScaleFactor && scaleFactor.getBound(0)>scaleFactor.getBase(0) )
  {
    printF("ErrorEstimator::computeErrorFunction:WARNING: Scale factors are only given for components %i to %i\n"
           "                but there are %i to %i components in the solution. I will ignore the scale factors.\n",
	   scaleFactor.getBase(0),scaleFactor.getBound(0),u.getComponentBase(0),u.getComponentBound(0));
  }

  int totalNumberOfPoints=0;
  int c,grid;
  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>=maximumNumberOfRefinementLevels-1 )
    {
//      printF("ErrorEstimator::computeErrorFunction: skip grid %i level=%i\n",grid,gc.refinementLevelNumber(grid));
      continue;  // no need to compute errors on the finest level
    }
//     printF("ErrorEstimator::computeErrorFunction: grid %i level=%i maxLevels=%i\n",grid,
// 	   gc.refinementLevelNumber(grid),maximumNumberOfRefinementLevels);
    
    realArray & err = error[grid];
    // err(all,all,all,0)=0.;
    assign(err,0.,all,all,all,all);
    

    MappedGrid & mg = gc[grid];
    getIndex( mg.gridIndexRange(),I1,I2,I3);

    realArray & v = u[grid];

    // choose a relative error 
    const intArray & mask = mg.mask();
    #ifdef USE_PPP
      bool ok=true;
      realSerialArray vLocal;   getLocalArrayWithGhostBoundaries(v,vLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3); 
      if( !ok ) continue;
    #else
      realSerialArray & vLocal=v;
      const intSerialArray & maskLocal=mask;
    #endif

    intSerialArray m(I1,I2,I3);
    m(I1,I2,I3) = maskLocal(I1,I2,I3) > 0;  

    int numberOfPoints=sum(m(I1,I2,I3));
    totalNumberOfPoints+=numberOfPoints;
    
    if( !useScaleFactor )
    {
      where( m(I1,I2,I3) )
      {
	for( c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
	{
	  meanForAllGrids(c)+=sum(fabs(vLocal(I1,I2,I3,c)));
	  maxForAllGrids(c)=max(maxForAllGrids(c),max(fabs(vLocal(I1,I2,I3,c))));
	}
      }
    
    }
  }
  totalNumberOfPoints=ParallelUtility::getSum(totalNumberOfPoints); // sum over all processors

  if( !useScaleFactor )
  {
    for( c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
    {
      meanForAllGrids(c)=ParallelUtility::getSum(meanForAllGrids(c));
      maxForAllGrids(c) =ParallelUtility::getMaxValue(maxForAllGrids(c));
    }
  }
    

  totalNumberOfPoints=max(1,totalNumberOfPoints);
  meanForAllGrids/=totalNumberOfPoints;
  if( !useScaleFactor )
  {
    for( c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
    {
      if( meanForAllGrids(c)==0. )
      {
	meanForAllGrids(c)=1.;
      }
      if( debug & 2 )
	printF("ErrorEstimator: mean for component %i is %8.2e, max=%8.2e\n",c,meanForAllGrids(c),maxForAllGrids(c));
    }
  }
  
  bool computeAverageAndMaximumError= debug>3;
  
  real errMax=-REAL_MAX*.5;
  real errAve=0.;
  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>=maximumNumberOfRefinementLevels-1 )
      continue;  // no need to compute errors on the finest level

    MappedGrid & mg = gc[grid];
    // getIndex( mg.gridIndexRange(),I1,I2,I3);
    getIndex( mg.dimension(),I1,I2,I3,-1);  // do as many points as possible
    const intArray & mask = mg.mask();
    realArray & v = u[grid];
    realArray & err = error[grid];

    #ifdef USE_PPP
      realSerialArray errLocal; getLocalArrayWithGhostBoundaries(err,errLocal);
      realSerialArray vLocal;   getLocalArrayWithGhostBoundaries(v,vLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);

      bool ok = ParallelUtility::getLocalArrayBounds(err,errLocal,I1,I2,I3); 
      if( !ok ) continue;
    #else
      const realSerialArray & errLocal  = err;
      const realSerialArray & vLocal  =  v;
      const intSerialArray & maskLocal  =  mask;
    #endif

    real *ep = errLocal.Array_Descriptor.Array_View_Pointer3;
    const int eDim0=errLocal.getRawDataSize(0);
    const int eDim1=errLocal.getRawDataSize(1);
    const int eDim2=errLocal.getRawDataSize(2);
    #undef E
    #define E(i0,i1,i2,i3) ep[i0+eDim0*(i1+eDim1*(i2+eDim2*(i3)))]

    real *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
    const int vDim0=vLocal.getRawDataSize(0);
    const int vDim1=vLocal.getRawDataSize(1);
    const int vDim2=vLocal.getRawDataSize(2);
    #undef V
    #define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]

    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
    #define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]

    const int numberOfComponents=u.getComponentBound(0)-u.getComponentBase(0)+1;
    for( c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
    {
      real eps = useScaleFactor ? 0. : max(.01*meanForAllGrids(c),REAL_MIN*100.);
      // real uScale = max(mean(c,grid),eps);
      real uScale = useScaleFactor ? scaleFactor(c) : max(maxForAllGrids(c),eps);

      real c1 = weightFirstDifference/(uScale*numberOfComponents);
      real c2 = weightSecondDifference/(uScale*numberOfComponents);

      // printF(" c=%i uScale=%e eps=%e\n",c,uScale,eps);
      

      // ***** no need to compute parts if c1==0 or c2==0
      // allow scale factor <=0 -> don't used
      // no need to compute the error on the finest level


      bool useOpt=true;
      if( useOpt )
      {
        int i1,i2,i3;
	if( gc.numberOfDimensions()==2 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( MASK(i1,i2,i3)>0 )
	    {
	      E(i1,i2,i3,0) += (fabs(V(i1+1,i2  ,i3,c)-2.*V(i1,i2,i3,c)+V(i1-1,i2  ,i3,c))*c2+
				fabs(V(i1  ,i2+1,i3,c)-2.*V(i1,i2,i3,c)+V(i1  ,i2-1,i3,c))*c2+
				fabs(V(i1+1,i2  ,i3,c)-V(i1-1,i2  ,i3,c))*c1+
				fabs(V(i1  ,i2+1,i3,c)-V(i1  ,i2-1,i3,c))*c1	);
	    }
	  }
	}
	else if( gc.numberOfDimensions()==3 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( MASK(i1,i2,i3)>0 )
	    {
	      E(i1,i2,i3,0) += (fabs(V(i1+1,i2  ,i3  ,c)-2.*V(i1,i2,i3,c)+V(i1-1,i2  ,i3  ,c))*c2+
				fabs(V(i1  ,i2+1,i3  ,c)-2.*V(i1,i2,i3,c)+V(i1  ,i2-1,i3  ,c))*c2+
				fabs(V(i1  ,i2  ,i3+1,c)-2.*V(i1,i2,i3,c)+V(i1  ,i2  ,i3-1,c))*c2+
				fabs(V(i1+1,i2  ,i3  ,c)-V(i1-1,i2  ,i3  ,c))*c1+
				fabs(V(i1  ,i2+1,i3  ,c)-V(i1  ,i2-1,i3  ,c))*c1+
				fabs(V(i1  ,i2  ,i3+1,c)-V(i1  ,i2  ,i3-1,c))*c1	);
 
	    }
	  }
	}
	else
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( MASK(i1,i2,i3)>0 )
	    {
	      E(i1,i2,i3,0) += (fabs(V(i1+1,i2  ,i3,c)-2.*V(i1,i2,i3,c)+V(i1-1,i2  ,i3,c))*c2+
				fabs(V(i1+1,i2  ,i3,c)-V(i1-1,i2  ,i3,c))*c1 );
	    }
	  }
	}
#undef E
#undef V
#undef MASK	
      }
      else
      {
	if( gc.numberOfDimensions()==2 )
	{
	  errLocal(I1,I2,I3,0) += (fabs(vLocal(I1+1,I2  ,I3,c)-2.*vLocal(I1,I2,I3,c)+vLocal(I1-1,I2  ,I3,c))*c2+
				   fabs(vLocal(I1  ,I2+1,I3,c)-2.*vLocal(I1,I2,I3,c)+vLocal(I1  ,I2-1,I3,c))*c2+
				   fabs(vLocal(I1+1,I2  ,I3,c)-vLocal(I1-1,I2  ,I3,c))*c1+
				   fabs(vLocal(I1  ,I2+1,I3,c)-vLocal(I1  ,I2-1,I3,c))*c1	);
	}
	else if( gc.numberOfDimensions()==3 )
	{
	  errLocal(I1,I2,I3,0) += (fabs(vLocal(I1+1,I2  ,I3  ,c)-2.*vLocal(I1,I2,I3,c)+vLocal(I1-1,I2  ,I3  ,c))*c2+
				   fabs(vLocal(I1  ,I2+1,I3  ,c)-2.*vLocal(I1,I2,I3,c)+vLocal(I1  ,I2-1,I3  ,c))*c2+
				   fabs(vLocal(I1  ,I2  ,I3+1,c)-2.*vLocal(I1,I2,I3,c)+vLocal(I1  ,I2  ,I3-1,c))*c2+
				   fabs(vLocal(I1+1,I2  ,I3  ,c)-vLocal(I1-1,I2  ,I3  ,c))*c1+
				   fabs(vLocal(I1  ,I2+1,I3  ,c)-vLocal(I1  ,I2-1,I3  ,c))*c1+
				   fabs(vLocal(I1  ,I2  ,I3+1,c)-vLocal(I1  ,I2  ,I3-1,c))*c1	);
 
	}
	else
	{
	  errLocal(I1,I2,I3,0) += (fabs(vLocal(I1+1,I2  ,I3,c)-2.*vLocal(I1,I2,I3,c)+vLocal(I1-1,I2  ,I3,c))*c2+
				   fabs(vLocal(I1+1,I2  ,I3,c)-vLocal(I1-1,I2  ,I3,c))*c1 );
	}
      }
      
    } // end for c
    
    if( computeAverageAndMaximumError )
    {
      where( maskLocal(I1,I2,I3) > 0 )
      {
	errMax = max(errMax, max(errLocal(I1,I2,I3,0)) );
	errAve+=sum(errLocal(I1,I2,I3,0));
      }
    }

    err.updateGhostBoundaries();
    
  }  // end for grid

  if( computeAverageAndMaximumError )
  {
    #ifdef USE_PPP
     errMax=ParallelUtility::getMaxValue(errMax);
     errAve=ParallelUtility::getSum(errMax);

    #endif
    errAve/=totalNumberOfPoints;
  
    printF("ErrorEstimator: maximum error=%9.2e, average error=%9.2e \n",errMax,errAve);
  }
  
  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{computeErrorFunction}} 
int ErrorEstimator::
computeAndSmoothErrorFunction( realCompositeGridFunction & u, 
			       realCompositeGridFunction & error,
			       int numberOfSmooths /* = defaultNumberOfSmooths */  )
//=========================================================================================
// /Description:
//     Given a solution u defined at all discretization and interpolation points, define
// an error function that can be given to the adaptive mesh Regrid function.
//
// /u (input) : compute the error from this function.
// /error (output) : an error function that can be used to perform an AMR regrid.
// /numberOfSmooths (input) : number of times to smooth the error. By default
//  use the default number of smoothing steps (usually 1) which can be set with the
// {\tt setDefaultNumberOfSmooths} member function.
//
// /Notes:
//    
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  computeErrorFunction(u,error);

  CompositeGridOperators *op;
  if( u.getOperators()!=NULL )
    op= u.getOperators();
  else if( error.getOperators()!=NULL )
    op= error.getOperators();
  else
  {
    printF("ErrorEstimator::computeAndSmoothErrorFunction:ERROR: no operators defined for u or error\n");
    Overture::abort("error");
  }
  smoothErrorFunction(error,numberOfSmooths,op);
  
  return 0;
  
}

//\begin{>>ErrorEstimatorInclude.tex}{\subsection{smoothErrorFunction}} 
int ErrorEstimator::
smoothErrorFunction( realCompositeGridFunction & error,
                      int numberOfSmooths /* = defaultNumberOfSmooths */,
		     CompositeGridOperators *op /* = NULL */ )
//=========================================================================================
// /Description:
//     Smooth an error function and interpolate across overlapping grid boundaries.
//
// /error (intput) : an error function that can be used to perform an AMR regrid.
// /numberOfSmooths (input) : number of times to smooth the error. By default
//  use the default number of smoothing steps (usually 1) which can be set with the
// {\tt setDefaultNumberOfSmooths} member function.
// /op (input) : optionally supply operators to use.
// /Notes:
//    
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  if( false )
  {
    debug=3; // **************************
    openDebugFile();
  }

  if( numberOfSmooths==defaultNumberOfSmooths )
    numberOfSmooths = defaultNumberOfSmoothingSteps;

  if( op==NULL )
    op= error.getOperators();

  if( op==NULL )
  {
    printF("ErrorEstimator::smoothErrorFunction:ERROR: no operators defined for u or error\n");
    Overture::abort("error");
  }

  if( debug & 2 )
    error.display("ErrorEstimator::START: error before interpolateAndApplyBoundaryConditions",debugFile,"%4.1f");
  
  interpolateAndApplyBoundaryConditions( error,*op );
  
  if( debug & 2 )
    error.display("ErrorEstimator::error after interpolateAndApplyBoundaryConditions",debugFile,"%4.1f");

  for( int it=0; it<numberOfSmooths; it++ )
  {
    smooth( error );
    if( debug & 2 )
      error.display(sPrintF("ErrorEstimator::error after smooth, it=%i",it),debugFile,"%4.1f");

    interpolateAndApplyBoundaryConditions( error,*op );

    if( debug & 2 )
      error.display(sPrintF("ErrorEstimator::error after smooth and interpAndApplyBC, it=%i",it),debugFile,"%4.1f");
  }

  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{plotErrorPoints}} 
int ErrorEstimator::
plotErrorPoints(  realGridCollectionFunction & error, 
                  real errorThreshhold,
                  PlotStuff & ps, PlotStuffParameters & psp )
//=========================================================================================
// /Description:
//    Plot those points where the error is greater than a threshold.
// /error (input):
// /errorThreshhold (input) :
//\end{ErrorEstimatorInclude.tex} 
//=========================================================================================
{
  GridCollection & gc = *error.getGridCollection();
  const int numberOfDimensions = gc.numberOfDimensions();


  Index I1,I2,I3;
  
  psp.set(GI_POINT_SIZE,(real)4.);

  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( gc.refinementLevelNumber(grid)>=maximumNumberOfRefinementLevels-1 )
      continue;  // no need to compute errors on the finest level

    MappedGrid & mg = gc[grid];
    mg.update(MappedGrid::THEmask);
    const intArray & mask = mg.mask();
    
    getIndex(mg.dimension(),I1,I2,I3);
    // intArray tag(I1,I2,I3);
    intArray tag; tag.partition(mg.getPartition()); tag.redim(I1,I2,I3);

    getIndex(extendedGridIndexRange(mg),I1,I2,I3);
    tag=0;
    where( error[grid](I1,I2,I3,0)>errorThreshhold )
    {
      tag(I1,I2,I3)=1;
    }
    where( mask(I1,I2,I3)==0 )
    {
      tag(I1,I2,I3)=0;
    }

    #ifdef USE_PPP
      const intSerialArray & tagLocalc = tag.getLocalArray();
      intSerialArray & tagLocal = (intSerialArray &)tagLocalc;
      const realSerialArray & vertex = mg.vertex().getLocalArray();
    #else
      intSerialArray & tagLocal = tag;
      const realArray & vertex = mg.vertex();
    #endif

    intSerialArray ia;
    ia = tagLocal.indexMap();   // **** fix this for P++ ****

    int i3=I3.getBase();
    if( ia.getLength(0)>0 )
    {
      
      Range R=ia.getLength(0);

      // if( min(ia(R,axis2))<I2.getBase() )
#ifdef USE_OLD_APP      
      if( numberOfDimensions>1 )
        ia(R,axis2)+=I2.getBase(); // **** A++ bug ****
#endif

      realSerialArray x(R,numberOfDimensions);
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	if( numberOfDimensions==2 )
	  x(R,axis)=vertex(ia(R,0),ia(R,1),i3,axis);
	else if( numberOfDimensions==3 )
	  x(R,axis)=vertex(ia(R,0),ia(R,1),ia(R,2),axis);
        else
	  x(R,axis)=vertex(ia(R,0),I2.getBase(),i3,axis);
      }
      // display(x,"error points");
      
      #ifdef USE_PPP
        // copy points to an array, x0,  on processor pDestination==0 

        const int num = R.length();
        const int maxNum = ParallelUtility::getMaxValue(num); // could use ,pDestination);
        const int totalNumber = ParallelUtility::getSum(num);

        const int pDestination=0;
	realArray x0; Partitioning_Type partition; 
        partition.SpecifyProcessorRange(Range(pDestination,pDestination)); x0.partition(partition);
	x0.redim(totalNumber,numberOfDimensions);

        const realSerialArray & x0Local = x0.getLocalArray();
        // send x(R,.) to processor 0
	const int myid = Communication_Manager::My_Process_Number;
	const int numberOfProcessors=Communication_Manager::Number_Of_Processors;

        MPI_Request sendRequest; 
	MPI_Request *receiveRequest=NULL;
	MPI_Status *receiveStatus=NULL;
	real **dbuffr; // destination buffers

        // post receives first
        int p;
	if( myid==pDestination )
	{
	  receiveRequest= new MPI_Request[numberOfProcessors];
	  receiveStatus = new MPI_Status[numberOfProcessors];
          dbuffr = new real *[numberOfProcessors];
	  for( p=0; p<numberOfProcessors; p++ )
	  {
            const int buffSize=maxNum*numberOfDimensions;
            dbuffr[p] = new real [buffSize];
            // look for values from processor p:
	    MPI_Irecv(dbuffr[p],buffSize,MPI_Real,p,MPI_ANY_TAG,MPI_COMM_WORLD,&receiveRequest[p] );
	  }
	}
        // send
        
        real *xp = x.getDataPointer(); // send the data in this array to pDestination

        // Send data to processor=pDestination
        int tag=num*numberOfDimensions;
	MPI_Isend(xp,max(1,num*numberOfDimensions),MPI_Real,pDestination,tag,MPI_COMM_WORLD,&sendRequest );

        if( myid==pDestination )
	{
          // wait for messages from all processors
          MPI_Waitall( numberOfProcessors, receiveRequest, receiveStatus );  // wait to receive all messages

	  if( true )
	  {
            FILE *debugFile=stdout;
	    for( p=0; p<numberOfProcessors; p++ )
	    {
	      int numild=receiveStatus[p].MPI_TAG; // total received

	      fprintf(debugFile,"plotErrorPoints: processor %i: received msg from processor %i, tag=%i p=%i \n",
		      myid,receiveStatus[p].MPI_SOURCE,receiveStatus[p].MPI_TAG,p);
	    }
	  }

          // Copy the results into x0Local
          int k=0;
	  for( p=0; p<numberOfProcessors; p++ )
	  {
	    const int numValues=receiveStatus[p].MPI_TAG; // total number of values received from process p
            const int sp =receiveStatus[p].MPI_SOURCE;  // source processor
	    assert( sp==p );  // is this necessary to have?

            const int nump=numValues/numberOfDimensions;
	    // fill in data received from processor p
	    real* xd = dbuffr[p];
            #define XD(i,m) xd[i+nump*(m)]

	    for( int j=0; j<nump; j++ )
	    {
	      for( int axis=0; axis<numberOfDimensions; axis++ )
	      {
		x0Local(k,axis) = XD(j,axis);
	      }
	      k++;
	    }
            #undef XD
	  }
          assert( k == totalNumber );

	  for( p=0; p<numberOfProcessors; p++ )
	  {
	    delete [] dbuffr[p];
	  }
	  delete [] receiveRequest;
	  delete [] receiveStatus;
	  delete [] dbuffr;
	}

	Communication_Manager::Sync();

        ps.plotPoints(x0,psp);

	Communication_Manager::Sync();
	
      #else
        ps.plotPoints(x,psp);
      #endif
      
    }
  }
  return 0;
}

//\begin{>>ErrorEstimator.tex}{\subsection{displayParameters}} 
int ErrorEstimator:: 
displayParameters(FILE *file /* = stdout */ ) const
// ===========================================================================
// /Description:
//   Display parameters.
// 
// /file (input) : display to this file.
//
//\end{ErrorEstimatorInclude.tex} 
// ==========================================================================
{
  fprintf(file,
	  "Regrid:: parameters:\n"
          "  default number of smooths=%i\n"
          "  weight for the first difference= %f\n"
          "  weight for the second difference= %f\n"
          "  maximum number of refinement levels=%i\n",
	  defaultNumberOfSmoothingSteps,weightFirstDifference,weightSecondDifference,
                 maximumNumberOfRefinementLevels);
  
  for( int c=scaleFactor.getBase(0); c<=scaleFactor.getBound(0); c++ )
    fprintf(file,"  scale factor for component %i = %e\n",c,scaleFactor(c));

  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{get}}
int ErrorEstimator::
get( const GenericDataBase & dir, const aString & name)
// ===========================================================================
// /Description:
//   Get from a data base file.
//\end{ErrorEstimatorInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"ErrorEstimator");

  aString className;
  subDir.get( className,"className" ); 
  if( className != "ErrorEstimator" )
  {
    cout << "ErrorEstimator::get ERROR in className!" << endl;
  }

  subDir.get(weightFirstDifference,"weightFirstDifference");
  subDir.get(weightSecondDifference,"weightSecondDifference");
  
  subDir.get(topHatCentre,"topHatCentre",3);
  subDir.get(topHatVelocity,"topHatVelocity",3);
  subDir.get(topHatRadius,"topHatRadius"); 
  subDir.get(topHatRadiusX,"topHatRadiusX"); 
  subDir.get(topHatRadiusY,"topHatRadiusY");
  subDir.get(topHatRadiusZ,"topHatRadiusZ");
  subDir.get(scaleFactor,"scaleFactor");
  
  // InterpolateRefinements *interpolateRefinements;

  subDir.get(defaultNumberOfSmoothingSteps,"defaultNumberOfSmoothingSteps");

  delete &subDir;
  return 0;
}

//\begin{>>ErrorEstimatorInclude.tex}{\subsection{put}}
int ErrorEstimator::
put( GenericDataBase & dir, const aString & name) const
// ===========================================================================
// /Description:
//   Put to a data base file.
//\end{ErrorEstimatorInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"ErrorEstimator");                   // create a sub-directory 

  subDir.put( "ErrorEstimator","className" );

  subDir.put(weightFirstDifference,"weightFirstDifference");
  subDir.put(weightSecondDifference,"weightSecondDifference");
  
  subDir.put(topHatCentre,"topHatCentre",3);
  subDir.put(topHatVelocity,"topHatVelocity",3);
  subDir.put(topHatRadius,"topHatRadius"); 
  subDir.put(topHatRadiusX,"topHatRadiusX"); 
  subDir.put(topHatRadiusY,"topHatRadiusY");
  subDir.put(topHatRadiusZ,"topHatRadiusZ");
  subDir.put(scaleFactor,"scaleFactor");
  
  // InterpolateRefinements *interpolateRefinements;

  subDir.put(defaultNumberOfSmoothingSteps,"defaultNumberOfSmoothingSteps");

  delete &subDir;
  return 0;
}


//\begin{>>ErrorEstimatorInclude.tex}{\subsection{update}} 
int ErrorEstimator::
update( GenericGraphicsInterface & gi )
// ===========================================================================
// /Description:
//   Change error estimator parameters interactively.
// 
// /gi (input) : use this graphics interface.
//
//\end{ErrorEstimatorInclude.tex} 
// ==========================================================================
{
  aString menu[]=
  {
    "!Error estimator",
    "display parameters",
    "default number of smooths",
    "maximum number of refinement levels",
    "weight for first difference",
    "weight for second difference",
    "set scale factors",
    "exit",
    ""
  };

//\begin{>>ErrorEstimatorInclude.tex}{}
//\no function header:
//
// \begin{description} \index{error estimator!parameters}
//  \item[weight for first difference] :
//  \item[weight for second difference] : 
//  \item[set scale factors] : Scale each component of the solution by this factor.
// \end{description}
//\end{ErrorEstimatorInclude.tex}

  aString answer,answer2;
  char buff[100];

  gi.appendToTheDefaultPrompt("ErrorEstimator>");  
  for(;;)
  {
    gi.getMenuItem(menu,answer,"choose a menu item");
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="display parameters" )
    {
      displayParameters();
    }
    else if( answer=="default number of smooths" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the default number of smooths (current=%i)",
               defaultNumberOfSmoothingSteps));
      sScanF(answer,"%i",&defaultNumberOfSmoothingSteps);
      printF("set defaultNumberOfSmoothingSteps=%i\n",defaultNumberOfSmoothingSteps);

    }
    else if( answer=="maximum number of refinement levels" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the maximum number of refinement levels (current=%i)",
               maximumNumberOfRefinementLevels));
      sScanF(answer,"%i",&maximumNumberOfRefinementLevels);
      printF("set maximumNumberOfRefinementLevels=%i\n",maximumNumberOfRefinementLevels);

    }
    else if( answer=="weight for first difference" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the weight for the first difference (current=%8.2e)",
               weightFirstDifference));
      sScanF(answer,"%e",&weightFirstDifference);
      printF("set weightFirstDifference=%e\n",weightFirstDifference);

    }
    else if( answer=="weight for second difference" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the weight for the second difference (current=%8.2e)",
               weightSecondDifference));
      sScanF(answer,"%e",&weightSecondDifference);
      printF("set weightSecondDifference=%e\n",weightSecondDifference);

    }
    else if( answer=="set scale factors" )
    {
      gi.getValues("enter scale factors",scaleFactor);
      for( int c=scaleFactor.getBase(0); c<=scaleFactor.getBound(0); c++ )
	printF("scale factor for component %i = %e\n",c,scaleFactor(c));
    }
    else
    {
      cout << "Unknown response: [" << answer2 << "]\n";
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();



  return 0;
}
