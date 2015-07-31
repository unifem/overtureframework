#include "Cgcns.h"
#include "CnsParameters.h"
#include "MappedGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "ArraySimple.h"

// ------------------------------------------------------------------------------------
// Compressible Navier Stokes
//     - compute du/dt for explicit CNS approximations
// 
// ------------------------------------------------------------------------------------

class UserDefinedEOSData;  // forward reference

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


#define U(c)     u(I1,I2,I3,c)   
#define UU(c)   uu(I1,I2,I3,c)
#define UX(c)   ux(I1,I2,I3,c)
#define UY(c)   uy(I1,I2,I3,c)
#define UZ(c)   uz(I1,I2,I3,c)
#define UXX(c) uxx(I1,I2,I3,c)
#define UXY(c) uxy(I1,I2,I3,c)
#define UXZ(c) uxz(I1,I2,I3,c)
#define UYY(c) uyy(I1,I2,I3,c)
#define UYZ(c) uyz(I1,I2,I3,c)
#define UZZ(c) uzz(I1,I2,I3,c)

#define CNSDU23 EXTERN_C_NAME(cnsdu23)
#define CNSDU22 EXTERN_C_NAME(cnsdu22)
#define CNSDU22A EXTERN_C_NAME(cnsdu22a)
#define DUDR2D EXTERN_C_NAME(dudr2d)
#define DUDR2DOLD EXTERN_C_NAME(dudr2dc)
#define DUDR3D EXTERN_C_NAME(dudr3d)
#define DUDR3DOLD EXTERN_C_NAME(dudr3dc)
#define CMPDU EXTERN_C_NAME(cmpdu)
#define DUDR EXTERN_C_NAME(dudr)
#define DUDRC EXTERN_C_NAME(dudrc)
#define DUDR2DMOVING EXTERN_C_NAME(dudr2dmoving)
#define cnsdts EXTERN_C_NAME(cnsdts)
#define TZCOMMON EXTERN_C_NAME(tzcommon)
#define ICNSRHS EXTERN_C_NAME(icnsrhs)
#define AVJST2D EXTERN_C_NAME(avjst2d)
#define CMFDU EXTERN_C_NAME(cmfdu)

extern "C"
{
  extern struct{ int64_t iexactp; real tzrhsl[15],tzrhsr[15],tzdt; int itz;} TZCOMMON;

  void dudr2comp (const real *u, real *ut, const real *rx, const real *det, const real *gv,
		  const real * rx2, const real * det2, const real *gv2, 
		  real *rp, int *ip, double *workspace, const void *OGFunc,
		  const void *mg, const real *vert);
  /*void dudr2comp (const real *u, real *ut, const real *rx, const real *det,
    real *rp, int *ip, double *workspace, const void *OGFunc,
    const void *mg, const real *vert);*/
  void CNSDU22 (const real & t, const int & nd, 
                const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, 
                const int & nrsab, const int & mrsab, const int & kr, const real & u, const real & xy, 
                const real & a, const real & aj, real & ut, real & v, 
                const int & nda, const int & ndb, real & w, real & aa, real & tmp,
                const int & ipu, const real & rpu, const int & moving, const real & gv);

  void CNSDU22A (const real & t, const int & nd,  const int &nc,
		 const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, 
		 const int & nrsab, const int & mrsab, const int & kr, const real & u, const real & xy, 
		 const real & a, const real & aj, real & ut, real & v, 
		 const int & nda, const int & ndb, real & w, real & aa, real & tmp,
		 const int & ipu, const real & rpu, const int & moving, const real & gv);

  void CNSDU23 (const real & t, const int & nd, 
                const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, 
                const int & ndta, const int & ndtb,
                const int & nrsab, const int & mrsab, const int & kr, const real & u, const real & xy, 
                const real & a, const real & aj, real & ut, real & v, 
                const int & nda, const int & ndb, real & w, real & aa, real & tmp,
                const int & ipu, const real & rpu, const int & moving, const real & gv);

/*  void DUDR2D(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
              const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
              const real & dr, const real & ds1, const real & ds2, const real & r,
              const real & rx, const real & gv, const real & det, const real & u,
              real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
              const int & nparam, real & param, const int & niparam, const int & iparam,
              const int & nrwk, real & rwk, const int & niwk, int & iwk, const int & idebug, int & ier); */

  void DUDR3DOLD(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
		 const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
		 const int & nd3a, const int & nd3b, const int & n3a, const int & n3b,
		 const real & dr, const real & ds1, const real & ds2, const real & ds3, const real & r,
		 const real & rx, const real & gv, const real & det, const real & u,
		 real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
		 const int & nparam, real & param, const int & niparam, const int & iparam,
		 const int & nrwk, real & rwk, const int & niwk, int & iwk, const int & idebug, int & ier);

  void DUDR3D(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
              const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
              const int & nd3a, const int & nd3b, const int & n3a, const int & n3b,
              const real & dr, const real & ds1, const real & ds2, const real & ds3, const real & r,
              const real & rx, const real & gv, const real & det, 
	      const real & rx2, const real & gv2, const real & det2, 
	      const real & u,
              real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
              const int & nparam, real & param, const int & niparam, const int & iparam,
              const int & nrwk, real & rwk, const int & niwk, int & iwk, 
	      const real & vertex, const int & idebug, int & ier);

  void DUDR(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
            const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
            const real & dr, const real & ds1, const real & ds2, const real & r,
            const real & a1, const real & gv, const real & aj, const real & u,
            real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
            const int & nparam, real & param, const int & niparam, const int & iparam,
            const int & nrwk, real & rwk, const int & niwk, int & iwk, const int & idebug, int & ier);

  void DUDRC(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
            const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
            const real & dr, const real & ds1, const real & ds2, const real & r,
            const real & a1, const real & gv, const real & aj, const real & u, 
            real & up, const int & mask, const int & ntau, real & tau,  const real & ad, const int & nvar, real & var, 
            const int & nparam, real & param, const int & niparam, const int & iparam,
            const int & nrwk, real & rwk, const int & niwk, int & iwk, const int & idebug, int & ier);

  void DUDR2D(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
              const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
              const real & dr, const real & ds1, const real & ds2, const real & r,
              const real & rx, const real & gv, const real & det,
              const real & rx2, const real & gv2, const real & det2,
              const real & xy, const real & u,
              real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
              const int & nparam, real & param, const int & niparam, const int & iparam,
              const int & nrwk, real & rwk, const int & niwk, int & iwk, 
              const real & xy2, const int & idebug, const DataBase *pdb, int & ier);

  void DUDR2DOLD(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
		 const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
		 const real & dr, const real & ds1, const real & ds2, const real & r,
		 const real & rx, const real & gv, const real & det,
		 const real & rx2, const real & gv2, const real & det2,
		 const real & xy, const real & u,
		 real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
		 const int & nparam, real & param, const int & niparam, const int & iparam,
		 const int & nrwk, real & rwk, const int & niwk, int & iwk, 
		 const int & idebug, int & ier);


  void CMPDU(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
              const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
              const real & dr, const real & ds1, const real & ds2, const real & r,
              const real & rx, const real & gv, const real & det,
              const real & rx2, const real & gv2, const real & det2,
              const real & xy, const real & u,
              real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
              const int & nparam, real & param, const int & niparam, const int & iparam,
              const int & nrwk, real & rwk, const int & niwk, int & iwk, const int & idebug, int & ier);

  void CMFDU(const int & m, const int & nd1a, const int & nd1b, const int & n1a, const int & n1b,
              const int & nd2a, const int & nd2b, const int & n2a, const int & n2b,
              const real & dr, const real & ds1, const real & ds2, const real & r,
              const real & rx, const real & gv, const real & det,
              const real & rx2, const real & gv2, const real & det2,
              const real & xy, const real & u,
              real & up, const int & mask, const int & ntau, real & tau, const real & ad, const int & nvar, real & var,
              const int & nparam, real & param, const int & niparam, const int & iparam,
              const int & nrwk, real & rwk, const int & niwk, int & iwk, const int & idebug, const DataBase *pdb,
              int & ier);

 void cnsdts(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask, const real& xy, const real& rx, const real& u, const real& uu, const real&gv,  
      const real & dw, const real & p, const real & dp, const real & dtVar,
      const int&bc, const int&ipar, const real&rpar, const int&ierr );

  void ICNSRHS(const int *igdim, const int *igint, 
	       const real *vertex, 
	       const real *rx,
	       const real * det,
	       const int *mask,
	       const int *iparam,
	       const real *param, 
	       const real *uL, 
	       real *ut); 


  void AVJST2D(const int & nd,  const int &nc,
		 const int & ndra, const int & ndrb, const int & ndsa, const int & ndsb, 
		 const int & nda, const int & ndb, 
		 const int & mrsab, const int & mask, const real & u, const real & xy, 
	       real & v, 
	       real & w, real & rx, const real & det, 
	       const int & ipu, const real & rpu, const int &isaxi, real &ut);
}

// ************* EOS USER DEFINED COMMON BLOCK **************
#define eosUserDefined EXTERN_C_NAME(eosuserdefined)
extern "C"
{
  extern struct {UserDefinedEOSData *userEOSDataPointer;} eosUserDefined;

}

static bool first[10] = { 1,1,1,1,1,1,1,1,1,1 };

//=================================================================================================
/// \brief Evaluate du/dt for the Navier-Stokes or Euler equations. 
/// \details This function will call the appropriate lower level fortran kernel to evaluate the discretization of the equations.
///
/// \param v (input) : current solution.
/// \param gridVelocity (input) : grid velocity (if this is a moving grid problem)
/// \param dvdt (output) : du/dt
/// \param iparam (input) : integer parameters.
/// \param rparam (input) : real parameters.
/// \param dvdtImplicit (output): for implicit methods this contains the ...
/// \param pmg2 (input) : for moving grids this points to a MappedGrid that holds the mask for the new time level. 
///                       For non-moving grids this points to the same MappedGrid associated with v.
/// \param pGridVelocity2 (input) : for moving grids only, supply the grid velocity at time t+dt for moving grids.
/// \author wdh.
//=================================================================================================
int Cgcns::
getUt(const realMappedGridFunction & v,
      const realMappedGridFunction & gridVelocity_, 
      realMappedGridFunction & dvdt, 
      int iparam[], real rparam[],
      realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
      MappedGrid *pmg2 /* = NULL */,
      const realMappedGridFunction *pGridVelocity2 /* = NULL */ )
{
  if( parameters.dbase.get<int>("simulateGridMotion")>0 ) return 0;

  real cpu0=getCPU();

  assert( pmg2!=NULL );

  const real & t=rparam[0];
  real tForce   =rparam[1];
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];
  const int level=iparam[1];
  const int numberOfStepsTaken = iparam[2];

  const int & myid = parameters.dbase.get<int >("myid");
  const real & dt = parameters.dbase.get<real >("dt");

  const CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE >("pde");
  const CnsParameters::PDEVariation & pdeVariation = parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation");
  const CnsParameters::GodunovVariation & conservativeGodunovMethod = 
                           parameters.dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  
  FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  assert( grid>=0 && grid<realPartOfEigenvalue.size() && grid<imaginaryPartOfEigenvalue.size() );
  real & realPartOfTimeSteppingEigenvalue      = realPartOfEigenvalue[grid];
  real & imaginaryPartOfTimeSteppingEigenvalue = imaginaryPartOfEigenvalue[grid];

//    if( true )
//    {
//      MappedGrid & mg = *(v.getMappedGrid());
//      const IntegerArray & mask = mg.mask();
//      Index I1,I2,I3;
//      getIndex(mg.gridIndexRange(),I1,I2,I3);
//      real minRho=0.,maxRho=0.;
//      int count=sum(mask(I1,I2,I3)!=0);
//      where( mask(I1,I2,I3)!=0 )
//      {
//        minRho=min(v(I1,I2,I3,0));
//        maxRho=max(v(I1,I2,I3,0));
//      }
//      printf("cns:start: grid=%i t=%9.3e min(rho)=%8.2e max(rho)=%8.2e (count=%i)\n",grid,t,minRho,maxRho,count);
//    }

  if( debug() & 4 ) 
    printf(">>> Cgcns::getUt: grid=%i level=%i numberOfStepsTaken=%i\n",grid,level,numberOfStepsTaken);

  // extrap interp neighbours here (since fixup unused points will overwrite them)
  if( false )
  {
    Range C=parameters.dbase.get<int >("numberOfComponents");
    realMappedGridFunction & vv = (realMappedGridFunction &) v;

    BoundaryConditionParameters extrapParams;
    if( pdeVariation==CnsParameters::conservativeGodunov )
      extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"); // 2; 

    vv.applyBoundaryCondition(C,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,t,
			      extrapParams,grid);
  }
  
  if( pdeVariation==CnsParameters::conservativeGodunov && conservativeGodunovMethod==1 )
  {
    Overture::abort("Don's C++ version of Godunov is nolonger supported");
//      printf("Use new godunov\n");
//      getUtCNSGodunov(v,gridVelocity_,dvdt,iparam,rparam,dvdtImplicit);
//      return 0;
  }
  else if( pdeVariation==CnsParameters::conservativeGodunov && conservativeGodunovMethod==2 )
  {
    Overture::abort("David's version of Godunov is nolonger supported");
//      printf("Use David's Godunov\n");
//      getUtGodunovCNS(v,gridVelocity_,dvdt,iparam,rparam,dvdtImplicit);
//      return 0;
  }
  

//   realArray & uu = get(WorkSpace::uu);
//   realArray & ux = get(WorkSpace::ux);
//   realArray & uy = get(WorkSpace::uy);
//   realArray & uz = get(WorkSpace::uz);
//   realArray & uxx= get(WorkSpace::uxx);
//   realArray & uyy= get(WorkSpace::uyy);
//   realArray & uzz= get(WorkSpace::uzz);
//   realArray & uxy= get(WorkSpace::uxy);
//   realArray & uxz= get(WorkSpace::uxz);
//   realArray & uyz= get(WorkSpace::uyz);



  if( debug() & 8 )
    printP("Cgcns::getUt: pde = %i \n",pde);
  
  const realArray & u = v;
  const realArray & gridVelocity = gridVelocity_;
  realArray & ut = dvdt;
  const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");
  const int & rc = parameters.dbase.get<int >("rc");
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");
  const bool & gridIsMoving = parameters.gridIsMoving(grid);

  const real & mu = parameters.dbase.get<real >("mu");
  const real & gamma = parameters.dbase.get<real >("gamma");
  const real & kThermal = parameters.dbase.get<real >("kThermal");
  const real & Rg = parameters.dbase.get<real >("Rg");
  const real & nuRho = parameters.dbase.get<real >("nuRho");
  const real & avr = parameters.dbase.get<real >("avr");
  const ArraySimpleFixed<real,3,1,1,1> &gravity=parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
  //  const real *gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
  
  RealArray & statistics = parameters.dbase.get<RealArray>("statistics");

  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
  DataBase *pdb = &parameters.dbase;

  // Look for the pointer to the user defined EOS:
  if( parameters.dbase.has_key("userDefinedEquationOfStateDataPointer") )
  {
    // set the fortran common block variable:
    eosUserDefined.userEOSDataPointer = parameters.dbase.get<UserDefinedEOSData*>("userDefinedEquationOfStateDataPointer");
  }

  MappedGrid & mg = *(v.getMappedGrid());
  Index I1,I2,I3;
  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  Range N = Range(rc,tc); // ***** note
  const int numberOfDimensions=mg.numberOfDimensions();

  if(  mu < 0. || kThermal < 0. || Rg <= 0.
      || gamma <= 0. || nuRho < 0. || avr < 0. )
  {
    printF("Cgcns:getUt dudt: Invalid parameter...\n"
           " numberOfComponents=%3i, mu=%e, kThermal=%e, Rg=%e, \n"
	   " gamma=%e, nuRho=%e, avr=%e, numberOfSpecies=%i ",numberOfComponents,
             mu,kThermal,Rg,gamma,nuRho,avr,parameters.dbase.get<int >("numberOfSpecies"));
    Overture::abort("error");
  }

  const real a43=4./3., a13=1./3.;
  const real gm1=gamma-1.;
  const real mubRg=mu/Rg;

  bool isRectangular = mg.isRectangular();
  if( gridIsMoving )
  {
    // *wdh* 081127 
    // -- the godunov codes assume that moving grids are curvilinear ---
    // A moving grid at t=0 may still be rectangular so we need to overide this 
    isRectangular=false;
  }
  


  if( debug() & 8 )
  {
    fPrintF(debugFile,"*************** getUtCNS: t=%e, grid=%i\n",t,grid);
    fprintf(pDebugFile,"*************** getUtCNS: t=%e, grid=%i\n",t,grid);
  }
  

  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray utLocal;  getLocalArrayWithGhostBoundaries(ut,utLocal);
    const realSerialArray & gridVelocityLocal= gridVelocity.getLocalArray();

    // For moving grids make sure we use the mask from the new grid locations:
    const intSerialArray & maskLocal= (*pmg2).mask().getLocalArray();

    utLocal=0.; // ***** do this ****

  #else
    const realSerialArray & uLocal  = u;
    const realSerialArray & utLocal0 = ut; 
    realSerialArray & utLocal = (realSerialArray &)utLocal0;
    const realSerialArray & gridVelocityLocal = gridVelocity;

    // For moving grids make sure we use the mask from the new grid locations:
    const intSerialArray  & maskLocal = (*pmg2).mask(); 

  #endif
  const int *pmask = maskLocal.getDataPointer();

  const int nGhost=2;
  const IntegerArray & gid = mg.gridIndexRange();
  const IntegerArray & indexRange = mg.indexRange();
  
  bool gridHasPointsOnThisProcessor=true;

  IntegerArray d(2,3),nr(2,3); 
  //kkc 060308  for( int axis=0; axis<3; axis++ )
  d=nr=0;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    d(0,axis)=uLocal.getBase(axis);
    d(1,axis)=uLocal.getBound(axis);
    gridHasPointsOnThisProcessor = d(1,axis)>=d(0,axis);
    
    nr(0,axis)=max(d(0,axis)+nGhost,gid(0,axis));
    nr(1,axis)=min(d(1,axis)-nGhost,gid(1,axis));
    
  }

//   int gridMax=ParallelUtility::getMaxValue(grid);
//   int gridMin=ParallelUtility::getMinValue(grid);
//   if( gridMax!=gridMin )
//   {
//     printf(" Cgcns:getUt:ERROR: values for grid are not the same on all proc's! myid=%i, grid=%i\n",myid,grid);
//     OV_ABORT("fatal");
//   }
  

  if( gridHasPointsOnThisProcessor && pmask==NULL )
  {
    printf("Cgcns:getUt:ERROR: pmask==NULL! This should not happen.\n");
    printf(" myid=%i grid=%i, isRectangular=%i, uLocal=[%i,%i][%i,%i]\n",
           myid,grid,(int)isRectangular,
           uLocal.getBase(0),uLocal.getBound(0),
	   uLocal.getBase(1),uLocal.getBound(1));

    intArray & m = (*pmg2).mask();
        printf("parallel: mask=[%i,%i][%i,%i]\n",
           m.getBase(0),m.getBound(0),
	   m.getBase(1),m.getBound(1));

    (*pmg2).update( MappedGrid::THEmask );

    ::display(mg.gridIndexRange(),"mg.gridIndexRange");
    ::display((*pmg2).gridIndexRange(),"mg2.gridIndexRange");
    
    // const intSerialArray  & maskLocal = (*pmg2).mask().getLocalArray(); 
    intSerialArray  maskLocal; getLocalArrayWithGhostBoundaries((*pmg2).mask(),maskLocal);
    printf("Update the mask in the mapped grid... mask=[%i,%i][%i,%i]\n",
           maskLocal.getBase(0),maskLocal.getBound(0),
	   maskLocal.getBase(1),maskLocal.getBound(1));
    
    OV_ABORT("fatal");
  }
  

  // form the local bc values (parallel ghost boundaries appear as periodic, bc==-1)
  IntegerArray bcLocal(2,3);
  ParallelGridUtility::getLocalBoundaryConditions( v,bcLocal );

  if( debug() & 4  )
  {
    // dvdt=-1.;
    fPrintF(debugFile,"Cgcns:getUt: ****BEFORE DUDR*** t=%e dt=%e, grid=%i \n",t,dt,grid);
    fprintf(pDebugFile,"Cgcns:getUt: ****BEFORE DUDR*** t=%e dt=%e, grid=%i, d=[%i,%i][%i,%i][%i,%i] \n",t,dt,grid,
	   d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2));
    printf(" ****BEFORE DUDR*** t=%e dt=%e, grid=%i, d=[%i,%i][%i,%i][%i,%i] \n",t,dt,grid,
	   d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2));
    
    real dtMax=ParallelUtility::getMaxValue(dt);
    real dtMin=ParallelUtility::getMinValue(dt);
    fPrintF(debugFile," diff(dt)=%8.2e\n",dtMax-dtMin);
    
   
    //::display(uLocal,sPrintF("Cgcns:getUt: Before v.updateGhostBoundaries() t=%e",t),pDebugFile,"%20.14e "); 
    //v.updateGhostBoundaries(); // ****************************************************************************************************
    //::display(uLocal,sPrintF("Cgcns:getUt: After v.updateGhostBoundaries() t=%e",t),pDebugFile,"%20.14e "); 

    
    ::display(v,sPrintF("Cgcns:getUt: Solution at start t=%e",t),debugFile,"%20.14e ");
    // ::display(utLocal,sPrintF("Cgcns:getUt: utLocal at start t=%e",t),pDebugFile,"%20.14e ");  

    outputSolution( v, t );

    if( debug() & 16 )
    {
      realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
      display(vLocal,sPrintF("Cgcns:getUt: Solution at start t=%e",t),pDebugFile,"%8.2e ");
    }

    if( gridIsMoving )
    {
      display(gridVelocity,sPrintF("Cgcns:getUt: **** gridVelocity before dudr *** t=%e dt=%e, grid=%i",t,dt,grid),
	      debugFile,"%12.9f ");

      if( gridVelocity.dimension(0)!=Range(d(0,0),d(1,0)) )
      {
	fPrintF(debugFile,"cns:ERROR: gridVelocity.dimension(0)!=Range(d(0,0),d(1,0)\n");
	printf("cns:ERROR: gridVelocity.dimension(0)!=Range(d(0,0),d(1,0))  [%i,%i]!=[%i,%i]\n",
             gridVelocity.getBase(0),gridVelocity.getBound(0),d(0,0),d(1,0));
        Overture::abort("error");
      }

    }
  }

  if( pde==CnsParameters::compressibleMultiphase )
  {
    // *********************************************************************************************
    // *************************Compressible Multiphase*********************************************
    // *********************************************************************************************

    assert( dt>0. );
    // ut=0.; // **** is this needed ???
    utLocal=0.;  // **** is this needed ???
    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    getIndex(d,I1,I2,I3);

// I'm not sure why this is needed, but u0 is referenced below
// so I don't think this statement can be simply deleted.  (DWS)
    realArray & u0 = (realArray &)u;

// This statement is a bit confusing to me also.  (DWS)
    realArray & gridVelocity0 = (realArray &)gridVelocity;
    const RealArray & artificialDiffusion = parameters.dbase.get<RealArray >("artificialDiffusion");


//c..parameters
//c
//c    iparam(1) =EOS model          (iparam(1)=0 => stiffened solid, ideal gas)
//c    iparam(2) =Reaction model     (iparam(2)=0 => no exchange source terms)
//c    iparam(3) =move               is the grid moving (=0 => no)
//c    iparam(4) =icart              Cartesian grid (=1 => yes)
//c    iparam(5) =iorder             order of the method (=1 or 2)
//c    iparam(6) =method             method of flux calculation (=0 => adaptive MP)
//c    iparam(7) =igrid              grid number
//c    iparam(8) =level              AMR level for this grid
//c    iparam(9) =nstep              number of steps taken
//c    iparam(10)=icount             the maximum number of sub-time steps is determined
//c    iparam(11)=iaxi               Axisymmetric problem? 0=>no, 1=>axisymmetric about grid line j1=j1axi
//c                                                               2=>axisymmetric about grid line j2=j2axi
//c    iparam(12)=j1axi(1) or j2axi(1)
//c    iparam(13)=j1axi(2) or j2axi(2)
//c    iparam(14)=mfsolid            mfsolid != 0 :  for multi-fluid solid 
//c
//c    rparam(1) =real(eigenvalue)   for time stepping
//c    rparam(2) =imag(eigenvalue)   for time stepping
//c    rparam(3) =viscosity          artificial viscosity
//c
//c   timings
//c    rparam(31)=tflux
//c    rparam(32)=tslope
//c    rparam(33)=tsourcer


    const int nrpu=100;
    RealArray rpu(nrpu);  rpu=0.;
    const int nipu=20;
    IntegerArray ipu(nipu);  ipu=0;

    // here is how one can look up user defined parameters

    ListOfShowFileParameters & pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

    ipu(0)=parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
    ipu(1)=parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType");
    ipu(2)=(int)gridIsMoving;
    ipu(3)=0;  // non-Cartesian is default
    ipu(4)=parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod");  // order of accuracy is 2
    ipu(5)=(int)parameters.dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver"); // method of computing numerical flux (0 is the only choice)
    ipu(6)=grid;
    ipu(7)=level;
    ipu(8)=numberOfStepsTaken;

//    printf("cns:order of accuracy=%i\n",parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod"));

    ipu(9)=0;  // will return as max number source sub-cycles

    if( conservativeGodunovMethod == CnsParameters::multiFluidVersion )
      ipu(13)=1; // multi-fluid 
     else
      ipu(13)=0;

    rpu(0)=0.;  // the re part of the time stepping eigenvalue will be returned here
    rpu(1)=0.;  // the im part of the time stepping eigenvalue will be returned here

    rpu(2)=parameters.dbase.get<real >("godunovArtificialViscosity");

    int ierr=0;
    if( numberOfDimensions==2 )
    {

      int ng1dLayer=(d(1,0)-d(0,0)+1);

//c..split up real work space =>
//      lw=1
//      lw1=lw+m*ngrid*3
//      la0=lw1+m*ngrid*5*2
//      la1=la0+4*ngrid
//      laj=la1+8*ngrid
//      lda0=laj+2*ngrid
//      lvaxi=lda0+2*ngrid
//      nreq=lvaxi+3*ngrid

      int niwk=1+2*ng1dLayer;
      int nrwk=1+(13*numberOfComponents+19)*ng1dLayer;

      IntegerArray iwk(niwk);
      RealArray rwk(nrwk);

      int ntau=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);

      assert( parameters.dbase.get<realCompositeGridFunction* >("truncationError")!=NULL );
      realArray & tau = (realArray&)((*parameters.dbase.get<realCompositeGridFunction* >("truncationError"))[grid]);


      #ifdef USE_PPP
        const realSerialArray & tauLocal = tau.getLocalArray();
      #else
        const realSerialArray & tauLocal = tau;
      #endif

      real *xyPtr = rpu.getDataPointer(); // give a default value
      if( parameters.isAxisymmetric() )
      {
        printF("Cgcns:getUt:Error : axisymmetric flow not supported by multiphase pde option\n");
        Overture::abort("error");
      }
      else
      {
        ipu(10)=0;
      }

      int idebug=debug() >3;
      if( isRectangular )
      {
        // cartesian version
	real dx[3];
	mg.getDeltaX(dx);

	RealArray rx(2,2);
	rx(0,0)=mg.gridSpacing(0)/dx[0];
	rx(1,0)=0.;
	rx(0,1)=0.;
	rx(1,1)=mg.gridSpacing(1)/dx[1];

	RealArray det(1);
	det(0)=1./(rx(0,0)*rx(1,1));
	ipu(3)=1;     // change flag to Cartesian version

	real *pgv = gridVelocityLocal.getDataPointer();
	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided

	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
          #ifdef USE_PPP
    	    const realSerialArray & gridVelocity2Local = gridVelocity2.getLocalArray();
          #else
	    const realSerialArray & gridVelocity2Local = gridVelocity2;
          #endif

          pgv2=gridVelocity2Local.getDataPointer();

	  if( (debug() & 4) && gridIsMoving )
	  {
	    display(gridVelocity2,sPrintF("Cgcns:getUt: **** gridVelocity2 before dudr *** t=%e dt=%e, grid=%i",t,dt,grid),
		    debugFile,"%6.2f ");
      
	  }
        }
	if( gridHasPointsOnThisProcessor )
	{
	  CMPDU (numberOfComponents,
		 d(0,0),d(1,0),nr(0,0),nr(1,0),
		 d(0,1),d(1,1),nr(0,1),nr(1,1),
		 dt,
		 mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		 *rx.getDataPointer(),*pgv,*det.getDataPointer(),
		 *rx.getDataPointer(),*pgv2,*det.getDataPointer(),*xyPtr,
		 *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		 *pmask,
		 ntau,*tauLocal.getDataPointer(),
		 artificialDiffusion(0),
		 parameters.dbase.get<int >("numberOfExtraVariables"),
		 uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		 nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		 nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
	  
	}
        // ::display(utLocal,sPrintF("Cgcns:getUt: utLocal at CMPDU t=%e",t),pDebugFile,"%20.14e ");  
	
      }
      else // curvilinear version
      {
	mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );

        #ifdef USE_PPP
	  const realSerialArray & det = mg.centerJacobian().getLocalArray();
	  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
        #else
	  const realSerialArray & det = mg.centerJacobian();
	  const realSerialArray & rx = mg.inverseCenterDerivative();
        #endif

        // ***** finish this for PPP *********

	real *pgv = gridVelocityLocal.getDataPointer();
	real *pdet= det.getDataPointer();
	real *prx = rx.getDataPointer();

	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	real *pdet2=pdet;
	real *prx2 =prx;

	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
	  mg2.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );


          #ifdef USE_PPP
	    pgv2 =gridVelocity2.getLocalArray().getDataPointer();   // Note : getLocalArray has the same dataPointer as getLAWGB
	    pdet2=mg2.centerJacobian().getLocalArray().getDataPointer();
	    prx2 =mg2.inverseCenterDerivative().getLocalArray().getDataPointer();
          #else
            pgv2 =gridVelocity2.getDataPointer();
	    pdet2=mg2.centerJacobian().getDataPointer();
	    prx2 =mg2.inverseCenterDerivative().getDataPointer();
          #endif


	}
	if( gridHasPointsOnThisProcessor )
	{
	  CMPDU (numberOfComponents,
		 d(0,0),d(1,0),nr(0,0),nr(1,0),
		 d(0,1),d(1,1),nr(0,1),nr(1,1),
		 dt,
		 mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		 *prx,*pgv,*pdet,
		 *prx2,*pgv2,*pdet2,*xyPtr,
		 *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		 *pmask,
		 ntau,*tauLocal.getDataPointer(),
		 artificialDiffusion(0),
		 parameters.dbase.get<int >("numberOfExtraVariables"),
		 uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		 nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		 nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
	  
	}
      }
    }
    else
    {
       printF("Cgcns:getUt:ERROR: 3D not supported by multiphase pde option\n");
      Overture::abort("error");
    }

    // ********************
    // ut=0.;
    // **********************


    realPartOfTimeSteppingEigenvalue=rpu(0);
    imaginaryPartOfTimeSteppingEigenvalue=rpu(1);

    
    // printf(" after dudr: imaginaryPartOfTimeSteppingEigenvalue=%8.2e\n",imaginaryPartOfTimeSteppingEigenvalue);

    if( level<0 || level>10 ) // sanity check
    {
      printf("ERROR: getUtCNS: grid=%i : Invalid value for level=%i\n",grid,level);
      Overture::abort();
    }
    if( statistics.getLength(0)<=level )
    {
      int oldNumber=statistics.getLength(0);
      int newNumber=20;
      statistics.resize(newNumber);
      statistics(Range(oldNumber,newNumber-1))=0.;
      parameters.dbase.get<aString* >("namesOfStatistics") = new aString [20];
    }
    int maxNumberOfSubCyles=ipu(9); // max number source sub-cycles
    real timeForFlux       =rpu(30);
    real timeForSlope      =rpu(31);
    real timeForSource     =rpu(32);
    real timeForMiddleSolid=rpu(33);
    real timeForMiddleGas  =rpu(34);
    real timeForCouple     =rpu(35);
    real timeForFluxes     =rpu(36);

    statistics(0)+=timeForFlux;
    statistics(1)+=timeForSlope;
    statistics(2)+=timeForSource;
    statistics(3)+=timeForMiddleSolid;
    statistics(4)+=timeForMiddleGas;
    statistics(5)+=timeForCouple;
    statistics(6)+=timeForFluxes;
    if( t<dt*1.5 )
      statistics(level+10)=0;  // reset to zero after the first step
    statistics(level+10)=max(maxNumberOfSubCyles+.5,statistics(level+10));

    if( ierr!=0 )
    {
      printf("ERROR return from cmpdu: ierr=%i\n",ierr);
      return ierr;
    }

    if( debug() & 64 )
    {
      fPrintF(debugFile," ***dvdt AFTER CMPDU*** t=%e dt=%e, grid=%i\n",t,dt,grid);
      fPrintF(debugFile," after dudr: im(lambda)=%18.10e re(lambda)=%18.10e\n",
                imaginaryPartOfTimeSteppingEigenvalue,realPartOfTimeSteppingEigenvalue);
      outputSolution( dvdt, t );
    }

    if( debug() & 4 )
    {
      display(uLocal,sPrintF("Cgcns::getUt: uLocal after CMPDU t=%9.3e",t),pDebugFile,"%11.8f ");    
      fprintf(pDebugFile,"\n******** Cgcns::getUt:utLocal after CMPDU t=%9.3e dt=%14.8e ********\n",t,dt);
      display(utLocal,"Cgcns::getUt: utLocal after CMPDU",pDebugFile,"%11.5e ");    
    }


  }
  else if( pde==CnsParameters::compressibleNavierStokes &&
           pdeVariation==CnsParameters::conservativeGodunov &&
           conservativeGodunovMethod==CnsParameters::multiFluidVersion  )
  {
    // *********************************************************************************************
    // ************************* Multi-Fluid Godunov************************************************
    // *********************************************************************************************


    assert( dt>0. );
    // ut=0.; // **** is this needed ???
    utLocal=0.;  // **** is this needed ???
    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    getIndex(d,I1,I2,I3);

    realArray & u0 = (realArray &)u;

    realArray & gridVelocity0 = (realArray &)gridVelocity;
    const RealArray & artificialDiffusion = parameters.dbase.get<RealArray >("artificialDiffusion");


    //c..parameters
    //c
    //c    iparam(1) =EOS model          (iparam(1)=0 => stiffened solid, ideal gas)
    //c    iparam(2) =Reaction model     (iparam(2)=0 => no exchange source terms)
    //c    iparam(3) =move               is the grid moving (=0 => no)
    //c    iparam(4) =icart              Cartesian grid (=1 => yes)
    //c    iparam(5) =iorder             order of the method (=1 or 2)
    //c    iparam(6) =method             method of flux calculation (=0 => adaptive MP)
    //c    iparam(7) =igrid              grid number
    //c    iparam(8) =level              AMR level for this grid
    //c    iparam(9) =nstep              number of steps taken
    //c    iparam(10)=icount             the maximum number of sub-time steps is determined
    //c    iparam(11)=iaxi               Axisymmetric problem? 0=>no, 1=>axisymmetric about grid line j1=j1axi
    //c                                                               2=>axisymmetric about grid line j2=j2axi
    //c    iparam(12)=j1axi(1) or j2axi(1)
    //c    iparam(13)=j1axi(2) or j2axi(2)
    //c
    //c    rparam(1) =real(eigenvalue)   for time stepping
    //c    rparam(2) =imag(eigenvalue)   for time stepping
    //c    rparam(3) =viscosity          artificial viscosity
    //c
    //c   timings
    //c    rparam(31)=tflux
    //c    rparam(32)=tslope
    //c    rparam(33)=tsourcer


    const int nrpu=100;
    RealArray rpu(nrpu);  rpu=0.;
    const int nipu=20;
    IntegerArray ipu(nipu);  ipu=0;

    // here is how one can look up user defined parameters

    ListOfShowFileParameters & pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

    ipu(0)=parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
    ipu(1)=parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType");
    ipu(2)=(int)gridIsMoving;
    ipu(3)=0;  // non-Cartesian is default
    ipu(4)=parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod");  // order of accuracy is 2
    ipu(5)=(int)parameters.dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver"); // method of computing numerical flux (0 is the only choice)
    ipu(6)=grid;
    ipu(7)=level;
    ipu(8)=numberOfStepsTaken;

    //    printf("cns:order of accuracy=%i\n",parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod"));

    ipu(9)=0;  // will return as max number source sub-cycles

    rpu(0)=0.;  // the re part of the time stepping eigenvalue will be returned here
    rpu(1)=0.;  // the im part of the time stepping eigenvalue will be returned here

    rpu(2)=parameters.dbase.get<real >("godunovArtificialViscosity");

    //  twilightZoneFlow ???
    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
//      mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
      ipu(13)=parameters.dbase.get<bool >("twilightZoneFlow");
      rpu(13)=(real &)parameters.dbase.get<OGFunction* >("exactSolution");
    }

    int ierr=0;
    if( numberOfDimensions==2 )
    {

      int ng1dLayer=(d(1,0)-d(0,0)+1);

      //c..split up real work space =>
      //      lw=1
      //      lw1=lw+m*ngrid*3
      //      la0=lw1+m*ngrid*5*2
      //      la1=la0+4*ngrid
      //      laj=la1+8*ngrid
      //      lda0=laj+2*ngrid
      //      lvaxi=lda0+2*ngrid
      //      nreq=lvaxi+3*ngrid

      int niwk=1+2*ng1dLayer;
      int nrwk=1+(13*numberOfComponents+19)*ng1dLayer;

      IntegerArray iwk(niwk);
      RealArray rwk(nrwk);

      int ntau=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);

      assert( parameters.dbase.get<realCompositeGridFunction* >("truncationError")!=NULL );
      realArray & tau = (realArray&)((*parameters.dbase.get<realCompositeGridFunction* >("truncationError"))[grid]);


      #ifdef USE_PPP
        const realSerialArray & tauLocal = tau.getLocalArray();
      #else
        const realSerialArray & tauLocal = tau;
      #endif

      real *xyPtr = rpu.getDataPointer(); // give a default value
      if( parameters.isAxisymmetric() || parameters.dbase.get<bool >("twilightZoneFlow") )
      {

       // get the grid vertices (in order to compute the radius which is assumed to be the y direction)
        mg.update(MappedGrid::THEvertex);
        #ifdef USE_PPP
          xyPtr=mg.vertex().getLocalArray().getDataPointer();
        #else
          xyPtr=mg.vertex().getDataPointer();
        #endif

        if( parameters.isAxisymmetric() )
        {

          // set default: axisymmetric index direction is 1 but no boundaries lie on the axis of symmetry
          ipu(10)=1;
          ipu(11)=d(0,0)-1;
          ipu(12)=d(1,0)+1;

          // check to see if any boundaries lie on the axis of symmetry
          if( bcLocal(0,0)==Parameters::axisymmetric || 
              bcLocal(1,0)==Parameters::axisymmetric )
          {
            if( bcLocal(0,1)==Parameters::axisymmetric || 
                bcLocal(1,1)==Parameters::axisymmetric )
            {
              printF("Cgcns:getUt:ERROR  : boundaries along different directions cannot both be axisymmetric\n");
              Overture::abort("error");
            }
            else
            {
              if( bcLocal(0,0)==Parameters::axisymmetric )
                ipu(11)=nr(0,0);
              if( bcLocal(1,0)==Parameters::axisymmetric )
                ipu(12)=nr(1,0);
            }
          }

          // now check the other direction
          if( bcLocal(0,1)==Parameters::axisymmetric || 
              bcLocal(1,1)==Parameters::axisymmetric )
          {
            ipu(10)=2;
            ipu(11)=d(0,1)-1;
            ipu(12)=d(1,1)+1;
            if( bcLocal(0,1)==Parameters::axisymmetric )
              ipu(11)=nr(0,1);
            if( bcLocal(1,1)==Parameters::axisymmetric )
              ipu(12)=nr(1,1);
          }
        }
        else
        {
          ipu(10)=0;
        }

      }

      int idebug=debug() >3;
      if( isRectangular )
      {
        // cartesian version
	real dx[3];
	mg.getDeltaX(dx);

	RealArray rx(2,2);
	rx(0,0)=mg.gridSpacing(0)/dx[0];
	rx(1,0)=0.;
	rx(0,1)=0.;
	rx(1,1)=mg.gridSpacing(1)/dx[1];

	RealArray det(1);
	det(0)=1./(rx(0,0)*rx(1,1));
	ipu(3)=1;     // change flag to Cartesian version

	real *pgv = gridVelocityLocal.getDataPointer();
	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided

	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
          #ifdef USE_PPP
    	    const realSerialArray & gridVelocity2Local = gridVelocity2.getLocalArray();
          #else
	    const realSerialArray & gridVelocity2Local = gridVelocity2;
          #endif

          pgv2=gridVelocity2Local.getDataPointer();

	  if( (debug() & 4) && gridIsMoving )
	  {
	    display(gridVelocity2,sPrintF("Cgcns:getUt: **** gridVelocity2 before dudr *** t=%e dt=%e, grid=%i",t,dt,grid),
		    debugFile,"%6.2f ");
      
	  }
        }
	if( gridHasPointsOnThisProcessor )
	{
	  CMFDU (numberOfComponents,
		 d(0,0),d(1,0),nr(0,0),nr(1,0),
		 d(0,1),d(1,1),nr(0,1),nr(1,1),
		 dt,
		 mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		 *rx.getDataPointer(),*pgv,*det.getDataPointer(),
		 *rx.getDataPointer(),*pgv2,*det.getDataPointer(),*xyPtr,
		 *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		 *pmask,
		 ntau,*tauLocal.getDataPointer(),
		 artificialDiffusion(0),
		 parameters.dbase.get<int >("numberOfExtraVariables"),
		 uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		 nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		 nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,pdb,ierr);
	  
	}
	
      }
      else // curvilinear version
      {
	mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );

        #ifdef USE_PPP
	  const realSerialArray & det = mg.centerJacobian().getLocalArray();
	  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
        #else
	  const realSerialArray & det = mg.centerJacobian();
	  const realSerialArray & rx = mg.inverseCenterDerivative();
        #endif

        // ***** finish this for PPP *********

	real *pgv = gridVelocityLocal.getDataPointer();
	real *pdet= det.getDataPointer();
	real *prx = rx.getDataPointer();

	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	real *pdet2=pdet;
	real *prx2 =prx;

	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
	  mg2.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );


          #ifdef USE_PPP
	    pgv2 =gridVelocity2.getLocalArray().getDataPointer();   // Note : getLocalArray has the same dataPointer as getLAWGB
	    pdet2=mg2.centerJacobian().getLocalArray().getDataPointer();
	    prx2 =mg2.inverseCenterDerivative().getLocalArray().getDataPointer();
          #else
            pgv2 =gridVelocity2.getDataPointer();
	    pdet2=mg2.centerJacobian().getDataPointer();
	    prx2 =mg2.inverseCenterDerivative().getDataPointer();
          #endif


	}
	if( gridHasPointsOnThisProcessor )
	{
	  CMFDU (numberOfComponents,
		 d(0,0),d(1,0),nr(0,0),nr(1,0),
		 d(0,1),d(1,1),nr(0,1),nr(1,1),
		 dt,
		 mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		 *prx,*pgv,*pdet,
		 *prx2,*pgv2,*pdet2,*xyPtr,
		 *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		 *pmask,
		 ntau,*tauLocal.getDataPointer(),
		 artificialDiffusion(0),
		 parameters.dbase.get<int >("numberOfExtraVariables"),
		 uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		 nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		 nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,pdb,ierr);
	  
	}
      }
    }
    else
    {
      printF("Cgcns:getUt:ERROR: 3D not supported by multifluid pde option\n");
      OV_ABORT("error");
    }


    realPartOfTimeSteppingEigenvalue=rpu(0);
    imaginaryPartOfTimeSteppingEigenvalue=rpu(1);

    
    // printf(" after dudr: imaginaryPartOfTimeSteppingEigenvalue=%8.2e\n",imaginaryPartOfTimeSteppingEigenvalue);

    if( level<0 || level>10 ) // sanity check
    {
      printf("ERROR: getUtCNS: grid=%i : Invalid value for level=%i\n",grid,level);
      OV_ABORT("error");
    }
    if( statistics.getLength(0)<=level )
    {
      int oldNumber=statistics.getLength(0);
      int newNumber=20;
      statistics.resize(newNumber);
      statistics(Range(oldNumber,newNumber-1))=0.;
      parameters.dbase.get<aString* >("namesOfStatistics") = new aString [20];
    }
    int maxNumberOfSubCyles=ipu(9); // max number source sub-cycles
    real timeForFlux       =rpu(30);
    real timeForSlope      =rpu(31);
    real timeForSource     =rpu(32);
    real timeForMiddleSolid=rpu(33);
    real timeForMiddleGas  =rpu(34);
    real timeForCouple     =rpu(35);
    real timeForFluxes     =rpu(36);

    statistics(0)+=timeForFlux;
    statistics(1)+=timeForSlope;
    statistics(2)+=timeForSource;
    statistics(3)+=timeForMiddleSolid;
    statistics(4)+=timeForMiddleGas;
    statistics(5)+=timeForCouple;
    statistics(6)+=timeForFluxes;
    if( t<dt*1.5 )
      statistics(level+10)=0;  // reset to zero after the first step
    statistics(level+10)=max(maxNumberOfSubCyles+.5,statistics(level+10));

    if( ierr!=0 )
    {
      printf("ERROR return from cmfdu: ierr=%i\n",ierr);
      return ierr;
    }

    if( debug() & 64 )
    {
      fPrintF(debugFile," ***dvdt AFTER CMFDU*** t=%e dt=%e, grid=%i\n",t,dt,grid);
      fPrintF(debugFile," after CMFDU: im(lambda)=%18.10e re(lambda)=%18.10e\n",
                imaginaryPartOfTimeSteppingEigenvalue,realPartOfTimeSteppingEigenvalue);
      outputSolution( dvdt, t );
    }

    if( debug() & 4 )
    {
      display(uLocal,sPrintF("Cgcns::getUt: uLocal after CMFDU t=%9.3e",t),pDebugFile,"%11.8f ");    
      fprintf(pDebugFile,"\n******** Cgcns::getUt:utLocal after CMFDU t=%9.3e dt=%14.8e ********\n",t,dt);
      display(utLocal,"Cgcns::getUt: utLocal after CMFDU",pDebugFile,"%11.5e ");    
    }




  }
  else if( pde==CnsParameters::compressibleNavierStokes &&
           pdeVariation==CnsParameters::conservativeGodunov  )
  {
    // *********************************************************************************************
    // *************************Conservative Godunov************************************************
    // *********************************************************************************************

    if( debug() & 16 )
      printf("Conservative Godnunov scheme numberOfComponents=%i, numberOfSpecies=%i \n",numberOfComponents,
           parameters.dbase.get<int >("numberOfSpecies"));

    assert( dt>0. );
    // ut=0.; // **** is this needed ???
    utLocal=0.;  // **** is this needed ???
    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    getIndex(d,I1,I2,I3);

// I'm not sure why this is needed, but u0 is referenced below
// so I don't think this statement can be simply deleted.  (DWS)
    realArray & u0 = (realArray &)u;

// This statement is a bit confusing to me also.  (DWS)
    realArray & gridVelocity0 = (realArray &)gridVelocity;
    const RealArray & artificialDiffusion = parameters.dbase.get<RealArray >("artificialDiffusion");


// c    iparam(1) =EOS model
// c    iparam(2) =Reaction model     (iparam(2)=0 => no reaction model)
// c    iparam(3) =ichg               is the grid changing with time (=0 => no)
// c    iparam(4) =icart
// c    iparam(5) =iorder
// c    iparam(6) =method
// c
// c    rparam(1) =real(eigenvalue)   for time stepping
// c    rparam(2) =imag(eigenvalue)   for time stepping
// c    rparam(3) =viscosity          artificial viscosity
// c
// c   ideal equation of state (iparam(1)=0)
// c    rparam(4)=gamma
// c
// c   JWL equation of state (iparam(1)=1)
// c    rparam(4)=gamma
// c
// c   one-step reaction model (iparam(2)=1)
// c    rparam(11)=heat release
// c    rparam(12)=1/activation energy
// c    rparam(13)=prefactor
// c
// c   chain branching reaction model (iparam(2)=2)
// c    param(11)=heat release
// c    param(12)=-absorbed energy
// c    param(13)=1/activation energy(I)
// c    param(14)=1/activation energy(B)
// c    param(15)=prefactor(I)
// c    param(16)=prefactor(B)

    const int nrpu=100;
    RealArray rpu(nrpu);
    const int nipu=22;
    IntegerArray ipu(nipu);

    // here is how one can look up user defined parameters

    ListOfShowFileParameters & pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

    ipu(0)=(int)parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
    ipu(1)=(int)parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType");
    ipu(2)=(int)gridIsMoving;
    ipu(3)=0;  // non-Cartesian is default
    ipu(4)=parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod");  // order of accuracy is 2 
    ipu(5)=(int)parameters.dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver"); // method of computing numerical flux is 1 (Roe's approximate Riemann solver)
    ipu(6)=grid;
    ipu(7)=level;
    ipu(8)=numberOfStepsTaken;

    ipu(9)=0;  // will return as max number source sub-cycles

    ipu(16)=myid;  // id for parallel processing

    rpu(0)=0.;  // the re part of the time stepping eigenvalue will be returned here
    rpu(1)=0.;  // the im part of the time stepping eigenvalue will be returned here

    rpu(2)=parameters.dbase.get<real >("godunovArtificialViscosity");
    rpu(3)=parameters.dbase.get<real >("gamma");

// for viscous calculations
    rpu(80)=parameters.dbase.get<real >("mu");
    rpu(81)=parameters.dbase.get<real >("kThermal");
    rpu(82)=parameters.dbase.get<real >("betaT"),  // 0=no temperature dependence.
    rpu(83)=parameters.dbase.get<real >("betaK"),
    rpu(84)=parameters.dbase.get<real >("rT0");

    if( conservativeGodunovMethod==CnsParameters::multiComponentVersion )
    {
      bool foundSlope, foundFix;
      bool foundDon;
      bool fdFourComp;
      bool fdacm;
      bool fdacoustic;
      fdacm = pdeParameters.getParameter( "acmConst",rpu(49) );
      if( !fdacm )
      {
	rpu(49) = 1.0; // default is to not use any compression
      }
      fdacoustic = pdeParameters.getParameter( "acousticSwitch",ipu(21) );
      if( !fdacoustic )
      {
	ipu(21) = 1; // default is to turn off energy correction near acoustic waves
      }
      foundDon=pdeParameters.getParameter("useDon",ipu(17));
      fdFourComp = pdeParameters.getParameter( "fourComp",ipu(20) );
      if( !fdFourComp )
      {
	ipu(20) = 0; // default is to not do the four component solver
      }
      if( !foundDon )
      {
	ipu(17)=0;
      }
      if( ipu(17) )
      {
	if( ipu(20) )
	{
	  // use Don's code with four ideal components 
	  bool fdg1,fdg2,fdg3,fdg4,fdc1,fdc2,fdc3,fdc4;
	  fdg1 = pdeParameters.getParameter( "gamma1",rpu(50) );
	  fdc1 = pdeParameters.getParameter( "cv1",rpu(51) );
	  fdg2 = pdeParameters.getParameter( "gamma2",rpu(52) );
	  fdc2 = pdeParameters.getParameter( "cv2",rpu(53) );
	  fdg3 = pdeParameters.getParameter( "gamma3",rpu(54) );
	  fdc3 = pdeParameters.getParameter( "cv3",rpu(55) );
	  fdg4 = pdeParameters.getParameter( "gamma4",rpu(56) );
	  fdc4 = pdeParameters.getParameter( "cv4",rpu(57) );
	  foundSlope=pdeParameters.getParameter("slope",ipu(18));
	  foundFix=pdeParameters.getParameter("fix",ipu(19));

	  if( !fdg1 || !fdg2 || !fdg3 || !fdg4 || !fdc1 || !fdc2 || !fdc3 || !fdc4 )
	  {
	    printF("Cgcns:getUt:Error (cns): \n");
	    printF("must define gamma1 through gamma4 and cv1 through cv4 in command file.\n");
	    printF("This is a fatal error!!!!! ... quiting\n");
            Overture::abort("error");
	  }
	}
	else
	{
	  // This option indicates we use Don's code with multicomponent stuff
	  if( parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")!=CnsParameters::jwlEOS )
	  {
	    // Ideal gas EOS
	    bool foundgi, foundgr, foundcvi, foundcvr;
	    foundgi=pdeParameters.getParameter("gammai",rpu(40));
	    foundgr=pdeParameters.getParameter("gammar",rpu(41));
	    foundcvi=pdeParameters.getParameter("cvi",rpu(42));
	    foundcvr=pdeParameters.getParameter("cvr",rpu(43));
	    foundSlope=pdeParameters.getParameter("slope",ipu(18));
	    foundFix=pdeParameters.getParameter("fix",ipu(19));
	    if( t == 0 && grid==0 )
	    {
	      printF( "Using Don's multicomponent code.\n" );
	      if ( !foundgi || !foundgr )
	      {
		printF("Error (cns): \n");
		printF("must define gammai, gammar in command file.\n");
		printF("This is a fatal error!!!!! ... quiting\n");
                Overture::abort("error");
	      }
	    }
	    if( !foundcvi || !foundcvr )
	    {
	      rpu(42)=-1.0;
	      rpu(43)=-1.0;
	    }
	    // mixture stiffened gas
	    rpu(44)=0.0;
	    rpu(45)=0.0;
	    bool foundStiffened, foundpii, foundpir;
	    foundStiffened=pdeParameters.getParameter("stiffenedEOS",ipu(13));
	    if( foundStiffened )
	    {
	      foundpii=pdeParameters.getParameter("pii",rpu(44));
	      foundpir=pdeParameters.getParameter("pir",rpu(45));
	    }
	    else
	    {
	      ipu(13)=0;
	    }
	  }
	  else 
	  {
	    // JWL EOS case
	    foundSlope=pdeParameters.getParameter("slope",ipu(18));
	    foundFix=pdeParameters.getParameter("fix",ipu(19));
	    if( t==0  && grid==0 )
	    {
	      printF( "Using Don's multicomponent code with JWL EOS\n" );
	    }
	  }
	}
      }      
      else
      {
	// this option is for the Jeff's multicomponent version
	bool foundg1, foundg2;
	bool foundcv1, foundcv2;
	bool foundpi1, foundpi2;
	bool fdacm;
	fdacm = pdeParameters.getParameter( "acmConst",rpu(49) );
	if( !fdacm )
	{
	  rpu(49) = 1.0;
	}
	foundg1=pdeParameters.getParameter("gamma1",rpu(40));
	foundg2=pdeParameters.getParameter("gamma2",rpu(43));
	foundcv1=pdeParameters.getParameter("cv1",rpu(41));
	foundcv2=pdeParameters.getParameter("cv2",rpu(44));
	foundpi1=pdeParameters.getParameter("pi1",rpu(42));
	foundpi2=pdeParameters.getParameter("pi2",rpu(45));
	if ( t == 0)
	{
	  printf( "Using Jeff's multicomponent code.\n" );
	  if ( !foundg1 || !foundg2 || !foundcv1 || !foundcv2 || !foundpi1 || !foundpi2 )
	  {
	    printF("Cgcns:getUt:ERROR: \n");
	    printF("must define gamma1, gamma2, cv1, cv2, pi1, pi2 in command file.\n");
	    printF("This is a fatal error!!!!! ... quiting\n");
            Overture::abort("error");
	  }
	}
	foundSlope=pdeParameters.getParameter("slope",ipu(10));
	foundFix=pdeParameters.getParameter("fix",ipu(11));
      }
      if (t == 0. )
      {
	if ( !foundSlope || !foundFix )
	{
	  printF("Cgcns:getUt:ERROR: \n");
	  printF("must define slope, and fix in command file.\n");
	  printF("This is a fatal error!!!!! ... quiting\n");
          Overture::abort("error");
	}
      }
    }
    else
    {
      ipu(17)=0;
      ipu(18)=0;
      ipu(19)=0;
      ipu(20)=0;     // four component ... should not make a difference but for completeness
      ipu(21) = 0;   // acoustic switch ... should not make a difference but for completeness
      rpu(49) = 1.0; // acm default ... should not make a difference but for completeness
    }
/* ------
   if( parameters.dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")==CnsParameters::jwlEOS && 
   conservativeGodunovMethod!=CnsParameters::multiComponentVersion )
   {
   bool fdomeg1, fdajwl11, fdajwl21, fdrjwl11, fdrjwl21;
   bool fdomeg2, fdajwl12, fdajwl22, fdrjwl12, fdrjwl22;
   bool fdvs0, fdvg0, fdcgcs, fdheat;

   fdomeg1=pdeParameters.getParameter("omeg1",rpu(50));
   fdajwl11=pdeParameters.getParameter("ajwl11",rpu(51));
   fdajwl21=pdeParameters.getParameter("ajwl21",rpu(52));
   fdrjwl11=pdeParameters.getParameter("rjwl11",rpu(53));
   fdrjwl21=pdeParameters.getParameter("rjwl21",rpu(54));

   fdomeg2=pdeParameters.getParameter("omeg2",rpu(55));
   fdajwl12=pdeParameters.getParameter("ajwl12",rpu(56));
   fdajwl22=pdeParameters.getParameter("ajwl22",rpu(57));
   fdrjwl12=pdeParameters.getParameter("rjwl12",rpu(58));
   fdrjwl22=pdeParameters.getParameter("rjwl22",rpu(59));

   fdvs0=pdeParameters.getParameter("vs0",rpu(60));
   fdvg0=pdeParameters.getParameter("vg0",rpu(61));
   fdcgcs=pdeParameters.getParameter("cgcs",rpu(62));
   fdheat=pdeParameters.getParameter("heat",rpu(63));

   if( t==0. )
   {
   if( !fdomeg1 || !fdajwl11 || !fdajwl21 || !fdrjwl11 || !fdrjwl21 )
   {
   printf("Error (cns) : undefined solid JWL EOS parameter(s)\n");
   exit(0);
   }
   if( !fdomeg2 || !fdajwl12 || !fdajwl22 || !fdrjwl12 || !fdrjwl22 )
   {
   printf("Error (cns) : undefined gas JWL EOS parameter(s)\n");
   exit(0);
   }
   if( !fdvs0 || !fdvg0 || !fdcgcs || !fdheat )
   {
   printf("Error (cns) : undefined equil. or heat EOS parameter(s)\n");
   exit(0);
   }
   }
   } 
   ----- */

    if( parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::oneStep || 
        parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::oneStepPress )
    {
      if( conservativeGodunovMethod==CnsParameters::multiComponentVersion )
      {
	if( ipu(20) )
	{
	  // 4-component case ... remember we are faking the code by using the reaction variable
	  if( t==0  && grid==0 )
	    printF( "four-component flow!!\n" );
	}
	else
	{
	  if( !pdeParameters.getParameter("useDon",ipu(17)) || 
	      (pdeParameters.getParameter("useDon",ipu(17)) && !ipu(17)) )
	  {
	    printF( "Jeff's code doesn't support rxn ... use \"useDon\" in cmd file.\n" );
            Overture::abort("error");
	  }
	  else
	  {
	    if( t==0  && grid==0 )
	      printF( "Using Don's multicomponent code with one step rxn!!\n" );
	  }
	}
      }
      rpu(10)=parameters.dbase.get<real >("heatRelease");
      if( parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::oneStepPress )
      {
	rpu(11)=parameters.dbase.get<real >("reciprocalActivationEnergy");
	rpu(12)=parameters.dbase.get<real >("rateConstant");
      }
      else if( parameters.dbase.get<real >("heatRelease")!=0. )
      {
	rpu(11)=1./parameters.dbase.get<real >("reciprocalActivationEnergy");
	rpu(12)=parameters.dbase.get<real>("rateConstant")*exp(rpu(11))*parameters.dbase.get<real >("reciprocalActivationEnergy")/
	  ((parameters.dbase.get<real >("gamma")-1)*parameters.dbase.get<real >("heatRelease"));
      }
      else
	rpu(12)=0.;
      
//        printf(" cns: heat,acti,rate= %8.2e %8.2e %8.2e \n",parameters.dbase.get<real >("heatRelease"),
//               parameters.dbase.get<real >("reciprocalActivationEnergy"), parameters.dbase.get<real >("rateConstant"));
      
    }
    else if( parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::branching )
    {
      rpu(10)=parameters.dbase.get<real >("heatRelease");
      rpu(11)=-parameters.dbase.get<real >("absorbedEnergy");
      rpu(12)=1./parameters.dbase.get<real >("reciprocalActivationEnergyI");
      rpu(13)=1./parameters.dbase.get<real >("reciprocalActivationEnergyB");
      rpu(14)=exp(rpu(12)/parameters.dbase.get<real >("crossOverTemperatureI"));
      rpu(15)=exp(rpu(13)/parameters.dbase.get<real >("crossOverTemperatureB"));
    }
    else if( parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::ignitionAndGrowth ||
	     parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::igDesensitization )
    {
      /*bool fdra, fdeb, fdex, fdec, fded, fdey, fdee, fdeg, fdez;
	bool fdal0, fdal1, fdal2, fdai, fdag1, fdag2, fdpref, fdtref;
	real ai, ag1, ag2, tref, pref;

	fdra=pdeParameters.getParameter("ra",rpu(10));
	fdeb=pdeParameters.getParameter("eb",rpu(11));
	fdex=pdeParameters.getParameter("ex",rpu(12));
	fdec=pdeParameters.getParameter("ec",rpu(13));
	fded=pdeParameters.getParameter("ed",rpu(14));
	fdey=pdeParameters.getParameter("ey",rpu(15));
	fdee=pdeParameters.getParameter("ee",rpu(16));
	fdeg=pdeParameters.getParameter("eg",rpu(17));
	fdez=pdeParameters.getParameter("ez",rpu(18));

	fdal0=pdeParameters.getParameter("al0",rpu(19));
	fdal1=pdeParameters.getParameter("al1",rpu(20));
	fdal2=pdeParameters.getParameter("al2",rpu(21));
	fdai=pdeParameters.getParameter("ai",ai);
	fdag1=pdeParameters.getParameter("ag1",ag1);
	fdag2=pdeParameters.getParameter("ag2",ag2);
	fdtref=pdeParameters.getParameter("tref",tref);
	fdpref=pdeParameters.getParameter("pref",pref);

	if( t==0. )
	{
        if( !fdra || !fdeb || !fdex || !fdec || !fded || !fdey || !fdee || !fdeg || !fdez )
        {
	printf("Error (cns) : undefined IG rate parameter(s)\n");
	exit(0);
        }
        if( !fdal0 || !fdal1 || !fdal2 || !fdai || !fdag1 || !fdag2 || !fdpref || !fdtref )
        {
	printf("Error (cns) : undefined IG amplitude or cut-off parameter(s)\n");
	exit(0);
        }
	}

	rpu(22)=tref*ai;
	rpu(23)=tref*ag1*pow(pref,rpu(15));
	rpu(24)=tref*ag2*pow(pref,rpu(18));
	if( parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")==Parameters::igDesensitization )
	{
	ipu(15)=1;
	}
	else
	{
	ipu(15)=0;
	}*/
    }
    else if( parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType")!=Parameters::noReactions )
    {
      printf("ERROR: unknown reactionType = %i\n",(int)parameters.dbase.get<Parameters::ReactionTypeEnum >("reactionType") );
      throw "error";
    }

    int ierr=0;
    if( numberOfDimensions==3 )
    {
      int oldVersion;
      if( !pdeParameters.getParameter("oldVersion",oldVersion) )
      {
	oldVersion = 0;
      }
      // oldVersion=true;    // uncomment to use old version no matter what

      //bool checkNewVersion=true;   // to check the new version of the 3-D code
      int checkNewVersion;
      if( !pdeParameters.getParameter("checkNewVersion",checkNewVersion) )
      {
	checkNewVersion = 0;
      }
      // checkNewVersion=false;    // uncomment to never check the new version

      // set common block for twilight zone
      real *xyPtr = rpu.getDataPointer(); // give a default value
      if( parameters.dbase.get<bool >("twilightZoneFlow") && !oldVersion )
      {
	mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
	TZCOMMON.itz    = parameters.dbase.get<bool >("twilightZoneFlow");
	TZCOMMON.iexactp = (int64_t)(parameters.dbase.get<OGFunction* >("exactSolution"));
	TZCOMMON.tzdt   = dt;
	mg.update(MappedGrid::THEvertex);
#ifdef USE_PPP
	xyPtr=mg.vertex().getLocalArray().getDataPointer();
#else
	xyPtr=mg.vertex().getDataPointer();
#endif
      }

      if( debug() & 4  )
      {
        Index I1,I2,I3;
        getIndex(mg.gridIndexRange(),I1,I2,I3);
        const realSerialArray & uLocal = u.getLocalArray();
        bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
        real minRho=0.,maxRho=0.;
	int count=0;
	if( ok )
	{
          // const intSerialArray & mask = mg.mask().getLocalArray();
	  count=sum(maskLocal(I1,I2,I3)!=0);
	  where( maskLocal(I1,I2,I3)!=0 )
	  {
	    minRho=min(uLocal(I1,I2,I3,rc));
	    maxRho=max(uLocal(I1,I2,I3,rc));
	  }
	}
	minRho=ParallelUtility::getMinValue(minRho);
	maxRho=ParallelUtility::getMaxValue(maxRho);
	count =ParallelUtility::getSum(count);
	
        fflush(0);
	Communication_Manager::Sync();
	printF("Before DUDR3D: grid=%i t=%9.3e min(rho)=%8.2e max(rho)=%8.2e (count=%i)\n",grid,t,minRho,maxRho,count);
        fflush(0);
	Communication_Manager::Sync();
      }
	
      int ng2dLayer=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1);  // d holds the local array bounds

      int niwk=1;
      // for now we pass as much workspace as the new version needs to both routines ... even though the old
      //   version can opperate with less memory.
      int nrwk=1+(14*numberOfComponents+26)*ng2dLayer+
	(6*numberOfComponents+11)*numberOfComponents+2*numberOfComponents+
	(2+5*numberOfComponents)*ng2dLayer;
      if ( parameters.dbase.get<int >("numberOfSpecies")>0 )
      {
	niwk+=3*ng2dLayer-1;
	nrwk+=(5*parameters.dbase.get<int >("numberOfSpecies")+numberOfComponents+3)*ng2dLayer;
      }

      IntegerArray iwk(niwk);
      RealArray rwk(nrwk);

      // allocate space for the truncation error
      int ntau=1;
      if ( parameters.dbase.get<int >("numberOfSpecies")>0 )
      {
        assert( parameters.dbase.get<realCompositeGridFunction* >("truncationError")!=NULL );
        ntau+=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1)-1;
      }

      // tau is not used if ntau==1
      realArray tauNull;
      realArray & tau = ntau==1 ? tauNull : (realArray&)((*parameters.dbase.get<realCompositeGridFunction* >("truncationError"))[grid]);
#ifdef USE_PPP
      const realSerialArray & tauLocal = tau.getLocalArray(); // has same data pointer as with ghost boundaries
#else
      const realSerialArray & tauLocal = tau;
#endif

      int idebug=debug() >3;
      if( isRectangular )
      {
        // cartesian version
        real dx[3];
        mg.getDeltaX(dx);

        RealArray rx(3,3);
        rx(0,0)=mg.gridSpacing(0)/dx[0];
        rx(1,0)=0.;
        rx(2,0)=0.;
        rx(0,1)=0.;
        rx(1,1)=mg.gridSpacing(1)/dx[1];
        rx(2,1)=0.;
        rx(0,2)=0.;
        rx(1,2)=0.;
        rx(2,2)=mg.gridSpacing(2)/dx[2];

        RealArray det(1);
        det(0)=1./(rx(0,0)*rx(1,1)*rx(2,2));
        ipu(3)=1;    // change flag to Cartesian value

	real *pgv = gridVelocityLocal.getDataPointer();
	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	  
	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
#ifdef USE_PPP 
	  const realSerialArray & gridVelocity2Local = gridVelocity2.getLocalArray();
#else
	  const realSerialArray & gridVelocity2Local = gridVelocity2;
#endif
             
          pgv2=gridVelocity2Local.getDataPointer();
	}
	if( oldVersion )
	{
	  if( gridHasPointsOnThisProcessor )
	  {
	    DUDR3DOLD (numberOfComponents,
		       d(0,0),d(1,0),nr(0,0),nr(1,0),
		       d(0,1),d(1,1),nr(0,1),nr(1,1),
		       d(0,2),d(1,2),nr(0,2),nr(1,2),
		       dt,
		       mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
		       *rx.getDataPointer(),*gridVelocityLocal.getDataPointer(),
		       *det.getDataPointer(),
		       *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		       *pmask,
		       ntau,*tauLocal.getDataPointer(),
		       artificialDiffusion(0),
		       parameters.dbase.get<int >("numberOfExtraVariables"),
		       uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		       nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		       nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
	  }
	  
//          DUDR3D (numberOfComponents,
//  	   d(0,0),d(1,0),nr(0,0),nr(1,0),
//  	   d(0,1),d(1,1),nr(0,1),nr(1,1),
//  	   d(0,2),d(1,2),nr(0,2),nr(1,2),
//  	   dt,
//  	   mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
//  	   *rx.getDataPointer(),*gridVelocity.getDataPointer(),
//  	   *det.getDataPointer(),
//  	   *u.getDataPointer(),*ut.getDataPointer(),
//  	   *pmask,
//  	   ntau,*tau.getDataPointer(),
//  	   artificialDiffusion(0),
//  	   parameters.dbase.get<int >("numberOfExtraVariables"),u0(u0.getBase(0),u0.getBase(1),u0.getBase(2),numberOfComponents),
//  	   nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
//  	   nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
	}
	else
	{
	  if( gridHasPointsOnThisProcessor )
	  {
	    DUDR3D (numberOfComponents,
		    d(0,0),d(1,0),nr(0,0),nr(1,0),
		    d(0,1),d(1,1),nr(0,1),nr(1,1),
		    d(0,2),d(1,2),nr(0,2),nr(1,2),
		    dt,
		    mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
		    *rx.getDataPointer(),*gridVelocityLocal.getDataPointer(),
		    *det.getDataPointer(),
		    *rx.getDataPointer(),*pgv2,*det.getDataPointer(),
		    *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		    *pmask,
		    ntau,*tauLocal.getDataPointer(),
		    artificialDiffusion(0),
		    parameters.dbase.get<int >("numberOfExtraVariables"),
		    uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		    nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		    nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),*xyPtr,idebug,ierr);
	  }
	}
	if( checkNewVersion && gridHasPointsOnThisProcessor )
	{
	  Range C=parameters.dbase.get<int >("numberOfComponents");
	  RealArray utSave;
	  utSave=utLocal; 
	  DUDR3DOLD (numberOfComponents,
		     d(0,0),d(1,0),nr(0,0),nr(1,0),
		     d(0,1),d(1,1),nr(0,1),nr(1,1),
		     d(0,2),d(1,2),nr(0,2),nr(1,2),
		     dt,
		     mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
		     *rx.getDataPointer(),*gridVelocityLocal.getDataPointer(),
		     *det.getDataPointer(),
		     *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		     *pmask,
		     ntau,*tauLocal.getDataPointer(),
		     artificialDiffusion(0),
		     parameters.dbase.get<int >("numberOfExtraVariables"),
		     uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		     nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		     nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  real maxDiff=0.;
	  where( maskLocal(I1,I2,I3)>0 )
          {
	    for( int c=0; c<numberOfComponents; c++ )
	      maxDiff= max(maxDiff, max(fabs(utLocal(I1,I2,I3,c)-utSave(I1,I2,I3,c))));
	  }
	  
	  printf(" getUtCNS: grid=%i, t=%e, maxDiff=%9.3e\n",grid,t,maxDiff);
	}
      }
      else
      {
        // curvilinear version
        mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );

#ifdef USE_PPP
	const realSerialArray & det = mg.centerJacobian().getLocalArray();
	const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
#else
	const realSerialArray & det = mg.centerJacobian();
	const realSerialArray & rx = mg.inverseCenterDerivative();
#endif

        real *pgv = gridVelocityLocal.getDataPointer();
	real *pdet= det.getDataPointer();
	real *prx = rx.getDataPointer();
	
	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	real *pdet2=pdet;
	real *prx2 =prx;

	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
	  mg2.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
	  
	  
#ifdef USE_PPP
	  pgv2 =gridVelocity2.getLocalArray().getDataPointer();   // Note : getLocalArray has the same dataPointer as getLAWGB
	  pdet2=mg2.centerJacobian().getLocalArray().getDataPointer();
	  prx2 =mg2.inverseCenterDerivative().getLocalArray().getDataPointer();
#else
	  pgv2 =gridVelocity2.getDataPointer();
	  pdet2=mg2.centerJacobian().getDataPointer();
	  prx2 =mg2.inverseCenterDerivative().getDataPointer();
#endif
	}

        getIndex(mg.dimension(),I1,I2,I3);

	if( false && debug() & 1  )
        {
	  display(rx(I1,I2,I3,0),sPrintF("rx on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,1),sPrintF("sx on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,2),sPrintF("tx on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,3),sPrintF("ry on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,4),sPrintF("sy on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,5),sPrintF("ty on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,6),sPrintF("rz on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,7),sPrintF("sz on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(rx(I1,I2,I3,8),sPrintF("tz on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	  display(det,sPrintF("det on grid=%i t=%9.3e",grid,t),pDebugFile,"%5.2f ");
	}

	if( oldVersion && gridHasPointsOnThisProcessor )
	{
	  DUDR3DOLD (numberOfComponents,
		     d(0,0),d(1,0),nr(0,0),nr(1,0),
		     d(0,1),d(1,1),nr(0,1),nr(1,1),
		     d(0,2),d(1,2),nr(0,2),nr(1,2),
		     dt,
		     mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
		     *rx.getDataPointer(),*gridVelocityLocal.getDataPointer(),
		     *det.getDataPointer(),
		     *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		     *pmask,
		     ntau,*tauLocal.getDataPointer(),
		     artificialDiffusion(0),
		     parameters.dbase.get<int >("numberOfExtraVariables"),
		     uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		     nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		     nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
//          DUDR3D (numberOfComponents,
//  	   d(0,0),d(1,0),nr(0,0),nr(1,0),
//  	   d(0,1),d(1,1),nr(0,1),nr(1,1),
//  	   d(0,2),d(1,2),nr(0,2),nr(1,2),
//  	   dt,
//  	   mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
//  	   *rx.getDataPointer(),*gridVelocity.getDataPointer(),
//  	   *det.getDataPointer(),
//  	   *u.getDataPointer(),*ut.getDataPointer(),
//  	   *pmask,
//  	   ntau,*tau.getDataPointer(),
//  	   artificialDiffusion(0),
//  	   parameters.dbase.get<int >("numberOfExtraVariables"),u0(u0.getBase(0),u0.getBase(1),u0.getBase(2),numberOfComponents),
//  	   nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
//  	   nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);
	}
	else if( gridHasPointsOnThisProcessor )
	{
	  DUDR3D (numberOfComponents,
		  d(0,0),d(1,0),nr(0,0),nr(1,0),
		  d(0,1),d(1,1),nr(0,1),nr(1,1),
		  d(0,2),d(1,2),nr(0,2),nr(1,2),
		  dt,
		  mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
		  *prx,*pgv,*pdet,
		  *prx2,*pgv2,*pdet2,
		  *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		  *pmask,
		  ntau,*tauLocal.getDataPointer(),
		  artificialDiffusion(0),
		  parameters.dbase.get<int >("numberOfExtraVariables"),
		  uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		  nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		  nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),*xyPtr,idebug,ierr);
	}
	if( checkNewVersion && gridHasPointsOnThisProcessor )
	{
	  Range C=parameters.dbase.get<int >("numberOfComponents");
	  RealArray utSave;
	  utSave=utLocal;
	  DUDR3DOLD (numberOfComponents,
		     d(0,0),d(1,0),nr(0,0),nr(1,0),
		     d(0,1),d(1,1),nr(0,1),nr(1,1),
		     d(0,2),d(1,2),nr(0,2),nr(1,2),
		     dt,
		     mg.gridSpacing(axis1),mg.gridSpacing(axis2),mg.gridSpacing(axis3),t,
		     *rx.getDataPointer(),*gridVelocityLocal.getDataPointer(),
		     *det.getDataPointer(),
		     *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		     *pmask,
		     ntau,*tauLocal.getDataPointer(),
		     artificialDiffusion(0),
		     parameters.dbase.get<int >("numberOfExtraVariables"),
		     uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		     nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		     nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),idebug,ierr);

	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  real maxDiff=0.;
	  where( maskLocal(I1,I2,I3)>0 )
	  {
	    for( int c=0; c<numberOfComponents; c++ )
	      maxDiff= max(maxDiff, max(fabs(utLocal(I1,I2,I3,c)-utSave(I1,I2,I3,c))));
	  }
	      
	  printf(" getUtCNS: grid=%i, t=%e, maxDiff=%9.3e\n",grid,t,maxDiff);
	}
      }

    }
    else
    {  // ******************** 2D ***********************

      if( conservativeGodunovMethod==CnsParameters::multiComponentVersion &&
	  !ipu(17) )
      {
	// call dudr2comp from here
	mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
	mg.update(MappedGrid::THEvertex);
#ifdef USE_PPP
	const realSerialArray & det = mg.centerJacobian().getLocalArray();
	const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
#else
	const realSerialArray & det = mg.centerJacobian();
	const realSerialArray & rx = mg.inverseCenterDerivative();
#endif
	//RealArray & det = mg.centerJacobian();
	//realArray & rx = mg.inverseCenterDerivative();
	    
	real *pgv = gridVelocityLocal.getDataPointer();
	real *pdet= det.getDataPointer();
	real *prx = rx.getDataPointer();
	
	real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	real *pdet2=pdet;
	real *prx2 =prx;
	
	if( gridIsMoving && pGridVelocity2 !=NULL )
	{
	  const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	  MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
	  mg2.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );	      
#ifdef USE_PPP
	  pgv2 =gridVelocity2.getLocalArray().getDataPointer();   // Note : getLocalArray has the same dataPointer as getLAWGB
	  pdet2=mg2.centerJacobian().getLocalArray().getDataPointer();
	  prx2 =mg2.inverseCenterDerivative().getLocalArray().getDataPointer();
#else
	  pgv2 =gridVelocity2.getDataPointer();
	  pdet2=mg2.centerJacobian().getDataPointer();
	  prx2 =mg2.inverseCenterDerivative().getDataPointer();
#endif
	}
	ipu(12) = (d(1,0)-d(0,0)+1)-4;
	ipu(13) = (d(1,1)-d(0,1)+1)-4;
	ipu(14) = parameters.dbase.get<bool >("twilightZoneFlow");
	rpu(39) = dt;
	rpu(38) = mg.gridSpacing(axis1);
	rpu(37) = mg.gridSpacing(axis2);
	rpu(36) = parameters.dbase.get<real >("godunovArtificialViscosity");
	rpu(35) = t;
	
	double workspace_jeff[13*(5*(ipu(12)+4))+3*(ipu(12)+4)];
	if( gridHasPointsOnThisProcessor )
	{
	  dudr2comp (u.getDataPointer(),
		     ut.getDataPointer(),
		     prx, pdet, pgv,
		     prx2, pdet2, pgv2,
		     rpu.getDataPointer(),
		     ipu.getDataPointer(),
		     workspace_jeff,
		     parameters.dbase.get<OGFunction* >("exactSolution"), 
		     &(mg), mg.vertex().getDataPointer());
	  /*dudr2comp (u.getDataPointer(),
	    ut.getDataPointer(),
	    prx, pdet,
	    rpu.getDataPointer(),
	    ipu.getDataPointer(),
	    workspace_jeff,
	    parameters.dbase.get<OGFunction* >("exactSolution"), 
	    &(mg), mg.vertex().getDataPointer());*/
	}
	
      }
      else // Don's Stuff
      {
	int ng1dLayer=(d(1,0)-d(0,0)+1);

	int niwk=1+2*ng1dLayer;
	int nrwk=1+(12*numberOfComponents+19+14)*ng1dLayer+
	  (4*numberOfComponents+9)*numberOfComponents+2*numberOfComponents+
	  3*numberOfComponents*ng1dLayer+
	  3*(parameters.dbase.get<int >("numberOfSpecies")+3)*ng1dLayer+
	  2*numberOfComponents*ng1dLayer+3*numberOfComponents;

	int oldVersion;
	if( !pdeParameters.getParameter("oldVersion",oldVersion) )
	{
	  oldVersion = 0;
	}

        // oldVersion=true;  // *wdh* --- use old version for now until we sort out some things -----
	int checkNewVersion;
	if( !pdeParameters.getParameter("checkNewVersion",checkNewVersion) )
	{
	  checkNewVersion = 0;
	}
	// bool checkNewVersion=false;   // -------------- for checking Jeff's new version

	// set common block for twilight zone
	if( parameters.dbase.get<bool >("twilightZoneFlow") && !oldVersion )
	{
	  mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
	  TZCOMMON.itz    = parameters.dbase.get<bool >("twilightZoneFlow");
	  TZCOMMON.iexactp = (int64_t)(parameters.dbase.get<OGFunction* >("exactSolution"));
	  TZCOMMON.tzdt   = dt;
	}
	else
	{
	  if ( !pdeParameters.getParameter("slope",ipu(18)) )
	    ipu(18)=0;
	  if( !pdeParameters.getParameter("fix",ipu(19)) )
	    ipu(19)=0;
	}

	if ( parameters.dbase.get<int >("numberOfSpecies")>0 )
	{
	  niwk+=2*ng1dLayer-1;
	  nrwk+=(5*parameters.dbase.get<int >("numberOfSpecies")+numberOfComponents+3)*ng1dLayer;
	}

        // workspace for viscous problems
        if( rpu(80)!=0. || rpu(81)!=0. )
        {
          nrwk+=(numberOfComponents*(d(1,1)-d(0,1)+1)+9)*ng1dLayer;
        }

	
	IntegerArray iwk(niwk);
	RealArray rwk(nrwk);
	
	// allocate space for the truncation error
	int ntau=1;
	if ( parameters.dbase.get<int >("numberOfSpecies")>0 )
	{
	  assert( parameters.dbase.get<realCompositeGridFunction* >("truncationError")!=NULL );
	  ntau+=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)-1;
	}
	// tau is not used if ntau==1
	realArray tauNull;
	realArray & tau = ntau==1 ? tauNull : (realArray&)((*parameters.dbase.get<realCompositeGridFunction* >("truncationError"))[grid]);
#ifdef USE_PPP
	const realSerialArray & tauLocal = tau.getLocalArray();
#else
	const realSerialArray & tauLocal = tau;
#endif

	real *xyPtr = rpu.getDataPointer(); // give a default value
	if( parameters.isAxisymmetric() )
	{

	  // get the grid vertices (in order to compute the radius which is assumed to be the y direction)
	  mg.update(MappedGrid::THEvertex);
#ifdef USE_PPP
	  xyPtr=mg.vertex().getLocalArray().getDataPointer();
#else
	  xyPtr=mg.vertex().getDataPointer();
#endif	  
	  // set default: axisymmetric index direction is 1 but no boundaries lie on the axis of symmetry
	  ipu(10)=1;
	  ipu(11)=d(0,0)-1;
	  ipu(12)=d(1,0)+1;

	  // check to see if any boundaries lie on the axis of symmetry
	  if( bcLocal(0,0)==Parameters::axisymmetric || 
              bcLocal(1,0)==Parameters::axisymmetric )
	  {
	    if( bcLocal(0,1)==Parameters::axisymmetric || 
                bcLocal(1,1)==Parameters::axisymmetric )
	    {
	      printF("Cgcns:getUt:ERROR  : boundaries along different directions cannot both be axisymmetric\n");
	      Overture::abort("error");
	    }
	    else
	    {
	      if( bcLocal(0,0)==Parameters::axisymmetric )
		ipu(11)=nr(0,0);
	      if( bcLocal(1,0)==Parameters::axisymmetric )
		ipu(12)=nr(1,0);
	    }
	  }

	  // now check the other direction
	  if( bcLocal(0,1)==Parameters::axisymmetric || 
              bcLocal(1,1)==Parameters::axisymmetric )
	  {
	    ipu(10)=2;
	    ipu(11)=d(0,1)-1;
	    ipu(12)=d(1,1)+1;
	    if( bcLocal(0,1)==Parameters::axisymmetric )
	      ipu(11)=nr(0,1);
	    if( bcLocal(1,1)==Parameters::axisymmetric )
	      ipu(12)=nr(1,1);
	  }
	}
	else
	{
	  ipu(10)=0;
	}

        if( !oldVersion && (parameters.dbase.get<real >("mu")!=0. || parameters.dbase.get<real >("kThermal")!=0.) )
	{ // for now these are needed even for cartesian grids *wdh*
	  //mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
	}


        if( parameters.dbase.get<bool >("twilightZoneFlow") )
	{ // the vertex is used for TZ
  	  mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
#ifdef USE_PPP
	  xyPtr=mg.vertex().getLocalArray().getDataPointer();
#else
	  xyPtr=mg.vertex().getDataPointer();
#endif
	}
	
	int idebug=debug() >3;
	if( isRectangular )
	{
	  // cartesian version
	  real dx[3];
	  mg.getDeltaX(dx);
	  
	  RealArray rx(2,2);
	  rx(0,0)=mg.gridSpacing(0)/dx[0];
	  rx(1,0)=0.;
	  rx(0,1)=0.;
	  rx(1,1)=mg.gridSpacing(1)/dx[1];
	  
	  RealArray det(1);
	  det(0)=1./(rx(0,0)*rx(1,1));
	  ipu(3)=1;     // change flag to Cartesian version

	  real *pgv = gridVelocityLocal.getDataPointer();
	  real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	  real *xyPtr2=xyPtr;
	  if( gridIsMoving && pGridVelocity2 !=NULL )
	  {
	    const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	    MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
            mg2.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

            // *wdh* 2014/05/09 -- we need to pass the xy coordinates at the new time for TZ
            OV_GET_SERIAL_ARRAY_CONST(real,mg2.vertex(),xy2Local);
            xyPtr2=xy2Local.getDataPointer();

            OV_GET_SERIAL_ARRAY_CONST(real,gridVelocity2,gridVelocity2Local);
            pgv2=gridVelocity2Local.getDataPointer();

	  }
	  
	  if( oldVersion && gridHasPointsOnThisProcessor )
	  {
	    DUDR2DOLD(numberOfComponents,
		       d(0,0),d(1,0),nr(0,0),nr(1,0),
		       d(0,1),d(1,1),nr(0,1),nr(1,1),
		       dt,
		       mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		       *rx.getDataPointer(),*pgv,*det.getDataPointer(),
		       *rx.getDataPointer(),*pgv2,*det.getDataPointer(),*xyPtr,
		       *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		       *pmask,
		       ntau,*tauLocal.getDataPointer(),
		       artificialDiffusion(0),
		       parameters.dbase.get<int >("numberOfExtraVariables"),
		       uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		       nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		       nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
		       idebug,ierr);

          }
	  else if( gridHasPointsOnThisProcessor )
	  {
	    DUDR2D(numberOfComponents,
		    d(0,0),d(1,0),nr(0,0),nr(1,0),
		    d(0,1),d(1,1),nr(0,1),nr(1,1),
		    dt,
		    mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		    *rx.getDataPointer(),*pgv,*det.getDataPointer(),
		    *rx.getDataPointer(),*pgv2,*det.getDataPointer(),*xyPtr,
		    *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		    *pmask,
		    ntau,*tauLocal.getDataPointer(),
		    artificialDiffusion(0),
		    parameters.dbase.get<int >("numberOfExtraVariables"),
		    uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		    nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		    nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
		    *xyPtr2, idebug, pdb, ierr);

	    if( checkNewVersion && gridHasPointsOnThisProcessor )
	    {
              Range C=parameters.dbase.get<int >("numberOfComponents");
	      RealArray utSave;
	      utSave=utLocal;
	      DUDR2DOLD (numberOfComponents,
			 d(0,0),d(1,0),nr(0,0),nr(1,0),
			 d(0,1),d(1,1),nr(0,1),nr(1,1),
			 dt,
			 mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
			 *rx.getDataPointer(),*pgv,*det.getDataPointer(),
			 *rx.getDataPointer(),*pgv2,*det.getDataPointer(),*xyPtr,
			 *uLocal.getDataPointer(),*utLocal.getDataPointer(),
			 *pmask,
			 ntau,*tauLocal.getDataPointer(),
			 artificialDiffusion(0),
			 parameters.dbase.get<int >("numberOfExtraVariables"),
			 uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
			 nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
			 nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
			 idebug,ierr);

              getIndex(mg.gridIndexRange(),I1,I2,I3);
              real maxDiff=0.;
              where( maskLocal(I1,I2,I3)>0 )
              {
                for( int c=0; c<numberOfComponents; c++ )
                  maxDiff= max(maxDiff, max(fabs(utLocal(I1,I2,I3,c)-utSave(I1,I2,I3,c))));
	      }
	      
	      printf(" getUtCNS: grid=%i, t=%e, maxDiff=%9.3e\n",grid,t,maxDiff);
	    }
	  }

	}
	else // curvilinear version
	{
	  mg.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
	  
#ifdef USE_PPP
	  const realSerialArray & det = mg.centerJacobian().getLocalArray();
	  const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
#else
	  const realSerialArray & det = mg.centerJacobian();
	  const realSerialArray & rx = mg.inverseCenterDerivative();
#endif

	  // ***** finish this for PPP *********

	  real *pgv = gridVelocityLocal.getDataPointer();
	  real *pdet= det.getDataPointer();
	  real *prx = rx.getDataPointer();
	  
	  real *pgv2 =pgv;  // use these if no gridVelocity2 is provided
	  real *pdet2=pdet;
	  real *prx2 =prx;
	  real *xyPtr2=xyPtr;
	  if( gridIsMoving && pGridVelocity2 !=NULL )
	  {
	    const realMappedGridFunction & gridVelocity2 = *pGridVelocity2;
	    MappedGrid & mg2 = (MappedGrid&) (*gridVelocity2.getMappedGrid());
	    mg2.update(MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
            mg2.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

            // *wdh* 2014/05/09 -- we need to pass the xy coordinates at the new time for TZ
            OV_GET_SERIAL_ARRAY_CONST(real,mg2.vertex(),xy2Local);
            xyPtr2=xy2Local.getDataPointer();


#ifdef USE_PPP
	    pgv2 =gridVelocity2.getLocalArray().getDataPointer();   // Note : getLocalArray has the same dataPointer as getLAWGB
	    pdet2=mg2.centerJacobian().getLocalArray().getDataPointer();
	    prx2 =mg2.inverseCenterDerivative().getLocalArray().getDataPointer();
#else
	    pgv2 =gridVelocity2.getDataPointer();
	    pdet2=mg2.centerJacobian().getDataPointer();
	    prx2 =mg2.inverseCenterDerivative().getDataPointer();
#endif

	    if( (debug() & 4) && gridIsMoving )
	    {
	      display(gridVelocity2,sPrintF("Cgcns:getUt: **** gridVelocity2 before dudr *** t=%e dt=%e, grid=%i",t,dt,grid),
		      debugFile,"%6.2f ");
      
	    }

	  }
	  
	  if( oldVersion && gridHasPointsOnThisProcessor )
	  {
	    DUDR2DOLD(numberOfComponents,
		       d(0,0),d(1,0),nr(0,0),nr(1,0),
		       d(0,1),d(1,1),nr(0,1),nr(1,1),
		       dt,
		       mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		       *prx,*pgv,*pdet,
		       *prx2,*pgv2,*pdet2,*xyPtr,
		       *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		       *pmask,
		       ntau,*tauLocal.getDataPointer(),
		       artificialDiffusion(0),
		       parameters.dbase.get<int >("numberOfExtraVariables"),
		       uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		       nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		       nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
		       idebug,ierr);




	  }
	  else if( gridHasPointsOnThisProcessor )
	  {
	    DUDR2D(numberOfComponents,
		    d(0,0),d(1,0),nr(0,0),nr(1,0),
		    d(0,1),d(1,1),nr(0,1),nr(1,1),
		    dt,
		    mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		    *prx,*pgv,*pdet,
		    *prx2,*pgv2,*pdet2,*xyPtr,
		    *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		    *pmask,
		    ntau,*tauLocal.getDataPointer(),
		    artificialDiffusion(0),
		    parameters.dbase.get<int >("numberOfExtraVariables"),
		    uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		    nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		    nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
		    *xyPtr2, idebug, pdb, ierr);

	    if( false )
	    {
              real re=realPartOfTimeSteppingEigenvalue, im=imaginaryPartOfTimeSteppingEigenvalue;

	      realPartOfTimeSteppingEigenvalue=rpu(0);
	      imaginaryPartOfTimeSteppingEigenvalue=rpu(1);

	      // debug check ***
	      fprintf(pDebugFile,
		      ">>cns:fullGrid reLambda=%9.3e, imLambda=%9.3e for "
		      "grid=%i local dimension: nr=[%i,%i][%i,%i][%i,%i] myid=%i\n",
		      realPartOfTimeSteppingEigenvalue,imaginaryPartOfTimeSteppingEigenvalue,grid,
		      nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2),myid);

              for( int k=0; k<=1; k++ )
	      {

                IntegerArray mr(2,3);
		mr=nr;
		if( k==0 )
		{
                  nr(1,0)=(mr(0,0)+mr(1,0))/2;
		}
		else
		{
                  nr(0,0)=(mr(0,0)+mr(1,0))/2+1;
		}
		
		DUDR2DOLD (numberOfComponents,
			   d(0,0),d(1,0),nr(0,0),nr(1,0),
			   d(0,1),d(1,1),nr(0,1),nr(1,1),
			   dt,
			   mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
			   *prx,*pgv,*pdet,
			   *prx2,*pgv2,*pdet2,*xyPtr,
			   *uLocal.getDataPointer(),*utLocal.getDataPointer(),
			   *pmask,
			   ntau,*tauLocal.getDataPointer(),
			   artificialDiffusion(0),
			   parameters.dbase.get<int >("numberOfExtraVariables"),
			   uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
			   nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
			   nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
			   idebug,ierr);


		realPartOfTimeSteppingEigenvalue=rpu(0);
		imaginaryPartOfTimeSteppingEigenvalue=rpu(1);

		fprintf(pDebugFile,
			"--->cns:halfGrid reLambda=%9.3e, imLambda=%9.3e for "
			"grid=%i local dimension: nr=[%i,%i][%i,%i][%i,%i] myid=%i\n",
			realPartOfTimeSteppingEigenvalue,imaginaryPartOfTimeSteppingEigenvalue,grid,
			nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2),myid);


                nr=mr;
		
	      }
	      

              rpu(0)=re;  // reset
              rpu(1)=im; 
	      
	    }
	    

	  }
	  if( checkNewVersion  && gridHasPointsOnThisProcessor )
	  {
	    Range C=parameters.dbase.get<int >("numberOfComponents");
	    RealArray utSave;
	    utSave=utLocal;

	    utLocal=0.;
	    
	    DUDR2DOLD (numberOfComponents,
		       d(0,0),d(1,0),nr(0,0),nr(1,0),
		       d(0,1),d(1,1),nr(0,1),nr(1,1),
		       dt,
		       mg.gridSpacing(axis1),mg.gridSpacing(axis2),t,
		       *prx,*pgv,*pdet,
		       *prx2,*pgv2,*pdet2,*xyPtr,
		       *uLocal.getDataPointer(),*utLocal.getDataPointer(),
		       *pmask,
		       ntau,*tauLocal.getDataPointer(),
		       artificialDiffusion(0),
		       parameters.dbase.get<int >("numberOfExtraVariables"),
		       uLocal(uLocal.getBase(0),uLocal.getBase(1),uLocal.getBase(2),numberOfComponents),
		       nrpu,*rpu.getDataPointer(), nipu,*ipu.getDataPointer(),
		       nrwk,*rwk.getDataPointer(),niwk,*iwk.getDataPointer(),
		       idebug,ierr);

	    getIndex(mg.gridIndexRange(),I1,I2,I3);
	    real maxDiff=0.;
	    where( maskLocal(I1,I2,I3)>0 )
	    {
	      for( int c=0; c<numberOfComponents; c++ )
		maxDiff= max(maxDiff, max(fabs(utLocal(I1,I2,I3,c)-utSave(I1,I2,I3,c))));
	    }
	      
	    printf(" getUtCNS: grid=%i, t=%e, maxDiff=%9.3e\n",grid,t,maxDiff);
	  }
	  
	}
      }
    }

    // ********************
    // ut=0.;
    // **********************

    realPartOfTimeSteppingEigenvalue=rpu(0);
    imaginaryPartOfTimeSteppingEigenvalue=rpu(1);

    if( false )
    {
      fprintf(pDebugFile,
	      "**cns: reLambda=%9.3e, imLambda=%9.3e for "
	      "grid=%i local dimension: nr=[%i,%i][%i,%i][%i,%i] myid=%i\n",
	      realPartOfTimeSteppingEigenvalue,imaginaryPartOfTimeSteppingEigenvalue,grid,
              nr(0,0),nr(1,0),nr(0,1),nr(1,1),nr(0,2),nr(1,2),myid);
    }
    
    
    // printf(" after dudr: imaginaryPartOfTimeSteppingEigenvalue=%8.2e\n",imaginaryPartOfTimeSteppingEigenvalue);

    if( level<0 || level>10 ) // sanity check
    {
      printf("ERROR: getUtCNS: grid=%i : Invalid value for level=%i\n",grid,level);
      Overture::abort();
    }
    if( statistics.getLength(0)<=level )
    {
      int oldNumber=statistics.getLength(0);
      int newNumber=20;
      statistics.resize(newNumber);
      statistics(Range(oldNumber,newNumber-1))=0.;
      parameters.dbase.get<aString* >("namesOfStatistics") = new aString [20];
    }
    int maxNumberOfSubCyles=ipu(9); // max number source sub-cycles
    real timeForFlux  =rpu(30);
    real timeForSlope =rpu(31);
    real timeForSource=rpu(32);

    statistics(0)+=timeForFlux;  // fix this for parallel
    statistics(1)+=timeForSlope;
    statistics(2)+=timeForSource;
    if( t<dt*1.5 )
      statistics(level+10)=0;  // reset to zero after the first step
    statistics(level+10)=max(maxNumberOfSubCyles+.5,statistics(level+10));

    if( ierr!=0 )
    {
      printf("ERROR return from dudr: ierr=%i\n",ierr);
      return ierr;
    }

    if( debug() & 4 )
    {
      // display(uLocal,sPrintF("uLocal for dudr (Godunov) t=%9.3e",t),pDebugFile,"%11.5e ");    
      display(uLocal,sPrintF("Cgcns::getUt: uLocal for dudr (Godunov) t=%9.3e",t),pDebugFile,"%11.8f ");    

      // display(maskLocal,"maskLocal after dudr (Godunov)",pDebugFile,"%i ");    

      fprintf(pDebugFile,"\n******** Cgcns::getUt:utLocal after dudr (Godunov) t=%9.3e dt=%14.8e ********\n",t,dt);
      display(utLocal,"Cgcns::getUt: utLocal after dudr (Godunov)",pDebugFile,"%11.5e ");    
    }

    if( debug() & 64 )
    {
      fprintf(debugFile," ***dvdt AFTER DUDR*** t=%e dt=%e, grid=%i\n",t,dt,grid);
      fprintf(debugFile," after dudr: im(lambda)=%18.10e re(lambda)=%18.10e\n",
	      imaginaryPartOfTimeSteppingEigenvalue,realPartOfTimeSteppingEigenvalue);
      outputSolution( dvdt, t );
    }

    }
    else if( pde==CnsParameters::compressibleNavierStokes &&
	     pdeVariation==CnsParameters::conservativeWithArtificialDissipation &&
	     parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::implicit )
    {
      // ****************************************************************************************
      // **************************Jameson Version***********************************************
      // ****************************************************************************************

      const int nd=mg.numberOfDimensions();
      IntegerArray nrsab(nd,2), mrsab(nd,2); // ****NOTE: switch to (axis,side) *****
      for( int axis=0; axis<nd; axis++)
	for( int side=0; side<=1; side++ )
	{
//  	nrsab(axis,side)=mg.gridIndexRange(side,axis);
//  	mrsab(axis,side)=mg.indexRange(side,axis);
	  nrsab(axis,side)=nr(side,axis); // *wdh* 040403
	  mrsab(axis,side)=nr(side,axis); // is this ok?
	}

      Index I1,I2,I3;
      getIndex(d,I1,I2,I3);
      const int nda=min(I1.getBase(),I2.getBase(),I3.getBase());
      const int ndb=max(I1.getBound(),I2.getBound(),I3.getBound());
      Range R(nda,ndb);
      int nadd = parameters.isAxisymmetric() ? 2 : 1;
      RealArray vv(I1,I2,I3,numberOfComponents), aa(R,nd*nd),tmp(R,(nd+nadd)*(nd+nadd)); // **** fix this
      RealArray w(R,numberOfComponents);  // w(R,nd+2) *wdh* 051114 -- leave space for axis-symmetric swirl component

      RealArray rpu(Range(1,20));  rpu=0.;
      IntegerArray ipu(20);  ipu=0;
      //     --- temperature dependent viscosity is input as:
      //    mu=amu*(Tp/rt0)**betat, kappa=akappa*(Tp/rt0)**betak

      real av2=parameters.dbase.get<real >("av2"),
	aw2=parameters.dbase.get<real >("aw2"),
	av4=parameters.dbase.get<real >("av4"),
	aw4=parameters.dbase.get<real >("aw4"),
	betaT=parameters.dbase.get<real >("betaT"),  // 0=no temperature dependence.
	betaK=parameters.dbase.get<real >("betaK"),
	rT0=parameters.dbase.get<real >("rT0");
      if( parameters.dbase.get<bool >("twilightZoneFlow") )
      {
	av2=aw2=av4=aw4=0.;
      }

      mg.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseCenterDerivative |
		MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );

      const int i1 = (I1.getBase()+I1.getBound())/2;
      const int i2 = (I2.getBase()+I2.getBound())/2;
      const int i3 = (I3.getBase()+I3.getBound())/2;
      const real signForDet = mg.centerJacobian()(i1,i2,i3)<0 ? -1. : 1.;
      // const real signForDet = mg.mapping().getMapping().getSignForJacobian();

      // av2=aw2=av4=aw4=0.; // *************************************************************************
      // av4=aw4=0.; // *************************************************************************


      ipu=0;
      ipu( 0)=grid;

      // kThermal : I think think is k/Rg , k=thermal-conductivity and Rg is the gas constant for the given gas
      rpu( 1)=mu;
      rpu( 2)=kThermal; // ?? akappa;  
      rpu( 3)=gamma;
      rpu( 4)=av2*signForDet; // artificial diffusion should have opposite sign if det(J)<0
      rpu( 5)=aw2;
      rpu( 6)=av4*signForDet;
      rpu( 7)=aw4;
      rpu( 8)=betaT;
      rpu( 9)=betaK;
      rpu(10)=rT0;
      rpu(11)=parameters.dbase.get<real >("reynoldsNumber");  
      rpu(12)=parameters.dbase.get<real >("machNumber"); 

      rpu(13)=mg.gridSpacing(0);  // *wdh* 050311
      rpu(14)=mg.gridSpacing(1);
      rpu(15)=mg.gridSpacing(2);

      rpu(16)=t;
      rpu(17)=dt;
    

      if( false && first[grid] ) // fix this
      {
	first[grid]=FALSE;
	realArray & det = mg.centerJacobian();
	realArray & rx = mg.inverseCenterDerivative();
	Index I1,I2,I3;
	real detMin,detMax;
	for( int extra=0; extra<=1; extra++ )
	{
	  getIndex(mg.extendedIndexRange(),I1,I2,I3,extra);
	  detMin=min(det(I1,I2,I3));
	  detMax=max(det(I1,I2,I3));
	  real detMinAbs=min(abs(det(I1,I2,I3)));

	  if( myid==0 )
    	  printf(" **** cns: first time through for grid=%i extra=%i : min(det)=%e, max(det)=%e, min(abs(det))=%e \n",grid,extra,
            detMin,detMax,detMinAbs);
      }

      if( debug() & 64 )
      {
        // mg.inverseCenterDerivative().periodicUpdate();
	// mg.centerJacobian().periodicUpdate();

	display(mg.indexRange(),"mg.indexRange()",debugFile);
	display(mg.gridIndexRange(),"mg.gridIndexRange()",debugFile);
	display(rx,"rx",debugFile,"%6.1e ");
	display(det,"det",debugFile,"%6.1e ");
	
      }
      
      if( detMin*detMax < 0. )
      {
        printf(" Setting `negative volumes' to be positive \n");
        Index I1p,I2p,I3p,I1m,I2m,I3m;
	Index all;
        for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
            if( mg.boundaryCondition(side,axis)>0 )
	    {
	      for( int ghost=1; ghost<=2; ghost++ )
	      {
		getGhostIndex(mg.gridIndexRange(),side,axis,I1p,I2p,I3p,ghost,2);
		// getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,-1,2);
		getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,2);
		det(I1p,I2p,I3p)=det(I1,I2,I3);
		rx(I1p,I2p,I3p,all,all)=rx(I1,I2,I3,all,all);
	      }
	    }
	  }
	}
      }
      
      
/* ----
      scaledInverseDerivative.redim(I1,I2,I3,nd*nd);
      realArray & rx = mg.inverseCenterDerivative();
      realArray & det = mg.centerJacobian();
      // if( signForDet<0 )
      //  det*=-1.;  // *** fix this pass signForDet into cnsdu22
      Range all;
      for( axis=0; axis<mg.numberOfDimensions(); axis++ )
	for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
	  scaledInverseDerivative(all,all,all,axis+nd*dir)=rx(all,all,all,axis+nd*dir)*det;
    
      
      if( debug() & 4 )
	display(scaledInverseDerivative,"scaledInverseDerivative =J*rx ",debugFile,"%6.1e ");
    
----- */
    }

    #ifdef USE_PPP
      const realSerialArray & vertex = mg.vertex().getLocalArray();
      const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
      const realSerialArray & det = mg.centerJacobian().getLocalArray();
    #else
      const realSerialArray & vertex = mg.vertex();
      const realSerialArray & rx = mg.inverseCenterDerivative();
      const realSerialArray & det = mg.centerJacobian();
    #endif

    real *pgv = gridIsMoving ? gridVelocityLocal.getDataPointer() : uLocal.getDataPointer();

    if( mg.numberOfDimensions()==2 )
    {
      // display(u,"u before CNSDU22");
      
      if( parameters.dbase.get<bool >("twilightZoneFlow") && debug() & 4 )
      {
        #ifndef USE_PPP
        Index J1,J2,J3;
        getIndex(mg.extendedIndexRange(),J1,J2,J3);
	display(evaluate(u(J1,J2,J3,uc)-e(mg,J1,J2,J3,rc,t)*e(mg,J1,J2,J3,uc,t)),
		"error in (rho*u)",debugFile,"%6.1e ");
	display(evaluate(u(J1,J2,J3,vc)-e(mg,J1,J2,J3,rc,t)*e(mg,J1,J2,J3,vc,t)),
		"error in (rho*v)",debugFile,"%6.1e ");
	const realArray & r0  = e(mg,J1,J2,J3,rc,t);
	const realArray & u0  = e(mg,J1,J2,J3,uc,t);
	const realArray & v0  = e(mg,J1,J2,J3,vc,t);
	const realArray & t0  = e(mg,J1,J2,J3,tc,t);

	display(evaluate( r0*( (Rg/(gamma-1.))*t0 + .5*( u0*u0 + v0*v0 ) ) - u(J1,J2,J3,tc) ),
		"error in E",debugFile,"%6.1e ");

        #endif
      }
      if( debug() & 4 )
      {
	display(u,sPrintF("cns: u before cnsdu22, processor=%i t=%8.2e",
			  Communication_Manager::My_Process_Number,t),debugFile,"%8.2e ");
	display(uLocal,sPrintF("cns: uLocal before cnsdu22, processor=%i t=%8.2e",
			       Communication_Manager::My_Process_Number,t),pDebugFile,"%8.2e ");
      }

      if( parameters.isAxisymmetric() )
      {
	// set default: axisymmetric index direction is 1 but no boundaries lie on the axis of symmetry
	// these ipu settings were stolen from the dudr2d section above
	ipu(10)=1;
	ipu(11)=d(0,0)-1;
	ipu(12)=d(1,0)+1;

	// check to see if any boundaries lie on the axis of symmetry
	if( bcLocal(0,0)==Parameters::axisymmetric || 
	    bcLocal(1,0)==Parameters::axisymmetric )
	{
	  if( bcLocal(0,1)==Parameters::axisymmetric || 
	      bcLocal(1,1)==Parameters::axisymmetric )
	  {
	    printF("Cgcns:getUt:ERROR : boundaries along different directions cannot both be axisymmetric\n");
	    Overture::abort("error");
	  }
	  else
	  {
	    if( bcLocal(0,0)==Parameters::axisymmetric )
	      ipu(11)=nr(0,0);
	    if( bcLocal(1,0)==Parameters::axisymmetric )
	      ipu(12)=nr(1,0);
	  }
	}
	  
	// now check the other direction
	if( bcLocal(0,1)==Parameters::axisymmetric || 
	    bcLocal(1,1)==Parameters::axisymmetric )
	{
	  ipu(10)=2;
	  ipu(11)=d(0,1)-1;
	  ipu(12)=d(1,1)+1;
	  if( bcLocal(0,1)==Parameters::axisymmetric )
	    ipu(11)=nr(0,1);
	  if( bcLocal(1,1)==Parameters::axisymmetric )
	    ipu(12)=nr(1,1);
	}
        if( gridHasPointsOnThisProcessor )
	{
	  CNSDU22A (t,mg.numberOfDimensions(),parameters.dbase.get<int >("numberOfComponents"),
		    d(0,0),d(1,0),d(0,1),d(1,1),nrsab(0,0),mrsab(0,0),
		    *pmask,
		    *uLocal.getDataPointer(),
		    *vertex.getDataPointer(),
		    *rx.getDataPointer(), // a
		    *det.getDataPointer(),   // aj
		    *utLocal.getDataPointer(),
		    *vv.getDataPointer(),
		    nda,ndb,
		    *(w.getDataPointer()),
		    *(aa.getDataPointer()),
		    *(tmp.getDataPointer()),
		    *(ipu.getDataPointer()),
		    *(rpu.getDataPointer()),
		    (int)gridIsMoving,
		    *pgv );
	}
	
      }
      else
      {
	if( gridHasPointsOnThisProcessor )
	{
	  CNSDU22 (t,mg.numberOfDimensions(),
		   d(0,0),d(1,0),d(0,1),d(1,1),nrsab(0,0),mrsab(0,0),
		   *pmask,
		   *uLocal.getDataPointer(),
		   *vertex.getDataPointer(),
		   *rx.getDataPointer(), // a
		   *det.getDataPointer(),   // aj
		   *utLocal.getDataPointer(),
		   *vv.getDataPointer(),
		   nda,ndb,
		   *(w.getDataPointer()),
		   *(aa.getDataPointer()),
		   *(tmp.getDataPointer()),
		   *(ipu.getDataPointer()),
		   *(rpu.getDataPointer()),
		   (int)gridIsMoving,
		   *pgv );
	}
      }

      if( debug() & 64  )
      {
        display(ut,"ut after CNSDU22",debugFile,"%6.1e ");    
      }
      if( debug() & 4 )
      {
        
	// display(rx,sPrintF("rx (local) for CNSDU22 t=%9.3e grid=%i",t,grid),
        //      pDebugFile,"%10.7f ");    
	// display(det,sPrintF("det (local) for CNSDU22 t=%9.3e grid=%i",t,grid),
        //   pDebugFile,"%10.7f ");    
        display(ipu,sPrintF("ipu for CNSDU22 t=%9.3e grid=%i",t,grid),pDebugFile,"%i ");
        display(rpu,sPrintF("rpu for CNSDU22 t=%9.3e grid=%i",t,grid),pDebugFile,"%10.7f ");
	display(uLocal,sPrintF("uLocal for CNSDU22 t=%9.3e grid=%i",t,grid),
                pDebugFile,"%10.7f ");    
	fprintf(pDebugFile,"\n**** utLocal after CNSDU22 (Jameson) t=%9.3e grid=%i ****\n",
                t,grid);
        display(utLocal,sPrintF("utLocal after CNSDU22, t=%9.3e grid=%i",t,grid),
                pDebugFile,"%10.7f ");    
      }
    }
    else if( mg.numberOfDimensions()==3 )
    {
      if( gridHasPointsOnThisProcessor )
      {
	CNSDU23 (t,mg.numberOfDimensions(),
		 d(0,0),d(1,0),d(0,1),d(1,1),d(0,2),d(1,2),nrsab(0,0),mrsab(0,0),
		 *pmask,
		 *uLocal.getDataPointer(),
		 *vertex.getDataPointer(),
		 *rx.getDataPointer(), // a
		 *det.getDataPointer(),   // aj
		 *utLocal.getDataPointer(),
		 *vv.getDataPointer(),
		 nda,ndb,
		 *(w.getDataPointer()),
		 *(aa.getDataPointer()),
		 *(tmp.getDataPointer()),
		 *(ipu.getDataPointer()),
		 *(rpu.getDataPointer()),
		 (int)gridIsMoving,
		 *pgv );
      }
      
      if( debug() & 4 )
      {
        ut.updateGhostBoundaries();
	// Communication_Manager::Sync();
        fPrintF(debugFile,"\n******** ut after CNSDU23 (Jameson) START t=%9.3e ********\n",t);
        ::display(ut,"ut after CNSDU23",debugFile,"%6.4f ");    
  	fPrintF(debugFile,"\n******** ut after CNSDU23 (Jameson) END t=%9.3e ********\n",t);
      }

    }
    else
    {
      throw "error";
    }
  }
  else if ( pde==CnsParameters::compressibleNavierStokes && 
	    (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit || parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton) )
    {
        const real theta = parameters.dbase.get<real >("implicitFactor");
	const int numberOfDimensions=mg.numberOfDimensions();
	const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");
	const int rc = parameters.dbase.get<int >("rc");
	const int uc = parameters.dbase.get<int >("uc");
	const int vc = parameters.dbase.get<int >("vc");
	const int wc = parameters.dbase.get<int >("wc");
	const int tc = parameters.dbase.get<int >("tc");
	const bool gridIsMoving = parameters.gridIsMoving(grid);
	const real signForDet = mg.mapping().getMapping().getSignForJacobian();

	//const real signForDet = mg.centerJacobian()(i1,i2,i3)<0 ? -1. : 1.;
	
	ArraySimpleFixed<int,20,1,1,1> iparam;
	iparam[0] = numberOfDimensions;
	iparam[1] = numberOfComponents;
	iparam[2] = rc;
	iparam[3] = uc;
	iparam[4] = vc;
	iparam[5] = wc;
	iparam[6] = tc;
	iparam[7] = gridIsMoving;
	iparam[8] = parameters.isAxisymmetric();
	iparam[9] = parameters.dbase.get<bool >("axisymmetricWithSwirl");

	const real mu = parameters.dbase.get<real >("mu");
	const real gamma = parameters.dbase.get<real >("gamma");
	const real kThermal = parameters.dbase.get<real >("kThermal");
	const real Rg = parameters.dbase.get<real >("Rg");
	const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
	const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
	const real machNumber = parameters.dbase.get<real >("machNumber");
	const real av2 = parameters.dbase.get<real >("av2");
	const real aw2 = parameters.dbase.get<real >("aw2");
	const real av4 = parameters.dbase.get<real >("av4");
	const real aw4 = parameters.dbase.get<real >("aw4");

	ArraySimpleFixed<real,20,1,1,1> rparam;
	rparam[0]=reynoldsNumber;
	rparam[1]=prandtlNumber;
	rparam[2]=machNumber;
	rparam[3]=gamma;
	rparam[4]=parameters.dbase.get<real >("implicitFactor");
	rparam[5]=mg.gridSpacing(0);  
	rparam[6]=mg.gridSpacing(1);
	rparam[7]=mg.gridSpacing(2);
	rparam[8]=av4; 
	rparam[9]=dt;

	rparam[17] = parameters.dbase.get<real>("strickwerdaCoeff");
	mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
#ifdef USE_PPP
	const realSerialArray & vertex = mg.vertex().getLocalArray();
	const realSerialArray & rx = mg.inverseCenterDerivative().getLocalArray();
	const realSerialArray & det = mg.centerJacobian().getLocalArray();
#else
	const realSerialArray & vertex = mg.vertex();
	const realSerialArray & rx = mg.inverseCenterDerivative();
	const realSerialArray & det = mg.centerJacobian();
#endif
	
	// // now actually build the RHS
	if( gridHasPointsOnThisProcessor )
	{
	  ICNSRHS(d.getDataPointer(),nr.getDataPointer(),vertex.getDataPointer(), rx.getDataPointer(), 
		  det.getDataPointer(), pmask,// grid info
		  iparam.ptr(),rparam.ptr(), // solver paramters
		  u.getDataPointer(), // state to linearize about
		  utLocal.getDataPointer()); // output: the rhs
	}

#if 1
	if ( false && (av2>REAL_EPSILON || av4 > REAL_EPSILON) ) {
	// // now add the dissipation using cnsdu's JST routine
	Index I1,I2,I3;
	getIndex(d,I1,I2,I3);

	const int nda=min(I1.getBase(),I2.getBase(),I3.getBase());
	const int ndb=max(I1.getBound(),I2.getBound(),I3.getBound());
	int nadd = parameters.isAxisymmetric() ? 2 : 1;
	Range R(nda,ndb);
	RealArray vv(I1,I2,I3,numberOfComponents),utd(I1,I2,I3,numberOfComponents); // *** fix this
	RealArray w(R,numberOfComponents);  // w(R,nd+2) *wdh* 051114 -- leave space for axis-symmetric swirl component

	for ( int c=uc; c<=tc; c++ ) vv(I1,I2,I3,c-1) = uLocal(I1,I2,I3,c);
	vv(I1,I2,I3,tc) = uLocal(I1,I2,I3,rc)*uLocal(I1,I2,I3,tc);

	  // set default: axisymmetric index direction is 1 but no boundaries lie on the axis of symmetry
	//	ipu=0;
	iparam( 0)=grid;
	
	rparam( 0)=mu;
	rparam( 1)=kThermal; // ?? akappa;
	rparam( 2)=gamma;
	rparam( 3)=av2*signForDet; // artificial diffusion should have opposite sign if det(J)<0
	rparam( 4)=aw2;
	rparam( 5)=av4*signForDet;
	rparam( 6)=aw4;

	rparam(10)=parameters.dbase.get<real >("reynoldsNumber");  
	rparam(11)=parameters.dbase.get<real >("machNumber"); 
	
	rparam(12)=mg.gridSpacing(0);  // *wdh* 050311
	rparam(13)=mg.gridSpacing(1);
	rparam(14)=mg.gridSpacing(2);
    
	rparam(15)=t;
	rparam(16)=dt;
	
	// these iparam settings were stolen from the dudr2d section above
	iparam(10)=1;
	iparam(11)=d(0,0)-1;
	iparam(12)=d(1,0)+1;
	
	// check to see if any boundaries lie on the axis of symmetry
	if( bcLocal(0,0)==Parameters::axisymmetric || 
	    bcLocal(1,0)==Parameters::axisymmetric )
	  {
	    if( bcLocal(0,1)==Parameters::axisymmetric || 
		bcLocal(1,1)==Parameters::axisymmetric )
	      {
		printF("Cgcns:getUt:ERROR : boundaries along different directions cannot both be axisymmetric\n");
	        Overture::abort("error");
	      }
	    else
	      {
		if( bcLocal(0,0)==Parameters::axisymmetric )
		  iparam(11)=nr(0,0);
		if( bcLocal(1,0)==Parameters::axisymmetric )
		  iparam(12)=nr(1,0);
	      }
	  }
	
	  // now check the other direction
	if( bcLocal(0,1)==Parameters::axisymmetric || 
	    bcLocal(1,1)==Parameters::axisymmetric )
	  {
	    iparam(10)=2;
	    iparam(11)=d(0,1)-1;
	    iparam(12)=d(1,1)+1;
	    if( bcLocal(0,1)==Parameters::axisymmetric )
	      iparam(11)=nr(0,1);
	    if( bcLocal(1,1)==Parameters::axisymmetric )
	      iparam(12)=nr(1,1);
	  }

	utd = 0.;
	IntegerArray mrsab(mg.numberOfDimensions(),2); // ****NOTE: switch to (axis,side) *****
	for( int axis=0; axis<mg.numberOfDimensions(); axis++)
	  for( int side=0; side<=1; side++ )
	    mrsab(axis,side)=nr(side,axis);// +1-2*side; 

	bool useAxiVisc = false;//parameters.isAxisymmetric();
        if( gridHasPointsOnThisProcessor )
	{
	  AVJST2D(mg.numberOfDimensions(),parameters.dbase.get<int >("numberOfComponents"),
		  d(0,0),d(1,0),d(0,1),d(1,1),
		  nda,ndb,
		  mrsab(0,0),
		  *pmask,
		  *uLocal.getDataPointer(),
		  *vertex.getDataPointer(),
		  *vv.getDataPointer(),
		  *(w.getDataPointer()),
		  *rx.getDataPointer(), // a
		  *det.getDataPointer(),   // aj
		  *(iparam.ptr()),
		  *(rparam.ptr()),
		  useAxiVisc,
		  *utd.getDataPointer()
	    );
	}
	
	//	utd(I1,I2,I3,1).display();
	Range Rd1(nr(0,0),nr(1,0)),Rd2(nr(0,1),nr(1,1)),Rd3(nr(0,2),nr(1,2));
	for ( int c=rc; c<=tc; c++ ) utLocal(Rd1,Rd2,Rd3,c) += utd(Rd1,Rd2,Rd3,c)/det(Rd1,Rd2,Rd3);

	
	}// add dissipation
#endif

    }
  else
  {

    // *********** non-conservative method **************

//   realArray & uu = get(WorkSpace::uu);
//   realArray & ux = get(WorkSpace::ux);
//   realArray & uy = get(WorkSpace::uy);
//   realArray & uz = get(WorkSpace::uz);
//   realArray & uxx= get(WorkSpace::uxx);
//   realArray & uyy= get(WorkSpace::uyy);
//   realArray & uzz= get(WorkSpace::uzz);
//   realArray & uxy= get(WorkSpace::uxy);
//   realArray & uxz= get(WorkSpace::uxz);
//   realArray & uyz= get(WorkSpace::uyz);

//    MappedGridOperators & operators = *(v.getOperators());
//    operators.getDerivatives(v,I1,I2,I3,N);

    MappedGridOperators & op = *(v.getOperators());

    realArray uu,ux,uy,uz, uxx,uxy,uxz, uyy,uyz,uzz;
    
    ux.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));

    op.derivative(MappedGridOperators::xDerivative,v,ux,I1,I2,I3,N);

    if( mg.numberOfDimensions()>1 )
    {
      uy.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));

      uxx.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));
      uxy.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));
      uyy.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));

      op.derivative(MappedGridOperators::yDerivative ,v,uy ,I1,I2,I3,N);
      op.derivative(MappedGridOperators::xxDerivative,v,uxx,I1,I2,I3,N);
      op.derivative(MappedGridOperators::xyDerivative,v,uxy,I1,I2,I3,N);
      op.derivative(MappedGridOperators::yyDerivative,v,uyy,I1,I2,I3,N);

    }
    if( mg.numberOfDimensions()>2 )
    {
      uz.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));

      uxz.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));
      uyz.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));
      uzz.redim(v.dimension(0),v.dimension(1),v.dimension(2),v.dimension(3));

      op.derivative(MappedGridOperators::zDerivative ,v,uz ,I1,I2,I3,N);
      op.derivative(MappedGridOperators::xzDerivative,v,uxz,I1,I2,I3,N);
      op.derivative(MappedGridOperators::yzDerivative,v,uyz,I1,I2,I3,N);
      op.derivative(MappedGridOperators::zzDerivative,v,uzz,I1,I2,I3,N);

    }
    


    #ifndef USE_PPP
      uu.redim(I1,I2,I3,Range(uc,uc+mg.numberOfDimensions()-1));
    #else
      uu.partition(mg.getPartition());
      uu.redim(v.dimension(0),v.dimension(1),v.dimension(2),Range(uc,uc+mg.numberOfDimensions()-1));
    #endif


    RealArray & ad = parameters.dbase.get<RealArray >("artificialDiffusion");

    if( mg.numberOfDimensions()==1 )
    {
      //  ---------------------------------------     
      //  ---1D : Evaluate the equations --------
      //  ---------------------------------------     
      if( gridIsMoving )
	uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-U(uc);
      else
	uu(I1,I2,I3,uc)=-U(uc);

      where( mg.mask()(I1,I2,I3) != 0 ) // do < 0 points for moving grids, exposed points
      {
	ut(I1,I2,I3,rc)= UU(uc)*UX(rc) - U(rc)*(UX(uc));
      
	if( nuRho!=0. )
	  ut(I1,I2,I3,rc)+=nuRho*(UXX(rc));
	if( avr!=0. )
	  ut(I1,I2,I3,rc)+=avr*(-2.*U(rc) + u(I1+1,I2,I3,rc)+u(I1-1,I2,I3,rc) );
      
	ut(I1,I2,I3,uc)= UU(uc)*UX(uc) - Rg*U(tc)/U(rc)*UX(rc) - Rg*UX(tc)   
	  +(mu/U(rc))*(a43*UXX(uc));  // *** could precompute mu/rho ********
	
	ut(I1,I2,I3,tc)= UU(uc)*UX(tc) - gm1*U(tc)*(UX(uc))  
	  +(gm1/U(rc))*( kThermal*(UXX(tc))
			 + mubRg*( a43*( SQR(UX(uc)) )) );
      }
    }
    else if( mg.numberOfDimensions()==2 )
    {
      //  ---------------------------------------     
      //  ---2D : Evaluate the equations --------
      //  ---------------------------------------     
      if( gridIsMoving )
      {
	uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-U(uc);
	uu(I1,I2,I3,vc)=gridVelocity(I1,I2,I3,1)-U(vc);
      }
      else
      {
	uu(I1,I2,I3,uc)=-U(uc);
	uu(I1,I2,I3,vc)=-U(vc);
      }

      const realArray & muOverRho = evaluate(mu/U(rc));
      where( mg.mask()(I1,I2,I3) != 0 ) // do < 0 points for moving grids, exposed points
      {
	ut(I1,I2,I3,rc)= UU(uc)*UX(rc) + UU(vc)*UY(rc) - U(rc)*(UX(uc)+UY(vc));
      
	if( nuRho!=0. )
	  ut(I1,I2,I3,rc)+=nuRho*(UXX(rc)+UYY(rc));

	ut(I1,I2,I3,uc)= UU(uc)*UX(uc) + UU(vc)*UY(uc) - Rg*U(tc)/U(rc)*UX(rc) - Rg*UX(tc)   
	  +muOverRho*(a43*UXX(uc)+UYY(uc)+a13*UXY(vc));  
	
	ut(I1,I2,I3,vc)= UU(uc)*UX(vc) + UU(vc)*UY(vc) - Rg*U(tc)/U(rc)*UY(rc) - Rg*UY(tc)   
	  +muOverRho*(UXX(vc)+a43*UYY(vc)+a13*UXY(uc));
	
	ut(I1,I2,I3,tc)= UU(uc)*UX(tc) + UU(vc)*UY(tc) - gm1*U(tc)*(UX(uc)+UY(vc))  
	  +(gm1/U(rc))*( kThermal*(UXX(tc)+UYY(tc))
			 + mubRg*( a43*( SQR(UX(uc))-UX(uc)*UY(vc) +SQR(UY(vc)))+SQR(UX(vc)+UY(uc)) ) );
	
      }
      
      Index J1,J2,J3;
      // getIndex(mg.extendedIndexRange(),J1,J2,J3);
      getIndex(mg.indexRange(),J1,J2,J3);
      // J1=I1, J2=I2, J3=I3;

      for( int m=0; m<numberOfComponents; m++ )
      {
	// second order dissipation:
	if( true )
	{
	  if( ad(m)!=0. )
	    ut(I1,I2,I3,m)+=ad(m)*(-4.*U(m)      
				   + u(I1+1,I2,I3,m)+u(I1-1,I2,I3,m)      
				   + u(I1,I2+1,I3,m)+u(I1,I2-1,I3,m)  );

	}
	else
	{
	  // fourth-order dissipation

	  if(  ad(m)!=0. )
	  {
	    ut(J1,J2,J3,m)+= ad(m)*(-12.*u(J1,J2,J3,m)      
				    +4.*(u(J1+1,J2,J3,m)+u(J1-1,J2,J3,m) 
					 +u(J1,J2+1,J3,m)+u(J1,J2-1,J3,m))
				    -(u(J1+2,J2,J3,m)+u(J1-2,J2,J3,m)      
				      +u(J1,J2+2,J3,m)+u(J1,J2-2,J3,m))  );
	  }
	}

      }
    }
    else if( mg.numberOfDimensions()==3 )
    {
      //  ---------------------------------------     
      //  ---3D : Evaluate the equations --------
      //  ---------------------------------------     
      if( gridIsMoving )
      {
	uu(I1,I2,I3,uc)=gridVelocity(I1,I2,I3,0)-U(uc);
	uu(I1,I2,I3,vc)=gridVelocity(I1,I2,I3,1)-U(vc);
	uu(I1,I2,I3,wc)=gridVelocity(I1,I2,I3,2)-U(wc);
      }
      else
      {
	uu(I1,I2,I3,uc)=-U(uc);
	uu(I1,I2,I3,vc)=-U(vc);
	uu(I1,I2,I3,wc)=-U(wc);
      }
      where( mg.mask()(I1,I2,I3) != 0 ) // **** fix this ***
      {
	ut(I1,I2,I3,rc)=UU(uc)*UX(rc) + UU(vc)*UY(rc) + UU(wc)*UZ(rc) - U(rc)*(UX(uc)+UY(vc)+UZ(wc));
	if( nuRho>0. )
	  ut(I1,I2,I3,rc)+=nuRho*(UXX(rc)+UYY(rc)+UZZ(rc));
	if( avr>0. )
	  ut(I1,I2,I3,rc)+=avr*(  -4.*U(rc)      
				  + u(I1+1,I2,I3,rc)+u(I1-1,I2,I3,rc)      
				  + u(I1,I2+1,I3,rc)+u(I1,I2-1,I3,rc)  );
	ut(I1,I2,I3,uc)=
	  UU(uc)*UX(uc) + UU(vc)*UY(uc) + UU(wc)*UZ(uc)      
	  -Rg*U(tc)/U(rc)*UX(rc) - Rg*UX(tc)   
	  +(mu/U(rc))*(a43*UXX(uc)+UYY(uc)+UZZ(uc)
		       +a13*(UXY(vc)+UXZ(wc)));
	
	ut(I1,I2,I3,vc)=
	  UU(uc)*UX(vc) + UU(vc)*UY(vc) + UU(wc)*UZ(vc)
	  -Rg*U(tc)/U(rc)*UY(rc) 	-Rg*UY(tc)   
	  +(mu/U(rc))*(UXX(vc)+a43*UYY(vc)+UZZ(vc)
		       +a13*(UXY(uc)+UYZ(wc)));

	ut(I1,I2,I3,wc)=
	  UU(uc)*UX(wc) + UU(vc)*UY(wc) + UU(wc)*UZ(wc)
	  -Rg*U(tc)/U(rc)*UZ(rc) -Rg*UZ(tc)   
	  +(mu/U(rc))*(UXX(wc)+UYY(wc)+a43*UZZ(wc)
		       +a13*(UXZ(uc)+UYZ(vc)));
	
	ut(I1,I2,I3,tc)=
	  UU(uc)*UX(tc) + UU(vc)*UY(tc) + UU(wc)*UZ(tc)
	  -gm1*U(tc)*(UX(uc)+UY(vc)+UZ(wc))  
	  +(gm1/U(rc))*( kThermal*(UXX(tc)+UYY(tc)+UZZ(tc))
			 + mubRg*( 
			   a43*( UX(uc)*(UX(uc)-UY(vc))
				 +       UY(vc)*(UY(vc)-UZ(wc))+SQR(UZ(wc)) )
			   +   2.*(UY(uc)*UX(vc)+UZ(uc)*UX(wc)
				   +       UZ(vc)*UY(wc))
			   + SQR(UY(uc))+SQR(UZ(uc))+SQR(UX(vc))
			   + SQR(UZ(vc))+SQR(UX(wc))+SQR(UY(wc))
			   )
	    );
      }
    }
  }
  
  // add gravity if it is on.
  int axis;
  getIndex(mg.indexRange(),I1,I2,I3);
  //kkc 071003  getIndex(mg.dimension(),I1,I2,I3);
  const real reynoldsNumber = parameters.dbase.get<real >("reynoldsNumber");
  const real prandtlNumber = parameters.dbase.get<real >("prandtlNumber");
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    if( gravity[axis]!=0. )
    {
      if( pdeVariation==CnsParameters::conservativeWithArtificialDissipation ||
          pdeVariation==CnsParameters::conservativeGodunov )
        ut(I1,I2,I3,uc+axis)+= gravity[axis]*u(I1,I2,I3,rc);   // conservative: add rho*g
      else
	{
	  ut(I1,I2,I3,uc+axis)+= gravity[axis];                  // non-conservative: add g
	}
    }
  }

  // add forcing terms
  if ( parameters.dbase.get<Parameters::ForcingType >("forcingType")==Parameters::showfileForcing )
    {
      assert(parameters.dbase.get<realCompositeGridFunction* >("forcingFunction"));
      realArray &ff = (*parameters.dbase.get<realCompositeGridFunction* >("forcingFunction"))[grid];
      ut(I1,I2,I3,rc) += ff(I1,I2,I3,rc);
      
      if ( pdeVariation==CnsParameters::nonConservative )
	{
	  for ( int vv=uc; vv<=max(vc,wc); vv++ )
	    {// interpret the forcing as a drag coefficient??? 
	      ut(I1,I2,I3,vv) += .5*ff(I1,I2,I3,vv)*pow(u(I1,I2,I3,vv),2);
	    }
	}
      else
	{
	  for ( int vv=uc; vv<=max(vc,wc); vv++ )
	    {// interpret the forcing as a drag coefficient???
	      ut(I1,I2,I3,vv) += .5*ff(I1,I2,I3,vv)*pow(u(I1,I2,I3,vv),2)*u(I1,I2,I3,rc);
	    }
	}

      ut(I1,I2,I3,tc) += ff(I1,I2,I3,tc);
    }

  // *********** *wdh* 060714 this should not be here ************
//  realPartOfTimeSteppingEigenvalue=Parameters::getMaxValue(realPartOfTimeSteppingEigenvalue); 
//  imaginaryPartOfTimeSteppingEigenvalue=Parameters::getMaxValue(imaginaryPartOfTimeSteppingEigenvalue); 

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetUt"))+=getCPU()-cpu0;

  addForcing(dvdt,v,iparam,rparam,dvdtImplicit);

  return 0;

} // end getUt





//.....equation 1
//  r_t + u r_x + v r_y + w r_z + (u_x+v_y+w_z)r - avr Delta r
#define uv(m)   e   (mg,I1,I2,I3,m,t)
#define uvt(m)  e.t (mg,I1,I2,I3,m,t)
#define uvx(m)  e.x (mg,I1,I2,I3,m,t)
#define uvy(m)  e.y (mg,I1,I2,I3,m,t)
#define uvz(m)  e.z (mg,I1,I2,I3,m,t)
#define uvxx(m) e.xx(mg,I1,I2,I3,m,t)
#define uvxy(m) e.xy(mg,I1,I2,I3,m,t)
#define uvxz(m) e.xz(mg,I1,I2,I3,m,t)
#define uvyy(m) e.yy(mg,I1,I2,I3,m,t)
#define uvyz(m) e.yz(mg,I1,I2,I3,m,t)
#define uvzz(m) e.zz(mg,I1,I2,I3,m,t)

#define ft30(t) uvt(rc) \
      +uv(uc)*uvx(rc)+uv(vc)*uvy(rc)+uv(wc)*uvz(rc)  \
      +uv(rc)*(uvx(uc)+uvy(vc)+uvz(wc))  \
      -nuRho*(uvxx(rc)+uvyy(rc)+uvzz(rc)) 
//.....equation 2
//  u_t + u u_x + v u_y + w u_z + p_x/r
//       - (mu/r)[Delta u +(1/3) (div u)_x] =
#define ft31(t) uvt(uc)  \
      +uv(uc)*uvx(uc)+uv(vc)*uvy(uc)+uv(wc)*uvz(uc)  \
      +Rg*uv(tc)/uv(rc)*uvx(rc)+Rg*uvx(tc)  \
      -(mu/uv(rc))*(a43*uvxx(uc)+uvyy(uc)+uvzz(uc)+a13*(uvxy(vc)+uvxz(wc)) )
//......equation 3
//   v_t + u v_x + v v_y + w v_z + p_y/r
//        - (mu/r)[Delta v +(1/3) (div u)_y] =
#define ft32(t) uvt(vc)  \
      +uv(uc)*uvx(vc)+uv(vc)*uvy(vc)+uv(wc)*uvz(vc)  \
      +Rg*uv(tc)/uv(rc)*uvy(rc)+Rg*uvy(tc)  \
      -(mu/uv(rc))*(uvxx(vc)+a43*uvyy(vc)+uvzz(vc)+a13*(uvxy(uc)+uvyz(wc)))
//......equation 4
//   w_t + u w_x + v w_y + w w_z + p_z/r
//        - (mu/r)[Delta w +(1/3) (div u)_z] =
#define ft33(t) uvt(wc)  \
      +uv(uc)*uvx(wc)+uv(vc)*uvy(wc)+uv(wc)*uvz(wc)  \
      +Rg*uv(tc)/uv(rc)*uvz(rc)+Rg*uvz(tc)  \
      -(mu/uv(rc))*(uvxx(wc)+uvyy(wc)+a43*uvzz(wc)+a13*(uvxz(uc)+uvyz(vc)))
//......equation 5
//   T_t + u T_x + v T_y + w T_z +(gamma-1) T div u
//       - (gamma-1) (kThermal/r) Delta T  - (gamma-1) mu/(Rg r) phi =
//       - (gamma-1) (kThermal/r) Delta T  - (gamma-1) mu/(Rg r) phi =
#define ft34(t) uvt(tc)  \
      +uv(uc)*uvx(tc)+uv(vc)*uvy(tc)+uv(wc)*uvz(tc)  \
      +gm1*uv(tc)*(uvx(uc)+uvy(vc)+uvz(wc))  \
      -(gm1/uv(rc))*( kThermal*(uvxx(tc)+uvyy(tc)+uvzz(tc))  \
           +mubRg*(  \
         a43*( uvx(uc)*(uvx(uc)-uvy(vc))+uvy(vc)*(uvy(vc)-uvz(wc))+SQR(uvz(wc)) )  \
         +2.*(uvy(uc)*uvx(vc)+uvz(uc)*uvx(wc)+uz(vc)*uvy(wc))  \
         +SQR(uvy(uc))+SQR(uvz(uc))+SQR(uvx(vc))+SQR(uvz(vc))+SQR(uvx(wc))+SQR(uvy(wc)) ) )


// ===================================================================================================================
/// \brief Add body forcing to du/dt. 
/// \details For flows in the twilight-zone, add twilight-zone forcing.
///
/// \param dvdt (input/output) : add the explicit body forcing here.
/// \param u (input) : current solution.
/// \param iparam (input) : integer parameters
/// \param rparam (input) : real parameters
/// \param dvdtImplicit (input/output) : add the implicit body forcing here.
// ===================================================================================================================
void Cgcns::
addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u,  int iparam[], real rparam[],
	   realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
           realMappedGridFunction *referenceFrameVelocity /* =NULL */ )
{
  if( !parameters.dbase.get<bool >("twilightZoneFlow") && !parameters.dbase.get<bool >("turnOnBodyForcing") )
  {
    // No forcing to add 
    return;
  }

  real cpu0=getCPU();

  FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
  const CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE>("pde");
  const CnsParameters::PDEVariation & pdeVariation = parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation");  const CnsParameters::GodunovVariation & conservativeGodunovMethod = 
                           parameters.dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");


  if( pde==CnsParameters::compressibleMultiphase )
  {
    if( debug() & 2 )
      printF(">>> addForcingCNS: Do nothing for compressibleMultiphase <<<<\n");

    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-cpu0;
    return;
  }

  // *wdh* 081207 MappedGrid & mg = *dvdt.getMappedGrid();
  MappedGrid & mg = *u.getMappedGrid();   // use this for moving grid cases since dvdt may not have the correct grid

  const real & t0=rparam[0];
  real t         =rparam[1];          // this is realy tForce
  const real & tImplicit=rparam[2];
  const int & grid = iparam[0];

  if( debug() & 8 )
  {
    fprintf(pDebugFile," ====== addForcingCNS t=%9.3e TZ=%i pdeVariation=%i ===== \n",t0,
	    (int)parameters.dbase.get<bool >("twilightZoneFlow"),(int)pdeVariation);
  }
  

  Index I1,I2,I3;
  realArray & ut = dvdt;
  OV_GET_SERIAL_ARRAY(real,ut,utLocal);

  // --- Add on any user defined forcing ---

  if( parameters.dbase.get<bool >("turnOnBodyForcing") )
  {

    // The body forcing will have already been computed.
    if( false )
      printF("Cgcns::addForcing:turnOnBodyForcing=%i\n",(int) parameters.dbase.get<bool >("turnOnBodyForcing"));


    assert( parameters.dbase.get<realCompositeGridFunction* >("bodyForce")!=NULL );
    realCompositeGridFunction & bodyForce = *(parameters.dbase.get<realCompositeGridFunction* >("bodyForce"));

    //     // Compute the user defined forcing:
    //     userDefinedForcing( userDefinedForce[grid], u, iparam, rparam );
  
    // Add the user defined force onto dvdt:
    OV_GET_SERIAL_ARRAY(real,bodyForce[grid],bodyForceLocal);

    // printF("addForcing: add body force: (min,max)=(%g,%g)\n",min(bodyForceLocal),max(bodyForceLocal));

    getIndex(mg.gridIndexRange(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(ut,utLocal,I1,I2,I3); 
    
    const Range & Rt = parameters.dbase.get<Range >("Rt");       // time dependent components

    if( false )
      printF("cns:getForcing: grid=%i, utLocal=[%i,%i][%i,%i], bodyForceLocal=[%i,%i][%i,%i]\n",grid,
	     utLocal.getBase(0),utLocal.getBound(0),
	     utLocal.getBase(1),utLocal.getBound(1),
	     bodyForceLocal.getBase(0),bodyForceLocal.getBound(0),
	     bodyForceLocal.getBase(1),bodyForceLocal.getBound(1));

    utLocal(I1,I2,I3,Rt) += bodyForceLocal(I1,I2,I3,Rt);

  }

  // *wdh* 2013/08/31 -- put this after assigning the bodyForce
  if( !parameters.dbase.get<bool >("twilightZoneFlow") || 
      conservativeGodunovMethod==CnsParameters::multiComponentVersion ||
      (pde==CnsParameters::compressibleNavierStokes &&
       pdeVariation==CnsParameters::conservativeGodunov) )
  {
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-cpu0;
    return;
  }


  const int & rc = parameters.dbase.get<int >("rc");
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");
  const real & mu = parameters.dbase.get<real >("mu");
  const real & gamma = parameters.dbase.get<real >("gamma");
  const real & kThermal = parameters.dbase.get<real >("kThermal");
  const real & Rg = parameters.dbase.get<real >("Rg");
  const real & nuRho = parameters.dbase.get<real >("nuRho");
  // const real & avr = parameters.dbase.get<real >("avr");
  // const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

  getIndex(mg.extendedIndexRange(),I1,I2,I3);

  const real a43=4./3., a23=2./3., a13=1./3.;
  real gm1,mubRg;

  // ---add forcing for twlight-zone flow---
  gm1=gamma-1.;
  mubRg=mu/Rg;

  getIndex( mg.gridIndexRange(),I1,I2,I3);
  const int numberOfDimensions= mg.numberOfDimensions();

  mg.update(MappedGrid::THEcenter);
  realArray & x= mg.center();
#ifdef USE_PPP
  realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
#else
  const realSerialArray & xLocal = x;
#endif  

  if( debug() & 8 )
  {
    if( parameters.gridIsMoving(grid) )
      display(xLocal,sPrintF("cns:addForcing:moving-grid: xLocal adding TZ forcing, grid=%i t=%9.4e",grid,t),pDebugFile,"%12.8f ");
  }

  bool useOpt=true;
  if( useOpt )
  {

    // loop bounds for this boundary:
    int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2];
    #ifdef USE_PPP
      n1a=max(I1.getBase(),utLocal.getBase(0)); n1b=min(I1.getBound(),utLocal.getBound(0));
      n2a=max(I2.getBase(),utLocal.getBase(1)); n2b=min(I2.getBound(),utLocal.getBound(1));
      n3a=max(I3.getBase(),utLocal.getBase(2)); n3b=min(I3.getBound(),utLocal.getBound(2));
    #else
      n1a=I1.getBase(); n1b=I1.getBound();
      n2a=I2.getBase(); n2b=I2.getBound();
      n3a=I3.getBase(); n3b=I3.getBound();
    #endif


    if( n1a>n1b || n2a>n2b || n3a>n3b ) return;

    I1=Range(n1a,n1b);
    I2=Range(n2a,n2b);
    I3=Range(n3a,n3b);



    bool isRectangular=false; // do this for now

    if( numberOfDimensions==1 )
    {
      assert( pdeVariation!=CnsParameters::conservativeWithArtificialDissipation );

      // this is new:
      realSerialArray r0(I1,I2,I3),r0t(I1,I2,I3),r0x(I1,I2,I3),r0xx(I1,I2,I3);
      realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0xx(I1,I2,I3);
      realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0xx(I1,I2,I3);
      realSerialArray t0(I1,I2,I3),t0t(I1,I2,I3),t0x(I1,I2,I3),t0xx(I1,I2,I3);
	

      e.gd( r0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,rc,t);
      e.gd( r0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,rc,t);
      e.gd( r0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,rc,t);
      e.gd( r0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,rc,t);

      e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
      e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
      e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
      e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);

      e.gd( t0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);
      e.gd( t0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
      e.gd( t0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
      e.gd( t0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);


      utLocal(I1,I2,I3,rc)+=r0t +u0*r0x+r0*u0x-nuRho*r0xx;
      utLocal(I1,I2,I3,uc)+=u0t +u0*u0x +Rg*(t0/r0*r0x+t0x)-(mu/r0)*(a43*u0xx);
      utLocal(I1,I2,I3,tc)+=v0t +u0*t0x +gm1*t0*u0x -(gm1/r0)*( kThermal*t0xx +mubRg*( a43*(SQR(u0x)) ));
    }
    else if( numberOfDimensions==2 )
    {

      // evaluate the exact solution and it's derivatives 

      realSerialArray r0(I1,I2,I3),r0t(I1,I2,I3),r0x(I1,I2,I3),r0y(I1,I2,I3);
      realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3);
      realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3);
      realSerialArray t0(I1,I2,I3),t0t(I1,I2,I3),t0x(I1,I2,I3),t0y(I1,I2,I3);
	

      e.gd( r0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,rc,t);
      e.gd( r0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,rc,t);
      e.gd( r0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,rc,t);
      e.gd( r0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,rc,t);

      e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
      e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
      e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
      e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);

      e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
      e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
      e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
      e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);

      e.gd( t0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);
      e.gd( t0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
      e.gd( t0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
      e.gd( t0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);

	  
      if( pdeVariation==CnsParameters::conservativeWithArtificialDissipation )
      {
	// conservative form (but TZ flow is primitive variables)

	realSerialArray rhot;
	rhot =r0t +u0*r0x+v0*r0y+r0*(u0x+v0y);

	utLocal(I1,I2,I3,rc)+=rhot;
	utLocal(I1,I2,I3,uc)+=r0*( u0t +u0*u0x+v0*u0y ) + Rg*(t0*r0x+t0x*r0) + rhot*u0;
	utLocal(I1,I2,I3,vc)+=r0*( v0t +u0*v0x+v0*v0y ) + Rg*(t0*r0y+t0y*r0) + rhot*v0;

	realSerialArray uSq; uSq= u0*u0 + v0*v0;
	  
	const real rGamma = Rg*gamma/(gamma-1.);
	realSerialArray h; h = rGamma*t0 + .5*uSq;

	utLocal(I1,I2,I3,tc)+=( r0*( Rg/(gamma-1.)*t0t + u0*u0t + v0*v0t) +
				r0t*(Rg/(gamma-1.)*t0  + .5*uSq ) + 
				(u0x+v0y)*r0*h +
				u0*( r0*( rGamma*t0x + u0*u0x + v0*v0x  ) + r0x*h ) +
				v0*( r0*( rGamma*t0y + u0*u0y + v0*v0y  ) + r0y*h ) );

	if( mu>0. )
	{
	  realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0xy(I1,I2,I3);
	  realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0xy(I1,I2,I3);
	  
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	  e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	  e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	  e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);
	  e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);


	  const realSerialArray & t11 = evaluate( a43*u0x-a23*v0y);
	  const realSerialArray & t12 = evaluate( u0y+v0x);
	  const realSerialArray & t22 = evaluate( a43*v0y-a23*u0x);
	  const realSerialArray & t11x= evaluate( a43*u0xx-a23*v0xy);
	  const realSerialArray & t12x= evaluate( u0xy+v0xx);
	  const realSerialArray & t12y= evaluate( u0yy+v0xy);
	  const realSerialArray & t22y= evaluate( a43*v0yy-a23*u0xy);

	  utLocal(I1,I2,I3,uc)-=mu*(t11x+t12y);
	  utLocal(I1,I2,I3,vc)-=mu*(t12x+t22y);

	  utLocal(I1,I2,I3,tc)-= mu*( u0*t11x+v0*t12x +u0x*t11+v0x*t12+
				      u0*t12y+v0*t22y +u0y*t12+v0y*t22  );
	}
	if( kThermal>0. )
	{
	  realSerialArray t0xx(I1,I2,I3),t0yy(I1,I2,I3); 

	  e.gd( t0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( t0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	  utLocal(I1,I2,I3,tc)-=kThermal*Rg*(t0xx+t0yy);
	}
	  
      }
      else
      {
	// non-conservative form
	realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0xy(I1,I2,I3);
	realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0xy(I1,I2,I3);
	  
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);

	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);

	realSerialArray t0xx(I1,I2,I3),t0yy(I1,I2,I3); 
	e.gd( t0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	e.gd( t0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);

	realSerialArray r0xx(I1,I2,I3),r0yy(I1,I2,I3); 
	e.gd( r0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,rc,t);
	e.gd( r0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,rc,t);

	utLocal(I1,I2,I3,rc)+=r0t +u0*r0x+v0*r0y+r0*(u0x+v0y)-nuRho*(r0xx+r0yy);
	utLocal(I1,I2,I3,uc)+=u0t +u0*u0x+v0*u0y
	  +Rg*(t0/r0*r0x+t0x)-(mu/r0)*(a43*u0xx+u0yy+a13*v0xy);
	utLocal(I1,I2,I3,vc)+=v0t +u0*v0x+v0*v0y             
	  +Rg*(t0/r0*r0y+t0y)-(mu/r0)*(v0xx+a43*v0yy+a13*u0xy);

	utLocal(I1,I2,I3,tc)+=t0t +u0*t0x+v0*t0y +gm1*t0*(u0x+v0y)
	  -(gm1/r0)*(  kThermal*(t0xx+t0yy) +mubRg*( a43*( SQR(u0x)-u0x*v0y+SQR(v0y) ) +SQR(v0x+u0y) ) );

	if ( parameters.dbase.get<bool >("axisymmetricProblem") )
	{
	  const realSerialArray &rad = xLocal(I1,I2,I3,1);

	  utLocal(I1,I2,I3,rc) += r0*v0/rad;
	  utLocal(I1,I2,I3,uc) += -(mu/r0)*( u0y/rad + a13*v0x/rad );
	  utLocal(I1,I2,I3,vc) += -(mu/r0)*( a43*(v0y - v0/rad)/rad );
	  utLocal(I1,I2,I3,tc) += +gm1*t0*v0/rad -(gm1/r0)*( kThermal*t0y/rad + mubRg*( a43*( v0*v0/rad-v0*(u0x+v0y) )/rad) );

	  if ( parameters.dbase.get<bool >("axisymmetricWithSwirl") )
	  {
	    realSerialArray w0(I1,I2,I3), w0t(I1,I2,I3), w0x(I1,I2,I3), w0y(I1,I2,I3),w0xx(I1,I2,I3),w0yy(I1,I2,I3);
	    e.gd( w0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,wc,t);
	    e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t);
	    e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
	    e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
	    e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
	    e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);

	    //nothing to add		utLocal(I1,I2,I3,rc) += ;
	    //nothing to add		utLocal(I1,I2,I3,uc) += ;
	    utLocal(I1,I2,I3,vc) += -w0*w0/rad;
	    utLocal(I1,I2,I3,wc) += w0t + u0*w0x + v0*w0y + v0*w0/rad - (mu/r0)*( w0xx + w0yy + ( w0y-w0/rad)/rad  ) ;
	    utLocal(I1,I2,I3,tc) += -(gm1/r0)*( mubRg*( w0x*w0x + w0y*w0y - 2.*w0y*w0/rad + w0*w0/rad/rad) );
	  }
	}
      }
    }
    else if( numberOfDimensions==3 )
    {

      realSerialArray r0(I1,I2,I3),r0t(I1,I2,I3),r0x(I1,I2,I3),r0y(I1,I2,I3),r0z(I1,I2,I3);
      realSerialArray u0(I1,I2,I3),u0t(I1,I2,I3),u0x(I1,I2,I3),u0y(I1,I2,I3),u0z(I1,I2,I3);
      realSerialArray v0(I1,I2,I3),v0t(I1,I2,I3),v0x(I1,I2,I3),v0y(I1,I2,I3),v0z(I1,I2,I3);
      realSerialArray w0(I1,I2,I3),w0t(I1,I2,I3),w0x(I1,I2,I3),w0y(I1,I2,I3),w0z(I1,I2,I3);
      realSerialArray t0(I1,I2,I3),t0t(I1,I2,I3),t0x(I1,I2,I3),t0y(I1,I2,I3),t0z(I1,I2,I3);
	

      e.gd( r0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,rc,t);
      e.gd( r0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,rc,t);
      e.gd( r0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,rc,t);
      e.gd( r0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,rc,t);
      e.gd( r0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,rc,t);

      e.gd( u0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,uc,t);
      e.gd( u0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
      e.gd( u0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,uc,t);
      e.gd( u0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,uc,t);
      e.gd( u0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,uc,t);

      e.gd( v0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,vc,t);
      e.gd( v0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,vc,t);
      e.gd( v0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,vc,t);
      e.gd( v0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,vc,t);
      e.gd( v0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,vc,t);

      e.gd( w0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,wc,t);
      e.gd( w0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,wc,t);
      e.gd( w0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,wc,t);
      e.gd( w0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,wc,t);
      e.gd( w0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,wc,t);

      e.gd( t0  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,tc,t);
      e.gd( t0t ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,tc,t);
      e.gd( t0x ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,tc,t);
      e.gd( t0y ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,tc,t);
      e.gd( t0z ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,tc,t);

	  
      if( pdeVariation==CnsParameters::conservativeWithArtificialDissipation )
      {
	// conservative form (but TZ flow is primitive variables)

	realSerialArray rhot;
	rhot =r0t +u0*r0x+v0*r0y+w0*r0z +r0*(u0x+v0y+w0z);

	utLocal(I1,I2,I3,rc)+=rhot;
	utLocal(I1,I2,I3,uc)+=r0*( u0t +u0*u0x+v0*u0y+w0*u0z ) + Rg*(t0*r0x+t0x*r0) + rhot*u0;
	utLocal(I1,I2,I3,vc)+=r0*( v0t +u0*v0x+v0*v0y+w0*v0z ) + Rg*(t0*r0y+t0y*r0) + rhot*v0;
	utLocal(I1,I2,I3,wc)+=r0*( w0t +u0*w0x+v0*w0y+w0*w0z ) + Rg*(t0*r0z+t0z*r0) + rhot*w0;

	realSerialArray uSq; uSq= u0*u0 + v0*v0 + w0*w0;
	  
	const real rGamma = Rg*gamma/(gamma-1.);
	realSerialArray h; h = rGamma*t0 + .5*uSq;

	utLocal(I1,I2,I3,tc)+=( r0*( Rg/(gamma-1.)*t0t + u0*u0t + v0*v0t +w0*w0t ) +
				r0t*(Rg/(gamma-1.)*t0  + .5*uSq ) + 
				(u0x+v0y+w0z)*r0*h +
				u0*( r0*( rGamma*t0x + u0*u0x + v0*v0x + w0*w0x  ) + r0x*h ) +
				v0*( r0*( rGamma*t0y + u0*u0y + v0*v0y + w0*w0y  ) + r0y*h ) + 
				w0*( r0*( rGamma*t0z + u0*u0z + v0*v0z + w0*w0z  ) + r0z*h ) );


	if( mu>0. )
	{
	  realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0xy(I1,I2,I3);
	  realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0xy(I1,I2,I3);
	  realSerialArray w0xx(I1,I2,I3),w0yy(I1,I2,I3),w0xy(I1,I2,I3);
	  
	  realSerialArray u0xz(I1,I2,I3),u0yz(I1,I2,I3),u0zz(I1,I2,I3);
	  realSerialArray v0xz(I1,I2,I3),v0yz(I1,I2,I3),v0zz(I1,I2,I3);
	  realSerialArray w0xz(I1,I2,I3),w0yz(I1,I2,I3),w0zz(I1,I2,I3);
	  
	  e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	  e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
	  e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);
	  e.gd( u0xz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,uc,t);
	  e.gd( u0yz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,uc,t);
	  e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,t);

	  e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	  e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);
	  e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);
	  e.gd( v0xz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,vc,t);
	  e.gd( v0yz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,vc,t);
	  e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,t);

	  e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
	  e.gd( w0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,wc,t);
	  e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);
	  e.gd( w0xz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,wc,t);
	  e.gd( w0yz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,wc,t);
	  e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,t);



	  const realSerialArray & t11 = evaluate( a43*u0x-a23*(v0y+w0z));
	  const realSerialArray & t12 = evaluate( u0y+v0x);
	  const realSerialArray & t13 = evaluate( u0z+w0x);
	  const realSerialArray & t22 = evaluate( a43*v0y-a23*(u0x+w0z));
	  const realSerialArray & t23 = evaluate( v0z+w0y);
	  const realSerialArray & t33 = evaluate( a43*w0z-a23*(u0x+v0y));

	  const realSerialArray & t11x= evaluate( a43*u0xx-a23*v0xy);
	  const realSerialArray & t12x= evaluate( u0xy+v0xx);
	  const realSerialArray & t12y= evaluate( u0yy+v0xy);
	  const realSerialArray & t22y= evaluate( a43*v0yy-a23*u0xy);
	  const realSerialArray & t13x= evaluate( u0xz+w0xx);
	  const realSerialArray & t13z= evaluate( u0zz+w0xz);
	  const realSerialArray & t23x= evaluate( v0xz+w0xy);
	  const realSerialArray & t23y= evaluate( v0yz+w0yy);
	  const realSerialArray & t23z= evaluate( v0zz+w0yz);
	  const realSerialArray & t33z= evaluate( a43*w0zz-a23*(u0xz+v0yz));

	  utLocal(I1,I2,I3,uc)-=mu*(t11x+t12y+t13z);
	  utLocal(I1,I2,I3,vc)-=mu*(t12x+t22y+t23z);
	  utLocal(I1,I2,I3,wc)-=mu*(t13x+t23y+t33z);

	  utLocal(I1,I2,I3,tc)-= mu*( u0*t11x+v0*t12x+w0*t13x +u0x*t11+v0x*t12+w0x*t13 +
				 u0*t12y+v0*t22y+w0*t23x +u0y*t12+v0y*t22+w0y*t23 +
				 u0*t13z+v0*t23z+w0*t33z +u0z*t13+v0z*t23+w0z*t33  );


	}
	if( kThermal>0. )
	{
	  realSerialArray t0xx(I1,I2,I3),t0yy(I1,I2,I3),t0zz(I1,I2,I3); 

	  e.gd( t0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	  e.gd( t0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	  e.gd( t0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,tc,t);
	  utLocal(I1,I2,I3,tc)-=kThermal*Rg*(t0xx+t0yy+t0zz);
	}
	  
      }
      else
      {
	// non-conservative form
	realSerialArray u0xx(I1,I2,I3),u0yy(I1,I2,I3),u0xy(I1,I2,I3);
	realSerialArray v0xx(I1,I2,I3),v0yy(I1,I2,I3),v0xy(I1,I2,I3);
	realSerialArray w0xx(I1,I2,I3),w0yy(I1,I2,I3),w0xy(I1,I2,I3);
	  
	realSerialArray u0xz(I1,I2,I3),u0yz(I1,I2,I3),u0zz(I1,I2,I3);
	realSerialArray v0xz(I1,I2,I3),v0yz(I1,I2,I3),v0zz(I1,I2,I3);
	realSerialArray w0xz(I1,I2,I3),w0yz(I1,I2,I3),w0zz(I1,I2,I3);
	  
	e.gd( u0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,uc,t);
	e.gd( u0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,uc,t);
	e.gd( u0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,uc,t);
	e.gd( u0xz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,uc,t);
	e.gd( u0yz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,uc,t);
	e.gd( u0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,uc,t);

	e.gd( v0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,vc,t);
	e.gd( v0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,vc,t);
	e.gd( v0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,vc,t);
	e.gd( v0xz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,vc,t);
	e.gd( v0yz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,vc,t);
	e.gd( v0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,vc,t);

	e.gd( w0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,wc,t);
	e.gd( w0xy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,wc,t);
	e.gd( w0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,wc,t);
	e.gd( w0xz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,wc,t);
	e.gd( w0yz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,wc,t);
	e.gd( w0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,wc,t);


	realSerialArray t0xx(I1,I2,I3),t0yy(I1,I2,I3),t0zz(I1,I2,I3); 
	e.gd( t0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,tc,t);
	e.gd( t0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,tc,t);
	e.gd( t0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,tc,t);

	realSerialArray r0xx(I1,I2,I3),r0yy(I1,I2,I3),r0zz(I1,I2,I3); 
	e.gd( r0xx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,rc,t);
	e.gd( r0yy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,rc,t);
	e.gd( r0zz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,rc,t);


	utLocal(I1,I2,I3,rc)+=r0t+u0*r0x+v0*r0y+w0*r0z+r0*(u0x+v0y+w0z) -nuRho*(r0xx+r0yy+r0zz);
	utLocal(I1,I2,I3,uc)+=u0t+u0*u0x+v0*u0y+w0*u0z +Rg*t0/r0*r0x+Rg*t0x 
	  -(mu/r0)*(a43*u0xx+u0yy+u0zz+a13*(v0xy+w0xz) );
	  
	utLocal(I1,I2,I3,vc)+=v0t+u0*v0x+v0*v0y+w0*v0z  +Rg*t0/r0*r0y+Rg*t0y  
	  -(mu/r0)*(v0xx+a43*v0yy+v0zz+a13*(u0xy+w0yz));
	  
	utLocal(I1,I2,I3,wc)+=w0t+u0*w0x+v0*w0y+w0*w0z +Rg*t0/r0*r0z+Rg*t0z  
	  -(mu/r0)*(w0xx+w0yy+a43*w0zz+a13*(u0xz+v0yz));



	utLocal(I1,I2,I3,tc)+=t0t+u0*t0x+v0*t0y+w0*t0z  +gm1*t0*(u0x+v0y+w0z) 
	  -(gm1/r0)*( kThermal*(t0xx+t0yy+t0zz) 
		      +mubRg*(  
			a43*( u0x*(u0x-v0y)+v0y*(v0y-w0z)+SQR(w0z) )
			+2.*(u0y*v0x+u0z*w0x+v0z*w0y)  
			+SQR(u0y)+SQR(u0z)+SQR(v0x)+SQR(v0z)+SQR(w0x)+SQR(w0y) ) );



      } // end non-conservative
      
    }  // end numberOfDimensions
      
  } 
  else // ============= OLD WAY no - opt  ===========================
  {
    if( numberOfDimensions==1 )
    {
      // this is new:
      const realArray & r0  = e   (mg,I1,I2,I3,rc,t);
      const realArray & r0x = e.x (mg,I1,I2,I3,rc,t);
      const realArray & r0xx= e.xx(mg,I1,I2,I3,rc,t);

      const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
      const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
      const realArray & u0xx= e.xx(mg,I1,I2,I3,uc,t);

      const realArray & t0  = e   (mg,I1,I2,I3,tc,t);
      const realArray & t0x = e.x (mg,I1,I2,I3,tc,t);
      const realArray & t0xx= e.xx(mg,I1,I2,I3,tc,t);

      ut(I1,I2,I3,rc)+=	e.t(mg,I1,I2,I3,rc,t) +u0*r0x+r0*u0x-nuRho*r0xx;
      ut(I1,I2,I3,uc)+=	e.t(mg,I1,I2,I3,uc,t) +u0*u0x +Rg*(t0/r0*r0x+t0x)-(mu/r0)*(a43*u0xx);
      ut(I1,I2,I3,tc)+=	e.t(mg,I1,I2,I3,tc,t) +u0*t0x +gm1*t0*u0x -(gm1/r0)*( kThermal*t0xx +mubRg*( a43*(SQR(u0x)) ));
    }
    else if( numberOfDimensions==2 )
    {
      const realArray & r0  = e   (mg,I1,I2,I3,rc,t);
      const realArray & r0x = e.x (mg,I1,I2,I3,rc,t);
      const realArray & r0y = e.y (mg,I1,I2,I3,rc,t);

      const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
      const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
      const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
      const realArray & u0xx= e.xx(mg,I1,I2,I3,uc,t);
      const realArray & u0xy= e.xy(mg,I1,I2,I3,uc,t);
      const realArray & u0yy= e.yy(mg,I1,I2,I3,uc,t);

      const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
      const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
      const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
      const realArray & v0xx= e.xx(mg,I1,I2,I3,vc,t);
      const realArray & v0xy= e.xy(mg,I1,I2,I3,vc,t);
      const realArray & v0yy= e.yy(mg,I1,I2,I3,vc,t);

      const realArray & t0  = e   (mg,I1,I2,I3,tc,t);
      const realArray & t0x = e.x (mg,I1,I2,I3,tc,t);
      const realArray & t0y = e.y (mg,I1,I2,I3,tc,t);
      const realArray & t0xx= e.xx(mg,I1,I2,I3,tc,t);
      const realArray & t0yy= e.yy(mg,I1,I2,I3,tc,t);


      if( pdeVariation==CnsParameters::conservativeWithArtificialDissipation )
      {
	// conservative form (but TZ flow is primitive variables)

	const realArray & rhot = evaluate( e.t(mg,I1,I2,I3,rc,t) +u0*r0x+v0*r0y+r0*(u0x+v0y) );
	ut(I1,I2,I3,rc)+=rhot;
	const realArray & ute = evaluate(r0*( e.t(mg,I1,I2,I3,uc,t) +u0*u0x+v0*u0y ) 
					 + Rg*(t0*r0x+t0x*r0) + rhot*u0);

	if( debug() & 16 )
	{
	  const realSerialArray & utLocal = ut.getLocalArray();
	  const realSerialArray & uteLocal = ute.getLocalArray();
	  
	  Range all;
	  display(utLocal(all,all,all,uc),sPrintF("utLocal before adding TZ forcing, t=%9.4e",t),pDebugFile,"%6.4f ");
	  display(uteLocal,sPrintF("uteLocal TZ forcing, t=%9.4e",t),pDebugFile,"%6.4f ");
	}

	ut(I1,I2,I3,uc)+=ute; 
	ut(I1,I2,I3,vc)+=r0*(e.t(mg,I1,I2,I3,vc,t) +u0*v0x+v0*v0y ) + Rg*(t0*r0y+t0y*r0)
	  + rhot*v0;
	// ut(I1,I2,I3,tc)+=	e.t(mg,I1,I2,I3,tc,t) +u0*t0x+v0*t0y +gm1*t0*(u0x+v0y)
	//   -(gm1/r0)*(  kThermal*(t0xx+t0yy) +mubRg*( a43*(SQR(u0x)) -u0x*v0y +SQR(v0y)+SQR(v0x+u0y) ) );

	// E+p = rho[ Rg gamma/(gamma-1) T + u*u ]
	// const realArray & e0  = evaluate( r0*( Rg/(gamma-1.)*t0 + .5*( u0*u0 + v0*v0 ) ) );
	// const realArray & p0 = evaluate( (gamma-1.)*( e0 - .5*r0*( u0*u0 + v0*v0 ) ) );
	const realArray & e0t  = evaluate( r0*( Rg/(gamma-1.)*e.t(mg,I1,I2,I3,tc,t) 
						+ u0*e.t(mg,I1,I2,I3,uc,t) + v0*e.t(mg,I1,I2,I3,vc,t)  )
					   + e.t(mg,I1,I2,I3,rc,t)*(Rg/(gamma-1.)*t0 + .5*(u0*u0 + v0*v0)) );
	const real rGamma = Rg*gamma/(gamma-1.);
	const realArray & ePlusP0  = evaluate( r0*( rGamma*t0 + .5*( u0*u0 + v0*v0 ) ) ); 
	const realArray & ePlusP0x = evaluate( r0*( rGamma*t0x + u0*u0x + v0*v0x  )
					       + r0x*(rGamma*t0 + .5*(u0*u0 + v0*v0)) );
	const realArray & ePlusP0y = evaluate( r0*( rGamma*t0y + u0*u0y + v0*v0y  )
					       + r0y*(rGamma*t0 + .5*(u0*u0 + v0*v0)) );
      
	ut(I1,I2,I3,tc)+=e0t + (u0x+v0y)*ePlusP0+u0*ePlusP0x+v0*ePlusP0y;
	if( mu>0. )
	{

	  const realArray & t11 = evaluate( a43*u0x-a23*v0y);
	  const realArray & t12 = evaluate( u0y+v0x);
	  const realArray & t22 = evaluate( a43*v0y-a23*u0x);
	  const realArray & t11x= evaluate( a43*u0xx-a23*v0xy);
	  const realArray & t12x= evaluate( u0xy+v0xx);
	  const realArray & t12y= evaluate( u0yy+v0xy);
	  const realArray & t22y= evaluate( a43*v0yy-a23*u0xy);

	  ut(I1,I2,I3,uc)-=mu*(t11x+t12y);
	  ut(I1,I2,I3,vc)-=mu*(t12x+t22y);

	  ut(I1,I2,I3,tc)-= mu*( u0*t11x+v0*t12x +u0x*t11+v0x*t12+
				 u0*t12y+v0*t22y +u0y*t12+v0y*t22  );
	}
	if( kThermal>0. )
	  ut(I1,I2,I3,tc)-=kThermal*Rg*(t0xx+t0yy);
      }
      else
      { // non-conservative
	const realArray & r0xx= e.xx(mg,I1,I2,I3,rc,t);
	const realArray & r0yy= e.yy(mg,I1,I2,I3,rc,t);

	ut(I1,I2,I3,rc)+=	e.t(mg,I1,I2,I3,rc,t) +u0*r0x+v0*r0y+r0*(u0x+v0y)-nuRho*(r0xx+r0yy);
	ut(I1,I2,I3,uc)+=	e.t(mg,I1,I2,I3,uc,t) +u0*u0x+v0*u0y
	  +Rg*(t0/r0*r0x+t0x)-(mu/r0)*(a43*u0xx+u0yy+a13*v0xy);
	ut(I1,I2,I3,vc)+=	e.t(mg,I1,I2,I3,vc,t) +u0*v0x+v0*v0y             
	  +Rg*(t0/r0*r0y+t0y)-(mu/r0)*(v0xx+a43*v0yy+a13*u0xy);

	ut(I1,I2,I3,tc)+=	e.t(mg,I1,I2,I3,tc,t) +u0*t0x+v0*t0y +gm1*t0*(u0x+v0y)
	  -(gm1/r0)*(  kThermal*(t0xx+t0yy) +mubRg*( a43*( SQR(u0x)-u0x*v0y+SQR(v0y) ) +SQR(v0x+u0y) ) );
      }
    }
    else if( mg.numberOfDimensions()==3 )
    {
      if( pdeVariation==CnsParameters::conservativeWithArtificialDissipation )
      {
	// conservative form (but TZ flow is primitive variables)

	const realArray & r0  = e   (mg,I1,I2,I3,rc,t);
	const realArray & r0x = e.x (mg,I1,I2,I3,rc,t);
	const realArray & r0y = e.y (mg,I1,I2,I3,rc,t);
	const realArray & r0z = e.z (mg,I1,I2,I3,rc,t);

	const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);
	const realArray & u0xx= e.xx(mg,I1,I2,I3,uc,t);
	const realArray & u0xy= e.xy(mg,I1,I2,I3,uc,t);
	const realArray & u0yy= e.yy(mg,I1,I2,I3,uc,t);
	const realArray & u0xz= e.xz(mg,I1,I2,I3,uc,t);
	const realArray & u0yz= e.yz(mg,I1,I2,I3,uc,t);
	const realArray & u0zz= e.zz(mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);
	const realArray & v0xx= e.xx(mg,I1,I2,I3,vc,t);
	const realArray & v0xy= e.xy(mg,I1,I2,I3,vc,t);
	const realArray & v0yy= e.yy(mg,I1,I2,I3,vc,t);
	const realArray & v0xz= e.xz(mg,I1,I2,I3,vc,t);
	const realArray & v0yz= e.yz(mg,I1,I2,I3,vc,t);
	const realArray & v0zz= e.zz(mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);
	const realArray & w0xx= e.xx(mg,I1,I2,I3,wc,t);
	const realArray & w0xy= e.xy(mg,I1,I2,I3,wc,t);
	const realArray & w0yy= e.yy(mg,I1,I2,I3,wc,t);
	const realArray & w0xz= e.xz(mg,I1,I2,I3,wc,t);
	const realArray & w0yz= e.yz(mg,I1,I2,I3,wc,t);
	const realArray & w0zz= e.zz(mg,I1,I2,I3,wc,t);

	const realArray & t0  = e   (mg,I1,I2,I3,tc,t);
	const realArray & t0x = e.x (mg,I1,I2,I3,tc,t);
	const realArray & t0y = e.y (mg,I1,I2,I3,tc,t);
	const realArray & t0z = e.z (mg,I1,I2,I3,tc,t);
	const realArray & t0xx= e.xx(mg,I1,I2,I3,tc,t);
	const realArray & t0yy= e.yy(mg,I1,I2,I3,tc,t);
	const realArray & t0zz= e.zz(mg,I1,I2,I3,tc,t);

	const realArray & rhot = evaluate( e.t(mg,I1,I2,I3,rc,t) +u0*r0x+v0*r0y+w0*r0z+r0*(u0x+v0y+w0z) );
	ut(I1,I2,I3,rc)+=rhot;
	ut(I1,I2,I3,uc)+=r0*( e.t(mg,I1,I2,I3,uc,t) +u0*u0x+v0*u0y+w0*u0z ) + Rg*(t0*r0x+t0x*r0) + rhot*u0 ;
	ut(I1,I2,I3,vc)+=r0*( e.t(mg,I1,I2,I3,vc,t) +u0*v0x+v0*v0y+w0*v0z ) + Rg*(t0*r0y+t0y*r0) + rhot*v0;
	ut(I1,I2,I3,wc)+=r0*( e.t(mg,I1,I2,I3,wc,t) +u0*w0x+v0*w0y+w0*w0z ) + Rg*(t0*r0z+t0z*r0) + rhot*w0;

	// E+p = rho[ Rg gamma/(gamma-1) T + u*u ]
	// const realArray & e0  = evaluate( r0*( Rg/(gamma-1.)*t0 + .5*( u0*u0 + v0*v0 ) ) );
	// const realArray & p0 = evaluate( (gamma-1.)*( e0 - .5*r0*( u0*u0 + v0*v0 ) ) );
	const realArray & e0t  = evaluate( r0*( Rg/(gamma-1.)*e.t(mg,I1,I2,I3,tc,t) 
						+ u0*e.t(mg,I1,I2,I3,uc,t) + v0*e.t(mg,I1,I2,I3,vc,t)+ w0*e.t(mg,I1,I2,I3,wc,t)  )
					   + e.t(mg,I1,I2,I3,rc,t)*(Rg/(gamma-1.)*t0 + .5*(u0*u0 + v0*v0+ w0*w0)) );
	const real rGamma = Rg*gamma/(gamma-1.);

	const realArray & ke = evaluate( .5*( u0*u0 + v0*v0+ w0*w0 ) );
	const realArray & kepe = evaluate( rGamma*t0 + ke );

	const realArray & ePlusP0  = evaluate( r0*( rGamma*t0 + ke ) ); 
	const realArray & ePlusP0x = evaluate( r0*( rGamma*t0x + u0*u0x + v0*v0x+ w0*w0x ) + r0x*kepe );
	const realArray & ePlusP0y = evaluate( r0*( rGamma*t0y + u0*u0y + v0*v0y+ w0*w0y ) + r0y*kepe );
	const realArray & ePlusP0z = evaluate( r0*( rGamma*t0z + u0*u0z + v0*v0z+ w0*w0z ) + r0z*kepe );
      
	ut(I1,I2,I3,tc)+=e0t + (u0x+v0y+w0z)*ePlusP0 +u0*ePlusP0x +v0*ePlusP0y +w0*ePlusP0z;
	if( mu>0. )
	{
	  const realArray & t11 = evaluate( a43*u0x-a23*(v0y+w0z));
	  const realArray & t12 = evaluate( u0y+v0x);
	  const realArray & t13 = evaluate( u0z+w0x);
	  const realArray & t22 = evaluate( a43*v0y-a23*(u0x+w0z));
	  const realArray & t23 = evaluate( v0z+w0y);
	  const realArray & t33 = evaluate( a43*w0z-a23*(u0x+v0y));

	  const realArray & t11x= evaluate( a43*u0xx-a23*v0xy);
	  const realArray & t12x= evaluate( u0xy+v0xx);
	  const realArray & t12y= evaluate( u0yy+v0xy);
	  const realArray & t22y= evaluate( a43*v0yy-a23*u0xy);
	  const realArray & t13x= evaluate( u0xz+w0xx);
	  const realArray & t13z= evaluate( u0zz+w0xz);
	  const realArray & t23x= evaluate( v0xz+w0xy);
	  const realArray & t23y= evaluate( v0yz+w0yy);
	  const realArray & t23z= evaluate( v0zz+w0yz);
	  const realArray & t33z= evaluate( a43*w0zz-a23*(u0xz+v0yz));

	  ut(I1,I2,I3,uc)-=mu*(t11x+t12y+t13z);
	  ut(I1,I2,I3,vc)-=mu*(t12x+t22y+t23z);
	  ut(I1,I2,I3,wc)-=mu*(t13x+t23y+t33z);

	  ut(I1,I2,I3,tc)-= mu*( u0*t11x+v0*t12x+w0*t13x +u0x*t11+v0x*t12+w0x*t13 +
				 u0*t12y+v0*t22y+w0*t23x +u0y*t12+v0y*t22+w0y*t23 +
				 u0*t13z+v0*t23z+w0*t33z +u0z*t13+v0z*t23+w0z*t33  );
	}
	if( kThermal>0. )
	  ut(I1,I2,I3,tc)-=kThermal*Rg*(t0xx+t0yy+t0zz);

/* -----
   if( debug() & 4 )
   {
   display(evaluate(ut(I1,I2,I3,rc)-e.t(mg,I1,I2,I3,rc,t) ),
   "error in (rho).t",debugFile,"%6.1e ");
   display(evaluate(ut(I1,I2,I3,uc)-( r0*e.t(mg,I1,I2,I3,uc,t)+u0*e.t(mg,I1,I2,I3,rc,t) )),
   "error in (rho*u).t",debugFile,"%6.1e ");
   display(evaluate(ut(I1,I2,I3,vc)-( r0*e.t(mg,I1,I2,I3,vc,t)+v0*e.t(mg,I1,I2,I3,rc,t) )),
   "error in (rho*v).t",debugFile,"%6.1e ");
   }
   ---- */
      }
      else
      {

	const realArray & r0  = e   (mg,I1,I2,I3,rc,t);
	const realArray & r0x = e.x (mg,I1,I2,I3,rc,t);
	const realArray & r0y = e.y (mg,I1,I2,I3,rc,t);
	const realArray & r0z = e.z (mg,I1,I2,I3,rc,t);

	const realArray & u0  = e   (mg,I1,I2,I3,uc,t);
	const realArray & u0x = e.x (mg,I1,I2,I3,uc,t);
	const realArray & u0y = e.y (mg,I1,I2,I3,uc,t);
	const realArray & u0z = e.z (mg,I1,I2,I3,uc,t);
	const realArray & u0xx= e.xx(mg,I1,I2,I3,uc,t);
	const realArray & u0xy= e.xy(mg,I1,I2,I3,uc,t);
	const realArray & u0yy= e.yy(mg,I1,I2,I3,uc,t);
	const realArray & u0xz= e.xz(mg,I1,I2,I3,uc,t);
	const realArray & u0yz= e.yz(mg,I1,I2,I3,uc,t);
	const realArray & u0zz= e.zz(mg,I1,I2,I3,uc,t);

	const realArray & v0  = e   (mg,I1,I2,I3,vc,t);
	const realArray & v0x = e.x (mg,I1,I2,I3,vc,t);
	const realArray & v0y = e.y (mg,I1,I2,I3,vc,t);
	const realArray & v0z = e.z (mg,I1,I2,I3,vc,t);
	const realArray & v0xx= e.xx(mg,I1,I2,I3,vc,t);
	const realArray & v0xy= e.xy(mg,I1,I2,I3,vc,t);
	const realArray & v0yy= e.yy(mg,I1,I2,I3,vc,t);
	const realArray & v0xz= e.xz(mg,I1,I2,I3,vc,t);
	const realArray & v0yz= e.yz(mg,I1,I2,I3,vc,t);
	const realArray & v0zz= e.zz(mg,I1,I2,I3,vc,t);

	const realArray & w0  = e   (mg,I1,I2,I3,wc,t);
	const realArray & w0x = e.x (mg,I1,I2,I3,wc,t);
	const realArray & w0y = e.y (mg,I1,I2,I3,wc,t);
	const realArray & w0z = e.z (mg,I1,I2,I3,wc,t);
	const realArray & w0xx= e.xx(mg,I1,I2,I3,wc,t);
	const realArray & w0xy= e.xy(mg,I1,I2,I3,wc,t);
	const realArray & w0yy= e.yy(mg,I1,I2,I3,wc,t);
	const realArray & w0xz= e.xz(mg,I1,I2,I3,wc,t);
	const realArray & w0yz= e.yz(mg,I1,I2,I3,wc,t);
	const realArray & w0zz= e.zz(mg,I1,I2,I3,wc,t);

	const realArray & t0  = e   (mg,I1,I2,I3,tc,t);
	const realArray & t0x = e.x (mg,I1,I2,I3,tc,t);
	const realArray & t0y = e.y (mg,I1,I2,I3,tc,t);
	const realArray & t0z = e.z (mg,I1,I2,I3,tc,t);
	const realArray & t0xx= e.xx(mg,I1,I2,I3,tc,t);
	const realArray & t0yy= e.yy(mg,I1,I2,I3,tc,t);
	const realArray & t0zz= e.zz(mg,I1,I2,I3,tc,t);


	where( mg.mask()(I1,I2,I3) != 0 )
	{
	  ut(I1,I2,I3,rc)+=uvt(rc)+u0*r0x+v0*r0y+w0*r0z+r0*(u0x+v0y+w0z) -nuRho*(uvxx(rc)+uvyy(rc)+uvzz(rc));
	  ut(I1,I2,I3,uc)+=uvt(uc)+u0*u0x+v0*u0y+w0*u0z +Rg*t0/r0*r0x+Rg*t0x 
	    -(mu/r0)*(a43*u0xx+u0yy+u0zz+a13*(v0xy+w0xz) );
	  
	  ut(I1,I2,I3,vc)+=uvt(vc)+u0*v0x+v0*v0y+w0*v0z  +Rg*t0/r0*r0y+Rg*t0y  
	    -(mu/r0)*(v0xx+a43*v0yy+v0zz+a13*(u0xy+w0yz));
	  
	  ut(I1,I2,I3,wc)+=uvt(wc)+u0*w0x+v0*w0y+w0*w0z +Rg*t0/r0*r0z+Rg*t0z  
	    -(mu/r0)*(w0xx+w0yy+a43*w0zz+a13*(u0xz+v0yz));


// 	ut(I1,I2,I3,tc)=
// 	  UU(uc)*UX(tc) + UU(vc)*UY(tc) + UU(wc)*UZ(tc)
// 	  -gm1*U(tc)*(UX(uc)+UY(vc)+UZ(wc))  
// 	  +(gm1/U(rc))*( kThermal*(UXX(tc)+UYY(tc)+UZZ(tc))
// 			 + mubRg*( 
// 			   a43*( UX(uc)*(UX(uc)-UY(vc))
// 				 +       UY(vc)*(UY(vc)-UZ(wc))+SQR(UZ(wc)) )
// 			   +   2.*(UY(uc)*UX(vc)+UZ(uc)*UX(wc)
// 				   +       UZ(vc)*UY(wc))
// 			   + SQR(UY(uc))+SQR(UZ(uc))+SQR(UX(vc))
// 			   + SQR(UZ(vc))+SQR(UX(wc))+SQR(UY(wc))
// 			   )
// 	    );


	  ut(I1,I2,I3,tc)+=uvt(tc)+u0*t0x+v0*t0y+w0*t0z  +gm1*t0*(u0x+v0y+w0z) 
	    -(gm1/r0)*( kThermal*(t0xx+t0yy+t0zz) 
			+mubRg*(  
			  a43*( u0x*(u0x-v0y)+v0y*(v0y-w0z)+SQR(w0z) )
			  +2.*(u0y*v0x+u0z*w0x+v0z*w0y)  
			  +SQR(u0y)+SQR(u0z)+SQR(v0x)+SQR(v0z)+SQR(w0x)+SQR(w0y) ) );
	}
      }
    }
    else
    {
      printF("Cgcns::addForcing:ERROR: unknown dimension\n");
      OV_ABORT("error");
    }

  }  // end no opt
    
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForForcing"))+=getCPU()-cpu0;

}

// ===================================================================================================================
/// \brief  Determine the real and imaginary parts of the eigenvalue for time stepping.
///
/// \param mg (input) : grid.
/// \param u (input) : current solution.
/// \param gridVelocity (input) : grid velocity.
/// \param reLambda (output) : real part of the time-stepping eigenvalue.
/// \param imLambda (output) : imaginary part of the time-stepping eigenvalue.
/// \param grid (input) : grid number.
// ===================================================================================================================
void Cgcns::
getTimeSteppingEigenvalue(MappedGrid & mg,
			  realMappedGridFunction & u0, 
			  realMappedGridFunction & gridVelocity,  
			  real & reLambda,
			  real & imLambda,
			  const int & grid)
{

  assert( grid>=0 && grid<realPartOfEigenvalue.size() && grid<imaginaryPartOfEigenvalue.size() );
  real & realPartOfTimeSteppingEigenvalue      = realPartOfEigenvalue[grid];
  real & imaginaryPartOfTimeSteppingEigenvalue = imaginaryPartOfEigenvalue[grid];

  const CnsParameters::PDE & pde = parameters.dbase.get<CnsParameters::PDE>("pde");
  const int & myid = parameters.dbase.get<int >("myid");

  FILE * debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE * pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  if( debug() & 8 )
  {
    printF(" >>>>>>> Cgcns::getTimeSteppingEigenvalue <<<<<\n"
           "   START: grid=%i, imaginaryPartOfTimeSteppingEigenvalue=%8.2e \n",
	   grid,imaginaryPartOfTimeSteppingEigenvalue);
  }

  if( realPartOfTimeSteppingEigenvalue>=0. || imaginaryPartOfTimeSteppingEigenvalue>=0. )
  {
    // Eigenvalues must have been computed by someone else (dudr)
    reLambda=realPartOfTimeSteppingEigenvalue;
    imLambda=imaginaryPartOfTimeSteppingEigenvalue;
    if( debug() & 4 )
    {
      if( true )
      {
        const IntegerArray & gid = mg.gridIndexRange();
	fPrintF(pDebugFile,
               "**getTimeSteppingEigenvalueCNS: precomputed: reLambda=%9.3e, imLambda=%9.3e for "
               "grid=%i [%i,%i][%i,%i][%i,%i] myid=%i\n",
	       reLambda,imLambda,grid,gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),myid);
      }
    }
    
    return;
  }
   
  if( pde==CnsParameters::compressibleMultiphase )
  {
    reLambda=10.;
    imLambda=10.;
    
    if( debug() & 2 )
      printF(">>> getTimeSteppingEigenvalueCNS: Do nothing for compressibleMultiphase <<<<\n");
    return;
  }

  const realArray & u = u0;

  const int & rc = parameters.dbase.get<int >("rc");
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");
  const bool & gridIsMoving = parameters.gridIsMoving(grid);

  const real & mu = parameters.dbase.get<real >("mu");
  const real & nuRho = parameters.dbase.get<real >("nuRho");
  const real & gamma = parameters.dbase.get<real >("gamma");
  const real & kThermal = parameters.dbase.get<real >("kThermal");
  const real & Rg = parameters.dbase.get<real >("Rg");
  // const real & avr = parameters.dbase.get<real >("avr");
  const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");

  const bool isRectangular = mg.isRectangular();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  IntegerArray bcLocal(2,3);
  ParallelGridUtility::getLocalBoundaryConditions( u0,bcLocal );

  bool useOpt=true;
  if( useOpt )
  {

    if( !isRectangular )
    {
      mg.update(MappedGrid::THEcenter | MappedGrid::THEinverseVertexDerivative );
    }
    else if( parameters.isAxisymmetric() )
    {
      mg.update( MappedGrid::THEcenter );
    }
    
    const realArray & rx = isRectangular ? u0 :  mg.inverseVertexDerivative();
    const realArray & center = parameters.isAxisymmetric() ? mg.center() : u0;

    #ifdef USE_PPP 
      realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0,u0Local);
      const intSerialArray & maskLocal = mg.mask().getLocalArray();
      const realSerialArray & rxLocal = rx.getLocalArray();
      const realSerialArray & xyLocal = center.getLocalArray();
      const realSerialArray & gridVelocityLocal = gridVelocity.getLocalArray();
    #else

      const realSerialArray & u0Local = u0; 
      const intSerialArray & maskLocal = mg.mask(); 
      const realSerialArray & rxLocal = rx; 
      const realSerialArray & xyLocal = center; 
      const realSerialArray & gridVelocityLocal = gridVelocity; 
    #endif


    getIndex(mg.gridIndexRange(),I1,I2,I3);  // *wdh* 030220  - evaluate du/dt here

    reLambda=0.;
    imLambda=0.;
    
    bool ok=ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3);  // do NOT include ghost *wdh* 070701
    if( !ok ) return; // no points on this processor
    

//     // Adjust I1,I2,I3 to fit uLocal -- leave a border since we difference the solution
//     const int numGhost=2;  // I think we need a border of 2
//     for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
//     {
//       int na=Iv[axis].getBase(), nb=Iv[axis].getBound();
//       if( na<u0Local.getBase(axis) )  na=u0Local.getBase(axis)+numGhost;
//       if( nb>u0Local.getBound(axis) ) nb=u0Local.getBound(axis)-numGhost;
//       Iv[axis]=Range(na,nb);
//     }
    

    const bool useFourthOrderArtificialDiffusion = false && parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
      !parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");

    const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params

    const int gridIsImplicit=/*kkc 060228 false &&*/ parameters.getGridIsImplicit(grid);

    int useWhereMask=true;
    real dx[3]={1.,1.,1.};
    real xab[2][3]={0.,1.,0.,1.,0.,1.};
    if( isRectangular )
      mg.getRectangularGridParameters( dx, xab );
   
    const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
    const int gridType= isRectangular ? 0 : 1;

    const real *rxp = rxLocal.getDataPointer(); 
    const real *pu = u0Local.getDataPointer();
    // For now we need the center array for the axisymmetric case:
    const real *xyp = isRectangular ? 0 : xyLocal.getDataPointer(); 

    Range J1=u0Local.dimension(0), J2=u0Local.dimension(1), J3=u0Local.dimension(2);
    
    realSerialArray uu; 
    // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)
    if( gridIsMoving )
    {
      uu.redim(J1,J2,J3,mg.numberOfDimensions()+1); 
    }

   
    realSerialArray p(J1,J2,J3),dp(J1,J2,J3,mg.numberOfDimensions()); // dp is a workspace

    // compute p for now assuming ideal gas
    p(J1,J2,J3)=u0Local(J1,J2,J3,rc)*u0Local(J1,J2,J3,tc)*parameters.dbase.get<real >("Rg"); 

    const real *puu = gridIsMoving ? uu.getDataPointer() : pu;

    const real *pdw = pu; // parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
                          // ((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getDataPointer();

    //kkc used now    const real *pVariableDt = pu; // not used pdtVar !=NULL ? pdtVar->getDataPointer() : pDivDamp;
    const real *pVariableDt = pdtVar !=NULL ? (*pdtVar)[grid].getLocalArray().getDataPointer() : pu;


    int i1a=mg.gridIndexRange(0,0);  // for rectangular grids -- to compute x,y,z -- not used yet
    int i2a=mg.gridIndexRange(0,1);
    int i3a=mg.gridIndexRange(0,2);
    int ipar[] ={parameters.dbase.get<int >("rc"),
                 parameters.dbase.get<int >("uc"),
                 parameters.dbase.get<int >("vc"),
                 parameters.dbase.get<int >("wc"),
                 parameters.dbase.get<int >("tc"),
                 parameters.dbase.get<int >("kc"),
                 parameters.dbase.get<int >("sc"),
                 grid,
		 orderOfAccuracy,
                 (int)parameters.gridIsMoving(grid),
		 useWhereMask,
                 (int)gridIsImplicit, // parameters.gridIsImplicit(grid),
                 (int)parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod"),
		 (int)parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
                 (int)parameters.isAxisymmetric(),
                 (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
                 (int)useFourthOrderArtificialDiffusion,
                 (int)parameters.dbase.get<bool >("advectPassiveScalar"),
                 gridType,
                 parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
                 (int)parameters.dbase.get<int >("useLocalTimeStepping"),
                 i1a,i2a,i3a
                 };

    const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric -- not used --
    const real  ajEps=REAL_EPSILON*100.; // for minimum value of the jacobian

//                  parameters.dbase.get<real >("av2")=0.; 
//                  parameters.dbase.get<real >("aw2")=0.;
//                  parameters.dbase.get<real >("av4")=0.;
//                  parameters.dbase.get<real >("aw4")=0.;


    real rpar[]={mg.gridSpacing(0),
                 mg.gridSpacing(1),
                 mg.gridSpacing(2),
                 dx[0],
                 dx[1],
                 dx[2],
                 parameters.dbase.get<real >("ad21"),
                 parameters.dbase.get<real >("ad22"),
                 parameters.dbase.get<real >("ad41"),
                 parameters.dbase.get<real >("ad42"),
                 parameters.dbase.get<real >("nuPassiveScalar"),
                 adcPassiveScalar,
                 parameters.dbase.get<real >("dtMax"),
                 reLambda,
                 imLambda,
                 parameters.dbase.get<real >("ad21n"),
                 parameters.dbase.get<real >("ad22n"),
                 parameters.dbase.get<real >("ad41n"),
                 parameters.dbase.get<real >("ad42n"),
                 xab[0][0],
                 xab[0][1],
                 xab[0][2],
                 yEps,
                 parameters.dbase.get<real >("av2"),
                 parameters.dbase.get<real >("aw2"),
                 parameters.dbase.get<real >("av4"),
                 parameters.dbase.get<real >("aw4"),
                 parameters.dbase.get<real >("gamma"),
                 parameters.dbase.get<real >("Rg"),
                 parameters.dbase.get<real >("mu"),
                 parameters.dbase.get<real >("kThermal"),
                 ajEps
                 };


    int ierr=0;
    

    cnsdts(mg.numberOfDimensions(),
	   I1.getBase(),I1.getBound(),
	   I2.getBase(),I2.getBound(),
	   I3.getBase(),I3.getBound(),
	   u0Local.getBase(0),u0Local.getBound(0),u0Local.getBase(1),u0Local.getBound(1),
	   u0Local.getBase(2),u0Local.getBound(2),u0Local.getBase(3),u0Local.getBound(3),
	   *maskLocal.getDataPointer(), *xyp, *rxp,
	   *pu, *puu, *gridVelocityLocal.getDataPointer(), *pdw, *p.getDataPointer(), *dp.getDataPointer(),
           *pVariableDt,
	   bcLocal(0,0), ipar[0], rpar[0], ierr );

    reLambda=rpar[13];
    imLambda=rpar[14];
    
    // *wdh* 060714  reLambda=Parameters::getMaxValue(reLambda);   // max value over all processors
    // *wdh* 060714  imLambda=Parameters::getMaxValue(imLambda);   // max value over all processors

    if( debug() & 4 ) printf("From cnsdts: NEW: (reLambda,imLambda)=(%9.3e,%9.3e)\n",reLambda,imLambda);
    
    return;

  }
  
}

#undef RX

