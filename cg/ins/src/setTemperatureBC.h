// ----------------------------------------------------------------------------
// Macro: Apply BC's on the Temperature 
// 
//   There are 3 cases: 
//      (1) apply a dirichlet BC                       (OPTION=dirichlet)
//      (2) extrapolate ghost pts on dirichlet BC's     (OPTION=extrapolateGhost)
//      (3) apply a mixed BC                           (OPTION=mixed)
// 
// Macro args:
// 
// tc : component to assign
// NAME : name of of the calling function (for comments)
// BCNAME : noSlipWall, inflowWithVelocityGiven etc. 
// OPTION: dirichlet, mixed, extrapolateGhost
// ----------------------------------------------------------------------------
#beginMacro setTemperatureBC(tc,NAME,BCNAME,OPTION)
 if( assignTemperature )
 {
  #If #OPTION == "dirichlet" || #OPTION == "mixed" || #OPTION == "extrapolateGhost"
  #Else
   Overture::abort("ERROR in calling setTemperatureBC macro with option=OPTION");
  #End

   FILE *& debugFile  =  parameters.dbase.get<FILE* >("debugFile");
   FILE *& pDebugFile =  parameters.dbase.get<FILE* >("pDebugFile");
   ForBoundary(side,axis)
   {

     if( mg.boundaryCondition(side,axis)==BCNAME )
     {

       if( interfaceType(side,axis,grid)!=Parameters::noInterface )
       { // This is an interface between domains

         // for now we only know about interfaces at no-slip walls: 
         assert( mg.boundaryCondition(side,axis)==noSlipWall );

	 // what about BC's applied at t=0 before the boundary data is set ??
	 // if( parameters.dbase.get<int >("globalStepNumber") < 2 ) continue; // ********************* TEMP *****

	 // if this is an iterface we should turn off the TZ forcing for the boundary condition since we want
	 // to use the boundary data instead.
         #ifdef USE_PPP
           realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
         #else
           const realSerialArray & uLocal = u;
         #endif

	 Index Ib1,Ib2,Ib3;
	 getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	 bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ib1,Ib2,Ib3);

	 if( debug() & 4 )
	 {
	   printP("NAME:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
		  side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));

	   fprintf(pDebugFile,"NAME:applyBC: apply a mixed BC on the interface (side,axis,grid)=(%i,%i,%i) %f*T + %f*T.n \n",
			       side,axis,grid,mixedCoeff(tc,side,axis,grid),mixedNormalCoeff(tc,side,axis,grid));
	   if( pBoundaryData[side][axis]==NULL )
	   {
	     if( !ok )
               fprintf(pDebugFile," NAME:applyBC on T: pBoundaryData[side][axis] == NULL! \n");
             else
               fprintf(pDebugFile," NAME:applyBC on T: ERROR: pBoundaryData[side][axis] == NULL! \n");
	   }
	   else
	   {
	     // RealArray & bd = *pBoundaryData[side][axis];
	     // ::display(bd,"boundaryData",pDebugFile,"%8.2e ");
	   }
	   
	 }

         assert( mixedCoeff(tc,side,axis,grid)!=0. || mixedNormalCoeff(tc,side,axis,grid)!=0. );

	 if( ok && pBoundaryData[side][axis]==NULL )
	 {
           // At t=0 when the initial conditions are being set-up (and initial conditions projected) there may
           // not be a boundaryData array created yet for the interface. We thus create one and fill in some default values
           // based on the current solution
           RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
           bd=0.;

           #ifdef USE_PPP
	     const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
           #else
	     const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
           #endif
	   real a0=mixedCoeff(tc,side,axis,grid);
	   real a1=mixedNormalCoeff(tc,side,axis,grid);

           // bd(Ib1,Ib2,Ib3,tc)=a0*uLocal(Ib1,Ib2,Ib3,tc);    // Assume that T.n=0 at the interface at t=0 -- could do better --

           MappedGridOperators & op = *(u.getOperators());
           Range N(tc,tc);
           RealArray ux(Ib1,Ib2,Ib3,N), uy(Ib1,Ib2,Ib3,N);
	   op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,N);
	   op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,N);
           if( mg.numberOfDimensions()==2 )
	   {
             bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
	   }
	   else
	   {
             RealArray uz(Ib1,Ib2,Ib3,N);
	     op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,N);
	     bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*ux(Ib1,Ib2,Ib3,tc)+
				    normal(Ib1,Ib2,Ib3,1)*uy(Ib1,Ib2,Ib3,tc)+
				    normal(Ib1,Ib2,Ib3,2)*uz(Ib1,Ib2,Ib3,tc)) + a0*uLocal(Ib1,Ib2,Ib3,tc);
	   }
	   
           const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
	   if( false && twilightZoneFlow ) //  *******************************************************
	   {
             fprintf(pDebugFile," NAME:applyBC on T: ********** Set TRUE value for bd at t=%8.2e ************\n",t);
	     const bool rectangular= mg.isRectangular() && !twilightZoneFlow;
      
	     realArray & x= mg.center();
#ifdef USE_PPP
	     realSerialArray xLocal; 
	     if( !rectangular || twilightZoneFlow ) 
	       getLocalArrayWithGhostBoundaries(x,xLocal);
#else
	     const realSerialArray & xLocal = x;
#endif

	     OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	     realSerialArray ue(Ib1,Ib2,Ib3), uex(Ib1,Ib2,Ib3), uey(Ib1,Ib2,Ib3); 
	     e.gd( ue ,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,0,Ib1,Ib2,Ib3,tc,t);
	     e.gd( uex,xLocal,mg.numberOfDimensions(),rectangular,0,1,0,0,Ib1,Ib2,Ib3,tc,t);
	     e.gd( uey,xLocal,mg.numberOfDimensions(),rectangular,0,0,1,0,Ib1,Ib2,Ib3,tc,t);

	     real a0=mixedCoeff(tc,side,axis,grid), a1=mixedNormalCoeff(tc,side,axis,grid);
	     if( mg.numberOfDimensions()==2 )
	     {
	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
	     }
	     else
	     {
	       realSerialArray uez(Ib1,Ib2,Ib3);
	       e.gd( uez,xLocal,mg.numberOfDimensions(),rectangular,0,0,0,1,Ib1,Ib2,Ib3,tc,t);
	       bd(Ib1,Ib2,Ib3,tc)=a1*(normal(Ib1,Ib2,Ib3,0)*uex(Ib1,Ib2,Ib3)+
				      normal(Ib1,Ib2,Ib3,1)*uey(Ib1,Ib2,Ib3)+
				      normal(Ib1,Ib2,Ib3,2)*uez(Ib1,Ib2,Ib3)) + a0*ue(Ib1,Ib2,Ib3);
	     }
	   } // *******************************************
	 }
	 


	 if( pBoundaryData[side][axis]!=NULL )
	   u.getOperators()->setTwilightZoneFlow( false );
	 else
	 {
           if( t>0. || debug() & 4 )
	   {
             if( ok )
    	       printP("$$$$ NAME:applyBC:INFO:interface but no boundaryData, t=%9.3e\n",t);
	   }
	 }
       }

	  
       if( debug() & 4 )
	 printF("++++NAME: BCNAME: (grid,side,axis)=(%i,%i,%i) : "
		"  BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
		"  BC: u: %3.2f*u+%3.2f*u.n=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
		grid,side,axis, 
		mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
		mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
		mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
	   );

//        if( mixedNormalCoeff(tc,side,axis,grid)!=0. ) // coeff of T.n is non-zero
//        {
// 	 mixedBoundaryConditionOnTemperature=true;

// 	 if( debug() & 4 )
// 	   printF("++++insBC: BCNAME:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
// 		  "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f,  \n"
//                   "  BC: u: %3.2f*u+%3.2f*u.n=%3.2f=%3.2f,  v: %3.2f*v+%3.2f*v.n=%3.2f \n",
// 		  grid,side,axis, 
//                   mixedCoeff(tc,side,axis,grid), mixedNormalCoeff(tc,side,axis,grid), mixedRHS(tc,side,axis,grid), 
//                   mixedCoeff(uc,side,axis,grid), mixedNormalCoeff(uc,side,axis,grid), mixedRHS(uc,side,axis,grid), 
//                   mixedCoeff(vc,side,axis,grid), mixedNormalCoeff(vc,side,axis,grid), mixedRHS(vc,side,axis,grid)
// 	     );
//        }

       if( mixedNormalCoeff(tc,side,axis,grid)==0. ) // coeff of T.n 
       {
	 // Dirichlet
#If #OPTION == "dirichlet"
	 u.applyBoundaryCondition(tc,dirichlet,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
				  bcParams,grid);
#Elif #OPTION == "extrapolateGhost"
	 u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
// 	      u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
// 				       bcParams,grid);
#End
       }
       else if( bd.hasVariableCoefficientBoundaryCondition(side,axis) )
       {
         // -- Variable Coefficient Temperature (const coeff.) BC --- 

	 printF("setTemperatureBC:INFO: grid=%i (side,axis)=(%i,%i) HAS a var coeff. temperature BC!\n",
		grid,side,axis);

         // BC is : a0(x)*T + an(x)*T.n = g 
         //  a0 = varCoeff(i1,i2,i3,0)
         //  an = varCoeff(i1,i2,i3,1)
	 RealArray & varCoeff = bd.getVariableCoefficientBoundaryConditionArray(
                                        BoundaryData::variableCoefficientTemperatureBC,side,axis );

         bcParams.setVariableCoefficientsArray(&varCoeff);
	 
	 u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
				  bcParams,grid);

         bcParams.setVariableCoefficientsArray(NULL);  // reset 

       } 
       else
       {
	 // --- Mixed or Neumann Temperature (const coeff.) BC ---


#If #OPTION == "mixed" 

	 // Mixed BC or Neumann
	 real a0=mixedCoeff(tc,side,axis,grid);
	 real a1=mixedNormalCoeff(tc,side,axis,grid);
	 bcParams.a.redim(3);
	 if( a0==0. && a1==1. )
	 {
	   if( debug() & 4 )
	     printF("++++NAME: BCNAME:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : apply neumannBC\n",
		    grid,side,axis);

//                 real b0=bcData(tc+2,side,axis,grid);
// 		u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),b0,t); // b0 ignored??

	   bcParams.a(0)=a0;
	   bcParams.a(1)=a1;
	   bcParams.a(2)=mixedRHS(tc,side,axis,grid);  // this is not used -- this does not work
	   if( false )
	   {  // **** TEMP FIX ****
	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
	   }
	   else
	   {
	     u.applyBoundaryCondition(tc,neumann,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
				      bcParams,grid);
	   }
	 }
	 else
	 {
	   
	   if( debug() & 4 )
	   {
	     fPrintF(pDebugFile,"++++NAME:BCNAME:adiabaticWall: (grid,side,axis)=(%i,%i,%i) : "
		    "Mixed BC for T: %3.2f*T+%3.2f*T.n=%3.2f (t=%8.2e)\n",
		     grid,side,axis,a0,a1,bcData(tc,side,axis,grid),t);
	   }
	   if( debug() & 4 )
	   {
             #ifndef USE_PPP
  	      Index Ib1,Ib2,Ib3;
	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
              RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);
	      ::display(bd(Ib1,Ib2,Ib3,tc),"NAME:BCNAME:T: RHS for mixed BC: bd(Ib1,Ib2,Ib3,tc)",pDebugFile,"%5.2f ");
  	      Index Ig1,Ig2,Ig3;
	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
              ::display(u(Ig1,Ig2,Ig3,tc),"NAME:BCNAME:T: BEFORE mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
             #endif
	   }

	   bcParams.a(0)=a0;
	   bcParams.a(1)=a1;
	   bcParams.a(2)=mixedRHS(tc,side,axis,grid); 
	   if( false )
	   {  // **** TEMP FIX ****
	     u.applyBoundaryCondition(tc,extrapolate,BCTypes::boundary(side,axis),0.,t);
	   }
	   else
	   {
	     u.applyBoundaryCondition(tc,mixed,BCTypes::boundary(side,axis),bcData,pBoundaryData,t,
				      bcParams,grid);
	   }
	   
	   if( debug() & 4 )
	   {
             #ifndef USE_PPP
  	      Index Ig1,Ig2,Ig3;
	      // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);
	      getIndex(mg.gridIndexRange(),Ig1,Ig2,Ig3,1);
              ::display(u(Ig1,Ig2,Ig3,tc),"NAME:BCNAME:T: AFTER mixed BC: u(I1,I2,I3,tc)",pDebugFile,"%5.2f ");
             #endif
	   }

	 }
		
#End
	 
       }

       if( interfaceType(side,axis,grid)!=Parameters::noInterface )
       { // reset TZ
	 u.getOperators()->setTwilightZoneFlow( parameters.dbase.get<bool >("twilightZoneFlow") );
       }

     } // end if bc = BCNAME

   } // end for boundary

   // ************ try this ********* 080909
   // u.updateGhostBoundaries(); // this is done in finish boundary conditions now
   
 } // end if assignTemperature
#endMacro
