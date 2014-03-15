#include "Mapping.h"

#define SURGRD EXTERN_C_NAME(surgrd)

int 
hyper(
      int & IFORM, int & IZSTRT, int & NZREG,
      int & NPZREG, real & ZREG,  real & DZ0, real &  DZ1,
      int & IBCJA,int & IBCJB, int & IBCKA,int & IBCKB,
      int & IVSPEC, real & EPSSS, int & ITSVOL,
      int & IMETH, real & SMU2,
      real & TIMJ, real & TIMK,
      int & IAXIS, real & EXAXIS, real & VOLRES,
      int & JMAX, int & KMAX,
      int & JDIM,int & KDIM,int & LMAX,
      real & X, real & Y, real & Z,
      realArray & XW, realArray & YW, realArray & ZW )
{
  cout << "Sorry -- hypgen not available, talk to Bill Henshaw \n";
  if( &IFORM )
    {throw "error";}
  return 1;
}


extern "C"
{
void SURGRD( 
  int & INIC, int & IJMAX, int & IJCMAX, int & IIRFAM, 
  int & IJRAXSA,int & IJRAXSB,int & IJRPER,int & IKRPER,
  int & IIBCJA, int & IIBCJB, int & IIAFAM,
  int & INGBCA, int & INGBCB, int & IKMAX, int & INNOD,
  int & IJNOD, real & ETAMX, real & DETA,
  real & DFAR, real & SMU, real & TIM, int & IITSVOL,
  int & jrmax, int & krmax,
  real & xsurf,
  int & ndra,int & ndrb,int & ndsa,int & ndsb,int & ndta,int & ndtb,
  int & nra, int & nrb, int & nsa, int & nsb, int & nta, int & ntb,
  real & xcurve,
  int & mdra,int & mdrb,int & mdsa,int & mdsb,int & mdta,int & mdtb,
  int & mra, int & mrb, int & msa, int & msb, int & mta, int & mtb,
  real & xhype,
  int & ldra,int & ldrb,int & ldsa,int & ldsb,int & ldta,int & ldtb,
  int & lra, int & lrb, int & lsa, int & lsb, int & lta, int & ltb)
{
  cout << "Sorry -- SURGRD not available, talk to Bill Henshaw \n";
}
}

