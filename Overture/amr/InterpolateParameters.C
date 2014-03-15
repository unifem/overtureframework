#include "InterpolateParameters.h"



const int InterpolateParameters::defaultAmrRefinementRatio =    4;
const GridFunctionParameters::GridFunctionType InterpolateParameters::defaultGridCentering 
                                                           =    GridFunctionParameters::vertexCentered;
const InterpolateParameters::InterpolateType InterpolateParameters::defaultInterpolateType 
                                                           =    InterpolateParameters::polynomial;
const int InterpolateParameters::defaultInterpolateOrder   = 2;
const int InterpolateParameters::defaultNumberOfDimensions = 2;

const real InterpolateParameters::coeffEps                 = REAL_EPSILON*100;

const InterpolateParameters::InterpolateOffsetDirection InterpolateParameters::defaultInterpolateOffsetDirection =
/* */  offsetInterpolateToLeft;

//\begin{>InterpolateParameters.tex}{\subsection{ InterpolateParameters default constructor}}
InterpolateParameters::
InterpolateParameters(const int numberOfDimensions_, const bool debug_)
//
// /Purpose: default constructor for the InterpolateParameters container class;
//           initialize the class and set default values
//
//\end{InterpolateParameters.tex}
{
  debug = debug_;
  
  amrRefinementRatio__.resize(3);
  IntegerArray nullArray;

  setNumberOfDimensions (numberOfDimensions_);
  setAmrRefinementRatio (nullArray);
  setInterpolateType ();
  setInterpolateOrder ();
  setGridCentering ();
//setPreComputeAllCoefficients ();
  setUseGeneralInterpolationFormula ();
}

//\begin{>>InterpolateParameters.tex}{\subsection{ InterpolateParameters destructor}}
InterpolateParameters::
~InterpolateParameters()
//
// /Purpose: destructor for the InterpolateParameters container class
//
//\end{InterpolateParameters.tex}
{
  // cout << "InterpolateParameters destructor called" << endl;
}


//\begin{>>InterpolateParameters.tex}{\subsection{setAmrRefinementRatio}}
void InterpolateParameters::
setAmrRefinementRatio (const IntegerArray& amrRefinementRatio_)
//
// /Purpose: set InterpolateParameters::amrRefinementRatio
//
// /amrRefinementRatio\_: the value in amrRefinementRatio\_(axis) is
//     the refinement ratio in the ``axis'' direction.  It is 
//     a positive number equal to the number of fine grid points
//     per coarse grid point in this direction; the Interpolate
//     class functions are only implemented for values of 
//     amrRefinementRatio that are a power of 2
//\end{InterpolateParameters.tex}
{

  int i;
  
  if (amrRefinementRatio_.isNullArray())
  {
    for (i=0; i<numberOfDimensions__; i++) amrRefinementRatio__(i)= InterpolateParameters::defaultAmrRefinementRatio;
    for (i=numberOfDimensions__; i<3; i++) amrRefinementRatio__(i) = 1;
  }
  else
  {
    for (i=0; i<3; i++)
    {
      amrRefinementRatio__(i) = amrRefinementRatio_(i);
    }
  }
  
}


//\begin{>>InterpolateParameters.tex}{\subsection{setInterpolateType}}
void InterpolateParameters::
setInterpolateType (const InterpolateParameters::InterpolateType interpolateType_)
//
// /Purpose: set InterpolateParameters::interpolateType
//
// /interpolateType\_: is used to set the type of interpolation. It can be
//    chosen from
//   enum InterpolateType
//  {
//    defaultValue,
//    polynomial,
//    fullWeighting,
//    nearestNeighbor,
//    injection,
//    numberOfInterpolateTypes
//  };
//
//  /000623: currently only polynomial interpolation is implemented in the \Interpolate
//           class
//
//\end{InterpolateParameters.tex}
{

  interpolateType__ = 
    interpolateType_ == InterpolateParameters::defaultValue ?
    defaultInterpolateType :
    interpolateType_;
}


//\begin{>>InterpolateParameters.tex}{\subsection{numberOfDimensions}}
void InterpolateParameters::
setNumberOfDimensions (const int numberOfDimensions_)
//
// /Purpose: set InterpolateParameters::numberOfDimensions
//
// /numberOfDimensions\_: since the \Interpolate functions only
//    deal with \realArray's, they have no way of knowing what the
//    dimension of the problem is.  This parameter is used to set
//    that value.
//
//\end{InterpolateParameters.tex}
{
  numberOfDimensions__ = numberOfDimensions_;
}


//\begin{>>InterpolateParameters.tex}{\subsection{setInterpolateOrder}}
void InterpolateParameters::
setInterpolateOrder (const int interpolateOrder_)
//
// /Purpose: set InterpolateParameters::interpolateOrder
//
// /interpolateOrder\_: this is the order of interpolation that will be
//   used by the interpolation functions in the \Interpolate class.
//   It must be set initially because the interpolation stencil size
//   is determined from it, and is used in the precomputation of the
//   interpolation coefficient matrix.
//
//\end{InterpolateParameters.tex}
{

  if (interpolateOrder_ > 0 )
  {
    interpolateOrder__ = interpolateOrder_;
  }
  else
  {
    cout << "InterpolateParameters::setInterpolateOrder: " << interpolateOrder_ << 
      " is not a valid choice for interpolateOrder " << endl;
    exit (-1);
  }
}


//\begin{>>InterpolateParameters.tex}{\subsection{setGridCentering}}
void InterpolateParameters::
setGridCentering (const GridFunctionParameters::GridFunctionType gridCentering_)
//
// /Purpose: set InterpolateParameters::gridCentering
//
// /gridCentering\_: this parameter is used to tell what kind of centering
//   is used on the underlying grid.  Again, since the \Interpolate 
//   class doesn't see anything but the \realArray's it can't tell what
//   the centering of the mesh was.
//
//\end{InterpolateParameters.tex}
{
  gridCentering__ = gridCentering_;
}

//\begin{>>InterpolateParameters.tex}{\subsection{setUseGeneralInterpolationFormula}}
void InterpolateParameters::
setUseGeneralInterpolationFormula (const bool TrueOrFalse // = LogicalFalse
  )
//
// /Purpose: set InterpolateParameters::UseGeneralInterpolationFormula. If
//    set to True, the interpolation will be computed using the general
//    formula rather than the ``optimized'' explicitly written-out formula 
//    for lower interpolation orders
//
//  /000626: N.B. The general interpolation formula is actually optimized for
//           the cases where some of the interpolation coefficients are
//           zero, i.e. it doesn't multiply by those coefficients.  The
//           explicit formulas are not currently optimized for these special
//           cases
//
// 
//
//\end{InterpolateParameters.tex}
{
  useGeneralInterpolationFormula__ = TrueOrFalse;
}


//\begin{>>InterpolateParameters.tex}{\subsection{interactivelySetParameters}}
int InterpolateParameters::
interactivelySetParameters ()
//
// /Purpose: interactively set InterpolateParameters parameters using NameList
//
//\end{InterpolateParameters.tex}
{
  aString answer(80), name(80);
//  bool error;
  int i;
  
  for (;;)
  {
    cout << "Enter changes to variables, exit [ex] to continue" << endl;
    cin >> answer;
    
    if (answer == "exit" || answer(0,1) == "ex") break;
    
    nl.getVariableName (answer, name);

    if ( name(0,2) == "deb")
    {
      debug = nl.intValue(answer);
      cout << "debug = " << debug << endl;
    }

    else if ( name(0,2) == "amr" )
    {
//      amrRefinementRatio = nl.intValue(answer);
      nl.getIntArray (answer, amrRefinementRatio__, i);
      cout << "amrRefinementRatio(" << i << ") = " << amrRefinementRatio__(i) << endl;
    }
    
    else if ( name(0,2) == "gri" )
    {
      gridCentering__ = (GridFunctionParameters::GridFunctionType) nl.intValue(answer);
      cout << "gridCentering = " << gridCentering__ << endl;
    }

    else if (name(0,3) == "useG" )
    {
      useGeneralInterpolationFormula__ = (bool) nl.intValue(answer);
      cout << "useGeneralInterpolationFormula = " << useGeneralInterpolationFormula__ << endl;
    }
        
    
    else if (name(0,1) == "it" || name(0,11) == "interpolateT")
    {
      interpolateType__ = (InterpolateParameters::InterpolateType) nl.intValue(answer);
      cout << "interpolateType = " << interpolateType__ << endl;
    }

    else if (name(0,1) == "io" || name(0,11) == "interpolateO")
    {
      interpolateOrder__ = nl.intValue(answer);
      cout << "interpolateOrder = " << interpolateOrder__ << endl;
    }

    else if (name(0,3) == "maxR")
    {
      maxRefinementRatio__ = nl.intValue(answer);
      cout << "maxRefinementRatio = " << maxRefinementRatio__ << endl;
    }

    else if (name(0,1) == "nd" || name(0,8) == "numberOfD")
    {
      numberOfDimensions__ = nl.intValue(answer);
      cout << "numberOfDimensions = " << numberOfDimensions__ << endl;
    }
            
    else
      cout << "unknown response [" << name << "]" << endl;
  }
  return 0;

}

void InterpolateParameters::
display () const
//
// /purpose: display values of all the parameters
//
{
  cout << "==================================================" << endl;
  cout << "                                          " << endl;
  cout << "debug                                     " << debug << endl;
  cout << "[maxR]efinementRatio                      " << maxRefinementRatio__ << endl;
  cout << "[amr]RefinementRatio                      " << amrRefinementRatio__(0) 
                                                  <<","<< amrRefinementRatio__(1)
                                                  <<","<< amrRefinementRatio__(2) << endl;
  cout << "[gri]dCentering                           " << gridCentering__ << endl;
  cout << "[useG]eneralInterpolationFormula          " << useGeneralInterpolationFormula__ << endl;
  cout << "[interpolateT]ype  [it]                   " << interpolateType__ << endl;
  cout << "[interpolateO]rder  [io]                  " << interpolateOrder__ << endl;
  cout << "                                          " << endl;
  cout << "==================================================" << endl;
}


//\begin{>>InterpolateParameters.tex}{\subsection{get}}
int InterpolateParameters::
get( const GenericDataBase & dir, const aString & name)
// ===========================================================================
// /Description:
//   Get from a data base file.
//\end{InterpolateRefinementsInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"InterpolateParameters");

  aString className;
  subDir.get( className,"className" ); 
  if( className != "InterpolateParameters" )
  {
    cout << "InterpolateParameters::get ERROR in className!" << endl;
  }

  subDir.get(amrRefinementRatio__,"amrRefinementRatio__");  
  subDir.get(maxRefinementRatio__,"maxRefinementRatio__");
  subDir.get(numberOfDimensions__,"numberOfDimensions__");
  int temp;
  subDir.get(temp,"gridCentering__"); gridCentering__=(GridFunctionParameters::GridFunctionType)temp;
  subDir.get(temp,"interpolateType__"); interpolateType__=(InterpolateType)temp;
  subDir.get(interpolateOrder__,"interpolateOrder__");
  subDir.get(useGeneralInterpolationFormula__,"useGeneralInterpolationFormula__");


  delete &subDir;
  return 0;
}

//\begin{>>InterpolateParameters.tex}{\subsection{put}}
int InterpolateParameters::
put( GenericDataBase & dir, const aString & name) const
// ===========================================================================
// /Description:
//   Put to a data base file.
//\end{InterpolateRefinementsInclude.tex} 
// ==========================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"InterpolateParameters");                   // create a sub-directory 

  subDir.put("InterpolateParameters","className");

  subDir.put(amrRefinementRatio__,"amrRefinementRatio__");  
  subDir.put(maxRefinementRatio__,"maxRefinementRatio__");
  subDir.put(numberOfDimensions__,"numberOfDimensions__");
  subDir.put((int)gridCentering__,"gridCentering__");
  subDir.put((int)interpolateType__,"interpolateType__");
  subDir.put(interpolateOrder__,"interpolateOrder__");
  subDir.put(useGeneralInterpolationFormula__,"useGeneralInterpolationFormula__");

  delete &subDir;
  return 0;
}

/*********
//Documentation for access functions

//\begin{>>InterpolateParameters.tex}{\subsection{amrRefinementRatio}}
int InterpolateParameters::
amrRefinementRatio (const int axis) const              
//
// /Purpose: return component of  InterpolateParameters::amrRefinementRatio
// 
// /axis: refinement ratio for the {\tt axis} direction will be returned
//\end{InterpolateParameters.tex}

//\begin{>>InterpolateParameters.tex}{\subsection{gridCentering}}
GridFunctionParameters::GridFunctionType InterpolationParameters::
gridCentering () const 
//
// /Purpose: return the gridCentering
// 
//\end{InterpolateParameters.tex}


//\begin{>>InterpolateParameters.tex}{\subsection{useGeneralInterpolationFormula}}
bool InterpolateParameters::
useGeneralInterpolationFormula () const 
//
// /Purpose: return the value of useGeneralInterpolationFormula
// 
//\end{InterpolateParameters.tex}

//\begin{>>InterpolateParameters.tex}{\subsection{interpolateType}}
InterpolateParameters::InterpolateType InterpolateParameters::
interpolateType () const    
//
// /Purpose: return type of interpolation that will be used
//\end{InterpolateParameters.tex}

//\begin{>>InterpolateParameters.tex}{\subsection{interpolateOrder}}
int InterpolateParameters::
interpolateOrder () const
//
// /Purpose: return the order of interpolation that will be used
//\end{InterpolateParameters.tex}

//\begin{>>InterpolateParameters.tex}{\subsection{numberOfDimensions}}
int InterpolateParameters::
numberOfDimensions () const
//
// /Purpose: return value for numberOfDimensions
//\end{InterpolateParameters.tex}

********/
