#include "DomainSolver.h"
#include "HDF_DataBase.h"
#include "GenericGraphicsInterface.h"

int DomainSolver::
saveRestartFile(const GridFunction & cgf, const aString & restartFileName )
// ========================================================================
//    Save a restart file
// ========================================================================
{
  
  if( debug() & 1 )
    cout << "Save a restart file, t= " << cgf.t << endl;
    
  HDF_DataBase db;
  db.mount(restartFileName,"I");
  parameters.put(db,"parameters");
  cgf.put(db,"cgf");
  db.unmount();
  return 0;
}


int 
readRestartFile(GridFunction & cgf, Parameters & parameters,
                const aString & restartFileName =nullString )
// ========================================================================
//    Read a restart file
// ========================================================================
{
  HDF_DataBase db;
  if( restartFileName!=nullString && restartFileName!="" )
  {
    cout << "try to mount restart file = " << restartFileName << endl;
    db.mount(restartFileName,"R");  // mount read only
  }
  else
  {
    aString fileName="ob1.restart";
    cout << "try to mount restart file = " << fileName << endl;
    db.mount(fileName,"R");
  }
  parameters.get(db,"parameters");

  cgf.get(db,"cgf");
  db.unmount();

  cout << "Read a restart file, t= " << cgf.t << endl;
  return 0;
}


int DomainSolver::
readRestartFile(GridFunction & cgf, const aString & restartFileName /* =nullString */ )
// ========================================================================
//    Read a restart file
// ========================================================================
{
  
    
  HDF_DataBase db;
  if( restartFileName!=nullString && restartFileName!="" )
  {
    cout << "try to mount restart file = " << restartFileName << endl;
    db.mount(restartFileName,"R");  // mount read only
  }
  else
  {
    aString fileName="ob1.restart";
    cout << "try to mount restart file = " << fileName << endl;
    db.mount(fileName,"R");
  }
  parameters.get(db,"parameters");

  cgf.get(db,"cgf");
  db.unmount();

  if( debug() & 1 )
    cout << "Read a restart file, t= " << cgf.t << endl;
  return 0;
}



int DomainSolver::
readRestartFile(realCompositeGridFunction & v, 
                real & t,
                const aString & restartFileName /* =nullString */ )
// ========================================================================
//    Read a restart file
// ========================================================================
{
  
    
  HDF_DataBase db;
  if( restartFileName!=nullString && restartFileName!="" )
  {
    cout << "try to mount restart file = " << restartFileName << endl;
    db.mount(restartFileName,"R");  // mount read only
  }
  else
  {
    aString fileName="ob1.restart";
    cout << "try to mount restart file = " << fileName << endl;
    db.mount(fileName,"R");
  }
  parameters.get(db,"parameters");

  GridFunction gfr;
  // gfr.u=v;  // *no* *** note **** we need to dimension gfr.u before reading from a file.

  gfr.get(db,"cgf");
  db.unmount();

  v.updateToMatchGrid(gfr.cg);
  cg=gfr.cg;
  if( cg->interpolant!=NULL )
    cg->interpolant->updateToMatchGrid(cg);
  
  v=gfr.u;
  t=gfr.t;
  
  // ***** this does not work yet for AMR *****

//   GraphicsParameters params;
//   params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
//   PlotStuff & gi = *Overture::getGraphicsInterface();
//   gi.contour(u,params);
//   params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  cout << "Read a restart file, t= " << t << endl;
  return 0;
}
