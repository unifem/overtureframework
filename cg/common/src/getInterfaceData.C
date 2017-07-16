#include "DomainSolver.h"
#include "Interface.h"  
#include "Parameters.h"

// ===================================================================================
/// \brief Return the requested interface data for the face of a grid
/// 
/// \param tSource: obtain data from source at this time
/// \param grid,side,axis : look for domain adjacent to this face.
///
// *wdh* initial version, Nov. 2016
// ===================================================================================
int
getInterfaceData( real tSource, int grid, int side, int axis, 
		  int interfaceDataOptions,
		  RealArray & data,
                  Parameters & parameters,
                  bool saveTimeHistory /* = false */  )
{
  printF("\n =============== START getInterfaceData (grid,side,axis)=(%i,%i,%i) =======================\n",
         grid,side,axis);
  


  // Here is the Cgmp object:
  DomainSolver *pCgmp = parameters.dbase.get<DomainSolver*>("multiDomainSolver");
  
  assert( pCgmp!=NULL );

  const int numberOfDomains = pCgmp->domainSolver.size();
  
  // look-up the domain number of this domain (the "target"): 
  const int targetDomain = parameters.dbase.get<int>("domainNumber");
  assert( targetDomain>=0 );
  
  // int sourceDomain = sourfaceDomainNumber(targetDomainNumber,grid,side,axis);
  
  printF("--DS-- getInterfaceData: This is a multi-domain problem: targetDomain=%i, numberOfDomains=%i\n",
	 targetDomain,numberOfDomains);
  
	
  InterfaceList & interfaceList = pCgmp->parameters.dbase.get<InterfaceList>("interfaceList");     

  
  // *** search for the source domain and face ******
  //   **DO THIS FOR NOW: FIX ME : store this info in a SparseArray ? ***
  int sourceDomain=-1, interfaceNumber=-1, sourceInterfaceSide=-1, sourceFace=-1;
  for( int inter=0; inter<interfaceList.size() && interfaceNumber==-1; inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // there may be multiple grid faces that lie on the interface:     
    for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face];
      GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face];
      
      const int d1=gridDescriptor1.domain, 
                grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
      const int d2=gridDescriptor2.domain, 
                grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;

      assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );

      printF(" Interface =%i, face=%i "
             "(domain1,grid1,side1,dir1)=(%i,%i,%i,%i) (domain2,grid2,side2,dir2)=(%i,%i,%i,%i)\n",
	     inter,face,d1,grid1,side1,dir1,d2,grid2,side2,dir2);
      
      if( d1==targetDomain && grid1==grid && side1==side && dir1==axis )
      {
        // target is side1, source is side2: 
	sourceDomain=d2; interfaceNumber=inter; sourceInterfaceSide=1; sourceFace=face;
	break;
      }
      if( d2==targetDomain && grid2==grid && side2==side && dir2==axis )
      {
        // target is side2, source is side1: 
	sourceDomain=d1; interfaceNumber=inter; sourceInterfaceSide=0; sourceFace=face;
	break;
      }
      // GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      // GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];
    } // end for face

  } // end for inter

  assert( interfaceNumber>=0 && sourceInterfaceSide>=0 && sourceFace>=0 );

  printF("... source found: sourceDomain=%i, interfaceNumber=%i face=%i interfaceSide=%i \n",
	 sourceDomain, interfaceNumber,sourceFace,sourceInterfaceSide);

  InterfaceDescriptor & interfaceDescriptor = interfaceList[interfaceNumber]; 
  GridList & gridListSource = sourceInterfaceSide==0 ? interfaceDescriptor.gridListSide1 : 
    interfaceDescriptor.gridListSide2;
  GridFaceDescriptor & gfd = gridListSource[sourceFace];

  GridFaceDescriptor info(sourceDomain,gfd.grid,gfd.side,gfd.axis);
  info.u = &data;   // set the pointer as to where the source data should be saved below

  // GridFaceDescriptor info;
  // GridFaceDescriptor gfd;
  
  // How do we determine which grid-function to use?
  int gfIndex=-1;  // this means get values at time=tSource

  assert( pCgmp->domainSolver[sourceDomain]!=NULL );
  DomainSolver & source = *(pCgmp->domainSolver[sourceDomain]);
  

  source.interfaceRightHandSide( DomainSolver::getInterfaceRightHandSide, // InterfaceOptionsEnum option, 
				 interfaceDataOptions,
				 info, // GridFaceDescriptor info;
				 gfd, //  GridFaceDescriptor gfd; the master GridFaceDescriptor. 
				 gfIndex, tSource, saveTimeHistory );
  
  // if( saveTimeHistory )
  // {
  //   if( interfaceDataOptions & Parameters::tractionInterfaceData )
  //   {
  //     // -- save a time history of the traction
  //     if( !gfd.dbase.has_key("tractionHistory") )
  //     {
  //       gfd.dbase.put<ArrayEvolution>("tractionHistory");
  //     }
  //     ArrayEvolution & tractionHistory = gfd.dbase.get<ArrayEvolution>("tractionHistory");

  //     // NOTE: array data may hold more than just the traction! **FIX ME**
  //     tractionHistory.add( tSource, data);
  //     printF("--GID-- Save traction time history at t=%9.3e\n",tSource);
  //   }
    
  // }
  

  printF(" =============== END getInterfaceData (grid,side,axis)=(%i,%i,%i) =======================\n",
         grid,side,axis);

  // OV_ABORT("stop here for now");
  
  return 0;
}

// ===================================================================================
/// \brief Return the Parameters object for the adjacent domain. Use this object
///  to look up parameters values from the adjacent domain.
/// 
/// \param grid,side,axis : look for domain adjacent to this face.
/// \Return value: parameter for the adjacent domain.
///
// ===================================================================================                
Parameters & 
getInterfaceParameters( int grid, int side, int axis, Parameters & parameters)
{
  // Here is the Cgmp object:
  DomainSolver *pCgmp = parameters.dbase.get<DomainSolver*>("multiDomainSolver");
  
  assert( pCgmp!=NULL );

  const int numberOfDomains = pCgmp->domainSolver.size();
  
  // look-up the domain number of this domain (the "target"): 
  const int targetDomain = parameters.dbase.get<int>("domainNumber");
  assert( targetDomain>=0 );
  
  // int sourceDomain = sourfaceDomainNumber(targetDomainNumber,grid,side,axis);
  
  if( false )
    printF("--DS-- getInterfaceParameters: This is a multi-domain problem: targetDomain=%i, numberOfDomains=%i\n",
           targetDomain,numberOfDomains);
	
  InterfaceList & interfaceList = pCgmp->parameters.dbase.get<InterfaceList>("interfaceList");     

  
  // *** search for the source domain and face ******
  //   **DO THIS FOR NOW: FIX ME : store this info in a SparseArray ? ***
  int sourceDomain=-1, interfaceNumber=-1, sourceInterfaceSide=-1, sourceFace=-1;
  for( int inter=0; inter<interfaceList.size() && interfaceNumber==-1; inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // there may be multiple grid faces that lie on the interface:     
    for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face];
      GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face];
      
      const int d1=gridDescriptor1.domain, 
                grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
      const int d2=gridDescriptor2.domain, 
                grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;

      assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );

      printF(" Interface =%i, face=%i "
             "(domain1,grid1,side1,dir1)=(%i,%i,%i,%i) (domain2,grid2,side2,dir2)=(%i,%i,%i,%i)\n",
	     inter,face,d1,grid1,side1,dir1,d2,grid2,side2,dir2);
      
      if( d1==targetDomain && grid1==grid && side1==side && dir1==axis )
      {
        // target is side1, source is side2: 
	sourceDomain=d2; interfaceNumber=inter; sourceInterfaceSide=1; sourceFace=face;
	break;
      }
      if( d2==targetDomain && grid2==grid && side2==side && dir2==axis )
      {
        // target is side2, source is side1: 
	sourceDomain=d1; interfaceNumber=inter; sourceInterfaceSide=0; sourceFace=face;
	break;
      }
      // GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      // GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];
    } // end for face

  } // end for inter

  assert( interfaceNumber>=0 && sourceInterfaceSide>=0 && sourceFace>=0 );

  // printF("... source found: sourceDomain=%i, interfaceNumber=%i face=%i interfaceSide=%i \n",
  //        sourceDomain, interfaceNumber,sourceFace,sourceInterfaceSide);

  assert( pCgmp->domainSolver[sourceDomain]!=NULL );
  DomainSolver & source = *(pCgmp->domainSolver[sourceDomain]);

  return source.parameters;
  
}

