#include "Cgcns.h"

void Cgcns::
addForcing(MappedGrid& mg, realMappedGridFunction & dvdt, int iparam[], real rparam[],
		realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction())
{
}


// int Cgcns:: 
// applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
// 			realMappedGridFunction & gridVelocity,
// 			const int & grid,
// 			const int & option=-1,
// 			realMappedGridFunction *puOld=NULL, 
// 			realMappedGridFunction *pGridVelocityOld=NULL,
// 			const real & dt=-1.)
// {
//   return 0;
// }

int Cgcns::
getUt(const realMappedGridFunction & v, 
	  const realMappedGridFunction & gridVelocity, 
	  realMappedGridFunction & dvdt, 
	  int iparam[], real rparam[],
	  realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
	  MappedGrid *pmg2=NULL,
      const realMappedGridFunction *pGridVelocity2= NULL)
{
  return 0;
}



void Cgcns::
getTimeSteppingEigenvalue(MappedGrid & mg, 
 			       realMappedGridFunction & u, 
 			       realMappedGridFunction & gridVelocity,  
 			       real & reLambda,
 			       real & imLambda, 
			  const int & grid)
{
}
