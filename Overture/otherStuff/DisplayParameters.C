#include "DisplayParameters.h"

//\begin{>DisplayParametersInclude.tex}{\subsection{constructor}}
DisplayParameters::
DisplayParameters()
// =======================================================================================
// /Description:
//   Build a DisplayParameters object.
//\end{DisplayParametersInclude.tex}
// =======================================================================================
{
  assert(MAX_ARRAY_DIMENSION >= 4 );
  file=stdout;
  for( int i=0; i<MAX_ARRAY_DIMENSION; i++ )
  {
    indexLabel[i]=true;
    stride[i]=1;
  }
  
  ordering=+1;
  iFormat="%6i ";
  fFormat="%11.4e ";
  dFormat="%11.4e ";
}

DisplayParameters::
~DisplayParameters()
{
}

//\begin{>>DisplayParametersInclude.tex}{\subsection{set(FILE)}}
int DisplayParameters::
set(FILE *file_)
// =======================================================================================
// /Description:
//   Specify a file to use to write the output to. This file must already be open. 
// /file_ (input) : use this file, if {\tt file_=NULL} then the standard output is used (stdout).
//\end{DisplayParametersInclude.tex}
// =======================================================================================
{
  file = file_==NULL ? stdout : file_;
  return 0;
}

//\begin{>>DisplayParametersInclude.tex}{\subsection{set}}
int DisplayParameters::
set(const DisplayOption & displayOption )
// =======================================================================================
// /Description:
//   Assign a value to an option. (Those options taking no values).
//\end{DisplayParametersInclude.tex}
// =======================================================================================
{
  int i;
  switch (displayOption)
  {
    case labelAllIndicies:    // put comments to number all indicies
      for( i=0; i<MAX_ARRAY_DIMENSION; i++ )
	indexLabel[i]=true;
      break;
    case labelNoIndicies:     // no comments
      for( i=0; i<MAX_ARRAY_DIMENSION; i++ )
	indexLabel[i]=false;
      break;
    case forwardOrdering:     // print index 0, then index 1, ...
      ordering=+1;
      break;
    case backwardOrdering:    // print last index first, second-last next, ...
      ordering=-1;
      break;
  default:
    cout << "ERROR: DisplayParameters::set(DisplayOption): the displayOption =" << displayOption 
         << ", cannot be set with no arguments \n";
    return 1;
  }
  return 0;
}


int DisplayParameters::
set(const DisplayOption & displayOption, const int & value )
// =======================================================================================
// /Description:
//   Assign a value to an option. (Those options taking an int).
//\end{DisplayParametersInclude.tex}
// =======================================================================================
{
  int i;
  switch (displayOption)
  {
    case strideForAllIndicies:   // print each index from bound,...,base 
      if( value==0 )
        for( i=0; i<MAX_ARRAY_DIMENSION; i++ )
	  stride[i]=1;
      else
        for( i=0; i<MAX_ARRAY_DIMENSION; i++ )
	  stride[i]=value;
      break;
  default:
    cout << "ERROR: DisplayParameters::set(DisplayOption,int): the displayOption =" << displayOption 
         << ", cannot be set by a int \n";
    return 1;
  }
  return 0;
}

int  DisplayParameters::
set(const DisplayOption & displayOption, const aString & value )
// =======================================================================================
// /Description:
//   Assign a value to an option. (Those options taking a aString).
//\end{DisplayParametersInclude.tex}
// =======================================================================================
{
  switch (displayOption)
  {
  case intFormat:           // set format for int's
    iFormat=value;
    break;
  case floatFormat:         // set format for float's
    fFormat=value;
    break;
  case doubleFormat:        // set format for double's
    dFormat=value;
    break;
  default:
    cout << "ERROR: DisplayParameters::set(DisplayOption,aString): the displayOption =" << displayOption 
         << ", cannot be set by a aString \n";
    return 1;
  }
  return 0;

}


int DisplayParameters::
set(const DisplayOption & displayOption, const int & index, const int & value )
// =======================================================================================
// /Description:
//   Assign a value to an option and a particular index. 
// 
// /displayOption==
//\end{DisplayParametersInclude.tex}
// =======================================================================================
{
  if( index<0 || index > MAX_ARRAY_DIMENSION-1 )
  {
    cout << "ERROR: DisplayParameters::set(DisplayOption,int,int): the index value =" << index 
	 << ", must be in the range " << 0 << " to " << MAX_ARRAY_DIMENSION-1 << endl;
    return 1;
  }
  switch (displayOption)
  {
  case labelIndex:          // indicate which indicies to label or not.
    indexLabel[index]=value;
    break;
  case indexStride:
    if( value==0 )
      stride[index]=1;
    else
      stride[index]=value;
    break;
  default:
    cout << "ERROR: DisplayParameters::set(DisplayOption,int,int): the displayOption =" << displayOption 
         << ", cannot be set by an index and a value \n";
    return 1;
  }
  
  return 0;
}
