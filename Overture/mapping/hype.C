#include "Overture.h"
#include "DataPointMapping.h"
#include "PlotStuff.h"
#include "MappingInformation.h"
#include "CircleMapping.h"

int 
hyper(
      int & IFORM, int & IZSTRT, int & NZREG,
      int & NPZREG, real & ZREG,  real & DZ0, real &  DZ1,
      int & IBCJA,int & IBCJB,int & IBCKA,int & IBCKB,
      int & IVSPEC, real & EPSSS, int & ITSVOL,
      int & IMETH, real & SMU2,
      real & TIMJ, real & TIMK,
      int & IAXIS, real & EXAXIS, real & VOLRES,
      int & JMAX, int & KMAX,
      int & JDIM,int & KDIM,int & LMAX,
      real & X, real & Y, real & Z,
      realArray & XW, realArray & YW, realArray & ZW );

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  
  int iform=1, jdim,kdim,lmax;
  int izstrt=1, nzreg=1;
  intArray npzreg(nzreg);
  realArray zreg(nzreg),dz0(nzreg),dz1(nzreg);
  npzreg(0)=25;
  zreg(0)=1.0;
  dz0(0)=0.0006;
  dz1(0)=0.0;
  int ibcja, ibcjb, ibcka, ibckb;
  ibcja=10;  // periodic
  ibcjb=10;  // periodic
  ibcka=3;   // 3=const z plane 2=const y plane; 
  ibckb=3;   //
  int ivspec=1;
  real epsss=0.;
  int itsvol=70;
  int imeth=2;
  real smu2=.3;
  real timj=0.;
  real timk=0.;
  int iaxis=0;
  real exaxis=0.;
  real volres=0.;
  
  
/* ---
C   IFORM = 0  unformatted output of PLOT3D volume grid file
C         = 1  formatted . . . . . . . . . . . . . . . . . .
C
C   IZSTRT = 1  exponential stretching in L
C          = 2  hyperbolic tangent stretching in L
C          = -1 stretching funtion specified in file zetastr.i (@)
C   NZREG  = number of L-regions
C   (@) Only 1 L-region can be used with this option and ZREG,DZO,DZ1
C       will be disregarded.
C
C   NPZREG = number of points (including ends) in each L-region
C   ZREG   > 0  distance to march out for this L-region
C          <=0  variable far field distance specified in file zetavar.i (#)
C   DZ0    = 0  initial spacing is not fixed for this L-region
C          > 0  initial spacing for this L-region
C          < 0  variable initial spacing specified in file zetavar.i (#)
C   DZ1    = 0  end spacing is not fixed for this L-region
C          > 0  end spacing for this L-region
C          < 0  variable end spacing specified in file zetavar.i (#)
C   (#) Applied to first L-region only
C
C   The boundary conditions types at J=1, J=JMAX, K=1, K=KMAX are indicated 
C   by IBCJA, IBCJB, IBCKA, IBCKB, respectively
C
C   IBCJA  = -1  float X, Y and Z - zero order extrapolation (free floating)
C          < -1  outward-splaying free floating boundary condition which bends
C                the edge away from the interior. Use small $|$IBCJA$|$ for
C                small bending - mixed zeroth and first order extrapolation
C                with EXTJA = -IBCJA/1000.0 where EXTJA must satisfy 0 <EXTJA< 1
C          =  1  fix X, float Y and Z (constant X plane)
C          =  2  fix Y, float X and Z (constant Y plane)
C          =  3  fix Z, float X and Y (constant Z plane)
C          =  4  float X, fix Y and Z
C          =  5  float Y, fix X and Z
C          =  6  float Z, fix X and Y
C          =  7  floating collapsed edge with matching upper and lower sides
C                (points along K=1,(KMAX+1)/2 are matched with those on K=KMAX,(KMAX+1)/2)
C          = 10  periodic condition (*)
C          = 11  reflected symmetry condition with X=constant plane
C          = 12  reflected symmetry condition with Y=constant plane
C          = 13  reflected symmetry condition with Z=constant plane
C          = 20  singular axis point
C          = 21  constant X planes for interior and boundaries slices (*)
C          = 22  constant Y planes for interior and boundaries slices (*)
C          = 23  constant Z planes for interior and boundaries slices (*)
C   (*) Must also apply at the other end condition in J
C
C   IBCJB, IBCKA, IBCKB likewise
C
C   IVSPEC = 1  volume spec. by cell area times arc length
C          = 2  volume spec. by mixed spherical volumes scaling
C   EPSSS  = parameter that controls how fast spherical volumes are mixed in
C            (used with IVSPEC=2 only)
C   ITSVOL = number of times volumes are averaged
C
C   IMETH  = 0  constant coef. dissipation
C          = 1  spatially-varying coef. dissipation
C          = 2  severe convex corners treated by solving averaging eqns.
C          = 3  severe convex corners treated by angle-bisecting predictor
C   SMU2   = second order dissipation coef.
C
C   TIMJ   = Barth implicitness factor in J
C   TIMK   = Barth implicitness factor in K
C   
C   The following 3 parameters are read in only if axis bc is activated
C
C   IAXIS = 1  extrapolation and volume scaling logic
C         = 2  same as 1 but with dimple smoothing
C   EXAXIS = 0  zeroth order extrapolation at axis
C          > 0 and < 1  control local pointedness at axis (~0.3)
C          = 1  first order extrapolation at axis
C   VOLRES      restrict volume at one point from axis. This parameter is
C               only switched on if exaxis is non-zero. Good values are
C               ~0.1 to ~0.5
C_______________________________________________________________________
--- */

  PlotStuff ps(TRUE,"hypgen");      
  GenericGraphicsInterface & gi = ps;
  PlotStuffParameters psp;          
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&ps;
    
  DataPointMapping dpm;
//  const int m3d=1781001;
  realArray xw,yw,zw,xyz;
  int domainDimension=2;
  int rangeDimension=2;

  int jmax=21, kmax=1;
  jdim=jmax+2;
  kdim=kmax+2;
  CircleMapping ellipse;
  realArray r(jdim,kdim,domainDimension-1),x(jdim,kdim,3),x0;
  r=0.;
  x=0.;
  real h=1./(jmax-1.);
  for( int j=0; j<kmax; j++ )
  {
    for( int i=0; i<jmax; i++ )
    {
      r(i,j,0)=i*h;
    }
  }
  r.reshape(jdim*kdim,domainDimension-1);
  x.reshape(jdim*kdim,3);
  ellipse.map(r,x);
  r.reshape(jdim,kdim,domainDimension-1);
  x.reshape(jdim,kdim,3);
  PlotIt::plot(ps,ellipse);
  Index I(0,jdim), J(0,kdim);
  // set x and z values for hypgen
//  x(I,J,2)=x(I,J,1);
//  x(I,J,1)=0.;

  char buff[80];
  aString answer,line;
  aString menu[] = { "plot", 
		    "generate",
                    "stretching in normal direction",
                    "lines in normal direction",
                    "boundary condition",
                    "dissipation",
                    "distance to march",
                    "spacing",
                    "volume parameters",
                    "Barth implicitness factors",
                    "axis bc parameters",
		    "exit",
                    "" };                       // empty string denotes the end of the menu
  for(;;)
  {
    ps.getMenuItem(menu,answer);               
    if( answer=="plot" )
    {
      PlotIt::plot(ps,dpm); // ,(GraphicsParameters&)psp);
    }
    else if( answer=="generate" )
    {
      x0=x;
      int jmax0=jmax;
      int kmax0=kmax;
      lmax = sum(npzreg(Range(0,nzreg-1)));
      
      hyper(iform,izstrt,nzreg,
             npzreg(0),zreg(0),dz0(0),dz1(0), 
             ibcja, ibcjb, ibcka, ibckb,
             ivspec, epsss, itsvol,
             imeth, smu2,
             timj, timk,
             iaxis,exaxis,volres,
             jmax,kmax,
             jdim,kdim,lmax,
             x0(0,0,0), x0(0,0,1), x0(0,0,2),
             xw,yw,zw );
      printf("\n\n ++++ jmax=%i, kmax=%i, jdim=%i, kdim=%i, lmax=%i \n",jmax,kmax,jdim,kdim,lmax);
      if( domainDimension==2 )
      {
        xyz.redim(jdim-2,lmax,1,domainDimension);
        // xw.resize(jdim,kdim,lmax);
        // yw.resize(jdim,kdim,lmax);
	// zw.resize(jdim,kdim,lmax);
        // xw.display("here is xw");
        // yw.display("here is yw");
        // zw.display("here is zw");
	
	for( int j=0; j<lmax; j++ )
	for( int i=0; i<jdim-2; i++ )
	{
          xyz(i,j,0,axis1)=xw(i+jdim*(1+kdim*j));   // take plane 1
          // xyz(i,j,0,axis2)=zw(i+jdim*(1+kdim*j));
          xyz(i,j,0,axis2)=yw(i+jdim*(1+kdim*j));
	}
      }
      // xyz.display("here is xyz");
      dpm.setDataPoints(xyz,3,domainDimension);
      PlotIt::plot(ps,dpm); // ,(GraphicsParameters&)psp);
      // reset
      jmax=jmax0;
      kmax=kmax0;
    }
    else if( answer=="stretching in normal direction" )
    {
      gi.outputString("IZSTRT = 1  exponential stretching in L");
      gi.outputString("       = 2  hyperbolic tangent stretching in L");
      gi.outputString("       = -1 stretching funtion specified in file zetastr.i (@)");
      gi.inputString(line,"Enter option (1=exp,2=hyperbolic tangent,3=specify explicitly)");
      if( line!="" ) sScanF( line,"%i",&izstrt);
    }
    else if( answer=="dissipation" )
    {
      gi.outputString("  IMETH  = 0  constant coef. dissipation");
      gi.outputString("         = 1  spatially-varying coef. dissipation");
      gi.outputString("         = 2  severe convex corners treated by solving averaging eqns.");
      gi.outputString("         = 3  severe convex corners treated by angle-bisecting predictor");
      gi.outputString("  SMU2   = second order dissipation coef.");
      gi.inputString(line,sPrintF(buff,"Enter imeth and smu2 (currrent values = %i, %e)",imeth,smu2));
      if( line!="" ) sScanF( line,"%i %e",&imeth,&smu2);
    }
    else if( answer=="lines in normal direction" )
    {
      gi.outputString("NPZREG = number of points (including ends) in each L-region");
      gi.inputString(line,sPrintF(buff,"Enter npzreg (number of lines in the normal direction, current=%i)",npzreg));
      if( line!="" ) sScanF( line,"%i",&npzreg(0));
    }
    else if( answer=="distance to march" )
    {
      gi.outputString("   ZREG   > 0  distance to march out for this L-region");
      gi.outputString("          <=0  variable far field distance specified in file zetavar.i (#)");
      gi.inputString(line,sPrintF(buff,"Enter zreg (current=%e)",zreg(0)));
      if( line!="" ) sScanF( line,"%e",&zreg(0));
    }
    else if( answer=="spacing" )
    {
      gi.outputString("   DZ0    = 0  initial spacing is not fixed for this L-region");
      gi.outputString("          > 0  initial spacing for this L-region");
      gi.outputString("          < 0  variable initial spacing specified in file zetavar.i (#)");
      gi.outputString("   DZ1    = 0  end spacing is not fixed for this L-region");
      gi.outputString("          > 0  end spacing for this L-region");
      gi.outputString("          < 0  variable end spacing specified in file zetavar.i (#)");
      gi.outputString("   (#) Applied to first L-region only");
      gi.inputString(line,sPrintF(buff,"Enter dz0,dz1 (current=%e,%e)",dz0(0),dz1(0)));
      if( line!="" ) sScanF( line,"%e %e",&dz0(0),&dz1(0));
    }
    else if( answer=="volume parameters" )
    {
      gi.outputString("   IVSPEC = 1  volume spec. by cell area times arc length");
      gi.outputString("          = 2  volume spec. by mixed spherical volumes scaling");
      gi.outputString("   EPSSS  = parameter that controls how fast spherical volumes are mixed in");
      gi.outputString("            (used with IVSPEC=2 only)");
      gi.outputString("   ITSVOL = number of times volumes are averaged");
      gi.inputString(line,sPrintF(buff,"Enter ivspec,epsss,itsvol (current=%i,%e,%i)",ivspec,epsss,itsvol));
      if( line!="" ) sScanF( line,"%i %e %i",&ivspec,&epsss,&itsvol);
    }
    else if( answer=="Barth implicitness factors" )
    {
      gi.outputString("   TIMJ   = Barth implicitness factor in J");
      gi.outputString("   TIMK   = Barth implicitness factor in K");
      gi.inputString(line,sPrintF(buff,"Enter timj,timk (current=%e,%e)",timj,timk));
      if( line!="" ) sScanF( line,"%e %e",&timj,&timk);
    }
    else if( answer=="boundary condition" )
    {
      gi.outputString("   The boundary conditions types at J=1, J=JMAX, K=1, K=KMAX are indicated ");
      gi.outputString("   by IBCJA, IBCJB, IBCKA, IBCKB, respectively");
      gi.outputString(" ");
      gi.outputString("   IBCJA  = -1  float X, Y and Z - zero order extrapolation (free floating)");
      gi.outputString("          < -1  outward-splaying free floating boundary condition which bends");
      gi.outputString("                the edge away from the interior. Use small $|$IBCJA$|$ for");
      gi.outputString("                small bending - mixed zeroth and first order extrapolation");
      gi.outputString("                with EXTJA = -IBCJA/1000.0 where EXTJA must satisfy 0 <EXTJA< 1");
      gi.outputString("          =  1  fix X, float Y and Z (constant X plane)");
      gi.outputString("          =  2  fix Y, float X and Z (constant Y plane)");
      gi.outputString("          =  3  fix Z, float X and Y (constant Z plane)");
      gi.outputString("          =  4  float X, fix Y and Z");
      gi.outputString("          =  5  float Y, fix X and Z");
      gi.outputString("          =  6  float Z, fix X and Y");
      gi.outputString("          =  7  floating collapsed edge with matching upper and lower sides");
      gi.outputString("                (points along K=1,(KMAX+1)/2 are matched with those on K=KMAX,(KMAX+1)/2)");
      gi.outputString("          = 10  periodic condition (*)");
      gi.outputString("          = 11  reflected symmetry condition with X=constant plane");
      gi.outputString("          = 12  reflected symmetry condition with Y=constant plane");
      gi.outputString("          = 13  reflected symmetry condition with Z=constant plane");
      gi.outputString("          = 20  singular axis point");
      gi.outputString("          = 21  constant X planes for interior and boundaries slices (*)");
      gi.outputString("          = 22  constant Y planes for interior and boundaries slices (*)");
      gi.outputString("          = 23  constant Z planes for interior and boundaries slices (*)");
      gi.outputString("   (*) Must also apply at the other end condition in J");
      gi.outputString("");
      gi.outputString("   IBCJB, IBCKA, IBCKB likewise");
      gi.inputString(line,sPrintF(buff,"Enter ibcja, ibcjb, ibcka, ibckb, (current =%i,%i,%i,%i)",
                     ibcja,ibcjb,ibcka,ibckb));
      if( line!="" ) sScanF( line,"%i %i %i %i",&ibcja, &ibcjb, &ibcka, &ibckb);
    }
    else if( answer=="axis bc parameters" )
    {
      
      gi.outputString("   The following 3 parameters are read in only if axis bc is activated");
      gi.outputString(" ");
      gi.outputString("   IAXIS = 1  extrapolation and volume scaling logic");
      gi.outputString("         = 2  same as 1 but with dimple smoothing");
      gi.outputString("   EXAXIS = 0  zeroth order extrapolation at axis");
      gi.outputString("          > 0 and < 1  control local pointedness at axis (~0.3)");
      gi.outputString("          = 1  first order extrapolation at axis");
      gi.outputString("   VOLRES      restrict volume at one point from axis. This parameter is");
      gi.outputString("               only switched on if exaxis is non-zero. Good values are");
      gi.outputString("               ~0.1 to ~0.5");
      gi.inputString(line,sPrintF(buff,"Enter iaxis,exaxis,volres (current=%i,%e,%e)",iaxis,exaxis,volres));
      if( line!="" ) sScanF( line,"%i %e %e",&iaxis,&exaxis,volres);
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  return 0;
}
