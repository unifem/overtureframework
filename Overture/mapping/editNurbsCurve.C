#include "editNurbsCurve.h"
#include "Point.h"
#include "ArraySimple.h"
// tmp
#include "mogl.h"

static int
plotPickingSquare(GenericGraphicsInterface & gi, ArraySimple<real> xb);

static void
erasePickingSquare(GenericGraphicsInterface & gi, int list) ;

int 
editNurbsCurve( NurbsMapping &curve, GenericGraphicsInterface& gi )
//=====================================================================================
// /Purpose: Interactively edit  a nurbs curve
// /gi (input): Holds a graphics interface to use.
//=====================================================================================
{
  const int maxPoints=100;
  Point points[maxPoints]; 
  int nPoints=0; // actual number of points
  int selectedPoint[maxPoints];
  int nSelectedPoints=0;
  int i, list=0;
  bool foundPoint;

  int status = 0;
  
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT, true);
  parameters.set(GI_USE_PLOT_BOUNDS_OR_LARGER, true);
  parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES, false);
  parameters.set(GI_PLOT_END_POINTS_ON_CURVES, true);

// turn on the axes inside this function
  bool oldPlotTheAxes = gi.getPlotTheAxes();

  ArraySimple<real> xb(2,3);
  xb(0,0) = 0;  xb(1,0) = 1;
  xb(0,1) = 0;  xb(1,1) = 1;
  xb(0,2) = 0;  xb(1,2) = 0;
  
  RealArray xBound(2,3);
  xBound=0.;
  xBound(1,nullRange)=1.;
  gi.setPlotTheBackgroundGrid(true);
  gi.setAxesDimension(2);
  gi.setPlotTheAxes(true);

  char buff[180];  // buffer for sprintf

  GUIState interface;

  interface.setWindowTitle("Edit Nurbs Curve");
  interface.setExitCommand("exit","Exit");

  MouseSelectMode mouseMode = nothing;

  aString mouseModeCommands[] = { "Mouse Mode NoOp",
				  "Mouse Mode Build Point",
				  "Mouse Mode Interpolate Curve",
				  "Mouse Mode Hide SubCurve",
//				 "Mouse Mode Delete SubCurve", 
				  "Mouse Mode Join W/Line Segment",
				  "Mouse Mode Move Curve Endpoint",
				  "Mouse Mode Snap To Intersection",
				  "Mouse Mode Split",
				  "Mouse Mode Edit SubCurve",
				  "begin curve",
				  "" };

  aString mouseModeLabels[] = { "No Operation",
				"Build Point",
				"Interpolate Curve",
				"Hide SubCurve",
//			       "Delete SubCurve",
				"Join W/Line Segment",
				"Move Curve Endpoint",
				"Snap To Intersection",
				"Split",
				"Edit SubCurve",
				"Assemble",
				"" };

  interface.addRadioBox("Mouse Picking",mouseModeCommands, mouseModeLabels, int(mouseMode), 2); // 2 columns

 // general state variables
  bool plotObject = true;
  bool plotAxes = false; // *ap*
  bool plotCurve = false; // *ap*
  bool plotOriginalCurve = false;
  bool plotAllSubcurves = true;
  bool plotControlPoints = false;
  bool autoExit=false;

  aString tbCommands[] = { "plot curve",
			   "plot original curve",
			   "plot all subcurves",
			   "plot control points",
			   "auto exit",
			   "" };
  aString tbLabels[] = { "Current Curve",
			 "Original Curve",
			 "SubCurves",
			 "Control Points",
			 "Exit on Curve Completion",
			 "" };
  int tbState[] = {plotCurve,
		   plotOriginalCurve,
		   plotAllSubcurves,
		   plotControlPoints,
		   autoExit};

  interface.setToggleButtons(tbCommands, tbLabels, tbState, 2); 

  enum pushButtons{
    clearLastPointPb=0,
    clearAllPointsPb,
    cancelPb,
    revertPb,
    autoPb,
    showAllPb,
    showUsedPb,
    hideAllPb,
    hideUnusedPb,
    showOnePb,
    numberOfPb
  };
 
  aString pbCommands[] = { 
    "clear last point",
    "clear all points",
    "cancel action",
    "undo curve",
    "auto assemble",
    "show all",
    "show used",
    "hide all",
    "hide unused",
    "show last hidden",
    "" 
  };

  aString pbLabels[]   = { 
    "Clear Last Point",
    "Clear All Points",
    "Cancel Action",
    "Revert Curve",
    "Auto Assemble",
    "Show All",
    "Show Used",
    "Hide All",
    "Hide Unused",
    "Show Last Hidden",
    "" 
  };

  interface.setPushButtons(pbCommands, pbLabels, 4); // 4 rows

  enum WindowButtons
  {
    doneWB=0,
    numberOfWB
  };
  
  aString windowButtons[][2] = {{"stop picking", "Done"},
				{"", ""}};
  interface.setUserButtons(windowButtons);
  
  SelectionInfo select;
  aString answer="", line;
  aString buf;
 

  // state variables used for endpoint move
  bool movingEndpoint = false;
  int moveCurve = -1;
  int moveEnd = Start;

  // variables for new curve assembly
  int numberOfAssembledCurves = 0;
  NurbsMapping **assemblyCurves = NULL;
  NurbsMapping newCurve, oldCurve;
  oldCurve = curve;
  bool curveRebuilt = false;

  // state variables for intersection calculation
  int curve1=-1, curve2=-1;
  int curve1End=0, curve2End=0;
  real c1click[2];

  // line segment state variables
  int nLineSegmentPoints = 0;
  RealArray linePts1(1,2),linePts2(1,2);
  linePts1 = linePts2 = 0.0;

  // point plotting state
  realArray plotpts;

// show hidden subcurve
  int hiddenSubCurve=-1;

  gi.pushGUI(interface);

// reset the viewing matrix
  gi.initView(gi.getCurrentWindow());

  RadioBox & rBox = interface.getRadioBox(0);

  for( int it=0;; it++ )
  {
// tmp way of making user button 0 insensitive
    gi.setUserButtonSensitive(doneWB, mouseMode == interpolateCurve); 

// set the sensitivity of the GUI
   if (mouseMode == curveAssembly)
   {
     rBox.setSensitive(nothing, false);
     rBox.setSensitive(lineSegmentJoin, false);
     rBox.setSensitive(endpointMove, false);
     rBox.setSensitive(intersection, false);
     rBox.setSensitive(split,false);
     rBox.setSensitive(updateCurve, false);
     //interface.setSensitive(true,  DialogData::pushButtonWidget, cancelPb);  // cancel pb
     interface.setSensitive(false, DialogData::pushButtonWidget, revertPb);// revert pb
   }
   else
   {
     interface.setSensitive(true, DialogData::radioBoxWidget, 0); // radio box active
     interface.setSensitive(true, DialogData::pushButtonWidget, cancelPb);// cancel pb
     interface.setSensitive(true, DialogData::pushButtonWidget,  revertPb);// revert pb
     interface.setSensitive(hiddenSubCurve >= curve.numberOfSubCurves() && 
			    hiddenSubCurve < curve.numberOfSubCurvesInList(), 
			    DialogData::pushButtonWidget,  showOnePb);// show one subcurve
     interface.setSensitive((nSelectedPoints > 0), DialogData::pushButtonWidget, clearLastPointPb);
     interface.setSensitive((nSelectedPoints > 0), DialogData::pushButtonWidget, clearAllPointsPb);
   }
     
     
   if( it==0 && plotObject )
     answer="plotObject";
   else
   {
     gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

     gi.getAnswer(answer, "", select);

     gi.savePickCommands(true); // restore
   }
   
   int len;
   if ( (len = answer.matches("Mouse Mode")) )
   {
     aString mode= (answer.length() > len+2)? answer(len+1,answer.length()-1): (aString)"";
	 
     if (mode.matches("NoOp"))
       mouseMode = nothing;
     else if (mode.matches("Build Point"))
       mouseMode = buildPoint;
     else if (mode.matches("Interpolate Curve"))
       mouseMode = interpolateCurve;
     else if (mode.matches("Hide SubCurve"))
       mouseMode = hideCurve;
     else if (mode.matches("Join W/Line Segment"))
       mouseMode = lineSegmentJoin;
     else if (mode.matches("Move Curve Endpoint"))
       mouseMode = endpointMove;
     else if (mode.matches("Snap To Intersection"))
       mouseMode = intersection;
     else if (mode.matches("Split"))
       mouseMode = split;
     else if (mode.matches("Edit SubCurve"))
       mouseMode = updateCurve;
     else
       gi.outputString(sPrintF(buf,"Unknown mouse mode: `%s'", SC mode));
	 
     if ( mouseMode>=0 && mouseMode < numberOfMouseModes )
     {
       if ( !rBox.setCurrentChoice(mouseMode) )
       {
	 aString buf;
	 sPrintF(buf,"ERROR : selection %d is inactive", mouseMode);
	 gi.outputString(buf);
       }
     }
     else
     {
       aString errbuff;
       sPrintF(errbuff, "ERROR : invalid mouse mode %d", int(mouseMode));
       gi.createMessageDialog(errbuff, errorDialog);
       gi.outputString(errbuff);
       mouseMode = nothing;
     }

   }
   else if ( answer(0,9)=="plot curve" )
   {
     plotCurve = !plotCurve;
     interface.setToggleState(0, plotCurve);
     //interface.setSensitive((plotOriginalCurve||plotCurve), DialogData::toggleButtonWidget, 3);
   }
   else if ( answer(0,8)=="auto exit" )
   {
     autoExit = !autoExit;
     interface.setToggleState(4,autoExit);
   }
   else if ( answer(0,18) == "plot original curve" )
   {
     plotOriginalCurve = !plotOriginalCurve;
     interface.setToggleState(1, plotOriginalCurve);
     //interface.setSensitive((plotOriginalCurve||plotCurve), DialogData::toggleButtonWidget, 3);
   }
   else if ( answer(0,17) == "plot all subcurves" )
   {
     plotAllSubcurves = !plotAllSubcurves;
     interface.setToggleState(2,plotAllSubcurves);
   }
   else if ( answer(0,18)=="plot control points")
   {
     plotControlPoints = !plotControlPoints;
     interface.setToggleState(3, plotControlPoints);
   }
   else if( len=answer.matches("snap to intersection") )
   {
     // int curve1,curve2,curve1End,curve2End;
     real c2click[2];
     sScanF(answer(len,answer.length()-1),"%i %i %i %i %e %e %e %e",&curve1,&curve2,&curve1End,&curve2End,
	    &c2click[0],&c2click[1],&c1click[0],&c1click[1]);

     printf(" snap: curve1=%i, curve2=%i, curve1End=%i, curve2End=%i\n",curve1,curve2,curve1End,curve2End);


     if( curve1<0 || curve2<0 )
     {
       gi.outputString("Invalid curves for snap to intersection");
       gi.stopReadingCommandFile();
     }
     else 
     {
       int status =snapCurvesToIntersection(gi,curve,curve1,curve2,curve1End,curve2End,
					    c2click,c1click);
     }
       
   }
   else if( len=answer.matches("hide curve") )
   {
     int c=-1;
     sScanF(answer(len,answer.length()-1),"%i",&c);
     if( c>=0 && c<curve.numberOfSubCurves() )
       hiddenSubCurve = curve.toggleSubCurveVisibility(c);
   }
   else if( len=answer.matches("join with line segment") )
   {
     int c=-1;
     sScanF(answer(len,answer.length()-1),"%e %e %e %e",&linePts1(0,0),&linePts1(0,1),&linePts2(0,0),&linePts2(0,1));

     NurbsMapping newLine;// = new NurbsMapping(1,2);
     newLine.line(linePts1, linePts2);
     bool added = false;
     curve.addSubCurve(newLine);
   }
   else if( len=answer.matches("move end point") )
   {
     RealArray xa(2);
     sScanF(answer(len,answer.length()-1),"%i %i %e %e",&moveCurve,&moveEnd,&xa(0),&xa(1));
     if( moveCurve>=0 && moveCurve<curve.numberOfSubCurves() && moveEnd>=0 && moveEnd<=1 )
     {
       curve.subCurveFromList(moveCurve).moveEndpoint(moveEnd, xa);
       movingEndpoint=false;
     }
     else
     {
       gi.outputString("Invalid arguments to `move end point'");
     }
   }
   else if( len=answer.matches("split curve") )
   {
     int crv;
     real rp;    // split curve at this r value
     sScanF(answer(len,answer.length()-1),"%i %e",&crv,&rp);
     if( crv>=0 && crv<curve.numberOfSubCurves() && rp<1.0 && rp>0.0 )
     {
       NurbsMapping *c1 = new NurbsMapping; c1->incrementReferenceCount();
       NurbsMapping *c2 = new NurbsMapping; c2->incrementReferenceCount();
       if ( (curve.subCurveFromList(crv).split(rp, *c1, *c2))==0 )
       {
	 curve.toggleSubCurveVisibility(crv);
	 curve.addSubCurve(*c1);
	 curve.addSubCurve(*c2);
       }
       else
	 gi.createMessageDialog("unknown error : cannot split the curve!", errorDialog);
       
       if( c1->decrementReferenceCount()==0 ) delete c1;
       if( c2->decrementReferenceCount()==0 ) delete c2;
       
     }
     else
     {
       gi.outputString("Invalid arguments to `split curve'");
     }
   }
   else if( len=answer.matches("assemble curve") )
   {
     int crv;
     sScanF(answer(len,answer.length()-1),"%i",&crv);
     if( crv>=0 && crv<curve.numberOfSubCurves() )
     {
       int status=assembleSubCurves(crv,
				    gi, 
				    curve,
				    newCurve,
				    numberOfAssembledCurves,
				    assemblyCurves,
				    mouseMode,
				    curveRebuilt,
				    plotCurve );

       if( curveRebuilt )
	 interface.setToggleState(0, plotCurve);
       if ( curveRebuilt && autoExit )  // exit to calling function directly!
	 break;
     }
     else
     {
       gi.outputString("Invalid arguments to `assemble curve'");
     }
   }
   else if( answer.matches("clear last point") )
   {
     if (nSelectedPoints > 0)
       nSelectedPoints--;
   }
   else if( answer.matches("clear all points") )
   {
     nSelectedPoints = 0;
   }
   else if (answer == "stop picking")
   {
     if (nSelectedPoints>=2)
     {
       NurbsMapping * subCurve_ = new NurbsMapping;
       subCurve_->incrementReferenceCount();
	
       realArray x(nSelectedPoints,2);
// fill in the coordinates
       for (i=0; i<nSelectedPoints; i++)
       {
	 x(i,0) = points[selectedPoint[i]].coordinate[0];
	 x(i,1) = points[selectedPoint[i]].coordinate[1];
//	 x(i,2) = points[selectedPoint[i]].coordinate[2];
       }
       subCurve_->interpolate(x);
// set a reasonable number of points
       subCurve_->setGridDimensions(axis1,max(nSelectedPoints,21));  
       if (curve.isInitialized())
	 curve.merge(*subCurve_);
       else
	 curve = *subCurve_;
       
// clear all points
       nSelectedPoints = 0;
     }
     else
     {
       if (nSelectedPoints<=1)
	 gi.createMessageDialog("You need at least 2 points to build a curve.",
				errorDialog);
     }
   }
   else if( select.nSelect>0 && mouseMode == buildPoint )
   {
     if (nPoints<maxPoints)
     {
       for (i=0; i<2; i++)
	 points[nPoints].coordinate[i] = select.x[i];
       points[nPoints].coordinate[2] = 0.0;

       gi.outputString(sPrintF(buf, "Point #%d: (%g, %g)", nPoints, 
			       points[nPoints].coordinate[0], 
			       points[nPoints].coordinate[1]));
       gi.erase(); // erase the old points
// save equivalent command
       gi.outputToCommandFile(sPrintF(buf, "newPoint %e %e\n", points[nPoints].coordinate[0], 
				      points[nPoints].coordinate[1]));
       nPoints++;
     }
     else
     {
       gi.createMessageDialog("Not enough space for another point. Clear the existing points before proceeding",
			      errorDialog);
       continue;
     }
   }
   else if( select.nSelect>0 && mouseMode == interpolateCurve )
   {
     foundPoint = false;
     for ( int s=0; s<select.nSelect && !foundPoint; s++ )
       for ( i=0; i<nPoints && !foundPoint; i++ )
	 if ( points[i].getGlobalID()==select.selection(s,0) )
	 {
	   selectedPoint[nSelectedPoints++] = i;
	   gi.outputString(sPrintF(buf,"Selected point #%i", i));
	 }
   }
   // ----------------------- mouse selection -------------------------------
   else if ( select.active && select.nSelect>0 )
   {
     real x[] = { 0.0, 0.0 };

     realArray X(1,2), r(2,1), xm(2,2);
     Range AXES(2);
     int end;
     int selectedCurve=-1;
     bool foundcurve=false;
     if ( mouseMode!=nothing )
     {
       // snap the selection to the nearest endpoint of the selected curve
       X(0,0) = select.x[0];
       X(0,1) = select.x[1];
       r(0,0) = 0.0;
       r(1,0) = 1.0;

       foundcurve = false;
       for ( int s=0; s<select.nSelect && !foundcurve; s++ )
	 for ( int sc=0; sc<curve.numberOfSubCurves() && !foundcurve; sc++ )
	   if ( curve.subCurve(sc).getGlobalID()==select.selection(s,0) )
	   {
	     curve.subCurveFromList(sc).reparameterize(0.,1.); // why? kkc
	     curve.subCurveFromList(sc).map(r,xm);
	     foundcurve = true;
	     selectedCurve = sc;
	     real dist1 = sum(pow(xm(0,AXES)-X(0,AXES),2));
	     real dist2 = sum(pow(xm(1,AXES)-X(0,AXES),2));
	     if ( dist1<dist2 )
	     {
	       x[0] = xm(0,0);
	       x[1] = xm(0,1);
	       end = Start;
	     }
	     else
	     {
	       x[0] = xm(1,0);
	       x[1] = xm(1,1);
	       end = End;
	     }
	   }
       X(0,0) = x[0];
       X(0,1) = x[1];
     }

     if ( mouseMode==hideCurve && foundcurve )
     {
       if ( (select.r[1]-select.r[0])>FLT_MIN &&
	    (select.r[3]-select.r[2])>FLT_MIN )
       {
	 for ( int s=0; s<select.nSelect; s++ )
	   for ( int sc=0; sc<curve.numberOfSubCurvesInList(); sc++ )
	     if ( curve.subCurveFromList(sc).getGlobalID() == select.selection(s,0) )
	     {
	       hiddenSubCurve = curve.toggleSubCurveVisibility(sc);
	       gi.outputToCommandFile(sPrintF(line,"hide curve %i\n",sc));
	       break;
	     }
       }
       else
       {
	 if ( !curve.isSubCurveHidden(selectedCurve) )
	 {
	   hiddenSubCurve =curve.toggleSubCurveVisibility(selectedCurve);
	   gi.outputToCommandFile(sPrintF(line,"hide curve %i\n",selectedCurve));
	 }
       }
     }
//  	 else if ( mouseMode==deleteSubCurve && foundcurve )
//  	   {
//  	     if ( (select.r[1]-select.r[0])>FLT_MIN &&
//  		  (select.r[3]-select.r[2])>FLT_MIN )
//  	       {
//  		 for ( int s=0; s<select.nSelect; s++ )
//  		   for ( int sc=0; sc<curve.numberOfSubCurvesInList(); sc++ )
//  		     if ( curve.subCurveFromList(sc).getGlobalID() == select.selection(s,0) )
//  		       {
//  			 curve.deleteSubCurve(sc);
//  			 break;
//  		       }
//  	       }
//  	     else
//  	       if ( curve.subCurveFromList(selectedCurve).getGlobalID() == select.selection(0,0) )
//  		 {
//  		   curve.deleteSubCurve(selectedCurve);
//  		 }
//  	   }
     else if ( mouseMode==lineSegmentJoin && foundcurve)
     {
       // now begin ( or end ) the line segment
       if ( foundcurve && nLineSegmentPoints == 0 )
       {
	 linePts1(0,0) = x[0];
	 linePts1(0,1) = x[1];
	 plotpts = linePts1;
	 nLineSegmentPoints++;
       }
       else if ( foundcurve && nLineSegmentPoints == 1 )
       {
	 linePts2(0,0) = x[0];
	 linePts2(0,1) = x[1];
	 if ( max(fabs(linePts2-linePts1))<FLT_EPSILON )
	 {
	   gi.createMessageDialog("points for line segment are too close together!", errorDialog);
	 }
	 else
	 {
	   NurbsMapping newLine;// = new NurbsMapping(1,2);
	   newLine.line(linePts1, linePts2);
	   bool added = false;
	   curve.addSubCurve(newLine);

           gi.outputToCommandFile(sPrintF(line,"join with line segment %e %e %e %e\n",linePts1(0,0),linePts1(0,1),
                       linePts2(0,0),linePts2(0,1)));
	 }
	 nLineSegmentPoints = 0;
	 linePts1 = linePts2 = 0.0;
	 plotpts.redim(0);
       }
     }
     else if ( mouseMode == curveAssembly )
     {
      int crv=-1;
      for ( int sc=0; sc<curve.numberOfSubCurvesInList(); sc++ )
	if ( curve.subCurveFromList(sc).getGlobalID()==select.selection(0,0) )
	{
	  crv=sc;
	  break;
	}

      if( crv>=0 )
      {
	int status=assembleSubCurves(crv,
				     gi, 
				     curve,
				     newCurve,
				     numberOfAssembledCurves,
				     assemblyCurves,
				     mouseMode,
				     curveRebuilt,
				     plotCurve );

        if( status==0 )
          gi.outputToCommandFile(sPrintF(line,"assemble curve %i\n",crv));

        if( curveRebuilt )
          interface.setToggleState(0, plotCurve);
	if ( curveRebuilt && autoExit )  // exit to calling function directly!
	  break;
      }
      
     }
     else if ( mouseMode==endpointMove )
     {
       if ( !movingEndpoint && foundcurve)
       {
	 plotpts.redim(1,2);
	 plotpts(0,0) = x[0];
	 plotpts(0,1) = x[1];
	 moveCurve = selectedCurve;
	 moveEnd = end;
	 movingEndpoint = true;
       }
       else
       {
	 X(0,0) = x[0];
	 X(0,1) = x[1];
	 X.reshape(2);
	 curve.subCurveFromList(moveCurve).moveEndpoint(moveEnd, X);
	 movingEndpoint=false;
	 X.reshape(1,2);

         gi.outputToCommandFile(sPrintF(line,"move end point %i %i %e %e \n",moveCurve,moveEnd,x[0],x[1]));

	 plotpts.redim(0);
       }
     }
     else if ( mouseMode==intersection && foundcurve )
     {
       if ( curve1==-1 )
       {
	 c1click[0] = select.x[0];
	 c1click[1] = select.x[1];
	 curve1 = selectedCurve;
	 curve1End = end;
       }
       else if ( curve1!=selectedCurve )
       {
	 curve2 = selectedCurve;
	 curve2End = end;

// save values for the outputToCommandFile, since these values might get changed inside snapCurvesToIntersection
	 int c1=curve1, c2=curve2;
	 int status =snapCurvesToIntersection(gi,curve,curve1,curve2,curve1End,curve2End,
					      select.x,c1click);
	 if( status==0 )
	   gi.outputToCommandFile(sPrintF(line,"snap to intersection %i %i %i %i %e %e %e %e \n",
					  c1, c2, curve1End, curve2End, select.x[0], select.x[1],
					  c1click[0], c1click[1]));
	 plotpts.redim(0);
       }
       else if ( curve1==selectedCurve && curve1!=-1 )
       {
	 gi.createMessageDialog("cannot intersect a curve with itself!", errorDialog);
       }
     }
     else if ( mouseMode==split && foundcurve )
     {
       realArray x(1,2), r(1,1);
       x(0,0) = select.x[0];
       x(0,1) = select.x[1];

       curve.subCurveFromList(selectedCurve).inverseMap(x,r);

       NurbsMapping *c1 = new NurbsMapping; c1->incrementReferenceCount();
       NurbsMapping *c2 = new NurbsMapping; c2->incrementReferenceCount();
       if ( r(0,0)<1.0 && r(0,0)>0.0 )
       {
	 if ( (curve.subCurveFromList(selectedCurve).split(r(0,0), *c1, *c2))==0 )
	 {
	   curve.toggleSubCurveVisibility(selectedCurve);
	   curve.addSubCurve(*c1);
	   curve.addSubCurve(*c2);

	   gi.outputToCommandFile(sPrintF(line,"split curve %i %e \n",selectedCurve,r(0,0)));
	   
	 }
	 else
	   gi.createMessageDialog("unknown error : cannot split the curve!", errorDialog);

       }
       else
	 gi.createMessageDialog("cannot split a curve past its endpoints!", errorDialog);

       if( c1->decrementReferenceCount()==0 ) delete c1;
       if( c2->decrementReferenceCount()==0 ) delete c2;

     }
     else if ( mouseMode == updateCurve && foundcurve )
     {
       gi.erase();
       MappingInformation mapInfo;
       mapInfo.graphXInterface = &gi;
       curve.subCurveFromList(selectedCurve).update(mapInfo);
     }
   }
   else if ( answer=="auto assemble" )
   {
     numberOfAssembledCurves = 0;
     mouseMode = curveAssembly;
     if (!assemblyCurves)
     {
       assemblyCurves = new NurbsMapping*[curve.numberOfSubCurvesInList()];
       for ( int c=0; c<curve.numberOfSubCurvesInList(); c++ ) assemblyCurves[c] = NULL;
     }

     if ( curve.numberOfSubCurvesInList()>0 )
     {
       int sc=0;
       numberOfAssembledCurves=0;
	     
       NurbsMapping **tempList = new NurbsMapping *[curve.numberOfSubCurvesInList()];

       bool completedCurve =false;

       for ( sc=0; sc<curve.numberOfSubCurvesInList() && !completedCurve; sc++ ) 
       {
	 for ( int c=0; c<curve.numberOfSubCurvesInList(); c++ ) tempList[c] = &curve.subCurveFromList(c);
		 
	 if ( !curve.isSubCurveHidden(sc) )
	 {
	   numberOfAssembledCurves = 0;
	   for ( int c=0; c<curve.numberOfSubCurvesInList(); c++ ) assemblyCurves[c] = NULL;

	   //NurbsMapping &tempNewCurve = newCurve;
	   //tempNewCurve= *currentSubCurves[sc];
	   newCurve= curve.subCurveFromList(sc);
	   tempList[sc] = NULL;
	   assemblyCurves[numberOfAssembledCurves++] = &curve.subCurveFromList(sc);
	   bool merged = true;
	   completedCurve = (newCurve.getIsPeriodic(axis1)!=Mapping::notPeriodic);
	   int ntries = 0;
	   while ( !completedCurve && ntries<(curve.numberOfSubCurvesInList()+1) )
	   {
	     for ( int scj=0; scj<curve.numberOfSubCurvesInList() && !completedCurve; scj++ )
	     {
			     
	       if ( tempList[scj]!=NULL && scj!=sc && !curve.isSubCurveHidden(scj) )
	       {
		 //merged = (tempNewCurve.merge(*tempList[scj], false)==0);
		 merged = (newCurve.merge(*tempList[scj], false)==0);
		 if ( merged ) 
		 {
		   assemblyCurves[numberOfAssembledCurves++] = tempList[scj];
		   tempList[scj] = NULL;
				     
		   //completedCurve = (tempNewCurve.getIsPeriodic(axis1)!=Mapping::notPeriodic);
		   //completedCurve = (newCurve.getIsPeriodic(axis1)!=Mapping::notPeriodic);
		   completedCurve = numberOfAssembledCurves==curve.numberOfSubCurves();
		 }
	       }
	     }
	     ntries++;
	   }
	   //if ( completedCurve ) 
	   //{
	   //newCurve = tempNewCurve;
	   //real area, arcLength, scale0, scale1;
	   //getAreaAndArcLength( tempNewCurve, area, arcLength, scale0, scale1 );
	   //if ( completedCurve )
	   //newCurve = tempNewCurve;
	   // }
		     
	 }
       }
       delete [] tempList;
	     
       if ( completedCurve ) 
       {

	 // *wdh* 010901 real area, arcLen, scale0, scale1;
	 // *wdh* 010901 getAreaAndArcLength( newCurve, area, arcLen, scale0, scale1 );
	 //         real area=getArea(newCurve);

	 if ( //!verifyTrimCurve((Mapping *)&newCurve) || 
		   numberOfAssembledCurves<curve.numberOfSubCurves() )
	 {
	   completedCurve = false;
	   aString buff;
	   bool curveok;
// note that we have not included all subcurves in newCurve at this point
	   if (numberOfAssembledCurves<curve.numberOfSubCurves())
	     buff = "ASSEMBLY FAILED : problems with new curve : there are unused subcurves";
	   // else
	   //  buff = "ASSEMBLY FAILED : problems with new curve : "+reportTrimCurveInfo( (Mapping*)&newCurve, curveok);
	   buff += "\ngoing manual...";
	   gi.createMessageDialog(buff, errorDialog);
	 }
	 else 
	 {
// add in all the remaining subcurves to newCurve
	   if ( numberOfAssembledCurves<curve.numberOfSubCurvesInList() )
	   {
	     bool used = false;
	     for ( int sc=0; sc<curve.numberOfSubCurvesInList(); sc++ )
	     {
	       used = false;
	       for ( int as=0; as<numberOfAssembledCurves && !used; as++ )
		 if ( curve.subCurveFromList(sc).getGlobalID()==assemblyCurves[as]->getGlobalID() )
		   used = true;
			     
	       if ( !used ) 
	       {
		 int nc = newCurve.addSubCurve(curve.subCurveFromList(sc));
		 if ( curve.isSubCurveHidden(sc) ) newCurve.toggleSubCurveVisibility(nc);
	       }
	     }
	   }
	   delete [] assemblyCurves;
	   assemblyCurves  = NULL;
	   numberOfAssembledCurves = 0;
	   mouseMode = nothing;
	   curve = newCurve;   // deep copy

	   // ::display(curve.getGrid(),"grid for curve","%3.1f");
           // PlotIt::plot(gi,curve);  // *************

	   curveRebuilt = true;
	   plotCurve = true;
	   interface.setToggleState(0, plotCurve);
	   gi.outputString("Curve is complete and appears to be valid!");
	   // AP: exit to calling function directly!
	   if ( autoExit ) break;
	 }
       }
       else
       {
	 gi.createMessageDialog( "could not automatically assemble curves, going manual...", warningDialog);
       }
     }
     else
       gi.createMessageDialog("there are no subcurves!", errorDialog);
   }
//                            01234567890123456789
   else if ( answer(0,10)=="begin curve" )
   {
     numberOfAssembledCurves = 0;
     mouseMode = curveAssembly;
     if ( !rBox.setCurrentChoice(mouseMode) )
     {
       aString buf;
       sPrintF(buf,"ERROR : selection %d is inactive", mouseMode);
       gi.outputString(buf);
     }
   }
   else if ( answer=="cancel action" )
   {
     plotpts.redim(0);
     if ( mouseMode==lineSegmentJoin )
     {
       nLineSegmentPoints = 0;
     }
     else if ( mouseMode == endpointMove )
     {
       movingEndpoint = false;
     }
     else if ( mouseMode == intersection )
     {
       curve1=curve2=-1;
     }
     else if ( mouseMode==curveAssembly )
     {
       delete [] assemblyCurves;
       assemblyCurves  = NULL;
       numberOfAssembledCurves = 0;
     }
     interface.setSensitive(true, DialogData::radioBoxWidget, 0); 
     interface.setSensitive(true, DialogData::pushButtonWidget, cancelPb);// cancel pb
     interface.setSensitive(true, DialogData::pushButtonWidget,  revertPb);// revert pb
     mouseMode = nothing;
     if ( !rBox.setCurrentChoice(mouseMode)) 
     {
       gi.outputString("the mouse mode is not active");
     }
   }
   else if ( answer=="undo curve" )
   {
     if ( numberOfAssembledCurves!=0 )
     {
       delete [] assemblyCurves;
       assemblyCurves  = NULL;
       numberOfAssembledCurves = 0;
     }
     curve = oldCurve;
// reset the other modification states
     movingEndpoint = false;
     curve1 = curve2 = -1;
     nLineSegmentPoints = 0;

   }//                      01234567890123456789
   else if ( answer.matches("show all") )
   {
     bool curveWasHidden;
     do
     {
       curveWasHidden = false;
       for ( int c=0;c<curve.numberOfSubCurvesInList(); c++ ) 
       {
	 if ( curve.isSubCurveHidden(c) ) 
	 {
	   curve.toggleSubCurveVisibility(c);
	   curveWasHidden = true;
	   break;
	 }
       }
     } while (curveWasHidden);
   }
   else if ( answer.matches("hide all") )
   {
// need to go backwards because elements are moved forwards in list when they are shown
     for ( int c=curve.numberOfSubCurvesInList()-1;c>=0; c--) 
     {
       if ( !curve.isSubCurveHidden(c) ) curve.toggleSubCurveVisibility(c);
     }
   }
   else if ( answer.matches("hide unused") )
   {
// we need to iterate since the list is reorganized every time an element is hidden
     bool curveWasHidden;
     do
     {
       curveWasHidden = false;
       for ( int c=0; c<curve.numberOfSubCurves(); c++)  // only loop over the visible sub curves
       {
	 if ( !curve.isSubCurveOriginal(c) && !curve.isSubCurveHidden(c) ) 
	 {
	   printf("Hiding subcurve %i with globalID=%i\n", c, curve.subCurve(c).getGlobalID());
	   curve.toggleSubCurveVisibility(c); // hide the subcurve
	   curveWasHidden = true;
	   break; // no point continuing since the list is now re-ordered
	 }
       }
     } while(curveWasHidden);
     
   }
   else if ( len=answer.matches("show last hidden") )
   {
     if (hiddenSubCurve >= curve.numberOfSubCurves() && 
	 hiddenSubCurve < curve.numberOfSubCurvesInList())
     {
       curve.toggleSubCurveVisibility(hiddenSubCurve);
       hiddenSubCurve = -1;
     }
   }
   else if ( answer.matches("show used") )
   {
// we need to iterate since the list is reorganized every time an element is made visible
     bool curveWasHidden;
     do
     {
       curveWasHidden = false;
       for ( int c=0; c<curve.numberOfSubCurvesInList(); c++)  // loop over all sub curves
       {
	 if ( curve.isSubCurveOriginal(c) && curve.isSubCurveHidden(c) )
	 {
	   curve.toggleSubCurveVisibility(c); // make it visible
	   curveWasHidden = true;
	   break; // no point continuing since the list is now re-ordered
	 }
       }
     } while(curveWasHidden);
   }
   else if ( answer=="exit" )
   {
     break;
   }
   else if ( answer == "plotObject" )
   {
     plotObject=true;
   }
   else if ( !select.active )
   {
     gi.outputString("could not understand command : "+answer);
   }

   if ( plotObject )
   {
     gi.erase();
     gi.setGlobalBound(xBound);

// draw the white plane onto which all points are picked
     if (mouseMode == buildPoint)
     {
       erasePickingSquare(gi, list);
       list = plotPickingSquare(gi, xb);
     }
     
     aString mapc = parameters.getMappingColour();

// plot the points
     parameters.set(GI_POINT_SIZE, 4);
     parameters.set(GI_POINT_COLOUR, "blue");
     for (i=0; i<nPoints; i++)
       points[i].plot(gi, parameters);

// plot the selected points
     parameters.set(GI_POINT_SIZE, 6);
     parameters.set(GI_POINT_COLOUR, "red");
     for (i=0; i<nSelectedPoints; i++)
       points[selectedPoint[i]].plot(gi, parameters);

     parameters.set(GI_POINT_SIZE, 4); // the size of endpoints is multiplied by 1.5
     parameters.set(GI_POINT_COLOUR, "black");
     if ( plotCurve )
       curve.plot( gi, parameters, plotControlPoints );
	 
     parameters.set(GI_MAPPING_COLOUR, "blue");
     if ( plotOriginalCurve ) 
       oldCurve.plot( gi, parameters, plotControlPoints);
     parameters.set(GI_MAPPING_COLOUR, mapc);

     if ( plotAllSubcurves ) 
     {
	     
       real linew;
       for ( int sc=0; sc<curve.numberOfSubCurves(); sc++ )
	     
       {
// tmp
//  	 printf("Plotting subcurve %i with globalID=%i, original=%i\n", sc, 
//  		curve.subCurve(sc).getGlobalID(), curve.isSubCurveOriginal(sc));

	 parameters.set(GI_MAPPING_COLOUR, gi.getColourName(sc));
		 
	 if ( sc==curve1 )
	 {
	   parameters.get(GraphicsParameters::curveLineWidth, linew);
	   parameters.set(GraphicsParameters::curveLineWidth, linew*3);
	 }

//		 PlotIt::plot(gi,*currentSubCurves[sc], parameters);
	 if ( !curve.isSubCurveHidden(sc) )
	   curve.subCurve(sc).plot( gi, parameters, plotControlPoints );

	 if ( sc==curve1 )
	   parameters.set(GraphicsParameters::curveLineWidth, linew);
		 
       }
       parameters.set(GI_MAPPING_COLOUR, mapc);
     }
	 
     if ( mouseMode == curveAssembly )
     {
       parameters.set(GI_MAPPING_COLOUR, "red");
       real linew;
       parameters.get(GraphicsParameters::curveLineWidth, linew);
       parameters.set(GraphicsParameters::curveLineWidth, 2*linew);
       if ( numberOfAssembledCurves>0 )
	 PlotIt::plot(gi,newCurve, parameters);

       parameters.set(GraphicsParameters::curveLineWidth, linew);
       parameters.set(GI_MAPPING_COLOUR, mapc);
     }

     if ( plotpts.getLength(0)>0 )
       gi.plotPoints(plotpts, parameters);
   }
 }

// erase the curves
 gi.erase();
 gi.popGUI();

// reset plotTheAxes
 gi.setPlotTheAxes(oldPlotTheAxes);

 return status;
}


int 
snapCurvesToIntersection(GenericGraphicsInterface & gi, 
                         NurbsMapping & curve, 
			 int &curve1, int &curve2, 
                         int curve1End, int curve2End,
                         const real *xSelect,
                         const real *c1click )
// =======================================================================================================
// /Access:
//     This is a protected function.
// /Description:
//     Join to curves where they intersect.
// =======================================================================================================
{
  int returnValue=0;
  
  IntersectionMapping intersect;
  int numberOfIntersectionPoints=0;
  realArray localIntersection;
  realArray rmap1, rmap2;

  bool parallel = intersect.intersectCurves(curve.subCurveFromList(curve1), curve.subCurveFromList(curve2), 
					       numberOfIntersectionPoints, rmap1, rmap2, 
					       localIntersection)==-1;

  if ( parallel )
    gi.createMessageDialog("the curves appear to be parallel!", errorDialog);
  else
  {
    int nIntersections = numberOfIntersectionPoints + 1;
    realArray inter(nIntersections,2);
    int in =0;
    for ( int i=0; i<numberOfIntersectionPoints; i++ )
    {
      inter(in, 0)= localIntersection(0,i);
      inter(in++,1) = localIntersection(1,i);
    }

    // now add the intersection point determined by the selected curve endpoints
    realArray r1(1,1), r2(1,1), x1(1,2), x2(1,2), xr1(1,2), xr2(1,2);
    r1 = real(curve1End);
    r2 = real(curve2End);
    curve.subCurveFromList(curve1).map(r1,x1,xr1);
    curve.subCurveFromList(curve2).map(r2,x2,xr2);
		     
    if ((1.0-fabs(sum(xr1*xr2)/sqrt(sum(pow(xr1,2))*sum(pow(xr2,2)))))>FLT_EPSILON )
    {
      real dxb1 = xr1(0,0)*x1(0,1) - xr1(0,1)*x1(0,0);
      real dxb2 = xr2(0,0)*x2(0,1) - xr2(0,1)*x2(0,0);
			 
      inter(in,0) = (xr2(0,0)*dxb1-xr1(0,0)*dxb2)/(xr1(0,0)*xr2(0,1)-xr2(0,0)*xr1(0,1));
      if ( fabs(xr1(0,0))>10*REAL_MIN )
	inter(in,1) = (xr1(0,1)*inter(in,0) + dxb1)/xr1(0,0);
      else
	inter(in,1) = (xr2(0,1)*inter(in,0) + dxb2)/xr2(0,0);
			 
    }
    else
    {
      nIntersections--;
    }

    if ( nIntersections!=0 )
    {
      Range AXES(2);

      realArray interUsed(1,2);
      // choose an appropriate intersection to use
      //   use the intersection closest to the mouse selection
      real mindist = REAL_MAX;
      int useinter = -1;
      real dist;
      for ( int i=0; i<nIntersections; i++ )
      {
	// *wdh* dist = (select.x[0]-inter(i,0))*(select.x[0]-inter(i,0)) +
	dist = (xSelect[0]-inter(i,0))*(xSelect[0]-inter(i,0)) +
	  (xSelect[1]-inter(i,1))*(xSelect[1]-inter(i,1));
	
	if ( dist<mindist )
	  {
	    mindist = dist;
	    useinter = i;
	  }
      }

      if ( useinter>-1 )
      {
	interUsed(0,AXES) = inter(useinter,AXES);
	realArray r(1,1), rclick(1,1), xclick(1,2);
	curve.subCurveFromList(curve2).inverseMap(interUsed, r);
			     
	xclick(0,0) = xSelect[0];
	xclick(0,1) = xSelect[1];
	curve.subCurveFromList(curve2).inverseMap(xclick, rclick);
			     
	if ( rclick(0,0)<r(0,0) )
	  curve2End = End;
	else
	  curve2End = Start;
			     
	xclick(0,0) = c1click[0];
	xclick(0,1) = c1click[1];
	curve.subCurveFromList(curve1).inverseMap(interUsed,r);
	curve.subCurveFromList(curve1).inverseMap(xclick,rclick);
			     
	if ( rclick(0,0)<r(0,0) )
	  curve1End = End;
	else
	  curve1End = Start;
			     
      }
      else
	curve1 = curve2 = -1;
			 
			 		     
      if ( curve1==-1 || curve2 == -1 )
      {
        returnValue=1;
	cout<<"NO VALID CURVES"<<endl;
	gi.createMessageDialog("intersection failed, try clicking closer to the intended intersection!", errorDialog);
      }
      else
      {
	// save the original curves, hidden, then create the new curves
	NurbsMapping c1,c2;
	c1 = curve.subCurveFromList(curve1);
	c2 = curve.subCurveFromList(curve2);
	curve.toggleSubCurveVisibility(max(curve1,curve2));
	curve.toggleSubCurveVisibility(min(curve1,curve2)); 
	c1.moveEndpoint(curve1End, interUsed);
	c2.moveEndpoint(curve2End, interUsed);
	curve.addSubCurve(c1);
	curve.addSubCurve(c2);

      }
    }
    else
      {
	cout<<"NO INTERSECTIONS"<<endl;
	gi.createMessageDialog("intersection failed, try clicking closer to the intended intersection!", errorDialog);
		     
      }
  }
  curve1 = curve2 = -1;
// *wdh*  plotpts.redim(0);
  return returnValue;
}


int 
assembleSubCurves(int & currentCurve,
		  GenericGraphicsInterface & gi, 
		  NurbsMapping & curve,
                  NurbsMapping & newCurve,
		  int & numberOfAssembledCurves,
                  NurbsMapping ** & assemblyCurves,
		  MouseSelectMode & mouseMode,
		  bool & curveRebuilt,
		  bool & plotCurve )
// =======================================================================================================
// /Access:
//     This is a protected function.
// /Description:
//     This function is called to assemble sub curves.
// =======================================================================================================

{
  int returnValue=0;
  
  // allocate storage for the curves if it isn't there
  if (!assemblyCurves)
  {
    assemblyCurves = new NurbsMapping*[curve.numberOfSubCurvesInList()];
    for ( int c=0; c<curve.numberOfSubCurvesInList(); c++ ) assemblyCurves[c] = NULL;
  }
	     
//   NurbsMapping *curveToAdd = NULL;
//   for ( int sc=0; sc<curve.numberOfSubCurvesInList(); sc++ )
//     if ( curve.subCurveFromList(sc).getGlobalID()==select.selection(0,0) )
//     {
//       curveToAdd = &curve.subCurveFromList(sc);
//       break;
//     }

  NurbsMapping *curveToAdd=&curve.subCurveFromList(currentCurve);

  // check that we don't use the same curve more than once!
  int q;
  for (q=0; q<numberOfAssembledCurves; q++)
  {
    if (assemblyCurves[q] == curveToAdd)
    {
      curveToAdd = NULL;
      break;
    }
  }
  
  if (curveToAdd == NULL)
  {
    gi.createMessageDialog("You can only use a subcurve once!", errorDialog);
    returnValue=1;
  }
  else
  {
    if ( numberOfAssembledCurves==0 )
    {
      newCurve = *curveToAdd;
      assemblyCurves[numberOfAssembledCurves++] = curveToAdd;
    }
    else if ( newCurve.merge(*curveToAdd)!=0 )
    {
      gi.createMessageDialog("curves are not close enough to join!", errorDialog);
      returnValue=1;
    }
    else
    {
      assemblyCurves[numberOfAssembledCurves++] = curveToAdd;
    }
	 
    if ( numberOfAssembledCurves<curve.numberOfSubCurvesInList() )
      {
	// also save the remaining subcurves in newCurve 
	bool used = false;
	for ( int sc=0; sc<curve.numberOfSubCurvesInList(); sc++ )
	  {
	    used = false;
	    for ( int as=0; as<numberOfAssembledCurves && !used; as++ )
	      {
		if ( curve.subCurveFromList(sc).getGlobalID()==assemblyCurves[as]->getGlobalID() )
		  used = true;
	      }
	    if ( !used ) 
	      {
		int nc = newCurve.addSubCurve(curve.subCurveFromList(sc));
		if ( curve.isSubCurveHidden(sc) ) newCurve.toggleSubCurveVisibility(nc);
	      }
	    
	  }
      }
    else
      {
	delete [] assemblyCurves;
	assemblyCurves  = NULL;
	numberOfAssembledCurves = 0;
	mouseMode = nothing;
	curve = newCurve;
	curveRebuilt = true;
	plotCurve = true;
    // *wdh*	interface.setToggleState(0, plotCurve);
    
    // gi.outputToCommandFile(sPrintF(line,"assemble curves %i\n",selectedCurve));
    
	gi.outputString("curve complete!");
      }
    
  }

  return returnValue;
}

static int
plotPickingSquare(GenericGraphicsInterface & gi, ArraySimple<real> xb) // draw a white square and make it pickable
{
// check that the graphicswindow is open!!!
  if( !gi.isGraphicsWindowOpen() )
    return 0;

  const real zLevel=-0.1;

  int list = gi.generateNewDisplayList();  // get a new display list to use
  assert(list!=0);

// AP: Need a way to allow for 2-D coordinates too!
//  gi.setAxesDimension(3);
      
  glNewList(list,GL_COMPILE);

// assign a name for picking
  glPushName(999999); 

  gi.setColour("white");

  glBegin(GL_QUADS);  
#ifndef OV_USE_DOUBLE
  glVertex3f(xb(0,0), xb(0,1), zLevel);
  glVertex3f(xb(0,0), xb(1,1), zLevel);
  glVertex3f(xb(1,0), xb(1,1), zLevel);
  glVertex3f(xb(1,0), xb(0,1), zLevel);
#else
  glVertex3d(xb(0,0), xb(0,1), zLevel);
  glVertex3d(xb(0,0), xb(1,1), zLevel);
  glVertex3d(xb(1,0), xb(1,1), zLevel);
  glVertex3d(xb(1,0), xb(0,1), zLevel);
#endif
  glEnd();

  glPopName();

  glEndList(); 

  return list;
}

static void
erasePickingSquare(GenericGraphicsInterface & gi, int list) 
{
// check that the graphicswindow is open!!!
  if( !gi.isGraphicsWindowOpen() )
    return;
  gi.deleteList(list);
}
