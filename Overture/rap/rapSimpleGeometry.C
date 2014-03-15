#include "ModelBuilder.h"
#include "rap.h"
#include "nurbsCurveEditor.h"

//\begin{>>ModelBuilderInclude.tex}{\subsection{simpleGeometry}}
void ModelBuilder::
simpleGeometry(MappingInformation &mapInfo, CompositeSurface & model, ListOfMappingRC &curveList,
		  PointList & points)
//===========================================================================
// /Description:
//    Build a model from simple geometrical tools.
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  assert(mapInfo.gp_!=NULL);
  GraphicsParameters & parameters = *mapInfo.gp_;

  static int planePoints[3];
  static int nPlanePoints=0;

  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT, true);
  parameters.set(GI_USE_PLOT_BOUNDS_OR_LARGER, true);
  parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES, false);
  parameters.set(GI_PLOT_END_POINTS_ON_CURVES, true);
  
  ViewLocation loc;

  GUIState interface;

  interface.setWindowTitle("Simple Geometry");
  interface.setExitCommand("exit","Exit");

  enum MouseMode{
    nothing=0,
    buildPoint,
    queryPoint,
    editCurve,
    buildPlane, 
    revolveX,
    revolveY,
    extrudeZ,
    editSurface,
    deleteSurface,
    numberOfMouseModes
  };

  MouseMode mouseMode = nothing;

  aString mouseModeCommands[] = { "Mouse Mode NoOp",
				  "Mouse Mode Build Point",
				  "Mouse Mode Query Point",
				  "Mouse Mode Edit Curve",
				  "Mouse Mode Build Plane",
				  "Mouse Mode Revolve X",
				  "Mouse Mode Revolve Y",
				  "Mouse Mode Extrude Z",
				  "Mouse Mode Edit Surface",
				  "Mouse Mode Delete Surface",
				  "" };

  aString mouseModeLabels[] = { "No Operation",
				"Build Point",
				"Query Point",
				"Edit Curve",
				"Build Plane",
				"Revolve around X",
				"Revolve around Y",
				"Extrude in Z",
				"Edit Surface",
				"Delete Surface",
				"" };

  interface.addRadioBox("Mouse Picking",mouseModeCommands, mouseModeLabels, int(mouseMode), 2); // 2 columns

 // general state variables
  bool plotObject = true;
  bool plotAxes = false; 
  bool plotCurves = true; 
  bool plotSurfaces = true;

  enum curveTB{plotCurvesTB=0, plotSurfacesTB, numberofTB };

  aString tbCommands[] = { "plot curves",
			   "plot surfaces",
			   "" };
  aString tbLabels[] = { "Curves",
			 "Surfaces",
			 "" };
  int tbState[] = {plotCurves,
		   plotSurfaces
  };

  interface.setToggleButtons(tbCommands, tbLabels, tbState, 2); 

  enum pushButtons{
    newCurve=0,
    unitSphere,
    numberOfPb
  };
 
  aString pbCommands[] = { 
    "new 2-d curve",
    "build unit sphere",
    "" 
  };

  aString pbLabels[]   = { 
    "New 2-D Curve",
    "Unit Sphere",
    "" 
  };

  interface.setPushButtons(pbCommands, pbLabels, 1); // 1 row

// setup textlabels
  aString textCmd[] = {"newPoint", "split revolving", ""};
  aString textLbl[] = {"New Point: (x, y, z)", "Revolving Surface Split", ""};
  aString textInit[3]; // don't forget to update this size when adding/deleting commands
  int cnt=0;
  int ptIndex = cnt; sPrintF(textInit[cnt++],"%g %g %g", 0., 0., 0.);           // new point
  int revolveSplit = 2; // default is to split revolving surfaces into 2 pieces (to avoid periodic patches)
  int revolveSplitIndex = cnt; sPrintF(textInit[cnt++],"%i", revolveSplit );           // new point
  textInit[cnt++] = ""; // last entry

  interface.setTextBoxes(textCmd, textLbl, textInit); 

  SelectionInfo select;
  aString answer="", line;
  aString buf;
  int len, i, j, axes;
 
  gi.pushGUI(interface);

  RadioBox & rBox = interface.getRadioBox(0);

  for( int it=0;; it++ )
  {
    
// set the sensitivity of the GUI
//    interface.setSensitive(true, DialogData::radioBoxWidget, 0); // radio box active
//    interface.setSensitive(hiddenSubCurve >= curve.numberOfSubCurves() && 
//  			   hiddenSubCurve < curve.numberOfSubCurvesInList(), 
//  			   DialogData::pushButtonWidget,  showOnePb);// show one subcurve
//      interface.setSensitive((nSelectedPoints > 0), DialogData::pushButtonWidget, clearLastPointPb);
//      interface.setSensitive((nSelectedPoints > 0), DialogData::pushButtonWidget, clearAllPointsPb);
     
     
    gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

    gi.getAnswer(answer, "", select);

    gi.savePickCommands(true); // restore
   
    plotObject = true;

    if ( answer.matches("exit") )
    {
      break;
    }
    else if ( (len = answer.matches("Mouse Mode")) )
    {
      aString mode= (answer.length() > len+1)? answer(len+1,answer.length()-1): (aString)"";
	 
      if (mode.matches("NoOp"))
	mouseMode = nothing;
      else if (mode.matches("Build Point"))
	mouseMode = buildPoint;
      else if (mode.matches("Query Point"))
	mouseMode = queryPoint;
      else if (mode.matches("Edit Curve"))
	mouseMode = editCurve;
      else if (mode.matches("Build Plane"))
	mouseMode = buildPlane;
      else if (mode.matches("Revolve X"))
	mouseMode = revolveX;
      else if (mode.matches("Revolve Y"))
	mouseMode = revolveY;
      else if (mode.matches("Extrude Z"))
	mouseMode = extrudeZ;
      else if (mode.matches("Edit Surface"))
	mouseMode = editSurface;
      else if (mode.matches("Delete Surface"))
	mouseMode = deleteSurface;
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
    else if (answer.matches("new 2-d curve"))
    {
      NurbsMapping *curve_ = new NurbsMapping;
      curve_->incrementReferenceCount();
      curve_->setRangeDimension(2);
  
      nurbsCurveEditor(*curve_, gi, points);

// add it to a special list only containing curves
      if (curve_->isInitialized())
      {
	MappingRC curveRC(*curve_);
	curveList.addElement(curveRC);
      }
      else
	printf("Throwing away uninitialized curve...\n");
      if (curve_->decrementReferenceCount() == 0)
	delete curve_;
    }
    else if ( (len=answer.matches("edit curve")) )
    {
      aString curveString = "";
      
      if (answer.length() > len+1)
	curveString = answer(len+1,answer.length()-1);
      int c1;
      if ( sScanF(curveString, "%i", &c1) == 1 )
      {
	Mapping * curve_ = & curveList[c1].getMapping();
	gi.outputString(sPrintF(buf,"Selected curve #%i", c1));
	if (curve_->getClassName() == "NurbsMapping")
	  nurbsCurveEditor((NurbsMapping &) *curve_, gi, points);
	else
	  gi.outputString("That curve is not a nurbs, so it can not be edited");
// need to redraw the screen?
	gi.redraw();
      }
      
    }
    else if ( (len=answer.matches("edit surface")) )
    {
      aString surfaceString = "";
      
      if (answer.length() > len+1)
	surfaceString = answer(len+1,answer.length()-1);
      int s1;
      if ( sScanF(surfaceString, "%i", &s1) == 1 )
      {
	Mapping * surface_ = & model[s1];
	gi.getView(loc); // get the viewpoint info
	gi.erase();
	aString oldName;
	parameters.get(GI_TOP_LABEL,oldName);
	parameters.set(GI_TOP_LABEL,sPrintF(buf,"sub-surface %i", s1));
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	surface_->update( mapInfo );
	parameters.set(GI_TOP_LABEL,oldName);
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	gi.setView(loc); // reset the view point
// need to erase the display lists for this surface
	model.eraseCompositeSurface(gi, s1);
	gi.redraw();
      }
      
    }
    else if ( (len=answer.matches("delete surface")) )
    {
      aString surfaceString = "";
      
      if (answer.length() > len+1)
	surfaceString = answer(len+1,answer.length()-1);
      int s1;
      if ( sScanF(surfaceString, "%i", &s1) == 1 )
      {
	Mapping * surface_ = & model[s1];
	gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be deleted", 
				s1, SC model[s1].getName(Mapping::mappingName)));
// delete any displaylists
	model.eraseCompositeSurface(gi, s1);
// remove the surface from the model
	model.remove(s1);
      }
    }
    else if ( (len=answer.matches("revolve around x")) )
    {
      aString curveString = "";
      
      if (answer.length() > len+1)
	curveString = answer(len+1,answer.length()-1);
      int c1;
      if ( sScanF(curveString, "%i", &c1) == 1 )
      {
	Mapping * curve_ = & curveList[c1].getMapping();
	gi.outputString(sPrintF(buf,"Selected curve #%i", c1));

	real dAngle = 1./((real) revolveSplit);
	for (int q=0; q<revolveSplit; q++)
	{
	  RevolutionMapping *rev_ = new RevolutionMapping;
	  rev_->incrementReferenceCount();
  
	  rev_->setRevolutionary(*curve_);

// revolve about the x-axis
	  RealArray origin(3), tangent(3);
	  origin = 0; tangent=0;
	  tangent(0) = 1;
	  rev_->setLineOfRevolution(origin, tangent);
  
	  real startAngle=q*dAngle, endAngle=(q+1)*dAngle;
	  rev_->setRevolutionAngle(startAngle, endAngle);

// add the revolutionary to the model
	  printf("Adding part %d of the new surface of revolution to the model\n", q);
	  model.add(*rev_, -1);
	  model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));

	  if (rev_->decrementReferenceCount() == 0)
	    delete rev_;
	}// end for q < revolveSplit
      }
    }
    else if ( (len=answer.matches("revolve around y")) )
    {
      aString curveString = "";
      
      if (answer.length() > len+1)
	curveString = answer(len+1,answer.length()-1);
      int c1;
      if ( sScanF(curveString, "%i", &c1) == 1 )
      {
	Mapping * curve_ = & curveList[c1].getMapping();
	gi.outputString(sPrintF(buf,"Selected curve #%i", c1));

	real dAngle = 1./((real) revolveSplit);
	for (int q=0; q<revolveSplit; q++)
	{
	  RevolutionMapping *rev_ = new RevolutionMapping;
	  rev_->incrementReferenceCount();
  
	  rev_->setRevolutionary(*curve_);

// revolve about the y-axis
	  RealArray origin(3), tangent(3);
	  origin = 0; tangent=0;
	  tangent(1) = 1;
	  rev_->setLineOfRevolution(origin, tangent);
  
	  real startAngle=q*dAngle, endAngle=(q+1)*dAngle;
	  rev_->setRevolutionAngle(startAngle, endAngle);

// add the revolutionary to the model
	  printf("Adding part %d of the new surface of revolution to the model\n", q);
	  model.add(*rev_, -1);
	  model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));

	  if (rev_->decrementReferenceCount() == 0)
	    delete rev_;
	} // end for q<revolveSplit
	
      }
    }
    else if( (len = answer.matches("extrude in z")) ) // make an arc
    {
      aString curveString = "";
      
      if (answer.length() > len+1)
	curveString = answer(len+1,answer.length()-1);
      int c1;
      if ( sScanF(curveString, "%i", &c1) == 1 )
      {
	Mapping *curve_ = & curveList[c1].getMapping();

	gi.getAnswer(answer, "");

	if (len = answer.matches("zBounds"))
	{
	  SweepMapping *extrusion_ = new SweepMapping(curve_, NULL, NULL, 2);
	  extrusion_->incrementReferenceCount();

	  aString newBounds = "";
	  if (answer.length() > len+1)
	    newBounds = answer(len+1,answer.length()-1);
	  printf("answer string: `%s'\n", SC answer);
	  printf("newBounds string: `%s'\n", SC newBounds);
	   
	  real zMin, zMax;
	  if ( sScanF(newBounds, "%g %g", &zMin, &zMax) == 2 )
	  {
	    gi.outputString(sPrintF(buf, "zMin = %g, zMax = %g", zMin, zMax));
	  }
	  else
	  {
	    gi.outputString("Error: could not read 2 reals! Please re-enter!");
	    sPrintF(newBounds,"%h %g", 0., 1.);           // new radius
	  }

	  if (zMin < zMax)
	  {
	    extrusion_->setExtrudeBounds(zMin, zMax);
// add the revolutionary to the model
	    printf("Adding the new extrusion surface to the model\n");
	    model.add(*extrusion_, -1);
	    model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
	  }
	  else
	  {
	    gi.outputString("Error: zMin must be smaller than zMax!\n"
			    "Please re-enter!");
	  }
	} // end if "zBounds"
	else if (answer.matches("cancel"))
	{
	  break;
	}
      }
      else
      {
	gi.outputString("Error: could not read the curve number! Please re-enter!");
      }
    } // end "extrude in z"...
    
    else if( (len = answer.matches("newPoint")) ) // read the point coordinates
    {
      aString pointString = "";
      if (answer.length() > len+1)
	pointString = answer(len+1,answer.length()-1);
      real x, y, z;
      Point newPoint;
      if ( sScanF(pointString, "%g %g %g", &x, &y, &z) == 3 )
      {
	newPoint.coordinate[0] = x;
	newPoint.coordinate[1] = y;
	newPoint.coordinate[2] = z;
	points.add( newPoint );
	gi.outputString(sPrintF(buf, "Point #%d: (%g, %g, %g)", points.size(), x, y, z));
      }
      else
      {
	gi.createMessageDialog("Error: could not read 3 reals! Please re-enter!", errorDialog);
	sPrintF(pointString,"%g %g %g", 0., 0., 0.);           // new point
      }
      
      interface.setTextLabel(ptIndex, pointString); // (re)set the textlabel
    }
    else if( (len = answer.matches("split revolving")) ) // read the point coordinates
    {
      aString splitString = "";
      if (answer.length() > len+1)
	splitString = answer(len+1,answer.length()-1);
      int newSplit=0;
      if ( sScanF(splitString, "%d", &newSplit) == 1 && newSplit>=1 )
      {
	revolveSplit = newSplit;
	gi.outputString(sPrintF(buf, "Will split revolving surfaces in %d segments", revolveSplit));
      }
      else
      {
	gi.createMessageDialog("Error: could not read 1 positive integer! Please re-enter!", errorDialog);
	sPrintF(splitString,"%d", revolveSplit);
      }
      
      interface.setTextLabel(revolveSplitIndex, splitString); // (re)set the textlabel
    }
    
    else if( select.nSelect > 0 && mouseMode == buildPoint )
    {
// find the closest (in terms of z-buff coord) underlying subsurface
      bool foundSurface=false;
      int s;
      for( i=0; i<select.nSelect && !foundSurface; i++ )
      {
	for( s=0; s<model.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == model[s].getGlobalID() )
	  {
	    printf("Sub-surface %i was selected\n",s);
	    foundSurface = true;
	    break;
	  }
	}
      }
      Point newPoint;
      if (foundSurface)
      {
// project and evaluate the mapping to get a point on the surface
	Mapping & subSurface = model[s];
	realArray rp(1,2), xp(1,3);
	xp(0,0) = select.x[0];
	xp(0,1) = select.x[1];
	xp(0,2) = select.x[2];
	printf("Coordinate from OpenGL: (%e, %e, %e)\n", select.x[0], select.x[1], select.x[2]);
	rp = -1; // no initial guess
	subSurface.inverseMap(xp,rp);
//
	subSurface.map(rp,xp);
	for (i=0; i<3; i++)
	  newPoint.coordinate[i] = xp(0,i);
	
	printf("Projected point according to mapping: (%e, %e, %e)\n",  
	       newPoint.coordinate[0], newPoint.coordinate[1], newPoint.coordinate[2]);
      }
      else
      {
// otherwise, just use the coordinate from OpenGL
	for (i=0; i<3; i++)
	  newPoint.coordinate[i] = select.x[i];
      }
// add the new point to the list
      points.add( newPoint );

      gi.outputString(sPrintF(buf, "Point #%d: (%g, %g, %g)", points.size(), 
			      newPoint.coordinate[0], 
			      newPoint.coordinate[1], 
			      newPoint.coordinate[2]));
// save equivalent command
      gi.outputToCommandFile(sPrintF(buf, "newPoint %e %e %e\n", newPoint.coordinate[0], 
				     newPoint.coordinate[1], newPoint.coordinate[2]));
    }

    else if ( select.nSelect>0 && mouseMode == queryPoint )
    {
      bool foundPoint = false;
      for ( int s=0; s<select.nSelect && !foundPoint; s++ )
	for ( i=0; i<points.size() && !foundPoint; i++ )
	  if ( points[i].getGlobalID()==select.selection(s,0) )
	  {
	    foundPoint = true;
	    gi.outputString(sPrintF(buf,"Point #%i, (%g, %g, %g)", i, points[i].coordinate[0], 
				    points[i].coordinate[1], points[i].coordinate[2]));
	  }
    }
    else if( select.nSelect > 0 && mouseMode == buildPlane )
    {
      if (nPlanePoints<3)
      {
// search the selection for a point
	bool found=false;
	for( i=0; i<select.nSelect && !found; i++ )
	{
	  for(j=0; j<points.size() && !found; j++ )
	  {
	    if( points[j].getGlobalID() == select.selection(i,0) )
	    {
	      gi.outputString(sPrintF(buf, "Point #%i selected for plane", j));
	      planePoints[nPlanePoints++] = j;
	      found = true; // breaks out of the outer loop
// save equivalent command
	      gi.outputToCommandFile(sPrintF(buf, "point for plane %i\n", j));
	    }
	  }
	}
      }
      else
      {
// this should never happen!
	gi.createMessageDialog("You have already selected 3 points!? (This should never happen!)",
			       errorDialog);
	continue;
      }
// Build the plane and erase the points
      if (nPlanePoints == 3 )
      {
// plot the green square for the last point
	parameters.set(GI_POINT_SIZE, 6);
	parameters.set(GI_POINT_COLOUR, "green");
	for (i=0; i<nPlanePoints; i++)
	  points[planePoints[i]].plot(gi, parameters);
	gi.redraw(true);

	real planeCoordinates[3][3];
	for (i=0; i<3; i++)
	  for (axes=0; axes<3; axes++)
	    planeCoordinates[i][axes] = points[planePoints[i]].coordinate[axes];

	addPlaneToModel(planeCoordinates, nPlanePoints, model, gi);
      }

    }
    else if ((len = answer.matches("point for plane")))
    {
      if (nPlanePoints<3)
      {
// read point number from string
	int j;
	aString newBounds = "";
	if (answer.length() > len+1)
	  newBounds = answer(len+1,answer.length()-1);
	if ( sScanF(newBounds, "%i", &j) == 1 )
	{
	  gi.outputString(sPrintF(buf, "Point #%i selected for plane", j));
	  planePoints[nPlanePoints++] = j;
	}
	else
	{
	  gi.createMessageDialog("Error: could not read the point number!", errorDialog);
	}
      }
      else
      {
// this should never happen!
	gi.createMessageDialog("You have already selected 3 points!? (This should never happen!)",
			       errorDialog);
	continue;
      }
// Build the plane and erase the points
      if (nPlanePoints == 3 )
      {
	real planeCoordinates[3][3];
	for (i=0; i<3; i++)
	  for (axes=0; axes<3; axes++)
	    planeCoordinates[i][axes] = points[planePoints[i]].coordinate[axes];

	addPlaneToModel(planeCoordinates, nPlanePoints, model, gi);
      }

    }
    
    else if ( select.nSelect > 0 && mouseMode == editCurve )
    {
      Mapping *curve_=NULL;
// loop to set curve_...
      bool foundCurve = false;
      for ( int s=0; s<select.nSelect && !foundCurve; s++ )
	for ( i=0; i<curveList.getLength() && !foundCurve; i++ )
	  if ( curveList[i].getMapping().getGlobalID()==select.selection(s,0) )
	  {
	    foundCurve = true;
	    curve_ = & curveList[i].getMapping();
	    gi.outputString(sPrintF(buf,"Selected curve #%i", i));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "edit curve %i\n", i));
	  }
      if (foundCurve)
      {
	if (curve_->getClassName() == "NurbsMapping")
	  nurbsCurveEditor((NurbsMapping &) *curve_, gi, points);
	else
	  gi.outputString("That curve is not a nurbs, so it can not be edited");
      }
    }
    
    else if (select.nSelect > 0 && mouseMode == editSurface)
    {
      Mapping *surface_=NULL;
// loop to set surface_...
      bool foundSurface = false;
      for ( int s=0; s<select.nSelect && !foundSurface; s++ )
	for ( i=0; i<model.numberOfSubSurfaces() && !foundSurface; i++ )
	  if ( model[i].getGlobalID()==select.selection(s,0) )
	  {
	    foundSurface = true;
	    surface_ = & model[i];
	    gi.outputString(sPrintF(buf,"Selected surface #%i", i));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "edit surface %i\n", i));
	    break; // avoid i getting incremented
	  }
      if (foundSurface)
      {
	gi.getView(loc); // get the viewpoint info
	gi.erase();
	aString oldName;
	parameters.get(GI_TOP_LABEL,oldName);
	parameters.set(GI_TOP_LABEL,sPrintF(buf,"sub-surface %i", i));
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	surface_->update( mapInfo );
	parameters.set(GI_TOP_LABEL,oldName);
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
	gi.setView(loc); // reset the view point
// need to erase the display lists for this surface
	model.eraseCompositeSurface(gi, i);
	gi.redraw();
      }
      
    }

    else if (select.nSelect > 0 && (mouseMode == deleteSurface))
    {
      Mapping *surface_=NULL;
// loop to set surface_...
      bool foundSurface = false;
      for ( int s=0; s<select.nSelect && !foundSurface; s++ )
	for ( i=0; i<model.numberOfSubSurfaces() && !foundSurface; i++ )
	  if ( model[i].getGlobalID()==select.selection(s,0) )
	  {
	    foundSurface = true;
	    surface_ = & model[i];
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be deleted", 
				    i, SC model[i].getName(Mapping::mappingName)));
// delete any displaylists
	    model.eraseCompositeSurface(gi, i);
// remove the surface from the model
	    model.remove(i);
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "delete surface %i\n", i));
	  }
    }
    
    else if (select.nSelect > 0 && (mouseMode == revolveX || mouseMode == revolveY))
    {
      Mapping *curve_=NULL;
// loop to set curve_...
      bool foundCurve = false;
      for ( int s=0; s<select.nSelect && !foundCurve; s++ )
	for ( i=0; i<curveList.getLength() && !foundCurve; i++ )
	  if ( curveList[i].getMapping().getGlobalID()==select.selection(s,0) )
	  {
	    foundCurve = true;
	    curve_ = & curveList[i].getMapping();
	    gi.outputString(sPrintF(buf,"Selected curve #%i", i));
// save equivalent command
	    if (mouseMode == revolveX)
	      gi.outputToCommandFile(sPrintF(buf, "revolve around x %i\n", i));
	    else
	      gi.outputToCommandFile(sPrintF(buf, "revolve around y %i\n", i));
	  }

      if (foundCurve)
      {
// add revolveSplit surfaces to the model
	real dAngle = 1./((real) revolveSplit);
	for (int q=0; q<revolveSplit; q++)
	{
	  RevolutionMapping *rev_ = new RevolutionMapping;
	  rev_->incrementReferenceCount();
  
	  rev_->setRevolutionary(*curve_);
	  RealArray origin(3), tangent(3);
	  origin = 0; tangent=0;
	  if (mouseMode == revolveX)
	  {
	    tangent(0) = 1;
	  }
// the default origin is (-1,0,0) so we need to set the tangent, even though the default
// is to revolve around y
	  else if (mouseMode == revolveY) 
	  {
	    tangent(1) = 1;
	  }
	  rev_->setLineOfRevolution(origin, tangent);

	  real startAngle=q*dAngle, endAngle=(q+1)*dAngle;
	  rev_->setRevolutionAngle(startAngle, endAngle);
	  
// add the revolutionary to the model
	  printf("Adding part %d of the new surface of revolution to the model\n", q);
	  model.add(*rev_, -1);
	  model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));

	  if (rev_->decrementReferenceCount() == 0)
	    delete rev_;
	} // end for q<revolveSplit
	
      } // end if foundCurve
    }
      
    else if (select.nSelect > 0 && mouseMode == extrudeZ)
    {
      Mapping *curve_=NULL;
// loop to set curve_...
      bool foundCurve = false;
      for ( int s=0; s<select.nSelect && !foundCurve; s++ )
	for ( i=0; i<curveList.getLength() && !foundCurve; i++ )
	  if ( curveList[i].getMapping().getGlobalID()==select.selection(s,0) )
	  {
	    foundCurve = true;
	    curve_ = & curveList[i].getMapping();
	    gi.outputString(sPrintF(buf,"Selected curve #%i", i));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "extrude in z %i\n", i));
	  }

      if (foundCurve)
      {
	SweepMapping *extrusion_ = new SweepMapping(curve_, NULL, NULL, 2);
	extrusion_->incrementReferenceCount();

// setup a mini GUI to get Zmin, Zmax
       GUIState extrusionGUI;

       extrusionGUI.setWindowTitle("Extrusion");
       extrusionGUI.setExitCommand("cancel","Cancel");
// setup textlabels
       aString textCmd[] = {"zBounds", ""};
       aString textLbl[] = {"zMin, zMax:", ""};
       aString textInit[2]; // don't forget to update this size when adding/deleting commands
       int cnt=0;
       int zIndex = cnt; sPrintF(textInit[cnt++],"%g %g", 0., 1.);           // new radius
       textInit[cnt++] = "";

       real zMin=0, zMax=1;

       extrusionGUI.setTextBoxes(textCmd, textLbl, textInit);

       gi.pushGUI(extrusionGUI);
       for(;;)
       {
	 gi.getAnswer(answer, "", select);
	 if (len = answer.matches("zBounds"))
	 {
	   aString newBounds = "";
	   if (answer.length() > len+1)
	     newBounds = answer(len+1,answer.length()-1);
	   printf("answer string: `%s'\n", SC answer);
	   printf("newBounds string: `%s'\n", SC newBounds);
	   
	   if ( sScanF(newBounds, "%g %g", &zMin, &zMax) == 2 )
	   {
	     gi.outputString(sPrintF(buf, "zMin = %g, zMax = %g", zMin, zMax));
	   }
	   else
	   {
	     gi.createMessageDialog("Error: could not read 2 reals! Please re-enter!", errorDialog);
	     sPrintF(newBounds,"%h %g", 0., 1.);           // new radius
	   }
	   extrusionGUI.setTextLabel(zIndex, newBounds); // (re)set the textlabel

	   if (zMin < zMax)
	   {
	     extrusion_->setExtrudeBounds(zMin, zMax);
// add the revolutionary to the model
	     printf("Adding the new extrusion surface to the model\n");
	     model.add(*extrusion_, -1);
	     model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));

	     break;
	   }
	   else
	   {
	     gi.createMessageDialog("Error: zMin must be smaller than zMax!\n"
				    "Please re-enter!", errorDialog);
	   }
	 }
	 else if (answer.matches("cancel"))
	 {
	   break;
	 }
       } // end for(;;)

       gi.popGUI();

// cleanup the sweepmapping
       if (extrusion_->decrementReferenceCount() == 0)
	 delete extrusion_;
      } // end if foundcurve...
    } // end if mouseMode==extrudeZ
    else if (answer == "build unit sphere")
    {
// make sphere mapping and insert it into the compositeSurface
      SphereMapping * newPatch_;
      newPatch_ = new SphereMapping(1.0, 1.0); // inner and outer radius the same
      newPatch_->incrementReferenceCount();
      
      newPatch_->setDomainDimension(2);
      
      model.add( *newPatch_, -1 );
      model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
      gi.outputString(sPrintF(buf, "Adding the new unit sphere to the model, globalID=%i", 
			      newPatch_->getGlobalID() ));
// cleanup
      if (newPatch_->decrementReferenceCount() == 0)
	delete newPatch_;
    }
    
    else if (answer.matches("plot curves"))
    {
     plotCurves = !plotCurves;
     interface.setToggleState(plotCurvesTB, plotCurves);
    }
    else if (answer.matches("plot surfaces"))
    {
     plotSurfaces = !plotSurfaces;
     interface.setToggleState(plotSurfacesTB, plotSurfaces);
    }
    else if (!select.active) // if a mogl-pickOutside event occurs, select.active will be true
    {
      gi.outputString( sPrintF(buf,"Unknown response=%s", (const char*)answer) );
      gi.stopReadingCommandFile();
      plotObject=false;
    }
    

   if ( plotObject )
   {
     gi.erase();

     parameters.set(GI_POINT_SIZE, 4); // the size of endpoints is multiplied by 1.5
     parameters.set(GI_POINT_COLOUR, "black");
     if ( plotCurves )
     {
       for (i=0; i<curveList.getLength(); i++)
       {
	   PlotIt::plot(gi, curveList[i].getMapping(), parameters );
       }       
     }
     
     if ( plotSurfaces ) 
     {
       PlotIt::plot(gi, model, parameters);
     }
     
// plot the points
     points.plot(gi, parameters);

// plot the points selected for building a plane
     if (mouseMode == buildPlane)
     {
       parameters.set(GI_POINT_SIZE, 6);
       parameters.set(GI_POINT_COLOUR, "green");
       for (i=0; i<nPlanePoints; i++)
	 points[planePoints[i]].plot(gi, parameters);
     }

   } // end if plotObject...

  } // end for it=0...
  
  gi.popGUI();
  
}
