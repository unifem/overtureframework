#include "EllipticGridGenerator.h"
#include "display.h"
#include "LineMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"

#include "Overture.h"
#include "MappedGridOperators.h"

int EllipticGridGenerator:: 
startingGrid(const realArray & u0, 
             const realArray & r0 /* = Overture::nullRealDistributedArray() */,
             const IntegerArray & gridIndexBounds /* =Overture::nullIntArray() */ )
// ===========================================================================================
/// \details 
///      Supply the starting grid (with optional ghost points).
/// 
/// \param u0 (input) : initial grid.
/// \param r0 (input) : optionally specify the unit square coordinates of u0, i.e. userMap->map(r0,u0).
///                If unspecified r0 is assumed to be a uniform grid.
/// \param gridIndexBounds (input) : marks the positions in the array u0 of the boundaries. Use this
///    array to supply ghost point values.
// ===========================================================================================
{
  // Get initial condition for u[0]
  Index J1,J2,J3;
  Range all;
  for( int level=0; level<numberOfLevels; level++)
  {
    getIndex(mg[level].gridIndexRange(),J1,J2,J3);
    if( level==0 )
      u[0](J1,J2,J3,Rx)=u0(J1,J2,J3,Rx);
    else
    {
      const bool isAGridFunction=TRUE;
      fineToCoarse(level-1,u[level-1],u[level],isAGridFunction );
    }
    u[level].applyBoundaryCondition(Rx,BCTypes::extrapolate,BCTypes::allBoundaries,0.);  // **** do better at ghost
    periodicUpdate(u[level]);

    // rBoundary = unit square coordinates for boundary values, used for slip-orthogonal
    // xBoundary = x boundary values corresponding to rBoundary
    rBoundary[level].updateToMatchGrid(mg[level],all,all,all,Rr);
    if( r0.getLength(0)==0 )
      rBoundary[level] = mg[level].vertex();  // holds unit square coords for boundary points.
    else
    {
      if( level==0 )
        rBoundary[level](J1,J2,J3,Rr) =r0(J1,J2,J3,Rx);
      else
	fineToCoarse(level-1,rBoundary[level-1],rBoundary[level]);    
    }

    xBoundary[level].updateToMatchGrid(mg[level],all,all,all,Rx);
    xBoundary[level](J1,J2,J3,Rx) = u[level](J1,J2,J3,Rx);

    if( level==0 )
    { // scaling for residual goes like dx^3  ( x.r*x.r*x.rr + ...
      residualNormalizationFactor=max(u[0](J1,J2,J3,Rx))-min(u[0](J1,J2,J3,Rx));
      residualNormalizationFactor=pow(residualNormalizationFactor,3.);
      printf("residualNormalizationFactor = (xMax-xMin)^3 = %9.2e\n",residualNormalizationFactor);
    }
    
  }

  return 0;
}



void EllipticGridGenerator::
getResidual(realArray & resid1, 
            const int & level )
// ==========================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///      Compute the residual. Do not recompute the control functions. 
// ==========================================================================================
{
  // J1,J2,J3 : interior plus periodic boundaries
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(gridIndex(Range(0,1),Range(0,2),level),J1,J2,J3);

  realArray coeff(J1,J2,J3,numberOfCoefficients);
  const bool computeCoefficients=TRUE,includeRightHandSide=TRUE,computeControlFunctions=FALSE;
  getResidual(resid1,level,Jv,coeff,computeCoefficients,includeRightHandSide,computeControlFunctions);
}

void EllipticGridGenerator::
getResidual(realArray & resid1, 
            const int & level,
            Index Jv[3],
            realArray & coeff,
            const bool & computeCoefficients /* =TRUE */,
            const bool & includeRightHandSide /* = TRUE */,
            const bool & computeControlFunctions /* = TRUE */,
            const SmoothingTypes & lineSmoothType /* =jacobiSmooth */ )
// ==============================================================================
/// \param Access: {\bf Protected}.
/// \details 
///      Computes the residual: resid1 = rhs - L(u[level])
/// 
/// \param Jv (input) : compute the residual at these points.
/// \param coeff (input/output) : optionally supply the coefficients for the operator. On output
///     this array will always hold values.
/// \param computeCoeffients (input) : if true, compute the coefficients. Otherwise the coeff array
///     will hold computed coefficients on output.
/// \param includeRightHandSide (input) if true add the right hand side to the residual operation.
///      if they have been computed. 
/// \param computeControlFunctions (input) : if true re-compute the control functions.
/// \param lineSmoothType (input) : if equal to line1Smooth or line2Smooth or line3Smooth
///    compute residual without terms that would be included in a line smooth in the given direction.
///    For all other smoothingtypes we compute the full residual.
// ==============================================================================
{
  int j;
  Index &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  realArray & u1 = u[level];

//  if( computeControlFunctions )
//    getControlFunctions(level);


  resid1=0.0;
  switch ( domainDimension )
  {
  case 1:
  {
    const realArray & urr = u[level].r1r1();
    const realArray & ur = u[level].r1();
    resid1(J1,J2,J3,0)=rhs[level]-( urr(J1,J2,J3,0)+ur(J1,J2,J3,0)*source[level](J1,J2,J3,0) );
    break;
  }
  case 2:
  {
    const realArray & urr = u[level].r1r1();
    const realArray & urs = u[level].r1r2();
    const realArray & uss = u[level].r2r2();
    const realArray & ur = u[level].r1();
    const realArray & us = u[level].r2();


    if( computeCoefficients )
    {
      // coeff.redim(J1,J2,J3,3);
      getCoefficients(coeff,J1,J2,J3,ur,us);
    }
    
    if( useBlockTridiag )
	printf("******getResidual: BlockTridiag not implemented yet. Using non-block line smooth *****\n");

    // printf("getResidual : compute urr...\n");
      
    for( j=0; j<rangeDimension; j++ )
    {
      switch (lineSmoothType)
      {
      case line1Smooth:
	resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,1)*( uss(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis2,level)))+
                                  source[level](J1,J2,J3,1)*us(J1,J2,J3,j) )+
			     coeff(J1,J2,J3,2)*( urs(J1,J2,J3,j) ) );
	break;
      case line2Smooth:
	resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,0)*( urr(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis1,level)))+
                             source[level](J1,J2,J3,0)*ur(J1,J2,J3,j) )+
			     coeff(J1,J2,J3,2)*( urs(J1,J2,J3,j) ) );
	break;
      default:	
	resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,0)*( urr(J1,J2,J3,j)+source[level](J1,J2,J3,0)*ur(J1,J2,J3,j) )+
			     coeff(J1,J2,J3,1)*( uss(J1,J2,J3,j)+source[level](J1,J2,J3,1)*us(J1,J2,J3,j) )+
			     coeff(J1,J2,J3,2)*( urs(J1,J2,J3,j) ) );
	break;
      }
      
    }
    if( includeRightHandSide )
      resid1(J1,J2,J3,Rx)+=rhs[level](J1,J2,J3,Rx);
    if( rangeDimension==3 )
    {
      // surface grid
      // remove the component of the residual in the direction normal to the surface
      //     r <- r - (r.n) n /(n.n)
      Index I1=Range(J1.getBase(),J1.getBound());  // do this in case of stride in J
      Index I2=Range(J2.getBase(),J2.getBound());
      Index I3=Range(J3.getBase(),J3.getBound());
      
      realArray normal(I1,I2,I3,Rx);
      normal(J1,J2,J3,0)=ur(J1,J2,J3,1)*us(J1,J2,J3,2)-ur(J1,J2,J3,2)*us(J1,J2,J3,1);
      normal(J1,J2,J3,1)=ur(J1,J2,J3,2)*us(J1,J2,J3,0)-ur(J1,J2,J3,0)*us(J1,J2,J3,2);
      normal(J1,J2,J3,2)=ur(J1,J2,J3,0)*us(J1,J2,J3,1)-ur(J1,J2,J3,1)*us(J1,J2,J3,0);
      
      realArray norm= evaluate( SQR(normal(J1,J2,J3,0)) + SQR(normal(J1,J2,J3,1)) + SQR(normal(J1,J2,J3,2)) );
      const realArray & normalDotResidual = evaluate( normal(J1,J2,J3,0)*resid1(J1,J2,J3,0)+
						normal(J1,J2,J3,1)*resid1(J1,J2,J3,1)+
						normal(J1,J2,J3,2)*resid1(J1,J2,J3,2) );
      where( norm>0. )
	norm=normalDotResidual/norm;
      for( j=0; j<rangeDimension; j++ )
	resid1(J1,J2,J3,j)-=norm*normal(J1,J2,J3,j);
    }

    if( debug & 4 )
    {
      display(coeff,"getResidual: coeff",debugFile);
      display(urr,"getResidual: urr",debugFile);
      display(uss,"getResidual: uss",debugFile);
      display(urs,"getResidual: urs",debugFile);
      display(resid1,"getResidual: resid1",debugFile);
    }
    

/* -----
    if ((boundaryCondition(0,0)==2)||(boundaryCondition(0,1)==2)||(boundaryCondition(1,0)==2)||	(boundaryCondition(1,1)==2))
    {
      get2DBoundaryResidual(level,resid1);
    }
----- */    
    break;
  }

  case 3:
  {
    const realArray & urr = u[level].r1r1();
    const realArray & urs = u[level].r1r2();
    const realArray & urt = u[level].r1r3();
    const realArray & uss = u[level].r2r2();
    const realArray & ust = u[level].r2r3();
    const realArray & utt = u[level].r3r3();
    const realArray & ur = u[level].r1();
    const realArray & us = u[level].r2();
    const realArray & ut = u[level].r3();

    if( computeCoefficients )
    {
      // coeff.redim(J1,J2,J3,numberOfCoefficients);
      getCoefficients(coeff,J1,J2,J3,ur,us,ut);
    }
    
    if( useBlockTridiag!= 1)
    {
      // printf("getResidual in 3D: ...\n");
      
      for( j=0;j<rangeDimension; j++)
      {
	switch (lineSmoothType)
	{
	case line1Smooth:
	  resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,1)*( uss(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis2,level)))+
                                                   source[level](J1,J2,J3,1)*us(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,2)*( utt(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis3,level)))+
                                                   source[level](J1,J2,J3,2)*ut(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,3)*urs(J1,J2,J3,j) +
			       coeff(J1,J2,J3,4)*urt(J1,J2,J3,j) +
			       coeff(J1,J2,J3,5)*ust(J1,J2,J3,j) );
	  break;
	case line2Smooth:
	  resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,0)*( urr(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis1,level)))+
						   source[level](J1,J2,J3,0)*ur(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,2)*( utt(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis3,level)))+
						   source[level](J1,J2,J3,2)*ut(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,3)*urs(J1,J2,J3,j) +
			       coeff(J1,J2,J3,4)*urt(J1,J2,J3,j) +
			       coeff(J1,J2,J3,5)*ust(J1,J2,J3,j) );
	  break;
	case line3Smooth:
	  resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,0)*( urr(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis1,level)))+
                                                   source[level](J1,J2,J3,0)*ur(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,1)*( uss(J1,J2,J3,j)+u1(J1,J2,J3,j)*(2./SQR(dx(axis2,level)))+
                                                   source[level](J1,J2,J3,1)*us(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,3)*urs(J1,J2,J3,j) +
			       coeff(J1,J2,J3,4)*urt(J1,J2,J3,j) +
			       coeff(J1,J2,J3,5)*ust(J1,J2,J3,j) );
	  break;
	default:	
	  resid1(J1,J2,J3,j)=-(coeff(J1,J2,J3,0)*( urr(J1,J2,J3,j)+source[level](J1,J2,J3,0)*ur(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,1)*( uss(J1,J2,J3,j)+source[level](J1,J2,J3,1)*us(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,2)*( utt(J1,J2,J3,j)+source[level](J1,J2,J3,2)*ut(J1,J2,J3,j) )+
			       coeff(J1,J2,J3,3)*urs(J1,J2,J3,j) +
			       coeff(J1,J2,J3,4)*urt(J1,J2,J3,j) +
			       coeff(J1,J2,J3,5)*ust(J1,J2,J3,j) );
	}
      }
      if( includeRightHandSide )
	resid1(J1,J2,J3,Rx)+=rhs[level](J1,J2,J3,Rx);

      
      if( debug & 4 )
      {
	display(coeff,"getResidual: coeff",debugFile);
	display(urr,"getResidual: urr",debugFile);
	display(uss,"getResidual: uss",debugFile);
	display(utt,"getResidual: utt",debugFile);
	display(urs,"getResidual: urs",debugFile);
	display(urt,"getResidual: urt",debugFile);
	display(ust,"getResidual: ust",debugFile);
	display(resid1,"getResidual: resid1",debugFile);
      }
	
    }
    else
    {
      {throw "error";}
    }
    break;
  }
  
  default:
    printf("getResidual:invalid value for rangeDimension=%i\n",rangeDimension);
    {throw "error";}
  }
}



void EllipticGridGenerator::
updateRightHandSideWithFASCorrection(int i )
// ==========================================================================================
/// \param Access: {\bf Protected}.
///    Update the rhs by adding on the FAS corection
///               rhs[level+1] += L_{level+1}(I(u[level])
///   where I is the interpolant from level to level+1.
// ==========================================================================================
{
  // get the residual with rhs=0
  // rhs[level]+=residual
  const int level = i;
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  getIndex(mg[level].gridIndexRange(),J1,J2,J3);
  realArray coeff(J1,J2,J3,numberOfCoefficients), resid1(J1,J2,J3,Rx);

  getIndex(gridIndex(Range(0,1),Range(0,2),level),J1,J2,J3);

  // printf("updaterhs...\n");

  const bool computeCoeffients=TRUE, includeRightHandSide=FALSE,computeControlFunctions=FALSE;;
  getResidual( resid1,level,Jv,coeff,computeCoeffients,includeRightHandSide,computeControlFunctions);
  rhs[i](J1,J2,J3,Rx)-=resid1(J1,J2,J3,Rx);

  rhs[i].periodicUpdate();

}

int EllipticGridGenerator::
getCoefficients(realArray & coeff, 
		const Index & J1, 
		const Index & J2, 
		const Index & J3,
                const realArray & ur, 
                const realArray & us,
                const realArray & ut /* = Overture::nullRealDistributedArray() */ )
// =================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///     Compute the coefficients of the elliptic system.
/// 
/// \param coeff (output) : compute the 3 (2D) or 6 (3D) coefficients
/// \param ur,us,ut (input) : first derivatives of the solution.
/// 
///   In 2D :
///      coeff_0 * u_{rr} + coeff_1 * u_{ss} + coeff_2 * u_{rs} 
///   In 3D :
///      coeff_0 * u_{rr} + coeff_1 * u_{ss} + coeff_2 * u_{tt} + coeff_3 u_{rs} + coeff_4 u_{rt} + coeff_5 u_{st}
/// \param coeff (output) : coefficients.
// ================================================================================
{
  if( domainDimension==1 )
    return 0;
  
  if( domainDimension==2 && rangeDimension==2 )
  {
    const realArray & xr  = ur(J1,J2,J3,0);
    const realArray & xs  = us(J1,J2,J3,0);
    const realArray & yr  = ur(J1,J2,J3,1);
    const realArray & ys  = us(J1,J2,J3,1);
    coeff(J1,J2,J3,0)=xs*xs+ys*ys;
    coeff(J1,J2,J3,1)=xr*xr+yr*yr;
    coeff(J1,J2,J3,2)=-2.*(xr*xs+yr*ys);
  }
  else if( domainDimension==2 && rangeDimension==3 )
  {
    const realArray & xr  = ur(J1,J2,J3,0);
    const realArray & xs  = us(J1,J2,J3,0);
    const realArray & yr  = ur(J1,J2,J3,1);
    const realArray & ys  = us(J1,J2,J3,1);
    const realArray & zr  = ur(J1,J2,J3,2);
    const realArray & zs  = us(J1,J2,J3,2);
    coeff(J1,J2,J3,0)=xs*xs+ys*ys+zs*zs;
    coeff(J1,J2,J3,1)=xr*xr+yr*yr+zr*zr;
    coeff(J1,J2,J3,2)=-2.*(xr*xs+yr*ys+zr*zs);
  }
  else if( domainDimension==3 )
  {
    const realArray & xr  = ur(J1,J2,J3,0);
    const realArray & xs  = us(J1,J2,J3,0);
    const realArray & xt  = ut(J1,J2,J3,0);
    const realArray & yr  = ur(J1,J2,J3,1);
    const realArray & ys  = us(J1,J2,J3,1);
    const realArray & yt  = ut(J1,J2,J3,1);
    const realArray & zr  = ur(J1,J2,J3,2);
    const realArray & zs  = us(J1,J2,J3,2);
    const realArray & zt  = ut(J1,J2,J3,2);

    const realArray & a11 = evaluate(xr*xr+yr*yr+zr*zr);
    const realArray & a22 = evaluate(xs*xs+ys*ys+zs*zs);
    const realArray & a33 = evaluate(xt*xt+yt*yt+zt*zt);
    const realArray & a12 = evaluate(xr*xs+yr*ys+zr*zs);
    const realArray & a13 = evaluate(xr*xt+yr*yt+zr*zt);
    const realArray & a23 = evaluate(xs*xt+ys*yt+zs*zt);
    
    coeff(J1,J2,J3,0)=a22*a33-a23*a23;
    coeff(J1,J2,J3,1)=a33*a11-a13*a13;
    coeff(J1,J2,J3,2)=a11*a22-a12*a12;
    coeff(J1,J2,J3,3)=2.*(a13*a23-a12*a33);
    coeff(J1,J2,J3,4)=2.*(a23*a12-a13*a22);
    coeff(J1,J2,J3,5)=2.*(a13*a12-a23*a11);
    
  }
  else
  {
    {throw "error";}
  }
  
  
  return 0;
}

int EllipticGridGenerator::
estimateUnderRelaxationCoefficients()
// ====================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    
///    Estimate the smoother relaxtion coefficients front the size of the control terms.
/// 
// ===============================================================================
{
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  
  real pMax[3]={0.,0.,0.};

  for( int l=0; l<numberOfLevels; l++ )
  {
    getIndex(mg[l].gridIndexRange(),I1,I2,I3);

    for( int axis=0; axis<domainDimension; axis++ )
       pMax[axis] = max(fabs(source[l](I1,I2,I3,axis)));
       

    if( domainDimension==2 )
    {
      omegaMax(0,l)=.5/( pMax[0]*dx(0,l)+pMax[1]*dx(1,l) );
    }
    else if( domainDimension==3 )
    {
      omegaMax(0,l)=.5/( pMax[0]*dx(0,l)+pMax[1]*dx(1,l)+pMax[2]*dx(2,l) );
    }
    else
    {
      omegaMax(0,l)=.5/( pMax[0]*dx(0,l) );
    }
    printf("estimateUnderRelaxationCoefficients: level=%i pMax=(%6.2e,%6.2e,%6.2e) omega=%6.2e\n",
	   l,pMax[0],pMax[1],pMax[2],omegaMax(0,l));
  }

  return 0;
}



int EllipticGridGenerator::
getControlFunctions(const int & level )
// ===============================================================================
/// \param Access: {\bf Protected}.
/// \details 
///     Compute the control function source terms, $P_n$  in the elliptic equations.
///  
///  /[
///     \sum C_{mn} \xv_{r^m r^n} + \sum_n C_{nn} P_n \xv_{r^n} 
///  /]
// ===============================================================================
{
  if( level>0 )
    return 0;

  controlFunctions=applyBoundarySourceControlFunction || numberOfPointsOfAttraction>0 || numberOfLinesOfAttraction>0;

  if( controlFunctionComputed && !applyBoundarySourceControlFunction ) 
//  if( controlFunctionComputed )
    return 0;

  controlFunctionComputed=TRUE;

  bool controlFunctionChanged=FALSE;
  
  if( normalCurvatureWeight>0. )
  {
    controlFunctionChanged=TRUE;
    defineSurfaceControlFunction();  // watch out this defines boundary values *******
  }
  

  // combined BC: compute P on the boundary and get interior values by interpolation.
  // Here we assume that the ghost line values have been set properly.

  //  P_n = -[ {\xv_n\cdot \xv_{00} \over \| \xv_0 \|^2 } + {\xv_n\cdot \xv_{11} \over \| \xv_1 \|^2 }

  realArray & u1 = u[level];
  int axis;


  if( applyBoundarySourceControlFunction ||  numberOfPointsOfAttraction>0 || numberOfLinesOfAttraction>0 )
  {
    controlFunctionChanged=TRUE;
    

    realArray & p = source[level];
    const realArray & r = mg[level].vertex();   // unit square grid
    int j, dir;

    p=0.0; // is this correct ??

    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    getIndex(mg[level].gridIndexRange(),J1,J2,J3);


    if( applyBoundarySourceControlFunction ) 
    {
      
      if( FALSE )
      {
        defineBoundaryControlFunction();
      }
      else
      {
        defineBoundaryControlFunction();


	int applyBoundarySource = 0;
	for( axis=0; axis<domainDimension; axis++ )
	  applyBoundarySource+=( boundaryCondition(0,axis)==noSlipOrthogonalAndSpecifiedSpacing || 
				 boundaryCondition(1,axis)==noSlipOrthogonalAndSpecifiedSpacing );

	Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
	Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
	for( axis=0; axis<domainDimension; axis++ )
	{
	  for( int side=Start; side<=End; side++ )
	  {
	    if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
	    {
	      getBoundaryIndex(mg[level].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	
	      const realArray & ur =  u[level].r1(Ib1,Ib2,Ib3,Rx)(Ib1,Ib2,Ib3,Rx);  // evaluate on the boundary only
	      const realArray & us =  u[level].r2(Ib1,Ib2,Ib3,Rx)(Ib1,Ib2,Ib3,Rx);
	      const realArray & urr = u[level].r1r1(Ib1,Ib2,Ib3,Rx)(Ib1,Ib2,Ib3,Rx);
	      const realArray & uss = u[level].r2r2(Ib1,Ib2,Ib3,Rx)(Ib1,Ib2,Ib3,Rx);
  
/* ----------
              // Scale the normal derivatives to be what we know they should be
              real boundaryLayerExponent=3.;   // *********************
              real boundaryLayerFactor=1.+boundaryLayerExponent*dx(axis,level);   // *********************
              // if axis==0 then |ur| = dx/dr and |urr|= (dx/dr)*(1+beta*dr -1.)=dx*beta

              if( axis==axis2 )
	      {
                realArray & usNormInverse = evaluate(1./(SQRT( SQR(us(Ib1,Ib2,Ib3,0)) + SQR(us(Ib1,Ib2,Ib3,1)) )));
                realArray & ussNormInverse = evaluate(1./(SQRT( SQR(uss(Ib1,Ib2,Ib3,0)) + SQR(uss(Ib1,Ib2,Ib3,1))) ));
		for( int dir=0; dir<rangeDimension; dir++ )
		{
                  // us(Ib1,Ib2,Ib3,dir)*=(boundarySpacing(side,axis)/mg[0].gridSpacing(axis))*usNormInverse;
		  // uss(Ib1,Ib2,Ib3,dir)*=(boundarySpacing(side,axis)/mg[0].gridSpacing(axis)*
                  //                       boundaryLayerExponent)*ussNormInverse;
		  uss(Ib1,Ib2,Ib3,dir)*=boundaryLayerExponent*ussNormInverse/usNormInverse;
		}
	      }
	      else
	      {
		throw "error";
	      }
------ */

	      const realArray & urNormSquaredInverse = evaluate(1./( SQR(ur(Ib1,Ib2,Ib3,0))+SQR(ur(Ib1,Ib2,Ib3,1)) ));
	      const realArray & usNormSquaredInverse = evaluate(1./( SQR(us(Ib1,Ib2,Ib3,0))+SQR(us(Ib1,Ib2,Ib3,1)) ));
	  
              assert( rangeDimension==2 );
	      
	      
	      printf("getControlFunctions: boundarySpacing(side,axis)=%e\n",boundarySpacing(side,axis));
	      printf("getControlFunctions: max(uss(Ib1,Ib2,Ib3,0)) = %e \n",max(fabs(uss(Ib1,Ib2,Ib3,0))));
	      printf("getControlFunctions: max(uss(Ib1,Ib2,Ib3,1)) = %e \n",max(fabs(uss(Ib1,Ib2,Ib3,1))));
	    
	      const realArray & urDotUrr = ur(Ib1,Ib2,Ib3,0)*urr(Ib1,Ib2,Ib3,0)+ur(Ib1,Ib2,Ib3,1)*urr(Ib1,Ib2,Ib3,1);
	      const realArray & urDotUss = ur(Ib1,Ib2,Ib3,0)*uss(Ib1,Ib2,Ib3,0)+ur(Ib1,Ib2,Ib3,1)*uss(Ib1,Ib2,Ib3,1);
	      p(Ib1,Ib2,Ib3,0) -= ( urDotUrr*urNormSquaredInverse + urDotUss*usNormSquaredInverse );

	      const realArray & usDotUrr = us(Ib1,Ib2,Ib3,0)*urr(Ib1,Ib2,Ib3,0)+us(Ib1,Ib2,Ib3,1)*urr(Ib1,Ib2,Ib3,1);
	      const realArray & usDotUss = us(Ib1,Ib2,Ib3,0)*uss(Ib1,Ib2,Ib3,0)+us(Ib1,Ib2,Ib3,1)*uss(Ib1,Ib2,Ib3,1);
	      p(Ib1,Ib2,Ib3,1) -= ( usDotUrr*urNormSquaredInverse + usDotUss*usNormSquaredInverse );


	      if( debug & 4 )
		display(p(Ib1,Ib2,Ib3,axis),"getControlFunctions: p(Ib1,Ib2,Ib3,axis)",debugFile);

	      getGhostIndex(mg[level].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,-1); // first line in
	      realArray uDiff(Ib1,Ib2,Ib3,Rx);
	      uDiff = u1(Ig1,Ig2,Ig3,Rx)-u1(Ib1,Ib2,Ib3,Rx);
	      if( rangeDimension==2 )
		uDiff(Ib1,Ib2,Ib3,0)=SQRT( SQR(uDiff(Ib1,Ib2,Ib3,0))+SQR(uDiff(Ib1,Ib2,Ib3,1)) );
	      else
		uDiff(Ib1,Ib2,Ib3,0)=SQRT( SQR(uDiff(Ib1,Ib2,Ib3,0))+
					   SQR(uDiff(Ib1,Ib2,Ib3,1))+
					   SQR(uDiff(Ib1,Ib2,Ib3,2)) );
	      real minSpacing=min(uDiff(Ib1,Ib2,Ib3,0));
	      real maxSpacing=max(uDiff(Ib1,Ib2,Ib3,0));

	      real averageSpacing=sum(uDiff(Ib1,Ib2,Ib3,0));
	      int num=Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
	      averageSpacing/=max(1,num);

	      printf("getControlFunctions: (side,axis)=(%i,%i) requested spacing=%6.2e, actual spacing: average=%6.2e,"
		     " min=%6.2e, max=%6.2e\n",side,axis,boundarySpacing(side,axis),averageSpacing,
		     minSpacing,maxSpacing);

	      // Fill in the interior values using transfinite interpolation.	    
	      K1=Ib1; K2=Ib2; K3=Ib3;

	      const realArray r1 = side==0 ? evaluate(1.-r(J1,J2,J3,axis)) : r(J1,J2,J3,axis);

	      for( int i=Jv[axis].getBase()+1; i<=Jv[axis].getBound()-1; i++ ) // assumes end points not changed
	      {
		Kv[axis]=i;
         	for( dir=0; dir<domainDimension; dir++ )
		  p(K1,K2,K3,dir)+=r1(K1,K2,K3)*p(Ib1,Ib2,Ib3,dir);  // incremental sum of TFI for interior pts.
	      }
	    }
	  }
	}
	// corner correction for the TFI   **** finish for 3D ***
	if( applyBoundarySource>1 )
	{
	  for( dir=0; dir<domainDimension; dir++ )
	  {
	    p(J1,J2,J3,dir)-=
	      (1.-r(J1,J2,J3,0))*( 
		(1.-r(J1,J2,J3,1))*p(J1.getBase() ,J2.getBase() ,J3.getBase(),dir) + 
		(   r(J1,J2,J3,1))*p(J1.getBase() ,J2.getBound(),J3.getBase(),dir)  )+
	      (   r(J1,J2,J3,0))*( 
		(1.-r(J1,J2,J3,1))*p(J1.getBound(),J2.getBase() ,J3.getBase(),dir) + 
		(   r(J1,J2,J3,1))*p(J1.getBound(),J2.getBound(),J3.getBase(),dir)  ) ;
	  }
	}
      
      }
      
    }


    if( numberOfPointsOfAttraction>0 || numberOfLinesOfAttraction>0 )
    {

      realArray rMinusR0(J1,J2,J3,domainDimension), norm(J1,J2,J3);
      int np,nperiod;
    
      for( axis=0; axis<domainDimension; axis++ )
      {
	// Attraction toward a point R_n^j 
	//  P_{n} = - a sign( r_n - R_j ) exp( - c | \rv-\Rv_j | )
    
	for( j=0; j<numberOfPointsOfAttraction; j++ )
	{
	  // In the periodic case we add on the periodic images of the attraction
	  nperiod=( boundaryCondition(0,axis)==-1 || boundaryCondition(1,axis)==-1 ) ? numberOfPeriods : 0;

	  for( np=-nperiod; np<=nperiod; np++)
	  {
	    norm=0.;
	    for( dir=0; dir<domainDimension; dir++ )
	    {
	      rMinusR0(J1,J2,J3,dir) = r(J1,J2,J3,dir) - (pointAttractionParameters(2+dir,j)-np);
	      norm += SQR(rMinusR0(J1,J2,J3,dir));
	    }
	    norm=SQRT(norm);
	    p(J1,J2,J3,axis) -=pointAttractionParameters(0,j)*signOf(rMinusR0(J1,J2,J3,axis))*
	      exp(-pointAttractionParameters(1,j)*norm);
	  }
	}
    
	// Attraction toward a line 
	//  P_{n} = - a sign( r_n - R_j ) exp( - c | r_n - R_j | )
	for( j=0; j<numberOfLinesOfAttraction; j++ )
	{
	  if( lineAttractionDirection(j)==axis )
	  {
	    // In the periodic case we add on the periodic images of the attraction
	    nperiod=( boundaryCondition(0,axis)==-1 || boundaryCondition(1,axis)==-1 ) ? numberOfPeriods : 0;

	    for( np=-nperiod; np<=nperiod; np++)
	    {
	      rMinusR0(J1,J2,J3,axis) = r(J1,J2,J3,axis) - (lineAttractionParameters(2,j)-np);
	      norm = fabs(rMinusR0(J1,J2,J3,axis));
	      p(J1,J2,J3,axis) -=lineAttractionParameters(0,j)*signOf(rMinusR0(J1,J2,J3,axis))*
		exp(-lineAttractionParameters(1,j)*norm);
	    }
	  }
	}
      }
    }

    if( debug & 4 )
      display(p,"getControlFunctions: source",debugFile);
  }

  if( controlFunctionChanged )
  {
    // relaxation parameters depend on the size of the source terms

    source[0].periodicUpdate();
    // Average the source term to coarser levels
    for( int l=0; l<numberOfLevels-1; l++ )
      fineToCoarse( l,source[l], source[l+1] );
    
    estimateUnderRelaxationCoefficients();
  }
  
  return 0;
}

int EllipticGridGenerator::
defineBoundaryControlFunction()
// =================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///     Determine the control functions on the boundary that impose
///   orthogonality and a specified grid spacing.
/// 
///   We compute the the boundary normal vector $\nv(r,s)$ to the boundary $t=t_0$. 
///  Given a specified grid spacing $\Delta x$ we have 
///  \[
///      \xv(r,s,t_0 \pm \Deta t) = \xv(r,s,t_0)  \pm \Delta x \nv(r,s)
///  \]
///  and thus we have the approximations  
///  \begin{align*}
///      \xv_t(r,s,t_0) &= {\Delta x \over \Delta t} \nv(r,s) \\
///                     &= = \beta \nv(r,s)
///  \end{align*}
///  If we have a boundary layer stretching then we expect that the
///  grid spacing will increase at an exponential rate in the normal direction,
///  \[
///       \xv(r,s,t) \approx \xv(r,s,0) + C e^{\alpha t} \nv(r,s)
///  \]
///  for $t<< 1$.
///  Thus the second derivative $\xv_tt$ will satisfy
///  \[
///     \xv_tt(r,s,0) \approx \alpha \xv_t(r,s,0) = \alpha {\Delta x \over \Delta t} \nv(r,s)
///  \]
///  We can make a guess for $\xv_tt(r,s,0)$ given a guess for $\alpha$ 
/// 
/// 
///  Initial grid: If the user has requested a very small spacing near the boundary we can
///  can explicitly stretch the initial grid to approximately statisfy the grid spacing.
///  To do this we measure the actual grid spacing near each boundary that needs to be stretched.
///  We then determine stretching functions.
///  
///  
///  
///  
// =================================================================================
{
  if( rangeDimension<2 )
    return 0;
  
  const int level=0;
  RealMappedGridFunction & uu = u[level];
  realArray & u1 = uu;
  realArray & p = source[level];
  const realArray & r = mg[level].vertex();   // unit square grid

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
//  int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
//  is[0]=is[1]=is[2]=0;

  bool applyNoSlipOrthogonalAndSpecifiedSpacing = FALSE;
  int axis,dir;
  for( axis=0; axis<domainDimension; axis++ )
    applyNoSlipOrthogonalAndSpecifiedSpacing = applyNoSlipOrthogonalAndSpecifiedSpacing || 
                       boundaryCondition(0,axis)==noSlipOrthogonalAndSpecifiedSpacing || 
                       boundaryCondition(1,axis)==noSlipOrthogonalAndSpecifiedSpacing;

  if( applyNoSlipOrthogonalAndSpecifiedSpacing )
  {
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    getIndex(mg[level].gridIndexRange(),J1,J2,J3);

    // position the ghost point to be a specified distance in the normal direction
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
    int applyBoundarySource=0;
    for( axis=0; axis<rangeDimension; axis++ )
    {
      // Here are the tangential direction(s)
      const int axisp1 = (axis+1) % rangeDimension;
      const int axisp2 = (axis+2) % rangeDimension;
      for( int side=Start; side<=End; side++ )
      {
        if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
	{
          applyBoundarySource++;
	  getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3);    // boundary line
          getGhostIndex(mg[level].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,+1); // first ghost line.
	  
          // first compute the outward unit normal : grad_x t
	  realArray normal(I1,I2,I3,Rx);

          const int sgn = 2*side-1;  // multiply normal by this to be an outward normal

	  realArray ur(I1,I2,I3,Rx), urr(I1,I2,I3,Rx); // tangential derivative
	  if( axisp1==0 )
	  {
	    ur=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    urr=uu.r1r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  }
	  else if( axisp1==1 )
	  {
	    ur=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    urr=uu.r2r2(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  }
	  else
	  {
	    ur=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    urr=uu.r3r3(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  }
	  realArray norm(I1,I2,I3);
	  if( rangeDimension==2 )
	  {
	    normal(I1,I2,I3,axis1)=-ur(I1,I2,I3,axis2); // this will be normal in the direction of increasing r_axis
	    normal(I1,I2,I3,axis2)= ur(I1,I2,I3,axis1);
	    norm=SQRT(SQR(normal(I1,I2,I3,axis1))+SQR(normal(I1,I2,I3,axis2)));
	  }
	  else
	  {
	    realArray us(I1,I2,I3,Rx); 
	    if( axisp2==0 )
	      us=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    else if( axisp2==1 )
	      us=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	    else 
	      us=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
            // this will be normal in the direction of increasing r_axis
            normal(I1,I2,I3,axis1)=ur(I1,I2,I3,axis2)*us(I1,I2,I3,axis3)-ur(I1,I2,I3,axis3)*us(I1,I2,I3,axis2);
            normal(I1,I2,I3,axis2)=ur(I1,I2,I3,axis3)*us(I1,I2,I3,axis1)-ur(I1,I2,I3,axis1)*us(I1,I2,I3,axis3);
            normal(I1,I2,I3,axis3)=ur(I1,I2,I3,axis1)*us(I1,I2,I3,axis2)-ur(I1,I2,I3,axis2)*us(I1,I2,I3,axis1);
	    norm=SQRT(SQR(normal(I1,I2,I3,axis1))+SQR(normal(I1,I2,I3,axis2))+SQR(normal(I1,I2,I3,axis3)));
	  }
          where( norm>0. ) // **** what to do about norm==0 ?
	  {
	    norm=(-sgn*boundarySpacing(side,axis))/norm;
	  }
          printf("defineBoundaryControlFunction: setting ghost point on (side,axis)=(%i,%i) \n",side,axis);
	  
	  for( dir=0; dir<rangeDimension; dir++ )
	    normal(I1,I2,I3,dir)*=norm;            // now inward !!

          //  u(+1) = u(0) + normal
          //  u(-1) = u(0) - normal/alpha
          // 
          real boundaryLayerExponent=10.;   // *********************
          real boundaryLayerFactor=1.+boundaryLayerExponent*dx(axis,level);   // *********************

          // the spacing  increases by a factor (boundaryLayerFactor) close to the boundary, 
          // therefore decrease the size of the distance to the first ghost line.
//          u1(Ig1,Ig2,Ig3,Rx)=u1(I1,I2,I3,Rx)+(1./boundaryLayerFactor)*normal(I1,I2,I3,Rx);
          u1(Ig1,Ig2,Ig3,Rx)=u1(I1,I2,I3,Rx)-normal(I1,I2,I3,Rx);

          if( &u1 ) // if( TRUE )  // **********************************************************
            continue;

/* ----
          realArray ut(I1,I2,I3,Rx);
	  if( axis==0 )
	    ut=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
          else if( axis==1 )
	    ut=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx);
          else
	    ut=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx);

          real averageUt = sum( SQRT(SQR(ut(I1,I2,I3,0)) + SQR(ut(I1,I2,I3,1)) ));
	  averageUt/=I1.getLength()*I2.getLength()*I3.getLength();
	  ut/=averageUt;
---- */
          // un = derivative in the normal direction (ur or us)
          // unn = second derivative in the normal direction.
          const realArray & un = evaluate( normal(I1,I2,I3,Rx)*(1./dx(axis,level)) ); // **** fix ****
          const realArray & unn= evaluate( un*boundaryLayerExponent/boundaryLayerFactor );
          const real unNormSquaredInverse = SQR(dx(axis,level)/boundarySpacing(side,axis));

          if( domainDimension==2 )
	  {
	    const realArray & urNormSquaredInverse = evaluate(1./( SQR(ur(I1,I2,I3,0)) + SQR(ur(I1,I2,I3,1)) ));

            // printf("getControlFunctions: max(uss) = %e \n",max(fabs(uss)));
	    
	    const realArray & unDotUrr = un(I1,I2,I3,0)*urr(I1,I2,I3,0)+un(I1,I2,I3,1)*urr(I1,I2,I3,1);
	    const realArray & unDotUnn = un(I1,I2,I3,0)*unn(I1,I2,I3,0)+un(I1,I2,I3,1)*unn(I1,I2,I3,1);
	    p(I1,I2,I3,axis  ) -= ( unDotUrr*urNormSquaredInverse  + unDotUnn*unNormSquaredInverse );

	    const realArray & urDotUrr = ur(I1,I2,I3,0)*urr(I1,I2,I3,0)+ur(I1,I2,I3,1)*urr(I1,I2,I3,1);
	    const realArray & urDotUnn = ur(I1,I2,I3,0)*unn(I1,I2,I3,0)+ur(I1,I2,I3,1)*unn(I1,I2,I3,1);
	    p(I1,I2,I3,axisp1) -= ( urDotUrr*urNormSquaredInverse  + urDotUnn*unNormSquaredInverse );
	    
	  }
	  else
	  {
	    throw "error";
	  }
	  
	  if( debug & 4 )
	    display(p(I1,I2,I3,axis),"getControlFunctions: p(I1,I2,I3,axis)",debugFile);


	  // Fill in the interior values using transfinite interpolation.	    
	  K1=I1; K2=I2; K3=I3;

	  const realArray r1 = side==0 ? evaluate(1.-r(J1,J2,J3,axis)) : r(J1,J2,J3,axis);

	  for( int i=Jv[axis].getBase()+1; i<=Jv[axis].getBound()-1; i++ ) // assumes end points not changed
	  {
	    Kv[axis]=i;
  	    for( dir=0; dir<domainDimension; dir++ )
	      p(K1,K2,K3,dir)+=r1(K1,K2,K3)*p(I1,I2,I3,dir);  // incremental sum of TFI for interior pts.
	  }
	}
      }
    }
    periodicUpdate(uu);
    if( applyBoundarySource>1 )
    {
      for( dir=0; dir<domainDimension; dir++ )
      {
	p(J1,J2,J3,dir)-=
	  (1.-r(J1,J2,J3,0))*( 
	    (1.-r(J1,J2,J3,1))*p(J1.getBase() ,J2.getBase() ,J3.getBase(),dir) + 
	    (   r(J1,J2,J3,1))*p(J1.getBase() ,J2.getBound(),J3.getBase(),dir)  )+
	  (   r(J1,J2,J3,0))*( 
	    (1.-r(J1,J2,J3,1))*p(J1.getBound(),J2.getBase() ,J3.getBase(),dir) + 
	    (   r(J1,J2,J3,1))*p(J1.getBound(),J2.getBound(),J3.getBase(),dir)  ) ;
      }
    }
    const int numberOfSmooths=16;
    smoothJacobi( source[level],numberOfSmooths );
    
  }

  return 0;
}

int EllipticGridGenerator::
smoothJacobi( RealMappedGridFunction & v,  const int & numberOfSmooths /* = 4 */ )
//===========================================================================
/// \param Access: {\bf Protected}.
/// \brief  
///     Perform some smoothing steps.
//===========================================================================
{    
  const int level=0;
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(mg[level].gridIndexRange(),I1,I2,I3); 
  Range C=v.dimension(3);
    
  const real omegaRelax=.5;
  for( int smooth=0; smooth<numberOfSmooths; smooth++ )
  {
    v.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.); 
    v.periodicUpdate();

    if( domainDimension==1 )
      v(I1,I2,I3,C)=(1.-omegaRelax)*v(I1,I2,I3,C)+omegaRelax*.5*( v(I1+1,I2,I3,C)+v(I1-1,I2,I3,C) );
    else if( domainDimension==2 )
      v(I1,I2,I3,C)=(1.-omegaRelax)*v(I1,I2,I3,C)+omegaRelax*.25*( v(I1+1,I2,I3,C)+v(I1-1,I2,I3,C)+
								   v(I1,I2-1,I3,C)+v(I1,I2+1,I3,C) );
    else 
      v(I1,I2,I3,C)=(1.-omegaRelax)*v(I1,I2,I3,C)+(omegaRelax/6.)*( v(I1+1,I2,I3,C)+v(I1-1,I2,I3,C)+
								    v(I1,I2-1,I3,C)+v(I1,I2+1,I3,C)+
								    v(I1,I2,I3+1,C)+v(I1-1,I2,I3-1,C)  );
  }
  v.applyBoundaryCondition(0,BCTypes::extrapolate,BCTypes::allBoundaries,0.); 
  v.periodicUpdate();
  return 0;
}


int EllipticGridGenerator::
defineSurfaceControlFunction()
// =================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///     Determine control functions for re-distributing points on a surface.
///   Use htis control function to distribute points on a surface grid where
///   the normal curvature is large.
///  
///  
// =================================================================================
{
  if( domainDimension!=2 || rangeDimension!=3 )
    return 0;
  
  const int level=0;
  RealMappedGridFunction & uu = u[level];
  // realArray & u1 = uu;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(mg[level].gridIndexRange(),I1,I2,I3); 


  const realArray & urr = uu.r1r1();
  const realArray & urs = uu.r1r2();
  const realArray & uss = uu.r2r2();
  const realArray & ur  = uu.r1();
  const realArray & us  = uu.r2();

  const realArray & urNorm = evaluate( mg[level].gridSpacing(axis1)/
				 SQRT( SQR(ur(I1,I2,I3,0))+SQR(ur(I1,I2,I3,1))+SQR(ur(I1,I2,I3,2)) ) );

  const realArray & usNorm = evaluate( mg[level].gridSpacing(axis2)/
				 SQRT( SQR(us(I1,I2,I3,0))+SQR(us(I1,I2,I3,1))+SQR(us(I1,I2,I3,2)) ) );

  realArray normal(I1,I2,I3,Rx);
  normal(I1,I2,I3,0)=ur(I1,I2,I3,1)*us(I1,I2,I3,2)-ur(I1,I2,I3,2)*us(I1,I2,I3,1);
  normal(I1,I2,I3,1)=ur(I1,I2,I3,2)*us(I1,I2,I3,0)-ur(I1,I2,I3,0)*us(I1,I2,I3,2);
  normal(I1,I2,I3,2)=ur(I1,I2,I3,0)*us(I1,I2,I3,1)-ur(I1,I2,I3,1)*us(I1,I2,I3,0);
      
  const realArray & norm=evaluate( 1./ SQRT(SQR(normal(I1,I2,I3,0))+SQR(normal(I1,I2,I3,1))+SQR(normal(I1,I2,I3,2))) );
  

  if( pWeight==NULL )
  {
    pWeight= new RealMappedGridFunction;
  }
  RealMappedGridFunction & weight = *pWeight;
  
  weight.updateToMatchGrid(mg[level]); 
  weight(I1,I2,I3) = ( fabs((urr(I1,I2,I3,0)*normal(I1,I2,I3,0)+
			     urr(I1,I2,I3,1)*normal(I1,I2,I3,1)+
			     urr(I1,I2,I3,2)*normal(I1,I2,I3,2))*urNorm)+
		       fabs((uss(I1,I2,I3,0)*normal(I1,I2,I3,0)+
			     uss(I1,I2,I3,1)*normal(I1,I2,I3,1)+
			     uss(I1,I2,I3,2)*normal(I1,I2,I3,2))*usNorm) )*norm;

  int numberOfSmooths=4;
  smoothJacobi(weight,numberOfSmooths);

  const real minimumWeight = min(weight(I1,I2,I3));
  const real maximumWeight = max(weight(I1,I2,I3));
  printf("Surface control function: min(weight)=%e, max(weight)=%e \n",minimumWeight,maximumWeight);

  getIndex(mg[level].gridIndexRange(),I1,I2,I3,1);  // include a ghost point
  weight(I1,I2,I3)=1.+weight(I1,I2,I3)*( normalCurvatureWeight/maximumWeight);

  // *** smooth the weight function ***** use jacobiSmooth from HyperbolicMapping.

  getIndex(mg[level].gridIndexRange(),I1,I2,I3); 
  source[level](I1,I2,I3,0)=weight.r1()(I1,I2,I3)/weight(I1,I2,I3);
  source[level](I1,I2,I3,1)=weight.r2()(I1,I2,I3)/weight(I1,I2,I3);
  
  numberOfSmooths=20;
  smoothJacobi(source[level],numberOfSmooths);


  return 0;
}

int EllipticGridGenerator::
weightFunction( RealMappedGridFunction & weight_ )
// ====================================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Supply a weight function for adaptation.
// ====================================================================================================
{
  const int level=0;
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  assert( pWeight!=NULL );
  RealMappedGridFunction & weight = *pWeight;

  userWeightFunctionDefined=TRUE;
  if( FALSE )
    weight=weight_;
  else
  {
    weight.updateToMatchGrid(mg[level]);
    weight=0.;
    const real x0=.5, x1=.5, r0=.25;
    getIndex(mg[level].gridIndexRange(),I1,I2,I3,1);  // include a ghost line.
    const realArray & x = mg[level].vertex();
    const realArray & dist = evaluate( fabs( SQRT( SQR(x(I1,I2,I3,0)-x0)+SQR(x(I1,I2,I3,1)-x1) ) - r0 ) );
    
    const real alpha=8., beta=7.;
    weight(I1,I2,I3)=1.+alpha*exp((-beta)*dist);
  }

  getIndex(mg[level].gridIndexRange(),I1,I2,I3); 
  source[level](I1,I2,I3,0)=weight.r1()(I1,I2,I3)/weight(I1,I2,I3);
  source[level](I1,I2,I3,1)=weight.r2()(I1,I2,I3)/weight(I1,I2,I3);

  return 0;
}



int EllipticGridGenerator::
periodicUpdate(realMappedGridFunction & x, 
	       const Range & C /* =nullRange */,
	       const bool & isAGridFunction /* = TRUE */ )
//===========================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Perform a periodic update. If isAGridFunction==true then the grid function
///  is assume to hold grid coordinates so that the derivativePeriodic case is handled
///  correctly. In this case the values are periodic but with a shift added.
///   For example the case of a square grid with periodic boundary conditions.
/// 
/// \param x (input/output) : update this grid function
/// \param C (input) : update these components (update all components by default)
/// \param isAGridFunction (input) : if true these are grid coordinates.
//===========================================================================
{
  x.periodicUpdate(C);
  if( isAGridFunction )
  {
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    MappedGrid & c = *x.getMappedGrid();
    int axis,dir;
    int c0 = C.getLength()<=0 ? 0                : C.getBase();
    int c1 = C.getLength()<=0 ? rangeDimension-1 : C.getBound();
  
    real periodVector[3]={0.,0.,0.}; 
    for( axis=0; axis<domainDimension; axis++ )
    {

      if( FALSE && (bool)userMap->getIsPeriodic(axis) )
      {
        // Make sure that grid points on periodic boundaries remain on the original edge.
	getBoundaryIndex(c.dimension(),Start,axis,I1,I2,I3);
	Iv[axis]=Range(c.gridIndexRange(Start,axis),c.gridIndexRange(Start,axis));
	x(I1,I2,I3,0)=-1.;
      
	// for( dir=c0; dir<=c1; dir++ )
	//  x(I1,I2,I3,dir)-=periodVector[dir];
      }

      if( userMap->getIsPeriodic(axis)==Mapping::derivativePeriodic )
      {
        real sumAbs=0.;
	for(  dir=0; dir<rangeDimension; dir++ )
	{
          periodVector[dir]=userMap->getPeriodVector(dir,axis);
          sumAbs+=fabs(periodVector[dir]);
	}
	
        if( ( fabs(periodVector[0])+fabs(periodVector[1])+fabs(periodVector[2]) ) ==0. )
	{
	  printf("periodicUpdate:WARNING: the grid axis=%i is derivativePeriodic but the periodVector is all zeroes.\n"
                 "There is probably a mistake in the original Mapping, not setting the periodVector properly \n"
                 "since the periodVector is a `new' option. I will try to set this periodVector.\n",axis);
	
          printf("periodicUpdate: periodVector=(%6.2e,%6.2e,%6.2e)\n",userMap->getPeriodVector(0,axis),
	       userMap->getPeriodVector(1,axis), rangeDimension==3 ? userMap->getPeriodVector(2,axis) : 0.);

          // compute the period vector
          const realArray & x = userMap->getGrid();
          Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
          for( dir=0; dir<3; dir++ )
	  {
   	    Iv[dir]=x.dimension(dir);
            Jv[dir]=Iv[dir];
	  }
	  Iv[axis]=Iv[axis].getBase();
	  Jv[axis]=Jv[axis].getBound();
          realArray xDiff(I1,I2,I3,Rx);
          xDiff = x(J1,J2,J3,Rx)-x(I1,I2,I3,Rx);
          printf("Setting the period vector to be (");
          for( dir=0; dir<rangeDimension; dir++ )
	  {
	    const real maxDiff=max(xDiff(I1,I2,I3,dir));
	    const real minDiff=min(xDiff(I1,I2,I3,dir));
            if( fabs(maxDiff-minDiff) <= REAL_EPSILON*10.*maxDiff )
	    {
	      periodVector[dir]=.5*(maxDiff+minDiff);
	      userMap->setPeriodVector(dir,axis,periodVector[dir]);
	    }
	    else
	    {
	      printf("\nperiodicUpdate:ERROR? the grid axis=%i is derivativePeriodic but the grid does not seem\n"
		     " to match. max(x_%i(right)-x_%i(left))=%e, min(x(right)-x(left))=%e \n",axis,dir,dir,
                     maxDiff,minDiff);
	      throw "error";
	    }
            sumAbs+=fabs(periodVector[dir]);
            printf("%6.2e,",periodVector[dir]);
	  }
          printf(")\n");
	}

        if( sumAbs>0. )
	{
	  getBoundaryIndex(c.dimension(),Start,axis,I1,I2,I3);
	  Iv[axis]=Range(c.dimension(Start,axis),c.gridIndexRange(Start,axis)-1);
	  for( dir=c0; dir<=c1; dir++ )
	    x(I1,I2,I3,dir)-=periodVector[dir];
	  Iv[axis]=Range(c.gridIndexRange(End,axis),c.dimension(End,axis));
	  for( dir=c0; dir<=c1; dir++ )
	    x(I1,I2,I3,dir)+=periodVector[dir];
	}
	
      }
    }
  }
  return 0;
}
