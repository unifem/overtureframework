#include "DomainSolver.h"
// include "CompositeGridOperators.h"
// include "GridCollectionOperators.h"
// include "interpolatePoints.h"
#include "PlotStuff.h"
#include "TridiagonalSolver.h"

#define setupArtDissLineSolve EXTERN_C_NAME(setupartdisslinesolve)
extern "C"
{
//    void insArtificialDiffusion(const int&nd,
//        const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
//        const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
//         const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
//         const int&mask,const real&rsxy, real&u,real&v, int&ipar, real&rpar, int&ierr );

  void setupArtDissLineSolve( const int&nd,
                              const int&nd1a,const int&nd1b,
                              const int&nd2a,const int&nd2b,
                              const int&nd3a,const int&nd3b,
                              const int&nd4a,const int&nd4b,
			      const int&nda1a,const int&nda1b,
                              const int&nda2a,const int&nda2b,
                              const int&nda3a,const int&nda3b,
			      const real&a, const real&b, const real&c, const real&d, const real&e, 
                              real&u, const int&mask, const real&rsxy, const int&ipar, const real&rpar );

}

static int firstTime=1;

int DomainSolver::
addArtificialDissipation( realCompositeGridFunction & u, 
                          real dt )
// ================================================================================================
//   /Description:
//      Add on any artificial diffusion
//
// /u (input) : solution to change
//
//  This function is called before dudt to add dissipation to u 
// ================================================================================================
{
  // printf("HHHHHHHHHHHHHHH addArtificialDissipation( realCGF ) HHHHHHHHHHHHHHHHH\n");

  int ierr=0;
  if( parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion") &&
      parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
      (parameters.dbase.get<real >("ad41")>0. || parameters.dbase.get<real >("ad42")>0.) )
  { // --- add artificial diffusion

    CompositeGrid & cg = *u.getCompositeGrid();
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      addArtificialDissipation( u[grid],dt,grid );
    }

  }

  return ierr;
}

int DomainSolver::
addArtificialDissipation( realMappedGridFunction & u, 
                          real dt, int grid )
// ===============================================================================================
//   /Description: Add an implicit fourth-order artificial dissipation
//
//   /Note: This should be done after applying the boundary conditions??
// ===============================================================================================
{
  // printf("HHHHHHHHHHHHHHH addArtificialDissipation( realMGF ) HHHHHHHHHHHHHHHHH\n");

  bool addDissipation= parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion") &&
    parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
    (parameters.dbase.get<real >("ad41")>0. || parameters.dbase.get<real >("ad42")>0.);
  
  if( !addDissipation ) 
    return 0;

  MappedGrid & mg= *u.getMappedGrid();
  const int numberOfDimensions = mg.numberOfDimensions();
  const IntegerArray & bc = mg.boundaryCondition();
  const IntegerArray & indexRange = mg.indexRange();
  const intArray & mask = mg.mask();



  const bool isRectangular = mg.isRectangular();
  real *prsxy = isRectangular ? u.getDataPointer() : mg.inverseVertexDerivative().getDataPointer();

  real dx[3]={1.,1.,1.};
  if( isRectangular )
    mg.getDeltaX(dx);

  TridiagonalSolver tri;
  
  const int gridType = isRectangular ? 0 : 1;

  real rpar[]={dx[0],dx[1],dx[2],
	       mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),  //
	       parameters.dbase.get<real >("ad41"),parameters.dbase.get<real >("ad42"),dt}; //

  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  Range all;

  I3=indexRange(0,2); // default for 2d
  
  int direction;
  for( direction=0; direction<numberOfDimensions; direction++ )
  {
    // add dissipation along axis==direction


    int axis,na,nb;
    for( axis=0; axis<numberOfDimensions; axis++ )
    {
      if( axis==direction )
      {
	if( (bool)mg.isPeriodic(axis) )
	{
	  na=indexRange(0,axis);
	  nb=indexRange(1,axis);
	}
	else
	{
	  na=indexRange(0,axis)-1;  // include 1 ghost line
	  nb=indexRange(1,axis)+1;
	}
      }
      else
      {
	if( (bool)mg.isPeriodic(axis) )
	{
	  na=indexRange(0,axis);
	  nb=indexRange(1,axis);
	}
	else
	{
	  // do not include boundaries in the tangential directions --- could include outflow?
	  na=indexRange(0,axis)+1;
	  nb=indexRange(1,axis)-1;
	} 
      }
    
      Iv[axis]=Range(na,nb);
    }
  
    

    realArray a(I1,I2,I3),b(I1,I2,I3),c(I1,I2,I3),d(I1,I2,I3),e(I1,I2,I3);

    // Boundary conditions:
    //      Dirichlet:  apply odd symmetry: u(-1)=2*u(0)-u(1)  u(-2)=2*u(0)-u(2)
    //      Neumann/extrapolation: apply even symmetry?   
    //      Periodic: solve periodic systems
    //      Interpolation: extrapolate interp neighbour

    // form the penta-diagonal matrix for 
    //     L =  I - ad(u)*dt*[ -1 4 -6 4 -1 ] 
    int ipar[]={I1.getBase(),I1.getBound(),1, 
		I2.getBase(),I2.getBound(),1, 
		I3.getBase(),I3.getBound(),1, //
		bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
		direction, 
		parameters.dbase.get<int >("orderOfAccuracy"),
		gridType,
		parameters.dbase.get<int >("uc"),parameters.dbase.get<int >("vc"),parameters.dbase.get<int >("wc")}; //

    setupArtDissLineSolve( numberOfDimensions,
                           u.getBase(0),u.getBound(0),
                           u.getBase(1),u.getBound(1),
                           u.getBase(2),u.getBound(2),
                           u.getBase(3),u.getBound(3),
			   a.getBase(0),a.getBound(0),
                           a.getBase(1),a.getBound(1),
                           a.getBase(2),a.getBound(2),
			   *a.getDataPointer(),
			   *b.getDataPointer(),
			   *c.getDataPointer(),
			   *d.getDataPointer(),
			   *e.getDataPointer(),
			   *u.getDataPointer(),
			   *mask.getDataPointer(),
			   *prsxy,ipar[0],rpar[0] );
    
    TridiagonalSolver::SystemType type = (bool)mg.isPeriodic(direction) ? TridiagonalSolver::periodic :
					  TridiagonalSolver::extended;

    #ifndef USE_PPP
      tri.factor(a,b,c,d,e,type,direction);
    #else
      Overture::abort("addArtificialDissipation:ERROR:finish me for parallel Bill!");
    #endif
    
    if( debug() & 8 && firstTime==1 )
    {
      firstTime=0;
      
      char buff[180];
      FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
      
      display(a,sPrintF(buff,"addArtDiss:  a, grid=%i direction=%i",grid,direction),debugFile,"%6.4f ");
      display(b,sPrintF(buff,"addArtDiss:  b, grid=%i direction=%i",grid,direction),debugFile,"%6.4f ");
      display(c,sPrintF(buff,"addArtDiss:  c, grid=%i direction=%i",grid,direction),debugFile,"%6.4f ");
      display(d,sPrintF(buff,"addArtDiss:  d, grid=%i direction=%i",grid,direction),debugFile,"%6.4f ");
      display(e,sPrintF(buff,"addArtDiss:  e, grid=%i direction=%i",grid,direction),debugFile,"%6.4f ");
    }
    
    // realArray r(I1,I2,I3); // rhs

    // solve L u = u 

    #ifndef USE_PPP
      for( int dir=0; dir<numberOfDimensions; dir++ )
        tri.solve( u(all,all,all,uc+dir), I1,I2,I3 );
    #else
      Overture::abort("addArtificialDissipation:ERROR:finish me for parallel Bill!");
    #endif
    
  }
  
  return 0;
}
