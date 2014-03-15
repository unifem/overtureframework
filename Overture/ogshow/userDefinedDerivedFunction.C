#include "DerivedFunctions.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"

// save parameters here
static real velocityCylindricalParameters[6];
static real stressCylindricalParameters[6];

#define FOR_3D(i1,i2,i3,I1,I2,I3)					\
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(); \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++)					\
  for(i2=I2Base; i2<=I2Bound; i2++)					\
    for(i1=I1Base; i1<=I1Bound; i1++)


//\begin{>>DerivedFunctionsInclude.tex}{\subsection{setupUserDefinedDerivedFunction}} 
int DerivedFunctions::
setupUserDefinedDerivedFunction(GenericGraphicsInterface & gi, 
                                int numberOfComponents, 
				aString *componentNames )
//==============================================================================================
// /Description:
//    User defined derived functions. This function is used to setup and define a derived function.
// The function getUserDefinedDerivedFunction is called to actually evaluate the derived function.
// Choose the "user defined" option from the DerivedFunctions::update options to have this routine
// called.
// /Notes:
//  \begin{itemize}
//    \item To to add a new user defined derived-function edit this file (Ogshow/userDefinedDerivedFunction.C)
//          and rebuild the plotStuff executable using the new version. This can be done by rebuilding the
//           Overture library in place, or by copying the files plotStuffDriver.C, plotStuff.C and 
//           userDefinedDerivedFunction.C to a new location and building a separate version of plotStuff.
//  \end{itemize}
//
// /Return values: 0=success, non-zero=failure.
//\end{DerivedFunctionsInclude.tex}
//==============================================================================================

{
  assert( showFileReader!=NULL );
  ShowFileReader & reader = *showFileReader;

  aString menu[]=
    {
      "!User Defined Derived Functions",
      "gas mass fraction",
      "mixture density",
      "mixture schlieren",
      "scaled schlieren",
      "mixture temperature",
      "reaction rate",
      "masked progress",
      "material interface",
      "stressNorm",
      "shearStress",
      "speed(FOS)",
      "stressNorm(SVK)",
      "velocity (cylindrical coordinates)",
      "stress (cylindrical coordinates)",
      "exit",
      ""
    };
  aString answer;
  
  // *** make a list of component names for later use ***
  aString *cNames = new aString [numberOfComponents+1];
  int n;
  for( n=0; n<numberOfComponents; n++ )
  { 
    cNames[n]=componentNames[n]; 
  }
  n=numberOfComponents;
  cNames[n]="";            // null terminated list

  // list of species 
  int numberOfSpecies=0;
  reader.getGeneralParameter("numberOfSpecies",numberOfSpecies );
  assert( numberOfSpecies>=0 );
  
  int sc=-1;
  if( numberOfSpecies>=0 )
    reader.getGeneralParameter("speciesComponent",sc);
  else
    sc=0;

  // *** make a list of species for later use ***
  aString *speciesNames = new aString [numberOfSpecies+1];
  for( n=0; n<numberOfSpecies; n++ )
  { 
    speciesNames[n]=componentNames[sc+n]; 
  }
  n=numberOfSpecies;
  speciesNames[n]=""; // null terminated list

  for( ;; )
  {
    int choice = gi.getMenuItem(menu,answer,"choose an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="gas mass fraction" )
    {
      add( userDefined,"gas mass fraction" );
      printF("Gas mass fraction was chosen.\n");
    }
    else if( answer=="mixture density" )
    {
      add( userDefined,"mixture density" );
      printF("Mixture density was chosen.\n");
    }
    else if( answer=="mixture schlieren" )
    {
      add( userDefined,"mixture schlieren" );
      printF("Mixture schlieren was chosen.\n");
    }
    else if( answer=="scaled schlieren" )
    {
      add( userDefined,"scaled schlieren" );
      printF("Scaled schlieren was chosen.\n");
    }
    else if( answer=="mixture temperature" )
    {
      add( userDefined,"mixture temperature" );
      printF("Mixture temperature was chosen.\n");
    }
    else if( answer=="reaction rate" )
    {
      int species=0;

      // we query to determine the species for which the reaction rate is required
      aString answer2;
      int s = gi.getMenuItem(speciesNames,answer2,"reaction rate for which species?");
      if( s>=0 && s<numberOfSpecies )
      {
	species=s+sc;
      }
      else
      {
	cout << "Unknown response: [" << answer << "]\n";
	gi.stopReadingCommandFile();
      }

      // save a user defined function with a given name ("reaction rate") 
      //    and an optional integer identifier (species)
      add( userDefined,"reaction rate",species );

      printF("Species %s was chosen (component number %i).\n",(const char*)componentNames[species],species);

    }
    else if( answer=="masked progress" )
    {
      if( numberOfSpecies<2 )
      {
	cout << "Need two species variables [lambda,mu]\n";
	gi.stopReadingCommandFile();
      }
      else
      {
	add( userDefined,"masked progress" );
	printF("Masked progress was chosen.\n");
      }
    }
    else if( answer=="material interface" )
    {
      if( numberOfSpecies<2 )
      {
	cout << "Need two species variables [lambda,mu]\n";
	gi.stopReadingCommandFile();
      }
      else
      {
	add( userDefined,"material interface" );
	printF("Material interface was chosen.\n");
      }
    }
    else if( answer=="stressNorm" )
    {
      add( userDefined,"stressNorm" );
      printF("Norm of the stress tensor was added.\n");
    }
    else if( answer=="shearStress" )
    {
      add( userDefined,"shearStress" );
      printF("The shear stress was added.\n");
    }
    else if( answer=="speed(FOS)" )
    {
      add( userDefined,"speed(FOS)" );
      printF("The speed (for FOS) was added.\n");
    }
    else if( answer=="stressNorm(SVK)" )
    {
      add( userDefined,"stressNorm(SVK)" );
      printF("The stress norm (for SVK) was added.\n");
    }
    else if( answer=="velocity (cylindrical coordinates)" )
    {
      real x0=0., y0=0., z0=0., d0=0., d1=0., d2=1.;
      printF("Cylindrical coordinates are defined by a point on the axis (x0,y0,z0) and a vector that points in the\n"
             " direction of the axis, (d0,d1,d2). Defaults: (x0,y0,z0)=(0,0,0) and (d0,d1,d2)=(0,0,1)\n");
      gi.inputString(answer,"Enter x0,y0,z0, d0,d1,d2");
      sScanF(answer,"%e %e %e %e %e %e",&x0,&y0,&z0, &d0,&d1,&d2);

      printF("The velocity in cylindrical coordinates was added, (x0,y0,z0)=(%g,%g,%g), (d0,d1,d2)=(%g,%g,%g)\n",
	     x0,y0,z0,d0,d1,d2);

      add( userDefined,"vr" );
      add( userDefined,"vTheta" );

      velocityCylindricalParameters[0]=x0;
      velocityCylindricalParameters[1]=y0;
      velocityCylindricalParameters[2]=z0;
      
      velocityCylindricalParameters[3]=d0;
      velocityCylindricalParameters[4]=d1;
      velocityCylindricalParameters[5]=d2;
    }
    else if( answer=="stress (cylindrical coordinates)" )
    {
      real x0=0., y0=0., z0=0., d0=0., d1=0., d2=1.;
      printF("Cylindrical coordinates are defined by a point on the axis (x0,y0,z0) and a vector that points in the\n"
             " direction of the axis, (d0,d1,d2). Defaults: (x0,y0,z0)=(0,0,0) and (d0,d1,d2)=(0,0,1)\n");
      gi.inputString(answer,"Enter x0,y0,z0, d0,d1,d2");
      sScanF(answer,"%e %e %e %e %e %e",&x0,&y0,&z0, &d0,&d1,&d2);

      stressCylindricalParameters[0]=x0;
      stressCylindricalParameters[1]=y0;
      stressCylindricalParameters[2]=z0;
      
      stressCylindricalParameters[3]=d0;
      stressCylindricalParameters[4]=d1;
      stressCylindricalParameters[5]=d2;

      add( userDefined,"rSr" );
      add( userDefined,"rSt" );
      add( userDefined,"tSt" );

      printF("The stress in cylindrical coordinates was added, (x0,y0,z0)=(%g,%g,%g), (d0,d1,d2)=(%g,%g,%g)\n",
	     x0,y0,z0,d0,d1,d2);
    }
    else
    {
      printF("ERROR: unknown answer=%s\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  
  delete [] cNames;
  delete [] speciesNames;
  return 0;
}


//\begin{>>DerivedFunctionsInclude.tex}{\subsection{getUserDefinedDerivedFunction}} 
int DerivedFunctions:: 
getUserDefinedDerivedFunction( int index, 
                               int indexOut, 
                               const aString & name, 
                               const int numberOfComponents,
                               realCompositeGridFunction & uIn,
                               realCompositeGridFunction & uOut,
                               bool & interpolationRequired )
//==============================================================================================
// /Description:
//    Assign a user defined derived function. 
//
// /index (input): the index of this derived function (derived functions have index=0,1,2,...)
// /indexOut (input): fill in this component of uOut (indexOut=index+numberOfComponents)
// /name (input): the name associated with the derived function
// /uIn : the solution read from the show file
// /uOut : fill the derived function into this composite grid function
// /Return values: 0=success, non-zero=failure.
//\end{DerivedFunctionsInclude.tex}
//==============================================================================================
{
  assert( showFileReader!=NULL );
  ShowFileReader & reader = *showFileReader;

  // This data-base object holds info about the current frame
  HDF_DataBase & db = *showFileReader->getFrame();
  real t=0.; 
  db.get(t,"t");  // here is the current time (for time dependent computations)

  printF("*** DerivedFunctions::getUserDefinedDerivedFunction: t=%9.3e\n",t);

  CompositeGrid & cg = *uIn.getCompositeGrid();
  const int numberOfDimensions = cg.numberOfDimensions();
  
  const int nc = numberOfComponents;

  Index I1,I2,I3;

  interpolationRequired=false;  // set to true if the function should be interpolated before plotting

  if( name=="reaction rate" ) // this should match the "name" given in the "add" statement in the setup routine
  {
    // PDE parameters are saved in the OverBlown file OB_Parameters.C -- function saveParametersToShowFile()

    // get component numbers, e.g. density is uIn(I1,I2,I3,rc)
    int rc=-1, tc=-1, pc=-1, uc=-1, vc=-1, wc=-1, sc=-1;
    reader.getGeneralParameter("densityComponent",rc);   
    reader.getGeneralParameter("speciesComponent",sc);
    reader.getGeneralParameter("temperatureComponent",tc);
    reader.getGeneralParameter("pressureComponent",pc);
    reader.getGeneralParameter("uComponent",uc);
    reader.getGeneralParameter("vComponent",vc);
    reader.getGeneralParameter("wComponent",wc);

    bool ok;

    // reactionName: 
    //    "noReactions" 
    //    "onestep"
    //    "branching"
    //    "ignitionAndGrowth" 
    //    "oneEquationMixtureFraction"
    //    "twoEquationMixtureFractionAndExtentOfReaction"
    //    "chemkinReaction" 
    //    "unknown reactionType"

    aString reactionName;
    ok=reader.getGeneralParameter("reactionType",reactionName);  // ok==false if not found

    // For oneStep reaction:
    real heatRelease=0.,reciprocalActivationEnergy=0.,rateConstant=0.;
    ok=reader.getGeneralParameter("heatRelease",heatRelease);  // ok==false if not found
    ok=reader.getGeneralParameter("reciprocalActivationEnergy",reciprocalActivationEnergy);
    ok=reader.getGeneralParameter("rateConstant",rateConstant);

    // for chain branching
    real reciprocalActivationEnergyI, reciprocalActivationEnergyB, crossOverTemperatureI, 
      crossOverTemperatureB, absorbedEnergy;
    
    reader.getGeneralParameter("reciprocalActivationEnergyI",reciprocalActivationEnergyI);
    reader.getGeneralParameter("reciprocalActivationEnergyB",reciprocalActivationEnergyB);
    reader.getGeneralParameter("crossOverTemperatureI",crossOverTemperatureI);
    reader.getGeneralParameter("crossOverTemperatureB",crossOverTemperatureB);
    reader.getGeneralParameter("absorbedEnergy",absorbedEnergy);

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);
        
      int species=derived(index,1);  // here is the integer parameter we saved with the derived function
      assert( species>=0 && species<numberOfComponents );
      // density:  vIn(I1,I2,I3,rc); 
      // pressure: vIn(I1,I2,I3,pc);

      // ak1=reaction rate for chain-initiation
      // ak2=reaction rate for chain-branching
      //     ak1=pr(1)*dexp(-at(1)/temp)
      //     ak2=pr(2)*dexp(-at(2)/temp)

      vOut(I1,I2,I3,indexOut) = vIn(I1,I2,I3,species);   // do this for now

    }
  }
  else if( name=="gas mass fraction" )
  {
    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);

      vOut(I1,I2,I3,indexOut) = (1.-vIn(I1,I2,I3,8))*vIn(I1,I2,I3,4)/((1.-vIn(I1,I2,I3,8))*vIn(I1,I2,I3,4)+vIn(I1,I2,I3,8)*vIn(I1,I2,I3,0));
    }
  }
  else if( name=="mixture density" )
  {
    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);

      vOut(I1,I2,I3,indexOut) = (1.-vIn(I1,I2,I3,8))*vIn(I1,I2,I3,4)+vIn(I1,I2,I3,8)*vIn(I1,I2,I3,0);
    }
  }
  else if( name=="mixture schlieren" )
  {
    // we should interpolate this quantity since we can't compute it directly at interpolation points:
    interpolationRequired=false;
    real sMin=REAL_MAX;
    real sMax=0.;

    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators cgop(gc);

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      MappedGridOperators & op = cgop[grid];
    
#ifdef USE_PPP
      realSerialArray vIn;  getLocalArrayWithGhostBoundaries(uIn[grid],vIn);
      realSerialArray vOut; getLocalArrayWithGhostBoundaries(uOut[grid],vOut);
      intSerialArray mask;  getLocalArrayWithGhostBoundaries(mg.mask(),mask);
#else
      const realSerialArray & vIn = uIn[grid];
      const realSerialArray & vOut = uOut[grid];
      const intSerialArray & mask = mg.mask();
#endif

      getIndex(mg.dimension(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,I1,I2,I3);
      if( !ok ) continue; // no points on this processor

// assuming 2d flow
      realSerialArray rsx(I1,I2,I3), rsy(I1,I2,I3), asx(I1,I2,I3), asy(I1,I2,I3), rgx(I1,I2,I3), rgy(I1,I2,I3);

      rsx=0.; rsy=0.; asx=0.; asy=0.; rgx=0.; rgy=0.;

      op.derivative(MappedGridOperators::xDerivative,vIn,rsx,I1,I2,I3,0);
      op.derivative(MappedGridOperators::yDerivative,vIn,rsy,I1,I2,I3,0);
      op.derivative(MappedGridOperators::xDerivative,vIn,asx,I1,I2,I3,8);
      op.derivative(MappedGridOperators::yDerivative,vIn,asy,I1,I2,I3,8);
      op.derivative(MappedGridOperators::xDerivative,vIn,rgx,I1,I2,I3,4);
      op.derivative(MappedGridOperators::yDerivative,vIn,rgy,I1,I2,I3,4);

      vOut(I1,I2,I3,indexOut)=sqrt( SQR((1-vIn(I1,I2,I3,8))*rgx(I1,I2,I3)+vIn(I1,I2,I3,8)*rsx(I1,I2,I3)+(vIn(I1,I2,I3,0)-vIn(I1,I2,I3,4))*asx(I1,I2,I3)) + SQR((1-vIn(I1,I2,I3,8))*rgy(I1,I2,I3)+vIn(I1,I2,I3,8)*rsy(I1,I2,I3)+(vIn(I1,I2,I3,0)-vIn(I1,I2,I3,4))*asy(I1,I2,I3)) );

//      where( mask(I1,I2,I3)<=0 )
//        vOut(I1,I2,I3,indexOut)=0.;

//      sMin=min(sMin,min(vOut(I1,I2,I3,indexOut)));
//      sMax=max(sMax,max(vOut(I1,I2,I3,indexOut)));

      Index J1,J2,J3;
      getIndex(mg.indexRange(),J1,J2,J3);
      ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,J1,J2,J3);
      if( ok )
      {
        where( mask(J1,J2,J3)>0 )
        {
          sMin=min(sMin,min(vOut(J1,J2,J3,indexOut)));
          sMax=max(sMax,max(vOut(J1,J2,J3,indexOut)));
        }
      }

    }
    printF("sMin sMax = %e %e\n",sMin,sMax);

//    #ifdef USE_PPP
//     sMin=ParallelUtility::getMinValue(sMin);
//     sMax=ParallelUtility::getMaxValue(sMax);
//    #endif


//    sMax=300.;
//    sMin=0.;

    if( fabs(sMax-sMin)>0. )
    {
      real exposure=1.;
      real amplification=15.;
      for( int grid=0; grid<cg.numberOfGrids(); grid++ )
      {
#ifdef USE_PPP
	realSerialArray vOut;  getLocalArrayWithGhostBoundaries(uOut[grid],vOut);
#else
	realSerialArray & vOut = uOut[grid];
#endif

        getIndex(cg[grid].dimension(),I1,I2,I3);
        bool ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,I1,I2,I3);
        if( ok )
        {
          vOut(I1,I2,I3,indexOut)=(vOut(I1,I2,I3,indexOut)-sMin)*(1./(sMax-sMin));
          vOut(I1,I2,I3,indexOut)=exposure*exp(-amplification*vOut(I1,I2,I3,indexOut));
        }
      }
    }

  }
  else if( name=="scaled schlieren" )
  {
    // we should interpolate this quantity since we can't compute it directly at interpolation points:
    interpolationRequired=false;
    real sMin=REAL_MAX;
    real sMax=0.;

    real rhoSolid=2297.;
    real rhoGas=1.4;
    real gamSolid=5.;
    real gamGas=1.4;

    real mu1Solid=1./(gamSolid-1.);
    real mu1Gas=1./(gamGas-1.);

    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators cgop(gc);

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      MappedGridOperators & op = cgop[grid];
    
#ifdef USE_PPP
      realSerialArray vIn;  getLocalArrayWithGhostBoundaries(uIn[grid],vIn);
      realSerialArray vOut; getLocalArrayWithGhostBoundaries(uOut[grid],vOut);
      intSerialArray mask;  getLocalArrayWithGhostBoundaries(mg.mask(),mask);
#else
      const realSerialArray & vIn = uIn[grid];
      const realSerialArray & vOut = uOut[grid];
      const intSerialArray & mask = mg.mask();
#endif

      getIndex(mg.dimension(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,I1,I2,I3);
      if( !ok ) continue; // no points on this processor

// assuming 2d flow
      realSerialArray rx(I1,I2,I3), ry(I1,I2,I3), lambda(I1,I2,I3), scale(I1,I2,I3);

      rx=0.; ry=0.; lambda=0.; scale=1.;

      op.derivative(MappedGridOperators::xDerivative,vIn,rx,I1,I2,I3,0);
      op.derivative(MappedGridOperators::yDerivative,vIn,ry,I1,I2,I3,0);

      lambda(I1,I2,I3)=(vIn(I1,I2,I3,4)-mu1Gas)*(1./(mu1Solid-mu1Gas));
      scale(I1,I2,I3)=rhoSolid*lambda(I1,I2,I3)+rhoGas*(1-lambda(I1,I2,I3));

      vOut(I1,I2,I3,indexOut)=sqrt(SQR(rx(I1,I2,I3))+SQR(ry(I1,I2,I3)))/scale(I1,I2,I3);

//      where( mask(I1,I2,I3)<=0 )
//        vOut(I1,I2,I3,indexOut)=0.;

//      sMin=min(sMin,min(vOut(I1,I2,I3,indexOut)));
//      sMax=max(sMax,max(vOut(I1,I2,I3,indexOut)));

      Index J1,J2,J3;
      getIndex(mg.indexRange(),J1,J2,J3);
      ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,J1,J2,J3);
      if( ok )
      {
        where( mask(J1,J2,J3)>0 )
        {
          sMin=min(sMin,min(vOut(J1,J2,J3,indexOut)));
          sMax=max(sMax,max(vOut(J1,J2,J3,indexOut)));
        }
      }

    }
    printF("sMin sMax = %e %e\n",sMin,sMax);

//    #ifdef USE_PPP
//     sMin=ParallelUtility::getMinValue(sMin);
//     sMax=ParallelUtility::getMaxValue(sMax);
//    #endif


//    sMax=300.;
//    sMin=0.;

    if( fabs(sMax-sMin)>0. )
    {
      real exposure=1.;
      real amplification=45.;
      for( int grid=0; grid<cg.numberOfGrids(); grid++ )
      {
#ifdef USE_PPP
	realSerialArray vOut;  getLocalArrayWithGhostBoundaries(uOut[grid],vOut);
#else
	realSerialArray & vOut = uOut[grid];
#endif

        getIndex(cg[grid].dimension(),I1,I2,I3);
        bool ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,I1,I2,I3);
        if( ok )
        {
          vOut(I1,I2,I3,indexOut)=(vOut(I1,I2,I3,indexOut)-sMin)*(1./(sMax-sMin));
          vOut(I1,I2,I3,indexOut)=exposure*exp(-amplification*vOut(I1,I2,I3,indexOut));
        }
      }
    }

  }
  else if( name=="mixture temperature" )
  {
    real p0=1.;
    real rhoGas=1.4;
    real cvSolid=1.5;
    real cvGas=0.717625;
    real gamSolid=5.;
    real gamGas=1.4;

    real mu1Solid=1./(gamSolid-1.);
    real mu1Gas=1./(gamGas-1.);
    real T0=p0*mu1Gas/(rhoGas*cvGas);

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);

      vOut(I1,I2,I3,indexOut) = (vIn(I1,I2,I3,4)-mu1Gas)*(1./(mu1Solid-mu1Gas));  // lambda
      vOut(I1,I2,I3,indexOut) = cvSolid*vOut(I1,I2,I3,indexOut)+cvGas*(1.-vOut(I1,I2,I3,indexOut));  // mixture Cv
      vOut(I1,I2,I3,indexOut) = (vIn(I1,I2,I3,3)*vIn(I1,I2,I3,4)+vIn(I1,I2,I3,5)/vIn(I1,I2,I3,0))/(T0*vOut(I1,I2,I3,indexOut));  // mixture temperature
    }
  }
  else if( name=="masked progress" )
  {
    int sc;
    real cutOff = 0.5;
    double notANumber=10.0;

    reader.getGeneralParameter("speciesComponent",sc);
    if( sc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: speciesComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }
    
    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);

      where( vIn(I1,I2,I3,sc) >= cutOff )
      {
	vOut(I1,I2,I3,indexOut) = vIn(I1,I2,I3,sc+1);
      }
      where( vIn(I1,I2,I3,sc) < cutOff )
      {
	vOut(I1,I2,I3,indexOut) = notANumber;
      }
    }
  }
  else if( name=="material interface" )
  {
    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators op(gc);
    uIn.setOperators(op);
    int sc;

    reader.getGeneralParameter("speciesComponent",sc);
    if( sc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: speciesComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);
      vOut(I1,I2,I3,indexOut) = SQRT(SQR(vIn.x()(I1,I2,I3,sc))+SQR(vIn.y()(I1,I2,I3,sc)));
      vOut(I1,I2,I3,indexOut) = 1.0/(1.0+vOut(I1,I2,I3,indexOut));
    }
  }
  else if( name=="stressNorm" )
  {
    // we should interpolate this quantity since we can't compute it directly at interpolation points:
    interpolationRequired=true;  

    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators cgop(gc);
    int sc;

    int uc=-1, vc=-1, wc=-1;
    reader.getGeneralParameter("uComponent",uc);
    reader.getGeneralParameter("vComponent",vc);
    reader.getGeneralParameter("wComponent",wc);

    if( uc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: uComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }
    if( vc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: vComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }
    if( cg.numberOfDimensions()==3 && wc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: wComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }

    real nu=0.;
    reader.getGeneralParameter("nu",nu);
    

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      MappedGridOperators & op = cgop[grid];
    
#ifdef USE_PPP
      realSerialArray vIn;  getLocalArrayWithGhostBoundaries(uIn[grid],vIn);
      realSerialArray vOut; getLocalArrayWithGhostBoundaries(uOut[grid],vOut);
#else
      const realSerialArray & vIn = uIn[grid];
      const realSerialArray & vOut = uOut[grid];
#endif

      getIndex(mg.dimension(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,I1,I2,I3);
      if( !ok ) continue; // no points on this processor

      if( mg.numberOfDimensions()==2 )
      {
        realSerialArray ux(I1,I2,I3), uy(I1,I2,I3), vx(I1,I2,I3), vy(I1,I2,I3);

        op.derivative(MappedGridOperators::xDerivative,vIn,ux,I1,I2,I3,uc);	
        op.derivative(MappedGridOperators::yDerivative,vIn,uy,I1,I2,I3,uc);	
        op.derivative(MappedGridOperators::xDerivative,vIn,vx,I1,I2,I3,vc);	
        op.derivative(MappedGridOperators::yDerivative,vIn,vy,I1,I2,I3,vc);	

        // check this: norm of the INS stress tensor: 
        vOut(I1,I2,I3,indexOut)= nu*sqrt( SQR(2.*ux) + 2.*SQR(uy+vx) + SQR(2.*vy) );

      }
      else if( mg.numberOfDimensions()==3 )
      {
        realSerialArray ux(I1,I2,I3), uy(I1,I2,I3), uz(I1,I2,I3);
        realSerialArray vx(I1,I2,I3), vy(I1,I2,I3), vz(I1,I2,I3);
        realSerialArray wx(I1,I2,I3), wy(I1,I2,I3), wz(I1,I2,I3);

        op.derivative(MappedGridOperators::xDerivative,vIn,ux,I1,I2,I3,uc);	
        op.derivative(MappedGridOperators::yDerivative,vIn,uy,I1,I2,I3,uc);	
        op.derivative(MappedGridOperators::zDerivative,vIn,uz,I1,I2,I3,uc);	

        op.derivative(MappedGridOperators::xDerivative,vIn,vx,I1,I2,I3,vc);	
        op.derivative(MappedGridOperators::yDerivative,vIn,vy,I1,I2,I3,vc);	
        op.derivative(MappedGridOperators::zDerivative,vIn,vz,I1,I2,I3,vc);	

        op.derivative(MappedGridOperators::xDerivative,vIn,wx,I1,I2,I3,wc);	
        op.derivative(MappedGridOperators::yDerivative,vIn,wy,I1,I2,I3,wc);	
        op.derivative(MappedGridOperators::zDerivative,vIn,wz,I1,I2,I3,wc);	


        // check this: norm of the INS stress tensor: 
        vOut(I1,I2,I3,indexOut)= nu*sqrt( SQR(2.*ux) + 2.*SQR(uy+vx) + 2.*SQR(uz+wx) + 
                                          SQR(2.*vy) + 2.*SQR(vz+wy) + 
                                          SQR(wz) );
      }
      else
      {
        realSerialArray ux(I1,I2,I3);
        op.derivative(MappedGridOperators::xDerivative,vIn,ux,I1,I2,I3,uc);	

        // check this: norm of the INS stress tensor: 
        vOut(I1,I2,I3,indexOut)= nu*sqrt( SQR(2.*ux) );
      }
    }
  }
  else if( name=="shearStress" )
  {
    // --- compute the shear stress on boundaries ---

    // we should interpolate this quantity since we can't compute it directly at interpolation points:
    interpolationRequired=true;  

    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators cgop(gc);
    int sc;

    int uc=-1, vc=-1, wc=-1;
    reader.getGeneralParameter("uComponent",uc);
    reader.getGeneralParameter("vComponent",vc);
    reader.getGeneralParameter("wComponent",wc);
    if( uc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: uComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }
    if( vc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: vComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }
    if( cg.numberOfDimensions()==3 && wc<0 )
    {
      printF("getUserDefinedDerivedFunction:%s:ERROR: wComponent is not in the show file!\n",
	     (const char*)name);
      return 1;
    }

    real nu=0.;
    reader.getGeneralParameter("nu",nu);
    

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      MappedGridOperators & op = cgop[grid];
      mg.update(MappedGrid::THEvertexBoundaryNormal); // make sure the normals are there
    
#ifdef USE_PPP
      realSerialArray vIn;  getLocalArrayWithGhostBoundaries(uIn[grid],vIn);
      realSerialArray vOut; getLocalArrayWithGhostBoundaries(uOut[grid],vOut);
#else
      const realSerialArray & vIn = uIn[grid];
      const realSerialArray & vOut = uOut[grid];
#endif

      Range all;
      vOut(all,all,all,indexOut)=0.;
	

      for( int side=0; side<=1; side++ )for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	if( mg.boundaryCondition(side,axis)>0 )
	{
          // --- this is a physical boundary ---

#ifdef USE_PPP
	  const realSerialArray & normal  = mg.vertexBoundaryNormalArray(side,axis);
#else
	  const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
#endif

	  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);

	  bool ok = ParallelUtility::getLocalArrayBounds(uOut[grid],vOut,I1,I2,I3);
	  if( !ok ) continue; // no points on this processor

	  if( mg.numberOfDimensions()==2 )
	  {
	    realSerialArray ux(I1,I2,I3), uy(I1,I2,I3), vx(I1,I2,I3), vy(I1,I2,I3);

	    op.derivative(MappedGridOperators::xDerivative,vIn,ux,I1,I2,I3,uc);	
	    op.derivative(MappedGridOperators::yDerivative,vIn,uy,I1,I2,I3,uc);	
	    op.derivative(MappedGridOperators::xDerivative,vIn,vx,I1,I2,I3,vc);	
	    op.derivative(MappedGridOperators::yDerivative,vIn,vy,I1,I2,I3,vc);	

            // The normal traction vector is n.sigma 
            //  T_n = ( n1*s_11 + n2*s_12 , n1*s_21 + n2*s_22 )
            //      = force on the boundary
            //  shear-stress = T_n - (n.T_n)n    (subtract off normal component of the traction)
            //              
            realSerialArray t1(I1,I2,I3), t2(I1,I2,I3), nDotT(I1,I2,I3);
            // no need to include the pressure :
            t1 = normal(I1,I2,I3,0)*( 2.*ux ) + normal(I1,I2,I3,1)*( uy+vx );
            t2 = normal(I1,I2,I3,0)*( uy+vx ) + normal(I1,I2,I3,1)*( 2.*vy );
	    
            nDotT= normal(I1,I2,I3,0)*t1 + normal(I1,I2,I3,1)*t2;

            t1 -= nDotT*normal(I1,I2,I3,0);
            t2 -= nDotT*normal(I1,I2,I3,1);

	    // norm of the shear stress vector
	    vOut(I1,I2,I3,indexOut)= nu*sqrt( SQR(t1) + SQR(t2) );

	  }
	  else if( mg.numberOfDimensions()==3 )
	  {
	    realSerialArray ux(I1,I2,I3), uy(I1,I2,I3), uz(I1,I2,I3);
	    realSerialArray vx(I1,I2,I3), vy(I1,I2,I3), vz(I1,I2,I3);
	    realSerialArray wx(I1,I2,I3), wy(I1,I2,I3), wz(I1,I2,I3);

	    op.derivative(MappedGridOperators::xDerivative,vIn,ux,I1,I2,I3,uc);	
	    op.derivative(MappedGridOperators::yDerivative,vIn,uy,I1,I2,I3,uc);	
	    op.derivative(MappedGridOperators::zDerivative,vIn,uz,I1,I2,I3,uc);	

	    op.derivative(MappedGridOperators::xDerivative,vIn,vx,I1,I2,I3,vc);	
	    op.derivative(MappedGridOperators::yDerivative,vIn,vy,I1,I2,I3,vc);	
	    op.derivative(MappedGridOperators::zDerivative,vIn,vz,I1,I2,I3,vc);	

	    op.derivative(MappedGridOperators::xDerivative,vIn,wx,I1,I2,I3,wc);	
	    op.derivative(MappedGridOperators::yDerivative,vIn,wy,I1,I2,I3,wc);	
	    op.derivative(MappedGridOperators::zDerivative,vIn,wz,I1,I2,I3,wc);	


            // The normal stress is n.sigma 
            //  normal-stress = ( n1*s_11 + n2*s_12 + n3*s_13, n1*s_21 + n2*s_22 + n3*s_23,n1*s_31 + n2*s_32 + n3*s_33)

            // The normal traction vector is n.sigma 
            //  T_n = ( n1*s_11 + n2*s_12 + n3*s_13, n1*s_21 + n2*s_22 + n3*s_23,n1*s_31 + n2*s_32 + n3*s_33)
            //      = force on the boundary
            //  shear-stress = T_n - (n.T_n)n    (subtract off normal component of the traction)
            //              
            realSerialArray t1(I1,I2,I3), t2(I1,I2,I3), t3(I1,I2,I3), nDotT(I1,I2,I3);
            // no need to include the pressure :
            t1 = normal(I1,I2,I3,0)*( 2.*ux ) + normal(I1,I2,I3,1)*( uy+vx ) + normal(I1,I2,I3,2)*( uz+wx );
            t2 = normal(I1,I2,I3,0)*( uy+vx ) + normal(I1,I2,I3,1)*( 2.*vy ) + normal(I1,I2,I3,2)*( vz+wy );
            t3 = normal(I1,I2,I3,0)*( uz+wx ) + normal(I1,I2,I3,1)*( vz+wy ) + normal(I1,I2,I3,2)*( 2.*wz );
	    
            nDotT= normal(I1,I2,I3,0)*t1 + normal(I1,I2,I3,1)*t2 + normal(I1,I2,I3,2)*t3;

            t1 -= nDotT*normal(I1,I2,I3,0);
            t2 -= nDotT*normal(I1,I2,I3,1);
            t3 -= nDotT*normal(I1,I2,I3,2);

	    // norm of the shear stress vector
	    vOut(I1,I2,I3,indexOut)= nu*sqrt( SQR(t1) + SQR(t2) + SQR(t3) );
	  }
	  else
	  {
	    vOut(I1,I2,I3,indexOut)= 0.;
	  }
	}
      } // end for side, for axis 
      
    }
  }
  else if( name=="speed(FOS)" )
  {
    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators op(gc);
    uIn.setOperators(op);

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);
      vOut(I1,I2,I3,indexOut) = SQRT(SQR(vIn(I1,I2,I3,0))+SQR(vIn(I1,I2,I3,1)));
    }
  }
  else if( name=="stressNorm(SVK)" )
  {
    CompositeGrid & gc = *uIn.getCompositeGrid();
    CompositeGridOperators op(gc);
    uIn.setOperators(op);

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);
      vOut(I1,I2,I3,indexOut) = SQRT(SQR(vIn(I1,I2,I3,2))+SQR(vIn(I1,I2,I3,3))+SQR(vIn(I1,I2,I3,4))+SQR(vIn(I1,I2,I3,5)));
    }
  }
  else if( name=="vr" || name=="vTheta" || 
           name=="rSr" || name=="rSt" || name=="tSt" )
  {
    const int option = ( name=="vr" ? 0 :
                         name=="vTheta" ? 1 :
                         name=="rSr" ? 2 :
                         name=="rSt" ? 3 : 4 );

    real x0[3], d[3];
    if( option==0 || option==1 )
    {
      x0[0] = velocityCylindricalParameters[0];
      x0[1] = velocityCylindricalParameters[1];
      x0[2] = velocityCylindricalParameters[2];
    			   
      d[0] = velocityCylindricalParameters[3];
      d[1] = velocityCylindricalParameters[4];
      d[2] = velocityCylindricalParameters[5];
    }
    else
    {
      x0[0] = stressCylindricalParameters[0];
      x0[1] = stressCylindricalParameters[1];
      x0[2] = stressCylindricalParameters[2];
    			   
      d[0] = stressCylindricalParameters[3];
      d[1] = stressCylindricalParameters[4];
      d[2] = stressCylindricalParameters[5];
    }
    
    int v1c,v2c,v3c;
    getVelocityComponents( v1c,v2c,v3c );
    
    int s11c, s12c, s13c, s21c,s22c,s23c, s31c,s32c,s33c;
    getStressComponents( s11c, s12c, s13c,
			 s21c, s22c, s23c,
			 s31c, s32c, s33c );


    printF("INFO: plottting %s: velocity components=[%i,%i,%i], stress components s11=%i s12=%i s13=%i, s21=%i s22=%i s23=%i s31=%i s32=%i s33=%i\n",
	   (const char*)name, v1c,v2c,v3c, s11c,s12c,s13c, s21c,s22c,s33c, s31c,s32c,s33c);
      

    for( int grid=0; grid<cg.numberOfGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realMappedGridFunction & vIn = uIn[grid];
      realMappedGridFunction & vOut = uOut[grid];

      getIndex(mg.dimension(),I1,I2,I3);
      mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
      const realArray & x = mg.vertex();

      // rHat : unit radial vector,
      // tHat : unit "theta" vector 
      real rHat[3], tHat[3], rHatNorm;
      int i1,i2,i3;
      const real rEps =REAL_MIN*100.;
      if( numberOfDimensions==2 )
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    rHat[axis] = x(i1,i2,i3,axis)-x0[axis];
	  rHatNorm = sqrt( SQR(rHat[0]) + SQR(rHat[1]) );

	  rHatNorm= 1./max(rHatNorm,rEps);
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	    rHat[axis] *= rHatNorm;
          // Here is the tangent vector:
          tHat[0]=-rHat[1];
	  tHat[1]= rHat[0];

	  if( option==0 )
	  {
            // vr: 
	    vOut(i1,i2,i3,indexOut) = rHat[0]*vIn(i1,i2,i3,v1c) + rHat[1]*vIn(i1,i2,i3,v2c);   
	  }
	  else if( option==1 )
	  {
            // vTheta: 
            vOut(i1,i2,i3,indexOut) = tHat[0]*vIn(i1,i2,i3,v1c) + tHat[1]*vIn(i1,i2,i3,v2c);  
	  }
	  else if( option==2 )
	  {
             // rhat. S . rhat 
	    vOut(i1,i2,i3,indexOut) = ( rHat[0]*rHat[0]*vIn(i1,i2,i3,s11c) + 
					rHat[0]*rHat[1]*(vIn(i1,i2,i3,s12c)+vIn(i1,i2,i3,s21c)) +
					rHat[1]*rHat[1]*vIn(i1,i2,i3,s22c) );
	  }
	  else if( option==3 )
	  {
             // rhat. S . that 
	    vOut(i1,i2,i3,indexOut) = ( rHat[0]*tHat[0]*vIn(i1,i2,i3,s11c) + 
					rHat[0]*tHat[1]*vIn(i1,i2,i3,s12c) + 
                                        rHat[1]*tHat[0]*vIn(i1,i2,i3,s21c) +
					rHat[1]*tHat[1]*vIn(i1,i2,i3,s22c) );
	  }
	  else if( option==4 )
	  {
             // that. S . that 
	    vOut(i1,i2,i3,indexOut) = ( tHat[0]*tHat[0]*vIn(i1,i2,i3,s11c) + 
					tHat[0]*tHat[1]*vIn(i1,i2,i3,s12c) + 
                                        tHat[1]*tHat[0]*vIn(i1,i2,i3,s21c) +
					tHat[1]*tHat[1]*vIn(i1,i2,i3,s22c) );
	  }
	  else
	  {
             OV_ABORT("ERROR: unexpected option.");
	  }
	  
	}
      }
      else
      {
	OV_ABORT("FINISH ME");
      }
      
    }

  }
  else
  {
    printF("DerivedFunctions::getUserDefinedDerivedFunction: ERROR: unknown user defined function: %s\n",
	   (const char*)name);
    return 1;
  }
  

  return 0;
}

