#ifndef DETERMINE_ERRORS_H
#define DETERMINE_ERRORS_H

#include "DetermineErrors.h"


real
determineError(realCompositeGridFunction & u, 
               realCompositeGridFunction & error )
{
  


  error=0.;
  for( grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    getIndex(cg[grid].indexRange(),I1,I2,I3);  
    where( cg[grid].mask()(I1,I2,I3)!=0 )
      error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
    if( Oges::debug & 32 ) 
    {
      getIndex(cg[grid].dimension(),I1,I2,I3);  
      u.display("Computed solution");
      exact(cg[grid],I1,I2,I3,0).display("exact solution");
      abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
    }
  }
  printf("Maximum error with neumann bc's= %e\n",error);  
