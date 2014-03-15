//
// Define the reaction mechanism for the H_2 - F_2 reaction
//
#include "Reactions.h"
#include "Overture.h"

#ifdef OV_USE_DOUBLE
  #define GECO EXTERN_C_NAME(dgeco)
  #define GESL EXTERN_C_NAME(dgesl)
  #define GEEV EXTERN_C_NAME(dgeev)
#else
  #define GECO EXTERN_C_NAME(sgeco)
  #define GESL EXTERN_C_NAME(sgesl)
  #define GEEV EXTERN_C_NAME(sgeev)
#endif

extern "C"
{
  void GECO( real & b, const int & nbd, int & nb, int & ipvt,real & rcond, real & work );

  void SGEDI( real & b, const int & nbd, int & nb, int & ipvt,real & det, real & work, 
              const int & job );

  void GESL( real & a, const int & lda,const int & n,const int & ipvt, real & b, const int & job);

  void GEEV( char jobvl[], char jobvr[], int & n, real & a, int & lda, real & wr, real & wi,
	      real & vl, int & ldlv, real & vr, int & ldvr, real & work, int & lwork, int & info,
              const int len_jobvl, const int len_jobvr );
}

/* ----
void Reactions::chemicalSource(const real & rho, const real & te, const RealArray & y, 
			       RealArray & sigma, RealArray & sy )
{
  cout << "Reactions::chemicalSource:ERROR: base class being called" << endl;
  throw "error";
}
---- */

int Reactions::debug=0;

//\begin{>ReactionsInclude.tex}{\subsection{constructor}} 
Reactions::
Reactions()
//=========================================================================================
// /Description:
//    Constructor. This is a base class for interfacing to general reactions. This
//  object should never be built, only derived classes such as Chemkin should be created.
//
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  numberOfSpecies_=0;  
  rho0=l0=u0=pStatic=p0=te0=1.; 
  numberOfElements_=0;
  numberOfReactions_=0;
  speciesName=NULL;
  speciesNumber=NULL;
  reactionMechanism=nonEquilibrium;
  pressureIsConstant=FALSE;
  pressureLevel=0.;
  variablesScaled=TRUE;
}

Reactions::
~Reactions()
{
  delete speciesName;
  delete speciesNumber;
}


//\begin{>>ReactionsInclude.tex}{\subsection{getName}} 
aString Reactions::
getName( const int & species ) const
//=========================================================================================
// /Description:
//   Return the name of a species
// /species (input) : a value from 0 to numberOfSpecies-1.
// /Return value: name of the species such as "H" or "N2".
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  if( species<0 || species>=numberOfSpecies_ )
  {
    cout << "Reactions::getName:ERROR invalid value for species = " << species << endl;
    throw "error";
  }
  return speciesName[species];
}

//\begin{>>ReactionsInclude.tex}{\subsection{setScaleForRho}} 
int Reactions::
setScaleForRho( real rho0_ )
//=========================================================================================
// /Description:
//    Set a scaling factor.
//
// /rho0\_ : scaling factor.
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  rho0=rho0_;

  R0=pStatic/(rho0*te0);
  rho0Saved=rho0;
  return 0;
}

//\begin{>>ReactionsInclude.tex}{\subsection{setScaleForT}} 
int Reactions:: 
setScaleForT( real te0_ )
//=========================================================================================
// /Description:
//    Set a scaling factor.
//
// /te0\_ : scaling factor.
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  te0=te0_;

  R0=pStatic/(rho0*te0);
  te0Saved=te0;
  return 0;
}

//\begin{>>ReactionsInclude.tex}{\subsection{setScaleForP}} 
int Reactions::
setScaleForP( real p0_, real pStatic_ )
//=========================================================================================
// /Description:
//    Set a scaling factor.
//
// \begin{verbatim}
//    p = (pressureLevel + pS)*p0   
//    R0 = pStatic/(rho0*te0)
// \end{verbatim}
//
// /p0\_,pStatic\_ : scaling factor.
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  p0=p0_;
  pStatic=pStatic_;

  R0=pStatic/(rho0*te0);
  p0Saved=p0;
  pStaticSaved=pStatic;
  return 0;

}

int Reactions:: 
setScaleForL( real l0_ )
{
  l0=l0_;
  l0Saved=l0;
  return 0;
}

int Reactions:: 
setScaleForU( real u0_ )
{
  u0=u0_;
  u0Saved=u0;
  return 0;
}


//\begin{>>ReactionsInclude.tex}{\subsection{setScales}} 
void Reactions:: 
setScales( real rho0_, 
           real te0_, 
           real pStatic_, 
           real l0_, 
           real u0_,
           real p0_ )
//=========================================================================================
// /Description:
//    Set scaling factors.
// \begin{verbatim}
//    T = Ts*te
//    p = (pressureLevel + pS)*p0   
//    R0 = pStatic/(rho0*te0)
// \end{verbatim}
//
// /rho,te,pStatic\_,l,u : scaling factors.
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  rho0=rho0_;
  te0=te0_;
  pStatic=pStatic_;
  l0=l0_;
  u0=u0_;
  p0=p0_;

  // p0=rho0*u0*u0;  // define p0 in this way (scale for the dynamic pressure)
  
  R0=pStatic/(rho0*te0);

  rho0Saved=rho0; pStaticSaved=pStatic; p0Saved=p0; te0Saved=te0; l0Saved=l0; u0Saved=u0; R0Saved=R0; 
}

real Reactions::
getScaleForRho() const
{
  return rho0;
}

real Reactions::
getScaleForT( ) const
{
  return te0;
}

real Reactions::
getScaleForP() const
{
  return p0;
}
real Reactions::
getScaleForStaticP() const
{
  return pStatic;
}

real Reactions::
getScaleForL( ) const
{
  return l0;
}

real Reactions::
getScaleForU() const
{
  return u0;
}

real Reactions::
getScaleForR() const
{
  return R0;
}

void Reactions::
getScales(real & rho0_, 
	  real & te0_, 
	  real & pStatic_,  
	  real & l0_,  
	  real & u0_,
	  real & p0_ ) const
{
  rho0_=rho0;
  te0_=te0;
  p0_=p0;
  pStatic_=pStatic;
  l0_=l0;
  u0_=u0;
}


//\begin{>>ReactionsInclude.tex}{\subsection{useScaledVariables}} 
void Reactions::
useScaledVariables(bool trueOrFalse /* =TRUE */ )
// ======================================================================================================
// /Description:
//
// /trueOrFalse (input):
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  variablesScaled=trueOrFalse;
  if( !variablesScaled )
  {
    // set all scalings to 1
    rho0=l0=u0=pStatic=p0=te0=1.;
  }
  else
  {
    rho0=rho0Saved; pStatic=pStaticSaved; p0=p0Saved; te0=te0Saved; l0=l0Saved; u0=u0Saved; R0=R0Saved; 
  }
}



//\begin{>>ReactionsInclude.tex}{\subsection{setPressureIsConstant}} 
void Reactions::
setPressureIsConstant( const bool & trueOrFalse /* =TRUE */ )
// ======================================================================================================
// /Description:
//   Set to 'true' if this is a constant pressure reaction.
//
// /trueOrFalse (input):
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
 pressureIsConstant=trueOrFalse;
}

//\begin{>>ReactionsInclude.tex}{\subsection{setPressureLevel}}
void Reactions::
setPressureLevel(const real & pressureLevel_)
// ======================================================================================================
// /Description:
//   Scaled pressure is (pScaled + pressureLevel)*p0 = p
//
// /pressureLevel\_ (input):
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  pressureLevel=pressureLevel_;
}

//\begin{>>ReactionsInclude.tex}{\subsection{getPressureIsConstant}}
bool Reactions::
getPressureIsConstant()
// ======================================================================================================
// /Description:
//   Return TRUE for constant pressure reactions.
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  return pressureIsConstant;
}

//\begin{>>ReactionsInclude.tex}{\subsection{massFractionToMoleFraction}}
RealArray & Reactions::
massFractionToMoleFraction( const RealArray & y, const RealArray & x_ )
// ======================================================================================================
// /Description:
//   Compute the mole fraction from the mass fraction
//\end{ReactionsInclude.tex}  
//=========================================================================================
{
  RealArray & x = (RealArray&) x_;
  
  Range R1(y.getBase(0),y.getBound(0)),
        R2(y.getBase(1),y.getBound(1)),
        R3(y.getBase(2),y.getBound(2));
  
  RealArray mBarInverse(R1,R2,R3);
  mBarInverse=0.;
  int yBase3=y.getBase(3), xBase3=x.getBase(3);
  int s;
  for( s=0; s<numberOfSpecies_; s++ )
    mBarInverse(R1,R2,R3)+=y(R1,R2,R3,s+yBase3)*(1./mw(s));

  for( s=0; s<numberOfSpecies_; s++ )
    x(R1,R2,R3,s+xBase3)=y(R1,R2,R3,s+yBase3)/(mBarInverse(R1,R2,R3)*mw(s));
  return x;
}

//\begin{>>ReactionsInclude.tex}{\subsection{chemicalSource}}
void Reactions::
chemicalSource(const RealArray & rhoS, 
	       const RealArray & teS, 
	       const RealArray & y, 
	       const RealArray & sigma_, 
	       const RealArray & sy_ /* =Overture::nullRealArray() */ )
// ======================================================================================================
//  /Description: Compute the chemical source term, sigma, and/or the jacobian of sigma/rho at an array
//     of points.
//
// /rhoS (input): density (scaled by rho0)
// /teS (input): temperature (scaled)
// /y (input): mass fractions
// /sigma (output): source term, (unscaled)  (if sigma!=nullArray)
// /sy (output): d(sigma/rho)/dy (unscaled) (if sy!=nullArray)
// 
//\end{ReactionsInclude.tex}  
// ===========================================================================================
{
  RealArray & sigma = (RealArray&) sigma_;
  RealArray & sy = (RealArray&) sy_;

  RealArray yI(numberOfSpecies_), sigmaI, syI;
  if( sigma.getLength(0)>0 )
    sigmaI.redim(numberOfSpecies_);
  if( sy.getLength(0)>0 )
    syI.redim(numberOfSpecies_,numberOfSpecies_);

  int rhoBase3=rhoS.getBase(3);
  int teBase3=teS.getBase(3);
  
  int i1s = sigma.getBase(0)-rhoS.getBase(0);   // **** fix for A++ bug ++++
  int i2s = sigma.getBase(1)-rhoS.getBase(1);
  int i3s = sigma.getBase(2)-rhoS.getBase(2);

  int i1y = y.getBase(0)-rhoS.getBase(0);
  int i2y = y.getBase(1)-rhoS.getBase(1);
  int i3y = y.getBase(2)-rhoS.getBase(2);
  int sBase = y.getBase(3);

  int s;
  for( int i3=rhoS.getBase(2); i3<=rhoS.getBound(2); i3++ )
  for( int i2=rhoS.getBase(1); i2<=rhoS.getBound(1); i2++ )
  for( int i1=rhoS.getBase(0); i1<=rhoS.getBound(0); i1++ )
  {
    for( s=0; s<numberOfSpecies_; s++ )
      yI(s)=y(i1+i1y,i2+i2y,i3+i3y,s+sBase);
    chemicalSource(rhoS(i1,i2,i3,rhoBase3),teS(i1,i2,i3,teBase3),yI,sigmaI,syI);
    if( sigma.getLength(0)>0 )
    {
      for( s=0; s<numberOfSpecies_; s++ )
        sigma(i1+i1s,i2+i2s,i3+i3s,s)=sigmaI(s);
    }
    if( sy.getLength(0)>0 )
    {
      for( int s1=0; s1<numberOfSpecies_; s1++ )
      for( int s2=0; s2<numberOfSpecies_; s2++ )
        sy(i1,i2,i3,s1,s2)=syI(s1,s2);
    }
    
  }
}



#undef MP
#define MP MaterialProperties

extern MaterialProperties mp;   // ********* fix this *******

//\begin{>>ReactionsInclude.tex}{\subsection{mw}}
real Reactions::
mw(const int & species ) const
// =================================================================================
// /Description:
// molecular weight of a species (kg/mole)
//\end{ReactionsInclude.tex}  
// =================================================================================
{
  if( species<0 || species>=numberOfSpecies_ )
  {
    cout << "Reactions::mw:ERROR invalid value for species = " << species << endl;
    throw "error";
  }
  return mp.mw(speciesNumber[species]); //   ***** *mp.gramsPerKilogram;  ** fixed 96/11/05
}

//\begin{>>ReactionsInclude.tex}{\subsection{h}}
real Reactions::
h(const int species, const real teS )
//====================================================================================
// /Description:
//   Return the ABSOLUTE enthalpy of a species
// /teS (input) : temperature (scaled)
// /h0 (output) : enthalpy (unscaled) in Mass units J/Kg
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( species<0 || species>=numberOfSpecies_ )
  {
    cout << "Reactions::h:ERROR invalid value for species = " << species << endl;
    throw "error";
  }
  return mp.hF(speciesNumber[species],teS*te0)/mw(species);  // mp returns mole units.
}

//\begin{>>ReactionsInclude.tex}{\subsection{h}}
RealArray & Reactions::
h(const int species, const RealArray & teS, const RealArray & h0_ )
//====================================================================================
// /Description:
// return the ABSOLUTE enthalpy of a species at an array of values
// /teS (input) : temperature (scaled)
// /h0 (output) : cp (unscaled) in Mass units J/Kg
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & h0 = (RealArray&) h0_;

  if( species<0 || species>=numberOfSpecies_ )
  {
    cout << "Reactions::h:ERROR invalid value for species = " << species << endl;
    throw "error";
  }
  for( int i3=teS.getBase(2); i3<=teS.getBound(2); i3++ )
  for( int i2=teS.getBase(1); i2<=teS.getBound(1); i2++ )
  for( int i1=teS.getBase(0); i1<=teS.getBound(0); i1++ )
  {
    // h0(i1,i2,i3)=mp.hF(speciesNumber[species],teS(i1,i2,i3)*te0)/mw(species);
    h0(i1,i2,i3)=mp.hF(speciesNumber[species],teS(i1,i2,i3)*te0)/mw(species); 
  }
  return h0;
}


//\begin{>>ReactionsInclude.tex}{\subsection{cp}}
real Reactions::
cp(const int species, const real teS )
//====================================================================================
// /Description:
// return the cp of a species
// /teS (input) : temperature (scaled)
// /cp0 (output) : cp (unscaled) in Mass units J/(Kg-K)
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( species<0 || species>=numberOfSpecies_ )
  {
    cout << "Reactions::cp:ERROR invalid value for species = " << species << endl;
    throw "error";
  }
  return mp.cp(speciesNumber[species],teS*te0)/mw(species);
}


//\begin{>>ReactionsInclude.tex}{\subsection{cp}}
RealArray & Reactions::
cp(const int species, const RealArray & teS, const RealArray & cp0_ )
//====================================================================================
// /Description:
// return the cp of a species at an array of values
// /teS (input) : temperature (scaled)
// /cp0\_ (output) : cp (unscaled) in Mass units J/(Kg-K)
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & cp0 = (RealArray&) cp0_;

  if( species<0 || species>=numberOfSpecies_ )
  {
    cout << "Reactions::cp:ERROR invalid value for species = " << species << endl;
    throw "error";
  }
  for( int i3=teS.getBase(2); i3<=teS.getBound(2); i3++ )
  for( int i2=teS.getBase(1); i2<=teS.getBound(1); i2++ )
  for( int i1=teS.getBase(0); i1<=teS.getBound(0); i1++ )
  {
    cp0(i1,i2,i3)=mp.cp(speciesNumber[species],teS(i1,i2,i3)*te0)/mw(species);
  }
  return cp0;
}


//\begin{>>ReactionsInclude.tex}{\subsection{entropy}}
RealArray & Reactions::
entropy(const RealArray & teS, const RealArray & pS, const RealArray & y, const RealArray & entropy_ )
//====================================================================================
// /Description:
// return the entropy of a species at an array of values
// /teS, pS, y (input) : scaled temperature, scaled pressure and mass fraction
// /entropy\_ (output) : entropy (unscaled) units ?
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & entropy = (RealArray&) entropy_;

  entropy.redim(pS);
  entropy=-mp.R*log((pS+pressureLevel)*p0);
  int sBase=y.getBase(3);
  int s;
  for( int i3=teS.getBase(2); i3<=teS.getBound(2); i3++ )
  for( int i2=teS.getBase(1); i2<=teS.getBound(1); i2++ )
  for( int i1=teS.getBase(0); i1<=teS.getBound(0); i1++ )
  {
    real mBar=0.;
    for( s=0; s<numberOfSpecies_; s++ )
      mBar+=y(i1,i2,i3,s+sBase)/mp.mw(speciesNumber[s]);
    mBar=1./mBar;
    for( s=0; s<numberOfSpecies_; s++ )
    {
      real x=y(i1,i2,i3,s+sBase)*mBar/mp.mw(speciesNumber[s]);
      entropy(i1,i2,i3)+=x*mp.s(speciesNumber[s],teS(i1,i2,i3)*te0) - mp.R*x*log(max(x,REAL_MIN));
    }
    entropy(i1,i2,i3)/=mBar;
  }
  return entropy;
}



//\begin{>>ReactionsInclude.tex}{\subsection{h}}
RealArray & Reactions::
h(const RealArray & teS, const RealArray & hi_ )
//====================================================================================
// /Description:
//    Compute the enthalpies for all species.
// /te(I1,I2,I3) (input) : temperature, (scaled)
// /hi(I1,I2,I3,S) (output) : enthalpies (unscaled) in mass units, MKS, J/Kg 
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & hi = (RealArray&) hi_;
  for( int i3=teS.getBase(2); i3<=teS.getBound(2); i3++ )
  for( int i2=teS.getBase(1); i2<=teS.getBound(1); i2++ )
  for( int i1=teS.getBase(0); i1<=teS.getBound(0); i1++ )
  {
    for( int s=0; s<numberOfSpecies_; s++ ) 
      hi(i1,i2,i3,s)=mp.hF(speciesNumber[s],teS(i1,i2,i3)*te0)/mw(s); 
  }
  return hi;
}



//\begin{>>ReactionsInclude.tex}{\subsection{cp}}
RealArray & Reactions::
cp(const RealArray & teS, const RealArray & cpi_ )
//====================================================================================
// /Description:
//    Compute cp for all species.
// /teS(I1,I2,I3) (input) : temperature, (scaled)
// /cpi\_(I1,I2,I3,S) (output) : cp (unscaled) in mass units, 
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & cpi = (RealArray&) cpi_;
  for( int i3=teS.getBase(2); i3<=teS.getBound(2); i3++ )
  for( int i2=teS.getBase(1); i2<=teS.getBound(1); i2++ )
  for( int i1=teS.getBase(0); i1<=teS.getBound(0); i1++ )
  {
    for( int s=0; s<numberOfSpecies_; s++ ) 
      cpi(i1,i2,i3,s)=mp.cp(speciesNumber[s],teS(i1,i2,i3)*te0)/mw(s); 
  }
  return cpi;
}


//\begin{>>ReactionsInclude.tex}{\subsection{pFromRTY}}
int Reactions::
pFromRTY(const RealArray & rhoS, const RealArray & teS, const RealArray & y, 
         RealArray & pS) const
// ====================================================================================
// /Description:
//   Compute p from the equation of state given $(\rho,T,Y_i)$
//
// /rhoS (input) : density (scaled)
// /teS (input) : temperature (scaled)
// /y (input) : species
// /pS (output) : pressure (scaled)
//\end{ReactionsInclude.tex}  
// ====================================================================================
{
  const Range I1=pS.dimension(0);
  const Range I2=pS.dimension(1);
  const Range I3=pS.dimension(2);

  RealArray r(I1,I2,I3);
  int s;
  r=0.;
  for( s=0; s<numberOfSpecies_; s++ )
    r+=y(I1,I2,I3,s)*(mp.R/molecularWeight(s));          // this is R:

  pS(I1,I2,I3)=(rho0*te0/p0)*rhoS(I1,I2,I3)*r(I1,I2,I3)*teS(I1,I2,I3);

  return 0;
}

//\begin{>>ReactionsInclude.tex}{\subsection{pFromRTY}}/
int Reactions::
rFromPTY(const RealArray & pS, const RealArray & teS, const RealArray & y, 
         RealArray & rhoS) const
// ====================================================================================
// /Description:
//   Compute $\rho$ from the equation of state given $(p,T,Y_i)$
//
// /te (input) : temperature
// /y (input) : species
// /p (input) : pressure
// /rho (output) : density 
//\end{ReactionsInclude.tex}  
// ====================================================================================
{
  const Range I1=rhoS.dimension(0);
  const Range I2=rhoS.dimension(1);
  const Range I3=rhoS.dimension(2);

  RealArray r(I1,I2,I3);
  r=0.;
  int s;
  for( s=0; s<numberOfSpecies_; s++ )
    r+=y(I1,I2,I3,s)*(mp.R/molecularWeight(s));          // this is R:

  rhoS(I1,I2,I3)=(p0/(te0*rho0))*pS(I1,I2,I3)/( r(I1,I2,I3)*teS(I1,I2,I3) );

  return 0;
}


//\begin{>>ReactionsInclude.tex}{\subsection{sigmaFromPTY}}
RealArray & Reactions::
sigmaFromPTY( const RealArray & pS,  const RealArray & teS, const RealArray & y, const RealArray & sigmai )
//====================================================================================
// /Description:
//   Compute $\sigma$ for all species from $(p,T,Y_i)$.
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( this!=NULL )
    throw "Reactions::error";
  return Overture::nullRealArray();
}


//\begin{>>ReactionsInclude.tex}{\subsection{sigmaFromPTY}}
RealArray & Reactions::
sigmaFromRPTY(const RealArray & rhoS,  
              const RealArray & pS,  
	      const RealArray & teS, 
	      const RealArray & y, 
	      const RealArray & sigmai )
//====================================================================================
// /Description:
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( this!=NULL )
  throw "Reactions::error";
  return Overture::nullRealArray();
}


// //begin{>>ReactionsInclude.tex}{\subsection{chemistrySource}}
// void Reactions::
// chemistrySource( const RealArray & p, const RealArray & te, const RealArray & y, 
// 		 const RealArray & rho, const RealArray & source )
// //====================================================================================
// // /Description:
// //end{ReactionsInclude.tex}  
// //====================================================================================
// {
//   throw "Reactions::chemistrySource:error";
// }



//\begin{>>ReactionsInclude.tex}{\subsection{viscosity}}
RealArray &  Reactions::
viscosity(const RealArray & teS, const RealArray & x, const RealArray & eta )
//====================================================================================
// /Description:
//   Compute the mixture viscosity, $\eta$. (note: pass mole-fractions)
// /teS (input) : temperature, (scaled)
// /x (input) : mole fractions
// /eta (output) : viscosity (unscaled) (MKS) Kg/(M s). To scale multiply by 1/(rho0*l0*u0).
// /Return value: eta
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( this!=NULL )
    throw "Reactions::viscosity:error";
  return Overture::nullRealArray();
}

//\begin{>>ReactionsInclude.tex}{\subsection{thermalConductivity}}
RealArray &  Reactions::
thermalConductivity(const RealArray & teS, const RealArray & x, const RealArray & lambda )
//====================================================================================
// /Description: 
//   Compute the mixture thermal conductivity, $\lambda$. NOTE pass mole fractions.
// /te (input) : temperature, (scaled)
// /x (input) : mole fractions
// /lambda (output) : thermal conductivity (unscaled) MKS N/(M K s). To scale multiply by (1/rho0*l0*u0*R0)
// /Return value: lambda
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( this!=NULL )
    throw "Reactions::thermalConductivity:error";
  return Overture::nullRealArray();
}


//\begin{>>ReactionsInclude.tex}{\subsection{diffusion}}
RealArray &  Reactions::
diffusion(const RealArray & p, const RealArray & teS, const RealArray & x, const RealArray & lambda )
//====================================================================================
// /Description:
//    Compute the mixture diffusion coefficients, $D_i$. (note: pass mole-fractions)
// /p (input) : pressure (scaled)
// /te (input) : temperature, (scaled)
// /x (input) : mole fractions
// /d (output) : mixture diffusion coefficients for all species, (unscaled) MKS, M*M/s. To non-dimensionalize
//   multiply by 1/(l0*u0)
// /Return value: d
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  if( this!=NULL )
    throw "Reactions::diffusion:error";
  return Overture::nullRealArray();
}


void Reactions::
computeEigenvaluesOfTheChemicalSourceJacobian(const RealArray & rhoS, 
                                              const RealArray & teS, 
					      const RealArray & y, 
					      const RealArray & reLambda_, 
					      const RealArray & imLambda_  )
//============================================================================================
// /Description: This routine will compute the eigenvalues of the chemical source jacobian
//   at an array of values
// /rhoS (input): scaled density
// /y (input) : mass fractions
// /reLambda (output) : real part of the eigenvalues
// /imLambda (output) : imaginary part of the eigenvalues
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & reLambda = (RealArray&) reLambda_;
  RealArray & imLambda = (RealArray&) imLambda_;

  RealArray yI(numberOfSpecies_), sigmaI(numberOfSpecies_), syI(numberOfSpecies_,numberOfSpecies_),
    reLambdaI(numberOfSpecies_),imLambdaI(numberOfSpecies_);


  int rhoBase3=rhoS.getBase(3);
  int teBase3=teS.getBase(3);
  int yBase3 = y.getBase(3);
  
  int i1s = reLambda.getBase(0)-rhoS.getBase(0);   // **** fix for A++ bug ++++
  int i2s = reLambda.getBase(1)-rhoS.getBase(1);
  int i3s = reLambda.getBase(2)-rhoS.getBase(2);

  int i1y = y.getBase(0)-rhoS.getBase(0);
  int i2y = y.getBase(1)-rhoS.getBase(1);
  int i3y = y.getBase(2)-rhoS.getBase(2);

  int s;
  for( int i3=rhoS.getBase(2); i3<=rhoS.getBound(2); i3++ )
  for( int i2=rhoS.getBase(1); i2<=rhoS.getBound(1); i2++ )
  for( int i1=rhoS.getBase(0); i1<=rhoS.getBound(0); i1++ )
  {
    for( s=0; s<numberOfSpecies_; s++ )
      yI(s)=y(i1+i1y,i2+i2y,i3+i3y,s+yBase3);

    chemicalSource(rhoS(i1,i2,i3,rhoBase3),teS(i1,i2,i3,teBase3),yI,sigmaI,syI);

    computeEigenvaluesOfTheChemicalSourceJacobian(rhoS(i1,i2,i3,rhoBase3),yI,sigmaI,syI,reLambdaI,imLambdaI);
    
    // sort eigenvalues by real part -- bubble sort ---
    for( int bubble=0; bubble<numberOfSpecies_; bubble++ )
    {
      for( s=0; s<numberOfSpecies_-1; s++ )
      {
	if( reLambdaI(s)>reLambdaI(s+1) )
	{
	  real temp=reLambdaI(s);
          reLambdaI(s)=reLambdaI(s+1);
	  reLambdaI(s+1)=temp;
	}
      }
    }
    for( s=0; s<numberOfSpecies_; s++ )
    {
      reLambda(i1+i1s,i2+i2s,i3+i3s,s)=reLambdaI(s);
      imLambda(i1+i1s,i2+i2s,i3+i3s,s)=imLambdaI(s);
    }
  }
}

void Reactions::
computeEigenvaluesOfTheChemicalSourceJacobian(const real rhoS, 
					      const RealArray & y, 
					      const RealArray & sigma, 
					      const RealArray & sy,
					      const RealArray & reLambda_, 
					      const RealArray & imLambda_ )
//============================================================================================
// /Description: This routine will compute the eigenvalues of the chemical source jacobian
// /rhoS (input): scaled density
// /y (input) : mass fractions
// /sigma (input) : source terms
// /sy (input) : (1/rho) d(sigma)/d(Y)
// /reLambda (output) : real part of the eigenvalues
// /imLambda (output) : imaginary part of the eigenvalues
//============================================================================================
//====================================================================================
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  RealArray & reLambda = (RealArray&) reLambda_;
  RealArray & imLambda = (RealArray&) imLambda_;

  real rho=rhoS*rho0;

  RealArray dy(numberOfSpecies_);
  intArray ipvt(numberOfSpecies_);
  RealArray work(numberOfSpecies_);
  real rcond;
  int job=0;

  // factor matrix
  RealArray fx;
  real dt=1.e-1;
  fx=0-sy;             // ****** fix when A++ is fixed *****
  fx(0,0)+=1./dt;
  fx(1,1)+=1./dt;
  fx(2,2)+=1./dt;
  fx(3,3)+=1./dt;
  fx(4,4)+=1./dt;
  

  GECO(fx(0,0), numberOfSpecies_, numberOfSpecies_, ipvt(0),rcond,work(0));
  if( debug & 4 )
    cout << "computeEigenvalues: condition number = " << rcond << endl;

  // solve sy*dy = sigma/rho
  dy=sigma/rho;
  GESL( fx(0,0), numberOfSpecies_, numberOfSpecies_, ipvt(0), dy(0), job);
  if( debug & 4 )
    dy.display("computeEigenvalues: here is dy = sy^{-1}*(sigma/rho)");

  if( debug & 4 )
    printf("computeEigenvalues: relative correction, dy/y(0)=%7.3e, dy/y(1)=%7.3e, dy/y(2)=%7.3e, "
             "dy/y(3)=%7.3e, dy/y(4)=%7.3e \n",
	     dy(0)/y(0), dy(1)/y(1),dy(2)/y(2),dy(3)/y(3),dy(4)/y(4));

  // compute eigenvalues and eigenvectors:
  int n=numberOfSpecies_;
  int lda=5, lwork=10*n, info;
  RealArray a(n,n), vl(n,n), vr(n,n), work2(lwork);
  a=sy;
  
  GEEV("V","V", n, a(0,0), lda, reLambda(reLambda.getBase(0)), imLambda(imLambda.getBase(0)), 
        vl(0,0), lda, vr(0,0), lda, work2(0),lwork,info,1,1 );

  if( debug & 4 )
  {
    cout << "computeEigenvalues: info = " << info << endl;
    reLambda.display("computeEigenvalues:Re(lambda)");
    imLambda.display("computeEigenvalues:Im(lambda)");
  }
  
  if( debug & 4 )
  {
    vr.display("computeEigenvalues:Here are the right eigenvectors");
    vl.display("computeEigenvalues:Here are the left  eigenvectors");
  }

}

void Reactions::
checkChemicalSourceJacobian()
//====================================================================================
//\end{ReactionsInclude.tex}  
//====================================================================================
{
  Range S(0,numberOfSpecies_-1);
  RealArray y(S);
  RealArray sigma(S),sy(S,S);
  RealArray sigmaPlus(S),sigmaMinus(S),dsdy(S,S);
  Range all;

  real rho=1.e-1;
  real yValue=.1;
  y=yValue;
  real deltaY=yValue*pow(REAL_EPSILON,1./3.);
  real te=500.;
  
  // compute sy:
  chemicalSource(rho,te,y,sigma,sy );

  // now check by differencing
//  for( int m=firstSpecies; m<=firstSpecies; m++ )
  for( int m=0; m<numberOfSpecies_; m++ )
  {
    // check entry sy(n,m)
    y(m)+=deltaY;
    chemicalSource(rho,te,y,sigmaPlus,Overture::nullRealArray() );
    y(m)-=2.*deltaY;
    chemicalSource(rho,te,y,sigmaMinus,Overture::nullRealArray() );
    y(m)=yValue; // reset
    dsdy(all,m)=(sigmaPlus-sigmaMinus)/(2.*deltaY*rho);
    // sigmaPlus.display("Here is sigmaPlus");
    // sigmaMinus.display("Here is sigmaMinus");
    
  }
  display(sy,"Here is (sigma/rho).y");
  display(dsdy,"Here is (sigma/rho).y by differences");
  real error = max(fabs(sy-dsdy));
  cout << "Maximum error = " << error << endl;

}


// BogusReaction::
// BogusReaction()
// {
//   rho0=p0=te0=1.;   // scaling values 

//   numberOfSpecies_=2;
//   speciesName = new aString[numberOfSpecies_];
//   speciesName[A]="A";
//   speciesName[B]="B";
//   speciesNumber = new MaterialProperties::material[numberOfSpecies_];
//   speciesNumber[A]=MP::H;
//   speciesNumber[B]=MP::H2;
// }

// BogusReaction::
// ~BogusReaction()
// {
//   delete speciesName;
//   delete speciesNumber;
// }



// void BogusReaction::
// reactionRates( const real & teS, const RealArray & kb, const RealArray & kf )
// // ----
// // /te (input): temperature
// // /kb (output): backward rate
// // /kf (output): forward rate
// //
// // Units m, mol, K
// {
// }


// void BogusReaction::
// chemicalSource(const RealArray & rhoS, const RealArray & teS, const RealArray & y, 
// 	       const RealArray & sigma, const RealArray & sy )
// // ======================================================================================================
// //  Evaluate the chemical source terms at an array of points
// // ======================================================================================================
// {
//   Reactions::chemicalSource(rhoS,teS,y,sigma,sy);
// }

// void BogusReaction::
// chemicalSource(const real & rhoS, const real & teS, const RealArray & y, 
//                const RealArray & sigma_, const RealArray & sy_ )
// // ===========================================================================================
// //  /Description: Compute the chemical source term, sigma, and/or the jacobian of sigma/rho
// //
// // /rho (input): density
// // /te (input): temperature
// // /y (input): mass fractions
// // /sigma (output): source term (if sigma!=nullArray)
// // /sy (output): d(sigma/rho)/dy (if sy!=nullArray)
// // ===========================================================================================
// {
//   RealArray & sigma = (RealArray&) sigma_;
//   RealArray & sy = (RealArray&) sy_;

//   real rho= rhoS*rho0;

//   const real epsA=1.e-1, epsB=2.e-1;
// /* ---
//   if( sigma.getLength(0)>0 )
//   {
//     sigma(A) = (.5-y(A))/epsA;
//     sigma(B) = (.5-y(B))/epsB;
//   }
//   if( sy.getLength(0)>0 )
//   {
//     // d(sigma/rho)/d(y) :
//     sy(A,A)=-1./(epsA*rho);
//     sy(A,B)=0.;
//     sy(B,A)=0.;
//     sy(B,B)=-1./(epsB*rho);
//   }
// ---- */  
//   if( sigma.getLength(0)>0 )
//   {
//     sigma(A) = (.75-y(A))/epsA;
//     sigma(B) = (.25-y(B))/epsB;
//   }
//   if( sy.getLength(0)>0 )
//   {
//     // d(sigma/rho)/d(y) :
//     sy(A,A)=-1./(epsA*rho);
//     sy(A,B)=0.;
//     sy(B,A)=0.;
//     sy(B,B)=-1./(epsB*rho);
//   }
// }


#undef MP
#undef GECO
#undef GESL
#undef GEEV
