#include "DomainSolver.h"
#include "NameList.h"
#include "GenericGraphicsInterface.h"
#include "MaterialProperties.h"
#include "Chemkin.h"
#include "Ogshow.h"
#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
#include <algorithm>	   // STL algorithms class library
OV_USINGNAMESPACE(std);
#else
#include <vector.h>
#include <algorithm.h>	   // STL algorithms class library
#endif

//\begin{>>DomainSolverInclude.tex}{\subsection{displayBoundaryConditions}} 
void DomainSolver::
displayBoundaryConditions(FILE *file /* = stdout */)
{
//===================================================================================
// /Description:
//   Print names for boundary conditions
//
// /file (input) : write to this file.
//\end{DomainSolverInclude.tex}  
//===================================================================================
  assert( file!=NULL );

  int maxNameLength=3;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    maxNameLength=max( maxNameLength,cg[grid].getName().length());

  char buff[80];
  sPrintF(buff," %%4i: %%%is     %%i    %%i    %%3i :",maxNameLength); // build a format string

  IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

  aString blanks="                                                                           ";
  fprintf(file," grid   name%s side axis    boundary condition and name\n",
           (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
  fprintf(file," ----   ----%s ---- ----    ---------------------------\n",
           (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
    for( int side=Start; side<=End; side++ )
    {
      int bc=cg[grid].boundaryCondition()(side,axis);
      // fprintf(file," %4i: %15s   %i    %i    %3i :",grid,(const char *)cg[grid].getName(),side,axis,bc);
      fprintf(file,buff,grid,(const char *)cg[grid].getName(),side,axis,bc);

      //      if( bc > 0 && bc<parameters.numberOfBCNames)
      if( bc>0 && parameters.checkForValidBoundaryCondition(bc,false)==0)//bc<parameters.numberOfBCNames )
        fprintf(file," %s",(const char*)parameters.bcNames[bc]);
      else if( bc==0 )
        fprintf(file," %s","none");
      else if( bc<0 )
        fprintf(file," %s","periodic");
      else
        fprintf(file," %s","unknown");

      if( interfaceType(side,axis,grid)!=Parameters::noInterface )
      {
        int iType=interfaceType(side,axis,grid);
	assert( iType>=0 && iType<parameters.icNames.size() );
	fprintf(file,"  (%s)",(const char*)parameters.icNames[iType]);
      }

      fprintf(file,"\n");
    }
  }
}

//\begin{>>ParametersInclude.tex}{\subsection{setDefaultDataForABoundaryCondition}} 
int 
Parameters::
setDefaultDataForABoundaryCondition(const int & side,
				    const int & axis,
				    const int & grid,
				    CompositeGrid & cg)
// ============================================================================================
// /Description:
//    Assign the default values for the data required by a boundary condition.
// /grid,side,axis (input) : defines a boundary
//  
//\end{ParametersInclude.tex}  
// ============================================================================================
{


  return 0;
}

//\begin{>>DomainSolverInclude.tex}{\subsection{setDefaultDataForBoundaryConditions}} 
int DomainSolver::
setDefaultDataForBoundaryConditions()
// ============================================================================================
// /Description:
//    Assign the default values for the data required by the boundary conditions.
//\end{DomainSolverInclude.tex}  
// ============================================================================================
{
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	parameters.setDefaultDataForABoundaryCondition(side,axis,grid,cg);
      }
    }
  }
  return 0;
}

//\begin{>>ParametersInclude.tex}{\subsection{checkForValidBoundaryCondition}} 
int 
Parameters::
checkForValidBoundaryCondition( const int & bc, bool reportErrors/*=true*/ )
// =================================================================================
// /Description:
//   Check that a given boundary condition is valid for a given PDE.
//\end{ParametersInclude.tex}  
// =================================================================================
{
  if ( bcNames.count(bc)==0 )
    {
      if( reportErrors )
	{
	  printF("checkForValidBoundaryConditionERROR::ERROR: The boundary condition number %i is not valid for this method\n",bc);
	}
      return 1;
    }

  return 0;
}
   
//  static int
//  chooseUserDefinedBoundaryValues(int side,int axis,int grid)
//  {
//    printF("chooseUserDefinedBoundaryValues: **** fix me ****\n");
//    return 0;
//  }

bool
Parameters::isMixedBC(int bc) { return false; }

//\begin{>>ParametersInclude.tex}{\subsection{setBoundaryConditionValues}}   
int 
Parameters::
setBoundaryConditionValues(const aString & answer,
			   const IntegerArray & originalBoundaryCondition,
			   CompositeGrid & cg)
// ============================================================================================
// /Description:
//     Parse the string that defines the boundary conditions and options.
//  Change the boundary conditions and set the data for the boundary conditions.
//
// /NOTE on the bcData array: 
//  Boundary condition parameter values are stored in the array bcData. Let nc=numberOfComponents,
//  then the values 
//
//          bcData(i,side,axis,grid)  : i=0,1,...,nc-1 
//
//  would normally represent the RHS values for dirichlet BC's on component i, such as
//            u(i1,i2,i3,i) = bcData(i,side,axis,grid) 
// 
//  For a Mixed-derivative boundary condition, the parameters (a0,a1,a2) in the mixed BC:
//               a1*u(i1,i2,i3,i) + a2*u(i1,i2,i3,i)_n = a0
//  are stored in
//          a_j = bcData(i+nc*(j),side,axis,grid),  j=0,1,2
// 
//  Thus bcData(i,side,axis,grid) still holds the RHS value for the mixed-derivative condition
// 
//\end{ParametersInclude.tex}  
// ============================================================================================
{
  const int nc = dbase.get<int >("numberOfComponents");

  int grid,side,axis,n;
//    aString answer2, answer3;
//    char buff[200];

  aString gridName, bcName;
  Range G,S,A;
  int bc;
  RealArray value;	
  int len=0;

  int length=answer.length();
  int i,mark=-1;
  for( i=0; i<length; i++ )
  {
    if( answer[i]=='(' || answer[i]=='=' )
    {
      mark=i-1;
      break;
    }
  }
  if( mark<0 )
  {
    printF("unknown form of answer=[%s]. Try again or type `help' for examples.\n",(const char *)answer);
    // gi.stopReadingCommandFile();
    return 1; 
  }

  gridName=answer(0,mark);  // this is the name of the grid or `all'
  S=Range(-1,-1);
  A=Range(-1,-1);
  if( answer[mark+1]=='(' )
  { // determine which side and axis to assign
    int side=-1,axis=-1;
    int numRead=sscanf(answer(mark+1,length-1),"(%i,%i)",&side,&axis);
    if( numRead==2 )
    {
      if( side>=0 && side<=1 && axis>=0 && axis<=cg.numberOfDimensions()-1 )
      {
	S=Range(side,side);
	A=Range(axis,axis);
	for( i=mark+1; i<length; i++ )
	{
	  if( answer[i]=='=' )
	  {
	    mark=i-1;
	    break;
	  }
	}
      }
      else
      {
	printf("invalid values for side=%i or axis=%i, 0<=side<=1, 0<=axis<=%i \n",side,axis,
	       cg.numberOfDimensions()-1);
	return 1;
      }
    }
  }
  else
  { // assign all sides:
    S=Range(0,1);
    A=Range(0,cg.numberOfDimensions()-1);
  }
  if( S.getBase()==-1 || A.getBase()==-1 || mark+2>length-1 )
  {
    printF("unknown form of answer=[%s]. Try again or type `help' for examples.\n",(const char *)answer);
    // gi.stopReadingCommandFile();
    return 1; 
  }
  // search for a blank separating the bc name from any options
  int endOfName=length-1;
  for( i=mark+3; i<length; i++ )
  {
    if( answer[i]==' ' || answer[i]==',' )
    {
      endOfName=i-1;
      break;
    }
  }
  bcName=answer(mark+2,endOfName);
  mark=endOfName+1;

  G=Range(-1,-1);
  int changeBoundaryConditionNumber=-1;  // for bcNumber#=bcName

  if( gridName=="all" )
  {
    G=Range(0,cg.numberOfComponentGrids()-1);
  }
  else if( len=gridName.matches("bcNumber") )
  {
    // BC of the form
    //    bcNumber3=noSlipWall
    G=Range(0,cg.numberOfComponentGrids()-1); // check all grids

    sScanF(gridName(len,gridName.length()-1),"%i",&changeBoundaryConditionNumber);
    printF("setting BC number %i to be %s\n",changeBoundaryConditionNumber,(const char*)bcName);

  }
  else
  { // search for the name of the grid
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( gridName==cg[grid].mapping().getName(Mapping::mappingName) )
      {
	G=Range(grid,grid);
	break;
      }
    }
  }
  if( G.getBase()==-1  )
  {
    printF("Unknown grid name = <%s> \n",(const char *)gridName);
    // gi.stopReadingCommandFile();
    return 1;
  }


  // ** search for the name of an interface condition -- put this first for now 
  // If the bc is not found then check for an interface condition *wdh* 080514
  if( true || bc<=0 )
  {
    int ic=-1;
    for ( Parameters::BCIterator it=icNames.begin(); it!=icNames.end(); it++,i++ )
    {
      if( bcName==it->second )
      {
	ic=it->first;
	break;
      }
    }
    if( ic>=0 )
    {
      // printF("Interface type found : %s\n",(const char*)bcName);
      IntegerArray & interfaceType = dbase.get<IntegerArray >("interfaceType");
      for( grid=G.getBase(); grid<=G.getBound(); grid++ )
      {
	for( int axis=A.getBase(); axis<=A.getBound(); axis++ )
	{
	  for( int side=S.getBase(); side<=S.getBound(); side++ )
	  {
	    if( cg[grid].boundaryCondition(side,axis) > 0 && 
		(changeBoundaryConditionNumber==-1 || 
		 originalBoundaryCondition(side,axis,grid)==changeBoundaryConditionNumber) )
	    {
              printF("Set interfaceType(%i,%i,%i)=%s\n",side,axis,grid,(const char*)bcName);
	      interfaceType(side,axis,grid)=ic;
	    }
	  }
	}
      }
      return 0;
    }
  }

  // search for the name of the boundary condition
  bc=-1;
  i=0;
  for ( Parameters::BCIterator it=bcNames.begin(); it!=bcNames.end(); it++,i++ )
  {
    if ( it->second == "interpolation" ) continue;
    if( bcName==it->second )
    {
      bc=it->first;
      break;
    }
  }


  if( bc<=0 || checkForValidBoundaryCondition( bc )!=0 )
  {
    printF("Unknown boundary condition = <%s> \n",(const char *)bcName);
    // gi.stopReadingCommandFile();
    return 1; 
  }

  // Certain names are reserved keywords and grids should not be named with these keywords.
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    aString gridName = cg[grid].getName();
    if( gridName=="uniform" || 
        gridName=="oscillate" ||
        gridName=="ramp" ||
        gridName=="parabolic" ||
        gridName=="blasius" ||
        gridName=="pressure" ||
        gridName=="mixedDerivative" ||
        gridName=="userDefinedBoundaryData" ||
        gridName=="variableBoundaryData" )
    {
      printF("setBoundaryConditionValues:ERROR: grid %i is named [%s] -- this is a reserved key word for boundary conditions.\n"
             " Please rename the grid.\n",grid,(const char*)gridName);
      OV_ABORT("ERROR");
    }
  }
  

  // *********************************************************
  // search for options that define the data for the BC's:
  //    uniform(c0=v0,c1=v1,...)
  //    parabolic(par0=val0,par1=val1,...)
  //    oscillate(par0=val0,...)
  //    ramp(par0=val0,...)
  // *********************************************************
  mark=endOfName+1;
  const char *line = answer;

  const char *uniform, *parabolic, *blasius, *oscillate, *pressure, *mixedDerivative, 
    *ramp, *jet, *userDefinedBoundaryData, *variableBoundaryData;
  uniform=strstr(line,"uniform(");
  oscillate=strstr(line,"oscillate(");
  ramp=strstr(line,"ramp(");
  parabolic=strstr(line,"parabolic(");
  blasius=strstr(line,"blasius(");
  pressure=strstr(line,"pressure(");
  mixedDerivative=strstr(line,"mixedDerivative(");
  userDefinedBoundaryData=strstr(line,"userDefinedBoundaryData");
  variableBoundaryData=strstr(line,"variableBoundaryData");
  jet=strstr(line,"jet");

  // 111205 kkc : see if there are any bc modifiers specified
  const char *bcModifier = 0;
  for ( Parameters::BCModCreatorIterator bcm=bcModCreators.begin();
	bcm!=bcModCreators.end() && !bcModifier;
	bcm++)
    {
      if ( strstr(line,bcm->first.c_str()) )
	{
	  cout<<"FOUND BC MODIFIER : "<<bcm->first.c_str()<<" : "<<line<<endl;
	  bcModifier = bcm->first.c_str();
	}
    }

  const int maxNumberOfParameters=20;
//  char c[maxNumberOfParameters][10];
  aString c[maxNumberOfParameters];
  real val[maxNumberOfParameters];
	  
  int numRead=0;
  if( uniform!=NULL || parabolic!=NULL || blasius!=NULL || jet!=NULL )
  {
    // *** extract values for the components rho,u,v,... that are specfied by a line of the form:
    //     prefix(name1=val1,name2=val2,...)
    // where prefix=uniform, or prefix=parabolic, ...

    aString prefix = uniform!=NULL ? "uniform" : parabolic!=NULL ? "parabolic" : blasius!=NULL ? "blasius" : "jet";
    int lenPrefix=prefix.length();
    // aString line=uniform;
    aString line=uniform!=NULL   ? uniform : 
                 parabolic!=NULL ? parabolic : 
                 blasius!=NULL   ? blasius : 
                 jet!=NULL       ? jet : 
                                   " ";
    line=line(lenPrefix+1,line.length()); // strip off "prefix("
    int len=line.length();
    int ie=len-1;
    for( int i=0; i<len; i++ )  // look for ending ')'
    {
      if( line[i]==')' )
      {
	ie=i-1;
	break;
      }
    }
    line=line(0,ie);
    // look for components that are assigned values, c0=v1,
    if( dbase.get<int >("debug") & 4 )
      printF("setBC: scanning for %s: \n",(const char*)prefix);
    numRead=parseValues(line,c,val,maxNumberOfParameters);

  }
 
  real jetRadius=.5, jetBoundaryLayerWidth=.1, jetX0=0., jetY0=0., jetZ0=0.;
  if( jet!=NULL )
  {

    // look for some extra parameters
    real d=.1,r0=.5,x0=0.,y0=0.,z0=0.;
	    
    const char *pos;
    pos=strstr(jet,"d=");
    if( pos!=NULL ) sScanF(pos+2,"%e",&jetBoundaryLayerWidth);
    pos=strstr(jet,"r=");
    if( pos!=NULL ) sScanF(pos+2,"%e",&jetRadius);
    pos=strstr(jet,"x=");
    if( pos!=NULL ) sScanF(pos+2,"%e",&jetX0);
    pos=strstr(jet,"y=");
    if( pos!=NULL ) sScanF(pos+2,"%e",&jetY0);
    pos=strstr(jet,"z=");
    if( pos!=NULL ) sScanF(pos+2,"%e",&jetZ0);

    printF("Jet: parameters: jetBoundaryLayerWidth=%e, r=%e, x=%e, y=%e, z=%e\n",jetBoundaryLayerWidth,
	   jetRadius,jetX0,jetY0,jetZ0);
	    
  }
	  

  real oscillateT0=0., oscillateOmega=1., oscillateAmplitude0=0., oscillateAmplitude1=1., 
    oscillateOffsetU=0., oscillateOffsetV=0., oscillateOffsetW=0., oscillateMaxTime=-1.;
  if( oscillate!=NULL )
  {
    // get parameters for oscillating inflow
    const char *pos;
    pos=strstr(oscillate,"t0=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&oscillateT0);
    pos=strstr(oscillate,"omega=");
    if( pos!=NULL )                           
      sScanF(pos+6,"%e",&oscillateOmega);
    pos=strstr(oscillate,"a0=");
    if( pos!=NULL )                           
      sScanF(pos+3,"%e",&oscillateAmplitude0);
    pos=strstr(oscillate,"a1=");
    if( pos!=NULL )                           
      sScanF(pos+3,"%e",&oscillateAmplitude1);
    pos=strstr(oscillate,"u0=");
    if( pos!=NULL )                           
      sScanF(pos+3,"%e",&oscillateOffsetU);
    pos=strstr(oscillate,"v0=");
    if( pos!=NULL )                           
      sScanF(pos+3,"%e",&oscillateOffsetV);
    pos=strstr(oscillate,"w0=");
    if( pos!=NULL )                           
      sScanF(pos+3,"%e",&oscillateOffsetW);
    pos=strstr(oscillate,"maxTime=");
    if( pos!=NULL )                           
      sScanF(pos+8,"%e",&oscillateMaxTime);
    printF(" Reading oscillation parameters: omega=%e, t0=%e, a0=%e, a1=%e, (u0,v0,w0)=(%e,%e,%e), maxTime=%e \n",
	   oscillateOmega,oscillateT0,oscillateAmplitude0,oscillateAmplitude1,
	   oscillateOffsetU,oscillateOffsetV,oscillateOffsetW,oscillateMaxTime);
  }

  real ta=0.,tb=1.,ua=0.,ub=1.,va=0.,vb=0.,wa=0.,wb=0.,Ta=1.,Tb=1.,ra=1.,rb=1.;
  if( ramp!=NULL )
  {
    // get parameters for ramped inflow
    const char *pos;
    pos=strstr(ramp,"ta=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&ta);
    pos=strstr(ramp,"tb=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&tb);
    pos=strstr(ramp,"ua=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&ua);
    pos=strstr(ramp,"ub=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&ub);
    pos=strstr(ramp,"va=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&va);
    pos=strstr(ramp,"vb=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&vb);
    pos=strstr(ramp,"wa=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&wa);
    pos=strstr(ramp,"wb=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&wb);
    pos=strstr(ramp,"Ta=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&Ta);
    pos=strstr(ramp,"Tb=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&Tb);

    pos=strstr(ramp,"ra=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&ra);
    pos=strstr(ramp,"rb=");
    if( pos!=NULL )
      sScanF(pos+3,"%e",&rb);

    printF(" Reading ramp parameters: ta=%6.2e, tb=%6.2e, ra=%6.2e, rb=%6.2e, ua=%6.2e, ub=%6.2e, va=%6.2e, vb=%6.2e,"
	   " wa=%6.2e, wb=%6.2e, Ta=%6.2e, Tb=%6.2e\n",ta,tb,ra,rb,ua,ub,va,vb,wa,wb,Ta,Tb);
  }
	  

  int component=-1;
  real mixedDerivValue[3];  // *wdh* 051201 -- to support adiabatic noSlipWall
  if( pressure!=NULL )
  {
    numRead=sScanF(pressure,"pressure(%e*p+%e*p.n=%e",&mixedDerivValue[0],&mixedDerivValue[1],&mixedDerivValue[2]);
    if( numRead==3 )
    {
      component=dbase.get<int >("pc");
      numRead=0;  // this means that there are no components whose values need to be filled in

      printF("Parameters::setBoundaryConditionValues: pressure BC: %e*p+%e*p.n=%e \n",
              mixedDerivValue[0],mixedDerivValue[1],mixedDerivValue[2]);
    }
    else
    {
      printF("Parameters::setBoundaryConditionValues: 'pressure' option found but there this no pressure component\n");
      Overture::abort("error");
    }
  }
  if( mixedDerivative!=NULL )
  {
    numRead=sScanF(mixedDerivative,"mixedDerivative(%e*t+%e*t.n=%e",&mixedDerivValue[0],&mixedDerivValue[1],&mixedDerivValue[2]);
    if( numRead==3 )
    {
      // The mixedDerivValue[i] values are set below...
      component=dbase.get<int >("tc");
      numRead=0;  // this means that there are no components whose values need to be filled in

      printF("Parameters::setBoundaryConditionValues: mixed derivative BC: %e*t+%e*t.n=%e (component='tc'=%i)\n",
	     mixedDerivValue[0],mixedDerivValue[1],mixedDerivValue[2],component);
    }
    else
    {
      numRead=sScanF(mixedDerivative,"mixedDerivative(%e*p+%e*p.n=%e",&mixedDerivValue[0],&mixedDerivValue[1],
                                                               &mixedDerivValue[2]);
      if( numRead==3 )
      {
	component=dbase.get<int >("pc");
	numRead=0;  // this means that there are no components whose values need to be filled in

	printF("Parameters::setBoundaryConditionValues: pressure BC: %e*p+%e*p.n=%e \n",
	       mixedDerivValue[0],mixedDerivValue[1],mixedDerivValue[2]);
      }
      else
      {
	printF("Parameters::setBoundaryConditionValues: mixedDerivative option found but there this no temperature"
               " or pressure component. \n");
	Overture::abort("error");
      }
    }
  }

  // *wdh* 2015/03/27 if( userDefinedBoundaryData!=NULL || variableBoundaryData!=NULL )
  if( variableBoundaryData!=NULL )
  {
    numRead=0;
    //             char *pos;
    //             pos=strstr(userDefinedBoundaryData,"code=");
    //             if( pos!=NULL )
    //     	      sScanF(pos+5,"%i",&code);
  }

  value.redim(nc+3); 
  value=(real)Parameters::defaultValue; // this means do not change the value from the default
  real parabolicBoundaryLayerWidth=.1;
  real reynoldsNumberX = 1000.;
	  

  // ****************************************************************************
  // fill in the value() array from c[] and val[]: 
  //             value(parameters.dbase.get<int >("uc"))=val[i] if c[i]='u'
  // ****************************************************************************
  if ( parabolic!=NULL )
  {
    // fill in the value array: value(parameters.dbase.get<int >("uc"))=val[i] if c[i]='u'
    //  "d" is an extra name to add to the list of known names.
    const int dc = nc+1;
    value(dc)=parabolicBoundaryLayerWidth;  // give a default value
    assignParameterValues("boundary conditions",value,numRead,c,val, "d",dc ); 
    parabolicBoundaryLayerWidth=value(dc);  // here we use the value extracted for "d"

    // printf("PARABOLIC: value=[%e,%e,%e,%e]\n",value(0),value(1),value(2),value(3));
    
  }
  else if ( blasius!=NULL )
  {
    // fill in the value array: value(parameters.dbase.get<int >("uc"))=val[i] if c[i]='u'
    //  "R" is an extra name to add to the list of known names.
    const int rxc= nc+1;
    value(rxc) = reynoldsNumberX;
    assignParameterValues("boundary conditions",value,numRead,c,val, "R",rxc );
    reynoldsNumberX = value(rxc);  // here we use the value extracted for "R"
  }
  else
  {

    // default case:
    assignParameterValues("boundary conditions",value,numRead,c,val ); 

    // *wdh* 070623 -- I think think this next section is obsolete: 
//     int nc=dbase.get<int >("numberOfComponents");
//     if( mixedDerivative!=NULL )
//     {
//       value(nc  )=mixedDerivValue[0];
//       value(nc+1)=mixedDerivValue[1];
//       value(nc+2)=mixedDerivValue[2];
//     }

  }


  RealArray timeDependenceParameters;
  // kkc  070130  BILL : this if statement was changed to get rid of the bc conditional; but I think it does the same thing
//   if( bc==Parameters::inflowWithVelocityGiven || bc==Parameters::subSonicInflow ||
//       bc==Parameters::noSlipWall )
  if ( parabolic || oscillate || ramp )
  {
    if( parabolic!=NULL )
    {
      if( parabolicBoundaryLayerWidth <=0. )
      {
	parabolicBoundaryLayerWidth=.1;
	printf("ERROR: width of the parabolic boundary layer is specified to be <= 0.\n"
	       "Setting to the default value of %e \n",parabolicBoundaryLayerWidth);
      }
      printF("width of the parabolic boundary layer = %e \n",parabolicBoundaryLayerWidth);
    }
    if( oscillate!=NULL )
    {
      timeDependenceParameters.redim(8);
      timeDependenceParameters(0)=oscillateOmega;
      timeDependenceParameters(1)=oscillateT0;
      timeDependenceParameters(2)=oscillateAmplitude0;
      timeDependenceParameters(3)=oscillateAmplitude1;
      timeDependenceParameters(4)=oscillateOffsetU;
      timeDependenceParameters(5)=oscillateOffsetV;
      timeDependenceParameters(6)=oscillateOffsetW;
      timeDependenceParameters(7)=oscillateMaxTime;
	      
      printF("the oscillating inflow has the form: a0 + a1*cos((t-t0)*(2*pi*omega))*( uniform/parabolic)");
      printF(" omega=%e, t0=%e, a0=%e, a1=%e, max time=%e \n",oscillateOmega,oscillateT0,oscillateAmplitude0,
	     oscillateAmplitude1,oscillateMaxTime);
    }
    if( ramp!=NULL )
    {
      int nDim=max(int(dbase.get<int >("uc")),max(int(dbase.get<int >("vc")),max(int(dbase.get<int >("wc")),int(dbase.get<int >("tc")))))+1;

      #define TIMEPAR(n,c) timeDependenceParameters(2+(n)+2*(c))

      timeDependenceParameters.redim(2+2*nDim);
      timeDependenceParameters=0.;

      timeDependenceParameters(0)=ta;
      timeDependenceParameters(1)=tb;

      TIMEPAR(0,dbase.get<int >("uc"))=ua;  TIMEPAR(1,dbase.get<int >("uc"))=ub;
      TIMEPAR(0,dbase.get<int >("vc"))=va;  TIMEPAR(1,dbase.get<int >("vc"))=vb;
      if( dbase.get<int >("wc")>=0 )
      { 
        TIMEPAR(0,dbase.get<int >("wc"))=wa;  TIMEPAR(1,dbase.get<int >("wc"))=wb;
      }
      if( dbase.get<int >("tc")>=0 )
      {
	TIMEPAR(0,dbase.get<int >("tc"))=Ta;  TIMEPAR(1,dbase.get<int >("tc"))=Tb;
      }
      if( dbase.get<int >("rc")>=0 )
      {
	TIMEPAR(0,dbase.get<int >("rc"))=ra;  TIMEPAR(1,dbase.get<int >("rc"))=rb;
      }
      #undef TIMEPAR
    }
	    
  }
  
  //kkc 110609 else 
if ( mixedDerivative!=NULL  || isMixedBC(bc) ) 
       /* if( bc==Parameters::outflow ||       // kkc 070130 BILL : this if block section commented out for reasons given above
	   bc==Parameters::subSonicOutflow || 
	   bc==Parameters::convectiveOutflow ||
	   bc==Parameters::tractionFree ) */ 
  {
    // kkc 070131 ???? It is not clear that this default case is always correct

    value.redim(nc*3);
    value=(real)Parameters::defaultValue;  // this means do not change the value from the default
    if( (pressure!=NULL || mixedDerivative!=NULL ) && component>=0 && component<nc )
    { 
      // Save info for mixed derivative, alpha*p + beta*p.n = gamma
      //   value(c) = "gamma" for component c
      //   value(c+NC*1) = "alpha"   (NC=numberOfComponents)
      //   value(c+NC*2) = "beta"
	
      printF("Parameters::setBoundaryConditionValues: Assign mixed-derivative values mixedDerivValue=[%g,%g,%g]"
             " for component=%i\n",mixedDerivValue[0],mixedDerivValue[1],mixedDerivValue[2],component);
      
      value(component+nc*0)=mixedDerivValue[2];  // note: rhs value goes first
      value(component+nc*1)=mixedDerivValue[0];
      value(component+nc*2)=mixedDerivValue[1];
    }
    // else 
    //   value(Range(0,nc-1))=0.;
  }

 size_t bcModID = 0;
 if ( bcModifier )
   {
     bcModID = bcModifiers.size()+1;
     CreateBCModifierFromName cbcm = bcModCreators[bcModifier];
     bcModifiers[bcModID] = cbcm(bcModifier);
     bcModifiers[bcModID]->inputFromGI(*dbase.get<GenericGraphicsInterface* >("ps"));
   }

  // assign boundary conditions and data

  RealArray & bcData = dbase.get<RealArray>("bcData");
  RealArray & bcParameters =dbase.get<RealArray>("bcParameters");
  
  if( gridName=="all")
  {
    printF("Setting boundary condition to %s (bc=%i) on all grids.\n",(const char*)bcNames[bc],bc);
  }
  bool bcWasAssigned=false;
  for( grid=G.getBase(); grid<=G.getBound(); grid++ )
  {
    for( int axis=A.getBase(); axis<=A.getBound(); axis++ )
    {
      for( int side=S.getBase(); side<=S.getBound(); side++ )
      {
	//	 printF("grid,side,axis,original,change=%i,%i,%i,  %i,%i\n",grid,side,axis,
	//	        originalBoundaryCondition(side,axis,grid),changeBoundaryConditionNumber);
		
	if( cg[grid].boundaryCondition(side,axis) > 0 && 
	    (changeBoundaryConditionNumber==-1 || 
	     originalBoundaryCondition(side,axis,grid)==changeBoundaryConditionNumber) )
	{
	  bcWasAssigned=true;

	  cg[grid].setBoundaryCondition(side,axis,bc);
	  // set underlying mapping too (for moving grids)
	  cg[grid].mapping().getMapping().setBoundaryCondition(side,axis,bc);

	  if( gridName!="all" )
	    printF("Setting (side,axis)=(%i,%i) of grid %s to bc=%s, (number=%i)\n",side,axis,
		   (const char*)cg[grid].mapping().getName(Mapping::mappingName),
		   (const char*)bcNames[bc],bc);

	  setDefaultDataForABoundaryCondition(side,axis,grid,cg);

	  // the array bcData(.,side,axis,grid) holds the data that is
	  // passed directly to the boundary condition routines (if the BC doesn't vary in space)

	  if( false &&  // *wdh* 070704 -- bcData should be allocated properly by Parameters::updateToMatchGrid
              value.getBound(0) > bcData.getBound(0) )
	  {
	    Range all,R0(bcData.getBound(0)+1,value.getBound(0));
		    
	    bcData.resize(value.getLength(0),bcData.getLength(1),
				     bcData.getLength(2),bcData.getLength(3));
                    
	    bcData(R0,all,all,all)=0.; // give initial values
	  }

          // *** assign parameter values for this face:
          const bool printBcDataValues=false; 

	  if( printBcDataValues ) printF("Assign bcData values : \n");
	  for( n=0; n<=value.getBound(0); n++ )
	  {
	    if( value(n)!=(real)Parameters::defaultValue )
	    {
	      if( printBcDataValues ) printF("n=%i : %g,  ",n,value(n));
	      bcData(n,side,axis,grid)=value(n);
	    }
	  }
	  if( printBcDataValues ) printF("\n");
	  
	  // kkc 070130 BILL : merged this if statement into one block since there was repetition anyway
	  //                   note that there was no error checking here anyway
	  //	  if( bc==Parameters::inflowWithVelocityGiven || bc==Parameters::subSonicInflow )
	  //	  {
	  if ( blasius!=NULL )
	  {
	    setBcType(side,axis,grid,Parameters::blasiusProfile);
	    setTimeDependenceBoundaryConditionParameters(side,axis,grid,timeDependenceParameters);		    
	    setBcVariesInSpace(side,axis,grid,true);
	    bcParameters(1,side,axis,grid)=reynoldsNumberX;

	  }
	  else if( parabolic!=NULL )
	  {
	    if( ramp!=NULL )
	    {
	      setBcType(side,axis,grid,Parameters::parabolicInflowRamped);
	      setBcIsTimeDependent(side,axis,grid,true);
	      setTimeDependenceBoundaryConditionParameters(side,axis,grid,timeDependenceParameters);
			
//                         int rpStart=1;
// 			assert( bcParameters.getLength(0)>value.getLength(0) );
// 			for( n=0; n<=value.getBound(0); n++ )
// 			  bcParameters(rpStart+n,side,axis,grid)=value(n);
	    }
	    else if( oscillate!=NULL )
	    {
	      setBcType(side,axis,grid,Parameters::parabolicInflowOscillating );
	      setBcIsTimeDependent(side,axis,grid,true);
	      setTimeDependenceBoundaryConditionParameters(side,axis,grid,timeDependenceParameters);
	    }
	    else if( variableBoundaryData!=NULL )
	    {
	      setBcType(side,axis,grid,Parameters::parabolicInflowOscillating );
	      setBcIsTimeDependent(side,axis,grid,true);

	      setBcVariesInSpace(side,axis,grid,true);  // by default assume this
	      defineVariableBoundaryValues(side,axis,grid,cg); // *new* 110816
	    }
	    else if( userDefinedBoundaryData!=NULL )
	    {
	      // setBcType(side,axis,grid,Parameters::parabolicInflowOscillating );
	      setBcType(side,axis,grid,Parameters::parabolicInflowUserDefinedTimeDependence ); // *wdh* 2015/03/27 
	      setBcIsTimeDependent(side,axis,grid,true);

	      printF("--SBC-- set bcType=%i (parabolicInflowUserDefinedTimeDependence) bcData=[%e,%e,%e]\n",bcType(side,axis,grid),
		     bcData(0,side,axis,grid),bcData(1,side,axis,grid),bcData(2,side,axis,grid));
	      
	      setBcVariesInSpace(side,axis,grid,true);  // always assume this for user BC's
	      chooseUserDefinedBoundaryValues(side,axis,grid,cg);

	      printF("--SBC-- after chooseUserDefinedBoundaryValues bcType=%i (parabolicInflowUserDefinedTimeDependence)\n",bcType(side,axis,grid));

	    }
	    else
	      setBcType(side,axis,grid,Parameters::parabolicInflow);

	    setBcVariesInSpace(side,axis,grid,true);

	    bcParameters(0,side,axis,grid)=parabolicBoundaryLayerWidth;

	    // printF(" +++ setting bcParameters(0,%i,%i,%i)=%e\n",side,axis,grid,parabolicBoundaryLayerWidth);
	  }
	  else if( uniform!=NULL )
	  {
	    setBcType(side,axis,grid,Parameters::uniformInflow);
	    if( ramp!=NULL )
	    {
	      setBcType(side,axis,grid,Parameters::rampInflow);
	      setBcIsTimeDependent(side,axis,grid,true);
	      setTimeDependenceBoundaryConditionParameters(side,axis,grid,timeDependenceParameters);
	    }
	    else if( oscillate!=NULL )
	    {
	      setBcType(side,axis,grid,Parameters::uniformInflowOscillating );
	      setBcIsTimeDependent(side,axis,grid,true);
	      setTimeDependenceBoundaryConditionParameters(side,axis,grid,timeDependenceParameters);
	    }

            // *wdh* 110830 -- this should always be done I think (c.f. oscillating uniform inflow)
	    for( int n=0; n<=value.getBound(0); n++ )
	    {
	      if( value(n)!=(real)Parameters::defaultValue )
	      {
		bcParameters(n,side,axis,grid)=value(n);

		printF("bcParameters(%i) = %e\n",n,value(n));
		
	      }
	    }

	    
	  }
	  else if( ramp!=NULL )
	  {
	    setBcIsTimeDependent(side,axis,grid,true);
	    setBcType(side,axis,grid,Parameters::rampInflow);
	    setTimeDependenceBoundaryConditionParameters(side,axis,grid,timeDependenceParameters);

	  }
	  else if( oscillate!=NULL )
	  {
	  }
	  else if( jet!=NULL )
	  {
	    // the jet values are actually assigned in jetInflow (bcForcing.C)
	    setBcType(side,axis,grid,Parameters::jetInflow);
	    setBcVariesInSpace(side,axis,grid,true);
		      
	    bcParameters(0,side,axis,grid)=jetRadius;
	    bcParameters(1,side,axis,grid)=jetBoundaryLayerWidth;
	    bcParameters(2,side,axis,grid)=jetX0;
	    bcParameters(3,side,axis,grid)=jetY0;
	    bcParameters(4,side,axis,grid)=jetZ0;
		      
	  }
	  else if( variableBoundaryData!=NULL )
	  {
	    setBcVariesInSpace(side,axis,grid,true);  // by default assume this
	    defineVariableBoundaryValues(side,axis,grid,cg);  // *new* 110816
	  }

	  else if( userDefinedBoundaryData!=NULL )
	  {
	    setBcVariesInSpace(side,axis,grid,true);  // always assume this for user BC's
	    chooseUserDefinedBoundaryValues(side,axis,grid,cg);
	  }

	  if ( bcModID ) // 111205 kkc add the bcModifier id if needed
	    {
	      Parameters::BCModifier *bcm = bcModifiers[bcModID];
	      setBcModifier(side, axis, grid, bcModID);
	    }
		    
	}
      }
    }
  } // end for grid
  if( !bcWasAssigned && changeBoundaryConditionNumber>0 )
  {
    printF("\n setBoundaryConditions:INFO: There were no boundaries assigned for bcNumber%i=%s\n\n",
	   changeBoundaryConditionNumber,(const char*)bcName );
  }
	  
  return 0;
}



//\begin{>>DomainSolverInclude.tex}{\subsection{setBoundaryConditionsInteractively}}   
int DomainSolver::
setBoundaryConditionsInteractively(const aString & answer,
				   const IntegerArray & originalBoundaryCondition )
// ============================================================================================
// /Description:
//   Old way: parse an answer holding a boundary condition specification.
//\end{DomainSolverInclude.tex}  
// ============================================================================================
{
  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  // PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  
  int grid,side,axis,n;
  aString answer2, answer3;
  char buff[200];

  if( answer=="boundary conditions" )
  {
    gi.appendToTheDefaultPrompt("bc>");
	
    aString gridName, bcName;
    Range G,S,A;
    int bc;
    RealArray value;	
    int len=0;

    for(;;)
    {
      gi.inputString(answer2,"Enter a new boundary condition (or type `help' or `done')");
      if( answer2=="done" || answer2=="exit" )
	break;
      else if( answer2=="help" )
      {
	printf("BC: Enter a change to the boundary conditions. Type a string of the form     \n"
               "                                                                             \n"
	       "       <grid name>(side,axis)=<boundary condition name> [,option] [,option] ...\n"
               "                                                                             \n"
	       " to change the boundary condition on a given side of a given grid, or        \n"
               "                                                                             \n"
	       "       <grid name>=<boundary condition name>  [,option] [,option] ...          \n"
               "                                                                             \n"
               " to change all boundaries on a given grid, or                                \n"
               "                                                                             \n"
	       "       bcNumber<num>=<boundary condition name>  [,option] [,option] ... \n"
               "                                                                             \n"
               " to change all boundaries that *originally* had a boundary condition value of `num'\n"
               " Here \n"
	       "   <grid name> is the name of the grid, side=0,1 and axis=0,1,2.  <grid name> can also be `all'.\n"
               "                                                                             \n"
               " The optional arguments specify data for the boundary conditions:            \n"
               "    option = `uniform(p=1.,u=1.,...)'       : to specify a uniform inflow profile   \n"
               "    option = `parabolic(d=2,p=1.,...)'      : to specify a parabolic inflow profile \n"
	       "    option = `blasius(p=1.,u=1.,R=1000)'  : to specify a laminar boundary layer profile \n"
               "    option = `pressure(.1*p+1.*p.n=0.)'     : pressure boundary condition at outflow\n"
               "    option = `oscillate(t0=.5,omega=1.,a0=0.,a1=1.,u0=0,v0=0.,w0=0.)'  : oscillating values    \n"
               "    option = `ramp(ta=0.,tb=1.,ua=0.,ub=1.,va=0.,..)  : ramped inflow parameters    \n" 
               "    option = `jet(d=.1,r=1.,x=0.,y=0,z=0.,p=1.,...)': to specify a parabolic jet inflow profile \n" 
               "    option = `userDefinedBoundaryData : choose user defined data  \n"
	       " The pressure outflow boundary condition is a*p + b*p.n = c \n"
	       "  where p=pressure, p.n=normal derivative of p.\n"
	       "  Note that a and b should have the same sign or else the condition is unstable\n"
               "                                                                               \n"
               " The oscillate option is used with `uniform' or `parabolic' and takes the form:\n" 
               "         [u0,v0,w0] + { a0+a1*cos((t-t0)*(2*pi*omega)) }*[ uniform or parabolic profile ]  \n"
               "                                                                             \n"
               "The userDefinedBoundaryData option calls the function userDefinedBoundaryValues that\n"
               "may be changed to suit your purposes. \n"
	       " Examples: \n"
	       "     square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)     \n"
	       "     annulus=noSlipWall                         \n"
	       "     all=slipWall                               \n"
	       "     bcNumber2=slipWall                         \n"
	       "     square(0,1)=outflow , pressure(.1*p+1.*p.n=0.) \n"
                );
      
          
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  printF(" grid %i : name=%s \n",grid,(const char*)cg[grid].mapping().getName(Mapping::mappingName));
	printf("Available boundary conditions are: \n");
	//	for( int i=1; i<parameters.numberOfBCNames; i++ )
	int i=0;
	for ( Parameters::BCIterator it=parameters.bcNames.begin(); it!=parameters.bcNames.end(); it++,i++ )
	  {
	    if ( it->second == "interpolation" ) continue;
	    
	    printF(" %s \n",(const char*)parameters.bcNames[it->first]);
	  }
      }
      else 
      {
	int length=answer2.length();
	int i,mark=-1;
	for( i=0; i<length; i++ )
	{
	  if( answer2[i]=='(' || answer2[i]=='=' )
	  {
	    mark=i-1;
	    break;
	  }
	}
	if( mark<0 )
	{
	  printF("unknown form of answer=[%s]. Try again or type `help' for examples.\n",(const char *)answer2);
	  gi.stopReadingCommandFile();
	  continue;
	}
	else
	{

	  parameters.setBoundaryConditionValues(answer2,originalBoundaryCondition,cg);

  	}
      }
    } // end for( ;; )
    
    gi.unAppendTheDefaultPrompt();
  } // end if( answer=="boundary conditions" )

  // kkc 070130 : BILL : can we get rid of the stuff in this ifdef block??
// #ifdef OLD_BC_INTERFACE
//   else if( answer=="data for boundary conditions" )
//   {
//     // fill in the parameters.bcData(.,side,axis,grid) array with data for boundary conditions
//     // for example an inflow BC will need some values

//     gi.appendToTheDefaultPrompt("bc data>");



//     RealArray value;
//     // first make a list of the distinct BC's
//     intArray distinctBC(cg.numberOfComponentGrids()*cg.numberOfDimensions()*2);
//     int numberOfDistinctBC=0, bc;
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     {
//       for( axis=0; axis<cg.numberOfDimensions(); axis++ )
//       {
// 	for( side=Start; side<=End; side++ )
// 	{
// 	  bc=cg[grid].boundaryCondition()(side,axis);
// 	  if( bc > 0 )
// 	  {
// 	    for( int i=0; i<numberOfDistinctBC; i++ )
// 	    {
// 	      if( bc==distinctBC(i) )
// 	      {
// 		bc=-1;  // this bc is already in the list
// 		break;
// 	      }
// 	    }
// 	    if( bc>0 && parameters.checkForValidBoundaryCondition(bc,false)==0)//bc<parameters.numberOfBCNames )
// 	      distinctBC(numberOfDistinctBC++)=bc;
// 	    else //if( bc > parameters.numberOfBCNames )
// 	    {
// 	      printF("ERROR: There is an unknown BC=%i on side (%i,%i,%i) = (%s,side,axis) \n",
// 		     bc,grid,side,axis,(const char *)cg[grid].mapping().getName(Mapping::mappingName));
// 	    }
// 	  }
// 	}
//       }
//     }
//     if( numberOfDistinctBC==0 )
//     {
//       printF("ERROR : There are no physical boundaries to give data for!\n");
//     }
//     else
//     {
//       aString *bcMenu = new aString[numberOfDistinctBC+2];
//       for( int i=0; i<numberOfDistinctBC; i++ )
// 	bcMenu[i]=parameters.bcNames[distinctBC(i)]; //  + sPrintF(buff," (number %i)",distinctBC(i));
//       bcMenu[numberOfDistinctBC]="done";
//       bcMenu[numberOfDistinctBC+1]="";
      
//       gi.appendToTheDefaultPrompt("bc>");
//       for(;;)
//       {
// 	bc=gi.getMenuItem(bcMenu,answer2,"Set data for which boundary condition?");
// 	if( answer2=="done" )
// 	  break;
// 	else if( bc<0 )
// 	{
// 	  cout << "unknown response=[" << answer2 << "]\n";
// 	  gi.stopReadingCommandFile();
// 	  continue;
// 	}
// 	// make a list of all sides with this boundary condition
// 	bc=distinctBC(bc);
// 	aString *sideMenu = new aString[cg.numberOfComponentGrids()*cg.numberOfDimensions()*2+3];
// 	int i=0;
// 	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// 	{
// 	  for( axis=0; axis<cg.numberOfDimensions(); axis++ )
// 	  {
// 	    for( side=Start; side<=End; side++ )
// 	    {
// 	      if( bc==cg[grid].boundaryCondition()(side,axis) )
// 	      {
// 		sideMenu[i++]=sPrintF(buff,"(%i,%i,%i) = (%s,side,axis)",grid,side,axis,
// 				      (const char*)cg[grid].mapping().getName(Mapping::mappingName));
// 	      }
// 	    }
// 	  }
// 	}
// 	sideMenu[i++]="all";
// 	sideMenu[i++]="done";
// 	sideMenu[i]="";
// 	gi.appendToTheDefaultPrompt("side>");
// 	for( ;; )
// 	{
// 	  int response=gi.getMenuItem(sideMenu,answer2,sPrintF(buff,"Set %s data for which side(s)?",
// 							       (const char*)parameters.bcNames[bc]));
// 	  if( response<0 )
// 	  {
// 	    printF("Unknown response = %s, try again..\n",(const char *) answer2);
// 	    gi.stopReadingCommandFile();
// 	    continue;
// 	  }
// 	  else if( answer2=="done" )
// 	    break;
// 	  else 
// 	  {
// 	    int inflowProfile=-1;
// 	    if( bc==Parameters::inflowWithVelocityGiven || bc==Parameters::subSonicInflow )
// 	    {
// 	      aString bcTypeMenu[]=
// 	      {
// 		"uniform",
// 		"parabolic",
// 		"uniform with oscillation",
// 		"parabolic with oscillation",
// 		""
// 	      };
// 	      gi.getMenuItem(bcTypeMenu,answer3,"Choose the inflow profile");
// 	      if( answer3=="uniform" || answer3=="uniform with oscillation" )
// 	      {
// 		inflowProfile=Parameters::uniformInflow;
// 	      }
// 	      else if( answer3=="parabolic" || answer3=="parabolic with oscillation" )
// 	      {
// 		inflowProfile=Parameters::parabolicInflow;
// 	      }
// 	      else
// 	      {
// 		cout << "Unknown response=" << answer3 << endl;
// 		gi.stopReadingCommandFile();
// 	      }

// 	      value.redim(nc+2); value=0;
// 	      for( n=0; n<nc; n++ )
// 	      {
// 		gi.inputString(answer3,sPrintF(buff,"Enter bc value for %s",
// 					       (const char *)parameters.dbase.get<aString* >("componentName")[n]));
// 		if( answer3!="" )
// 		  sScanF(answer3,"%e",&value(n));
// 	      }
// 	      printF(" %s boundary condition data: (",(const char*)parameters.bcNames[bc]);
// 	      for( n=0; n<nc; n++ )
// 		printf("%s=%f,",(const char *)parameters.dbase.get<aString* >("componentName")[n],value(n));
// 	      printF(")\n");
// 	      if( answer3=="uniform with oscillation" || answer3=="parabolic with oscillation" )
// 	      {
// 		n=nc;
// 		value(n)=1.;
// 		value(n+1)=0.;
// 		printf("the oscillating inflow has time dependence of cos[ (t-t0)*(2*pi*omega) ] ");
// 		gi.inputString(answer3,"Enter omega and t0");
// 		if( answer3!="" )
// 		  sScanF(answer3,"%e %e",&value(n),&value(n+1));
// 	      }
// 	    }
// 	    else if( bc==Parameters::outflow || 
//                      bc==Parameters::subSonicOutflow ||
//                      bc==Parameters::convectiveOutflow ||
//                      bc==Parameters::tractionFree )
// 	    {
// 	      value.redim(3);
// 	      value(0)=1., value(1)=1., value(2)=0.;
// 	      printF("The outflow/convectiveOutflow/tractionFree boundary condition for p is a*p + b*p.n = c \n"
// 		     " where p=pressure, p.n=normal derivative of p.\n"
// 		     " Note that a and b should have the same sign or else the condition is unstable\n"
// 		     " Default values are a=%e, b=%e, c=%e \n",value(0),value(1),value(2));
// 	      gi.inputString(answer3,"Enter a,b,c for bc: a*p + b*p.n = c");
// 	      if( answer3!="" )
// 		sScanF(answer3,"%e %e %e",&value(0),&value(1),&value(1));
// 	      printF(" new values: a=%e, b=%e, c=%e \n",value(0),value(1),value(2));
// 	    }
// 	    else if( bc==Parameters::noSlipWall )
// 	    {
// 	      value.redim(nc); value=0;
// 	      for( n=0; n<nc; n++ )
// 	      {
// 		gi.inputString(answer3,sPrintF(buff,"Enter bc value for %s",
// 					       (const char *)parameters.dbase.get<aString* >("componentName")[n]));
// 		if( answer3!="" )
// 		  sScanF(answer3,"%e",&value(n));
// 	      }
// 	      printF(" %s boundary condition data: (",(const char*)parameters.bcNames[bc]);
// 	      for( n=0; n<nc; n++ )
// 		printf("%s=%f,",(const char *)parameters.dbase.get<aString* >("componentName")[n],value(n));
// 	      printF(")\n");
// 	    }
// 	    else
// 	    {
// 	      printF("Sorry: I do not know what data to give this boundary condition\n");
// 	      break;
// 	    }
	      
// 	    // now fill in the bcData array
//             RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
//             RealArray & bcParameters = parameters.dbase.get<RealArray>("bcParameters");
// 	    if( answer2=="all" )
// 	    {
// 	      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
// 		for( axis=0; axis<cg.numberOfDimensions(); axis++ )
// 		  for( side=Start; side<=End; side++ )
// 		    if( bc==cg[grid].boundaryCondition()(side,axis) )
// 		    {
// 		      assert( value.getBound(0)<=bcData.getBound(0) );
// 		      for( n=0; n<=value.getBound(0); n++ )
// 			bcData(n,side,axis,grid)=value(n);

// 		      if( inflowProfile==Parameters::parabolicInflow )
// 		      {
// 			parameters.setBcType(side,axis,grid,Parameters::parabolicInflow);
// 			gi.inputString(answer3,sPrintF(buff,
// 						       "(grid=%s,side=%i,axis=%i) Enter the width of the parabolic boundary layer",
// 						       (const char*)cg[grid].mapping().getName(Mapping::mappingName),side,axis));
// 			if( answer3!="" )
// 			{
// 			  sScanF(answer3,"%e",&bcParameters(0,side,axis,grid));
// 			  printF("width of the parabolic boundary layer = %e \n",
//                                  bcParameters(0,side,axis,grid));
// 			}
// 		      }
// 		    }
// 	    }
// 	    else
// 	    {
// 	      sScanF(answer2,"(%i %i %i)",&grid,&side,&axis);
// 	      assert( grid>=0 && grid<cg.numberOfComponentGrids() && side>=0 && side<=1 
// 		      && axis>=0 && axis<cg.numberOfDimensions() );
// 	      assert( value.getBound(0)<=bcData.getBound(0) );

// 	      for( n=0; n<=value.getBound(0); n++ )
// 		bcData(n,side,axis,grid)=value(n);

// 	      if( inflowProfile==Parameters::parabolicInflow )
// 	      {
// 		parameters.setBcType(side,axis,grid,Parameters::parabolicInflow);
// 		gi.inputString(answer3,"Enter the width of the parabolic boundary layer");
// 		if( answer3!="" )
// 		{
// 		  sScanF(answer3,"%e",&bcParameters(0,side,axis,grid));
// 		  printF("width of the parabolic boundary layer = %e \n",bcParameters(0,side,axis,grid));
// 		}
// 	      }
// 	    }
// 	  }
// 	} // for(;;)  this bc
// 	gi.unAppendTheDefaultPrompt();
// 	delete [] sideMenu;
//       }
//       gi.unAppendTheDefaultPrompt();
//       delete [] bcMenu;
//     }
//     gi.unAppendTheDefaultPrompt();
//   }
// #endif

  return 0;
  
}

//  int Parameters::
//  updateBoundaryConditionDialog( int side, int axis, int grid,
//                                 CompositeGrid & cg )
//  // =======================================================================================
//  // /Description:
//  //    Prompt for changes to boundary condition parameters for a particular face of a grid.
//  //
//  //     This function knows how to assign parameters and rhs data for all boundary conditions.
//  // =======================================================================================
//  {
//    assert( ps !=NULL );
//    GenericGraphicsInterface & gi = *ps;

//    GUIState dialog;
//    // DialogData & dialog = dialog.getDialogSibling();

//    dialog.setExitCommand("close","close");

//    assert( grid>=0 && grid<cg.numberOfComponentGrids() );
  
//    int bc0=cg[grid].boundaryCondition(side,axis);
//    assert( bc0>0 && bc0<numberOfBCNames );

//    BoundaryCondition bc = BoundaryCondition(bc0);
  
//    const aString sideLabel[]={ "left","right","bottom","top","back","front"};  //
//    aString label= sideLabel[side+2*axis];

//    const int numberOfTextStrings=20;
//    aString textLabels[numberOfTextStrings];
//    aString textStrings[numberOfTextStrings];    
//    int nt=0;

//    if( bc==noSlipWall )
//    {
//      aString name="noSlipWall";
//      dialog.setWindowTitle(label+" bc : "+name);
   
//      textLabels[nt] = "wall values:";  sPrintF(textStrings[nt], "u=%g, v=%g, w=%g",0.,0.,0.);  nt++; 

//  	// null strings terminal list
//      textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//      dialog.setTextBoxes(textLabels, textLabels, textStrings);

//    }
//    else if( bc==inflowWithVelocityGiven )
//    {
//      aString name="inflowWithVelocityGiven";
//      dialog.setWindowTitle(label+" bc : "+name);
   
//      aString tbCommands[] = {"uniform", 
//  			    "parabolic",
//  			    "oscillate",
//  			    ""};
//      int tbState[4];
//      tbState[0] = 0;
//      tbState[1] = 0;
//      tbState[2] = 0;
//      tbState[3] = 0;
//      int numColumns=1;
//      dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

//      textLabels[nt] = "uniform";    sPrintF(textStrings[nt], "u=%g",0.);  nt++; 
//      textLabels[nt] = "parabolic";  sPrintF(textStrings[nt], "width=%g",.25);  nt++; 

//  	// null strings terminal list
//      textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//      dialog.setTextBoxes(textLabels, textLabels, textStrings);

//    }
//    else if( bc==tractionFree )
//    {
//      gi.outputString("The traction free BC is:  P n_i - alpha nu n_i( D_j u_i + D_i u_j ) where alpha=1 (default)");

//      aString name="tractionFree";
//      dialog.setWindowTitle(label+" bc : "+name);
   
//      textLabels[nt] = "alpha :";  sPrintF(textStrings[nt], "alpha=%g",1.);  nt++; 

//  	// null strings terminal list
//      textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//      dialog.setTextBoxes(textLabels, textLabels, textStrings);
//    }
//    else
//    {
//      printF("updateBoundaryConditionDialog:ERROR: unknown value for bc=%i\n",bc);
//      return 1;
//    }
  

//    aString answer,line;

//    gi.pushGUI(dialog);
//    for( ;; )
//    {
//      gi.getAnswer(answer,"");
//      if( answer=="close" )
//      {
//        break;
//      }
//      else
//      {
//        cout << "Unknown command: " << answer << endl;
//      }
//    }

//    gi.popGUI();

//    return 0;
  
//  }

namespace
{ // these are ok as local variables 
  aString bcBoundaryChoice,faceChoice,bcTypeName,bcOption1,bcOption2;
}

// return the BC command
static aString 
getCommand()
{
  aString bcCommand;
  char buff[500];
  bcCommand = sPrintF(buff,"%s%s=%s %s %s",(const char*)bcBoundaryChoice,(const char*)faceChoice,
                      (const char*)bcTypeName,(const char*)bcOption1,(const char*)bcOption2);
  return bcCommand;
}


int Parameters::
defineBoundaryConditions(CompositeGrid & cg, 
                         const IntegerArray & originalBoundaryCondition,
                         const aString & command /* = nullString */,
			 DialogData *interface /* =NULL */ )
// =====================================================================================
// /Description:
//     Define the boundary conditions. This is the *new* dialog based way to specify
//  boundary conditions.
// =====================================================================================
{
  int returnValue=0;

  assert( dbase.get<GenericGraphicsInterface*>("ps")!=NULL );
  GenericGraphicsInterface & gi = *dbase.get<GenericGraphicsInterface*>("ps");

  // For now turn off the prefix -- not really needed anymore
  // aString prefix = "OBBC:"; // prefix for commands to make them unique.
//    const bool executeCommand = command!=nullString;
//    if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
//      return 1;

  const bool executeCommand = false;
  aString prefix = ""; // prefix for commands to make them unique.


  aString answer,line;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();
  

  GUIState gui;
  gui.setWindowTitle("Boundary conditions");
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;


  const int maxCommands=max(cg.numberOfGrids()+2,numberOfBCNames+1);
  aString *cmd = new aString[maxCommands];

  int &numberOfComponents = dbase.get<int>("numberOfComponents");
  BoundaryConditionParameters::ExtrapolationOptionEnum &extrapolationOption = dbase.get<BoundaryConditionParameters::ExtrapolationOptionEnum>("extrapolationOption");
  aString *&componentName = dbase.get<aString*>("componentName");
  int &orderOfExtrapolationForInterpolationNeighbours = dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours");
  int &orderOfExtrapolationForSecondGhostLine = dbase.get<int>("orderOfExtrapolationForSecondGhostLine");
  int & orderOfExtrapolationForOutflow = dbase.get<int >("orderOfExtrapolationForOutflow");

  GraphicsParameters &psp = dbase.get<GraphicsParameters>("psp");
  
  RealArray defaultState(numberOfComponents);
  defaultState=0.;

  int activeGrid=0;
  
  bcBoundaryChoice="all";
  faceChoice="";
  bcTypeName="noSlipWall";
  bcOption1=""; // ", uniform(p=0.,u=0.,v=0.)";
  bcOption2="";

  // Make a list of names of possible boundary conditions that are valid for this pde
  aString *bcNameList = new aString [numberOfBCNames+1];
  aString *bcNameLabel = new aString [numberOfBCNames+1];
  int numberOfValidBoundaryConditions=0;
  //  for( i=0; i<numberOfBCNames; i++ )
  int i=0;
  for ( Parameters::BCIterator it=bcNames.begin(); it!=bcNames.end(); it++,i++ )
  {
    if( checkForValidBoundaryCondition(it->first,false)==0 && it->first!=Parameters::interpolation )
    {
      bcNameList[numberOfValidBoundaryConditions]=bcNames[it->first];
      bcNameLabel[numberOfValidBoundaryConditions]=sPrintF(buff,"bc: %s",(const char*)bcNames[it->first]);
      if( numberOfValidBoundaryConditions==0 )
        bcTypeName=bcNames[it->first];  // set default
      
      numberOfValidBoundaryConditions++;
    }
  }
  bcNameList[numberOfValidBoundaryConditions]="";
  bcNameLabel[numberOfValidBoundaryConditions]="";


  if( interface==NULL || command=="build dialog" )
  {

    dialog.setOptionMenuColumns(1);

    aString *gridName = new aString [cg.numberOfComponentGrids()+2];
    aString *gridLabel = new aString [cg.numberOfComponentGrids()+2];
    gridName[0]="all";
    gridLabel[0]="boundary: all";
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      gridName[grid+1]=sPrintF(buff,"%s",(const char*)cg[grid].getName());
      gridLabel[grid+1]=sPrintF(buff,"boundary: %s",(const char*)cg[grid].getName());
    }
    gridName[cg.numberOfComponentGrids()+1]="";
    gridLabel[cg.numberOfComponentGrids()+1]="";
    
    dialog.addOptionMenu("boundary:", gridLabel, gridName,  0);


    // Find all the distinct boundary condition numbers *** should use old values of the bc ****
    std::vector<int> bcNumbers;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      for( int side=0; side<=1; side++ )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
          int bc = originalBoundaryCondition(side,axis,grid);
	  if( bc>0 && find(bcNumbers.begin(),bcNumbers.end(),bc)==bcNumbers.end() )
	  {
	    bcNumbers.push_back(bc);
	  }
	}
      }
    }
    // *** sort the vector ***
    sort(bcNumbers.begin(),bcNumbers.end());
    
    const int numberOfBoundaryConditions=bcNumbers.size();
    // build a list of valid boundary condition numbers
    aString *bcNumberCommand = new aString [numberOfBoundaryConditions+1];   
    aString *bcNumberLabel   = new aString [numberOfBoundaryConditions+1];   
    for( i=0; i<numberOfBoundaryConditions; i++ )
    {
      bcNumberCommand[i]=sPrintF("bc=%i",bcNumbers[i]);
      bcNumberLabel[i]=sPrintF("bcNumber: %i",bcNumbers[i]);
    }
    bcNumberCommand[numberOfBoundaryConditions]="";
    bcNumberLabel[numberOfBoundaryConditions]="";
    
    dialog.addOptionMenu("bc number:", bcNumberLabel, bcNumberCommand, 0);
    delete [] bcNumberCommand;
    delete [] bcNumberLabel;

    aString sideName[]= {"all", "left","right","bottom","top","back","front",""  };//
    aString sideLabel[]= {"set face all", "set face left","set face right","set face bottom","set face top",
                              "set face back","set face front",""  };

    if( cg.numberOfDimensions()==2 )
    {
      sideName[5]="";
      sideLabel[5]="";
    }
      
    dialog.addOptionMenu("set face:", sideLabel, sideName, 0 );
   
    dialog.addOptionMenu("bc:", bcNameLabel, bcNameList,0);

    // Main options

//          "    option = `uniform(p=1.,u=1.,...)'       : to specify a uniform inflow profile   \n"
//          "    option = `parabolic(d=2,p=1.,...)'      : to specify a parabolic inflow profile \n"
//          "    option = `blasius(p=1.,u=1.,R=1000)'  : to specify a laminar boundary layer profile \n"
//          "    option = `pressure(.1*p+1.*p.n=0.)'     : pressure boundary condition at outflow\n"
//          "    option = `oscillate(t0=.5,omega=1.,a0=0.,a1=1.,u0=0,v0=0.,w0=0.)'  : oscillating values    \n"
//          "    option = `ramp(ta=0.,tb=1.,ua=0.,ub=1.,va=0.,..)  : ramped inflow parameters    \n" 
//          "    option = `jet(d=.1,r=1.,x=0.,y=0,z=0.,p=1.,...)': to specify a parabolic jet inflow profile \n" 
//          "    option = `userDefinedBoundaryData : choose user defined data  \n"

    aString optionCommand[] = { "no option",
                                "uniform",
                                "parabolic",
                                "blasius",
                                "pressure",
                                "jet",
                                "user defined",
                                "" };  // 
    dialog.addOptionMenu("option:", optionCommand, optionCommand, 0);

    aString option2Command[] = { "no option2",
                                 "oscillate",
				 "ramp",
				 "" };  // 
    dialog.addOptionMenu("option2:", option2Command, option2Command, 0);


    aString extrapOptionCommand[] = { "polynomial extrapolation",
                                      "limited extrapolation",
				      "" };  // 
    dialog.addOptionMenu("extrap option:", extrapOptionCommand, extrapOptionCommand, (int)extrapolationOption);

/* ---
    aString sideName[3][2]= {"left:    ","right:  ","bottom:","top:     ","back:    ","front:   "  };//
    aString sideLabel[3][2]= {"set bc left ","set bc right","set bc bottom","set bc top",
                              "set bc back","set bc front"  };//
    aString periodicName[] ={ "periodic",""};  //
    aString interpolationName[] ={ "interpolation",""};  //
      
    int axis,side;
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( side=0; side<=1; side++ )
      {
        int bc=cg[activeGrid].boundaryCondition(side,axis);
	if( bc>0 )
	{
	  addPrefix(validBCName,sideLabel[axis][side],bcLabel,numberOfBCNames);
          addPrefix(bcLabel,prefix,cmd,maxCommands);
  	  dialog.addOptionMenu(sideName[axis][side], cmd, validBCName, bc );
	}
        else if( bc<0 )
  	  dialog.addOptionMenu(sideName[axis][side], periodicName, periodicName,0);
        else 
  	  dialog.addOptionMenu(sideName[axis][side], interpolationName, interpolationName,0);
      }
    }
  ---- */
    
//      aString outflowOptions[]={"default outflow",
//                                "check for inflow at outflow",
//                                "expect inflow at outflow"};

//      dialog.addOptionMenu("outflow options", outflowOptions,outflowOptions,parameters.dbase.get<int >("checkForInflowAtOutFlow"));
    


//      aString editBC[] = {"edit left","edit right","edit bottom","edit top","edit back","edit front",""};
//      editBC[cg.numberOfDimensions()*2]="";   // chop off those that are not needed.
//      addPrefix(editBC,prefix,cmd,maxCommands);
//      int numRows=1;
//      dialog.setPushButtons( cmd, editBC, numRows ); 

    aString pushButtonCommands[]={"apply bc command","help","plot grid",""}; //
    addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
    int numRows=5;
    dialog.setPushButtons( cmd, pushButtonCommands, numRows ); 

    // ----- Text strings ------
    const int numberOfTextStrings=20;
    aString textCommands[numberOfTextStrings];
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    
    textCommands[nt] = "bc command";  textLabels[nt]=textCommands[nt];
    textStrings[nt] = getCommand();
    nt++; 

    textCommands[nt] = "default state";  textLabels[nt]=textCommands[nt];
    textStrings[nt] = "";
    for( int n=0; n<numberOfComponents; n++ )
    {
      textStrings[nt]+=sPrintF(buff,"%s=%g",(const char *)componentName[n],defaultState(n));
      if( n<numberOfComponents-1 ) textStrings[nt]+=",";
    }
    nt++; 

    textCommands[nt] = "order of extrap for interp neighbours";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i", orderOfExtrapolationForInterpolationNeighbours); 
    nt++; 
    textCommands[nt] = "order of extrap for outflow";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i (-1=default)",orderOfExtrapolationForOutflow);  
    nt++; 

    textCommands[nt] = "order of extrap for 2nd ghost line";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i", orderOfExtrapolationForSecondGhostLine);  
    nt++; 

    textCommands[nt] = "moving body pressure BC coefficient";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%e", dbase.get<real>("movingBodyPressureCoefficient"));
    nt++; 

    textCommands[nt] = "moving body pressure BC";  textLabels[nt]=textCommands[nt];
    sPrintF(textStrings[nt], "%i", dbase.get<int>("movingBodyPressureBC"));
    nt++; 

    // null strings terminal list
    textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textCommands,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);


    delete [] cmd;
    delete [] gridName;
    delete [] gridLabel;
      
    const int maxNumberOfToggleButtons=5;
    aString tbCommands[maxNumberOfToggleButtons]; 
    int tbState[maxNumberOfToggleButtons];
    int ntb=0;

    tbCommands[ntb]="apply interface conditions";
    tbState[ntb]=dbase.get<int>("applyInterfaceBoundaryConditions");
    ntb++;
    

    assert( ntb<maxNumberOfToggleButtons );
    tbCommands[ntb]="";
     
    const int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    if( executeCommand ) return 0;
  }
  
  const bool squaresWerePlotted=gi.getPlotTheColouredSquares();
  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("bc>");  

    // colour boundaries by bc number
    psp.getBoundaryColourOption()=GraphicsParameters::colourByBoundaryCondition;
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    gi.setPlotTheColouredSquares(true);
  }
  int len=0;
  for(int it=0; ; it++)
  {

    if( !executeCommand )
    {
      gi.erase();
      PlotIt::plot(gi,cg,psp);  // plot the grid

      gi.getAnswer(answer,"");
    }
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    // gi.getMenuItem(pdeParametersMenu,answer,"change a pde parameter");


    if( answer=="done" )
      break;
    else if( answer=="plot grid" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      gi.erase();
      PlotIt::plot(gi,cg,psp);  // plot the grid
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( len=answer.matches("boundary:") )
    {
      line=answer(len+1,answer.length());
      if( line!="all" )
      { // check that we have chosen a valid grid name
        
      }

      // reset the options
      bcOption1=""; 
      dialog.getOptionMenu("option:").setCurrentChoice("no option");
      bcOption2=""; 
      dialog.getOptionMenu("option2:").setCurrentChoice("no option2");

      bcBoundaryChoice=line;
      dialog.setTextLabel("bc command",(const char*)getCommand());
    }
    else if( len=answer.matches("bc number:") )
    {
      int bcNumber=-1;
      sScanF(answer(len+1,answer.length()),"%i",&bcNumber);
      if( bcNumber>0 )
      { // check that we have chosen a valid bcNumber
        
	// reset the options
	bcOption1=""; 
	dialog.getOptionMenu("option:").setCurrentChoice("no option");
	bcOption2=""; 
	dialog.getOptionMenu("option2:").setCurrentChoice("no option2");

	bcBoundaryChoice=sPrintF(buff,"bcNumber%i",bcNumber);
	dialog.setTextLabel("bc command",(const char*)getCommand());

        dialog.getOptionMenu("bc number:").setCurrentChoice(answer);
      }
    }
    else if( len=answer.matches("set face") )
    {
      faceChoice = ( answer=="set face left"   ? "(0,0)" :
		     answer=="set face right"  ? "(1,0)" :
		     answer=="set face bottom" ? "(0,1)" :
		     answer=="set face top"    ? "(1,1)" :
		     answer=="set face back"   ? "(0,2)" : 
		     answer=="set face front"  ? "(1,2)" : "" );

      dialog.setTextLabel("bc command",(const char*)getCommand());
      dialog.getOptionMenu("set face:").setCurrentChoice(answer(len+1,answer.length()-1));
    }
    else if( len=answer.matches("bc:") )
    {
      line=answer(len+1,answer.length());
      // check that we have chosen a valid bc
      // **** finish this *****
      int bcOption=0;
      for( i=0; i<numberOfValidBoundaryConditions; i++ )
      {
	if( line==bcNameList[i] )
	{
	  bcOption=i;
	  break;
	}
      }
      if( bcOption>=0 )
      {
	// reset the options
	bcOption1=""; 
	dialog.getOptionMenu("option:").setCurrentChoice("no option");
	bcOption2=""; 
	dialog.getOptionMenu("option2:").setCurrentChoice("no option2");

	bcTypeName=line;
	dialog.setTextLabel("bc command",(const char*)getCommand());
	dialog.getOptionMenu("bc:").setCurrentChoice((int)bcOption);
      }
      else
      {
	printf("ERROR: invalid `bc:' command=[%s]\n",(const char *)answer);
	gi.stopReadingCommandFile();
      }
    }
    else if( answer=="no option" ||
             answer=="uniform" ||
	     answer=="parabolic" ||
	     answer=="blasius" ||
             answer=="pressure" ||
	     answer=="jet" ||
	     answer=="user defined" )
    {
      
      int option=0;
      if( answer=="no option" )
      {
	bcOption1="";
      }
      else if( answer=="uniform" )
      {
	// bcOption1=", uniform(p=0.,u=0.,v=0.)";

	bcOption1=", uniform(";

        // we should popup a sibling dialog for parameters

      }
      else if( answer=="parabolic" )
      {
	bcOption1=", parabolic(d=.1,";
        // popup sibling dialog for parameters
      }
      else if( answer=="blasius" )
      {
	bcOption1=", blasius(R=1000.,";
      }
      else if( answer=="jet" )
      {
	bcOption1=", jet(d=.1,r=1.,x=0.,y=0,z=0.,";
      }
      else if( answer=="pressure" )
      {
	bcOption1=", pressure(.1*p+1.*p.n=0.)";
      }
      else if( answer=="user defined" )
      {
	bcOption1=", userDefinedBoundaryData";
      }
      else
      {
        Overture::abort("error");
      }
      if( answer=="uniform" || answer=="parabolic" || answer=="blasius" || answer=="jet" )
      {  // complete the option string
	for( int n=0; n<numberOfComponents; n++ )
	{
	  bcOption1+=sPrintF(buff,"%s=%g",(const char *)componentName[n],defaultState(n));
          if( n<numberOfComponents-1 ) bcOption1+=",";
	}
        bcOption1+=")";
   
      }
      // reset the option2
      bcOption2=""; 
      dialog.getOptionMenu("option2:").setCurrentChoice("no option2");
      
      dialog.setTextLabel("bc command",(const char*)getCommand());
      dialog.getOptionMenu("option:").setCurrentChoice(answer);

    }
    else if( answer=="no option2" ||
	     answer=="oscillate" ||
             answer=="ramp" )
    {
      if( answer=="no option2" )
      {
	bcOption2="";
      }
      else if( answer=="oscillate" )
      {
        bcOption2=", oscillate(t0=.5,omega=1.,a0=0.,a1=1.,u0=0,v0=0.,w0=0.)";
      }
      else if( answer=="ramp" )
      {
        bcOption2=", ramp(ta=0.,tb=1.,ua=0.,ub=1.,va=0.,vb=1.)";
      }
      else
      {
	Overture::abort("error");
      }
      dialog.setTextLabel("bc command",(const char*)getCommand());
      dialog.getOptionMenu("option2:").setCurrentChoice(answer);
    }
    else if( answer=="polynomial extrapolation" || 
             answer=="limited extrapolation" )
    {
      extrapolationOption = answer=="polynomial extrapolation" ? BoundaryConditionParameters::polynomialExtrapolation :
	BoundaryConditionParameters::extrapolateWithLimiter;
      dialog.getOptionMenu("extrap option:").setCurrentChoice((int)extrapolationOption);
      printF(" *** setting extrapolationOption to [%s]\n",(const char*)answer);
    }
    else if( answer=="apply bc command"  )
    {
      aString bcCommand = getCommand();
      gi.outputToCommandFile("* "+bcCommand+"\n");  // output current command as a comment
      setBoundaryConditionValues(bcCommand,originalBoundaryCondition,cg);
 
    }
    else if( len=answer.matches("bc command") )
    {
      aString bcCommand=answer(len+1,answer.length()-1);
      setBoundaryConditionValues(bcCommand,originalBoundaryCondition,cg);
 
    }
    else if( len=answer.matches("default state") )
    {
      answer=answer(len,answer.length()-1);
      inputParameterValues(answer,"default state",defaultState);

      aString state="";
      for( int n=0; n<numberOfComponents; n++ )
      {
	state+=sPrintF(buff,"%s=%g",(const char *)componentName[n],defaultState(n));
	if( n<numberOfComponents-1 ) state+=",";
      }
      dialog.setTextLabel("default state",(const char*)state);
    }
    
    else if( answer=="help" )
    {
      printF("\n"
             " ================================================================================================\n"
             " The objective of this menu is to build one or more `bc command' text strings\n"
             " Each `bc command' text string will assign boundary conditions to one or more boundaries\n"
             " Steps:\n" 
             "   1. Choose a `boundary:' or choose a `bc number' to assign.\n"
             "   2. Choose a 'bc:' such a noSlipWall, inflow etc.\n"
             "   3. If desired choose a primary option from `option:' (see below for more info on options) \n"
             "   4. If desired choose a modifier option from `option2'\n"
             "   5. Choose `apply bc command' to apply the bc or optionally edit \n"
             "      the `bc command string' to change values. After changing the string \n"
             "      hit return to apply the boundary condition.\n"
             );

      printF("The `bc command' text string is of the form  \n"
	     "                                                                             \n"
	     "       <grid name>(side,axis)=<boundary condition name> [,option] [,option] ...\n"
	     "                                                                             \n"
	     " to change the boundary condition on a given side of a given grid, or        \n"
	     "                                                                             \n"
	     "       <grid name>=<boundary condition name>  [,option] [,option] ...          \n"
	     "                                                                             \n"
	     " to change all boundaries on a given grid, or                                \n"
	     "                                                                             \n"
	     "       bcNumber<num>=<boundary condition name>  [,option] [,option] ... \n"
	     "                                                                             \n"
	     " to change all boundaries that *originally* had a boundary condition value of `num'\n"
	     " Here \n"
	     "   <grid name> is the name of the grid, side=0,1 and axis=0,1,2.  <grid name> can also be `all'.\n"
	     "                                                                             \n"
	     " The optional arguments specify data for the boundary conditions:            \n"
	     "    option = `uniform(p=1.,u=1.,...)'       : to specify a uniform inflow profile   \n"
	     "    option = `parabolic(d=2,p=1.,...)'      : to specify a parabolic inflow profile \n"
	     "    option = `blasius(p=1.,u=1.,R=1000)'  : to specify a laminar boundary layer profile \n"
	     "    option = `pressure(.1*p+1.*p.n=0.)'     : pressure boundary condition at outflow\n"
	     "    option = `oscillate(t0=.5,omega=1.,a0=0.,a1=1.,u0=0,v0=0.,w0=0.)'  : oscillating values    \n"
	     "    option = `ramp(ta=0.,tb=1.,ua=0.,ub=1.,va=0.,..)  : ramped inflow parameters    \n" 
	     "    option = `jet(d=.1,r=1.,x=0.,y=0,z=0.,p=1.,...)': to specify a parabolic jet inflow profile \n" 
	     "    option = `userDefinedBoundaryData : choose user defined data  \n"
	     " The pressure outflow boundary condition is a*p + b*p.n = c \n"
	     "  where p=pressure, p.n=normal derivative of p.\n"
	     "  Note that a and b should have the same sign or else the condition is unstable\n"
	     "                                                                               \n"
	     " The oscillate option is used with `uniform' or `parabolic' and takes the form:\n" 
	     "         [u0,v0,w0] + { a0+a1*cos((t-t0)*(2*pi*omega)) }*[ uniform or parabolic profile ]  \n"
	     "                                                                             \n"
	     "The userDefinedBoundaryData option calls the function userDefinedBoundaryValues that\n"
	     "  may be changed to suit your purposes. \n"
             " \n"
	     " Examples: \n"
	     "    square(0,0)=inflowWithVelocityGiven , uniform(p=1.,u=1.)     \n"
	     "    annulus=noSlipWall                         \n"
	     "    all=slipWall                               \n"
	     "    bcNumber2=slipWall                         \n"
	     "    square(0,1)=outflow , pressure(.1*p+1.*p.n=0.) \n"
             "    bcNumber2=inflowWithVelocityGiven, parabolic(d=1.,p=1.,u=1.) , ramp(ta=0.,tb=10.,ua=0.0,ub=1.)\n"
             "    square(0,0)=inflowWithVelocityGiven , parabolic(d=.2,p=1.,u=1.), oscillate(t0=.3,omega=2.5)\n"
             " ================================================================================================\n"
	);
      
    }
    else if( len=answer.matches("order of extrap for interp neighbours") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&orderOfExtrapolationForInterpolationNeighbours);
      orderOfExtrapolationForInterpolationNeighbours=max(1,min(10,orderOfExtrapolationForInterpolationNeighbours));
      printF(" orderOfExtrapolationForInterpolationNeighbours=%i\n",orderOfExtrapolationForInterpolationNeighbours);
      printF(" order=1 is constant extrapolation, order=2 is linear, order=3 is quadratic etc. \n");
      
      dialog.setTextLabel("order of extrap for interp neighbours",sPrintF(line,"%i", orderOfExtrapolationForInterpolationNeighbours)); 
    }
    else if( len=answer.matches("order of extrap for 2nd ghost line") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&orderOfExtrapolationForSecondGhostLine);
      orderOfExtrapolationForSecondGhostLine=max(1,min(10,orderOfExtrapolationForSecondGhostLine));
      printF(" orderOfExtrapolationForSecondGhostLine=%i\n",orderOfExtrapolationForSecondGhostLine);
      printF(" order=1 is constant extrapolation, order=2 is linear, order=3 is quadratic etc. \n");
      dialog.setTextLabel("order of extrap for 2nd ghost line",sPrintF(line,"%i", orderOfExtrapolationForSecondGhostLine)); 
    }
    else if( len=answer.matches("order of extrap for outflow") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&orderOfExtrapolationForOutflow);
      printF(" orderOfExtrapolationForOutflow=%i (-1: use default), order=1 is constant extrapolation, order=2 is linear, order=3 is quadratic etc. \n",
              orderOfExtrapolationForOutflow);
      dialog.setTextLabel("order of extrap for outflow",sPrintF(line,"%i (-1=default)", orderOfExtrapolationForOutflow)); 
    }
    else if( dialog.getTextValue(answer,"moving body pressure BC coefficient","%e",dbase.get<real>("movingBodyPressureCoefficient")) )
    {
      printF(" INFO: Setting the coeff for the special wall BC for moving `light' bodies: p.n + a*p = ..., a=%8.2e\n",
	     dbase.get<real>("movingBodyPressureCoefficient"));
    }
    else if( dialog.getTextValue(answer,"moving body pressure BC","%i",dbase.get<int>("movingBodyPressureBC")) )
    {
      printF(" INFO: Turning on the special wall BC for moving `light' bodies. Value=%i. "
             "(1=apply to moving walls, 2=apply to all walls\n",dbase.get<int>("movingBodyPressureBC"));
    }
    else if( dialog.getToggleValue(answer,"apply interface conditions",
				   dbase.get<int>("applyInterfaceBoundaryConditions")) ){ }
    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	cout << "Unknown response=[" << answer << "]\n";
	gi.stopReadingCommandFile();
      }
       
    }

  }

  delete [] bcNameList;
  delete [] bcNameLabel;

//  delete [] pdeParametersMenu;

//  updatePDEparameters();  // update parameters such as ReynoldsNumber, MachNumber, ... to be consistent.


  if( !executeCommand  )
  {
    if( !squaresWerePlotted ) gi.setPlotTheColouredSquares(false);
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

 return returnValue;
}


