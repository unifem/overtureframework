// ========================================================================================================
/// \class Cgmp
/// \brief A multi-physics multi-domain solver.
/// \details This solver can be use to solve multi-physics problems where different equations are solved
///   in different domians. One such example would be to solve a thermal hydraulics problem where
///   Cgins is used in the fluid domain and Cgad in the solid domain.
// ========================================================================================================


#include "Cgmp.h"
#include "MpParameters.h"
#include "GenericGraphicsInterface.h"

#include "Cgad.h"
#include "Cgasf.h"
#include "Cgcns.h"
#include "Cgins.h"
#include "Cgsm.h"

//\begin{>CgmpInclude.tex}{\subsection{constructor}}
Cgmp::
Cgmp(CompositeGrid & cg, GenericGraphicsInterface *ps /* =NULL */,
     Ogshow *show /* =NULL */, const int & plotOption /* =1 */ )
  : DomainSolver(*(new MpParameters),cg,ps,show,plotOption)
// =============================================================================================================
// /Description:
//    Constructor for Cgmp, the multi-physics solver
//
// /cg (input) : CompositeGrid. For multi-physics problems this composite grid will generally be built with multiple domains.
// /ps (input) : pointer to a graphics object.
// /show (input) : pointer to a show file object.
// /plotOption (input) : the plot option for building PlotStuff.
//
//\end{CgmpInclude.tex}
// =============================================================================================================
{
  className="Cgmp";
  name="mp";
  interpolant=NULL;
  // 071005 kkc added the following code from VulcanTH
  cg.update(GridCollection::THEdomain);
  domainSolver.resize(cg.numberOfDomains());
  std::vector<int> & domainOrder = parameters.dbase.get<std::vector<int> >("domainOrder");
  domainOrder.resize(cg.numberOfDomains());    // advance the domains in this order
  for ( int d=0; d<domainSolver.size(); d++ )
  {
    domainSolver[d] = 0;
    domainOrder[d] = d;
    // set the domain name in each domain CG to match the multi-domain names *wdh* 080814
    cg.domain[d].setDomainName(0,cg.getDomainName(d));
  }
  if ( !parameters.dbase.has_key("plotOption") )
    parameters.dbase.put<int>("plotOption",plotOption);
  else
    parameters.dbase.get<int>("plotOption") = plotOption;

}


Cgmp::
~Cgmp()
{
  ForDomain(d)
  {
    delete domainSolver[d];
    domainSolver[d] = 0;

    if( interpolant!=NULL )
    {
      if( interpolant[d].decrementReferenceCount()==0 )
	delete &interpolant[d];
    }
  }

  delete &parameters;
}

//\begin{>>Cgad.tex}{\subsection{setParametersInteractively}}
int Cgmp::
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition)
//===================================================================================
// /Description:
//    Setup the PDE to be solved
// /Author: WDH
//
//\end{CgadInclude.tex}
// =======================================================================================================
{
  return 0;

}

//\begin{>>CgmpInclude.tex}{\subsection{getModelInfo}}
int Cgmp::
getModelInfo( std::vector<aString> & modelName )
// ========================================================================================================
// /Description:
//    This function is used to provide Cgmp with information about the 'models' (i.e. PDE solver classes that
//   have been derived from the class DomainSolver).
//
//  /modelName (output) : an array of names of the models.
//
//\end{CgmpInclude.tex}
// ========================================================================================================
{
  modelName.push_back("Cgins");
  modelName.push_back("Cgcns");
  modelName.push_back("Cgad");
  modelName.push_back("Cgasf");
  modelName.push_back("Cgsm");

  return 0;
}

//\begin{>>CgmpInclude.tex}{\subsection{buildModel}}
DomainSolver* Cgmp::
buildModel( const aString & modelName,
            CompositeGrid & cg, GenericGraphicsInterface *ps /* =NULL */, Ogshow *show /* =NULL */, const int & plotOption /* =1 */ )
// ========================================================================================================
// /Description:
//    This function acts as a virtual constructor to 'new' a copy of a DomainSolver model.
//
//  /modelName (input) : build an model with this name.
//  /cg,ps,show,plotOption (input) : parameters that are passed to the DomainSolver constructor. cg will be the CompositeGrid
//      that corresponds to the domain over-which the model is valid.
//
//  /Return value: a pointer to the model.
//
//\end{CgmpInclude.tex}
// ========================================================================================================
{
  DomainSolver *solver=NULL;
  if( modelName=="Cgasf")
  {
    solver = new Cgasf(cg,ps,show,plotOption);
  }
  else if( modelName=="Cgins")
  {
    solver = new Cgins(cg,ps,show,plotOption);
  }
  else if( modelName=="Cgcns")
  {
    solver = new Cgcns(cg,ps,show,plotOption);
  }
  else if( modelName=="Cgad" )
  {
    solver = new Cgad(cg,ps,show,plotOption);
  }
  else if( modelName=="Cgsm" )
  {
    solver = new Cgsm(cg,ps,show,plotOption);
  }
  else
  {
    printF("Cgmp:buildModel:ERROR: unknown modelName=[%s]\n",(const char*)modelName);
    OV_ABORT("Cgmp:buildModel:ERROR: unknown modelName");
  }

  // Tell the domain solver that this is a multi-domain problem.
  solver->parameters.dbase.get<int>("multiDomainProblem")=1;

  // Provide the domain solver with a pointer to Cgmp
  solver->parameters.dbase.get<DomainSolver*>("multiDomainSolver")=this;

  return solver;
}



// int Cgmp::
// updateForNewTimeStep(GridFunction & gf)
// {
//   for( int d=0; d<domainSolver.size(); d++ )
//   {
//     domainSolver[d]->updateForNewTimeStep(domainSolver[d]->gf[0]);
//   }
//   return 0;
// }
