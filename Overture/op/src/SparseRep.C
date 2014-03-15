#include "SparseRep.h"
#include "MappedGrid.h"
#include "display.h"
#include "ParallelUtility.h"

int SparseRepForMGF::debug=0;

//
//          *** here are some comments ****
//
//\begin{>SparseRepInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// Here are the public enumerators:
//
// /classifyTypes:
// This enumerator contains a list of classify types..
// Any non-negative value indicates a used point. Negative values are equations with zero for the rhs
// {\footnotesize
// \begin{verbatim}     
//   enum classifyTypes  
//   {                  
//     interior=1,
//     boundary=2,
//     ghost1=3,
//     ghost2=4,
//     ghost3=5,
//     ghost4=6,
//     interpolation=-1,
//     periodic=-2,
//     extrapolation=-3,
//     unused=0
//   };
// \end{verbatim}     
// }
//
//\end{SparseRepInclude.tex}

//\begin{>>SparseRepInclude.tex}{\subsubsection{Constructors}}  
SparseRepForMGF::
SparseRepForMGF()
//=======================================================================================
//\end{SparseRepInclude.tex}
//=======================================================================================
{ 
  equationOffset=0;
  numberOfComponents=1;
  numberOfGhostLines=1;
  stencilSize=9;

}

SparseRepForMGF::
~SparseRepForMGF()
{ 
}

SparseRepForMGF::
SparseRepForMGF(const SparseRepForMGF & rep)
// copy constructor
{
  *this=rep;
}


SparseRepForMGF & SparseRepForMGF::
operator=( const SparseRepForMGF & x )
{
  equationOffset     =x.equationOffset;
  numberOfComponents =x.numberOfComponents;
  equationNumber     =x.equationNumber;
  classify           =x.classify;
  return *this;
}



//\begin{>>SparseRepInclude.tex}{\subsubsection{indexToEquation}}  
int SparseRepForMGF::
indexToEquation( int n, int i1, int i2, int i3)
//=======================================================================================
// /Description: Return the equation number for given indices
//  /n (input): component number ( n=0,1,..,numberOfComponents-1 )
//  /i1,i2,i3 (input): grid indices
// /Return value:
//    The equation number.
//\end{SparseRepInclude.tex}
//=======================================================================================
{
    return n+1+   numberOfComponents*(i1-equationNumber.getBase(1)+
         equationNumber.getLength(1)*(i2-equationNumber.getBase(2)+
         equationNumber.getLength(2)*(i3-equationNumber.getBase(3)))) + equationOffset;
}



#define MN(m,n) (n)+(m)*numberOfComponents

//\begin{>>SparseRepInclude.tex}{\subsubsection{setCoefficientIndex}}  
int SparseRepForMGF::
setCoefficientIndex(const int & m, 
                    const int & na, const Index & I1a, const Index & I2a, const Index & I3a,
                    const int & nb, const Index & I1b, const Index & I2b, const Index & I3b)
//===============================================================================================
// /Description:
//     Assign row and column numbers to entries in a sparse matrix.
//  Rows and columns in the sparse matrix are numbered according to the values of
//             (n,I1,I2,I3)
//  where n is the component number and (I1,I2,I3) are the coordinate indicies on the grid.
//  The component number n runs from 0 to the numberOfComponentsForCoefficients and is used
//  when solving a system of equations.
//
// /m (input): assign row/column values for the m'th entry in the sparse matrix
// /na,I1a,I2a,I3a (input): defines the row(s)
// /Nb,I1b,I2b,I3b (input): defines the column(s)
//
//\end{SparseRepInclude.tex}
//===============================================================================================
{
  // real time=getCPU();
  // check  I1.getLength() == I1b.getLength()   ***********************
  int i2,i3, i1b,i2b,i3b;

/* ---
  int dimension1=equationNumber.getLength(1);
  int dimension2=equationNumber.getLength(2);
  int base1 = equationNumber.getBase(1);
  int base2 = equationNumber.getBase(2);
  int base3 = equationNumber.getBase(3);
  
  for( i3=I3a.getBase(), i3b=I3b.getBase(); i3<=I3a.getBound(); i3++, i3b++ )
  for( i2=I2a.getBase(), i2b=I2b.getBase(); i2<=I2a.getBound(); i2++, i2b++ )
  for( i1=I1a.getBase(), i1b=I1b.getBase(); i1<=I1a.getBound(); i1++, i1b++ )
  {
    // equationNumber(MN(m,n),i1,i2,i3)=indexToEquation(nb,i1b,i2b,i3b);
    equationNumber(m,i1,i2,i3) =indexToEquation(nb,i1b,i2b,i3b);
    //equationNumber(m,i1,i2,i3) =
    //  nb+1+numberOfComponents*(i1b-base1+ dimension1*(i2b-base2+dimension2*(i3b-base3))) + equationOffset;
  }
---- */
  #ifdef USE_PPP 
    const intSerialArray & equationNumberLocal = equationNumber.getLocalArray();
      
    const int n1a = max(I1a.getBase() , equationNumberLocal.getBase(1));
    const int n1b = min(I1a.getBound(),equationNumberLocal.getBound(1));

    const int n2a = max(I2a.getBase() , equationNumberLocal.getBase(2));
    const int n2b = min(I2a.getBound(),equationNumberLocal.getBound(2));

    const int n3a = max(I3a.getBase() , equationNumberLocal.getBase(3));
    const int n3b = min(I3a.getBound(),equationNumberLocal.getBound(3));


    if( n1a>n1b || n2a>n2b || n3a>n3b ) return 0; 

    Range J1a(n1a,n1b);

    const int m1a = I1b.getBase() + n1a-I1a.getBase();  // offset the "b" Index's in the same way as the "a"
    const int m2a = I2b.getBase() + n2a-I2a.getBase();
    const int m3a = I3b.getBase() + n3a-I3a.getBase();
    

    for( i3=n3a, i3b=m3a; i3<=n3b; i3++, i3b++ )
    for( i2=n2a, i2b=m2a; i2<=n2b; i2++, i2b++ )
    {
      i1b=m1a;
      equationNumberLocal(m,J1a,i2,i3).seqAdd(indexToEquation(nb,i1b,i2b,i3b),numberOfComponents); // seqAdd(base,stride)
    }

  #else
    for( i3=I3a.getBase(), i3b=I3b.getBase(); i3<=I3a.getBound(); i3++, i3b++ )
    for( i2=I2a.getBase(), i2b=I2b.getBase(); i2<=I2a.getBound(); i2++, i2b++ )
    {
      i1b=I1b.getBase();
      equationNumber(m,I1a,i2,i3).seqAdd(indexToEquation(nb,i1b,i2b,i3b),numberOfComponents); // seqAdd(base,stride)
    }
  #endif

  // printf("SparseRepForMGF::setCoefficientIndex: time=%e \n",getCPU()-time);
  return 0;
}

//\begin{>>SparseRepInclude.tex}{\subsubsection{setCoefficientIndex}}  
int SparseRepForMGF::
setCoefficientIndex(const int & m, 
                    const int & na, const Index & I1a, const Index & I2a, const Index & I3a,
                    const int & equationNumber0 )
//===============================================================================================
// /Description:
//     Assign row and column numbers to entries in a sparse matrix.
//    This routine is normally only used for assign equation numbers on CompositeGrid's
//  when the equationNumber belongs to a point on a different MappedGrid.
//  Rows and columns in the sparse matrix are numbered according to the values of
//             (n,I1,I2,I3)
//  where n is the component number and (I1,I2,I3) are the coordinate indicies on the grid.
//  The component number n runs from 0 to the numberOfComponentsForCoefficients and is used
//  when solving a system of equations.
//
// /m (input): assign row/column values for the m'th entry in the sparse matrix
// /na,I1a,I2a,I3a (input): defines the row(s)
// /equationNumber (input): defines an equation number
//
//\end{SparseRepInclude.tex}
//===============================================================================================
{
// *wdh* 050411:
//   int i1,i2,i3;
//   for( i3=I3a.getBase(); i3<=I3a.getBound(); i3++ )
//   for( i2=I2a.getBase(); i2<=I2a.getBound(); i2++ )
//   for( i1=I1a.getBase(); i1<=I1a.getBound(); i1++ )
//   {
//     equationNumber(m,i1,i2,i3)=equationNumber0;
//   }

  #ifdef USE_PPP
    const intSerialArray & equationNumberLocal = equationNumber.getLocalArray();
    const intSerialArray & classifyLocal = classify.getLocalArray();
  #else
    const intSerialArray & equationNumberLocal = equationNumber;
    const intSerialArray & classifyLocal = classify;
  #endif
  Index I1=I1a,I2=I2a,I3=I3a;
  const int includeGhost=1; // include ghost since we use getLocalArray (and not localArrayWithGhost)
  bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);  
  if( ok )
    equationNumberLocal(m,I1,I2,I3)=equationNumber0;

  return 0;
}

//\begin{>>SparseRepInclude.tex}{\subsubsection{sizeOf}}
real SparseRepForMGF::
sizeOf(FILE *file /* = NULL */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by this object; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{SparseRepInclude.tex}
//==========================================================================
{
  real size=sizeof(*this);
  size+=equationNumber.sizeOf()+classify.sizeOf();

  return size;
}

/* -----
void SparseRepForMGF::
setNumberOfComponents(int number)
{
  numberOfComponents=number;
}

void SparseRepForMGF::
setOffset(int number)
// use this number to offset the equation numbering for this grid
{
  equationOffset=number;
}
------ */

#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123N(m1,m2,m3,n) (M123(m1,m2,m3)+stencilSize*(n))
#define ForStencil(m1,m2,m3)   \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2;  m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1;  m1++) 

#define ForStencilN(n,m1,m2,m3)   \
    for( n=0; n<numberOfComponents; n++) \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1; m1++) 

// ********* should be able to optionally specify the stencil width ******************************
//\begin{>>SparseRepInclude.tex}{\subsubsection{updateToMatchGrid}}  
int SparseRepForMGF::
updateToMatchGrid(MappedGrid & mg, 
		  int stencilSize0,        /* = unchanged */
		  int numberOfGhostLines0, /* = unchanged */
		  int numberOfComponents0, /* = unchanged */
                  int offset0 /* = unchanged */ )
//===============================================================================================
// /Description:
//   Initialize the equationNumber and classify arrays.
//   The equation number array is initialized according to value of stencilSize. The stencil width will
//   be chosen to be pow(stencilSize,1/d) where d is the number of space dimensions. Thus
//   \begin{itemize}
//     \item If $3^d \le \text{~stencilSize~} < 5^d$ (d=space dimension) then the stencil is assumed to be a 
//       standard $3^d$ stencil and the first $3^d$ entries are initialized in the standard form. Any excess
//       entries are given an equation number of 0 (unused).
//     \item If $5^d \le \text{~stencilSize~} < 7^d$ then the stencil is assumed to be a  standard $5^d$
//       setncil and initialized in the standard form.  Any excess
//       entries are given an equation number of 0 (unused).
//      \item etc.
//     \item If stencilSize is less than $3^d$ then equationNumber array is set to zero.
//  \end{itemize}
// 
// /mg (input): update to match this grid.
// /stencilSize0 (input): maximum size for the stencil (for each component). 
//    By default (i.e. if no
//    value is specified then stencilSize0 remains unchanged from its current value. (It is initially
//    set to 9).
// /numberOfComponents0 (input): number of components.
//    By default (i.e. if no
//    value is specified then numberOfComponents0 remains unchanged from its current value. (It is initially
//    set to 1).
// /offset0 (input): offset equation numbers by this amount.
//    By default (i.e. if no
//    value is specified then offset0 remains unchanged from its current value. (It is initially
//    set to 0).
//\end{SparseRepInclude.tex}
//===============================================================================================
{
  if( SparseRepForMGF::debug & 1 || Mapping::debug & 1 )
    printf(">>>>>> SparseRep::update to match grid <<<<<<<<<< \n");
  // real time=getCPU();

  if( numberOfComponents0!=1 )
    numberOfComponents=numberOfComponents0;
  if( numberOfGhostLines0!=1 )
    numberOfGhostLines=numberOfGhostLines0;
  if( stencilSize0!=9 )
    stencilSize=stencilSize0;
  if( offset0!=0 )
    equationOffset=offset0;

  Range all;
  int stencilDimension=stencilSize*SQR(numberOfComponents);
  equationNumber.updateToMatchGrid(mg,stencilDimension,all,all,all);
  // set default equation numbers:
  equationNumber=0;
  Index I1,I2,I3;
  getIndex(mg.dimension(),I1,I2,I3);   // initialize all points

  assert( mg.numberOfDimensions() > 0 );

  int width;  // width of the stencil
  // compute the stencil widith (add epsilon to offset truncation error)
  width = int( pow(real(stencilSize),1./real(int(mg.numberOfDimensions())))+REAL_EPSILON*stencilSize*10. );
  if( width % 2 == 0 )  // width should be odd
    width--;
  // cout << "SparseRep:: updateToMatchGrid: width = " << width << ", stencilSize= " << stencilSize << endl;
  
  int halfWidth1 = width/2;
  int halfWidth2 = mg.numberOfDimensions() > 1 ? halfWidth1 : 0; 
  int halfWidth3 = mg.numberOfDimensions() > 2 ? halfWidth1 : 0;
  int m1,m2,m3,c,e;

  
// Use this for indexing into coefficient matrices representing systems of equations
#define CE(c,e) (stencilSize*((c)+numberOfComponents*(e)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

  if( width>0 )
  { // assign equation numbers to be in standard form
    for( e=0; e<numberOfComponents; e++ )                        
      for( c=0; c<numberOfComponents; c++ )                        
      {
	ForStencil(m1,m2,m3)  
	  setCoefficientIndex(M123CE(m1,m2,m3,c,e), e,I1,I2,I3, c,(I1+m1),(I2+m2),(I3+m3) );  
	// fill in any extra values for oges, give a default value
	for( int m=M123(halfWidth1,halfWidth2,halfWidth3)+1; m<stencilSize; m++ )
	  setCoefficientIndex(m+CE(c,e), e,I1,I2,I3, c,I1,I2,I3 );  
      }
  }
  
  // printf("SparseRep:updateToMatchGrid: time after setCoefficientIndex = %e \n",getCPU()-time);


  classify.updateToMatchGrid(mg,all,all,all,numberOfComponents);
  classify=unused;
  Index N(0,numberOfComponents);

  #ifdef USE_PPP
    const intSerialArray & classifyLocal = classify.getLocalArray();
  #else
    const intSerialArray & classifyLocal = classify;
  #endif

  getIndex(mg.indexRange(),I1,I2,I3);
  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
  if( !ok ) return 0;   // no points to assign in this case
  
  classifyLocal(I1,I2,I3,N)=interior;

  for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
  for( int side=Start; side<=End; side++ )
  {
    if( (bool)mg.isAllCellCentered() )
    {
      // Mark first ghost cell as "boundary" and other ghost cells as "extrapolation"
      // (first mark ghost line 1 as extrapolation so we mark the corner)
      for( int ghost=1; ghost<=numberOfGhostLines; ghost++ )
      {
	getGhostIndex(mg.indexRange(),side,axis,I1,I2,I3,ghost,ghost);
        bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
        if( ok )
  	  classifyLocal(I1,I2,I3,N)=extrapolation;
      }
      getGhostIndex(mg.indexRange(),side,axis,I1,I2,I3,1);
      bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
      if( ok )
        classifyLocal(I1,I2,I3,N)=boundary;
    }
    else
    {
      // Mark boundary points as "boundary" and ghost lines as "extrapolation"
      getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
      if( ok )
        classifyLocal(I1,I2,I3,N)=boundary;
      for( int ghost=1; ghost<=numberOfGhostLines; ghost++ )
      {
	getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,ghost,ghost);
        bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
        if( ok )
          classifyLocal(I1,I2,I3,N)=extrapolation;
      }
    }
  }


  // equationNumber.display("SparseRep:updateToMatchGrid: Here is equationNumber");
  // classify.display("SparseRep:updateToMatchGrid: Here is classify");
  
  // time=getCPU()-time;
  // printf("time for SparseRep:updateToMatchGrid = %e \n",time);
  return 0;
}

  
//\begin{>>SparseRepInclude.tex}{\subsubsection{setParameters}}  
void SparseRepForMGF::
setParameters(int stencilSize0,        /* = unchanged */
	      int numberOfGhostLines0, /* = unchanged */
	      int numberOfComponents0, /* = unchanged */
	      int offset0 /* = unchanged */ )
//===============================================================================================
// /Description:
//   Set various parameters. Use this routine if you want to set the properties of the
//  SparseRep object before you have a MappedGrid. You must call updateToMatchGrid for
//  these values to take effect.
// 
// /stencilSize0 (input): maximum size for the stencil (for each component). 
//    By default (i.e. if no
//    value is specified then stencilSize0 remains unchanged from its current value. (It is initially
//    set to 9).
// /numberOfComponents0 (input): number of components.
//    By default (i.e. if no
//    value is specified then numberOfComponents0 remains unchanged from its current value. (It is initially
//    set to 1).
// /offset0 (input): offset equation numbers by this amount.
//    By default (i.e. if no
//    value is specified then offset0 remains unchanged from its current value. (It is initially
//    set to 0).
//\end{SparseRepInclude.tex}
//===============================================================================================
{
  if( numberOfComponents0!=1 )
    numberOfComponents=numberOfComponents0;
  if( numberOfGhostLines0!=1 )
    numberOfGhostLines=numberOfGhostLines0;
  if( stencilSize0!=9 )
    stencilSize=stencilSize0;
  if( offset0!=0 )
    equationOffset=offset0;
}

//\begin{>>SparseRepInclude.tex}{\subsubsection{setClassify}}  
int SparseRepForMGF::
setClassify(const classifyTypes & type, 
            const Index & I1_, const Index & I2_, const Index & I3_, const Index & N )
//===============================================================================================
// /Description:
//   Specify the classification for a set of Index values
//\end{SparseRepInclude.tex}
//===============================================================================================
{
   #ifdef USE_PPP
     const intSerialArray & classifyLocal = classify.getLocalArray();
   #else
     const intSerialArray & classifyLocal = classify;
   #endif
   Index I1=I1_, I2=I2_, I3=I3_;
   const int includeGhost=1;
   bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
   if( ok )  
     classifyLocal(I1,I2,I3,N)=type;

//  classify(I1_,I2_,I3_,N)=type;
  return 0;
}



//\begin{>>SparseRepInclude.tex}{\subsubsection{equationToIndex}}  
int SparseRepForMGF::
equationToIndex( const int eqnNo, int & n, int & i1, int & i2, int & i3 )
//=============================================================================
// /Description:
//   Convert an Equation Number to a point on a grid (Inverse of indexToEquation)
//  /eqnNo0 (input): equation number
//  /n (output): component number ( n=0,1,..,numberOfComponents-1 )
//  /i1,i2,i3 (output): grid indices
//\end{SparseRepInclude.tex}
//=============================================================================
{
  int eqn=eqnNo-equationOffset;

  n= (eqn % numberOfComponents);
  eqn/=numberOfComponents;
  i1=(eqn % equationNumber.getLength(1))+equationNumber.getBase(1);
  eqn/=equationNumber.getLength(1);
  i2=(eqn % equationNumber.getLength(2))+equationNumber.getBase(2);
  eqn/=equationNumber.getLength(2);
  i3=(eqn % equationNumber.getLength(3))+equationNumber.getBase(3);
  return 0;
}


#undef  ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<c.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

//\begin{>>SparseRepInclude.tex}{\subsubsection{fixUpClassify}}  
int SparseRepForMGF::
fixUpClassify(realMappedGridFunction & coeff )
//=====================================================================================
// /Description:
//   Fixup up the classify array to take into account the mask array and periodicity
//
// /coeff (input):  The coefficient matrix
//\end{SparseRepInclude.tex}
//=====================================================================================
{
  // SparseRepForMGF::debug=15;  // ***
  
  if( SparseRepForMGF::debug & 2 )
    cout << "Entering SparseRepForMGF::fixUpClassify " << endl;

  Index I1,I2,I3,Ib1,Ib2,Ib3,Ie1,Ie2,Ie3,R[3];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
  
  Index In(0,numberOfComponents);
  int side,axis;
  int n;

  
  assert(coeff.grid!=NULL);
  MappedGrid & c = *coeff.mappedGrid;

#ifdef USE_PPP
  const intSerialArray & classifyLocal = classify.getLocalArray();
  const intSerialArray & mask = c.mask().getLocalArray();
#else
  const intSerialArray & classifyLocal = classify;
  const intArray & mask = c.mask();
#endif
  
  if( SparseRepForMGF::debug & 2 )
    displayMask(c.mask(),"classify: here is the mask array");

  if( SparseRepForMGF::debug & 4 )
  {
    cout << "***SparseRep: numberOfGhostLines=" << numberOfGhostLines << endl;
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << " 1=interior, 2=bndry, 3=ghost1, 4=ghost2, -1=interp, -2=periodic, -3=extrap, 0=unused\n";
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    display(classify,"***SparseRep:fixupClassify: classify on input***");
  }

/* ----
// Notes:
//   getBoundaryIndex should use myIndexRange, defined below
//   getGhostIndexRange should also use myGridIndex
const IntegerArray & myIndexRange = (int)c.isAllVertexCentered() ? c.gridIndexRange() : c.indexRange();

IntegerArray extendedRange(2,3);
extendedRange = (int)c.isAllVertexCentered() ? c.gridIndexRange() : c.indexRange();
Range Rx(0,c.numberOfDimensions()-1);
extendedRange(Start,Rx)=min(extendedRange(Start,Rx),c.extendedIndexRange()(Start,Rx));
extendedRange(End  ,Rx)=max(extendedRange(End  ,Rx),c.extendedIndexRange()(End  ,Rx));
--- */

  // mark interpolation points   ******************* would be faster to use interpolationPoint ***
  // 990918 getIndex( c.extendedIndexRange(),I1,I2,I3 );   
  getIndex( c.extendedRange(),I1,I2,I3 );   
  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
  if( ok )
  {

    where( mask(I1,I2,I3) < 0 )
    {
      for( n=0; n<numberOfComponents; n++ )
	classifyLocal(I1,I2,I3,n)=interpolation;
    }
    
    // mark periodic sides
    ForBoundary(side,axis)
    {
      if( c.boundaryCondition(side,axis)<0 )
      {
	for( int ghost=1; ghost<=numberOfGhostLines+side; ghost++ )  // for each ghost line
	{
	  getGhostIndex(c.indexRange(),side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);  
	  for( int dir=1; dir<c.numberOfDimensions(); dir++ ) // *wdh* 990715 we need to catch corners if doubly periodic
	  {
	    int axisp= (axis+dir) % c.numberOfDimensions();   // tangential direction to axis
	    if( c.boundaryCondition(side,axisp)<0 )
	      Igv[axisp]=Range(Igv[axisp].getBase(),Igv[axisp].getBound()+1); // add one if a periodic tangential direction
	  }
          bool ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,Ig1,Ig2,Ig3,includeGhost);
          if( ok ) 
	    classifyLocal(Ig1,Ig2,Ig3,In)=periodic;
	}
      }
    }
    // mark unused points
    getIndex( c.dimension(),I1,I2,I3 );   // *wdh* 980408
    ok = ParallelUtility::getLocalArrayBounds(classify,classifyLocal,I1,I2,I3,includeGhost);
    if( ok )
    {
      where( mask(I1,I2,I3)==0 )
      {
	for( n=0; n<numberOfComponents; n++ )
	  classifyLocal(I1,I2,I3,n)=unused;
      }
    }
    
  }
  
  if( SparseRepForMGF::debug & 2 )
  {
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    cout << " 1=interior, 2=bndry, 3=ghost1, 4=ghost2, -1=interp, -2=periodic, -3=extrap, 0=unused\n";
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    display(classify,"***SparseRep:fixupClassify: Here is the classify array***","%4i");
  }

  return 0;
}






#undef  ForBoundary
