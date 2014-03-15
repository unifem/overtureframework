#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "PenaltySlipWallBC.h"
#include "InsParameters.h"
#include "GUIState.h"
#include "kkcdefs.h"

namespace {
  const int parallelBCFlag = -31415926;
}

Parameters::BCModifier *createPenaltySlipWallBC(const aString &name)
{
  return new PenaltySlipWallBC(name);
}

  
PenaltySlipWallBC::
PenaltySlipWallBC(const aString &nm) : BCModifier(nm), normalFlux(0.0), zeroTangentialVelocity(false) {}

PenaltySlipWallBC::
   ~PenaltySlipWallBC() {}

bool 
PenaltySlipWallBC::
inputFromGI(GenericGraphicsInterface &gi)
{
  GUIState gui;
  gui.setExitCommand("done","done");

  aString txtBox[] = {"normal component",""};
  aString txtCmd[] = {"nComponent",""};
  aString txtValues[] = {"0.0",""};

  gui.setTextBoxes(txtCmd,txtBox,txtValues);
  
  aString tbCmd[] = {"noSlipWall",""};
  aString tbTxt[] = {"no slip wall",""};
  int tbVal[] = {zeroTangentialVelocity,0};
  gui.setToggleButtons(tbCmd,tbTxt,tbVal,1);

  aString answer="";
  while (1)
    {
      gi.getAnswer(answer,"");
      if ( answer.matches("done") )
	break;
      else if ( gui.getToggleValue(answer,tbCmd[0],zeroTangentialVelocity))
	{
	}
      else if ( gui.getTextValue(answer, txtCmd[0],"%e",normalFlux) )
	{
	}					
      else
	gi.outputString("ERROR : PenaltySlipWallBC::inputFromGI did not understand answer: "+answer);
    }

  return true;
}

bool
PenaltySlipWallBC::
applyBC(Parameters &parameters, 
	const real & t, const real &dt,
	realMappedGridFunction &u,
	const int & grid,
	int side /* = -1 */,
	int axis /* = -1 */,
       	realMappedGridFunction *gridVelocity/* = 0*/)
{
  // here we set the ghost point values using vector symmetry for both ghost lines
  const InsParameters::PDEModel &pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");

  const bool assignTemperature = pdeModel==InsParameters::BoussinesqModel ||
                                 pdeModel==InsParameters::viscoPlasticModel;

  MappedGrid &mg = *u.getMappedGrid();
  int nd = mg.numberOfDimensions();
  int uc = parameters.dbase.get<int>("uc");
  int tc = parameters.dbase.get<int>("tc");
  Range U(uc,uc+nd-1);

  BoundaryConditionParameters bcParams;
  u.applyBoundaryCondition(U, BCTypes::vectorSymmetry, BCTypes::boundary(side,axis), 0.,t);  // this is not right for a wall with a normal flux, the normal component should probably be extrapolated or else the average of the normal components on the interior and ghost should be the normal flux.
  if ( assignTemperature )
    {
      u.applyBoundaryCondition(tc, BCTypes::neumann, BCTypes::boundary(side,axis), 0.,t);
    }

  bcParams.ghostLineToAssign=2;
  u.applyBoundaryCondition(U, BCTypes::vectorSymmetry, BCTypes::boundary(side,axis), 0.,t,bcParams);
  if ( assignTemperature )
    {
      u.applyBoundaryCondition(tc, BCTypes::evenSymmetry, BCTypes::boundary(side,axis), 0.,t,bcParams);
    }

  return true;
}

bool
PenaltySlipWallBC::
setBCCoefficients(Parameters &parameters, 
		  const real & t, const real &dt,
		  realMappedGridFunction &u,
		  realMappedGridFunction &coeff,
		  const int & grid,
		  int side /* = -1 */,
		  int axis /* = -1 */,
		  realMappedGridFunction *gridVelocity)
{
  Overture::abort("PenaltySlipWallBC::setBCCoefficients not implemented yet!");
  return true;
}
  

bool 
PenaltySlipWallBC::
addPenaltyForcing(Parameters &parameters, 
		  const real & t, const real &dt,
		  const realMappedGridFunction &u,
		  realMappedGridFunction &dudt,
		  const int & grid,
		  int side /* = -1 */,
		  int axis /* = -1 */,
		  const realMappedGridFunction *gridVelocity)
{
  // add the penalty method forcing for the normal flux condition
  // If the normal flux is given by u.n = g then the solution we want looks like ubc = u-(u.n)*n+g*n, the forcing is then
  //   dudt = beta*(ubc-u) = beta*(u-(u.n)*n+g*n-u) = beta*(g-u.n)*n
  // where beta = 1/(dt)
  // Note that if there is a grid velocity then u <-- u-gridVelocity in the above discussion so that the normal flux is defined relative to the moving frame.
  real beta = 1./(dt);
  MappedGrid &mg = *dudt.getMappedGrid();
  int nd = mg.numberOfDimensions();

  Index Ib[3],&Ib1=Ib[0],&Ib2=Ib[1],&Ib3=Ib[2];
  const intSerialArray &indexRange = mg.gridIndexRange();
  getBoundaryIndex(indexRange,side,axis,Ib1,Ib2,Ib3);

  IntegerArray indexRangeLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,indexRangeLocal,dimLocal,bcLocal, parallelBCFlag); 
  OV_GET_LOCAL_ARRAY_FROM(real,u,(realMappedGridFunction &)u);
  OV_GET_LOCAL_ARRAY(real,dudt);
  bool havePointsOnProcess = ParallelUtility::getLocalArrayBounds(dudt,dudtLocal,Ib1,Ib2,Ib3,true);

  // adjust the bounds so that we are not adding the forcing on boundary points that don't use a penalty method
  for ( int ad=1; ad<nd; ad++ )
    {
      int a = (axis+ad)%nd;
      if ( bcLocal(0,a)>0 && bcLocal(0,a)!=InsParameters::penaltyBoundaryCondition /*&& bcLocal(0,a)!=InsParameters::outflow*/) // is this the right thing to do in parallel??
	{
	  Ib[a] = Range(Ib[a].getBase()+1,Ib[a].getBound());
	}
      if ( bcLocal(1,a)>0 && bcLocal(1,a)!=InsParameters::penaltyBoundaryCondition /*&& bcLocal(1,a)!=InsParameters::outflow*/) // is this the right thing to do in parallel??
	{
	  Ib[a] = Range(Ib[a].getBase(),Ib[a].getBound()-1);
	}
    }


#ifdef USE_PPP
  int order = parameters.dbase.get<int >("orderOfAccuracy");
  bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
  int nGhost = order>2 || useFourthOrderArtificialDiffusion ? 2 : 1;
  for ( int d=1; d<nd; d++ )
    {
      const int ad=(axis+d)%nd;
      if ( bcLocal(0,ad)==parallelBCFlag ) Ib[ad] = Range(Ib[ad].getBase()+nGhost,Ib[ad].getBound());
      if ( bcLocal(1,ad)==parallelBCFlag ) Ib[ad] = Range(Ib[ad].getBase(),Ib[ad].getBound()-nGhost);
    }
#endif

  int uc = parameters.dbase.get<int>("uc");

  if ( havePointsOnProcess )
    {
      ((MappedGrid &)mg).update(MappedGrid::THEvertexBoundaryNormal);
        #ifdef USE_PPP
	  const RealArray & normalLocal = mg.vertexBoundaryNormalArray(side,axis);
        #else
	  const RealArray & normalLocal = mg.vertexBoundaryNormal(side,axis);
        #endif
	  //      const realMappedGridFunction &normal = mg.vertexBoundaryNormal(side,axis);
	  //      OV_GET_LOCAL_ARRAY(real,normal);

      realSerialArray ue(Ib1,Ib2,Ib3),tzNormalFlux(Ib1,Ib2,Ib3),gvb(Ib1,Ib2,Ib3);

      tzNormalFlux(Ib1,Ib2,Ib3) = 0.0;
      gvb(Ib1,Ib2,Ib3) = 0.0;

      OV_GET_LOCAL_ARRAY_FROM(int,mask,mg.mask());

      OV_APP_TO_PTR_4D(real,normalLocal,np);
      OV_APP_TO_PTR_4D(real,uLocal,up);
      OV_APP_TO_PTR_4D(real,dudtLocal,dudtp);
      OV_APP_TO_PTR_3D(real,ue,uep);
      OV_APP_TO_PTR_3D(real,tzNormalFlux,tznfp);
      OV_APP_TO_PTR_3D(real,gvb,gvbp);
      OV_APP_TO_PTR_3D(int,maskLocal,maskp);

      if ( parameters.dbase.get<bool >("twilightZoneFlow") )
	{
	  ((MappedGrid &)mg).update(MappedGrid::THEcenter);

	  realArray &x = mg.center();
	  OV_GET_LOCAL_ARRAY(real,x);
	  for ( int d=0; d<nd; d++ )
	    {
	      OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	      e.gd( ue  ,xLocal,nd,false,0,0,0,0,Ib1,Ib2,Ib3,uc+d,t);
	      for ( int i3=Ib3.getBase(); i3<=Ib3.getBound(); i3++ )
		for ( int i2=Ib2.getBase(); i2<=Ib2.getBound(); i2++ )
		  for ( int i1=Ib1.getBase(); i1<=Ib1.getBound(); i1++ )
		    {
		      A_3D(tznfp,i1,i2,i3) += A_4D(np,i1,i2,i3,d)*A_3D(uep,i1,i2,i3);
		    }
	    }
	}

      if ( gridVelocity )
	{
	  realMappedGridFunction &gv = (realMappedGridFunction &)(*gridVelocity);
	  OV_GET_LOCAL_ARRAY_FROM(real,gv,(realMappedGridFunction&)gv);
	  OV_APP_TO_PTR_4D(real,gvLocal,gvp);

	  for ( int i3=Ib3.getBase(); i3<=Ib3.getBound(); i3++ )
	    for ( int i2=Ib2.getBase(); i2<=Ib2.getBound(); i2++ )
	      for ( int i1=Ib1.getBase(); i1<=Ib1.getBound(); i1++ )
		{
		  real udotn=0;
		  for ( int d=0; d<nd; d++ )
		    udotn += A_4D(np,i1,i2,i3,d)*A_4D(gvp,i1,i2,i3,d);
		  A_3D(gvbp,i1,i2,i3) = udotn;
		}
	}

      int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
      for ( i3=Ib3.getBase(); i3<=Ib3.getBound(); i3++ )
	for ( i2=Ib2.getBase(); i2<=Ib2.getBound(); i2++ )
	  for ( i1=Ib1.getBase(); i1<=Ib1.getBound(); i1++ )
	    if ( A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint )
	      {
		real udotn=0;
		for ( int d=0; d<nd; d++ )
		  udotn += A_4D(np,i1,i2,i3,d)*(A_4D(up,i1,i2,i3,uc+d));
		
		udotn -= A_3D(gvbp,i1,i2,i3);
		bool onAnEdge = nd==3 && ( ( ii[(axis+1)%nd]==Ib[(axis+1)%nd].getBase() || ii[(axis+1)%nd]==Ib[(axis+1)%nd].getBound() ) ||
					   ( ii[(axis+2)%nd]==Ib[(axis+2)%nd].getBase() || ii[(axis+2)%nd]==Ib[(axis+2)%nd].getBound() ) );
		for ( int d=0; d<nd; d++ )
		  {
		    A_4D(dudtp,i1,i2,i3,uc+d) += beta*A_4D(np,i1,i2,i3,d)*(normalFlux+A_3D(tznfp,i1,i2,i3)-udotn);
		    if ( zeroTangentialVelocity && !onAnEdge ) A_4D(dudtp,i1,i2,i3,uc+d) -= beta*(A_4D(up,i1,i2,i3,uc+d)-udotn*A_4D(np,i1,i2,i3,d));
		  }
	      }
    }

  return true;
}

const bool 
PenaltySlipWallBC::
isPenaltyBC() const
{
  return true;
}

