#include "CnsParameters.h"
#include <math.h>

//   F77_BLANK_COMMON
//      will expand as follows:
//      All Unix _BLNK__
//  extern struct {int i,j,k;} F77_BLANK_COMMON;


#define EOSDAT EXTERN_C_NAME(eosdat)
#define EOSBURNTDAT EXTERN_C_NAME(eosburntdat)
#define GASDAT EXTERN_C_NAME(gasdat)
#define SRCPRM EXTERN_C_NAME(srcprm)
#define COMDAT EXTERN_C_NAME(comdat)
#define CMPSRC EXTERN_C_NAME(cmpsrc)
#define CMPRXN EXTERN_C_NAME(cmprxn)
#define CMPIGN EXTERN_C_NAME(cmpign)
#define CMPFLX EXTERN_C_NAME(cmpflx)
#define CMPROM EXTERN_C_NAME(cmprom)
//#define CMPMID cmpmid_
#define viscosityCoefficients EXTERN_C_NAME(viscositycoefficients)
#define MVARS EXTERN_C_NAME(mvars)
#define MJWL EXTERN_C_NAME(multijwl)
#define DESEN EXTERN_C_NAME(desensitization)
#define IGDAT EXTERN_C_NAME(igdat)

extern "C"
{

  // here is  dbase.get<real >("a") common block from fortran
  //        common / eosdat / omeg(2),ajwl(2,2),rjwl(2,2),vs0,ts0,
  //       *                  fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,heat,
  //       *                  eospar(20)
  extern struct {real omeg[2],ajwl[2][2],rjwl[2][2],vs0,ts0,fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,heat,eospar[20];} EOSDAT;

  extern struct {real omeg[2],ajwl[2][2],rjwl[2][2],vs0,ts0,fsvs0,zsvs0,vg0,fgvg0,zgvg0,cgcs,heat,eospar[20];} EOSBURNTDAT;
		      
  //  common /NavierStokes/ amu,akappa,cmu1,cmu2,cmu3,ckap1,ckap2,ckap3
  extern struct{ real amu,akappa,cmu1,cmu2,cmu3,ckap1,ckap2,ckap3;} viscosityCoefficients;  //

//      common / gasdat / gam(2),gm1(2),gp1(2),em(2),ep(2),ps0,bgas
  extern struct{ real gam[2],gm1[2],gp1[2],em[2],ep[2],ps0,bgas;} GASDAT;

//      common / srcprm / delta,rmuc,htrans,cratio,abmin,abmax,isrc
  extern struct{ real delta,rmuc,htrans,cratio,abmin,abmax; int isrc;} SRCPRM;

//      common / comdat / cfact1,cfact2,heat
  extern struct{ real cfact1,cfact2,heat;} COMDAT;

//      common / cmpsrc / tol,qmax,qmin,itmax
  extern struct{ real tol,qmax,qmin; int itmax;} CMPSRC;

//      common / cmprxn / sigma,pgi,anu
  extern struct{ real sigma,pgi,anu;} CMPRXN;

//      common / cmpign / sigmai,pfref,ab0,anui,phieps,phimin
  extern struct{ real sigmai,pfref,ab0,anui,phieps,phimin;} CMPIGN;

//      common / cmpflx / rtol, lcont
  extern struct{ real rtol; int lcont;} CMPFLX;

//      common / cmprom / atol, nrmax
  extern struct{ real atol; int nrmax;} CMPROM;

//      common / cmpmid / toli, tolv, itmax
//  extern struct{ real toli,tolv; int itmax;} CMPMID;

  //    common / mvars / mh,mr,me,ieos,irxn,imult,islope,ifix,
  //   *                 ivisco
  extern struct{ int mh,mr,me,ieos,irxn,imult,islope,ifix,ivisco,iupwind,ilimit,ides;} MVARS;

  extern struct{ real gm1s[3],amjwl[2][2],rmjwl[2][2],ai[2],ri[2],fs0,fg0,gs0,gg0,fi0,gi0,ci,cs,cg,mjwlq,mvi0,mvs0,mvg0,iheat; int iterations,newMethod; } MJWL;

  extern struct{ real Ar,er,ra1,alamc; } DESEN;

  extern struct{ real ra,eb,ex, ec,ed,ey,ee,eg,ez,al0,al1,al2,ai,ag1,ag2; } IGDAT;

}

//\begin{>>Parameters.tex}{\subsection{setUserDefinedParameters}}  
int CnsParameters::
setUserDefinedParameters()  // allow user defined  dbase.get<ListOfShowFileParameters >("pdeParameters") to be passed to C or Fortran routines.
// ==============================================================================================
//  /Description:
//     This function is used to pass user defined pdeParameters to C or Fortran routines.
//   In the case of Fortran we assign common block variables by making the common block look like
//  a struct. 
//\end{ParametersInclude.tex}  
// ==============================================================================================
{

  viscosityCoefficients.amu= dbase.get<real >("mu");
  viscosityCoefficients.akappa= dbase.get<real >("kThermal");
  
  // for variable viscosity with temperature, by default no dependence
  // amu0=amu*(cmu1+cmu2*abs(tmp(3,0))**cmu3)
  viscosityCoefficients.cmu1=1.;
  viscosityCoefficients.cmu2=0.;
  viscosityCoefficients.cmu3=1.;
  
  viscosityCoefficients.ckap1=1.;
  viscosityCoefficients.ckap2=0.;
  viscosityCoefficients.ckap3=1.;

  int ivisco,iupwind,ilimit;
  if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("fullSet",ivisco) )
  {
    printF(" setUserDefinedParameters: setting ivisco=%i\n", ivisco);
    MVARS.ivisco=ivisco;
  }
  else
  {
    MVARS.ivisco=0;
  }
  if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("upWind",iupwind) )
  {
    printF(" setUserDefinedParameters: setting iupwind=%i\n", iupwind);
    MVARS.iupwind=iupwind;
  }
  else
  {
    // Default is to upwind in Godunov
    MVARS.iupwind=1;
  }
  if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("SlopeLimiter",ilimit) )
  {
    printF(" setUserDefinedParameters: setting ilimit=%i\n", ilimit);
    MVARS.ilimit=ilimit;
  }
  else
  {
    // Default is to slope limit in Godunov
    MVARS.ilimit=1;
  }

  EquationOfStateEnum & equationOfState = dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
  

  MVARS.ides=0; // default to zero ... possibly change below
  if(  dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")==ignitionAndGrowth ||  dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")==igDesensitization )
  {
    bool fdra, fdeb, fdex, fdec, fded, fdey, fdee, fdeg, fdez;
    bool fdal0, fdal1, fdal2, fdai, fdag1, fdag2, fdpref, fdtref;
    real ai, ag1, ag2, tref, pref;

    fdra= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ra",IGDAT.ra);
    fdeb= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("eb",IGDAT.eb);
    fdex= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ex",IGDAT.ex);
    fdec= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ec",IGDAT.ec);
    fded= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ed",IGDAT.ed);
    fdey= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ey",IGDAT.ey);
    fdee= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ee",IGDAT.ee);
    fdeg= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("eg",IGDAT.eg);
    fdez= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ez",IGDAT.ez);
    
    fdal0= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("al0",IGDAT.al0);
    fdal1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("al1",IGDAT.al1);
    fdal2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("al2",IGDAT.al2);
    fdai= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ai",ai);
    fdag1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ag1",ag1);
    fdag2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ag2",ag2);
    fdtref= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("tref",tref);
    fdpref= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pref",pref);

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
    
    IGDAT.ai=tref*ai;
    IGDAT.ag1=tref*ag1*pow(pref,IGDAT.ey);
    IGDAT.ag2=tref*ag2*pow(pref,IGDAT.ez);
    if(  dbase.get<CnsParameters::ReactionTypeEnum >("reactionType")==igDesensitization ) 
    {
      DESEN.alamc=0.01;                 // default value
      bool fdAr, fder, fdra1, fdalamc;
      fdAr= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("desensitizationAr",DESEN.Ar);
      fder= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("desensitizationDelay",DESEN.er);
      fdra1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("desensitizationRa1",DESEN.ra1);
      fdalamc= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("desensitizationLambdaC",DESEN.alamc);
      
      if( !fdAr || !fder || !fdra1 )
      {
	printf("Error (setUserDefineParameters) : undefined desensitization parameter(s)\n");
	exit(0);
      }
      MVARS.ides=1;
    }
  }

  if(  equationOfState==jwlEOS )
  {
    if(  dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod")==multiComponentVersion )
    {
      // multicomponent JWL
      bool fdwi,fdws,fdwg,fda11,fda12,fda21,fda22,fda13,fda23;
      bool fdr11,fdr12,fdr21,fdr22,fdr13,fdr23;
      bool fdci,fdcs,fdcg,fdheat;
      bool fdvi0,fdvs0, fdvg0;
      
      real vs0,vg0,vi0, e0;
      int ier;
      
      // Remember A and R are stored as FORTRAN arrays!!!
      fdwi= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("omegi",MJWL.gm1s[0]);
      fdws= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("omegs",MJWL.gm1s[1]);
      fdwg= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("omegg",MJWL.gm1s[2]);
      
      fdci= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvi",MJWL.ci);
      fdcs= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvs",MJWL.cs);
      fdcg= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cvg",MJWL.cg);
      
      fda11= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl11",MJWL.amjwl[0][0]);
      fda21= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl21",MJWL.amjwl[0][1]);
      fdr11= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl11",MJWL.rmjwl[0][0]);
      fdr21= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl21",MJWL.rmjwl[0][1]);
      
      fda12= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl12",MJWL.amjwl[1][0]);
      fda22= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl22",MJWL.amjwl[1][1]);
      fdr12= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl12",MJWL.rmjwl[1][0]);
      fdr22= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl22",MJWL.rmjwl[1][1]);
      
      fda13= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ai1",MJWL.ai[0]);
      fda23= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ai2",MJWL.ai[1]);
      fdr13= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ri1",MJWL.ri[0]);
      fdr23= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ri2",MJWL.ri[1]);
      
      fdvi0= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("vi0",MJWL.mvi0);
      fdvs0= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("vs0",MJWL.mvs0);
      fdvg0= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("vg0",MJWL.mvg0);
      fdheat= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("heat",MJWL.mjwlq);
      vi0=MJWL.mvi0;
      vs0=MJWL.mvs0;
      vg0=MJWL.mvg0;
      
      if( !fdws || !fda11 || !fda21 || !fdr11 || !fdr21 )
      {
	printf("Error (setUserDefineParameters) : undefined solid multi-JWL EOS parameter(s)\n");
	exit(0);
      }
      if( !fdwg || !fda12 || !fda22 || !fdr12 || !fdr22 )
      {
	printf("Error (setUserDefineParameters) : undefined gas multi-JWL EOS parameter(s)\n");
	exit(0);
      }
      if( !fdwi || !fda13 || !fda23 || !fdr13 || !fdr23 )
      {
	printf("Error (setUserDefineParameters) : undefined inert multi-JWL EOS parameter(s)\n");
	exit(0);
      }
      if( !fdvi0 || !fdvs0 || !fdvg0 || !fdci || !fdcs || !fdcg || !fdheat )
      {
	printf("Error (setUserDefineParameters) : undefined equil. or heat EOS parameter(s)\n");
	exit(0);
      }
      MJWL.iterations=0;
      MJWL.fs0=0.;
      MJWL.fg0=0.;
      MJWL.gs0=0.;
      MJWL.gg0=0.;
      MJWL.fi0=0.;
      MJWL.gi0=0.;
      
      real vi1,fi1=0.;
      if( !( dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("vi1",vi1)) )
      {
	vi1=vi0;
      }
      for( int i=0; i<2; i++ )
      {
	MJWL.fi0+=MJWL.ai[i]*(vi0/MJWL.gm1s[0]-1./MJWL.ri[i])*exp(-MJWL.ri[i]*vi0);
	MJWL.fs0+=MJWL.amjwl[0][i]*(vs0/MJWL.gm1s[1]-1./MJWL.rmjwl[0][i])*exp(-MJWL.rmjwl[0][i]*vs0);
	MJWL.fg0+=MJWL.amjwl[1][i]*(vg0/MJWL.gm1s[2]-1./MJWL.rmjwl[1][i])*exp(-MJWL.rmjwl[1][i]*vg0);
	MJWL.gi0+=MJWL.ai[i]/MJWL.ri[i]*exp(-MJWL.ri[i]*vi0);
	MJWL.gs0+=MJWL.amjwl[0][i]/MJWL.rmjwl[0][i]*exp(-MJWL.rmjwl[0][i]*vs0);
	MJWL.gg0+=MJWL.amjwl[1][i]/MJWL.rmjwl[1][i]*exp(-MJWL.rmjwl[1][i]*vg0);

	fi1+=MJWL.ai[i]*(vi1/MJWL.gm1s[0]-1./MJWL.ri[i])*exp(-MJWL.ri[i]*vi1);
      }
      
      // find  iheat 
      //MJWL.iheat=MJWL.mjwlq;
      MJWL.iheat=MJWL.mjwlq+fi1-MJWL.fi0;

      if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("newMethod",MJWL.newMethod) )
      {
	printf(" setUserDefinedParameters: setting newMethod=%i\n", MJWL.newMethod);
      }
      else
      {
	// Default is to use old method
	MJWL.newMethod=0;
      }
      if( MJWL.newMethod )
      {
	// set up eosdat common block to mimic multijwl common block
	for( int i=0; i<2; i++ )
	{
	  EOSDAT.omeg[i]=MJWL.gm1s[i+1];
	  for( int j=0; j<2; j++ )
	  {
	    EOSDAT.ajwl[i][j]=MJWL.amjwl[i][j];
	    EOSDAT.rjwl[i][j]=MJWL.rmjwl[i][j];
	  }
	}
	EOSDAT.vs0=MJWL.mvs0;
	EOSDAT.vg0=MJWL.mvg0;
	EOSDAT.fsvs0=MJWL.fs0;
	EOSDAT.fgvg0=MJWL.fg0;
	EOSDAT.zsvs0=MJWL.fs0+MJWL.gs0;
	EOSDAT.zgvg0=MJWL.fg0+MJWL.gg0;
	EOSDAT.cgcs=MJWL.cs*MJWL.cg;
	EOSDAT.heat=MJWL.mjwlq;
	EOSDAT.ts0=1.0;
      }
      else
      {
	// set up eosdat common block to mimic solid and gas (called when  dbase.get<real >("mu")=1)
	for( int i=0; i<2; i++ )
	{
	  EOSDAT.omeg[i]=MJWL.gm1s[i+1];
	  for( int j=0; j<2; j++ )
	  {
	    EOSDAT.ajwl[i][j]=MJWL.amjwl[i][j];
	    EOSDAT.rjwl[i][j]=MJWL.rmjwl[i][j];
	  }
	}
	EOSDAT.vs0=MJWL.mvs0;
	EOSDAT.vg0=MJWL.mvg0;
	EOSDAT.fsvs0=MJWL.fs0;
	EOSDAT.fgvg0=MJWL.fg0;
	EOSDAT.zsvs0=MJWL.fs0+MJWL.gs0;
	EOSDAT.zgvg0=MJWL.fg0+MJWL.gg0;
	EOSDAT.cgcs=MJWL.cs*MJWL.cg;
	EOSDAT.heat=MJWL.mjwlq;
	EOSDAT.ts0=1.0;

	// set up eosburntdat common block for burnt stuff (called when lam=1)
	EOSBURNTDAT.omeg[0]=MJWL.gm1s[0];
	EOSBURNTDAT.omeg[1]=MJWL.gm1s[2];

	EOSBURNTDAT.ajwl[0][0]=MJWL.ai[0];
	EOSBURNTDAT.ajwl[0][1]=MJWL.ai[1];
	EOSBURNTDAT.ajwl[1][0]=MJWL.amjwl[1][0];
	EOSBURNTDAT.ajwl[1][1]=MJWL.amjwl[1][1];

	EOSBURNTDAT.rjwl[0][0]=MJWL.ri[0];
	EOSBURNTDAT.rjwl[0][1]=MJWL.ri[1];
	EOSBURNTDAT.rjwl[1][0]=MJWL.rmjwl[1][0];
	EOSBURNTDAT.rjwl[1][1]=MJWL.rmjwl[1][1];

	EOSBURNTDAT.vs0=MJWL.mvi0;
	EOSBURNTDAT.vg0=MJWL.mvg0;
	EOSBURNTDAT.fsvs0=MJWL.fi0;
	EOSBURNTDAT.fgvg0=MJWL.fg0;
	EOSBURNTDAT.zsvs0=MJWL.fi0+MJWL.gi0;
	EOSBURNTDAT.zgvg0=MJWL.fg0+MJWL.gg0;
	EOSBURNTDAT.cgcs=MJWL.ci*MJWL.cg;
	EOSBURNTDAT.heat=MJWL.iheat;
	EOSBURNTDAT.ts0=1.0;
      }
    }
    else
    {
      // single component JWL
      bool fdomeg1, fdajwl11, fdajwl21, fdrjwl11, fdrjwl21;
      bool fdomeg2, fdajwl12, fdajwl22, fdrjwl12, fdrjwl22;
      bool fdvs0, fdvg0, fdcgcs, fdheat;

      // Remember A and R are stored as FORTRAN arrays!!!
      fdomeg1= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("omeg1",EOSDAT.omeg[0]);
      fdajwl11= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl11",EOSDAT.ajwl[0][0]);
      fdajwl21= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl21",EOSDAT.ajwl[0][1]);
      fdrjwl11= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl11",EOSDAT.rjwl[0][0]);
      fdrjwl21= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl21",EOSDAT.rjwl[0][1]);

      fdomeg2= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("omeg2",EOSDAT.omeg[1]);
      fdajwl12= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl12",EOSDAT.ajwl[1][0]);
      fdajwl22= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ajwl22",EOSDAT.ajwl[1][1]);
      fdrjwl12= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl12",EOSDAT.rjwl[1][0]);
      fdrjwl22= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rjwl22",EOSDAT.rjwl[1][1]);

      fdvs0= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("vs0",EOSDAT.vs0);
      fdvg0= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("vg0",EOSDAT.vg0);
      fdcgcs= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cgcs",EOSDAT.cgcs);
      fdheat= dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("heat",EOSDAT.heat);

      EOSDAT.zsvs0=EOSDAT.vs0*(EOSDAT.ajwl[0][0]*exp(-EOSDAT.rjwl[0][0]*EOSDAT.vs0)+EOSDAT.ajwl[0][1]*exp(-EOSDAT.rjwl[0][1]*EOSDAT.vs0))/EOSDAT.omeg[0];
      EOSDAT.fsvs0=EOSDAT.zsvs0-EOSDAT.ajwl[0][0]*exp(-EOSDAT.rjwl[0][0]*EOSDAT.vs0)/EOSDAT.rjwl[0][0]-EOSDAT.ajwl[0][1]*exp(-EOSDAT.rjwl[0][1]*EOSDAT.vs0)/EOSDAT.rjwl[0][1];
      EOSDAT.zgvg0=EOSDAT.vg0*(EOSDAT.ajwl[1][0]*exp(-EOSDAT.rjwl[1][0]*EOSDAT.vg0)+EOSDAT.ajwl[1][1]*exp(-EOSDAT.rjwl[1][1]*EOSDAT.vg0))/EOSDAT.omeg[1];
      EOSDAT.fgvg0=EOSDAT.zgvg0-EOSDAT.ajwl[1][0]*exp(-EOSDAT.rjwl[1][0]*EOSDAT.vg0)/EOSDAT.rjwl[1][0]-EOSDAT.ajwl[1][1]*exp(-EOSDAT.rjwl[1][1]*EOSDAT.vg0)/EOSDAT.rjwl[1][1];


      EOSDAT.ts0=1.0;

      if( !fdomeg1 || !fdajwl11 || !fdajwl21 || !fdrjwl11 || !fdrjwl21 )
      {
	printf("Error (setUserDefinedParameters) : undefined solid JWL EOS parameter(s)\n");
	exit(0);
      }
      if( !fdomeg2 || !fdajwl12 || !fdajwl22 || !fdrjwl12 || !fdrjwl22 )
      {
	printf("Error (setUserDefinedParameters) : undefined gas JWL EOS parameter(s)\n");
	exit(0);
      }
      if( !fdvs0 || !fdvg0 || !fdcgcs || !fdheat )
      {
	printf("Error (setUserDefinedParameters) : undefined equil. or heat EOS parameter(s)\n");
	exit(0);
      }
    }
  }

  if(  equationOfState==mieGruneisenEOS )
  {
    EOSDAT.eospar[0]=0.;
    EOSDAT.eospar[1]=0.;
    EOSDAT.eospar[2]=1.;
    EOSDAT.eospar[3]=1.;  // This is kappa,  Cp = Cv + kappa*R

    real alphaMG, betaMG,v0MG, kappaMG;
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("alphaMG",alphaMG) )
    {
      printf(" setUserDefinedParameters: setting alphaMG=%9.3e\n",alphaMG);
      EOSDAT.eospar[0]=alphaMG;
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("betaMG",betaMG) )
    {
      printf(" setUserDefinedParameters: setting betaMG=%9.3e\n",betaMG);
      EOSDAT.eospar[1]=betaMG;
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("V0MG",v0MG) )
    {
      printf(" setUserDefinedParameters: setting v0MG=%9.3e\n",v0MG);
      EOSDAT.eospar[2]=v0MG;
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("kappaMG",kappaMG) )
    {
      printf(" setUserDefinedParameters: setting kappaMG=%9.3e\n",kappaMG);
      EOSDAT.eospar[3]=kappaMG;
    }
  }

  // ************ do this for now ***********
  if( equationOfState==stiffenedGasEOS ||
      equationOfState==taitEOS )
  {
    EOSDAT.eospar[0]=0.;
    EOSDAT.eospar[1]=0.;
    EOSDAT.eospar[2]=1.;
    EOSDAT.eospar[3]=1.;  // This is kappa,  Cp = Cv + kappa*R

    real alphaMG, betaMG,v0MG, kappaMG;
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("alphaMG",alphaMG) )
    {
      printf(" setUserDefinedParameters: setting alphaMG=%9.3e\n",alphaMG);
      EOSDAT.eospar[0]=alphaMG;
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("betaMG",betaMG) )
    {
      printf(" setUserDefinedParameters: setting betaMG=%9.3e\n",betaMG);
      EOSDAT.eospar[1]=betaMG;
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("V0MG",v0MG) )
    {
      printf(" setUserDefinedParameters: setting v0MG=%9.3e\n",v0MG);
      EOSDAT.eospar[2]=v0MG;
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("kappaMG",kappaMG) )
    {
      printf(" setUserDefinedParameters: setting kappaMG=%9.3e\n",kappaMG);
      EOSDAT.eospar[3]=kappaMG;
    }
  }

  if(  dbase.get<CnsParameters::PDE >("pde")==compressibleMultiphase )
  {
    GASDAT.gam[0]=1.4;    // default values
    GASDAT.gam[1]=1.4;
    GASDAT.ps0=0.0;
    GASDAT.bgas=0.0;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gammaSolid",GASDAT.gam[0]) )
    {
      printf(" setUserDefinedParameters: setting gammaSolid=%9.3e\n",GASDAT.gam[0]);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("gammaGas",GASDAT.gam[1]) )
    {
      printf(" setUserDefinedParameters: setting gammaGas=%9.3e\n",GASDAT.gam[1]);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("p0Solid",GASDAT.ps0) )
    {
      printf(" setUserDefinedParameters: setting p0Solid=%9.3e\n",GASDAT.ps0);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("bGas",GASDAT.bgas) )
    {
      printf(" setUserDefinedParameters: setting bGas=%9.3e\n",GASDAT.bgas);
    }

    for ( int j=0; j<=1; j++ )
    {
      GASDAT.gm1[j]=GASDAT.gam[j]-1.0;
      GASDAT.gp1[j]=GASDAT.gam[j]+1.0;
      GASDAT.em[j]=0.5*GASDAT.gm1[j]/GASDAT.gam[j];
      GASDAT.ep[j]=0.5*GASDAT.gp1[j]/GASDAT.gam[j];
    }

    SRCPRM.delta=0.;    // default values
    SRCPRM.rmuc=0.;
    SRCPRM.htrans=0.;
    SRCPRM.cratio=1.;
    SRCPRM.abmin=.0001;
    SRCPRM.abmax=.9999;
    SRCPRM.isrc=0;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("delta",SRCPRM.delta) )
    {
      printf(" setUserDefinedParameters: setting delta=%9.3e\n",SRCPRM.delta);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rmuc",SRCPRM.rmuc) )
    {
      printf(" setUserDefinedParameters: setting rmuc=%9.3e\n",SRCPRM.rmuc);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("htrans",SRCPRM.htrans) )
    {
      printf(" setUserDefinedParameters: setting htrans=%9.3e\n",SRCPRM.htrans);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("cratio",SRCPRM.cratio) )
    {
      printf(" setUserDefinedParameters: setting cratio=%9.3e\n",SRCPRM.cratio);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("abmin",SRCPRM.abmin) )
    {
      printf(" setUserDefinedParameters: setting abmin=%9.3e\n",SRCPRM.abmin);
    }
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("abmax",SRCPRM.abmax) )
    {
      printf(" setUserDefinedParameters: setting abmax=%9.3e\n",SRCPRM.abmax);
    }

    if( SRCPRM.delta>0. || SRCPRM.rmuc>0. || SRCPRM.htrans>0 )
      SRCPRM.isrc=1;

    COMDAT.cfact1=0.;    // default values
    COMDAT.cfact2=1.;
    COMDAT.heat=0.;

    real asRef=0.;
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("asRef",asRef) )
    {
      printf(" setUserDefinedParameters: setting asRef=%9.3e\n",asRef);
    }
    real rsRef=0.;
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rsRef",rsRef) )
    {
      printf(" setUserDefinedParameters: setting rsRef=%9.3e\n",rsRef);
    }
    real psRef=0.;
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("psRef",psRef) )
    {
      printf(" setUserDefinedParameters: setting psRef=%9.3e\n",psRef);
    }
    real pgRef=0.;
    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pgRef",pgRef) )
    {
      printf(" setUserDefinedParameters: setting pgRef=%9.3e\n",pgRef);
    }

    if( asRef!=0. && rsRef!=0. && psRef!=0. && pgRef!=0. )
    {
      COMDAT.cfact1=(pgRef-psRef)*pow(2.-asRef,2.)/(asRef*rsRef*log(1.-asRef));
      COMDAT.cfact2=(2.-asRef)/pow(1.-asRef,(1.-asRef)/(2.-asRef));
      printf(" setUserDefinedParameters: setting cfact1,cfact2=%9.3e %9.3e\n",COMDAT.cfact1,COMDAT.cfact2);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("heat",COMDAT.heat) )
    {
      printf(" setUserDefinedParameters: setting heat=%9.3e\n",COMDAT.heat);
    }

    CMPSRC.tol=1.e-4;    // default values
    CMPSRC.qmax=4.;
    CMPSRC.qmin=.1;
    CMPSRC.itmax=500;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("tol",CMPSRC.tol) )
    {
      printf(" setUserDefinedParameters: setting tol=%9.3e\n",CMPSRC.tol);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("qmax",CMPSRC.qmax) )
    {
      printf(" setUserDefinedParameters: setting qmax=%9.3e\n",CMPSRC.qmax);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("qmin",CMPSRC.qmin) )
    {
      printf(" setUserDefinedParameters: setting qmin=%9.3e\n",CMPSRC.qmin);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("itmax",CMPSRC.itmax) )
    {
      printf(" setUserDefinedParameters: setting itmax=%6d\n",CMPSRC.itmax);
    }

    CMPRXN.sigma=0.;    // default values
    CMPRXN.pgi=0.;
    CMPRXN.anu=1.;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("sigma",CMPRXN.sigma) )
    {
      printf(" setUserDefinedParameters: setting sigma=%9.3e\n",CMPRXN.sigma);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pgi",CMPRXN.pgi) )
    {
      printf(" setUserDefinedParameters: setting pgi=%9.3e\n",CMPRXN.pgi);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("anu",CMPRXN.anu) )
    {
      printf(" setUserDefinedParameters: setting anu=%9.3e\n",CMPRXN.anu);
    }

    if( CMPRXN.sigma>0. )
      SRCPRM.isrc=1;

//      common / cmpign / sigmai,pfref,ab0,anui,phieps,phimin
    CMPIGN.sigmai=0.;    // default values
    CMPIGN.pfref=1.;
    CMPIGN.ab0=0.73;
    CMPIGN.anui=1.;
    CMPIGN.phieps=1.;
    CMPIGN.phimin=1.;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("sigmai",CMPIGN.sigmai) )
    {
      printf(" setUserDefinedParameters: setting sigmai=%9.3e\n",CMPIGN.sigmai);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pfref",CMPIGN.pfref) )
    {
      printf(" setUserDefinedParameters: setting pfref=%9.3e\n",CMPIGN.pfref);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("ab0",CMPIGN.ab0) )
    {
      printf(" setUserDefinedParameters: setting ab0=%9.3e\n",CMPIGN.ab0);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("anui",CMPIGN.anui) )
    {
      printf(" setUserDefinedParameters: setting anui=%9.3e\n",CMPIGN.anui);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("phieps",CMPIGN.phieps) )
    {
      printf(" setUserDefinedParameters: setting phieps=%9.3e\n",CMPIGN.phieps);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("phimin",CMPIGN.phimin) )
    {
      printf(" setUserDefinedParameters: setting phimin=%9.3e\n",CMPIGN.phimin);
    }

    if( CMPIGN.sigmai>0. )
      SRCPRM.isrc=1;

    CMPFLX.rtol=1.e-10;    // default values
    CMPFLX.lcont=0;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("rtol",CMPFLX.rtol) )
    {
      printf(" setUserDefinedParameters: setting rtol=%9.3e\n",CMPFLX.rtol);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("lcont",CMPFLX.lcont) )
    {
      printf(" setUserDefinedParameters: setting lcont=%3d\n",CMPFLX.lcont);
    }

    CMPROM.atol=1.e-10;    // default values
    CMPROM.nrmax=20;

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("atol",CMPROM.atol) )
    {
      printf(" setUserDefinedParameters: setting atol=%9.3e\n",CMPROM.atol);
    }

    if(  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("nrmax",CMPROM.nrmax) )
    {
      printf(" setUserDefinedParameters: setting nrmax=%3d\n",CMPROM.nrmax);
    }

//     CMPMID.toli=1.e-4;    // default values
//     CMPMID.tolv=1.e-4;
//     CMPMID.itmax=6;
// 
//     if( pdeParameters.getParameter("toli",CMPMID.toli) )
//     {
//       printf(" setUserDefinedParameters: setting toli=%9.3e\n",CMPMID.toli);
//     }
// 
//     if( pdeParameters.getParameter("toli",CMPMID.tolv) )
//     {
//       printf(" setUserDefinedParameters: setting tolv=%9.3e\n",CMPMID.tolv);
//     }
// 
//     if( pdeParameters.getParameter("itmax",CMPMID.itmax) )
//     {
//       printf(" setUserDefinedParameters: setting itmax=%3d\n",CMPMID.itmax);
//     }

  }

  return 0;
}



