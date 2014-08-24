//#define BOUNDS_CHECK
//#define OV_DEBUG

//TODO
/// global vertex ids and io operations

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <stack>

#include "GenericGraphicsInterface.h"
#include "GUIState.h"
#include "PlotIt.h"
#include "SquareMapping.h"
#include "NurbsMapping.h"
#include "nurbsCurveEditor.h"
#include "AdvancingFront.h"
#include "ArraySimple.h"

#include "smesh.hh"

using namespace std;

//
// optimize is experimental, it is not part of the regular distribution
extern void optimize(UnstructuredMapping &umap, RealCompositeGridFunction *cf);

namespace {

  typedef vector< Region * > RegionContainer;
  // not sure if a map is really needed now
  typedef map<int, Curve * > CurveMappingContainer;

  // globally used data for the application.  
  CurveMappingContainer curves; // list of all curves, manages curve deletion
  //  PointList points; // list of all points, move this below because of strange gcc error?
  RegionContainer regions; // list of all regions, manages region deletion

  Region *createTFIRegion(GenericGraphicsInterface &gi, 
			  CurveMappingContainer &curves, real dx, real dy);

  Region *createUnstructuredRegion(GenericGraphicsInterface &gi, 
				   CurveMappingContainer &curves, 
				   real dx, real dy);

  // plot a list of curves, coloured if neccessary, using the 
  //   graphics interface gi.
  void plotCurves(GenericGraphicsInterface &gi, 
		  GraphicsParameters &gp, CurveMappingContainer &crv );

  // its all the same code man!
  bool collectCurves(GenericGraphicsInterface &gi, CurveMappingContainer &crv,
		     aString windowTitle, int nCurvesMin, int nCurvesMax, 
		     vector<aString> &curveNames, bool checkPeriodic, 
		     vector<Curve*> &boundingCurves);

  Curve *getSelectedCurve(SelectionInfo &select, CurveMappingContainer &curs)
  {
    Curve *retC = 0;
    for ( int s=0; s<select.nSelect; s++ )
      for ( CurveMappingContainer::iterator c=curs.begin(); c!=curs.end() && !retC; c++ )
	if ( dynamic_cast<SimpleCurve*>(c->second) )
	  if ( c->second->getNurbs().getGlobalID()==select.selection(s,0) )
	    retC = c->second;

    return retC;
  }

  Region *getSelectedRegion(SelectionInfo &select, RegionContainer &regs)
  {
    Region *retR = 0;
    for ( int s=0; s<select.nSelect; s++ )
      for ( RegionContainer::iterator r=regs.begin(); r!=regs.end() && !retR; r++ )
	if ( (*r)->getMapping().getGlobalID()==select.selection(s,0) )
	  retR = *r;

    return retR;

  }

  void writeToIngrid(std::string fileName, PointList &points, CurveMappingContainer & curves,
		     RegionContainer &regions);
}

int main(int argc, char *argv[])
{
  Overture::start(argc,argv);

#ifndef OV_DEBUG
  Index::setBoundsCheck(off); 
#endif

  PointList points;

  aString commandFileName="";
  bool plotOption=true;
  if( argc > 0 && argc<4 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line.matches("noplot") )
        plotOption=false;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `"<<argv[0]<<" [noplot][file.cmd]' \n"
            "          noplot:   run without graphics \n" 
            "          file.cmd: read this command file \n";

  GenericGraphicsInterface & gi = 
    *Overture::getGraphicsInterface("How hard can it be?",plotOption);
  
  if ( commandFileName.length() )
    gi.readCommandFile(commandFileName);

  gi.saveEchoFile("smesh.log");
  gi.saveCommandFile("smesh.cmd");

  GUIState gui;
  
  gui.setWindowTitle("2 cent mesh generator");
  gui.setExitCommand("exit","exit");

  aString txtLbl[] = {"Default dx, dy", ""};
  aString txtCmd[] = {"dxdy",""};
  aString txtInit[] = {".1,.1",""};
  gui.setTextBoxes(txtCmd,txtLbl,txtInit);

  enum ToggleOptions { plotBG=0, plotCurves, plotPoints, plotRegions, nTB };
  int tbState[] = { true, true, true, true };
  //  int tbState[] = { plotBG };
  aString tbCmd[] = {"tb Plot Reference Grids", "tb Plot Curves", 
		     "tb Plot Points", "tb Plot Regions", ""};
  aString tbLbl[] = {"Plot Reference Grids", "Plot Curves", "Plot Points", 
		     "Plot Regions", ""};
  gui.setToggleButtons(tbCmd, tbLbl, tbState, nTB/2);

  enum PushButtons { createCurve=0, createTFIRegion, createUnsRegion, 
		     writeIngrid,
		     nPB };
  aString pbCmd[] = { "Create Curves", "Create TFI Region", 
		      "Create Unstructured Region", "Save Mesh", "",};
  aString pbLbl[] = { "Create Curves", "Create TFI Region", 
		      "Create Unstructured Region", "Save Mesh", ""};

  gui.setPushButtons(pbCmd,pbLbl,nPB);

  enum PickOptions { noOp=0, nCurvePts, strCurvePts, regionName, delCurve, delRegion, optimizeRegion, nPickOps };
  aString pckCmd[] = { "mm noOp", "mm nCurvePts", "mm strCurve", 
		       "mm regionName", "mm delCurve", "mm delRegion", "mm optimize","" };
  aString pckLbl[] = { "no operation", "specify # of curve points", "stretch curve points", "specify region name", 
		       "delete a curve", "delete a region", "optimize unstructured","" };
  PickOptions pickFunction = noOp;
  gui.addRadioBox("Mouse Selection:", pckCmd, pckLbl, pickFunction, 2);
  RadioBox &pickRBox = gui.getRadioBox(0);

  GraphicsParameters squareGP;
  squareGP.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  squareGP.set(GI_MAPPING_COLOUR,"khaki");
  squareGP.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByGrid);
  squareGP.set(GI_PLOT_GRID_LINES,TRUE);
  squareGP.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,FALSE);
  squareGP.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE);

  GraphicsParameters curveGP;
  curveGP.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  curveGP.set(GI_PLOT_GRID_POINTS_ON_CURVES,FALSE);
  curveGP.set(GI_PLOT_GRID_POINTS_ON_CURVES,FALSE);

  GraphicsParameters pointsGP;
  pointsGP.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  
  GraphicsParameters regionGP;
  regionGP.set(GI_PLOT_BLOCK_BOUNDARIES,FALSE);
  regionGP.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,FALSE);
  regionGP.set(GI_PLOT_MAPPING_EDGES,TRUE);
  regionGP.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  regionGP.set(GI_PLOT_UNS_FACES, TRUE);
  regionGP.set(GI_PLOT_UNS_EDGES, TRUE);

  aString answer="";
  SelectionInfo select;
  int len;

  gi.pushGUI(gui);

  real dxRef=.1, dyRef=.1;

  for ( ;; )
    {
      gi.savePickCommands(false);
      gi.getAnswer(answer,"",select);
      gi.savePickCommands(true);

      len=0;
      if ( answer=="exit" )
	break;
      else if ( (len = answer.matches("dxdy")) )
	{
	  sScanF(answer(len,answer.length()-1),"%g %g",&dxRef, &dyRef);
	  
	  if ( dxRef<=0. || dyRef<=0. ) 
	    {
	      aString msg = "make sure dx>0 and dy>0!";
	      gi.outputString(msg);
	      gi.createMessageDialog(msg, errorDialog);
	    }
	  else
	    {
  	      aString str;
	      sPrintF(str,"%g, %g",dxRef,dyRef);
	      gui.setTextLabel(txtLbl[0],str);
	    }
	}
      else if ( (len = answer.matches("tb ") ) )
	{
	  for ( int t=0; t<nTB; t++ )
	    {
	      if ( answer(len,answer.length()-1).matches(tbLbl[t]) )
		{
		  tbState[t] = !tbState[t];
		  gui.setToggleState(t, tbState[t]);
		  break;
		}
	    }
	}
      else if ( answer.matches(pbCmd[createCurve]) )
	{
	  NurbsMapping nurbForCurves;

	  bool pbg = gi.getPlotTheBackgroundGrid();
	  nurbsCurveEditor(nurbForCurves,gi,points);
	  gi.setPlotTheBackgroundGrid(pbg);
	  
	  // add all the created curves to the list
	  for ( int sc=0; sc<nurbForCurves.numberOfSubCurves(); sc++ )
	    {
	      //int id = curveIDCounter++; //curves.size();
	      NurbsMapping &nurb = nurbForCurves.subCurve(sc);
	      if ( nurb.getOrder()>0 )
		{
		  nurb.setGridDimensions(0,100);
		  realArray r(2,1);
		  r(0,0) = nurb.getDomainBound(0,0);
		  r(1,0) = nurb.getDomainBound(1,0);
		  //		  r.display("r for nurb");
		  realArray x(2,2);
		  nurb.map(r,x);
		  //		  x.display("x for nurb");
		  int ps=-1, pe=-1;
		  real dx,dy;
		  for ( int i=0; i<points.size(); i++ )
		    {
		      dx = (x(0,0)-points[i].coordinate[0]);
		      dy = (x(0,1)-points[i].coordinate[1]);
		      if ( (dx*dx+dy*dy)<10*REAL_EPSILON )
			ps = i;
		      
		      dx = (x(1,0)-points[i].coordinate[0]);
		      dy = (x(1,1)-points[i].coordinate[1]);
		      if ( (dx*dx+dy*dy)<10*REAL_EPSILON )
			pe = i;
		      
		      if ( pe!=-1 && ps!=-1 ) break;
		    }
		  
		  //		  assert( pe!=-1 && ps!=-1 );
		  if ( pe==-1 )
		    {
		      Point np;
		      np.coordinate[0] = x(1,0);
		      np.coordinate[1] = x(1,1);
		      np.coordinate[2] = 0.;
		      points.add(np);
		      pe = points.size()-1;
		    }
		  if ( ps==-1 )
		    {
		      Point np;
		      np.coordinate[0] = x(0,0);
		      np.coordinate[1] = x(0,1);
		      np.coordinate[2] = 0.;
		      points.add(np);
		      ps = points.size()-1;
		    }

		  if ( nurb.isSubCurveHidden(0) )
		    nurb.toggleSubCurveVisibility(0);

		  Curve *newCurve = new SimpleCurve(nurb,ps,pe);
		  curves[newCurve->ID()] = newCurve;
		}
	    }
	}
      else if ( answer.matches(pbCmd[createTFIRegion]) )
	{
	  Region *newReg = ::createTFIRegion(gi,curves,dxRef,dyRef);
	  if ( newReg ) regions.push_back(newReg);
	}
      else if ( answer.matches(pbCmd[createUnsRegion]) )
	{
	  Region *newReg = ::createUnstructuredRegion(gi,curves,dxRef,dyRef);
	  if ( newReg ) regions.push_back(newReg);
	}
      else if ( answer.matches(pbCmd[writeIngrid]) )
	{
	  if ( !regions.size() )
	    {
	      aString msg = "You must create some regions first!";
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      aString fileName;
	      gi.inputFileName(fileName, "", ".msh");
	      if ( fileName.length() && fileName!=" " )
		writeToIngrid(fileName.c_str(), points, curves, regions);
	    }
	}
      else if ( (len=answer.matches("mm")) )
	{
	  // change the mouse mode
	  aString mode = answer.length()>len+2 ? answer(len+1,answer.length()-1) : aString("");
	  int m;//=nPickOps;

	  for ( m=noOp; m<nPickOps; m++ )
	    if ( mode.matches(pckCmd[m](len+1,pckCmd[m].length()-1)) )
	      {
		pickFunction = PickOptions(m);
		break;
	      }

	  if ( m==nPickOps )
	    {
	      gi.outputString("unknown pick command : "+answer);
	      gi.createMessageDialog("unknown pick command : "+answer, errorDialog);
	      gi.stopReadingCommandFile();
	    }

	  pickRBox.setCurrentChoice(pickFunction);
	}
      else if ( select.nSelect>0 && pickFunction>noOp && pickFunction<nPickOps )
	{
	  if ( pickFunction==nCurvePts )
	    {
	      Curve * pickedC = getSelectedCurve(select, curves);
	      if ( pickedC && pickedC->region_1() )
		{ // this curve has already been specified using a region, dont change it
		  aString msg = "curve spacing bound to an existing region!";
		  gi.outputString(msg);
		  gi.createMessageDialog(msg, errorDialog);
		}
	      else if ( pickedC )
		{
		  GUIState gg;
		  aString currNpts;
		  if ( pickedC->numberOfPoints() )
		    sPrintF(currNpts, "%d", pickedC->numberOfPoints());
		  else
		    currNpts = "auto";

		  CurveMappingContainer::iterator ci=curves.begin(); 
		  for ( ; ci!=curves.end(); ci++ )
		    if ( ci->second == pickedC ) break;
		  
		  int cid=ci->first;
  
		  aString txtc[] = {"",""};
		  aString txti[] = {"blah",""};
		  sPrintF(txtc[0],"points on curve %d ",cid);
		  gg.setTextBoxes(txtc,txtc,txti);
		  gg.setTextLabel(txtc[0],currNpts);
		  gg.setExitCommand("Cancel","Cancel");

		  gi.pushGUI(gg);
		  aString ans;
		  gi.getAnswer(ans,"");
		  len = ans.matches(txtc[0]);
		  aString outCmd="";
		  if ( ans(len+1,ans.length()-1).matches("auto") )
		    {
		      SimpleCurve *sc = dynamic_cast<SimpleCurve*>(pickedC);
		      if ( sc )
			sc->setNumberOfPoints(-1);
		      sPrintF(outCmd,"points on curve %d auto\n",cid);
		    }
		  else if ( ans!="Cancel" )
		    {
		      int np=-1;
		      sScanF(ans(len+1,ans.length()-1),"%d",&np);
		      if ( np<=0 )
			{
			  gi.outputString("number of points must be >0!");
			  gi.createMessageDialog("number of points must be >0!",errorDialog);
			}
		      else
			{
			  SimpleCurve *sc = dynamic_cast<SimpleCurve*>(pickedC);
			  if ( sc )
			    sc->setNumberOfPoints(np);
			  
			  sPrintF(outCmd,"points on curve %d %d\n",cid,np);

			}
		    }
		  //		  if ( outCmd.length() ) gi.outputToCommandFile(outCmd);
		  gi.popGUI();
		}
	    }
	  else if ( pickFunction==strCurvePts )
	    {
	      Curve * pickedC = getSelectedCurve(select, curves);
	      if ( pickedC && pickedC->region_1() )
		{ // this curve has already been specified using a region, dont change it
		  aString msg = "curve spacing bound to an existing region!";
		  gi.outputString(msg);
		  gi.createMessageDialog(msg, errorDialog);
		}
	      else if ( pickedC )
		{
		  GUIState gg;

		  CurveMappingContainer::iterator ci=curves.begin(); 
		  for ( ; ci!=curves.end(); ci++ )
		    if ( ci->second == pickedC ) break;
		  
		  int cid=ci->first;
  
		  aString pbc[3];
		  sPrintF(pbc[0],"uniform points on curve %d",cid);
		  sPrintF(pbc[1],"stretch points on curve %d",cid);
		  pbc[2]="";

		  aString pbl[] = { "uniform", "stretch", "" };

		  gg.setPushButtons(pbc,pbl,2);
		  gi.pushGUI(gg);
		  aString ans;
		  aString outCmd;

		  gi.getAnswer(ans,"");
		  if ( ans.matches("uniform") )
		    {
		      SimpleCurve *sc = dynamic_cast<SimpleCurve*>(pickedC);
		      if ( sc )
			sc->deleteStretching();
		    }
		  else if ( ans.matches("stretch") )
		    {
		      SimpleCurve *sc = dynamic_cast<SimpleCurve*>(pickedC);
		  
		      if ( sc )
			sc->stretchPoints();
		    }

		  gi.popGUI();
		}
	    }
	  else if ( pickFunction==regionName )
	    {
	      Region * pickedR = getSelectedRegion(select, regions);
	      if ( pickedR )
		{
		  GUIState gg;
		  aString txtc[] = {"",""};
		  aString txti[] = {"blah",""};
		  sPrintF(txtc[0],"name region %d : ",pickedR->ID());
		  gg.setTextBoxes(txtc,txtc,txti);
		  aString name = pickedR->getName()=="" ? "- none -" : pickedR->getName().c_str();
		  gg.setTextLabel(txtc[0],name);

		  gi.pushGUI(gg);
		  aString ans;
		  gi.getAnswer(ans,"");
		  int len = ans.matches(txtc[0]);
		  name = ans(len+1, ans.length()-1);
		  
		  if ( name!="- none -" )
		    {
		      string nm1 = (const char *)name;
		      int start = nm1.find_first_of(':')+1;
		      string name = nm1.substr(start, nm1.size()-start);
		      pickedR->setName(name);
		      aString outCmd;
		      sPrintF(outCmd,"name region %d %s\n",pickedR->ID(),
			      (char *)(name.c_str()));
		    }
		  gi.popGUI();
		}
	    }
	  else if ( pickFunction==optimizeRegion )
	    {
	      Region * pickedR = getSelectedRegion(select, regions);
	      if ( pickedR )
		{
		  UnstructuredRegion *ur=0;
		  if ( (ur=dynamic_cast<UnstructuredRegion*>(pickedR)) )
		    {
		      gi.outputString("optimizing...");
		      optimize((UnstructuredMapping &)ur->getMapping(),0);
		      gi.outputString("             ... done!");

		      aString outCmd;
		      sPrintF(outCmd,"optimize region %d\n",pickedR->ID());
		      gi.outputToCommandFile(outCmd);
		    }
		  else
		    {
		      gi.outputString("only unstructured regions can be optimized");
		      gi.createMessageDialog("only unstructured regions can be optimized",errorDialog);
		    }
		}
	    }
	  else if ( pickFunction==delCurve )
	    {
	      Curve * pickedC = getSelectedCurve(select, curves);
	      if ( pickedC->region_1() )
		{ // this curve belongs to a region and cannot be deleted
		  aString msg = "cannot delete curve, it is bound to an existing region!";
		  gi.outputString(msg);
		  gi.createMessageDialog(msg, errorDialog);
		}
	      else if ( pickedC )
		{
		  CurveMappingContainer::iterator ci;
		  for ( ci=curves.begin(); 
			ci!=curves.end(); ci++ )
		    if ( ci->second == pickedC ) break;
		  
		  aString outCmd;
		  sPrintF(outCmd,"delete curve %d\n",ci->first);
		  gi.outputToCommandFile(outCmd);
		  delete ci->second;
		  curves.erase(ci);
		}
	    }
	  else if ( pickFunction==delRegion )
	    {
	      Region * pickedR = getSelectedRegion(select, regions);
	      if ( pickedR )
		{
		  RegionContainer::iterator ri=regions.begin();
		  for ( ; ri!=regions.end(); ri++ )
		    if ( *ri==pickedR ) break;
		  aString outCmd;
		  sPrintF(outCmd,"delete region %d\n",(*ri)->ID());
		  gi.outputToCommandFile(outCmd);
		  delete (*ri);
		  regions.erase(ri);
		}
	    }
	}
      else if ( (len=answer.matches("delete region") ) )
	{
	  int r=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d",&r);
	  RegionContainer::iterator ri=regions.begin();
	  for ( ; ri!=regions.end(); ri++ )
	    if ( r==(*ri)->ID() ) break;
	  if ( r==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no region with id %d!",r);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      delete *ri;
	      regions.erase(ri);
	    }
	}
      else if ( (len=answer.matches("delete curve") ) )
	{
	  int c=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d",&c);
	  CurveMappingContainer::iterator ci;
	  for ( ci=curves.begin(); 
		ci!=curves.end(); ci++ )
	    if ( ci->first==c ) break;

	  if ( c==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no curve with id %d!",c);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      delete ci->second;
	      curves.erase(ci);
	    }
	}
      else if ( (len=answer.matches("name region") ) )
	{
	  int r=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d",&r);
	  RegionContainer::iterator ri=regions.begin();
	  for ( ; ri!=regions.end(); ri++ )
	    if ( r==(*ri)->ID() ) break;
	  if ( r==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no region with id %d!",r);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      if ( answer(len+1,answer.length()-1).matches("- none -") )
		(*ri)->setName("");
	      else
		{
		  string nm1 = (const char *)answer(len+1,answer.length()-1);
		  int start = nm1.find_first_of(':')+1;
		  string name = nm1.substr(start, nm1.size()-start);
		  (*ri)->setName(name);
		}
	    }
	}
      else if ( (len=answer.matches("optimize region") ) )
	{
	  int r=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d",&r);
	  RegionContainer::iterator ri=regions.begin();
	  for ( ; ri!=regions.end(); ri++ )
	    if ( r==(*ri)->ID() ) break;
	  if ( r==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no region with id %d!",r);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      Region *pickedR = *ri;
	      if ( pickedR )
		{
		  UnstructuredRegion *ur=0;
		  if ( (ur=dynamic_cast<UnstructuredRegion*>(pickedR)) )
		    {
		      gi.outputString("optimizing...");
		      optimize((UnstructuredMapping &)ur->getMapping(),0);
		      gi.outputString("             ... done!");
		    }
		  else
		    {
		      gi.outputString("only unstructured regions can be optimized");
		      gi.createMessageDialog("only unstructured regions can be optimized",errorDialog);
		    }
		}
	    }
	}
      else if ( (len=answer.matches("points on curve") ) )
	{
	  int c=-1,np=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d %d",&c,&np);
	  CurveMappingContainer::iterator ci;
	  for ( ci=curves.begin(); 
		ci!=curves.end(); ci++ )
	    if ( ci->first==c ) break;

	  if ( c==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no curve with id %d!",c);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      SimpleCurve * sc = dynamic_cast<SimpleCurve*>(ci->second);
	      if ( sc && !ci->second->region_1())
		sc->setNumberOfPoints(np);
	    }
	}
      else if ( (len=answer.matches("uniform points on curve")) )
	{
	  int c=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d",&c);
	  CurveMappingContainer::iterator ci;
	  for ( ci=curves.begin(); 
		ci!=curves.end(); ci++ )
	    if ( ci->first==c ) break;

	  if ( c==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no curve with id %d!",c);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      SimpleCurve * sc = dynamic_cast<SimpleCurve*>(ci->second);
	      if ( sc && !ci->second->region_1())
		sc->deleteStretching();
	    }
	}
      else if ( (len=answer.matches("stretch points on curve")) )
	{
	  int c=-1;
	  sScanF(answer(len+1,answer.length()-1),"%d",&c);
	  CurveMappingContainer::iterator ci;
	  for ( ci=curves.begin(); 
		ci!=curves.end(); ci++ )
	    if ( ci->first==c ) break;

	  if ( c==-1 )
	    {
	      aString msg;
	      sPrintF(msg,"no curve with id %d!",c);
	      gi.outputString(msg);
	      gi.createMessageDialog(msg,errorDialog);
	    }
	  else
	    {
	      SimpleCurve * sc = dynamic_cast<SimpleCurve*>(ci->second);
	      if ( sc && !ci->second->region_1())
		sc->stretchPoints();
	    }
	}
      else if ( answer!="" && answer!="open graphics" )
	{
	  aString msg = "unknown command : "+answer;
	  gi.outputString(msg);
	  gi.createMessageDialog(msg,errorDialog);
	  gi.stopReadingCommandFile();
	}

      gi.erase(gi.getCurrentWindow(),true);

      if ( tbState[plotBG] )
	{
	  for ( vector<Region*>::iterator r=regions.begin(); 
		r!=regions.end(); r++ )
	    PlotIt::plot(gi,(Mapping &)(*r)->getReferenceGrid(), squareGP);
	}

      if ( tbState[plotCurves] )
	::plotCurves(gi,curveGP,curves);

      if ( tbState[plotPoints] )
	points.plot(gi,pointsGP);

      if ( tbState[plotRegions] )
	{
	  int rc = 0;
	  IntegerArray sq(regions.size());
	  for ( RegionContainer::iterator r=regions.begin(); r!=regions.end();
		r++ )
	    {
	      sq(rc) = (*r)->ID();
	      regionGP.set(GI_MAPPING_COLOUR, gi.getColourName(sq(rc++)));
	      PlotIt::plot(gi,(Mapping &)(*r)->getMapping(),regionGP);
	    }
	  gi.drawColouredSquares(sq);
	}
    }

  gi.popGUI();

//    if ( regions.size() )
//      writeToIngrid("test.msh", points, curves, regions);

  for ( RegionContainer::iterator r=regions.begin(); r!=regions.end(); r++ )
    delete *r;

  regions.clear();

  for ( CurveMappingContainer::iterator c=curves.begin(); c!=curves.end(); c++ )
    delete c->second;
      
  curves.clear();

  Overture::finish();

  return 0;
}

namespace {

  Region *
  createTFIRegion(GenericGraphicsInterface &gi, 
		  CurveMappingContainer &curves, real dx, real dy)
  {
    // make a tfi region by selecting 4 bounding curves 

    if ( curves.size()<4 )
      { // there are not enough curves available in the list
	aString msg="at least 4 curves are neccessary to make a TFI Region";
	gi.outputString(msg);
	gi.createMessageDialog(msg, errorDialog);
	return 0;
      }

    vector<aString> curveNames;
    curveNames.push_back("left");
    curveNames.push_back("right");
    curveNames.push_back("bottom");
    curveNames.push_back("top");

    vector<Curve *>boundingCurves;
    if ( !
	 collectCurves(gi, curves, "TFI Curve Selection", 4, 4, 
		       curveNames, false, boundingCurves) )
      return 0; // cancelled!

    TFIRegion *newReg = new TFIRegion(dx, dy);

    // now make sure the 4 curves are "consistent", ie thier points match up at
    //   the corners.  Each endpoint in the curves must be used exactly twice.
    //   Also the opposing sides must have the same number of vertices on each curve,
    //   or the curves must not currently be discretized.
    int c;
    map<int,int> ptCnt;
    bool consistent = true;

    for ( c=0; c<4 && consistent; c++ )
      {
	// log end point use
	int ps = boundingCurves[c]->getStartPointID();
	int pe = boundingCurves[c]->getEndPointID();

	if ( ptCnt.count(ps)==0 )
	  ptCnt[ps]=1;
	else
	  ptCnt[ps]++;

	if ( ptCnt.count(pe)==0 )
	  ptCnt[pe]=1;
	else
	  ptCnt[pe]++;

	// now check opposite curve
	if ( boundingCurves[c]->region_1() )
	  {
	    // this curve has already been discretized
	    // does it match the other side? 
	    // is the other side even discretized?
	    int cn = c + (c==0||c==2 ? 1 : -1);
	    if ( boundingCurves[cn]->region_1() )
	      consistent = 
		(boundingCurves[c]->numberOfPoints()==boundingCurves[cn]->numberOfPoints())||
		(boundingCurves[c]->numberOfPoints()==0) || 
		(boundingCurves[cn]->numberOfPoints()==0);

	    if ( !consistent )
	      {
		aString msg="The "+curveNames[c]+" and "+curveNames[cn]+
		  " do not match grid sizes!";
		gi.outputString(msg);
		gi.createMessageDialog(msg,errorDialog);
	      }
	  }
      }

    if ( !consistent )
      {
	delete newReg;
	return 0;
      }

    // each point should be used twice and only twice
    for ( map<int,int>::iterator mc=ptCnt.begin(); mc!=ptCnt.end() && consistent;
	  mc++ )
      consistent= mc->second==2;

    if ( !consistent )
      {
	delete newReg;
	aString msg = "Endpoints of curves must match at corners!";
	gi.outputString(msg);
	gi.createMessageDialog(msg,errorDialog);
	return 0;
      }

    // ok! we made it this far! the curves are ok, add them to the region
    // and build the tfi.
    for ( c=0; c<4; c++ )
      newReg->addCurve(boundingCurves[c]);

    // pull up a dialog for the name and dx,dy
    GUIState regDialog;
    regDialog.setExitCommand("generate","Generate!");
    regDialog.setWindowTitle("TFI Region Options");
    aString txtCmd[] = { "dx,dy :", "name", "" };
    aString txtInit[] = { "", "- none -", "" };
    sPrintF(txtInit[0],"%g, %g",dx,dy);
    regDialog.setTextBoxes(txtCmd,txtCmd,txtInit);
    gi.pushGUI(regDialog);
    aString answer="";
    int len;
    while(1)
      {
	gi.getAnswer(answer,"");
	
	if ( (len=answer.matches(txtCmd[0])) )
	  {
	    sScanF(answer(len+1,answer.length()-1),"%g, %g", &dx, &dy);
	    if ( dx<=0. || dy<=0. ) 
	      {
		aString msg = "make sure dx>0 and dy>0!";
		gi.outputString(msg);
		gi.createMessageDialog(msg, errorDialog);
	      }
	    else
	      {
		aString str;
		sPrintF(str,"%g, %g",dx,dy);
		regDialog.setTextLabel(txtCmd[0],str);
		newReg->setDx(dx);
		newReg->setDy(dy);
	      }
	  }
	else if ( (len=answer.matches(txtCmd[1])) )
	  newReg->setName((const char *)answer(len+1,answer.length()-1));
	else if ( answer=="generate" )
	  break;
      }
    gi.popGUI();

    return newReg;
  }

  Region *createUnstructuredRegion(GenericGraphicsInterface &gi, 
				   CurveMappingContainer &curves, real dx, real dy)
  {
    if ( curves.size()<1 )
      { // there are not enough curves available in the list
	aString msg="at least 1 curve is neccessary to make an Unstructured Region";
	gi.outputString(msg);
	gi.createMessageDialog(msg, errorDialog);
	return 0;
      }

    vector<aString> curveNames;
    curveNames.push_back("outer");
    curveNames.push_back("inner");
    vector<Curve *>boundingCurves;
    if ( !
	 collectCurves(gi, curves, "Unstructured Region Curve Selection", 1, -1, 
		       curveNames, true, boundingCurves) )
      return 0; // cancellation

    UnstructuredRegion *newReg = new UnstructuredRegion(dx, dy);

    for ( int c=0; c<boundingCurves.size(); c++ )
      if ( boundingCurves[c] ) newReg->addCurve(boundingCurves[c]);

    // pull up a dialog for the name and dx,dy
    GUIState regDialog;
    aString toggle[] = { "use cutout",""};
    int toginit[] = { 1 };

    regDialog.setToggleButtons(toggle,toggle,toginit);
    
    regDialog.setExitCommand("generate","Generate!");
    regDialog.setWindowTitle("Unstructured Region Options");
    aString txtCmd[] = { "dx,dy : ", "name", "" };
    aString txtInit[] = { "", "- none -", "" };
    sPrintF(txtInit[0],"%g, %g",dx,dy);
    regDialog.setTextBoxes(txtCmd,txtCmd,txtInit);
    gi.pushGUI(regDialog);
    aString answer="";
    int len;
    while(1)
      {
	gi.getAnswer(answer,"");
	
	if ( (len=answer.matches(txtCmd[0])) )
	  {
	    sScanF(answer(len+1,answer.length()-1),"%g %g", &dx, &dy);
	    if ( dx<=0. || dy<=0. ) 
	      {
		aString msg = "make sure dx>0 and dy>0!";
		gi.outputString(msg);
		gi.createMessageDialog(msg, errorDialog);
	      }
	    else
	      {
		aString str;
		sPrintF(str,"%g, %g",dx,dy);
		regDialog.setTextLabel(txtCmd[0],str);
		newReg->setDx(dx);
		newReg->setDy(dy);
	      }
	  }
	else if ( (len=answer.matches(txtCmd[1])) )
	  newReg->setName((const char *)answer(len+1,answer.length()-1));
	else if ( answer.matches(toggle[0]) )
	  {
	    toginit[0] = !toginit[0];
	    regDialog.setToggleState(0, toginit[0]);
	    if ( toginit[0] )
	      newReg->useCutout();
	    else
	      newReg->dontUseCutout();
	  }
	else if ( answer=="generate" )
	  break;
      }
    gi.popGUI();

    return newReg;
  }

  void plotCurves(GenericGraphicsInterface &gi, 
		  GraphicsParameters &gp, CurveMappingContainer &crv )
  {
    for ( CurveMappingContainer::iterator c=crv.begin(); 
	  c!=crv.end(); c++ )
      {
	if ( ! dynamic_cast<CompositeCurve *>(c->second) )
	  {
	    gp.set(GI_MAPPING_COLOUR,gi.getColourName(c->first));
	    PlotIt::plot(gi,(Mapping &)c->second->getNurbs(),gp);
	  }
      }
  }

  bool collectCurves(GenericGraphicsInterface &gi, CurveMappingContainer &curves,
		     aString windowTitle, int nCurvesMin, int nCurvesMax, 
		     vector<aString> &curveNames, bool checkPeriodic, 
		     vector<Curve*> &boundingCurves)
  {
    // bounding curve markers and pointers
    //    aString curveNames[] = {"outer","inner"};
    //    Curve *boundingCurves[] = {0,0};

    //    boundingCurves[0]=boundingCurves[1]=0;
    int nCurvesExpected = nCurvesMax>0 ? nCurvesMax : 10; 
    boundingCurves.reserve(nCurvesExpected);
    for ( int i=0; i<max(nCurvesMin,nCurvesMax); i++ ) boundingCurves.push_back(0);

    // build the gui, the info label will change as curves are selected
    GUIState curveSelectionGUI;
    curveSelectionGUI.setWindowTitle(windowTitle);
    curveSelectionGUI.setExitCommand("Cancel","Cancel"); 
    curveSelectionGUI.addInfoLabel(""); 

    aString pbCmd[] = { "Done","" };
    curveSelectionGUI.setPushButtons(pbCmd,pbCmd,1);

    GraphicsParameters gp;
    gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    gp.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);
    // set up the graphics parameters so that selected curves
    //  will show up as bold black lines
    real lw;
    gp.get(GraphicsParameters::curveLineWidth,lw);

    unsigned int curveNameIndex=0; // name of the current curve we are reading
    unsigned int curveIndex=0; // index in boundingCurves of the curve we are reading

    bool finishedSpecifyingCurves=false;
    aString answer="";
    SelectionInfo select;
  
    gi.pushGUI(curveSelectionGUI);
    bool foundCurve=true;
    int len;

    Curve *curveToAdd=0;

    ArraySimple<bool> alreadyUsed(curves.size());
    for ( int i=0; i<curves.size(); i++ ) alreadyUsed[i]=false;

    bool lastCurveFromCmd=false;
    unsigned int nCurvesAdded =0;

    int it=0;

    while(!finishedSpecifyingCurves)
      {
	len = 0;

	if (foundCurve) 
	  curveSelectionGUI.setInfoLabel(0,"select "+curveNames[curveNameIndex]+" curve");

	
	gi.savePickCommands(false); // temporarily turn off saving of pick commands.
	if ( it!=0 ) gi.getAnswer(answer,"",select);
	gi.savePickCommands(true);

	if ( it==0 )
	  { // just plot it
	    it++;
	  }
	else if ( answer.matches("Cancel") )
	  {
	    // make sure any newly created composite curves are deleted
	    if ( curveToAdd )
	      {
		if ( dynamic_cast<CompositeCurve *>(curveToAdd) ) 
		  {
		    curves.erase(curves.size()-1);
		    delete curveToAdd;
		  }
		curveToAdd=0;
	      }

	    for ( int bc=0; bc<boundingCurves.size(); bc++ )
	      {
		if ( boundingCurves[bc] )
		  {
		    if ( dynamic_cast<CompositeCurve *>(boundingCurves[bc]) )
		      {
			for ( CurveMappingContainer::iterator c=curves.begin(); 
			      c!=curves.end(); c++ )
			  if ( boundingCurves[bc]==c->second )
			    {
			      curves.erase(c->first);
			      delete boundingCurves[bc];
			    }
		      }
		    
		    boundingCurves[bc]=0;
		  }
	      }
	    boundingCurves.clear();
	    finishedSpecifyingCurves = true;
	    curveNameIndex=curveIndex=0;
	    //	    gi.outputToCommandFile("Cancel");
	  }
	else if ( answer.matches("Done") )
	  {
	    if ( curveToAdd )
	      {
		bool periodicOk = 
		  checkPeriodic ? 
		  curveToAdd->getStartPointID()==curveToAdd->getEndPointID() : true;
		
		if ( periodicOk )
		  {
		    nCurvesAdded++;
		    if ( curveIndex<boundingCurves.size() && curveIndex>=0 )
		      boundingCurves[curveIndex] = curveToAdd;
		    else
		      boundingCurves.push_back(curveToAdd);

		    if ( lastCurveFromCmd )
		      for ( curveIndex=0; 
			    curveIndex<boundingCurves.size() && 
			      boundingCurves[curveIndex]; curveIndex++ ) 
			{
			  if ( curveIndex<curveNames.size() )
			    curveNameIndex = curveIndex;
			}
		    else
		      {
			curveIndex++;
			curveNameIndex = min((unsigned long )curveIndex, (unsigned long)curveNames.size()-1);
		      }
		    curveToAdd=0;
		    finishedSpecifyingCurves = nCurvesAdded == nCurvesMax;
		  }
		else
		  {
		    // not done yet!
		    aString msg="Curve must be periodic!";
		    gi.outputString(msg);
		    gi.createMessageDialog(msg,errorDialog);
		  }
	      }
	    else if ( curveIndex>=(nCurvesMin-1) && nCurvesMax==-1 ) 
	      finishedSpecifyingCurves=true;
	  }
	else if ( select.nSelect>0 || (len=answer.matches("select")) )
	  {
	    Curve *selectedC = 0;
	    int cc=0;
	    if ( select.nSelect>0 ) // make a selection with the mouse
	      {
		for ( int ss=0; ss<select.nSelect && !selectedC; ss++ )
		  {
		    cc=0;
		    for ( CurveMappingContainer::iterator sc=curves.begin(); 
			  sc!=curves.end() && !selectedC;
			  sc++,cc++ )
		      {
			if ( dynamic_cast<SimpleCurve*>(sc->second) )
			  {
			    int gid = sc->second->getNurbs().getGlobalID();
			    if ( gid==select.selection(ss,0) )
			      selectedC = sc->second;
			  }
		      }
		  }
		cc--;
	      }
	    else // read a selection from the command file
	      {
		char cName[20];
		//		int cc;
		sScanF(answer(len+1,answer.length()),"%s %d",cName,&cc);
		int cfind=0;
		CurveMappingContainer::iterator sc=curves.begin();
		//		while (cfind++!=cc) sc++;
		while (sc->first!=cc) 
		  {
		    cfind++; sc++; 
		  }
		cc = cfind;
		selectedC = sc->second;
		curveNameIndex=0;
		while (curveNames[curveNameIndex]!=cName && 
		       curveNameIndex<curveNames.size()) curveNameIndex++;
	      }

	    bool duplicate = alreadyUsed[cc];

	    if ( !duplicate )//&& isPeriodic ) 
	      {
		//		aString cmd;
		//		gi.outputToCommandFile(cmd);

		//boundingCurves[c] = selectedC;
		CompositeCurve *compC=0;
		SimpleCurve *simpC=0;
		bool curveAdded = true;
		if ( curveToAdd && 
		     (compC=dynamic_cast<CompositeCurve*>(curveToAdd) ))
		  {
		    if (!compC->push(selectedC))
		      {
			curveAdded = false;
			// curve was not connected to the previous one
			aString msg = "Curves for composite curve must be unique and match at a point!";
			gi.outputString(msg);
			gi.createMessageDialog(msg, errorDialog);
			
		      }
		  }
		else if ( !curveToAdd )
		  curveToAdd = selectedC;
		else if ( curveToAdd && 
			  (simpC=dynamic_cast<SimpleCurve*>(curveToAdd) ))
		  {
		    Curve *oldC = curveToAdd;
		    curveToAdd = new CompositeCurve();
		    ((CompositeCurve *)curveToAdd)->push(oldC);
		    if ( !((CompositeCurve *)curveToAdd)->push(selectedC) )
		      {
			curveAdded = false;
			// curve was not connected to the previous one
			aString msg = "ERROR: Could not begin Composite Curve!";
			gi.outputString(msg);
			gi.createMessageDialog(msg, errorDialog);
			delete curveToAdd;
			curveToAdd = oldC;
		      }
		    else
		      {
			int id = curves.size();
			curves[id] = curveToAdd;
		      }

		  }
		  
		if ( curveAdded )
		  {
		    if ( len ) // find the next curve to use, or we are done
		      lastCurveFromCmd=true;//for ( c=0; c<2 && boundingCurves[c]; c++ ) ;
		    else 
		      { // since a mouse command was used, log the selection 
			//  intelligably to the command file
			lastCurveFromCmd=false;
			aString cmd="";
			sPrintF(cmd,"select %s %d\n",
				(char *)((const char *)curveNames[curveNameIndex]),selectedC->ID());
			gi.outputToCommandFile(cmd);
			//		    c++;
		      }
		    alreadyUsed[cc]=true;
		    foundCurve = true;
		  }
	      }
	    else if ( duplicate )
	      {
		aString msg="Curves can only be selected once!";
		gi.outputString(msg);
		gi.createMessageDialog(msg,errorDialog);
	      }
	  }
	else
	  finishedSpecifyingCurves=false;
	
	// plot the currently selected curves
	gi.erase(gi.getCurrentWindow(),true);
	gp.set(GraphicsParameters::curveLineWidth,2*lw);
	gp.set(GI_MAPPING_COLOUR,"black");
	for ( int cp=0; cp<boundingCurves.size(); cp++ )
	  if ( boundingCurves[cp] )
	    PlotIt::plot(gi,boundingCurves[cp]->getNurbs(),gp);

	if ( curveToAdd )
	  PlotIt::plot(gi,curveToAdd->getNurbs(),gp);

	gp.set(GraphicsParameters::curveLineWidth,lw);
	::plotCurves(gi,gp,curves);

//  	finishedSpecifyingCurves = finishedSpecifyingCurves || 
//  	  curveIndex==max(nCurvesMax-1,(int)boundingCurves.size());
      }
    
    gi.popGUI();

    return boundingCurves.size()>0;

  }

  void writeNode(ofstream &o, int id, real x, real y)
  {
    o<<" "<<id+1<<" "<<x<<" "<<y<<" "<<endl;
  }

  void writeElement(ofstream &o, int eid, int rid, ArraySimpleFixed<int, 4,1,1,1> &eids)
  {
    o<<setw(8)<<eid+1<<setw(5)<<rid;
    for ( int v=0; v<4 ; v++ )
      o<<setw(8)<<eids[v]+1;
    o<<" "<<endl;
  }

  void writeToIngrid(std::string fileName, PointList &points, CurveMappingContainer & curves,
		     RegionContainer &regions)
  {
    int nVerts=points.size();
    int nElems=0;
    for ( RegionContainer::iterator r=regions.begin(); r!=regions.end(); r++ )
      {
	int nCrvPts = (*r)->numberOfCurves(); // curve are bounded by periodic loops
	for ( Region::curve_iterator ci=(*r)->curve_begin(); ci!=(*r)->curve_end(); ci++ )
	  nCrvPts += (*ci)->numberOfPoints()-2; 
	

	nVerts+=(*r)->numberOfVertices()-nCrvPts;
	nElems+=(*r)->numberOfElements();
      }

    // now add back curve points
    for ( CurveMappingContainer::iterator ci=curves.begin(); ci!=curves.end(); ci++ )
      {
	Curve *c = ci->second;
	if ( c->region_1() && !dynamic_cast<CompositeCurve *>(c) )
	  nVerts+=c->numberOfPoints()-2;
      }

    int nRegions = regions.size();

    ofstream igf(fileName.c_str());
    igf.precision(12);
    igf.setf(ios::scientific,ios::floatfield);

    igf<<"OVERTUREUMapping : SMESH Unstructured Mesh File : Overture UMapping format"<<endl;
    igf<<" "<<nRegions<<" "<<nVerts<<" "<<nElems<<" "<<4<<" "<<2<<" "<<2<<"  "<<endl;
    
    // First all the vertices are written out.  We will start with 
    //   points, then write all the curves, and then the interiors of all the regions

    // reset the vertex id information so we can start anew
    resetVertexIDs();
    for ( CurveMappingContainer::iterator ci=curves.begin(); ci!=curves.end(); ci++ )
      ci->second->resetIDList();
    for ( RegionContainer::iterator r=regions.begin(); r!=regions.end(); r++ )
      (*r)->resetIDList();
    
    int pTotal = points.size();
    for ( int p=0; p<points.size(); p++ )
      {
	int id = getVertexID();
	writeNode(igf, id, points[p].coordinate[0], points[p].coordinate[1]);
      }
    
    for ( CurveMappingContainer::iterator ci=curves.begin(); ci!=curves.end(); ci++ )
      {
	Curve *c = ci->second;
	if ( c->region_1() && !dynamic_cast<CompositeCurve*>(c) )
	  {
	    ArraySimple<int> & gridIDList = c->getGridIDList();
	    //	    	    	    cout<<"GRID ID LIST FOR CURVE "<<gridIDList<<endl;
	    if ( gridIDList.size() )
	      {
		ArraySimple<real> & grid = c->getVertices();
		for ( int p=1; p<grid.size(0)-1; p++ )
		  writeNode(igf, gridIDList[p],grid(p,0),grid(p,1));
	      }
	    pTotal+=gridIDList.size()-2;
	  }
      }

    for ( RegionContainer::iterator ri=regions.begin(); ri!=regions.end(); ri++ )
      {
	int idBase = currentIDCounter();
	Region *r = *ri;
	ArraySimple<int> & gridIDList = r->getGridIDList();
	if ( gridIDList.size() )
	  {
	    ArraySimple<real> &grid = r->getVertices();
	    for ( int p=0; p<grid.size(0); p++ )
	      {
		if ( gridIDList[p]>=idBase )
		  writeNode(igf, gridIDList[p],grid(p,0),grid(p,1));
		pTotal++;
	      }
	  }
	//		cout<<gridIDList<<endl;
      }

    // now write out the elements in each region
    int elemID=0;
    for ( RegionContainer::iterator ri=regions.begin(); ri!=regions.end(); ri++ )
      {
	Region *r = *ri;
	ArraySimple<int> & gridIDList = r->getGridIDList();
	for ( int e=0; e<r->numberOfElements(); e++ )
	  {
  	    ArraySimpleFixed<int,4,1,1,1> eids = r->getElement(e);
  	    writeElement(igf,elemID++,r->ID(),eids);
	    //	    igf<<elemID++<<" "<<r->ID()<<endl;
	  }
      }

    // write out region numbers and names
    for ( RegionContainer::iterator ri=regions.begin(); ri!=regions.end(); ri++ )
      {
	Region *r = *ri;
	igf<<r->ID()<<"  "<<r->getName()<<endl;
      }

    // write out a summary of the data in this file
    igf<<"FORMAT SUMMARY :: \nelements vertices specified counter-clockwise, \n1 based index with 0 specifying a null vertex (vertex 4 of a triangle)"<<endl;
    igf<<"line 1 : Comment Line"<<endl;
    igf<<"line 2 : nRegions nVertices nElements maxNVertsInElement domainDimension rangeDimension"<<endl;
    igf<<"line 3 : 0 x0 y0"<<endl;
    igf<<"line 4 : 1 x1 y1"<<endl;
    igf<<"..."<<endl;
    igf<<"line nVertices+2 : nVertices-1 xNv yNv "<<endl;
    igf<<"line nVertices+3 : 0 reg0 e0v1 e0v2 e0v3 e0v4"<<endl;
    igf<<"line nVertices+4 : 1 reg1 e1v1 e1v2 e1v3 "<<endl;
    igf<<"..."<<endl;
    igf<<"line nVertices+2+nElements : nElements regNe eNv1 eNv2 eNv3 eNv4"<<endl;
    igf<<"line nVertices+3+nElements : region0ID region0Name"<<endl;
    igf<<"..."<<endl;
    igf<<"line nVerties+3+nElements+nRegions : regionN regionNName"<<endl;
    igf<<"This Summary"<<endl;
  
    igf.close();
  }
  
}
