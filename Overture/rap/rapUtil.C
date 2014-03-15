#include "ModelBuilder.h"
#include "rap.h"

bool ModelBuilder::
addPrefix(aString cmd[], const aString & prefix)
// ==============================================================================================
// /Description:
//    Add a prefix string to the start of every command.
// /cmd (input/output) : null terminated array of strings.
// /prefix (input) : all this string as a prefix.
// ==============================================================================================
{
    
  int i;
  for( i=0; cmd[i]!=""; i++ )
    cmd[i]=prefix+cmd[i];

  return true;
}

//\begin{>>ModelBuilderInclude.tex}{\subsection{checkModel}}
void ModelBuilder::
checkModel(GenericGraphicsInterface & gi)
//===========================================================================
// /Description:
//    Check the model.
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  GUIState interface;
  
  interface.setWindowTitle("Check model");
  
  interface.setExitCommand("close", "Close");

  aString pbCommands[] = {"check leaks", "check normals", ""};
  aString pbLabels[] = {"Check Leaks", "Check Normals", ""};
  
  interface.setPushButtons( pbCommands, pbLabels, 2 ); // default is 2 rows

// define pulldown menus
  aString pdCommand2[] = {"help check leaks", "help check normals", ""};
  aString pdLabel2[] = {"Check Leaks", "Check Normals", ""};
  interface.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  interface.setLastPullDownIsHelp(true);
// done defining pulldown menus  

  gi.pushGUI(interface);
  
  int retCode;
  aString answer;
  
  for(;;)
  {
    retCode = gi.getAnswer(answer, "");
    if (answer == "close")
    {
      break;
    }
//                           01234
    else if (answer(0,3) == "help")
    {
      aString topic;
      topic = answer(5,answer.length()-1);
      if (!gi.displayHelp(topic))
      {
	aString msg;
	sPrintF(msg,"Sorry, there is currently no help for `%s'", SC topic);
	gi.createMessageDialog(msg, informationDialog);
      }
    }
  }
  gi.popGUI();
}




//\begin{>>ModelBuilderInclude.tex}{\subsection{getClosestCurve}}
Edge* ModelBuilder::
getClosestCurve(int &s, CompositeSurface &model, SelectionInfo &select, GenericGraphicsInterface &gi, 
		bool buildSpline /* = false */)
//===========================================================================
// /Description:
//    Find the closest edge on the model to the user selection.
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  Edge *edge_ = NULL;
  int i;
  s=-1;
  bool foundSurface=false;
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
  
  if (s >= 0 && s<model.numberOfSubSurfaces())
  {
    aString buf;
    edge_ = closestEdgeOnSurface(select.x[0], select.x[1], select.x[2], model, s, buildSpline);
  } // end if a subSurface was selected
  else
    printf("NO sub-surface was selected\n");

  return edge_;
}

//\begin{>>ModelBuilderInclude.tex}{\subsection{closestEdgeOnSurface}}
Edge* ModelBuilder::
closestEdgeOnSurface(real x, real y, real z, CompositeSurface &model, int s, bool buildSpline /* = false */)
//===========================================================================
// /Description:
//    Find the closest edge on the subsurface s of the model to the point (x,y,z)
// /x,y,z (input) : find the closest edge to this point.
// /s (input) : check this sub-surface.
// /buildSpline (input) : if true build a spline for the edge...
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
  const int numberOfPoints=15; // should choose this number more carefully

  Edge *edge_ = new Edge;
  edge_->subSurface = s;
  Mapping & subSurface = model[s];
  NurbsMapping * spline_=NULL;
  

  Range I=numberOfPoints;
  realArray rp(1,2), xp(1,3), r(I,2);

  if (buildSpline)
  {
    spline_ = edge_->spline_ = new NurbsMapping;
    edge_->spline_->incrementReferenceCount();
    edge_->nKnots = numberOfPoints;
    edge_->x.redim(I,3);
  }
          
  xp(0,0) = x; //select.x[0];
  xp(0,1) = y; //select.x[1];
  xp(0,2) = z; //select.x[2];
	
  rp = -1; // no initial guess
  if (subSurface.getClassName() == "TrimmedMapping")
  {
    TrimmedMapping & trim = (TrimmedMapping&) subSurface;
    trim.untrimmedSurface()->inverseMap(xp,rp);
  }
  else
    subSurface.inverseMap(xp,rp);
  printf("subSurface parameter coordinate: (%e, %e)\n", rp(0,0), rp(0,1));
	
  edge_->mappingIsTrimmed = (subSurface.getClassName()=="TrimmedMapping");
  if( !edge_->mappingIsTrimmed )
  {

    printf("The subSurface `%s' is NOT trimmed\n", SC subSurface.getName(Mapping::mappingName));
    int side,axis;
    if( min(fabs(rp(0,0)),fabs(rp(0,0)-1.)) <  min(fabs(rp(0,1)),fabs(rp(0,1)-1.)) )
    {
      axis=0;
    }
    else
    {
      axis=1;
    }
    side=fabs(rp(0,axis)) < fabs(rp(0,axis)-1.) ? 0 : 1;
    printf("Closest edge: axis=%i, side=%i\n", axis, side);

    if (buildSpline)
    {
      real dr=1./(numberOfPoints-1);
      r(I,axis)=(real)side;
      int axisp1=(axis+1)%2;
      r(I,axisp1).seqAdd(0.,dr);

      subSurface.map(r,/*x*/ edge_->x);

      spline_->interpolate( edge_->x );
      if( subSurface.getIsPeriodic(axisp1)==Mapping::functionPeriodic ) // AP: getIsPeriodic(axisp1)?
      {
	spline_->setIsPeriodic(axis1,Mapping::functionPeriodic);
      }
// set a reasonable resolution
      spline_->setGridDimensions(axis1, edge_->nKnots);
    } // end if buildSpline...
  }
  else
  {
// The trimming curves are in the parameter space of the surface of the TrimmedMapping.
    printf("The subSurface `%s' IS trimmed\n", SC subSurface.getName(Mapping::mappingName));
    TrimmedMapping & trim = (TrimmedMapping&)subSurface;
// collect nearest parameter space coordinate in rSc
    realArray tSc(1,1), rSc(1,2);
    real dist, minDist=1.e7;
    Mapping * closestCurve_ = NULL;
    int nearTrimCurve=-1, nearSubTrimCurve=-1;
	  
    for ( int i=0; i<trim.getNumberOfTrimCurves(); i++ )
    {
      Mapping & trimCurve = *(trim.getTrimCurve(i));
      if( trimCurve.getClassName()=="NurbsMapping" )
      {
	NurbsMapping & nurb = (NurbsMapping&)trimCurve;
	printf("trimCurve #%i is a nurb, number of subCurves = %i \n",i, nurb.numberOfSubCurves());
	for( int subCurve=0; subCurve<nurb.numberOfSubCurves(); subCurve++ )
	{
	  NurbsMapping & subTrimCurve = nurb.subCurve(subCurve);
// get the closest point on the subTrimCurve
	  tSc = -1; // no initial guess
	  subTrimCurve.inverseMap(rp, tSc);
// evaluate the subCurve at tSc
	  subTrimCurve.map(tSc, rSc);
// get the distance (in parameter space) between rSc and rp
	  dist = sqrt( SQR(rSc(0,0)-rp(0,0)) + SQR(rSc(0,1)-rp(0,1)) );
	  if (dist < minDist)
	  {
	    minDist = dist;
	    closestCurve_ = &subTrimCurve;
	    nearTrimCurve = i;
	    nearSubTrimCurve = subCurve;
	  }
	}
      }
      else
      {
	printf("trimCurve #%i is NOT a nurb!");
// get the closest point on the trimCurve
	tSc=-1;
	trimCurve.inverseMap(rp, tSc);
// evaluate the subCurve at tSc
	trimCurve.map(tSc, rSc);
// get the distance (in parameter space) between rSc and rp
	dist = sqrt( SQR(rSc(0,0)-rp(0,0)) + SQR(rSc(0,1)-rp(0,1)) );
	if (dist < minDist)
	{
	  minDist = dist;
	  closestCurve_ = &trimCurve;
	  nearTrimCurve = i;
	  nearSubTrimCurve = -1;
	}
      }
    } // end for all trimcurves

// report the closest trimCurve and subTrimCurve
    edge_->trimCurve = nearTrimCurve;
    edge_->subTrimCurve = nearSubTrimCurve;
    printf("The closest curve was trimCurve=%i, subTrimCurve=%i\n", nearTrimCurve, nearSubTrimCurve);

    if (buildSpline)
    {
// stepsize
      real dr=1./(numberOfPoints-1);
      realArray tc(I,1);
      tc(I,axis1).seqAdd(0.,dr);
// evaluate the subTrimCurve 
      closestCurve_->map(tc,r);
// evaluate the subSurface along the subTrimCurve
      subSurface.map(r,/*x*/ edge_->x);

      spline_->interpolate( edge_->x );
// set a reasonable resolution
      spline_->setGridDimensions(axis1, edge_->nKnots);

    }
    
  } // end if trimmed mapping

  return edge_;
}
