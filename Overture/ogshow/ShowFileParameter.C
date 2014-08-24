#include "ShowFileParameter.h"

// This class defines parameters for use with a show file.
// Parameters of type int, real or string can be created.


ShowFileParameter::
ShowFileParameter()
// ======================================================================
// /Description:
//    Default constructor
// ======================================================================
{ type=intParameter; name="unknown"; ivalue=0; rvalue=0;  }

ShowFileParameter::
ShowFileParameter(const aString & name, int value)
// ======================================================================
// /Description:
//    Build an "int" show file parameter with a given name.
// ======================================================================
{ 
  set(name,value);
}


ShowFileParameter::
ShowFileParameter(const aString & name, real value)
// ======================================================================
// /Description:
//    Build a "real" show file parameter with a given name.
// ======================================================================
{ 
  set(name,value);
}

ShowFileParameter::
ShowFileParameter(const aString & name, const aString & value)
// ======================================================================
// /Description:
//    Build a "string" show file parameter with a given name.
// ======================================================================
{ 
  set(name,value);
}

ShowFileParameter::
~ShowFileParameter(){}


ShowFileParameter::ParameterType ShowFileParameter::
getType() const
// ======================================================================
// /Description:
//    Return the type of the parameter
// ======================================================================
{
  return type;
}

const aString& ShowFileParameter::
getName() const
// ======================================================================
// /Description:
//    Return the name of the parameter
// ======================================================================
{
  return name;
}


int ShowFileParameter::
get( aString & name_, ParameterType & type_, int & ivalue_, real & rvalue_, aString & stringValue_ ) const
// ======================================================================
// /Description:
// Return the name, type and value for the parameter.
// Only one of the values (ivalue,rvalue,stringValue) will make sense,
//  the one corresponding to the "type"
// ======================================================================
{
  name_=name;
  type_=type;
  ivalue_=ivalue;
  rvalue_=rvalue;
  stringValue_=stringValue;
  return 0;
}
 
int ShowFileParameter::
set( const aString & name_, ParameterType type_, int ivalue_, real rvalue_, const aString & stringValue_ ) 
// ======================================================================
// /Description:
//    Set a type name and value for a particular type. Only one of the values (ivalue,rvalue,stringValue) will make sense,
//  the one corresponding to the "type"
// ======================================================================
{
  name=name_;
  type=type_;
  ivalue=ivalue_;
  rvalue=rvalue_;
  stringValue=stringValue_;
  return 0;
}

// assign a name and value to a parameter
int ShowFileParameter::
set(const aString & name_, int value)
// ======================================================================
// /Description:
//    Assign a name and value to a parameter
// ======================================================================
{ 
  type=intParameter; 
  name=name_; 
  ivalue=value; 
  rvalue=value;    // give a default value to this unused value
  return 0;
}

int ShowFileParameter::
set(const aString & name_, real value)
// ======================================================================
// /Description:
//    Assign a name and value to a parameter
// ======================================================================
{ 
  type=realParameter; 
  name=name_;
  rvalue=value;
  ivalue=int(value);    // give a default value to this unused value
  return 0;
}

int ShowFileParameter::
set(const aString & name_, const aString & value)
// ======================================================================
// /Description:
//    Assign a name and value to a parameter
// ======================================================================
{ 
  type=stringParameter; 
  name=name_; 
  stringValue=value;
  rvalue=0.;
  ivalue=0; 
  return 0;
}


// assign a name and value to a parameter
int ShowFileParameter::
set(int value)
// ======================================================================
// /Description:
//    Assign a name and value to a parameter
// ======================================================================
{ 
  return set(name,value);
}

int ShowFileParameter::
set(real value)
// ======================================================================
// /Description:
//    Assign a name and value to a parameter
// ======================================================================
{ 
  return set(name,value);
}

int ShowFileParameter::
set(const aString & value)
// ======================================================================
// /Description:
//    Assign a name and value to a parameter
// ======================================================================
{ 
  return set(name,value);
}


// ***********************************************************************************



//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{getParameter(int)}} 
bool ListOfShowFileParameters::
getParameter(const aString & name, int & ivalue ) const
//----------------------------------------------------------------------
// /Description:
//    Get a parameter with type `int'
// /return value: 
//  /Author: WDH
//\end{ListOfShowFileParametersInclude.tex}
//----------------------------------------------------------------------
{
  real rvalue;
  aString stringValue;
  return getParameter(name,ShowFileParameter::intParameter,ivalue,rvalue,stringValue);
}

//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{getParameter(int)}} 
bool ListOfShowFileParameters::
getParameter(const aString & name, real & rvalue ) const
//----------------------------------------------------------------------
// /Description:
//    Get a parameter with type `int'
// /return value: 
//  /Author: WDH
//\end{ListOfShowFileParametersInclude.tex}
//----------------------------------------------------------------------
{
  int ivalue;
  aString stringValue;
  return getParameter(name,ShowFileParameter::realParameter,ivalue,rvalue,stringValue);
}

//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{getParameter(int)}} 
bool ListOfShowFileParameters::
getParameter(const aString & name, aString & stringValue ) const 
//----------------------------------------------------------------------
// /Description:
//    Get a parameter with type `int'
// /return value: 
//  /Author: WDH
//\end{ListOfShowFileParametersInclude.tex}
//----------------------------------------------------------------------
{
  int ivalue;
  real rvalue;
  return getParameter(name,ShowFileParameter::stringParameter,ivalue,rvalue,stringValue);
}


//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{getParameter(int)}} 
bool ListOfShowFileParameters::
getParameter(const aString & name, ShowFileParameter::ParameterType type, int & ivalue, real & rvalue, 
                    aString & stringValue ) const 
//----------------------------------------------------------------------
// /Description:
//    Get a parameter with the given name and type
// /name, type (input) : get the parameter value for a parameter with this name and type.
// /ivalue (output) : return integer parameters in this variable (if type==intParameter)
// /rvalue (output) : return real parameters in this variable (if type==realParameter)
// /stringValue (output) : return string parameters in this variable (if type==stringParameter)
// /return value: 
//  /Author: WDH
//\end{ListOfShowFileParametersInclude.tex}
//----------------------------------------------------------------------
{

  ListOfShowFileParameters::iterator iter; 
  ListOfShowFileParameters parameterList = (ListOfShowFileParameters&)(*this); // cast away const
  for( iter=parameterList.begin(); iter!=parameterList.end(); iter++ )
  {
    if( iter->getName()==name )
      break;
  }
  if( iter==parameterList.end() )
  {
    // printf("ListOfShowFileParameters::getParameter:WARNING: name=%s was not found.\n",(const char*)name);
    return false;  // not found
  }

  ShowFileParameter & sfp = *iter;
  
  ShowFileParameter::ParameterType typeFound = sfp.getType();
  if( type==typeFound )
  {
    aString pname;
    sfp.get(pname,type,ivalue,rvalue,stringValue);
  }
  else 
  {
    // printf("ListOfShowFileParameters::getParameter:WARNING: name=%s found but it is not type=%i (typeFound=%i)\n",
    //    (const char*)name,type,typeFound);
    return false;
  }

  return true;
}

//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{matchAndSetValue}} 
bool ListOfShowFileParameters::
matchAndSetValue( const aString & answer )
//----------------------------------------------------------------------
// /Description:
//     Parse an answer and look for [name value] -- set the value if the name is found
//   This function can be used when the show file parameters have been added to a memu.
//
// /answer (input) : a string containing a "name" and "value". 
// /return value: true if a name was found and a value set.
//  /Author: WDH
//\end{ListOfShowFileParametersInclude.tex}
//----------------------------------------------------------------------
{
  std::list<ShowFileParameter>::iterator iter; 
  int len=0;
  for( iter=begin(); iter!=end(); iter++ )
  {
    if( (len=matches(answer,iter->getName())) )
      break;
  }
  if( iter==end() )
  {
    return false;  // not found
  }
  

  ShowFileParameter & sfp = *iter;
  ShowFileParameter::ParameterType type = sfp.getType();
  aString name =substring(answer,0,len-1);
  if( type==ShowFileParameter::intParameter )
  {
    int value;
    sScanF(substring(answer,len,answer.length()-1),"%i",&value);
    sfp.set(value);
    printf("ListOfShowFileParameters::matchAndSetValue: set int parameter %s = %i\n",
           (const char*)name,value);
  }
  else if( type==ShowFileParameter::realParameter )
  {
    real value;
    sScanF(substring(answer,len,answer.length()-1),"%e",&value);
    sfp.set(value);
    printf("ListOfShowFileParameters::matchAndSetValue: set real parameter %s = %e\n",
           (const char*)name,value);
  }
  else 
  {
    aString value=substring(answer,len,answer.length()-1);
    sfp.set(value);
    printf("ListOfShowFileParameters::matchAndSetValue: set string parameter %s = %s\n",
           (const char*)name,(const char*)value);
  }

  return true;
}




//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{setParameter}}
bool ListOfShowFileParameters::
setParameter(const aString & name, const int value ) 
// ---------------------------------------------------------------------------------------
// /Description:
// 
//  Set a parameter with a given name (create the parameter if not found)
// /name (input) : name 
// /value (input) : value
// /Return value: return true if the parameter was newly created.
//\end{ListOfShowFileParametersInclude.tex}
// ---------------------------------------------------------------------------------------
{
  std::list<ShowFileParameter>::iterator iter; 
  int len=0;
  for( iter=begin(); iter!=end(); iter++ )
  {
    if( (len=matches(name,iter->getName())) )
      break;
  }
  if( iter==end() )
  { // name not found, create a new parameter.
    push_back(ShowFileParameter(name,value));
    return true;
  }
  else
  {
    ShowFileParameter & sfp = *iter;
    sfp.set(value);
    return false;
  }
  
}

//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{setParameter}}
bool ListOfShowFileParameters::
setParameter(const aString & name, const real value ) 
// ---------------------------------------------------------------------------------------
// /Description:
// 
//  Set a parameter with a given name (create the parameter if not found)
// /name (input) : name 
// /value (input) : value
// /Return value: return true if the parameter was newly created.
//\end{ListOfShowFileParametersInclude.tex}
// ---------------------------------------------------------------------------------------
{
  std::list<ShowFileParameter>::iterator iter; 
  int len=0;
  for( iter=begin(); iter!=end(); iter++ )
  {
    if( (len=matches(name,iter->getName())) )
      break;
  }
  if( iter==end() )
  { // name not found, create a new parameter.
    push_back(ShowFileParameter(name,value));
    return true;
  }
  else
  {
    ShowFileParameter & sfp = *iter;
    sfp.set(value);
    return false;
  }
  
}

//\begin{>>ListOfShowFileParametersInclude.tex}{\subsubsection{setParameter}}
bool ListOfShowFileParameters::
setParameter(const aString & name, const aString & value ) 
// ---------------------------------------------------------------------------------------
// /Description:
// 
//  Set a parameter with a given name (create the parameter if not found)
// /name (input) : name 
// /value (input) : value
// /Return value: return true if the parameter was newly created.
//\end{ListOfShowFileParametersInclude.tex}
// ---------------------------------------------------------------------------------------
{
  std::list<ShowFileParameter>::iterator iter; 
  int len=0;
  for( iter=begin(); iter!=end(); iter++ )
  {
    if( (len=matches(name,iter->getName())) )
      break;
  }
  if( iter==end() )
  { // name not found, create a new parameter.
    push_back(ShowFileParameter(name,value));
    return true;
  }
  else
  {
    ShowFileParameter & sfp = *iter;
    sfp.set(value);
    return false;
  }
  
}

