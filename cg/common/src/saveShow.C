// This file automatically generated from saveShow.bC with bpp.
#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "ParallelUtility.h"
#include "Controller.h"
#include "BodyForce.h"


// ===================================================================================================================
/// \brief Save a solution in the show file. 
/// \param gf0 (input) : save this grid function.
/// \note The array of Strings, parameters.dbase.get<aString* >("showVariableName"), holds a list of the things
///    to save in the show file.
// ===================================================================================================================
void DomainSolver::
saveShow( GridFunction & gf0 )
{
    if( parameters.dbase.get<Ogshow* >("show")==NULL )
        return;

    real cpu0=getCPU();

    const int appendToOldShowFile = parameters.dbase.get<int >("appendToOldShowFile");
    if( numberSavedToShowFile==-1 && appendToOldShowFile )
    {
    // -- do not save first solution if we are appending to an existing show file.
    // This solution is already there.
        numberSavedToShowFile=0;
    // There is no need to save the grid if we are appending:
        parameters.dbase.get<int >("saveGridInShowFile")=false;
        return;
    }
    

    if( numberSavedToShowFile==-1 )
    {
    // first call -- save general parameters
        numberSavedToShowFile=0;
        parameters.saveParametersToShowFile();
    }
    numberSavedToShowFile++;

    if( parameters.dbase.get<bool >("saveRestartFile") )
    {
    // keep two restart files, just in case we crash while writing one of them
        restartNumber = (restartNumber+1) % 2;
        saveRestartFile(gf0, restartNumber==0 ? "ob1.restart" : "ob2.restart" );
    }
    
    if( debug() & 1 )
        printF("saving a solution in the show file\n");
    
    Ogshow & show = *parameters.dbase.get<Ogshow* >("show");
    show.startFrame();

    HDF_DataBase *dbp=NULL;
    #ifdef OV_USE_HDF5
        bool putToDataBase=true;    // hdf5  -- put on all processors
    #else
        bool putToDataBase= parameters.dbase.get<int >("myid")==0; // hdf4 - only put on processor 0
    #endif
    if( putToDataBase )
    {
        dbp = show.getFrame();
        assert( dbp!=NULL );
    }

    realCompositeGridFunction & u = gf0.u;
    CompositeGrid & cg0 = gf0.cg;
    
    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const int & tc = parameters.dbase.get<int >("tc");
    const int & pc = parameters.dbase.get<int >("pc");
//  const int & sc = parameters.dbase.get<int >("sc");
//  const int & numberOfSpecies = parameters.dbase.get<int >("numberOfSpecies");
    
    const int numberOfDimensions = cg0.numberOfDimensions();
    
    aString *showVariableName=parameters.dbase.get<aString* >("showVariableName");
    const IntegerArray & showVariable = parameters.dbase.get<IntegerArray >("showVariable");

    
    aString timeLine;
    if( !parameters.isSteadyStateSolver() )
    {
        if( gf0.t==0. || (gf0.t > .1 && gf0.t < 1.e4) )
        {
            if( gf0.t < 1. )
                sPrintF(timeLine,"t=%4.3f, dt=%8.2e",gf0.t,dt);
            else if( gf0.t < 10. )
                sPrintF(timeLine,"t=%5.3f, dt=%8.2e",gf0.t,dt);
            else if( gf0.t < 100. )
                sPrintF(timeLine,"t=%6.3f, dt=%8.2e",gf0.t,dt);
            else if( gf0.t < 1000. )
                sPrintF(timeLine,"t=%7.3f, dt=%8.2e",gf0.t,dt);
            else
                sPrintF(timeLine,"t=%8.3f, dt=%8.2e",gf0.t,dt);
        }
        else
            sPrintF(timeLine,"t=%9.2e, dt=%8.2e",gf0.t,dt);
    }
    else
    {
        sPrintF(timeLine,"it=%i",parameters.dbase.get<int >("globalStepNumber")+1);
    }
    
  // save parameters that go in this frame
    if( putToDataBase ) 
    { 
          assert( dbp!=NULL );
          HDF_DataBase & db = *dbp;
          db.put(gf0.t,"time");
          db.put(dt,"dt");
    }
    
    parameters.dbase.put<aString>("timeLine",timeLine);
    saveShowFileComments( show );
    parameters.dbase.remove("timeLine");
    
/* ----
  // add species if we compute reactions
    int firstSpecies=numberOfShowVariables;
    for( int s=0; s<numberOfSpecies; s++ )
    {
        showVariableName[numberOfShowVariables++]=u.getName(s+sc);
    }
    showVariableName[numberOfShowVariables]="";
---- */


 // save some parameters for a restart
    if( putToDataBase )
    { 
        assert( dbp!=NULL );
        HDF_DataBase & db = *dbp;
        db.put(gf0.t,"t");

    // printF("*** saveShow: parameters.isMovingGridProblem()=%i\n",parameters.isMovingGridProblem());
        
        db.put((int)parameters.isMovingGridProblem(),"isMovingGridProblem");
        if( parameters.isMovingGridProblem() )
        {
            printF("*** saveShow: Save the movingsGrids info... \n");
      // save info for moving grids
            parameters.dbase.get<MovingGrids >("movingGrids").put(db,"movingGrids");
        }

    // --- Save body/boundary force regions in the first frame, if this is non a moving grid problem ---
    //     For moving grids - save the regions in all frames (could do better) since we may
    //     save moving grids as bodies
        if( numberSavedToShowFile==1 ||  parameters.isMovingGridProblem() )
        {
      // Save both moving grid bodies and body force regions as a "BodyForce"
      // Note : for plotting purposes we save some moving grids (e.g. beams) as a "BodyForce"
            std::vector<BodyForce*> movingBodies;
            if( parameters.isMovingGridProblem() )
                  parameters.dbase.get<MovingGrids >("movingGrids").getBodies( movingBodies );
            const int numberOfMovingBodies=movingBodies.size();

            int numberOfBodyForceRegions = 0;
            if( parameters.dbase.get<bool >("turnOnBodyForcing") )
      	numberOfBodyForceRegions = parameters.dbase.get<std::vector<BodyForce*> >("bodyForcings").size();

            const int totalNumberOfBodies=numberOfMovingBodies + numberOfBodyForceRegions;
      // save the total number of bodies: 
            db.put(totalNumberOfBodies,"numberOfBodyForceRegions");

            if( totalNumberOfBodies>0 )
            {
      	for( int mb=0; mb<movingBodies.size(); mb++ )
      	{
        	  const BodyForce & bodyForce = *movingBodies[mb];
        	  bodyForce.put(db,sPrintF("BodyForce%i",mb));
      	}
      	for( int mb=0; mb<movingBodies.size(); mb++ )
        	  delete movingBodies[mb];

      	if( parameters.dbase.get<bool >("turnOnBodyForcing") )
      	{
	  // Here is the array of body forcings:
        	  std::vector<BodyForce*> & bodyForcings =  parameters.dbase.get<std::vector<BodyForce*> >("bodyForcings");
        	  for( int bf=0; bf<bodyForcings.size(); bf++ )
        	  {
          	    const BodyForce & bodyForce = *bodyForcings[bf];
          	    bodyForce.put(db,sPrintF("BodyForce%i",bf+numberOfMovingBodies));
        	  }
      	}
            }
            
            if( parameters.dbase.get<bool >("turnOnBoundaryForcing") )
            {
	// Here is the array of boundary forcings:
      	std::vector<BodyForce*> & boundaryForcings =  parameters.dbase.get<std::vector<BodyForce*> >("boundaryForcings");
        // -- save the number of regions:
                db.put((int)boundaryForcings.size(),"numberOfBoundaryForceRegions");

      	for( int bf=0; bf<boundaryForcings.size(); bf++ )
      	{
        	  const BodyForce & boundaryForce = *boundaryForcings[bf];
                    boundaryForce.put(db,sPrintF("BoundaryForce%i",bf));
      	}
            }
            else
            {
                db.put(0,"numberOfBoundaryForceRegions"); // there are no boundary force regions
            }
            
        }

    }


    const bool saveAugmentedSolutionToShowFile = parameters.dbase.get<bool>("saveAugmentedSolutionToShowFile");
    realCompositeGridFunction q;
    if( saveAugmentedSolutionToShowFile )
    {
    // --  New default way  --
    // Save variables that are in the augmented solution
        printP("*** saveShow:   saveAugmentedSolutionToShowFile ***\n");
        
        realCompositeGridFunction & u = getAugmentedSolution(gf0,q);

    }
    else
    {
    // *** OLD WAY ***

    // first count the number of variables we are going to save
        int numberOfShowVariables=0;
        int i;
        for( i=0; showVariableName[i]!=""; i++ )
            if( showVariable(i)>=0 )
      	numberOfShowVariables++;
        Range all;
        q.updateToMatchGrid(cg0,all,all,all,numberOfShowVariables);  
        q.setOperators(*gf0.u.getOperators());

  
        aString solutionName[1] = { "u" }; // *******************************************
        q.setName(solutionName[0]);                           // name grid function

        bool ok;
        const int includeGhost=1;

        Index I1,I2,I3;
        int grid;
        i=-1;
        for( int n=0; showVariableName[n]!=""; n++ )
        {

      // printF(" n=%i showVariableName=%s showVariable=%i\n",n,(const char*)(showVariableName[n]),showVariable(n));
        
            if( showVariable(n)< 0 )
      	continue;

            i++;
        
            q.setName( showVariableName[n],i);




            if( showVariable(n) < parameters.dbase.get<int >("numberOfComponents") )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
	  // q[grid](all,all,all,i)=u[grid](all,all,all,showVariable(n)); // *wdh* 061013
        	  getIndex(cg0[grid].dimension(),I1,I2,I3);
                #ifdef USE_PPP
                            realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                            realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                #else
                            realSerialArray & qg = q[grid];
                            realSerialArray & ug = u[grid];
                #endif
                            ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                            if( !ok ) continue;  
        	  qg(I1,I2,I3,i)=ug(I1,I2,I3,showVariable(n));
      	}
            
            }
            else if( showVariableName[n]=="p" )
            {
      	if( tc>=0 && parameters.getDerivedFunction("pressure",u,q,i,gf0.t,parameters)==0 )
      	{
	  // we could compute it from a derived function implementation
      	}
      	else if ( pc>=0 ) 
      	{ // the pc index actually marks the pressure variable
        	  for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        	  {
	    // q[grid](all,all,all,i)=u[grid](all,all,all,pc);         
          	    getIndex(cg0[grid].dimension(),I1,I2,I3);
                    #ifdef USE_PPP
                                realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                                realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                    #else
                                realSerialArray & qg = q[grid];
                                realSerialArray & ug = u[grid];
                    #endif
                                ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                                if( !ok ) continue;  
          	    qg(I1,I2,I3,i)=ug(I1,I2,I3,pc);
        	  }
      	}
      	else
      	{
        	  cout << "saveShow: I don't know how to compute the pressure from the variables being used\n";
        	  Overture::abort("error");
      	}
            
	// kkc 070126 XXXX BILL : is it ok to comment these lines out... it is quite a special case
//       if( cg0.numberOfComponentGrids()==1 && cg0.numberOfDimensions()==2 && !parameters.dbase.get<bool >("twilightZoneFlow") 
//           && parameters.dbase.get<Parameters::PDE >("pde")==Parameters::incompressibleNavierStokes )
//       {
// 	// set mean of the pressure
//         Index I1,I2,I3;
//         getIndex(cg0[0].gridIndexRange(),I1,I2,I3,-1);
//         real mean=sum(q[0](I1,I2,I3,i)); // sum interior points
//         for( int side=Start; side<=End; side++)
// 	{
//           for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
// 	  {
// 	    getBoundaryIndex(cg0[0].gridIndexRange(),side,axis,I1,I2,I3,-1);  // boundary points sans corners
//             mean+=.5*sum(q[0](I1,I2,I3,i));
// 	  }
// 	}
//         // add in corners
//         const IntegerArray & gi = cg0[0].gridIndexRange();
// 	mean+=.25*(q[0](gi(Start,axis1),gi(Start,axis2),gi(Start,axis3),i) 
//                   +q[0](gi(End  ,axis1),gi(Start,axis2),gi(Start,axis3),i) 
//                   +q[0](gi(Start,axis1),gi(End  ,axis2),gi(Start,axis3),i) 
// 		  +q[0](gi(End  ,axis1),gi(End  ,axis2),gi(Start,axis3),i));
            
//         mean*=cg0[0].gridSpacing()(axis1)*cg0[0].gridSpacing()(axis2);
//         printf("saveShow: mean(p) = %e, setting to zero \n",mean);
// 	q[0](all,all,all,i)-=mean;
//  }
            }
            else if( showVariableName[n]=="p.x" )
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        	  q[grid](all,all,all,i)=u[grid].x()(all,all,all,pc);       
            else if( showVariableName[n]=="p.y" )
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        	  q[grid](all,all,all,i)=u[grid].y()(all,all,all,pc);       
            else if( showVariableName[n]=="T" )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  getIndex(cg0[grid].dimension(),I1,I2,I3);
                #ifdef USE_PPP
                            realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                            realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                #else
                            realSerialArray & qg = q[grid];
                            realSerialArray & ug = u[grid];
                #endif
                            ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                            if( !ok ) continue;  
        	  if( tc>= 0 )
          	    qg(I1,I2,I3,i)=ug(I1,I2,I3,tc);          // ***** may have to compute ****
        	  else if( pc>=0 && rc>=0 )
          	    qg(I1,I2,I3,i)=ug(I1,I2,I3,pc)/(ug(I1,I2,I3,rc)*parameters.dbase.get<real >("Rg"));    
        	  else
        	  {
          	    cout << "saveShow: unable to compute the temperature \n";
          	    Overture::abort("error");
        	  }
      	}
            
//       for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
//         if( tc>= 0 )
//           q[grid](all,all,all,i)=u[grid](all,all,all,tc);          // ***** may have to compute ****
//         else if( pc>=0 && rc>=0 )
//           q[grid](all,all,all,i)=u[grid](all,all,all,pc)/(u[grid](all,all,all,rc)*parameters.dbase.get<real >("Rg"));    
//         else
// 	{
//           cout << "saveShow: unable to compute the temperature \n";
//           Overture::abort("error");
// 	}
            }
            else if( showVariableName[n]=="Mach Number" )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  getIndex(cg0[grid].dimension(),I1,I2,I3);
                #ifdef USE_PPP
                            realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                            realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                #else
                            realSerialArray & qg = q[grid];
                            realSerialArray & ug = u[grid];
                #endif
                            ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                            if( !ok ) continue;  

        	  realSerialArray uSq;
        	  if( cg0.numberOfDimensions()==1 ) 
          	    uSq=SQR(ug(all,all,all,uc));
        	  else if( cg0.numberOfDimensions()==2 )
          	    uSq=SQR(ug(all,all,all,uc))+SQR(ug(all,all,all,vc));
        	  else
          	    uSq=SQR(ug(all,all,all,uc))+SQR(ug(all,all,all,vc))+SQR(ug(all,all,all,wc));

        	  if( tc>= 0 )
          	    qg(all,all,all,i)=SQRT(uSq/(parameters.dbase.get<real >("gamma")*parameters.dbase.get<real >("Rg")*ug(all,all,all,tc)));     
        	  else if( pc>=0 && rc>=0 )
          	    qg(all,all,all,i)=SQRT(uSq/parameters.dbase.get<real >("gamma")*ug(all,all,all,pc)/ug(all,all,all,rc));    
        	  else
        	  {
          	    cout << "saveShow: unable to compute the Mach Number \n";
          	    Overture::abort("error");
        	  }
      	}
            }
            else if( showVariableName[n]=="divergence" )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  getIndex(cg0[grid].dimension(),I1,I2,I3);
                #ifdef USE_PPP
                            realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                            realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                #else
                            realSerialArray & qg = q[grid];
                            realSerialArray & ug = u[grid];
                #endif
                            ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                            if( !ok ) continue;  

        	  RealArray div(I1,I2,I3);
        	  Range V(uc,uc+cg0.numberOfDimensions()-1);
        	  MappedGridOperators & op = *(u[grid].getOperators());
        	  op.derivative(MappedGridOperators::divergence,ug,div,I1,I2,I3,V);
        	  qg(I1,I2,I3,i)=div;

        	  if( cg0.numberOfDimensions()==2 && parameters.isAxisymmetric() )
        	  {
	    // div(u) = u.x + v.y + v/y for y>0   or u.x + 2 v.y at y=0
                    #ifdef USE_PPP
                                realSerialArray vertex; getLocalArrayWithGhostBoundaries(cg0[grid].vertex(),vertex);
                    #else
                                realSerialArray & vertex = cg0[grid].vertex();
                    #endif

          	    RealArray radiusInverse(I1,I2,I3);
          	    radiusInverse = 1./max(REAL_MIN,vertex(I1,I2,I3,axis2));
          	    Index Ib1,Ib2,Ib3;
          	    for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
          	    {
            	      for( int side=0; side<=1; side++ )
            	      {
            		if( cg0[grid].boundaryCondition(side,axis)==Parameters::axisymmetric )
            		{
              		  getBoundaryIndex(cg0[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
              		  ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,Ib1,Ib2,Ib3,includeGhost);
              		  if( !ok ) continue;  
              		  radiusInverse(Ib1,Ib2,Ib3)=0.;
              		  RealArray uy(Ib1,Ib2,Ib3);
              		  op.derivative(MappedGridOperators::yDerivative,ug,uy,Ib1,Ib2,Ib3,vc);
              		  qg(Ib1,Ib2,Ib3,i)+=uy(Ib1,Ib2,Ib3);
            		}
            	      }
          	    }
          	    qg(I1,I2,I3,i)+=ug(I1,I2,I3,vc)*radiusInverse;  // add v/y except on the axis of symmetry
        	  }
      	
      	}
            
//       for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
//         if( cg0.numberOfDimensions()==1 )
//           q[grid](all,all,all,i)=u[grid].x()(all,all,all,uc);
//         else if( cg0.numberOfDimensions()==2 )
// 	{
//           q[grid](all,all,all,i)=u[grid].x()(all,all,all,uc)+u[grid].y()(all,all,all,vc); 
// 	  if( parameters.isAxisymmetric() )
// 	  {
//             // div(u) = u.x + v.y + v/y for y>0   or u.x + 2 v.y at y=0
// 	    realArray radiusInverse = 1./max(REAL_MIN,cg0[grid].vertex()(all,all,all,axis2));
//             Index Ib1,Ib2,Ib3;
//             for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
// 	    {
// 	      for( int side=0; side<=1; side++ )
// 	      {
// 		if( cg0[grid].boundaryCondition(side,axis)==Parameters::axisymmetric )
// 		{
// 		  getBoundaryIndex(cg0[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
// 		  radiusInverse(Ib1,Ib2,Ib3)=0.;
// 		  q[grid](Ib1,Ib2,Ib3,i)+=u[grid].y()(Ib1,Ib2,Ib3,vc);
// 		}
// 	      }
// 	    }
// 	    q[grid](all,all,all,i)+=u[grid](all,all,all,vc)*radiusInverse;
// 	  }
// 	}
//         else
//           q[grid](all,all,all,i)=u[grid].x()(all,all,all,uc)+u[grid].y()(all,all,all,vc)
//                                 +u[grid].z()(all,all,all,wc);
            }
            else if( showVariableName[n]=="vorticity" || showVariableName[n]=="vorticityZ"  )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  getIndex(cg0[grid].dimension(),I1,I2,I3);
                #ifdef USE_PPP
                            realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                            realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                #else
                            realSerialArray & qg = q[grid];
                            realSerialArray & ug = u[grid];
                #endif
                            ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                            if( !ok ) continue;  

        	  if( cg0.numberOfDimensions()==1 )
          	    qg(all,all,all,i)=0.;
        	  else 
        	  {
          	    RealArray vx(I1,I2,I3),uy(I1,I2,I3);
          	    MappedGridOperators & op = *(u[grid].getOperators());
          	    op.derivative(MappedGridOperators::xDerivative,ug,vx,I1,I2,I3,vc);
          	    op.derivative(MappedGridOperators::yDerivative,ug,uy,I1,I2,I3,uc);
          	    qg(I1,I2,I3,i)=vx-uy;
        	  }
//         if( cg0.numberOfDimensions()==1 )
//           q[grid](all,all,all,i)=0.;
//         else 
//           q[grid](all,all,all,i)=u[grid].x()(all,all,all,vc)-u[grid].y()(all,all,all,uc); // flipped sign 030215
      	}
            
            }
            else if( showVariableName[n]=="vorticityX" )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        	  if( cg0.numberOfDimensions()<3 )
          	    q[grid](all,all,all,i)=0.;
        	  else
          	    q[grid](all,all,all,i)=u[grid].y()(all,all,all,wc)-u[grid].z()(all,all,all,vc); // flipped sign 030215
            }
            else if( showVariableName[n]=="vorticityY" )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        	  if( cg0.numberOfDimensions()<3 )
          	    q[grid](all,all,all,i)=0.;
        	  else
          	    q[grid](all,all,all,i)=u[grid].z()(all,all,all,uc)-u[grid].x()(all,all,all,wc); // flipped sign 030215
            }
            else if( showVariableName[n]=="speed" )
            {
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  getIndex(cg0[grid].dimension(),I1,I2,I3);
                #ifdef USE_PPP
                            realSerialArray qg; getLocalArrayWithGhostBoundaries(q[grid],qg);
                            realSerialArray ug; getLocalArrayWithGhostBoundaries(u[grid],ug);
                #else
                            realSerialArray & qg = q[grid];
                            realSerialArray & ug = u[grid];
                #endif
                            ok = ParallelUtility::getLocalArrayBounds(q[grid],qg,I1,I2,I3,includeGhost);
                            if( !ok ) continue;  
        	  if( cg0.numberOfDimensions()==1 )
          	    qg(I1,I2,I3,i)=fabs(ug(I1,I2,I3,uc));
        	  else if( cg0.numberOfDimensions()==2 )
          	    qg(I1,I2,I3,i)=SQRT( SQR(ug(I1,I2,I3,uc))+SQR(ug(I1,I2,I3,vc)) );
        	  else
          	    qg(I1,I2,I3,i)=SQRT( SQR(ug(I1,I2,I3,uc))+SQR(ug(I1,I2,I3,vc))+SQR(ug(I1,I2,I3,wc)) );
      	}
            
            }
            else if( showVariableName[n]=="minimumScale" )
            {
	// save the INVERSE of the scaled minimum scale: SQRT( |Du| / [nu *( 1/dr^2 + 1/ds^2 )] )
      	real nu = max(REAL_MIN,parameters.dbase.get<real >("nu"));
            
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  const RealArray & dr = cg0[grid].gridSpacing();
        	  if( cg0.numberOfDimensions()==1 )
          	    q[grid](all,all,all,i)=SQRT(fabs(u[grid].x()(all,all,all,uc))* ((1./(nu*(1./SQR(dr(0)))))) );
        	  else if( cg0.numberOfDimensions()==2 )
        	  {
          	    q[grid](all,all,all,i)=SQRT(
            	      (fabs(u[grid].x()(all,all,all,uc))+fabs(u[grid].y()(all,all,all,uc))+
             	       fabs(u[grid].x()(all,all,all,vc))+fabs(u[grid].y()(all,all,all,vc)))
            	      *((1./(4.*.5*nu*( 1/SQR(dr(0)) + 1/SQR(dr(1)) ))))  ); // scale by number of terms in Du
        	  }
        	  else 
        	  {
          	    q[grid](all,all,all,i)=SQRT(
            	      (fabs(u[grid].x()(all,all,all,uc))+fabs(u[grid].y()(all,all,all,uc))+fabs(u[grid].z()(all,all,all,uc))+
             	       fabs(u[grid].x()(all,all,all,vc))+fabs(u[grid].y()(all,all,all,vc))+fabs(u[grid].z()(all,all,all,vc))+
             	       fabs(u[grid].x()(all,all,all,wc))+fabs(u[grid].y()(all,all,all,wc))+fabs(u[grid].z()(all,all,all,wc)))
            	      *((1./(9./3.*nu*( 1./SQR(dr(0)) + 1./SQR(dr(1)) +1./SQR(dr(2)) )))) );
        	  }
	  // printf(" **** grid=%i : max(min scale)=%e\n",grid,max(fabs(q[grid](all,all,all,i))));
      	}
            }
            else if( showVariableName[n]=="minimumScale1" || 
             	       showVariableName[n]=="minimumScale2" ||
             	       showVariableName[n]=="minimumScale3" )
            {
	// save the INVERSE of the scaled minimum scale: SQRT( |Du| / [nu *( 1/dr^2 + 1/ds^2 )] )
      	real nu = max(REAL_MIN,parameters.dbase.get<real >("nu"));
      	cg0.update(MappedGrid::THEvertexDerivative);
            
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      	{
        	  const RealArray & dr = cg0[grid].gridSpacing();
        	  const realMappedGridFunction & xr = cg0[grid].vertexDerivative();
        	  const realArray & v = u[grid];
        	  const intArray & mask = cg0[grid].mask();
      	
        	  Index I1,I2,I3;

        	  if( cg0.numberOfDimensions()==1 )
        	  {
          	    q[grid](all,all,all,i)=0.;
        	  }
        	  else if( cg0.numberOfDimensions()==2 )
        	  {
          	    realArray du,uNorm,xr1Norm,dur1,xr2Norm,dur2;
          	    du =(fabs(u[grid].x()(all,all,all,uc))+fabs(u[grid].y()(all,all,all,uc))+
             		 fabs(u[grid].x()(all,all,all,vc))+fabs(u[grid].y()(all,all,all,vc)))*.25;
                    
          	    const real eps = SQRT(REAL_MIN)*.01;
        	  
          	    uNorm =max(eps,evaluate(SQRT( SQR(v(I1,I2,I3,uc))+SQR(v(I1,I2,I3,vc)))));
        	  
          	    if( showVariableName[n]=="minimumScale1" )
          	    {
            	      xr1Norm =max(eps,SQRT(SQRT(SQR(xr(I1,I2,I3,0,0))+SQR(xr(I1,I2,I3,1,0)))));
            	      dur1 = fabs( (v(I1,I2,I3,uc)*xr(I1,I2,I3,0,0)+v(I1,I2,I3,vc)*xr(I1,I2,I3,1,0))/(xr1Norm*uNorm) );

	      // display(v(I1,I2,I3,uc),"u");
	      // display(v(I1,I2,I3,vc),"v");
	      // display(xr1Norm,"xr1Norm");
	      // display(uNorm,"uNorm");
	      // display(dur1,"dur1");
	      // display(du,"du");
          	    
	      // printf(" max(dur1)=%e, min(xr1Norm)=%e uNorm=%e\n",max(dur1),min(xr1Norm),min(uNorm));
          	    
            	      q[grid](all,all,all,i)=SQRT( du*dur1*SQR(dr(0))/nu );
          	    }
          	    else if( showVariableName[n]=="minimumScale2" )
          	    {
            	      xr2Norm =max(eps,SQRT(SQRT(SQR(xr(I1,I2,I3,0,1))+SQR(xr(I1,I2,I3,1,1)))));
            	      dur2 = fabs( (v(I1,I2,I3,uc)*xr(I1,I2,I3,0,1)+v(I1,I2,I3,vc)*xr(I1,I2,I3,1,1))/(xr2Norm*uNorm) );
            	      q[grid](all,all,all,i)=SQRT( du*dur2*SQR(dr(1))/nu );
          	    }

          	    where( mask(I1,I2,I3)==0 )
            	      q[grid](all,all,all,i)=0.;
          	    
	    // display(q[grid](all,all,all,i),"q");
        	  
        	  }
        	  else 
        	  {
          	    q[grid](all,all,all,i)=0.;
        	  }
	  // printf(" **** grid=%i : max(min scale)=%e\n",grid,max(fabs(q[grid](all,all,all,i))));
      	}
            }
            else if( parameters.getDerivedFunction(showVariableName[n],u,q,i,gf0.t,parameters)==0 )
            {
	// the show file variable was found as a derived function
            }
            else 
            {
      	cout << "saveShow: unknown showVariableName = " << (const char*) showVariableName[n] << endl;
      	for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        	  q[grid](all,all,all,i)=0.;
            }
        }


    }   // ***** END OLD WAY *****
    
  // fix this for parallel
    #ifndef USE_PPP
        q.interpolate();          // interpolate to get divergence correct  
    #endif
    
//    if( cg0.numberOfRefinementLevels()>1 )
//    {
//      printf(" ***saveShow: cg0-> computedGeometry & THErefinementLevel=%i \n",
//  	   cg0->computedGeometry & CompositeGrid::THErefinementLevel);
//      cg0->computedGeometry = cg0->computedGeometry | CompositeGrid::THErefinementLevel;
//    }
    

    if( parameters.dbase.get<int >("saveGridInShowFile") )
    {
        printF("***Save grid in the show file: numberOfComponentGrids=%i\n",cg0.numberOfComponentGrids());
        show.saveSolution( q,"u",Ogshow::useCurrentFrame );  // save the grid and the grid function
        parameters.dbase.get<int >("saveGridInShowFile")=false;
        parameters.dbase.get<int >("showFileFrameForGrid")=show.getNumberOfFrames();
    }
    else
    {
        printF("***Save solution in the show file: showFileFrameForGrid=%i (-2=default)\n",
                      parameters.dbase.get<int >("showFileFrameForGrid"));
        show.saveSolution( q,"u",parameters.dbase.get<int >("showFileFrameForGrid") );  // save the grid function
    }
    
  // Here we save time sequences to the show file
  // Only save if this is the last frame in a subFile
    if( parameters.dbase.get<bool >("saveSequencesEveryTime") && parameters.dbase.get<Ogshow* >("show")!=NULL &&
            parameters.dbase.get<Ogshow* >("show")->isLastFrameInSubFile() )
    {
    // *** NOTE: this code also appears in advance.bC **** FIX ME 

        saveSequencesToShowFile();
    // time sequence info for moving grids is saved here
        if( parameters.isMovingGridProblem() )
            parameters.dbase.get<MovingGrids >("movingGrids").saveToShowFile();

    // Save control sequences to the show file
        printF(" ++++++++++ save control sequences ? \n");
        if( parameters.dbase.has_key("Controller") )
        {
            Controller & controller = parameters.dbase.get<Controller>("Controller");
      // Controller & controller = *(parameters.dbase.get<Controller*>("Controller"));
            controller.saveToShowFile();
        }
        

    }
    
    if( debug() & 1 )
    {
        real time=ParallelUtility::getMaxValue(getCPU()-cpu0);
        printF("Time to save the solution in the show file = %8.2e (s)\n",time);  
    }
    
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForShowFile"))+=getCPU()-cpu0;
}


// ===================================================================================================================
/// \brief Save sequence info (such as the norm of the residual) into the time history arrays.
/// \param t0 (input) : current time.
/// \param residual (input) : holds the residual (when appropriate).
// ===================================================================================================================
int DomainSolver::
saveSequenceInfo( real t0, const realCompositeGridFunction & residual )
{
//    if( parameters.dbase.get<Ogshow* >("show")==NULL )
//      return 0;
//   Range N = parameters.dbase.get<Range >("Rt");

    real maximumResidual=0;
    real maximuml2 = 0.;

    getResidualInfo( t0, residual,maximumResidual, maximuml2, stdout );
    
    if( parameters.dbase.get<Ogshow* >("show")==NULL )
        return 0;

    int & sequenceCount = parameters.dbase.get<int >("sequenceCount");
    RealArray & timeSequence =  parameters.dbase.get<RealArray >("timeSequence");
    RealArray & sequence =  parameters.dbase.get<RealArray >("sequence");
    
    if( sequenceCount >= timeSequence.getLength(0) )
    { // allocate more space in the sequence arrays
        int num=timeSequence.getLength(0);
        Range R(0,num-1),all;
        RealArray seq;  seq=sequence;
        num=int(num*1.5+100);
        timeSequence.resize(num);
        sequence.redim(num,parameters.dbase.get<int >("numberOfSequences"));
        sequence(R,all)=seq;
    }

    timeSequence(sequenceCount)=t0;
    sequence(sequenceCount,0)=log10(max(REAL_MIN,maximumResidual));
    sequence(sequenceCount,1)=log10(max(REAL_MIN,maximuml2));
    sequenceCount++;
    return 0;
}



// ===================================================================================================================
/// \brief Save sequence info to the show file.
// ===================================================================================================================
int DomainSolver::
saveSequencesToShowFile()
{
    if( parameters.dbase.get<Ogshow* >("show")==NULL || parameters.dbase.get<int >("sequenceCount")<=0 )
        return 0;
    
    if( debug() & 4 )
        printF("Save sequences to the show file: sequenceCount=%i\n",parameters.dbase.get<int >("sequenceCount"));
    
    Range N(0,parameters.dbase.get<int >("sequenceCount")-1), S=parameters.dbase.get<int >("numberOfSequences");
    aString name[]={"maxResidualLog10","l2ResidualLog10"};
  // parameters.dbase.get<Ogshow* >("show")->startFrame(parameters.dbase.get<Ogshow* >("show")->getNumberOfFrames());
    parameters.dbase.get<Ogshow* >("show")->saveSequence("maxResidualLog10",parameters.dbase.get<RealArray>("timeSequence")(N),parameters.dbase.get<RealArray>("sequence")(N,S),name);

    return 0;
}

// ===================================================================================================================
/// \brief Save comments in the show file.
/// \param show (input) : show file.
// ===================================================================================================================
void DomainSolver::
saveShowFileComments( Ogshow &show )
{

    char buffer[80]; 
    aString showFileTitle[5];
    showFileTitle[0]=sPrintF(buffer,"%s",parameters.pdeName.c_str());
    showFileTitle[1] = "";

    for( int i=0; showFileTitle[i]!=""; i++ )
        show.saveComment(i,showFileTitle[i]);


}
