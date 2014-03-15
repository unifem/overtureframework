#include "Mapping.h"
#include "OvertureTypes.h"
#include "DataPointMapping.h"
#include "PlotStuff.h"
#include "MappingInformation.h"
#include "HyperbolicSurfaceMapping.h"

#define SURGRD EXTERN_C_NAME(surgrd)

extern "C"
{
  
    void SURGRD( 
               int & INIC, int & IJMAX, int & IJCMAX, int & IIRFAM, 
               int & IJRAXSA,int & IJRAXSB,int & IJRPER,int & IKRPER,
               int & IIBCJA, int & IIBCJB, int & IIAFAM,
               int & INGBCA, int & INGBCB, int & IKMAX, int & INNOD,
               int & IJNOD, real & ETAMX, real & DETA,
               real & DFAR, real & SMU, real & TIM, int & IITSVOL,
	       int & jrmax, int & krmax,
               real & xsurf,
                      int & ndra,int & ndrb,int & ndsa,int & ndsb,int & ndta,int & ndtb,
                      int & nra, int & nrb, int & nsa, int & nsb, int & nta, int & ntb,
               real & xcurve,
                      int & mdra,int & mdrb,int & mdsa,int & mdsb,int & mdta,int & mdtb,
                      int & mra, int & mrb, int & msa, int & msb, int & mta, int & mtb,
               real & xhype,
                      int & ldra,int & ldrb,int & ldsa,int & ldsb,int & ldta,int & ldtb,
	              int & lra, int & lrb, int & lsa, int & lsb, int & lta, int & ltb);
}

int
readPlot3d( MappingInformation & mapInfo, const aString & plot3dFileName=nullString );


// -----------------------------------------------------------------------
//    INPUT NOTES:
// -----------------------------------------------------------------------
//    Boundary condition types for J=1
//    IBCJA  = -1 Free floating condition
//           =  1 Constant plane condition in X, float Y and Z
//           =  2 Constant plane condition in Y, float X and Z
//           =  3 Constant plane condition in Z, float X and Y
//           =  4 Reflected symmetry condition in X
//           =  5 Reflected symmetry condition in Y
//           =  6 Reflected symmetry condition in Z
//           = 10 Periodic condition (*)
//           = 11 Floating condition along KS=const line in +JS direction
//           = 12 Floating condition along KS=const line in -JS direction
//           = 13 Floating condition along JS=const line in +KS direction
//           = 14 Floating condition along JS=const line in -KS direction
//           = 15 Floating condition along user supplied line of points
//           = 20 Exact coordinates of boundary points prescribed by user
//           = 21 Constant X plane for all J from 1 to JMAX (*)
//           = 22 Constant Y plane for all J from 1 to JMAX (*)
//           = 23 Constant Z plane for all J from 1 to JMAX (*)
//           = 31 Float along KS=const line in +JS direction for all J (*)
//           = 32 Float along KS=const line in -JS direction for all J (*)
//           = 33 Float along JS=const line in +KS direction for all J (*)
//           = 34 Float along JS=const line in -KS direction for all J (*)
// 
//    (*) Must also apply for IBCJB
// 
//    Similarly for IBCJB at J=JMAX
// 
//    IAFAM >0  Family number of family on which to project grid to
// 
//    If IBCJA,IBCJB is 11-14 or 31-34 then enter NGBCA,NGBCB
//    NGBCA = Reference grid number for floating constant JS,KS bc at J=1
//    NGBCB = Reference grid number for floating constant JS,KS bc at J=JMAX
// 
//    KMAX = Number of points in eta (marching direction)
//    NNOD <=1  Constant far field, initial/end grid spacing.
//         > 1  Number of nodes of piece-wise linear function for variable far
//              field distance, initial/end grid spacing.
//              (must be in range 2 < = NNOD < = MNOD, a constant far field,
//               distance, initial/end grid spacing is specified with NNOD=1)
// 
//    Repeat the following for each node
//    Do N=1,NNOD
//     JNOD(N) = Node index of Nth node (-1 = last node, disregarded if NNOD=1)
//     ETAMX(N) = Far field distance at node N
//     DETA(N) = First grid point spacing in marching direction from initial curve
//               (or 0 for no spacing control)
//     DFAR(N) = Last grid point spacing in marching direction from initial curve
//               (or 0 for no spacing control)
//    Enddo
// 
//    SMU    = Explicit smoothing coefficient O(1)
//    TIM    = TIM factor for smoothing in marching direction (0<=TIM<=3)
//    ITSVOL = Number of times prescribed areas DAREA(J) is smoothed
// 
//    NSPBC  = 0 topologies of all reference grids to be determined automatically
//           > 0 number of grids to specify topology
//    IAUC   = 0  Do not automatically concatenate consecutive grids that share
//                a common initial curve
//           = 1  Automatically concatenate consecutive grids that share
//                a common initial curve
//           = -1 Do not do projection
// 
//    Repeat the following for each reference grid that require topology
//    specification.
//    Topologies of grids not specified here will be determined automatically.
//    
//    IG     =     grid number of reference grid
//    IBCJS  = 0   non-periodic condition with no axis pts in J
//           = 1   axis point at J=1 only
//           = 2   axis point at J=jmax only
//           = 3   axis point at both J=1 and jmax
//           = 10  periodic condition in J
//    IBCKS  = 0   non-periodic condition with no axis pts in K
//           = 10  periodic condition in K
// 
//    NFAM   > 0   Number of reference grid families
//           = 0   All input reference grids belong to same family, i.e family #1
// 
//   If NFAM>0 read the following
// 
//    IFNUM  = Family number
//    NGFAM  = Number of grids in IFNUM family
//    IGNUM  = Reference grid number in IFNUM family


int
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  PlotStuff ps(TRUE,"surGrd");      
  GenericGraphicsInterface & gi = ps;
  GraphicsParameters params;
  MappingInformation mapInfo;
  mapInfo.graphXInterface=&ps;

  // get surface (wing)
  readPlot3d( mapInfo,"wingbody.dat" );
  // DataPointMapping & surface = (DataPointMapping&) (*(mapInfo.mappingList[0].mapPointer));

  // PlotIt::plot(ps,surface,params);
  // params.set(GI_USE_PLOT_BOUNDS,TRUE); 

  // get intersection line
  readPlot3d( mapInfo,"lines.dat" );
  // DataPointMapping & curve = (DataPointMapping&) (*(mapInfo.mappingList[2].mapPointer));

  // params.set(GI_SET_MAPPING_COLOUR,"green");
  // PlotIt::plot(ps,curve,params);


  HyperbolicSurfaceMapping hsm;
  
  hsm.update(mapInfo);
  

/* ----

  const int mrgrd=30;  // should be the same as chimera_dimens.h
  const int mnod=200;
      

  IntegerArray inic(mrgrd), jmax(mrgrd), jcmax(mrgrd), irfam(mrgrd), 
    jraxsa(mrgrd),jraxsb(mrgrd),jrper(mrgrd),krper(mrgrd),
    ibcja(mrgrd), ibcjb(mrgrd), iafam(mrgrd),
    ngbca(mrgrd), ngbcb(mrgrd), kmax(mrgrd), nnod(mrgrd),
    jnod(mnod,mrgrd), itsvol(mrgrd);
  
  RealArray etamx(mnod,mrgrd), deta(mnod,mrgrd),
    dfar(mnod,mrgrd), smu(mrgrd), tim(mrgrd);


//  grid 0:

  int nspbc=0;  // determine topology automatically
  int iauc=0;   // Do not automatically concatenate consecutive grids that share a common initial curve
  int nfam=1;
  IntegerArray ngfam(mrgrd), igfam(mrgrd,mrgrd);
  ngfam(0)=1;    // family 1 has 1 grid
  igfam(0,0)=1;  // family 1: conatins grid 1
  


  inic(0)=1;    // use curve 1  (correct)

  ibcja(0)=10;  // periodic
  ibcjb(0)=10;

  ngbca(0)=1; // for ibcja,ibcjb = 15 or 20, (user supplied boundary points) curve number
  ngbcb(0)=1;

  iafam(0)=1;   // use this family
  irfam(0)=1;
  
  jraxsb(0)=1;  // grid in reference family 1
  
  kmax(0)=12;   // number of points in marching direction
  nnod(0)=1;    // constant far field initial/end spacing
  for( int n=0; n<nnod(0); n++ )
  {
    jnod(n,0)=1;    // disregard
    etamx(n,0)=2.0;
    deta(n,0)=.1;
    dfar(n,0)=0.;
  }
  
  smu(0)=.5;
  tim(0)=0.;
  itsvol(0)=2;  
  
  jmax(0)=201;  // **** ???
  
  const RealArray & xs = surface.getDataPoints();
  const IntegerArray & sDimension = surface.getDimension();
  const IntegerArray & sIndex = surface.getGridIndexRange();

  const RealArray & xc = curve.getDataPoints();
  const IntegerArray & cDimension = curve.getDimension();
  const IntegerArray & cIndex = curve.getGridIndexRange();
  

  // array to hold new hyperbolic surface:
  RealArray hyperbolicSurface(jmax(0),kmax(0),1,3);
  IntegerArray hDimension(2,3),hIndex(2,3);
  hDimension(0,0)=0; hDimension(1,0)=jmax(0)-1;
  hDimension(0,1)=0; hDimension(1,1)=kmax(0)-1;
  hDimension(0,2)=0; hDimension(1,2)=0;
  hIndex=hDimension;

  // dimensions of the reference surface:
  IntegerArray jrmax(mrgrd),krmax(mrgrd);
  jrmax(0)=sIndex(End,0)-sIndex(Start,0)+1;
  krmax(0)=sIndex(End,1)-sIndex(Start,1)+1;
  
  // dimensions of the initial curve
  jcmax(0)=cIndex(End,0)-cIndex(Start,0)+1;


  DataPointMapping dpm;






  char buff[80];
  aString answer,line;
  aString menu[] = { 
                    "choose surface",
                    "choose initial curve",
                    "generate",
                    "boundary condition (IBCJA,IBCJB)",
                    "dissipation (SMU, TIM, ITSVOL)",
                    "spacing (ETAMX,DETA,DFAR)",
		    "exit",
                    "" };                       // empty string denotes the end of the menu
  for(;;)
  {
    gi.getMenuItem(menu,answer);               
    if( answer=="choose surface" )
    {
    }
    else if( answer=="choose initial curve" )
    {
    }
    else if( answer=="generate" )
    {
      SURGRD(inic(0), jmax(0), jcmax(0), irfam(0), 
	     jraxsa(0),jraxsb(0),jrper(0),krper(0),
	     ibcja(0), ibcjb(0), iafam(0),
	     ngbca(0), ngbcb(0), kmax(0), nnod(0),
	     jnod(0,0), etamx(0,0), deta(0,0),
	     dfar(0,0), smu(0), tim(0), itsvol(0),
	     jrmax(0),krmax(0),
	     xs(0,0,0,0),
	     sDimension(0,0),sDimension(1,0),sDimension(0,1),sDimension(1,1),sDimension(0,2),sDimension(1,2),
	     sIndex(0,0),sIndex(1,0),sIndex(0,1),sIndex(1,1),sIndex(0,2),sIndex(1,2),
	     xc(0,0,0,0),
	     cDimension(0,0),cDimension(1,0),cDimension(0,1),cDimension(1,1),cDimension(0,2),cDimension(1,2),
	     cIndex(0,0),cIndex(1,0),cIndex(0,1),cIndex(1,1),cIndex(0,2),cIndex(1,2),
	     hyperbolicSurface(0,0,0,0),
	     hDimension(0,0),hDimension(1,0),hDimension(0,1),hDimension(1,1),hDimension(0,2),hDimension(1,2),
	     hIndex(0,0),hIndex(1,0),hIndex(0,1),hIndex(1,1),hIndex(0,2),hIndex(1,2));

      // if( returnValue!=0 )
      //   break;  // hypgen not available
   

      dpm.setDataPoints(hyperbolicSurface,3,2);
      params.set(GI_SET_MAPPING_COLOUR,"blue");
      PlotIt::plot(ps,dpm,params);
      
    }
    else if( answer=="dissipation (SMU, TIM, ITSVOL)" )
    {
      gi.outputString("SMU    = Explicit smoothing coefficient O(1)");
      gi.outputString("TIM    = TIM factor for smoothing in marching direction (0<=TIM<=3)");
      gi.outputString("ITSVOL = Number of times prescribed areas DAREA(J) is smoothed");
      gi.inputString(line,sPrintF(buff,"Enter smu,tim,itsvol (currrent values = %e, %e, %i)",
          smu(0),tim(0),itsvol(0)));
      if( line!="" ) sScanF( line,"%e %e %i",&smu(0),&tim(0),&itsvol(0));
    }
    else if( answer=="spacing (ETAMX,DETA,DFAR)" )
    {
      gi.outputString(" ETAMX = Far field distance");
      gi.outputString(" DETA  = First grid point spacing in marching direction from initial curve");
      gi.outputString("           (or 0 for no spacing control)");
      gi.outputString(" DFAR  = Last grid point spacing in marching direction from initial curve");
      gi.outputString("           (or 0 for no spacing control)");
      gi.inputString(line,sPrintF(buff,"Enter etamx,deta,dfar (current=%e,%e,%e)",etamx(0,0),deta(0,0),dfar(0,0)));
      if( line!="" ) sScanF( line,"%e %e &e",&etamx(0,0),&deta(0,0),&dfar(0,0));
    }
    else if( answer=="boundary condition (IBCJA,IBCJB)" )
    {
      gi.outputString("    Boundary condition types for J=1");
      gi.outputString("    IBCJA  = -1 Free floating condition");
      gi.outputString("           =  1 Constant plane condition in X, float Y and Z");
      gi.outputString("           =  2 Constant plane condition in Y, float X and Z");
      gi.outputString("           =  3 Constant plane condition in Z, float X and Y");
      gi.outputString("           =  4 Reflected symmetry condition in X");
      gi.outputString("           =  5 Reflected symmetry condition in Y");
      gi.outputString("           =  6 Reflected symmetry condition in Z");
      gi.outputString("           = 10 Periodic condition (*)");
      gi.outputString("           = 11 Floating condition along KS=const line in +JS direction");
      gi.outputString("           = 12 Floating condition along KS=const line in -JS direction");
      gi.outputString("           = 13 Floating condition along JS=const line in +KS direction");
      gi.outputString("           = 14 Floating condition along JS=const line in -KS direction");
      gi.outputString("           = 15 Floating condition along user supplied line of points");
      gi.outputString("           = 20 Exact coordinates of boundary points prescribed by user");
      gi.outputString("           = 21 Constant X plane for all J from 1 to JMAX (*)");
      gi.outputString("           = 22 Constant Y plane for all J from 1 to JMAX (*)");
      gi.outputString("           = 23 Constant Z plane for all J from 1 to JMAX (*)");
      gi.outputString("           = 31 Float along KS=const line in +JS direction for all J (*)");
      gi.outputString("           = 32 Float along KS=const line in -JS direction for all J (*)");
      gi.outputString("           = 33 Float along JS=const line in +KS direction for all J (*)");
      gi.outputString("           = 34 Float along JS=const line in -KS direction for all J (*)");
      gi.outputString(" ");
      gi.outputString("    (*) Must also apply for IBCJB");
      gi.outputString(" ");
      gi.outputString("    Similarly for IBCJB at J=JMAX");

      gi.inputString(line,sPrintF(buff,"Enter ibcja, ibcjb, (current =%i,%i)",
                     ibcja(0),ibcjb(0)));
      if( line!="" ) sScanF( line,"%i %i ",&ibcja(0), &ibcjb(0));
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }
  }


---- */







}
