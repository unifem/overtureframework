#include "DomainSolver.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "InsParameters.h"
#include "GUIState.h"
#include "PenaltyWallFunction.h"
#include "ArraySimple.h"
#include "kkcdefs.h"

namespace {
  const int parallelBCFlag = -31415926;
}

Parameters::BCModifier *createPenaltyWallFunctionBC(const aString &name)
{
  return new PenaltyWallFunctionBC(name);
}

PenaltyWallFunctionBC::
PenaltyWallFunctionBC(const aString &nm) : PenaltySlipWallBC(nm), fixedWallDistance(-1), linearLayerTopYPlus(11.0), wallFunctionType(PenaltyWallFunctionBC::logLaw), includeArtificialDissipationInShearStress(false), useFullVelocity(true) 
{
  zeroTangentialVelocity = true; // this seems to be a good thing...
}

PenaltyWallFunctionBC::
~PenaltyWallFunctionBC() {}

bool
PenaltyWallFunctionBC::
inputFromGI(GenericGraphicsInterface &gi)
{
  GUIState gui;
  gui.setExitCommand("done","done");
  gui.setWindowTitle("Wall Functions");

  aString txtBox[] = {"normal component",
		      "fixed wall distance",
		      "linear layer top y+",
		      ""};
  aString txtCmd[] = {"nComponent",
		      "nDistance",
		      "llyplus",
		      ""};
  aString txtValues[] = {"0.0","-1","11",""};
  gui.setTextBoxes(txtCmd,txtBox,txtValues);

  aString options = "Wall Models";
  aString optTxt[] = {"log law",
		      "simple log law",
		      "werner wengle",
		      "fixed utau",
		      ""};
  aString optCmd[] = {"slipWall",
		      "logLaw",
		      "simpleLogLaw",
		      "wernerWengle",
		      "fixedUTau",
		      ""};

  gui.addOptionMenu(options,optCmd,optTxt,PenaltyWallFunctionBC::slipWall);
  OptionMenu &wallModelOptions = gui.getOptionMenu(options);

  aString toggleBox = "options";
  aString togTxt[] = {"include artificial dissipation",
		      "use full velocity magnitude",
		      "no slip wall",
		     ""};
  aString togCmd[] = {"includeAD",
		      "useFullVelocity",
		      "noSlipWall",
		     ""};

  int togState[] = {includeArtificialDissipationInShearStress,useFullVelocity,0};
  gui.setToggleButtons(togCmd,togTxt,togState,1);

  aString answer="";

  gi.pushGUI(gui);
  while(1)
    {
      gi.getAnswer(answer,"");
      if ( answer.matches("done") )
	break;
      else if ( gui.getToggleValue(answer,togCmd[0],includeArtificialDissipationInShearStress) )
	{}
      else if ( gui.getToggleValue(answer,togCmd[1],useFullVelocity) )
	{}
      else if ( gui.getToggleValue(answer,togCmd[2],zeroTangentialVelocity ))
	{}
      else if ( gui.getTextValue(answer,txtCmd[0],"%f",PenaltySlipWallBC::normalFlux) )
	{}
      else if ( gui.getTextValue(answer,txtCmd[1],"%f",fixedWallDistance) )
	{
	}
      else if ( gui.getTextValue(answer,txtCmd[2],"%f",linearLayerTopYPlus) )
	{}
      else if ( answer.matches(optCmd[0]) )
	{
	  wallFunctionType = slipWall;
	}
      else if ( answer.matches(optCmd[1]) )
	{
	  wallFunctionType = logLaw;
	  if ( !db.has_key("log_law_parameters") ) 
	    {
	      db.put<DBase::DataBase>("log_law_parameters");
	    }
	  DataBase &lldb = db.get<DBase::DataBase>("log_law_parameters");
	  lldb.put<real>("E",9.8);
	}
      else if ( answer.matches(optCmd[2]) )
	{
	  wallFunctionType = simpleLogLaw;
	  if ( !db.has_key("simple_log_law_parameters") ) 
	    {
	      db.put<DBase::DataBase>("simple_log_law_parameters");
	    }
	  DataBase &lldb = db.get<DBase::DataBase>("simple_log_law_parameters");
	  real roughnessHeight = .01;
	  aString answer="";
	  gi.inputString(answer,"Enter roughness height: ");
	  sScanF(answer,"%f %f",&roughnessHeight);
	  lldb.put<real>("roughnessHeight",roughnessHeight);
	}
      else if ( answer.matches(optCmd[3]) )
	{
	  wallFunctionType = wernerWengle;
	  if ( !db.has_key("werner_wengle_parameters") ) 
	    {
	      db.put<DBase::DataBase>("werner_wengle_parameters");
	    }
	  DataBase &wwdb = db.get<DBase::DataBase>("werner_wengle_parameters");
	  wwdb.put<real>("A",8.3);
	}
      else if ( answer.matches(optCmd[4]) )
	{
	  wallFunctionType = fixedUTau;
	  if ( !db.has_key("fixed_utau_parameters") ) 
	    {
	      db.put<DBase::DataBase>("fixed_utau_parameters");
	    }
	  DataBase &wwdb = db.get<DBase::DataBase>("fixed_utau_parameters");
	  real utau=0.212;
	  aString answer="";
	  gi.inputString(answer,"Enter utau: ");
	  sScanF(answer,"%f",&utau);
	  wwdb.put<real>("utau",utau);
	}
      else
	gi.outputString("ERROR : PenaltyWallFunctionBC::inputFromGI did not understand answer: "+answer);
    }
  gi.popGUI();

  return true;
}

bool
PenaltyWallFunctionBC::
applyBC(Parameters &parameters, 
	const real & t, const real &dt,
	realMappedGridFunction &u,
	const int & grid,
	int side /* = -1 */,
	int axis /* = -1 */,
       	realMappedGridFunction *gridVelocity/* = 0*/)
{

  //  PlotIt::contour(*Overture::getGraphicsInterface(),u);
  const InsParameters::PDEModel &pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");

  const bool assignTemperature = pdeModel==InsParameters::BoussinesqModel ||
                                 pdeModel==InsParameters::viscoPlasticModel;

  MappedGrid &mg = *u.getMappedGrid();
  int nd = mg.numberOfDimensions();
  int uc = parameters.dbase.get<int>("uc");
  int vc = parameters.dbase.get<int>("vc");
  int wc = parameters.dbase.get<int>("wc");
  int tc = parameters.dbase.get<int>("tc");
  int order = parameters.dbase.get<int >("orderOfAccuracy");
  const bool isRectangular = mg.isRectangular();
  bool isMoving =  parameters.gridIsMoving(grid);

  real nu = parameters.dbase.get<real>("nu");
  bool useSecondOrderArtificialDiffusion = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion");
  bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
  int nGhost = order>2 || useFourthOrderArtificialDiffusion ? 2 : 1;
  int pGhost = mg.getMinimumNumberOfDistributedGhostLines();
  bool stabilizeBC =  parameters.dbase.get<bool >("stabilizeHighOrderBoundaryConditions");
  real ad21 = parameters.dbase.get<real >("ad21");
  real ad22 = parameters.dbase.get<real >("ad22");
  real ad41 = parameters.dbase.get<real >("ad41");
  real ad42 = parameters.dbase.get<real >("ad42");
  if ( stabilizeBC ) 
    {
      ad21 = max(ad21,ad41);
      ad22 = max(ad22,ad42);
      ad41 = ad42 = 0.;
    }

  ArraySimpleFixed<real,3,1,1,1> dx,dr;
  dx = dr = 0.;
  if ( isRectangular ) 
    mg.getDeltaX(dx.ptr());
  else
    mg.update(MappedGrid::THEinverseVertexDerivative);

  mg.update(MappedGrid::THEvertexBoundaryNormal);
  mg.update(MappedGrid::THEcenterBoundaryTangent);

  for ( int d=0; d<nd; d++ )
    dr[d] = mg.gridSpacing(d);

  Index Ib[3],&Ib1=Ib[0],&Ib2=Ib[1],&Ib3=Ib[2];
  const intSerialArray &indexRange = mg.gridIndexRange();
  getBoundaryIndex(indexRange,side,axis,Ib1,Ib2,Ib3);

  IntegerArray indexRangeLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( u,indexRangeLocal,dimLocal,bcLocal,parallelBCFlag ); 

  OV_GET_LOCAL_ARRAY_FROM(real,u,(realMappedGridFunction &)u);

  bool havePointsOnProcess = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3,true);

  if ( havePointsOnProcess )
    {
#ifdef USE_PPP
      const RealArray & normalLocal = mg.vertexBoundaryNormalArray(side,axis);
      const RealArray & tangentLocal = mg.centerBoundaryTangentArray(side,axis);

      for ( int d=1; d<nd; d++ )
	{
	  const int ad=(axis+d)%nd;
	  if ( bcLocal(0,ad)==parallelBCFlag ) Ib[ad] = Range(Ib[ad].getBase()+nGhost,Ib[ad].getBound());
	  if ( bcLocal(1,ad)==parallelBCFlag ) Ib[ad] = Range(Ib[ad].getBase(),Ib[ad].getBound()-nGhost);
	}
#else
      const RealArray & normalLocal = mg.vertexBoundaryNormal(side,axis);
      const RealArray & tangentLocal = mg.centerBoundaryTangent(side,axis);
#endif

      OV_GET_LOCAL_ARRAY_FROM(int,mask,mg.mask());
      OV_GET_LOCAL_ARRAY_CONDITIONAL(real,rsxy,isRectangular,uLocal,mg.inverseVertexDerivative());
      realMappedGridFunction &gv = isMoving ? (*gridVelocity) : u;
      OV_GET_LOCAL_ARRAY_CONDITIONAL(real,gv,!isMoving,uLocal,gv);

      OV_APP_TO_PTR_4D(real,normalLocal,np);
      OV_RGF_TO_PTR_5D(real,tangentLocal,tp,2);
      OV_APP_TO_PTR_4D(real,uLocal,up);
      OV_RGF_TO_PTR_5D(real,rsxyLocal, rxp, nd);
      OV_APP_TO_PTR_4D(real,gvLocal,gvp);
      OV_APP_TO_PTR_3D(int,maskLocal,maskp);

      ArraySimpleFixed<real,3,3,1,1> A, rx;
      ArraySimpleFixed<real,3,1,1,1> f,nrm,tng1,tng[2],avg,mprod,ughost,uavg;
      ArraySimpleFixed<real,2,1,1,1> tau;
      nrm = tng[0] = tng[1] = f = uavg = mprod = 0.;
      A = rx = 0.;

      ArraySimpleFixed<int,3,1,1,1> ib,ii,ig,ii2,off; // boundary, interior and ghost point indices
      ArraySimpleFixed<int,3,1,1,1> ittm[2],ittp[2],toff; // adjacent tangent indices along the boundary
      int &i1 = ib[0], &i2=ib[1], &i3=ib[2];
      for ( i3=Ib3.getBase(); i3<=Ib3.getBound(); i3++ )
	for ( i2=Ib2.getBase(); i2<=Ib2.getBound(); i2++ )
	  for ( i1=Ib1.getBase(); i1<=Ib1.getBound(); i1++ )
	    if ( A_3D(maskp,i1,i2,i3) & MappedGrid::ISdiscretizationPoint )
	    {

	      A = 0.;
	      f = 0.;

	      ig = ii = ii2 = ib;

	      ig[axis] = ib[axis] - (1-2*side); // first ghost point
	      ii[axis] = ib[axis] + (1-2*side); // first interior point
	      ii2[axis] = ib[axis] + 2*(1-2*side); // second interior point for 4th order dissipation shear stress
	      real distance = fixedWallDistance>0 ? fixedWallDistance : 0.0;
	      for ( int d=0; d<nd; d++ )
		{
		  nrm[d] = A_4D(np,ib[0],ib[1],ib[2],d);

		  // uavg will be used to determine the log layer location, i.e. set the shear stress
		  //      some people average this over several adjacent grid points
		  uavg[d] = (fixedWallDistance>0&&!zeroTangentialVelocity) ? A_4D(up,ib[0],ib[1],ib[2],uc+d) : A_4D(up,ii[0],ii[1],ii[2],uc+d);
		  // and uavg needs to be relative to any moving wall
		  if ( isMoving ) uavg[d] -= (fixedWallDistance>0&&!zeroTangentialVelocity) ? A_4D(gvp,ib[0],ib[1],ib[2],d) : A_4D(gvp,ii[0],ii[1],ii[2],d);

		  mprod[d] = 0;
		  for ( int t=0; t<nd && !isRectangular; t++ )
		    {
		      mprod[d] += nu*A_4D(np,ib[0],ib[1],ib[2],t)*A_5D(rxp,i1,i2,i3,d,t); // these are sums of products like nx*rx+ny*ry, etc
		      rx(d,t) = A_5D(rxp,i1,i2,i3,d,t);
		    }
		}

	      // compute the tangent vectors
	      real udotn= uavg[0]*nrm[0] + uavg[1]*nrm[1] + uavg[2]*nrm[2];
	      // remove the normal component of the velocity
	      //	      	      cout<<"IB : "<<ib[0]<<", "<<ib[1]<<", "<<ib[2]<<endl;
	      //	      cout<<"UAVG 1: "<<uavg[0]<<", "<<uavg[1]<<", "<<uavg[2]<<endl;
	      for ( int d=0; d<nd; d++ )
		uavg[d] -= udotn*nrm[d];
	      //	      cout<<"UAVG 2: "<<uavg[0]<<", "<<uavg[1]<<", "<<uavg[2]<<endl;

	      real umag = sqrt(uavg[0]*uavg[0] + uavg[1]*uavg[1] + uavg[2]*uavg[2]);

	      if ( umag <REAL_EPSILON )
		{
		  for ( int d=0; d<nd; d++ )
		    //		    for ( int t=0; t<nd-1; t++ )
		      {
			tng[0][d] = A_5D(tp,ib[0],ib[1],ib[2],d,0);
		      }
		}
	      else
		{
		  // compute the tangents from the velocity and the normal

		  for ( int d=0; d<nd; d++ )
		    {
		      tng[0][d] = uavg[d]; // already been done above - udotn *nrm[d];
		    }
		}
	      if ( nd==3 )
		{ // tng[1] = nrm cross tng[0], so that nrm = tng[0] cross tng[1] 
		  tng[1][0] =   nrm[1]*tng[0][2] - nrm[2]*tng[0][1];
		  tng[1][1] = -(nrm[0]*tng[0][2] - nrm[2]*tng[0][0]);
		  tng[1][2] =   nrm[0]*tng[0][1] - nrm[1]*tng[0][0];
		}
	      
	      real mt0 = sqrt(tng[0][0]*tng[0][0] + tng[0][1]*tng[0][1] + tng[0][2]*tng[0][2]);
	      real mt1 = nd==3 ? sqrt(tng[1][0]*tng[1][0] + tng[1][1]*tng[1][1] + tng[1][2]*tng[1][2]) : 1;
	      for ( int d=0; d<nd; d++ )
		{
		  tng[0][d] /= mt0;
		  tng[1][d] /= mt1;
		}		  

	      if ( isRectangular ) 
		{
		  mprod[axis] = nu*nrm[axis]*dr[axis]/dx[axis];
		  for ( int t=0; t<nd && !isRectangular; t++ )
		    rx(t,t) = dr[t]/dx[t];

		  if ( fixedWallDistance<0 ) distance = dx[axis];
		}
	      else if ( fixedWallDistance<0 )
		{
		  OV_GET_LOCAL_ARRAY_FROM(real,x,mg.vertex());
		  OV_APP_TO_PTR_4D(real,xLocal,xp);

		  for ( int d=0; d<nd; d++ )
		    distance += (A_4D(xp,ii[0],ii[1],ii[2],d)-A_4D(xp,ib[0],ib[1],ib[2],d))*(A_4D(xp,ii[0],ii[1],ii[2],d)-A_4D(xp,ib[0],ib[1],ib[2],d));

		  distance = sqrt(distance);
		}

	      //	      if ( f[0]!=f[0] ) abort();
	      // the remaining nd-1 equations come from the normal derivative of the tangential components setting
	      //   the shear stress in one (2D) or two (3D) directions 
	      // NOTE: set the shear stress using only 2nd order difference approximations for now,
	      //       is 4th order gilding the lilly? After all, wall models are fairly crude to begin with...
	      
	      /** In 2D, on a curvilinear grid, this looks like:
		  
	      nx * ( rx*Dor(u.t) + sx*Dos(u.t) ) + ny * (ry*Dor(u.t) + sy*Dos(u.t) ) = tau/nu
	      
	      - or -
	      Dor(u.t)*(nx*rx + ny*ry) + Dos(u.t)*(nx*sx + ny*sy) = tau/nu
	                  mprod[0]                   mprod[1]
	      where u is really u-gv. If we are on an axis=0 boundary (i.e. r direction is normal) we can add 

	      (2*side-1)* ( hr^2*(ad21+|ux|*ad22)*Do(u.t) + hr^4*(ad41+|ux|*ad42)*D+D-D+(u.t) )/nu

	      to the left hand side.  NOTE: to be consistent with the D+D- used in the second derivative in the rest of the code
	      we could choose to replace the Do operator above with D-.  Similarly, the third derivative in the dissipation would
	      really be D-D+D-, but this would include the second ghost line.  */

	      getShearStresses(nd,parameters,distance,uavg,tng,tau);
	      //	      cout<<"after call to get shear stresses : "<<tau[0]<<"  "<<tau[1]<<endl;
	      real avC2 = 0.0, avC4=0.0;
	      if ( includeArtificialDissipationInShearStress && nu>REAL_MIN && (useFourthOrderArtificialDiffusion || useSecondOrderArtificialDiffusion) )
		{
		  // compute the norm of the strain rate tensor using second order accurate approximations for the velocity derivatives
		  ArraySimpleFixed<real,3,1,1,1> rownorm;
		  rownorm[0] = rownorm[1] = rownorm[2] = 0.0;
		  for ( int d=0; d<nd; d++ )
		    {
		      // each row entry has D_xs(u_d) + D_xd(u_s)
		      for ( int s=0; s<nd; s++ )
			{
			  real a = 0.5* ( rx(0,s)*(A_4D(up,i1+1,i2,i3,uc+d)-A_4D(up,i1-1,i2,i3,uc+d))/(dr[0]) +
					  rx(1,s)*(A_4D(up,i1,i2+1,i3,uc+d)-A_4D(up,i1,i2-1,i3,uc+d))/(dr[1]) +
					  rx(0,d)*(A_4D(up,i1+1,i2,i3,uc+s)-A_4D(up,i1-1,i2,i3,uc+s))/(dr[0]) +
					  rx(1,d)*(A_4D(up,i1,i2+1,i3,uc+s)-A_4D(up,i1,i2-1,i3,uc+s))/(dr[1]) );
			  if ( nd==3 ) 
			    a+= 0.5*( rx(2,s)*(A_4D(up,i1,i2,i3+1,uc+d)-A_4D(up,i1,i2,i3-1,uc+d))/(dr[2]) +
				      rx(2,d)*(A_4D(up,i1,i2,i3+1,uc+s)-A_4D(up,i1,i2,i3-1,uc+s))/(dr[2]) );
			    
			  rownorm[d] += fabs(a);
			}
		    }
		  real uad = max(rownorm[0],rownorm[1],rownorm[2]);

		  //		  if (side==1 && axis==1 ) cout<<"wf : "<<ib[0]<<", "<<ib[1]<<", "<<uad<<endl;
		  // here are the coefficients we will use to add the artificial viscosity to the boundary conditions
		  // NOTE: we multiply by dr here because the undivided difference in the shear stress equation will give us 
		  //       dr (2nd order) or dr^3 (4th order) and we need to multiply by one more to get the scaling used by the dissipation.
		  avC2 = dr[axis]*(ad21 + ad22*uad); 
		  avC4 =-dr[axis]*(ad41 + ad42*uad);// note the - sign to get the dissipation correct

		} // end conditional for the artificial viscosity coefficients

	      //----------------- now we have everything needed to fill in the other nd-1 equations

	      ArraySimpleFixed<real,3,1,1,1> tngp,tngm; // plus and minus tangent vectors for tangentially adjacent grid points

	      // loop over the ghost points
	      for ( int ghost=0; ghost<nGhost; ghost++ )
		{
		  // we need to reset the ii and ig points so that we get the correct difference approximation
		  //   note that the previous stuff remains the same since we are only changing the stencil width

		  ig[axis] = ib[axis] - (ghost+1)*(1-2*side); // first ghost point
		  ii[axis] = ib[axis] + (ghost+1)*(1-2*side); // first interior point
		  ii2[axis] = ib[axis] + (ghost+1)*2*(1-2*side); // second interior point for 4th order dissipation shear stress

		  real drg[] = {dr[0], dr[1], dr[2]};
		  for ( int d=0; d<nd; d++ )
		    drg[d] *= (ghost+1);

		  // the first equation sets the average value of the normal component, 
		  //  usually this average is zero (as in vector symmetry for the normal component) but in general
		  //  is the value of the "normal flux".  Note we could make this more "conservative" by incorporating the
		  //  the mapping derivatives, but leave that for later...
		  f = 0.;
		  A = 0.;
		  f[0] = 2.*normalFlux;
		  for ( int d=0; d<nd; d++ )
		    {
		      const int cc = uc+d;
		      A(0,d) =  nrm[d];
		      f[0] -= nrm[d]*(A_4D(up,ii[0],ii[1],ii[2],cc));
		      //		      if ( side==1 && axis==1 ) cout<<"f[0] = "<<f[0]<<", "<<nrm[d]<<", "<<A_4D(up,ii[0],ii[1],ii[2],cc)<<endl;
		      if ( isMoving ) f[0] -=  nrm[d]*(-A_4D(gvp,ii[0],ii[1],ii[2],d)-A_4D(gvp,ig[0],ig[1],ig[2],d));
		    }

		  for ( int t=1; t<nd; t++ ) // LOOP OVER THE TANGENTIAL COMPONENTS
		    { 
		      //130912 get rid of 1/nu here, multiply mprod by nu above		      f[t] = nu > REAL_MIN ? tau[t-1]/nu : 0.0;
		      f[t] = nu > REAL_MIN ? tau[t-1] : 0.0;

		      for ( int d=0; d<nd; d++ ) // loop over the velocity components (basically the u.tangent part for each grid point involved)
			{
			  A(t,d) = (2*side-1)*(tng[t-1][d]*(0.5*mprod[axis]/drg[axis] + avC2 + avC4));
			  f(t)  += (2*side-1)*tng[t-1][d]*( (0.5*mprod[axis]/drg[axis] + avC2 - 3.0*avC4)*A_4D(up,ii[0],ii[1],ii[2],uc+d) +
							    avC4*(A_4D(up,ii2[0],ii2[1],ii2[2],uc+d)+3.0*A_4D(up,ib[0],ib[1],ib[2],uc+d)) );
			  //		      cout<<"mprod/dr = "<<mprod[axis]/drg[axis]<<endl;
			  if ( isMoving )
			    {
			      f(t) += -(2*side-1)*(tng[t-1][d]*(0.5*mprod[axis]/drg[axis] + avC2 + avC4))*A_4D(gvp,ig[0],ig[1],ig[2],d) -
				(2*side-1)*tng[t-1][d]*( (0.5*mprod[axis]/drg[axis] + avC2 - 3.0*avC4)*A_4D(gvp,ii[0],ii[1],ii[2],d) +
							 avC4*(A_4D(gvp,ii2[0],ii2[1],ii2[2],d)+3.0*A_4D(gvp,ib[0],ib[1],ib[2],d)) );
			    }

			  // here are the tangential parameter space derivatives, these will only have nonzero contributions on non-orthogonal grids
			  // all of the contributions go on the right hand side of the system.  Note that we compute the adjacent tangents in this loop.
			  for ( int pt=1; pt<nd && !isRectangular; pt++ )
			    {
			      int p = (axis+pt)%nd;
			      off = 0;
			      off[p] = 1;

			      // compute the two tangents at the + and - grid points using uavg and the adjacent normals
			      if ( umag <REAL_EPSILON )
				{
				  tngp = tngm = tng[t-1];  // basically we are computing a vectory symmetry, the shear stress is zero I guess...
				}
			      else
				{
				  // compute the tangents from the uavg and the normal
				  //   use uavg instead of the adjacent velocities so that the change in tangent 
				  //   is due solely to the geometric change in the normal
				  real udotnp,udotnm;
				  ArraySimpleFixed<real,3,1,1,1> nrmp,nrmm;
				  nrmp = nrmp = 0;
				  udotnp = udotnm = 0;
				  for ( int d=0; d<nd; d++ )
				    { 
				      nrmp[d] = A_4D(np,ib[0]+off[0],ib[1]+off[1],ib[2]+off[2],d);
				      udotnp += nrmp[d]*uavg[d];
				      nrmm[d] = A_4D(np,ib[0]-off[0],ib[1]-off[1],ib[2]-off[2],d);
				      udotnm += nrmm[d]*uavg[d];
				    }

				  for ( int d=0; d<nd; d++ )
				    {
				      tngp[d] = uavg[d] - udotnp *nrmp[d];
				      tngm[d] = uavg[d] - udotnm *nrmm[d];
				    }
		      
				  if ( nd==3 && t==2 )
				    { // tng[1] = nrm cross tng[0], so that nrm = tng[0] cross tng[1] 
				      ArraySimpleFixed<real,3,1,1,1> tmp;
				      tmp = tngp;
				      tngp[0] =   nrmp[1]*tmp[2] - nrmp[2]*tmp[1];
				      tngp[1] = -(nrmp[0]*tmp[2] - nrmp[2]*tmp[0]);
				      tngp[2] =   nrmp[0]*tmp[1] - nrmp[1]*tmp[0];

				      tmp = tngm;
				      tngm[0] =   nrmm[1]*tmp[2] - nrmm[2]*tmp[1];
				      tngm[1] = -(nrmm[0]*tmp[2] - nrmm[2]*tmp[0]);
				      tngm[2] =   nrmm[0]*tmp[1] - nrmm[1]*tmp[0];
				    }
			      
				  real mtp = sqrt(tngp[0]*tngp[0] + tngp[1]*tngp[1] + tngp[2]*tngp[2]);
				  real mtm = sqrt(tngm[0]*tngm[0] + tngm[1]*tngm[1] + tngm[2]*tngm[2]);
				  for ( int d=0; d<nd; d++ )
				    {
				      tngp[d] /= mtp;
				      tngm[d] /= mtm;
				    }		  
				} // end of compute tangents tngm and tngp

			      f(t) -= (0.5*mprod[p]/drg[p])* // D_o(velocity .dot. tangent) 
				(A_4D(up,ib[0]+off[0],ib[1]+off[1],ib[2]+off[2],uc+d)*tngp[d]-
				 A_4D(up,ib[0]-off[0],ib[1]-off[1],ib[2]-off[2],uc+d)*tngm[d]);

			      if ( isMoving )
				{
				  f(t) += (0.5*mprod[p]/drg[p])* // D_o(grid velocity .dot. tangent) 
				    (A_4D(gvp,ib[0]+off[0],ib[1]+off[1],ib[2]+off[2],d)*tngp[d]-
				     A_4D(gvp,ib[0]-off[0],ib[1]-off[1],ib[2]-off[2],d)*tngm[d]);
				} // is moving
			    } // not is rectangular
			} // loop over velocity components
		  
		  
		    } // END LOOP OVER THE TANGENTIAL DIRECTIONS
		  //	      if ( f[0]!=f[0] ) abort();
#if 0
		  cout<<"--- dbg --"<<endl;
		  cout<<uavg<<endl;
		  cout<<tau[0]<<"  "<<tau[1]<<endl;
		  cout<<A<<endl;
		  cout<<f<<endl;
		  cout<<tngp<<endl;
		  cout<<tng[0]<<endl;
		  cout<<tng[1]<<endl;
		  cout<<tngm<<endl;
#endif

		  solveSmallSystem(nd,A,f,ughost);

#if 0
		  //	  if ( side==1 && axis==1 )
		  //		  if ( grid==0 && ib[0]==1 && ib[1]==0 ) 
		  if ( sqrt(ughost[0]*ughost[0]+ughost[1]*ughost[1]+ughost[2]*ughost[2])>1e3 ) {
		        		  cout<<"A = "<<A<<endl;
		      		      		  cout<<"f = "<<f<<endl;
		      		  cout<<"ughost = "<<ughost<<endl;
		      cout<<grid<<", "<<ib[0]<<", "<<ib[1]<<", "<<ib[2]<<": tau = "<<tau[0]<<", "<<tau[1]<<endl;
		      cout<<"uavg = "<<uavg<<endl;
		      cout<<"distance = "<<distance<<endl;
		      //		  cout<<"avC2, avC4 = "<<avC2<<", "<<avC4<<endl;
		    }
#endif
		  for ( int d=0; d<nd; d++ )
		    A_4D(up,ig[0],ig[1],ig[2],uc+d) = ughost[d];

		} // end loop over ghost points

	    } // end loop over boundary points and is discreatization point

    } // end conditional on havePointsOnProcess

  // If we did the boundary conditition to 4th order then we would include the second ghost point in the system above. For now, though,
  //   just we just applied the 2nd order accurate bc on a wider stencil to get the second ghost line.  
  // NOTE: we have not implemented a "thermal wall function" yet so we just assume adiabatic for the moment.
  //   Another alternative would be to allow a fixed temperature...
  BoundaryConditionParameters bcParams;
  bcParams.ghostLineToAssign=2;
  bcParams.orderOfExtrapolation=order;
  Range U(uc, (nd==2 ? vc : wc));
  //  u.applyBoundaryCondition(U, BCTypes::extrapolate, BCTypes::boundary(side,axis), 0.,t,bcParams);
  if ( assignTemperature )
    {
      bcParams.ghostLineToAssign=1; 
      u.applyBoundaryCondition(tc, BCTypes::evenSymmetry, BCTypes::boundary(side,axis), 0.,t,bcParams); // yeah, we should probably have a thermal wall model
      bcParams.ghostLineToAssign=2;
      u.applyBoundaryCondition(tc, BCTypes::evenSymmetry, BCTypes::boundary(side,axis), 0.,t,bcParams);
    }

  return true;
}

void
PenaltyWallFunctionBC::
getShearStresses(const int &nd,
		 Parameters &parameters,
		 const real &distance,
		 const ArraySimpleFixed<real,3,1,1,1> &uavg,
		 const ArraySimpleFixed<real,3,1,1,1> tng[],
		 ArraySimpleFixed<real,2,1,1,1> &tau)
{
  // 120118 : kkc initial version
  /** Here is the actual implementation of the wall model where we compute the shear stresses on the surface from
      the "mean" velocity uavg.  There are currently three basic models:
      
      1) u+ = (1/kappa) ln(E y+) : which is a traditional log layer model (can also be written (1/kappa) ln (y+) + B
      2) u+ = 8.3 (y+)^(1/7) : the Werner-Wengle model
      3) u+ = (1/kappa) ln(y/y0) : a simple "rough wall" model sometimes used for atmospheric boundary layers

      u+ = uavg/utau, y+=y*utau/nu, y0=roughness height, kappa=Karman's constant, utau = sqrt(tau/rho)

      tau = mu * d(uavg .dot. tangent)/dn = mu * normal .dot. grad(uavg.dot.tangent) 

      There are some options to the above models:  

       - If y+<linearLayerTopYPlus then u+=y+ 

       NOTE: this routine actually returns the shear stress divided by the density which is nu*d(u.tangent)/dn
  **/

  const real kappa = 0.41;

  real umag = sqrt(uavg[0]*uavg[0] + uavg[1]*uavg[1] + uavg[2]*uavg[2]);
  real nu = parameters.dbase.get<real>("nu");

  for ( int t=0; t<nd-1; t++ )
    {
      // for each tangent vector

      // compute the tangent or full velocity magnitude 
      real va = useFullVelocity ? umag : fabs(uavg[0]*tng[t][0] + uavg[1]*tng[t][1] + uavg[2]*tng[t][2]);

      real utau=0, uplus=0;

      if ( wallFunctionType == slipWall )
	{
	  utau = 0;
	}
      if ( wallFunctionType == fixedUTau )
	{
	  utau = db.get<DBase::DataBase>("fixed_utau_parameters").get<real>("utau");
	}
      else if ( wallFunctionType==simpleLogLaw )
	{
	  real roughnessHeight = db.get<DBase::DataBase>("simple_log_law_parameters").get<real>("roughnessHeight");
	  utau = va*kappa/( log(distance/roughnessHeight) );
	}
      else if ( wallFunctionType==wernerWengle )
	{
	  real Aww = db.get<DBase::DataBase>("werner_wengle_parameters").get<real>("A");
	  utau = pow(va*pow(distance/nu,-1.0/7.0)/Aww,7.0/8.0);
	}
      else if ( wallFunctionType==logLaw )
	{ 
	  /** We need to solve a nonlinear equation to get tau:
	      u+ = (1/kappa) ln(E y+)
	      which gives
	      va = (utau/kappa) ln (E distance utau/nu)
	      - or -
	      va = (X/kappa) ln( a X )
	      where X=utau and a = E*distance/nu.

	      If we write
	      F(X) = -va + X ln (a X)/kappa
	      we want X such that F(X) = 0.
	      Note dF/dX = (ln(a X) + 1/a)/kappa .
	      A simple Newton iteration gives:
	      F(X^k + dX) = F( X^k ) + dX dF(X^k)/dX
	      so that 
	      dX = -F(X^k)/(dF(X^k)/dX)
	      as usual. 

	      Guess X^0 = the werner-wengle value 

	      NOTE: I doubt it is really this simple...
	  **/
	  
	  real Ell = db.get<DBase::DataBase>("log_law_parameters").get<real>("E");
	  utau = pow(va*pow(distance/nu,-1.0/7.0)/8.3,7.0/8.0);
	  real dX = REAL_MAX;
	  real tol = 1e-7;
	  int it=0, itmax=10;
	  real a = distance*Ell/nu;
	  while ( utau>REAL_MIN && it<itmax && fabs(dX)>tol )
	    {
	      real F = -va + utau*log(a*utau)/kappa;
	      real Fx= (log(a*utau)+1.0/a)/kappa;
	      dX = -F/Fx;
	      utau = utau + dX;
	      it++;
	      //	      cout<<"it = "<<it<<", dX = "<<dX<<endl;
	      // I guess we should check to make sure utau is positive...?
	    }

	  if ( it==itmax && fabs(dX)>tol )
	    cout<<"WARNING :: log law wall function iteration failed to converge"<<endl;
	} // end of loglaw utau calculation

      real yplus=distance*utau/nu; // Adjust utau for the linear layer if needed.  By the way, this estimate is wrong if we are actually in the linear layer...

      //      if (yplus>10*REAL_EPSILON && yplus<linearLayerTopYPlus) utau = sqrt(va*nu/distance); // ... and it would be nice to use some blending function
      //             if ( yplus<linearLayerTopYPlus )
      //      	      cout<<"YPLUS = "<<yplus<<", "<<distance<<", "<<utau<<", "<<va<<", ("<<uavg[0]<<","<<uavg[1]<<","<<uavg[2]<<"), ("<<tng[t][0]<<", "<<tng[t][1]<<", "<<tng[t][2]<<")"<< endl;
      // now we have utau, compute the magnitude of tau/rho
      tau[t] = -utau*utau;

      if ( useFullVelocity && umag>REAL_EPSILON)
	{
	  real udt = fabs(uavg[0]*tng[t][0] + uavg[1]*tng[t][1] + uavg[2]*tng[t][2])/umag;
	  tau[t] = -utau*utau*udt;
	}

    } // end of tangent loop
}
