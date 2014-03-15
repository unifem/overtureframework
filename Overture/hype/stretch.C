#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "DataPointMapping.h"
#include "display.h"
#include "GL_GraphicsInterface.h"
#include "MappingInformation.h"

#include "CompositeSurface.h"

#include "MappingProjectionParameters.h"
#include "UnstructuredMapping.h"

#include "StretchTransform.h"
#include "StretchedSquare.h"
#include "StretchMapping.h"
#include "SplineMapping.h"
#include "ComposeMapping.h"

#include "arrayGetIndex.h"


static int gridIsStretched=false;


int HyperbolicMapping::
buildMarchingSpacingDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//     Options for defining the spacing/stretching when we march
// ==========================================================================================
{
  dialog.setOptionMenuColumns(1);

  aString opLabel[] = {"uniform","geometric","inverse hyperbolic","stretch Mapping",
		       "user defined",""}; //
  aString opCmd[] =   {"spacing: constant",
		       "spacing: geometric",
		       "spacing: inverse hyperbolic",
		       "spacing: stretch Mapping",
		       "spacing: user defined",
		       ""}; //
  // addPrefix(label,prefix,cmd,maxCommands);
  dialog.addOptionMenu("spacing:", opCmd,opLabel,spacingType);

  const int numLabels=6;
  aString opLabel2[numLabels] = {"initial spacing from distance and lines",
				 "distance from lines and initial spacing",
				 "lines from distance and initial spacing",
				 ""}; //
//  aString opCmd2[numLabels];
//  GUIState::addPrefix(opLabel,"OBSTR:",opCmd,numLabels);
  dialog.addOptionMenu("determine:", opLabel2,opLabel2,(int)spacingOption);


  aString pbLabels[] = {"help marching spacing",
                        // "reset",
			""};
  int numRows=1;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 


//    aString tbCommands[] = {"project smoothed grid onto reference surface",
//                            "smooth ghost points",
//  			  ""};
//    int tbState[10];
//    tbState[0] = projectSmoothedGridOntoReferenceSurface==true; 
//    tbState[1] = smoothGridGhostPoints==true;
//    int numColumns=1;
//    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  
  int nt=0;
//    textLabels[nt] = "initial spacing";  // this is now with target spacing
//    sPrintF(textStrings[nt], "%g (<0 means choose default)",initialSpacing); nt++; 

  textLabels[nt] = "geometric stretch factor"; 
  sPrintF(textStrings[nt], "%g ",geometricFactor); nt++; 

  real stretchFactor=10.;
  textLabels[nt] = "inv hyp stretch factor"; 
  sPrintF(textStrings[nt], "%g ",stretchFactor); nt++; 


  textLabels[nt] = "initial lines with constant spacing";
  sPrintF(textStrings[nt], "%i",numberOfLinesWithConstantSpacing); nt++; 


  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}


bool HyperbolicMapping::
updateMarchingSpacingOptions(aString & answer, DialogData & dialog, MappingInformation & mapInfo )
// ==========================================================================================
// /Description:
//     Assign values in the dialog
//
// /answer (input) : check this answer to see if it is a marching parameter.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;
  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;


  if( answer=="help marching spacing" )
  {
    gi.outputString("-----------------------------------------------------------------------------------------\n"
                    "         Grid Spacing while Marching  \n"
                    "-----------------------------------------------------------------------------------------\n" );
  }
  else if( answer.matches("spacing:") )
  {
    if( len=answer.matches("spacing: constant")>0 )
    {
      spacingType=constantSpacing;
    }
    else if( len=answer.matches("spacing: geometric")>0 )
    {
      spacingType=geometricSpacing;
    }
    else if( len=answer.matches("spacing: inverse hyperbolic")>0 )
    {
      spacingType=inverseHyperbolicSpacing;
    }
    else if( len=answer.matches("spacing: stretch Mapping")>0 )
    {
      spacingType=oneDimensionalMappingSpacing;

      StretchMapping & stretch = *new StretchMapping;
      if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
	delete normalDistribution;
      normalDistribution=&stretch;
      normalDistribution->incrementReferenceCount();
      stretch.update(mapInfo);

    }
    else if( len=answer.matches("spacing: user defined")>0 )
    {
      spacingType=userDefinedSpacing;
      printf("User defined spacing not implemented yet\n");
    }
    dialog.getOptionMenu(0).setCurrentChoice(spacingType);
  }
  else if( answer.matches("initial spacing from distance and lines") ||
           answer.matches("distance from lines and initial spacing") ||
	   answer.matches("lines from distance and initial spacing") )
  {
    spacingOption=(answer.matches("initial spacing from distance and lines") ? spacingFromDistanceAndLines :
                   answer.matches("distance from lines and initial spacing") ? distanceFromLinesAndSpacing :
		   linesFromDistanceAndSpacing );
    dialog.getOptionMenu(1).setCurrentChoice((int)spacingOption);
  }
  else if( len=answer.matches("initial spacing") )  // keep here for backward compatibility
  {
    sScanF(answer(len,answer.length()-1),"%e",&initialSpacing);
//    dialog.setTextLabel("initial spacing",sPrintF(line, "%g (<0 means choose default)",initialSpacing));
    if( spacingOption==spacingFromDistanceAndLines )
    {
      spacingOption=distanceFromLinesAndSpacing;
      dialog.getOptionMenu(1).setCurrentChoice((int)spacingOption);
      gi.outputString("INFO: setting spacingOption equal to `distance from lines and initial spacing'\n"
                      "      You could also set the spacingOption to `lines from distance and initial spacing'");
    }
  }
  else if( len=answer.matches("geometric stretch factor") )
  {
    sScanF( answer(len,answer.length()-1),"%e",&geometricFactor);
    if( geometricFactor==1. )
    {
      printf("WARNING: The geometric stretch factor cannot be 1. setting to 1.00001\n");
      geometricFactor=1.00001;
    }
    dialog.setTextLabel("geometric stretch factor",sPrintF(line, "%g ",geometricFactor));
    spacingType=geometricSpacing;
    dialog.getOptionMenu(0).setCurrentChoice(spacingType);

  }
  else if( len=answer.matches("inv hyp stretch factor") )
  {
    real exponent=10.;
    sScanF( answer(len,answer.length()-1),"%e",&exponent);
    dialog.setTextLabel("inv hyp stretch factor",sPrintF(line, "%g ",exponent));

    spacingType=oneDimensionalMappingSpacing;  // ***** set option dialog.
    dialog.getOptionMenu(0).setCurrentChoice(spacingType);

    StretchMapping & stretch = *new StretchMapping;
    if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
      delete normalDistribution;
    normalDistribution=&stretch;
    normalDistribution->incrementReferenceCount();

    stretch.setStretchingType( StretchMapping::inverseHyperbolicTangent );
    stretch.setNumberOfLayers(1);
    stretch.setLayerParameters(0,1.,exponent,0.);

  }
  else
  {
    returnValue=false;
  }

  return returnValue;
}




int HyperbolicMapping::
buildStartCurveSpacingDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//    Options for stretching grid lines on the start curve
// ==========================================================================================
{
//    const int numLabels=6;
//    aString opLabel[numLabels] = {"edges",
//  				"coordinate line 0",
//  				"coordinate line 1",
//  				"points on surface",
//                                  "boundary curve",
//  				""}; //
//    aString opCmd[numLabels];
//    GUIState::addPrefix(opLabel,"initial curve:",opCmd,numLabels);

//    int choice = 0;
//    if( surface!=NULL && surfaceGrid && surface->getClassName()=="UnstructuredMapping" )
//    {
//      initialCurveOption=initialCurveFromBoundaryCurves;
//    }
//    surfaceGridParametersDialog.addOptionMenu("initial curve from:", opCmd,opLabel,(int)initialCurveOption);


  aString pbLabels[] = {"stretch start curve",
                        "reset to unstretched start curve",
                        "help start curve stretching",
			""};
  int numRows=3;
  dialog.setPushButtons( pbLabels, pbLabels, numRows ); 


  aString tbCommands[] = {"use stretching while marching",
 			  ""};
  int tbState[10];
  tbState[0] = useStartCurveStretchingWhileMarching;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  if( pStartCurveStretchParams==NULL )
    pStartCurveStretchParams = new RealArray;
  RealArray & startCurveStretchParams = *pStartCurveStretchParams;
  if( startCurveStretchParams.getLength(0)==0 )
  {
    startCurveStretchParams.redim(4,10);
    startCurveStretchParams=0.;
    startCurveStretchParams(0,0)=0.;  // axis
    startCurveStretchParams(1,0)=0.;  // a
    startCurveStretchParams(2,0)=5.;  // b
    startCurveStretchParams(3,0)=.5;  // c
    
    startCurveStretchParams(0,1)=1.;
    startCurveStretchParams(1,1)=0.;
    startCurveStretchParams(2,1)=10.;
    startCurveStretchParams(3,1)=.0;

  }
  int stretchID=0;
  int nt=0;
  textLabels[nt] = "SC:stretch r1";
  sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
         startCurveStretchParams(1,stretchID),startCurveStretchParams(2,stretchID),startCurveStretchParams(3,stretchID) ); nt++; 

  if( domainDimension==3 )
  {
    stretchID=1;
    textLabels[nt] = "SC:stretch r2";
    sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
	    startCurveStretchParams(1,stretchID),startCurveStretchParams(2,stretchID),startCurveStretchParams(3,stretchID) ); nt++; 
  }
  
  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}

bool HyperbolicMapping::
updateStartCurveSpacingOptions(aString & answer, DialogData & dialog, MappingInformation & mapInfo )
// ==========================================================================================
// /Description:
//     Assign values in the dialog
//
// /answer (input) : check this answer to see if it is a marching parameter.
//
// /Return value: true if the answer was processed, false otherwise.
// ==========================================================================================
{
  bool returnValue=true;

  if( pStartCurveStretchParams==NULL )
    pStartCurveStretchParams = new RealArray;
  RealArray & startCurveStretchParams = *pStartCurveStretchParams;

  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  if( answer=="help start curve stretching" )
  {
    gi.outputString("-----------------------------------------------------------------------------------------\n"
                    "               Start Curve Grid Stretching \n"
                    "The start curve (or start surface) can be stretched along the coordinate direction r1 (or r1,r2).\n"
                    " `stretch r1 id a b c ' defines a stretching of grid points along coordinate direction r1\n"
                    "   where id = the unique identifier for the stretching, id=0,1,2,3... \n"
                    "         a = weight of the stretching, a>0 (a good value is 1., a=0 means no stretching) \n"
                    "         b = the exponent of the stretching, b>0 (b=5 give some stretching b=10 gives more)\n"
                    "         c = the position of the stretching on the unit interval, 0 <= c <=1\n"
                    " You may define multiple stretchings along each of the coordinate directions by choosing\n"
                    " a different id. \n"
                    " NOTE: that each stretching (over all directions) must have a unique id. \n"
                    " Example: To stretch at both ends of direction r1 and in the middle of direction r2\n"
                    "    stretch r1 0 1. 10. 0.    (id=0 : stretch at r1=0.)\n"
                    "    stretch r1 1 1. 10. 1.    (id=1 : stretch at r1=1.)\n"
                    "    stretch r2 2 1. 5. .5     (id=2 : stretch at r2=.5\n"
                    "-----------------------------------------------------------------------------------------\n" );
  }
  else if( len=answer.matches("use stretching while marching") )
  {
    int value;
    sScanF(answer(len,answer.length()-1),"%i",&value); useStartCurveStretchingWhileMarching=value;
    dialog.setToggleState("use stretching while marching",useStartCurveStretchingWhileMarching);
    if( !useStartCurveStretchingWhileMarching )
    {
      printf("INFO: I am setting the equidistribution weight to 0. since you do not want to stretch while marching\n"
             "      You can change the equidistribution weight (small values work best with a stretched start curve)\n");
      equidistributionWeight=0.;
    }
  }
  else if( len=answer.matches("SC:stretch r") )
  {
    int axis = (len=answer.matches("SC:stretch r1")) ? 0 :
               (len=answer.matches("SC:stretch r2")) ? 1 : 2;

    gi.outputString(sPrintF(line,"INFO: Stretch the grid along axis%i",axis+1));
    int stretchID=-1;
    sScanF(answer(len,answer.length()-1),"%i",&stretchID);

    if( stretchID<0 || stretchID>100 )
    {
      printf("ERROR: invalid value for stretchID=%i\n",stretchID);
      return returnValue;
    }
    
    if( stretchID>startCurveStretchParams.getBound(1) )
    {
      int num=startCurveStretchParams.getLength(1);
      startCurveStretchParams.resize(startCurveStretchParams.getLength(0),stretchID+10);
      Range all;
      startCurveStretchParams(all,Range(num,startCurveStretchParams.getBound(1)))=0;
    }
    startCurveStretchParams(0,stretchID)=axis;
    
    sScanF(answer(len,answer.length()-1),"%i %e %e %e",&stretchID,
	   &startCurveStretchParams(1,stretchID),&startCurveStretchParams(2,stretchID),&startCurveStretchParams(3,stretchID) );

    if( axis==0 )
    {
      dialog.setTextLabel("SC:stretch r1",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  startCurveStretchParams(1,stretchID),startCurveStretchParams(2,stretchID),startCurveStretchParams(3,stretchID) ));
    }
    else if( axis==1 )
    {
      dialog.setTextLabel("SC:stretch r2",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  startCurveStretchParams(1,stretchID),startCurveStretchParams(2,stretchID),startCurveStretchParams(3,stretchID) ));
    }
    else
    {
      dialog.setTextLabel("SC:stretch r3",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  startCurveStretchParams(1,stretchID),startCurveStretchParams(2,stretchID),startCurveStretchParams(3,stretchID) ));
    }
    
  }
  else if( answer.matches("reset to unstretched start curve") )
  {
    // undo all stretching
    printf("*** reset all stretching \n");

    gridIsStretched=false;

    startCurveStretchParams=0.;
    startCurveStretchParams=0.;
    startCurveStretchParams(0,0)=0.;  // axis
    startCurveStretchParams(1,0)=0.;  // a
    startCurveStretchParams(2,0)=10.; // b
    startCurveStretchParams(3,0)=.5;  // c
    
    startCurveStretchParams(0,1)=1.;
    startCurveStretchParams(1,1)=0.;
    startCurveStretchParams(2,1)=10.;
    startCurveStretchParams(3,1)=.5;

    if( startCurveStretchMapping!=NULL && startCurveStretchMapping->decrementReferenceCount()==0 )
      delete startCurveStretchMapping;
    startCurveStretchMapping=NULL;
    
    bool updateNumberOfGridLines=false;
    updateForInitialCurve(updateNumberOfGridLines);

    plotHyperbolicSurface=false;
    plotDirectionArrowsOnInitialCurve=true;
    // referenceSurfaceHasChanged=true;
    plotObject=true; 

  }
  else if( len=answer.matches("stretch start curve") )
  {
    // *************************************
    // ***** Stretch the start curve *******
    // *************************************

    printf("stretch start curve:INFO: The stretching of the grid is weighted by the equidistribution weight.\n");
    if( equidistributionWeight<=0. )
    {
      printf("stretch start curve:INFO: the equidistribution weight is currently zero.  I am setting this value to .5 \n");
      equidistributionWeight=.5;
    }
    if( numberOfVolumeSmoothingIterations>5 && useStartCurveStretchingWhileMarching )
    {
      // 
      numberOfVolumeSmoothingIterations=2;
      printf("stretch start curve:INFO:I am reducing the volume smoothing to %i iterations since this usually works better\n",
	     numberOfVolumeSmoothingIterations);
    }
    
    if( startCurveStretchMapping==NULL )
    {
      startCurveStretchMapping= new StretchedSquare(1); // note we only stretch in 1D
      startCurveStretchMapping->incrementReferenceCount();
    }
    
    StretchedSquare & stretchedSquare = (StretchedSquare &)(*startCurveStretchMapping);

    if( startCurve!=NULL )
      stretchedSquare.setIsPeriodic(axis1,startCurve->getIsPeriodic(axis1) );
    
    // first count the number of layers in each direction
    int numberOfLayers[3]={0,0,0};
    int i,axis;
    for( i=0; i<=startCurveStretchParams.getBound(1); i++ )
    {
      axis=int(startCurveStretchParams(0,i)+.5);
      if( startCurveStretchParams(1,i)>0. )
      {
        assert( axis>=0 && axis<3 );
	numberOfLayers[axis]++;
      }
    }

    for( axis=0; axis<domainDimension; axis++ )
    {
      printf(" Stretch: axis=%i setNumberOfLayers=%i\n",axis,numberOfLayers[axis]);
      
      if( numberOfLayers[axis]>0 )
      {
        StretchMapping & stretch = stretchedSquare.stretchFunction(axis);
	stretch.setNumberOfLayers( numberOfLayers[axis] );

	stretch.setStretchingType(StretchMapping::inverseHyperbolicTangent);

	int index=0;
	for( i=0; i<=startCurveStretchParams.getBound(1); i++ )
	{
	  if( int(startCurveStretchParams(0,i)+.5)==axis && startCurveStretchParams(1,i)>0. )
	  {
	    printf(" Stretch: axis=%i index=%i i=%i (a,b,c)=(%8.2e,%8.2e,%8.2e)\n",axis,index,i,
                   startCurveStretchParams(1,i),startCurveStretchParams(2,i),startCurveStretchParams(3,i));

	    stretch.setLayerParameters(index, startCurveStretchParams(1,i),startCurveStretchParams(2,i),startCurveStretchParams(3,i));
	    index++;
	  }
	}
      }
    }
    
    if( surfaceGrid )
    {
      bool updateNumberOfGridLines=false;
      updateForInitialCurve(updateNumberOfGridLines);
      plotHyperbolicSurface=false;
      plotDirectionArrowsOnInitialCurve=true;
    }
    else
    {
      referenceSurfaceHasChanged=true;
    }
    plotObject=true; 

  }
  else
  {
    returnValue=false;
  }

  return returnValue;
}
