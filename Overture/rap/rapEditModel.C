#include "ModelBuilder.h"
#include "rap.h"

static void 
crossProduct(real *v1, real *v2, real *res)
{
  res[0]=v1[1]*v2[2]-v1[2]*v2[1];
  res[1]=-(v1[0]*v2[2]-v2[0]*v1[2]);
  res[2]=v1[0]*v2[1]-v2[0]*v1[1];
}

static real
dotProduct(real *v1, real *v2)
{
  return(v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]);
}

//\begin{>>ModelBuilderInclude.tex}{\subsection{addPlaneToModel}}
bool ModelBuilder::
addPlaneToModel(real planeCoordinates[3][3], int &planePoints, CompositeSurface &model, 
		GenericGraphicsInterface &gi)
//===========================================================================
// /Description:
//    Given three points, build a plane and add it to the model.
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  if (planePoints == 3)
  {
// make sure the points are not co-linear and re-align the third point to make the plane rectangular
    real t12[3], t13[3], n[3], length, t13n[3];
    int i;
    aString buf;
    for (i=0; i<3; i++)
    {
      t12[i] = planeCoordinates[1][i] - planeCoordinates[0][i];
      t13[i] = planeCoordinates[2][i] - planeCoordinates[0][i];
    }
    real t12_l = sqrt(dotProduct(t12, t12));
    real t13_l = sqrt(dotProduct(t13, t13));
    for (i=0; i<3; i++)
    {
      t12[i] /= t12_l;
      t13[i] /= t13_l;
    }
    crossProduct(t12, t13, n);
// check if the points are co-linear
    if ( dotProduct(n, n) < 1.e-7 )
    {
      gi.outputString("The points are co-linear! Reenter the last point!");
      planePoints = 2;
      return false;
    }
    else
    {
      crossProduct(n, t12, t13n);
      length = sqrt(dotProduct(t13n, t13n));
      for (i=0; i<3; i++)
	t13n[i] /= length;
      real beta = dotProduct(t13, t13n);
// change the third point
      for (i=0; i<3; i++)
	planeCoordinates[2][i] = planeCoordinates[0][i] + t13_l*beta*t13n[i];
	
// make plane mapping and insert it into the compositeSurface
      PlaneMapping * newPatch_;
      newPatch_ = new PlaneMapping(planeCoordinates[0][0], planeCoordinates[0][1], planeCoordinates[0][2],
				   planeCoordinates[1][0], planeCoordinates[1][1], planeCoordinates[1][2],
				   planeCoordinates[2][0], planeCoordinates[2][1], planeCoordinates[2][2]);
      newPatch_->incrementReferenceCount();
      
      model.add( *newPatch_, -1 );
      model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
      gi.outputString(sPrintF(buf, "Adding mapping `%s'", SC newPatch_->getName(Mapping::mappingName)));

      planePoints = 0;

      return true;
    }
  }
  return false;
}

static void
projectSurfaceOnModel(int s, CompositeSurface &model, GenericGraphicsInterface &gi)
{
// evaluate surface grid
  int rPoints = model[s].getGridDimensions(0), sPoints = model[s].getGridDimensions(1);
	
  realArray r(rPoints, sPoints, 2);
  real dr = 1./((real) rPoints-1), ds = 1./((real) sPoints-1);
	
  Index I(0,rPoints), J(0,sPoints);
  for( int i1=0; i1<rPoints; i1++ )
    r(i1,J,0) = i1*dr;
  for( int i2=0; i2<sPoints; i2++ )
    r(I,i2,1) = i2*ds;

  r.reshape(rPoints*sPoints, 2);
  realArray x(rPoints*sPoints, 3);
	
  model[s].map(r, x);
	
// setup the mappingProjectionParameter to ignore surface s
  MappingProjectionParameters mpp;
  intArray & ignoreSurface = mpp.getIntArray(MappingProjectionParameters::ignoreThisSubSurface);
  ignoreSurface.redim(rPoints*sPoints);
  ignoreSurface = s;
	
  model.project(x, mpp);

  x.reshape(rPoints, sPoints, 3);

// Build the NURBS surface that interpolates the projected points
  NurbsMapping * nSurf_;
  nSurf_ = new NurbsMapping;
  nSurf_->incrementReferenceCount();
  nSurf_->interpolate(x);

  model.add(*nSurf_, -1);
  model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
  aString buf;
  gi.outputString(sPrintF(buf, "Adding mapping `%s'", SC nSurf_->getName(Mapping::mappingName)));
}


static void
projectPointsOnSurface(realArray & x, int s, CompositeSurface &model, 
		     CompositeSurface &deletedSurfaces, GenericGraphicsInterface &gi)
// the points to be projected are in the array x(0:numberOfPoints-1,0:2), where the last index
// corresponds to the (x,y,z)-ccordinates.
{
  Mapping *projectMap_ = &model[s];

  printf("Computing projection...\n");
  MappingProjectionParameters mpp;
  if (projectMap_->getClassName() == "TrimmedMapping")
  {
    TrimmedMapping * trim_ = (TrimmedMapping*) projectMap_;
    trim_->untrimmedSurface()->project( x, mpp );
  }
  else
    projectMap_->project( x, mpp );
  realArray & parameterCoord = mpp.getRealArray(MappingProjectionParameters::r);
// allocate the newSubCurve on the heap, so it doesn't get deleted outside this scope
  NurbsMapping & newSubCurve = *new NurbsMapping; 
  newSubCurve.incrementReferenceCount();
  
  newSubCurve.interpolate(parameterCoord);
// make a trimmed mapping of projectMap_ (unless it already is trimmed)
  TrimmedMapping * trim_=NULL;
  if (projectMap_->getClassName() != "TrimmedMapping")
  {
    printf("Making a TrimmedMapping based on projectMap_\n");		
    trim_ = new TrimmedMapping(*projectMap_, NULL, 0, NULL); // make a new trimmed mapping 
    trim_->incrementReferenceCount();
// tmp
    printf("after newing the TrimmedMapping\n");		
// get the colour of the untrimmed surface and
// remove the untrimmed surface from the model
    aString oldColour="blue";

    oldColour = model.getColour(s);
// delete any displaylists
    model.eraseCompositeSurface(gi, s);
// add the surface to the collection of deleted surfaces
    deletedSurfaces.add(model[s], model.getSurfaceID(s));
    deletedSurfaces.setColour(deletedSurfaces.numberOfSubSurfaces()-1, "gray50");
// remove the surface from the model
    model.remove(s);

    model.add( *trim_, -1 );
    model.setColour(model.numberOfSubSurfaces()-1, oldColour);
  }
  else
  {
    printf("projectMap_ is trimmed\n");		
    trim_ = (TrimmedMapping *) projectMap_;
// just to be consistent with the untrimmed case
    trim_->incrementReferenceCount();

// force redraw of the display lists so that sub-surface s will get replotted properly
    model.eraseCompositeSurface(gi, s);
    gi.redraw();
  }
// add the newSubCurve to the last trim curve in the trimmed mapping	
  int nTrim = trim_->getNumberOfTrimCurves();
  if (nTrim > 0 && trim_->getTrimCurve(nTrim-1)->getClassName() == "NurbsMapping" )
  {
    printf("The last trim curve (#%d) is a NurbsMapping. Adding the intersection curve...\n", nTrim);
    NurbsMapping & lastTrimCurve = (NurbsMapping &) *(trim_->getTrimCurve(nTrim-1));
    lastTrimCurve.merge(newSubCurve);
// invalidate trimming to force the user to edit the trim curve
    trim_->invalidateTrimming();
  }
  else if (nTrim > 0)
  {
    printf("Sorry, but the last trim curve (#%d) is a %s. Can't add the intersection curve!\n",
	   nTrim, SC trim_->trimCurves[nTrim-1]->getClassName());
  }
  else
  {
    printf("Error: This trimmed mapping does not have ANY trim curves!!!\n");
  }
  if (trim_->decrementReferenceCount()==0)
    delete trim_;
  if (newSubCurve.decrementReferenceCount() == 0)
    delete &newSubCurve;
  
}


static bool
intersectTwoSurfaces(CompositeSurface &model, CompositeSurface &deletedSurfaces,
		     GenericGraphicsInterface & gi,
		     Mapping *map0, Mapping *map1, IntersectionMapping * &interSect_)
{
  bool rc=true;
  Mapping *iMap_[2];
  iMap_[0] = map0;
  iMap_[1] = map1;
  
  if (iMap_[0] != iMap_[1])
  {
    if (interSect_ && interSect_->decrementReferenceCount() == 0)
      delete interSect_;
    interSect_ = new IntersectionMapping;
    interSect_->incrementReferenceCount();
    
    int retCode = interSect_->intersect( *iMap_[0], *iMap_[1] );
    printf("Return code from intersect: %d, Curve is %s\n", retCode, 
	   interSect_->curve? "SET" : "NULL");
// add the new curve to the set of trim curves!
    int i;
    for (i=0; i<2; i++)
    {
      if (interSect_->curve)
      {
	TrimmedMapping * trim_;
// make a trimmed mapping of the untrimmed
	if (iMap_[i]->getClassName() != "TrimmedMapping")
	{
	  printf("Making a TrimmedMapping based on iMap[%i]\n", i);		
	  trim_ = new TrimmedMapping(*iMap_[i], NULL, 0, NULL); // make a new trimmed mapping 
	  trim_->incrementReferenceCount();
// get the colour of the untrimmed surface and
// remove the untrimmed surface from the model
	  aString oldColour="blue";
	  for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	  {
	    if( iMap_[i] == &model[s] )
	    {
	      oldColour = model.getColour(s);
// delete any displaylists
	      model.eraseCompositeSurface(gi, s);
// add the surface to the collection of deleted surfaces
	      deletedSurfaces.add(model[s], model.getSurfaceID(s));
	      deletedSurfaces.setColour(deletedSurfaces.numberOfSubSurfaces()-1, "gray50");
// remove the surface from the model
	      model.remove(s);
	      break;
	    }
	  }
	  model.add( *trim_, -1 );
	  model.setColour(model.numberOfSubSurfaces()-1, oldColour);
	}
	else
	{
	  printf("iMap[%i] is trimmed\n", i);		
	  trim_ = (TrimmedMapping *) iMap_[i];
	  for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	  {
	    if( iMap_[i] == &model[s] )
	    {
// force redraw of the display lists so that sub-surface s will get replotted properly
	      model.eraseCompositeSurface(gi, s);
	      gi.redraw();
	    }
	  }
	}

	int nTrim = trim_->getNumberOfTrimCurves();
	Mapping *rCurve = (i==0)? interSect_->rCurve1 : interSect_->rCurve2;
	if (nTrim > 0 && trim_->getTrimCurve(nTrim-1)->getClassName() == "NurbsMapping" &&
	    rCurve->getClassName() == "NurbsMapping")
	{
	  printf("The last trim curve (#%d) is a NurbsMapping. Adding the intersection curve...\n", nTrim);
	  NurbsMapping & lastTrimCurve = (NurbsMapping &) *(trim_->getTrimCurve(nTrim-1));
	  NurbsMapping & intersectionCurve = (NurbsMapping &) *rCurve;
	  printf("There are %i subCurves in the intersection curve...\n", intersectionCurve.numberOfSubCurves());
	  int qq;
	  for (qq=0; qq<intersectionCurve.numberOfSubCurves(); qq++)
	  {
	    NurbsMapping & newSubCurve = intersectionCurve.subCurve(qq);
	    lastTrimCurve.merge(newSubCurve); 
	  }
	  
// invalidate trimming to force the user to edit the trim curve
	  trim_->invalidateTrimming();
	}
	else if (nTrim > 0)
	{
	  printf("Sorry, but the last trim curve (#%d) is a %s. Can't add the intersection curve!\n",
		 nTrim,SC trim_->trimCurves[nTrim-1]->getClassName());
	  rc = false;
	}
	else
	{
	  printf("Error: This trimmed mapping does not have ANY trim curves!!!\n");
	  rc = false;
	}
	      
      }
    } // end for i...
	  
  }
  else
  {
    gi.createMessageDialog("You cannot intersect a mapping with itself! Please reselect the mappings.",
			   errorDialog);
    rc = false;
  }
  return rc;
}

	


static bool
aStringFromIntArray(ArraySimple<int> & v, int nInts, aString & buf)
{
  aString subString;
  int i;
  if (nInts <= v.size()) // make sure we are not trying to save more data than we have
  {
    for (i=0; i<nInts; i++)
    {
      sPrintF(subString, " %i", v(i));
      buf += subString; // concatenate the string
    }
    buf += "\n"; // final newline
  }
  else
  {
    printf("ERROR: aStringFromIntArray: Vector.size() < nInts\n");
    return false;
  }
  return true;
}


static bool
intArrayFromAString(aString & answer, int len, ArraySimple<int> & A)
{
  int last=answer.length()-1, nInts, i;
  const char * cs = answer;
  len += strspn(&cs[len], " \t"); // skip white space after the command
//      printf("Remaining string: `%s'\n", &cs[len]);
  if (len <= last && sscanf(&cs[len], "%i", &nInts) == 1 && nInts > 0)
  {
//	printf("Number of int: %i\n", nInts);
    A = ArraySimple<int>(nInts+1);
    A(0) = nInts;
    for (i=1; i<=nInts; i++)
    {
      len += strcspn(&cs[len], " \t"); // skip the number just read
      len += strspn(&cs[len], " \t"); // skip white space after number
//	  printf("Remaining string: `%s'\n", &cs[len]);
      A(i) = -1;
      if (!(len <= last && sscanf(&cs[len], "%i", &A(i)) == 1))
      {
	printf("Error in intArrayFromAString: could not read integer # %i\n", i);
	return false;
      }
    } // end for i...
  }
  else
  {
    printf("Error in intArrayFromAString: invalid number of integers: %i\n", nInts);
    return false;
  }
  return true;
}

// AP debug
static void
printBB(CompositeSurface & model)
{
// loop over all surfaces
  int s;
  for (s=0; s<model.numberOfSubSurfaces(); s++)
  {
    for( int axis=0; axis<model.getRangeDimension(); axis++ )
    {
      printf("printBB: surface=%i, axis=%i, bounds=[%e, %e]\n", s, axis,
	     (real) model[s].getRangeBound(Start,axis), (real) model[s].getRangeBound(End,axis) );
      
    } // end for axis
  } // end for numberOfSurfaces
}

//\begin{>>ModelBuilderInclude.tex}{\subsection{editModel}}
void ModelBuilder::
editModel(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & deletedSurfaces,
	     ListOfMappingRC &curveList, PointList & points)
//===========================================================================
// /Description:
//    Edit a model.
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  assert(mapInfo.gp_!=NULL);
  GraphicsParameters & par = *mapInfo.gp_;
  GraphicsParameters splinePar;
  splinePar.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  splinePar.set(GI_PLOT_MAPPING_EDGES, TRUE); // do plot the edges
  splinePar.set(GraphicsParameters::curveLineWidth,2.);
  splinePar.set(GI_PLOT_GRID_POINTS_ON_CURVES, false);
  splinePar.set(GI_PLOT_END_POINTS_ON_CURVES, true);

  bool plotCurves=true;

  int i, j, axes;
  int plotShadedMappingBoundaries, plotLinesOnMappingBoundaries, plotMappingEdges, plotTitleLabels,
    plotSquares;
  par.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedMappingBoundaries);
  par.get(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnMappingBoundaries);
  par.get(GI_PLOT_MAPPING_EDGES, plotMappingEdges);
  par.get(GI_PLOT_LABELS, plotTitleLabels);
  par.get(GI_LABEL_GRIDS_AND_BOUNDARIES, plotSquares);

  GUIState interface;
  aString buf;
  
  interface.setWindowTitle("Edit model");
  
  interface.setExitCommand("close", "Close");

  aString prefix="HEAL:";
  enum PickEnum { noOp=0, buildPoint, buildCurve, surfaceEdge, buildPlane, intersectSurf, selectCurve,
		  projectCurve, 
		  projectSurface, editTrimCurve, deleteTrimCurve, deleteSurface, 
		  undeleteSurface, hideSurface, showSurface, 
		  examineSurface, numberOfSelectionFunctions };
  static PickEnum selectionFunction=noOp;
  
// ordering of the text labels
  static int mnIndex, ptIndex; 
  
// define layout of option menus
  interface.setOptionMenuColumns(1);

// first option menu
  aString opCommand4[] = {"selection function NoOp",           "selection function Build Point", 
			  "selection function Interpolate Curve",
			  "selection function Surface Edge",     "selection function Build Plane",
			  "selection function Intersect Surfaces", "selection function Select Curve",
			  "selection function Project Curve", "selection function Project Surface",
			  "selection function Edit Trimcurve", "selection function Delete Trimcurve", 
			  "selection function Delete Surface", "selection function Undelete Surface", 
			  "selection function Hide Surface",   "selection function Show Surface", 
			  "selection function Examine Surface",""};
  aString opLabel4[] = {"No operation",    "Build Point", 
			"Interpolate Curve",
			"Surface Edge",    "Build Plane", 
			"Intersect Surf.", "Select Curve",
			"Project Curve",   "Project Surface", 
			"Edit Trimcurve",  "Delete Trimcurve", 
			"Delete Surface",  "Undelete Surface", 
			"Hide Surface",    "Show Surface", 
			"Examine Surface", "" };

// initial choice: noOp
  addPrefix(opCommand4, prefix);
  interface.addRadioBox( "Selection Function", opCommand4, opLabel4, selectionFunction, 3); // 3 columns
  RadioBox &rBox = interface.getRadioBox(0);

// toggle buttons
  enum TBEnum {plotShadedTB=0, plotLinesTB, plotEdgesTB, plotCurvesTB  };

  aString tbLabels[] = {"Shade", "Grid", "Boundary", "Curves", ""};
  aString tbCommands[] = {"plot shaded surfaces (3D) toggle",
			  "plot grid lines on boundaries (3D) toggle",
			  "plot sub-surface boundaries (toggle)",
			  "plot curves (toggle)",
			  ""};

  int tbState[] = {plotShadedMappingBoundaries, 
		   plotLinesOnMappingBoundaries, 
		   plotMappingEdges,
		   plotCurves
  };
    
  addPrefix(tbCommands, prefix);
  interface.setToggleButtons(tbCommands, tbLabels, tbState, 4); // organize in 4 columns
// done defining toggle buttons

  enum PBEnum {clearAllPoints=0, clearLastPoint, clearAllEdges, clearLastEdge, buildSurface,
	       deleteLastSurface}; // these are the buttons that sometimes are made insensitive

// setup a user defined menu and some user defined buttons
  aString buttonCommands[] = {"clear all points",
			      "clear last point",
			      "clear all edges",
			      "clear last edge",
			      "build tfi",
			      "delete last mapping",
			      "plotObject", 
			      "erase",
			      "unhide all sub-surfaces", 
			      "hide all sub-surfaces", 
			      "show broken sub-surfaces", 
			      "hide broken sub-surfaces", 
			      "show valid sub-surfaces", 
			      "hide valid sub-surfaces", 
			      "delete hidden sub-surfaces",
			      "undelete trimcurve",
			      ""};
  aString buttonLabels[] = {"Clear All Points",
			    "Clear Last Point",
			    "Clear All Curves",
			    "Clear Last Curve",
			    "Build Surface",
			    "Delete Last Surface",
			    "Replot",
			    "Erase",
			    "Show all",
			    "Hide all",
			    "Show broken",
			    "Hide broken",
			    "Show valid",
			    "Hide valid",
			    "Delete hidden",
			    "Undelete trimcurve",
			    ""};
  
  addPrefix(buttonCommands, prefix);
  interface.setPushButtons(buttonCommands, buttonLabels, 6); // six rows

  enum WindowButtons
  {
    doneWB=0,
    numberOfWB
  };
  
  aString windowButtons[][2] = {{"build edge", "Done"},
				{"", ""}};
  interface.setUserButtons(windowButtons);
  
// setup textlabels
  aString textCmd[] = {"mappingName", "newPoint", ""};
  aString textLbl[] = {"Name:", "New Point: (x, y, z)", ""};
  aString textInit[3]; // don't forget to update this size when adding/deleting commands
  int cnt=0;
  mnIndex = cnt; textInit[cnt++] = model.getName(Mapping::mappingName);  // mapping name
  ptIndex = cnt; sPrintF(textInit[cnt++],"%g %g %g", 0., 0., 0.);           // new point
  textInit[cnt++] = "";

  addPrefix(textCmd, prefix);
  interface.setTextBoxes(textCmd, textLbl, textInit); 

// define pulldown menus
  aString pdCommand2[] = {"help selection function", ""};
  aString pdLabel2[] = {"Selection function", ""};
  interface.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  interface.setLastPullDownIsHelp(true);
// done defining pulldown menus  

// make info labels with the number of broken surfaces and the total number of surfaces
  int numberOfBroken=0;
  for (int s=0; s<model.numberOfSubSurfaces(); s++)
  {
    if (model[s].getClassName() == "TrimmedMapping")
    {
      TrimmedMapping &trim = (TrimmedMapping &) model[s];
      if (!trim.trimmingIsValid())
      {
	numberOfBroken++;
      }
    }
  }
  int nosLabel = interface.addInfoLabel(sPrintF(buf,"There are %i component surfaces in the model", 
						model.numberOfSubSurfaces()));
  int brokenLabel = interface.addInfoLabel(sPrintF(buf,"of which %i are broken trimmed surfaces", 
						   numberOfBroken));
  int oldNoS = model.numberOfSubSurfaces();
  int oldNoB = numberOfBroken;

  gi.pushGUI(interface);
  
  ViewLocation loc;
  int edges[4];
  int nEdges=0;
  edges[0] = edges[1] = edges[2] = edges[3] = -1;
  aString edgeColour[]= {"red", "green", "magenta", "yellow"};

  static int planePoints[3];
  static int nPlanePoints=0;
  const int maxEdgePoints=100;
  static int edgePoints[maxEdgePoints];
  static int nEdgePoints=0;
  
  TrimmedMapping * lastTrimmedMapping = NULL;
  IntersectionMapping *interSect_=NULL;
  Mapping *iMap_[2] = {NULL, NULL};
  int nIMap=0;
  

  int retCode;
  bool plotObject;
  aString answer;
  SelectionInfo select;
  
  for(;;)
  {
// set the sensitivity of some buttons
// make "Done" button sensitive only when mouseMode == interpolateCurve
    gi.setUserButtonSensitive(doneWB, selectionFunction == buildCurve); 
// points
    interface.setSensitive((nEdgePoints>0), DialogData::pushButtonWidget, clearAllPoints);
    interface.setSensitive((nEdgePoints>0), DialogData::pushButtonWidget, clearLastPoint);
// edges
    interface.setSensitive((nEdges>0), DialogData::pushButtonWidget, clearAllEdges);
    interface.setSensitive((nEdges>0), DialogData::pushButtonWidget, clearLastEdge);
// surfaces
    interface.setSensitive((nEdges==2 || nEdges==4), DialogData::pushButtonWidget, buildSurface); // build tfi
    interface.setSensitive(model.numberOfSubSurfaces()>0, DialogData::pushButtonWidget, deleteLastSurface);

    rBox.setSensitive(projectCurve, (nEdges>0));

// count the number of broken surfaces
    int numberOfBroken=0;
    for (int s=0; s<model.numberOfSubSurfaces(); s++)
    {
      if (model[s].getClassName() == "TrimmedMapping")
      {
	TrimmedMapping &trim = (TrimmedMapping &) model[s];
	if (!trim.trimmingIsValid())
	{
	  numberOfBroken++;
	}
      }
    }
// change the labels
    if (oldNoS != model.numberOfSubSurfaces())
    {
      interface.setInfoLabel(nosLabel, sPrintF(buf,"There are %i component surfaces in the model", 
					       model.numberOfSubSurfaces()));
      oldNoS = model.numberOfSubSurfaces();
    }
    if (oldNoB != numberOfBroken)
    {
      interface.setInfoLabel(brokenLabel,sPrintF(buf,"of which %i are broken trimmed surfaces", 
						 numberOfBroken));
      oldNoB = numberOfBroken;
    }
    
    plotObject=true;

    gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

    retCode = gi.getAnswer(answer, "", select);

    gi.savePickCommands(true); // restore

// take off the prefix
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);
 
//                     01234567890123456789
    int len;
    if( (len=answer.matches("selection function")) )
    {
      PickEnum s = noOp, oldFunction=selectionFunction;
      aString mode = (answer.length() > len+2)? answer(len+1,answer.length()-1): (aString)"";
      
      if (mode.matches("NoOp"))
      {
	selectionFunction = noOp;
	gi.erase();
      }
      else if (mode.matches("Build Point"))
	selectionFunction = buildPoint;
      else if (mode.matches("Interpolate Curve"))
	selectionFunction = buildCurve;
      else if (mode.matches("Surface Edge"))
	selectionFunction = surfaceEdge;
      else if (mode.matches("Build Plane"))
	selectionFunction = buildPlane;
      else if (mode.matches("Intersect Surfaces"))
	selectionFunction = intersectSurf;
      else if (mode.matches("Select Curve"))
	selectionFunction = selectCurve;
      else if (mode.matches("Project Curve"))
	selectionFunction = projectCurve;
      else if (mode.matches("Project Surface"))
	selectionFunction = projectSurface;
      else if (mode.matches("Edit Trimcurve"))
	selectionFunction = editTrimCurve;
      else if (mode.matches("Delete Trimcurve"))
	selectionFunction = deleteTrimCurve;
      else if (mode.matches("Delete Surface"))
	selectionFunction = deleteSurface;
      else if (mode.matches("Undelete Surface"))
	selectionFunction = undeleteSurface;
      else if (mode.matches("Hide Surface"))
	selectionFunction = hideSurface;
      else if (mode.matches("Show Surface"))
	selectionFunction = showSurface;
      else if (mode.matches("Examine Surface"))
	selectionFunction = examineSurface;
      else
	gi.outputString(sPrintF(buf, "unknown selection function `%s'", SC mode));

//      sScanF(answer(18,answer.length()-1),"%d",&s);
      if ( noOp <= selectionFunction && selectionFunction < numberOfSelectionFunctions )
      {
	if (rBox.setCurrentChoice(selectionFunction))
	{
	  gi.outputString(sPrintF(buf, "selection function %s (#%d)", SC opLabel4[selectionFunction],
				  selectionFunction));
	}
	else
	{
	  gi.outputString(sPrintF(buf, "ERROR: selection function %s (#%d) is inactive", 
				  SC opLabel4[selectionFunction], selectionFunction));
	  selectionFunction = noOp;
	}
	
      }
      else
      {
	gi.outputString(sPrintF(buf, "Error: Bad selection function: %d", selectionFunction));
      }
      plotObject = true; // necessary to replot if switching to/from undeleteSurface
// hide display lists for the deleted surfaces
      if (oldFunction == undeleteSurface && selectionFunction != undeleteSurface)
      {
	gi.erase(); // erase the deleted surfaces
      }
      
    }
    else if( select.nSelect > 0 && selectionFunction == noOp )
    {
      continue;
    }
    else if( select.nSelect > 0 && selectionFunction == surfaceEdge )
    {
      
// AP debug
      printf("\nBefore surfaceEdge:\n");
//        printBB(model);
      int s;
      Edge *bCurve_ = getClosestCurve(s, model, select, gi, true);
      if (bCurve_)
      {
	MappingRC splineRC(*(bCurve_->spline_));
	curveList.addElement(splineRC); // increments reference count for  bCurve->spline_

// decrements bCurve_->spline_
	delete  bCurve_;
	
	gi.outputString("Made a NEW curve");

// save equivalent command
	gi.outputToCommandFile(sPrintF(buf, "surface edge %i %e %e %e\n", s, 
				       select.x[0], select.x[1], select.x[2]));
      }
    }
    else if( select.nSelect > 0 && selectionFunction == buildPoint )
    {
// find the closest (in terms of z-buff coord) underlying subsurface
      bool foundSurface=false;
      int s;
      for( i=0; i<select.nSelect; i++ )
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
	if (foundSurface) break;
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
      plotObject = true;
      
// save equivalent command
      gi.outputToCommandFile(sPrintF(buf, "HEAL:newPoint %e %e %e\n", newPoint.coordinate[0], 
				     newPoint.coordinate[1], newPoint.coordinate[2]));
    }
    else if( select.nSelect > 0 && selectionFunction == buildPlane )
    {
      if (nPlanePoints<3)
      {
// search the selection for a point
	bool found=false;
	for( i=0; i<select.nSelect && !found; i++ )
	{
	  for( j=0; j<points.size() && !found; j++ )
	  {
	    if( points[j].getGlobalID() == select.selection(i,0) )
	    {
	      gi.outputString(sPrintF(buf, "Point #%i selected", j));
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
      if (nPlanePoints == 3)
      {
// plot the green square for the last point
	par.set(GI_POINT_SIZE, 6);
	par.set(GI_POINT_COLOUR, "green");
	for (i=0; i<nPlanePoints; i++)
	  points[planePoints[i]].plot(gi, par);
	gi.redraw(true);

	real planeCoordinates[3][3];
	for (i=0; i<3; i++)
	  for (axes=0; axes<3; axes++)
	    planeCoordinates[i][axes] = points[planePoints[i]].coordinate[axes];
	
	addPlaneToModel(planeCoordinates, nPlanePoints, model, gi);
      }

    }
    else if( select.nSelect > 0 && selectionFunction == buildCurve )
    {
// search the selection for a point
      bool found=false;
      for( i=0; i<select.nSelect && !found; i++ )
	for( j=0; j<points.size() && !found; j++ )
	  if( points[j].getGlobalID() == select.selection(i,0) )
	  {
	    gi.outputString(sPrintF(buf, "Point #%i selected", j));
	    edgePoints[nEdgePoints++] = j;
	    found = true; // breaks out of the outer loop
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "point for edge %i\n", j));
	  }
    }
    
    else if( select.nSelect > 0 && selectionFunction == deleteTrimCurve )
    {
      int s;
      Edge *bCurve_ = getClosestCurve(s, model, select, gi);
      if (bCurve_)
      {
	if (bCurve_->mappingIsTrimmed)
	{
	  int s=bCurve_->subSurface;
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
// remember where the last trim curve was deleted, so we can undelete
	  lastTrimmedMapping = &trim;
	  trim.deleteTrimCurve(bCurve_->trimCurve);
// after delete and add operations, trimmingIsValid can be out of sync
	  trim.validateTrimming();
// delete the display lists so that sub-surface s will get replotted properly
	  model.eraseCompositeSurface(gi, s);
	  plotObject=true;
// save equivalent command
	  gi.outputToCommandFile(sPrintF(buf, "delete trim curve %i %i\n", 
					 bCurve_->trimCurve, bCurve_->subSurface));
	}
	else
	  gi.outputString("The mapping is NOT trimmed!");
	
// cleanup	
	delete bCurve_;
      }
    }

    else if( select.nSelect > 0 && selectionFunction == editTrimCurve )
    {
      int s;
      Edge *bCurve_ = getClosestCurve(s, model, select, gi);
      if (bCurve_)
      {
	if (bCurve_->mappingIsTrimmed)
	{
	  int s=bCurve_->subSurface;
	  gi.outputString(sPrintF(buf,"Editing trim curve %i on sub-surface %i",
				  bCurve_->trimCurve, s));
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  Mapping *curve = trim.getTrimCurve(bCurve_->trimCurve);
	  if (curve)
	  {
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "edit trim curve %i %i\n", 
					   bCurve_->trimCurve, bCurve_->subSurface));
	    
	    gi.getView(loc); // get the viewpoint info
	    trim.editTrimCurve(*curve, mapInfo);
	    gi.setView(loc); // reset the view point
	    gi.redraw();
// delete the display lists so that sub-surface s will get replotted properly
	    model.eraseCompositeSurface(gi, s);
	    plotObject=true;
	  }
	  else
	  {
	    gi.createMessageDialog(sPrintF(buf, "Sorry, trim curve %i is not available", 
					   bCurve_->trimCurve), errorDialog);
	  }
	}
	else
	  gi.outputString("The mapping is NOT trimmed!");
// cleanup	
	delete bCurve_;
      }
    }
    
    else if( select.nSelect > 0 && selectionFunction == intersectSurf )
    {
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);

      for( i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	{ 
// the first surface (nIMap == 0) can be any surface, but the second surface must not be the same
	  if( model[s].getGlobalID() == select.selection(i,0) && (nIMap == 0 || iMap_[0] != &model[s]) )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' selected for intersection", 
				    s, SC model[s].getName(Mapping::mappingName)));
	    iMap_[nIMap] = &model[s];
	    nIMap++;
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "surface for intersection %i\n", s));
	    break;
	  }
	}
	if (singleSelect || nIMap >= 2) break;
      } 
// compute the intersection
      if (nIMap == 2)
      {
	intersectTwoSurfaces(model, deletedSurfaces, gi, iMap_[0], iMap_[1], interSect_);
// AP: should add the intersection curve to curveList...
// reset pointers and counter
	iMap_[0] = NULL;
	iMap_[1] = NULL;
	nIMap = 0;
      }
    }
    else if( select.nSelect > 0 && selectionFunction == selectCurve)
    {
      if (nEdges < 4)
      {
// loop to find the curve...
	bool foundCurve = false;
	for ( int s=0; s<select.nSelect && !foundCurve; s++ )
	  for ( i=0; i<curveList.getLength() && !foundCurve; i++ )
	    if ( curveList[i].getMapping().getGlobalID()==select.selection(s,0) )
	    {
	      foundCurve = true;
	      edges[nEdges++] = i;
	      gi.outputString(sPrintF(buf,"Selected curve #%i", i));
	    }
// save equivalent command
	if (foundCurve)
	  gi.outputToCommandFile(sPrintF(buf, "select curve %i\n", edges[nEdges-1]));
      }
      else
	gi.outputString("You have already selected 4 curves. Clear at least one curve before selecting the next one");
    }
    else if( select.nSelect > 0 && selectionFunction == projectCurve)
    {
      Mapping *projectMap_ = NULL;
      int s=-1;
      
      for( i=0; i<select.nSelect; i++ )
      {
	for( s=0; s<model.numberOfSubSurfaces(); s++ )
	{
	  if( model[s].getGlobalID() == select.selection(i,0) )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface `%s' selected for projection", 
				    SC model[s].getName(Mapping::mappingName)));
	    projectMap_ = &model[s];
	    break;
	  }
	}
	if (projectMap_) break;
      }
      if (!projectMap_)
      {
	gi.outputString("Error: No surface selected for projection!");
	continue;
      }
      if (nEdges <= 0)
      {
	gi.outputString("Error: You must select an edge before projecting! (This should never happen)");
	continue;
      }
      Mapping & curve = curveList[edges[nEdges-1]].getMapping();
// make a copy so we can reshape without worrying
// kkc this does not actually make a copy      realArray x = curve.getGrid();
      realArray x;
      x = curve.getGrid(); // kkc but this does :)

      //
      x.reshape(x.dimension(0),x.dimension(3));
      projectPointsOnSurface(x, s, model, deletedSurfaces, gi);
// save equivalent command
      gi.outputToCommandFile(sPrintF(buf, "project edge %i %i\n", nEdges-1, s));
    }
    else if( select.nSelect > 0 && selectionFunction == projectSurface )
    {
      bool singleSelect = true; // only project one surface at the time
      int nSurfaces=0;
      int s;
      for( i=0; i<select.nSelect; i++ )
      {
	for( s=0; s<model.numberOfSubSurfaces(); s++ )
	{
// can't project trimmed mappings
	  if( model[s].getGlobalID() == select.selection(i,0) && model[s].getClassName() != "TrimmedMapping" )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be projected", 
				    s, SC model[s].getName(Mapping::mappingName)));
	    nSurfaces++;
	    if (singleSelect)
	      break;
	  }
	}
	if (singleSelect && nSurfaces>0) break;
      } // end for i...
      
      if (nSurfaces != 1)
	gi.createMessageDialog("Sorry, it is only possible to project ONE UNTRIMMED mapping", informationDialog);
      else
      {
	projectSurfaceOnModel(s, model, gi);
// save equivalent command
	gi.outputToCommandFile(sPrintF(buf,"project surface %i\n", s));
      }
      
    }
    else if( select.nSelect > 0 && selectionFunction == deleteSurface )
    {
// AP debug
//        printf("Before deleting a surface\n");
//        printBB(model);

      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);

// save the list of surfaces for the equivalent command
      ArraySimple<int> surfaces(select.nSelect+1); 
      int nSurfaces = 0;

      for( i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	{
	  if( model[s].getGlobalID() == select.selection(i,0) &&
	      model.isVisible(s) )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be deleted", 
				    s, SC model[s].getName(Mapping::mappingName)));
// delete any displaylists
	    model.eraseCompositeSurface(gi, s);
// add the surface to the collection of deleted surfaces
	    deletedSurfaces.add(model[s], model.getSurfaceID(s));
	    deletedSurfaces.setColour(deletedSurfaces.numberOfSubSurfaces()-1, "gray50");
// remove the surface from the model
	    model.remove(s);
// save list of undeleted surfaces
	    surfaces(1 + nSurfaces++) = s;
	    if (singleSelect)
	      break;
	  }
	}
	if (singleSelect && nSurfaces>0) break;
      } // end for i...
      
// save equivalent command
      surfaces(0) = nSurfaces;
      buf = "delete surfaces";
      if (surfaces(0) > 0 && aStringFromIntArray(surfaces, nSurfaces+1, buf))
	gi.outputToCommandFile(buf);
// AP debug
//        printf("After deleting a surface\n");
//        printBB(model);
    }
    else if( select.nSelect > 0 && selectionFunction == undeleteSurface )
    {
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);

// save the list of surfaces for the equivalent command
      ArraySimple<int> surfaces(select.nSelect+1); 
      int nSurfaces = 0;

      for( i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<deletedSurfaces.numberOfSubSurfaces(); s++ )
	{
	  if( deletedSurfaces[s].getGlobalID() == select.selection(i,0) )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be UNdeleted",
				    s, SC deletedSurfaces[s].getName(Mapping::mappingName)));
// delete any displaylists
	    deletedSurfaces.eraseCompositeSurface(gi, s);
// add the surface to the model
	    model.add(deletedSurfaces[s], deletedSurfaces.getSurfaceID(s));
	    model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
// remove the surface from the collection of deleted surfaces
	    deletedSurfaces.remove(s);
// save list of undeleted surfaces
	    surfaces(1 + nSurfaces++) = s;
	    if (singleSelect)
	      break;
	  }
	}
	if (singleSelect && nSurfaces>0) break;
      }
// save equivalent command
      surfaces(0) = nSurfaces;
      buf = "undelete surfaces";
      if (surfaces(0) > 0 && aStringFromIntArray(surfaces, nSurfaces+1, buf))
	gi.outputToCommandFile(buf);
    }
    else if( select.nSelect > 0 && selectionFunction == hideSurface )
    {
// hide the selected sub-surface(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);

// save the list of surfaces for the equivalent command
      ArraySimple<int> surfaces(select.nSelect+1); 
      int nSurfaces = 0;
      
      for( i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == model[s].getGlobalID() && model.isVisible(s))
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s', ID=%i, will be hidden",s,
				    SC model[s].getName(Mapping::mappingName), model.getSurfaceID(s)));
	    model.setIsVisible(s, FALSE);
	    surfaces(1 + nSurfaces++) = s;
	    if (singleSelect)
	      break;
	  }
	}
	if (singleSelect && nSurfaces>0) break;
      } // end for i...
      surfaces(0) = nSurfaces;
      
// save equivalent command
      buf = "hide surfaces";
      if (surfaces(0) > 0 && aStringFromIntArray(surfaces, nSurfaces+1, buf))
	gi.outputToCommandFile(buf);
    }
    else if( select.nSelect > 0 && selectionFunction == showSurface )
    {
// show the selected sub-surface(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);

// save the list of surfaces for the equivalent command
      ArraySimple<int> surfaces(select.nSelect+1); 
      int nSurfaces = 0;

      for( i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == model[s].getGlobalID() && !model.isVisible(s) )
	  {
	    gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be shown",s,
				    SC model[s].getName(Mapping::mappingName)));
	    model.setIsVisible(s, TRUE);
	    surfaces(1 + nSurfaces++) = s;
	    if (singleSelect)
	      break;
	  }
	}
	if (singleSelect && nSurfaces>0) break;
      }
      
// save equivalent command
      surfaces(0) = nSurfaces;
      buf = "show surfaces";
      if (surfaces(0) > 0 && aStringFromIntArray(surfaces, nSurfaces+1, buf))
	gi.outputToCommandFile(buf);
    }
    else if( select.nSelect > 0 && selectionFunction == examineSurface )
    {
// save the surface number
      int mappingToExamine=-1;

// examine the closest sub-surface
      for( i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<model.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0)  == model[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Examining sub-surface %i named `%s'", s, 
				    SC model[s].getName(Mapping::mappingName)));
	    mappingToExamine = s;
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "examine surface %i\n", mappingToExamine));

	    gi.getView(loc); // get the viewpoint info
	    gi.erase();
	    par.set(GI_TOP_LABEL,sPrintF(buf,"sub-surface %i", s));
	    par.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	    Mapping *map = &model[s];  // make a pointer so virtual function call works.
	    map->update( mapInfo );
	    gi.setView(loc); // reset the view point
	    gi.redraw();
// delete the display lists so that sub-surface s will get replotted properly
	    model.eraseCompositeSurface(gi, s);
	    par.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	    par.set(GI_TOP_LABEL,model.getName(Mapping::mappingName));
	    plotObject=true;
	    break; // only examine one surface!
	  } // end if
	}
	if (mappingToExamine != -1) break;
      } // end for i...

    }
    
    else if( answer.matches("count broken") )
    {
// count the number of broken surfaces
      int numberOfBroken=0;
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
      {
	if (model[s].getClassName() == "TrimmedMapping")
	{
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  if (!trim.trimmingIsValid())
	  {
	    numberOfBroken++;
	  }
	}
      }
      gi.outputString(sPrintF(buf, "There are %i broken sub-surfaces", numberOfBroken));
    }
    else if( answer(0,20)=="hide all sub-surfaces" )
    {
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
	model.setIsVisible(s, FALSE);
    }
    else if( answer=="unhide all sub-surfaces" )
    {
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
	model.setIsVisible(s, TRUE);
    }
    else if( answer=="hide broken sub-surfaces" )
    {
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
      {
	if (model[s].getClassName() == "TrimmedMapping")
	{
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  if (!trim.trimmingIsValid())
	  {
	    model.setIsVisible(s, FALSE);
	  }
	}
      }
    }
    else if( answer=="show broken sub-surfaces" )
    {
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
      {
	if (model[s].getClassName() == "TrimmedMapping")
	{
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  if (!trim.trimmingIsValid())
	  {
	    model.setIsVisible(s, TRUE);
	  }
	}
      }
    }
    else if( answer=="show valid sub-surfaces" )
    {
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
      {
	if (model[s].getClassName() == "TrimmedMapping")
	{
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  if (trim.trimmingIsValid())
	    model.setIsVisible(s, TRUE);
	}
	else // untrimmed surfaces
	  model.setIsVisible(s, TRUE);
      }
    }
    else if( answer=="hide valid sub-surfaces" )
    {
      for (int s=0; s<model.numberOfSubSurfaces(); s++)
      {
	if (model[s].getClassName() == "TrimmedMapping")
	{
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  if (trim.trimmingIsValid())
	    model.setIsVisible(s, FALSE);
	}
	else // untrimmed surfaces can't be broken
	  model.setIsVisible(s, FALSE);
      }
    }
    else if( answer=="undelete trimcurve" )
    {
      if (lastTrimmedMapping)
      {
	lastTrimmedMapping->undoLastDelete();
// compute the subSurface number by comparing pointers
	int s = -1, ss;
	for (ss=0; ss<model.numberOfSubSurfaces(); ss++)
	  if (lastTrimmedMapping == &(model[ss]))
	    s=ss;
	
	lastTrimmedMapping = NULL; // can only undelete the last trimming curve 
// delete the display lists so that sub-surface s will get replotted properly
	if (s>=0)
	{
	  model.eraseCompositeSurface(gi, s);
	  plotObject=true;
	}
	
      }
      else
      {
	gi.createMessageDialog("You can only un-delete the last deleted trim curve.",
			       errorDialog);
      }
      
    }
    else if( answer=="delete hidden sub-surfaces" )
    {
      for( int s=model.numberOfSubSurfaces()-1; s>=0; s-- )
      {
        if( !model.isVisible(s) )
	{
	  gi.outputString(sPrintF(buf, "Sub-surface `%s' will be deleted", 
				  SC model[s].getName(Mapping::mappingName)));
// delete any displaylists
	  model.eraseCompositeSurface(gi, s);
// add the surface to the collection of deleted surfaces
	  deletedSurfaces.add(model[s], model.getSurfaceID(s));
	  deletedSurfaces.setColour(deletedSurfaces.numberOfSubSurfaces()-1, "gray50");
// remove the surface from the model
	  model.remove(s);
	}
	
      }
    }
    else if (answer == "clear all points")
    {
      nEdgePoints = 0;
      nPlanePoints = 0;
    }
    else if (answer == "clear last point")
    {
      if (selectionFunction == buildCurve && nEdgePoints > 0)
	nEdgePoints--;
      else if (selectionFunction == buildPlane && nPlanePoints > 0)
  	nPlanePoints--;
      
      plotObject = true;
    }
    else if (answer.matches("build edge"))
    {
// the edge will interpolate all points in the edgePoint list and then clear that list.
      if (nEdgePoints>=2)
      {
	SplineMapping * spline_ = new SplineMapping;
	spline_->incrementReferenceCount();
// make a reference (for convenience)	
	SplineMapping &spline = *spline_;

	Range I=nEdgePoints;
	realArray x(I,3);
// fill in the coordinates
	for (i=0; i<nEdgePoints; i++)
	{
	  x(i,0) = points[edgePoints[i]].coordinate[0];
	  x(i,1) = points[edgePoints[i]].coordinate[1];
	  x(i,2) = points[edgePoints[i]].coordinate[2];
	}
// initialize the spline
	spline.setPoints( x(I,0), x(I,1), x(I,2) );

// clear all points
	nEdgePoints = 0;

// add the spline to the global list of curves
	MappingRC splineRC(spline);
	curveList.addElement(splineRC);

	plotObject = true;
      }
      else
      {
	gi.createMessageDialog("You need at least 2 points to build an edge.", errorDialog);
      }
    }
    else if (answer == "clear all edges")
    {
      for (i=0; i<4; i++)
	edges[i] = -1;
      nEdges = 0;
      plotObject = true;
    }
    else if (answer == "clear last edge")
    {
      if (nEdges>0)
      {
	nEdges--;
	plotObject = true;
      }
      else
      {
	gi.createMessageDialog("There are no edges to clear.", errorDialog);
      }
    }
    else if (answer == "build tfi")
    {
      if (nEdges == 2 || nEdges == 4)
      {
	TFIMapping * newPatch_;
//	newPatch_ = new TFIMapping(edges[0], edges[1], edges[2], edges[3]);
	if (nEdges == 2)
	  newPatch_ = new TFIMapping(&curveList[edges[0]].getMapping(), &curveList[edges[1]].getMapping());
	else if (nEdges == 4)
	  newPatch_ = new TFIMapping(&curveList[edges[0]].getMapping(), &curveList[edges[1]].getMapping(),
				     &curveList[edges[2]].getMapping(), &curveList[edges[3]].getMapping());
	newPatch_->incrementReferenceCount();
      
	model.add( *newPatch_, -1 );
	model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
	gi.outputString(sPrintF(buf, "Adding mapping `%s'", SC newPatch_->getName(Mapping::mappingName)));
// remove the edges
  	for (i=0; i<4; i++)
  	{
	  edges[i] = -1;
	}
	nEdges = 0;
	plotObject = true;
      }
      else
      {
	gi.createMessageDialog("You must select 2 or 4 edges before building a TFI mapping", errorDialog);
      }
    }
    else if (answer == "delete last mapping")
    {
      int s = model.numberOfSubSurfaces()-1;
      if (s >=0)
      {
	gi.outputString(sPrintF(buf, "Sub-surface `%s' will be deleted", 
				SC model[s].getName(Mapping::mappingName)));
// delete any displaylists
	model.eraseCompositeSurface(gi, s);
// add the surface to the collection of deleted surfaces
	deletedSurfaces.add(model[s], model.getSurfaceID(s));
	deletedSurfaces.setColour(deletedSurfaces.numberOfSubSurfaces()-1, "gray50");
// remove the surface from the model
	model.remove(s);
      }
      else
      {
	gi.createMessageDialog("There are no sub-surfaces to delete", errorDialog);
      }
    }
    else if (answer == "close")
    {
      break;
    }
    else if( answer=="plotObject" )
    {
// purge all display lists
      model.eraseCompositeSurface(gi);
      model.recomputeBoundingBox();
      plotObject = true;
    }
    else if( answer=="erase" )
    {
// purge all display lists
      model.eraseCompositeSurface(gi);
      gi.erase();
      plotObject = false;
    }
    else if( (len = answer.matches("hide surfaces")) ) 
    {
// syntax for hiding the 5 surfaces 23, 4, 321, 1, and 19 is: `hide surfaces 5 23 4 321 1 19'
      ArraySimple<int> surfs;
      if (intArrayFromAString(answer, len, surfs))
      {
	for (i=1; i<=surfs[0]; i++)
	{
	  gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be hidden", surfs[i],
				  SC model[surfs[i]].getName(Mapping::mappingName)));
	  model.setIsVisible(surfs[i], FALSE);
	}
      }      
    }
    else if( (len = answer.matches("show surfaces")) )
    {
      ArraySimple<int> surfs;
      if (intArrayFromAString(answer, len, surfs))
      {
	for (i=1; i<=surfs[0]; i++)
	{
	  gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be shown", surfs[i],
				  SC model[surfs[i]].getName(Mapping::mappingName)));
	  model.setIsVisible(surfs[i], TRUE);
	}
      }      
    }
    else if( (len = answer.matches("delete surfaces")) )
    {
      ArraySimple<int> surfs;
      if (intArrayFromAString(answer, len, surfs))
      {
	for (i=1; i<=surfs[0]; i++)
	{
	  gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be deleted", 
				  surfs(i), SC model[surfs(i)].getName(Mapping::mappingName)));
// delete any displaylists
	  model.eraseCompositeSurface(gi, surfs(i));
// add the surface to the collection of deleted surfaces
	  deletedSurfaces.add(model[surfs(i)], model.getSurfaceID(surfs(i)));
	  deletedSurfaces.setColour(deletedSurfaces.numberOfSubSurfaces()-1, "gray50");
// remove the surface from the model
	  model.remove(surfs(i));
	}
      } // end if...
    }
    else if( (len = answer.matches("undelete surfaces")) )
    {
      ArraySimple<int> surfs;
      if (intArrayFromAString(answer, len, surfs))
      {
	for (i=1; i<=surfs[0]; i++)
	{
	  gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be UNdeleted",
				  surfs[i], SC deletedSurfaces[surfs[i]].getName(Mapping::mappingName)));
// delete any displaylists
	  deletedSurfaces.eraseCompositeSurface(gi, surfs[i]);
// add the surface to the model
	  model.add(deletedSurfaces[surfs[i]], deletedSurfaces.getSurfaceID(surfs[i]));
	  model.setColour(model.numberOfSubSurfaces()-1, gi.getColourName(model.numberOfSubSurfaces()-1));
// remove the surface from the collection of deleted surfaces
	  deletedSurfaces.remove(surfs[i]);
	}
      } // end if...
    }
    else if( (len = answer.matches("examine surface")) )
    {
      int s;
// syntax for examine surface 23 is: `examine surfaces 23'
      if (sScanF(answer(len,answer.length()-1), "%i", &s) == 1 && s>=0)
      {
	gi.getView(loc); // get the viewpoint info
	gi.erase();
	par.set(GI_TOP_LABEL,sPrintF(buf,"sub-surface %i", s));
	par.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	Mapping *map = &model[s];  // make a pointer so virtual function call works.
	map->update( mapInfo );
	gi.setView(loc); // reset the view point
	gi.redraw();
// delete the display lists so that sub-surface s will get replotted properly
	model.eraseCompositeSurface(gi, s);
	par.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	par.set(GI_TOP_LABEL,model.getName(Mapping::mappingName));
	plotObject=true;
      }      
    }
    else if( (len = answer.matches("edit trim curve")) )
    {
      int s=0, tc=-1;
// syntax for editing trim curve 23 on surface 9 is: `edit trim curve 23 9'
      if (sScanF(answer(len,answer.length()-1), "%i %i", &tc, &s) == 2 && s>=0 && tc >= 0)
      {
	if (model[s].getClassName() == "TrimmedMapping")
	{
	  gi.outputString(sPrintF(buf,"Editing trim curve %i on sub-surface %i, ID=%i", tc, s, 
				  model.getSurfaceID(s)));
	  
	  TrimmedMapping &trim = (TrimmedMapping &) model[s];
	  Mapping *curve = trim.getTrimCurve(tc);
	  if (curve)
	  {
	    gi.getView(loc); // get the viewpoint info
	    trim.editTrimCurve(*curve, mapInfo);
	    gi.setView(loc); // reset the view point
	    gi.redraw();
// delete the display lists so that sub-surface s will get replotted properly
	    model.eraseCompositeSurface(gi, s);
	    plotObject=true;
	  }
	  else
	  {
	    gi.createMessageDialog(sPrintF(buf, "Sorry, trim curve %i is not available", tc), 
				   errorDialog);
	  }
	}
	else
	{
	  gi.createMessageDialog(sPrintF(buf, "Sorry, surface %i is NOT trimmed", s), 
				 errorDialog);
	}
	
	
      }
    }
    else if( (len = answer.matches("delete trim curve")) )
    {
      int s=0, tc=-1;
// syntax for editing trim curve 23 on surface 9 is: `edit trim curve 23 9'
      if (sScanF(answer(len,answer.length()-1), "%i %i", &tc, &s) == 2 && s>=0 && tc >= 0)
      {
	TrimmedMapping &trim = (TrimmedMapping &) model[s];
// remember where the last trim curve was deleted, so we can undelete
	lastTrimmedMapping = &trim;
	trim.deleteTrimCurve(tc);
// after delete and add operations, trimmingIsValid can be out of sync
	trim.validateTrimming();
// delete the display lists so that sub-surface s will get replotted properly
	model.eraseCompositeSurface(gi, s);
	plotObject=true;
      }
    }
    else if( (len = answer.matches("surface for intersection")) )
    {
      int s;
      if (sScanF(answer(len,answer.length()-1), "%i", &s) == 1 && s>=0)
      {
	iMap_[nIMap] = &model[s];
	nIMap++;
      }
// compute the intersection
      if (nIMap == 2)
      {
	intersectTwoSurfaces(model, deletedSurfaces, gi, iMap_[0], iMap_[1], interSect_);
// reset pointers and counter
	iMap_[0] = NULL;
	iMap_[1] = NULL;
	nIMap = 0;
      }
    }
    else if( (len = answer.matches("surface edge")) )
    {
      int s=-1;
      real x, y, z;
      if (sScanF(answer(len,answer.length()-1),"%i %e %e %e", &s, &x, &y, &z) == 4 && s>=0)
      {
	Edge *bCurve_ = closestEdgeOnSurface(x, y, z, model, s, true);

	MappingRC splineRC(*(bCurve_->spline_));
	curveList.addElement(splineRC); // increments reference count for  bCurve->spline_

// decrements bCurve_->spline_
	delete  bCurve_;
	
	gi.outputString("Made a NEW curve");
      }
    }
    else if( (len = answer.matches("select curve")) )
    {
      int c=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &c) == 1 && c>=0 && nEdges<4)
      {
	gi.outputString(sPrintF(buf,"Selecting curve %i", c));
	edges[nEdges++] = c;
      }
      else
      {
	gi.outputString(sPrintF(buf,"Error: Cannot select curve %i (nEdges = %i)", c, nEdges));
      }
    }
    else if( (len = answer.matches("project edge")) )
    {
      int s=-1, e=-1;
      if (sScanF(answer(len,answer.length()-1),"%i %i", &e, &s) == 2 && s>=0 && e>=0 && e<nEdges)
      {
	gi.outputString(sPrintF(buf,"Projecting edge %i onto subSurface %i", e, s));
	Mapping & curve = curveList[edges[e]].getMapping();
// make a copy so we can reshape without worrying
	// kkc this does not actually make a copy realArray x = curve.getGrid();
	realArray x;
	x = curve.getGrid(); // kkc but this does

	x.reshape(x.dimension(0),x.dimension(3));
	projectPointsOnSurface(x, s, model, deletedSurfaces, gi);
      }
      else
      {
	gi.outputString(sPrintF(buf,"Error: Cannot project edge %i onto subSurface %i, nEdges=%i", e, s, nEdges));
      }
    }
    else if( (len = answer.matches("project surface")) )
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0 && s<model.numberOfSubSurfaces())
      {
	gi.outputString(sPrintF(buf,"Projecting surface %i onto the model", s));
	projectSurfaceOnModel(s, model, gi);
      }
      else
      {
	gi.outputString(sPrintF(buf,"Error: Cannot project surface %i onto the model", s));
      }
    }
    else if( (len = answer.matches("point for plane")) )
    {
      if (nPlanePoints<3)
      {
	int p=-1;
	if (sScanF(answer(len,answer.length()-1),"%i", &p) == 1 && p>=0 && p<points.size())
	{
	  gi.outputString(sPrintF(buf, "Point #%i selected", p));
	  planePoints[nPlanePoints++] = p;
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
      if (nPlanePoints == 3)
      {
	real planeCoordinates[3][3];
	for (i=0; i<3; i++)
	  for (axes=0; axes<3; axes++)
	    planeCoordinates[i][axes] = points[planePoints[i]].coordinate[axes];
	addPlaneToModel(planeCoordinates, nPlanePoints, model, gi);
      }
      
    }
    else if( answer.matches("plot shaded surfaces (3D) toggle") )
    {
      plotShadedMappingBoundaries = !plotShadedMappingBoundaries;   
      par.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotShadedMappingBoundaries);
      interface.setToggleState(plotShadedTB, plotShadedMappingBoundaries);
    }//                     01234567890123456789012345678901234567890
    else if( answer.matches("plot grid lines on boundaries (3D) toggle") )
    {
      plotLinesOnMappingBoundaries = !plotLinesOnMappingBoundaries;
      par.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,plotLinesOnMappingBoundaries);
      interface.setToggleState(plotLinesTB, plotLinesOnMappingBoundaries);
    }
    else if( answer.matches("plot sub-surface boundaries (toggle)") )
    {
      plotMappingEdges = !plotMappingEdges; // reverse the flag
      par.set(GI_PLOT_MAPPING_EDGES, plotMappingEdges); 
      interface.setToggleState(plotEdgesTB, plotMappingEdges);
    }//                     012345678901234567890123456789
    else if( answer.matches("plot curves") )
    {
      plotCurves = !plotCurves; // reverse the flag
      interface.setToggleState(plotCurvesTB, plotCurves);
    }
    else if( answer.matches("mappingName") ) // read the name off answer
    {
      aString newName = "";
      if (answer.length() > 12)
	newName = answer(12,answer.length()-1);

      if (newName != "" && newName != " ")
      {
	model.setName(Mapping::mappingName, newName);
	gi.eraseLabels(par); // erase the old label
	par.set(GI_TOP_LABEL, model.getName(Mapping::mappingName));
      }
      else
	gi.outputString("Invalid name");
      const aString name =  model.getName(Mapping::mappingName); // gcc warning, setTextLabel should take const aString &
      interface.setTextLabel(mnIndex, name); // (re)set the textlabel
    }
    else if( (len = answer.matches("newPoint")) ) // read the point coordinates
    {
      aString pointString = "";
      if (answer.length() > len+2)
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
    else if (!select.active) // if a mogl-pickOutside event occurs, select.active will be true
    {
      gi.outputString( sPrintF(buf,"Unknown response=%s", (const char*)answer) );
      gi.stopReadingCommandFile();
      plotObject=false;
    }

    if( plotObject )
    {
      gi.erase(); // erase the points	
      if (selectionFunction == undeleteSurface)
	PlotIt::plot(gi,deletedSurfaces, par);  
      par.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);
      PlotIt::plot(gi, model, par);  

// plot the curves
      splinePar.set(GI_POINT_SIZE, 4); // the size of endpoints is multiplied by 1.5
      splinePar.set(GI_MAPPING_COLOUR,"black");
      if ( plotCurves )
      {
	for (i=0; i<curveList.getLength(); i++)
	{
	 if ( curveList[i].getMapping().getRangeDimension()==3 ) // kkc selecting 2D curves by accident can cause A++ assertion failures when inverting mappings
	   {
	     splinePar.set(GI_MAPPING_COLOUR,"black");
	     // use a different color for the selected curves
	     for (j=0; j<nEdges; j++)
	       if (edges[j] == i)
		 {
		   splinePar.set(GI_MAPPING_COLOUR,edgeColour[j]);
		   break;
		 }
	     PlotIt::plot(gi, curveList[i].getMapping(), splinePar );
	   }
	}       
      }

// plot the points
     par.set(GI_POINT_SIZE, 4);
     par.set(GI_POINT_COLOUR, "black");
     points.plot(gi, par);

// plot the points selected for building an edge
     if (selectionFunction == buildCurve)
     {
       par.set(GI_POINT_SIZE, 6);
       par.set(GI_POINT_COLOUR, "red");
       for (i=0; i<nEdgePoints; i++)
	 points[edgePoints[i]].plot(gi, par);
     }

// plot the points selected for building a plane
     if (selectionFunction == buildPlane)
     {
       par.set(GI_POINT_SIZE, 6);
       par.set(GI_POINT_COLOUR, "green");
       for (i=0; i<nPlanePoints; i++)
	 points[planePoints[i]].plot(gi, par);
     }

// plot the intersection
      if (selectionFunction == intersectSurf && interSect_ && interSect_->curve)
      {
	splinePar.set(GI_MAPPING_COLOUR, "red");
	splinePar.set(GI_PLOT_GRID_POINTS_ON_CURVES, FALSE);
	NurbsMapping & iCurve = (NurbsMapping &) *interSect_->curve;
	PlotIt::plot(gi, iCurve, splinePar);
      }
      
    }
//                           01234
//      else if (answer(0,3) == "help")
//      {
//        aString topic;
//        topic = answer(5,answer.length()-1);
//        if (!gi.displayHelp(topic))
//        {
//  	aString msg;
//  	sPrintF(msg,"Sorry, there is currently no help for `%s'", SC topic);
//  	gi.createMessageDialog(msg, informationDialog);
//        }
//      }
  }
  gi.popGUI();

// set the state!
  par.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedMappingBoundaries);
  par.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnMappingBoundaries);
  par.set(GI_PLOT_MAPPING_EDGES, plotMappingEdges);
  par.set(GI_PLOT_LABELS, plotTitleLabels);
  par.set(GI_LABEL_GRIDS_AND_BOUNDARIES, plotSquares);
}


