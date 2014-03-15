#ifndef MATERIAL_PROPERTIES_H
#define MATERIAL_PROPERTIES_H

#include "A++.h"

// include "realPrecision.h"

class MaterialProperties
{
public:
  enum material
  {
    O2=0,
    O,
    N2,
    CO,
    CO2,
    A,
    H,
    H2,
    H2O,
    CH4,
    C2H4,
    F,
    F2,
    HF,
    numberOfMaterials  // counts items in this enum
    };

  enum Units
  {
    ee=0,
    msi,
    si
    };
  
 
  // NOTE: only si (MKS) units currently supported 
  MaterialProperties( const Units unitsToUse=si );
  ~MaterialProperties();
  
  inline double cp( const material & m, const double t ) const;
  inline double h( const material & m, const double t ) const;
  inline double hMinusRef( const material & m, const double t ) const;
  inline double hF( const material & m, const double t ) const;
  inline double s( const material & m, const double t ) const;
  inline double lnKp( const material & m, const double t ) const;
  inline double logKp( const material & m, const double t ) const;
  inline double Kp( const material & m, const double t ) const;
  inline double mw (const material & m) const;

private:
  
  double coeff[numberOfMaterials][2][5];   // materials, 2 temperature ranges, 5 coefficients for cp
  double h0[numberOfMaterials][2];         // h0
  double hRef[numberOfMaterials];          // h(tRef)
  double deltaH[numberOfMaterials];        // deltaH formation
  double sRef[numberOfMaterials];          // phi(tRef)
  double phi0[numberOfMaterials][2];

  double lCoeff[numberOfMaterials][2][6]; // coeff's for lnKp
  
  double t0;               // coefficients change at this value

public:
  double R;
  double tRef;
  double gramsPerKilogram;
  double newtonMeterSquaredPerAtmosphere;

  Units units;               // 0=EE, 1=modified SI using calories, 2=SI using Joules

};


double MaterialProperties::
cp( const material & m, const double t ) const
  // return  \bar{c}_p^0  : mole based, units of R, J/(mole-K) if si (MKS)
  // t : temperature in K
{ 
  const double *c = t<t0 ? &(coeff[m][0][0]) : &(coeff[m][1][0]);
  return (c[0]+t*(c[1]+t*(c[2]+t*(c[3]+t*c[4]))))*R;
}

double  MaterialProperties::
h( const material & m, const double t ) const
  // return  \bar{H}^0 : mole based, units of RT, J/(mole) if si (MKS)
  // t : temperature in K
{ 
  int mt = t<t0 ? 0 : 1; 
  const double *c = &(coeff[m][mt][0]);
  return (h0[m][mt] + t*(c[0]+t*(.5*c[1]+t*(c[2]/3.+t*(c[3]*.25+t*c[4]*.2)))))*R;
}

double  MaterialProperties::
hMinusRef( const material & m, const double t ) const
  // return  \bar{H}^0-hRef
  // t : temperature in K
{ 
  return h(m,t)-hRef[m];
}

double  MaterialProperties::
hF( const material & m, const double t ) const
  // return  \bar{H}^0-hRef + DeltaH_f,ref
  // t : temperature in K
{ 
  return h(m,t)-hRef[m]+deltaH[m];
}

double  MaterialProperties::
s( const material & m, const double t ) const
  // return  \bar{s}^0
  // t : temperature in K
{ 
  int mt = t<t0 ? 0 : 1; 
  const double *c = &(coeff[m][mt][0]);
  return (phi0[m][mt] + log(t)*c[0]+t*(c[1]+t*(.5*c[2]+t*(c[3]/3.+t*c[4]*.25))))*R;
}

double  MaterialProperties::
lnKp( const material & m, const double t ) const
  // return  ln(K_p)
  // t : temperature in K
  //    ln(K_p) = -(1/RT) sum_i (\nu_i^''-\nu_i^')( h_i-T*s_i )
{ 
  int mt;
  switch( m )
  {
  case H2O:
    // H2O : (1)*H2O + (-1)H2 - (.5)O2 = 0
    return ( (t*s(H2O,t)-h(H2O,t)) - (t*s(H2,t)-h(H2,t)) - .5*(t*s(O2,t)-h(O2,t)) )/(R*t);
  case H2:
  case O2:
  case F2:
    return 0.;
  case F:
  case HF:
  case H:
  {
    mt = t<t0 ? 0 : 1; 
    const double *c = &(lCoeff[m][mt][0]);
    return (c[0]+t*(c[1]+t*(c[2]+t*(c[3]+t*(c[4]+t*c[5])))))*R/t;
  }
  }
  // default:
  return (t*s(m,t)-h(m,t))/(R*t);
}

double  MaterialProperties::
logKp( const material & m, const double t ) const
  // return  log_10(K_p)
  // t : temperature in K
  //    ln(K_p) = -(1/RT) sum_i (\nu_i^''-\nu_i^')( h_i-T*s_i )
{ 
  return lnKp(m,t)*log10(exp(1.));
}
  
double  MaterialProperties::
Kp( const material & m, const double t ) const
  // Return Kp
  // **** Note that these Kp are based on reference state of pressure of 1 atmosphere
  //  Thus for some Kp you may want to comnvert from atmospheres to N/m^2
{
  return exp(lnKp(m,t));
}
  
double  MaterialProperties::
mw (const material & m) const
  // molecular weight (kg/mole)
{
  switch( m )
  {
  case H2O:
    return .018016; // 18.016/gramsPerKilogram;
  case  H2:
    return .002016; //  2.016/gramsPerKilogram;
  case O2:
    return .032;    // 32.0/gramsPerKilogram;
  case F2:
    return .038;    // 38.0/gramsPerKilogram;
  case F:
    return .019;    // 19.0/gramsPerKilogram;
  case HF:
    return .020008;  // 20.008/gramsPerKilogram;
  case H:
    return .001008;  //  1.008/gramsPerKilogram;
  default:
    printf("mw:ERROR: unknown material\n");
    throw "error";
  }
}



#endif
