#ifndef SHOW_FILE_PARAMETER_H
#define SHOW_FILE_PARAMETER_H

#include "Overture.h"

// This class defines parameters for use with a show file.
// Parameters of type int, real or string can be created, each with a given name.

class ShowFileParameter
{
public:
  enum ParameterType
  {
    intParameter=0,
    realParameter,
    stringParameter
  };

ShowFileParameter();

ShowFileParameter(const aString & name_, int value);

ShowFileParameter(const aString & name_, real value);

ShowFileParameter(const aString & name_, const aString & value);

~ShowFileParameter();

ParameterType getType() const;
const aString& getName() const;

// Return the type and the value. Only one of the values (ivalue,rvalue,stringValue) will make sense,
//  the one corresponding to the "type"
int get( aString & name_, ParameterType & type_, int & ivalue_, real & rvalue_, aString & stringValue_ ) const;

int set( const aString & name_, ParameterType type_,  int ivalue_, real rvalue_, const aString & stringValue_ );

// assign a name and value to a parameter
int set(const aString & name_, int value);
int set(const aString & name_, real value);
int set(const aString & name_, const aString & value);

// set a value without changing the name
int set(int value);
int set(real value);
int set(const aString & value);

protected:

  ParameterType type;

  aString name;
  real rvalue;
  int ivalue;
  aString stringValue;

};



#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
#else
#include <list.h>
#endif

class ListOfShowFileParameters : public std::list<ShowFileParameter> 
{
  public:

  // get a parameter with a given name
  bool getParameter(const aString & name, int & value ) const;
  bool getParameter(const aString & name, real & value ) const;
  bool getParameter(const aString & name, aString & value ) const;

  // get a parameter with a given name and type
  bool getParameter(const aString & name, 
		    ShowFileParameter::ParameterType type, 
		    int & value, 
		    real & rValue, 
		    aString & stringValue ) const;

  // Parse an answer and look for [name value] -- set the value if the name is found
  bool matchAndSetValue( const aString & answer );

  // set a parameter with a given name (create the parameter if not found)
  bool setParameter(const aString & name, const int value );
  bool setParameter(const aString & name, const real value );
  bool setParameter(const aString & name, const aString & value );


};


#endif
