#include "Overture.h"  
#include "PlotStuff.h"
#include "FilamentMapping.h"
#include "Mapping.h"
#include "MappingInformation.h"
#include "HDF_DataBase.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  initializeMappingList();

  printf(" --------------------------------------------------------------------------------- \n");
  printf(" Test a FilamentMapping {.h, .C} \n");
  printf("  * using the GUI\n");
  printf(" --------------------------------------------------------------------------------- \n");

  //FilamentMapping filam(80);
  FilamentMapping filam(80);

  realArray r, x, xr, x_t, x_tt, xr_t, xr_tt;
  real x0=0., y0=0., x0_t=0., y0_t=0., x0_tt=0.,  y0_tt=0.;
  int n = filam.nFilamentPoints;  real tcomp=0.;
  r.redim(n);
  x.redim(n,2);   x_t.redim(n,2);    x_tt.redim(n,2);
  xr.redim(n,2);  xr_t.redim(n,2);   xr_tt.redim(n,2);

  filam.formNormalizedParametrization( r ); // r=0, ..., 1.  =normalized param
  //filam.formChebyshevParametrization( r );

  for(int qq=0; qq<n; ++qq) printf("  %f  ", r(qq)); printf("\n");
  filam.computeTravelingWaveFilament( tcomp, r, x, xr);
  filam.computeTravelingWaveFilamentTimeDerivatives( tcomp, r, x, xr,
					       x_t, x_tt, xr_t, xr_tt,
					       x0, y0, x0_t, y0_t, x0_tt, y0_tt);


  // Make interactive changes to the mapping
  PlotStuff ps(TRUE,"testFilamentMapping");      // create a PlotStuff object
  MappingInformation mapInfo;              // parameters used by map.update
  mapInfo.graphXInterface=&ps;             // pass graphics interface
  filam.update(mapInfo);

  return 0;
}

