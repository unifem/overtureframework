#include "AdamsPCData.h"

AdamsPCData::
AdamsPCData()
// =========================================================================
// Ths class holds state data for the adams predictor-corrector methods
// =========================================================================
{ 
  dtb=0.;
  mab0=0; 
  mab1=1; 
  mab2=2; 
  nab0=0; 
  nab1=1; 
  nab2=2; 
  nab3=3; 
  ndt0=0; 
  for( int i=0; i<5; i++ )
    dtp[i]=0.;
} 

AdamsPCData::
~AdamsPCData()
{}


AdamsPCData::
AdamsPCData(const AdamsPCData & x)
// Copy constructor
{
  *this = x;
}

AdamsPCData& AdamsPCData:: 
operator=(const AdamsPCData & x)
{
  dtb=x.dtb;
  mab0=x.mab0;
  mab1=x.mab1;
  mab2=x.mab2;
  nab0=x.nab0;
  nab1=x.nab1;
  nab2=x.nab2;
  nab3=x.nab3;
  ndt0=x.ndt0;
  for( int i=0; i<5; i++ )
    dtp[i]=x.dtp[i];
  
  return *this;
}


