#include "GenericGridMotionParameters.h"

//\begin{>>GenericGridMotionParameters.tex}{\subsubsection{Default Constructor}}
GenericGridMotionParameters::
GenericGridMotionParameters()
//
// /Purpose: Default constructor for this class. 
//\end{GenericGridMotionParameters.tex}
{
  cout << " GenericGridMotionParameters::default constructor called" << endl;
}
  
//\begin{>>GenericGridMotionParameters.tex}{\subsubsection{Copy Constructor}}
GenericGridMotionParameters::
GenericGridMotionParameters(const GenericGridMotionParameters & params_) 
  :
  movingGrid      ( params_.movingGrid),
  numberOfGrids   ( params_.numberOfGrids)
//
// /Purpose:
//   copy constructor for this class.
//\end{GenericGridMotionParameters.tex}
{
  cout << "GenericGridmotionParameters::copy constructor called" << endl;
}

//\begin{>>GenericGridMotionParameters.tex}{\subsubsection{operator=}}
GenericGridMotionParameters& GenericGridMotionParameters::
operator=(const GenericGridMotionParameters & params_)
//
// /Purpose: operator= for this class;
//
//\end{GenericGridMotionParameters.tex}
{
  if (this == &params_) return *this;
  
  movingGrid    = params_.movingGrid;
  numberOfGrids = params_.numberOfGrids;

  return *this;
}

GenericGridMotionParameters::
GenericGridMotionParameters(const CompositeGrid& compositeGrid_)
{

  cout << "  <<<<<>>>>>>>>>>>>>>>>> " << endl;
  cout << "<<<WARNING>>> You've called an obsolete constructor for the GenericGridMotionParameters" << endl;
  cout << "  Use GenericGridMotionParameters(numberOfDimensions, numberOfGrids) instead " << endl;
  exit (-1);

}

//\begin{>>GenericGridMotionParameters.tex}{\subsubsection{Main constructor}}
GenericGridMotionParameters::
GenericGridMotionParameters(const int& numberOfDimensions_,
			    const int& numberOfGrids_)
//
// /Purpose:
//   main constructor for this class.  
// /numberOfDimensions (input): compositeGrid.numberOfDimensions()
// /numberOfGrids      (input): compositeGrid.numberOfGrids()
//\end{GenericGridMotionParameters.tex}
{
//numberOfGrids = compositeGrid_.numberOfGrids();
//numberOfDimensions = compositeGrid_.numberOfDimensions();
  
  numberOfDimensions = numberOfDimensions_;
  numberOfGrids = numberOfGrids_;

  movingGrid.resize(numberOfGrids);
  for (int i=0; i<numberOfGrids; i++)
    movingGrid(i) = LogicalFalse;
}

GenericGridMotionParameters::
~GenericGridMotionParameters ()
{
  cout << "GenericGridMotionParameters::destructor called" << endl;
}
