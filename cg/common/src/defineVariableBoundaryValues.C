// This file automatically generated from defineVariableBoundaryValues.bC with bpp.
#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "ExternalBoundaryData.h"
#include "BodyForce.h"
#include "TimeFunction.h"
#include "UnstructuredMapping.h"

// The next file defines macros used for body forces (bodyForcing.bC) and boundary forces (defineVariableBoundaryValues.bC)
// ---------------------------------------------------------------------------------------------------------------------------
// 
// bodyForcingMacros.h: 
//   This file defines macros used for body forces (bodyForcing.bC) and boundary forces (defineVariableBoundaryValues.bC)
//
// ---------------------------------------------------------------------------------------------------------------------------


// =================================================================
// Macro to compute the grid point coordinates.
// =================================================================

// ============================================================================================
// This macro computes the directions in which the region box is longest.
// ============================================================================================

// ===================================================================================
// Macro: Add a body force or boundary force (i.e. assign the RHS to a BC):
//
//  This macro will add a body/boundary force over the appropriate region.
//
// Parameters:
//  TYPE : body or boundary to indicate whether this is a body forcing or BC forcing
//  I1,I2,I3 : apply forcing over this indicies.
//
// Implied Parameters:
//   regionType : a string (e.g. "box") denoting the region.
//   bodyForce : a BodyForce object that contains info on the region.
// NOTE: 
// This macro expects the perl variable $statements to hold the statements that assign 
// the body/boundary force at a single point 
// ===================================================================================


// =====================================================================================
// Save info about the body force region and profile in the bodyForce object.
// =====================================================================================


#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define ForBoundary(side,axis)   for( int axis=0; axis<numberOfDimensions; axis++ ) for( int side=0; side<=1; side++ )


// ===================================================================================================================
/// \brief Build the dialog that defines different options for the temperature boundary condition on a region.
///       For example, set the right-hand-side or define the coefficients in the temperature BC equation.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int Parameters::
buildTemperatureBoundaryConditionsDialog(DialogData & dialog, BodyForceRegionParameters & regionPar )
{

    real *temperatureMixedCoefficients =  regionPar.dbase.get<real[2]>("temperatureMixedCoefficients");
    int & temperatureBoundaryConditionCoefficientsOption = regionPar.dbase.get<int>("temperatureBoundaryConditionCoefficientsOption");


    aString temperatureEquationCommands[] = { "temperature BC is constant coefficients",
                                  					    "temperature BC is variable coefficients",
                                                                                        "define temperature time variation...",
                                  					    "" };
    dialog.addOptionMenu("Temperature BC:",temperatureEquationCommands, temperatureEquationCommands,temperatureBoundaryConditionCoefficientsOption);


//   aString forcingProfilesCommands[] = { "uniform forcing profile",
// 					"parabolic forcing profile",
//                                         "tanh forcing profile",
// 					"" };
//   dialog.addOptionMenu("Profile:",forcingProfilesCommands, forcingProfilesCommands, 0 );

  // ----- Text strings ------
    const int numberOfTextStrings=10;
    aString textCommands[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;

    textCommands[nt] = "temperature forcing:";  
    sPrintF(textStrings[nt],"%g",regionPar.dbase.get<real>("temperatureForce"));  nt++; 

    textCommands[nt] = "a0, an:";  
    sPrintF(textStrings[nt],"%g, %g (a0*T+an*T.n=)",temperatureMixedCoefficients[0],temperatureMixedCoefficients[1]);  nt++; 

  // null strings terminal list
    textCommands[nt]="";   textCommands[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textCommands, textCommands, textStrings);

    return 0;
}


//================================================================================
/// \brief: Look for a response to the ForcingProfiles
///
/// \param answer (input) : check this command 
/// \param regionPar (output) : changes to the region are returned in this object.
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int Parameters::
getTemperatureBoundaryConditionsOption(const aString & answer,
                    			  BodyForceRegionParameters & regionPar,
                    			  DialogData & dialog )
{
    GenericGraphicsInterface & gi = *dbase.get<GenericGraphicsInterface* >("ps");
    GraphicsParameters & psp = dbase.get<GraphicsParameters >("psp");

    int found=true; 
    int len=0;

    if( dialog.getTextValue(answer,"temperature forcing:","%e",regionPar.dbase.get<real>("temperatureForce")) )
    {
        printF("INFO: The temperature forcing defines the right-hand-side to the temperature BC.\n");
        printF("      The temperature BC may be a dirichlet BC, Neumann BC or mixed BC.\n");
    }

    else if( answer=="temperature BC is constant coefficients" ||
         	   answer=="temperature BC is variable coefficients" )
    {
        int & temperatureBoundaryConditionCoefficientsOption = regionPar.dbase.get<int>("temperatureBoundaryConditionCoefficientsOption");
        
        if( answer=="temperature BC is constant coefficients" )
            temperatureBoundaryConditionCoefficientsOption=BodyForce::temperatureBoundaryConditionIsConstantCoefficients;
        else if( answer=="temperature BC is variable coefficients" )
            temperatureBoundaryConditionCoefficientsOption=BodyForce::temperatureBoundaryConditionIsVariableCoefficients;
        else
        {
            OV_ABORT("getTemperatureBoundaryConditionsOption:ERROR: unknown temperature BC option.");
        }
    }
    else if( len=answer.matches("a0, an:") )
    {
        real & parabolicProfileDepth =  regionPar.dbase.get<real>("parabolicProfileDepth");
        real a0=0., an=1.;
        sScanF(answer(len,answer.length()-1),"%e %e",&a0,&an);

        real *temperatureMixedCoefficients =  regionPar.dbase.get<real[2]>("temperatureMixedCoefficients");
        temperatureMixedCoefficients[0]=a0;
        temperatureMixedCoefficients[1]=an;
        
        printF("Temperature BC a0*T+an*T.n=g is set to a0=%g, an=%g.\n",a0,an);
        dialog.setTextLabel("a0, an:",sPrintF(answer,"%g, %g (a0*T+an*T.n=)",a0,an));
    }
    else
    {
        found=false;
    }

    return found;
}





// =========================================================================================
/// \brief Interactively define boundary values that vary along a boundary.
///        This function is called from setBoundaryConditionValues.
///
/// \details This function can be used to define sub-regions on a boundary where the boundary    
///          values take on certain values. For example, one can set the temperature on one 
//           half of a cylinder to one value and another value on the second half. 
///
/// \note:  The actual boundary values are NOT assigned in this routine. This is done in the
///    setVariableBoundaryValues function.
/// 
/// \param side,axis,grid (input): assign boundary conditions for this face.
///
// =========================================================================================
int Parameters::
defineVariableBoundaryValues( int side, int axis, int grid, CompositeGrid & cg )
{
    
    assert( dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
    GenericGraphicsInterface & gi = *dbase.get<GenericGraphicsInterface* >("ps");

    Parameters & parameters = *this;

    printF("DefineVariableBoundaryValues: for (side,axis,grid)=(%i,%i,%i) gridName=%s\n",side,axis,grid,
       	 (const char*)cg[grid].getName());



  // We save the different boundary forcings in an array:
    if( !dbase.has_key("boundaryForcings") )
    {
        dbase.put<std::vector<BodyForce*> >("boundaryForcings");
    }
    
  // Here is the array of boundary forcings:
    std::vector<BodyForce*> & boundaryForcings =  dbase.get<std::vector<BodyForce*> >("boundaryForcings");

  // Here is where we save the current parameters that define any regions used to define boundary forcing
    BodyForceRegionParameters regionPar;

    regionPar.dbase.put<real>("temperatureForce",0.);
    real & temperatureForce=regionPar.dbase.get<real>("temperatureForce");
    real velocityForce[3]={0.,0.,0.};

  // The Temperature BC is a0*T = an*T.n = g 
    regionPar.dbase.put<int>("temperatureBoundaryConditionCoefficientsOption");
    int & temperatureBoundaryConditionCoefficientsOption = regionPar.dbase.get<int>("temperatureBoundaryConditionCoefficientsOption");
    temperatureBoundaryConditionCoefficientsOption=BodyForce::temperatureBoundaryConditionIsConstantCoefficients;

  // Here is where we store a0 and an: 
    regionPar.dbase.put<real[2]>("temperatureMixedCoefficients");
    real *temperatureMixedCoefficients =  regionPar.dbase.get<real[2]>("temperatureMixedCoefficients");
    temperatureMixedCoefficients[0]=0.;  // a0 
    temperatureMixedCoefficients[1]=1.;  // an 

  // Set this to true if boundary forcings are defined.
    bool & turnOnBoundaryForcing = dbase.get<bool>("turnOnBoundaryForcing");
    
  // boundary forcing name:
    aString boundaryForcingName="boundaryForcing0";
    
    real xa=0., xb=1., ya=0., yb=1.;

    GUIState dialog;

    dialog.setWindowTitle("Define Variable Boundary Values");
    dialog.setExitCommand("exit", "exit");

    aString cmds[] = {"choose region...",
                    // "add variable inflow",
		    // "add variable temperature",
		    // "add variable heat flux",
                                        "forcing profiles...",
                		    "add velocity forcing",
                                        "add temperature forcing",
                                        "temperature boundary conditions...",
                                        "set temperature BC",
                                        "define temperature time variation...",
                                        "plot",
                		    "help",
                		    ""};

    int numberOfPushButtons=8;  // number of entries in cmds
    int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 


    const int numberOfTextStrings=7;  // max number allowed
    aString textCommands[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;

    textCommands[nt] = "velocity forcing:";  
    sPrintF(textStrings[nt],"%g,%g,%g (u,v,w)",velocityForce[0],velocityForce[1],velocityForce[2]);  nt++; 

    textCommands[nt] = "boundary forcing name:";  
    sPrintF(textStrings[nt],"%s",(const char*)boundaryForcingName);  nt++;   

  // null strings terminal list
    textCommands[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textCommands, textCommands, textStrings);


  // ********************* Region options ********************************
    DialogData &boundaryForceRegionsDialog = dialog.getDialogSibling();
    boundaryForceRegionsDialog.setWindowTitle("Boundary Force Regions");
    boundaryForceRegionsDialog.setExitCommand("close region options", "close");
    buildBodyForceRegionsDialog( boundaryForceRegionsDialog,regionPar );


  // ********************* Forcing profile Options ********************************
    DialogData &forcingProfilesDialog = dialog.getDialogSibling();
    forcingProfilesDialog.setWindowTitle("Forcing Profiles");
    forcingProfilesDialog.setExitCommand("close forcing profiles", "close");
    buildForcingProfilesDialog( forcingProfilesDialog,regionPar );

  // ********************* temperature BC Options ********************************
    DialogData &temperatureBoundaryConditionsDialog = dialog.getDialogSibling();
    temperatureBoundaryConditionsDialog.setWindowTitle("Temperature Boundary Conditions");
    temperatureBoundaryConditionsDialog.setExitCommand("close temperature BC", "close");
    buildTemperatureBoundaryConditionsDialog( temperatureBoundaryConditionsDialog,regionPar );


    gi.pushGUI(dialog);

    int len=0;
    aString answer,line,answer2; 

    bool plotObject=true;
    GraphicsParameters & psp = dbase.get<GraphicsParameters >("psp");
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

    gi.appendToTheDefaultPrompt("varBC>"); // set the default prompt

    for( int it=0;; it++ )
    {
        if( it==0 && plotObject )
            answer="plotObject";
        else
            gi.getAnswer(answer,"");  // gi.getMenuItem(menu,answer);
  

        if( len=answer.matches("velocity forcing:") )
        {
            sScanF(answer(len,answer.length()-1),"%e %e %e",&velocityForce[0],&velocityForce[1],&velocityForce[2]);
            if( !gi.isGraphicsWindowOpen() )
                dialog.setTextLabel("velocity forcing:",sPrintF(answer,"%g,%g,%g (u,v,w)",
                                                        velocityForce[0],velocityForce[1],velocityForce[2]));

            printF("INFO: The velocity forcing defines the right-hand-side to the velocity BC.\n");

        }
        else if( dialog.getTextValue(answer,"boundary forcing name:","%s",boundaryForcingName) )
        {
            printF("Setting the boundary forcing name=[%s], this will be the name of NEXT boundary forcing created.\n",
           	     (const char*)boundaryForcingName);
        }
        else if( answer=="choose region..." )
        {
            boundaryForceRegionsDialog.showSibling();
        }
        else if( answer=="close region options" )
        {
            boundaryForceRegionsDialog.hideSibling(); 
        }
        else if( getBodyForceRegionsOption(answer,regionPar,boundaryForceRegionsDialog,cg ) )
        {
            printF("Answer=%s found in getBodyForceRegionsOption.\n",(const char*)answer);
        }

        else if( answer=="forcing profiles..." )
        {
            forcingProfilesDialog.showSibling();
        }
        else if( answer=="close forcing profiles" )
        {
            forcingProfilesDialog.hideSibling(); 
        }
        else if( getForcingProfilesOption(answer,regionPar,forcingProfilesDialog ) )
        {
            printF("Answer=%s found in getForcingProfilesOption.\n",(const char*)answer);
        }

        else if( answer=="temperature boundary conditions..." )
        {
            temperatureBoundaryConditionsDialog.showSibling();
        }
        else if( answer=="close temperature BC" )
        {
            temperatureBoundaryConditionsDialog.hideSibling(); 
        }
        else if( getTemperatureBoundaryConditionsOption(answer,regionPar,temperatureBoundaryConditionsDialog ) )
        {
            printF("Answer=%s found in getTemperatureBoundaryConditionsOption.\n",(const char*)answer);
        }

        else if( answer=="add velocity forcing"  ||
                          answer=="add temperature forcing" )
        {
      // Make a new BodyForce and add it to the list:
            boundaryForcings.push_back(new BodyForce());
        	  
            BodyForce & boundaryForce = *boundaryForcings[boundaryForcings.size()-1];
        	  
            printF("Creating a boundary forcing with name=[%s]. (Note: you should choose the name before creation).\n",
                          (const char*)boundaryForcingName);
            boundaryForce.dbase.get<aString >("bodyForcingName")=boundaryForcingName;

      // here is the default name for the next boundary forcing:
            sPrintF(boundaryForcingName,"boundaryForcing%i",boundaryForcings.size());
            dialog.setTextLabel("boundary forcing name:",boundaryForcingName);


            if( answer=="add velocity forcing" )
            {
                boundaryForce.dbase.get<aString >("forcingType")="variableInflow";
      	boundaryForce.dbase.get<bool >("forcingIsTimeDependent")=false; // this forcing does NOT depend on time
      	boundaryForce.dbase.get<bool >("forcingHasBeenAssigned")=false;

                boundaryForce.dbase.put<real[3]>("velocityForce");
                real *vf = boundaryForce.dbase.get<real[3]>("velocityForce");
      	for( int dir=0; dir<3; dir++ )
        	  vf[dir]=velocityForce[dir];
      	
            }
            else if( answer=="add temperature forcing" )
            {
                boundaryForce.dbase.get<aString >("forcingType")="variableTemperature";
      	boundaryForce.dbase.get<bool >("forcingIsTimeDependent")=false; // this forcing does NOT depend on time
      	boundaryForce.dbase.get<bool >("forcingHasBeenAssigned")=false;
                boundaryForce.dbase.put<real >("temperatureForce",temperatureForce);
            }
            else
            {
                OV_ABORT("error");
            }
            
      // Save (side,axis,grid) associated with this boundary forcing
            boundaryForce.dbase.put<int[3]>("sideAxisGrid");
            int * sideAxisGrid = boundaryForce.dbase.get<int[3]>("sideAxisGrid");
            sideAxisGrid[0]=side;
            sideAxisGrid[1]=axis;
            sideAxisGrid[2]=grid;
            
      // Save info about the body force region and profile: 
            {
        // Here is the region type ("box", "ellipse", ... ) chosen by the user
                const aString & regionType = regionPar.dbase.get<aString>("regionType");
        // Save the region type:
                if( !boundaryForce.dbase.has_key("regionType") )
                    boundaryForce.dbase.put<aString>("regionType");  // region type
                boundaryForce.dbase.get<aString>("regionType")=regionType;
                if( !boundaryForce.dbase.has_key("linesToPlot") )
                    boundaryForce.dbase.put<int[3]>("linesToPlot"); 
                int *linesToPlot = boundaryForce.dbase.get<int[3]>("linesToPlot"); 
                int *lines =  regionPar.dbase.get<int[3] >("linesToPlot");
                for( int i=0; i<3; i++ )
                    linesToPlot[i]=lines[i];
                if( regionType=="box" )
                {
                    boundaryForce.dbase.put<real[6] >("boxBounds");
                    real *boxBounds =  boundaryForce.dbase.get<real[6]>("boxBounds");
                    const real *bpar = regionPar.dbase.get<real[6]>("boxBounds");
                    for( int i=0; i<6; i++ )
                        boxBounds[i]=bpar[i];
                }
                else if( regionType=="ellipse" )
                {
                    boundaryForce.dbase.put<real[6] >("ellipse");
                    real *ellipse    =  boundaryForce.dbase.get<real[6]>("ellipse");
                    const real *epar =  regionPar.dbase.get<real[6]>("ellipse");
                    for( int i=0; i<6; i++ )
                        ellipse[i]=epar[i];
                }
                else if( regionType=="maskFromGridFunction" )
                {
          // region defined from a mask in a grid function
                }
                else if( regionType=="mapping" )
                {
          // region defined by a closed curve in 2D or a 3D surface
          // -- Make a copy of the Mapping that was temporarily saved in the regionPar --
                    assert( regionPar.dbase.has_key("bodyForceMapping") );
                    MappingRC *& regionParMapping = regionPar.dbase.get<MappingRC*>("bodyForceMapping"); 
                    assert( regionParMapping!=NULL );
                    printF("saveBodyForce: regionParMapping referenceCount=%i BEFORE\n",regionParMapping->getMapping().getReferenceCount());
                    if( !boundaryForce.dbase.has_key("bodyForceMapping") )
                    {
                        boundaryForce.dbase.put<MappingRC*>("bodyForceMapping");
                        boundaryForce.dbase.get<MappingRC*>("bodyForceMapping")=NULL;
                    }
                    MappingRC *& bodyForceMapping = boundaryForce.dbase.get<MappingRC*>("bodyForceMapping");
                    if( bodyForceMapping==NULL )
                        bodyForceMapping = new MappingRC();
                    bodyForceMapping->reference(*regionParMapping);
                    printF("saveBodyForce: regionParMapping referenceCount=%i\n",regionParMapping->getMapping().getReferenceCount());
                    printF("saveBodyForce: bodyForceMapping referenceCount=%i\n",bodyForceMapping->getMapping().getReferenceCount());
                }
                else
                {
                    printF("defineVariableBoundaryValues:ERROR: unexpected regionType=%s\n",(const char*)regionType);
                    OV_ABORT("ERROR: finish me...");
                }
        // Save the profile type
                const aString & profileType = regionPar.dbase.get<aString>("profileType");
                boundaryForce.dbase.put<aString>("profileType");  
                boundaryForce.dbase.get<aString>("profileType")=profileType;
                if( profileType=="parabolic" )
                {
                    boundaryForce.dbase.put<real>("parabolicProfileDepth");
                    boundaryForce.dbase.get<real>("parabolicProfileDepth")=regionPar.dbase.get<real>("parabolicProfileDepth"); 
                }
                else if( profileType=="tanh" )
                {
                    boundaryForce.dbase.put<real>("tanhProfileExponent");
                    boundaryForce.dbase.get<real>("tanhProfileExponent")=regionPar.dbase.get<real>("tanhProfileExponent"); 
                }
            }

        }

        else if( answer=="set temperature BC" )
        {
      // We save the different BC coefficients in an array of BodyForce objects:
            if( !dbase.has_key("variableBCInfo") )
            {
      	dbase.put<std::vector<BodyForce> >("variableBCInfo");
            }
            std::vector<BodyForce> & variableBCInfo = dbase.get<std::vector<BodyForce> >("variableBCInfo");

      // Make a new BodyForce and add it to the list:
            variableBCInfo.push_back(BodyForce());
        	  
            BodyForce & boundaryForce = variableBCInfo[variableBCInfo.size()-1];

      // Save (side,axis,grid) associated with this boundary forcing
            boundaryForce.dbase.put<int[3]>("sideAxisGrid");
            int * sideAxisGrid = boundaryForce.dbase.get<int[3]>("sideAxisGrid");
            sideAxisGrid[0]=side;
            sideAxisGrid[1]=axis;
            sideAxisGrid[2]=grid;
            
      // Save info about the body force region and profile: 
            {
        // Here is the region type ("box", "ellipse", ... ) chosen by the user
                const aString & regionType = regionPar.dbase.get<aString>("regionType");
        // Save the region type:
                if( !boundaryForce.dbase.has_key("regionType") )
                    boundaryForce.dbase.put<aString>("regionType");  // region type
                boundaryForce.dbase.get<aString>("regionType")=regionType;
                if( !boundaryForce.dbase.has_key("linesToPlot") )
                    boundaryForce.dbase.put<int[3]>("linesToPlot"); 
                int *linesToPlot = boundaryForce.dbase.get<int[3]>("linesToPlot"); 
                int *lines =  regionPar.dbase.get<int[3] >("linesToPlot");
                for( int i=0; i<3; i++ )
                    linesToPlot[i]=lines[i];
                if( regionType=="box" )
                {
                    boundaryForce.dbase.put<real[6] >("boxBounds");
                    real *boxBounds =  boundaryForce.dbase.get<real[6]>("boxBounds");
                    const real *bpar = regionPar.dbase.get<real[6]>("boxBounds");
                    for( int i=0; i<6; i++ )
                        boxBounds[i]=bpar[i];
                }
                else if( regionType=="ellipse" )
                {
                    boundaryForce.dbase.put<real[6] >("ellipse");
                    real *ellipse    =  boundaryForce.dbase.get<real[6]>("ellipse");
                    const real *epar =  regionPar.dbase.get<real[6]>("ellipse");
                    for( int i=0; i<6; i++ )
                        ellipse[i]=epar[i];
                }
                else if( regionType=="maskFromGridFunction" )
                {
          // region defined from a mask in a grid function
                }
                else if( regionType=="mapping" )
                {
          // region defined by a closed curve in 2D or a 3D surface
          // -- Make a copy of the Mapping that was temporarily saved in the regionPar --
                    assert( regionPar.dbase.has_key("bodyForceMapping") );
                    MappingRC *& regionParMapping = regionPar.dbase.get<MappingRC*>("bodyForceMapping"); 
                    assert( regionParMapping!=NULL );
                    printF("saveBodyForce: regionParMapping referenceCount=%i BEFORE\n",regionParMapping->getMapping().getReferenceCount());
                    if( !boundaryForce.dbase.has_key("bodyForceMapping") )
                    {
                        boundaryForce.dbase.put<MappingRC*>("bodyForceMapping");
                        boundaryForce.dbase.get<MappingRC*>("bodyForceMapping")=NULL;
                    }
                    MappingRC *& bodyForceMapping = boundaryForce.dbase.get<MappingRC*>("bodyForceMapping");
                    if( bodyForceMapping==NULL )
                        bodyForceMapping = new MappingRC();
                    bodyForceMapping->reference(*regionParMapping);
                    printF("saveBodyForce: regionParMapping referenceCount=%i\n",regionParMapping->getMapping().getReferenceCount());
                    printF("saveBodyForce: bodyForceMapping referenceCount=%i\n",bodyForceMapping->getMapping().getReferenceCount());
                }
                else
                {
                    printF("defineVariableBoundaryValues:ERROR: unexpected regionType=%s\n",(const char*)regionType);
                    OV_ABORT("ERROR: finish me...");
                }
        // Save the profile type
                const aString & profileType = regionPar.dbase.get<aString>("profileType");
                boundaryForce.dbase.put<aString>("profileType");  
                boundaryForce.dbase.get<aString>("profileType")=profileType;
                if( profileType=="parabolic" )
                {
                    boundaryForce.dbase.put<real>("parabolicProfileDepth");
                    boundaryForce.dbase.get<real>("parabolicProfileDepth")=regionPar.dbase.get<real>("parabolicProfileDepth"); 
                }
                else if( profileType=="tanh" )
                {
                    boundaryForce.dbase.put<real>("tanhProfileExponent");
                    boundaryForce.dbase.get<real>("tanhProfileExponent")=regionPar.dbase.get<real>("tanhProfileExponent"); 
                }
            }


      // --- The variable coefficient BC's are stored in the BoundaryData object for this grid ---

            std::vector<BoundaryData> & boundaryData = dbase.get<std::vector<BoundaryData> >("boundaryData");
            if( grid >= boundaryData.size() )
      	boundaryData.resize(grid+1,BoundaryData());

            BoundaryData & bd = boundaryData[grid];
      // Here is the array where we store the variable coefficients.
            RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(BoundaryData::variableCoefficientTemperatureBC,side,axis );
            const bool varCoeffExists = varCoeff.getLength(0)>0;  // true if we have already allocated space for the array

      // --- allocate space for the varCoeff array (should we do this in the BoundaryData class??) --- 
            MappedGrid & mg = cg[grid];
            const int numberOfDimensions = mg.numberOfDimensions();
            
            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            getBoundaryIndex(mg.dimension(),side,axis,I1,I2,I3);
            Iv[axis]=Range(mg.gridIndexRange(side,axis),mg.gridIndexRange(side,axis));
            
            bool ok=true;
            #ifdef USE_PPP
                intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
                int includeGhost=1;
                ok=ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,I1,I2,I3,includeGhost);
            #endif
            if( ok && !varCoeffExists )
            {
                varCoeff.redim(I1,I2,I3,2);
            }

            int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
            real xv[3]={0.,0.,0.};

      // -- To save space we do not create the array of grid vertices on rectangular grids --
            const bool isRectangular = mg.isRectangular();
            real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
            int iv0[3]={0,0,0}; //
            if( isRectangular )
            {
      	mg.getRectangularGridParameters( dvx, xab );
      	for( int dir=0; dir<numberOfDimensions; dir++ )
      	{
        	  iv0[dir]=mg.gridIndexRange(0,dir);
        	  if( mg.isAllCellCentered() )
          	    xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      	}
            }
            else
            { // for curvilinear grids we need the array of grid vertices
      	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
            }
    
      // This macro defines the grid points for rectangular grids:
            #undef XC
            #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

      // Extract local serial arrays:
            OV_GET_SERIAL_ARRAY_CONDITIONAL(real,mg.vertex(),vertexLocal,!isRectangular);

            if( temperatureBoundaryConditionCoefficientsOption==BodyForce::temperatureBoundaryConditionIsConstantCoefficients )
            {
        // --- We are setting a local const coefficient Temperature BC ---


//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)
      	if( ok && !varCoeffExists )
      	{
	  // --- Initialize the variable coefficients to the current default values ---
	  // The default coefficients appear in the bcData array ...
            
                    const int numberOfComponents = dbase.get<int >("numberOfComponents");
                    const RealArray & bcData = dbase.get<RealArray>("bcData");

        	  const int tc=dbase.get<int >("tc");
        	  assert( tc>=0 );
        	  const real a0= mixedCoeff(tc,side,axis,grid);
        	  const real an= mixedNormalCoeff(tc,side,axis,grid);
                    
        	  if( true )
          	    printF("Default temperature BC is: a0=%g, an=%g for grid=%i (side,axis)=(%i,%i)\n",
               		   a0,an,grid,side,axis);


        	  varCoeff(I1,I2,I3,0)=a0;
        	  varCoeff(I1,I2,I3,1)=an;

      	}
      	
      	const real & a0= temperatureMixedCoefficients[0];
      	const real & an= temperatureMixedCoefficients[1];
      	if( true )
        	  printF("Setting a local const. coeff. temperature BC : a0=%g, an=%g for grid=%i (side,axis)=(%i,%i)\n",
             		 a0,an,grid,side,axis);

      	if( ok )
      	{

        	  const aString & regionType = regionPar.dbase.get<aString>("regionType");
          // -- todo : fill in bodyForce with region, profile etc. ---

        	  BodyForce & bodyForce = boundaryForce;
        	  
          // Here are the statements that assign the local BC coeff's
          // -- do this for now: 
                    const int addBodyForce=0, addBoundaryForce=1;
                      const int forcingType=addBoundaryForce;
                    real profileFactor=1.;  // The forcing is multiplied by this factor (changed below for parabolic, ...)
                    if( regionType=="box" )
                    {
            // -- drag is applied over a box (square in 2D) --
                        const real *boxBounds =  bodyForce.dbase.get<real[6] >("boxBounds");
                        #define xab(side,axis) boxBounds[(side)+2*(axis)]
                        const real & xa = xab(0,0), &xb = xab(1,0);
                        const real & ya = xab(0,1), &yb = xab(1,1);
                        const real & za = xab(0,2), &zb = xab(1,2);
                        const aString & profileType = bodyForce.dbase.get<aString>("profileType");
            // if( debug() & 4 )
            // printF("computeBodyForce: profileType=%s, box bounds = [%e,%e]x[%e,%e][%e,%e]\n",(const char*)profileType,xa,xb,ya,yb,za,zb);
                        if( profileType=="uniform" )
                        {
              // --- uniform profile ---
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
        	// Get the grid coordinates xv[axis]:
                                if( isRectangular )
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=XC(iv,axis);
                                }
                                else
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=vertexLocal(i1,i2,i3,axis);
                                }
        	// Turn on the drag if we are inside the box
                      	if( xv[0]>=xa && xv[0]<=xb && xv[1]>=ya && xv[1]<=yb && xv[2]>=za && xv[2]<=zb )
                      	{
                                    varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                      	}
                            } // end FOR_3D
                        }
                        else if( profileType=="parabolic" )
                        {
              // -- Parabolic profile --
              // Near each edge of the region the parabolic profile looks like: 
              //     u(x) = U(x)*( 1 - (1-d(x)/W)^2 ),  for d(x) < W
              //     u(x) = U(x) ,                      for d(x) > W
              // where d(x) is the distance from the point x to the box that defines the region,
              // and W=parabolicProfileDepth is the width of the parabolic profile. 
                            const real & parabolicProfileDepth = bodyForce.dbase.get<real>("parabolicProfileDepth");
                            const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;
              // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
                            int dir1, dir2;  
                              if( numberOfDimensions==2 )
                              {
                 // dir1 = the longest axes of the box (in 2D) 
                 // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
                                  dir1 = xWidth > yWidth ? 0 : 1;
                              }
                              else
                              {
                 // Find the two directions (dir1,dir2) that define the two longest axes of the box
                 // 
                                  if( xWidth < min(yWidth,zWidth) )
                                  {
                                      dir1=1; dir2=2;
                                  }
                                  else if( yWidth < min(xWidth,zWidth) )
                                  {
                                      dir1=0; dir2=2;
                                  }
                                  else
                                  {
                                      dir1=0; dir2=1;
                                  }
                              }
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
        	// Get the grid coordinates xv[axis]:
                                if( isRectangular )
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=XC(iv,axis);
                                }
                                else
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=vertexLocal(i1,i2,i3,axis);
                                }
                                real dist;
                  // Boundary: compute the minimum distance to the box edges (ignore box faces in the normal direction to the boundary):
                                    dist = min( xv[dir1]-xab(0,dir1), xab(1,dir1)-xv[dir1] );
                                    if( numberOfDimensions==3 )
                              	    dist=min( dist, xv[dir2]-xab(0,dir2), xab(1,dir2)-xv[dir2] );
        	// printF("parabolic: (i1,i2)=(%i,%i) x=(%g,%g) dist=%g \n",i1,i2,xv[0],xv[1],dist);
                      	if( dist>=0. )
                      	{  
                                    dist /= parabolicProfileDepth;
                                    if( dist<1. )
                        	  {
                    // 1 - (1-d)^2 = 2*d-d^2 = d*(2-d)
                                        profileFactor = dist*(2.-dist);
                        	  }
                        	  else
                        	  {
                                        profileFactor=1.;
                        	  }
        	  // printF("         : profileFactor=%g \n",profileFactor);
                                    varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                      	}
                            } // end FOR_3D
                        }
                        else if( profileType=="tanh" )
                        {
              // -- Tanh profile:
              // The one-dimensional tanh profile is of the form:
              //     u = U(x) *[  .5*( tanh( b*(x-xa) ) - tanh( b*(x-xb) ) ) ]
              // 
                            const real & b  = bodyForce.dbase.get<real>("tanhProfileExponent");
                            const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;
              // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
                            int dir1, dir2;  
                              if( numberOfDimensions==2 )
                              {
                 // dir1 = the longest axes of the box (in 2D) 
                 // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
                                  dir1 = xWidth > yWidth ? 0 : 1;
                              }
                              else
                              {
                 // Find the two directions (dir1,dir2) that define the two longest axes of the box
                 // 
                                  if( xWidth < min(yWidth,zWidth) )
                                  {
                                      dir1=1; dir2=2;
                                  }
                                  else if( yWidth < min(xWidth,zWidth) )
                                  {
                                      dir1=0; dir2=2;
                                  }
                                  else
                                  {
                                      dir1=0; dir2=1;
                                  }
                              }
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
        	// Get the grid coordinates xv[axis]:
                                if( isRectangular )
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=XC(iv,axis);
                                }
                                else
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=vertexLocal(i1,i2,i3,axis);
                                }
                // --> we could have a cutoff if we are far away from the transition zone, to avoid
                //  evaluating the tanh's.
                  // Boundary force:
                                    profileFactor = .5*( tanh( b*(xv[dir1]-xab(0,dir1)) ) - tanh( b*(xv[dir1]-xab(1,dir1)) ) );
                        	  if( numberOfDimensions==3 )
                                        profileFactor *= .5*( tanh( b*(xv[dir2]-xab(0,dir2)) ) - tanh( b*(xv[dir2]-xab(1,dir2)) ) );
                                varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                            } // end FOR_3D
                        }
                        else
                        {
                            printF("addBodyForceMacro: ERROR: unknown profileType=%s\n",(const char*)profileType);
                            OV_ABORT("ERROR");
                        }
                    }
                    else if( regionType=="ellipse" )
                    {
            // -- drag is applied over an ellipse --
            //   [(x-xe)/ae]^2 + [(y-ye)/be]^2 + [(z-ze)/ce]^2 = 1
                        const real *ellipse =  bodyForce.dbase.get<real[6] >("ellipse");
                        const real ae = ellipse[0];
                        const real be = ellipse[1];
                        const real ce = ellipse[2];
                        const real xe = ellipse[3];
                        const real ye = ellipse[4];
                        const real ze = ellipse[5];
                        FOR_3D(i1,i2,i3,I1,I2,I3)
                        {
              // Get the grid coordinates xv[axis]:
                                if( isRectangular )
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=XC(iv,axis);
                                }
                                else
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=vertexLocal(i1,i2,i3,axis);
                                }
                            real rad;
                            if( numberOfDimensions==2 )
                            {
                      	real xa = (xv[0]-xe)/ae;
                      	real ya = (xv[1]-ye)/be;
                      	rad = xa*xa+ya*ya;
                            }
                            else
                            {
                      	real xa = (xv[0]-xe)/ae;
                      	real ya = (xv[1]-ye)/be;
                      	real za = (xv[2]-ze)/ce;
                      	rad = xa*xa+ya*ya+za*za;
                            }
              //       // amp = 1 inside the circle and 0 outside
              //       // -- here is a smooth transition from 0 to damp at "radius" rad0
              //       real amp = .5*damp*(tanh( -beta*(rad-rad0) )+1.);
              //       fg(i1,i2,i3,uc) =  -amp*ug(i1,i2,i3,uc);
              //       fg(i1,i2,i3,vc) =  -amp*ug(i1,i2,i3,vc);
              // here we turn on the drag as a step function at rad=rad0
                            if( rad < 1. )
                            {
                      	varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                            }
                        } // end FOR_3D
                    }
                    else if( regionType=="maskFromGridFunction" )
                    {
            // ---- The region is defined by a grid function that holds a mask ----
                        if( !parameters.dbase.has_key("bodyForceMaskGridFunction") )
                        {
                            printF("ERROR: regionType==`maskFromGridFunction' but the grid function does not exist!\n");
                            OV_ABORT("ERROR");
                        }
            // printF("Setting a body force for regionType==maskFromGridFunction for grid=%i\n",grid);
                        realCompositeGridFunction *maskPointer = 
                                                            parameters.dbase.get<realCompositeGridFunction*>("bodyForceMaskGridFunction");
                        assert( maskPointer!=NULL );
                        realCompositeGridFunction & bodyForceMask = *maskPointer;
                        realArray & bfMask = bodyForceMask[grid];
                        OV_GET_SERIAL_ARRAY(real,bfMask,bfMaskLocal);
                        getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
            // restrict bounds to local processor, include ghost
                        bool ok = ParallelUtility::getLocalArrayBounds(bfMask,bfMaskLocal,I1,I2,I3,1);
                        if( ok )
                        {
                            profileFactor=1.;
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
                      	if( bfMaskLocal(i1,i2,i3)<=0. )  // signed distance 
                      	{
                                    varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                      	}
                            }
                        }
                    }
                    else if( regionType=="mapping" )
                    {
            // --- region is defined by a Mapping ---
            //   2D : closed curve
            //   3D : water-tight surface
                        if( !bodyForce.dbase.has_key("bodyForceMapping") )
                        {
                            printF("computeBodyForcing:WARNING: there is no body force Mapping!\n");
                            continue;
                        }
                        MappingRC *& pBodyForceMapping = bodyForce.dbase.get<MappingRC*>("bodyForceMapping");
                        if( pBodyForceMapping==NULL )
                        {
                            printF("computeBodyForcing:WARNING: the body force Mapping is NULL!\n");
                            continue;
                        }
                        Mapping & bodyForceMapping = pBodyForceMapping->getMapping();
                        if( numberOfDimensions==2 )
                        {
                            IntegerArray cross(1);
                            RealArray xa(1,3);
                            xa=0.;
                            assert( bodyForceMapping.approximateGlobalInverse !=NULL );
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
        	// Get the grid coordinates xv[axis]:
                                if( isRectangular )
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=XC(iv,axis);
                                }
                                else
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=vertexLocal(i1,i2,i3,axis);
                                }
        	// Turn on the drag if we are inside the body
                // ----- OPTIMZE ME -- could save a mask ---
                      	cross=0;
                                xa(0,0)=xv[0]; xa(0,1)=xv[1];
                      	bodyForceMapping.approximateGlobalInverse->countCrossingsWithPolygon( xa,cross );
                                int inside = (cross(0) % 2 == 0) ? 0 : +1;
        	// printF("computeBodyForcing: point (%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),inside);
                      	if( inside )
                      	{
                                    varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                      	}
                            } // end FOR_3D
                        }
                        else
                        {
                            IntegerArray inside(1); 
                            RealArray xa(1,3);
                            xa=0.;
                            assert( bodyForceMapping.getClassName()=="UnstructuredMapping" );
                            UnstructuredMapping & uMap = (UnstructuredMapping&)bodyForceMapping;
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
        	// Get the grid coordinates xv[axis]:
                                if( isRectangular )
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=XC(iv,axis);
                                }
                                else
                                {
                                    for( int axis=0; axis<numberOfDimensions; axis++ )
                                        xv[axis]=vertexLocal(i1,i2,i3,axis);
                                }
        	// Turn on the drag if we are inside the body
                // ----- OPTIMZE ME -- could do many pts at once, save a mask ---
                                xa(0,0)=xv[0]; xa(0,1)=xv[1]; xa(0,2)=xv[2];
                                #ifndef USE_PPP
                            	  uMap.insideOrOutside(xa,inside);
                                #else
                                    OV_ABORT("finish me for parallel");
                                #endif
        	// printF("computeBodyForcing: point (%8.2e,%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),xa(0,2),inside(0));
                      	if( inside(0) )
                      	{
                                    varCoeff(i1,i2,i3,0) = a0; varCoeff(i1,i2,i3,1) = an;
                      	}
                            } // end FOR_3D
                        }
                    }
                    else
                    {
                        printF("computeBodyForcing:ERROR: unexpected regionType=%s\n",(const char*)regionType);
                        OV_ABORT("ERROR: finish me...");
                    }

          // printF(" ********** FINISH ME **************\n");
        	  
      	}
      	if( true )
      	{
        	  ::display(varCoeff,"varCoeff for temperature BC","%5.2f ");
      	}
      	

            }
            else if( temperatureBoundaryConditionCoefficientsOption==BodyForce::temperatureBoundaryConditionIsVariableCoefficients )
            {
            }

        }
        else if( answer=="define temperature time variation..." )
        {
      // -- Define a time dependent temperature forcing for the current bodyForce --

            printF("Define a time dependent temperature boundary condition...\n");
            
            BodyForce & boundaryForcing = *boundaryForcings[boundaryForcings.size()-1];
            boundaryForcing.dbase.get<bool >("forcingIsTimeDependent")=true; // this forcing DOES depend on time

            if( !boundaryForcing.dbase.has_key("timeFunctionTemperature") )
            {
                boundaryForcing.dbase.put<TimeFunction>("timeFunctionTemperature");
            }
            TimeFunction & timeFunction = boundaryForcing.dbase.get<TimeFunction>("timeFunctionTemperature");

            timeFunction.update(gi);

        }

        else if( answer=="plot" )
        {
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
            psp.set(GI_TOP_LABEL,cg[grid].getName());
            gi.erase();
            PlotIt::plot(gi,cg[grid],psp); 
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
        }
        else if( answer=="help" )
        {

            printF("Use this routine to define values on the boundaries of grids that vary over the face of one grid.\n");
    

        }
        else if( answer=="exit" )
        {
            break;
        }
        else if( answer=="plotObject" )
        {
            plotObject=true;
        }
        else
        {
            printF("Unknown answer =[%s]\n",(const char*)answer2);
            gi.stopReadingCommandFile();
        }

        turnOnBoundaryForcing = boundaryForcings.size()>0;

        if( plotObject )
        {
            psp.set(GI_TOP_LABEL,cg[grid].getName());
            gi.erase();

      // Plot body/boundary forcing regions and immersed boundaries. 
            BodyForce::plotForcingRegions(gi,dbase,cg,psp); 

            PlotIt::plot(gi,cg[grid],psp); 
        }
    }
    gi.erase();
    gi.unAppendTheDefaultPrompt();  // reset

    gi.popGUI(); // restore the previous GUI

    turnOnBoundaryForcing = boundaryForcings.size()>0;

    return 0;

}


// =========================================================================================
/// \brief Assign the right-hand-side for variable boundary conditions.
///
/// \note  This function assigns the variable boundary conditions.
///
/// \param t (input) : current time.
/// \param gf0 (input) : holds the current solution.
/// \param grid (input): the component grid we are assigning.
/// \param forcingType (input) : if forcingType==computeForcing then return the rhs for the 
///  boundary condition; if forcingType==computeTimeDerivativeOfForcing then return the 
///   first time derivative of the forcing.
///
// =========================================================================================
int DomainSolver::
setVariableBoundaryValues(const real & t, 
                    			  GridFunction & gf0,
                    			  const int & grid,
                    			  int side0 /* = -1 */,
                    			  int axis0 /* = -1 */,
                    			  ForcingTypeEnum forcingType /* =computeForcing */)
{

    if( !parameters.dbase.get<bool>("turnOnBoundaryForcing") )
    {
    // there are no boundary forcings
        return 0;
    }
    

  // printF("***setVariableBoundaryValues: grid=%i (side,axis)=(%i,%i)\n",grid,side0,axis0 );

    realMappedGridFunction & u = gf0.u[grid];
  // realMappedGridFunction & gridVelocity = gf0.getGridVelocity(grid);


  // Here is the array of boundary forcings:
    std::vector<BodyForce*> & boundaryForcings =  parameters.dbase.get<std::vector<BodyForce*> >("boundaryForcings");



    MappedGrid & mg = *u.getMappedGrid();
    const int numberOfDimensions = mg.numberOfDimensions();
    
    assert( side0>=-1 && side0<2 );
    assert( axis0>=-1 && axis0<numberOfDimensions );
    
    const int axisStart= axis0==-1 ? 0 : axis0;
    const int axisEnd  = axis0==-1 ? numberOfDimensions-1 : axis0;
    const int sideStart= side0==-1 ? 0 : side0;
    const int sideEnd  = side0==-1 ? 1 : side0;

    int numberOfSidesAssigned=0;

    Range C(0,parameters.dbase.get<int >("numberOfComponents")-1);
    const int pc=parameters.dbase.get<int >("pc");
    const int uc=parameters.dbase.get<int >("uc");
    const int vc=parameters.dbase.get<int >("vc");
    const int wc=parameters.dbase.get<int >("wc");
    const int tc=parameters.dbase.get<int >("tc");
    
    const bool gridIsMoving = parameters.gridIsMoving(grid); // true if the grid is moving

    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
    real xv[3]={0.,0.,0.};

  // -- To save space we do not create the array of grid vertices on rectangular grids --
    const bool isRectangular = mg.isRectangular();
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    if( isRectangular )
    {
        mg.getRectangularGridParameters( dvx, xab );
        for( int dir=0; dir<numberOfDimensions; dir++ )
        {
            iv0[dir]=mg.gridIndexRange(0,dir);
            if( mg.isAllCellCentered() )
      	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
        }
    }
    else
    { // for curvilinear grids we need the array of grid vertices
        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
    }
    
  // This macro defines the grid points for rectangular grids:
    #undef XC
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

  // Extract local serial arrays:
    OV_GET_SERIAL_ARRAY_CONDITIONAL(real,mg.vertex(),vertexLocal,!isRectangular);
    OV_GET_SERIAL_ARRAY(real,u,uLocal);

    Index Ib1,Ib2,Ib3;


  // --- loop over different boundary forcings ---
    bool pBoundaryDataHasBeenInitialized[6];  // set to true if the boundary data for this grid has been initialized to zero.
#define boundaryDataHasBeenInitialized(side,axis) pBoundaryDataHasBeenInitialized[(side)+2*(axis)]
    ForBoundary(side,axis)
    {
        boundaryDataHasBeenInitialized(side,axis)=false;
    }
    
    for( int bf=0; bf<boundaryForcings.size(); bf++ )
    {
        BodyForce & bodyForce = *boundaryForcings[bf];
        const bool forcingIsTimeDependent = bodyForce.dbase.get<bool >("forcingIsTimeDependent");
        bool & forcingHasBeenAssigned = bodyForce.dbase.get<bool >("forcingHasBeenAssigned");

        if( !forcingIsTimeDependent && forcingHasBeenAssigned )
        {
      // This forcing is NOT time dependent and has already been assigned.
            continue;
        }


    // -- find which side this boundary force applies to ---

        const int *sideAxisGrid = bodyForce.dbase.get<int[3]>("sideAxisGrid");
        const int side =sideAxisGrid[0];
        const int axis =sideAxisGrid[1];
        const int gridForForcing=sideAxisGrid[2];
        if( gridForForcing!=grid || side<sideStart || side>sideEnd || axis<axisStart || axis>axisEnd )
            continue;
            
        getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

        int includeGhost=1;
        bool ok=ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,includeGhost);
        if( !ok ) continue;  // no points on this processor

        const aString & forcingType = bodyForce.dbase.get<aString >("forcingType");
        const aString & regionType = bodyForce.dbase.get<aString>("regionType");

        forcingHasBeenAssigned=true;  // for time independent forcing we only need to evaluate the RHS once.
        numberOfSidesAssigned++;

        RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

    // initialize to zero first time through. 
        if( !boundaryDataHasBeenInitialized(side,axis) )
        {
            boundaryDataHasBeenInitialized(side,axis)=true;
            bd=0.;
        }
        
        if( forcingType=="variableTemperature" && tc>=0 )
        {
      // -- assign a given temperature in the region ---

            const real temperatureForce = bodyForce.dbase.get<real >("temperatureForce");

            real bodyTemp = temperatureForce;
            if( bodyForce.dbase.has_key("timeFunctionTemperature") )
            {
	// The temperature is time dependent
	// *wdh* 2012/10/02 -- CHECK ME --
      	TimeFunction & timeFunction = bodyForce.dbase.get<TimeFunction>("timeFunctionTemperature");
      	timeFunction.eval(t,bodyTemp);
      	printF("BBB timeFunction.eval: t=%9.3e, bodyTemp=%8.2e\n",t,bodyTemp);
            }

            if( true || t==0. || debug() & 4 )
      	printF("setVariableBoundaryValues: grid=%i (side,axis)=(%i,%i) variableTemperature: bf=%i, region=%s, bodyTemp=%g, "
             	       "at time t=%9.3e\n",
             	       grid,side,axis,bf,(const char*)regionType,bodyTemp,t);

      // Here are the statements that impose the forcing:
      // Note: the forcing is multipled by profileFactor (defined in the add body force macro)

                const int addBodyForce=0, addBoundaryForce=1;
                  const int forcingType=addBoundaryForce;
                real profileFactor=1.;  // The forcing is multiplied by this factor (changed below for parabolic, ...)
                if( regionType=="box" )
                {
          // -- drag is applied over a box (square in 2D) --
                    const real *boxBounds =  bodyForce.dbase.get<real[6] >("boxBounds");
                    #define xab(side,axis) boxBounds[(side)+2*(axis)]
                    const real & xa = xab(0,0), &xb = xab(1,0);
                    const real & ya = xab(0,1), &yb = xab(1,1);
                    const real & za = xab(0,2), &zb = xab(1,2);
                    const aString & profileType = bodyForce.dbase.get<aString>("profileType");
          // if( debug() & 4 )
          // printF("computeBodyForce: profileType=%s, box bounds = [%e,%e]x[%e,%e][%e,%e]\n",(const char*)profileType,xa,xb,ya,yb,za,zb);
                    if( profileType=="uniform" )
                    {
            // --- uniform profile ---
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
      	// Turn on the drag if we are inside the box
                  	if( xv[0]>=xa && xv[0]<=xb && xv[1]>=ya && xv[1]<=yb && xv[2]>=za && xv[2]<=zb )
                  	{
                                bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                  	}
                        } // end FOR_3D
                    }
                    else if( profileType=="parabolic" )
                    {
            // -- Parabolic profile --
            // Near each edge of the region the parabolic profile looks like: 
            //     u(x) = U(x)*( 1 - (1-d(x)/W)^2 ),  for d(x) < W
            //     u(x) = U(x) ,                      for d(x) > W
            // where d(x) is the distance from the point x to the box that defines the region,
            // and W=parabolicProfileDepth is the width of the parabolic profile. 
                        const real & parabolicProfileDepth = bodyForce.dbase.get<real>("parabolicProfileDepth");
                        const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;
            // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
                        int dir1, dir2;  
                          if( numberOfDimensions==2 )
                          {
               // dir1 = the longest axes of the box (in 2D) 
               // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
                              dir1 = xWidth > yWidth ? 0 : 1;
                          }
                          else
                          {
               // Find the two directions (dir1,dir2) that define the two longest axes of the box
               // 
                              if( xWidth < min(yWidth,zWidth) )
                              {
                                  dir1=1; dir2=2;
                              }
                              else if( yWidth < min(xWidth,zWidth) )
                              {
                                  dir1=0; dir2=2;
                              }
                              else
                              {
                                  dir1=0; dir2=1;
                              }
                          }
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
                            real dist;
                // Boundary: compute the minimum distance to the box edges (ignore box faces in the normal direction to the boundary):
                                dist = min( xv[dir1]-xab(0,dir1), xab(1,dir1)-xv[dir1] );
                                if( numberOfDimensions==3 )
                          	    dist=min( dist, xv[dir2]-xab(0,dir2), xab(1,dir2)-xv[dir2] );
      	// printF("parabolic: (i1,i2)=(%i,%i) x=(%g,%g) dist=%g \n",i1,i2,xv[0],xv[1],dist);
                  	if( dist>=0. )
                  	{  
                                dist /= parabolicProfileDepth;
                                if( dist<1. )
                    	  {
                  // 1 - (1-d)^2 = 2*d-d^2 = d*(2-d)
                                    profileFactor = dist*(2.-dist);
                    	  }
                    	  else
                    	  {
                                    profileFactor=1.;
                    	  }
      	  // printF("         : profileFactor=%g \n",profileFactor);
                                bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                  	}
                        } // end FOR_3D
                    }
                    else if( profileType=="tanh" )
                    {
            // -- Tanh profile:
            // The one-dimensional tanh profile is of the form:
            //     u = U(x) *[  .5*( tanh( b*(x-xa) ) - tanh( b*(x-xb) ) ) ]
            // 
                        const real & b  = bodyForce.dbase.get<real>("tanhProfileExponent");
                        const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;
            // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
                        int dir1, dir2;  
                          if( numberOfDimensions==2 )
                          {
               // dir1 = the longest axes of the box (in 2D) 
               // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
                              dir1 = xWidth > yWidth ? 0 : 1;
                          }
                          else
                          {
               // Find the two directions (dir1,dir2) that define the two longest axes of the box
               // 
                              if( xWidth < min(yWidth,zWidth) )
                              {
                                  dir1=1; dir2=2;
                              }
                              else if( yWidth < min(xWidth,zWidth) )
                              {
                                  dir1=0; dir2=2;
                              }
                              else
                              {
                                  dir1=0; dir2=1;
                              }
                          }
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
              // --> we could have a cutoff if we are far away from the transition zone, to avoid
              //  evaluating the tanh's.
                // Boundary force:
                                profileFactor = .5*( tanh( b*(xv[dir1]-xab(0,dir1)) ) - tanh( b*(xv[dir1]-xab(1,dir1)) ) );
                    	  if( numberOfDimensions==3 )
                                    profileFactor *= .5*( tanh( b*(xv[dir2]-xab(0,dir2)) ) - tanh( b*(xv[dir2]-xab(1,dir2)) ) );
                            bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                        } // end FOR_3D
                    }
                    else
                    {
                        printF("addBodyForceMacro: ERROR: unknown profileType=%s\n",(const char*)profileType);
                        OV_ABORT("ERROR");
                    }
                }
                else if( regionType=="ellipse" )
                {
          // -- drag is applied over an ellipse --
          //   [(x-xe)/ae]^2 + [(y-ye)/be]^2 + [(z-ze)/ce]^2 = 1
                    const real *ellipse =  bodyForce.dbase.get<real[6] >("ellipse");
                    const real ae = ellipse[0];
                    const real be = ellipse[1];
                    const real ce = ellipse[2];
                    const real xe = ellipse[3];
                    const real ye = ellipse[4];
                    const real ze = ellipse[5];
                    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                    {
            // Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
                        real rad;
                        if( numberOfDimensions==2 )
                        {
                  	real xa = (xv[0]-xe)/ae;
                  	real ya = (xv[1]-ye)/be;
                  	rad = xa*xa+ya*ya;
                        }
                        else
                        {
                  	real xa = (xv[0]-xe)/ae;
                  	real ya = (xv[1]-ye)/be;
                  	real za = (xv[2]-ze)/ce;
                  	rad = xa*xa+ya*ya+za*za;
                        }
            //       // amp = 1 inside the circle and 0 outside
            //       // -- here is a smooth transition from 0 to damp at "radius" rad0
            //       real amp = .5*damp*(tanh( -beta*(rad-rad0) )+1.);
            //       fg(i1,i2,i3,uc) =  -amp*ug(i1,i2,i3,uc);
            //       fg(i1,i2,i3,vc) =  -amp*ug(i1,i2,i3,vc);
            // here we turn on the drag as a step function at rad=rad0
                        if( rad < 1. )
                        {
                  	bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                        }
                    } // end FOR_3D
                }
                else if( regionType=="maskFromGridFunction" )
                {
          // ---- The region is defined by a grid function that holds a mask ----
                    if( !parameters.dbase.has_key("bodyForceMaskGridFunction") )
                    {
                        printF("ERROR: regionType==`maskFromGridFunction' but the grid function does not exist!\n");
                        OV_ABORT("ERROR");
                    }
          // printF("Setting a body force for regionType==maskFromGridFunction for grid=%i\n",grid);
                    realCompositeGridFunction *maskPointer = 
                                                        parameters.dbase.get<realCompositeGridFunction*>("bodyForceMaskGridFunction");
                    assert( maskPointer!=NULL );
                    realCompositeGridFunction & bodyForceMask = *maskPointer;
                    realArray & bfMask = bodyForceMask[grid];
                    OV_GET_SERIAL_ARRAY(real,bfMask,bfMaskLocal);
                    getIndex( mg.dimension(),Ib1,Ib2,Ib3 );          // all points including ghost points.
          // restrict bounds to local processor, include ghost
                    bool ok = ParallelUtility::getLocalArrayBounds(bfMask,bfMaskLocal,Ib1,Ib2,Ib3,1);
                    if( ok )
                    {
                        profileFactor=1.;
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
                  	if( bfMaskLocal(i1,i2,i3)<=0. )  // signed distance 
                  	{
                                bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                  	}
                        }
                    }
                }
                else if( regionType=="mapping" )
                {
          // --- region is defined by a Mapping ---
          //   2D : closed curve
          //   3D : water-tight surface
                    if( !bodyForce.dbase.has_key("bodyForceMapping") )
                    {
                        printF("computeBodyForcing:WARNING: there is no body force Mapping!\n");
                        continue;
                    }
                    MappingRC *& pBodyForceMapping = bodyForce.dbase.get<MappingRC*>("bodyForceMapping");
                    if( pBodyForceMapping==NULL )
                    {
                        printF("computeBodyForcing:WARNING: the body force Mapping is NULL!\n");
                        continue;
                    }
                    Mapping & bodyForceMapping = pBodyForceMapping->getMapping();
                    if( numberOfDimensions==2 )
                    {
                        IntegerArray cross(1);
                        RealArray xa(1,3);
                        xa=0.;
                        assert( bodyForceMapping.approximateGlobalInverse !=NULL );
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
      	// Turn on the drag if we are inside the body
              // ----- OPTIMZE ME -- could save a mask ---
                  	cross=0;
                            xa(0,0)=xv[0]; xa(0,1)=xv[1];
                  	bodyForceMapping.approximateGlobalInverse->countCrossingsWithPolygon( xa,cross );
                            int inside = (cross(0) % 2 == 0) ? 0 : +1;
      	// printF("computeBodyForcing: point (%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),inside);
                  	if( inside )
                  	{
                                bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                  	}
                        } // end FOR_3D
                    }
                    else
                    {
                        IntegerArray inside(1); 
                        RealArray xa(1,3);
                        xa=0.;
                        assert( bodyForceMapping.getClassName()=="UnstructuredMapping" );
                        UnstructuredMapping & uMap = (UnstructuredMapping&)bodyForceMapping;
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
      	// Turn on the drag if we are inside the body
              // ----- OPTIMZE ME -- could do many pts at once, save a mask ---
                            xa(0,0)=xv[0]; xa(0,1)=xv[1]; xa(0,2)=xv[2];
                            #ifndef USE_PPP
                        	  uMap.insideOrOutside(xa,inside);
                            #else
                                OV_ABORT("finish me for parallel");
                            #endif
      	// printF("computeBodyForcing: point (%8.2e,%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),xa(0,2),inside(0));
                  	if( inside(0) )
                  	{
                                bd(i1,i2,i3,tc) +=  profileFactor*bodyTemp;
                  	}
                        } // end FOR_3D
                    }
                }
                else
                {
                    printF("computeBodyForcing:ERROR: unexpected regionType=%s\n",(const char*)regionType);
                    OV_ABORT("ERROR: finish me...");
                }
            
        }
        else if( forcingType=="variableInflow" && uc>=0 )
        {
      // -- Set the velocity on the boundary ---

            const real *velocityForce = bodyForce.dbase.get<real[3]>("velocityForce");

            if( true || t==0. || debug() & 4 )
      	printF("setVariableBoundaryValues: grid=%i (side,axis)=(%i,%i) variableInflow, bf=%i, region=%s, velocityForce=[%g,%g,%g], "
             	       "at time t=%9.3e\n",
             	       grid,side,axis,bf,(const char*)regionType,velocityForce[0],velocityForce[1],velocityForce[2],t);

      // Here are the statements that impose the forcing:
      // Note: the forcing is multipled by profileFactor (defined in the add body force macro)

                const int addBodyForce=0, addBoundaryForce=1;
                  const int forcingType=addBoundaryForce;
                real profileFactor=1.;  // The forcing is multiplied by this factor (changed below for parabolic, ...)
                if( regionType=="box" )
                {
          // -- drag is applied over a box (square in 2D) --
                    const real *boxBounds =  bodyForce.dbase.get<real[6] >("boxBounds");
                    #define xab(side,axis) boxBounds[(side)+2*(axis)]
                    const real & xa = xab(0,0), &xb = xab(1,0);
                    const real & ya = xab(0,1), &yb = xab(1,1);
                    const real & za = xab(0,2), &zb = xab(1,2);
                    const aString & profileType = bodyForce.dbase.get<aString>("profileType");
          // if( debug() & 4 )
          // printF("computeBodyForce: profileType=%s, box bounds = [%e,%e]x[%e,%e][%e,%e]\n",(const char*)profileType,xa,xb,ya,yb,za,zb);
                    if( profileType=="uniform" )
                    {
            // --- uniform profile ---
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
      	// Turn on the drag if we are inside the box
                  	if( xv[0]>=xa && xv[0]<=xb && xv[1]>=ya && xv[1]<=yb && xv[2]>=za && xv[2]<=zb )
                  	{
                                for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                  	}
                        } // end FOR_3D
                    }
                    else if( profileType=="parabolic" )
                    {
            // -- Parabolic profile --
            // Near each edge of the region the parabolic profile looks like: 
            //     u(x) = U(x)*( 1 - (1-d(x)/W)^2 ),  for d(x) < W
            //     u(x) = U(x) ,                      for d(x) > W
            // where d(x) is the distance from the point x to the box that defines the region,
            // and W=parabolicProfileDepth is the width of the parabolic profile. 
                        const real & parabolicProfileDepth = bodyForce.dbase.get<real>("parabolicProfileDepth");
                        const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;
            // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
                        int dir1, dir2;  
                          if( numberOfDimensions==2 )
                          {
               // dir1 = the longest axes of the box (in 2D) 
               // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
                              dir1 = xWidth > yWidth ? 0 : 1;
                          }
                          else
                          {
               // Find the two directions (dir1,dir2) that define the two longest axes of the box
               // 
                              if( xWidth < min(yWidth,zWidth) )
                              {
                                  dir1=1; dir2=2;
                              }
                              else if( yWidth < min(xWidth,zWidth) )
                              {
                                  dir1=0; dir2=2;
                              }
                              else
                              {
                                  dir1=0; dir2=1;
                              }
                          }
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
                            real dist;
                // Boundary: compute the minimum distance to the box edges (ignore box faces in the normal direction to the boundary):
                                dist = min( xv[dir1]-xab(0,dir1), xab(1,dir1)-xv[dir1] );
                                if( numberOfDimensions==3 )
                          	    dist=min( dist, xv[dir2]-xab(0,dir2), xab(1,dir2)-xv[dir2] );
      	// printF("parabolic: (i1,i2)=(%i,%i) x=(%g,%g) dist=%g \n",i1,i2,xv[0],xv[1],dist);
                  	if( dist>=0. )
                  	{  
                                dist /= parabolicProfileDepth;
                                if( dist<1. )
                    	  {
                  // 1 - (1-d)^2 = 2*d-d^2 = d*(2-d)
                                    profileFactor = dist*(2.-dist);
                    	  }
                    	  else
                    	  {
                                    profileFactor=1.;
                    	  }
      	  // printF("         : profileFactor=%g \n",profileFactor);
                                for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                  	}
                        } // end FOR_3D
                    }
                    else if( profileType=="tanh" )
                    {
            // -- Tanh profile:
            // The one-dimensional tanh profile is of the form:
            //     u = U(x) *[  .5*( tanh( b*(x-xa) ) - tanh( b*(x-xb) ) ) ]
            // 
                        const real & b  = bodyForce.dbase.get<real>("tanhProfileExponent");
                        const real xWidth=xb-xa, yWidth=yb-ya, zWidth=zb-za;
            // For now we assume the boundary is parallel to the longest axis (2D) axes (3D) of the box.
                        int dir1, dir2;  
                          if( numberOfDimensions==2 )
                          {
               // dir1 = the longest axes of the box (in 2D) 
               // xb-xa > yb-ya : assume the boundary is horizontal, else vertical
                              dir1 = xWidth > yWidth ? 0 : 1;
                          }
                          else
                          {
               // Find the two directions (dir1,dir2) that define the two longest axes of the box
               // 
                              if( xWidth < min(yWidth,zWidth) )
                              {
                                  dir1=1; dir2=2;
                              }
                              else if( yWidth < min(xWidth,zWidth) )
                              {
                                  dir1=0; dir2=2;
                              }
                              else
                              {
                                  dir1=0; dir2=1;
                              }
                          }
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
              // --> we could have a cutoff if we are far away from the transition zone, to avoid
              //  evaluating the tanh's.
                // Boundary force:
                                profileFactor = .5*( tanh( b*(xv[dir1]-xab(0,dir1)) ) - tanh( b*(xv[dir1]-xab(1,dir1)) ) );
                    	  if( numberOfDimensions==3 )
                                    profileFactor *= .5*( tanh( b*(xv[dir2]-xab(0,dir2)) ) - tanh( b*(xv[dir2]-xab(1,dir2)) ) );
                            for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                        } // end FOR_3D
                    }
                    else
                    {
                        printF("addBodyForceMacro: ERROR: unknown profileType=%s\n",(const char*)profileType);
                        OV_ABORT("ERROR");
                    }
                }
                else if( regionType=="ellipse" )
                {
          // -- drag is applied over an ellipse --
          //   [(x-xe)/ae]^2 + [(y-ye)/be]^2 + [(z-ze)/ce]^2 = 1
                    const real *ellipse =  bodyForce.dbase.get<real[6] >("ellipse");
                    const real ae = ellipse[0];
                    const real be = ellipse[1];
                    const real ce = ellipse[2];
                    const real xe = ellipse[3];
                    const real ye = ellipse[4];
                    const real ze = ellipse[5];
                    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                    {
            // Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
                        real rad;
                        if( numberOfDimensions==2 )
                        {
                  	real xa = (xv[0]-xe)/ae;
                  	real ya = (xv[1]-ye)/be;
                  	rad = xa*xa+ya*ya;
                        }
                        else
                        {
                  	real xa = (xv[0]-xe)/ae;
                  	real ya = (xv[1]-ye)/be;
                  	real za = (xv[2]-ze)/ce;
                  	rad = xa*xa+ya*ya+za*za;
                        }
            //       // amp = 1 inside the circle and 0 outside
            //       // -- here is a smooth transition from 0 to damp at "radius" rad0
            //       real amp = .5*damp*(tanh( -beta*(rad-rad0) )+1.);
            //       fg(i1,i2,i3,uc) =  -amp*ug(i1,i2,i3,uc);
            //       fg(i1,i2,i3,vc) =  -amp*ug(i1,i2,i3,vc);
            // here we turn on the drag as a step function at rad=rad0
                        if( rad < 1. )
                        {
                  	for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                        }
                    } // end FOR_3D
                }
                else if( regionType=="maskFromGridFunction" )
                {
          // ---- The region is defined by a grid function that holds a mask ----
                    if( !parameters.dbase.has_key("bodyForceMaskGridFunction") )
                    {
                        printF("ERROR: regionType==`maskFromGridFunction' but the grid function does not exist!\n");
                        OV_ABORT("ERROR");
                    }
          // printF("Setting a body force for regionType==maskFromGridFunction for grid=%i\n",grid);
                    realCompositeGridFunction *maskPointer = 
                                                        parameters.dbase.get<realCompositeGridFunction*>("bodyForceMaskGridFunction");
                    assert( maskPointer!=NULL );
                    realCompositeGridFunction & bodyForceMask = *maskPointer;
                    realArray & bfMask = bodyForceMask[grid];
                    OV_GET_SERIAL_ARRAY(real,bfMask,bfMaskLocal);
                    getIndex( mg.dimension(),Ib1,Ib2,Ib3 );          // all points including ghost points.
          // restrict bounds to local processor, include ghost
                    bool ok = ParallelUtility::getLocalArrayBounds(bfMask,bfMaskLocal,Ib1,Ib2,Ib3,1);
                    if( ok )
                    {
                        profileFactor=1.;
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
                  	if( bfMaskLocal(i1,i2,i3)<=0. )  // signed distance 
                  	{
                                for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                  	}
                        }
                    }
                }
                else if( regionType=="mapping" )
                {
          // --- region is defined by a Mapping ---
          //   2D : closed curve
          //   3D : water-tight surface
                    if( !bodyForce.dbase.has_key("bodyForceMapping") )
                    {
                        printF("computeBodyForcing:WARNING: there is no body force Mapping!\n");
                        continue;
                    }
                    MappingRC *& pBodyForceMapping = bodyForce.dbase.get<MappingRC*>("bodyForceMapping");
                    if( pBodyForceMapping==NULL )
                    {
                        printF("computeBodyForcing:WARNING: the body force Mapping is NULL!\n");
                        continue;
                    }
                    Mapping & bodyForceMapping = pBodyForceMapping->getMapping();
                    if( numberOfDimensions==2 )
                    {
                        IntegerArray cross(1);
                        RealArray xa(1,3);
                        xa=0.;
                        assert( bodyForceMapping.approximateGlobalInverse !=NULL );
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
      	// Turn on the drag if we are inside the body
              // ----- OPTIMZE ME -- could save a mask ---
                  	cross=0;
                            xa(0,0)=xv[0]; xa(0,1)=xv[1];
                  	bodyForceMapping.approximateGlobalInverse->countCrossingsWithPolygon( xa,cross );
                            int inside = (cross(0) % 2 == 0) ? 0 : +1;
      	// printF("computeBodyForcing: point (%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),inside);
                  	if( inside )
                  	{
                                for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                  	}
                        } // end FOR_3D
                    }
                    else
                    {
                        IntegerArray inside(1); 
                        RealArray xa(1,3);
                        xa=0.;
                        assert( bodyForceMapping.getClassName()=="UnstructuredMapping" );
                        UnstructuredMapping & uMap = (UnstructuredMapping&)bodyForceMapping;
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
                        {
      	// Get the grid coordinates xv[axis]:
                            if( isRectangular )
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=XC(iv,axis);
                            }
                            else
                            {
                                for( int axis=0; axis<numberOfDimensions; axis++ )
                                    xv[axis]=vertexLocal(i1,i2,i3,axis);
                            }
      	// Turn on the drag if we are inside the body
              // ----- OPTIMZE ME -- could do many pts at once, save a mask ---
                            xa(0,0)=xv[0]; xa(0,1)=xv[1]; xa(0,2)=xv[2];
                            #ifndef USE_PPP
                        	  uMap.insideOrOutside(xa,inside);
                            #else
                                OV_ABORT("finish me for parallel");
                            #endif
      	// printF("computeBodyForcing: point (%8.2e,%8.2e,%8.2e) : inside=%i.\n",xa(0,0),xa(0,1),xa(0,2),inside(0));
                  	if( inside(0) )
                  	{
                                for( int dir=0; dir<numberOfDimensions; dir++ ){ bd(i1,i2,i3,uc+dir) += profileFactor*velocityForce[dir]; }
                  	}
                        } // end FOR_3D
                    }
                }
                else
                {
                    printF("computeBodyForcing:ERROR: unexpected regionType=%s\n",(const char*)regionType);
                    OV_ABORT("ERROR: finish me...");
                }
            
        }
//     else if( parameters.userBcType(side,axis,grid)==boundaryDataFromAFile )
//     {
//       ExternalBoundaryData & ebd = *parameters.dbase.get<ExternalBoundaryData*>("externalBoundaryData");
//       RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
//       ebd.getBoundaryData( t, cg, side, axis, grid, bd );
      	
//     }
        else
        {
            printF("setVariableBoundaryValues:ERROR: unknown forcingType=[%s]\n",(const char*)forcingType);
            OV_ABORT("error");
        }


    } // end for bf
        
        
    return numberOfSidesAssigned;
}
