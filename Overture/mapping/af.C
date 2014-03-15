#include "AirfoilMapping.h"
#include "MappingInformation.h"

//\begin{>AirfoilMappingInclude.tex}{\subsubsection{Constructor}}
AirfoilMapping::
AirfoilMapping(const AirfoilTypes & airfoilType_, 
	       const real xa /* = -1.5 */ , 
	       const real xb /* = 1.5 */ , 
	       const real ya /* = 0. */ , 
	       const real yb /* = 2. */  ) 
  : Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
// /Description:
//    Create a mapping for an airfoil, one of the following (enum AirfoilTypes)
//  \begin{description}
//   \item[arc] : grid with a bump on the bottom that is an arc of a circle.
//   \item[sinusoid] : grid with a bump on the bottom that is an sinusoid.
//   \item[diamond] : grid with a bump on the bottom that is a diamond.
//   \item[naca] : a curve that is one of the NACA 4 digit airfoils.
//  \end{description}
// /airfoilType\_ (input): an airfoil type from the above choices.
// /xa,xb,ya,yb (input) : boundaries of the bounding box (not used for naca airfoils).
//\end{AirfoilMappingInclude.tex}
//===========================================================================
{ 
  className="AirfoilMapping";
  setName( Mapping::mappingName,"airfoil");
  setGridDimensions( axis1,51 );
  setGridDimensions( axis2,11 );
  airfoilType=airfoilType_;   
// * airfoilType=naca;   // *****************************
  xBound.redim(2,3);
  xBound=0.;
  xBound(Start,axis1)=xa;
  xBound(End  ,axis1)=xb;
  xBound(Start,axis2)=ya;
  xBound(End,  axis2)=yb;
  chord=1.;
  mappingHasChanged(); 

  for( int axis=axis1; axis<=axis2; axis++ )
    for(int side=Start; side<=End; side++ )
      setRangeBound(side,axis,xBound(side,axis));


 // parameters for NACA 4 airfoil   NACAabcc
 // set values for a NACA0010
  maximumCamber=0.;                // first digit
  positionOfMaximumCamber=-.01;    // second digit  (negative -> not used)
  if( airfoilType!=naca )
    thicknessToChordRatio=.10;      
  else
  { // naca airfoil is just a curve:
    domainDimension=1;
    thicknessToChordRatio=.12;       // last two digits
    setIsPeriodic(axis1,Mapping::functionPeriodic);
    setBoundaryCondition(Start,axis1,-1);
    setBoundaryCondition(End,  axis1,-1);
  }
  
  trailingEdgeEpsilon=.02;  // trailingEdge is rounded off
}

// Copy constructor is deep by default
AirfoilMapping::
AirfoilMapping( const AirfoilMapping & map, const CopyType copyType )
{
  className="AirfoilMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "AirfoilMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

AirfoilMapping::
~AirfoilMapping()
{ if( debug & 4 )
  cout << " AirfoilMapping::Desctructor called" << endl;
}

AirfoilMapping & AirfoilMapping::
operator=( const AirfoilMapping & X )
{
  if( className != X.getClassName() )
  {
    cout << "AirfoilMapping::operator= ERROR trying to set a AirfoilMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  xBound       =X.xBound;
  airfoilType  =X.airfoilType;
  chord        =X.chord;
  thicknessToChordRatio=X.thicknessToChordRatio;
  maximumCamber=X.maximumCamber;
  positionOfMaximumCamber=X.positionOfMaximumCamber;
  trailingEdgeEpsilon=X.trailingEdgeEpsilon;
  
  return *this;
}


  // set bounds on the rectangle that the airfoil sits in
//\begin{>>AirfoilMappingInclude.tex}{\subsubsection{setBoxBounds}}
int AirfoilMapping::
setBoxBounds(const real xa /* =-1.5 */, 
	     const real xb /* =1.5 */, 
	     const real ya /* =0. */, 
	     const real yb /* =2. */ )
//===========================================================================
// /Description:
// set bounds on the rectangle that the airfoil sits in
// /xa,xb,ya,yb (input) : boundaries of the bounding box (not used for naca airfoils).
//\end{AirfoilMappingInclude.tex}
//===========================================================================
{
  xBound(Start,axis1)=xa;
  xBound(End  ,axis1)=xb;
  xBound(Start,axis2)=ya;
  xBound(End,  axis2)=yb;
  return 0;
}

 
//\begin{>>AirfoilMappingInclude.tex}{\subsubsection{setParameters}}
int AirfoilMapping:: 
setParameters(const AirfoilTypes & airfoilType_,
              const real & chord_ /* =1. */, 
	      const real & thicknessToChordRatio_ /* =.1 */,
	      const real & maximumCamber_ /* =0. */,
	      const real & positionOfMaximumCamber_ /* =0. */,
	      const real & trailingEdgeEpsilon_  /* =.02 */)
//===========================================================================
// /Description:
//    Create a mapping for an airfoil, one of the following (enum AirfoilTypes)
//  \begin{description}
//   \item[arc] : grid with a bump on the bottom that is an arc of a circle.
//   \item[sinusoid] : grid with a bump on the bottom that is an sinusoid.
//   \item[diamond] : grid with a bump on the bottom that is a diamond.
//   \item[naca] : a curve that is one of the NACA 4 digit airfoils.
//  \end{description}
// /airfoilType\_ (input): an airfoil type from the above choices.
// /chord\_ (input): length of the chord.
// /thicknessToChordRatio\_ (input): thickness to chord ratio.
// /maximumCamber\_ (input): maximum camber
// /positionOfMaximumCamber\_ (input): position of maximum camber
// /trailingEdgeEpsilon\_ (input) : parameter for rounding the trailing edge.
//\end{AirfoilMappingInclude.tex}
//===========================================================================
{
  airfoilType=airfoilType_;
  chord=chord_;
  thicknessToChordRatio=thicknessToChordRatio_;
  maximumCamber=maximumCamber_;
  positionOfMaximumCamber=positionOfMaximumCamber_;
  trailingEdgeEpsilon=trailingEdgeEpsilon_;
  return 0;
}





void AirfoilMapping::
map( const RealArray & r, RealArray & x, RealArray & xr,
                         MappingParameters & params )
{
  if( params.coordinateType != cartesian )
    cerr << "AirfoilMapping::map - coordinateType != cartesian " << endl;

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  real height = thicknessToChordRatio*chord;
  real theta0=2.*atan(2.*thicknessToChordRatio);
  real s0=sin(theta0);  
  real c0=cos(theta0);  
  real rad=chord/(2.*s0);
  real xa=xBound(0,0), xb=xBound(1,0), ya=xBound(0,1), yb=xBound(1,1); 

  real ra=(-.5*chord -xa)/(xb-xa),        // r value at start leading edge
       rb=1.-(xb-.5*chord)/(xb-xa);       // r value at trailing edge

  RealArray t(I);
  real scaleFactor=2.*theta0/(rb-ra);
  const real & x1=positionOfMaximumCamber;

  real t1;  // for rounding the trailing edge

  switch( airfoilType )
  {
  case arc:
    t=(r(I,axis1)-ra)*scaleFactor;   // t in [0,2*theta0]
    if( computeMap )
    {
      where( r(I,axis1)<= ra ||  r(I,axis1)>= rb )
      {
        x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
        x(I,axis2)=(yb-ya)*r(I,axis2)+ya; 
      }
      elsewhere( )
      {
        x(I,axis1)=(-rad*sin(theta0-t))*(1.-r(I,axis2))
                   +(-.5*chord+t*(chord/(2.*theta0)))*r(I,axis2);
        x(I,axis2)=(rad*(cos(theta0-t)-c0) )*(1.-r(I,axis2))
                   +r(I,axis2)*yb;
      }
    }
    if( computeMapDerivative )
    {
      where( r(I,axis1)<= ra ||  r(I,axis1)>= rb )
      {
	xr(I,axis1,axis1)=xb-xa;
	xr(I,axis1,axis2)=0.;
	xr(I,axis2,axis1)=0.;
	xr(I,axis2,axis2)=yb-ya;
      }
      elsewhere( )
      {
	xr(I,axis1,axis1)=((+rad*cos(theta0-t))*(1.-r(I,axis2))
			   +(chord/(2.*theta0))*r(I,axis2) )*scaleFactor;
	xr(I,axis1,axis2)=rad*sin(theta0-t) +(-.5*chord+t*(chord/(2.*theta0)));
	xr(I,axis2,axis1)=(+rad*(sin(theta0-t)) )*(1.-r(I,axis2)) *scaleFactor;
	xr(I,axis2,axis2)=-(rad*(cos(theta0-t)-c0) )+yb;
      }
    }
    break;

  case sinusoid:

    scaleFactor=twoPi/(rb-ra);
    t=(r(I,axis1)-ra)*scaleFactor;   // t in [0,2*theta0]
    if( computeMap )
    {
      x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
      where( r(I,axis1)<= ra ||  r(I,axis1)>= rb )
        x(I,axis2)=(yb-ya)*r(I,axis2)+ya; 
      elsewhere( )
        x(I,axis2)=( .5*height*(1.-cos(t)) + ya )*(1.-r(I,axis2))+ yb*r(I,axis2);
    }
    if( computeMapDerivative )
    {
      xr(I,axis1,axis1)=xb-xa;
      xr(I,axis1,axis2)=0.;
      where( r(I,axis1)<= ra ||  r(I,axis1)>= rb )
      {
	xr(I,axis2,axis1)=0.;
	xr(I,axis2,axis2)=yb-ya;
      }
      elsewhere( )
      {
	xr(I,axis2,axis1)=( .5*height*scaleFactor*sin(t) )*(1.-r(I,axis2));
	xr(I,axis2,axis2)=yb-ya-.5*height*(1.-cos(t));
      }
    }
    break;

  case diamond:
    if( computeMap )
    {
      x(I,axis1)=(xb-xa)*r(I,axis1)+xa; 
      x(I,axis2)=(yb-ya)*r(I,axis2)+ya; 
      where( r(I,axis1) > ra &&  r(I,axis1) <= .5*(ra+rb) )
      {
        x(I,axis2)=(2.*height/(rb-ra))*(r(I,axis1)-ra)*(1.-r(I,axis2)) + yb*r(I,axis2);
      }
      elsewhere( r(I,axis1) > .5*(ra+rb) &&  r(I,axis1) <=rb )
      {
        x(I,axis2)=(2.*height/(rb-ra))*(rb-r(I,axis1))*(1.-r(I,axis2)) + yb*r(I,axis2);
      }
    }
    if( computeMapDerivative )
    {
      xr(I,axis1,axis1)=xb-xa;
      xr(I,axis1,axis2)=0.;
      xr(I,axis2,axis1)=0.;
      xr(I,axis2,axis2)=yb-ya;
      where( r(I,axis1) > ra &&  r(I,axis1) <= .5*(ra+rb) )
      {
        xr(I,axis2,axis1)=(2.*height/(rb-ra))*(1.-r(I,axis2));
        xr(I,axis2,axis2)=(-2.*height/(rb-ra))*(r(I,axis1)-ra) + yb;
      }
      elsewhere( r(I,axis1) >  .5*(ra+rb) &&  r(I,axis1) <=rb )
      {
        xr(I,axis2,axis1)=(-2.*height/(rb-ra))*(1.-r(I,axis2));
        xr(I,axis2,axis1)=(-2.*height/(rb-ra))*(rb-r(I,axis1)) + yb;
      }
    }
    break;

    case naca:
      // define a naca series airfoil
      // parameters for NACA 4 airfoil   NACAabcc
      // maximumCamber                 // first digit
      // positionOfMaximumCamber       // second digit
      // thicknessToChordRatio         // last two digits

      // ***********************
      // **** upper surface ****
      // ***********************
      yc.redim(I), yt.redim(I), theta.redim(I);

      // parameterize the mapping using r^2 to remove the square root singularity at r=0
      where( r(I,axis1)<=.5 )
      {
	// for upper surface from trailing edge to leading edge
        t=.5*(1.+cos(twoPi*r(I,axis1)));  // dt/dr = -2 pi sqrt(t(1-t))
        // t=SQR(1.-2.*r(I,axis1));  // dt/dr = -4.*sqrt(t)
        // t=1.-2.*r(I,axis1);
      }
      elsewhere( )
      { // for lower surface from leading edge to trailing edge
        t=.5*(1.+cos(twoPi*r(I,axis1)));  //
        // t=SQR(2.*r(I,axis1)-1.);  // dt/dr = 4.*sqrt(t) 
        // t=2.*r(I,axis1)-1.;
      }
      
      // define the camber line, yc
      where( t >= x1 )
      { 
	yc=maximumCamber*(1./SQR(1.-x1))*((1.-2*x1)+2*x1*t-SQR(t));
	theta=atan(maximumCamber*(1./SQR(1.-x1))*(2*x1-2.*t));   // tan(theta) = d(yc)/dx
      }
      elsewhere( )
      {
	yc=maximumCamber*(1./SQR(x1))*(2*x1*t-SQR(t));
	theta=atan(maximumCamber*(1./SQR(x1))*(2*x1-2.*t));   // tan(theta) = d(yc)/dx
      }

      // thickness from camber line, yt
      yt=5.*thicknessToChordRatio*(0.29690*SQRT(t) +t*(-.12600 + t*(-.35160 + t*(.28430+ t*(-.10150)))) );

      t1=1.-trailingEdgeEpsilon;
      where( t > t1 )
      { // blend airfoil with a small circle
	yt+=  (t-t1)/(1.-t1)* (trailingEdgeEpsilon*SQRT(1.-t) -yt);   // round off the tip
      }

      if( computeMap )
      {
        where( r(I,axis1)<=.5 )
	{ // upper surface from trailing edge to leading edge
          x(I,axis1)= (t - yt*sin(theta))*chord;
          x(I,axis2)= (yc      + yt*cos(theta))*chord;
	}
	elsewhere( )
	{ // lower surface from leading edge to trailing edge
          x(I,axis1)= (t + yt*sin(theta))*chord;
          x(I,axis2)= (yc      - yt*cos(theta))*chord;
	}
	
      }
      
      if( computeMapDerivative )
      {
        ytt.redim(I);  yct.redim(I); thetat.redim(I);
        // ytt = d(yt)/dr * sqrt(t(1.-t))
	ytt=5.*thicknessToChordRatio*
	   (0.29690*.5*SQRT(1.-t) +SQRT(t*(1.-t))*(-.12600 + t*(-.35160*2. + t*(.28430*3.+ t*(-.10150*4.)))));
	where( t > t1 )
	{ // blend airfoil with a small circle
	  ytt+=  1./(1.-t1)*( trailingEdgeEpsilon*SQRT(1.-t) -yt )*SQRT(t*(1.-t))
	    +(t-t1)/(1.-t1)*(-trailingEdgeEpsilon*.5*SQRT(t)-ytt );  // note: ytt already is multiplied by sqrt()
	}
	where( t >= x1 )
	{
	  // d(yc)/dt :
	  yct=maximumCamber*(1./SQR(1.-x1))*(2*x1-2.*t);
	  // d(theta)/dt = yc'' * cos(theta)^2
	  thetat=maximumCamber*(1./SQR(1.-x1))*(-2.)*SQR(cos(theta));
	}
	elsewhere( )
	{
	  // d(yc)/dt :
	  yct=maximumCamber*(1./SQR(x1))*(2*x1-2.*t);
	  // d(theta)/dt = yc'' * cos(theta)^2
	  thetat=maximumCamber*(1./SQR(x1))*(-2.)*SQR(cos(theta));  
	}
	
        where( r(I,axis1)<=.5 )
	{ // upper surface from trailing edge to leading edge
          xr(I,axis1,axis1)=( (1.    -yt*cos(theta)*thetat)*SQRT(t*(1.-t)) -ytt*sin(theta) 
	                    )*(-2.*Pi*chord);
          xr(I,axis2,axis1)=( (yct-yt*sin(theta)*thetat)*SQRT(t*(1.-t)) +ytt*cos(theta) 
	                    )*(-2.*Pi*chord);
	}
	elsewhere( )
	{ // lower surface from leading edge to trailing edge
          xr(I,axis1,axis1)=( (1.    +yt*cos(theta)*thetat)*SQRT(t*(1.-t)) +ytt*sin(theta) 
	                    )*(+2.*Pi*chord);
          xr(I,axis2,axis1)=( (yct+yt*sin(theta)*thetat)*SQRT(t*(1.-t)) -ytt*cos(theta) 
	                    )*(+2.*Pi*chord);
	}
      }

      break;

  default:
    cout << "AirfoilMapping::ERROR:map: unknown airfoilType! \n";
    {throw "AirfoilMapping::ERROR:map: unknown airfoilType!";}
  }
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int AirfoilMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering AirfoilMapping::get" << endl;

  subDir.get( className,"className" ); 
  if( className != "AirfoilMapping" )
  {
    cout << "AirfoilMapping::get ERROR in className!" << endl;
  }
  subDir.get( xBound,"xBound" );
  int temp;
  subDir.get( temp,"airfoilType" );  airfoilType=(AirfoilTypes)temp;
  subDir.get( chord,"chord");
  subDir.get( maximumCamber,"maximumCamber" );
  subDir.get( positionOfMaximumCamber,"positionOfMaximumCamber");
  subDir.get( thicknessToChordRatio,"thicknessToChordRatio");
  subDir.get( trailingEdgeEpsilon,"trailingEdgeEpsilon");
  
  Mapping::get( subDir, "Mapping" );

  mappingHasChanged(); 

  delete &subDir;
  return 0;
}
int AirfoilMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( className,"className" );
  subDir.put( xBound,"xBound" );
  subDir.put( (int)airfoilType,"airfoilType" );
  subDir.put( chord,"chord");
  subDir.put( maximumCamber,"maximumCamber" );
  subDir.put( positionOfMaximumCamber,"positionOfMaximumCamber");
  subDir.put( thicknessToChordRatio,"thicknessToChordRatio");
  subDir.put( trailingEdgeEpsilon,"trailingEdgeEpsilon");

  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping *AirfoilMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==className )
    retval = new AirfoilMapping();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int AirfoilMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  const aString menu[] = 
    {
      "airfoil type",
      "specify corners",
      "chord",
      "thickness-chord ratio",
      "camber",
      "position of maximum camber",
      "round trailing edge",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  const aString help[] = 
    {
      "airfoil type       : choose an air-foil type (arc, sinusoid, diamond)",
      "specify corners    : Specify the corners of the Airfoil",
      "chord              : chord length",
      "thickness-chord ratio : specify the ratio of thickness/chord",
      "camber             : for NACA airfoil only",
      "position of maximum camber: for NACA airfoil only",
      "round trailing edge: specify a small parameter that rounds off the trailing edge",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line; 

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("AirFoil>");

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="specify corners" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter xa,ya,xb,yb (default=(%e,%e,%e,%e)): ",
          xBound(0,0),xBound(0,1),xBound(1,0),xBound(1,1)));
      if( line!="" ) sScanF(line,"%e %e %e %e ",&xBound(0,0),&xBound(0,1),&xBound(1,0),&xBound(1,1));
      mappingHasChanged();
    }
    else if( answer=="airfoil type" ) 
    {
      aString menu[] = {  
                         "arc",
                         "sinusoid",
                         "diamond",
                         "naca",
                         "no change",
                         ""
                       }; 
      int response = gi.getMenuItem(menu,answer,"Enter the airfoil type");   
      if( answer!="" && response >= 0 && response <= 3 )
      {
        airfoilType=(AirfoilTypes)response;

        if( airfoilType==naca )
	{
          gi.outputString("The NACA[c][p][tc] airfoil is defined by:");
          gi.outputString("  c = maximum camber/chord *100");
          gi.outputString("  p = position of maximum camber/chord * 10");
          gi.outputString(" tc = thickness/Chord *100");

          domainDimension=1;   // naca airfoil is just a curve
          thicknessToChordRatio=.12;
	  
	  setIsPeriodic(axis1,Mapping::functionPeriodic);
	  setBoundaryCondition(Start,axis1,-1);
	  setBoundaryCondition(End,  axis1,-1);
	}
	else
	{
          domainDimension=2;
	  setIsPeriodic(axis1,Mapping::notPeriodic);
	  setBoundaryCondition(Start,axis1,1);
	  setBoundaryCondition(End,  axis1,1);
	}
        mappingHasChanged();
      }
    }
    else if( answer=="chord" )
    {
      gi.inputString(line,sPrintF(buff,"Enter chord length (default=(%e)): ",chord));
      if( line!="" ) sScanF(line,"%e ",&chord);
      mappingHasChanged();
    }
    else if( answer=="thickness-chord ratio" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the value for thickness/chord (default=(%e)): ",thicknessToChordRatio));
      if( line!="" ) sScanF(line,"%e ",&thicknessToChordRatio);
      mappingHasChanged();
    }
    else if( answer=="camber" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the value for maximum camber/chord (default=(%e)): ",maximumCamber));
      if( line!="" ) sScanF(line,"%e ",&maximumCamber);
      mappingHasChanged();
    }
    else if( answer=="position of maximum camber" )
    {
      positionOfMaximumCamber=0.;      // second digit
      gi.inputString(line,sPrintF(buff,"Enter the position of the maximum camber (default=(%e)): ",
            positionOfMaximumCamber));
      if( line!="" ) sScanF(line,"%e ",&positionOfMaximumCamber);
      mappingHasChanged();
    }
    else if( answer=="round trailing edge" )
    {
      positionOfMaximumCamber=0.;      // second digit
      gi.inputString(line,sPrintF(buff,"Enter the rounding parameter (should be small) (default=(%e)): ",
            trailingEdgeEpsilon));
      if( line!="" ) sScanF(line,"%e ",&trailingEdgeEpsilon);
      mappingHasChanged();
    }
    else if( answer=="show parameters" )
    {
      printf(" chord = %e, thicknessToChordRatio = %e \n",chord,thicknessToChordRatio);
      printf(" (xa,xb) = (%e,%e), (ya,yb)=(%e,%e) \n",xBound(0,0),xBound(0,1),xBound(1,0),xBound(1,1));
      display();
      mappingHasChanged();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject )
    {
      if( airfoilType==naca )
      {
        parameters.set(GI_TOP_LABEL,getName(mappingName)+sPrintF(buff," NACA%1i%1i%2i",
           int(maximumCamber*chord*100+.5),int(positionOfMaximumCamber*chord*10+.5),
           int(thicknessToChordRatio*100.+.5)));
      }
      else
        parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}
