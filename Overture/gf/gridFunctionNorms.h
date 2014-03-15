#ifndef GRID_FUNCTION_NORMS_H


real 
l2Norm(const realMappedGridFunction & u, const int cc=0, int maskOption=0, int extra=0 );
real 
l2Norm(const realCompositeGridFunction & u, const int cc=0, int maskOption=0, int extra=0 );


real 
maxNorm(const realMappedGridFunction & u, const int cc=0, int maskOption=0, int extra=0 );
real 
maxNorm(const realCompositeGridFunction & u, const int cc=0, int maskOption=0, int extra=0 );

real 
lpNorm(const int p, const realCompositeGridFunction & u, const int cc=0, int maskOption=0, int extra=0, 
       int normOption=0 );

namespace GridFunctionNorms
{

// Compute the min and max values of components of a grid function
int getBounds(const realCompositeGridFunction & u, RealArray & uMin, RealArray & uMax, const Range & C=nullRange  );
int getBounds(const realMappedGridFunction & u, RealArray & uMin, RealArray & uMax, const Range & C=nullRange  );

};


#endif
