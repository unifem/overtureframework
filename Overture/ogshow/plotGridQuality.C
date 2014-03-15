#include <algorithm>

#include "Overture.h"
#include "PlotStuff.h"
#include "PlotIt.h"
#include "GridCollectionOperators.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "gridFunctionNorms.h"

using namespace std;

// ========= we need a version for CompositeGrid and MappedGrid =====


void PlotIt::
plotMappingQuality( GenericGraphicsInterface & gi, Mapping & map,
                    GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ==============================================================================
//  /Description: plot the quality of a Mapping. 
//    This is done by building a MappedGrid  and checking the quality of the grid.
// 
// ==============================================================================
{
  if( !gi.graphicsIsOn() ) return;

  MappedGrid mg(map);

  const int numGhost=3;
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    for( int side=Start; side<=End; side++ )
      mg.setNumberOfGhostPoints(side,axis,numGhost);

  mg.updateReferences();
  plotGridQuality(gi,mg);
  return;
  
}

void PlotIt::
plotGridQuality( GenericGraphicsInterface & gi, MappedGrid & mg,
		 GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ==============================================================================
//  /Description: plot the quality of a MappedGrid.
// ==============================================================================
{
  if( !gi.graphicsIsOn() ) return;

  GridCollection gc(mg.numberOfDimensions(),1);  // make a collection with 1 component grid
  gc[0].reference(mg);
  gc.updateReferences();

  plotGridQuality(gi,gc,parameters );
  
  return;
}




namespace // this makes the next class local to this file
{

class GridError
{
public:
  GridError( int grid_, real maxErr_, real l2Err_ ){ grid=grid_; maxErr=maxErr_; l2Err=l2Err_; }
  // For sorting by maxErr:
  bool operator< ( const GridError & x )const{ return maxErr < x.maxErr; } 

  real maxErr,l2Err;
  int grid;

};
}


void PlotIt::
plotGridQuality( GenericGraphicsInterface & gi, 
                 GridCollection & gc,
		 GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
// ==============================================================================
//  /Description: 
//    Plot the quality of a GridCollection
// ==============================================================================
{
  if( !gi.graphicsIsOn() ) return;

  const int numberOfDimensions = gc.numberOfDimensions();

  int orderOfAccuracy=2;

  const aString prefix="GC:";
  
  GUIState gui;

  DialogData & dialog=gui;

  dialog.setWindowTitle("Grid Quality Plotter");
  dialog.setExitCommand("continue", "continue");

  dialog.setOptionMenuColumns(1);

  const int numLabels=6;
  aString opLabel[numLabels] = {"laplacian errors",
                                "xr",
				"xrr",
				"skewness",
                                "derivative errors", // plot errors in u.x(), u.xx() etc. for u={1,x,y,x^2,...}
				""}; //
  aString opCmd[numLabels];
  GUIState::addPrefix(opLabel,prefix,opCmd,numLabels);

  int initialPlotOption=0;
  dialog.addOptionMenu("plot:", opCmd,opLabel,initialPlotOption);

  enum TZEnum
  {
    polynomial=0,
    trigonometric=1,
    numberOfTZEnum
  } twilightZoneOption=trigonometric;
  
  const int numLabels2=numberOfTZEnum+1;
  aString opLabel2[numLabels2] = {"polynomial",
				  "trigonometric",
				""}; //
  aString opCmd2[numLabels2];
  GUIState::addPrefix(opLabel2,prefix,opCmd2,numLabels2);

  dialog.addOptionMenu("analytic solution:", opCmd2,opLabel2,(int)twilightZoneOption);


//    aString pushButtonCommands[] = {"plot xr",
//                                    "plot xrr",
//                                    "plot skewness",
//                                    "erase",
//  				  ""};
//    int numRows=4;
//    dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 


  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "order of accuracy";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",orderOfAccuracy); nt++; 

  int degreeInSpace=2;  // for polynomial TZ
  textCommands[nt] = "degree in space";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",degreeInSpace); nt++; 

  // scale the trig polynomial by the size of the domain
  RealArray xBound(2,3);
  getGridBounds(gc,parameters,xBound);
  real fv[3]={1.,1.,1.};  //
  real &fx=fv[0], &fy=fv[1], &fz=fv[2];
  real xScale=REAL_MIN*100.;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    xScale=max(xScale,xBound(1,axis)-xBound(0,axis));
  }
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    // fv[axis]=2./max(xBound(1,axis)-xBound(0,axis),REAL_MIN*100.);
    fv[axis]=2./xScale;
  }

  textCommands[nt] = "fx, fy, fz";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f %f %f",fx,fy,fz); nt++; 

  int numberOfGhostPointsToCheck=1;
  textCommands[nt] = "ghost points to check";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfGhostPointsToCheck); nt++; 

  textCommands[nt] = "plot quality for grid";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",-1); nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);


  gi.pushGUI(gui);

  GraphicsParameters par;
  
  aString answer,line;
  int len=0;
  for(;;) 
  {
    gi.getAnswer(answer,"");      

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    if( answer=="continue" || answer=="exit" )
    {
      break;
    }
    
    else if( dialog.getTextValue(answer,"order of accuracy","%i",orderOfAccuracy) ){}// 
    else if( dialog.getTextValue(answer,"degree in space","%i",degreeInSpace) ){}// 
    else if( dialog.getTextValue(answer,"ghost points to check","%i",numberOfGhostPointsToCheck) ){}// 
    // else if( dialog.getTextValue(answer,"fx, fy, fz","%e %e %e",fx,fy,fz) ){}// 
    else if( len=answer.matches("fx, fy, fz") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&fx,&fy,&fz);
      dialog.setTextLabel("fx, fy, fz",sPrintF("%f %f %f",fx,fy,fz));
    }
    else if( len=answer.matches("plot quality for grid") )
    {
      int gridToPlot=-1;
      sScanF(answer(len,answer.length()-1),"%i",&gridToPlot);
      if( gridToPlot>=0 && gridToPlot<gc.numberOfComponentGrids() )
      {
	plotGridQuality(gi,gc[gridToPlot],parameters);
      }
      else
      {
        printF("ERROR: invalid grid to plot=%i, there are only %i grids\n",gridToPlot,gc.numberOfComponentGrids());
      }
      
    }
    else if( answer=="xr" ) 
    {
      dialog.getOptionMenu("plot:").setCurrentChoice(answer);
      gc.update(MappedGrid::THEvertexDerivative);
      
      Range all;
      realGridCollectionFunction xr(gc,all,all,all,SQR(numberOfDimensions));
      xr.setName("xr",0);
      if( numberOfDimensions>1 )
      {
        xr.setName("yr",1);
        xr.setName("xs",2);
	xr.setName("ys",3);
      }
      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
        xr[grid]=gc[grid].vertexDerivative();
      }
      
	
      gi.erase();
      PlotIt::contour(gi,xr,par);
    }
    else if( answer=="xrr" )
    {
      dialog.getOptionMenu("plot:").setCurrentChoice(answer);

      // mg.update(MappedGrid::THEvertexDerivative);
      gc.update(MappedGrid::THEcenter);
      
      Range all;
      const int nc = numberOfDimensions==1 ? 1 : numberOfDimensions==2 ? 6 : 18;
      realGridCollectionFunction xrr(gc,all,all,all,nc);
      xrr=0.;
      
      xrr.setName("xrr",0);
      if( numberOfDimensions==2 )
      {
        xrr.setName("yrr",1);
        xrr.setName("xss",2);
	xrr.setName("yss",3);
	xrr.setName("xrs",4);
	xrr.setName("yrs",5);
      }
      else if( numberOfDimensions==3 )
      {
        xrr.setName("yrr",1); 
        xrr.setName("zrr",2); 
        xrr.setName("xss",3);
	xrr.setName("yss",4);
	xrr.setName("zss",5);
        xrr.setName("xtt",6);
	xrr.setName("ytt",7);
	xrr.setName("ztt",8);
	xrr.setName("xrs",9);
	xrr.setName("yrs",10);
	xrr.setName("zrs",11);
	xrr.setName("xrt",12);
	xrr.setName("yrt",13);
	xrr.setName("zrt",14);
	xrr.setName("xst",15);
	xrr.setName("yst",16);
	xrr.setName("zst",17);
      }
      
      // const realArray & xr= mg.vertexDerivative();

      GridCollectionOperators gcop(gc);
      gcop.setOrderOfAccuracy(orderOfAccuracy);


      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = gc[grid];
	realArray & xrr0 = xrr[grid];
	
	const realMappedGridFunction & x = mg.center();
        MappedGridOperators & op = gcop[grid];

	Index I1,I2,I3;
	// getIndex(mg.dimension(),I1,I2,I3);

	if( numberOfDimensions==1 )
	{
	  xrr0(I1,I2,I3,0)=op.r1r1(x,I1,I2,I3,0);
	}
	else if( numberOfDimensions==2 )
	{
	  xrr0(I1,I2,I3,0)=op.r1r1(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3,1)=op.r1r1(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3,2)=op.r2r2(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3,3)=op.r2r2(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3,4)=op.r1r2(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3,5)=op.r1r2(x,I1,I2,I3,1);
	}
	else if( numberOfDimensions==3 )
	{
	  xrr0(I1,I2,I3, 0)=op.r1r1(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3, 1)=op.r1r1(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3, 2)=op.r1r1(x,I1,I2,I3,2);

	  xrr0(I1,I2,I3, 3)=op.r2r2(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3, 4)=op.r2r2(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3, 5)=op.r2r2(x,I1,I2,I3,2);

	  xrr0(I1,I2,I3, 6)=op.r3r3(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3, 7)=op.r3r3(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3, 8)=op.r3r3(x,I1,I2,I3,2);

	  xrr0(I1,I2,I3, 9)=op.r1r2(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3,10)=op.r1r2(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3,11)=op.r1r2(x,I1,I2,I3,2);

	  xrr0(I1,I2,I3,12)=op.r1r3(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3,13)=op.r1r3(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3,14)=op.r1r3(x,I1,I2,I3,2);

	  xrr0(I1,I2,I3,15)=op.r2r3(x,I1,I2,I3,0);
	  xrr0(I1,I2,I3,16)=op.r2r3(x,I1,I2,I3,1);
	  xrr0(I1,I2,I3,17)=op.r2r3(x,I1,I2,I3,2);

	}
	else
	{
	  Overture::abort("error: numberOfDimensions");
	}
      }
      
      gi.erase();
      PlotIt::contour(gi,xrr,par);
    }
    else if( answer=="skewness" )
    {
      dialog.getOptionMenu("plot:").setCurrentChoice(answer);

      gc.update(MappedGrid::THEvertexDerivative);

      Range all;
      realGridCollectionFunction skew(gc,all,all,all);
      skew.setName("skewness");
      skew.setName("skewness",0);

      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = gc[grid];

	realMappedGridFunction & xr = mg.vertexDerivative();
	realMappedGridFunction & sr = skew[grid];

	//  skew = [ x.r dot x.s ] / ( ||x.r|| ||x.s|| ) 
	// **only for 2d**
	sr= (xr(all,all,all,0,0)*xr(all,all,all,0,1)+xr(all,all,all,1,0)*xr(all,all,all,1,1))/
	  (SQRT(SQR(xr(all,all,all,0,0)) + SQR(xr(all,all,all,1,0)))*
	   SQRT(SQR(xr(all,all,all,0,1)) + SQR(xr(all,all,all,1,1))));
	
      }
      
      gi.erase();
      PlotIt::contour(gi,skew,par);
    }
    else if( answer=="laplacian errors" )
    {
      dialog.getOptionMenu("plot:").setCurrentChoice(answer);

      gc.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEinverseVertexDerivative);

      GridCollectionOperators gcop(gc);
      gcop.setOrderOfAccuracy(orderOfAccuracy);

      Range all;
      realGridCollectionFunction du(gc,all,all,all,2);
      
      du.setName("errors");
      du.setName("err(Delta u)",0);
      du.setName("u",1);
      

      // assign twilight zone

      // create a twilight-zone function for checking the errors
      OGFunction *exactPointer;
      if( twilightZoneOption==trigonometric )
      {
	printf("TwilightZone: trigonometric polynomial, fx=%9.3e, fy=%9.3e, fz=%9.3e\n",fx,fy,fz);
	exactPointer = new OGTrigFunction(fx,fy,fz); 
      }
      else
      {
	printf("TwilightZone: algebraic polynomial, degreeInSpace=%i\n",degreeInSpace);

	int degreeOfSpacePolynomial = degreeInSpace; 
	int degreeOfTimePolynomial = 0;
	int numberOfComponents = gc.numberOfDimensions();
	exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,gc.numberOfDimensions(),numberOfComponents,
					  degreeOfTimePolynomial);
    
      
      }
      OGFunction & exact = *exactPointer;

      Index I1,I2,I3;
      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = gc[grid];
	getIndex(mg.dimension(),I1,I2,I3);  

        realMappedGridFunction ug(mg);
        ug.setOperators(gcop[grid]);

        realArray fg(I1,I2,I3);
        realArray & dug = du[grid];
	

	ug(I1,I2,I3)=exact(mg,I1,I2,I3,0);

	// find out how many ghost points:
	int numGhost = min(mg.numberOfGhostPoints()(all,Range(0,mg.numberOfDimensions()-1)));
	numGhost-=orderOfAccuracy/2;

        numGhost=min(numGhost,numberOfGhostPointsToCheck);
	
	printf("--> grid=%i, evaluate derivatives at %i ghost points\n",grid,numGhost);

	// compute derivatives at ghost points where possible
	getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost); // include ghost if possible

	const real eps=REAL_MIN*1000.;
	dug=0.;
	fg(I1,I2,I3)=exact.laplacian(mg,I1,I2,I3,0);
	dug(I1,I2,I3,0)=(ug.laplacian(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));

	dug(all,all,all,1)=ug(all,all,all);  // just plot u

	du[grid].periodicUpdate();
      }

      for( int n=du.getComponentBase(0); n<=du.getComponentBound(0); n++ )
      {
        printF("\n"
               " Values of %s sorted by max-value\n"
               " -------------------------------------------------\n",(const char*)du.getName(n));
        // Sort grids by largest max-err
        std::vector<GridError> gridErrList;
	for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
	  real maxErr= maxNorm(du[grid], n );
	  real l2Err = l2Norm( du[grid], n );
          gridErrList.push_back(GridError(grid,maxErr,l2Err));
	}
        sort(gridErrList.begin(), gridErrList.end());  // sort from smallest to largest maxErr
        // print from largest to smallest: 
	for( int g=gc.numberOfComponentGrids()-1; g>=0; g-- )
	{
          int grid=gridErrList[g].grid;
	  real maxErr=gridErrList[g].maxErr;
	  real l2Err =gridErrList[g].l2Err;
	  printF(" grid=%5i : max-value=%8.2e l2-value=%8.2e for %s on grid=%i (%s)\n", 
                 grid,
                 maxErr,l2Err,(const char*)du.getName(n),
		 grid,(const char*)gc[grid].getName());
	}
      }

      gi.erase();
      par.set(GI_TOP_LABEL,sPrintF("Rel-Errors in delta(u), orderOfAccuracy=%i",orderOfAccuracy));  // set title
      PlotIt::contour(gi,du,par);

      delete exactPointer;
    }
    else if( answer=="derivative errors" )
    {
      dialog.getOptionMenu("plot:").setCurrentChoice(answer);

      gc.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEinverseVertexDerivative);

      GridCollectionOperators gcop(gc);
      gcop.setOrderOfAccuracy(orderOfAccuracy);

      Range all;
      realGridCollectionFunction du(gc,all,all,all,7);
      
      du.setName("errors");
      du.setName("err(Delta u)",0);
      du.setName("err(u.x)",1);
      du.setName("err(u.y)",2);
      du.setName("err(u.xx)",3);
      du.setName("err(u.xy)",4);
      du.setName("err(u.yy)",5);
      du.setName("u",6);
      

      // assign twilight zone

      // create a twilight-zone function for checking the errors
      OGFunction *exactPointer;
      if( twilightZoneOption==trigonometric )
      {
	printf("TwilightZone: trigonometric polynomial, fx=%9.3e, fy=%9.3e, fz=%9.3e\n",fx,fy,fz);
	exactPointer = new OGTrigFunction(fx,fy,fz); 
      }
      else
      {
	printf("TwilightZone: algebraic polynomial, degreeInSpace=%i\n",degreeInSpace);

	int degreeOfSpacePolynomial = degreeInSpace; 
	int degreeOfTimePolynomial = 0;
	int numberOfComponents = gc.numberOfDimensions();
	exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,gc.numberOfDimensions(),numberOfComponents,
					  degreeOfTimePolynomial);
    
      
      }
      OGFunction & exact = *exactPointer;

      Index I1,I2,I3;
      for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = gc[grid];
	getIndex(mg.dimension(),I1,I2,I3);  

        realMappedGridFunction ug(mg);
        ug.setOperators(gcop[grid]);

        realArray fg(I1,I2,I3);
        realArray & dug = du[grid];
	

	ug(I1,I2,I3)=exact(mg,I1,I2,I3,0);

	// find out how many ghost points:
	int numGhost = min(mg.numberOfGhostPoints()(all,Range(0,mg.numberOfDimensions()-1)));
	numGhost-=orderOfAccuracy/2;

        numGhost=min(numGhost,numberOfGhostPointsToCheck);
	
	printf("--> grid=%i, evaluate derivatives at %i ghost points\n",numGhost);

	// compute derivatives at ghost points where possible
	getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost); // include ghost if possible

	const real eps=REAL_MIN*1000.;
	dug=0.;
	fg(I1,I2,I3)=exact.laplacian(mg,I1,I2,I3,0);
	dug(I1,I2,I3,0)=(ug.laplacian(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));


	fg(I1,I2,I3)=exact.x(mg,I1,I2,I3,0);
	dug(I1,I2,I3,1)=(ug.x(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));

	fg(I1,I2,I3)=exact.y(mg,I1,I2,I3,0);
	dug(I1,I2,I3,2)=(ug.y(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));

	fg(I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
	dug(I1,I2,I3,3)=(ug.xx(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));

	fg(I1,I2,I3)=exact.xy(mg,I1,I2,I3,0);
	dug(I1,I2,I3,4)=(ug.xy(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));

	fg(I1,I2,I3)=exact.yy(mg,I1,I2,I3,0);
	dug(I1,I2,I3,5)=(ug.yy(I1,I2,I3)(I1,I2,I3)-fg(I1,I2,I3))/max(eps,max(fabs(fg(I1,I2,I3))));

	dug(all,all,all,6)=ug(all,all,all);  // just plot u

	du[grid].periodicUpdate();
      }
      
      for( int n=du.getComponentBase(0); n<=du.getComponentBound(0); n++ )
      {
        RealArray eMax(gc.numberOfComponentGrids()), eL2(gc.numberOfComponentGrids());
	for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
	  eMax(grid)= maxNorm(du[grid], n );
	  eL2(grid) = l2Norm( du[grid], n );
	  printF(" max-value=%8.2e l2-value=%8.2e for %s on grid=%i (%s)\n", eMax(grid),eL2(grid),
                 (const char*)du.getName(n),
		 grid,(const char*)gc[grid].getName());
	}
      }
      

      gi.erase();
      par.set(GI_TOP_LABEL,sPrintF("Rel-Errors in derivatives, orderOfAccuracy=%i",orderOfAccuracy));  // set title
      PlotIt::contour(gi,du,par);

      delete exactPointer;
    }
    else if( answer=="polynomial" || answer=="trigonometric" )
    {
      twilightZoneOption= answer=="polynomial" ? polynomial : trigonometric;
      dialog.getOptionMenu("analytic solution:").setCurrentChoice(answer);
    }
    else if( answer=="erase" )
    {
      gi.erase();
    }
    else
    {
      cout << "Unknown response = " << answer << endl;
      gi.stopReadingCommandFile();
    }
    
  }

  gi.popGUI();  // pop dialog

  return;
  
}


