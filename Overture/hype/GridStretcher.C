#include "GridStretcher.h"

#include "MappingInformation.h"
#include "DataPointMapping.h"
#include "display.h"
#include "GL_GraphicsInterface.h"

#include "MappingProjectionParameters.h"

#include "StretchTransform.h"
#include "StretchedSquare.h"
#include "StretchMapping.h"
#include "ComposeMapping.h"

#include "arrayGetIndex.h"

GridStretcher::
GridStretcher(int domainDimension_, int rangeDimension_)
{
  domainDimension=domainDimension_;
  rangeDimension=rangeDimension_;
  
  stretchID=0;
  numberOfStretch=0;
  projectStretchedGridOntoReferenceSurface=1;
  gridIsStretched=false;

  pickingOption=pickToStretchInDirection1;
  defaultWeight=.5;
  defaultExponent=10.;
}

GridStretcher::
~GridStretcher()
{
}

int GridStretcher::
buildDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//    Options for post stretching of grid lines (i.e. stretching after we have marched)
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


  const int maximumCommands=30;
  aString cmdWithPrefix[maximumCommands];
  const aString prefix="GST:";

  aString pbLabels[] = {"stretch grid",
                        "reset to unstretched grid",
                        "pick to stretch",
                        "show stretching parameters",
                        "help stretching",
			""};
  GUIState::addPrefix(pbLabels,prefix,cmdWithPrefix,maximumCommands);

  int numRows=3;
  dialog.setPushButtons( cmdWithPrefix, pbLabels, numRows ); 


  aString tbCommands[] = {"project stretched grid onto reference surface",
 			  ""};
  int tbState[10];
  tbState[0] = projectStretchedGridOntoReferenceSurface;
  int numColumns=1;
  GUIState::addPrefix(tbCommands,prefix,cmdWithPrefix,maximumCommands);
  dialog.setToggleButtons(cmdWithPrefix, tbCommands, tbState, numColumns); 

  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  if( stretchParams.getLength(0)==0 )
  {
    stretchParams.redim(4,10);
    stretchParams=0.;
    stretchParams(0,0)=0.;  // axis
    stretchParams(1,0)=0.;  // a
    stretchParams(2,0)=5.;  // b
    stretchParams(3,0)=.5;  // c
    
    stretchParams(0,1)=1.;
    stretchParams(1,1)=0.;
    stretchParams(2,1)=10.;
    stretchParams(3,1)=.0;

    stretchParams(0,2)=1.;
    stretchParams(1,2)=0.;
    stretchParams(2,2)=10.;
    stretchParams(3,2)=.5;
    
  }
  int stretchID=0;
  int nt=0;
  textLabels[nt] = "stretch r1";
  sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
         stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ); nt++; 

  stretchID=1;
  textLabels[nt] = "stretch r2";
  sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
         stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ); nt++; 

  if( domainDimension==3 )
  {
    stretchID=2;
    textLabels[nt] = "stretch r3";
    sPrintF(textStrings[nt], "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
	    stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ); nt++; 
  }
  

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  // addPrefix(textLabels,prefix,cmd,maxCommands);
  GUIState::addPrefix(textLabels,prefix,cmdWithPrefix,maximumCommands);
  dialog.setTextBoxes(cmdWithPrefix, textLabels, textStrings);

  return 0;
}

int GridStretcher::
update(aString & answer, DialogData & dialog, MappingInformation & mapInfo,
       realArray & x,  // source grid points including ghost points
       const IntegerArray & gridIndexRange,     // identifies boundaries
       const IntegerArray & projectIndexRange,  // identifies which points to project for surface grids
       DataPointMapping & dpm,   // the resulting stretched grid
       Mapping *surface /* =NULL */ // for projecting onto surface grids
  )
// ==========================================================================================================
// /Description: Stretch grid points defined in the array "x" and return in the DataPointMapping dpm
// /answer (input) : check this answer to see if it is a stretching parameter.
// /Return value: A non-zero value if an answer was processed. The return value is one of the values
//   in the ReturnValueEnum:
//                0=answerNotProcessed, 1=parametersChanged, 2=gridWasChanged
// ==========================================================================================================
{
  return update( answer, dialog, mapInfo,
                 &x,&gridIndexRange,&projectIndexRange,&dpm,surface,NULL );
}


int GridStretcher::
update(aString & answer, DialogData & dialog, MappingInformation & mapInfo, StretchTransform & stretchedMapping )
// ==========================================================================================================
// /Description: Stretch a mapping through StretchTransform "stretchedMapping"
// /answer (input) : check this answer to see if it is a stretching parameter.
// /Return value: A non-zero value if an answer was processed. The return value is one of the values
//   in the ReturnValueEnum:
//                0=answerNotProcessed, 1=parametersChanged, 2=gridWasChanged
// ==========================================================================================================
{
  return update( answer, dialog, mapInfo,
                 NULL,NULL,NULL,NULL,NULL,
                 &stretchedMapping );
}


bool GridStretcher::
checkForStretchCommands(const aString & answer, GenericGraphicsInterface & gi, DialogData & dialog )
// ============================================================================================
//   Process any stretching commands. Return true if a command was processed.
// ===========================================================================================
{
  bool returnValue=true;
  aString line;
  int len;
  if( len=answer.matches("stretch r") )
  {
    int axis = (len=answer.matches("stretch r1")) ? 0 :
               (len=answer.matches("stretch r2")) ? 1 : (len=answer.matches("stretch r3")) ? 2 : -1;

    assert( axis>=0 );
    
    gi.outputString(sPrintF(line,"INFO: Stretch the grid along axis%i",axis+1));
    int stretchID=-1;
    sScanF(answer(len,answer.length()-1),"%i",&stretchID);

    if( stretchID<0 || stretchID>100 )
    {
      printf("GridStretcher::ERROR: invalid value for stretchID=%i\n",stretchID);
      return returnValue;
    }
    
    if( stretchID>stretchParams.getBound(1) )
    {
      int num=stretchParams.getLength(1);
      stretchParams.resize(stretchParams.getLength(0),stretchID+10);
      Range all;
      stretchParams(all,Range(num,stretchParams.getBound(1)))=0;
    }
    stretchParams(0,stretchID)=axis;
    
    sScanF(answer(len,answer.length()-1),"%i %e %e %e",&stretchID,
	   &stretchParams(1,stretchID),&stretchParams(2,stretchID),&stretchParams(3,stretchID) );

    if( axis==0 )
    {
      dialog.setTextLabel("stretch r1",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ));
    }
    else if( axis==1 )
    {
      dialog.setTextLabel("stretch r2",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ));
    }
    else
    {
      dialog.setTextLabel("stretch r3",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
				  stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ));
    }
    
  }
  else
  {
    returnValue=false;
  }
  return returnValue;
  
}



int GridStretcher::
update(aString & answer_, DialogData & dialog, MappingInformation & mapInfo,
       realArray *px,  // source grid points including ghost points
       IntegerArray const *pgridIndexRange,     // identifies boundaries
       IntegerArray const *pprojectIndexRange,  // identifies which points to project for surface grids
       DataPointMapping *dpm,   // the resulting stretched grid
       Mapping *surface /* =NULL */ , // for projecting onto surface grids
       StretchTransform *stretchedMapping /* =NULL */
  ) 
// ==========================================================================================
// /Access: protected
// /Description:
//      MASTER routine for stretching -- supports both the above functions.
//
// /answer (input) : check this answer to see if it is a stretching parameter.
//
// /Return value: A non-zero value if an answer was processed. The return value is one of the values
//   in the ReturnValueEnum:
//                0=answerNotProcessed, 1=parametersChanged, 2=gridWasChanged
//        
// ==========================================================================================
{
  realArray & x = px!= NULL ? *px : Overture::nullRealDistributedArray();
  const IntegerArray & gridIndexRange = pgridIndexRange!= NULL ? *pgridIndexRange : Overture::nullIntArray();
  const IntegerArray & projectIndexRange = pprojectIndexRange!= NULL ? *pprojectIndexRange : Overture::nullIntArray();

  int returnValue=parametersChanged;   // default
  aString line;
  int len;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  const aString prefix="GST:";

  aString answer=answer_;
  // take off the prefix
  if( answer(0,prefix.length()-1)==prefix )
    answer=answer(prefix.length(),answer.length()-1);
  else
  {
    return answerNotProcessed;
  }
  

  if( answer=="help stretching" )
  {
    gi.outputString("-----------------------------------------------------------------------------------------\n"
                    "                   Grid Stretching \n"
                    "The grid can be stretched along each of the coordinate directions, r1,r2 or r3.\n"
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
  else if( answer=="show stretching parameters" )
  {
    int i,axis;
    for( i=0; i<=stretchParams.getBound(1); i++ )
    {
      axis=int(stretchParams(0,i)+.5);
      if( stretchParams(1,i)>0. )
      {
        assert( axis>=0 && axis<3 );
        printf(" id=%i axis=%i inverse-tanh: weight=%8.2e exponent=%8.2e position=%8.2e\n",i,axis,
               stretchParams(1,i),stretchParams(2,i),stretchParams(3,i));
	
      }
    }
  }
  else if( checkForStretchCommands(answer,gi,dialog ) )
  {
  }
//    else if( len=answer.matches("stretch r") )
//    {
//      int axis = (len=answer.matches("stretch r1")) ? 0 :
//                 (len=answer.matches("stretch r2")) ? 1 : (len=answer.matches("stretch r3")) ? 2 : -1;

//      assert( axis>=0 );
    
//      gi.outputString(sPrintF(line,"INFO: Stretch the grid along axis%i",axis+1));
//      int stretchID=-1;
//      sScanF(answer(len,answer.length()-1),"%i",&stretchID);

//      if( stretchID<0 || stretchID>100 )
//      {
//        printf("GridStretcher::ERROR: invalid value for stretchID=%i\n",stretchID);
//        return returnValue;
//      }
    
//      if( stretchID>stretchParams.getBound(1) )
//      {
//        int num=stretchParams.getLength(1);
//        stretchParams.resize(stretchParams.getLength(0),stretchID+10);
//        Range all;
//        stretchParams(all,Range(num,stretchParams.getBound(1)))=0;
//      }
//      stretchParams(0,stretchID)=axis;
    
//      sScanF(answer(len,answer.length()-1),"%i %e %e %e",&stretchID,
//  	   &stretchParams(1,stretchID),&stretchParams(2,stretchID),&stretchParams(3,stretchID) );

//      if( axis==0 )
//      {
//        dialog.setTextLabel("stretch r1",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
//  				  stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ));
//      }
//      else if( axis==1 )
//      {
//        dialog.setTextLabel("stretch r2",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
//  				  stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ));
//      }
//      else
//      {
//        dialog.setTextLabel("stretch r3",sPrintF(line, "%i %.2g %.2g %.2g (id,weight,exponent,position)",stretchID,
//  				  stretchParams(1,stretchID),stretchParams(2,stretchID),stretchParams(3,stretchID) ));
//      }
    
//    }
  else if( len=answer.matches("project stretched grid onto reference surface") )
  {
    sScanF(answer(len,answer.length()-1),"%i",&projectStretchedGridOntoReferenceSurface);
    dialog.setToggleState("project stretched grid onto reference surface",
                   projectStretchedGridOntoReferenceSurface);
  }
  else if( answer=="pick to stretch" )
  {
    GUIState gui;
    gui.setWindowTitle("Grid Stretching: pick to stretch");
    gui.setExitCommand("exit", "continue");
    DialogData & pickDialog = gui;

    const int maximumCommands=30;
    aString cmdWithPrefix[maximumCommands];
    const aString prefix="GST:";

    pickDialog.addInfoLabel("Pick points to stretch at.");

    aString opLabel1[] = {"pick to do nothing",
			  "pick to stretch in direction 1",
			  "pick to stretch in direction 2",
			  "pick to stretch in direction 3",
			  ""};  //

    GUIState::addPrefix(opLabel1,prefix,cmdWithPrefix,maximumCommands);
    int numberOfColumns=1;
    pickDialog.addRadioBox("Picking:", cmdWithPrefix,opLabel1,(int)pickingOption,numberOfColumns);

    const int numberOfTextStrings=20;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];
    int nt=0;
  
    textLabels[nt] = "default weight";
    sPrintF(textStrings[nt], "%g (used with picking)",defaultWeight); nt++;
  
    textLabels[nt] = "default exponent";
    sPrintF(textStrings[nt], "%g (used with picking)",defaultExponent); nt++;

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    GUIState::addPrefix(textLabels,prefix,cmdWithPrefix,maximumCommands);
    pickDialog.setTextBoxes(cmdWithPrefix, textLabels, textStrings);
  
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("stretch>");  

    SelectionInfo select; select.nSelect=0;  
    for( int it=0; ; it++ )
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
      gi.getAnswer(answer,"", select);
      gi.savePickCommands(true); // turn back on

      if( answer(0,prefix.length()-1)==prefix )
	answer=answer(prefix.length(),answer.length()-1);

      if( answer=="exit" || answer=="continue" || answer=="done" )
      {
	break;
      }
      else if( checkForStretchCommands(answer,gi,dialog ) )
      {
      }
      else if( len=answer.matches("default weight") )
      {
	sScanF(answer(len,answer.length()-1),"%e",&defaultWeight);
	pickDialog.setTextLabel("default weight",sPrintF(line, "%g (used with picking)",defaultWeight));
      }
      else if( len=answer.matches("default exponent") )
      {
	sScanF(answer(len,answer.length()-1),"%e",&defaultExponent);
	pickDialog.setTextLabel("default exponent",sPrintF(line, "%g (used with picking)",defaultExponent));
      }
      else if( answer.matches("pick to" ) )
      {
	pickingOption= (answer=="pick to do nothing" ?             pickToDoNothing :
			answer=="pick to stretch in direction 1" ? pickToStretchInDirection1 :
			answer=="pick to stretch in direction 2" ? pickToStretchInDirection2 : 
                                                                   pickToStretchInDirection3);
        
	pickDialog.getRadioBox("Picking:").setCurrentChoice((int)pickingOption);
      }
      else if( select.nSelect )
      {
        realArray x(1,3), r(1,3), xp(1,3);
	x(0,0)=select.x[0]; x(0,1)=select.x[1]; x(0,2)=select.x[2];
        r=-1.;
	dpm->inverseMap(x,r);
        dpm->map(r,xp);
	
        r=min(1.,max(0.,r));
	
	printf("Point (%9.3e,%9.3e,%9.3e) is located at r=(%9.3e,%9.3e,%9.3e) xp=(%9.3e,%9.3e,%9.3e)\n",x(0,0),x(0,1),x(0,2),
                  r(0,0),r(0,1),(domainDimension==2? 0. : r(0,2)),xp(0,0),xp(0,1),xp(0,2));
	
        // find an unused value for stretchID
        stretchID=-1;
	for( int i=0; i<stretchParams.getLength(1); i++ )
	{
	  if( stretchParams(1,i)<=0. )
	  {
	    stretchID=i;
	    break;
	  }
	}
	if( stretchID<0 )
	{
          // add more space
          stretchID=stretchParams.getLength(1);
	  int num=stretchParams.getLength(1);
	  stretchParams.resize(stretchParams.getLength(0),stretchID+10);
	  Range all;
	  stretchParams(all,Range(num,stretchParams.getBound(1)))=0;
	}
	
        printf("Setting stretchID=%i\n",stretchID);
	
	stretchParams(1,stretchID)=defaultWeight;
	stretchParams(2,stretchID)=defaultExponent;
	if( pickingOption==pickToStretchInDirection1 )
	{
	  stretchParams(0,stretchID)=0;
  	  stretchParams(3,stretchID)=r(0,0);
	  gi.outputToCommandFile(sPrintF(line,"stretch r1 %i %.2g %.2g %.2g (id,weight,exponent,position)\n",stretchID,
					 stretchParams(1,stretchID),stretchParams(2,stretchID),
                                         stretchParams(3,stretchID)));
	}
	else if( pickingOption==pickToStretchInDirection2 )
	{
	  stretchParams(0,stretchID)=1;
  	  stretchParams(3,stretchID)=r(0,1);
	  gi.outputToCommandFile(sPrintF(line,"stretch r2 %i %.2g %.2g %.2g (id,weight,exponent,position)\n",stretchID,
					 stretchParams(1,stretchID),stretchParams(2,stretchID),
                                         stretchParams(3,stretchID)));
	}
	else if( pickingOption==pickToStretchInDirection3 )
	{
	  stretchParams(0,stretchID)=2;
  	  stretchParams(3,stretchID)=r(0,2);
	  gi.outputToCommandFile(sPrintF(line,"stretch r3 %i %.2g %.2g %.2g (id,weight,exponent,position)\n",stretchID,
					 stretchParams(1,stretchID),stretchParams(2,stretchID),
                                         stretchParams(3,stretchID)));
	}
	else 
	{
	  printf("INFO: The picking option is currently to do nothing\n");
	}
      }
      else
      {
	printf("ERROR: unknown response = [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
    }
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }
  else if( answer.matches("reset to unstretched grid") )
  {
    // undo all stretching
    gridIsStretched=false;
    stretchParams=0.;
    stretchParams=0.;
    stretchParams(0,0)=0.;  // axis
    stretchParams(1,0)=0.;  // a
    stretchParams(2,0)=10.; // b
    stretchParams(3,0)=.5;  // c
    
    stretchParams(0,1)=1.;
    stretchParams(1,1)=0.;
    stretchParams(2,1)=10.;
    stretchParams(3,1)=.5;
 
    stretchParams(0,2)=1.;
    stretchParams(1,2)=0.;
    stretchParams(2,2)=10.;
    stretchParams(3,2)=.5;

    if( stretchedMapping!=NULL )
    {
      applyStretching(*stretchedMapping);
    }
    else
    {
      Range Rx=rangeDimension;
      bool xWasReshaped=false;
      if( domainDimension==2 && x.getLength(1)==1 && x.getLength(2)>1 )
      {
	// When called from the HyperbolicGridGenerator x may have a funny shape
	xWasReshaped=true;
	x.reshape(x.dimension(0),x.dimension(2),1,Rx);
      }

   

      dpm->setDataPoints(x,3,domainDimension,0,gridIndexRange);

      
      if( xWasReshaped && domainDimension==2 )
	x.reshape(x.dimension(0),1,x.dimension(1),Rx);
    }
    returnValue=gridWasChanged;
  }
  else if( len=answer.matches("stretch grid") )
  {
    gridIsStretched=true;
    if( stretchedMapping!=NULL )
    {
      applyStretching(*stretchedMapping);
    }
    else
    {
      assert( dpm!=NULL );

      StretchTransform stretchedDPM;
      stretchedDPM.setMapping(*dpm);
      StretchedSquare & stretchedSquare = stretchedDPM.getStretchedSquare();

      applyStretching(stretchedDPM);
      int axis;
      for( axis=0; axis<domainDimension; axis++ )
      {
	stretchedSquare.setIsPeriodic(axis,dpm->getIsPeriodic(axis));
      }
      
      // evaluate the stretched grid -- include ghost point ---
      Range Rx=rangeDimension;
      bool xWasReshaped=false;
      if( domainDimension==2 && x.getLength(1)==1 && x.getLength(2)>1 )
      {
	// When called from the HyperbolicGridGenerator x may have a funny shape
	xWasReshaped=true;
	x.reshape(x.dimension(0),x.dimension(2),1,Rx);
      }
    
      Index I1,I2,I3;
      I1=x.dimension(0); I2=x.dimension(1); I3=x.dimension(2);

      printf("stretch: gridIndexRange=[%i,%i][%i,%i][%i,%i] projectIndexRange=[%i,%i][%i,%i] "
	     "x=[%i,%i][%i,%i][%i,%i]\n",
             gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),
             gridIndexRange(0,2),gridIndexRange(1,2),projectIndexRange(0,0),projectIndexRange(1,0),
             projectIndexRange(0,1),projectIndexRange(1,1),
             x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2));
    
      if( gridIsStretched )
      {
	// reset the grid if it was already stretched
	dpm->setDataPoints(x,3,domainDimension,0,gridIndexRange);
      }

      realArray r(I1,I2,I3,domainDimension);
      realArray xs(I1,I2,I3,rangeDimension);
    
      real dr[3];
      for( axis=axis1; axis<=axis3; axis++ )
	dr[axis]=1./max(gridIndexRange(1,axis)-gridIndexRange(0,axis),1);

      const int i1Base=gridIndexRange(0,axis1);
      const int i2Base=gridIndexRange(0,axis2);
      const int i3Base=gridIndexRange(0,axis3);
      int i1,i2,i3;
      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	r(i1,I2,I3,0)=(i1-i1Base)*dr[axis1];
      if( domainDimension>1 )
      {
	// printf(" dr=%e %e %e I2=%i %i\n",dr[0],dr[1],dr[2],I2.getBase(),I2.getBound());
      
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  r(I1,i2,I3,1)=(i2-i2Base)*dr[axis2];
      }
      if( domainDimension>2 )
      {
	for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  r(I1,I2,i3,2)=(i3-i3Base)*dr[axis3];
      }

      // ::display(r,"r","%5.2f ");
      stretchedDPM.mapGrid(r,xs);
      // ::display(x,"stretch x","%8.2e ");
    
      if( domainDimension==2 && rangeDimension==3 && projectStretchedGridOntoReferenceSurface && surface!=NULL )
      {
	printf("*** stretch project points onto the reference surface...\n");
	// we should have a common projection scheme for smooth ??

	MappingProjectionParameters mpParams;  // should be save this for subsequent projections??
	typedef MappingProjectionParameters MPP;
	mpParams.setIsAMarchingAlgorithm(false);
	mpParams.setAdjustForCornersWhenMarching(false);

	intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
	subSurfaceIndex=-1;  // set initial guess

	// *** include appropriate ghost points ****
	Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
	J1=0; J2=0; J3=0;
	// for( axis=0; axis<domainDimension; axis++ )
	// Jv[axis]=Range(gid(0,axis),gid(1,axis));

	Jv[axis1]=Range(projectIndexRange(0,axis1),projectIndexRange(1,axis1));
	Jv[axis2]=Range(projectIndexRange(0,axis2),projectIndexRange(1,axis2));
      
	printf("Project points [%i,%i][%i,%i] onto the reference surface\n",J1.getBase(),J1.getBound(),
	       J2.getBase(),J2.getBound());
      

	realArray xx;
	xx=xs(J1,J2,J3,Rx);
	Range R=J1.getLength()*J2.getLength()*J3.getLength();
	xx.reshape(R,Rx);
	
	surface->project(xx,mpParams);

	xx.reshape(J1,J2,J3,Rx);
	xs(J1,J2,J3,Rx)=xx;

	// do a periodic update here
	periodicUpdate( xs,gridIndexRange,*dpm );

      }

      dpm->setDataPoints(xs,3,domainDimension,0,gridIndexRange);

      
      if( xWasReshaped && domainDimension==2 )
	x.reshape(x.dimension(0),1,x.dimension(1),Rx);

    }
    
    returnValue=gridWasChanged;
  }
  else
  {
    returnValue=answerNotProcessed;
  }

  return returnValue;
}



int GridStretcher::
periodicUpdate( realArray & x, const IntegerArray & indexRange, Mapping & map )
// =============================================================================================
// =============================================================================================
{
  Index Is1,Is2,Is3,Ie1,Ie2,Ie3;
  int is[3]={0,0,0}; 
  Range Rx=x.dimension(3); // (0,rangeDimension-1);
  
  for( int dir=0; dir<domainDimension; dir++ )
  {
    if( (bool)map.getIsPeriodic(dir) )
    {
      is[dir]=1;
      getBoundaryIndex(indexRange,Start,dir,Is1,Is2,Is3);
      getBoundaryIndex(indexRange,End  ,dir,Ie1,Ie2,Ie3);
      x(Ie1,Ie2,Ie3,Rx)=x(Is1,Is2,Is3,Rx);
      x(Ie1+is[0],Ie2+is[1],Ie3+is[2],Rx)=x(Is1+is[0],Is2+is[1],Is3+is[2],Rx);
      x(Is1-is[0],Is2-is[1],Is3-is[2],Rx)=x(Ie1-is[0],Ie2-is[1],Ie3-is[2],Rx);

      is[dir]=0;
    }
  }
  return 0;
}


int GridStretcher::
applyStretching( StretchTransform & stretchedDPM )
{
  StretchedSquare & stretchedSquare = stretchedDPM.getStretchedSquare();

    // first count the number of layers in each direction
  int numberOfLayers[3]={0,0,0};
  int i,axis;
  for( i=0; i<=stretchParams.getBound(1); i++ )
  {
    axis=int(stretchParams(0,i)+.5);
    if( stretchParams(1,i)>0. )
    {
      assert( axis>=0 && axis<3 );
      numberOfLayers[axis]++;
    }
  }
  for( axis=0; axis<domainDimension; axis++ )
  {
    StretchMapping & stretch = stretchedSquare.stretchFunction(axis);
      
    printf(" Stretch: axis=%i setNumberOfLayers=%i\n",axis,numberOfLayers[axis]);
      
    if( numberOfLayers[axis]>0 )
    {
      stretch.setNumberOfLayers( numberOfLayers[axis] );

      stretch.setStretchingType(StretchMapping::inverseHyperbolicTangent);
      printf("GridStretcher::applyStretching: stretchedDPM.getIsPeriodic(%i)=%i\n",
	     axis,stretchedDPM.getIsPeriodic(axis));
      
      stretch.setIsPeriodic(axis,stretchedDPM.getIsPeriodic(axis));

      int index=0;
      for( i=0; i<=stretchParams.getBound(1); i++ )
      {
	if( int(stretchParams(0,i)+.5)==axis && stretchParams(1,i)>0. )
	{
	  printf(" Stretch: axis=%i index=%i i=%i (a,b,c)=(%8.2e,%8.2e,%8.2e)\n",axis,index,i,
		 stretchParams(1,i),stretchParams(2,i),stretchParams(3,i));

	  stretch.setLayerParameters(index, stretchParams(1,i),stretchParams(2,i),stretchParams(3,i));
	  index++;
	}
      }
    }
  }
  
  return 0;
}
