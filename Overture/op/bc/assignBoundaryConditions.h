#ifndef ASSIGNBOUNDARYCONDITIONS_H
#define ASSIGNBOUNDARYCONDITIONS_H

#define assignBoundaryConditions EXTERN_C_NAME(assignboundaryconditions)

extern "C"
{
  void assignBoundaryConditions( const int& nd, 
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&ndu1a,const int&ndu1b,const int&ndu2a,const int&ndu2b,const int&ndu3a,const int&ndu3b,
      const int&ndu4a,const int&ndu4b,
      const int&ndv1a,const int&ndv1b,const int&ndv2a,const int&ndv2b,const int&ndv3a,const int&ndv3b,
      const int&ndv4a,const int&ndv4b,
      const int&ndc1a,const int&ndc1b,const int&ndc2a,const int&ndc2b,const int&ndc3a,const int&ndc3b,
      const int&ndc4a,const int&ndc4b,
      const int&ndg1a,const int&ndg1b,const int&ndg2a,const int&ndg2b,const int&ndg3a,const int&ndg3b,
      const int&ndg4a,const int&ndg4b,
      const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
      const int&ndf4a,const int&ndf4b,
      const int&ndw1a,const int&ndw1b,
      const int&ndm1a,const int&ndm1b,const int&ndm2a,const int&ndm2b,const int&ndm3a,const int&ndm3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const real & rsxy,
      real &u, real &v, const real &coeff, const int&mask,
      const real &scalarData,const real &gfData,const real &fData,const real &vData, 
      const real &dx,const real &dr,const int &ipar, const real &par, 
      const int&ca,const int&cb, const int&uCBase, const int&uC, const int&fCBase, const int&fC,
      const int&side,const int&axis,const int&grid, const int&bcType,const int&bcOption,
      const int&gridType,const int&order,const int&useWhereMask, const int & lineForForcing );
}

#endif

