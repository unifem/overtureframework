#include "TwilightZoneWizard.h"
//==============================================================================
OGFunction*
setTwilightZoneFlowFunction (const int & numberOfDimensions)
		

// /Purpose:
//    returns a pointer to a TwilightZoneFlowFunction; this version
//    new's a TZFunction of type TZType. Also, it interactively 
//    determines which kind of OGFunction to use.
//

//
// /TZType (input): which type of function to set; choices:
//  \begin{itemize}
//    \item{PolyFunction}
//    \item{TrigFunction}
//  \end{itemize}
//
// /numberOfDimensions (input):
// /TZFunction (output) pointer to the setup TwilightZone function
//
//
//  usage:
//     OGFunction& TZFunction = settwilightzoneflowfunction (type, numberOfDimensions);
//      ...
//     if (TZFunction) delete TZFunction;
//
//==============================================================================
{
//  bool set = LogicalFalse;
  
  cout << "TwilightZone Flow Function choices: " << endl;
  cout << "  no OGFunction    0  " << endl;
  cout << "  OGPolyFunction   1  " << endl;
  cout << "  OGTrigFunction   2  " << endl;
  int type = setIntParameter ("Enter choice:");
  TwilightZoneFlowFunctionType TZFtype;
  OGFunction *returnedOGFunction;
  
  switch (type)
  {
  case 0:
    TZFtype = none;
    break;
    
  case 1:
    TZFtype = PolyFunction;
    break;

  case 2:
    TZFtype = TrigFunction;
    break;

  default:
    cout << "TwilightZoneWizard: ERROR, " << type << " is not a valid type " << endl;
    cout << "  Setting type to 'none' " << endl;
    TZFtype = none;
    break;

  };
  
  returnedOGFunction = setTwilightZoneFlowFunction (TZFtype, numberOfDimensions);
  
  return (returnedOGFunction);
  
}



//==============================================================================
OGFunction*
setTwilightZoneFlowFunction (const TwilightZoneFlowFunctionType & TZType, 
			     const int & numberOfDimensions)
		

// /Purpose:
//    returns a pointer to a TwilightZoneFlowFunction; this version
//    new's a TZFunction of type TZType if appropriate.
//
//
// /TZType (input): which type of function to set; choices:
//  \begin{itemize}
//    /item{none}
//    \item{PolyFunction}
//    \item{TrigFunction}
//  \end{itemize}
//
// /numberOfDimensions (input):
// /twilightZoneFlowFunction (output) pointer to the setup TwilightZone function
//
//
//  usage:
//     twilightZoneFlowFunction = settwilightzoneflowfunction (type, numberofdimensions);
//      ...
//     if (twilightZoneFlowFunction) delete twilightZoneFlowFunction;
//
//==============================================================================
{
//  bool TZfuncNewed = LogicalFalse;
  
  OGFunction* twilightZoneFlowFunction= NULL;
  int degreeOfSpacePolynomial = 2;
  int numberOfComponents = numberOfDimensions+3;
  int degreeOfTimePolynomial = 0;
  real fx0 = 2.;

  real fy0 = 2.;
  real fz0 = 0.;
  real ft0 = 0.;

  RealArray a, c; //polynomial coefficients
  RealArray fx,fy,fz,ft; //trig coefficients
  bool setPolyCoeffs = FALSE;
  bool setTrigCoeffs = FALSE;

  NameList nl;
  aString answer(80), name(80);

  Index allComponents(0,numberOfComponents);
  real aValue, cValue, fxValue, fyValue, fzValue, ftValue;
  int i,j,k,m,n;

  switch (TZType)
  {
    
    // ====================
    case none:
    // ====================
    break;

    // ====================
    case PolyFunction:
    // ====================
    printf (

      " Set TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Polynomial Parameters: \n"
      "\n"
      "degreeOfSpacePolynomial [sp]   (int)   %4i \n"
      "degreeOfTimePolynomial  [tp]   (int)   %4i \n"
      "[set]PolyCoeffs                (bool)  %4i \n"
      "\n"
      "================================================================================\n\n",

      degreeOfSpacePolynomial,
      degreeOfTimePolynomial,
      setPolyCoeffs
      );
  
    for (;;)
    {
      cout << "Enter changes to variables, exit to continue" << endl;
      cin >> answer;
    
      if (answer == "exit" || answer(0,0) == "x") break;
    
      nl.getVariableName (answer, name);

      if ( name(0,1) == "sp" )
      {
	degreeOfSpacePolynomial = nl.intValue(answer);
	cout << "degreeOfSpacePolynomial = " << degreeOfSpacePolynomial << endl;
      }
    
      else if ( name(0,1) == "tp" )
      {
	degreeOfTimePolynomial = nl.intValue(answer);
	cout << "degreeOfTimePolynomial = " << degreeOfTimePolynomial << endl;
      }

      else if ( name(0,2) == "set" )
      {
	setPolyCoeffs = nl.intValue(answer);
	cout << "setPolyCoeffs = " << setPolyCoeffs << endl;
      }
      else
	 cout << "unknown response [" << name << "]" << endl;
    }

    printf (

      "  Final Values for TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Polynomial Parameters: \n"
      "\n"
      "degreeOfSpacePolynomial [sp]   (int)   %4i \n"
      "degreeOfTimePolynomial  [tp]   (int)   %4i \n"
      "setPolyCoeffs                (bool)  %4i \n"
      "\n"
      "================================================================================\n\n",

      degreeOfSpacePolynomial,
      degreeOfTimePolynomial,
      setPolyCoeffs
      );


    if (setPolyCoeffs)
    {
    
      a.resize(5,numberOfComponents);
      c.resize(5,5,5,numberOfComponents);
      a = 0;
      a(0,allComponents) = 1.;
      c = 0;
    
      for (;;)
      {
	cout << "\n==============================================================================\n" <<
	  "   Enter i,j,k,n,c(i,j,k,n)   for c(i,j,k,n)x^i*y^j*z^k, component n, i<0 to exit: " << endl;
      

	cin >> i >> j >> k >> n >> cValue;
	if (i<0) break;
      
	c(i,j,k,n) = cValue;
      }

      for (;;)
      {
	cout << "\n==============================================================================\n" <<
	  "   Enter m,n,a(m,n) for a(m,n)*t^m, component n, m<0 to exit: " << endl;
      
	cin >> m >> n >> aValue;
	if (m<0) break;
      
	a(m,n) = aValue;
      }
    }

    if (setPolyCoeffs)
    {
      c.display("Here is c");
      a.display("Here is a");
      
      twilightZoneFlowFunction = new OGPolyFunction;
      ((OGPolyFunction*)twilightZoneFlowFunction)->setCoefficients (c,a);
      // TZfuncNewed = LogicalTrue;
    }
    else
    {
      twilightZoneFlowFunction = 
	new OGPolyFunction (degreeOfSpacePolynomial,
			    numberOfDimensions, 
			    numberOfComponents, 
			    degreeOfTimePolynomial);
      // TZfuncNewed = LogicalTrue;
    }
    
    
    
    break;
    
    // ====================
    case TrigFunction:
    // ====================
    printf (

      " Set TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Trig Parameters: \n"
      "\n"
      "  fx0                          (real)  %4e \n"
      "  fy0                          (real)  %4e \n"
      "  fz0                          (real)  %4e \n"
      "  ft0                          (real)  %4e \n"
      "[set]TrigCoeffs                (bool)  %4i \n"
      "================================================================================\n\n",

      fx0,
      fy0,
      fz0,
      ft0,
      setTrigCoeffs
      );
  
    for (;;)
    {
      cout << "Enter changes to variables, exit to continue" << endl;
      cin >> answer;
    
      if (answer == "exit" || answer(0,0) == "x") break;
    
      nl.getVariableName (answer, name);


      if ( name(0,3) = "setT" )
      {
	setTrigCoeffs = nl.intValue(answer);
	cout << "setTrigCoeffs = " << setTrigCoeffs << endl;
      }
    
      else if ( name(0,2) == "fx0" )
      {
	fx0 = nl.realValue(answer);
	cout << "fx0 = " << fx0 << endl;
      }
      else if ( name(0,2) == "fy0" )
      {
	fy0 = nl.realValue(answer);
	cout << "fy0 = " << fy0 << endl;
      }
      else if ( name(0,2) == "fz0" )
      {
	fz0 = nl.realValue(answer);
	cout << "fz0 = " << fz0 << endl;
      }
      else if ( name(0,2) == "ft0" )
      {
	ft0 = nl.realValue (answer);
	cout << "ft0 = " << ft0 << endl;
      }
            
      else
	 cout << "unknown response [" << name << "]" << endl;
    }

    printf (

      "  Final Values for TwilightZone Parameters: \n"
      "================================================================================\n"
      "   NAME                        type    current value \n"
      "Trig Parameters: \n"
      "\n"
      "  fx0                          (real)  %4e \n"
      "  fy0                          (real)  %4e \n"
      "  fz0                          (real)  %4e \n"
      "  ft0                          (real)  %4e \n"
      "  setTrigCoeffs                (int)   %4i \n"
      "================================================================================\n\n",

      fx0,
      fy0,
      fz0,
      ft0,
      setTrigCoeffs
      );

    if (setTrigCoeffs)
    {
      fx.resize(numberOfComponents);
      fy.resize(numberOfComponents);
      fz.resize(numberOfComponents);
      ft.resize(numberOfComponents);

      fx(allComponents) = (real) 0.0;
      fy(allComponents) = (real) 0.0;
      fz(allComponents) = (real) 0.0;
      ft(allComponents) = (real) 0.0;

      fx.display("Here is fx");
      fy.display("Here is fy");
      fz.display("Here is fz");
      ft.display("Here is ft");
        
      for (;;)
      {
      
	cout << "\n==============================================================================\n" <<
	  "     For cos(fx(n)*Pi*x) * cos(fy(n)*Pi*y) * cos(fz(n)*Pi*z) * cos(ft(n)*Pi*t \n" <<
	  "   Enter n,fx(n),fy(n),fz(n),ft(n) for component n, n=-1 to exit   " << endl;
      
	cin >> n >> fxValue >> fyValue >> fzValue >> ftValue;
	if (n<0) break;
       
	fx(n) = fxValue;
	if (numberOfDimensions>1) fy(n) = fyValue;
	if (numberOfDimensions>2) fz(n) = fzValue;
	ft(n) = ftValue;
	fx.display("Here is fx");
	if (numberOfDimensions>1) fy.display("Here is fy");
	if (numberOfDimensions>2) fz.display("Here is fz");
	ft.display("Here is ft");

      }
    }
  
    if (setTrigCoeffs)
    {
      fx.display("Here is fx");
      if (numberOfDimensions>1) fy.display("Here is fy");
      if (numberOfDimensions>2) fz.display("Here is fz");
      ft.display("Here is ft");
      
      twilightZoneFlowFunction = new OGTrigFunction (fx, fy, fz, ft);
      // TZfuncNewed = LogicalTrue;
      
    
    }
    else
    {
    
      twilightZoneFlowFunction = new OGTrigFunction (fx0, fy0, fz0, ft0, numberOfComponents);
      // TZfuncNewed = LogicalTrue;
    }
  
    break;
    
    default:
      cout << "TwilightZoneWizard: something wrong here" << endl;
      
    break;
  }; 


  return (twilightZoneFlowFunction);
  
}
