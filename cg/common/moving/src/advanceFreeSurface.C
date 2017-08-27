#include "DeformingBodyMotion.h"
#include "DeformingGrid.h"
#include "DeformingGridGenerationInformation.h"
#include "DomainSolver.h"
#include "NurbsMapping.h"

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define FOR_SIDE(side,axis )for( int axis=0; axis<=1; axis++ )for( int side=0; side<=1; side++ ) 

// ================================================================================================
/// \brief Smooth the interface.
// ================================================================================================
int DeformingBodyMotion::
smoothInterface( RealArray & x0, CompositeGrid & cg, 
                 const int sideToMove, const int axisToMove, const int gridToMove, const real *vScale )
{
  const bool & smoothSurface = deformingBodyDataBase.get<bool>("smoothSurface");
  if( !smoothSurface )
  {
    return 0;
  }
  

  const int numberOfDimensions = cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  const int & numberOfSurfaceSmooths = deformingBodyDataBase.get<int>("numberOfSurfaceSmooths");

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");



  // -- Add a fourth-order filter to the new positions  --
  if( numberOfDimensions==2  )
  {
    printF("--DBM-- smoothInterface: smooth the boundary curve, numberOfSurfaceSmooths=%i "
           " (4th order filter) bc=[%i,%i]...\n",
           numberOfSurfaceSmooths,boundaryCondition(0,0),boundaryCondition(1,0));
	    
    int axisp = (axisToMove + 1) % numberOfDimensions;
    Index I1=x0.dimension(0), I2=x0.dimension(1);
            
    const int base=x0.getBase(0), bound=x0.getBound(0);
    RealArray x1(Range(base-2,bound+2),2);  // add 2 ghost points
    Range R2(0,1);

    if( vScale[0]==0. )
      R2=Range(1,1);
    if( vScale[1]==0. )
      R2=Range(0,0);
	    
    x1(I1,R2)=x0(I1,R2);

    // I : smooth these points. 
    const int i1a= boundaryCondition(0,0)==dirichletBoundaryCondition ? base+1 : base;
    const int i1b= boundaryCondition(1,0)==dirichletBoundaryCondition ? bound-1 : bound;
    Range I(i1a,i1b);

    // -- check if the grid is periodic in the tangential direction ---
    const int isPeriodic = cg[gridToMove].isPeriodic(axisp);
    real dist[2]={0.,0.};    // for derivative-periodic : length of periodic interval 
    dist[axisp]= x0(bound,axisp)-x0(base,axisp);  // *check me*

    if( true || isPeriodic==Mapping::derivativePeriodic )
      printF("--DBM-- isPeriodic=%i dist=%9.3e\n",isPeriodic,dist[axisp]);
            

    real omega=.5;	    
    for( int smooth=0; smooth<numberOfSurfaceSmooths; smooth++ )
    {
      // -- boundary conditions  **FIX ME for derivative periodic**
      int side=0;
      if( boundaryCondition(side,0)==dirichletBoundaryCondition )
      {
        // Dirichlet BC on left
        x1(base-1,R2) = 2.*x1(base,R2)-x1(base+1,R2);  // what should this be ??
        x1(base-2,R2) = 2.*x1(base,R2)-x1(base+2,R2);
      }
      else if( boundaryCondition(side,0)==neumannBoundaryCondition ||
               boundaryCondition(side,0)==slideBoundaryCondition )
      {
        // Neumann BC on left: 
        x1(base-1,R2) = x1(base+1,R2);  // what should this be ??
        x1(base-2,R2) = x1(base+2,R2);
      }
      else if( boundaryCondition(side,0)==periodicBoundaryCondition )
      {
        // Periodic: 
        x1(base-1,R2) = x1(bound-1,R2);
        x1(base-2,R2) = x1(bound-2,R2);
        if( isPeriodic==Mapping::derivativePeriodic )
        {
          // derivative is periodic -- include spatial shift 
          x1(base-1,axisp) -= dist[axisp];
          x1(base-2,axisp) -= dist[axisp];
        }                
      }
      else
      {
        OV_ABORT("ERROR: unknown BC for advect-body");
      }
	      
      // BC on right: 
      side=1;
      if( boundaryCondition(side,0)==dirichletBoundaryCondition )
      {
        // Dirichlet BC on right
        x1(bound+1,R2) = 2.*x1(bound,R2)-x1(bound-1,R2);
        x1(bound+2,R2) = 2.*x1(bound,R2)-x1(bound-2,R2);
      }
      else if( boundaryCondition(side,0)==neumannBoundaryCondition ||
               boundaryCondition(side,0)==slideBoundaryCondition )
      {
        // Neumann BC on right
        x1(bound+1,R2) = x1(bound-1,R2);
        x1(bound+2,R2) = x1(bound-2,R2);
      }
      else if( boundaryCondition(side,0)==periodicBoundaryCondition )
      {
        // Periodic:
        x1(bound  ,R2) = x1(base  ,R2);
        x1(bound+1,R2) = x1(base+1,R2);
        x1(bound+2,R2) = x1(base+2,R2);
        if( isPeriodic==Mapping::derivativePeriodic )
        { // derivative periodic 
          x1(bound  ,axisp) += dist[axisp];
          x1(bound+1,axisp) += dist[axisp];
          x1(bound+2,axisp) += dist[axisp];
        }
                
      }
      else
      {
        OV_ABORT("ERROR: unknown BC for advect-body");
      }

	      
      // smooth interior: 
	      
      x1(I,R2)= x1(I,R2) + (omega/16.)*(-x1(I-2,R2) + 4.*x1(I-1,R2) -6.*x1(I,R2) + 4.*x1(I+1,R2) -x1(I+2,R2) );
	      
    } // end smooths
	    
    x0(I1,R2)= x1(I1,R2);

  } 
  else if( numberOfDimensions==3 )
  {
    // =========== SMOOTH INTERFACE in THREE DIMENSIONS =============

    printF("smoothInterface: numberOfSurfaceSmooths=%i (4th order filter)...\n",
           numberOfSurfaceSmooths);
	    
    Index I1=x0.dimension(0), I2=x0.dimension(1);
    const int axisp1 = (axisToMove + 1) % numberOfDimensions;
    const int axisp2 = (axisToMove + 2) % numberOfDimensions;

    const int base0=x0.getBase(0), bound0=x0.getBound(0);
    const int base1=x0.getBase(1), bound1=x0.getBound(1);
    IntegerArray gid(2,3);
    gid(0,0)=base0; gid(1,0)=bound0;
    gid(0,1)=base1; gid(1,1)=bound1;
	    
    const int numGhost=2;   // add 2 ghost points
    RealArray x1(Range(base0-numGhost,bound0+numGhost),
                 Range(base1-numGhost,bound1+numGhost),numberOfDimensions);
    Range Rx=numberOfDimensions;

    if( vScale[0]==0. && vScale[1]==0. )
    {
      Rx=Range(2,2);
    }
    else if( vScale[0]==0. && vScale[2]==0. )
    {
      Rx=Range(1,1);
    }
    else if( vScale[1]==0. && vScale[2]==0. )
    {
      Rx=Range(0,0);
    }
			 
    x1(I1,I2,Rx)=x0(I1,I2,Rx);

    // Smooth these points. 
    const int i1a= boundaryCondition(0,0)==dirichletBoundaryCondition ? base0+1 : base0;
    const int i1b= boundaryCondition(1,0)==dirichletBoundaryCondition ? bound0-1 : bound0;
    Range J1(i1a,i1b);

    const int i2a= boundaryCondition(0,1)==dirichletBoundaryCondition ? base1+1 : base1;
    const int i2b= boundaryCondition(1,1)==dirichletBoundaryCondition ? bound1-1 : bound1;
    Range J2(i2a,i2b);

    int isv[2], &is1=isv[0], &is2=isv[1];

    real omega=.5;
    for( int smooth=0; smooth<numberOfSurfaceSmooths; smooth++ )
    {
      // -- boundary conditions  **FIX ME for derivative periodic**
      FOR_SIDE(side,axis)
      {
        is1=0; is2=0; isv[axis]=1-2*side;
        Index Jbv[2], &Jb1=Jbv[0], &Jb2=Jbv[1];
        Jb1=I1; Jb2=I2;
        Jbv[axis] = gid(side,axis);
		
        if( boundaryCondition(side,axis)==dirichletBoundaryCondition )
        {
          // Dirichlet BC on left
          // what should this be ??
          x1(Jb1-  is1,Jb2-  is2,Rx) = 2.*x1(Jb1,Jb2,Rx)-x1(Jb1+  is1,Jb2+  is2,Rx);  
          x1(Jb1-2*is1,Jb2-2*is2,Rx) = 2.*x1(Jb1,Jb2,Rx)-x1(Jb1+2*is1,Jb2+2*is2,Rx);
        }
        else if( boundaryCondition(side,axis)==neumannBoundaryCondition ||
                 boundaryCondition(side,axis)==slideBoundaryCondition )
        {
          // Neumann BC on left: 
          x1(Jb1-  is1,Jb2-  is2,Rx) = x1(Jb1+  is1,Jb2+  is2,Rx);  // what should this be ??
          x1(Jb1-2*is1,Jb2-2*is2,Rx) = x1(Jb1+2*is1,Jb2+2*is2,Rx);
        }
        else if( boundaryCondition(side,axis)==periodicBoundaryCondition )
        {
          // Periodic: FIX ME for derivative periodic **
          const int ia=gid(0,axis), ib=gid(1,axis);
          if( axis==0 )
          {
            x1(ia-1,Jb2,Rx) = x1(ib-1,Jb2,Rx);
            x1(ia-2,Jb2,Rx) = x1(ib-2,Jb2,Rx);
		    
            x1(ib  ,Jb2,Rx) = x1(ia  ,Jb2,Rx);
            x1(ib+1,Jb2,Rx) = x1(ia+1,Jb2,Rx);
            x1(ib+2,Jb2,Rx) = x1(ia+2,Jb2,Rx);
          }
          else if( axis==1 )
          {
            x1(Jb1,ia-1,Rx) = x1(Jb1,ib-1,Rx);
            x1(Jb1,ia-2,Rx) = x1(Jb1,ib-2,Rx);

            x1(Jb1,ib  ,Rx) = x1(Jb1,ia  ,Rx);
            x1(Jb1,ib+1,Rx) = x1(Jb1,ia+1,Rx);
            x1(Jb1,ib+2,Rx) = x1(Jb1,ia+2,Rx);
          }
          else
          {
            OV_ABORT("--DBM-- This should not happen");
          }
		  
        }
        else
        {
          OV_ABORT("--DBM-- ERROR: unknown BC for freeSurface motion");
        }
      }
	      

      // smooth interior: 
      x1(I1,I2,Rx)= x1(I1,I2,Rx) + (omega/32.)*(
        -x1(I1-2,I2,Rx) + 4.*x1(I1-1,I2,Rx) -6.*x1(I1,I2,Rx) + 4.*x1(I1+1,I2,Rx) -x1(I1+2,I2,Rx) 
        -x1(I1,I2-2,Rx) + 4.*x1(I1,I2-1,Rx) -6.*x1(I1,I2,Rx) + 4.*x1(I1,I2+1,Rx) -x1(I1,I2+2,Rx) 
        );
	      
    } // end smooths
	    
    x0(I1,I2,Rx)= x1(I1,I2,Rx); // what about ghost pts??

  }
  else
  {
    OV_ABORT("smoothInterface:ERROR: unknown number of dimensions");
    
  }
  
  return 0;

}



// ===========================================================================================
/// \brief Advance a free surface
///
/// \param advanceOption : 0 = predictor-step, 1 =corrector step. 
///
/// The free surface is advected with the fluid velocity: 
///
///    dx/dt = v 
///
/// Note: The surface tension and gravity effects apear in the pressure boundary condition:
///        p = p_a  + n.tau.n + gamma*kappa 
/// 
/// \param t1,t2,t3 : 
///      For advanceOption=0 : t1=old-time, t3=new-time, t2=grid-velocity is provided from this time.
///      For advanceOption=1 : Correct using times t1=tOld and t2=tNew (t3=t2, cgf3==cgf2)
/// 
// ===========================================================================================
int DeformingBodyMotion::
advanceFreeSurface(real t1, real t2, real t3, 
                   GridFunction & cgf1,
                   GridFunction & cgf2,
                   GridFunction & cgf3,
                   int advanceOption )
{
  int ierr=0;

  const int numberOfDimensions = cgf1.cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  const bool & smoothSurface = deformingBodyDataBase.get<bool>("smoothSurface");
  const int & numberOfSurfaceSmooths = deformingBodyDataBase.get<int>("numberOfSurfaceSmooths");

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  assert( deformingBodyType==userDefinedDeformingBody );
  assert( userDefinedDeformingBodyMotionOption==freeSurface );

  CompositeGrid & cg = cgf3.cg;

  real tForce=t2;
  real tNew=t3;
  // We need to grab the surface points from cgf1
  // const real dt = advanceOption==0 ?  t3-t2 : t2-t1; 
  const real dt = t3-t1;
  
  if( debug & 2 || t3 < 5.*dt )
  {
    printF("\n >>>>>>> advanceFreeSurface: %s tNew=%f, dt=%9.3e\n",
           (advanceOption==0 ? "PREDICTOR" : "CORRECTOR"),tNew,dt);
    printF("  advanceOption=%i: t1=%9.3e, t2=%9.3e, t3=%9.3e cg1.t=%9.3e cgf2.t=%9.3e cgf3.t=%9.3e\n",
           advanceOption,t1,t2,t3,cgf1.t,cgf2.t,cgf3.t);
    
  }
  
  // if( advanceOption==1 )
  // {
  //   printF(">>>>>>> advanceFreeSurface t2=%9.3e, t3=%9.3e -- CORRECTION STEP  **FINISH ME** \n",t2,t2);
  //   return 0;
  // }

  Index Ib1,Ib2,Ib3;

  // --------------------------------------
  // --- LOOP over faces on the surface ---
  // --------------------------------------
  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 

    // ---- FREE SURFACE: 
    // advect the surface from time t1 to t3 using velocity from time t2 

    const int uc = parameters.dbase.get<int >("uc");
    const int vc = parameters.dbase.get<int >("vc");
    const int wc = parameters.dbase.get<int >("wc");
    const int pc = parameters.dbase.get<int >("pc");

    // Extract the arrays that hold the interface position at different times
    vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
    vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
    assert( face<surfaceArray.size() );
    RealArray *px = surfaceArray[face];

    // New:
    //   xPrev = x(t-dt)
    //   xCur = x(t   )
    //   xNew = x(t+dt)
    //  
    // currentFreeSurface[face] = integer index into the px[] array indicating current time level 
    vector<int> & currentFreeSurface =  deformingBodyDataBase.get<vector<int> >("currentFreeSurface");

    const int numberOfTimeLevelsForFreeSurface=deformingBodyDataBase.get<int>("numberOfTimeLevelsForFreeSurface");
    int & cur = currentFreeSurface[face];
    const int prev = ovmod(cur-1,numberOfTimeLevelsForFreeSurface);
    const int next = ovmod(cur+1,numberOfTimeLevelsForFreeSurface);

    RealArray & xCur  = px[cur];
    RealArray & xPrev = px[prev];
    RealArray & xNext = px[next];
    
    real & tCur = surfaceArrayTime[face][cur];
    real & tNext= surfaceArrayTime[face][next];
    tNext=t3;
    printF("--> freeSurface: prev=%i cur=%i next=%i , tCur=%9.3e tNext=%9.3e\n",prev,cur,next,tCur,tNext);
    

    // const real dt = t3-tx0;   
    // tx0=t3; // x0 will now live at this time


    realArray & u    = cgf2.u[gridToMove];
    realArray & uNew = cgf3.u[gridToMove];  // for corrector, this is the updated solution
	
    Index Ib1,Ib2,Ib3;
    getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);

    real *par = deformingBodyDataBase.get<real [10]>("freeSurfaceParameters");
    real & surfaceTension = par[0];
    aString & surfaceGridMotion = deformingBodyDataBase.get<aString>("surfaceGridMotion");

    BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");


    // --- Option to restrict the motion -- *fix me* Make this a run time option
    real vScale[3]={1.,1.,1.};  // scale velocity by these factors 
    if( surfaceGridMotion=="restrict to x direction" )
    {
      vScale[1]=vScale[2]=0.;
    }
    else if( surfaceGridMotion=="restrict to y direction" )
    {
      vScale[0]=vScale[2]=0.;
    }
    else if( surfaceGridMotion=="restrict to z direction" )
    {
      vScale[0]=vScale[1]=0.;
    }
    else if( surfaceGridMotion=="free motion" )
    {
    }
    else
    {
      printF("ERROR: unknown surfaceGridMotion=[%s]\n",(const char*)surfaceGridMotion);
      OV_ABORT("error");
    }
	
    if( numberOfDimensions==2 )
    {
      assert( uc>=0 && vc>=0 );
      int axisp = (axisToMove + 1) % numberOfDimensions;
      Index I1=xCur.dimension(0), I2=xCur.dimension(1);
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
      {
        int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
        if( advanceOption==0 )
        {
          // --- predictor --- 
          if( true )
          { // Leap-frog 
            xNext(j1,0) = xPrev(j1,0) + 2.*dt*u(i1,i2,i3,uc)*vScale[0];
            xNext(j1,1) = xPrev(j1,1) + 2.*dt*u(i1,i2,i3,vc)*vScale[1];
          }
          else
          {
            // forward-Euler 
            xNext(j1,0) = xCur(j1,0) + dt*u(i1,i2,i3,uc)*vScale[0];
            xNext(j1,1) = xCur(j1,1) + dt*u(i1,i2,i3,vc)*vScale[1];
          }
          
        }
        else
        {
          // --- corrector ---
          // Trapezoidal rule: Note: xCur is now the solution at the new time
          xCur(j1,0) = xPrev(j1,0) + .5*dt*( u(i1,i2,i3,uc)+uNew(i1,i2,i3,uc) )*vScale[0];
          xCur(j1,1) = xPrev(j1,1) + .5*dt*( u(i1,i2,i3,vc)+uNew(i1,i2,i3,vc) )*vScale[1];
        }
        
        if( false )
          printF("--DBM-- freeSurface: xCur=(%5.2f,%5.2f) u=(%5.2f,%5.2f) xNext=(%5.2f,%5.2f)\n",
                  xCur(j1,0),xCur(j1,1),u(i1,i2,i3,uc),u(i1,i2,i3,vc),xNext(j1,0),xNext(j1,1));
	    
      }

      // -- Optionally smooth the interface ---
      smoothInterface( xNext, cg, sideToMove, axisToMove, gridToMove, vScale );

    }
    else if( numberOfDimensions==3)
    {
      //    *******************************************************
      //    ************* 3D free surface movement ****************
      //    *******************************************************
      assert( uc>=0 && vc>=0 && wc>= 0 );

      Index I1=xCur.dimension(0), I2=xCur.dimension(1);
      const int axisp1 = (axisToMove + 1) % numberOfDimensions;
      const int axisp2 = (axisToMove + 2) % numberOfDimensions;
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
      {
        int j1 = iv[axisp1];  // j1 = i1 or i2 or i3 
        int j2 = iv[axisp2];  // j1 = i1 or i2 or i3 

        // **CHECK ME***
        if( advanceOption==0 )
        {
          // --- predictor --- 
          if( true )
          { // Leap-frog 
            xNext(j1,j2,0) = xPrev(j1,j2,0) + 2.*dt*u(i1,i2,i3,uc)*vScale[0];
            xNext(j1,j2,1) = xPrev(j1,j2,1) + 2.*dt*u(i1,i2,i3,vc)*vScale[1];
            xNext(j1,j2,2) = xPrev(j1,j2,2) + 2.*dt*u(i1,i2,i3,wc)*vScale[2];
          }
          else
          {
            // forward-Euler 
            xNext(j1,j2,0) = xCur(j1,j2,0) + dt*u(i1,i2,i3,uc)*vScale[0];
            xNext(j1,j2,1) = xCur(j1,j2,1) + dt*u(i1,i2,i3,vc)*vScale[1];
            xNext(j1,j2,2) = xCur(j1,j2,2) + dt*u(i1,i2,i3,wc)*vScale[2];
          }
          
        }
        else
        {
          // --- corrector ---
          // Trapezoidal rule: 
          xCur(j1,j2,0) = xPrev(j1,j2,0) + .5*dt*( u(i1,i2,i3,uc)+uNew(i1,i2,i3,uc) )*vScale[0];
          xCur(j1,j2,1) = xPrev(j1,j2,1) + .5*dt*( u(i1,i2,i3,vc)+uNew(i1,i2,i3,vc) )*vScale[1];
          xCur(j1,j2,2) = xPrev(j1,j2,2) + .5*dt*( u(i1,i2,i3,wc)+uNew(i1,i2,i3,wc) )*vScale[2];
        }

        // real u0=u(i1,i2,i3,uc)*vScale[0], v0=u(i1,i2,i3,vc)*vScale[1], w0=u(i1,i2,i3,wc)*vScale[2];

        // xNext(j1,j2,0) = xCur(j1,0) + dt*u0;
        // xNext(j1,j2,1) = xCur(j1,1) + dt*v0;
        // xNext(j1,j2,2) = xCur(j1,2) + dt*w0;

      }
	  
      // -- Optionally smooth the interface ---
      smoothInterface( xNext, cg, sideToMove, axisToMove, gridToMove, vScale );

    }
	
    // The "surface" Mapping holds the start curve in 2D
    vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
    assert( face<surface.size() );
    NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);

    if( advanceOption==0 )
    {
      cur = next; // increment time-level after predictor stage
    }
    

#ifdef USE_PPP
    OV_ABORT("fix me");
#else
    int option=0, degree=3;
    const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
    startCurve.interpolate(xNext,option,Overture::nullRealDistributedArray(),degree,
                           (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
#endif

  
    
// --------- OLD ------

//     // The "surface" Mapping holds the start curve in 2D
//     vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
//     assert( face<surface.size() );
//     NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);


//     // The undeformed surface, x0,  is stored here: 
//     vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
//     vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
//     assert( face<surfaceArray.size() );
//     RealArray *px = surfaceArray[face];
//     RealArray &x0 = px[0];
//     assert( face<surfaceArrayTime.size() );
//     real & tx0= surfaceArrayTime[face][0];

//     const int numGhost=1;  // include ghost points 
//     Range Rx=numberOfDimensions;

//     // --- The current surface position and velocity are stored in the GridFunction data-base ---
//     for( int m=0; m<3; m++ )
//     {
//       GridFunction & gf = m==0 ? cgf1 : m==1 ? cgf2 : cgf3;
//       if( !gf.dbase.has_key("xShell") )
//       {
// 	// note: include ghost points:
// 	getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
// 	gf.dbase.put<RealArray>("xShell");
// 	gf.dbase.put<RealArray>("vShell");

// 	RealArray & x = gf.dbase.get<RealArray>("xShell");
// 	RealArray & v = gf.dbase.get<RealArray>("vShell");
 
// 	gf.cg[gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now 
// #ifdef USE_PPP
// 	RealArray vertex; getLocalArrayWithGhostBoundaries(gf.cg[gridToMove].vertex(),vertex);
// #else
// 	RealArray & vertex = gf.cg[gridToMove].vertex();
// #endif
// 	x= vertex(Ib1,Ib2,Ib3,Rx);
// 	v.redim(Ib1,Ib2,Ib3,Rx);

// 	v=0.;                       // do this for now   ** fix me **
//       }
//     }
	
//     getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);

//     RealArray & x1 = cgf1.dbase.get<RealArray>("xShell");
//     RealArray & v1 = cgf1.dbase.get<RealArray>("vShell");
	
//     RealArray & x2 = cgf2.dbase.get<RealArray>("xShell");
//     RealArray & v2 = cgf2.dbase.get<RealArray>("vShell");
	
//     RealArray & x3 = cgf3.dbase.get<RealArray>("xShell");
//     RealArray & v3 = cgf3.dbase.get<RealArray>("vShell");
	

//     // RealArray x1,x2,x3, v1,v2,v3;

// #ifdef USE_PPP
//     RealArray u2; getLocalArrayWithGhostBoundaries(cgf2.u[gridToMove],u2);
//     const RealArray & normal2 = cgf2.cg[gridToMove].vertexBoundaryNormalArray(sideToMove,axisToMove);
//     RealArray vertex2; getLocalArrayWithGhostBoundaries(cgf2.cg[gridToMove].vertex(),vertex2);
// #else
//     RealArray & u2 = cgf2.u[gridToMove];
//     const RealArray & normal2 = cgf2.cg[gridToMove].vertexBoundaryNormal(sideToMove,axisToMove);
//     const RealArray & vertex2 = cgf2.cg[gridToMove].vertex();
// #endif
	
//     if( numberOfDimensions==2 )
//     {
//       assert( uc>=0 && vc>=0 && pc>=0 );
//       int axisp = (axisToMove + 1) % numberOfDimensions;  // axis in the tangential direction 
//       // Index I1=x0.dimension(0), I2=x0.dimension(1);
//       int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
//       int ivp[3], &i1p=ivp[0], &i2p=ivp[1], &i3p=ivp[2];
//       int ivm[3], &i1m=ivm[0], &i2m=ivm[1], &i3m=ivm[2];
//       FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
//       {
// 	// int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
// 	i1p=i1, i2p=i2, i3p=i3;
// 	i1m=i1, i2m=i2, i3m=i3;
// 	ivp[axisp]+=1;
// 	ivm[axisp]-=1;
// 	real p2=u2(i1,i2,i3,pc);
	    
// 	// compute the local arc-length (use vertex2 since it has ghost points)
// 	real ds = .5*( sqrt( SQR(vertex2(i1p,i2p,i3p,0)-vertex2(i1m,i2m,i3m,0))+
// 			     SQR(vertex2(i1p,i2p,i3p,1)-vertex2(i1m,i2m,i3m,1)) ) );

// 	real ac[3]; // holds acceleration 
// 	for( int dir=0; dir<numberOfDimensions; dir++ )
// 	{
// 	  // Curvature term: dxss = (x-x0).ss 
// 	  // ** we need ghost point values for this ***
// 	  real dxss =( (vertex2(i1p,i2p,i3p,dir)-2.*vertex2(i1,i2,i3,dir)+vertex2(i1m,i2m,i3m,dir)) - 
// 		       (x0(i1p,i2p,i3p,dir)-2.*x0(i1,i2,i3,dir)+x0(i1m,i2m,i3m,dir)) )/(ds*ds);

// 	  // smoothing terms -- note: for "leap-frog" we should lag the dissipation terms *wdh* 100828
// 	  //real vss = v2(i1p,i2p,i3p,dir)-2.*v2(i1,i2,i3,dir)+v2(i1m,i2m,i3m,dir);
// 	  //real xss = x2(i1p,i2p,i3p,dir)-2.*x2(i1,i2,i3,dir)+x2(i1m,i2m,i3m,dir);
// 	  real vss = v1(i1p,i2p,i3p,dir)-2.*v1(i1,i2,i3,dir)+v1(i1m,i2m,i3m,dir);
// 	  real xss = x1(i1p,i2p,i3p,dir)-2.*x1(i1,i2,i3,dir)+x1(i1m,i2m,i3m,dir);

// 	  ac[dir]=( (p2-volumePenaltyForce)*normal2(i1,i2,i3,dir) -(ke/ds)*( x2(i1,i2,i3,dir)-x0(i1,i2,i3,dir) ) 
// 		    + te*dxss -be*v2(i1,i2,i3,dir) )/(rhoe) + ad2*vss;
// 	  v3(i1,i2,i3,dir) = v1(i1,i2,i3,dir) + 2.*dt*ac[dir];
// 	  x3(i1,i2,i3,dir) = x1(i1,i2,i3,dir) + 2.*dt*( v2(i1,i2,i3,dir) +ad2*xss ) ;
// 	}
// 	if( false )
// 	{
// 	  printF("elasticShell: x=(%5.2f,%5.2f) x0=(%5.2f,%5.2f) ds=%8.2e v=(%5.2f,%5.2f) a=(%8.2e,%8.2e) "
// 		 "(u,v,p)=(%8.2e,%8.2e,%8.2e)\n",
// 		 x3(i1,i2,i3,0),x3(i1,i2,i3,1),x0(i1,i2,i3,0),x0(i1,i2,i3,1),ds, 
// 		 v3(i1,i2,i3,0),v3(i1,i2,i3,1),ac[0],ac[1],u2(i1,i2,i3,uc),u2(i1,i2,i3,vc),p2);
	      
// 	}
//       } // end for
      

//       // 2013/03/18 -- fix periodicity
 
//       // --- Boundary Conditions ---

//       const int axisp1 = (axisToMove+1) % numberOfDimensions;  // tangential direction
      
//       for( int side=0; side<=1; side++ ) // loop over sides
//       {

// //	if( side==0 && boundaryCondition(side,axisp1)==periodicBoundaryCondition )
// 	if( boundaryCondition(side,axisp1)==periodicBoundaryCondition )
// 	{
// 	  // printF("advanceElasticShell: t=%g, BC=periodic!\n",t2);

//           // *wdh* TURN OFF 2013/08/03 -- wrong for derivativePeriodic
// 	  // startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic); 

// 	  if( axisToMove==1 )
// 	  {
// 	    // boundary: i1=i1a,i1a+1,...,i1b
// 	    const int i2 =Ib2.getBase(), i3 =Ib3.getBase();
// 	    const int i1a=Ib1.getBase(), i1b=Ib1.getBound();
// 	    for( int dir=0; dir<numberOfDimensions; dir++ )
// 	    {
// 	      // Set periodic images to be the same:
// 	      real v3Ave = .5*(v3(i1a,i2,i3,dir)+v3(i1b,i2,i3,dir));
// 	      v3(i1a,i2,i3,dir) = v3Ave;
// 	      v3(i1b,i2,i3,dir) = v3Ave;

// 	      real x3Ave = .5*(x3(i1a,i2,i3,dir)+x3(i1b,i2,i3,dir));
// 	      x3(i1a,i2,i3,dir) = x3Ave;
// 	      x3(i1b,i2,i3,dir) = x3Ave;
	  
// 	      // set ghost values by periodicity
// 	      v3(i1a-1,i2,i3,dir)=v3(i1b-1,i2,i3,dir);
// 	      v3(i1b+1,i2,i3,dir)=v3(i1a+1,i2,i3,dir);
	  
// 	      x3(i1a-1,i2,i3,dir)=x3(i1b-1,i2,i3,dir);
// 	      x3(i1b+1,i2,i3,dir)=x3(i1a+1,i2,i3,dir);

// 	    }
// 	  }
// 	  else
// 	  {
// 	    // boundary: i2=i2a,i2a+1,...,i2b
// 	    assert( axisToMove==0 );
// 	    const int i1 =Ib1.getBase(), i3 =Ib3.getBase();
// 	    const int i2a=Ib2.getBase(), i2b=Ib2.getBound();
// 	    for( int dir=0; dir<numberOfDimensions; dir++ )
// 	    {
// 	      // Set periodic images to be the same:
// 	      real v3Ave = .5*(v3(i1,i2a,i3,dir)+v3(i1,i2b,i3,dir));
// 	      v3(i1,i2a,i3,dir) = v3Ave;
// 	      v3(i1,i2b,i3,dir) = v3Ave;

// 	      real x3Ave = .5*(x3(i1,i2a,i3,dir)+x3(i1,i2b,i3,dir));
// 	      x3(i1,i2a,i3,dir) = x3Ave;
// 	      x3(i1,i2b,i3,dir) = x3Ave;
	  
// 	      // set ghost values by periodicity
// 	      v3(i1,i2a-1,i3,dir)=v3(i1,i2b-1,i3,dir);
// 	      v3(i1,i2b+1,i3,dir)=v3(i1,i2a+1,i3,dir);
	  
// 	      x3(i1,i2a-1,i3,dir)=x3(i1,i2b-1,i3,dir);
// 	      x3(i1,i2b+1,i3,dir)=x3(i1,i2a+1,i3,dir);

// 	    }
	

// 	  }
// 	} // end if periodic BC
// 	else if( boundaryCondition(side,axisp1)==dirichletBoundaryCondition )
// 	{
// 	  // printF("advanceElasticShell: t=%g, side=%i, BC=dirichlet!\n",t2,side);


// 	  // --- Dirichlet boundary conditions ---
// 	  const int i3 =Ib3.getBase();
// 	  const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
// 	  const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
// 	  const int is1 = axisp1==0 ? 1-2*side : 0; 
// 	  const int is2 = axisp1==1 ? 1-2*side : 0;
	  
// 	  for( int dir=0; dir<numberOfDimensions; dir++ )
// 	  {
//             // Dirichlet BC: 
// 	    x3(i1,i2,i3,dir) = x1(i1,i2,i3,dir);
// 	    v3(i1,i2,i3,dir) = 0.;
	
// 	    // ghost points: extrapolate: x.ss=0 
// 	    x3(i1-is1,i2-is2,i3,dir) = 2.*x3(i1,i2,i3,dir) - x3(i1+is1,i2+is2,i3,dir);
// 	    v3(i1-is1,i2-is2,i3,dir) = 2.*v3(i1,i2,i3,dir) - v3(i1+is1,i2+is2,i3,dir);
	  
// 	  }
// 	}
// 	else if( boundaryCondition(side,axisp1)==slideBoundaryCondition )
// 	{
// 	  // printF("advanceElasticShell: t=%g, side=%i, BC=slide!\n",t2,side);


// 	  // --- Slide boundary condition ---
// 	  const int i3 =Ib3.getBase();
// 	  const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
// 	  const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
// 	  const int is1 = axisp1==0 ? 1-2*side : 0; 
// 	  const int is2 = axisp1==1 ? 1-2*side : 0;
	  
// 	  for( int dir=0; dir<numberOfDimensions; dir++ )
// 	  {
//             // slide: 
//             if( dir==axisp1 )
// 	    {
//   	      x3(i1,i2,i3,dir) = x1(i1,i2,i3,dir);
// 	      v3(i1,i2,i3,dir) = 0.;
// 	    }
	    
// 	    // ghost points: extrapolate: x.ss=0 
// 	    x3(i1-is1,i2-is2,i3,dir) = 2.*x3(i1,i2,i3,dir) - x3(i1+is1,i2+is2,i3,dir);
// 	    v3(i1-is1,i2-is2,i3,dir) = 2.*v3(i1,i2,i3,dir) - v3(i1+is1,i2+is2,i3,dir);
	  
// 	  }
// 	}
// 	else
// 	{
// 	  // --- un-implemented BC ---
// 	  OV_ABORT("elasticShell: un-implemented BC: FINISH ME BILL!");

// 	}
	
//       } // end for side
      
//       x3.reshape(x3.dimension(axisp),Rx);

//       #ifdef USE_PPP
// 	Overture::abort("fix me");
//       #else
//       int option=0, degree=3;
//       const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
//       startCurve.interpolate(x3,option,Overture::nullRealDistributedArray(),degree,
//                              (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization,numGhost);
//       // 081107 startCurve.interpolate(x3,option,Overture::nullRealDistributedArray(),degree,
//       //                               NurbsMapping::parameterizeByChordLength,numGhost);
//       #endif

//       getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
//       x3.reshape(Ib1,Ib2,Ib3,Rx);

//     }
//     else if( numberOfDimensions==3)
//     {
//       Overture::abort("error");
//     }
	
//     tx0=t3; // x0 points now live at this time
  
    
  } // end for face
  
  return ierr;
}
