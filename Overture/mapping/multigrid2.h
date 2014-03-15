#include "Overture.h"
//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"
#include "Mapping.h"
#include "LineMapping.h"
#include "Square.h"
#include "MappedGridOperators.h"
#include "NameList.h"
#include "TridiagonalSolver.h"
 
class multigrid2{
 // A class to solve pde using the multigrid method

 public:
	 int ndimension;  // The dimension of the problem 1d, 2d or 3d
	 int nlevel;      // The number of multigrid levels used
	 real *a, *b;     // The end points on the x-axis
	 int *dim;        // The number of points in the x, y and z
	                       // directions
         int smoothingMethod;  // 1: Jacobi   2: Red-Black   3: Line-Relaxation

 public:
	 // The constructor
         multigrid2();

         multigrid2(const multigrid2 &old_multigrid); // The copy constructor

	 ~multigrid2();    // The destructor

 public:
	 LineMapping grid;
	 SquareMapping sq_grid;
	 MappedGrid  *mg;
	 realMappedGridFunction *u;
	 Index *Ig1, *Ig2, *Ig3;       //Index with ghost points
	 Index *I1, *I2, *I3;          //Index for grid points
	 Index *Iint1, *Iint2, *Iint3; //Index for interior points
         realArray *Source,*RHS, *w, *utmp, *uprev;   //The right hand side
	 real *dx, *dy, *dz;           //The grid width
         int niter, Maxiter,iter;      //The number of iteration
	 			       //for the smoother
         real omega;                   // Optimal omega for underelaxed jacobi
	 real lambda;                  // Interpolation power
				       // when there is a gridBc=3
         real alpha, alphaPrev, omega1;
	 int numberOfPeriods;          // For doing many sheets in the 
				       // periodic case
	 int numberOfPointattractions;
	 int numberOfIlineattractions;
	 int numberOfJlineattractions;
	 int numberOfKlineattractions;
	 int useBlockTridiag;
	 int **PointAttraction, *ILineAttraction, *JLineAttraction;
	 int *KLineAttraction;
	 real *APointcoeff, *CPointcoeff;
	 real *AIlinecoeff, *CIlinecoeff;
	 real *AJlinecoeff, *CJlinecoeff;
	 real *AKlinecoeff, *CKlinecoeff;
	 realArray *CoeffInterp[4]; // When there are combined BC
	 realArray *P[4], *Q[4], P1[4],P2[4],Q1[4],Q2[4];
	 realArray *Xee0[2],*Xcc0[2];
	 realArray XE[2], XC[2], XEE[2],XCC[2];
	 intArray gridBc,*GRIDBC[4];
	 realArray dB;                 //The specified boundary thickness
	 Mapping *userMap;

 public:
         realArray SignOf(realArray uarray);
  
	 void getResidual(realArray &resid1, int i, realArray *up);


	 void getRHS(realArray &RH, int i, realArray *uprev);
	 
	 void getSource(int i, int ichange);

	 void solve(int i, int niter1, int imethod,realArray *uprev, int
	            ichange);

	 void Interpolate(int levelFrom, int levelTo, realArray &uFrom, 
			  realArray &uTo, int jmax);

	 void multigridVcycle(int ilevel, int ilevelFiner, int ilevelCoarser,
			      realArray *uprev);

	 void chooseMethod(int itmp);

	 void applyMultigrid(const realArray &u0, Index Igrid, Index Jgrid,
	                     Index Kgrid, Index Rr);

	 int make2Power(int n);

         void setup(const int ndimension=1, const int nlevel=4, 
		    const real a0=0.0, const real b0=1.0, const int xdim=33, 
		    const real a1=0.0, const real b1=1.0, const int ydim=33,  
		    const real a2=0.0, const real b2=0.0, const int zdim=1);
	 

	 void applyBC(realArray &u1, realArray &v1, int i);

	 void updateRHS(int i, realArray *up);

	 void find2Dcoefficients(realArray &a1, realArray &b1, 
	               realArray &c1, realArray &d1, realArray &e1,
		       Index I11, Index I22, Index I33, int i, int j);

         void jacobi2Dsolve(const realArray &a1, const realArray &b1, 
			    const realArray &c1, const realArray &d1, 
			    const realArray &e1,Index I11, 
			    Index I22, Index I33, int j, int i, 
			    realArray &u1,realArray *uprev, 
			    int periodicCorrection);

         void line2Dsolve(realArray &a1, realArray &b1, 
			  realArray &c1, realArray &d1, 
			  Index I11, Index I22, Index I33,
			  const realArray &coeff1, const realArray &coeff2, 
			  const realArray &coeff3, const realArray &coeff4, 
			  const realArray &coeff5, Index Ic1, Index Ic2, 
			  Index Ic3, int j, int i, realArray &u1, int isweep, 
			  realArray *uprev);

	 void Initialize(realMappedGridFunction *u, const realArray &u0,
			 realArray *up, Index Igrid, Index Jgrid, 
			 Index Kgrid, Index Rr);

         int** IntArray2d(int istart, int iend, int jstart, int jend);
 
         realArray** RealArray2d(int istart, int iend, int jstart, int jend);
 
         void DeleteIntArray2d(int **t, int istart, int iend, int
         jstart, int jend);
 
         void DeleteRealArray2d(realArray **t, int istart, int iend, 
         int jstart, int jend);

	 void bcUpdate(realMappedGridFunction &u, const realArray &u0, 
		       realArray &Src);

	 void project_u(realMappedGridFunction &u1, Index I11, 
			Index J11, Index K11, Index Rr);

         void get2DBoundaryResidual(int i, realArray &res, realArray *up);

	 void getBCtridiag( int isweep, realArray &a1, realArray &b1,
	                    realArray &c1,realArray d1,int i,int j,
			    realArray *uprev);

	 void getAlpha(void);

	 void blockLine2Dsolve(realArray &a1, realArray &b1, realArray &c1,
               realArray &d1, Index I11, Index I22, Index I33, 
               realArray &coeff1, realArray &coeff2, realArray &coeff3, 
               realArray &coeff4, realArray &coeff5, Index Ic1, Index Ic2, 
               Index Ic3, int i, realArray &u1, int isweep, realArray *uprev);

	 void findPQ(const realArray &u1, const realArray &coeff1, 
		     const realArray &coeff2, Index I11, Index I22, 
		     Index I33, int i, int isweep, int ichangePQ);
         void updateBC(int i, int iside, int isweep, realArray &u1, 
			realArray *uprev);
         void applyOrthogonalBoundaries(Index I11, Index I22, Index I33,
	                                int j, int i, realArray &u1, 
					realArray *uprev);

};
