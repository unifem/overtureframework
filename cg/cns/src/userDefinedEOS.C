
#include <math.h>
#include <stdio.h>
#include "GenericGraphicsInterface.h"
#include "CnsParameters.h"



// This class holds any state information associated with the EOS.
// This data is filled in at the start in updateUserDefinedEOS (below).
// This data is then looked up when the EOS is evaluated in getUserDefinedEOS (below).
class UserDefinedEOSData
{
public:

enum EquationOfStateEnum
{
  noUserDefinedEOS=0,
  idealGas=1,
  stiffenedGas=2   // add new EOS here
};

EquationOfStateEnum equationOfState;

real gamma;  // gamma for ideal gas
real gammaStiff, pStiff;  // gamma and p-offset for stiffened gas

int sc;  // component number of first reactive species

// put other user EOS data here: 

};
  

// ==========================================================================================
/// \brief Interactively select a user defined Equation of State (EOS).
// ==========================================================================================
int CnsParameters::
updateUserDefinedEOS(GenericGraphicsInterface & gi)
{

  // Create an object to store any user EOS state data.

  if( !dbase.has_key("userDefinedEquationOfStateData") )
  {
    dbase.put<UserDefinedEOSData>("userDefinedEquationOfStateData");
    dbase.put<UserDefinedEOSData*>("userDefinedEquationOfStateDataPointer",NULL);

    dbase.put<aString>("userDefinedEquationOfStateName");
  }
  UserDefinedEOSData & userEOSData = dbase.get<UserDefinedEOSData>("userDefinedEquationOfStateData");
  UserDefinedEOSData *& userEOSDataPointer = dbase.get<UserDefinedEOSData*>("userDefinedEquationOfStateDataPointer");
  aString & userDefinedEquationOfStateName = dbase.get<aString>("userDefinedEquationOfStateName");

  userEOSData.equationOfState=UserDefinedEOSData::noUserDefinedEOS;  // by default no user defined EOS is used.

  const aString menu[]=
    {
      "none",
      "ideal gas",
      "stiffened gas",
      "done",
      ""
    }; 

  gi.appendToTheDefaultPrompt("userDefinedEOS>");
  aString answer;
  for( ;; ) 
  {

    int response=gi.getMenuItem(menu,answer,"Choose an EOS");
    
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="none" ) 
    {

      userDefinedEquationOfStateName="none";
      userEOSData.equationOfState=UserDefinedEOSData::noUserDefinedEOS;

    }
    else if( answer=="ideal gas" ) 
    {
      userDefinedEquationOfStateName="ideal gas";
      userEOSData.equationOfState=UserDefinedEOSData::idealGas;
      userEOSData.gamma=dbase.get<real>("gamma");  // look up and save gamma
      userEOSData.sc=dbase.get<int>("sc");         // look up species component index (not currently used)

    }
    else if( answer=="stiffened gas" ) 
    {

      userDefinedEquationOfStateName="stiffened gas";
      userEOSData.equationOfState=UserDefinedEOSData::stiffenedGas;
      
      // default values:
      userEOSData.gammaStiff=1.4;
      userEOSData.pStiff=100.;
       
      gi.inputString(answer,"Enter gammaStiff, pStiff");
      sScanF(answer,"%e %e",&userEOSData.gammaStiff,&userEOSData.pStiff);
      printF("stiffened gas: setting gammaStiff=%8.2e, pStiff=%8.2e\n",userEOSData.gammaStiff,userEOSData.pStiff);

    }
    else
    {
      printF("unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }

  if( userEOSData.equationOfState!=UserDefinedEOSData::noUserDefinedEOS )
  {
    dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=userDefinedEOS;
    // Set the opaque pointer to the user EOS data. NULL means there is no user EOS.
    userEOSDataPointer=&userEOSData;
    // printF(" &userEOSData = [%p]\n",&userEOSData);
  }
  else
    userEOSDataPointer=NULL;
  

  gi.unAppendTheDefaultPrompt();
  return 0;
}






#define getUserDefinedEOS EXTERN_C_NAME(getuserdefinedeos)

extern "C"
{
void getUserDefinedEOS( real & r,
			real & e, 
			real & p,
			real * dp,
			int  & option,
			int  & derivOption,
			real *q,
			int  *iparam,
			real *rparam,
                        UserDefinedEOSData **dataPointer,
			int    &ierr );
}

// ==========================================================================================
/// \brief Evaluate the user defined Equation of State (EOS).
/// \details This routine is called very often so make it as efficient as possible.
///
/// \param r (input)        : density
/// \param e (input/output) : internal energy per unit mass, r*E = r*e + .5*r*( u^2+v^2+w^2 )
/// \param p (input/ouput)  : pressure
/// \param dp[3] (output) : holds derivatives on output depending on derivOption
/// \param option (input) :  
///        option = 0 : return energy given rho and pressure
///        option = 1 : return pressure given rho and energy
/// \param derivOption (input):
///        derivOption = 0 : no derivatives needed
///        derivOption = 1 : evaluate dp[0] = dp/dr     (r*e=const)
///                          evaluate dp[1] = dp/d(r*e) (r=const)
/// \param q (input) : state vector (e.g. holds species concentrations for reactive flow)
/// \param iparam (input) : iparam[0]=nd number of space dimensions (1,2, or 3)
/// \param rparam (input) : optional real parameters for future use
/// \param dataPointer (input) : opaque pointer to the user EOS data.
/// \param ierr (output) : 0=success, 1=failure
///
// ==========================================================================================
void getUserDefinedEOS( real &r,
			real &e, 
			real &p,
			real *dp,
			int  & option,
			int  & derivOption,
			real *q,
			int  *iparam,
			real *rparam,
                        UserDefinedEOSData **dataPointer,
			int  &ierr )
{
  // if( true )
  //   printF("getUserDefinedEOS: (r,e,p)=(%e,%e,%e) option=%i derivOption=%i\n",r,e,p,option,derivOption);

  // Cast the opaque pointer to the class holding the user EOS data:
  if( dataPointer==NULL )
  {
    printF("getUserDefinedEOS:ERROR: dataPointer==NULL\n");
    OV_ABORT("error");
  }
  UserDefinedEOSData & userDefinedEOSData = **dataPointer;

  if( false )
    printF("getUserDefinedEOS: equationOfState=%i\n",(int) userDefinedEOSData.equationOfState);

  int nd = iparam[0];
  if( nd<1 || nd>3 )
  {
    printF("getUserDefinedEOS:ERROR: number of dimensions nd=%i\n",nd);
    OV_ABORT("error");
  }
  

  if( userDefinedEOSData.equationOfState==UserDefinedEOSData::idealGas )
  {
    // -- ideal gas (for testing)
    const real & gamma = userDefinedEOSData.gamma;
    if( option==0 )
    {
      // compute e=E(r,p)
      e = p/(r*(gamma-1.0));  
    }
    else if( option==1 )
    {
      // compute p=P(r,e)
      p = (gamma-1.0)*r*e;
    }
    else
    {
      printF("getUserDefinedEOS:ERROR option=%i unknown!\n");
      ierr=1;
      return;
    }
    if( derivOption==1 )
    {
      dp[0] = 0.0;         // dp/dr     (r*e=const)
      dp[1] = gamma-1.0;   // dp/d(r*e) (r=const)
    }

    ierr = 0;
  }
  else if( userDefinedEOSData.equationOfState==UserDefinedEOSData::stiffenedGas )
  {
    const real & gammaStiff = userDefinedEOSData.gammaStiff;
    const real & pStiff = userDefinedEOSData.pStiff;
    
    if( option==0 )
    {
      // compute e=E(r,p)
      e = (p+gammaStiff * pStiff)/(r*(gammaStiff-1.0));  
    }
    else if( option==1 )
    {
      // compute p=P(r,e)

      p = (gammaStiff-1.)*r*e - gammaStiff*pStiff;
      
    }
    else
    {
      printF("getUserDefinedEOS:ERROR option=%i unknown!\n");
      ierr=1;
      return;
    }
    if( derivOption==1 )
    {
      dp[0] = 0.0;              // dp/dr     (r*e=const)
      dp[1] = gammaStiff-1.0;   // dp/d(r*e) (r=const)
    }

  }
  else
  {
    printF("getUserDefinedEOS:ERROR: unknown user defined EOS = %i\n",(int)userDefinedEOSData.equationOfState);
    OV_ABORT("error");
  }
  

  return;
}

