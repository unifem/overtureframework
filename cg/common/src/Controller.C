#include "Controller.h"
#include "Parameters.h"
#include "Ogshow.h"
#include "Integrate.h"
#include "GenericGraphicsInterface.h"


//============================================================================
/// \brief Contoller constructor.
//============================================================================
Controller::
Controller(Parameters & parameters_) 
  : parameters(parameters_)
{
}

//============================================================================
/// \brief Contoller destructor.
//============================================================================
Controller::
~Controller()
{
}


//============================================================================
/// \brief Copy constructor
//============================================================================
Controller::
Controller( const Controller & x ) 
  : parameters(x.parameters)
{
  // finish me 
}



//============================================================================
/// \brief Get from a data base file.
//============================================================================
int Controller::
get( const GenericDataBase & dir, const aString & name)
{
  OV_ABORT("Controller:ERROR: finish me");
  return 0;
}

//============================================================================
/// \brief Compute the control variable (single output)
/// \param t (input) : evaluate the control function at this time.
/// \param uControl : control value
/// \param uControlDot : time derivative of the control value
//============================================================================
int Controller::
getControl( const real t, real & uControl, real & uControlDot ) const
{

  // Check that a controller has been created:
  if( !parameters.dbase.has_key("ControllerData") )
  {
    printF("Controller::getControl:ERROR: the controller has not been initialized.\n");
    OV_ABORT("error");
  }
  DataBase & dbase = parameters.dbase.get<DataBase>("ControllerData");

  // Do this for now: We could interpolate/extrapolate from the sequence of control values 

  uControl = dbase.get<real>("uControl");
  uControlDot = dbase.get<real>("uControlDot");


  return 0;

}


// update the control function based on the solution v. (use getControl to return current values)
int Controller::
updateControl( realCompositeGridFunction & v, const real t, const real dt )
//============================================================================
/// \brief Update the control function based on the current solution v.
/// \details This function is called to update the current state for the control function.
///    Use the function getControl to return the values of the control function. In normal
/// usage, updateControl is called once (per solution time-step or solution predictor-corrector stage) and 
/// getControl is called multiple times to assign the control to each boundary where it is used.
/// 
/// \param v,t,dt (input) : current solution, time and time step
//============================================================================
{

  // Check that a controller has been created:
  if( !parameters.dbase.has_key("ControllerData") )
  {
    printF("Controller::updateControl:ERROR: the controller has not been initialized.\n");
    OV_ABORT("error");
  }
  
  DataBase & dbase = parameters.dbase.get<DataBase>("ControllerData");

  aString & controllerType = dbase.get<aString>("controllerType");
  if( controllerType!="PID" )
  {
    printF("Controller::updateControl:ERROR: the controller type=[%s] is not PID!?\n",(const char*)controllerType);
    OV_ABORT("error");
  }

  const int & targetComponent = dbase.get<int>("targetComponent");
  if( targetComponent<0 || targetComponent>=parameters.dbase.get<int>("numberOfComponents") )
  {
    printF("Controller::updateControl:ERROR: the targetComponent=%i is invalid.\n",targetComponent);
    OV_ABORT("error");
  }
  

  real & errorTimeIntegral = dbase.get<real>("errorTimeIntegral");
  const real *gainPID = dbase.get<real[3]>("gainPID");
  const real Kp = gainPID[0], Ki=gainPID[1], Kd=gainPID[2];

  const real & targetSetPoint = dbase.get<real>("targetSetPoint");

  Integrate *pIntegrate = parameters.dbase.get<Integrate*>("integrate");
  assert( pIntegrate!=NULL );
  Integrate & integrate = *pIntegrate;

  const real regionVolume = parameters.dbase.get<real>("regionVolume");
  real targetIntegral = integrate.volumeIntegral( v,targetComponent );
  real targetAve = targetIntegral/regionVolume;
  printF("updateControl: t=%9.3e, targetIntegral=%9.3e, regionVolume=%8.2e, targetAve = %9.3e\n",
	 t,targetIntegral,regionVolume,targetAve);
        
  real & tControl = dbase.get<real>("tControl");
  real & uControl = dbase.get<real>("uControl");
  real & uControlDot = dbase.get<real>("uControlDot");

  real tControlOld = tControl;
  real uControlOld = uControl;

  tControl=t;  // save the time
  
  if( t > 0. )
  {
    real error = targetSetPoint - targetAve;
    
    errorTimeIntegral += error*dt;

    real errorDot=0.;  // fix me 

    uControl = targetSetPoint + Kp*error + Ki*errorTimeIntegral + Kd*errorDot;

    if( tControl>tControlOld+.1*dt ) // only update the time derivative if we have taken a big enough step
      uControlDot = (uControl-uControlOld)/(tControl-tControlOld);  
  }
  else
  {
    uControl=targetSetPoint;  // 
    uControlDot=0.;
  }
  
  printF("             : t=%9.3e, dt=%8.2e, (dtControl=%8.2e): uControl=%f, Ave=%f, Set=%f, uControlDot=%f\n",
         t,dt,tControl-tControlOld,uControl,targetAve,targetSetPoint,
                      uControlDot);

  // Save control sequence info (if the show file is being saved)
  if( dt>0. && parameters.dbase.get<Ogshow* >("show")!=NULL )
  {
    aString sequenceName = "ControlData";
    if( !parameters.dbase.has_key(sequenceName) ) // check if this sequence exists -- is this the right way?
    {
      parameters.dbase.put<aString>(sequenceName);

      // Define a sequence with a given name and given components 
      std::vector<aString> componentNames;
      componentNames.push_back("control");
      componentNames.push_back("sensor");
      componentNames.push_back("setPoint");

      createControlSequence( sequenceName, componentNames );
    }
	
    // Here are the values we save
    std::vector<real> controlValues;
    controlValues.push_back(uControl);
    controlValues.push_back(targetAve);
    controlValues.push_back(targetSetPoint);
	
    saveControlSequenceData( sequenceName, t, controlValues );

  }

  return 0;

}



//============================================================================
/// \brief Put to a data base file.
//============================================================================
int Controller::
put( GenericDataBase & dir, const aString & name) const
{
  OV_ABORT("Controller:ERROR: finish me");
  return 0;
}

// =================================================================================================
/// \brief Create a control sequence (to be saved in the show file).
/// \details A control sequence can consist of multiple components. 
/// \param sequenceName (input) : name of the sequence.
/// \param componentNames (input) : names of the components of the sequence.
// =================================================================================================
int Controller::
createControlSequence( const aString & sequenceName, const std::vector<aString> componentNames )
{
  // Make a sub-directory in the data-base to store control variables 
  if( !parameters.dbase.has_key("ControllerData") )
    parameters.dbase.put<DataBase>("ControllerData");

  DataBase & dbase = parameters.dbase.get<DataBase>("ControllerData");

  typedef std::vector<aString> ArrayOfStrings;
  typedef std::vector<ArrayOfStrings> ArrayOfArrayOfStrings;

  if( !dbase.has_key("controlSequenceNames") )
  {
    dbase.put<std::vector<aString> >("controlSequenceNames");
    dbase.put<ArrayOfArrayOfStrings >("controlSequenceComponentNames");
  }

  std::vector<aString> & controlSequenceNames =  dbase.get<std::vector<aString> >("controlSequenceNames");
  ArrayOfArrayOfStrings & controlSequenceComponentNames =  
    dbase.get<ArrayOfArrayOfStrings>("controlSequenceComponentNames");

  controlSequenceNames.push_back(sequenceName);
  controlSequenceComponentNames.push_back(componentNames);

  return 0;

}


// =================================================================================================
/// \brief Save control information that will be saved in a sequence in the show file.
/// \param sequenceName (input) : name of the sequence (createControlSequence must already have been called
///                      with this name.
/// \param t (input) : current time.
/// \param values (input) : values of the components, 
// =================================================================================================
int Controller::
saveControlSequenceData( const aString & sequenceName, const real t, const std::vector<real> values )
{
  DataBase & dbase = parameters.dbase.get<DataBase>("ControllerData");

  // Make up names for variables that hold the sequence data
  aString sequenceValuesName, sequenceTimeName, sequenceLengthName, buff;
  sequenceValuesName=sPrintF(buff,"%sControlValues",(const char*)sequenceName);
  sequenceTimeName=sPrintF(buff,"%sControlTime",(const char*)sequenceName);
  sequenceLengthName=sPrintF(buff,"%sControlLength",(const char*)sequenceName);
  
  if( !dbase.has_key(sequenceValuesName) ) 
     dbase.put<RealArray>(sequenceValuesName);
  if( !dbase.has_key(sequenceTimeName) ) 
     dbase.put<RealArray>(sequenceTimeName);
  if( !dbase.has_key(sequenceLengthName) ) 
  {
    dbase.put<int>(sequenceLengthName);
    dbase.get<int>(sequenceLengthName)=0;
  }
  
  RealArray & sequenceTime  = dbase.get<RealArray>(sequenceTimeName);
  RealArray & sequenceValues = dbase.get<RealArray>(sequenceValuesName);
  int & numValues = dbase.get<int>(sequenceLengthName);

  const int numberOfComponents = values.size();

  if( numValues>= sequenceValues.getLength(0) )
  {
    const int newMaxNumValues = max( numValues+500, int(numValues*1.2));
    sequenceTime.resize(newMaxNumValues);
    sequenceValues.resize(newMaxNumValues,numberOfComponents);
  }
  

  sequenceTime(numValues)=t;
  for( int c=0; c<numberOfComponents; c++ )
    sequenceValues(numValues,c)=values[c];

  numValues++;

//   seq.push_back(

  return 0;
}


// =================================================================================================
/// \brief Save control information to the show file.
// =================================================================================================
int Controller::
saveToShowFile( ) const
{
  Ogshow *ogshow = parameters.dbase.get<Ogshow* >("show");
  if( ogshow!=NULL && parameters.dbase.has_key("ControllerData") )
  {
    DataBase & dbase = parameters.dbase.get<DataBase>("ControllerData");

    typedef std::vector<aString> ArrayOfStrings;
    typedef std::vector<ArrayOfStrings> ArrayOfArrayOfStrings;

    if( !dbase.has_key("controlSequenceNames") )
      return 0;  // there are no sequences to save

    ArrayOfStrings & controlSequenceNames =  dbase.get<ArrayOfStrings>("controlSequenceNames");
    ArrayOfArrayOfStrings & controlSequenceComponentNames = 
                          dbase.get<ArrayOfArrayOfStrings>("controlSequenceComponentNames");

    // -- save the different sequences --
    for( int seq=0; seq<controlSequenceNames.size(); seq++ )
    {
      const aString & sequenceName = controlSequenceNames[seq];

      aString sequenceValuesName, sequenceTimeName, sequenceLengthName, buff;
      sequenceValuesName=sPrintF(buff,"%sControlValues",(const char*)sequenceName);
      sequenceTimeName=sPrintF(buff,"%sControlTime",(const char*)sequenceName);
      sequenceLengthName=sPrintF(buff,"%sControlLength",(const char*)sequenceName);
  
      if( !dbase.has_key(sequenceValuesName) || 
          !dbase.has_key(sequenceTimeName) || 
          !dbase.has_key(sequenceLengthName) ) 
      {
	printF("Controller::saveToShowFile:ERROR: sequence info NOT found!\n");
	OV_ABORT("ERROR");
      }
  
      RealArray & sequenceTime  = dbase.get<RealArray>(sequenceTimeName);
      RealArray & sequenceValues = dbase.get<RealArray>(sequenceValuesName);
      const int & numValues = dbase.get<int>(sequenceLengthName);
      const int numberOfComponents = sequenceValues.getLength(1);
      
      Range all, N = numValues;

      printF("***** Controller::saveToShowFile: save sequence=[%s] to the show file, numValues=%d*****\n",
             (const char*)sequenceName,numValues);

      std::vector<aString> & sequenceComponentNames = controlSequenceComponentNames[seq];
      aString *componentNames = new aString[numberOfComponents];
      for( int c=0; c<numberOfComponents; c++ )
	componentNames[c]=sequenceComponentNames[c];

      ogshow->saveSequence( sequenceName,sequenceTime(N),sequenceValues(N,all),componentNames);

      delete [] componentNames;
    }
  }
  


//   if( debug() & 2 )
//     printF("***** MovingGrids::saveToShowFile ***\n");
//   if( parameters.dbase.get<Ogshow* >("show")!=NULL && numberOfRigidBodies>0 && rigidBodyInfoCount>0 )
//   {
//     if( debug() & 2 )
//       printF("***** MovingGrids::saveToShowFile: save a sequence ***\n");
    
//     Range all,N(0,rigidBodyInfoCount-1);
//     char buff[40];
//     for( int b=0; b<numberOfRigidBodies; b++ )
//     {
//       parameters.dbase.get<Ogshow* >("show")->saveSequence( sPrintF(buff,"rigid body %i",b),rigidBodyInfoTime(N),rigidBodyInfo(N,all,b),
// 				 rigidBodyInfoName);
//     }
//   }
  return 0;
}


// =================================================================================================
/// \brief Define the control properties interactively.
/// \details This routine is used to define controls. 
///   Type of controls include:
///     - boundary controls : define inflow boundary conditions for control.
///     - sensor regions : define regions where sensors should be located.
// =================================================================================================
int Controller::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  // Make a sub-directory in the data-base to store control variables 
  if( !parameters.dbase.has_key("ControllerData") )
    parameters.dbase.put<DataBase>("ControllerData");

  DataBase & dbase = parameters.dbase.get<DataBase>("ControllerData");


  // The default controller is a PID (proportional, integral, derivative)
  if( !dbase.has_key("controllerType") ) 
  {
    dbase.put<aString>("controllerType");
    dbase.get<aString>("controllerType")="PID";
  }
  aString & controllerType = dbase.get<aString>("controllerType");

  // target component : 
  if( !dbase.has_key("targetComponent") ) dbase.put<int>("targetComponent",0);

  int & targetComponent = dbase.get<int>("targetComponent");

  // gainPID[3] : holds the PID gain coefficients Kp, Ki, Kd
  if( !dbase.has_key("gainPID") )  
  {
    dbase.put<real[3]>("gainPID");
    real *gainPID = dbase.get<real[3]>("gainPID");
    // Default PID gains:
    gainPID[0]=1.;
    gainPID[1]=1.;
    gainPID[2]=0.;
  }
  real *gainPID = dbase.get<real[3]>("gainPID");
  
  if( !dbase.has_key("targetSetPoint") ) 
  {
    dbase.put<real>("targetSetPoint");
  }
  real & targetSetPoint = dbase.get<real>("targetSetPoint");

  GUIState dialog;

  dialog.setWindowTitle("Controller");
  dialog.setExitCommand("exit", "exit");


  // Controller types:
  aString opCommand1[] = {"PID",
			  ""};
  dialog.setOptionMenuColumns(1);
  int controllerTypeID= controllerType=="PID" ? 0 : 0;
  dialog.addOptionMenu( "Type:", opCommand1, opCommand1, controllerTypeID );

  // Set-point types:
  aString opCommand2[] = {"constant set-point",
                          "variable set-point",
			  ""};
  dialog.setOptionMenuColumns(1);
  int setPointType=0;  // finish me 
  dialog.addOptionMenu( "Set-point:", opCommand2, opCommand2, setPointType); 

  aString cmds[] = {"Set points...",
                    "sensors...",
		    "help",
		    ""};
  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  const int numberOfTextStrings=7;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "targetComponent:"; 
  sPrintF(textStrings[nt],"%i",targetComponent);  nt++; 

  textLabels[nt] = "Kp, Ki, Kd:"; 
  sPrintF(textStrings[nt],"%g,%g,%g (PID gains)",gainPID[0],gainPID[1],gainPID[2]);  nt++; 

  textLabels[nt] = "Set point:"; 
  sPrintF(textStrings[nt],"%g",targetSetPoint);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(dialog);

  int len=0;
  aString answer,buff;

  gi.appendToTheDefaultPrompt("Controller>"); // set the default prompt

  for( int it=0;; it++ )
  {
    gi.getAnswer(answer,"");  // gi.getMenuItem(menu,answer);
 

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="PID" )
    {
      controllerType="PID";
    }
    else if( dialog.getTextValue(answer,"targetComponent:","%i",targetComponent) ){} //
    else if( dialog.getTextValue(answer,"Set point:","%e",targetSetPoint) ){} //
    else if( len=answer.matches("Kp, Ki, Kd:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&gainPID[0],&gainPID[1],&gainPID[2]);
      printF("Setting PID gains: Kp=%g, Ki=%g, Kd=%g\n",gainPID[0],gainPID[1],gainPID[2]);
      
      if( !gi.isGraphicsWindowOpen() )
        dialog.setTextLabel("Kp, Ki, Kd:",sPrintF(answer,"%g,%g,%g (PID gains)",gainPID[0],gainPID[1],gainPID[2]));
    }
    else if( answer=="Set points..." )
    {
      printF("answer=[%s] FINISH ME!\n",(const char*)answer);
    }
    else if( answer=="sensors..." )
    {
      printF("answer=[%s] FINISH ME!\n",(const char*)answer);
    }
    else if( answer=="constant set-point" ||
             answer=="variable set-point" )
    {
      printF("answer=[%s] FINISH ME!\n",(const char*)answer);
    }
    else if( answer=="help" )
    {
      printF("PID: The proportional-integral-derivative controller is:\n"
             "   uControl = Kp*err + Ki*int_0^t err(tau) dtau + Kd*d(err)/dt \n"
             " where \n"
             "   err(t) = setPoint - sensor\n");
    }
    else
    {
      gi.outputString( sPrintF(buff,"Controller::unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
  }  // end for it 
   
  // NOTE: We should share this Integrate object with the one in MovingGrids! *******************

  gi.unAppendTheDefaultPrompt();  // reset

  gi.popGUI(); // restore the previous GUI


  parameters.dbase.get<bool >("turnOnController")=true;

  // For now we make a single output control:
  if( !dbase.has_key("tControl") ) dbase.put<real>("tControl",0.);  // time that the controller was last updated
  if( !dbase.has_key("uControl") ) dbase.put<real>("uControl",targetSetPoint);
  if( !dbase.has_key("uControlDot") ) dbase.put<real>("uControlDot",0.);

  if( !parameters.dbase.has_key("integrate")) 
  {
    printF("Controller: create an integrate object...\n");
    parameters.dbase.put<Integrate*>("integrate");  
    parameters.dbase.get<Integrate*>("integrate")=NULL;
  }
    
  Integrate *& pIntegrate = parameters.dbase.get<Integrate*>("integrate");
  // cout << "pIntegrate=" << pIntegrate << endl;
  if( pIntegrate==NULL )
  {
    printF("Controller: Build an Integration object...\n");
    pIntegrate = new Integrate(cg);  // ************************************ who deletes this??
  }
    
  // -- compute the volume (needed to compute the average value of the target)---
  // NOTE: save this in the main directory so other people can access the volume.
  if( !parameters.dbase.has_key("regionVolume") ) parameters.dbase.put<real>("regionVolume",0.);
  real & regionVolume = parameters.dbase.get<real>("regionVolume");

  Integrate & integrate = *pIntegrate;
  real cpu = getCPU();
  regionVolume = integrate.volume();
  printF("Controller: regionVolume = %e, cpu=%8.2e(s).\n",regionVolume,getCPU()-cpu);

  if( controllerType=="PID" )
  {
    // Here are some variables needed by the PID controller:
    if( !dbase.has_key("errorTimeIntegral") ) dbase.put<real>("errorTimeIntegral");
    dbase.get<real>("errorTimeIntegral")=0.;
  }
  

  return 0;
  
}
