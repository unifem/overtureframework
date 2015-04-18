// ========================================================================================================
/// \class Cgad
/// \brief Solver for advection-diffusion (AD) equations.
// ========================================================================================================

#include "Cgad.h"
#include "AdParameters.h"
#include "Ogshow.h"

Cgad::
Cgad(CompositeGrid & cg_, 
      GenericGraphicsInterface *ps /* =NULL */, 
      Ogshow *show /* =NULL */ , 
      const int & plotOption_ /* =1 */) 
   : DomainSolver(*(new AdParameters),cg_,ps,show,plotOption_)
// ===================================================================================================
// Notes:
//   AdParameters (passed to the DomainSolver constructor above) replaces the base class Parameters
// ===================================================================================================
{
  className="Cgad";
  name="ad";

  // should this be somewhere else? setup?
  if( realPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    realPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  if( imaginaryPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    imaginaryPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
}



Cgad::
~Cgad()
{
  delete & parameters;
}




int Cgad::
updateToMatchGrid(CompositeGrid & cg)
{
  printF("\n $$$$$$$$$$$$$$$ Cgad: updateToMatchGrid(CompositeGrid & cg) $$$$$$$$$$$$\n\n");
  
  int returnValue =DomainSolver::updateToMatchGrid(cg);

  if( realPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    realPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  if( imaginaryPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    imaginaryPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);

  return returnValue;
  
}

// int Cgad::
// formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
// 			       const real & dt0, 
// 			       int scalarSystem, 
// 			       realMappedGridFunction & uL,
// 			       const int & grid )
// {
//   Overture::abort("Cgad::formImplicitTimeSteppingMatrix:ERROR: not implemented");
//   return 0;
// }


int Cgad::
updateGeometryArrays(GridFunction & cgf)
{
  if( debug() & 4 ) printF(" --- Cgad::updateGeometryArrays ---\n");

  real cpu0=getCPU();

  int grid;
  for( grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
  {
    if( !cgf.cg[grid].isRectangular() || twilightZoneFlow() ||	parameters.gridIsMoving(grid) )
      cgf.cg[grid].update(MappedGrid::THEcenter | MappedGrid::THEvertex );  
  }
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdatePressureEquation"))+=getCPU()-cpu0;

  if( realPartOfEigenvalue.size() != cgf.cg.numberOfComponentGrids() )
    realPartOfEigenvalue.resize(cgf.cg.numberOfComponentGrids(),-1.);
  
  if( imaginaryPartOfEigenvalue.size() != cgf.cg.numberOfComponentGrids() )
    imaginaryPartOfEigenvalue.resize(cgf.cg.numberOfComponentGrids(),-1.);

  return DomainSolver::updateGeometryArrays(cgf);
}


void Cgad::
saveShowFileComments( Ogshow &show )
{
    // save comments that go at the top of each plot
  char buffer[80]; 
  // save comments that go at the top of each plot
  aString timeLine="";
  if(  parameters.dbase.has_key("timeLine") )
    timeLine=parameters.dbase.get<aString>("timeLine");

  std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = parameters.dbase.get<std::vector<real> >("a");
  std::vector<real> & b = parameters.dbase.get<std::vector<real> >("b");
  std::vector<real> & c = parameters.dbase.get<std::vector<real> >("c");   
  
  aString showFileTitle[5];
  if( parameters.dbase.get<int>("numberOfDimensions")==2 )
    showFileTitle[0]=sPrintF(buffer,"Convection Diffusion, a=%g, b=%g, kappa=%g",a[0],b[0],kappa[0]);
  else
    showFileTitle[0]=sPrintF(buffer,"Convection Diffusion, a=%g, b=%g, c=%g, kappa=%g",
                             a[0],b[0],c[0],kappa[0]);
  showFileTitle[1]=timeLine;
  showFileTitle[2]="";  // marks end of titles
  
  for( int i=0; showFileTitle[i]!=""; i++ )
    show.saveComment(i,showFileTitle[i]);
}


// ===================================================================================================================
/// \brief Output run-time parameters for the header.
/// \param file (input) : write values to this file.
///
// ===================================================================================================================
void 
Cgad::
writeParameterSummary( FILE * file )
{
  DomainSolver::writeParameterSummary( file );

  const int & numberOfComponents = parameters.dbase.get<int>("numberOfComponents");
  std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");
  std::vector<real> & a = parameters.dbase.get<std::vector<real> >("a");
  std::vector<real> & b = parameters.dbase.get<std::vector<real> >("b");
  std::vector<real> & c = parameters.dbase.get<std::vector<real> >("c");

  if ( file==parameters.dbase.get<FILE* >("checkFile") )
  {
    fPrintF(file,"\\caption{advection-diffusion, gridName, $\\kappa=%3.2g$, $t=%2.1f$, ",
 	    kappa[0],parameters.dbase.get<real >("tFinal"));

     return;
  }


  real & thermalConductivity = parameters.dbase.get<real>("thermalConductivity");

  if( parameters.dbase.get<bool >("variableDiffusivity") )
  {
    fPrintF(file," The coefficients of diffusivity are variable.\n");
  }
  else
  {
    fPrintF(file," The coefficients of diffusivity are constant:\n  ");
    aString name = "kappa";
    for( int m=0; m<numberOfComponents; m++ )
    {
      if( numberOfComponents==1 )
	fPrintF(file," %s=%g",(const char*)name,kappa[m]);
      else
	fPrintF(file," %s[%i]=%g,",(const char*)name,m,kappa[m]);
    }
    fPrintF(file,"\n");

    
  }
  if( parameters.dbase.get<bool >("variableAdvection") )
  {
    fPrintF(file," The advection coefficients are variable.\n");
  }
  else
  {
    fPrintF(file," The advection coefficients are constant:\n");
    for( int n=1; n<4; n++ )
    {
      std::vector<real> & par = n==1 ? a : n==2 ? b : c;
      aString name = n==1 ? "a" : n==2 ? "b" : "c";
      for( int m=0; m<numberOfComponents; m++ )
      {
	if( numberOfComponents==1 )
	  fPrintF(file,"   %s=%g",(const char*)name,par[m]);
	else
	  fPrintF(file,"   %s[%i]=%g,",(const char*)name,m,par[m]);
      }
      fPrintF(file,"\n");
    }
    
  }
  const bool & implicitAdvection = parameters.dbase.get<bool >("implicitAdvection");
  if( implicitAdvection )
    fPrintF(file," Treat advection terms implicitly (when using implicit time-stepping).\n");
  else
    fPrintF(file," Treat advection terms explicitly (when using implicit time-stepping).\n");
  
  fPrintF(file," thermalConductivity=%g\n",thermalConductivity);

}
