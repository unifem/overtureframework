//\begin{>testInclude.tex}{\subsection{divergence}} 
int
divergence (realArray & x, 
            const intArray & y,
            const floatArray & z )
//=========================================================================================
//
// /Purpose:
//   The purpose of this routine is to compute the divergence of a realArray.
//
//   /x(all,all,all,numberOfDimensions) (input): the vector x is used to compute
//          the divergence.
//   /y (output): the vector y:
//   /z (input/output): this vector is never really used but would otherwise
//              be equal to $x^y$. 
//
// /Return Values:  ~
//   \begin{itemize}  
//     \item 0 : success
//     \item 1 : error
//   \end{itemize}
// /Errors:
//   none
//  
// /Author: WDH
// /Date: 1995
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}

// Here are some free comments -- use //\no function header:

//\begin{>>testInclude.tex}{\subsection{Public Data members}} 
//\no function header:
// 
//  Here are the public data members:
// /intR numberOfDimensions:    equals value found in grid, this is here for convenience
// /MappedGrid *grid: pointer to the MappedGrid
// /MappedGridOperators *operators:  pointer to operators
//
//\end{testInclude.tex} 


//\begin{>>testInclude.tex}{\subsection{gradient}} 
MappedGrid::int MappedGridOperators::
gradient(realArray & x,          /* = nullRealArray */
	 const intArray & y,     // = nullIntArray
	 const floatArray & z,   // = nullFloatArray
         real a /* = 1. */, 
         real b /* = 2. */ ) :  baseGradient(),
    x(1.), y(2.)
//=========================================================================================
//
// /Purpose:
//   The purpose of this routine is to compute the gradient of a realArray.
//
//    /x(all,all,all,numberOfDimensions) (input) : the vector x is used to compute
//	    the gradient.
//    /y (output) : the vector y
//    /z (input/output) : this vector is never really used but would otherwise
//		be equal to $x^y$. 
//
// /Return Values: ~
//   \begin{itemize}
//     \item 0 : success
//     \item 1 : error
//   \end{itemize}
// /Errors:
//   none
//  
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}

//\begin{>>testInclude.tex}{\subsection{laplacian}} 
int MappedGrid::laplacian(realArray & x, const intArray & y, const floatArray & z )
:  i(1), j(2)
//=========================================================================================
//
// /Purpose:
//   The purpose of this routine is to compute the laplacian of a realArray.
//
//   /x(all,all,all,numberOfDimensions) (input) : the vector x is used to compute
//          the laplacian.
//     \begin{verbatim}  
//         // some sample code:
//           z=laplacian(x);
//     \end{verbatim}  
//   /y (output) : the vector y
//   /z (input/output) : this vector is never really used but would otherwise
//              be equal to $x^y$. 
//
// /Return Values: ~
//   \begin{itemize}
//     \item 0 : success
//     \item 1 : error
//   \end{itemize}
// /Errors:
//   none
//  
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}


//\begin{>>testInclude.tex}{\subsection{Div}} 
int Div(realArray & x, const intArray & y, const floatArray & z,
     doubleArray & c)
//=========================================================================================
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}



//\begin{>>testInclude.tex}{\subsection{aabb}} 
int aabb(realArray & x, const intArray & y, const floatArray & z,
     doubleArray & c)
//=========================================================================================
// Purpose : hi.
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}




//\begin{>>testInclude.tex}{\subsection{Div2}} 
int Div2(realArray & x, const intArray & y, const floatArray & z,
     doubleArray & c)
//=========================================================================================
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}

//\begin{>>testInclude.tex}{\subsection{Div2}} 
int Test::
Div2(realArray & x, const intArray & y, const floatArray & z,
     doubleArray & c)
//=========================================================================================
//\end{testInclude.tex} 
//=========================================================================================
{
  for( int i=0; i<1; i++ )
  {
  }
}

//\begin{>>testInclude.tex}{\subsection{destructor}} 
MappedGridFunction::
~MappedGridFunction()
//=========================================================================================
//\end{testInclude.tex} 
//=========================================================================================
{
}

//\begin{>>testInclude.tex}{\subsection{destructor}} 
MappedGridFunction::~MappedGridFunction()
//=========================================================================================
//\end{testInclude.tex} 
//=========================================================================================
{
}

//\begin{>>testInclude.tex}{\subsection{Public Member Functions}}
//\no function header:
//\end{OrderedTriple.tex}

//\begin{>>testInclude.tex}{\subsubsection{Constructor}}
template<class T>
OrderedTriple<T>::
OrderedTriple()
        :mOrderedTripleIsSet(false)
//================================================================
// /Description:  Default constructor for OrderedTriple.
// /Return Values: None.
//
// /Errors:
//   None.
//
// /Author:  BJM
// /Date:  21 May 1999
//\end{OrderedTriple.tex}
//================================================================
{}



//\begin{>>testInclude.tex}{\subsubsection{Operator$<$}}
template<class T>
bool
OrderedTriple<T>::operator<(const OrderedTriple &rhs)
//================================================================
// /Description:  Less than operator for OrderedTriple.  
// 
// /Errors:  Warning given if one is not 'set'.
//
// /Author:  BJM
// /Date:  2 June 1999
//\end{testInclude.tex}
//================================================================
{
    if( (mOrderedTripleIsSet==false) || (rhs.mOrderedTripleIsSet==false))
    {
        cout<<" one or both of OrderedTriples is not set in < operator."<<endl;
        return false;
    }
   
    if(mK<rhs.mK)
    {
        return true;
    }
    if((mK==rhs.mK)&&(mJ<rhs.mJ))
    {
        return true;
    }
    if((mK==rhs.mK)&&(mJ==rhs.mJ)&&(mI<rhs.mI))
    {
        return true;
    }

    return false;
}

//\begin{>>testInclude.tex}{\subsubsection{Operator$<$}}
template<class T>
bool
OrderedTriple<T>::
operator<(const OrderedTriple &rhs) const
//================================================================
// /Description:  
// 
// /Errors:  Warning given if one is not 'set'.
//
// /Author:  BJM
// /Date:  2 June 1999
//\end{testInclude.tex}
//================================================================
{
    if( (mOrderedTripleIsSet==false) || (rhs.mOrderedTripleIsSet==false))
    {
        cout<<" one or both of OrderedTriples is not set in < operator."<<endl;
        return false;
    }
    
    if(mK<rhs.mK)
    {
        return true;
    }
    if((mK==rhs.mK)&&(mJ<rhs.mJ))
    {
        return true;
    }
    if((mK==rhs.mK)&&(mJ==rhs.mJ)&&(mI<rhs.mI))
    {
        return true;
    }

    return false;
}



//\begin{>>testInclude.tex}{\subsection{Postfix Increment}}
GeometricADT::iterator
GeometricADT::iterator::
operator++(int)
// =====================================================================
// /Purpose : increment the iterator (advance the iteration) and then
return the old value
//\end{testInclude.tex}
{
}


//\begin{>>testInclude.tex}{\subsection{Class myClass}}
class myClass : public yourClass
// =====================================================================
// /Purpose : Define a class.
//  /initialize : a member function
//\end{testInclude.tex}
// =====================================================================
{
}
