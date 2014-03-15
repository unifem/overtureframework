#include "MaterialProperties.h"
#include "wdhdefs.h"

MaterialProperties::
MaterialProperties( const Units unitsToUse /* =si */ )
  // NOTE: only si (MKS) units currently supported 
{
  units=unitsToUse;
  gramsPerKilogram=1000.;
  newtonMeterSquaredPerAtmosphere=1.01325e5;
    
  tRef=298.15;
    
  if( units==ee )
  {
    R=1.98586;
  }
  else if( units==msi )
  {
    R=8.31434;
  }
  else  // si
  {
    R=8.31434;  // J/(mol-K)
  }
    
  double Rsi=8.31434;
  t0=1000.;
    
  for( int m=0; m<numberOfMaterials; m++ )
  {
    h0[m][0]=h0[m][1]=0.;
    hRef[m]=0.;
    deltaH[m]=0.;
    sRef[m]=0.;
    phi0[m][0]=phi0[m][1]=0.;

    for( int i=0; i<5; i++ )
    {
      coeff[m][0][i]=coeff[m][1][i]=0.;
      lCoeff[m][0][i]=lCoeff[m][1][i]=0.;
    }
  
  }

  // ****** O2 t<=t0 ******* from Zucrow and Hoffman, Table 1.6 p56 V1
  coeff[O2][0][0]=3.62560;     coeff[O2][0][1]=-1.87822e-3; coeff[O2][0][2]=7.05545e-6; 
  coeff[O2][0][3]=-6.76351e-9; coeff[O2][0][4]=2.15560e-12;
  // O2 t>=t0
  coeff[O2][1][0]=3.62195;     coeff[O2][1][1]=.736183e-3; coeff[O2][1][2]=-.196522e-6; 
  coeff[O2][1][3]=.0362016e-9; coeff[O2][1][4]=-.00289456e-12;

  // ****** fix h0 *************
  h0[O2][0]  =-1.04752e+3;   h0[O2][1]  =-1.20198e+3;
  phi0[O2][0]=4.30528;       phi0[O2][1]=3.61510;

  deltaH[O2]=0.;

  // ****** CO t<=t0 ******* from Zucrow and Hoffman, Table 1.6 p56 V1
  coeff[CO][0][0]=3.71009;     coeff[CO][0][1]=-1.61910e-3; coeff[CO][0][2]=3.69236e-6; 
  coeff[CO][0][3]=-2.03197e-9; coeff[CO][0][4]=.239533e-12;
  // CO t>=t0
  coeff[CO][1][0]=2.98407;    coeff[CO][1][1]=1.48914e-3; coeff[CO][1][2]=-.578997e-6; 
  coeff[CO][1][3]=.103646e-9; coeff[CO][1][4]=-.00693536e-12;

  // ****** fix h0 *************
  h0[CO][0]  =-14.3563e+3;   h0[CO][1]  =-14.2452e+3;
  phi0[CO][0]=2.95554;       phi0[CO][1]=6.34792;
  deltaH[CO]=-110529.;

  // ****** H2 t<=t0 ******* from Zucrow and Hoffman, Table 1.6 p56 V1
  coeff[H2][0][0]=3.05745;     coeff[H2][0][1]=2.67652e-3; coeff[H2][0][2]=-5.80992e-6; 
  coeff[H2][0][3]=5.52104e-9; coeff[H2][0][4]=-1.81227e-12;
  //    t>=t0
  coeff[H2][1][0]=3.10019;    coeff[H2][1][1]=0.511195e-3; coeff[H2][1][2]=0.0526442e-6; 
  coeff[H2][1][3]=-.0349100e-9; coeff[H2][1][4]=.00369453e-12;

  // h0(298.15) = 2024 cal/GFW * 4.1840 J/cal = 8468.4 J/mole from Stull and Sinke
  h0[H2][0]  =-0.988905e+3+8468.4/Rsi;   h0[H2][1]  =-.877380e+3+8468.4/Rsi;
  phi0[H2][0]=-2.29971;       phi0[H2][1]=-1.96294;

  deltaH[H2]=0.;

  // ****** H2O t<=t0 *******
  coeff[H2O][0][0]=4.07013;     coeff[H2O][0][1]=-1.10845e-3; coeff[H2O][0][2]=4.15212e-6; 
  coeff[H2O][0][3]=-2.96374e-9; coeff[H2O][0][4]=.807021e-12;
  //    t>=t0
  coeff[H2O][1][0]=2.71676;    coeff[H2O][1][1]=2.94514e-3; coeff[H2O][1][2]=-.802243e-6; 
  coeff[H2O][1][3]=0.102267e-9; coeff[H2O][1][4]=-.00484721e-12;

  // ****** fix h0 *************
  h0[H2O][0]  =-30.2797e+3;   h0[H2O][1]  =-29.9058e+3;
  phi0[H2O][0]=-.322700;       phi0[H2O][1]=6.63057;

  deltaH[H2O]=-241827;

  // ****** F t<=t0 ******* (fit to table with interpolate([100,300,500,700,1000],[3.291,...],x)/8.31434
  coeff[F][0][0]=2.8131217;    coeff[F][0][1]=-.0062757322e-3; coeff[F][0][2]=-1.279587865e-6; 
  coeff[F][0][3]=1.6705217e-9; coeff[F][0][4]=-.640030185e-12;
  //    t>=t0
  coeff[F][1][0]=2.70219885;    coeff[F][1][1]=-.22602516e-3; coeff[F][1][2]=.9968219567e-7; 
  coeff[F][1][3]=-.19514477e-10; coeff[F][1][4]=.140820959e-14;

  // h0(298.15) = 1558 cal/GFW * 4.1840 J/cal = 6518.67 J/mole from Stull and Sinke
  h0[F][0]  =-830.1866879+6518.67/Rsi;   h0[F][1]  =-774.8562195+6518.67/Rsi;
  phi0[F][0]=3.09747244;     phi0[F][1]=3.796870527;

  lCoeff[F][0][0]=-1114.310569;    lCoeff[F][0][1]=.6078885703;       lCoeff[F][0][2]=.0006047996727;  
  lCoeff[F][0][3]=-.6777852848e-6; lCoeff[F][0][4]=.3995457072e-9;   lCoeff[F][0][5]=-.9375621588e-13; 
  //    t>=t0
  lCoeff[F][1][0]=-1160.898853;    lCoeff[F][1][1]=.8555847889;   lCoeff[F][1][2]=.00004136153018; 
  lCoeff[F][1][3]=-.112095335e-7; lCoeff[F][1][4]=.1643515427e-11; lCoeff[F][1][5]=-.9956703288e-16;

  deltaH[F]=78910;

  // ****** F2 t<=t0 ******* (fit to table with interpolate([100,300,500,700,1000],[3.291,...],x)/8.31434
  coeff[F2][0][0]=2.844710331;  coeff[F2][0][1]=.004012511665; coeff[F2][0][2]=-.3214275182e-5; 
  coeff[F2][0][3]=.4716782235e-9; coeff[F2][0][4]=.3566859745e-12;
  //    t>=t0
  coeff[F2][1][0]=4.039767438;   coeff[F2][1][1]=.000608797572; coeff[F2][1][2]=-.2150451309e-6;
  coeff[F2][1][3]=.4062258701e-10;  coeff[F2][1][4]=-.2831453449e-14;

  // h0(298.15) = 2110 cal/GFW * 4.1840 J/cal = 8828.24 J/mole from Stull and Sinke
  h0[F2][0]  =-999.1402327+8828.24/Rsi;   h0[F2][1]  =-1312.385356+8828.24/Rsi;
  phi0[F2][0]=7.112786980;    phi0[F2][1]=.9952500018;

  deltaH[F2]=0.;



  // ****** HF t<=t0 ******* (fit to table with interpolate([100,300,500,700,1000],[3.291,...],x)/8.31434
  coeff[HF][0][0]=3.4659947;    coeff[HF][0][1]=.000332459408; coeff[HF][0][2]=-.1005307965e-5; 
  coeff[HF][0][3]=.1204578810e-8; coeff[HF][0][4]=-.3691747564e-12;
  //    t>=t0
  coeff[HF][1][0]=3.002282803;   coeff[HF][1][1]=.000694693345; coeff[HF][1][2]=-.5580218436e-7;
  coeff[HF][1][3]=-.1484383205e-10; coeff[HF][1][4]=.222005996e-14;

  // ****** fix h0 *************
  // h0(298.15) = ???? cal/GFW * 4.1840 J/cal = 8659. ??????? extrapolation to T=0e
  h0[HF][0]  =-1041.456667+8659./Rsi;  h0[HF][1]  =-844.8226871+8659./Rsi;
  phi0[HF][0]=1.075709281;    phi0[HF][1]=3.755463392; 

  lCoeff[HF][0][0]=3943.308244;     lCoeff[HF][0][1]=.0920671983;       lCoeff[HF][0][2]=.00004973328605;
  lCoeff[HF][0][3]=-.9189286906e-7; lCoeff[HF][0][4]=.2839052473e-10;  lCoeff[HF][0][5]=.5952628614e-14;  
  //    t>=t0
  lCoeff[HF][1][0]=3946.7712;     lCoeff[HF][1][1]=.11481002;     lCoeff[HF][1][2]=-.000042073665;   
  lCoeff[HF][1][3]=.9106097509e-8; lCoeff[HF][1][4]=-.1109414125e-11; lCoeff[HF][1][5]=.5472889885e-16; 

  deltaH[HF]=-272456.;

  // ****** H t<=t0 ******* (fit to table with interpolate([100,300,500,700,1000],[3.291,...],x)/8.31434
  coeff[H][0][0]=2.500018;     coeff[H][0][1]=0;             coeff[H][0][2]=0.; 
  coeff[H][0][3]=0.;             coeff[H][0][4]=0.;
  //    t>=t0
  coeff[H][1][0]=2.500018;      coeff[H][1][1]=0.;            coeff[H][1][2]=0.;
  coeff[H][1][3]=0.;               coeff[H][1][4]=0.;

  // h0(298.15) = 1481 cal/GFW * 4.1840 J/cal = 6196.5 J/mole from Stull and Sinke
  h0[H][0]  =-745.4109+6196.5/Rsi;      h0[H][1]  =-745.4109+6196.5/Rsi;
  phi0[H][0]=-.4601769341;   phi0[H][1]=-.4602411256;

  lCoeff[H][0][0]=-3131.48044;     lCoeff[H][0][1]=.539452206;        lCoeff[H][0][2]=.00046225318;    
  lCoeff[H][0][3]=-.5045114309e-6; lCoeff[H][0][4]=.3305163924e-9;   lCoeff[H][0][5]=-.9066534451e-13; 
  //    t>=t0
  lCoeff[H][1][0]=-3157.6069;    lCoeff[H][1][1]=.686790974;    lCoeff[H][1][2]=.0000995934083;   
  lCoeff[H][1][3]=-.2685012968e-7; lCoeff[H][1][4]=.3863992141e-11;  lCoeff[H][1][5]=-.2268282073e-15;

  deltaH[H]=217986.; 



  for( int i=0; i<numberOfMaterials; i++ )
  {
    material m;
    m=(material)i;
    printF(" material =%i\n",m);
    
    hRef[i]=h((material)i,tRef);

    sRef[i]=s((material)i,tRef);
  }

}

MaterialProperties::
~MaterialProperties()
{
}
  
