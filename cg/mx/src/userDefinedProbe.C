#include "Maxwell.h"
#include "ProbeInfo.h"
#include "GenericGraphicsInterface.h"
#include "InterpolatePointsOnAGrid.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3)					\
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(); \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++)					\
  for(i2=I2Base; i2<=I2Bound; i2++)					\
    for(i1=I1Base; i1<=I1Bound; i1++)

// =======================================================================================
/// \brief Create a user defined probe. 
// =======================================================================================
int Maxwell::
userDefinedProbe( GenericGraphicsInterface & gi )
{

  // create the probe:
  if(!parameters.dbase.has_key("userProbeList") ) parameters.dbase.put<std::vector<ProbeInfo*> >("userProbeList");

  std::vector<ProbeInfo* > & userProbeList = parameters.dbase.get<std::vector<ProbeInfo*> >("userProbeList");
  ProbeInfo & probe = *( new ProbeInfo(parameters) );
  userProbeList.push_back(&probe);

  int & probeGrid          = probe.dbase.put<int>("probeGrid");
  real & incidentAmplitude = probe.dbase.put<real>("incidentAmplitude");
  real & incidentPhase     = probe.dbase.put<real>("incidentPhase");
  real & offset            = probe.dbase.put<real>("offset");

  probeGrid=-1;
  offset=.25;  // offset length when computing T and R 
  incidentAmplitude=1.;
  incidentPhase=0.;

  aString & probeName = probe.dbase.get<aString>("probeName");
  probeName="myProbe";

  aString & fileName= probe.fileName;
  fileName= "myProbe.dat";

  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  RealArray box(2,3); // box for transmission probe 
  Range Rx=3;
  box(0,Rx)=0.; box(1,Rx)=1.;


  GUIState gui;
  gui.setWindowTitle("User Defined Probe");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;


  const int maxCommands=40;
  aString cmd[maxCommands];
  aString prefix;

  aString pbLabels[] = {"reflection probe",
                        "transmission probe",
                        ""};
  addPrefix(pbLabels,prefix,cmd,maxCommands);
  int numRows=1;
  dialog.setPushButtons( cmd, pbLabels, numRows ); 

  // dialog.setOptionMenuColumns(1);
  // aString typeCommands[] = { "grid point probe",
  //                            "location probe",
  //                            "bounding box probe",
  //                            "region probe",
  //                            "surface probe",
  //                            "" };
  // dialog.addOptionMenu("Type:",typeCommands,typeCommands,probeType );

  const int numberOfTextStrings=40;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "probe name"; sPrintF(textStrings[nt], "%s",(const char*)probeName);  nt++; 

  textLabels[nt] = "probe grid";  sPrintF(textStrings[nt], "%i (-1 = find grid automatically)",probeGrid);
  
  textLabels[nt] = "probe box";  sPrintF(textStrings[nt], "%g,%g, %g,%g, %g,%g (xa,xb, ya,yb, za,zb)",
                                         box(0,0),box(1,0), box(0,1),box(1,1), box(0,2),box(1,2)); nt++;
  textLabels[nt] = "R/T offset";  sPrintF(textStrings[nt], "%e",offset); nt++;

  textLabels[nt] = "incident amplitude";  sPrintF(textStrings[nt], "%e",incidentAmplitude); nt++;
  textLabels[nt] = "incident phase";  sPrintF(textStrings[nt], "%e",incidentPhase); nt++;

  // textLabels[nt] = "nearest grid point to"; sPrintF(textStrings[nt], "%g %g %g",0.,0.,0.); nt++;
  // textLabels[nt] = "location"; sPrintF(textStrings[nt], "%g %g %g",0.,0.,0.); nt++;
														      

  textLabels[nt] = "file name"; sPrintF(textStrings[nt], "%s",(const char*)fileName);  nt++; 


  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("userProbe>");

  aString answer;
  
  int len=0;
  for(int it=0; ; it++)
  {
    gi.getAnswer(answer,"");
  
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"probe name","%s",probeName) ){} // 

    else if( dialog.getTextValue(answer,"probe grid","%i",probeGrid) ){} // 

    else if( dialog.getTextValue(answer,"R/T offset","%e",offset) ){} // 

    else if( dialog.getTextValue(answer,"incident amplitude","%e",incidentAmplitude) ){} // 
    else if( dialog.getTextValue(answer,"incident phase","%e",incidentPhase) ){} // 

    else if( len=answer.matches("file name") )
    {
      // remove initial and trailing blanks.
      int length=answer.length();
      int istart=len;
      while( istart<length && answer[istart]==' ') istart++;
      int iend=length-1;
      while( iend>=istart && answer[iend]==' ') iend--;
      if( iend>=istart )
      {
	fileName=answer(istart,iend);
	printF("userDefinedProbe: setting probe file name = [%s]\n",(const char*)fileName);
      }
      else
      {
        printF("userDefinedProbe::update:ERROR: file name was empty!\n");
        gi.stopReadingCommandFile();
      }
    }

    else if( len=answer.matches("probe box") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e"
                ,&box(0,0),&box(1,0), &box(0,1),&box(1,1), &box(0,2),&box(1,2));

      printf("userDefinedProbe: Setting transmission box: [%e,%e]X[%e,%e]X[%e,%e]\n",
             box(0,0),box(1,0), box(0,1),box(1,1), box(0,2),box(1,2));

      for( int axis=0; axis<3; axis++ )
      {
        if( box(0,axis) > box(1,axis) )
        {
          printF("--PROBE-- ERROR: box(0,%i)=%g > box(1,%i)=%g\n",axis,box(0,axis),axis,box(1,axis));
          OV_ABORT("ERROR");
        }
      }
      
    }
      
    else if( answer=="reflection probe" ||
             answer=="transmission probe" )
    {
      bool reflection= answer=="reflection probe";

      printF("The reflection/transmission probe is used to compute transmission and reflection coefficients\n"
             " The probe will evaluate certain sums over a rectangular grid of points\n");

      printF("The reflected and transmitted fields are assumed to be of the form:\n"
             "        Refelected field = Im( A*exp(2*pi*i(kx*x- omega*t -phi )) \n"
             "                             + R*exp(2*pi*i( -kx*(x-L) - omega*t ) ) )\n"
             "        Transmitted field = Im( T*exp(2*pi*i( kx*(x-L) - omega*t ) \n");

      printF(" Parameters defining the probe are\n"
             "   probe box : (xa,xb) X (ya,yb) X (za,zb) : average over grid points in this box\n"
             "   probe grid: Background Cartesian grid that holds the probe box (-1=find automatically)\n"
             "   offset : offset=L is the length L in the forumla for T or R (affects the phase of R/T) \n"
             "   incident amplitude : equals A in the formula for the reflected field\n"
             "   incident phase : equals phi in the formula for the reflected field\n");

      probe.probeType=ProbeInfo::probeUserDefined;
      aString & userProbeType = probe.dbase.put<aString>("userProbeType");
      if( reflection ) 
        userProbeType="reflection";
      else
        userProbeType="transmission";

      // probe.fileName=fileName;
      // printF("Setting probe.fileName=[%s]\n",(const char*)probe.fileName);

      // probe.dbase.put<real>("incidentAmplitude")=incidentAmplitude;
      // probe.dbase.put<real>("incidentPhase")    =incidentPhase;
      // probe.dbase.put<real>("offset")           =offset;

      // boxIndexRange(side,axis) : index box of the grid points that will be averaged 
      IntegerArray & boxIndexRange = probe.dbase.put<IntegerArray>("boxIndexRange");
      boxIndexRange.redim(2,3);
      boxIndexRange=0;
      
      real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
      int iv0[3]={0,0,0}; //
      int iv[3]={0,0,0}, &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
      real xv[3]={0.,0.,0.};

      // gi.inputString(answer,"Enter the bounds on the transmisionn box: xa,xb, ya,yb, za,zb");
      // sScanF(answer,"%e %e %e %e %e %e",&xa,&xb,&ya,&yb,&za,&zb);

      if( probeGrid<0 || probeGrid>=cg.numberOfComponentGrids() )
      {
        // Locate the far field grid that holds the probe box
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
          MappedGrid & mg = cg[grid];
          if( mg.isRectangular() )
          {
            mg.getRectangularGridParameters( dvx, xab );
            bool ok=true;
            for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
            {
              printF(" grid=%i axis=%i box=[%e,%e] xab=[%e,%e]\n",
                     grid,axis, box(0,axis),box(1,axis),xab[0][axis],xab[1][axis]);
              
              real xm=.5*( box(0,axis) + box(1,axis) );  // mid point of box
              if( xm< xab[0][axis] || xm > xab[1][axis] )
              {
                ok=false;
                break;
              }
            }
            if( ok )
            {
              probeGrid=grid;
              break;
            }
          }
        }
        if( probeGrid>=0  && probeGrid<cg.numberOfComponentGrids() )
          printF("Probe box is found on grid=%i (%s)\n",probeGrid,(const char*)cg[probeGrid].getName());
        
      }
      
      if( probeGrid<0 || probeGrid>=cg.numberOfComponentGrids() )
      {
        printF("--userDefinedProbe:ERROR: unable to find a Carteasian background grid tha holds the probe box\n");
        OV_ABORT("error");
      }

      // save mid-point of the box as the probe location
      for( int axis=0; axis<3; axis++ )
        probe.xv[axis]=.5*( box(0,axis) + box(1,axis) );  // mid point of box

      // Find box in index space of the back-ground grid
      RealArray x(1,3); x=0.;
      IntegerArray il(1,4); il=0; // holds (donor,i1,i2,i3)
      RealArray ci(1,3);    ci=0.;

      // cg.update(MappedGrid::THEmask); 

      // -- for now assume the box we sum over is on the back-ground grid --
      MappedGrid & mg = cg[probeGrid];
      const IntegerArray & gid = cg[probeGrid].gridIndexRange();

      const bool isRectangular=mg.isRectangular();
      assert( isRectangular );
      

      mg.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
        iv0[dir]=mg.gridIndexRange(0,dir);
        if( mg.isAllCellCentered() )
          xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }

      // This macro defines the grid points for rectangular grids:
#undef XC
#define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

      for( int side3=0; side3<=numberOfDimensions-2; side3++ )
      for( int side2=0; side2<=1; side2++ )
      for( int side1=0; side1<=1; side1++ )
      {
        x(0,0)=box(side1,0);
        x(0,1)=box(side2,1);
        x(0,2)=numberOfDimensions==2 ? 0. : box(side3,2);
        if( isRectangular )
        {
          il(0,0)=probeGrid;
          for( int axis=0; axis<numberOfDimensions; axis++ )
          {
            // closest point 
            iv[axis] = int( (x(0,axis)-xab[0][axis])/dvx[axis]+iv0[axis] +.5);
            iv[axis] = min(gid(1,axis),max(gid(0,axis), iv[axis] )); // restrict to grid bounds 

            il(0,axis+1) = iv[axis];
            
          }
          if( true )
          {

            printF(" Nearest grid point to x=(%g,%g,%g) is donor=%i index=(%i,%i,%i) -> xv=[%g,%g,%g)\n",
                   x(0,0),x(0,1),x(0,2), il(0,0), il(0,1),il(0,2),il(0,3), XC(iv,0),XC(iv,1),XC(iv,2));
          }
        
          
        }
        else 
        {
          // -- more general way -- watch out for periodic grids -- nearest point is shifted******          
          InterpolatePointsOnAGrid::findNearestValidGridPoint( cg, x, il, ci );
          if( true )
          {
            printF(" Nearest grid point to x=(%g,%g,%g) is donor=%i index=(%i,%i,%i) \n",
                   x(0,0),x(0,1),x(0,2), il(0,0), il(0,1),il(0,2),il(0,3));
          }
        
        }
        

        int donor = il(0,0);  // donor donor 
        assert( donor>=0 && donor <cg.numberOfComponentGrids() );

        assert( donor==probeGrid );  // do this for now

        boxIndexRange(side1,0)=il(0,1);
        boxIndexRange(side2,1)=il(0,2);
        boxIndexRange(side3,2)=il(0,3);

      } // end for side1, side2, side3 
      
      
      printF(" probe box index range: [%i,%i] X [%i,%i] X [%i,%i]\n",
        boxIndexRange(0,0),boxIndexRange(1,0), 
        boxIndexRange(0,1),boxIndexRange(1,1), 
        boxIndexRange(0,2),boxIndexRange(1,2));
      
      
      printF(" Grid=%i: gid: [%i,%i] X [%i,%i] X [%i,%i]\n",
        probeGrid,
        gid(0,0),gid(1,0), 
        gid(0,1),gid(1,1), 
        gid(0,2),gid(1,2));

    }
    else
    {
      printF("userDefinedProbe::update:ERROR: Unknown response: [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
       
    }

  }
  
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();


  return 0;
}


// =======================================================================================
/// \brief Output any user defined probes
// =======================================================================================
int Maxwell::
outputUserDefinedProbes( int current, real t, real dt, int stepNumber )
{


  if(!parameters.dbase.has_key("userProbeList") )
  {
    // there are no user defined probes
    return 0;
  }
  
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  realCompositeGridFunction & gf =  cgfields[current];
  

  std::vector<ProbeInfo* > & userProbeList = parameters.dbase.get<std::vector<ProbeInfo*> >("userProbeList");

  // printF("outputUserDefinedProbes: number of probes=%i\n",userProbeList.size());
  
  for( int i=0; i<userProbeList.size(); i++ )
  {
    assert( userProbeList[i]!=NULL );
    ProbeInfo & probe = *(userProbeList[i]);

    // Output user defined probe data 
    aString & userProbeType = probe.dbase.get<aString>("userProbeType");
    
    if( userProbeType=="reflection" ||
        userProbeType=="transmission"  )
    {
      // ------ Reflection or Transmission probe  ------

      bool reflection= userProbeType=="reflection";

      if( t<= 3.*dt )
        printF("outputUserDefinedProbes: %s : t=%9.3e, stepNumber=%i\n",(const char*)userProbeType,t,stepNumber);

      const int & probeGrid          = probe.dbase.get<int>("probeGrid");
      const real & incidentAmplitude = probe.dbase.get<real>("incidentAmplitude");
      const real & incidentPhase     = probe.dbase.get<real>("incidentPhase");
      const real & offset            = probe.dbase.get<real>("offset");

      const int numberOfComponents=2;
      aString componentNames[2];
      // we save the real and imaginary parts of R and T
      if( reflection )
      { 
        componentNames[0]="Rr";
        componentNames[1]="Ri";
      }
      else
      {
        componentNames[0]="Tr";
        componentNames[1]="Ti";
      }
      
      FILE *& file = probe.file;
      if( file==NULL &&  stepNumber==0 )
      { // print info message here for myid=0, but open file below on proc. that owns the data.
	printF("Open user defined probe file %s for probe %i.\n",(const char*)probe.fileName,i);
	file = fopen((const char*)probe.fileName,"w");
	assert( file!=NULL );
	assert( probe.file!=NULL );

	if( i==0 )
	{
	  // printF("The probe files can be plotted with the matlab script plotProbes.m\n");
	}

	int numHeader=4;  // number of header comments 
	int numColumns=1 + 3  + numberOfComponents;         // time + (x,y,z) + components 

	// --- write header comments to the probe file  ---

	// line 1 holds the number-of-header lines and the number of columns 
	fprintf(file,"%i %i    (number-of-header-lines number-of-columns)\n",numHeader,numColumns);

        // Note: this is the title on the matlab plot
        fprintf(file,"%s, name=%s\n",(const char*)userProbeType,
                (const char*)probe.dbase.get<aString>("probeName"));
	fprintf(file,"This file can be read with the matlab script plotProbes.m, using >plotProbes %s\n",
                (const char*)probe.fileName);


	// Get the current date
	time_t *tp= new time_t;
	time(tp);
	const char *dateString = ctime(tp);
	fprintf(file,"File created on %s",dateString); // note: dateString ends in a newline.
	delete tp;

	fprintf(file,"        t                x                y                z");
	for( int c=0; c<numberOfComponents; c++ )
	{
	  fprintf(file,"                %s",(const char*)componentNames[c]);
	}
	fprintf(file,"\n");
      }

      fprintf(file,"%16.9e ",t);  // time
      fprintf(file,"%16.9e %16.9e %16.9e ",probe.xv[0],probe.xv[1],probe.xv[2]); // print x location


      IntegerArray & boxIndexRange = probe.dbase.get<IntegerArray>("boxIndexRange"); 

      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    
      MappedGrid & mg = cg[probeGrid];
      realMappedGridFunction & u = gf[probeGrid];

      const IntegerArray & gid = cg[probeGrid].gridIndexRange();

      const bool isRectangular=mg.isRectangular();
      assert( isRectangular );
      
      real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
      int iv0[3]={0,0,0}; //
      int iv[3]={0,0,0}, &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
      real xv[3]={0.,0.,0.};

      mg.getRectangularGridParameters( dvx, xab );
      for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
      {
        iv0[dir]=mg.gridIndexRange(0,dir);
        if( mg.isAllCellCentered() )
          xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
      }

      // This macro defines the grid points for rectangular grids:
#undef XC
#define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))


      getIndex(boxIndexRange,I1,I2,I3);

      // c = omega/kx, assumes ky=0 
      real omega=kx*cGrid(probeGrid); 
       
      real a11=0., a12=0., a22=0.;
      real f1=0., f2=0.;
      
      const real kxpm = reflection ? -kx : kx;  // reflected wave travels backward

      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
        assert( iv[0]==i1 && iv[1]==i2 );
        
        real x = XC(iv,0);
        
        // real xi = kx*(x-L);
        real xi = twoPi*( kxpm *(x-offset)-omega*t);
        // RePart: real c = cos(xi), s=-sin(xi);
        // ImPart: -- I think the plane wave is sin(2*pi*(kx*x-omega*t))
        real c = sin(xi), s=cos(xi);
        
        a11 += c*c;
        a12 += c*s;
        a22 += s*s;
         
        real uEy = u(i1,i2,i3,ey);
        if( reflection )
        {  // subtract off the incident field
          uEy -= incidentAmplitude*sin(twoPi*(kx*x-omega*t-incidentPhase));
        }
        

        f1 +=  c*uEy;
        f2 +=  s*uEy;
      }
      
      real det = a11*a22 -a12*a12;
      if( fabs(det)<REAL_MIN*100. ){ det=1.; }  // 
      real Tr =  (a22*f1 - a12*f2)/det;
      real Ti = (-a12*f1 + a11*f2)/det;

      // Least squares solution:
      // [ a11 a12 ][ Tr ] = [ f1 ]
      // [ a12 a22 ][ Ti ] = [ f2 ]
      //
      // Tr = ( a22*f1 - a12*f2)*deti
      // Ti = (-a12*f1 + a11*f2)*deti
       
  
      if( t<= 10.*dt )
      {
        printF("outputUserDefinedProbes:\n");
        printF(" probe box index range: [%i,%i] X [%i,%i] X [%i,%i] (probeGrid=%i)\n",
               boxIndexRange(0,0),boxIndexRange(1,0), 
               boxIndexRange(0,1),boxIndexRange(1,1), 
               boxIndexRange(0,2),boxIndexRange(1,2),probeGrid);
      }
      if( (stepNumber % 10 )==0  )
      {
        // printF(" a11=%g, a12=%g, a22=%g, f1=%g, f2=%g det=%9.2e kx=%g\n",a11,a12,a22,f1,f2,det,kx);
      
        if( reflection )
          printF(" relection probe:    t=%9.3e, R = (%8.6g,%8.6g) |R|=%8.6g (step=%i)\n",
                 t,Tr,Ti,sqrt(Tr*Tr+Ti*Ti),stepNumber);
        else
          printF(" transmission probe: t=%9.3e, T = (%8.6g,%8.6g) |T|=%8.6g (step=%i) \n",
                 t,Tr,Ti,sqrt(Tr*Tr+Ti*Ti),stepNumber);

      }

      real val[2]={Tr,Ti}; // 
      for( int c=0; c<numberOfComponents; c++ )
      {
        fprintf(file,"%16.9e ",val[c]);
      }
      fprintf(file,"\n");

      fflush(file); // flush the file


    }
    else 
    {
      printF("outputUserDefinedProbes:ERROR: unknown userProbeType=[%s]\n",(const char*)userProbeType);
      OV_ABORT("error");

    }
    

  }
  


  return 0;
}
