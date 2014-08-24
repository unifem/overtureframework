#include "ModelBuilder.h"
#include "nurbsCurveEditor.h"
#include "ArraySimple.h"
#include "RevolutionMapping.h"
#include "SphereMapping.h"
#include "MappingProjectionParameters.h"
#include "CompositeSurface.h"
#include "UnstructuredMapping.h"
#include "CompositeTopology.h"
#include "MappingGeometry.h"
#include "RandomSampling.h"
// #include "TrimmedMapping.h"

int
fillVolumeWithSpheres( CompositeSurface & model, GenericGraphicsInterface& gi, SphereLoading & sphereDist,
                       int & nsp, int & nsr, real & sphereProbability, real & volume, real & volumeFraction,
                       int & numberOfSpheres, RealArray & sr, RealArray & sp, int & RNGSeed, int debug );

int
fillVolumeWithUniformlySpacedSpheres(CompositeSurface & model, GenericGraphicsInterface& gi, PointList & points,
				     SphereLoading & sphereLoading, real & volume,
                                     int & numberOfSpheres, real & sphereRadius, int debug );


static int 
buildLinearLinerDialog(DialogData & dialog, realArray & xll )
// ==========================================================================================
// /Description:
//   Build the time stepping options dialog.
// ==========================================================================================
{
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "ll point 1";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xll(0,0),xll(0,1));  nt++; 
  textCommands[nt] = "ll point 2";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xll(1,0),xll(1,1));  nt++; 
  textCommands[nt] = "ll point 3";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xll(2,0),xll(2,1));  nt++; 
  textCommands[nt] = "ll point 4";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xll(3,0),xll(3,1));  nt++; 


  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}

static int 
buildQuadraticLinerDialog(DialogData & dialog, realArray & xql0, realArray & xql2 )
// ==========================================================================================
// /Description:
//   Build the time stepping options dialog.
// ==========================================================================================
{
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "ql point 1";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xql0(0,0),xql0(0,1));  nt++; 
  textCommands[nt] = "ql point 2";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xql0(1,0),xql0(1,1));  nt++; 
  textCommands[nt] = "ql point 3";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xql0(2,0),xql0(2,1));  nt++; 

  textCommands[nt] = "ql point 4";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xql2(0,0),xql2(0,1));  nt++; 
  textCommands[nt] = "ql point 5";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xql2(1,0),xql2(1,1));  nt++; 
  textCommands[nt] = "ql point 6";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%6.4f %6.4f",xql2(2,0),xql2(2,1));  nt++; 


  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}


static int 
buildFreeFormLinerDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the time stepping options dialog.
// ==========================================================================================
{
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  aString pushButtonCommands[] = {"specify free form liner",
				  ""};
  int numRows=2;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

//    int nt=0;

//    // null strings terminal list
//    assert( nt<numberOfTextStrings );
//    textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
//    dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}



static int
buildQuadraticLiner( NurbsMapping & quadraticLinerCurve,
                     NurbsMapping *& quadraticLinerSubCurves,
                     realArray & xql0, realArray & xql2 )
// =========================================================================================
// /Description:
//    Build the Quadratic liner
// =========================================================================================
{
  const int numberOfQuadraticLinerSubCurves=3;
  if( quadraticLinerSubCurves==NULL )
  {
    quadraticLinerSubCurves = new NurbsMapping [numberOfQuadraticLinerSubCurves];
  }
  
  const int quadraticDegree=2;

  quadraticLinerSubCurves[0].interpolate(xql0,0,Overture::nullRealDistributedArray(),quadraticDegree);
  quadraticLinerSubCurves[0].setGridDimensions(axis1,51);

  quadraticLinerSubCurves[2].interpolate(xql2,0,Overture::nullRealDistributedArray(),quadraticDegree);
  quadraticLinerSubCurves[2].setGridDimensions(axis1,51);

  // line 
  realArray xql1(2,2);
  
  xql1(0,0) = xql0(2,0);         xql1(0,1)=xql0(2,1);    
  xql1(1,0) = xql2(2,0);         xql1(1,1)=xql2(2,1); 

  quadraticLinerSubCurves[1].interpolate(xql1,0,Overture::nullRealDistributedArray(),quadraticDegree);
  quadraticLinerSubCurves[1].setGridDimensions(axis1,11);
  
  quadraticLinerCurve=quadraticLinerSubCurves[0];
  for( int m=1; m<numberOfQuadraticLinerSubCurves; m++ )
  {
    if ( quadraticLinerCurve.merge(quadraticLinerSubCurves[m])!=0 )
    {
      printf("ERROR:quadraticLiner : unable to merge subcurves!\n");
      break;
    }
  }

  return 0;
}


static int 
computeParticleVelocity(SphereLoading & sphereLoading, 
                        NurbsMapping & lightingCurve,
                        NurbsMapping & lightingTime,
                        NurbsMapping & lightingSpeed )
// ========================================================================================
// /Description:
//    Assign the velocity and start time to the particles.
// ========================================================================================
{

  real timea=getCPU();

  RealArray & spheres = sphereLoading.sphereCenter;

  const int numberOfSpheres = spheres.getLength(0);

  RealArray & velocity = sphereLoading.sphereVelocity; 
  RealArray & startTime = sphereLoading.sphereStartTime;
  velocity.redim(numberOfSpheres,3);
  startTime.redim(numberOfSpheres);

  
  realArray xp(numberOfSpheres,2), rp(numberOfSpheres,1), xr(numberOfSpheres,2,1);
  int i;
  for( i=0; i<numberOfSpheres; i++ )
  {
    real xv[3];
    xv[0] = spheres(i,0);  xv[1]=spheres(i,1); xv[2]=spheres(i,2);
    // Point on the body of revolution is of the form 
    //       (x,r*cos(theta),r*sin(theta))
    // 
    real theta; 
    if( fabs(xv[1])>0. || fabs(xv[2])>0. ) 
      theta = atan2((double)xv[2],(double)xv[1]);
    else
      theta=0.;

    real radius = sqrt(xv[1]*xv[1]+xv[2]*xv[2]);
    xp(i,0)=xv[0];
    xp(i,1)=radius;
  }
  rp=-1.;
  lightingCurve.inverseMap(xp,rp);   // find the closest point on the lightingCurve to the particle location
  for( i=0; i<numberOfSpheres; i++ )
  {
    if( fabs(rp(i,0)-.5)>5. )
    {
      printf("ERROR:inverting the lightingCurve! i=%i rp=%9.3e\n",i,rp(i,0));
      rp(i,0)=.5;
    }
  }
  
  lightingCurve.map(rp,xp,xr);

  // Fix this -- we should not use rp to evaluate this curve -- 
  //      need to find lightingSpeed.x(u)=rp and then use lightingSpeed.y(u)
  // To evaluate the lightingTime: C(s) =(x(s),y(s))  -> form the curve x(s) representing the x-component
  // We can then solve x(s) = rp to give the value of s at which we should compute y(s) which is the
  // time we want.
   NurbsMapping lightingTimeX, lightingSpeedX;
  lightingTime.buildComponentCurve( lightingTimeX,0 );
  lightingSpeed.buildComponentCurve( lightingSpeedX,0 );

  realArray up(numberOfSpheres,1);
  lightingSpeedX.inverseMap(rp,up);  // invert rp values to find up such that x(up)=rp 
  for( i=0; i<numberOfSpheres; i++ )
  {
    if( fabs(up(i,0)-.5)>5. )
    {
      printf("ERROR:inverting the lightingSpeed! i=%i rp=%9.3e up=%9.3e\n",i,rp(i,0),up(i,0));
      up(i,0)=.5;
    }
  }
  realArray xSpeed(numberOfSpheres,2);
  lightingSpeed.map(up,xSpeed);  // evaluate the speed for all particles.
    
  lightingTimeX.inverseMap(rp,up);
  for( i=0; i<numberOfSpheres; i++ )
  {
    if( fabs(up(i,0)-.5)>5. )
    {
      printf("ERROR:inverting the lightingTime! i=%i rp=%9.3e up=%9.3e\n",i,rp(i,0),up(i,0));
      up(i,0)=.5;
    }
  }

  realArray xTime(numberOfSpheres,2);
  lightingTime.map(up,xTime);    // evaluate the lighting time for all particles

  Range I=numberOfSpheres;
  #ifndef USE_PPP
  startTime(I)=xTime(I,1);
  #endif
    
  for( i=0; i<numberOfSpheres; i++ )
  {
    real xv[3];
    xv[0] = spheres(i,0);  xv[1]=spheres(i,1); xv[2]=spheres(i,2);
    // Point on the body of revolution is of the form 
    //       (x,r*cos(theta),r*sin(theta))
    // 
    real theta; 
    if( fabs(xv[1])>0. || fabs(xv[2])>0. ) 
      theta = atan2((double)xv[2],(double)xv[1]);
    else
      theta=0.;


    // Normal to the un-rotated point is (nv[0],nv[1],0)
    real nv[3];
    real xs=xr(i,0,0), ys=xr(i,1,0);
    real norm=max(REAL_MIN*1000.,sqrt(xs*xs+ys*ys));
    nv[0]= ys/norm;
    nv[1]=-xs/norm;
    nv[2]=0.;
       
    // Now rotate the normal about the x-axis
    real rv = nv[1];
    nv[1] = rv*cos(theta);
    nv[2] = rv*sin(theta);
      
    real speed=xSpeed(i,1);
    velocity(i,0)=nv[0]*speed;
    velocity(i,1)=nv[1]*speed;
    velocity(i,2)=nv[2]*speed;
       
  }


  timea=getCPU()-timea;
  printf("Time for computing sphere velocities = %8.2e \n",timea);

  return 0;
}


//\begin{>>ModelBuilderInclude.tex}{\subsection{linerGeometry}}
int ModelBuilder::
linerGeometry( CompositeSurface & model, GenericGraphicsInterface& gi, PointList & points,
               SphereLoading & sphereLoading )
//=====================================================================================
// /Purpose: 
//      Define different liner shapes.
//
// /model (output): holds the liner generated by this routine
// /gi (input): Holds a graphics interface to use.
// /points (input/output) : holds existing points on input, plus any new points on output
// /sphereLoading (input/output): holds a distribution of sphere sizes and volume fractions on input.
//            Holds the sphere centers and radii on output.
//\end{ModelBuilderInclude.tex}
//=====================================================================================
{
  int debug=0;

  const int maxPoints=100;
  static int selectedPoint[maxPoints];
  static int nSelectedPoints=0;
  static int arcPoints[2];
  static int nArcPoints=0;
  
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

  RealArray xBound(2,3);
  xBound=0.;
  xBound(1,nullRange)=1.;

  gi.setPlotTheBackgroundGrid(true);
  gi.setAxesDimension(2);
  gi.setPlotTheAxes(true);

  bool linerVolumeIsValid=false;  // set to true if the liner volume is built

  // here is the default linear liner
  realArray xll(4,2);
  xll(0,0) = .25;   xll(0,1)=0.;   // point 1 on axis
  xll(1,0) = .75;   xll(1,1)=1.0;  // point 2 
  xll(2,0) = .75;   xll(2,1)=.50;  // point 3 
  xll(3,0) = .50;   xll(3,1)=0.;   // point 4 on axis
  
  const int linearDegree=1;  // make piecewise linear
  NurbsMapping & linearLinerCurve = *new NurbsMapping(); linearLinerCurve.incrementReferenceCount();
  linearLinerCurve.interpolate(xll,0,Overture::nullRealDistributedArray(),linearDegree,
                               NurbsMapping::parameterizeByIndex);
  linearLinerCurve.setGridDimensions(0,81);
  
  // linearLinerCurve.useRobustInverse(true);
  
  // ************** here is the default quadratic liner ***********************
  // The quadratic liner consists of three sub-curves
  NurbsMapping & quadraticLinerCurve= *new NurbsMapping(); quadraticLinerCurve.incrementReferenceCount();
  NurbsMapping *quadraticLinerSubCurves = NULL; 
  realArray xql0(3,2), xql2(3,2);
  // circle: centre (.75,0) radius .5
  const real x0=.75, y0=0., r0=.5;
  xql0(0,0) = x0-r0;           xql0(0,1)=y0+0.;           // point 1 on axis
  xql0(1,0) = x0-r0/sqrt(2.);  xql0(1,1)=y0+r0/sqrt(2.);  // point 2 
  xql0(2,0) = x0;              xql0(2,1)=y0+r0;          // point 3 

  // circle: centre (.75,0) radius .25
  const real x2=.75, y2=0., r2=.25;
  xql2(0,0) = x2-r2;           xql2(0,1)=y2+0.;           // point 1 on axis
  xql2(1,0) = x2-r2/sqrt(2.);  xql2(1,1)=y2+r2/sqrt(2.);  // point 2 
  xql2(2,0) = x2;              xql2(2,1)=y2+r2;          // point 3 
  
  buildQuadraticLiner( quadraticLinerCurve, quadraticLinerSubCurves, xql0, xql2 );


  // ******************** free form ******************************
  NurbsMapping & freeFormLinerCurve = *new NurbsMapping;
  freeFormLinerCurve.incrementReferenceCount();
  freeFormLinerCurve.setRangeDimension(2);

  NurbsMapping *linerCurve=NULL;   // This points to the current liner curve
  linerCurve = &linearLinerCurve;  

  // quadraticLinerCurve.useRobustInverse(true);

  char buff[180];  // buffer for sprintf

  GUIState dialog;

  dialog.setWindowTitle("Liner Builder");
  dialog.setExitCommand("exit","Exit");


 // general state variables
  bool plotObject = true;
  bool plotAxes = false; 
  bool automaticallyComputeTopology=true;
  
  dialog.setOptionMenuColumns(1);

  enum LinerTypeEnum
  {
    linearLiner,
    quadraticLiner,
    freeFormLiner
  } linerType=linearLiner;
  
  aString linerTypeCommands[] = {"linear liner...", "quadratic liner...", "free form liner...", "" };
  dialog.addOptionMenu("method:", linerTypeCommands, linerTypeCommands, (int)linerType );


  enum SphereColourOptionEnum
  {
    colourSpheresBySpeed=0,
    colourSpheresByDistanceToCM,
    colourSpheresByRadius,
    colourSpheresByStartTime
  } sphereColourOption=colourSpheresByStartTime;
  
  aString sphereColourCommands[] = {"colour spheres by speed", "colour spheres by dist from xCM", 
                                    "colour spheres by radius", "colour spheres by start time", "" };
  dialog.addOptionMenu("spheres:", sphereColourCommands, sphereColourCommands, (int)sphereColourOption );

  aString pushButtonCommands[] = {"revolve around axis",
                                  "topology",
                                  "fill with spheres",
                                  "lighting curve",
                                  "lighting time",
                                  "lighting speed",
                                  "rotate spheres to z-axis",
				  ""};
  int numRows=3;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  aString tbCommands[] = {"plot curve",
                          "plot surface",
                          "plot shaded surface",
                          "plot spheres",
                          "plot spheres as points",
                          "plot lighting curve",
                          "plot velocity arrows",
                          "plot triangulation",
                          "fill with evenly spaced spheres",
 			  ""};
  bool plotCurve=true;
  bool plotSurface=true;
  bool plotShadedSurface=true;
  bool plotSpheres=true;
  bool plotSpheresAsPoints=false;
  bool plotLightingCurve=true;
  bool plotVelocityArrows=false;
  bool plotTriangulation=false;
  bool fillWithUniformlySpacedSpheres=false;
  
  int tbState[10];
  tbState[0] = plotCurve;
  tbState[1] = plotSurface;
  tbState[2] = plotShadedSurface;
  tbState[3] = plotSpheres;
  tbState[4] = plotSpheresAsPoints;
  tbState[5] = plotLightingCurve;
  tbState[6] = plotVelocityArrows;
  tbState[7] = plotTriangulation;
  tbState[8] = fillWithUniformlySpacedSpheres;
  

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  dialog.addInfoLabel("Volume = 0");

  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  aString name="liner";
  
  int nt=0;
  textCommands[nt] = "name";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%s", (const char*)name);  nt++; 

  // real sphereRadius=.1;
  // textCommands[nt] = "radius for spheres";  textLabels[nt]=textCommands[nt];
  // sPrintF(textStrings[nt], "%9.3e",sphereRadius);  nt++; 

  const int MAX_SPHEREDISTS = 25;
  real sphereRadius=.1;
  RealArray sr(MAX_SPHEREDISTS);
  int nsr = 1;
  sr(0) = 0.1;
  textCommands[nt] = "radius for spheres";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%9.3e",sr(0));  nt++; 

  real sphereProbability=1.;
  RealArray sp(MAX_SPHEREDISTS);
  int nsp = 1;
  sp(0) = 1.0;
  textCommands[nt] = "probability for spheres";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%9.3e",sp(0));  nt++; 

  real volumeFraction=.25;
  textCommands[nt] = "total volume fraction";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%9.3e",volumeFraction);  nt++; 

  int RNGSeed = 0;
  textCommands[nt] = "RNG seed";  
  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%d",RNGSeed);  nt++; 

  //textCommands[nt] = "project a point";  textLabels[nt]=textCommands[nt];
  //sPrintF(textStrings[nt], "%5.2f %5.2f %5.2f",1.,.75,0.);  nt++; 


  // null strings terminate list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);


  // --- Build the sibling dialog for linear liner options ---
  DialogData &linearLinerOptionsDialog = dialog.getDialogSibling();
  linearLinerOptionsDialog.setWindowTitle("Linear Liner Options");
  linearLinerOptionsDialog.setExitCommand("close linear liner", "close");
  buildLinearLinerDialog(linearLinerOptionsDialog,xll);

  // --- Build the sibling dialog for quadratic liner options ---
  DialogData &quadraticLinerOptionsDialog = dialog.getDialogSibling();
  quadraticLinerOptionsDialog.setWindowTitle("Quadratic Liner Options");
  quadraticLinerOptionsDialog.setExitCommand("close quadratic liner", "close");
  buildQuadraticLinerDialog(quadraticLinerOptionsDialog,xql0,xql2);

  // --- Build the sibling dialog for the free form liner options ---
  DialogData &freeFormLinerOptionsDialog = dialog.getDialogSibling();
  freeFormLinerOptionsDialog.setWindowTitle("Free Form Liner Options");
  freeFormLinerOptionsDialog.setExitCommand("close free form liner", "close");
  buildFreeFormLinerDialog(freeFormLinerOptionsDialog);


  SelectionInfo select;
  aString answer="", line;
  aString buf;
 

  gi.pushGUI(dialog);

  // reset the viewing matrix
  gi.initView(gi.getCurrentWindow());

  // RadioBox & rBox = dialog.getRadioBox(0);

  // bring up initial dialog for linear liner parameters
  linearLinerOptionsDialog.showSibling();

  RevolutionMapping **revolution=NULL;

  // lightingCurve: x=C(s)  (In the (x,y) plane, 0<= s <=1 )
  //      The normal to the lighting curve, n(s), defines the direction of the initial velocity
  //      This velocity is applied to all spheres that lie along the normal, n(s) 
  // lighting time: t=T(s)  (
  //      The time at which a sphere begins to move, applies to all spheres  that lie along the normal, n(s) to C(s)
  // lighting speed:  v=S(s)
  //      The initial speed of the spheres that lie along the normal, n(s) to C(s)

  // Thus: given a sphere with center (x,y,z) : first rotate the point to the (x,y) plane,
  //    and then find (s,u) such that (x0,y0) = C(s) + n(s)*u 
  
  NurbsMapping lightingCurve; lightingCurve.setName(Mapping::mappingName,"lighting curve");
  NurbsMapping lightingTime;  lightingTime.setName(Mapping::mappingName,"lighting time");
  NurbsMapping lightingSpeed; lightingSpeed.setName(Mapping::mappingName,"lighting speed");
  
  // lighting curve: Initially a straight line from (xa,ya) to (xb,yb)
  real xa=.5, ya=0.,  xb=.8, yb=1.;
  realArray xlc(2,2);
  xlc(0,0)=xa;  xlc(0,1)=ya;
  xlc(1,0)=xb;  xlc(1,1)=yb;
  lightingCurve.interpolate(xlc,0,Overture::nullRealDistributedArray(),linearDegree,NurbsMapping::parameterizeByIndex);

  // lighting time: Initially a straight line from (0,ya) to (1,yb)
  // *note* make the lighting time and lighting speed curves map R^1 -> R^2 so we can edit them more easily
  ya=0., yb=1.;
  realArray xlt(2,2);
  xlt(0,0)=0.;  xlt(0,0)=ya;
  xlt(1,0)=1.;  xlt(1,0)=yb;
  lightingTime.interpolate(xlt,0,Overture::nullRealDistributedArray(),linearDegree,NurbsMapping::parameterizeByIndex);
  
  // lighting speed: initially a straight line: 
  ya=1., yb=1.;
  realArray xls(2,2);
  xls(0,0)=0.; xls(0,1)=ya; 
  xls(1,0)=1.; xls(1,1)=yb; 
  lightingSpeed.interpolate(xls,0,Overture::nullRealDistributedArray(),linearDegree,NurbsMapping::parameterizeByIndex);

  int numberOfSpheres=0;
  RealArray & spheres = sphereLoading.sphereCenter;

  real volume=1.;   // The volume will be computed when the surface of revolution is built
  RealArray centerOfMass(3);  centerOfMass=0.;
  RealArray momentOfInertial(3,3); momentOfInertial=0;  

  bool spheresHaveBeenComputed=false;
  bool updateParticleVelocities=true;
     

  const int numberOfRevolutionSegments=2;

  for( int it=0;; it++ )
  {
     
   if( it==0 && plotObject )
     answer="plotObject";
   else
   {
     gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

     gi.getAnswer(answer, "", select);

     gi.savePickCommands(true); // restore
   }
   
   int len;
   if( answer=="linear liner..." )
   {
     linerType=linearLiner;
     linerCurve = &linearLinerCurve;  

     linearLinerOptionsDialog.showSibling();
   }
   else if( answer=="close linear liner" )
   {
     linearLinerOptionsDialog.hideSibling();
   }
   else if( (len=answer.matches("quadratic liner...")) )
   {
     linerType=quadraticLiner;
     linerCurve = &quadraticLinerCurve;  
     quadraticLinerOptionsDialog.showSibling();
   }
   else if( answer=="close quadratic liner" )
   {
     quadraticLinerOptionsDialog.hideSibling();
   }
   else if( (len=answer.matches("free form liner...")) )
   {
     linerType=freeFormLiner;
     linerCurve = &freeFormLinerCurve; 
     freeFormLinerOptionsDialog.showSibling();
   }
   else if( (len=answer.matches("close free form liner")) )
   {
     freeFormLinerOptionsDialog.hideSibling();
   }
   else if (answer.matches("specify free form liner"))
   {
     
     nurbsCurveEditor(freeFormLinerCurve, gi, points);
     freeFormLinerCurve.setGridDimensions(0,81);
     linerVolumeIsValid=false;

//       // add it to a special list only containing curves
//       if (curve_->isInitialized())
//       {
//         MappingRC curveRC(*curve_);
//         curveList.addElement(curveRC);
//       }
//       else
//         printf("Throwing away uninitialized curve...\n");
//       if (curve_->decrementReferenceCount() == 0)
//         delete curve_;
   }
   else if( answer=="lighting curve" )
   {
     PointList points; 
     nurbsCurveEditor(lightingCurve, gi, points);

     updateParticleVelocities=true;  // we need to recompute the particle velocities
     //  MappingInformation mapInfo;
     // mapInfo.graphXInterface= &gi;
     // lightingCurve.update(mapInfo);
   }
   else if( answer=="lighting time" )
   {
     // --- the lighting time is really a map from R^1 -> R^1 but we treat is as a map R^1 -> R^2
     PointList points; 
     nurbsCurveEditor(lightingTime, gi, points);

     updateParticleVelocities=true;  // we need to recompute the particle velocities

     // MappingInformation mapInfo;
     // mapInfo.graphXInterface= &gi;
     // lightingTime.update(mapInfo);
   }
   else if( answer=="lighting speed" )
   {
     // --- the lighting speed is really a map from R^1 -> R^1 but we treat is as a map R^1 -> R^2
     PointList points; 
     nurbsCurveEditor(lightingSpeed, gi, points);

     updateParticleVelocities=true; // we need to recompute the particle velocities

     // MappingInformation mapInfo;
     // mapInfo.graphXInterface= &gi;
     // lightingSpeed.update(mapInfo);
   }
//     else if( (len=answer.matches("radius for spheres")) )
//     {
//        sScanF(answer(len,answer.length()-1),"%e",&sphereRadius);
//        dialog.setTextLabel(answer(0,len-1),sPrintF(line, "%8.2e",sphereRadius));
//     }
   else if ( (len=answer.matches("name")) )
   {
       name = answer(len, answer.length()-1);
   }
   else if( (len=answer.matches("radius for spheres")) )
   {
     nsr=sScanF(answer(len, answer.length()-1),
                "%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e",
                &sr(0),   &sr(1),  &sr(2),  &sr(3),  &sr(4),  &sr(5),  &sr(6),  &sr(7), &sr(8),
                &sr(9),  &sr(10), &sr(11), &sr(12), &sr(13), &sr(14), &sr(15),  &sr(16), 
                &sr(17), &sr(18), &sr(19), &sr(20), &sr(21), &sr(22), &sr(23), &sr(24));

     dialog.setTextLabel(answer(0,len-1),answer(len,answer.length()));
   }
   else if( (len=answer.matches("probability for spheres")) )
   {
     nsp=sScanF(answer(len, answer.length()-1),
                "%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e",
                &sp(0),   &sp(1),  &sp(2),  &sp(3),  &sp(4),  &sp(5),  &sp(6),  &sp(7), &sp(8),
                &sp(9),  &sp(10), &sp(11), &sp(12), &sp(13), &sp(14), &sp(15),  &sp(16), 
                &sp(17), &sp(18), &sp(19), &sp(20), &sp(21), &sp(22), &sp(23), &sp(24));

     dialog.setTextLabel(answer(0,len-1),answer(len,answer.length()));
   }
   else if( (len=answer.matches("total volume fraction")) )
   {
      sScanF(answer(len,answer.length()-1),"%e",&volumeFraction);
      dialog.setTextLabel(answer(0,len-1),sPrintF(line, "%8.2e",volumeFraction));
   }
   else if( (len=answer.matches("RNG seed")) )
   {
      sScanF(answer(len,answer.length()-1),"%d",&RNGSeed);
      dialog.setTextLabel(answer(0,len-1),sPrintF(line, "%d",RNGSeed));
   }
   else if( (len=answer.matches("ll point ")) )
   {
     // change a point of the linear liner

     len++;
     int pointNumber = (answer.matches("ll point 1") ? 1 :
			answer.matches("ll point 2") ? 2 :
			answer.matches("ll point 3") ? 3 : 4);

     real xpt=xll(pointNumber-1,0), ypt=xll(pointNumber-1,1);
     sScanF(answer(len,answer.length()-1),"%e %e",&xpt,&ypt);
     xll(pointNumber-1,0)=xpt;
     xll(pointNumber-1,1)=ypt;
     linearLinerCurve.interpolate(xll,0,Overture::nullRealDistributedArray(),linearDegree,
                                  NurbsMapping::parameterizeByIndex);
     linearLinerCurve.setGridDimensions(0,81);
     
     linearLinerOptionsDialog.setTextLabel(answer(0,len-1),sPrintF(line, "%6.4f %6.4f",xpt,ypt));

     linerVolumeIsValid=false;
     
   }
   else if( (len=answer.matches("ql point ")) )
   {
     // change a point of the quadratic liner

     len++;
     int pointNumber = (answer.matches("ql point 1") ? 1 :
			answer.matches("ql point 2") ? 2 :
			answer.matches("ql point 3") ? 3 :
			answer.matches("ql point 4") ? 4 :
			answer.matches("ql point 5") ? 5 : 6);

     real xpt=0., ypt=0.;
     if( pointNumber<=3 )
     {
       pointNumber-=1;
       
       xpt=xql0(pointNumber,0), ypt=xql0(pointNumber,1);

       sScanF(answer(len,answer.length()-1),"%e %e",&xpt,&ypt);
       xql0(pointNumber,0)=xpt;
       xql0(pointNumber,1)=ypt;
     }
     else 
     {
       pointNumber-=4;
       xpt=xql2(pointNumber,0), ypt=xql2(pointNumber,1);

       sScanF(answer(len,answer.length()-1),"%e %e",&xpt,&ypt);
       xql2(pointNumber,0)=xpt;
       xql2(pointNumber,1)=ypt;
     }
     
     buildQuadraticLiner( quadraticLinerCurve, quadraticLinerSubCurves, xql0, xql2 );

     quadraticLinerOptionsDialog.setTextLabel(answer(0,len-1),sPrintF(line, "%6.4f %6.4f",xpt,ypt));

     linerVolumeIsValid=false;

   }
   else if( (len=answer.matches("revolve around axis")) ||
            (len=answer.matches("revolve around x-axis")) )
   {
     RealArray origin(3), tangent(3);
     origin = 0; tangent=0;
     tangent(0) = 1;

     if( revolution==NULL )
     {
       revolution = new RevolutionMapping* [numberOfRevolutionSegments];
       for( int i=0; i<numberOfRevolutionSegments; i++ )
       {
	 revolution[i] = new RevolutionMapping;
         revolution[i]->incrementReferenceCount();
       }
     }
     for( int i=0; i<numberOfRevolutionSegments; i++ )
     {
       assert( linerCurve!=NULL );
       revolution[i]->setRevolutionary(*linerCurve);

       //  int nr=161, ns=161;
       // revolution[i]->setGridDimensions(0,nr);
       // revolution[i]->setGridDimensions(1,ns);

       revolution[i]->setLineOfRevolution(origin, tangent);

       revolution[i]->setRevolutionAngle(real(i)/numberOfRevolutionSegments,real(i+1)/numberOfRevolutionSegments);

       model.add(*revolution[i]);
       for( int map=0; map<model.numberOfSubSurfaces(); map++ )
	 model.setColour(map,gi.getColourName(map));

     }
     linerVolumeIsValid=true;
     
     // get bounds on the model
     real boundingBoxp[6];
     #define boundingBox(side,axis) boundingBoxp[(side)+2*(axis)]
     real xScale=0.;
     int axis;
     for( axis=0; axis<model.getRangeDimension(); axis++ )
     {
       for( int side=0; side<=1; side++ )
	 boundingBox(side,axis)=(real)model.getRangeBound(side,axis);

       xScale=max(xScale,boundingBox(1,axis)-boundingBox(0,axis));
     }

     if( automaticallyComputeTopology )
     {
       const bool allocateTopology=true;
       CompositeTopology & topo = *model.getCompositeTopology(allocateTopology); 


       real ds=.02*xScale;       // length of edges on curves *** fix this ***
       printf(" *** build topology: xScale=%8.2e, arclength for triangulation: ds=%8.2e **** \n",xScale,ds);

       real maxArea=.5*ds*ds;   // area of a typlical triangle
       topo.setDeltaS(ds);
       topo.setMaximumArea(maxArea);
       if( model.computeTopology(gi) )
       {
         printf("The topology (global triangulation) of the model was successfully determined.\n");

	 UnstructuredMapping *uns = model.getCompositeTopology()->getTriangulation();
	 assert( uns!=NULL );
	 RealArray rvalues;
	 IntegerArray ivalues;
	 MappingGeometry::getGeometricProperties( *uns, rvalues, ivalues );

	 volume=rvalues(0); 
         if( volume>.01 && volume<100. )
           dialog.setInfoLabel(0,sPrintF(line,"Volume = %7.4f",volume));
         else
           dialog.setInfoLabel(0,sPrintF(line,"Volume = %8.2e",volume));

	 centerOfMass(0)      = rvalues( 4);
	 centerOfMass(1)      = rvalues( 5);
	 centerOfMass(2)      = rvalues( 6);
	 momentOfInertial(0,0)= rvalues( 7);
	 momentOfInertial(0,1)= rvalues( 8);
	 momentOfInertial(0,2)= rvalues( 9);
	 momentOfInertial(1,0)= rvalues(10);
	 momentOfInertial(1,1)= rvalues(11);
	 momentOfInertial(1,2)= rvalues(12);
	 momentOfInertial(2,0)= rvalues(13);
	 momentOfInertial(2,1)= rvalues(14);
	 momentOfInertial(2,2)= rvalues(15);

         printf("\n"
                " --------------- liner properties ----------------------\n"
                "        volume = %10.3e \n"
                "        center of mass = [%10.3e,%10.3e,%10.3e]\n"
                "                                            \n"
                "                         [%10.3e,%10.3e,%10.3e]\n"
                "    moment of inertial = [%10.3e,%10.3e,%10.3e]\n"
                "                         [%10.3e,%10.3e,%10.3e]\n"
                " -------------------------------------------------------\n",
                volume,
                centerOfMass(0),centerOfMass(1),centerOfMass(2),
                momentOfInertial(0,0),momentOfInertial(0,1),momentOfInertial(0,2),
                momentOfInertial(1,0),momentOfInertial(1,1),momentOfInertial(1,2),
                momentOfInertial(2,0),momentOfInertial(2,1),momentOfInertial(2,2));
	 
       }
       else
       {
         printf("ERROR: There was a problem generating the topology (global triangulation) of the model.\n");
       }
 
     }
     
     // lighting curve: reset for new base and bound 
     real alpha=.25, beta=.8;
     real xa=boundingBox(0,0)*(1.-alpha)+boundingBox(1,0)*alpha, 
          ya=0.,
          xb=boundingBox(0,0)*(1.-beta)+boundingBox(1,0)*beta,
          yb=boundingBox(1,1);
     realArray xlc(2,2);
     xlc(0,0)=xa;  xlc(0,1)=ya;
     xlc(1,0)=xb;  xlc(1,1)=yb;
     lightingCurve.interpolate(xlc,0,Overture::nullRealDistributedArray(),linearDegree,NurbsMapping::parameterizeByIndex);

     #undef boundingBox
   }
   else if (model.numberOfSubSurfaces()>0 && answer == "topology")
   {
     if( !linerVolumeIsValid ) 
     {
       printf("The liner volume must be built before the topology can be built.\n");
       continue;
     }

     model.updateTopology();
     if( model.isTopologyDetermined() )
     {
       UnstructuredMapping *uns = model.getCompositeTopology()->getTriangulation();
       assert( uns!=NULL );
       RealArray rvalues;
       IntegerArray ivalues;
       MappingGeometry::getGeometricProperties( *uns, rvalues, ivalues );

       volume=rvalues(0); 
       dialog.setInfoLabel(0,sPrintF(line,"Volume = %8.2e",volume));
     }
     
     gi.erase(); 
   }
   else if( answer=="colour spheres by speed"  || answer=="colour spheres by dist from xCM" || 
            answer=="colour spheres by radius" || answer=="colour spheres by start time" )
   {
     sphereColourOption = (answer=="colour spheres by speed" ? colourSpheresBySpeed :
			   answer=="colour spheres by dist from xCM" ? colourSpheresByDistanceToCM :
                           answer=="colour spheres by radius" ? colourSpheresByRadius :
                           colourSpheresByStartTime );

     // printf(" sphereColourOption=%i\n",sphereColourOption);
     
     dialog.getOptionMenu("spheres:").setCurrentChoice((int)sphereColourOption);
   }
   else if( answer=="fill with spheres" )
   {
     if( !linerVolumeIsValid ) // revolution==NULL )
     {
       printf("The liner volume must be built before spheres can be made\n");
       continue;
     }
     if( !model.isTopologyDetermined() )
     {
       printf("The topology must be determined before spheres can be made.\n");
       continue;
     }
     
     if( !fillWithUniformlySpacedSpheres )
     {
       status=fillVolumeWithSpheres(model,gi,sphereLoading,
				    nsp,nsr,sphereProbability,volume,volumeFraction,
				    numberOfSpheres, sr,sp,RNGSeed,debug);
     }
     else
     {
       status=fillVolumeWithUniformlySpacedSpheres(model,gi,points,sphereLoading,volume,numberOfSpheres,sr(0),
                                                   debug );
     }
     if( status!=0 )
     {
       printf("ERROR filling the volume with spheres\n");
       break;
     }


     spheresHaveBeenComputed=true;
     updateParticleVelocities=true;   // update particle velocities below
     
     
   }
   else if( (len=answer.matches("rotate spheres to z-axis")) )
   {
     // rotate results to align along the z-axis

    /*
      we need to rotate axis since the assumption is:
      Cone opens right to left with face at z=0 and axis of rotation along z.
    */

     Range I = spheres.dimension(0);
     RealArray temp;

     temp=spheres(I,0);
     spheres(I,0)=spheres(I,1);
     spheres(I,1)=spheres(I,2);
     spheres(I,2)=temp;
     
     RealArray & velocity = sphereLoading.sphereVelocity; 
     temp=velocity(I,0);
     velocity(I,0)=velocity(I,1);
     velocity(I,1)=velocity(I,2);
     velocity(I,2)=temp;
     
   }
   else if( (len=answer.matches("project a point")) )
   {
     // Mapping::debug=15;

     realArray x(1,3),xP(1,3); // arrays to hold initial and projected points
     x=0.;

     sScanF(answer(len,answer.length()-1),"%e %e %e ",&x(0,0),&x(0,1),&x(0,2));
      
     // This next object is used to pass and return additional parameters to the "project" function.
     MappingProjectionParameters mpParams;
     typedef MappingProjectionParameters MPP;
     realArray & surfaceNormal  = mpParams.getRealArray(MPP::normal);
     intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
     realArray & xOld           = mpParams.getRealArray(MPP::x);  // this could be used as an initial guess

//       realArray & r  = mpParams.getRealArray(MPP::r);
//       r.redim(1,3);
//       r=-1.;

     // Allocate space for the normal to indicate that it should be computed
     int numberOfPointsToProject=1;  // we only project 1 point at a time here
     surfaceNormal.redim(numberOfPointsToProject,3);
     surfaceNormal=0.;       
//     subSurfaceIndex.redim(numberOfPointsToProject);
//     subSurfaceIndex=0;
     

     xP=x;
     model.project( xP,mpParams );  // project xP to the closet point on the surface

     // compute the distance to the surface 
     real dist = sqrt( SQR(xP(0,0)-x(0,0))+SQR(xP(0,1)-x(0,1))+SQR(xP(0,2)-x(0,2)) );
     // the dot-product will indicate whether we are inside or outside
     real dot = (xP(0,0)-x(0,0))*surfaceNormal(0,0) +
       (xP(0,1)-x(0,1))*surfaceNormal(0,1) +
       (xP(0,2)-x(0,2))*surfaceNormal(0,2);
     dot = dot/max(REAL_MIN*100.,dist);
       
     IntegerArray inside(1); inside=-1;
     model.insideOrOutside(x,inside);

     printf("\n >>> point=(%8.2e,%8.2e,%8.2e), projected point=(%8.2e,%8.2e,%8.2e), inside=%i \n"
	    "       surfaceNormal=(%8.2e,%8.2e,%8.2e), dist=%8.2e, dot=%8.2e subSurface=%i \n",
	    x(0,0),x(0,1),x(0,2),xP(0,0),xP(0,1),xP(0,2),inside(0),
	    surfaceNormal(0,0),surfaceNormal(0,1),surfaceNormal(0,2),
	    dist,dot, subSurfaceIndex(0));

   }
   else if( dialog.getToggleValue(answer,"plot curve",plotCurve) ){}//
   else if( dialog.getToggleValue(answer,"plot surface",plotSurface) ){}//
   else if( dialog.getToggleValue(answer,"plot shaded surface",plotShadedSurface) ){}//
   else if( dialog.getToggleValue(answer,"plot spheres as points",plotSpheresAsPoints) ){} // check this before one below
   else if( dialog.getToggleValue(answer,"plot spheres",plotSpheres) ){}//
   else if( dialog.getToggleValue(answer,"plot lighting curve",plotLightingCurve) ){}//
   else if( dialog.getToggleValue(answer,"plot triangulation",plotTriangulation) ){}//
   else if( dialog.getToggleValue(answer,"plot velocity arrows",plotVelocityArrows) ){}//
   else if( dialog.getToggleValue(answer,"fill with evenly spaced spheres",fillWithUniformlySpacedSpheres) ){}//
   else if ( answer=="exit" )
   {
     sphereLoading.printSpheres(name);
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

   if( spheresHaveBeenComputed && updateParticleVelocities )
   {
     // Assign the sphere velocity and start time
     computeParticleVelocity(sphereLoading,lightingCurve, lightingTime,lightingSpeed);
     updateParticleVelocities=false;
   }
   

   if ( plotObject )
   {
     gi.erase();
     gi.setGlobalBound(xBound);


     if( linerVolumeIsValid )
     {
       assert( revolution!=NULL );

       if( plotSurface )
       {
         parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotShadedSurface);
	 PlotIt::plot(gi, model, parameters );
         parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,true); 
       }
       
       if( plotSpheres && numberOfSpheres>0 )
       {
	 parameters.set(GI_POINT_SIZE, 6);
	 parameters.set(GI_POINT_COLOUR, "red");

         if( !plotSpheresAsPoints )
	 {
           // plot real spheres with a colour and radius
	   realArray value(numberOfSpheres,2);
           real val;
	   for( int i=0; i<numberOfSpheres; i++ )
	   {
             if( sphereColourOption==colourSpheresByDistanceToCM )
	     {
	       // -- base the colour on the distance from the center of mass  ---
	       val = (SQR(sphereLoading.sphereCenter(i,0)-centerOfMass(0))+
		      SQR(sphereLoading.sphereCenter(i,1)-centerOfMass(1))+
		      SQR(sphereLoading.sphereCenter(i,2)-centerOfMass(2)) );
	     
	     }
	     else if( sphereColourOption==colourSpheresBySpeed )
	     {
               // base colour on the intial speed
               val=sqrt( SQR(sphereLoading.sphereVelocity(i,0))+
                         SQR(sphereLoading.sphereVelocity(i,1))+
                         SQR(sphereLoading.sphereVelocity(i,2)) );
	     }
             else if( sphereColourOption==colourSpheresByRadius )
	     {
               val = sphereLoading.sphereRadius(i);
	     }
	     else
	     {
               val = sphereLoading.sphereStartTime(i);
	     }
	     
	     value(i,0)=val;       // defines a normalized colour from the colour table
	     // value(i,0)=i/(numberOfSpheres-1.);   // defines a normalized colour from the colour table
	     value(i,1)=sphereLoading.sphereRadius(i); 
	   }
           #ifndef USE_PPP
   	   gi.plotPoints(spheres,value,parameters);
           #endif
	 }
	 else
	 {
           #ifndef USE_PPP
   	   gi.plotPoints(spheres,parameters);
           #endif
	 }
	 
       }
       if( plotTriangulation && model.isTopologyDetermined() )
       {
	 UnstructuredMapping *uns = model.getCompositeTopology()->getTriangulation();
	 PlotIt::plot(gi, *uns, parameters );
       }

     }
     if( plotCurve )
     {
       parameters.set(GI_MAPPING_COLOUR, "blue");
       PlotIt::plot(gi, *linerCurve, parameters );

//  	 PlotIt::plot(gi, quadraticLinerSubCurves[0], parameters );
//  	 PlotIt::plot(gi, quadraticLinerSubCurves[1], parameters );
//  	 PlotIt::plot(gi, quadraticLinerSubCurves[2], parameters );

     }
     if( plotLightingCurve )
     {
       parameters.set(GI_MAPPING_COLOUR, "green");

       real oldCurveLineWidth;
       parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
       parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth*2.);

       PlotIt::plot(gi, lightingCurve, parameters );

       parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth); // reset
     }
     if( plotVelocityArrows && sphereLoading.sphereCenter.getLength(0)>0 )
     {
       RealArray & sphereCenter = sphereLoading.sphereCenter;
       RealArray & sphereVelocity=sphereLoading.sphereVelocity; 
       RealArray & sphereRadius=sphereLoading.sphereRadius; 
       const int numberOfSpheres=sphereCenter.getLength(0);


       Range all;
       const real maxSphereRadius=fillWithUniformlySpacedSpheres ? sphereRadius(0) :
                                  max(sphereLoading.sphereDistribution(all,0));

       realArray arrows(numberOfSpheres,3,2);
       for( int i=0; i<numberOfSpheres; i++ )
       {
	 // start point of arrow
	 arrows(i,0,0)=sphereCenter(i,0);
	 arrows(i,1,0)=sphereCenter(i,1);
	 arrows(i,2,0)=sphereCenter(i,2);
	 // end point
         // real arrowScale=sphereRadius(i)*1.5;  // length 
         real arrowScale=maxSphereRadius*2.; // 1.75;  // length 
	 arrows(i,0,1)=arrows(i,0,0)+sphereVelocity(i,0)*arrowScale;
	 arrows(i,1,1)=arrows(i,1,0)+sphereVelocity(i,1)*arrowScale;
	 arrows(i,2,1)=arrows(i,2,0)+sphereVelocity(i,2)*arrowScale;
       }

       real oldCurveLineWidth;
       parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
       parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth*2.);

       gi.plotLines(arrows,parameters);

       parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth); // reset
     }
     
   } // end if plotObject...
   
  }
  

  if( linearLinerCurve.decrementReferenceCount()==0 ) delete &linearLinerCurve;
  if( quadraticLinerCurve.decrementReferenceCount()==0 ) delete &quadraticLinerCurve;
  if( freeFormLinerCurve.decrementReferenceCount()==0 ) delete &freeFormLinerCurve;

  //   delete revolution;
  for( int i=0; i<numberOfRevolutionSegments; i++ )
  {
    if( revolution[i]->decrementReferenceCount()==0 ) delete revolution[i];
  }
  delete [] revolution;

// erase the curves
 gi.erase();
 gi.popGUI();

// reset plotTheAxes
 gi.setPlotTheAxes(oldPlotTheAxes);

 return status;
}

