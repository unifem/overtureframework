// Stencil.C

/*
//===========================================================================
This class represents a stencil.
It can act on mappedgridfunctions and can be used to update a coefficient 
matrix


WTB: krister@tdb.uu.se

//==========================================================================
*/
#include <assert.h>

#include "Stencil.h"

// int Stencil::debug = 1;
int Stencil::debug = 2;

// static Index all; // We use nullIndex instead of all

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Constructors and assignment}}
Stencil::
Stencil()
//==========================================================================
// /Description:
//   Construct an (empty) Stencil
// /Author: K\AA
//\end{StencilPublic.tex}
{
  if ( debug & 1 )
    cout << "Stencil :)" << endl;

  nrOfWeights = 0;
  width.redim(3,2); width = 0; 
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
Stencil::
Stencil(RealArray weights_, IntegerArray offsets_)
//==========================================================================
// /Description:
//   Construct a Stencil according to {\ff weights\_} and {\ff offsets\_}
//  /weights\_ (input): Use these weights (nx1) 
//  /offsets\_ (input): For these offsets (nx3) 
//  /Author: K\AA
//\end{StencilPublic.tex}
{
  if ( debug & 1 )
    cout << "Stencil(realArray weights_, IntegerArray offsets_) :)" << endl;

  int n = weights_.getBound(axis1);
  if( (weights_.getBase(axis1) != 0) || 
      (offsets_.getBase(axis1) != 0) ||
      (offsets_.getBase(axis2) != 0) ||
      (offsets_.getBound(axis1) != n) ||
      (offsets_.getBound(axis2) != 2) )
    {
      cout <<"Error!!!  Stencil::Stencil(realArray weights_, IntegerArray offsets_)"
	   << " got wrongly sized matrixes!" << endl;
      cout << "weight_ shall be (0:n) and offsets_ shall be (0:n,0:2)" << endl;
      weights_.display("This is weight");
      offsets_.display("This is offsets");
      exit(1);
    }
  
  nrOfWeights = n+1;
  weights = weights_;
  offsets = offsets_;
  width.redim(3,2); 
  updateWidth();
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
Stencil::Stencil(const Stencil &s)
//==========================================================================
// /Description:
//   Copy constructor
//  /s (input): Copy this stencil. 
// /Author: K\AA
//\end{StencilPublic.tex}
{
  if ( debug & 1 )
    cout << "Stencil copy constructor :)" << endl;

  nrOfWeights = s.nrOfWeights;
  width = s.width;
  weights = s.weights;
  offsets = s.offsets;

}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
Stencil::Stencil(const predefinedStencils &choice)
//==========================================================================
// /Description:
//   Construct a stencil according to choice.
//  /choice (input): Make this stencil (see above for different choices). 
// /Author: K\AA
//\end{StencilPublic.tex}
{
  // A way of conveniently making simple stencils
  if ( debug & 1 )
    cout << "Stencil copy constructor :)" << endl;

     
  RealArray weigh; 
  IntegerArray offs;
 
  switch(choice)
    {
    case identity :
      weigh.redim(1); weigh = 1.0;
      offs.redim(1,3); offs = 0;
      break;
    case displacex  :
      weigh.redim(1); weigh = 1.0;
      offs.redim(1,3); offs = 0;
      offs(0,0) = 1; // offs(0,nullIndex) = (1,0,0)
      break;
    case inversedisplacex :
      weigh.redim(1); weigh = 1.0;
      offs.redim(1,3); offs = 0;
      offs(0,0) = -1; // offs(0,nullIndex) = (-1,0,0)
      break;

    case dplusx :
      weigh.redim(2); weigh(0) = -1; weigh(1) = 1;
      offs.redim(2,3); offs=0;
      offs(1,0) = 1; //  offs(1,nullIndex) = (1,0,0)
      break;
    case dminusx :
      weigh.redim(2); weigh(0) = -1; weigh(1) = 1;
      offs.redim(2,3); offs=0;
      offs(0,0) = -1; //  offs(0,nullIndex) = (-1,0,0)
      break;
    case dzerox :
    case dnullx :
      weigh.redim(2); weigh(0) = -0.5; weigh(1) = 0.5;
      offs.redim(2,3); offs=0;
      offs(0,0) = -1; //  offs(0,nullIndex) = (-1,0,0)
      offs(1,0) =  1; //  offs(1,nullIndex) = ( 1,0,0)
      break;
 
   case dplusxdminusx :
      weigh.redim(3); weigh(0) = 1; weigh(1) = -2; weigh(2) = 1;
      offs.redim(3,3); offs=0;
      offs(0,0) = -1; //  offs(0,nullIndex) = (-1,0,0)
      offs(2,0) =  1; //  offs(2,nullIndex) = ( 1,0,0)
      break;

    default:
      cout << "Error!!! Stencil::Stencil(const predefinedStencils &choice)"
	   << "has got unknown choice" << endl;
      cout << "Choice = " << choice << endl;
      exit(1);
      break;
    }

  Stencil s(weigh,offs); // Call this constructor and copy s
 
  nrOfWeights = s.nrOfWeights;
  width = s.width;
  weights = s.weights;
  offsets = s.offsets;
   
}

//==========================================================================
Stencil::~Stencil()
// /Description:
//   Destructor
// /Author: K\AA
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil Destructor"<< endl;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
Stencil &
Stencil::operator= (const Stencil &s)
// /Description:
// Deep copy assignment 
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil &operator= (const Stencil &s) :)\n";

  nrOfWeights = s.nrOfWeights;
  width = s.width;
  if(nrOfWeights>0) // Only assign these arrays if they exist
    {
      weights.redim(s.weights);
      offsets.redim(s.offsets);
      weights = s.weights;
      offsets = s.offsets;
    }

  return *this;
}


//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Operator()}}
Stencil 
Stencil::operator() (const Stencil &s) const
// /Description:
// Returns Stencil corresponding to $D(s)$, where $D$ is this Stencil.
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::operator() (const Stencil &s)\n";

  Stencil res;
  int i,j;
  for(i = 0;  i< nrOfWeights; i++)
      for(j = 0;  j< s.nrOfWeights; j++)
	res.addWeight( weights(i)*s.weights(j),offsets(i,nullIndex) + s. offsets(j,nullIndex));

  return res;
}
//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Addition and subtraction}}
//\no function header:
// \begin{flushleft} \textbf{ Stencil Stencil::operator\textit{X}(const Stencil \&s)}\end{flushleft}
// /Description:
// Stencils can be added or subtracted. \textit{X} is  ``+'' or ``-''. 
// Unary minus and compound expressions like ``+='' or ``-='' exist, too.
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================

Stencil Stencil::operator+(const Stencil &s) const
{
  if ( debug & 1 )
    cout << "Stencil::operator+(Stencil s) const " << endl;
  int i;
  Stencil res(s); // Set result to equal s

  for (i = 0;  i< nrOfWeights; i++)
    res.addWeight(weights(i),offsets(i,nullIndex));
  return res;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Scaling}}
//\no function header:
// \begin{flushleft} \textbf{ Stencil Stencil::operator\textit{X}(Real d)}\end{flushleft}
// /Description:
// Stencils can be scaled. \textit{X} is  ``*'' or ``/''. 
// Compound expressions like ``*='' or ``/='' exist, too.
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
Stencil Stencil::operator*(Real d) const
{
  if ( debug & 1 )
    cout << "Stencil::operator*(double d) const " << endl;

  Stencil res(*this);
  res.weights *= d;

  return res;
}

Stencil Stencil::operator-(const Stencil &s) const
{
  if ( debug & 1 )
    cout << "Stencil::operator-(Stencil s) const " << endl;
  int i;
  Stencil res(-s); // Set result to equal -s

  for (i = 0;  i< nrOfWeights; i++)
    res.addWeight(weights(i),offsets(i,nullIndex));
  return res;
}

Stencil Stencil::operator-() const
{
  if ( debug & 1 )
    cout << "Stencil::operator-() const " << endl;

  Stencil res(*this);
  res.weights = -res.weights ; // negate weights

  return res;
}

Stencil Stencil::operator/(Real d) const
{
  if ( debug & 1 )
    cout << "Stencil::operator*(double d) const " << endl;

  Stencil res(*this);
  res.weights /= d;

  return res;
}

void Stencil::operator+= (const Stencil &s)
{
  if ( debug & 1 )
    cout << "Stencil::operator+=(Stencil s) " << endl;
  int i;

  for (i = 0;  i< s.nrOfWeights; i++)
    addWeight(s.weights(i),s.offsets(i,nullIndex));
}

void Stencil::operator-= (const Stencil &s)
{
  if ( debug & 1 )
    cout << "Stencil::operator-=(Stencil s) " << endl;
  int i;

  for (i = 0;  i< s.nrOfWeights; i++)
    addWeight(-s.weights(i),s.offsets(i,nullIndex));
}

void Stencil::operator*= (Real d)
{
  if ( debug & 1 )
    cout << "Stencil::operator*= (Real d)" << endl;

  weights *= d;
}

void Stencil::operator/= (Real d)
{
  if ( debug & 1 )
    cout << "Stencil::operator/= (Real d)" << endl;

  weights /= d;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Rotation}}
Stencil 
Stencil::rotate(int axis,/* = axis3 */ 
	        int step /* = 1 */ ) const 
// /Description:
// Returns the stencil you get if you rotate ``this'' stencil {\ff step} quarters of a circle counterclockwise, seen from axis {\ff axis}.
// It is only meaningful for stencils on grid with the same spacing in different dimensions.
// The default parameters obtain a stencil for second dimension given a stencil in first dimension
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::rotate() steps =" <<step%4<< endl;
  // Be sure step > 0
  while(step<0) 
    { step += 4; } 

  Stencil res;
  int x = (axis+1)%3;  // if axis == axis3, (== 2), then x becomes axis1 and y axis2.
  int y = (axis+2)%3;  // Otherwise, permute cyclic.
  int st;
  int i; 
  IntegerArray newOffsets = offsets;
  IntegerArray tmp(nrOfWeights);
  // Rotate the new offsets, do  anticlockwise 90 degrees rotation 
  // from z-view step%4 times 
  for (st =0; st< step%4; st++)
    {
      tmp = newOffsets(nullIndex,x); 
      newOffsets(nullIndex,x) = - newOffsets(nullIndex,y);
      newOffsets(nullIndex,y) = tmp;
    }

  for (i = 0;  i< nrOfWeights; i++)
    { 
      res.addWeight(weights(i),newOffsets(i,nullIndex));
    }

  return res;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{addWeight}}
void 
Stencil::addWeight(Real d,const Point &offs_)
// /Description:
// Adds weight {\ff d} to offset {\ff offs\_ }.
// Mostly used by the internal operators, but can be used to build a stencil from scratch.
// 
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  Point offs;   // A++ copy constructor cannot make an array from a view 
  offs = offs_; // To fix this we use assignment
  resizePoint(offs); // offs becomes 1x3
  if ( debug & 1 )
    cout << "Stencil::addWeights() " << endl;
  int i;
  for (i = 0;  i< nrOfWeights; i++) // search existing offsets.
    if ( (offs(0,0) == offsets(i,0)) && (offs(0,1) == offsets(i,1))
	 && (offs(0,2) == offsets(i,2)) )
      {
	weights(i) += d;  //  offsets exists, Add to coeff.
	return;
      }
  weights.resize(nrOfWeights+1); // resize preserves earlier data
  offsets.resize(nrOfWeights+1,3);  // resize preserves earlier data
  weights(nrOfWeights) = d;
  offsets(nrOfWeights,nullIndex) = offs;
  nrOfWeights++;
  // if necessary, update width
  for( i=0; i<3; i++)
    {
      if( offs(0,i) < width(i,Start) )
	width(i,Start) =  offs(0,i);
      if( offs(0,i) > width(i,End) )
	width(i,End) =  offs(0,i);
    }
}


//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Display methods}}
void 
Stencil::display(const aString &str, displayWhat what, ostream &os) const
// /Description:
// Displays information. {\ff displayWhat} is the following enum:
// {\footnotesize
// \begin{verbatim}
//  enum displayWhat { text=1, structure, both };
// \end{verbatim}
// }
// and lets the user decide if structure, text or both shall be displayed.
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::display(ostream &) what == " << what << endl;

  os << "\nStencil::display() --- " << str << endl;

  if(what & text)
    displayText("",os);

  if(what & structure)
    displayStructure("",os);
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
void 
Stencil::displayText(const aString &str, ostream &os) const
// /Description:
// Displays relevant data, weights, offsets, width, and number of weights.
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  os << "\nStencil::displayText() --- " << str << endl;

  os << "\nNumber of coefficients: " << nrOfWeights << endl;
  
  width.display("The width of the stencil");
  offsets.display("The offsets of the ...");
  weights.display("... weights of the stencil");
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
void 
Stencil::displayStructure(const aString &str,ostream &os, int legend ) const
// /Description:
// Displays the structure as an ascii-table. If {\ff legend} == TRUE,
// a legend explaining the notation is shown. 
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  // Ascii-illustrate the structure
  os <<"\nStencil::displayStructure() --- " << str << endl; 

  if( legend )
    {
      os <<"The structure as an ascii-display. Legend:" << endl;
      os <<"   * : Point (0,0) with value" << endl;
      os <<"   o : Point (0,0) without value" << endl;
      os <<"   + : A Point with value" << endl;
      os <<"   . : A Point without value" << endl;
      os <<"The structure is illustrated in each plane orthogonal to axis3" 
	 << endl;
    }

  Point p(3);
  for(int k=width(axis3,Start); k<=width(axis3,End); k++)
    {
      os << "Structure for axis3 == " << k << endl;
      for(int j=width(axis2,End);j>=width(axis2,Start);j--) {
	os << "\t";
	for(int i=width(axis1,Start); i<=width(axis1,End);i++)
	  {
	    p(0)=i; p(1)=j; p(2)=k;
	    if ( offsetExist(p))
	      if (i==0 && j==0)
		os << '*';
	      else
		os << '+';
	    else
	      if (i==0 && j==0)
		os << 'o';
	      else
		os << '.';
	  }
	os << endl ;
      }
    }
  os << endl ;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Applying Stencils for forward derivatives}}
RealMappedGridFunction &
Stencil::applyStencil(const RealMappedGridFunction &sourceGF, 
		      RealMappedGridFunction &targetGF, 
		      const Index &I1, 
		      const Index &I2, 
		      const Index &I3, 
		      const Index &N) const
// /Description:
// Applies the {\ff \ST } according to the indexes. 
// Previous values in {\ff targetGF} is overwritten.
// 
// Returns reference to targetGF to enhance efficient calls
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    {      
      cout << "Stencil::applyStencil()" << endl;
      cout << "********   Check of Indexes: "<<endl;
      I1.display("I1");
      I2.display("I2");
      I3.display("I3");
      N.display("N");
    }

  // Use efficient syntax up to some number of weights
  switch(nrOfWeights) 
    {
    case 1 :  
      targetGF(I1,I2,I3,N) = 
	weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N);
      break;
    case 2 :
      targetGF(I1,I2,I3,N) = 
	weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N) +
	weights(1)*sourceGF(I1+offsets(1,0),I2+offsets(1,1),I3+offsets(1,2),N);
      break;
    case 3 :
      targetGF(I1,I2,I3,N) = 
	weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N) +
	weights(1)*sourceGF(I1+offsets(1,0),I2+offsets(1,1),I3+offsets(1,2),N) +
	weights(2)*sourceGF(I1+offsets(2,0),I2+offsets(2,1),I3+offsets(2,2),N);
      break;
    default :
      targetGF(I1,I2,I3,N) = 0;
      int i;
      for (i = 0;  i< nrOfWeights; i++)
	{
	  targetGF(I1,I2,I3,N) += 
	    weights(i)*sourceGF(I1+offsets(i,0),I2+offsets(i,1),I3+offsets(i,2),N);
	}
    }
  return targetGF; // Makes a nicer syntax possible 
  
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
RealArray &
Stencil::applyStencil(const RealArray  &sourceGF, 
	       RealArray  &targetGF, 
	       const Index &I1, 
	       const Index &I2, 
	       const Index &I3, 
	       const Index &N) const  
// /Description:
// Applies the {\ff \ST } according to the indexes. 
// Previous values in {\ff targetGF} is overwritten.
// 
// Returns reference to targetGF to enhance efficient calls
// (This signature is required by \MGO ::ComputeDerivatives)
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    {      
      cout << "Stencil::applyStencil()" << endl;
      cout << "********   Check of Indexes: "<<endl;
      I1.display("I1");
      I2.display("I2");
      I3.display("I3");
      N.display("N");
    }

  // Use efficient syntax up to some number of weights.
  const int someNumber = 3; //## Change for a higher " some number"
  switch(nrOfWeights) 
    {
    case 1 :  
      targetGF(I1,I2,I3,N) =
	  weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N);
      break;
    case 2 :
      targetGF(I1,I2,I3,N) = 
	weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N) +
	weights(1)*sourceGF(I1+offsets(1,0),I2+offsets(1,1),I3+offsets(1,2),N);
      break;
      //## Insert here for a higher " some number"      
    case someNumber :
      targetGF(I1,I2,I3,N) = 
	weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N) +
	weights(1)*sourceGF(I1+offsets(1,0),I2+offsets(1,1),I3+offsets(1,2),N) +
	weights(2)*sourceGF(I1+offsets(2,0),I2+offsets(2,1),I3+offsets(2,2),N);
      //## Update here for a higher " some number"      
      break;
    default : // nrOfWeights >= 4
      targetGF(I1,I2,I3,N) = 
	weights(0)*sourceGF(I1+offsets(0,0),I2+offsets(0,1),I3+offsets(0,2),N) +
	weights(1)*sourceGF(I1+offsets(1,0),I2+offsets(1,1),I3+offsets(1,2),N) +
	weights(2)*sourceGF(I1+offsets(2,0),I2+offsets(2,1),I3+offsets(2,2),N) +
	weights(3)*sourceGF(I1+offsets(3,0),I2+offsets(3,1),I3+offsets(3,2),N);
    }

  // if more weights remain, add these (one at a time, could be more efficient)
  for (int i = someNumber+2;  i< nrOfWeights; i++)
    {
      targetGF(I1,I2,I3,N) += 
	weights(i)*sourceGF(I1+offsets(i,0),I2+offsets(i,1),I3+offsets(i,2),N);
    }
  return targetGF; // Makes a nicer syntax sometimes possible 
  
}

// include for next method
#include "SparseRep.h"

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Obtaining coefficient matrixes for backward derivatives}}
void 
Stencil::makeCoefficients(CoefficientMatrix &coeff, 
		const Index &I1, 
		const Index &I2, 
		const Index &I3, 
		const int iE  /* = 0 */,
		const int iC  /* = 0 */
		) const
// /Description:
// \textit{Note: Interface likely to change} \\
// Methods dealing with creation of a coefficient matrix.
// Adds weights to coeff (that is, it does not nullify)
// /coeff (input/output): an initialized coefficient matrix
// /I1 etc (input): Indexes (for now, asserts != nullIndex)
// /iE (input): Equation number (for systems)
// /iC (input): Component number (for systems)
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::updateMatrix()" << endl;

  assert(coeff.sparse!=NULL);
  assert( I1.length() && I2.length() && I3.length() );

  int spaceDim;
  int stencilSize;

  //====== Estimate spaceDim and stencilsize  =======
  if (I3.length() > 1 || I3.getBase() != 0 )
    spaceDim = 3;
  else if (I2.length() > 1 || I2.getBase() != 0 )
    spaceDim = 2;
  else 
    spaceDim = 1;

  // Take the Stencil structure into account
  if( spaceDim < 3 && (width(axis3, Start) || width(axis3, End)) )
    spaceDim = 3;
  else if( spaceDim < 2 && (width(axis2, Start) || width(axis2, End)) )
    spaceDim = 2;
  
  // Change spaceDim if ok
  assert(spaceDim <= coeff.mappedGrid->numberOfDimensions() );
  spaceDim = coeff.mappedGrid->numberOfDimensions();

  //$$$int halfWidth = max( abs(width) );
  //$$$int stencilSizeInOneDimension = 2*halfWidth + 1;  

  // Get the total stencil size in one component
  stencilSize = coeff.sparse->stencilSize;

  if( debug & 2)
    {
      cout << "stencilSize = " << stencilSize << endl;
      cout << "spaceDim = " << spaceDim << endl;
    }

  //======= Convert the weights to fit with Overture in one grid point ======  

  int totalStencilSize = stencilSize * SQR( coeff.sparse->numberOfComponents );
  RealArray weightsForOverture(totalStencilSize); weightsForOverture = 0;

  if ( debug & 2 )
    {
      cout << "Check equationNumber for base point" << endl
	   << "iE = " << iE << endl;
      cout << "BasePoint = " << I1.getBase() 
	   << " " << I2.getBase()
	   << " " << I3.getBase() << endl;
      cout << "BasePoint to equationNumber " 
	   << coeff.sparse->indexToEquation( iE, I1.getBase(),
					     I2.getBase(),I3.getBase() )
	   << endl;
    }

  // fill in weights and offsets for overture 
  Index S(0,stencilSize); 
  int base = stencilSize*(iC+coeff.sparse->numberOfComponents*iE); 
  weightsForOverture( S+base ) = weights(S);
  for(int iS = 0; iS<stencilSize; iS++)
    {
      coeff.sparse->setCoefficientIndex
	(iS + base,
	 iE, I1, I2, I3,
	 iC, I1 + offsets(iS,0), I2 + offsets(iS,1), I3 + offsets(iS,2) );
    }

   
  if( debug & 2)
    {
      weightsForOverture.display("This is weightsForOverture");
    }

  //======= Insert the weights into the coefficient matrix  
/* --- wdh : 
  for(int i = I1.getBase(); i<= I1.getBound(); i++)        
    for(int j = I2.getBase(); j<= I2.getBound(); j++)
      for(int k = I3.getBase(); k<= I3.getBound(); k++)
	coeff(nullIndex,i,j,k) += weightsForOverture; // Note: - we add weights
--- */
// *wdh : change to work in parallel, also more efficient this way
  for( int m=coeff.getBase(0), n=weightsForOverture.getBase(0); n<=weightsForOverture.getBound(0); n++,m++ )
    coeff(m,I1,I2,I3) += weightsForOverture(n); // Note: - we add weights

}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
void 
Stencil::makeCoefficients(CoefficientMatrix &coeff, 
		 const Index &I1, 
		 const Index &I2, 
		 const Index &I3, 
		 const Index &C  
		 ) const
// /Description:
// \textit{Note: Interface likely to change} \\
// For systems, this method returns the coefficient matrix corresponding to this {\ff \ST} applied
// for each equation $i$ in {\ff C} on component $i$ of a grid function.
// This method only calls the previous.
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::makeCoefficients ( ... Index C)" << endl;

  for(int i = C.getBase(); i <= C.getBound(); i += C.getStride() )
    makeCoefficients( coeff, I1, I2, I3, i, i);

}




//==========================================================================
//\begin{>>StencilPublic.tex}{}
void 
Stencil::conformOffsets(IntegerArray newOffsets)
// /Description:
// conformOffsets allows the user to force the ordering of the offsets.
// Its main purpose is to generate conform coefficient matrixes
// so that the A++ operator+ works for coefficient matrixes obtained from Stencils.
// /newOffsets (input): (nx3) array which contains the new ordering. Must contain existing offsets.
//
// /Author: K\AA
// 
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::conformOffsets(IntegerArray newOffsets)" << endl;

  int n = newOffsets.getBound(axis1) + 1; // number of new weights
 
  RealArray newWeights(n); newWeights = 0; // Will become new weights.
  Stencil newStencil(newWeights,newOffsets);

  int j; // index for where the weight shall be
  for(int i = 0; i<nrOfWeights; i++)
    {
      if( newStencil.offsetExist( offsets(i, nullIndex) , j) )
	newStencil.addWeight( weights(i),newOffsets(j, nullIndex) );
      else
	{
	  cout <<"ERROR:Stencil::conformOffsets got an offset that does not"
	       <<"hold correct points.";
	  newOffsets.display("This is new offsets");
	  offsets.display("This is old offsets");
	  exit(1);
	}
    }
  *this = newStencil;
}

//=====================================================================

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Methods giving some information }}
//\no function header:
// The following methods give some information.
//\end{StencilPublic.tex}{}
//==========================================================================

//==========================================================================
//\begin{>>StencilPublic.tex}{}
IntegerArray 
Stencil::getWidth() const
// /Description:
// Returns the {\ff \ST} width.
//
// /Author: K\AA
// 
//
//\end{StencilPublic.tex}{}
//==========================================================================
{
  if ( debug & 1 )
    cout << "Stencil::getWidth(ostream &)" << endl;
  return width;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
int 
Stencil::offsetExist(const Point &p_, int &i) const
// /Description:
// Returns TRUE if the point {\ff p} exists in the {\ff \ST}s offsets. 
// if return is TRUE, i holds the index for the point
//
// /Author: K\AA
// 
//
//\end{StencilPublic.tex}{}
//==========================================================================
{
  Point p; // A++ copy constructor cannot make an array from a view 
  p = p_;  // To fix this we use assignment
  resizePoint(p); // size becomes 1x3
  //p.display("   after resizePoint ");
  for (i = 0;  i< nrOfWeights; i++)
    if ( (p(0,0) == offsets(i,0)) && (p(0,1) == offsets(i,1)) 
	 && (p(0,2) == offsets(i,2)) )
      return 1;
  return 0;   
}

//==========================================================================
//\begin{>>StencilPublic.tex}{}
int 
Stencil::offsetExist(const Point &p_) const
// /Description:
// Returns TRUE if the point {\ff p} exists in the {\ff \ST}s offsets. 
//
// /Author: K\AA
// 
//
//\end{StencilPublic.tex}{}
//==========================================================================
{
  int tmp;
  return offsetExist(p_, tmp);
}


// includes for following method
//#include "Cube.h"
#include "SquareMapping.h"
#include "LineMapping.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "math.h"
#include "MappedGridFunction.h"
//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{The order of stencils}}
Real 
Stencil::checkOrder(MappedGridOperators::derivativeTypes type) const
// /Description:
// Method that estimates the order of the stencil for non-mixed derivatives.
// Works only for unit stencils. Useful for debugging purposes. 
// Used by {\ff StencilOperators} to check user supplied {\ff \ST}s.
//
// /type (input): xDerivative or xxDerivative (at least for now, this is enough.)
//
// /Author: K\AA
// 
//
//\end{StencilPublic.tex}{}
//==========================================================================
{
  /** NOTE: Remains: Treat other dims than x ***/

  if ( debug & 1 )
    cout << "Stencil::checkOrder(MappedGridOperators::derivativeTypes type)" 
	 << endl;

  LineMapping map1; //*** Use line for now.
  LineMapping map2;
  map1.setGridDimensions(axis1,11);
  map2.setGridDimensions(axis1,21);

  MappedGrid grid1(map1);
  MappedGrid grid2(map2);
  /*** Did not work. Fix some day...
  // Make sure enough ghost points
  grid1.dimension()(axis1, Start) -= width(axis1, Start); // *** Only x for now
  grid1.dimension()(axis1, End)   += width(axis1, End);
  grid1.numberOfGhostPoints()(axis1, Start) -= width(axis1, Start);
  grid1.numberOfGhostPoints()(axis1, End)   += width(axis1, End);
  grid2.dimension()(axis1, Start) -= width(axis1, Start);
  grid2.dimension()(axis1, End)   += width(axis1, End);
  grid2.numberOfGhostPoints()(axis1, Start) -= width(axis1, Start);
  grid2.numberOfGhostPoints()(axis1, End)   += width(axis1, End);
  ***/
    
  grid1.update();
  grid2.update();

  OGTrigFunction exactFunction;
  //OGPolyFunction exactFunction;
  RealMappedGridFunction u1(grid1,nullIndex,nullIndex,nullIndex);
  RealMappedGridFunction u2(grid2,nullIndex,nullIndex,nullIndex);
  RealMappedGridFunction exactDerivative1(grid1,nullIndex,nullIndex,nullIndex);
  RealMappedGridFunction exactDerivative2(grid2,nullIndex,nullIndex,nullIndex);

  Index I1, I2, I3, R1(0,1);
  getIndex(grid1.dimension(),I1,I2,I3);   // all grid points
  u1(I1,I2,I3,R1) = exactFunction(grid1,I1,I2,I3,R1);

  Index J1, J2, J3;
  getIndex(grid2.dimension(),J1,J2,J3);   // all grid points
  u2(J1,J2,J3,R1) = exactFunction(grid2,J1,J2,J3,R1);
  //  I1.display("I1 all grid points");
  //  J1.display("J1 all grid points");

  Stencil tmp1(*this);
  Stencil tmp2(*this);

  Real scaling1, scaling2;
  switch(type)
    {
      case MappedGridOperators::xDerivative :
	scaling1 = 1.0/grid1.gridSpacing()(axis1);
	scaling2 = 1.0/grid2.gridSpacing()(axis1);
	exactDerivative1 = exactFunction.x(grid1,I1,I2,I3,R1);
	exactDerivative2 = exactFunction.x(grid2,J1,J2,J3,R1);
	break;
      case MappedGridOperators::xxDerivative :
	scaling1 = 1.0/SQR(grid1.gridSpacing()(axis1));
	scaling2 = 1.0/SQR(grid2.gridSpacing()(axis1));
	exactDerivative1 = exactFunction.xx(grid1,I1,I2,I3,R1);
	exactDerivative2 = exactFunction.xx(grid2,J1,J2,J3,R1);
	break;
    default:
      cout << "Stencil::checkOrder called with derivative types that"
	   << " it can't handle. type =" << type << endl;
    }
	
  // Scale the stencils
  tmp1 *= scaling1;
  tmp2 *= scaling2;

  Real error1, error2;

  // Change index to calculate derivatives in interior 
  getIndex(grid1.gridIndexRange(),I1,I2,I3); 
  getIndex(grid2.gridIndexRange(),J1,J2,J3); 
  //  I1.display("I1 gridIndexRange");
  //  J1.display("J1 gridIndexRange");


  // allocate gridfunctions for the target derivatives
  RealMappedGridFunction target1(grid1,nullIndex,nullIndex,nullIndex);
  RealMappedGridFunction target2(grid2,nullIndex,nullIndex,nullIndex);

  getIndex(grid1.gridIndexRange(),I1,I2,I3); // interior and boundary points
  error1 = max(abs(exactDerivative1(I1,I2,I3) - 
    tmp1.applyStencil(u1,target1,I1,I2,I3,R1)(I1,I2,I3)));
  error2 = max(abs(exactDerivative2(J1,J2,J3) - 
    tmp2.applyStencil(u2,target2,J1,J2,J3,R1)(J1,J2,J3)));

  if( debug &  1 )
    {
      cout << "error1 "<< error1 << endl;
      cout << "error2 "<< error2 << endl;
      target1.display("Target 1");
      exactDerivative1.display("exactDerivative1");
      target2.display("Target 2");
      exactDerivative2.display("exactDerivative2");
      tmp1.display("This is tmp1");
      tmp2.display("This is tmp2");
    }
  Real orderEstimate = log(error1/error2)/log(double(2.));
  
  return orderEstimate;
}

//==========================================================================
//\begin{>>StencilPublic.tex}{\subsubsection{Remove zeros}}
void Stencil::removeZeros()
// /Description:
//
// Sometimes, operator() for instance may yield zero weights. In order to compress the {\ff \ST},
// the user can remove zeros via this method.   
//
// /Author: K\AA
// 
//
//\end{StencilPublic.tex}{}
//==========================================================================
{
  for (int i = 0;  i< nrOfWeights; i++)
    if(weights(i) == 0)
      removeWeight(i);
}


// Private methods

void Stencil::resizePoint(Point &p) const
{
  int n = p.elementCount();
  int valid = (n<=3); 
  valid = valid && 
    ( ( p.getBase(0) - p.getBound(0) == 0) || 
      ( p.getBase(1) - p.getBound(1) == 0));
  valid = valid && ( p.getBase(2) - p.getBound(2) == 0);
  if( !valid ) 
    {
      cout << "Error!!! Stencil::resizePoint() got a not valid Point !!!\n";
      cout << "Valid points are vectors in axis1 or axis2," 
	   << "with max 3 elements" << endl;
      p.display("This is p. ");
      exit(1);
    }
  /* // Strange bug fix 970323: sometimes it just didn't work.
     // Therefore new algorithm.
     // Krister
  // Maybe add zeros and maybe reshape. 
  if( p.getBase(0) - p.getBound(0) == 0) // vector 1xn
    {
      p.resize(1,3);
      for(int i=n; i<3; i++)
	p(0,i) = 0; 
    }
  else // vector nx1
    {
      p.resize(3,1);
      for(int i=n; i<3; i++)
	p(i,0) = 0;
      p.reshape(1,3);
    }
    */
  // New algo. Store values in tmp, build p from scratch.
  Point tmp; tmp = p; 
  if( p.getBase(0) - p.getBound(0) == 0) // vector 1xn
    {
      int base = tmp.getBase(0);
      p.resize(1,3); p = 0;
      for(int i=0; i<n; i++)
	p(0,i) = tmp(base,i); 
    }
  else // vector nx1
    {
      int base = tmp.getBase(1);
      p.resize(1,3); p = 0;
      for(int i=0; i<n; i++)
	p(0,i) = tmp(i,base);
    }
}

void Stencil::updateWidth()
{
  width = 0;
  
  for(int j = 0;  j< nrOfWeights; j++)
    for(int i=0; i<3; i++)
      {
	if( offsets(j,i) < width(i,Start) )
	  width(i,Start) =  offsets(j,i);
	if( offsets(j,i) > width(i,End) )
	  width(i,End) =  offsets(j,i);
      }

}

void Stencil::removeWeight(int i)
{
  if(i+1 < nrOfWeights) // Move data
    {
      Range toBeMoved(i+1,nrOfWeights-1);
      weights(toBeMoved-1) = weights(toBeMoved);
      offsets(toBeMoved-1,nullIndex) = offsets(toBeMoved,nullIndex);
    }

  nrOfWeights--;
  weights.resize(nrOfWeights); // resize preserves earlier data
  offsets.resize(nrOfWeights,3);  // resize preserves earlier data

  updateWidth();    
}



void Stencil::test()
{
  int test = 64;
  cout << "*********************************************************" << endl;
  cout << "Stencil:: Internal test routine. test flag== " << test
       << endl << endl;

  if(test & 1)
    {
      cout << "Test of resizePoint\n====================\n";
      Point p(1); p(0) = 1;
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;
 
      p.redim(1,2); p(0,0) = 1; p(0,1) = 2;
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;

      p.redim(1,3); p(0,0) = 1; p(0,1) = 2; p(0,2) =4;
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;
  
      p.redim(3); p(0) = 1; p(1) = 2; p(2) =4;
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;

      p.redim(2); p(0) = 1; p(1) = 2; 
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;

      IntegerArray arr(3,3); arr = -8; 
      arr(0,0) = 1; arr(0,1) = 1;arr(0,2)=2; arr(1,0) = -1; arr(2,0) = -2;
      arr.display("This is the array we use for getting rows or columns");
      p = arr(0,nullIndex);
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;
 
      p.redim(3,1);
      p = arr(nullIndex,1);
      p.display("Before resize"); resizePoint(p); 
      p.display("After resize"); cout << endl;

      // Note: Copy constructor cannot make array from view
      // Therefore, A++ complains when q or q2 are resized.
      // Code must therefore use assignment instead:
      // IntegerArray q; q=arr(nullIndex,1); // This works.
      IntegerArray q = arr(nullIndex,1);
      q.display("q Before resize"); resizePoint(q); 
      q.display("After resize"); cout << endl;

      IntegerArray q2 = arr(2,nullIndex);
      q2.display("q2 Before resize"); resizePoint(q2); 
      q2.display("After resize"); cout << endl;

    }

  if(test & 2)
    {
      cout << "Test of addWeight()\n====================\n";
      Stencil S;      
      Point p(1,3); p=0; p(0,0) = 1;

      S.addWeight(1,p);
      S.addWeight(2,p);
      S.display("Should now have weight 3 in (1,0,0) ");
      p(0,1)=1;
      S.addWeight(-1,p);
      S.display("Should now have weight -1 in (1,1,0) ");
      p=-1;
      S.addWeight(5,p);
      S.display("Should now have weight 5 in (-1,-1,-1) ");
      S.addWeight(-5,p);
      S.display("Should now have no weight in (-1,-1,-1) ");
      S.addWeight(-2,p);
      S.display("Should now have weight -2 in (-1,-1,-1) ");
      p=0; p(0,0) = 1;
      S.addWeight(-3,p);
      S.display("Should now have no weight in (1,0,0) ");



      cout << "Test of displayStructure()\n====================\n";
      S.displayStructure("Here is the structure of the stencil");
      p=0;
      S.addWeight(3,p);
      S.displayStructure
	("Here is the structure of the stencil, now with value in origo");

    }

  if(test & 4)
    {
      cout << "Test of rotate()\n====================\n";
      Stencil Sx,Sy,Sz,Sxyz;      
      Point p(1,3); p=0; 
      Sx.addWeight(-1,p);
      p(0,0) = 1;
      Sx.addWeight(1,p);
      Sx.displayStructure("This is Dplusx");

      Sy = Sx.rotate();
      Sy.displayStructure("This is Dplusy");

      Sz = Sx.rotate(axis2,-5);
      Sz.displayStructure("This is Dplusz");

      Sz = Sy.rotate(axis1);
      Sz.displayStructure("This is again Dplusz");

      Stencil minusSx = Sy.rotate();
      minusSx.displayStructure("This is '-Dplusx'");
      minusSx.display("This is values '-Dplusx'");
      Stencil Dminusx;
      Dminusx = -1*minusSx;
      Dminusx.display("This is Dminus");

      cout << "Test of operator+()\n====================\n";
      Sxyz = Sx + Sy + Sz;
      Sxyz.display("This is Dplusx + Dplusy + Dplusz");

      cout << "Test of operator*()\n====================\n";
      Stencil scaledSxyz;
      scaledSxyz = Sxyz * 0.1;
      scaledSxyz.display("This is 0.1*(Dplusx + Dplusy + Dplusz)");
      scaledSxyz = 0.1* Sxyz * 0.1;
      scaledSxyz.display("This is 0.01*(Dplusx + Dplusy + Dplusz)");

    }
 if(test & 8)
    {
      cout << "Test of Constructor(weights,offsets)\n====================\n";
      RealArray weights_(3); weights_ = 1; weights_(1) = -2;
      weights_.display("Weights for DplusDminus");
      IntegerArray offsets_(3,3); offsets_ = 0;
      offsets_(0,0)=-1; // offsets(0,all) = (-1,0,0)
      offsets_(2,0)=1; // offsets(0,all) = (1,0,0)
      offsets_.display("Offsets for DplusDminus");
      Stencil DplusxDminusx(weights_,offsets_);
      DplusxDminusx.display("DplusDminus");

      Stencil DplusyDminusy = DplusxDminusx.rotate();
      Stencil DpluszDminusz = DplusyDminusy.rotate(axis1);

      Stencil Laplace2d=DplusxDminusx + DplusyDminusy;
      Stencil Laplace3d=Laplace2d+DpluszDminusz;

      Laplace2d.display("This is Laplace in 2d");
      Laplace2d.displayStructure("This is the Structure");

      Laplace3d.display("This is Laplace in 3d");
      Laplace3d.displayStructure("This is the Structure");

      // Test if the check of matrixes may work
      //RealArray ra; IntegerArray ia;
      //Stencil noWork(ra,ia);
      //noWork.display("Shall not work");


      cout << "Test of operator()\n====================\n";

      weights_.redim(2); weights_(0)=-1;weights_(1)=1;
      offsets_.redim(2,3); offsets_=0; 
      offsets_(1,0)=1; // offsets_(1,all) = (1,0,0)
      Stencil Dplus(weights_,offsets_);
      offsets_(0,0)=-1; // offsets_(0,all) = (-1,0,0)
      offsets_(1,0)=0; // offsets_(1,all) = (0,0,0)
      Stencil Dminus(weights_,offsets_);

      Dplus.display("Dplus");
      Dminus.display("Dminus");

      Stencil DplusDminus,DminusDplus;
      DplusDminus = Dplus(Dminus);
      DplusDminus.display("DplusDminus");
      DminusDplus = Dminus(Dplus);
      DminusDplus.display("DminusDplus");

      Stencil zero = DminusDplus + -1*DplusDminus;
      zero.display("Shall have zero weights");
      zero.displayStructure("Shall have no weights");

      cout << "Test of operator=\n====================\n";
      // mistreat zero...
      zero = zero;
      zero = zero + Dplus;
      zero.displayStructure("Again Dplus");

      cout << "Test of operator- \n====================\n";
      zero = zero - Dplus;
      zero.displayStructure("This is zero");
      zero = -Dplus;
      zero.display("This is -Dplus");
      zero = -zero;
      zero.displayStructure("Again Dplus");

      cout << "Test of operator/ \n====================\n";
      Dplus = Dplus/0.1;
      Dplus.display("This is scaled Dplus");

      cout << "Test of copy constructor \n====================\n";
      Stencil S0;
      Stencil S1 = S0;
      S0.display("This is nothing");
      S1.display("This is nothing again");
      S0 = Dminus;
      Stencil S2 = S0;
      S0.display("This is Dminus");
      S2.display("This is Dminus again");
      Stencil S3 = S0 - S0;
      S3.display("This is nothing again");
   }
 if(test & 16)
    {
      cout << "Test of composite operators\n====================\n";
      Stencil Identity, Ex;
      Point p(1,3); p=0; 
      Identity.addWeight(1.0,p);
      p(0,0) = 1; // p= (1,0,0)
      Ex.addWeight(1.0,p);
      
      Stencil Dplus = Ex;
      Dplus.display("This is not yet Dplus");
      Dplus -= Identity;
      Dplus.display("This is Dplus");

      Dplus *= 0; // Stencil does not treat this nice - width remains...
      Dplus.display("This is Dplus*0");

      Dplus =  -Identity;
      Stencil Ez = Ex.rotate(axis2,-1); // Make Ez
      Dplus += Ez; 
      Dplus.display("This is Dplusz");

      Dplus/=0.1;
      Dplus.display("This is scaled Dplusz");
    }
 if(test & 32)
    {
      cout << "Test of constructor(predefinedStencil)\n====================\n";

      Stencil Identity(Stencil::identity);
      Identity.display("This is identity");

      Stencil Displacex(Stencil::displacex);
      Displacex.display("This is displacex");

      Stencil Inversedisplacex(Stencil::inversedisplacex);
      Inversedisplacex.display("This is inversedisplacex");

      Stencil Dplusx(Stencil::dplusx);
      Dplusx.display("This is dplusx");

      Stencil Dminusx(Stencil::dminusx);
      Dminusx.display("This is dminusx");

      Stencil Dzerox(Stencil::dzerox);
      Dzerox.display("This is dzerox");

      Stencil Dnullx(Stencil::dnullx);
      Dnullx.display("This is dnullx");

      Stencil Dplusxdminusx(Stencil::dplusxdminusx);
      Dplusxdminusx.display("This is dplusxdminusx");

      cout << "Test of checkOrder" << endl;
      Real order = Dplusx.checkOrder(MappedGridOperators::xDerivative);
      cout << "The order of Dplusx is estimated to "
	   << order << endl;
      order = Dnullx.checkOrder(MappedGridOperators::xDerivative);
      cout << "The order of Dnullx is estimated to "
	   << order << endl; 
      order = Dplusxdminusx.checkOrder(MappedGridOperators::xxDerivative);
      cout << "The order of Dplusxdminusx is estimated to "
	   << order << endl; 
    }
 if(test & 64)
    {
      cout << "Test of getting coefficient matrix\n====================\n";
      Index I1, I2, I3;
      SquareMapping square; MappedGrid mg( square );

      int stencilSize = 4;
/***###      

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      IntegerArray fourPointStar(4,3); fourPointStar = 0;
      fourPointStar(0,0) = -1; // (-1,0,0)
      fourPointStar(1,0) =  1; // ( 1,0,0)
      fourPointStar(2,1) = -1; // (0,-1,0)
      fourPointStar(3,1) =  1; // (0, 1,0)

      // *** 
      RealMappedGridFunction coeff1(mg, stencilSize, nullIndex, nullIndex, nullIndex), 
	coeff2(mg, stencilSize, nullIndex, nullIndex, nullIndex);

      coeff1.setIsACoefficientMatrix(TRUE, stencilSize);
      coeff2.setIsACoefficientMatrix(TRUE, stencilSize);

      Stencil Dnull(Stencil::dnullx);
      Dnull.conformOffsets( fourPointStar );
      Dnull.display("This is fourPointStar Dnullx");

      Dnull.makeCoefficients(coeff1, I1, I2, I3);

      coeff1.display("This is coefficient matrix for Dnullx");
      coeff1.sparse->equationNumber.display("This is equation numbers");


      Dnull = Dnull.rotate();
      Dnull.conformOffsets( fourPointStar ); // Rotating change the structure
      Dnull.display("This is fourPointStar Dnully");

      Dnull.makeCoefficients(coeff2, I1, I2, I3);
      coeff2.display("This is coefficient matrix for Dnully");
      coeff2.sparse->equationNumber.display("This is equation numbers");

      (coeff1 + coeff2).display("Coefficient matrix for Dnullx + Dnully");

      //      bus error for this one - very strange
      Dnull.makeCoefficients(coeff1, I1, I2, I3);
      coeff1.display("Coefficient matrix two for Dnullx + Dnully");
//      ***
// *********************####
      cout << "Test for systems" << endl;
      // u_xx + v_x   = f_1
      // 2u_y  + v_yy = f_2
      Stencil Dx(Stencil::dnullx);
      Stencil Dxx(Stencil::dplusxdminusx);
      Stencil Dy = Dx.rotate();
      Stencil Dyy = Dxx.rotate();
      
      IntegerArray fivePointStar(5,3); 
      fivePointStar = 0;
      fivePointStar(Index(0,4),nullIndex) = fourPointStar;

      Dx.conformOffsets( fivePointStar );
      Dxx.conformOffsets( fivePointStar );
      Dy.conformOffsets( fivePointStar );
      Dyy.conformOffsets( fivePointStar );
###*****/

      stencilSize = 5; 
      int nrGhost = 1; // Why not?
      int numberC = 2;
      int totalStencilSize = stencilSize*SQR(numberC); 
      RealMappedGridFunction coeff3(mg, totalStencilSize, nullIndex, nullIndex, nullIndex); 

      coeff3.setIsACoefficientMatrix(TRUE, stencilSize, nrGhost, numberC);
/************### 
      Dxx.makeCoefficients(coeff3,I1,I2,I3, 0,0);
      Dx.makeCoefficients(coeff3,I1,I2,I3, 0,1);
      Dy.makeCoefficients(coeff3,I1,I2,I3, 1,0);
      Dyy.makeCoefficients(coeff3,I1,I2,I3, 1,1);
###*****/
      //$#$#$ following gives segmentation fault
      //coeff3.display("After Dyy v, second equation"); 
      
    }

  cout << "*********************************************************" << endl;
}

// ************************************************************


ostream &operator<<(ostream &os, const Stencil &s)
{
  s.display("",Stencil::text,os);
  return os;
}


Stencil operator*(Real d,const Stencil &s)
{
  return s*d;
}

