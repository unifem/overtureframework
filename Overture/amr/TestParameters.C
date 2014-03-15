#include "TestParameters.h"

//\begin{>TestParameters.tex}{\subsection{ TestParameters default constructor}}
TestParameters::
TestParameters(const int numberOfDimensions_ // = InterpolateParameters::defaultNumberOfDimensions
  )
//
// /Purpose: default constructor for the TestParameters container class;
//           initialize the class and set default values
//
//\end{TestParameters.tex}
{
  int i;
  
  numberOfDimensions = numberOfDimensions_;
  debug = LogicalFalse;
  plotting = LogicalFalse;

  
  amrRefinementRatio.resize(3);
  intArray nullArray;


  if (numberOfDimensions<3)
  {
    for (i=numberOfDimensions; i<3; i++) amrRefinementRatio(i) = 1;
    for (i=0; i<numberOfDimensions; i++) amrRefinementRatio(i) = InterpolateParameters::defaultAmrRefinementRatio;
  }
  else
  {
    for (i=0; i<numberOfDimensions; i++) amrRefinementRatio(i) = 2;
  }
  
  interpolateType                = InterpolateParameters::defaultInterpolateType;
  interpolateOrder               = InterpolateParameters::defaultInterpolateOrder;
  gridCentering                  = InterpolateParameters::defaultGridCentering;
  useGeneralInterpolationFormula = LogicalFalse;
  tzType                         = TrigFunction;

  TZFunctionTypeString[0] = "none";
  TZFunctionTypeString[1] = "PolyFunction";
  TZFunctionTypeString[2] = "TrigFunction";

//   InterpolateOffsetDirectionString[0] = "offsetInterpolateToLeft";
//   InterpolateOffsetDirectionString[1] = "offsetInterpolateToRight";

//   interpolateOffsetDirection.resize(3);
//   for (i=0; i<3; i++) interpolateOffsetDirection(i) = (int) InterpolateParameters::offsetInterpolateToLeft;

}

//\begin{>>TestParameters.tex}{\subsection{ TestParameters destructor}}
TestParameters::
~TestParameters()
//
// /Purpose: destructor for the TestParameters container class
//
//\end{TestParameters.tex}
{
  cout << "TestParameters destructor called" << endl;
}



//\begin{>>TestParameters.tex}{\subsection{interactivelySetParameters}}
int TestParameters::
interactivelySetParameters ()
//
// /Purpose: interactively set TestParameters parameters using NameList
//
//\end{TestParameters.tex}
{
  aString answer(80), name(80);
  bool error;
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

    else if ( name(0,1) == "pl")
    {
      plotting = nl.intValue(answer);
      cout << "plotting = " << plotting << endl;
    }

    else if ( name(0,2) == "amr" )
    {
      nl.getIntArray (answer, amrRefinementRatio, i);
      cout << "amrRefinementRatio(" << i << ") = " << amrRefinementRatio(i) << endl;
    }
    
    else if (name(0,1) == "nd" || name(0,8) == "numberOfD")
    {
      numberOfDimensions = nl.intValue(answer);
      cout << "numberOfDimensions = " << numberOfDimensions << endl;
    }

    else if ( name(0,2) == "gri" )
    {
      gridCentering = (GridFunctionParameters::GridFunctionType) nl.intValue(answer);
      cout << "gridCentering = " << gridCentering << endl;
    }

    else if (name(0,1) == "it" || name(0,11) == "interpolateT")
    {
      interpolateType = (InterpolateParameters::InterpolateType) nl.intValue(answer);
      cout << "interpolateType = " << interpolateType << endl;
    }

    else if (name(0,2) == "ord" || name(0,11) == "interpolateO")
    {
      interpolateOrder = nl.intValue(answer);
      cout << "interpolateOrder = " << interpolateOrder << endl;
    }


    else if (name(0,3) == "useG" )
    {
      useGeneralInterpolationFormula = (bool) nl.intValue(answer);
      cout << "useGeneralInterpolationFormula = " << useGeneralInterpolationFormula << endl;
    }

    else if (name(0,2) == "tzt" || name(0,2) == "tzT")
    {
      tzType = (TwilightZoneFlowFunctionType) nl.intValue(answer);
      cout << "tzType = " << tzType << " " << TZFunctionTypeString[tzType] << endl;
    }

//     else if (name(0,2) == "iod")
//     {
//       nl.getIntArray (answer, interpolateOffsetDirection, i);
//       cout << "interpolateOffsetDirection(" << i << ") = " << interpolateOffsetDirection(i) << " "
// 	   << InterpolateOffsetDirectionString[interpolateOffsetDirection(i)] << endl;
//     }
                    
    else
      cout << "unknown response [" << name << "]" << endl;
  }
  return 0;

}

void TestParameters::
display () const
//
// /purpose: display values of all the parameters
//
{
  cout << "==================================================" << endl;
  cout << "                                          " << endl;
  cout << "debug                                     " << debug << endl;
  cout << "[pl]otting                                " << plotting << endl;
  cout << "[amr]RefinementRatio                      " << amrRefinementRatio(0) 
                                                  <<","<< amrRefinementRatio(1)
                                                  <<","<< amrRefinementRatio(2) << endl;
  cout << "numberOfDimensions [nd]                   " << numberOfDimensions << endl;
  cout << "[gri]dCentering                           " << gridCentering << endl;
  cout << "[interpolateT]ype  [it]                   " << interpolateType << endl;
  cout << "[interpolateO]rder  [ord]                  " << interpolateOrder << endl;
  cout << "[useG]eneralInterpolationFormula          " << useGeneralInterpolationFormula << endl;
  cout << "twilightZoneFlowFunctionType [tzt]        " << tzType << " " << TZFunctionTypeString[tzType] << endl;
//cout << "interpolateOffsetDirection [iod]          " 
//        << interpolateOffsetDirection(0) << " " << InterpolateOffsetDirectionString[(int)interpolateOffsetDirection(0)] << ","
//        << interpolateOffsetDirection(1) << " " << InterpolateOffsetDirectionString[(int)interpolateOffsetDirection(1)] << ","
//        << interpolateOffsetDirection(2) << " " << InterpolateOffsetDirectionString[(int)interpolateOffsetDirection(2)] << endl;
    cout << "                                          " << endl;
  cout << "==================================================" << endl;
}

