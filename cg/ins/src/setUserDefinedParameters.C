#include "InsParameters.h"

//   F77_BLANK_COMMON
//      will expand as follows:
//      All Unix _BLNK__
//  extern struct {int i,j,k;} F77_BLANK_COMMON;



//\begin{>>Parameters.tex}{\subsection{setUserDefinedParameters}}  
int InsParameters::
setUserDefinedParameters()  // allow user defined pdeParameters to be passed to C or Fortran routines.
// ==============================================================================================
//  /Description:
//     This function is used to pass user defined pdeParameters to C or Fortran routines.
//   In the case of Fortran we assign common block variables by making the common block look like
//  a struct. 
//\end{ParametersInclude.tex}  
// ==============================================================================================
{


  return 0;
}



