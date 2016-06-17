// ========================================================================================================
/// \class Cgasf
/// \brief All-speed Compressible Navier-Stokes flow solver.
/// \details The all-speed solver can solve the compressible Navier-Stokes equations for low to 
///          moderate Mach numbers.
// ========================================================================================================

#include "Cgasf.h"
#include "AsfParameters.h"
#include "App.h"
#include "Ogshow.h"

Cgasf::
Cgasf(CompositeGrid & cg_, 
      GenericGraphicsInterface *ps /* =NULL */, 
      Ogshow *show /* =NULL */ , 
      const int & plotOption_ /* =1 */) 
   : DomainSolver(*(new AsfParameters),cg_,ps,show,plotOption_)
// ===================================================================================================
// Notes:
//   AsfParameters (passed to the DomainSolver constructor above) replaces the base class Parameters
// ===================================================================================================
{
  className="Cgasf";
  name="asf";
  gridMachNumber.resize(cg.numberOfComponentGrids(),0.);
  
  numberOfImplicitSolves=0;
  refactorImplicitMatrix=false;

}



Cgasf::
~Cgasf()
{
  delete & parameters;
}

int Cgasf::
applyBoundaryConditions(GridFunction & cgf,
                        const int & option /* =-1 */,
                        int grid/* = -1 */,
                        GridFunction *puOld /* =NULL */, 
                        const real & dt /* =-1. */ )
// Put this here since we overloaded another function of this name
{
  return DomainSolver::applyBoundaryConditions(cgf,option,grid,puOld,dt);
}

int Cgasf::
initializeSolution()
{
  printF(" ----Cgasf::initializeSolution------ \n");

  // The initial conditions should have alaredy been assigned
  const int & rc = parameters.dbase.get<int >("rc");
  const int & pc = parameters.dbase.get<int >("pc");

  if( parameters.dbase.get<int >("linearizeImplicitMethod") )
  {
    cout << ".......Cgasf::initializeSolution for rL and pL (linearized solution).... \n";

    // update grid functions that hold the linearized state
    if( parameters.dbase.get<int >("linearizeImplicitMethod") )
    {
      if( prL==NULL )
      {
	prL=new realCompositeGridFunction(cg);
	ppL=new realCompositeGridFunction(cg);
	if( parameters.dbase.get<bool >("computeReactions") )
	{
	  pgam=new realCompositeGridFunction(cg);
	  gam()=1.4;
	}
      }
      else
      {
	rL().updateToMatchGrid(cg);
	pL().updateToMatchGrid(cg);
	if( parameters.dbase.get<bool >("computeReactions") )
	{
	  gam().updateToMatchGrid(cg);
	  gam()=1.4;
	}
      }
    }

    CompositeGrid & cg = gf[current].cg;
    Range all;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const realArray & u0 = gf[current].u[grid];
      realMappedGridFunction & r0 = rL()[grid];
      realMappedGridFunction & p0 = pL()[grid];

      // r0=u0(all,all,all,parameters.dbase.get<int >("rc"));
      // p0=u0(all,all,all,parameters.dbase.get<int >("pc"));  // leave off pressureLevel

      
      assign(r0,all,all,all,0, u0,all,all,all,rc);
      assign(p0,all,all,all,0, u0,all,all,all,pc);
      

    }
  }

  int returnValue = DomainSolver::initializeSolution();

  // kkc 070201 stuff moved here from if block in DomainSolver::initilizeSolution
  if( !parameters.dbase.get<bool >("twilightZoneFlow") )
    // *wdh* 070708 && parameters.dbase.get<AsfParameters::TestProblems >("testProblem")==AsfParameters::standard  )
  {
    // define initial forces on moving bodies -- we really should iterate here since the 
    // forces depend on the pressure and the pressure depends on the forces.
    if( movingGridProblem() && gf[current].t==0. )
      correctMovingGrids( gf[current].t,gf[current].t, gf[current],gf[current] ); 
      
    // The initial condition routine does not supply p for the INS for real runs
    if( parameters.dbase.get<bool >("projectInitialConditions") )
    {
      printF("Cgasf::initializeSolution:Solve for the initial pressure field \n");
      solveForTimeIndependentVariables( gf[current] );     
    }
      
  }

  dt= getTimeStep( gf[current] ); 

  return returnValue;
}

int Cgasf::
updateGeometryArrays(GridFunction & cgf)
{
  if( debug() & 8 ) printF(" --- Cgasf::updateGeometryArrays ---\n");
  
  CompositeGrid & cg = cgf.cg;

  gridMachNumber.resize(cg.numberOfComponentGrids(),0.);

  int grid;
  for( grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cgf.cg[grid];
    if( mg.isRectangular() )
    {
      mg.update(MappedGrid::THEmask);
    }
    else
    {
      mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEinverseCenterDerivative );
    }
  }

  return DomainSolver::updateGeometryArrays(cgf);
}

int Cgasf::
updateToMatchGrid(CompositeGrid & cg)
{
  printF("\n $$$$$$$$$$$$$$$ Cgasf: updateToMatchGrid(CompositeGrid & cg) $$$$$$$$$$$$\n\n");
  
  int returnValue =DomainSolver::updateToMatchGrid(cg);

  const int numberOfDimensions=cg.numberOfDimensions();
  
  if( parameters.dbase.get<bool >("computeReactions")  )
  {
    Range all;
//     // this holds the viscous and diffusion coefficients:
//     if( ptransportCoefficients==NULL )
//       ptransportCoefficients= new realMappedGridFunction(mappedGrid,all,all,all,parameters.dbase.get<int >("numberOfSpecies")+2);
//     else
//       transportCoefficients().updateToMatchGrid(mappedGrid,all,all,all,parameters.dbase.get<int >("numberOfSpecies")+2);

//     transportCoefficients().setOperators(*transportOperators);
//     // --- Here are the derivatives we use for the viscous and diffusion terms (transport coefficients)
//     assert( transportOperators!=NULL );
//     transportOperators->setNumberOfDerivativesToEvaluate( mappedGrid.numberOfDimensions() );
//     transportOperators->setDerivativeType( 0,MappedGridOperators::xDerivative,get(WorkSpace::transportCoefficientsX));
//     if( mappedGrid.numberOfDimensions() > 1 )
//     {
//       transportOperators->setDerivativeType(1,MappedGridOperators::yDerivative,get(WorkSpace::transportCoefficientsY));
//       if( mappedGrid.numberOfDimensions() > 2 )
//         transportOperators->setDerivativeType( 2,MappedGridOperators::zDerivative, 
//                     get(WorkSpace::transportCoefficientsZ));
//     }
//     Range S0(0,parameters.dbase.get<int >("numberOfSpecies")-1);
//     if( pmoleFraction==NULL )
//     {
//       pmoleFraction = new realMappedGridFunction(mappedGrid,all,all,all,S0);
//       pdCoeff = new realMappedGridFunction(mappedGrid,all,all,all,S0);
//     }
//     else
//     {
//       moleFraction().updateToMatchGrid(mappedGrid,all,all,all,S0);
//       dCoeff().updateToMatchGrid(mappedGrid,all,all,all,S0);
//     }
    
//     moleFraction().setOperators(*transportOperators);
//     dCoeff().setOperators(*transportOperators);
  }
  

  // update grid functions that hold the linearized state
  if( parameters.dbase.get<int >("linearizeImplicitMethod") )
  {
    if( prL==NULL )
    {
      prL=new realCompositeGridFunction(cg);
      ppL=new realCompositeGridFunction(cg);
      if( parameters.dbase.get<bool >("computeReactions") )
      {
	pgam=new realCompositeGridFunction(cg);
	gam()=1.4;
      }
    }
    else
    {
      rL().updateToMatchGrid(cg);
      pL().updateToMatchGrid(cg);
      if( parameters.dbase.get<bool >("computeReactions") )
      {
	gam().updateToMatchGrid(cg);
	gam()=1.4;
      }
    }
  }

  
//   if( pde==OB_Parameters::allSpeedNavierStokes )
//   {
//     realArray & phi = get(WorkSpace::phi);
//     phi.redim(I1,I2,I3);
//   }
  
  return returnValue;
}

void Cgasf::
saveShowFileComments( Ogshow &show )
{
    // save comments that go at the top of each plot
  char buffer[80]; 
  aString timeLine="";
  if(  parameters.dbase.has_key("timeLine") )
    timeLine=parameters.dbase.get<aString>("timeLine");
  aString showFileTitle[5];
  showFileTitle[0]=sPrintF(buffer,"All speed NS, mu=%8.2e, k=%8.2e, gamma=%4.2f",parameters.dbase.get<real >("mu"),
			   parameters.dbase.get<real >("kThermal"),parameters.dbase.get<real >("gamma"));
  showFileTitle[1]=timeLine;
  showFileTitle[2]="";  // marks end of titles
  
  for( int i=0; showFileTitle[i]!=""; i++ )
    show.saveComment(i,showFileTitle[i]);
}

void Cgasf::
writeParameterSummary( FILE * file )
{
  DomainSolver::writeParameterSummary( file );

  if ( file==parameters.dbase.get<FILE* >("checkFile") )
    {
      fPrintF(parameters.dbase.get<FILE* >("checkFile"),"\\caption{All-speed Navier Stokes, gridName, $\\mu=%8.1e$, $t=%2.1f$, ",
	      parameters.dbase.get<real >("mu"),parameters.dbase.get<real >("tFinal"));
      return;
    }

  fPrintF(file," machNumber=%e, reynoldsNumber=%e, prandtlNumber=%e \n",parameters.dbase.get<real >("machNumber"),
	  parameters.dbase.get<real >("reynoldsNumber"),parameters.dbase.get<real >("prandtlNumber"));
  fPrintF(file," mu=%e, kThermal=%e, Rg=%e, gamma=%e \n",parameters.dbase.get<real >("mu"),parameters.dbase.get<real >("kThermal"),parameters.dbase.get<real >("Rg"),
	  parameters.dbase.get<real >("gamma"));

  const ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  if( gravity[0]!=0. || gravity[1]!=0. || gravity[2]!=0. )
    fPrintF(file," gravity is on, acceleration due to gravity = (%8.2e,%8.2e,%8.2e) \n",
	    gravity[0],gravity[1],gravity[2]);
      
  fPrintF(file," implicitMethod=");
  if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::notImplicit )
    fPrintF(file," notImplicit\n");
  else if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::backwardEuler )
    fPrintF(file," backwardEuler\n");
  else if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::secondOrderBDF )
    fPrintF(file," secondOrderBDF\n");
  else if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::crankNicolson )
    fPrintF(file," crankNicolson\n");
  else 
    fPrintF(file," unknown !!\n");

  if( parameters.dbase.get<int >("linearizeImplicitMethod") )
    {
      fPrintF(file," linearize implicit method, refactorFrequency=%i \n",parameters.dbase.get<int >("refactorFrequency"));
    }

  if( parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod")==Parameters::implicitAllSpeed )
    {
      fPrintF(file," Mach number = %e, pressureLevel =%e \n",parameters.dbase.get<real >("machNumber"), parameters.dbase.get<real >("pressureLevel"));
    }

  if( parameters.dbase.get<AsfParameters::TestProblems >("testProblem")!=AsfParameters::standard )
    {
      if( parameters.dbase.get<AsfParameters::TestProblems >("testProblem")== AsfParameters::laminarFlame ) 
	fPrintF(file," testProblem = laminarFlame \n");
      else
	fPrintF(file," testProblem = %i \n",parameters.dbase.get<AsfParameters::TestProblems >("testProblem"));
    }

}

