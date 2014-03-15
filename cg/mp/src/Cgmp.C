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


// int Cgmp::
// updateForNewTimeStep(GridFunction & gf)
// {
//   for( int d=0; d<domainSolver.size(); d++ )
//   {
//     domainSolver[d]->updateForNewTimeStep(domainSolver[d]->gf[0]);
//   }
//   return 0;
// }
