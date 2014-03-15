#include "CnsParameters.h"
#include "GridFunction.h"
#include "ParallelUtility.h"

class UserDefinedEOSData;  // forward reference

#define ADDPSI EXTERN_C_NAME(addpsi)
#define CONSPRIM EXTERN_C_NAME(consprim)

extern "C"
{
  void ADDPSI(const int & nd1a, const int & nd1b, const int & nd2a, const int & nd2b,
              const real & fact, const real & rho, real & u);

  void CONSPRIM(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b, 
		const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
		const int &nd,const int &ns, 
		const int &rc,const int &uc,const int &vc,const int &wc,const int &tc, const int &sc,
		real & q, const int & mask, const real & val, const int & ipar, const real & rpar, 
                const int & option, const int & fixup, const real & epsRho );

}

// ************* EOS USER DEFINED COMMON BLOCK **************
#define eosUserDefined EXTERN_C_NAME(eosuserdefined)
extern "C"
{
  extern struct {UserDefinedEOSData *userEOSDataPointer;} eosUserDefined;

}

//\begin{>>CnsParametersInclude.tex}{\subsection{primitiveToConservative}} 
int CnsParameters::
primitiveToConservative(GridFunction & gf,
                        int gridToConvert  /* =-1 */, 
                        int fixupUnsedPoints /* =false */)
// ==================================================================================
// /Description:
//   Convert primitive variables to conservative
// primitive : rho, u,v,w, T, species
// conservative rho, (rho*u), (rho*v), (rho*w), E, (rho*species)
//
// /gridToConvert (input) : by default (grid==-1) convert all grids, otherwise convert this grid.
// /fixUnsedPoints (input) : if true fixup unused points
//\end{CnsParametersInclude.tex}  
// =========================================================================================
{
  GridFunction::Forms & form = gf.form;

  if( form==GridFunction::conservativeVariables )
    return 0;

  CompositeGrid & cg = gf.cg;
  realCompositeGridFunction & u = gf.u;
  
  form=GridFunction::conservativeVariables;
  Range G = gridToConvert==-1 ? Range(0,cg.numberOfGrids()-1) : Range(gridToConvert,gridToConvert);

  const PDE & pde = dbase.get<CnsParameters::PDE >("pde");
  const GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  const PDEVariation & pdeVariation = dbase.get<CnsParameters::PDEVariation >("pdeVariation");
  
  // Look for the pointer to the user defined EOS:
  if( dbase.has_key("userDefinedEquationOfStateDataPointer") )
  {
    // set the fortran common block variable:
    eosUserDefined.userEOSDataPointer = dbase.get<UserDefinedEOSData*>("userDefinedEquationOfStateDataPointer");
  }

  if( true )
  {
    // ** new way ***
    // This version will also NOT fix up unused points since we need extra extrapolated values
    // at points where mask==0
    const real epsRho=1./pow(REAL_MAX,.125); // 1.e-10; // 1./pow(REAL_MAX,.125); // pow(REAL_MIN,.25); // SQRT(REAL_MIN);

    for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
    {
      if( !useConservativeVariables(grid) )
      { // this grid is not converted
	continue;
      }
      
      #ifdef USE_PPP
        realSerialArray q;  getLocalArrayWithGhostBoundaries(u[grid],q);
        const intSerialArray & mask = cg[grid].mask().getLocalArray();
      #else
        const realSerialArray & q = u[grid];
        const intSerialArray & mask = cg[grid].mask();
      #endif
  
      real rpar[100]; // *wdh*  multicomponent version uses high values 050119
                      // *jwb* four component uses very high values 29Sept2008
      int ipar[22];
      


      // These are for the standard case
      rpar[0]= dbase.get<real >("Rg"); 
      rpar[1]= dbase.get<real >("gamma"); 
      rpar[2]= dbase.get<real >("heatRelease"); 
      rpar[3]= dbase.get<real >("absorbedEnergy");
      
      ipar[0]= pdeVariation; 
      ipar[1]= dbase.get<CnsParameters::ReactionTypeEnum >("reactionType"); 
      ipar[2]= dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
      ipar[3]= conservativeGodunovMethod;
      ipar[4]= pde;

      ipar[18]=0;  // istiff : by default off *wdh* 090529

      if(  conservativeGodunovMethod==multiComponentVersion )
      {
	bool foundDon;
	bool fdFourComp;
	fdFourComp = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("fourComp",ipar[20]);
	foundDon= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("useDon",ipar[17]);
	if( !fdFourComp )
        {
	  ipar[20] = 0;
	}
	if( !foundDon )
	{
	  ipar[17]=0;
	}
	if( ipar[17] )
	{
          // We are using Don's code with multicomponent stuff
	  if( ipar[20] )
	  {
	    // use Don's code with four ideal components 
	    bool fdg1,fdg2,fdg3,fdg4,fdc1,fdc2,fdc3,fdc4;
	    fdg1 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma1",rpar[50] );
	    fdc1 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv1",rpar[51] );
	    fdg2 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma2",rpar[52] );
	    fdc2 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv2",rpar[53] );
	    fdg3 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma3",rpar[54] );
	    fdc3 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv3",rpar[55] );
	    fdg4 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma4",rpar[56] );
	    fdc4 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv4",rpar[57] );

	    if( !fdg1 || !fdg2 || !fdg3 || !fdg4 || !fdc1 || !fdc2 || !fdc3 || !fdc4 )
	    {
	      printF("CnsParameters::primitiveToConservative:ERROR: \n");
	      printF("must define gamma1 through gamma4 and cv1 through cv4 in command file.\n");
	      printF("This is a fatal error!!!!! ... quiting\n");
	      Overture::abort("error");
	    }
	  }
	  else
	  {
	    // We are using Don's code with multicomponent stuff
	    if(  dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState") != jwlEOS )
	    {
	      bool foundgi, foundgr, foundcvi, foundcvr;
	      foundgi= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gammai",rpar[40]);
	      foundgr= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gammar",rpar[41]);
	      foundcvi= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvi",rpar[42]);
	      foundcvr= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvr",rpar[43]);
	      if ( !foundgi || !foundgr )
	      {
		printF("CnsParameters::primitiveToConservative:ERROR: \n");
		printF("must define gammai, gammar in command file.\n");
		printF("This is a fatal error!!!!! ... quiting\n");
		Overture::abort("error");
	      }
	      if( !foundcvi || !foundcvr )
	      {
		rpar[42]=-1.0;
		rpar[43]=-1.0;
	      }
	      // new stiffened gas case
	      rpar[44]=0.;
	      rpar[45]=0.;
	      bool foundStiffened, foundpii, foundpir;
	      foundStiffened= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("stiffenedEOS",ipar[18]);
	      if( foundStiffened )
	      {
		foundpii= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pii",rpar[44]);
		foundpir= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pir",rpar[45]);
	      }
	      else
	      {
		ipar[18]=0;
	      }
	    }
	  }
	}
	else
	{
	  // We are using Jeff's multicomponent stuff
	  bool foundg1, foundg2, foundcv1, foundcv2, foundpi1, foundpi2;
	  foundg1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gamma1",rpar[4]);
	  foundg2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gamma2",rpar[5]);
	  foundcv1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cv1",rpar[6]);
	  foundcv2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cv2",rpar[7]);
	  foundpi1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pi1",rpar[8]);
	  foundpi2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pi2",rpar[9]);

	  if ( !foundg1 || !foundg2 || !foundcv1 || !foundcv2 || !foundpi1 || !foundpi2 ) 
	  {
	    printF("CnsParameters::primitiveToConservative:ERROR:\n");
	    printF("Must define gamma1, gamma2, cv1, cv2, pi1, and pi2 in command file\n");
	    printF("Can't continue ---- Aborting\n");
	    Overture::abort("error");
	  }
	}
      }
      else
      {
	ipar[20] = 0; // four component flag
      }

      int option=0;
      
      RealArray values( dbase.get<int >("numberOfComponents"));
      values=0.;
      values( dbase.get<int >("rc"))=1.;  // set unused points of rho to this value
      values( dbase.get<int >("tc"))=.5;  // set unused points of T to this value
      for( int s=0; s< dbase.get<int >("numberOfSpecies"); s++ )
	values( dbase.get<int >("sc")+s)=1.;  // Don S. wants lambda=1

      // kkc 051115 added because axiSymmetric flow with swirl should be treated as 3D for conversions
      int nd = cg.numberOfDimensions();
      if (  dbase.get<bool >("axisymmetricWithSwirl") ) nd++;

      CONSPRIM(q.getBase(0),q.getBound(0),q.getBase(1),q.getBound(1),q.getBase(2),q.getBound(2),
	       q.getBase(0),q.getBound(0),q.getBase(1),q.getBound(1),q.getBase(2),q.getBound(2),
	       nd /*kkc cg.numberOfDimensions()*/, dbase.get<int >("numberOfSpecies"),  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("wc"), dbase.get<int >("tc"), dbase.get<int >("sc"), 
	       *q.getDataPointer(), *mask.getDataPointer(), values(0),ipar[0],rpar[0],option,
               fixupUnsedPoints,epsRho );
      
    }
    return 0;
  }
  
  const int & rc =  dbase.get<int >("rc");
  const int & uc =  dbase.get<int >("uc");
  const int & vc =  dbase.get<int >("vc");
  const int & wc =  dbase.get<int >("wc");
  const int & tc =  dbase.get<int >("tc");
  const real & Rg =  dbase.get<real >("Rg");
  const real & gamma =  dbase.get<real >("gamma");

  // printf(" ++++++ primitiveToConservative called ...Rg=%e\n", dbase.get<real >("Rg"));
  
  Range all;
  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
  {
    realArray & q = u[grid];
    const realArray & rho = q(all,all,all, dbase.get<int >("rc"));
    const realArray & te  = q(all,all,all, dbase.get<int >("tc"));
    const realArray & uu  = q(all,all,all, dbase.get<int >("uc"));

    if( cg.numberOfDimensions()==1 )
    {
      q(all,all,all, dbase.get<int >("uc"))*=rho;
      q(all,all,all, dbase.get<int >("tc"))=rho*( ( dbase.get<real >("Rg")/( dbase.get<real >("gamma")-1.))*te+ .5*(uu*uu) );    // E = ( p/( dbase.get<real >("gamma")-1) + .5*rho*u*u )
    }
    else if( cg.numberOfDimensions()==2 ) 
    {
      const realArray & v   = q(all,all,all, dbase.get<int >("vc"));
      q(all,all,all, dbase.get<int >("tc"))=rho*( ( dbase.get<real >("Rg")/( dbase.get<real >("gamma")-1.))*te+ .5*(uu*uu+v*v) );   // E = ( p/( dbase.get<real >("gamma")-1) + .5*rho*u*u )
      q(all,all,all, dbase.get<int >("uc"))*=rho;
      q(all,all,all, dbase.get<int >("vc"))*=rho;

      
    }
    else
    {
      const realArray & v   = q(all,all,all, dbase.get<int >("vc"));
      const realArray & w   = q(all,all,all, dbase.get<int >("wc"));
      q(all,all,all, dbase.get<int >("tc"))=rho*( ( dbase.get<real >("Rg")/( dbase.get<real >("gamma")-1.))*te+ .5*(uu*uu+v*v+w*w) );   // E = ( p/( dbase.get<real >("gamma")-1) + .5*rho*u*u )
      q(all,all,all, dbase.get<int >("uc"))*=rho;
      q(all,all,all, dbase.get<int >("vc"))*=rho;
      q(all,all,all, dbase.get<int >("wc"))*=rho;
    }
    for( int s=0; s< dbase.get<int >("numberOfSpecies"); s++ )
      q(all,all,all, dbase.get<int >("sc")+s)*=rho;

    if(  pdeVariation==conservativeGodunov )
    {

// here is where psi comes in for general eos, e.g.
// q(all,all,all,tc)+=rho*psi(rho);

      const int & nd1a=rho.getBase(0);
      const int & nd1b=rho.getBound(0);
      const int & nd2a=rho.getBase(1);
      const int & nd2b=rho.getBound(1);
      const real fact=1.;
//      const realArray & E=q(all,all,all,tc);
//      ADDPSI(nd1a,nd1b,nd2a,nd2b,fact,*rho.getDataPointer(),*E.getDataPointer());
      ADDPSI(nd1a,nd1b,nd2a,nd2b,fact,q(q.getBase(0),q.getBase(1),q.getBase(2), dbase.get<int >("rc")),
                                      q(q.getBase(0),q.getBase(1),q.getBase(2), dbase.get<int >("tc")));

      if(  dbase.get<real >("heatRelease")!=0. &&  dbase.get<int >("numberOfSpecies")==1 )
      {
        // E = E - Q*(rho*product)
	q(all,all,all, dbase.get<int >("tc"))-= dbase.get<real >("heatRelease")*q(all,all,all, dbase.get<int >("sc"));
      }
      else if(  dbase.get<int >("numberOfSpecies")==2 )
      {  
        // E = E - [Q*(rho*product) - R*(rho*radical)]
        q(all,all,all, dbase.get<int >("tc"))-= dbase.get<real >("heatRelease")*q(all,all,all, dbase.get<int >("sc"))- dbase.get<real >("absorbedEnergy")*q(all,all,all, dbase.get<int >("sc")+1);
      }
    }
    
  }
  return 0;
}

//\begin{>>CnsParametersInclude.tex}{\subsection{conservativeToPrimitive}} 
int CnsParameters::
conservativeToPrimitive(GridFunction & gf,
                        int gridToConvert  /* =-1 */, 
                        int fixupUnsedPoints /* =false */ )
// ==================================================================================
// /Description:
//   Convert conservative variables to primitive.
// primitive : rho, u,v,w, T. species
// conservative rho, (rho*u), (rho*v), (rho*w), E, (rho*species)
//
// /gridToConvert (input) : by default (grid==-1) convert all grids, otherwise convert this grid.
// /fixUnsedPoints (input) : if true fixup unused points
//
//\end{CnsParametersInclude.tex}  
// =========================================================================================
{
  GridFunction::Forms & form = gf.form;
  if( form==GridFunction::primitiveVariables )
    return 0;

  CompositeGrid & cg = gf.cg;
  realCompositeGridFunction & u = gf.u;

  form=GridFunction::primitiveVariables;
  Range G = gridToConvert==-1 ? Range(0,cg.numberOfGrids()-1) : Range(gridToConvert,gridToConvert);

  const real epsRho=1./pow(REAL_MAX,.125); // 1.e-10; // =1./pow(REAL_MAX,.125); // =pow(REAL_MIN,.25); // SQRT(REAL_MIN);
  const PDE & pde = dbase.get<CnsParameters::PDE >("pde");
  const GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  const PDEVariation & pdeVariation = dbase.get<CnsParameters::PDEVariation >("pdeVariation");
  
  // Look for the pointer to the user defined EOS:
  if( dbase.has_key("userDefinedEquationOfStateDataPointer") )
  {
    // set the fortran common block variable:
    eosUserDefined.userEOSDataPointer = dbase.get<UserDefinedEOSData*>("userDefinedEquationOfStateDataPointer");
  }

  if( true )
  {
    // ** new way ***
    // This version will also fix up unused points
    for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
    {
      if( !useConservativeVariables(grid) )
      { // this grid is not converted
	continue;
      }

      #ifdef USE_PPP
        realSerialArray q;  getLocalArrayWithGhostBoundaries(u[grid],q);
        const intSerialArray & mask = cg[grid].mask().getLocalArray();
      #else
        const realSerialArray & q = u[grid];
        const intSerialArray & mask = cg[grid].mask();
      #endif
    
      real rpar[100];
      int ipar[22];
      
      // These are for the standard case
      rpar[0]= dbase.get<real >("Rg"); 
      rpar[1]= dbase.get<real >("gamma"); 
      rpar[2]= dbase.get<real >("heatRelease"); 
      rpar[3]= dbase.get<real >("absorbedEnergy");

      ipar[0]= pdeVariation; 
      ipar[1]= dbase.get<CnsParameters::ReactionTypeEnum >("reactionType"); 
      ipar[2]= dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
      ipar[3]= conservativeGodunovMethod;
      ipar[4]= pde;

      ipar[18]=0;  // istiff : by default off *wdh* 090529

      if( conservativeGodunovMethod==multiComponentVersion )
      {
	bool foundDon;
	bool fdFourComp;
	fdFourComp = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("fourComp",ipar[20]);
	foundDon= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("useDon",ipar[17]);
	if( !fdFourComp )
        {
	  ipar[20] = 0;
	}
	if( !foundDon )
	{
	  ipar[17]=0;
	}
	if( ipar[17] )
	{
	  if( ipar[20] )
	  {
	    // use Don's code with four ideal components 
	    bool fdg1,fdg2,fdg3,fdg4,fdc1,fdc2,fdc3,fdc4;
	    fdg1 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma1",rpar[50] );
	    fdc1 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv1",rpar[51] );
	    fdg2 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma2",rpar[52] );
	    fdc2 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv2",rpar[53] );
	    fdg3 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma3",rpar[54] );
	    fdc3 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv3",rpar[55] );
	    fdg4 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "gamma4",rpar[56] );
	    fdc4 = dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter( "cv4",rpar[57] );

	    if( !fdg1 || !fdg2 || !fdg3 || !fdg4 || !fdc1 || !fdc2 || !fdc3 || !fdc4 )
	    {
	      printF("CnsParameters::primitiveToConservative:ERROR: \n");
	      printF("must define gamma1 through gamma4 and cv1 through cv4 in command file.\n");
	      printF("This is a fatal error!!!!! ... quiting\n");
	      Overture::abort("error");
	    }
	  }
	  else
	  {
	    // We are using Don's code with multicomponent stuff
	    if(  dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState") != jwlEOS )
	    {
	      bool foundgi, foundgr, foundcvi, foundcvr;
	      foundgi= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gammai",rpar[40]);
	      foundgr= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gammar",rpar[41]);
	      foundcvi= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvi",rpar[42]);
	      foundcvr= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvr",rpar[43]);
	      if ( !foundgi || !foundgr )
	      {
		printF("CnsParameters::primitiveToConservative:ERROR: \n");
		printF("must define gammai, gammar in command file.\n");
		printF("This is a fatal error!!!!! ... quiting\n");
		Overture::abort("error");
	      }
	      if( !foundcvi || !foundcvr )
	      {
		rpar[42]=-1.0;
		rpar[43]=-1.0;
	      }
	      // new stiffened gas case
	      rpar[44]=0.;
	      rpar[45]=0.;
	      bool foundStiffened, foundpii, foundpir;
	      foundStiffened= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("stiffenedEOS",ipar[18]);
	      if( foundStiffened )
	      {
		foundpii= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pii",rpar[44]);
		foundpir= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pir",rpar[45]);
	      }
	      else
	      {
		ipar[18]=0;
	      }
	    }
	  }
	}
	else
	{
	  // We are using Jeff's multicomponent stuff
	  bool foundg1, foundg2, foundcv1, foundcv2, foundpi1, foundpi2;
	  foundg1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gamma1",rpar[4]);
	  foundg2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gamma2",rpar[5]);
	  foundcv1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cv1",rpar[6]);
	  foundcv2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cv2",rpar[7]);
	  foundpi1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pi1",rpar[8]);
	  foundpi2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pi2",rpar[9]);

	  if ( !foundg1 || !foundg2 || !foundcv1 || !foundcv2 || !foundpi1 || !foundpi2 ) 
	  {
	    printF("CnsParameters::primitiveToConservative:ERROR: \n");
	    printF("Must define gamma1, gamma2, cv1, cv2, pi1, and pi2 in command file\n");
	    printF("Can't continue ---- Aborting\n");
	    Overture::abort("error");
	  }
	}
      }
      else
      {
	ipar[20] = 0; // four component flag
      }

      int option=1;
      
      RealArray values( dbase.get<int >("numberOfComponents"));
      values=0.;
      values( dbase.get<int >("rc"))=1.;  // set unused points of rho to this value
      values( dbase.get<int >("tc"))=.5;  // set unused points of T to this value
      for( int s=0; s< dbase.get<int >("numberOfSpecies"); s++ )
	values( dbase.get<int >("sc")+s)=1.;  // Don S. wants lambda=1

      // kkc 051115 added because axiSymmetric flow with swirl should be treated as 3D for conversions
      int nd = cg.numberOfDimensions();
      if (  dbase.get<bool >("axisymmetricWithSwirl") ) nd++;

      CONSPRIM(q.getBase(0),q.getBound(0),q.getBase(1),q.getBound(1),q.getBase(2),q.getBound(2),
	       q.getBase(0),q.getBound(0),q.getBase(1),q.getBound(1),q.getBase(2),q.getBound(2),
	       nd /*cg.numberOfDimensions()*/, dbase.get<int >("numberOfSpecies"),  dbase.get<int >("rc"), dbase.get<int >("uc"), dbase.get<int >("vc"), dbase.get<int >("wc"), dbase.get<int >("tc"), dbase.get<int >("sc"), 
	       *q.getDataPointer(), *mask.getDataPointer(), values(0),ipar[0],rpar[0],option,
               fixupUnsedPoints,epsRho );
      
    }
    return 0;
  }


  const int & rc =  dbase.get<int >("rc");
  const int & uc =  dbase.get<int >("uc");
  const int & vc =  dbase.get<int >("vc");
  const int & wc =  dbase.get<int >("wc");
  const int & tc =  dbase.get<int >("tc");
  const real &Rg =  dbase.get<real >("Rg");
  const real &gamma =  dbase.get<real >("gamma");
  
  // printf(" ++++++ conservativeToPrimitive called ... Rg=%e\n", dbase.get<real >("Rg"));


  Range all;
  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
  {
    realArray & q = u[grid];
    const realArray & rhoInverse = evaluate(1./max(epsRho,q(all,all,all, dbase.get<int >("rc"))));
    const realArray & uu = q(all,all,all, dbase.get<int >("uc"));
    const realArray & e  = q(all,all,all, dbase.get<int >("tc"));

    if( cg.numberOfDimensions()==1 )
    {
      q(all,all,all, dbase.get<int >("uc"))*=rhoInverse;

      q(all,all,all, dbase.get<int >("tc"))=(( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))*( e*rhoInverse - .5*(uu*uu) );    // this is T on the lhs
    }
    else if( cg.numberOfDimensions()==2 ) 
    {
      const realArray & v   = q(all,all,all, dbase.get<int >("vc"));
      q(all,all,all, dbase.get<int >("uc"))*=rhoInverse;
      q(all,all,all, dbase.get<int >("vc"))*=rhoInverse;
      q(all,all,all, dbase.get<int >("tc"))=(( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))*( e*rhoInverse - .5*(uu*uu+v*v) );   // this is T on the lhs
    }
    else
    {
      const realArray & v   = q(all,all,all, dbase.get<int >("vc"));
      const realArray & w   = q(all,all,all, dbase.get<int >("wc"));
      q(all,all,all, dbase.get<int >("uc"))*=rhoInverse;
      q(all,all,all, dbase.get<int >("vc"))*=rhoInverse;
      q(all,all,all, dbase.get<int >("wc"))*=rhoInverse;
      q(all,all,all, dbase.get<int >("tc"))=(( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))*( e*rhoInverse - .5*(uu*uu+v*v+w*w) );   // this is T on the lhs

    }
    for( int s=0; s< dbase.get<int >("numberOfSpecies"); s++ )
      q(all,all,all, dbase.get<int >("sc")+s)*=rhoInverse;

    if(  pdeVariation==conservativeGodunov )
    {

// here is where psi comes in for general eos, e.g.
// q(all,all,all,tc)-=((gamma-1.)/Rg)*psi(rho);

      const realArray & rho=q(all,all,all, dbase.get<int >("rc"));
      const int & nd1a=rho.getBase(0);
      const int & nd1b=rho.getBound(0);
      const int & nd2a=rho.getBase(1);
      const int & nd2b=rho.getBound(1);
      const real fact=-( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg");
//      realArray & E=q(all,all,all,tc);
//       ADDPSI(nd1a,nd1b,nd2a,nd2b,fact,*rho.getDataPointer(),*E.getDataPointer());
      ADDPSI(nd1a,nd1b,nd2a,nd2b,fact,q(q.getBase(0),q.getBase(1),q.getBase(2), dbase.get<int >("rc")),
                                      q(q.getBase(0),q.getBase(1),q.getBase(2), dbase.get<int >("tc")));

      if(  dbase.get<int >("numberOfSpecies")==1 &&  dbase.get<real >("heatRelease")!=0. )
      {
        q(all,all,all, dbase.get<int >("tc"))+=( (( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))* dbase.get<real >("heatRelease"))*q(all,all,all, dbase.get<int >("sc"));
      }
      else if(  dbase.get<int >("numberOfSpecies")==2 )
      {
        q(all,all,all, dbase.get<int >("tc"))+= ((( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))* dbase.get<real >("heatRelease"))*q(all,all,all, dbase.get<int >("sc"))
                           -((( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))* dbase.get<real >("absorbedEnergy"))*q(all,all,all, dbase.get<int >("sc")+1);
      }
    }
  }
  return 0;
}

