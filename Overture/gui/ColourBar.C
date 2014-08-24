#ifndef NO_APP
#include "GenericDataBase.h"
#else
#include "GUITypes.h"
#endif

#include "GL_GraphicsInterface.h"
#include "GraphicsParameters.h"
#include "ColourBar.h"

#ifdef NO_APP
#define redim resize
#define getBound(x) size((x))-1
#define getLength size
using GUITypes::real;
using std::cout;
using std::endl;
#endif

#ifndef NO_APP
#undef aString
#include "aString.H"
#endif

//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{eraseColourBar}} 
void GL_GraphicsInterface:: 
eraseColourBar()
//------------------------------------------------------------------------
// /Description:
//       Erase the colour bar.
//\end{GL_GraphicsInterfaceInclude.tex} 
//------------------------------------------------------------------------
{
  glDeleteLists(getColourBarDL(currentWindow),1);  // erase the colour bar
}


//\begin{>>GL_GraphicsInterfaceInclude.tex}{\subsection{drawColourBar}} 
void GL_GraphicsInterface:: 
drawColourBar(const int & numberOfContourLevels,
              RealArray & contourLevels,
              real uMin,
              real uMax,
              GraphicsParameters & parameters,
	      real xLeft   /* =.775 */,  // .8
	      real xRight  /* =.825 */, // .85
	      real yBottom /* =-.75 */,
	      real yTop    /* =.75  */)
//------------------------------------------------------------------------
// /Description:
//       Draw the colour Bar *** this is the old version ***
//  /numberOfContourLevels (input): put this many labels on the colour bar 
//  /contourLevels (input) : if not null, this array species the contour levels
//  uMin,uMax : these values determine the labels
//  /xLeft, xRight, xBotton, xTop (input): position of colour bar in normalized coordinates, [-1,1]
//\end{GL_GraphicsInterfaceInclude.tex} 
//------------------------------------------------------------------------
{
// AP: Draw the colourbar in the center and shift it to the right spot in the display function
  xLeft = -0.0225;  // *wdh* 030701 -0.025; 
  xRight = 0.0225;
  yBottom = -0.75;
  yTop = 0.75;

  if (glIsList(getColourBarDL(currentWindow)))
    glDeleteLists(getColourBarDL(currentWindow),1);  // erase the colour bar if it is there
  glNewList(getColourBarDL(currentWindow),GL_COMPILE);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glShadeModel(GL_SMOOTH);     // interpolate colours between vertices

  real xDiff=xRight-xLeft;
  real yDiff=yTop-yBottom;
  
//    if( yDiff>xDiff )
//    {
//      // vertical bar
//      if( xLeft>0. )
//      {
//        xRight*=(rightSide[currentWindow] - leftSide[currentWindow])*.5;
//        xLeft=xRight-xDiff;
//      }
//      else
//      {
//        xLeft*=(rightSide[currentWindow] - leftSide[currentWindow])*.5;
//        xRight=xLeft+xDiff;
//      }
//    }
//    else
//    {
//      // horizontal bar
//      if( yBottom>0. )
//      {
//        yBottom*=(top[currentWindow]-bottom[currentWindow])*.5;  // shift colour bar to the top
//        yTop=yBottom+yDiff;
//      }
//      else
//      {
//        yTop*=(top[currentWindow] - bottom[currentWindow])*.5;  // shift colour bar to the top
//        yBottom=yTop+yDiff;
//      }
//    }
  

  //  add a white frame behind the colour bar
  const real delta=.02, deltar=.145;
  
  real zOffset=.99;  // raise the colour bar so that it is not covered by the plot (NOTE: the front clip plane is at z=1)
  glBegin(GL_QUAD_STRIP);

  setColour(GenericGraphicsInterface::backGroundColour);  

  glVertex3f(xLeft -delta ,yBottom-delta,zOffset);
  glVertex3f(xRight+deltar,yBottom-delta,zOffset);

  glVertex3f(xLeft-delta  ,yTop+delta,zOffset);
  glVertex3f(xRight+deltar,yTop+delta,zOffset);
  glEnd();

  zOffset+=.0001;
  const int numberOfIntervals=50;
  real y;
  glBegin(GL_QUAD_STRIP);
  int i;
  for( i=0; i<numberOfIntervals; i++ )
  {
    y=yBottom+i*(yTop-yBottom)/(numberOfIntervals-1);
    setColourFromTable((y-yBottom)/(yTop-yBottom),parameters);
    glVertex3f(xLeft ,y,zOffset);
    glVertex3f(xRight,y,zOffset);
  }
  glEnd();


  if( parameters.labelColourBar )
  {

    // ---label colour bar---
    int numberOfLabels=numberOfContourLevels;
    setColour(textColour); // label colour 
    real size=parameters.size(axisNumberSize)*.75;
    char buff[180];
#ifndef NO_APP
    bool contourLevelsSpecified = contourLevels.getLength(0)>0;
#else
    bool contourLevelsSpecified = contourLevels.size();
#endif
    int skipLabel = int(1. + numberOfLabels /20);  // plot at most 20 labels, if more then skip

    const real xLabelPos=xRight-size*.25;

    // printf("ColourBar: uMin=%8.2e uMax=%8.2e xLabelPos=%8.2e\n",uMin,uMax,xLabelPos);
  
    for( i=0; i<numberOfLabels; i+=skipLabel )
    {
      if( contourLevelsSpecified )
	y=yBottom+(yTop-yBottom)*(contourLevels(i)-uMin)/(uMax-uMin);
      else
	y=yBottom+i*(yTop-yBottom)/(numberOfLabels-1);
      real alpha=(y-yBottom)/(yTop-yBottom);
      if( y>=yBottom && y<=yTop ) 
      {
	// *wdh* 991004 if( max(fabs(uMax),fabs(uMin)) < .001 )
	real uVal=uMin+alpha*(uMax-uMin);
	if(  max(fabs(uMax),fabs(uMin)) < .01 )
	{
	  if( uVal>=0. )
	    label(sPrintF(buff," %7.2e",uVal),xLabelPos,y,size,-1,0.,parameters);  // flush left
	  else // *wdh* 030627 -- if negative show one less digit so we can see the fullnumber
	    label(sPrintF(buff," %6.1e",uVal),xLabelPos,y,size,-1,0.,parameters); 
	}
	else if( max(fabs(uMax),fabs(uMin)) < 10. )
	  label(sPrintF(buff," %6.3f",uVal),xLabelPos,y,size,-1,0.,parameters);  // flush left
	else if( max(fabs(uMax),fabs(uMin)) < 100. )
	  label(sPrintF(buff," %7.2f",uVal),xLabelPos,y,size,-1,0.,parameters);  // flush left
	else if( uVal>=0. )
	  label(sPrintF(buff," %7.2e",uVal),xLabelPos,y,size,-1,0.,parameters);  // flush left
	else // *wdh* 030627 -- if negative show one less digit so we can see the fullnumber
	  label(sPrintF(buff," %6.1e",uVal),xLabelPos,y,size,-1,0.,parameters);  // flush left
      }
    }
    // ---draw lines on colour bar corresponding to the contour levels
    for( i=0; i<numberOfContourLevels; i+=skipLabel )
    {
      if( ((i+4+100) % 5)/4 == 0 )  // make every fifth line wider
	glLineWidth(parameters.size(minorContourWidth)*parameters.size(lineWidth)*
		    lineWidthScaleFactor[currentWindow]); 
      else
	glLineWidth(parameters.size(majorContourWidth)*parameters.size(lineWidth)*
		    lineWidthScaleFactor[currentWindow]);  
      if( contourLevelsSpecified )
	y=yBottom+(yTop-yBottom)*(contourLevels(i)-uMin)/(uMax-uMin);
      else
	y=yBottom+i*(yTop-yBottom)/(numberOfLabels-1);

      glBegin(GL_LINES);
      if( y>=yBottom && y<=yTop ) 
      {
	glVertex3(xLeft ,y,zOffset+.001);  // raise the lines so we see them
	glVertex3(xRight,y,zOffset+.001);
      }
      glEnd();
    }
    glLineWidth(parameters.size(lineWidth)*lineWidthScaleFactor[currentWindow]);   // reset
  }
  
  
  glEndList();
}



//
// ------------ COLOUR BAR
//
ColourBar::
ColourBar(GL_GraphicsInterface*  gi      /*= NULL*/,
	  GraphicsParameters*    gparams /*= NULL*/ )
  : _gi(gi), _gparameters( gparams )
{
  pi = 4.*atan(1.);

  // for ColourBars: default is a vertical bar on the right
  //.. all lengths are in normalized units, where screen is [-1,1]
  //colourBarPosition                 = rightColourBar;
  colourBarPosition                 = useOldColourBar;
  colourBarWidth                    = 0.05;
  colourBarLength                   = 1.5;

  colourBarCenter[0]                = 0.75 -1.5;   // .775 .8 *wdh* : -1 to centre, -.5 to un-offset
  colourBarCenter[1]                = -.9; // 0. *wdh* 
  
  colourBarAngle                    = 0.; //in degrees
  colourBarCurvature                = 0.; 
  colourBarOffsetFromPlot           = 0.;

  colourBarLabelOption              = colourBarLabelsOn;  //..colourbar labels
  colourBarLabelOnRight             = TRUE;
  colourBarLabelAngle               = 0.;
  colourBarRelativeAngle            = FALSE;
  colourBarLabelNormalOffset        = 0.05;
  colourBarLabelTangentialOffset    = 0.;
  colourBarNumberOfIntervals        = 50;
  colourBarMaximumNumberOfLabels    = 20;
  colourBarLabelScaling             = 0.75;
  colourBarThickLineInterval        = 5;

}
  
ColourBar::
~ColourBar()
{
  //default
}

void ColourBar::
setGraphicsInterface(GL_GraphicsInterface *gi)
{
  _gi=gi;
}

void ColourBar::
setGraphicsParameters(GraphicsParameters  *parameters)
{
  _gparameters=parameters;
}



void ColourBar::
positionInWindow(real leftSide_, real rightSide_, real bottom_, real top_)
// =====================================================================================
// /Description:
//   This next function is called by the GL_GraphicsInterface::draw to correctly position
// the colour bar after the window has been resized.
// =====================================================================================
{
  leftSide=leftSide_;
  rightSide=rightSide_;
  bottom=bottom_;
  top=top_;
  if( colourBarPosition==leftColourBar )
  {
    glTranslate(leftSide+colourBarCenter[0], 0., 0.);
  }
  else if( colourBarPosition==rightColourBar )
  {
    glTranslate(rightSide+colourBarCenter[0], 0., 0.);
  }
  else if( colourBarPosition==topColourBar )
  {
    glTranslate(0.,top+colourBarCenter[1], 0.);
  }
  else if( colourBarPosition==   bottomColourBar )
  {
    glTranslate(0.,bottom+colourBarCenter[1], 0.);
  }
  else
  {
    // glTranslate(rightSide-0.225, 0., 0.);
    glTranslate(rightSide-0.20, 0., 0.);
  }
  
}



void ColourBar::
draw(const int & numberOfContourLevels_,
     RealArray & contourLevels_,
     real uMin_,
     real uMax_)
{
    

  if(!preCheck()) return;
  
  if( colourBarPosition==useOldColourBar )
  {
     setupDraw( numberOfContourLevels_, contourLevels_, uMin_, uMax_);
    _gi->drawColourBar(numberOfContourLevels_,contourLevels_,uMin_,uMax_,*_gparameters);
    return;
  }

  // cout <<"ColourBar::draw called, number of contour levels="<< numberOfContourLevels_<<".\n";
  
  //..Start the display list
  currentWindow = _gi->getCurrentWindow();
  if (glIsList(_gi->getColourBarDL(currentWindow)))       // -->sub: newColourBarDisplayList
    glDeleteLists(_gi->getColourBarDL(currentWindow),1);  // erase the colour bar if it is there: sub: eraseCBDispList
  glNewList(_gi->getColourBarDL(currentWindow),GL_COMPILE); //open display list --> use newColourBarDisplayList

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glShadeModel(GL_SMOOTH);     // interpolate colours between vertices
// *wdh*   _gi->setNormalizedCoordinates();   // The colour bar is drawn in a way that is unaffected by rotations and scalings 

  //..Draw the bar & the labels
  // const int nlevels = 11;
  setupDraw(  numberOfContourLevels_, contourLevels_, uMin_, uMax_);
  drawBar();   
  drawBarLines();
  drawLabels();

  //..Finish up 
  glLineWidth( sizeLineWidth * lineWidthScaleFactor ); // reset
// *AP* the following function is obsolete and does absolutely nothing
//  _gi->unsetNormalizedCoordinates();  
  glEndList(); // close display

}

//void ColourBar::
//update( GL_GraphicsInterface*  gi      = NULL,   //gui interface
//	GraphicsParameters*    gparams = NULL )
void ColourBar::
update()
{
  if (!preCheck()) return;

  aString prefix="CBAR:"; // prefix for all commands

  //....setup menus
  const int maxMenuItems=40;
  aString *colourBarMenu = new aString [maxMenuItems];
  int n=0;
  colourBarMenu[n++]="!colour bar options";
  colourBarMenu[n++]= "done";
  
  colourBarMenu[n]="";
  assert( n<maxMenuItems );
 
  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = (DialogData &)gui;

  GUIState advancedGUI;
  bool     showingAdvancedGUI = FALSE;

  {//..build the dialog
    const int maxCommands=20;
    const int maxLabels  =30;
    const int numberOfColumns=1;
    int   dummy;
    dialog.setWindowTitle("Colour Bar Options");
    dialog.setOptionMenuColumns(numberOfColumns);

    //TODO -- fix the addPrefix routine. Have it return a cmd[]

    //..Colour tables: these should be read from somewhere else,
    //    shouldn't be hardwired here. **pf
    aString cmd[maxLabels];
    aString cmdLabels[] = {"rainbow", "gray", "red",  "green",  "blue",  "userDefined", ""};  
    //GUIState::addPrefix(cmdLabels, prefix, cmd, maxCommands);
    //dialog.addOptionMenu("Colour table      ", cmd, cmdLabels, _cbparameters->colourTable);
    dialog.addOptionMenu("Colour table      ", cmdLabels, cmdLabels, 
			 _gparameters->get( GI_COLOUR_TABLE, dummy));
    
    aString cmd2[maxLabels];
    aString cmdLabels2[] = {"left", "right", "top", "bottom", "user defined position","use old colour bar",""};
    //GUIState::addPrefix(cmdLabels2, prefix, cmd2, maxCommands);
    //dialog.addOptionMenu("Position          ", cmd2, cmdLabels2, _cbparameters->colourBarPosition);
    dialog.addOptionMenu("Position          ", cmdLabels2, cmdLabels2,(int)colourBarPosition);
    
    aString cmd3[maxLabels];
    aString cmdLabels3[] = {"off","endsOnly", "on",""};
    //GUIState::addPrefix(cmdLabels3, prefix, cmd3, maxCommands);
    //dialog.addOptionMenu("Labels            ", cmd3, cmdLabels3, _cbparameters->colourBarLabelOption);
    dialog.addOptionMenu("Labels            ", cmdLabels3, cmdLabels3,(int)colourBarLabelOption);
    
    aString cmd4[maxLabels];
    aString cmdLabels4[] = {"left","right",""};
    //GUIState::addPrefix(cmdLabels4, prefix, cmd4, maxCommands);
    //dialog.addOptionMenu("Label position    ", cmd4, cmdLabels4, _cbparameters->colourBarLabelOnRight);
    dialog.addOptionMenu("Label position    ", cmdLabels4, cmdLabels4,(int)colourBarLabelOnRight);
    
    aString cmd5[maxLabels];
    aString cmdLabels5[] = {"horizontal","vertical","custom orientation",""};
    //GUIState::addPrefix(cmdLabels5, prefix, cmd5, maxCommands);
    //dialog.addOptionMenu("Label orientation ", cmd5, cmdLabels5, _cbparameters->labelOrientation);
    dialog.addOptionMenu("Label orientation ", cmdLabels5, cmdLabels5, getLabelOrientation( dummy ));
    
    //aString buttonLabels[] = {"Plot", "Advanced...", ""};
    //aString buttonCommands[], buttonCommandList[] = {"plot", "advanced...",  ""};
    //GUIState::addPrefix(buttonCommandList, prefix, buttonCommands, maxCommands);
    aString buttonLabels[] = {"Apply", "Pick center...", ""};
    aString buttonCommands[maxLabels];
    aString buttonCommandList[] = {"apply", "pick center",  ""};

    
    const int numberOfRows=1;
    dialog.setPushButtons(buttonCommands, buttonLabels, numberOfRows); 

    showAdvancedDialog( prefix,  gui  ); //sets up the rest of the dialog

    gui.buildPopup(colourBarMenu);
    //if( ps.isGraphicsWindowOpen() )
    dialog.openDialog(1);   // open the dialog here so we can reset the parameter values below
  }

  //..display popup & wait for answers
  aString answer;
  char buff[100];
  int len;

  _gi->pushGUI(gui);
  _gi->appendToTheDefaultPrompt("colour bar options>");  

  while(1)  // dialog processing loop
  {
    bool plotObject=FALSE;
    _gi->getAnswer(answer,"");
    cout <<"ColourBar -- answer="<<answer<<endl;
    if( substring(answer,0,prefix.length()-1)==prefix ) //remove prefix
      answer=substring(answer,prefix.length(),answer.length()-1);
    
    int len=-1;
    if( matches(answer,"center") ) cout << "found `center'\n";
    if( matches(answer,"width") )  cout << "found `width'\n";

    if( answer=="done" ) {
      cout << "DONE---exiting\n";
      break;
      //Colour tables
    } 
    else if( answer=="rainbow" || answer=="gray" || answer=="red" || answer=="green" || answer=="blue" ||
             answer=="user defined" )
    {
      int ct;
      if( answer=="rainbow" )
        ct=GraphicsParameters::rainbow;
      else if( answer=="gray" )
        ct=GraphicsParameters::gray;
      else if( answer=="red" )
        ct=GraphicsParameters::red;
      else if( answer=="green" )
        ct=GraphicsParameters::green;
      else if( answer=="blue" )
        ct=GraphicsParameters::blue;
      else
        ct=GraphicsParameters::userDefined;

      _gparameters->set(GI_COLOUR_TABLE, 
			GraphicsParameters::ColourTables(max(min(ct,GraphicsParameters::numberOfColourTables),0))); //?? **pf
    } 
    else if( answer=="left" || answer=="right" || answer=="bottom" || answer=="top" )
    {
      
      if( answer=="left" )
      {
        colourBarPosition = leftColourBar;
	colourBarCenter[0]=.05;  // offset from left side
	colourBarCenter[1]=0.;  // offset from centre
	colourBarAngle=90.;
	colourBarWidth=.05;
	colourBarLength=1.5;
	colourBarLabelAngle=0.;
	colourBarLabelNormalOffset=1.;
	colourBarLabelTangentialOffset=0.;
	
	plotObject = true;
      }
      else if( answer=="right" )
      {
        colourBarPosition = rightColourBar;
	colourBarCenter[0]=-.085;  // offset from right side
	colourBarCenter[1]=0.;
	colourBarAngle=90.;
	colourBarWidth=.05;
	colourBarLength=1.5;
	colourBarLabelAngle=0.;
	colourBarLabelNormalOffset=-1.;
	colourBarLabelTangentialOffset=0.;

	plotObject = true;
      }
      else if( answer=="top" )
      {
        colourBarPosition = topColourBar;
	colourBarCenter[0]=0.;
	colourBarCenter[1]=-.125; // offset from top
	colourBarAngle=0.;
	colourBarWidth=.05;
	colourBarLength=1.5;
	colourBarLabelAngle=90.;
	colourBarLabelNormalOffset=.5;
	colourBarLabelTangentialOffset=-.5;


	plotObject = true;
      }
      else if( answer=="bottom" )
      {
	colourBarPosition = bottomColourBar;
	colourBarCenter[0]=0.;
	colourBarCenter[1]=+.075; // offset from bottom
	colourBarAngle=0.;
	colourBarWidth=.05;
	colourBarLength=1.5;
	colourBarLabelAngle=90.;
	colourBarLabelNormalOffset=-1.5;
	colourBarLabelTangentialOffset=-.5;

	plotObject = true;
      }

      dialog.setTextLabel(0,(const aString)sPrintF(answer, "%g,%g",colourBarCenter[0],colourBarCenter[1])); 
      dialog.setTextLabel(1,(const aString)sPrintF(answer, "%g", colourBarWidth)); 
      dialog.setTextLabel(2,(const aString)sPrintF(answer, "%g", colourBarLength));
      dialog.setTextLabel(4,(const aString)sPrintF(answer, "%g",colourBarAngle));
      dialog.setTextLabel(6,(const aString)sPrintF(answer, "%g",colourBarLabelAngle));
      dialog.setTextLabel(7,(const aString)sPrintF(answer, "%g",colourBarLabelNormalOffset));
      dialog.setTextLabel(8,(const aString)sPrintF(answer, "%g",colourBarLabelTangentialOffset));
    }
    else if( matches(answer,"user defined position") 
	     || answer==("use old colour bar")  || matches(answer,"wiggly colour bar"))
    {

      if( answer=="user defined position" )
	colourBarPosition = userDefinedColourBar;
      else if( matches(answer,"use old colour bar") )
	colourBarPosition = useOldColourBar;
      else if( matches(answer,"wiggly colour bar") )
	colourBarPosition = customColourBar1;
      else cout << "Warning. ColourBar::update (colourBarPosition) -- answer = `"<<answer<<"' unknown.\n";

      cout << "ColourBar::update -- position = "<<colourBarPosition<<endl;
      
    }
    else if( answer=="Apply")
    {
      cout << "...plotting the colourbar...\n";
      plotObject = TRUE;
    }
    else if( answer=="Advanced...")
    {
      cout << "...advanced not active...\n";
      //showAdvancedDialog( prefix, advancedGUI );
      //showingAdvancedGUI = TRUE;
      //_gi->pushGUI( advancedGUI );
    }
    else if( answer=="done(advanced)") 
    {
      cout << "...advanced not active...\n";
      //if(showingAdvancedGUI) {
      //	showingAdvancedGUI = FALSE;
      //	//_gi->popGUI();
      //}
    }
    //
    //------------ processing for advanced options
    //
    else if( (len=matches(answer,"center (x,y))")) )
    {
      sScanF(substring(answer,len,answer.length()),"%e %e", &colourBarCenter[0], &colourBarCenter[1] );
      cout << "..center   x=" << colourBarCenter[0] << ",   y=" << colourBarCenter[1] << endl;
      dialog.setTextLabel(0,(const aString)sPrintF(answer, "%g,%g",colourBarCenter[0],colourBarCenter[1])); 
      plotObject = TRUE;
    }
    else if( (len=matches(answer,"width")) )
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarWidth);
      cout << "..width = " << colourBarWidth << endl;
      dialog.setTextLabel(1,(const aString)sPrintF(answer, "%g", colourBarWidth)); 
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"length")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarLength);
      cout << "..length = " << colourBarLength << endl;
      dialog.setTextLabel(2,(const aString)sPrintF(answer, "%g", colourBarLength));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"offset from plot")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarOffsetFromPlot);
      cout << "..offset from plot = " << colourBarOffsetFromPlot << endl;
      dialog.setTextLabel(3,(const aString)sPrintF(answer, "%g",colourBarOffsetFromPlot));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"angle")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarAngle);
      cout << "..angle = " << colourBarAngle << endl;
      dialog.setTextLabel(4,(const aString)sPrintF(answer, "%g",colourBarAngle));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"curvature")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarCurvature);
      cout << "..curvature = " << colourBarCurvature;
      if (colourBarCurvature>1e-5) cout<<",  radius of curvature = "<< 1.0/colourBarCurvature;
      cout << endl;
      dialog.setTextLabel(5,(const aString)sPrintF(answer, "%g",colourBarCurvature));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"label angle")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarLabelAngle);
      cout << "..label angle = " << colourBarLabelAngle << endl;
      dialog.setTextLabel(6,(const aString)sPrintF(answer, "%g",colourBarLabelAngle));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"label normal offset")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarLabelNormalOffset);
      cout << "..label normal offset = " << colourBarLabelNormalOffset << endl;
      dialog.setTextLabel(7,(const aString)sPrintF(answer, "%g",colourBarLabelNormalOffset));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"label tangential offset")))
    {
      sScanF(substring(answer,len,answer.length()),"%e", &colourBarLabelTangentialOffset);
      cout << "..label tangential offset = " << colourBarLabelTangentialOffset << endl;
      dialog.setTextLabel(8,(const aString)sPrintF(answer, "%g",colourBarLabelTangentialOffset));
      plotObject = TRUE;
    }
    else if(  (len=matches(answer,"number of intervals"))) // integer
    {
      sScanF(substring(answer,len,answer.length()),"%i", &colourBarNumberOfIntervals);
      cout << "..number of intervals = " << colourBarNumberOfIntervals << endl;
      dialog.setTextLabel(9,(const aString)sPrintF(answer, "%i",colourBarNumberOfIntervals));
      plotObject = TRUE;
    }
    else
    {
      cout << "Unknown response=[" << answer << "]\n";
      _gi->stopReadingCommandFile();
    }    

    if(plotObject) 
    {
      if(pContourLevels!=NULL) 
      {
	draw( numberOfContourLevels, *pContourLevels, uMin, uMax );
	_gi->redraw(TRUE);
      }
      else cout << "ColourBar::update  plotting ERROR -- no contourLevels.\n";
      plotObject = FALSE;
    }

  } //end while -- the answer loop
  delete [] colourBarMenu;
  
  _gi->popGUI();
  _gi->unAppendTheDefaultPrompt();
}


//
// ..utilities
//

//precheck -- check validity of parameters before starting to draw the bar
bool ColourBar::
preCheck()
{
  bool flag = true;
  if( _gi == NULL ) {
    flag = flag && ( _gi!=NULL );
    cout<< "ColourBar::draw  no GL_GraphicsInterface, returning\n";  
  }
  if(_gparameters == NULL ) {
    flag = flag && ( _gparameters!=NULL );
    cout<< "ColourBar::draw  no GraphicsParameters, returning\n";
  }
  return flag;
}

//..compute the parameters needed for drawing the colour bar & labels
void ColourBar::
setupDraw(  const int & numberOfContourLevels_,  RealArray & contourLevels_,
 	    real uMin_,  real uMax_)
{
  uMin = uMin_;
  uMax = uMax_;
  numberOfContourLevels = numberOfContourLevels_;
  if( numberOfContourLevels==0 )
  {
    numberOfContourLevels=11; // use default
  }
  
  pContourLevels        = &contourLevels_;

  //isHorizontalBar  = true; //false; //true; //false;
  // cout << "ColourBar::setupDraw,  coloutBarPosition = "<< colourBarPosition <<endl;

  currentWindow = _gi->getCurrentWindow();
  _gi->getWindowShape( currentWindow, leftSide,rightSide,top,bottom); 

  if( false )
    printf("\n...Window shape: left %6.2f, right %6.2f, top %6.2f, bottom %6.2f\n",
	 leftSide, rightSide,top,bottom);


  real minWin    = minimumWindowLength( leftSide, rightSide, top, bottom);
  real maxWin    = maximumWindowLength( leftSide, rightSide, top, bottom);

  //..compute bar position
  //....RESCALING CODE?
  //..horizontal
  //if( yBottom>0.) {  // on the top
  //  yBottom*=(top-bottom)*.5;
  //  yTop=yBottom+yDiff;
  //} else {           // on the bottom
  //  yTop*=(top - bottom)*.5; // this moves the bar up by minWin*0.5
  //  yBottom=yTop-yDiff;
  //}
  //barBase   = xLeft;
  //barLength = xRight - xLeft;
  real barStep   =  colourBarLength/(colourBarNumberOfIntervals-1);

  //printf(" AFTER:  xLeft=%6.2e, xRight=%6.2e, yTop=%6.2e, yBottom=%6.2e.\n",
  //	 xLeft, xRight, yTop, yBottom);

  //..Line widths & data for labels
  lineWidthScaleFactor  =  _gi->getLineWidthScaleFactor(currentWindow);
  _gparameters->get(GraphicsParameters::lineWidth,         sizeLineWidth);
  _gparameters->get(GraphicsParameters::minorContourWidth, sizeMinorContourWidth );
  _gparameters->get(GraphicsParameters::majorContourWidth, sizeMajorContourWidth);
  _gparameters->get(GraphicsParameters::axisNumberSize,    sizeAxisNumberSize);
  
  int idummy; real rdummy;
//  printf(" numberOfContourLevels=%i\n",numberOfContourLevels);
//  printf(" colourBarMaximumNumberOfLabels=%i\n",colourBarMaximumNumberOfLabels);
  skipLabel = int(1. + numberOfContourLevels/colourBarMaximumNumberOfLabels);    // plot at most 20 labels, if more then skip
  labelSize=  sizeAxisNumberSize*colourBarLabelScaling;

  //labelDrawFlag is GI_COLOURBAR_LABEL_OPTION = {0 no labels, 1 endsOnly, 2 On}
  labelDrawFlag = 2;  // will be an enum, from GraphicsParameters ??? what is this? **pf

}

//..draw the colour bar, no labels yet
void ColourBar::
drawBar()
{
  glBegin(GL_QUAD_STRIP);
  int i;
  real xa,ya, xb, yb, za=.95;

  cout << "DRAWING THE BAR = QuadStrip\n";
  for( i=0; i<colourBarNumberOfIntervals; i++ )
  {
    //real barOffset = i*barStep; // bar offset from barBase
    real r=i/real(colourBarNumberOfIntervals-1.);
    computeBarQuad( r, xa, ya, xb, yb );
    _gi->setColourFromTable( r, *(_gparameters));
    glVertex3f( xa, ya,za );
    glVertex3f( xb, yb,za );
    printf("   i= %5i xa= %6.2g,  ya= %6.2g,  xb= %6.2g,  yb= %6.2g,  colour=  %6.2g\n",
	   i,xa,ya,xb,yb, r);
  }
  printf("\n");
  glEnd();

  if( false )
  {
    // draw a filled rectangle to put the colour bar on
    _gi->setColour("red"); // make red while testing
  
    za=.94;
    glBegin(GL_QUAD_STRIP);
    computeBarQuad( 0., xa, ya, xb, yb );
    real wx=fabs(xb-xa);
    real wy=fabs(yb-ya);
    real w=max(wx,wy);

    real borderWidth=.025;
    real labelLength=labelSize*5;
    if( wx<wy )
    { // horizontal
      real offseta= max(0.,-colourBarLabelNormalOffset*labelLength);
      real offsetb= max(0., colourBarLabelNormalOffset*labelLength);

      ya=ya-borderWidth-offseta;                            xa=xa-borderWidth;
      yb=yb+borderWidth+offsetb; xb=xb-borderWidth;

      glVertex3f( xa, ya,za );
      glVertex3f( xb, yb,za );
      computeBarQuad( 1., xa, ya, xb, yb );
      ya=ya-borderWidth-offseta;                            xa=xa+borderWidth;
      yb=yb+borderWidth+offsetb; xb=xb+borderWidth;

      glVertex3f( xa, ya,za );
      glVertex3f( xb, yb,za );
    }
    else
    {
      // vertical
      real offseta= max(0.,colourBarLabelNormalOffset*labelLength);
      real offsetb= max(0.,-colourBarLabelNormalOffset*labelLength);

      xa=xa-borderWidth-offseta; ya=ya-borderWidth;
      xb=xb+borderWidth+offsetb; yb=yb-borderWidth;

      glVertex3f( xa, ya,za );
      glVertex3f( xb, yb,za );
      computeBarQuad( 1., xa, ya, xb, yb );

      xa=xa-borderWidth-offseta;                            ya=ya+borderWidth;
      xb=xb+borderWidth+offsetb; yb=yb+borderWidth;

      glVertex3f( xa, ya,za );
      glVertex3f( xb, yb,za );
    }

    glEnd();
  }
  
}

void ColourBar::
drawBarLines()
{
  // ---draw lines on colour bar corresponding to the contour levels
  _gi->setColour(GL_GraphicsInterface::textColour); // label colour 
  int i;
  cout <<"Drawing bar lines: number of contour levels="<< numberOfContourLevels <<"\n";
  for( i=0; i<numberOfContourLevels; i+=skipLabel )
  {
    real xa,ya,xb,yb;
    real r=i/real(numberOfContourLevels-1.);
    computeBarLevelLine( r, xa, ya, xb, yb);
    const real barZOffset = .95+ 0.01; //raise the lines so we can see them
    const real width = computeContourLineWidth( i );
    glLineWidth( width ); //computeContourLineWidth( i ) );
    glBegin(GL_LINES);
    glVertex3( xa, ya, barZOffset );
    glVertex3( xb, yb, barZOffset );
    printf("   i= %5i xa= %6.2g,  ya= %6.2g,  xb= %6.2g,  yb= %6.2g,  colour=  %6.2g width=%g \n",
	   i,xa,ya,xb,yb, r, width);
    glEnd();
  }
  glLineWidth( sizeLineWidth * lineWidthScaleFactor ); // reset
}

void ColourBar::
drawLabels()
{
  if ( !labelDrawFlag ) return; //return if no labels

  //_gi->setColour(GenericGraphicsInterface::textColour); // label colour 
  _gi->setColour(GL_GraphicsInterface::textColour); // label colour 
  char buff[180];
  //cout << "..Printing labels:\n";

  real angle = -pi/2.;
  int i;
  for( i=0; i<numberOfContourLevels; i+=skipLabel )    // numberOfLabels
  {
    real xPos, yPos; // label position
    //real barLabelOffset, 
    real alpha, angle;
    computeLabelPosition( i, alpha, xPos, yPos, angle );
    real value=alpha*2 - 1.; //uMin+alpha*(uMax-uMin); FIX
    printf("....label %i, x=%6.2e, y=%6.2e, alp=%6.2e, angle=%6.2e, value=%6.2e \n", 
    	   i, xPos, yPos, alpha, angle, value);

    // *wdh* 991004 if( max(fabs(uMax),fabs(uMin)) < .001 )
    if( max(fabs(uMax),fabs(uMin)) < .01 ) 
      _gi->label(sPrintF(buff,"%7.2e",value),xPos,yPos,labelSize,-1,angle, *_gparameters);
    else if( max(fabs(uMax),fabs(uMin)) < 10. )
      _gi->label(sPrintF(buff,"%6.3f",value),xPos,yPos,labelSize,-1,angle, *_gparameters);
    else if( max(fabs(uMax),fabs(uMin)) < 100. )
      _gi->label(sPrintF(buff,"%7.2f",value),xPos,yPos,labelSize,-1,angle, *_gparameters);
    else if( uMin>=0. )
      _gi->label(sPrintF(buff,"%7.2e",value),xPos,yPos,labelSize,-1,angle, *_gparameters); 
    else
      _gi->label(sPrintF(buff,"%6.1e",value),xPos,yPos,labelSize,-1,angle, *_gparameters); 
  }
}

//utility for drawColourBar **pf
void ColourBar:: 
computeLabelPosition( int i, real & alpha, real &xPos, real & yPos, real&labelAngle)
{
  real barStep = 1./(numberOfContourLevels -1);
  real r = (i + colourBarLabelTangentialOffset)*barStep;
  //bool contourLevelsSpecified = pContourLevels->getLength(0)>0;
  bool contourLevelsSpecified = FALSE;
  if( contourLevelsSpecified )
    alpha = ( (*pContourLevels)(i)-uMin)/(uMax-uMin);
  else
    alpha = i*barStep;

  real x,y, nx, ny, tx, ty;  // centerline, normal & tangent
  getCenterLine( r, x,y, nx, ny, tx, ty);

  //...ADD--- 
  //   **code for left or right side of the label
  //   **code for correct handling of the angle
  //
  xPos =  x + (0.5 + colourBarLabelNormalOffset )*colourBarWidth*nx;
  yPos =  y + (0.5 + colourBarLabelNormalOffset )*colourBarWidth*ny;
  stretchToScreenCoordinates( xPos, yPos );

  colourBarRelativeAngle = true;
  if ( !colourBarRelativeAngle)
    labelAngle = colourBarLabelAngle;
  else
    labelAngle = colourBarLabelAngle + getAngle(nx,ny) -180.;
  
}

//utility for drawColourBar **pf
inline real ColourBar:: 
computeBarLabelOffset( const int & numberOfContourLevels,
		       RealArray & contourLevels,
		       real uMin, real uMax,
		       real barLength, int i, real indexOffset /* =0. */ )  
{
  real barOffset;
  real barStep = barLength/(numberOfContourLevels -1);
#ifndef NO_APP
  bool contourLevelsSpecified = contourLevels.getLength(0)>0;
#else
  bool contourLevelsSpecified = contourLevels.size();
#endif
  if( contourLevelsSpecified )
    barOffset = barLength*(contourLevels(i)-uMin)/(uMax-uMin) +indexOffset*barStep  ;
  else
    barOffset = ( i + indexOffset)*barStep;
  return ( barOffset );
}

//utility for drawLabels
void ColourBar::
barLabel( real value, real xPos, real yPos, real angle)
{

}



//
// ..tiny utility funcs
//

// bool ColourBar::
// useOldColourBar()
// {
//   int idummy;
//   return( colourBarPosition == GraphicsParameters::useOldColourBar);
// }

real  ColourBar::
minimumWindowLength(real left_,real right_,real top_,real bottom_)
{ 
  return( min( right_  -  left_, top_  -  bottom_ ));
}

real  ColourBar::
maximumWindowLength(real left_,  real right_,   real top_,  real bottom_)
{ 
  return( max( right_  -  left_, top_  -  bottom_ ));
}

real  ColourBar::
computeContourLineWidth( int i )
{
  if( (( i + 100 + (colourBarThickLineInterval-1)) % colourBarThickLineInterval)/(colourBarThickLineInterval-1) == 0 ) 
    return( sizeMinorContourWidth   *sizeLineWidth*lineWidthScaleFactor);//thicker
  else
    return( sizeMajorContourWidth   *sizeLineWidth*lineWidthScaleFactor);//normal
}

void  ColourBar::
computeBarQuad( real r,  real &xa, real &ya, real &xb, real &yb)
{
  real x,y, nx, ny, tx, ty;  // centerline, normal & tangent
  getCenterLine( r, x,y, nx, ny, tx, ty);
  
  xa = x + 0.5* colourBarWidth*nx;  ya= y + 0.5* colourBarWidth*ny;
  xb = x - 0.5* colourBarWidth*nx;  yb= y - 0.5* colourBarWidth*ny;

  stretchToScreenCoordinates( xa, ya );
  stretchToScreenCoordinates( xb, yb );
}

void  ColourBar::
computeBarLevelLine( real r, real &xa, real &ya, real &xb, real &yb)
{
  //not done
  //cout << "ColourBar::computeBarLevelLine --- don't call me yet...\n";  
  computeBarQuad( r, xa, ya, xb,yb );
}

void  ColourBar::
getCenterLine( real r, real &x, real &y, real &nx, real &ny, 
	       real &tx, real &ty)
{
  const real minimumCurvature = 0.01; // i.e. radius of curv>100 is just flat

  //..compute centerline about x0=0, y0=0, angle=0., then rotate +translate. First curvature=0.
  real xOffset, yOffset;
  if ( colourBarCurvature<minimumCurvature ){ //flat
    //yOffset = 0.;
    //xOffset = (r - 0.5)*colourBarLength;
    y = 0.;
    x = (r - 0.5)*colourBarLength;
    nx = 0; ny = 1.;
  } 
  else //curved
  {
    const real radius  = 1./colourBarCurvature;
    const real y0      = radius;
    const real theta   = (r - 0.5)*colourBarLength/(2*pi*radius);

    const real thetaOffset = 1./4.;
    x =      radius*cos(2*pi*(theta - thetaOffset));
    y = y0 + radius*sin(2*pi*(theta - thetaOffset));
    tx = -sin( 2*pi*(theta - thetaOffset));
    ty =  cos( 2*pi*(theta - thetaOffset));
    nx = -ty;
    ny = tx;
  }
  //real oldAngle = getAngle( nx, ny);
  rotateAndTranslate( colourBarAngle, colourBarCenter[0], colourBarCenter[1], x, y );
  rotateAndTranslate( colourBarAngle, 0., 0., nx, ny);
  rotateAndTranslate( colourBarAngle, 0., 0., tx, ty);
  //real normalAngle = getAngle(nx, ny), rot = normalAngle - oldAngle;
  //  printf( " oldAngle=%6.2f, nAngle=%6.2f, rot=%6.2f ",oldAngle, normalAngle, rot);
}

void   ColourBar::
stretchToScreenCoordinates( real &xa, real &ya)
{
  xa = xa*(rightSide-leftSide)/2.;
  ya = ya*(top-bottom)/2.;
}


void   ColourBar::
rotateAndTranslate( real angle, real x0, real y0, real &x, real &y)
{
  real xtang0 = cos( pi*angle/180. );
  real ytang0 = sin( pi*angle/180. );

  real xtemp  = x;
  x = x0 + xtang0*xtemp  -ytang0*y;   // rotate + translate
  y = y0 + ytang0*xtemp  +xtang0*y;
}

real  ColourBar::
getAngle( real x, real y)
{
  real theta = 360.*atan2(double(y),double(x))/(2.*pi);
  if (theta < 0.) theta += 360.;
  return theta;
}
//
// ..show `advanced' items, i.e., label angles & custom position of the colourbar etc.
// .. N.B: * all dialog processing is done in 'update()'
//         * this allows moving functionality from the standard dialog to advanced & vice versa.
//
void ColourBar::
showAdvancedDialog(const aString &prefix, GUIState &advancedGUI ) 
{
  if (!preCheck()) return;

  //advancedGUI.setExitCommand("done(advanced)", "continue");
  DialogData & dialog = (DialogData &)advancedGUI;

  {//..build the dialog
    const int maxCommands=20;
    const int numberOfColumns=1;
    //dialog.setWindowTitle("Colour Bar Advanced Options");
    //dialog.setOptionMenuColumns(numberOfColumns);
    
    //..Colour tables: these should be read from somewhere else,
    //    shouldn't be hardwired here. **pf
    //aString buttonLabels[] = {"Plot", "Pick center...", ""};
    //aString buttonCommands[], buttonCommandList[] = {"plot","pick center",  ""};
    //GUIState::addPrefix(buttonCommandList, prefix, buttonCommands, maxCommands);
    
    //const numberOfRows=1;
    //dialog.setPushButtons(buttonCommands, buttonLabels, numberOfRows); 

    real rdummy; int idummy;
    
    const int numberOfTextStrings=12;  
    aString textLabels[numberOfTextStrings];  aString textStrings[numberOfTextStrings];
    int nt=0;
    textLabels[nt] = "center (x,y)      ";  sPrintF(textStrings[nt], "%g,%g",colourBarCenter[0],colourBarCenter[1]);nt++;
    textLabels[nt] = "width             ";  sPrintF(textStrings[nt], "%g",colourBarWidth);nt++;
    textLabels[nt] = "length            ";  sPrintF(textStrings[nt], "%g",colourBarLength);nt++;
    textLabels[nt] = "offset from plot  ";  sPrintF(textStrings[nt], "%g",colourBarOffsetFromPlot);nt++;
    textLabels[nt] = "angle             ";  sPrintF(textStrings[nt], "%g",colourBarAngle);nt++;
    textLabels[nt] = "curvature ";          sPrintF(textStrings[nt], "%g",colourBarCurvature);            nt++; 

    textLabels[nt] = "label angle              ";  sPrintF(textStrings[nt], "%g",colourBarLabelAngle);       nt++; 
    textLabels[nt] = "label normal offset      ";  sPrintF(textStrings[nt], "%g",colourBarLabelNormalOffset);  nt++; 
    textLabels[nt] = "label tangential offset  ";  sPrintF(textStrings[nt], "%g",colourBarLabelTangentialOffset);nt++; 
    textLabels[nt] = "number of intervals      ";  sPrintF(textStrings[nt], "%i",colourBarNumberOfIntervals); nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    //GUIState::addPrefix(textLabels,prefix,pTextLabels,maxCommands);
    dialog.setTextBoxes( textLabels, textLabels, textStrings);
    //dialog.setTextBoxes( cmds, labels, initialValueStrings);
    //gui.buildPopup(colourBarMenu);
  }
  //const int dontWait =0; //= 1;
  //cout << "++dialog.openDialog -- advanced\n";
  //dialog.openDialog(dontWait); //open Dialog & return. Processing done by caller
  //cout << "++ _gi->pushGUI( advancedGUI )\n";
  //_gi->pushGUI( advancedGUI );
  //cout << "++ returning \n";
}

//
//  JUNK ==================================================================================
//       00 I keep old versions around to make sure I don't break the code 00
//            **pf**

//
// ..old versions
//

//utility for figuring out label orientation: vertical or horizontal

void ColourBar::
setLabelOrientation( int labelOrientation_  )
{
  if (!preCheck()) return;
  labelOrientation = (LabelOrientation)labelOrientation_;
  const real verticalAngle   =  90.;
  const real horizontalAngle =  0.;
  const real halfALabelLeft  = -0.5; 
  const real zeroLabelOffset = 0.;

  int   value;

  const bool verticalBar    = (colourBarPosition == leftColourBar)
                             || (colourBarPosition == rightColourBar);
  const bool horizontalBar  = (colourBarPosition == topColourBar)
                             || (colourBarPosition == bottomColourBar);
  real tangOffset=0.;

  if (labelOrientation == labelHorizontal) 
  {
    if (verticalBar)       tangOffset  = zeroLabelOffset;
    else if(horizontalBar) tangOffset  = halfALabelLeft;

    colourBarRelativeAngle=false;
    colourBarLabelAngle=horizontalAngle;
    colourBarLabelTangentialOffset= tangOffset;
    
  } 
  else if (labelOrientation == labelVertical)
  {
    if (verticalBar)           tangOffset  = halfALabelLeft;
    else if(horizontalBar)     tangOffset  = zeroLabelOffset;

    colourBarRelativeAngle=false;
    colourBarLabelAngle=verticalAngle;
    colourBarLabelTangentialOffset= tangOffset;

  }
}

int ColourBar::
getLabelOrientation( int &labelOrientation_ )
{
  real verySmall = 1.e-5;
  real verticalAngle = 90.; // counterclockwise in degrees
  labelOrientation = labelCustomOrientation; // default

  if( !colourBarRelativeAngle) //=absolute angle for labels
  {
    if( fabs(colourBarLabelAngle)< verySmall ) 
      labelOrientation  = labelHorizontal;
    else if (fabs(colourBarLabelAngle - verticalAngle )< verySmall ) 
      labelOrientation  = labelVertical;
  }
  labelOrientation_ = labelOrientation;
  return labelOrientation_;
}

