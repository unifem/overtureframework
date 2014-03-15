// ======================================================================================================
//  This macro will update the grids and grid functions when using AMR
// ======================================================================================================
#beginMacro updateForAdaptiveGridsMacro()
// --- Start AMR ---

const int regridFrequency = parameters.dbase.get<int >("amrRegridFrequency")>0 ? 
                            parameters.dbase.get<int >("amrRegridFrequency") :
                            parameters.dbase.get<Regrid* >("regrid")==NULL ? 2 : 
                            parameters.dbase.get<Regrid* >("regrid")->getRefinementRatio();

if( parameters.isAdaptiveGridProblem() && ((globalStepNumber % regridFrequency) == 0) )
{
  // ****************************************************************************
  // ****************** Adaptive Grid Step  *************************************
  // ****************************************************************************

  if( debug & 2 )
  {
    printP("***** advance: AMR regrid at step %i t=%e dt=%8.2e***** \n",globalStepNumber,t,dt);
    fPrintF(debugFile,"***** advance: AMR regrid at step %i t=%e dt=%8.2e***** \n",globalStepNumber,t,dt);
  }
  
  real timea=getCPU();
 
  if( debug & 4 )
    fPrintF(debugFile,"\n ***** advance: AMR regrid at step %i ***** \n\n",globalStepNumber);


  if( debug & 8 )
  {
    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      getErrors( current,t,dt,sPrintF(" advance: errors before regrid, t=%e \n",t) );
    }
    else
    {
      fPrintF(debugFile," ***advance: before regrid: solution ***\n");
      outputSolution( gf[current].u,t );
    }
  }


  int numberToUpdate=0; // we need to update ub to live on the new grid, and interpolate values.
  if( ((SmParameters&)parameters).isSecondOrderSystem() )
  {
    numberToUpdate=1;  // also update and interpolate prev solution to the new grid 
  }
  
  adaptGrids( gf[current], numberToUpdate,&(gf[prev].u), NULL );  // last arg is for work-space **fix me **

  // *wdh* do this: 090315
  cg.reference(gf[current].cg);
     gf[prev].cg.reference(gf[current].cg);
     gf[next].cg.reference(gf[current].cg);
     gf[next].u.updateToMatchGrid(cg);

  // printF(" After adaptGrids: prev=%i, current=%i, next=%i\n",prev,current,next);
  for( int n=0; n<numberOfTimeLevels; n++ )
  {
    if( n!=current )
    {
	gf[n].cg.reference(gf[current].cg);  //
	if( n!=prev ) // this was already done for prev in adaptGrids
	  gf[n].u.updateToMatchGrid(gf[current].cg);
    }
    
    gf[n].u.setOperators(*cgop);
    // printF(" After adaptGrids: gf[%i].cg.numberOfComponentGrids = %i\n",n,gf[n].cg.numberOfComponentGrids());
    // printF(" After adaptGrids: gf[%i].u.getCompositeGrid()->numberOfComponentGrids = %i\n",n,gf[n].u.getCompositeGrid()->numberOfComponentGrids());
  }

  // ** do this for now ** fix me **
  if( checkErrors )
  {
    assert( cgerrp!=NULL );
    (*cgerrp).updateToMatchGrid(cg);
  }

  
  // the next has been moved into adaptGrids 070706
  //     real time1=getCPU();
  //     cgf1.cg.rcData->interpolant->updateToMatchGrid( cgf1.cg ); 
  //     parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-time1;
  real time1=getCPU();
  
  if( debug & 8 )
  {
    outputSolution( gf[current].u,t,
                   sPrintF(" advance:after adaptGrids, before interpAndApplyBC at t=%11.4e \n",t) );
  }

  interpolateAndApplyBoundaryConditions( gf[current] );

  // *wdh* 090829
  if( numberToUpdate==1 )
  {
    interpolateAndApplyBoundaryConditions( gf[prev] );
  }
  

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrBoundaryConditions"))+=getCPU()-time1;    

  if( debug & 4 )
  {
    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      getErrors( prev   ,t-dt,dt,sPrintF(" advance: errors in prev    after regrid, t=%e \n",t-dt) );
      getErrors( current,t   ,dt,sPrintF(" advance: errors in current after regrid, t=%e \n",t) );
    }
    else
    {
      fPrintF(debugFile," ***after regrid: solution ***\n");
      outputSolution( gf[current].u,t );
    }
  }
  
    
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrRegrid"))+=getCPU()-timea;
}



// --- End AMR ---
#endMacro
