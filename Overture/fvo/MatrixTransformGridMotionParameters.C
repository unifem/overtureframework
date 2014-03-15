#include "MatrixTransformGridMotionParameters.h"


//\begin{>>MatrixTransformGridMotionParameters.tex}{\subsubsection{Default Constructor}}
MatrixTransformGridMotionParameters::
MatrixTransformGridMotionParameters()
//
// /Purpose: Default constructor for this class.
//           (DLB981001) It is not clear to me that you can use this constructor since there is
//           no way to initialize the CompositeGrid-related parameters in the class
//\end{MatrixTransformGridMotionParameters.tex}           
{
  cout << " MatrixTransformGridMotionParameters::default constructor called" << endl;
  motionFunctionInitialized = LogicalFalse;
}

//\begin{>>MatrixTransformGridMotionParameters.tex}{\subsubsection{Copy Constructor}}
MatrixTransformGridMotionParameters::
MatrixTransformGridMotionParameters(const MatrixTransformGridMotionParameters & params_) 
  :
  GenericGridMotionParameters ( params_),
  useMotionFunction_          ( params_.useMotionFunction_),
  rotationRate                ( params_.rotationRate),
  translationRate             ( params_.translationRate)
//
// /Purpose:
//  copy constructor for this class; note that it is supposed to 
// make a deep copy of the motionFunction, however at the moment it makes a shallow copy.
//\end{MatrixTransformGridMotionParameters.tex}
{
  cout << "MatrixTransformGridMotionParameters::copy constructor called" << endl;
  //...deep copy of the motionFuntion
  motionFunction_ = copyMotionFunction (params_.motionFunction_);
}

//\begin{>>MatrixTransformGridMotionParameters.tex}{\subsubsection{operator=}}
MatrixTransformGridMotionParameters& MatrixTransformGridMotionParameters::
operator=(const MatrixTransformGridMotionParameters & params_) 
//
// /Purpose:  operator= for this class; this is supposed to make a deep copy of the motionFunction,
//    but at the moment it makes a shallow copy instead
//\end{MatrixTransformGridMotionParameters.tex}
{
  if (this == &params_) return *this;
  
  static_cast<GenericGridMotionParameters&>(*this) = params_;

  useMotionFunction_ = params_.useMotionFunction_;
  rotationRate    = params_.rotationRate;
  translationRate = params_.translationRate;

  //...deep copy of motionFunction
  motionFunction_  = copyMotionFunction(params_.motionFunction_);  

  return *this;
}

MatrixTransformGridMotionParameters::
MatrixTransformGridMotionParameters(const CompositeGrid& compositeGrid_)
{
  cout << "  <<<<<>>>>>>>>>>>>>>>>> " << endl;
  cout << "<<<WARNING>>> You've called an obsolete constructor for the MatrixTransformGridMotionParameters" << endl;
  cout << "  Use MatrixTransformGridMotionParameters(numberOfDimensions, numberOfGrids) instead " << endl;
  exit (-1);
  
}

  
//\begin{>>MatrixTransformGridMotionParameters.tex}{\subsubsection{Main Constructor}}
MatrixTransformGridMotionParameters::
MatrixTransformGridMotionParameters(const int& numberOfDimensions_,
				    const int& numberOfGrids_)
  : GenericGridMotionParameters (numberOfDimensions_, numberOfGrids_)
//
// /Purpose: main constructor for this class
//     This constructor should be used to instantiate this class.
// /compositeGrid\_:  this CompositeGrid will be used to determine parameters for this class
//\end{MatrixTransformGridMotionParameters.tex}
{
  rotationRate.resize(numberOfGrids);
  translationRate.resize(numberOfGrids,3);
  for (int i=0; i<numberOfGrids; i++)
  {
    rotationRate(i) = (real)0.;
    for (int j=0; j<3; j++)
      translationRate(i,j) = (real) 0.;
  }

  setupMotionFunction();
}

MatrixTransformGridMotionParameters::
~MatrixTransformGridMotionParameters ()
{
  cout << "MatrixTransformGridMotionParameters::destructor called" << endl;
  destructMotionFunction();
}

//\begin{>>MatrixTransformGridMotionParameters.tex}{\subsubsection{setMotionFunction}}
void MatrixTransformGridMotionParameters::
setMotionFunction (const int& grid,
		   MatrixTransformMotionFunction* motionFunction__)
//
// /Purpose:
//    set a MotionFunction for a particular grid
// /grid: which grid
// /motionFunction\_: which MotionFunction to use
//\end{MatrixTransformGridMotionParameters.tex}
{
  assert (motionFunctionInitialized);
  assert (grid>0 && grid<numberOfGrids);
  
  motionFunction_[grid] = motionFunction__;
  useMotionFunction_(grid) = LogicalTrue;
}

MatrixTransformMotionFunction** MatrixTransformGridMotionParameters::
copyMotionFunction (MatrixTransformMotionFunction** mf)
{
  cout << "MatrixTransformGridMotionParameters::copyMotionFunction ***NOT IMPLEMENTED***" << endl;
  cout << " Making a shallow copy instead " << endl;
  return mf;
}

void MatrixTransformGridMotionParameters::
setupMotionFunction()
{
  Overture::abort("This no longer works");
/* ---  
  assert (!motionFunctionInitialized);
  assert (numberOfGrids>0);
  
  motionFunction_ = new MatrixTransformMotionFunction*[numberOfGrids];
  assert (motionFunction_ != NULL);
  for (int grid=0; grid<numberOfGrids; grid++)
  {
    motionFunction_[grid] = NULL;
  }
  motionFunctionInitialized = LogicalTrue;

  useMotionFunction_.redim(numberOfGrids);
  useMotionFunction_ = LogicalFalse;
  --- */
}


void MatrixTransformGridMotionParameters::
destructMotionFunction()
{
  delete [] motionFunction_;
  motionFunctionInitialized = LogicalFalse;
}

//\begin{>>MatrixTransformGridMotionParameters.tex}{\subsubsection{setupWizard}}
bool MatrixTransformGridMotionParameters::
setupWizard()
//
// /Purpose:
//   Provides an interactive interface for setting up parameters for the
//   MatrixTransformGridMotionParameters class.  This will query and
//   setup parameters as well as MotionFunction's
//
//\end{MatrixTransformGridMotionParameters.tex}
{
  bool success = LogicalTrue;
  
  cout << "There are " << numberOfGrids << " component grids" << endl;
  cout << "All but the first one may be moved" << endl;

  int move;
  real rate;

//==============================================================================
// trCoeff, trFreq, trOffset, angCoeff, angFreq and angOffset
//   are parameters for the MatrixTransformMotionFunction class
//==============================================================================

  RealArray trCoeff(numberOfDimensions), trFreq(numberOfDimensions),trOffset(numberOfDimensions);
  trCoeff = 0.;
  trFreq = 0.;
  trOffset = 0.;
  real angCoeff=0., angFreq=0., angOffset=0.;

  int grid;
  
  for (grid=1; grid<numberOfGrids; grid++)
  {
    //============================================================
    //...here we allow for different ways to move (or not) the grids
    //============================================================

    cout << "Move grid number " << grid << "?(0=no,1=const,2=function) ";
    cin >> move;
    this->movingGrid(grid) = move>0 ? LogicalTrue: LogicalFalse;
    
    switch (move)
    {
    case 0:
      break;
      
    case 1:
      cout << "Rotation rate (in revolutions per unit time) for grid " << grid << ": ";
      cin >> rate;
      this->rotationRate(grid) = rate;
      break;
      
    case 2:

      trCoeff = 0.;
      trFreq = 0.;
      trOffset = 0.;

      cout << endl << " angle = angCoeff * sin (2*Pi*angFreq * (t - angOffset))" << endl << endl;
      
      cout << "enter angCoeff, angFreq, angOffset: ";
      cin >> angCoeff >> angFreq >> angOffset;

      cout << endl << " x = trCoeff * sin (2*Pi*trFreq*(t-trOffset)) " << endl << endl;
      
      cout << "enter trCoeff, trFreq, trOffset for axis1: ";
      cin >> trCoeff(axis1) >> trFreq(axis1) >> trOffset(axis1);
      cout << "enter trCoeff, trFreq, trOffset for axis2: ";
      cin >> trCoeff(axis2) >> trFreq(axis2) >> trOffset(axis2);

      //============================================================
      // make a MatrixTransformMotionFunction and set its parameters
      //============================================================

      MatrixTransformMotionFunction* motionFunction = new MatrixTransformMotionFunction (numberOfDimensions);
      if (motionFunction == NULL) success = LogicalFalse;

      motionFunction->setAngularParameters (angCoeff, angFreq, angOffset);
      motionFunction->setTranslationalParameters (trCoeff, trFreq, trOffset);

      //============================================================
      //tell the Parameters class to use this motion function for this grid
      //============================================================
      this->setMotionFunction (grid, motionFunction);
      
      break;
      
    };
    
  }

  return success;

}

