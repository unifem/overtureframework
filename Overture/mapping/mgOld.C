#include "multigrid1.h"
#include "blockTridiag2d.h"
#include "display.h"

static int TEST_RESIDUAL=0;

void EllipticGridGenerator::
Interpolate(int i1, int i2, realArray &u1, realArray &u2, int jmax)
// ===================================================================================
// /Description:
//   Interpolate the solution at level1 from level2.
//   if level1<level2 then this is a prologations, otherwise it is a restriction.
// ==================================================================================
{
  Index Idouble, I, Jdouble, J;

  Index J1,J2,J3;
  getIndex(mg[i1].gridIndexRange(),J1,J2,J3);
  
  Index K1,K2,K3;
  getIndex(mg[i2].gridIndexRange(),K1,K2,K3);

  switch (rangeDimension)
  {
  case 1:
    if ((i1>i2)&&(J1.getBound()<K1.getBound()))
    {
      // interpolation from coarser to finer
      Idouble=Range(K1.getBase(), K1.getBound(),2);
      u2(Idouble,K2,K3,Rx)=u1(J1,J2,J3,Rx);
      Idouble=Range(K1.getBase(), K1.getBound()-2,2);
      I=Range(J1.getBase(), J1.getBound()-1);

      u2(Idouble+1,K2,K3,Rx)=0.5*(u1(I,J2,J3,Rx)+u1(I+1,J2,J3,Rx));
    }
    else if ((i1<i2)&&(J1.getBound()>K1.getBound()))
    {
      // interpolation from finer to coarser
      Idouble=Range(2,J1.getBound()-2,2);
      I=Range(1,K1.getBound()-1);

      u2(K1.getBase(),K2,K3,Rx)=	u1(J1.getBase(),J2,J3,Rx);
      u2(K1.getBound(),K2,K3,Rx)=u1(J1.getBound(),J2,J3,Rx);
      u2(I,K2,K3,Rx)=0.25*(u1(Idouble-1,J2,J3,Rx)+2.0*u1(Idouble,J2,J3,Rx)+u1(Idouble+1,J2,J3,Rx));
    }
    else fprintf(stdout,"\n Wrong interpolation order \n"), exit(1);
    break;

  case 2:
    if ((i1>i2)&&(J1.getBound()<K1.getBound())&&
	(J2.getBound()<K2.getBound()))
    {
      // Interpolate from coarser to finer
      // start with the 2i,2j points
      Idouble=Range(K1.getBase(), K1.getBound(),2);
      Jdouble=Range(K2.getBase(), K2.getBound(),2);
      u2(Idouble,Jdouble,K3,Rx)=u1(J1,J2,J3,Rx);

      // i is even (2i) and j is odd (2j+1)
      Idouble=Range(K1.getBase(), K1.getBound(),2);
      Jdouble=Range(K2.getBase()+1, K2.getBound()-1,2);
      I=Range(J1.getBase(), J1.getBound());
      J=Range(J2.getBase(), J2.getBound()-1);
      u2(Idouble,Jdouble,K3,Rx)=0.5*(u1(I,J,J3,Rx)+u1(I,J+1,J3,Rx));

	// i is odd (2i+1) and j is even (2j)
      Idouble=Range(K1.getBase()+1, K1.getBound()-1,2);
      Jdouble=Range(K2.getBase(),K2.getBound(),2);
      I=Range(J1.getBase(), J1.getBound()-1);
      J=Range(J2.getBase(), J2.getBound());
      u2(Idouble,Jdouble,K3,Rx)=0.5*(u1(I,J,J3,Rx)+u1(I+1,J,J3,Rx));

	// i is odd (2i+1) and j is odd (2j+1)
      Idouble=Range(K1.getBase()+1, K1.getBound()-1,2);
      Jdouble=Range(K2.getBase()+1, K2.getBound()-1,2);
      I=Range(J1.getBase(), J1.getBound()-1);
      J=Range(J2.getBase(), J2.getBound()-1);
      u2(Idouble,Jdouble,K3,Rx)=0.25*(u1(I,J,J3,Rx)+u1(I+1,J,J3,Rx)+ u1(I,J+1,J3,Rx)+u1(I+1,J+1,J3,Rx));
    }
    else if ((i1<i2)&&(J1.getBound()>K1.getBound())&&
	     (J2.getBound()>K2.getBound()))
    {
      // interpolation from finer to coarser
      // Interior point use 9 points full weighted
      Idouble=Range(J1.getBase()+2, J1.getBound()-2,2);
      Jdouble=Range(J2.getBase()+2, J2.getBound()-2,2);
      I=Range(K1.getBase()+1, K1.getBound()-1);
      J=Range(K2.getBase()+1, K2.getBound()-1);
      u2(I,J,K3,Rx)=(1.0/16.0)*(u1(Idouble-1, Jdouble-1, J3,Rx)+
			       2.0*u1(Idouble,   Jdouble-1, J3,Rx)+
			       u1(Idouble+1, Jdouble-1, J3,Rx)+
			       2.0*u1(Idouble-1, Jdouble,   J3,Rx)+
			       4.0*u1(Idouble,   Jdouble,   J3,Rx)+
			       2.0*u1(Idouble+1, Jdouble,   J3,Rx)+
			       u1(Idouble-1, Jdouble+1, J3,Rx)+
			       2.0*u1(Idouble,   Jdouble+1, J3,Rx)+
			       u1(Idouble+1, Jdouble+1, J3,Rx));

      //Use also full weighting on the boundary points
      //and same values in the 4 corners
      Idouble=Range(J1.getBase()+2,J1.getBound()-2,2);
      Jdouble=Range(J2.getBase()+2,J2.getBound()-2,2);
      I=Range(K1.getBase()+1, K1.getBound()-1);
      J=Range(K2.getBase()+1, K2.getBound()-1);
      u2(I,K2.getBase(),K3,Rx)=
	0.25*(   u1(Idouble-1,J2.getBase(),J3,Rx)+
		 2.0*u1(Idouble,  J2.getBase(),J3,Rx)+
		 u1(Idouble+1,J2.getBase(),J3,Rx));
      u2(I,K2.getBound(),K3,Rx)=
	0.25*(   u1(Idouble-1,J2.getBound(),J3,Rx)+
		 2.0*u1(Idouble,  J2.getBound(),J3,Rx)+
		 u1(Idouble+1,J2.getBound(),J3,Rx));
      u2(K1.getBase(),J,K3,Rx)=
	0.25*(   u1(J1.getBase(),Jdouble-1,J3,Rx)+
		 2.0*u1(J1.getBase(),Jdouble,  J3,Rx)+
		 u1(J1.getBase(),Jdouble+1,J3,Rx));
      u2(K1.getBound(),J,K3,Rx)=
	0.25*(   u1(J1.getBound(),Jdouble-1,J3,Rx)+
		 2.0*u1(J1.getBound(),Jdouble,  J3,Rx)+
		 u1(J1.getBound(),Jdouble+1,J3,Rx));
      u2(K1.getBase(), K2.getBase(),K3,Rx)=
	u1(J1.getBase(),J2.getBase(),J3,Rx);
      u2(K1.getBound(), K2.getBase(),K3,Rx)=
	u1(J1.getBound(),J2.getBase(),J3,Rx);
      u2(K1.getBase(), K2.getBound(),K3,Rx)=
	u1(J1.getBase(),J2.getBound(),J3,Rx);
      u2(K1.getBound(), K2.getBound(),K3,Rx)=
	u1(J1.getBound(),J2.getBound(),J3,Rx);
    }
    else 
      fprintf(stdout,"\n Wrong interpolation order \n"), exit(1);

    break;
       
  default:
    printf("Interpolate:Untreated condition\n");
    {throw "error";}
  }
}


int **multigrid::
IntArray2d(int istart, int iend, int jstart, int jend)
{
  int **t;
  int i,nrow=iend-istart+1, ncolumn=jend-jstart+1;

  t = new int*[nrow];
  for (i=0;i<nrow;i++)
  {
    t[i]=new int[ncolumn];
  }
  return t;
}

void multigrid::
DeleteIntArray2d(int **t, int istart, int iend, int jstart, int jend)
{
  int i, nrow=iend-istart+1, ncolumn=jend-jstart+1;
  for (i=0; i<nrow; i++) delete[] t[i], t[i]=NULL;
  delete[] t, t=NULL;
}

realArray **multigrid::
RealArray2d(int istart, int iend, int jstart, int jend)
{
  realArray **t;
  int i,nrow=iend-istart+1, ncolumn=jend-jstart+1;

  t = new realArray*[nrow];
  for (i=0;i<nrow;i++)
  {
    t[i]=new realArray[ncolumn];
  }
  return t;
}

void multigrid::
DeleteRealArray2d(realArray **t, int istart, int iend, int jstart, int jend)
{
  int i, nrow=iend-istart+1, ncolumn=jend-jstart+1;
  for (i=0; i<nrow; i++) delete[] t[i], t[i]=NULL;
  delete[] t, t=NULL;
}


void multigrid::
get2DBoundaryResidual(int i, realArray &resid1, realArray *up)
// =================================================================================
// Computes the residual on the boundary in the case of orthogonal grid boundary condition
// /i (input) : level number
// /resid1 (output) :
// /up : Not used??
// =================================================================================
{
  realArray Xc_e, Yc_e;
  Index I11, I22, I33;
  int istart, istop, jstart,jstop, kstart,kstop;
  IntegerArray index(2,3);
  int is[3] =   {0,0,0}; // To control the sign according to side
  int icdif[3] =   {0,0,0}; // To control centered differences

  index(0,0)=I1[i].getBase();
  istart=index(0,0);
  index(1,0)=I1[i].getBound();
  istop=index(1,0);
  index(0,1)=I2[i].getBase();
  jstart=index(0,1);
  index(1,1)=I2[i].getBound();
  jstop=index(1,1);
  index(0,2)=I3[i].getBase();
  kstart=index(0,2);
  index(1,2)=I3[i].getBound();
  kstop=index(1,2);

  for (int axis=0; axis<2; axis++)
    for (int side=0; side<=1; side++)
    {
      getBoundaryIndex(index,side,axis,I11,I22,I33);
      if (gridBc(side,axis)==2)
      {
	is[0]=0, is[1]=0, is[2]=0;
	is[axis]=1-2*side;
	if (axis==0) 
	{
	  icdif[0]=0; 
	  icdif[1]=1; 
	  //if (rangeDimension==3) icdif[2]=1;
	  //else icdif[2]=0;
        }
	else if (axis==1)
	{
	  icdif[0]=1;
	  icdif[1]=0;
	  //if (rangeDimension==3) icdif[2]=1;
	  //else icdif[2]=0;
        }
	Xc_e.redim(I11,I22,I33), Yc_e.redim(I11,I22,I33);
	Xc_e(I11,I22,I33)=uprev[i](I11+icdif[0],I22+icdif[1],I33,0)-
	  uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
	Yc_e(I11,I22,I33)=uprev[i](I11+icdif[0],I22+icdif[1],I33,1)-
	  uprev[i](I11-icdif[0],I22-icdif[1],I33,1);
        if (useBlockTridiag != 1)
	{
          if (i==0)
	  {
            resid1(I11,I22,I33,0)=
	      -Xc_e(I11,I22,I33)*u[i](I11+is[0],I22+is[1],I33,0)-
	      Yc_e(I11,I22,I33)*u[i](I11+is[0],I22+is[1],I33,1)+
	      Xc_e(I11,I22,I33)*u[i](I11,I22,I33,0)+
	      Yc_e(I11,I22,I33)*u[i](I11,I22,I33,1);
            resid1(I11,I22,I33,1)=
	      Xc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,1)-
	      Yc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,0)+
	      Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0)-
	      Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1);
          }
          else 
	  {
            resid1(I11,I22,I33,0)=
	      RHS[i](I11,I22,I33,0)-
	      Xc_e(I11,I22,I33)*(u[i](I11+is[0],I22+is[1],I33,0)-
				 u[i](I11,I22,I33,0))-
	      Yc_e(I11,I22,I33)*(u[i](I11+is[0],I22+is[1],I33,1)-
				 u[i](I11,I22,I33,1));
            resid1(I11,I22,I33,1)=
	      RHS[i](I11,I22,I33,1)-
	      Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	      Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
          }
        }
        else
	{
          if (i==0)
	  {
            where (fabs(Xc_e(I11,I22,I33))>0.00001)
	    {
	      resid1(I11,I22,I33,0)=
		-Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
				    u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
				   u[i](I11+is[0],I22+is[1],I33,1));
              resid1(I11,I22,I33,1)=
		-Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0)+
	        Xc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,1)-
	        Yc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
            }
            elsewhere()
	    {
	      resid1(I11,I22,I33,0)=
		-Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0)+
	        Xc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,1)-
	        Yc_e(I11,I22,I33)*uprev[i](I11-icdif[0],I22-icdif[1],I33,0);
              resid1(I11,I22,I33,1)=
		-Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
				    u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
				   u[i](I11+is[0],I22+is[1],I33,1));
            }
          }
          else
	  {
	    where (fabs(Xc_e(I11,I22,I33))>0.00001)
	    {
	      resid1(I11,I22,I33,0)=
	        RHS[i](I11,I22,I33,0)-
	        Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
				   u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
				   u[i](I11+is[0],I22+is[1],I33,1));
              resid1(I11,I22,I33,1)=
	        RHS[i](I11,I22,I33,1)-
	        Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
	    }
	    elsewhere()
	    {
	      resid1(I11,I22,I33,0)=
	        RHS[i](I11,I22,I33,0)-
	        Xc_e(I11,I22,I33)*u[i](I11,I22,I33,1)+
	        Yc_e(I11,I22,I33)*u[i](I11,I22,I33,0);
              resid1(I11,I22,I33,1)=
	        RHS[i](I11,I22,I33,1)-
	        Xc_e(I11,I22,I33)*(u[i](I11,I22,I33,0)-
				   u[i](I11+is[0],I22+is[1],I33,0))-
                Yc_e(I11,I22,I33)*(u[i](I11,I22,I33,1)-
				   u[i](I11+is[0],I22+is[1],I33,1));
	    }
          }
        }
      }
      where((GRIDBC[2*axis+side][i](I11,I22,I33)==1)||
	    (GRIDBC[2*axis+side][i](I11,I22,I33)==10))
      {
 	resid1(I11,I22,I33,0)=0.;
  	resid1(I11,I22,I33,1)=0.;
      }
    }
  resid1(istart,jstart,kstart,Range(0,1))=0.;
  resid1(istop,jstart,kstart,Range(0,1))=0.;
  resid1(istart,jstop,kstart,Range(0,1))=0.;
  resid1(istop,jstop,kstart,Range(0,1))=0.;
}



void multigrid::
find2Dcoefficients(realArray &a1, realArray &b1, realArray &c1,
		   realArray &d1, realArray &e1, Index I11, 
		   Index I22, Index I33, int i, int j)
{
  realArray Xc,Xe,Yc,Ye;
  int j11, istart, istop, jstart, jstop;

  Xc.redim(I11,I22,I33);
  Yc.redim(I11,I22,I33);
  Xe.redim(I11,I22,I33);
  Ye.redim(I11,I22,I33);

  Yc(I11,I22,I33)=(u[i](I11+1,I22,I33,1)-
		   u[i](I11-1,I22,I33,1))/(2*dx(0,i));
  Ye(I11,I22,I33)=(u[i](I11,I22+1,I33,1)-
		   u[i](I11,I22-1,I33,1))/(2*dx(1,i));
  Xc(I11,I22,I33)=(u[i](I11+1,I22,I33,0)-
		   u[i](I11-1,I22,I33,0))/(2*dx(0,i));
  Xe(I11,I22,I33)=(u[i](I11,I22+1,I33,0)-
		   u[i](I11,I22-1,I33,0))/(2*dx(1,i));

  a1(I11,I22,I33)=Xe(I11,I22,I33)*
    Xe(I11,I22,I33)+
    Ye(I11,I22,I33)*
    Ye(I11,I22,I33);
  b1(I11,I22,I33)=Xc(I11,I22,I33)*
    Xc(I11,I22,I33)+
    Yc(I11,I22,I33)*
    Yc(I11,I22,I33);
  c1(I11,I22,I33)=2.0*(Xc(I11,I22,I33)*
		       Xe(I11,I22,I33)+
		       Yc(I11,I22,I33)*
		       Ye(I11,I22,I33));
}

void multigrid::
getAlpha(void)
{
  int i,j;
  realArray a1(I1[0],I2[0],I3[0]), b1(I1[0],I2[0],I3[0]);
  realArray c1(I1[0],I2[0],I3[0]), d1, e1;
  real a00, b00, c00=1.0;

  a1=0.0, b1=0.0, c1=0.0;
  getSource(0,0);
  find2Dcoefficients(a1,b1,c1,d1,e1,Iint1[0],Iint2[0],Iint3[0],0,0);
  real dx0=dx(0,0), dy0=dx(1,0);
  
  for (int j2=1;j2<I2[0].getBound();j2++)
  {
    real maxdiff, maxquotient;
    int imax, jmax;
    maxdiff=0., maxquotient=1.0, imax=1, jmax=1;
    for (int i2 = 1; i2<I1[0].getBound(); i2++)
    {
      a00=0.2*(dx0*dx0*b1(i2,j2,0)+dy0*dy0*a1(i2,j2,0))-
	0.5*dx0*dy0*fabs(c1(i2,j2,0));
      b00=fabs(0.5*dx0*dy0*dy0*a1(i2,j2,0)*Source[0](i2,j2,0,0))+
	fabs(0.5*dx0*dx0*dy0*b1(i2,j2,0)*Source[0](i2,j2,0,1));
      if ((a00/b00)<c00) c00=a00/b00;
    }
  }
  alpha=c00;
  printf("alpha= %g\t",alpha);
}

void multigrid::
jacobi2Dsolve(const realArray &a1, const realArray &b1, 
	      const realArray &c1, const realArray &d1,
	      const realArray &e1, Index I11, Index I22, 
	      Index I33, int j, int i, realArray &u1, 
	      realArray *up, int periodicCorrection)
{ 
  int istart, jstart, istop, jstop;
  real omega2=1.0;

  //Apply first the Boundary condition
  applyOrthogonalBoundaries(I11,I22,I33,j,i,u1);
  if (TEST_RESIDUAL)
  {
    printf("Dans solve apres BC i=%i\t j=%i\n",i,j);
    int imax=0,jmax=0,i1max=0, j1max=0;
    real resmaxx=0.0, resmaxy=0.0;
    realArray restemp1;
    restemp1.redim(I1[i],I2[i],I3[i],ndimension);
    restemp1=0.0;
    getResidual(restemp1,i);
    //(u1(I1[i],Range(0,2),I3[i],Range(0,1))).display("u after BC");
    //Source[i](I1[i],Range(0,2),I3[i],Range(0,1)).display("SOURCE");
    //restemp1(I1[i],Range(0,2),I3[i],Range(0,1)).display("restemp1 before source");
    for (int j1=0; j1<=I2[i].getBound(); j1++)
      for (int i1=0; i1<=I1[i].getBound(); i1++)
      {
	if (fabs(restemp1(i1,j1,0,0))>resmaxx)
	  imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
	if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	  i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
      }
    printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
	   imax,jmax,resmaxx,i1max,j1max,resmaxy);
  }
  // Regular method
  if ((smoothingMethod==1)||((smoothingMethod==2)&&
			     (periodicCorrection != 1)))
    u1(I11,I22,I33,j)=
      (1.0-omega)*u1(I11,I22,I33,j)+
      omega*(dx(1,i)*dx(1,i)*a1(I11,I22,I33)*
	     (u1(I11+1,I22,I33,j)+
	      u1(I11-1,I22,I33,j))+
	     dx(0,i)*dx(0,i)*b1(I11,I22,I33)*
	     (u1(I11,I22+1,I33,j)+
	      u1(I11,I22-1,I33,j))+
	     dx(1,i)*dx(1,i)*dx(0,i)*a1(I11,I22,I33)*
	     Source[i](I11,I22,I33,0)*
	     (u1(I11+1,I22,I33,j)-
	      u1(I11-1,I22,I33,j))/2.0+
	     dx(0,i)*dx(0,i)*dx(1,i)*b1(I11,I22,I33)*
	     Source[i](I11,I22,I33,1)*
	     (u1(I11,I22+1,I33,j)-
	      u1(I11,I22-1,I33,j))/2.0-
	     dx(0,i)*dx(1,i)*c1(I11,I22,I33)*
	     (u1(I11+1,I22+1,I33,j)-
	      u1(I11-1,I22+1,I33,j)-
	      u1(I11+1,I22-1,I33,j)+
	      u1(I11-1,I22-1,I33,j))/4.0-
	     dx(0,i)*dx(0,i)*dx(1,i)*dx(1,i)*
	     RHS[i](I11,I22,I33,j))/
      (2.0*a1(I11,I22,I33)*dx(1,i)*dx(1,i)+
       2.0*b1(I11,I22,I33)*dx(0,i)*dx(0,i));
  if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic)
  {
    istart=I1[i].getBase(), istop=I1[i].getBound();
    if ((smoothingMethod==1)||((smoothingMethod==2)&&
			       (periodicCorrection==1)))
    {
      u1(istart,I22,I33,j)=
	(1.0-omega)*u1(istart,I22,I33,j)+
	omega*(dx(1,i)*dx(1,i)*a1(istart,I22,I33)*
	       (u1(istart+1,I22,I33,j)+
		u1(istop-1,I22,I33,j))+
	       dx(0,i)*dx(0,i)*b1(istart,I22,I33)*
	       (u1(istart,I22+1,I33,j)+
		u1(istart,I22-1,I33,j))+
	       dx(1,i)*dx(1,i)*dx(0,i)*a1(istart,I22,I33)*
	       Source[i](istart,I22,I33,0)*
	       (u1(istart+1,I22,I33,j)-
		u1(istop-1,I22,I33,j))/2.0+
	       dx(0,i)*dx(0,i)*dx(1,i)*b1(istart,I22,I33)*
	       Source[i](istart,I22,I33,1)*
	       (u1(istart,I22+1,I33,j)-
		u1(istart,I22-1,I33,j))/2.0-
	       dx(0,i)*dx(1,i)*c1(istart,I22,I33)*
	       (u1(istart+1,I22+1,I33,j)-
		u1(istop-1,I22+1,I33,j)-
		u1(istart+1,I22-1,I33,j)+
		u1(istop-1,I22-1,I33,j))/4.0-
	       dx(0,i)*dx(0,i)*dx(1,i)*dx(1,i)*
	       RHS[i](istart,I22,I33,j))/
	(2.0*a1(istart,I22,I33)*dx(1,i)*dx(1,i)+
	 2.0*b1(istart,I22,I33)*dx(0,i)*dx(0,i));

      u1(istop,I22,I33,j)=u1(istart,I22,I33,j);
      //ghostpoints
      u1(istart-1,I22,I33,j)=u1(istop-1,I22,I33,j);
      u1(istop+1,I22,I33,j)=u1(istart+1,I22,I33,j);
    }
  }
  if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic)
  {
    jstart=I2[i].getBase(), jstop=I2[i].getBound();
    if ((smoothingMethod==1)||((smoothingMethod==2)&&
			       (periodicCorrection==1)))
    {
      u1(I11,jstart,I33,j)=
	(1.0-omega)*u1(I11,jstart,I33,j)+
	omega*(dx(1,i)*dx(1,i)*a1(I11,jstart,I33)*
	       (u1(I11+1,jstart,I33,j)+
		u1(I11-1,jstart,I33,j))+
	       dx(0,i)*dx(0,i)*b1(I11,jstart,I33)*
	       (u1(I11,jstart+1,I33,j)+
		u1(I11,jstop-1,I33,j))+
	       dx(1,i)*dx(1,i)*dx(0,i)*a1(I11,jstart,I33)*
	       Source[i](I11,jstart,I33,0)*
	       (u1(I11+1,jstart,I33,j)-
		u1(I11-1,jstart,I33,j))/2.0+
	       dx(0,i)*dx(0,i)*dx(1,i)*b1(I11,jstart,I33)*
	       Source[i](I11,jstart,I33,1)*
	       (u1(I11,jstart+1,I33,j)-
		u1(I11,jstop-1,I33,j))/2.0-
	       dx(0,i)*dx(1,i)*c1(I11,jstart,I33)*
	       (u1(I11+1,jstart+1,I33,j)-
		u1(I11-1,jstart+1,I33,j)-
		u1(I11+1,jstop-1,I33,j)+
		u1(I11-1,jstop-1,I33,j))/4.0-
	       dx(0,i)*dx(0,i)*dx(1,i)*dx(1,i)*
	       RHS[i](I11,jstart,I33,j))/
	(2.0*a1(I11,jstart,I33)*dx(1,i)*dx(1,i)+
	 2.0*b1(I11,jstart,I33)*dx(0,i)*dx(0,i));

      u1(I11,jstop,I33,j)=u1(I11,jstart,I33,j);
      //ghostpoints
      u1(I11,jstart-1,I33,j)=u1(I11,jstop-1,I33,j);
      u1(I11,jstop+1,I33,j)=u1(I11,jstart+1,I33,j);
    }
  }

  if (TEST_RESIDUAL)
  {
    printf("Dans solve apres interior i=%i\t j=%i\n",i,j);
    int imax=0,jmax=0,i1max=0, j1max=0;
    real resmaxx=0.0, resmaxy=0.0;
    realArray restemp1;
    restemp1.redim(I1[i],I2[i],I3[i],ndimension);
    restemp1=0.0;
    getResidual(restemp1,i);
    for (int j1=0; j1<=I2[i].getBound(); j1++)
      for (int i1=0; i1<=I1[i].getBound(); i1++)
      {
	if (fabs(restemp1(i1,j1,0,0))>resmaxx)
	  imax=i1, jmax=j1, resmaxx= fabs(restemp1(i1,j1,0,0));
	if (fabs(restemp1(i1,j1,0,1))>resmaxy)
	  i1max=i1, j1max=j1, resmaxy=fabs(restemp1(i1,j1,0,1));
      }
    printf("Xerror :: %i  %i %g\t Yerror :: %i %i %g\n\n",
	   imax,jmax,resmaxx,i1max,j1max,resmaxy);
    //restemp1(I1[i],Range(0,3),0,Range(0,1)).display("C'est restemp1");
  }
  //applyOrthogonalBoundaries(I11,I22,I33,j,i,u1,uprev);
  //printf("x33=%g   y33=%g   :: %g    %g\t <> %g   %g\n",u[i](33,0,0,0),u[i](33,0,0,1),u1(33,0,0,0),u1(33,0,0,1), uprev[i](33,0,0,0), uprev[i](33,0,0,1));
  //printf("x32=%g   y32=%g\t :: %g    %g\n",u[i](32,0,0,0), u[i](32,0,0,1), uprev[i](32,0,0,0), uprev[i](32,0,0,1));
  //printf("x31=%g   y31=%g\t :: %g    %g\n",u[i](31,0,0,0), u[i](31,0,0,1), uprev[i](31,0,0,0), uprev[i](31,0,0,1));
}

void multigrid::
line2Dsolve(realArray &a1, realArray &b1, 
	    realArray &c1, realArray &d1,
	    Index I11, Index I22, Index I33, 
	    const realArray &coeff1, const realArray &coeff2, 
	    const realArray &coeff3, const realArray &coeff4,
	    const realArray &coeff5, Index Ic1, Index Ic2, 
	    Index Ic3, int j, int i, realArray &u1, int isweep, 
	    realArray *up)
{
  TridiagonalSolver tri;
  realArray Xc, Yc, Xe, Ye, X2, Y2, XY, normXc, normXe;
  int i1, j1,istart, jstart, istop,jstop, isharp=0;
  jstart=I2[i].getBase(), jstop=I2[i].getBound();
  istart=I1[i].getBase(), istop=I1[i].getBound();

    /*****VOYONS
    printf("\n variable=%i\t level=%i\t isweep=%i\n\n", j,i,isweep);
    //if (i==1){
    for (j1=jstart;j1<=jstop;j1++)
    for (i1=istart;i1<=istop;i1++)
    printf("Before i=%i\t j=%i\t x=%g\t y=%g\t xp=%g\t yp=%g\n",i1,j1,u1(i1,j1,0,0),u1(i1,j1,0,1),uprev[i](i1,j1,0,0),uprev[i](i1,j1,0,1));
    //}
    VOYONS****/
  a1=0.0, b1=0.0, c1=0.0, d1=0.0;
  b1(Ic1,Ic2,Ic3)=-2.0*coeff1(Ic1,Ic2,Ic3)/(dx(0,i)*dx(0,i))-
    2.0*coeff2(Ic1,Ic2,Ic3)/(dx(1,i)*dx(1,i));
  int axis=isweep, side, i0, j0;
  Index I111,I222,I333;
  intArray index(2,3);
  int is[3], icdif[3];

  index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
  index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
  index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
  if (isweep==0)
  {
    istart=I11.getBase(), istop=I11.getBound();
    a1(Ic1,Ic2,Ic3)=coeff1(Ic1,Ic2,Ic3)*(1.0/(dx(0,i)*dx(0,i))-
					 Source[i](Ic1,Ic2,Ic3,0)/(2*dx(0,i)));
    c1(Ic1,Ic2,Ic3)=coeff1(Ic1,Ic2,Ic3)*(1.0/(dx(0,i)*dx(0,i))+
					 Source[i](Ic1,Ic2,Ic3,0)/(2*dx(0,i)));
    d1(Ic1,Ic2,Ic3)=RHS[i](Ic1,Ic2,Ic3,j)+
      coeff3(Ic1,Ic2,Ic3)*(u1(Ic1+1,Ic2+1,Ic3,j)-
			   u1(Ic1+1,Ic2-1,Ic3,j)-
			   u1(Ic1-1,Ic2+1,Ic3,j)+
			   u1(Ic1-1,Ic2-1,Ic3,j))/
      (4.0*dx(0,i)*dx(1,i))-
      coeff2(Ic1,Ic2,Ic3)*((u1(Ic1,Ic2+1,Ic3,j)+
			    u1(Ic1,Ic2-1,Ic3,j))/(dx(1,i)*dx(1,i))+
			   Source[i](Ic1,Ic2,Ic3,1)*
			   (u1(Ic1,Ic2+1,Ic3,j)-u1(Ic1,Ic2-1,Ic3,j))/(2*dx(1,i)));
    if (userMap->getIsPeriodic(0) != Mapping::functionPeriodic)
    {
      for (side=0;side<2;side++)
      {
	i0=2*axis+side;
	is[axis]=1-2*side;
	getBoundaryIndex(index,side,axis,I111,I222,I333);
	if ((gridBc(side,axis)==1)||(gridBc(side,axis)==3))
	{
	  b1(I111,I22,I33)=1.0;
	  d1(I111,I22,I33)=u1(I111,I22,I33,j);
	}
	else if (gridBc(side,axis)==2)
	{
	  Xe.redim(I111,I22,I333), Ye.redim(I111,I22,I333);
	  X2.redim(I111,I22,I333), Y2.redim(I111,I22,I333);
	  XY.redim(I111,I22,I333), normXe.redim(I111,I22,I333);

	  Xe(I111,I22,I333)=uprev[i](I111,I22+1,I33,0)-
	    uprev[i](I111,I22-1,I33,0);
	  Ye(I111,I22,I333)=uprev[i](I111,I22+1,I33,1)-
	    uprev[i](I111,I22-1,I33,1);
	  X2(I111,I22,I333)=Xe(I111,I22,I333)*Xe(I111,I22,I333);
	  Y2(I111,I22,I333)=Ye(I111,I22,I333)*Ye(I111,I22,I333);
	  XY(I111,I22,I333)=Xe(I111,I22,I333)*Ye(I111,I22,I333);
	  normXe(I111,I22,I333)=X2(I111,I22,I333)+Y2(I111,I22,I333);

	  if (side==0) a1(I111,I22,I33)= 0.0;
	  else if (side==1) c1(I111,I22,I33)=0.0;
	  b1(I111,I22,I33)= normXe(I111,I22,I333);
	  if (i==0)
	  {
	    if (j==0)
	    {
	      if (side==0) c1(I111,I22,I33)= -X2(I111,I22,I33);
	      else if (side==1) a1(I111,I22,I33) = -X2(I111,I22,I33);
	      d1(I111,I22,I33)=
		XY(I111,I22,I333)*u1(I111+is[0],I22,I33,1)-
		XY(I111,I22,I333)*uprev[i](I111,I22-1,I33,1)+
		Y2(I111,I22,I333)*uprev[i](I111,I22-1,I33,0);
	    }
	    else
	    {
	      if (side==0) c1(I111,I22,I33)= -Y2(I111,I22,I333);
	      else if (side==1) a1(I111,I22,I33)= -Y2(I111,I22,I333);
	      d1(I111,I22,I33)= 
		X2(I111,I22,I333)*uprev[i](I111,I22-1,I33,1)-
		XY(I111,I22,I333)*uprev[i](I111,I22-1,I33,0)+
		XY(I111,I22,I333)*u1(I111+is[0],I22,I33,0);
	    }
	  }
	  else
	  {
	    if (j==0)
	    {
	      if (side==0) c1(I111,I22,I33)= -X2(I111,I22,I333);
	      else if (side==1) a1(I111,I22,I333)=-X2(I111,I22,I333);
	      d1(I111,I22,I33)= 
		XY(I111,I22,I333)*u1(I111+is[0],I22,I33,1)-
		Xe(I111,I22,I333)*RHS[i](I111,I22,I33,0)-
		Ye(I111,I22,I333)*RHS[i](I111,I22,I33,1);
	    }
	    else
	    {
	      if (side==0) c1(I111,I22,I33)= -Y2(I111,I22,I333);
	      else if (side==1) a1(I111,I22,I33)= -Y2(I111,I22,I333);
	      d1(I111,I22,I33)=
		Xe(I111,I22)*RHS[i](I111,I22,I33,1)+
		XY(I111,I22)*u1(I111+is[0],I22,I33,0)-
		Ye(I111,I22)*RHS[i](I111,I22,I33,0);
	    }
	  }
	  if (I22.getBase()==jstart)
	  {
	    a1(I111,jstart,I33)=0.0;
	    c1(I111,jstart,I33)=0.0;  
	    d1(I111,jstart,I33)=u1(I111,jstart,I33,j);
	    b1(I111,jstart,I33)=1.0;
	  }
	  if (I22.getBound()==jstop)
	  {
	    a1(I111,jstop,I33)=0.0;
	    c1(I111,jstop,I33)=0.0;  
	    d1(I111,jstop,I33)=u1(I111,jstop,I33,j);
	    b1(I111,jstop,I33)=1.0;
	  }
	  /******VOYONS****/
	  if (i==0)
	  {
	    if (smoothingMethod==3)
	    {
	      isharp=0;
	      for (i1=I111.getBase();i1<=I111.getBound();i1++)
		for (j1=I22.getBase();j1<=I22.getBound();j1++)
		  if (((u1(i1,j1+1,0,0)-u1(i1,j1,0,0))*
		       (u1(i1,j1-1,0,0)-u1(i1,j1,0,0))+
		       (u1(i1,j1+1,0,1)-u1(i1,j1,0,1))* 
		       (u1(i1,j1-1,0,1)-u1(i1,j1,0,1)))>10.*REAL_EPSILON)
		  {
		    isharp=1;
		    //printf("sharp at i1=%i\t j1=%i\t voyons=%g\n",i1,j1,
		    //(u1(i1,j1+1,0,0)-u1(i1,j1,0,0))*
		    //(u1(i1,j1-1,0,0)-u1(i1,j1,0,0))+
		    //(u1(i1,j1+1,0,1)-u1(i1,j1,0,1))*
		    //(u1(i1,j1-1,0,1)-u1(i1,j1,0,1)));
		    //printf("i1, j1+1 :: x=%g  <>  %g\t y=%g  <>  %g\n",u1(i1,j1+1,0,0), uprev[i](i1,j1+1,0,0), u1(i1,j1+1,0,1), uprev[i](i1,j1+1,0,1));
		    //printf("i1, j1 :: x=%g  <>  %g\t y=%g  <>  %g\n",u1(i1,j1,0,0), uprev[i](i1,j1,0,0),u1(i1,j1,0,1),uprev[i](i1,j1,0,1));
		    //printf("i1, j1-1 :: x=%g  <>  %g\t y=%g  <>  %g\n",u1(i1,j1-1,0,0),uprev[i](i1,j1-1,0,0), u1(i1,j1-1,0,1),uprev[i](i1,j1-1,0,1));
		    break;
		  }
	      if (isharp==1)
		for (i1=I111.getBase();i1<=I111.getBound();i1++)
		  for (j1=I22.getBase();j1<=I22.getBound();j1+=I22.getStride())
		  {
		    if (((uprev[i](i1,j1+1,0,0)-uprev[i](i1,j1,0,0))*
			 (uprev[i](i1,j1-1,0,0)-uprev[i](i1,j1,0,0))>0.)||
			((uprev[i](i1,j1+1,0,1)-uprev[i](i1,j1,0,1))* 
			 (uprev[i](i1,j1-1,0,1)-uprev[i](i1,j1,0,1))>0.)||
			(((utmp[i](i1,j1+1,0,0)-u1(i1,j1,0,0))*
			  (utmp[i](i1,j1-1,0,0)-u1(i1,j1,0,0)))+
			 ((utmp[i](i1,j1+1,0,1)-u1(i1,j1,0,1))* 
			  (utmp[i](i1,j1-1,0,1)-u1(i1,j1,0,1)))>0.))
		    {
		      GRIDBC[i0][i](i1,j1,0)=1;
		      a1(i1,j1,0)=0.0;
		      c1(i1,j1,0)=0.0;
		      b1(i1,j1,0)=1.0;
		      d1(i1,j1,0)=uprev[i](i1,j1,0,j);
		      if (j1<I22.getBound())
		      {
			GRIDBC[i0][i](i1,j1+1,0)=1;
			a1(i1,j1+1,0)=0.0;
			c1(i1,j1+1,0)=0.0;
			b1(i1,j1+1,0)=1.0;
			d1(i1,j1+1,0)=uprev[i](i1,j1+1,0,j);
		      }
		      if (j1>I22.getBase())
		      {
			GRIDBC[i0][i](i1,j1-1,0)=1;
			a1(i1,j1-1,0)=0.0;
			c1(i1,j1-1,0)=0.0;
			b1(i1,j1-1,0)=1.0;
			d1(i1,j1-1,0)=uprev[i](i1,j1-1,0,j);
		      }
		    }
		    else if (GRIDBC[i0][i](i1,j1,0)==1)
		    {
		      a1(i1,j1,0)=0.0;
		      c1(i1,j1,0)=0.0;
		      b1(i1,j1,0)=1.0;
		      d1(i1,j1,0)=uprev[i](i1,j1,0,j);
		    }
		  }
	      /****VOYONS****/
	    }
	    else 
	    {
	      where (GRIDBC[i0][i](I111,I22,I333)==1)
	      {
		a1(I111,I22,0)=0.0;
		c1(I111,I22,0)=0.0;
		b1(I111,I22,0)=1.0;
		d1(I111,I22,0)=uprev[i](I111,I22,0,j);
	      }
	    }
	  }
	  else 
	  {
	    for (i1=I111.getBase();i1<=I111.getBound();i1++)
	    {
	      for (j1=jstart;j1<=jstop;j1++)
		GRIDBC[i0][i](i1,j1,0)=GRIDBC[i0][i-1](2*i1,2*j1,0);

	      GRIDBC[i0][i](i1,-1,0)=GRIDBC[i0][i-1](2*i1,-1,0);
	      GRIDBC[i0][i](i1,jstop+1,0)=GRIDBC[i0][i-1](2*i1,I2[i-1].getBound()+1,0);
	    }

	    where (GRIDBC[i0][i](I111,I22,0)==1)
	    {
	      a1(I111,I22,0)=0.0;
	      c1(I111,I22,0)=0.0;
	      b1(I111,I22,0)=1.0;
	      d1(I111,I22,0)=uprev[i](I111,I22,0,j);
	    }
	  }
	  /****VOYONS****/
	}
      }

      if (userMap->getIsPeriodic(1)==Mapping::functionPeriodic)
      {
	jstart=I2[i].getBase(), jstop=I2[i].getBound();
	b1(Ic1,jstart,Ic3)=-2.0*coeff1(Ic1,jstart,Ic3)/(dx(0,i)*dx(0,i))-
	  2.0*coeff2(Ic1,jstart,Ic3)/(dx(1,i)*dx(1,i));
	a1(Ic1,jstart,Ic3)=coeff1(Ic1,jstart,Ic3)*(1.0/(dx(0,i)*dx(0,i))-
						   Source[i](Ic1,jstart,Ic3,0)/(2*dx(0,i)));
	c1(Ic1,jstart,Ic3)=coeff1(Ic1,jstart,Ic3)*(1.0/(dx(0,i)*dx(0,i))+
						   Source[i](Ic1,jstart,Ic3,0)/(2*dx(0,i)));
	d1(Ic1,jstart,Ic3)=RHS[i](Ic1,jstart,Ic3,j)+
	  coeff3(Ic1,jstart,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
				  u1(Ic1-1,jstart+1,Ic3,j)-
				  u1(Ic1+1,jstop-1,Ic3,j)+
				  u1(Ic1-1,jstop-1,Ic3,j))/
	  (4.0*dx(0,i)*dx(1,i))-
	  coeff2(Ic1,jstart,Ic3)*((u1(Ic1,jstart+1,Ic3,j)+
				   u1(Ic1,jstop-1,Ic3,j))/(dx(1,i)*dx(1,i))+
				  Source[i](Ic1,jstart,Ic3,1)*
				  (u1(Ic1,jstart+1,Ic3,j)-u1(Ic1,jstop-1,Ic3,j))/(2*dx(1,i)));
       
	b1(Ic1,jstop,Ic3)=-2.0*coeff1(Ic1,jstop,Ic3)/(dx(0,i)*dx(0,i))-
	  2.0*coeff2(Ic1,jstop,Ic3)/(dx(1,i)*dx(1,i));
	a1(Ic1,jstop,Ic3)=coeff1(Ic1,jstop,Ic3)*(1.0/(dx(0,i)*dx(0,i))-
						 Source[i](Ic1,jstop,Ic3,0)/(2*dx(0,i)));
	c1(Ic1,jstop,Ic3)=coeff1(Ic1,jstop,Ic3)*(1.0/(dx(0,i)*dx(0,i))+
						 Source[i](Ic1,jstop,Ic3,0)/(2*dx(0,i)));
	d1(Ic1,jstop,Ic3)=RHS[i](Ic1,jstop,Ic3,j)+
	  coeff3(Ic1,jstop,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
				 u1(Ic1-1,jstart+1,Ic3,j)-
				 u1(Ic1+1,jstop-1,Ic3,j)+
				 u1(Ic1-1,jstop-1,Ic3,j))/
	  (4.0*dx(0,i)*dx(1,i))-
	  coeff2(Ic1,jstop,Ic3)*((u1(Ic1,jstart+1,Ic3,j)+
				  u1(Ic1,jstop-1,Ic3,j))/(dx(1,i)*dx(1,i))+
				 Source[i](Ic1,jstop,Ic3,1)*
				 (u1(Ic1,jstart+1,Ic3,j)-u1(Ic1,jstop-1,Ic3,j))/(2*dx(1,i)));
      }
      tri.factor(a1,b1,c1,TridiagonalSolver::normal,axis1);
      tri.solve(d1,I11,I22,I33);
      u1(I11,I22,I33,j)=(1.0-omega)*u1(I11,I22,I33,j)+
	omega*d1(I11,I22,I33);
    }
    else if (userMap->getIsPeriodic(0) == Mapping::functionPeriodic)
    {
      realArray a11,b11,c11,d11;
      Index I111;

      I111=Range(I11.getBase(),I11.getBound()-1);
      a11.redim(I111,I22,I33), a11=0.0;
      b11.redim(I111,I22,I33), b11=0.0;
      c11.redim(I111,I22,I33), c11=0.0;
      d11.redim(I111,I22,I33), d11=0.0;

      b1(istart,Ic2,Ic3)=-2.0*coeff1(istart,Ic2,Ic3)/(dx(0,i)*dx(0,i))-
	2.0*coeff2(istart,Ic2,Ic3)/(dx(1,i)*dx(1,i));
      a1(istart,Ic2,Ic3)=coeff1(istart,Ic2,Ic3)*(1.0/(dx(0,i)*dx(0,i))-
						 Source[i](istart,Ic2,Ic3,0)/(2*dx(0,i)));
      c1(istart,Ic2,Ic3)=coeff1(istart,Ic2,Ic3)*(1.0/(dx(0,i)*dx(0,i))+
						 Source[i](istart,Ic2,Ic3,0)/(2*dx(0,i)));
      d1(istart,Ic2,Ic3)=RHS[i](istart,Ic2,Ic3,j)+
	coeff3(istart,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
				u1(istart+1,Ic2-1,Ic3,j)-
				u1(istop-1,Ic2+1,Ic3,j)+
				u1(istop-1,Ic2-1,Ic3,j))/
	(4.0*dx(0,i)*dx(1,i))-
	coeff2(istart,Ic2,Ic3)*((u1(istart,Ic2+1,Ic3,j)+
				 u1(istart,Ic2-1,Ic3,j))/(dx(1,i)*dx(1,i))+
				Source[i](istart,Ic2,Ic3,1)*
				(u1(istart,Ic2+1,Ic3,j)-u1(istart,Ic2-1,Ic3,j))/(2*dx(1,i)));

      a11(I111,I22,I33)=a1(I111,I22,I33);
      b11(I111,I22,I33)=b1(I111,I22,I33);
      c11(I111,I22,I33)=c1(I111,I22,I33);
      d11(I111,I22,I33)=d1(I111,I22,I33);

      /**** SHOULD NOT BE INCLUDED
	b1(istop,Ic2,Ic3)=-2.0*coeff1(istop,Ic2,Ic3)/(dx(0,i)*dx(0,i))-
	2.0*coeff2(istop,Ic2,Ic3)/(dx(1,i)*dx(1,i));
	a1(istop,Ic2,Ic3)=coeff1(istop,Ic2,Ic3)*(1.0/(dx(0,i)*dx(0,i))-
	Source[i](istop,Ic2,Ic3,0)/(2*dx(0,i)));
	c1(istop,Ic2,Ic3)=coeff1(istop,Ic2,Ic3)*(1.0/(dx(0,i)*dx(0,i))+
	Source[i](istop,Ic2,Ic3,0)/(2*dx(0,i)));
	d1(istop,Ic2,Ic3)=RHS[i](istop,Ic2,Ic3,j)+
	coeff3(istop,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
	u1(istart+1,Ic2-1,Ic3,j)-
	u1(istop-1,Ic2+1,Ic3,j)+
	u1(istop-1,Ic2-1,Ic3,j))/
	(4.0*dx(0,i)*dx(1,i))-
	coeff2(istop,Ic2,Ic3)*((u1(istop,Ic2+1,Ic3,j)+
	u1(istop,Ic2-1,Ic3,j))/(dx(1,i)*dx(1,i))+
	Source[i](istop,Ic2,Ic3,1)*
	(u1(istop,Ic2+1,Ic3,j)-u1(istop,Ic2-1,Ic3,j))/(2*dx(1,i)));
        SHOULD NOT BE INCLUDED *****/

      tri.factor(a11,b11,c11,TridiagonalSolver::periodic,axis1);
      tri.solve(d11,I111,I22,I33);
      u1(I111,I22,I33,j)=d11(I111,I22,I33);
      u1(istop,I22,I33,j)=u1(istart,I22,I33,j);
    }
    u1(istart,jstart,0,Range(0,1))=uprev[i](istart,jstart,0,Range(0,1));
    u1(istop,jstart,0,Range(0,1))=uprev[i](istop,jstart,0,Range(0,1));
    u1(istart,jstop,0,Range(0,1))=uprev[i](istart,jstop,0,Range(0,1));
    u1(istop,jstop,0,Range(0,1))=uprev[i](istop,jstop,0,Range(0,1));
    fflush(stdout);
  }
  else if (isweep==1)
  {
    jstart=I22.getBase(), jstop=I22.getBound();
    a1(Ic1,Ic2,Ic3)=coeff2(Ic1,Ic2,Ic3)*(1.0/(dx(1,i)*dx(1,i))-
					 Source[i](Ic1,Ic2,Ic3,1)/(2*dx(1,i)));
    c1(Ic1,Ic2,Ic3)=coeff2(Ic1,Ic2,Ic3)*(1.0/(dx(1,i)*dx(1,i))+
					 Source[i](Ic1,Ic2,Ic3,1)/(2*dx(1,i)));
    d1(Ic1,Ic2,Ic3)=RHS[i](Ic1,Ic2,Ic3,j)+
      coeff3(Ic1,Ic2,Ic3)*(u1(Ic1+1,Ic2+1,Ic3,j)-
			   u1(Ic1+1,Ic2-1,Ic3,j)-
			   u1(Ic1-1,Ic2+1,Ic3,j)+
			   u1(Ic1-1,Ic2-1,Ic3,j))/
      (4.0*dx(0,i)*dx(1,i))-
      coeff1(Ic1,Ic2,Ic3)*((u1(Ic1+1,Ic2,Ic3,j)+
			    u1(Ic1-1,Ic2,Ic3,j))/(dx(0,i)*dx(0,i))+
			   Source[i](Ic1,Ic2,Ic3,0)*
			   (u1(Ic1+1,Ic2,Ic3,j)-u1(Ic1-1,Ic2,Ic3,j))/(2*dx(0,i)));
    if (userMap->getIsPeriodic(1) != Mapping::functionPeriodic)
    {
      for (side=0;side<2;side++)
      {
	i0=2*axis+side;
	is[axis]=1-2*side;
	getBoundaryIndex(index,side,axis,I111,I222,I333);
	if ((gridBc(side,axis)==1)||(gridBc(side,axis)==3))
	{
	  b1(I11,I222,I33)=1.0;
	  d1(I11,I222,I33)=u1(I11,I222,I33,j);
	}
	else if (gridBc(side,axis)==2)
	{
	  Xc.redim(I11,I222,I333), Yc.redim(I11,I222,I333);
	  X2.redim(I11,I222,I333), Y2.redim(I11,I222,I333);
	  XY.redim(I11,I222,I333), normXc.redim(I11,I222,I333);

	  Xc(I11,I222,I333)=uprev[i](I11+1,I222,I33,0)-
	    uprev[i](I11-1,I222,I33,0);
	  Yc(I11,I222,I333)=uprev[i](I11+1,I222,I33,1)-
	    uprev[i](I11-1,I222,I33,1);
	  X2(I11,I222,I333)=Xc(I11,I222,I333)*Xc(I11,I222,I333);
	  Y2(I11,I222,I333)=Yc(I11,I222,I333)*Yc(I11,I222,I333);
	  XY(I11,I222,I333)=Xc(I11,I222,I333)*Yc(I11,I222,I333);
	  normXc(I11,I222,I333)=X2(I11,I222,I333)+Y2(I11,I222,I333);
  
	  if (side==0) a1(I11,I222,I33)= 0.0;
	  else if (side==1) c1(I11,I222,I33)=0.0;
	  b1(I11,I222,I33)= normXc(I11,I222,I33);
	  if (i==0)
	  {
	    if (j==0)
	    {
	      if (side==0) c1(I11,I222,I33)= -X2(I11,I222,I33);
	      else if (side==1) a1(I11,I222,I33)= -X2(I11,I222,I33);
	      d1(I11,I222,I33)=
		XY(I11,I222,I33)*u1(I11,I222+1,I33,1)-
		XY(I11,I222,I33)*uprev[i](I11-1,I222,I33,1)+
		Y2(I11,I222,I33)*uprev[i](I11-1,I222,I33,0);
	    }
	    else
	    {
	      if (side==0) c1(I11,I222,I33)= -Y2(I11,I222,I33);
	      else if (side==1) a1(I11,I222,I33)= -Y2(I11,I222,I33);
	      d1(I11,I222,I33)= 
		X2(I11,I222,I33)*uprev[i](I11-1,I222,I33,1)-
		XY(I11,I222,I33)*uprev[i](I11-1,I222,I33,0)+
		XY(I11,I222,I33)*u1(I11,I222+is[1],I33,0);
	    }
	  }
	  else
	  {
	    if (j==0)
	    {
	      if (side==0) c1(I11,I222,I33)= -X2(I11,I222,I33);
	      else if (side==1) a1(I11,I222,I33)= -X2(I11,I222,I33);
	      d1(I11,I222,I33)= 
		XY(I11,I222,I33)*u1(I11,I222+is[1],I33,1)-
		Xc(I11,I222,I33)*RHS[i](I11,I222,I33,0)-
		Yc(I11,I222,I33)*RHS[i](I11,I222,I33,1);
	    }
	    else
	    {
	      if (side==0) c1(I11,I222,I33)= -Y2(I11,I222,I33);
	      else if (side==1) a1(I11,I222,I33)= -Y2(I11,I222,I33);
	      d1(I11,I222,I33)=
		Xc(I11,I222,I33)*RHS[i](I11,I222,I33,1)+
		XY(I11,I222,I33)*u1(I11,I222+is[1],I33,0)-
		Yc(I11,I222,I33)*RHS[i](I11,I222,I33,0);
	    }
	  }
	  if (I11.getBase()==istart)
	  {
	    a1(istart,I222,I33)=0.0;
	    c1(istart,I222,I33)=0.0;  
	    d1(istart,I222,I33)=uprev[i](istart,I222,I33,j);
	    b1(istart,I222,I33)=1.0;
	  }
	  if (I11.getBound()==istop)
	  {
	    a1(istop,I222,I33)=0.0;
	    c1(istop,I222,I33)=0.0;  
	    d1(istop,I222,I33)=uprev[i](istop,I222,I33,j);
	    b1(istop,I222,I33)=1.0;
	  }

	  /******VOYONS****/
	  if (i==0)
	  {
	    if (smoothingMethod==3)
	    {
	      isharp=0;
	      for (j1=I222.getBase();j1<=I222.getBound();j1++)
		for (i1=I11.getBase();i1<=I11.getBound();i1++)
		  if (((u1(i1+1,j1,0,0)-u1(i1,j1,0,0))*
		       (u1(i1-1,j1,0,0)-u1(i1,j1,0,0))+
		       (u1(i1+1,j1,0,1)-u1(i1,j1,0,1))* 
		       (u1(i1-1,j1,0,1)-u1(i1,j1,0,1)))>10.*REAL_EPSILON)
		  {
		    isharp=1;
		    break;
		  }
	      if (isharp==1)
		for (j1=I222.getBase();j1<=I222.getBound();j1++)
		  for (i1=I11.getBase();i1<=I11.getBound();i1+=I11.getStride())
		  {
		    if (((uprev[i](i1+1,j1,0,0)-uprev[i](i1,j1,0,0))*
			 (uprev[i](i1-1,j1,0,0)-uprev[i](i1,j1,0,0))>0.)||
			((uprev[i](i1+1,j1,0,1)-uprev[i](i1,j1,0,1))* 
			 (uprev[i](i1-1,j1,0,1)-uprev[i](i1,j1,0,1))>0.)||
			(((utmp[i](i1+1,j1,0,0)-u1(i1,j1,0,0))*
			  (utmp[i](i1-1,j1,0,0)-u1(i1,j1,0,0)))+
			 ((utmp[i](i1+1,j1,0,1)-u1(i1,j1,0,1))* 
			  (utmp[i](i1-1,j1,0,1)-u1(i1,j1,0,1)))>0.))
		    {
		      GRIDBC[i0][i](i1,j1,0)=1;
		      a1(i1,j1,0)=0.0;
		      c1(i1,j1,0)=0.0;
		      b1(i1,j1,0)=1.0;
		      d1(i1,j1,0)=uprev[i](i1,j1,0,j);
		      if (i1<I11.getBound())
		      {
			GRIDBC[i0][i](i1+1,j1,0)=1;
			a1(i1+1,j1,0)=0.0;
			c1(i1+1,j1,0)=0.0;
			b1(i1+1,j1,0)=1.0;
			d1(i1+1,j1,0)=uprev[i](i1+1,j1,0,j);
		      }
		      if (i1>I11.getBase())
		      {
			GRIDBC[i0][i](i1-1,j1,0)=1;
			a1(i1-1,j1,0)=0.0;
			c1(i1-1,j1,0)=0.0;
			b1(i1-1,j1,0)=1.0;
			d1(i1-1,j1,0)=uprev[i](i1-1,j1,0,j);
		      }
		    }
		    else if (GRIDBC[i0][i](i1,j1,0)==1)
		    {
		      a1(i1,j1,0)=0.0;
		      c1(i1,j1,0)=0.0;
		      b1(i1,j1,0)=1.0;
		      d1(i1,j1,0)=uprev[i](i1,j1,0,j);
		    }
		  }
	      /****VOYONS****/
	    }
	    else 
	    {
	      where (GRIDBC[i0][i](I11,I222,I333)==1)
	      {
		a1(I11,jstart,0)=0.0;
		c1(I11,jstart,0)=0.0;
		b1(I11,jstart,0)=1.0;
		d1(I11,jstart,0)=uprev[i](I11,jstart,0,j);
	      }
	    }
	  }
	  else 
	  {
	    for (j1=I222.getBase();j1<=I222.getBound();j1++)
	    {
	      for (i1=istart;i1<=istop;i1++)
		GRIDBC[i0][i](i1,j1,0)=GRIDBC[i0][i-1](2*i1, 2*j1, 0);

	      GRIDBC[i0][i](-1,j1,0)=GRIDBC[i0][i-1](-1,2*j1,0);
	      GRIDBC[i0][i](istop+1,j1,0)=
		GRIDBC[i0][i-1](I1[i-1].getBound()+1,2*j1,0);
	    }

	    where (GRIDBC[i0][i](I11,I222,0)==1)
	    {
	      a1(I11,I222,0)=0.0;
	      c1(I11,I222,0)=0.0;
	      b1(I11,I222,0)=1.0;
	      d1(I11,I222,0)=uprev[i](I11,I222,0,j);
	    }
	  }
	}
      }

      if (userMap->getIsPeriodic(0)==Mapping::functionPeriodic)
      {
	istart=I1[i].getBase(), istop=I1[i].getBound();
	b1(istart,Ic2,Ic3)=-2.0*coeff1(istart,Ic2,Ic3)/(dx(0,i)*dx(0,i))-
	  2.0*coeff2(istart,Ic2,Ic3)/(dx(1,i)*dx(1,i));
	a1(istart,Ic2,Ic3)=coeff2(istart,Ic2,Ic3)*(1.0/(dx(1,i)*dx(1,i))-
						   Source[i](istart,Ic2,Ic3,1)/(2*dx(1,i)));
	c1(istart,Ic2,Ic3)=coeff2(istart,Ic2,Ic3)*(1.0/(dx(1,i)*dx(1,i))+
						   Source[i](istart,Ic2,Ic3,1)/(2*dx(1,i)));
	d1(istart,Ic2,Ic3)=RHS[i](istart,Ic2,Ic3,j)+
	  coeff3(istart,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
				  u1(istart+1,Ic2-1,Ic3,j)-
				  u1(istop-1,Ic2+1,Ic3,j)+
				  u1(istop-1,Ic2-1,Ic3,j))/
	  (4.0*dx(0,i)*dx(1,i))-
	  coeff1(istart,Ic2,Ic3)*((u1(istart+1,Ic2,Ic3,j)+
				   u1(istop-1,Ic2,Ic3,j))/(dx(0,i)*dx(0,i))+
				  Source[i](istart,Ic2,Ic3,0)*
				  (u1(istart+1,Ic2,Ic3,j)-u1(istop-1,Ic2,Ic3,j))/(2*dx(0,i)));

	b1(istop,Ic2,Ic3)=-2.0*coeff1(istop,Ic2,Ic3)/(dx(0,i)*dx(0,i))-
	  2.0*coeff2(istop,Ic2,Ic3)/(dx(1,i)*dx(1,i));
	a1(istop,Ic2,Ic3)=coeff2(istop,Ic2,Ic3)*(1.0/(dx(1,i)*dx(1,i))-
						 Source[i](istop,Ic2,Ic3,1)/(2*dx(1,i)));
	c1(istop,Ic2,Ic3)=coeff2(istop,Ic2,Ic3)*(1.0/(dx(1,i)*dx(1,i))+
						 Source[i](istop,Ic2,Ic3,1)/(2*dx(1,i)));
	d1(istop,Ic2,Ic3)=RHS[i](istop,Ic2,Ic3,j)+
	  coeff3(istop,Ic2,Ic3)*(u1(istart+1,Ic2+1,Ic3,j)-
				 u1(istart+1,Ic2-1,Ic3,j)-
				 u1(istop-1,Ic2+1,Ic3,j)+
				 u1(istop-1,Ic2-1,Ic3,j))/
	  (4.0*dx(0,i)*dx(1,i))-
	  coeff1(istop,Ic2,Ic3)*((u1(istart+1,Ic2,Ic3,j)+
				  u1(istop-1,Ic2,Ic3,j))/(dx(0,i)*dx(0,i))+
				 Source[i](istop,Ic2,Ic3,0)*
				 (u1(istart+1,Ic2,Ic3,j)-u1(istop-1,Ic2,Ic3,j))/(2*dx(0,i)));
      }

      tri.factor(a1,b1,c1,TridiagonalSolver::normal,axis2);
      tri.solve(d1,I11,I22,I33);
      u1(I11,I22,I33,j)=(1.0-omega)*u1(I11,I22,I33,j)+
	omega*d1(I11,I22,I33);
    }
    else if (userMap->getIsPeriodic(1) == Mapping::functionPeriodic)
    {
      realArray a11,b11,c11,d11;
      Index I222;

      I222=Range(I22.getBase(),I22.getBound()-1);
      a11.redim(I11,I222,I33), a11=0.0;
      b11.redim(I11,I222,I33), b11=0.0;
      c11.redim(I11,I222,I33), c11=0.0;
      d11.redim(I11,I222,I33), d11=0.0;

      b1(Ic1,jstart,Ic3)=-2.0*coeff1(Ic1,jstart,Ic3)/(dx(0,i)*dx(0,i))-
	2.0*coeff2(Ic1,jstart,Ic3)/(dx(1,i)*dx(1,i));
      a1(Ic1,jstart,Ic3)=coeff2(Ic1,jstart,Ic3)*(1.0/(dx(1,i)*dx(1,i))-
						 Source[i](Ic1,jstart,Ic3,1)/(2*dx(1,i)));
      c1(Ic1,jstart,Ic3)=coeff2(Ic1,jstart,Ic3)*(1.0/(dx(1,i)*dx(1,i))+
						 Source[i](Ic1,jstart,Ic3,1)/(2*dx(1,i)));
      d1(Ic1,jstart,Ic3)=RHS[i](Ic1,jstart,Ic3,j)+
	coeff3(Ic1,jstart,Ic3)*(u1(Ic1+1,jstart+1,Ic3,j)-
				u1(Ic1+1,jstop-1,Ic3,j)-
				u1(Ic1-1,jstart+1,Ic3,j)+
				u1(Ic1-1,jstop-1,Ic3,j))/
	(4.0*dx(0,i)*dx(1,i))-
	coeff1(Ic1,jstart,Ic3)*((u1(Ic1+1,jstart,Ic3,j)+
				 u1(Ic1-1,jstart,Ic3,j))/(dx(0,i)*dx(0,i))+
				Source[i](Ic1,jstart,Ic3,0)*
				(u1(Ic1+1,jstart,Ic3,j)-u1(Ic1-1,jstart,Ic3,j))/(2*dx(0,i)));


      a11(I11,I222,I33)=a1(I11,I222,I33);
      b11(I11,I222,I33)=b1(I11,I222,I33);
      c11(I11,I222,I33)=c1(I11,I222,I33);
      d11(I11,I222,I33)=d1(I11,I222,I33);


      tri.factor(a11,b11,c11,TridiagonalSolver::periodic,axis2);
      tri.solve(d11,I11,I222,I33);
      u1(I11,I222,I33,j)=(1.0-omega)*u1(I11,I222,I33,j)+
	omega*d1(I11,I222,I33);
      u1(I11,jstop,I33,j)=u1(I11,jstart,I33,j);
    }
    u1(istart,jstart,0,Range(0,1))=uprev[i](istart,jstart,0,Range(0,1));
    u1(istop,jstart,0,Range(0,1))=uprev[i](istop,jstart,0,Range(0,1));
    u1(istart,jstop,0,Range(0,1))=uprev[i](istart,jstop,0,Range(0,1));
    u1(istop,jstop,0,Range(0,1))=uprev[i](istop,jstop,0,Range(0,1));
    fflush(stdout);
  }

    /*****VOYONS
    printf("\n variable=%i\t level=%i\n\n", j,i);
    //if (i==1){
     for (j1=jstart;j1<=jstop;j1++)
     for (i1=istart;i1<=istop;i1++)
      printf("After i=%i\t j=%i\t x=%g\t y=%g\t xp=%g\t yp=%g\n",i1,j1,u1(i1,j1,0,0),u1(i1,j1,0,1),uprev[i](i1,j1,0,0),uprev[i](i1,j1,0,1));
    //}
    VOYONS***/
    if ((j==1)&&(iter<3)) applyOrthogonalBoundaries(I1[i],I2[i],I3[i],1,i,u[i]);
}


void multigrid::
getSource(int i, int ichange)
// ===============================================================================
//  Computes source terms when the simultaneous solver is not used.
// ===============================================================================
{
  realArray IJKSrc(I1[i],I2[i],I3[i],ndimension);
  realArray ptemp(I1[i],I2[i],I3[i],3), ptemp2norm(I1[i],I2[i],I3[i]);
  realArray Ireal(I1[i]);
  int i11,j11,j,k,ipmax, jpmax,iqmax, jqmax,nperiod;
  int i0, i1, i2, j0, j1, j2, k0, k1, k2, np,isharp;
  real ptemp1,qtemp1;

  i0=I1[i].getBase(), i1=I1[i].getBound(), i2=I1[i].getStride();
  j0=I2[i].getBase(), j1=I2[i].getBound(), j2=I2[i].getStride();
  k0=I3[i].getBase(), k1=I3[i].getBound(), k2=I3[i].getStride();
    
  IJKSrc=0.0, ptemp=0.0, Ireal=0.0;
  Ireal.seqAdd(real(i0),real(i2)); 
  for (j=j0; j<=j1; j += j2) 
    IJKSrc(I1[i],j,k0,0)=Ireal(I1[i]);
  for (j=k0+k2; j<=k1; j += k2)
    IJKSrc(I1[i],I2[i],j,0)=IJKSrc(I1[i],I2[i],k0,0);
    
  if (ndimension>1)
  {
    Ireal.redim(1,I2[i]);
    Ireal.seqAdd(real(j0),real(j2));
    for (j=i0; j<=i1; j += i2)
      IJKSrc(j,I2[i],k0,1)=Ireal(0,I2[i]);
    for (j=k0+k2; j<=k1; j += k2)
      IJKSrc(I1[i],I2[i],j,1)=IJKSrc(I1[i],I2[i],k0,1);
    if (ndimension>2)
    {
      Ireal.redim(1,1,I3[i]);
      Ireal.seqAdd(real(k0),real(k2));
      for (j=i0; j<=i1; j += i2)
	IJKSrc(j,j0,I3[i],2)=Ireal(0,0,I3[i]);
      for (j=j0+j2;j<=j1; j += j2)
	IJKSrc(I1[i],j,I3[i],2)=IJKSrc(I1[i],j0,I3[i],2);
    }
  }

  Source[i]=0.0;

  for (j=0; j<numberOfPointattractions;j++)
  {
    //First find P
    if ((gridBc(0,0)==-1)||(gridBc(1,0)==-1)) nperiod=numberOfPeriods;
    else nperiod=0;

    for (np=-nperiod;np<=nperiod;np++)
    {
      ptemp2norm=0.0;
      // Compute \xi-\xi_i and eventually \eta - \eta_j and \zeta-\zeta_k
      ptemp(I1[i],I2[i],I3[i],0)=(IJKSrc(I1[i],I2[i],I3[i],0)-
				  (PointAttraction[j][0]-np*(dim[0]-1.))/
				  pow(2.0,real(i)))/
	((dim[0]-1.0)/pow(2.0,real(i)));
      ptemp(I1[i],I2[i],I3[i],1)=(IJKSrc(I1[i],I2[i],I3[i],1)-
				  PointAttraction[j][1]/pow(2.0,real(i)))/
	((dim[1]-1.0)/pow(2.0,real(i)));

      for (k=0;k<ndimension;k++) 
        ptemp2norm(I1[i],I2[i],I3[i]) += ptemp(I1[i],I2[i],I3[i],k)*
	  ptemp(I1[i],I2[i],I3[i],k);

      ptemp2norm = sqrt(ptemp2norm);

      Source[i](I1[i],I2[i],I3[i],0) +=
	-APointcoeff[j]*SignOf(ptemp(I1[i],I2[i],I3[i],0))*
	exp(-CPointcoeff[j]*ptemp2norm);
    }

    //Then find Q
    if ((gridBc(0,1)==-1)||(gridBc(1,1)==-1)) nperiod=numberOfPeriods;
    else nperiod=0;

    for (np=-nperiod;np<=nperiod;np++)
    {
      ptemp2norm=0.0;
      // Compute \xi-\xi_i and eventually \eta - \eta_j and \zeta-\zeta_k
      ptemp(I1[i],I2[i],I3[i],1)=(IJKSrc(I1[i],I2[i],I3[i],1)-
				  (PointAttraction[j][1]-np*(dim[1]-1.))/
				  pow(2.0,real(i)))/
	((dim[1]-1.0)/pow(2.0,real(i)));
      ptemp(I1[i],I2[i],I3[i],0)=(IJKSrc(I1[i],I2[i],I3[i],0)-
				  PointAttraction[j][0]/pow(2.0,real(i)))/
	((dim[0]-1.0)/pow(2.0,real(i)));

      for (k=0;k<ndimension;k++) 
        ptemp2norm(I1[i],I2[i],I3[i]) += ptemp(I1[i],I2[i],I3[i],k)*
	  ptemp(I1[i],I2[i],I3[i],k);

      ptemp2norm = sqrt(ptemp2norm);

      Source[i](I1[i],I2[i],I3[i],1) +=
	-APointcoeff[j]*SignOf(ptemp(I1[i],I2[i],I3[i],1))*
	exp(-CPointcoeff[j]*ptemp2norm);
    }
  }
  if (ndimension==2)
  {
    if ((gridBc(0,0)==-1)||(gridBc(1,0)==-1)) nperiod=numberOfPeriods;
    else nperiod=0.0;
    for (k=0;k<numberOfIlineattractions;k++)
    {
      for (np=-nperiod;np<=nperiod;np++)
      {
	ptemp(I1[i],I2[i],I3[i],0)=(IJKSrc(I1[i],I2[i],I3[i],0)-
				    (ILineAttraction[k]-np*(dim[0]-1))/
				    pow(2.0,real(i)))/
	  ((dim[0]-1.0)/pow(2.0,real(i)));
	Source[i](I1[i],I2[i],I3[i],0) += -AIlinecoeff[k]*
	  SignOf(ptemp(I1[i],I2[i],I3[i],0))*
	  exp(-CIlinecoeff[k]*fabs(ptemp(I1[i],I2[i],I3[i],0)));
      }
    }

    if ((gridBc(0,1)==-1)||(gridBc(1,1)==-1)) nperiod=numberOfPeriods;
    else nperiod=0;
    for (k=0;k<numberOfJlineattractions;k++)
    {
      for (np=-nperiod;np<=nperiod;np++)
      {
	ptemp(I1[i],I2[i],I3[i],1)=(IJKSrc(I1[i],I2[i],I3[i],1)-
				    (JLineAttraction[k]-np*(dim[1]-1.))/
				    pow(2.0,real(i)))/
	  ((dim[1]-1.0)/pow(2.0,real(i)));
	Source[i](I1[i],I2[i],I3[i],1) += -AJlinecoeff[k]*
	  SignOf(ptemp(I1[i],I2[i],I3[i],1))*
	  exp(-CJlinecoeff[k]*fabs(ptemp(I1[i],I2[i],I3[i],1)));
      }
    }
  } 

  if ((gridBc(0,0)==3)||(gridBc(0,1)==3)||(gridBc(1,0)==3)||
      (gridBc(1,1)==3))
  {
    if (gridBc(0,0)==3)
    {
      CoeffInterp[0][i](I1[i],I2[i],I3[i]) = 
	((real(i1)-IJKSrc(I1[i],I2[i],I3[i],0))/real(i1))*
	exp(-lambda*IJKSrc(I1[i],I2[i],I3[i],0)/real(i1));
    }
    if (gridBc(0,1)==3)
    {
      CoeffInterp[2][i](I1[i],I2[i],I3[i])=
	((real(j1)-IJKSrc(I1[i],I2[i],I3[i],1))/real(j1))*
	exp(-lambda*IJKSrc(I1[i],I2[i],I3[i],1)/real(j1));
    }
    if (gridBc(1,0)==3)
    {
      CoeffInterp[1][i](I1[i],I2[i],I3[i]) = 
	(IJKSrc(I1[i],I2[i],I3[i],0)/real(i1))*
	exp(-lambda*(real(i1)-IJKSrc(I1[i],I2[i],I3[i],0))/real(i1));
    }
    if (gridBc(1,1)==3)
    {
      CoeffInterp[3][i](I1[i],I2[i],I3[i]) = 
	(IJKSrc(I1[i],I2[i],I3[i],1)/real(j1))*
	exp(-lambda*(real(j1)-IJKSrc(I1[i],I2[i],I3[i],1))/real(j1));
    }
    if (useBlockTridiag==0)
    {
      realArray Xc, Xe, Xcc, Xee, normXc, normXe, Sign;
      realArray ptmp;
      static realArray ptmp0[4];
      intArray index(2,3);
      Index I111,I222,I333;

      index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
      index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
      index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
      int is[3], icdif[3], axis,side;
      Index Rtmp=Range(0,1);
      for (axis=0;axis<=1;axis++)
	for (side=0;side<=1;side++)
	{
	  is[0]=0, is[1]=0, is[2]=0;
	  is[axis]=1-2*side;
	  if (axis==0) icdif[0]=0, icdif[1]=1;
	  if (axis==1) icdif[0]=1, icdif[1]=0;
	  getBoundaryIndex(index,side,axis,I111,I222,I333);
	  // omega0=1.0; For under relaxed jacobi
          if (gridBc(side,axis)==3)
	  {
            Xc.redim(I111,I222,I333,ndimension);
	    Xe.redim(I111,I222,I333,ndimension);
            normXc.redim(I111,I222,I333);
	    normXe.redim(I111,I222,I333);
            Xcc.redim(I111,I222,I333,ndimension);
	    Xee.redim(I111,I222,I333,ndimension);
            Sign.redim(I111,I222,I333);
            ptmp.redim(I1[i],I2[i],I3[i],ndimension);
	   
	    /**** For underelaxed jacobi ****/
	    if (iter<=0)
	    {
	      ptmp0[i].redim(I1[i],I2[i],I3[i],ndimension);
	      ptmp0[i]=1.0;
	      ptmp=ptmp0[i];
	    }
            else if (ichange != 0)
	    {
	      /***** For underelaxed jacobi *********/ 
	      Xc=0., Xe=0., Xcc=0., Xee=0.,Sign=0., ptmp=0.;
              if (axis==1)
	      {
	        Xc(I111,I222,I333,Rtmp)=(u[i](I111+1,I222,I333,Rtmp)-
					 u[i](I111-1,I222,I333,Rtmp))/(2.*dx(0,i));
	        Xe(I111,I222,I333,Rtmp)=
		  real(is[1])*(u[i](I111,I222+is[1],I333,Rtmp)-
			       u[i](I111,I222,I333,Rtmp))/dx(1,i);
	        Xcc(I111,I222,I333,Rtmp)=(u[i](I111+1,I222,I333,Rtmp)-
					  2.0*u[i](I111,I222,I333,Rtmp)+
					  u[i](I111-1,I222,I333,Rtmp))/(dx(0,i)*dx(0,i));

                normXc(I111,I222,I333)=sqrt(Xc(I111,I222,I333,0)*
					    Xc(I111,I222,I333,0)+
					    Xc(I111,I222,I333,1)*
					    Xc(I111,I222,I333,1));
		Sign=SignOf(Xe(I111,I222,I333,0)*Xc(I111,I222,I333,1)-
			    Xe(I111,I222,I333,1)*Xc(I111,I222,I333,0));

		Xe(I111,I222,I333,0)=dB(side,axis)*Sign(I111,I222,I333)*
		  Xc(I111,I222,I333,1)/normXc(I111,I222,I333);
		Xe(I111,I222,I333,1)=-dB(side,axis)*Sign(I111,I222,I333)*
		  Xc(I111,I222,I333,0)/normXc(I111,I222,I333);

		Xee(I111,I222,I333,Rtmp)=(-7.*u[i](I111,I222,I333,Rtmp) + 
					  8.*u[i](I111,I222+is[1],I333,Rtmp) -
					  u[i](I111,I222+2*is[1],I333,Rtmp)-
					  real(is[1])*6.0*dx(1,i)*
					  Xe(I111,I222,I333,Rtmp))/(2.*dx(1,i)*dx(1,i));
		/********
		  ((Xc(I1[i],0)*Xcc(I1[i],0)+Xc(I1[i],1)*Xcc(I1[i],1))/
		  (normXc(I1[i])*normXc(I1[i]))).display("Le premier");
		  ((Xc(I1[i],0)*Xee(I1[i],0)+Xc(I1[i],1)*Xee(I1[i],1))/
		  (dB(1,0)*dB(1,0))).display("Le deuxieme");
		  ((Xe(I1[i],0)*Xcc(I1[i],0)+Xe(I1[i],1)*Xcc(I1[i],1))/
		  (normXc(I1[i])*normXc(I1[i]))).display("Le troisieme");
		  ((Xe(I1[i],0)*Xee(I1[i],0)+Xe(I1[i],1)*Xee(I1[i],1))/
		  (dB(1,0)*dB(1,0))).display("Le quatrieme");
		  ********/

		ptmp(I111,I222,I333,0)=
		  -(Xc(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
		    Xc(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		  (normXc(I111,I222,I333)*normXc(I111,I222,I333))-
		  (Xc(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		   Xc(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
		  (dB(side,axis)*dB(side,axis));
		ptmp(I111,I222,I333,1)=
		  -(Xe(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
		    Xe(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		  (normXc(I111,I222,I333)*normXc(I111,I222,I333))-
		  (Xe(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		   Xe(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
		  (dB(side,axis)*dB(side,axis));
              }
	      else if (axis==0)
	      {
		Xe(I111,I222,I333,Rtmp)=(u[i](I111, I222+1,I333,Rtmp)-
					 u[i](I111,I222-1,I333,Rtmp))/(2.*dx(1,i));
		Xc(I111,I222,I333,Rtmp)=
		  real(is[0])*(u[i](I111+is[0],I222,I333,Rtmp)-
			       u[i](I111,I222,I333,Rtmp))/dx(0,i);
	        Xee(I111,I222,I333,Rtmp)=(u[i](I111,I222+1,I333,Rtmp)-
					  2.0*u[i](I111,I222,I333,Rtmp)+
					  u[i](I111,I222-1,I333,Rtmp))/(dx(1,i)*dx(1,i));
		//Xc.display("C'est Xc");
		//Xcc.display("C'est Xcc");

		normXe(I111,I222,I333)=sqrt(Xe(I111,I222,I333,0)*
					    Xe(I111,I222,I333,0)+
					    Xe(I111,I222,I333,1)*
					    Xe(I111,I222,I333,1));
		Sign=SignOf(Xc(I111,I222,I333,0)*Xe(I111,I222,I333,1)-
			    Xc(I111,I222,I333,1)*Xe(I111,I222,I333,0));

		Xc(I111,I222,I333,0)=dB(side,axis)*Sign(I111,I222,I333)*
		  Xe(I111,I222,I333,1)/normXe(I111,I222,I333);
		Xc(I111,I222,I333,1)=-dB(side,axis)*Sign(I111,I222,I333)*
		  Xe(I111,I222,I333,0)/normXe(I111,I222,I333);
              
		Xcc(I111,I222,I333,Rtmp)=(-7.*u[i](I111,I222,I333,Rtmp) + 
					  8.*u[i](I111+is[0],I222,I333,Rtmp) -
					  u[i](I111+2*is[0],I222,I333,Rtmp)-
					  real(is[0])*6.0*dx(0,i)*Xc(I111,I222,I333,Rtmp))/
		  (2.*dx(0,i)*dx(0,i));
             
	   /********
	   ((Xc(I1[i],0)*Xcc(I1[i],0)+Xc(I1[i],1)*Xcc(I1[i],1))/
	    (normXc(I1[i])*normXc(I1[i]))).display("Le premier");
           ((Xc(I1[i],0)*Xee(I1[i],0)+Xc(I1[i],1)*Xee(I1[i],1))/
	    (dB(1,0)*dB(1,0))).display("Le deuxieme");
	   ((Xe(I1[i],0)*Xcc(I1[i],0)+Xe(I1[i],1)*Xcc(I1[i],1))/
	    (normXc(I1[i])*normXc(I1[i]))).display("Le troisieme");
           ((Xe(I1[i],0)*Xee(I1[i],0)+Xe(I1[i],1)*Xee(I1[i],1))/
	    (dB(1,0)*dB(1,0))).display("Le quatrieme");
	    ********/

		ptmp(I111,I222,I333,0)=
		  -(Xc(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
		    Xc(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		  (dB(side,axis)*dB(side,axis))-
                  (Xc(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		   Xc(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
		  (normXe(I111,I222,I333)*normXe(I111,I222,I333));
		ptmp(I111,I222,I333,1)=
		  -(Xe(I111,I222,I333,0)*Xcc(I111,I222,I333,0)+
		    Xe(I111,I222,I333,1)*Xcc(I111,I222,I333,1))/
		  (dB(side,axis)*dB(side,axis))-
                  (Xe(I111,I222,I333,0)*Xee(I111,I222,I333,0)+
		   Xe(I111,I222,I333,1)*Xee(I111,I222,I333,1))/
		  (normXe(I111,I222,I333)*normXe(I111,I222,I333));
	      }
           
	      for (j11=I222.getBase();j11<=I222.getBound();j11++)
		for (i11=I111.getBase();i11<=I111.getBound();i11++)
		  if (((u[i](i11+icdif[0],j11+icdif[1],0,0)-u[i](i11,j11,0,0))*
		       (u[i](i11-icdif[0],j11-icdif[1],0,0)-u[i](i11,j11,0,0))+
		       (u[i](i11+icdif[0],j11,0,1)-u[i](i11,j11,0,1))* 
		       (u[i](i11-icdif[0],j11,0,1)-u[i](i11,j11,0,1)))>0.000)
		  {
		    if (((i11<i1)&&(i11>i0))||((j11<j1)&&(j11>j0)))
		    {
		      ptmp(i11,j11,0,0)=
			0.5*(ptmp(i11+icdif[0],j11+icdif[1],0,0)+
			     ptmp(i11-icdif[0],j11-icdif[1],0,0));
		      ptmp(i11,j11,0,1)=
			0.5*(ptmp(i11+icdif[0],j11+icdif[1],0,1)+
			     ptmp(i11-icdif[0],j11-icdif[1],0,1));
		    }
		    else if (((i11==i0)&&(axis==1))||((j11==j0)&&(axis==0)))
		    {
		      if (axis==1)
		      {
			ptmp(i11,j11,0,0)=
			  0.5*(ptmp(i11+2,j11,0,0)+
			       ptmp(i1-2,j11,0,0));
			ptmp(i11+1,j11,0,0)=ptmp(i11,j11,0,0);
			ptmp(i11,j11,0,1)=0.5*(ptmp(i11+2,j11,0,1)+
					       ptmp(i1-2,j11,0,1));
			ptmp(i11+1,j11,0,1)=ptmp(i11,j11,0,1);
		      }
		      if (axis==0)
		      {
			ptmp(i11,j11,0,0)=
			  0.5*(ptmp(i11,j11+2,0,0)+
			       ptmp(i11,j1-2,0,0));
			ptmp(i11,j11+1,0,0)=ptmp(i11,j11,0,0);
			ptmp(i11,j11,0,1)=0.5*(ptmp(i11,j11+2,0,1)+
					       ptmp(i11,j1-2,0,1));
			ptmp(i11,j11+1,0,1)=ptmp(i11,j11,0,1);
		      }
		    }
		    else if (((i11==i1)&&(axis==1))||((j11==j1)&&(axis==0)))
		    {
		      if (axis==1)
		      {
			ptmp(i11,j11,0,0)=0.5*(ptmp(i11-2,j11,0,0)+
					       ptmp(i0+2,j11,0,0));
			ptmp(i11-1,j11,0,0)=ptmp(i11,j11,0,0);
			ptmp(i11,j11,0,1)=0.5*(ptmp(i11-2,j11,0,1)+
					       ptmp(i0+2,j11,0,1));
			ptmp(i11-1,j11,0,1)=ptmp(i11,j11,0,1);
		      }
		      if (axis==0)
		      {
			ptmp(i11,j11,0,0)=0.5*(ptmp(i11,j11-2,0,0)+
					       ptmp(i11,j0+2,0,0));
			ptmp(i11,j11-1,0,0)=ptmp(i11,j11,0,0);
			ptmp(i11,j11,0,1)=0.5*(ptmp(i11,j11-2,0,1)+
					       ptmp(i11,j0+2,0,1));
			ptmp(i11,j11-1,0,1)=ptmp(i11,j11,0,1);
		      }
		    }
		  }

	      if (axis==1)
	      {
		int j00=I222.getBase();
		for (j=j0; j<=j1; j+=j2)
		  ptmp(I1[i],j,I3[i],Rtmp)=ptmp(I1[i],j00,I3[i],Rtmp);
	      }
	      else if (axis==0)
	      {
		int i00=I111.getBase();
		for (j=i0; j<=i1; j+=i2)
		  ptmp(j,I2[i],I3[i],Rtmp)=ptmp(i00,I2[i],I3[i],Rtmp);
	      }

	      //if (iter==300) TEST_RESIDUAL=1;
	      ptmp0[i](I1[i],I2[i],I3[i],Rtmp) =
		(1-omega1)*ptmp0[i](I1[i],I2[i],I3[i],Rtmp)+
		omega1*ptmp(I1[i],I2[i],I3[i],Rtmp);
	    }

	    for (j=0;j<ndimension;j++)
	      Source[i](I1[i],I2[i],I3[i],j) += ptmp0[i](I1[i],I2[i],I3[i],j)*
		CoeffInterp[2*axis+side][i](I1[i],I2[i],I3[i]);
         
	  }
	}
    }
  }
  //Source[0](I1[0],I2[0],I3[0],0).display("Source0");
  //Source[0](I1[0],I2[0],I3[0],1).display("Source1");
  //for (int j11=0;j11<=I2[i].getBound();j11++)
  //for (i11=0;i11<=I1[i].getBound();i11++)
  //printf("i=%i\t j=%i\t P=%g\t Q=%g\n",i11,j11,Source[i](i11,j11,0,0),
  //Source[i](i11,j11,0,1));
}




void multigrid::
applyOrthogonalBoundaries(Index I11, Index I22, Index I33, int j, 
			  int i, realArray &u1)
// =================================================================================
// /Description:
//   Applies the "slip" orthogonal boundary conditions when the line solvers are not used.
//
//  Adjust the points on the boundary to make the grid orthogonal al the boundary.
//      $ \xv_r \cdot \xv_s = 0$.
//  
// =================================================================================
{
  real omega2=1.0;
  int istart,istop,jstart,jstop,isharp=0, i1, j1;
  realArray Xc_e,Yc_e,normXc_e,X2,Y2,XY,Xe,Ye,normXe;
  Index I111, I222, I333;
  intArray index(2,3);
  int is[3]=  {0,0,0}, icdif[3]={0,0,0};
  int axis, side,i0;

  jstart=I2[i].getBase();
  jstop=I2[i].getBound();
  istart=I1[i].getBase();
  istop=I1[i].getBound();
  index(0,0)=I1[i].getBase(), index(1,0)=I1[i].getBound();
  index(0,1)=I2[i].getBase(), index(1,1)=I2[i].getBound();
  index(0,2)=I3[i].getBase(), index(1,2)=I3[i].getBound();
  for (axis=0;axis<=1;axis++)
    for (side=0;side<=1;side++)
    {
      is[0]=0, is[1]=0, is[2]=0;
      if (axis==0) icdif[0]=0, icdif[1]=1, icdif[2]=0;
      else if (axis==1) icdif[0]=1,icdif[1]=0, icdif[2]=0;
      is[axis]=1-2*side;
      i0=2*axis+side;
      getBoundaryIndex(index,side,axis,I111,I222,I333);
      if (I111.getBase() != I111.getBound()) I111=I11;
      if (I222.getBase() != I222.getBound()) I222=I22;
      if (gridBc(side,axis)==2)
      {
	Xc_e.redim(I111,I222,I333), X2.redim(I111,I222,I333);
	Yc_e.redim(I111,I222,I333), Y2.redim(I111,I222,I333);
	normXc_e.redim(I111,I222,I333), XY.redim(I111,I222,I333);
	Xc_e(I111,I222,I333)=uprev[i](I111+icdif[0],I222+icdif[1],I33,0)-
	  uprev[i](I111-icdif[0],I222-icdif[1],I33,0);
	Yc_e(I111,I222,I333)=uprev[i](I111+icdif[0],I222+icdif[1],I33,1)-
	  uprev[i](I111-icdif[0],I222-icdif[1],I33,1);
	X2(I111,I222,I333)=Xc_e(I111,I222,I333)*Xc_e(I111,I222,I333);
	Y2(I111,I222,I333)=Yc_e(I111,I222,I333)*Yc_e(I111,I222,I333);
	XY(I111,I222,I333)=Xc_e(I111,I222,I333)*Yc_e(I111,I222,I333);
	normXc_e(I111,I222,I333)=X2(I111,I222,I333)+Y2(I111,I222,I333);
	if (i==0)
	{
	  u1(I111,I222,I333,0)=
	    (1.-omega2)*u1(I111,I222,I333,0)+
	    omega2*(X2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
		    XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1)-
		    XY(I111,I222,I333)*
		    uprev[i](I111-icdif[0],I222-icdif[1],I33,1)+
		    Y2(I111,I222,I333)*
		    uprev[i](I111-icdif[0],I222-icdif[1],I33,0))/
	    normXc_e(I111,I222,I333);
	  u1(I111,I222,I333,1)=
	    (1-omega2)*u1(I111,I222,I333,1)+
	    omega2*(X2(I111,I222,I333)*
		    uprev[i](I111-icdif[0],I222-icdif[1],I33,1)-
		    XY(I111,I222,I333)*
		    uprev[i](I111-icdif[0],I222-icdif[1],I33,0)+
		    XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
		    Y2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1))/
	    normXc_e(I111,I222,I333);
	}
	else 
	{
          u1(I111,I222,I333,0)=
	    (1.-omega2)*u1(I111,I222,I333,0)+
	    omega2*(X2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
		    XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1)-
		    Xc_e(I111,I222,I333)*RHS[i](I111,I222,I333,0)-
		    Yc_e(I111,I222,I333)*RHS[i](I111,I222,I333,1))/normXc_e(I111,I222,I333);
          u1(I111,I222,I333,1)=
	    (1-omega2)*u1(I111,I222,I333,1)+
	    omega2*(Xc_e(I111,I222,I333)*RHS[i](I111,I222,I333,1)+
		    XY(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,0)+
		    Y2(I111,I222,I333)*u1(I111+is[0],I222+is[1],I333,1)-
		    Yc_e(I111,I222,I333)*RHS[i](I111,I222,I333,0))/normXc_e(I111,I222,I333);
	}
	jstart=I2[i].getBase();
	jstop=I2[i].getBound();
	istart=I1[i].getBase();
	istop=I1[i].getBound();
	u1(istart,jstart,I33,Range(0,1))=uprev[i](istart,jstart,I33,Range(0,1));
	u1(istop,jstart,I33,Range(0,1))=uprev[i](istop,jstart,I33,Range(0,1));
	u1(istart,jstop,I33,Range(0,1))=uprev[i](istart,jstop,I33,Range(0,1));
	u1(istop,jstop,I33,Range(0,1))=uprev[i](istop,jstop,I33,Range(0,1));
        
	// The remainder deals with sharp corners on the boundary
	if (I111.getBase() != I111.getBound()) I111=I1[i];
	if (I222.getBase() != I222.getBound()) I222=I2[i];
	istart=I111.getBase(), istop=I111.getBound();
	jstart=I222.getBase(), jstop=I222.getBound();
	if (i==0)
	{
	  if (smoothingMethod<3)
	  {
	    isharp=0;
	    for (j1=jstart;j1<=jstop;j1++)
	      for (i1=istart; i1<=istop; i1++)
	      {
		if (((utmp[i](i1+icdif[0],j1+icdif[1],0,0)-utmp[i](i1,j1,0,0))*
		     (utmp[i](i1-icdif[0],j1-icdif[1],0,0)-utmp[i](i1,j1,0,0))+
		     (utmp[i](i1+icdif[0],j1+icdif[1],0,1)-utmp[i](i1,j1,0,1))*
		     (utmp[i](i1-icdif[0],j1-icdif[1],0,1)-utmp[i](i1,j1,0,1)))>0.000)
		{
		  isharp=1;
		  break;
		}
	      }
	    if (isharp==1)
	    {
	      where(((utmp[i](I111+icdif[0],I222+icdif[1],I333,0)-
		      u1(I111,I222,I333,0))*
		     (utmp[i](I111-icdif[0],I222-icdif[1],I333,0)-
		      u1(I111,I222,I333,0))+
		     (utmp[i](I111+icdif[0],I222+icdif[1],I333,1)-
		      u1(I111,I222,I333,1))*
		     (utmp[i](I111-icdif[0],I222-icdif[1],I333,1)-
		      u1(I111,I222,I333,1)))>10.*REAL_EPSILON)
	      {
		u1(I111,I222,I333,0)=utmp[i](I111,I222,I333,0);
		u1(I111,I222,I333,1)=utmp[i](I111,I222,I333,1);
		GRIDBC[i0][i](I111,I222,I333)=1;
	      }

	      where(((uprev[i](I111+icdif[0],I222+icdif[1],I333,0)-
		      uprev[i](I111,I222,I333,0))*
		     (uprev[i](I111-icdif[0],I222-icdif[1],I333,0)-uprev[i](I111,I222,I333,0))>10.*REAL_EPSILON)||
		    ((uprev[i](I111+icdif[0],I222+icdif[1],I333,1)-uprev[i](I111,I222,I333,1))*
		     (uprev[i](I111-icdif[0],I222-icdif[1],I333,1)-uprev[i](I111,I222,I333,1))>10.*REAL_EPSILON))
	      {
		GRIDBC[i0][i](I111,I222,I333)=10;
		u1(I111,I222,I333,0)=uprev[i](I111,I222,I333,0);
		u1(I111,I222,I333,1)=uprev[i](I111,I222,I333,1);
		GRIDBC[i0][i](I111+icdif[0],I222+icdif[1],I333)=10;
		u1(I111+icdif[0],I222+icdif[1],I333,0)=uprev[i](I111+icdif[0],I222+icdif[1],I333,0);
		u1(I111+icdif[0],I222+icdif[1],I333,1)=uprev[i](I111+icdif[0],I222+icdif[1],I333,1);
		GRIDBC[i0][i](I111-icdif[0],I222-icdif[1],I33)=10;
		u1(I111-icdif[0],I222-icdif[1],I333,0)=uprev[i](I111-icdif[0],I222-icdif[1],I333,0);
		u1(I111-icdif[0],I222-icdif[1],I333,1)=uprev[i](I111-icdif[0],I222-icdif[1],I333,1);
	      }
	    }  
	  }
	  else 
	  {
	    where(GRIDBC[i0][i](I111,I222,I333)==1)
	    {
	      u1(I111,I222,I333,0)=utmp[i](I111,I222,I333,0);
	      u1(I111,I222,I333,1)=utmp[i](I111,I222,I333,1);
	    }
	    where (GRIDBC[i0][i](I111,I222,I333)==10)
	    {         
	      u1(I111,I222,I333,0)=uprev[i](I111,I222,I333,0);
	      u1(I111,I222,I333,1)=uprev[i](I111,I222,I333,1);
	    } 
          }
	}
	else 
	{
	  for (j1=I222.getBase();j1<=I222.getBound();j1++)
	    for (i1=I111.getBase();i1<=I111.getBound();i1++)
	      GRIDBC[i0][i](i1,j1,0)=GRIDBC[i0][i-1](2*i1,2*j1,0);

	  if ((axis==1)&&(side==0))
	  {
	    GRIDBC[i0][i](-1,jstart,0)=
	      GRIDBC[i0][i-1](-1,2*jstart,0);
	    GRIDBC[i0][i](istop+1,jstart,0)=
	      GRIDBC[i0][i-1](2*istop+1,2*jstart,0);
	  }
	  if ((side==0)&&(axis==0))
	  {
	    GRIDBC[i0][i](istart,-1,0)=
	      GRIDBC[i0][i-1](2*istart,-1,0);
	    GRIDBC[i0][i](istart,jstop+1,0)=
	      GRIDBC[i0][i-1](2*istart,2*jstop+1,0);
	  }
	  if ((side==1)&&(axis==1))
	  {
	    GRIDBC[i0][i](-1,jstop,0)=
	      GRIDBC[i0][i-1](-1,2*jstop,0);
	    GRIDBC[i0][i](istop+1,jstop,0)=
	      GRIDBC[i0][i-1](2*istop+1,2*jstop,0);
	  }
	  if ((side==1)&&(axis==0))
	  {
	    GRIDBC[i0][i](istop,-1,0)=
	      GRIDBC[i0][i-1](2*istop,-1,0);
	    GRIDBC[i0][i](istop,jstop+1,0)=
	      GRIDBC[i0][i-1](2*istop,2*jstop+1,0);
	  }

	  where(GRIDBC[i0][i](I111,I222,I333)==1)
	  {
	    u1(I111,I222,I333,0)=utmp[i](I111,I222,I333,0);
	    u1(I111,I222,I333,1)=utmp[i](I111,I222,I333,1);
	  }
	  elsewhere(GRIDBC[i0][i](I111,I222,I333)==10)
	  {
	    u1(I111,I222,I333,0)=uprev[i](I111,I222,I333,0);
	    u1(I111,I222,I333,1)=uprev[i](I111,I222,I333,1);
	  }
	}
      }
    }
}


void multigrid::
project_u(realMappedGridFunction &u1,Index I11, Index J11, 
	  Index K11, Index Rr)
{
  realArray r, x;
  int isize, jsize, ksize;
  x.redim(I11,J11,K11,Rr);
  r.redim(I11,J11,K11,Rr);
  x(I11,J11,K11,Rr)=u1(I11,J11,K11,Rr);

  isize=(I11.getBound()-I11.getBase())/(I11.getStride())+1;
  jsize=(J11.getBound()-J11.getBase())/(J11.getStride())+1;
  ksize=(K11.getBound()-K11.getBase())/(K11.getStride())+1;
  x.reshape(isize*jsize*ksize,Rr);
  r.reshape(isize*jsize*ksize,Rr);
  userMap->inverseMap(x,r);
  r.reshape(I11,J11,K11,Rr);
  r(I11,J11.getBase(),K11,axis2)=0.;
  r(I11,J11.getBound(),K11,axis2)=1.;
  r(I11.getBase(),J11,K11,axis1)=0.;
  r(I11.getBound(),J11,K11,axis1)=1.;
  r.reshape(isize*jsize*ksize,Rr);
  userMap->map(r,x);
  x.reshape(I11,J11,K11,Rr);
  u1(I11,J11,K11,Rr)=x(I11,J11,K11,Rr);
}
