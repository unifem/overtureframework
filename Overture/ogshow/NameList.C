#include "NameList.h"

//\begin{>NameListInclude.tex}{\subsubsection{constructor}} 
NameList::
NameList()
//----------------------------------------------------------------------
// /Description:
//   Build a NameList object that can be used to selectively change parameters.
//  /Author: WDH
//\end{NameListInclude.tex}
//----------------------------------------------------------------------
{
}

NameList::
~NameList()
//----------------------------------------------------------------------
// /Description:
//  /Author: WDH
//\end{NameListInclude.tex}
//----------------------------------------------------------------------
{
}


//\begin{>NameListInclude.tex}{\subsubsection{getVariableName}} 
void NameList::
getVariableName( aString & answer, aString & name )
//=================================================================
// /Description:
//   Parse the aString "answer" and return the variable name:
// \begin{verbatim}
//     answer: "method=4"         -> name="method"
//           : "array(5,6)=56.7"  -> name="array"
// \end{verbatim}
// /answer (input) : string to parse.
// /name (output) : string before the first "=" sign
//\end{NameListInclude.tex}
//=================================================================
{
  int j=0;
  // skip initial blanks
  for( j=0; j<answer.length(); j++ )
  {
    if( answer[j]!=' ' )
      break;
  }
  int k=j;
  for( int i=j; i<answer.length(); i++ )
  {
    if( answer[i]=='=' || answer[i]=='(' || answer[i]==' ' ) break;
    k=i;
    // printf("answer[%i]=",i); cout << answer[i] << endl;
  }
  name=subString(answer,j,k);
  
  // cout << "Parse: answer=[" << answer << "], name =[" << name << "]" << endl;
}

//\begin{>>NameListInclude.tex}{\subsubsection{intValue}} 
int NameList::
intValue( aString & answer )
//===================================================================
// /Description:
//  Return an int for a string of the form "name=int"
// /answer (input) : a aString of the form "name=int"
// /Return value: The value of the rhs in "name=int"
//\end{NameListInclude.tex}
//===================================================================
{
  char buf[80];  // use new and delete
  for( int i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  char *rest;
  rest = strchr(buf,'='); 
  if( rest==0 )
  {
    printf("NameList:ERROR: answer=`%s' does not contain an = sign. Returning zero as the value.\n",
      (const char*)answer);
    return 0;
  }
  rest++;  // find '=' in the string
  //  cout << "rest=[" << rest << "]" << endl; 
  //  cout << "atoi(rest) = " << atoi(rest) << endl;
  return atoi(rest);

}

//\begin{>>NameListInclude.tex}{\subsubsection{realValue}} 
real NameList::
realValue( aString & answer )
//===================================================================
// /Description:
//  Return a real for a string of the form "name=real"
// /answer (input) : a aString of the form "name=real"
// /Return value: The value of the rhs in "name=real"
//\end{NameListInclude.tex}
//===================================================================
{
  char buf[80];
  for( int i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  char *rest;
  rest = strchr(buf,'=');  
  if( rest==0 )
  {
    printf("NameList:ERROR: answer=`%s' does not contain an = sign. Returning zero as the value.\n",
      (const char*)answer);
    return 0.;
  }
  rest++; // find '=' in the string
  //  cout << "rest=[" << rest << "]" << endl; 
  //cout << "atof(rest) = " << atof(rest) << endl;
  return atof(rest);

}


//\begin{>>NameListInclude.tex}{\subsubsection{getIntArray}} 
int NameList::
getIntArray( aString & answer, IntegerArray & a )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i0,i1,i2,i3;
  return getIntArray( answer,a,i0,i1,i2,i3 );
}  

//\begin{>>NameListInclude.tex}{} 
int NameList::
getIntArray( aString & answer, IntegerArray & a, int & i0 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i1,i2,i3;
  return getIntArray( answer,a,i0,i1,i2,i3 );
}  

//\begin{>>NameListInclude.tex}{} 
int NameList::
getIntArray( aString & answer, IntegerArray & a, int & i0, int & i1 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i2,i3;
  return getIntArray( answer,a,i0,i1,i2,i3 );
}  

//\begin{>>NameListInclude.tex}{} 
int NameList::
getIntArray( aString & answer, IntegerArray & a, int & i0, int & i1, int & i2 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i3;
  return getIntArray( answer,a,i0,i1,i2,i3 );
}  


//\begin{>>NameListInclude.tex}{} 
int NameList::
getIntArray( aString & answer, IntegerArray & a, int & i0, int & i1, int & i2, int & i3 )
//=====================================================================
// /Description: 
// Assign the value in an IntegerArray from a string of
// one of the following forms
// \begin{verbatim}
//      name=value
//      name(i0)=value  
//      name(i0,i1)=value
//      name(i0,i1,i2)=value
//      name(i0,i1,i2,i3)=value
// \end{verbatim}
// /answer (input) : a aString of one of the above forms
// /a (output) : an array that is to be assigned
// /i0,i1,i2,i3 (output) : Return the values for the indices used in evaluating the array.
// /Return value: Return TRUE if successful
//\end{NameListInclude.tex}
//====================================================================
{
  int value;

  char buf[80];
  int i;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  // first get the value:
  char* rest;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "getIntArray: Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  value=atoi(rest);


  // now look for indices
  IntegerArray index(4);
  for( i=0; i<4; i++ )
    index(i)=a.getBase(i);  // set default values
    
  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    a(index(0),index(1),index(2),index(3))=value;
    i0=index(0); i1=index(1); i2=index(2); i3=index(3);
    return TRUE;
  }
  
  for( i=0; i<4; i++ )
  {
    rest++;
    index(i) = atoi(rest);
    if( index(i)>a.getBound(i) )
    {
      cerr << "getIntArray: index(" << i << ")=" << index(i) << " out of bounds for " 
           << answer << endl;
      return FALSE;
    }    

    rest= strchr(rest,','); 
    if( i==3 || rest==NULL )
    {
      a(index(0),index(1),index(2),index(3))=value;
      i0=index(0); i1=index(1); i2=index(2); i3=index(3);
      return TRUE;
    }
  }
  return TRUE;
}


//\begin{>>NameListInclude.tex}{\subsubsection{getRealArray}} 
int NameList::
getRealArray( aString & answer, RealArray & a )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i0,i1,i2,i3;
  return getRealArray( answer,a,i0,i1,i2,i3 );
}  

//\begin{>>NameListInclude.tex}{}
int NameList::
getRealArray( aString & answer, RealArray & a, int & i0 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i1,i2,i3;
  return getRealArray( answer,a,i0,i1,i2,i3 );
}  

//\begin{>>NameListInclude.tex}{}
int NameList::
getRealArray( aString & answer, RealArray & a, int & i0, int & i1 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i2,i3;
  return getRealArray( answer,a,i0,i1,i2,i3 );
}  

//\begin{>>NameListInclude.tex}{}
int NameList::
getRealArray( aString & answer, RealArray & a, int & i0, int & i1, int & i2 )
//=====================================================================
// /Description: Assign values of a RealArray. see the documentation for
//  getIntArray.
//\end{NameListInclude.tex}
//====================================================================
{
  int i3;
  return getRealArray( answer,a,i0,i1,i2,i3 );
}  


//\begin{>>NameListInclude.tex}{}
int NameList::
getRealArray( aString & answer, RealArray & a, int & i0, int & i1, int & i2, int & i3 )
//=====================================================================
// /Description: Assign values of a RealArray. see the documentation for
//  getIntArray.
//\end{NameListInclude.tex}
//====================================================================
{
  real value;

  char buf[80];
  int i;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  // first get the value:
  char* rest;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "getRealArray: Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  // cout << "rest=[" << rest << "]" << endl; 
  // cout << "atof(rest) = " << atof(rest) << endl;
  value=atof(rest);


  // now look for indices
  IntegerArray index(4);
  for( i=0; i<4; i++ )
    index(i)=a.getBase(i);  // set default values
    
  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    a(index(0),index(1),index(2),index(3))=value;
    i0=index(0); i1=index(1); i2=index(2); i3=index(3);
    return TRUE;
  }
  
  for( i=0; i<4; i++ )
  {
    rest++;
    index(i) = atoi(rest);
    // cout << " index(" << i << ") = " << index(i) << endl;
    if( index(i)>a.getBound(i) )
    {
      cerr << "getRealArray: index(" << i << ")=" << index(i) << " out of bounds for " 
           << answer << endl;
      return FALSE;
    }    

    rest= strchr(rest,','); 
    if( i==3 || rest==NULL )
    {
      a(index(0),index(1),index(2),index(3))=value;
      i0=index(0); i1=index(1); i2=index(2); i3=index(3);
      return TRUE;
    }
  }
  return TRUE;
}

//\begin{>>NameListInclude.tex}{\subsubsection{intArrayValue}}
int NameList::
intArrayValue( aString & answer, int & value, int & i0 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i1,i2,i3;
  return intArrayValue(answer,value,1, i0,i1,i2,i3);
}
//\begin{>>NameListInclude.tex}{}
int NameList::
intArrayValue( aString & answer, int & value, int & i0 , int & i1)
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i2,i3;
  return intArrayValue(answer,value,2, i0,i1,i2,i3);
}
//\begin{>>NameListInclude.tex}{}
int NameList::
intArrayValue( aString & answer, int & value, int & i0, int & i1, int & i2)
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i3;
  return intArrayValue(answer,value,3, i0,i1,i2,i3);
}
//\begin{>>NameListInclude.tex}{}
int NameList::
intArrayValue( aString & answer, int & value, int & i0, int & i1, int & i2, int & i3)
//=====================================================================
// /Description:
// Return value and indices i0,i1,i2,i3 from a string of the form
// \begin{verbatim}
//   name(i0)         =value  : intArrayValue(answer, value, i0 )
//   name(i0,i1)      =value  : intArrayValue(answer, value, i0, i1)
//   name(i0,i1,i2)   =value  : intArrayValue(answer, value, i0, i1, i2)
//   name(i0,i1,i2,i3)=value  : intArrayValue(answer, value, i0, i1, i2, i3)
// \end{verbatim}
// /answer (input) : string to parse.
// /value (output) : value found on the rhs of the string
// /i0,i1,i2,i3 (output) : index values
// /Return value :  Return TRUE if successful
//\end{NameListInclude.tex}
//====================================================================
{
  return intArrayValue(answer,value,4, i0,i1,i2,i3);
}

   
int NameList::
intArrayValue( aString & answer, int & value, const int & numberExpected, int & i0, int & i1, int & i2, int & i3 )
  //--------------------------------------------------------------------
  // Return value and i0,i1,i2,i3 from a string of the form
  //      name(i0,i1,i2,i3)=value  
  //  Return TRUE if successful
  //
//\end{NameListInclude.tex}
  //-------------------------------------------------------------------
{
  char buf[80];
  int i;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  char* rest;

  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    cerr << "Format error in answer = " << answer << endl;
    return FALSE;
  }

  int ii[4];
  for( i=0; i<numberExpected; i++ )
  {
    rest++;
    ii[i] = atoi(rest);
    if( i<numberExpected-1 )
    {
      rest= strchr(rest,','); 
      if( rest==NULL )
      {
	cerr << "Format error in answer = " << answer << endl;
	return FALSE;
      }
    }
  }

  i0=ii[0]; 
  i1=ii[1];
  i2=ii[2];
  i3=ii[3];
  
  rest++;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  value=atoi(rest);
  return TRUE;
  
}

//\begin{>>NameListInclude.tex}{\subsubsection{realArrayValue}}
int NameList::
realArrayValue( aString & answer, real & value, int & i0 )
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i1,i2,i3;
  return realArrayValue(answer,value,1, i0,i1,i2,i3);
}
//\begin{>>NameListInclude.tex}{}
int NameList::
realArrayValue( aString & answer, real & value, int & i0 , int & i1)
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i2,i3;
  return realArrayValue(answer,value,2, i0,i1,i2,i3);
}
//\begin{>>NameListInclude.tex}{}
int NameList::
realArrayValue( aString & answer, real & value, int & i0, int & i1, int & i2)
//=====================================================================
//\end{NameListInclude.tex}
//====================================================================
{
  int i3;
  return realArrayValue(answer,value,3, i0,i1,i2,i3);
}
//\begin{>>NameListInclude.tex}{}
int NameList::
realArrayValue( aString & answer, real & value, int & i0, int & i1, int & i2, int & i3)
//=====================================================================
// /Description:
// Return value and indices i0,i1,i2,i3 from a string of the form
// \begin{verbatim}
//   name(i0)         =value  : realArrayValue(answer, value, i0 )
//   name(i0,i1)      =value  : realArrayValue(answer, value, i0, i1)
//   name(i0,i1,i2)   =value  : realArrayValue(answer, value, i0, i1, i2)
//   name(i0,i1,i2,i3)=value  : realArrayValue(answer, value, i0, i1, i2, i3)
// \end{verbatim}
// /answer (input) : string to parse.
// /value (output) : value found on the rhs of the string
// /i0,i1,i2,i3 (output) : index values
// /Return value :  Return TRUE if successful
//\end{NameListInclude.tex}
//====================================================================
{
  return realArrayValue(answer,value,4, i0,i1,i2,i3);
}

   
int NameList::
realArrayValue( aString & answer, real & value, const int & numberExpected, int & i0, int & i1, int & i2, int & i3 )
  //--------------------------------------------------------------------
  // Return value and i0,i1,i2,i3 from a string of the form
  //      name(i0,i1,i2,i3)=value  
  //  Return TRUE if successful
  //
//\end{NameListInclude.tex}
  //-------------------------------------------------------------------
{
  char buf[80];
  int i;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  char* rest;

  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    cerr << "Format error in answer = " << answer << endl;
    return FALSE;
  }

  int ii[4];
  for( i=0; i<numberExpected; i++ )
  {
    rest++;
    ii[i] = atoi(rest);
    if( i<numberExpected-1 )
    {
      rest= strchr(rest,','); 
      if( rest==NULL )
      {
	cerr << "Format error in answer = " << answer << endl;
	return FALSE;
      }
    }
  }

  i0=ii[0]; 
  i1=ii[1];
  i2=ii[2];
  i3=ii[3];
  
  rest++;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  value=atof(rest);
  return TRUE;
  
}

aString NameList::
getString(aString & answer)
// ========================================================
// Return the string that follows the first "=" sign
// ========================================================
{
  char buf[80];  // use new and delete
  for( int i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  char *rest;
  rest = strchr(buf,'='); rest++;  // find '=' in the string
  return rest;
}

aString & NameList::
subString( aString & s, const int i1, const int i2 )
{
  char buf[80]; // use new and delete
  for( int i=i1; i<=i2; i++ )
    buf[i-i1]=s[i];
  buf[i2-i1+1]='\0';
  
//  strncpy(buf,((char*)s)[i1],i2-i1+1);
//  cout << "buff = " << buf << endl;
  return *(new aString(buf));
}

int NameList::
arrayEqualsName( aString & answer, const aString nameList[], IntegerArray & a)
//\end{NameListInclude.tex}
{
  int i0,i1,i2,i3;
  return arrayEqualsName(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayEqualsName( aString & answer, const aString nameList[], IntegerArray & a, int & i0)
//\end{NameListInclude.tex}
{
  int i1,i2,i3;
  return arrayEqualsName(answer,nameList,a,i0,i1,i2,i3);
}

int NameList::
arrayEqualsName( aString & answer, const aString nameList[], IntegerArray & a, int & i0, int & i1)
//\end{NameListInclude.tex}
{
  int i2,i3;
  return arrayEqualsName(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayEqualsName( aString & answer, const aString nameList[], IntegerArray & a, int & i0, int & i1, int & i2)
//\end{NameListInclude.tex}
{
  int i3;
  return arrayEqualsName(answer,nameList,a,i0,i1,i2,i3);
}

//\begin{>>NameListInclude.tex}{\subsubsection{arrayEqualsName}}
int NameList::
arrayEqualsName(aString & answer, 
		const aString nameList[], 
		IntegerArray & a, 
		int & i0, /* optional argument */
		int & i1, /* optional argument */
		int & i2, /* optional argument */
		int & i3  /* optional argument */ )
//=====================================================================
// /Description:
//    The aString answer should be of the form of one of
//    \begin{itemize}
//       \item arrayName(i0)=name
//       \item arrayName(i0,i1)=name
//       \item arrayName(i0,i1,i2)=name
//       \item arrayName(i0,i1,i2,i3)=name
//    \end{itemize}
//   and the result of this function will be to set
//    \begin{itemize}
//       \item a(i0)=value where nameList[value]==name
//       \item a(i0,i1)=value where nameList[value]==name
//       \item a(i0,i1,i2)=value where nameList[value]==name
//       \item a(i0,i1,i2,i3)=value where nameList[value]==name
//    \end{itemize}
//
//    
// /answer (input) : a aString that should be of the form shown above.
// /nameList (input) : a null terminated array of names. These names will
//    appear on the right hand side of the equals sign.
// /a (output): assign a value into this array.
// /i0,i1,i2,i3 (ouput) : optional arguments, return the values used in assigning a.
// /Return values: return TRUE if successful
//\end{NameListInclude.tex}
//====================================================================
{
  char buf[80];
  int i;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  // first get the potential enum name
  char* rest;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "getIntArray: Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  aString name = rest;

  int value=-1;
  for( i=0; nameList[i]!=""; i++ )
  {
    if( name==nameList[i] )
    {
      value=i;
      break;
    }
  }
  if( value==-1 )
  {
    printf(" NameList::arrayEqualsName:ERROR: unable to find name =[%s] in the list of enum Names! no change made \n",
            (const char *) name);
    printf("here are the known names: \n");
    for( i=0; nameList[i]!=""; i++ )
      printf(" %i: %s \n",i,(const char *)nameList[i]);
    return 1;
  }

  // now look for indices
  IntegerArray index(4);
  for( i=0; i<4; i++ )
    index(i)=a.getBase(i);  // set default values
    
  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    a(index(0),index(1),index(2),index(3))=value;
    i0=index(0); i1=index(1); i2=index(2); i3=index(3);
    return TRUE;
  }
  
  for( i=0; i<4; i++ )
  {
    rest++;
    index(i) = atoi(rest);
    if( index(i)>a.getBound(i) )
    {
      cerr << "getIntArray: index(" << i << ")=" << index(i) << " out of bounds for " 
           << answer << endl;
      return FALSE;
    }    

    rest= strchr(rest,','); 
    if( i==3 || rest==NULL )
    {
      a(index(0),index(1),index(2),index(3))=value;
      i0=index(0); i1=index(1); i2=index(2); i3=index(3);
      return TRUE;
    }
  }
  return TRUE;
}





//\begin{>>NameListInclude.tex}{\subsubsection{arrayOfNameEqualsValue}}
int NameList::
arrayOfNameEqualsValue(aString & answer, 
		const aString nameList[], 
		IntegerArray & a, 
		int & i0, /* optional argument */
		int & i1, /* optional argument */
		int & i2, /* optional argument */
		int & i3  /* optional argument */)
//=====================================================================
// /Description:
//    The aString answer should be of the form of one of
//    \begin{itemize}
//       \item arrayName(name0)=value
//       \item arrayName(name0,name1)=value
//       \item arrayName(name0,name1,name2)=value
//       \item arrayName(name0,name1,name2,name3)=value
//    \end{itemize}
//   and the result of this function will be to set
//    \begin{itemize}
//       \item a(i0)=value where nameList[i0]==name0
//       \item a(i0,i1)=value where nameList[i0]==name0, nameList[i1]==name1
//       \item a(i0,i1,i2)=value where nameList[i0]==name0, nameList[i1]==name1,...
//       \item a(i0,i1,i2,i3)=value where nameList[i0]==name0, nameList[i1]==name1,...
//    \end{itemize}
//
//    
// /answer (input) : a aString that should be of the form shown above.
// /nameList (input) : a null terminated array of names. These names will
//    appear as array arguements in answer.
// /a (output): assign a value into this array.
// /i0,i1,i2,i3 (ouput) : optional arguments, return the values used in assigning a.
// /Return values: return TRUE if successful
//\end{NameListInclude.tex}
//====================================================================
{
  char buf[80];
  int i,j,k;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  // first get the value to the right of the equals sign
  char* rest;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "getIntArray: Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  int value=atoi(rest);
  // printf("arrayOfName=value: value=%i \n",value);

  // now look for index names
  IntegerArray index(4);
  for( i=0; i<4; i++ )
    index(i)=a.getBase(i);  // set default values
    
  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    a(index(0),index(1),index(2),index(3))=value;
    i0=index(0); i1=index(1); i2=index(2); i3=index(3);
    return TRUE;
  }
  aString indexName;   // this will hold name0 or name1, ...
  for( i=0; i<4; i++ )
  {
    rest++;  // rest points to name0,name1,...
    indexName=rest;
    bool found=FALSE;
    for( j=0; indexName[j]; j++ )
    {
      if( indexName[j]==',' )
      {
        found=TRUE;
	indexName=indexName(0,j-1);
	break;
      }
      if( indexName[j]==')' )
      {
        found=TRUE;
	indexName=indexName(0,j-1);
	rest=NULL;
        break;
      }
      rest++;
    }
    if( found )    
    {
      // cout << "indexName = " << indexName << endl;
      // look for a match in nameList      
      index(i)=a.getBase(i)-1;
      for( k=0; nameList[k]!=""; k++ )
      {
	if( indexName==nameList[k] )
	{
	  index(i)=k;
	  break;
	}
      }
      if( index(i)==a.getBase(i)-1 )
      {
	printf(" NameList::arrayOfNameEqualsValue:ERROR: unable to find name =[%s] in the nameList! no change made \n",
	       (const char *) indexName);
	printf("here are the known names: \n");
	for( i=0; nameList[i]!=""; i++ )
	  printf(" %i: %s \n",i,(const char *)nameList[i]);
	return 1;
      }
      if( index(i)>a.getBound(i) )
      {
	cerr << "getIntArray: index(" << i << ")=" << index(i) << " out of bounds for " 
	     << answer << endl;
	return FALSE;
      }    
    }
    else
      rest=NULL;
    if( i==3 || rest==NULL )
    {
      a(index(0),index(1),index(2),index(3))=value;
      i0=index(0); i1=index(1); i2=index(2); i3=index(3);
      return TRUE;
    }

  }
  return TRUE;
}




int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], IntegerArray & a, int & i0, int & i1, int & i2)
//\end{NameListInclude.tex}
{
  int i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], IntegerArray & a, int & i0, int & i1)
//\end{NameListInclude.tex}
{
  int i2,i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], IntegerArray & a, int & i0)
//\end{NameListInclude.tex}
{
  int i1,i2,i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], IntegerArray & a)
//\end{NameListInclude.tex}
{
  int i0,i1,i2,i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}

//\begin{>>NameListInclude.tex}{\subsection{arrayOfNameEqualsValue}}
int NameList::
arrayOfNameEqualsValue(aString & answer, 
		const aString nameList[], 
		RealArray & a, 
		int & i0, 
		int & i1, 
		int & i2, 
		int & i3)
//=====================================================================
// /Description:
//    The aString answer should be of the form of one of
//    \begin{itemize}
//       \item arrayName(name0)=value
//       \item arrayName(name0,name1)=value
//       \item arrayName(name0,name1,name2)=value
//       \item arrayName(name0,name1,name2,name3)=value
//    \end{itemize}
//   and the result of this function will be to set
//    \begin{itemize}
//       \item a(i0)=value where nameList[i0]==name0
//       \item a(i0,i1)=value where nameList[i0]==name0, nameList[i1]==name1
//       \item a(i0,i1,i2)=value where nameList[i0]==name0, nameList[i1]==name1,...
//       \item a(i0,i1,i2,i3)=value where nameList[i0]==name0, nameList[i1]==name1,...
//    \end{itemize}
//
//    
// /answer (input) : a aString that should be of the form shown above.
// /nameList (input) : a null terminated array of names. These names will
//    appear as array arguements in answer.
// /a (output): assign a value into this array.
// /i0,i1,i2,i3 (ouput) : optional arguments, return the values used in assigning a.
// /Return values: return TRUE if successful
//\end{NameListInclude.tex}
//====================================================================
{
  char buf[80];
  int i,j,k;
  for( i=0; i< answer.length(); i++ )
    buf[i]=answer[i];
  buf[answer.length()]='\0';

  // first get the value to the right of the equals sign
  char* rest;
  rest = strchr(buf,'=');  
  if( rest==NULL )
  {
    cerr << "getIntArray: Format error in answer = " << answer << endl;
    return FALSE;
  }
  rest++;
  real value=atof(rest);
  // printf("arrayOfName=value: value=%i \n",value);

  // now look for index names
  IntegerArray index(4);
  for( i=0; i<4; i++ )
    index(i)=a.getBase(i);  // set default values
    
  rest= strchr(buf,'('); 
  if( rest==NULL )
  {
    a(index(0),index(1),index(2),index(3))=value;
    i0=index(0); i1=index(1); i2=index(2); i3=index(3);
    return TRUE;
  }
  aString indexName;   // this will hold name0 or name1, ...
  for( i=0; i<4; i++ )
  {
    rest++;  // rest points to name0,name1,...
    indexName=rest;
    bool found=FALSE;
    for( j=0; indexName[j]; j++ )
    {
      if( indexName[j]==',' )
      {
        found=TRUE;
	indexName=indexName(0,j-1);
	break;
      }
      if( indexName[j]==')' )
      {
        found=TRUE;
	indexName=indexName(0,j-1);
	rest=NULL;
        break;
      }
      rest++;
    }
    if( found )    
    {
      // cout << "indexName = " << indexName << endl;
      // look for a match in nameList      
      index(i)=a.getBase(i)-1;
      for( k=0; nameList[k]!=""; k++ )
      {
	if( indexName==nameList[k] )
	{
	  index(i)=k;
	  break;
	}
      }
      if( index(i)==a.getBase(i)-1 )
      {
	printf(" NameList::arrayOfNameEqualsValue:ERROR: unable to find name =[%s] in the nameList! no change made \n",
	       (const char *) indexName);
	printf("here are the known names: \n");
	for( i=0; nameList[i]!=""; i++ )
	  printf(" %i: %s \n",i,(const char *)nameList[i]);
	return 1;
      }
      if( index(i)>a.getBound(i) )
      {
	cerr << "getIntArray: index(" << i << ")=" << index(i) << " out of bounds for " 
	     << answer << endl;
	return FALSE;
      }    
    }
    else
      rest=NULL;
    if( i==3 || rest==NULL )
    {
      a(index(0),index(1),index(2),index(3))=value;
      i0=index(0); i1=index(1); i2=index(2); i3=index(3);
      return TRUE;
    }

  }
  return TRUE;
}




int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], RealArray & a, int & i0, int & i1, int & i2)
{
  int i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], RealArray & a, int & i0, int & i1)
{
  int i2,i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], RealArray & a, int & i0)
{
  int i1,i2,i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}
int NameList::
arrayOfNameEqualsValue( aString & answer, const aString nameList[], RealArray & a)
{
  int i0,i1,i2,i3;
  return arrayOfNameEqualsValue(answer,nameList,a,i0,i1,i2,i3);
}

