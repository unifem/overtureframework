#include "Projection.h"
#include "OgesEnums.h"
//==============================================================================
//\begin{>setEllipticSolverParameter.tex}{\subsection{ellipticSolverParameterWizard}}
void Projection:: 
ellipticSolverParameterWizard ()
//
// /Purpose: interactively set elliptic solver parameters for the Projection class
//
//\end{setEllipticSolverParameter.tex}
//============================================================================== )
{
    
  int solverType_;
  
  cout << "***Enter elliptic solver type ("
       << Oges::yale   <<"=yale,"
       << Oges::harwell<<"=harwell,"
       << Oges::bcg    <<"=bcg, "
       << Oges::sor    <<"=sor): ";
  cin >> solverType_;
  this->setEllipticSolverParameter (Projection::solverType, (Oges::solvers) solverType_);
  
  if (solverType_ == (int)Oges::bcg)
  {
    int preconditionerType;
    cout << "***Enter preconditioner ("
	 << Oges::none         <<"=none, "
	 << Oges::diagonal     << "=diagonal, "
	 << Oges::incompleteLU << "=incompleteLU, "
	 << Oges::SSOR         << "=SSOR): ";
    cin >> preconditionerType;
    this->setEllipticSolverParameter (Projection::conjugateGradientPreconditioner, 
					   (Oges::conjugateGradientPreconditioners) preconditionerType);
  }

  if (solverType_ == (int)Oges::bcg)
  {
    int conjugateGradientType_;
    cout << "***Enter conjugate gradient type ("
	 << Oges::biConjugateGradient        << "=biCG, "
	 << Oges::biConjugateGradientSquared << "=biCGSquared,  "
	 << Oges::incompleteLU               << "=GMRes, "
	 << Oges::CGStab                     << "=CGStab): ";
    cin >> conjugateGradientType_;
    this->setEllipticSolverParameter (Projection::conjugateGradientType, (Oges::conjugateGradientTypes) conjugateGradientType_);
  }
  
  
  if (solverType_ != (int)Oges::yale && solverType_ != (int)Oges::harwell)
  {
    int numberOfIterations;
    cout << "***Enter number of iterations: ";
    cin >> numberOfIterations;
    if (solverType_ == (int)Oges::bcg)
      this->setEllipticSolverParameter (Projection::conjugateGradientNumberOfIterations, numberOfIterations);
    if (solverType_ == (int)Oges::sor)
      this->setEllipticSolverParameter (Projection::sorNumberOfIterations, numberOfIterations);
  }
}


//==============================================================================
//\begin{>setEllipticSolverParameter.tex}{\subsection{setEllipticSolverParameter(EllipticSolverParameter\&, Oges::conjugateGradientPreconditioners\&)}} 
void Projection:: 
setEllipticSolverParameter (const EllipticSolverParameter & parameterName, const Oges::conjugateGradientPreconditioners & type)
//
//\end{setEllipticSolverParameter.tex}
//============================================================================== )
{
  switch (parameterName)
  {
  case conjugateGradientPreconditioner:
    cout << "Projection::setEllipticSolverParameter: conjugateGradientPreconditioner to " << type << endl;
    conjugateGradientPreconditionerValue = type;
    conjugateGradientPreconditionerReset = TRUE;
    break;
    
  default:
    cout << "Projection::setEllipticSolverParameter: unrecognized option " << parameterName << endl;
    assert (FALSE);
    break;
    
  };
  
}


//===========================================================================
//\begin{>>setEllipticSolverParameter.tex}{\subsection{setEllipticSolverParameter(EllipticSolverParameter\&, Oges::conjugateGradientTypes\&)}}
void Projection:: 
setEllipticSolverParameter (const EllipticSolverParameter & parameterName, const Oges::conjugateGradientTypes & type)
//
//\end{setEllipticSolverParameter.tex}
//============================================================================== 
{
  switch (parameterName)
  {
  case conjugateGradientType:
    cout << "Projection::setEllipticSolverParameter: conjugateGradientType to " << type << endl;
    conjugateGradientTypeValue = type;
    conjugateGradientTypeReset = TRUE;
    break;
    
  default:
    cout << "Projection::setEllipticSolverParameter: unrecognized option " << parameterName << endl;
    assert (FALSE);
    break;
    
  };
}

//==============================================================================
//\begin{>>setEllipticSolverParameter.tex}{\subsection{setEllipticSolverParameter(EllipticSolverParameter\&, int\&)}} 
void Projection:: 
setEllipticSolverParameter (const EllipticSolverParameter & parameterName, const int & value)
//
//\end{setEllipticSolverParameter.tex}
//============================================================================== 
{
  switch (parameterName)
  {
  case conjugateGradientNumberOfIterations:
    cout << "Projection::setEllipticSolverParameter conjugateGradientNumberOfIterations to " << value << endl;
    conjugateGradientNumberOfIterationsValue = value;
    conjugateGradientNumberOfIterationsReset = TRUE;
    break;
    
  case conjugateGradientNumberOfSaveVectors:
    cout << "Projection::setEllipticSolverParameter conjugateGradientNumberOfSaveVectors to " << value << endl;
    conjugateGradientNumberOfSaveVectorsValue = value;
    conjugateGradientNumberOfSaveVectorsReset = TRUE;
    break;
    
  case sorNumberOfIterations:
    cout << "Projection::setEllipticSolverParameter sorNumberOfIterations to " << value << endl;
    sorNumberOfIterationsValue = value;
    sorNumberOfIterationsReset = TRUE;
    break;

  case iterativeImprovement:
    cout << "Projection::setEllipticSolverParameter: iterativeImprovement to " << value << endl;
    iterativeImprovementValue = value;
    iterativeImprovementReset = TRUE;
    
    break;
    
  case preconditionBoundary:
    cout << "Projection::setEllipticSolverParameter: preconditionBoundary to " << value << endl;
    preconditionBoundaryValue = value;
    preconditionBoundaryReset = TRUE;
    
    break;    
  default:
    cout << "Projection::setEllipticSolverParameter: unrecognized option " << parameterName << endl;
    assert (FALSE);
    break;
    
  };    
}
//==============================================================================
//\begin{>>setEllipticSolverParameter.tex}{\subsection{setEllipticSolverParameter(EllipticSolverParameter\&, REAL\&)}} 
void Projection:: 
setEllipticSolverParameter (const EllipticSolverParameter & parameterName, const REAL & value)
//
//\end{setEllipticSolverParameter.tex}
//============================================================================== 
{
  switch (parameterName)
  {
  case fillinRatio:
    cout << "Projection::setEllipticSolverParameter: fillinRatio to " << value << endl;
    fillinRatioValue = value;
    fillinRatioReset = TRUE;
    
    break;
    
  case zeroRatio:
    cout << "Projection::setEllipticSolverParamter: zeroRatio to " << value << endl;
    zeroRatioValue = value;
    zeroRatioReset = TRUE;
    
    break;
    
  case fillinRatio2:
    cout << "Projection::setEllipticSolverParameter: fillinRatio2 to " << value << endl;
    fillinRatio2Value = value;
    fillinRatio2Reset = TRUE;
    
    break;
    
  case harwellTolerance:
    cout << "Projection::setEllipticSolverParameter: harwellTolerance to " << value << endl;
    harwellToleranceValue = value;
    harwellToleranceReset = TRUE;
    
    break;
    
  case matrixCutoff:
    cout << "Projection::setEllipticSolverParameter: matrixCutoff to " << value << endl;
    matrixCutoffValue = value;
    matrixCutoffReset = TRUE;
    
    break;
    
  case sorOmega:
    cout << "Projection::setEllipticSolverParameter: sorOmega to " << value << endl;
    sorOmegaValue = value;
    sorOmegaReset = TRUE;
    
    break;
    
  default:
    cout << "Projection::setEllipticSolverParameter: unrecognized option " << parameterName << endl;
    assert (FALSE);
    break;
    
  };
}


//==============================================================================
//\begin{>>setEllipticSolverParameter.tex}{\subsection{setEllipticSolverParameter(EllipticSolverParameter\&, Oges::solvers\&)}} 
void Projection:: 
setEllipticSolverParameter (const EllipticSolverParameter & parameterName, const Oges::solvers & type)
//
// /Purpose:
//   These routines provide an interface to the parameters that can be set in
//   the  solver used in the Projection class. The parameters are not actually
//   set by these routines, rather a request is registered, and the parameters
//   are set at an appropriate time. Here is a list of possible values for
//   "parameterName" as well as possible values for the second argument:
//   see the {\bf Oges} documentation for more information. 
//
//   \begin{itemize} 
//     \item{conjugateGradientPreconditioner} \{Oges::none,Oges::diagonal, Oges::incompleteLU, Oges::SSOR \} 
//     \item{conjugateGradientType} \{Oges::biConjugateGradient ,
//                                    Oges::biConjugateGradientSquared,
//                                    Oges::GMRes,
//                                    Oges::CGStab\} 
//     \item{conjugateGradientNumberOfIterations} \{any positive integer\} 
//     \item{conjugateGradientNumberOfSaveVectors} \{any positive integer\}  
//     \item{fillinRatio} \{any REAL number\} 
//     \item{harwellTolerance} \{a REAL number\}  
//     \item{matrixCutoff} \{a REAL number\}  
//     \item{iterativeImprovement}  \{TRUE or FALSE\}  
//     \item{preconditionBoundary}  \{TRUE or FALSE\} 
//     \item{solverType} \{Oges::yale, Oges::harwell, Oges::bcg, Oges::sor \} 
//     \item{sorNumberOfIterations} \{a positive integer\} 
//     \item{sorOmega}  \{a REAL number\} 
//   \end{itemize} 
//\end{setEllipticSolverParameter.tex}
//============================================================================== )
{
  switch (parameterName)
  {
  case solverType:
    cout << "Projection::setEllipticSolverParameter: solverType to " << type << endl;
    solverTypeValue = type;
    solverTypeReset = TRUE;
    break;
    
  default:
    cout << "Projection::setEllipticSolverParameter: unrecognized option " << parameterName << endl;
    assert (FALSE);
    break;
    
  };    
}

			    
