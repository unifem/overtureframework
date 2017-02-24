#include "Cgmp.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "App.h"
#include "Interface.h"
#include "InterfaceTransfer.h"

#define FOR_4D(i1,i2,i3,i4,I1,I2,I3,I4) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
for(i4=I4Base; i4<=I4Bound; i4++) \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_4(i1,i2,i3,i4,I1,I2,I3,I4) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
for(i4=I4Base; i4<=I4Bound; i4++) \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


#define FOR_4IJD(i1,i2,i3,i4,I1,I2,I3,I4,j1,j2,j3,j4,J1,J2,J3,J4) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase(),  J4Base =J4.getBase();  \
for(i4=I4Base,j4=J4Base; i4<=I4Bound; i4++,j4++) \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)

#define FOR_4IJ(i1,i2,i3,i4,I1,I2,I3,I4,j1,j2,j3,j4,J1,J2,J3,J4) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase(),  J4Base =J4.getBase();  \
for(i4=I4Base,j4=J4Base; i4<=I4Bound; i4++,j4++) \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


bool Cgmp::
checkIfInterfacesMatch(Mapping &map1, int &dir1, int &side1, Mapping &map2, int &dir2, int &side2)
{
  bool interfacesMatch = false;
  int numberOfDimensions = map1.getRangeDimension();
  if (map1.getRangeDimension()!=map1.getDomainDimension()) return false; // then we have a problem, should be an assert? 
  realSerialArray r(1,3),x(1,3),xp(1,3); // arrays for geometric tests
  real dr = 0.01; // parameter space displacement from the boundary used when doing geometric tests

  r = x = xp = 0;
  /// invert points from grid1 onto grid2
  for ( int s=0; s<2 && !interfacesMatch; s++ ) 
  {
    r( 0,dir1) = (real)side1;
    r( 0, (dir1+1)%numberOfDimensions ) = (1-2*s)*dr;
    if ( numberOfDimensions==3 ) r( 0, (dir1+2)%numberOfDimensions ) =         dr;
	
    map1.mapS(r,x);
    r = -1;
    if ( numberOfDimensions==2 ) r(0,2) = 0;
    map2.inverseMapS(x,r);
    bool wasInvertedOntoMapping = fabs(r(0,dir2)-(side2))<100*REAL_EPSILON; // make sure the inversion happend onto the correct boundary
    for ( int a=0; a<numberOfDimensions && wasInvertedOntoMapping; a++ ) // make sure the inversion was still within the bounds of the mapping
      wasInvertedOntoMapping = wasInvertedOntoMapping && ( r(0,a)>-100*REAL_EPSILON && r(0,a)<(1+100*REAL_EPSILON) ); // should have a tolerance probably...
    if ( wasInvertedOntoMapping )
    {
      map2.mapS(r,xp);
      interfacesMatch = ( SQR(x(0,0)-xp(0,0)) + SQR(x(0,1)-xp(0,1)) + SQR(x(0,2)-x(0,2)) ) < 100*REAL_EPSILON; // ahh, my old friend, 100*REAL_EPSILON
    }

#if 0
    if ( interfacesMatch )
    {
      cout<<map1.getName(Mapping::mappingName)<<" matched to "<<map2.getName(Mapping::mappingName)<<endl;
      cout<<r(0,0)<<"   "<<r(0,1)<<endl;
      x.display("X");
      xp.display("XP");
    }
#endif

    if ( !interfacesMatch && numberOfDimensions==3 ) // check the other corner
    {
      r( 0,dir1) = (real)side1;
      r( 0, (dir1+1)%numberOfDimensions ) = (1-2*s)*dr;
      r( 0, (dir1+2)%numberOfDimensions ) =        -dr; // NOTE the - sign is the only change from the above block!!!
	    
      map1.mapS(r,x);
      r = -1;
      map2.inverseMapS(x,r);
      bool wasInvertedOntoMapping = fabs(r(0,dir2)-(side2))<100*REAL_EPSILON; // make sure the inversion happend onto the correct boundary
      for ( int a=0; a<numberOfDimensions && wasInvertedOntoMapping; a++ )
	wasInvertedOntoMapping = wasInvertedOntoMapping && ( r(0,a)>-100*REAL_EPSILON && r(0,a)<(1+100*REAL_EPSILON) ); // should have a tolerance probably...
      if ( wasInvertedOntoMapping )
      {
	map2.mapS(r,xp);
	interfacesMatch = ( SQR(x(0,0)-xp(0,0)) + SQR(x(0,1)-xp(0,1)) + SQR(x(0,2)-x(0,2)) ) < 100*REAL_EPSILON; // ahh, my old friend, 100*REAL_EPSILON
      } // if was inverted ok
	    
    } // if the previous interface did not match
  } // for each side on the face we are matching
  return interfacesMatch;
} // checkIfInterfacesMatch



// void
// getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
//                                      IntegerArray & gidLocal, 
//                                      IntegerArray & dimensionLocal, 
//                                      IntegerArray & bcLocal );

#define interfaceCgCm EXTERN_C_NAME(interfacecgcm)
extern "C"
{
void interfaceCgCm( const int&nd, 
		    const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
		    const int&gridIndexRange1, real&u1, const int&mask1,const real&rsxy1, const real&xy1, 
		    const int&boundaryCondition1, 
		    const int&md1a,const int&md1b,const int&md2a,const int&md2b,const int&md3a,const int&md3b,
		    const int&gridIndexRange2, real&u2, const int&mask2,const real&rsxy2, const real&xy2, 
		    const int&boundaryCondition2,
		    const int&ipar, const real&rpar, 
		    real&aa2, real&aa4, real&aa8, 
		    int&ipvt2, int&ipvt4, int&ipvt8,
		    int&ierr );
}




// =====================================================================================================
//
///  \brief Find the interfaces and initialize the work-space. 
/// \details Interfaces are located by faces on different domains that have an interface BC and
///    have the same share flag.
/// \param gfIndex (input) : Domain d should use the grid function : domainSolver[d]->gf[gfIndex[d]]
/// \notes This function is called by setParametersInteractively
// =====================================================================================================
int Cgmp::
initializeInterfaces(std::vector<int> & gfIndex)
{
  real cpu0=getCPU();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];


  // **** Make a list of grids on each side of each the interface. *****
  //      In general there will be multiple grids on each side that meet at the interface (e.g. AMR grids)

  
  const int numberOfDomains = domainSolver.size();
  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");

  if( interfaceList.size()>0 )
  {
    printF("Cgmp::initializeInterfaces:INFO: interfaceList being cleared, this must be for AMR? isAdaptiveGridProblem=%i, ...\n",
           parameters.isAdaptiveGridProblem());
    assert( parameters.isAdaptiveGridProblem() );
    interfaceList.clear();
  }


  assert( interfaceList.size()==0 );

  
  fPrintF(interfaceFile,"\n ------- Cgmp::initializeInterfaces: Here are the input domains ----------\n");
  ForDomain(dm)
  {
    const IntegerArray & originalBoundaryCondition = 
      domainSolver[dm]->parameters.dbase.get<IntegerArray>("originalBoundaryCondition");
    const IntegerArray & interfaceType = 
      domainSolver[dm]->parameters.dbase.get<IntegerArray >("interfaceType");

    CompositeGrid & cg = domainSolver[dm]->gf[gfIndex[dm]].cg;
    const int numberOfDimensions = cg.numberOfDimensions();
    fPrintF(interfaceFile,"**** domain %i %s (%s) : *******\n"
	    "   side axis grid interfaceType bc(orig) bc   share\n",dm,
            (const char*)domainSolver[dm]->getName(),
            (const char*)domainSolver[dm]->getClassName());
    for( int g=0; g<cg.numberOfComponentGrids(); g++ )
    {
      MappedGrid & mg = cg[g];
      const IntegerArray & bc = mg.boundaryCondition();
      const IntegerArray & share = mg.sharedBoundaryFlag();
      const int baseGrid = cg.baseGridNumber(g);	    
      for( int s=0; s<=1; s++) for( int d=0; d<numberOfDimensions; d++ )
      {
	fPrintF(interfaceFile,"   %2i  %2i  %4i   %5i         %3i     %3i    %3i\n",s,d,g,interfaceType(s,d,g),
		originalBoundaryCondition(s,d,baseGrid),bc(s,d),share(s,d)  );
      }
    }
  }


  // -------------------------------------------------------------------
  // ----- Loop over domains looking for grid faces on interfaces ------
  // -------------------------------------------------------------------
  ForDomain(d1)
  {
    CompositeGrid & cg1 = domainSolver[d1]->gf[gfIndex[d1]].cg;
    const int numberOfDimensions = cg1.numberOfDimensions();
    const IntegerArray & originalBoundaryCondition1 = 
                domainSolver[d1]->parameters.dbase.get<IntegerArray>("originalBoundaryCondition");
    const IntegerArray & interfaceType1 = 
                domainSolver[d1]->parameters.dbase.get<IntegerArray >("interfaceType");

    // --- look for base grids with an interface BC ---
    // for( int grid1=0; grid1<cg1.numberOfBaseGrids(); grid1++ ) 
    // --- look for all component grids (for AMR) : we should treat AMR grids in a special way --- *wdh* 100801
    for( int grid1=0; grid1<cg1.numberOfComponentGrids(); grid1++ ) 
    {

      MappedGrid & mg1 = cg1[grid1];
      const IntegerArray & bc1 = mg1.boundaryCondition();
      const IntegerArray & share1 = mg1.sharedBoundaryFlag();
      const int baseGrid1 = cg1.baseGridNumber(grid1);  // base grid number (for AMR grids)

      // check for interface boundary conditions
      for( int dir1=0; dir1<mg1.numberOfDimensions(); dir1++ )
      {
	for( int side1=0; side1<=1; side1++ )
	{
	  if( interfaceType1(side1,dir1,grid1)!=Parameters::noInterface )  
	  {
            // --- This face (side1,dir1,grid1,d1) lies on an interface ---

	    if( true )
	    {
	      // --- new way ----
	      // The interface is assumed to be uniquely determined by :
	      //     share1(side1,dir1)
	      //     originalBoundaryCondition1(side1,dir1,grid1)


	      // --- Add this face to a new or existing interface ---
              int matchingInterface=-1;
	      int interfaceSide=-1;
	      for( int inter=0; inter < interfaceList.size(); inter++ ) // check existing interfaces
	      {
		InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

                // The interface domain must match "d1" or interfaceDescriptor.domain2 must not be assigned yet: 
		if( interfaceDescriptor.domain1==d1 || interfaceDescriptor.domain2==d1 || interfaceDescriptor.domain2==-1 )
		{

                  // We compare to the first face on interfaceSide==0 (which may or may not be from domain d1)
                  // (We could compare to any existing face on either side of the interface.)
		  GridList & gridList = interfaceDescriptor.gridListSide1;

		  assert( gridList.size()>0 );
		  
		  GridFaceDescriptor & gridDescriptor = gridList[0];  // first face on this interface
		  const int d2=gridDescriptor.domain, grid2=gridDescriptor.grid, side2=gridDescriptor.side, dir2=gridDescriptor.axis;

		  CompositeGrid & cg2 = domainSolver[d2]->gf[gfIndex[d2]].cg;
		  const IntegerArray & originalBoundaryCondition2 = 
		    domainSolver[d2]->parameters.dbase.get<IntegerArray>("originalBoundaryCondition");
                    
                  // NOTE: check originalBoundaryCondition using the baseGrid numbers
                  const int baseGrid2 = cg2.baseGridNumber(grid2);
		  if( mg1.sharedBoundaryFlag(side1,dir1) == cg2[grid2].sharedBoundaryFlag(side2,dir2) &&
		      originalBoundaryCondition1(side1,dir1,baseGrid1)==originalBoundaryCondition2(side2,dir2,baseGrid2) )
		  {
                    assert( bc1(side1,dir1)>0 && cg2[grid2].boundaryCondition(side2,dir2)>0 );  // sanity check for AMR
		    matchingInterface=inter;
  		    interfaceSide = interfaceDescriptor.domain1==d1 ? 0 : 1;  // new face will belong to this side of the interface 
		    break;
		  }
		}
	      }
	      if( interfaceList.size()==0 )
		fPrintF(interfaceFile,"\n**********************************************************************\n");

	      if( matchingInterface>=0 )
	      {
                // Add a face to an existing interface

		InterfaceDescriptor & interfaceDescriptor = interfaceList[matchingInterface];
                if( interfaceSide==1 && interfaceDescriptor.domain2==-1 )
		  interfaceDescriptor.domain2=d1;  // this was the first face on this side of the interface 

		GridList & gridList = interfaceSide==0 ? interfaceDescriptor.gridListSide1 : interfaceDescriptor.gridListSide2;
                // add a new face to this grid list
                gridList.push_back(GridFaceDescriptor(d1,grid1,side1,dir1));

                fPrintF(interfaceFile," Add (domain=%i,grid=%i,side=%i,dir=%i,share=%i) to side %i of the existing interface=%i.\n",
			d1,grid1,side1,dir1,share1(side1,dir1),interfaceSide,matchingInterface);
	      }
	      else
	      {
                // No existing interface was found, make a new one

		interfaceList.push_back(InterfaceDescriptor());  // add a new interface to the list
		InterfaceDescriptor & interface =  interfaceList.back(); 

		interface.domain1=d1;
		interface.domain2=-1; // this means that no domain has been associated with this side of the interface yet.
		interface.gridListSide1.push_back(GridFaceDescriptor(d1,grid1,side1,dir1));

                fPrintF(interfaceFile," Add (domain=%i,grid=%i,side=%i,dir=%i,share=%i) to side %i of the new interface=%i.\n",
			d1,grid1,side1,dir1,share1(side1,dir1),interfaceSide,interfaceList.size()-1);
	      }



	    }
	    else
	    {
	      // --- old way 



	      // **** Find the adjacent base grid that matches this interface ****
	      bool foundAnInterfaceForThisBoundary = false;
	      bool interfaceFoundButDoesNotMatchGeometrically = false;

	      // --- look at higher numbered domains for faces that also lie on this same interface ---
	      for( int d2=d1+1; d2<numberOfDomains; d2++ ) if( domainSolver[d2]!=NULL )
	      {
		CompositeGrid & cg2 = domainSolver[d2]->gf[gfIndex[d2]].cg;
		const IntegerArray & originalBoundaryCondition2 = 
		  domainSolver[d2]->parameters.dbase.get<IntegerArray>("originalBoundaryCondition");

		const IntegerArray & interfaceType2 = 
		  domainSolver[d2]->parameters.dbase.get<IntegerArray >("interfaceType");

		for( int grid2=0; grid2<cg2.numberOfComponentGrids(); grid2++ )
		{

		  MappedGrid & mg2 = cg2[grid2];
		  const IntegerArray & bc2 = mg2.boundaryCondition();
		  const IntegerArray & share2 = mg2.sharedBoundaryFlag();

		  for( int dir2=0; dir2<mg2.numberOfDimensions(); dir2++ )
		  {
		    for( int side2=0; side2<=1; side2++ )
		    {
		      // (1) A matching interface has the same share flag
		      // (2) must also have the same original boundary conditions (*wdh* 080219)
		      //     This is needed if there are multiple grids on the same boundary (i.e. have the same share flag)
		      // (3) it must also match geometrically at at least one boundary point, here are some examples:
		      /*
			+---------------------------------------------------+ boundary 1
			+---------------------------------------------------+ boundary 2 matches exactly, this is the easiest case

			+---------------------------------------------------+ boundary 1
			+-----------------------------+               boundary 2 matches at both points (must check the reversed condition too)

			+----------------------+                              boundary 1
			+-----------------------------+               boundary 2 matches at one point (must check the reversed condition too)

			+----------------------+                              boundary 1
			+--------------+               boundary 2 does not really match boundary 1, hence the increment dr

			In 3D there are 8 possible checks to make (4 for each face) but we always stop when one match is found (e.g. in the second example
			we would stop after the third check, the first point in boundary 2).
			kkc 080425
		      */
		      if( share1(side1,dir1)==share2(side2,dir2)  && // this must be a matching interface 
			  // bc2(side2,dir2)==Parameters::interfaceBoundaryCondition &&
			  interfaceType2(side2,dir2,grid2)!=Parameters::noInterface &&
			  originalBoundaryCondition1(side1,dir1,grid1)==originalBoundaryCondition2(side2,dir2,grid2) )
		      {
			// geometric tests
			bool interfacesMatch = false;
			Mapping & map1 = mg1.mapping().getMapping();
			Mapping & map2 = mg2.mapping().getMapping();
		      
			if( parameters.dbase.get<bool>("matchInterfacesGeometrically") )
			{
			  // Match interfaces geometrically as well as by the share and bc values:
			  interfacesMatch = checkIfInterfacesMatch(map1,dir1,side1,map2,dir2,side2);
			  if ( !interfacesMatch )
			  { // invert points from grid2 onto grid1
			    interfacesMatch = checkIfInterfacesMatch(map2,dir2,side2,map1,dir1,side1);
			  }
			}
			else
			{
			  interfacesMatch=true;
			}
		      
		      
			// for info keep track if we have an interface that may match but doesn't
			// satisfy the geometric matching conditions: 
			interfaceFoundButDoesNotMatchGeometrically = !interfacesMatch;
		      
			if ( interfacesMatch ) 
			{
			  if ( foundAnInterfaceForThisBoundary ) 
			  {
			    printF("--Cgmp:initializeInterface: ERROR found more than one match for (d1,grid1,side1,dir1,share)=(%i,%i,%i,%i,%i)\n"
				   "                            additional match is (d2,grid2,side2,dir2,share)=(%i,%i,%i,%i,%i)\n",
				   d1,grid1,side1,dir1,share1(side1,dir1),
				   d2,grid2,side2,dir2,share2(side2,dir2));
			    printF(" grid1=%s, grid2=%s\n",(const char*)mg1.getName(),(const char*)mg2.getName());

			    Overture::abort("error");
			  }
			  
			  // **** interface found *************
			  if( interfaceList.size()==0 )
			    fPrintF(interfaceFile,"\n**********************************************************************\n");
			  fPrintF(interfaceFile,
				  "--Cgmp:initializeInterfaces: interface found: %s\n"
				  "       (d1,grid1,side1,dir1,share)=(%i,%i,%i,%i,%i)  grid=%s domain=%s\n"
				  "       (d2,grid2,side2,dir2,share)=(%i,%i,%i,%i,%i)  grid=%s domain=%s\n",
				  (const char*)parameters.icNames[interfaceType1(side1,dir1,grid1)],
				  d1,grid1,side1,dir1,share1(side1,dir1), (const char*)mg1.getName(),
				  (const char*)domainSolver[d1]->getName(),
				  d2,grid2,side2,dir2,share2(side2,dir2),(const char*)mg2.getName(), 
				  (const char*)domainSolver[d2]->getName());

			  interfaceList.push_back(InterfaceDescriptor());  // add a new interface to the list
			  InterfaceDescriptor & interface =  interfaceList.back(); 

			  interface.domain1=d1;
			  interface.domain2=d2;
			  interface.gridListSide1.push_back(GridFaceDescriptor(d1,grid1,side1,dir1));
			  
			  interface.gridListSide2.push_back(GridFaceDescriptor(d2,grid2,side2,dir2));
			  foundAnInterfaceForThisBoundary = true;

			} // if found an interface and they match geometrically

		      } // if found a possible interface by matching share and boundary conditions
		    } // for side2
		  } // for dir2
		} // for grid2
	      } // for domain 2 (d2)

	      if( !foundAnInterfaceForThisBoundary )
	      {
		// If no matching interface was found then we still need to check that the 
		// interface was not matched previously ...

		for( int inter=0; inter < interfaceList.size(); inter++ )
		{
		  InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

		  // there may be multiple grid faces that lie on the interface:     
		  for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
		  {
		    // GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face];
		    GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face];
		    if( gridDescriptor2.grid==grid1 && gridDescriptor2.side==side1 && gridDescriptor2.axis==dir1 )
		    {
		      foundAnInterfaceForThisBoundary=true;
		    }
		  }
		}
		if( !foundAnInterfaceForThisBoundary )
		{
		  printF("--Cgmp:initializeInterfaces: ERROR: no matching interface found for :\n"
			 "       (d1,grid1,side1,dir1,share,bc)=(%i,%i,%i,%i,%i,%i)  (grid1=%s)\n",
			 d1,grid1,side1,dir1,share1(side1,dir1),bc1(side1,dir1),(const char*)mg1.getName());
		  if( interfaceFoundButDoesNotMatchGeometrically )
		    printF("NOTE: A potential matching interface was found but the boundaries did not match "
			   "geometrically.\n");

		  printF("\n See `interfaceFile.log' for more info.");
		
		  Overture::abort("error");
		}
	      }
	      
	    } // end "old way"
	    
	  } // if an interface bc
	} // for side1
      } // for dir1
    } // for grid1
  } // for domain 1 (d1)

  fflush(interfaceFile);
  fPrintF(interfaceFile,"\n -------------- Summary of Interfaces Defined ------------------------\n");
  for( int inter=0; inter < interfaceList.size(); inter++ ) 
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 
    fPrintF(interfaceFile," -- Interface %i is an interface between domain1=%i (%s,%s) and domain2=%i (%s,%s)--- \n",
            inter,interfaceDescriptor.domain1,
            (const char*)domainSolver[interfaceDescriptor.domain1]->getName(),
            (const char*)domainSolver[interfaceDescriptor.domain1]->getClassName(),
            interfaceDescriptor.domain2,
            (const char*)domainSolver[interfaceDescriptor.domain2]->getClassName(),
            (const char*)domainSolver[interfaceDescriptor.domain2]->getName());
    for( int interfaceSide=0; interfaceSide<=1; interfaceSide++ )
    {
      const int domain = interfaceSide==0 ? interfaceDescriptor.domain1 :  interfaceDescriptor.domain2;
      GridList & gridList = interfaceSide==0 ? interfaceDescriptor.gridListSide1 : interfaceDescriptor.gridListSide2;
      fPrintF(interfaceFile,"   Interface side %i : domain=%i (%s) has %i faces: \n",interfaceSide,domain,
	      (const char*)domainSolver[domain]->getName(),gridList.size());

      for( int face=0; face<gridList.size(); face++ )
      {
	GridFaceDescriptor & gridDescriptor = gridList[face];
	const int d=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
        assert( d==domain );
        CompositeGrid & cg = domainSolver[d]->gf[gfIndex[d]].cg;
	const IntegerArray & originalBoundaryCondition = 
	  domainSolver[d]->parameters.dbase.get<IntegerArray>("originalBoundaryCondition");
	const int baseGrid = cg.baseGridNumber(grid);	  
        fPrintF(interfaceFile,"    face=%i : domain=%i (side,axis,grid)=(%i,%i,%i) share=%i bc=%i orig-bc=%i\n",
		face,d,side,dir,grid,cg[grid].sharedBoundaryFlag(side,dir),cg[grid].boundaryCondition(side,dir),
                originalBoundaryCondition(side,dir,baseGrid) );


        // --- Save info in the domain solvers so that they know about the InterfaceDescriptor ----
    
        // **************** FINISH ME  *********************

 	BoundaryData::BoundaryDataArray & pBoundaryData = domainSolver[domain]->parameters.getBoundaryData(grid); // this will create the BDA if it is not there
	std::vector<BoundaryData> & boundaryDataArray =domainSolver[domain]->parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
	BoundaryData & bd = boundaryDataArray[grid];

        typedef InterfaceDescriptor* (InterfaceDescriptorType)[2][3];
	if( !bd.dbase.has_key("interfaceDescriptorArray") )
	{
	  bd.dbase.put<InterfaceDescriptorType>("interfaceDescriptorArray");
          InterfaceDescriptorType & interfaceDescriptorArray = bd.dbase.get<InterfaceDescriptorType>("interfaceDescriptorArray");
	  for( int s=0; s<=1; s++ )for( int a=0; a<3; a++ ){ interfaceDescriptorArray[s][a]=NULL;}  // 
	}
	InterfaceDescriptorType & interfaceDescriptorArray = bd.dbase.get<InterfaceDescriptorType>("interfaceDescriptorArray");
	interfaceDescriptorArray[side][dir]=&interfaceDescriptor;

      }
      
    }
  }
  fPrintF(interfaceFile," ---------------------------------------------------------------------\n");

  if( interfaceList.size()>0 )
  {
    fPrintF(interfaceFile,"**********************************************************************\n\n");
    gridHasMaterialInterfaces=true;
  }
  else
  {
    gridHasMaterialInterfaces=false;
  }
  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;
  
  return 0;
}



//===============================================================================================
// 
/// \brief: Assign the boundary conditions at an internal interface by solving the coupled interface equations. 
/// \param gfIndex (input) : Domain d should use the grid function : domainSolver[d]->gf[gfIndex[d]]
/// \details: This routine solves the coupled heatFlux boundary conditions directly.
//
//===============================================================================================
int Cgmp::
assignInterfaceBoundaryConditions(std::vector<int> & gfIndex, 
				  const real dt )
{
  real cpu0=getCPU();

  if( !gridHasMaterialInterfaces ) return 0;

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  
  if( interfaceList.size()==0 )
  {
    // -- Initialize the list of interfaces --
    initializeInterfaces(gfIndex);

    if( interfaceList.size()>0 ) gridHasMaterialInterfaces=true;

    if( interfaceList.size()>0 )
    {
      printF("**** assignInterfaceBoundaryConditions:initializeInterfaces: number of interfaces =%i\n",
	     interfaceList.size());
    }
    else
    {
      printF("**** assignInterfaceBoundaryConditions:initializeInterfaces: There are NO interfaces\n");
    }
    
  }
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  const int numberOfDomains=domainSolver.size();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  

  // ************************************************
  // ******* Apply the interface conditions. ********
  // ************************************************

  // We need to interpolate the grid function if an interface is covered by more than 1 face
  std::vector<bool> interpolateThisDomain(numberOfDomains,false);

  // loop over interfaces
  for( int inter=0; inter < interfaceList.size(); inter++ )
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

      GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];

      CompositeGrid & cg1 = gf1.cg;
      assert( grid1>=0 && grid1<=cg1.numberOfComponentGrids());
      MappedGrid & mg1 = cg1[grid1];
      const IntegerArray & bc1 = mg1.boundaryCondition();
      const IntegerArray & share1 = mg1.sharedBoundaryFlag();

      CompositeGrid & cg2 = gf2.cg;
      assert( grid2>=0 && grid2<=cg2.numberOfComponentGrids());
      MappedGrid & mg2 = cg2[grid2];
      const IntegerArray & bc2 = mg2.boundaryCondition();
      const IntegerArray & share2 = mg2.sharedBoundaryFlag();

      IntegerArray gidLocal1(2,3), dimLocal1(2,3), bcLocal1(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf1.u[grid1],gidLocal1,dimLocal1,bcLocal1 );

      IntegerArray gidLocal2(2,3), dimLocal2(2,3), bcLocal2(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf2.u[grid2],gidLocal2,dimLocal2,bcLocal2 );

      const int numberOfDimensions = cg1.numberOfDimensions();

      const int orderOfAccuracyInSpace=2;  // **** do this for now ***
      const int tc1 = domainSolver[d1]->parameters.dbase.get<int >("tc");    // *** fix this *** assume T for now 
      const int tc2 = domainSolver[d2]->parameters.dbase.get<int >("tc");


      const int extra=0; // orderOfAccuracyInSpace/2;
      getBoundaryIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,extra);
      getBoundaryIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,extra);
  
      // check that the number of points in the tangential directions match -- eventually we will fix this
      for( int dir=1; dir<mg1.numberOfDimensions(); dir++ )
      {
	int dir1p = (dir1+dir) % mg1.numberOfDimensions();
	int dir2p = (dir2+dir) % mg2.numberOfDimensions();
	if( Iv[dir1p].getLength()!=Jv[dir2p].getLength() )
	{
	  printF("applyInterfaceBC:ERROR: The number of grid points on the two interfaces do not match\n"
	         " (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i) Iv=[%i,%i][%i,%i][%i,%i]\n"
		 " (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i) Jv=[%i,%i][%i,%i][%i,%i]\n",
		 d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),
                   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                 d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2),
                   J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound());
	  cout<<"grid names are "<<mg1.getName()<<" , "<<mg2.getName()<<endl;
	  Overture::abort("error");
	}

        // We need to interpolate the grid function if an interface has interpolation points on it:

        // **** NOTE: I think this is needed because we do not check the mask array when assigning
        //            interface points and thus over-write interpolation points **** FIX ME ***

        if( bc1(0,dir1p)==0 || bc1(1,dir1p)==0 )
	{
         interpolateThisDomain[d1]=true;
	}
        if( bc2(0,dir2p)==0 || bc2(1,dir2p)==0 )
	{
         interpolateThisDomain[d2]=true;
	}

      }

      realArray & u1 = gf1.u[grid1];
      realArray & u2 = gf2.u[grid2];

      #ifdef USE_PPP
        // First try for parallel -- assume aligned grids

        // copy values from u2 into an array u2b that is distributed in the same way as u1
        realArray u2b; u2b.partition(u1.getPartition());
        // the next line causes a bug in u2b.updateGhostBoundaries(); below -- doesn't like non-zero base
        // u2b.redim(u1.dimension(0),u1.dimension(1),u1.dimension(2),Range(tc2,tc2)); // note last arg
        u2b.redim(u1.dimension(0),u1.dimension(1),u1.dimension(2),u2.dimension(3)); // note last arg
        realSerialArray u2bLocal; getLocalArrayWithGhostBoundaries(u2b,u2bLocal);
	
        // **** we should extra values for extrpolation -- see mx/src/assignInterfaceBoundaryConditions.bC

        // stencil half width: (we copy points (-halfWidth,+halfWidth)
        const int halfWidth= orderOfAccuracyInSpace/2;
        Index Iv1[4], Iv2[4];
	getIndex(mg1.dimension(),Iv1[0],Iv1[1],Iv1[2]);
	Iv1[dir1]=Range(mg1.gridIndexRange(side1,dir1)-halfWidth,mg1.gridIndexRange(side1,dir1)+halfWidth);
	getIndex(mg2.dimension(),Iv2[0],Iv2[1],Iv2[2]);
	Iv2[dir2]=Range(mg2.gridIndexRange(side2,dir2)-halfWidth,mg2.gridIndexRange(side2,dir2)+halfWidth);

	Iv1[3]=tc2; Iv2[3]=tc2;

        const int nd=4;

        // NOTE: We also assume below that the grid metric match !  ** fix me ***

        // *wdh* 081122 -- I think we are assuming that the values come from the same side and dir !!
        //  -- otherwise we need to add a stride to the copy below I think --
	if( dir1!=dir2 )
	{
	  printF("Cgmx:assignInterfaceBC: Error: in parallel we assume that the interface satisfies\n"
		 " dir1==dir2 : you may have to remake the grid to satisfy this\n");
	  Overture::abort("error");
	}
        if( debug() & 8 )
	{
	  printF("interfaceBC: Iv1=[%i,%i][%i,%i][%i,%i][%i,%i]  Iv2=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
		 Iv1[0].getBase(),Iv1[0].getBound(),Iv1[1].getBase(),Iv1[1].getBound(),
		 Iv1[2].getBase(),Iv1[2].getBound(),Iv1[3].getBase(),Iv1[3].getBound(),
		 Iv2[0].getBase(),Iv2[0].getBound(),Iv2[1].getBase(),Iv2[1].getBound(),
		 Iv2[2].getBase(),Iv2[2].getBound(),Iv2[3].getBase(),Iv2[3].getBound());
	}
	
        // *** DO NOT reverse the points ***
        // We just pretend that u2 pts are on the same side but shifted in index space *** 

        u2bLocal=0.; 
        ParallelUtility::copy(u2b,Iv1,u2,Iv2,nd);  // u2b(Iv1)=u2(Iv2)
        // u2b(Iv1[0],Iv1[1],Iv1[2],Iv1[3])=u2(Iv2[0],Iv2[1],Iv2[2],Iv2[3]);
	
        // copy values from u1 into an array u1b that is distributed in the same way as u2
        realArray u1b; u1b.partition(u2.getPartition());
        // u1b.redim(u2.dimension(0),u2.dimension(1),u2.dimension(2),Range(tc1,tc1));
        u1b.redim(u2.dimension(0),u2.dimension(1),u2.dimension(2),u1.dimension(3));
        realSerialArray u1bLocal; getLocalArrayWithGhostBoundaries(u1b,u1bLocal);
	
	Iv1[3]=tc1; Iv2[3]=tc1;
        u1bLocal=0.; 
        ParallelUtility::copy(u1b,Iv2,u1,Iv1,nd);  // u1b(Iv2)=u1(Iv1)
        // u1b(Iv2[0],Iv2[1],Iv2[2],Iv2[3])=u1(Iv1[0],Iv1[1],Iv1[2],Iv1[3]);

        u1b.updateGhostBoundaries(); // *********** these are currently needed ********************
        u2b.updateGhostBoundaries();

        realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1,u1Local);
        realSerialArray u2Local; getLocalArrayWithGhostBoundaries(u2,u2Local);
	
        int includeGhost=0;  // do NOT include parallel ghost since we can't apply the stencil there
        bool ok1 = ParallelUtility::getLocalArrayBounds(u1,u1Local,I1,I2,I3,includeGhost);
        bool ok2 = ParallelUtility::getLocalArrayBounds(u2,u2Local,J1,J2,J3,includeGhost);

	// ::display(u1Local,"interfaceBC: u1Local","%5.2f ");
	// ::display(u1bLocal,"interfaceBC:u1bLocal after copy","%5.2f ");

	// ::display(u2Local,"interfaceBC: u2Local","%5.2f ");
	// ::display(u2bLocal,"interfaceBC:u2bLocal after copy","%5.2f ");
	

      #else

        realSerialArray & u1Local = u1;
        realSerialArray & u2Local = u2;
	
      #endif

      // IN parallel :
//       #ifdef USE_PPP
//         const int includeGhost=1;
//         bool ok = ParallelUtility::getLocalArrayBounds(u1,u1Local,I1a,I2a,I3a,includeGhost);

//         realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1,u1Local);
//         realSerialArray u2Local; getLocalArrayWithGhostBoundaries(u2,u2Local);

//         J1b = I1a + J1.getBase() - I1.getBase();
	
//         realSerialArray u2b(J1b,J2b,J3b,R);  // copy of u2 overlapping u1Local
// 	// copy u2b from u2

//         realSerialArray u1b(I1b,I2b,I3b,R);  // copy of u1 overlapping u2Local
// 	// copy u1b from u1

//         // Now solve interface eqns for 
//         //     I  : (u1Local,u2b)   --> gives u1Local (throw away u2b)
//         //     II : (u1b,u2Local)   --> gives u2Local (throw away u1b)

//       #endif

      real t1=gf1.t;
      real t2=gf2.t;
      if( fabs(t1-t2) > REAL_EPSILON*100.*max(t1,t2) )
      {
	printf("applyInterfaceBC:WARNING: t1=%9.3e and t2=%9.3e are not the same, t1-t2=%8.2e\n",t1,t2,t1-t2);
      }
      
      real t= t1;

      // Apply a BC at the inteface


      bool useOpt=true;
      if( useOpt )
      {

	// optimised version 

	int n1a=I1.getBase(),n1b=I1.getBound(),
	  n2a=I2.getBase(),n2b=I2.getBound(),
	  n3a=I3.getBase(),n3b=I3.getBound();

	int m1a=J1.getBase(),m1b=J1.getBound(),
	  m2a=J2.getBase(),m2b=J2.getBound(),
	  m3a=J3.getBase(),m3b=J3.getBound();


	bool isRectangular1= mg1.isRectangular();
	real dx1[3]={0.,0.,0.}; //
	if( isRectangular1 )
	  mg1.getDeltaX(dx1);

	bool isRectangular2= mg2.isRectangular();
	real dx2[3]={0.,0.,0.}; //
	if( isRectangular2 )
	  mg2.getDeltaX(dx2);

	if( true )
	{ // for testing -- make rectangular grids look curvilinear ***************************************
	  isRectangular1=false;
	  isRectangular2=false;
	}
	if( !isRectangular1 )
	{
	  mg1.update(MappedGrid::THEinverseVertexDerivative);
	  mg2.update(MappedGrid::THEinverseVertexDerivative);
	}
		  
	assert(isRectangular1==isRectangular2);

	int useForcing = parameters.dbase.get<bool >("twilightZoneFlow");
        assert( parameters.dbase.get<bool >("twilightZoneFlow")==domainSolver[d1]->parameters.dbase.get<bool >("twilightZoneFlow"));
        assert( parameters.dbase.get<bool >("twilightZoneFlow")==domainSolver[d2]->parameters.dbase.get<bool >("twilightZoneFlow"));
	if( useForcing )
	{
	  mg1.update(MappedGrid::THEcenter);
	  mg2.update(MappedGrid::THEcenter);
	}
	

	int gridType = isRectangular1 ? 0 : 1;  // ******************************* fix this --> gridType[1,2]
	int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
	int useWhereMask=true;

        int materialInterfaceOption=0;  // unused 
        int interfaceInitialized=0;  // unused 
	int numberOfIterationsForInterfaceBC=0;
	real omegaForInterfaceIteration=.9;


        real ktc[2]  ={-1.,-1.};   // coefficient of thermal conductivity
	real kappa[2]={-1.,-1.};   //  coefficient of thermal diffusion 
	for( int dd=0; dd<=1; dd++ )
	{
	  const int d = dd==0 ? d1 : d2;
          ktc[dd] = domainSolver[d]->parameters.dbase.get<real>("thermalConductivity");

	  if( domainSolver[d]->parameters.dbase.has_key("kappa") )
	  {
	    kappa[dd]=domainSolver[d]->parameters.dbase.get<std::vector<real> >("kappa")[0];
	  } 
	  else if( domainSolver[d]->parameters.dbase.has_key("kThermal") )
	  {
	    kappa[dd]=domainSolver[d]->parameters.dbase.get<real>("kThermal");
	  }
	}
	if( kappa[0] <=0. || kappa[1]<=0. )
	{
	  printF("applyInterfaceBC:ERROR: a negative thermal diffusivity was found, kappa[0]=%e kappa[1]=%e\n",
                 kappa[0],kappa[1]);
	  Overture::abort("error");
	}
	if( ktc[0] <=0. || ktc[1]<=0. )
	{
	  printF("applyInterfaceBC:ERROR: a negative thermal conductivity was found, ktc[0]=%e ktc[1]=%e\n",
                 ktc[0],ktc[1]);
	  Overture::abort("error");
	}

        real ktc1=ktc[0], ktc2=ktc[1];

	if( t1<2.*dt || debug() & 2 )
	  printf("applyInterfaceBC: (d1,grid1,side1,dir1,kappa1,k1)=(%i,%i,%i,%i,%8.2e,%8.2e), "
		 "(d2,grid2,side2,dir2,kappa2,k2)=(%i,%i,%i,%i,%8.2e,%8.2e) \n",
		 d1,grid1,side1,dir1,kappa[0],ktc[0], d2,grid2,side2,dir2,kappa[1],ktc[1]);
	
        // We take the normal from one side the of the interface. normalSign1 and normalSign2
        // are used to flip the sign from one side to the other
        int normalSign1=1-2*side1;  
	int normalSign2=1-2*side2;

	int ierr=0;
	int ipar[]={ //
	  side1, dir1, grid1,
	  n1a,n1b,n2a,n2b,n3a,n3b,
	  side2, dir2, grid2,
	  m1a,m1b,m2a,m2b,m3a,m3b,
 	  gridType,            
 	  orderOfAccuracyInSpace,    
 	  orderOfExtrapolation,
 	  useForcing,          
 	  tc1,                  
 	  tc2,                  
 	  np,
 	  myid,
          normalSign1,
	  normalSign2,
 	  0,  // for use 
 	  0,  // for use 
 	  useWhereMask,       
 	  parameters.dbase.get<int >("debug"),
 	  numberOfIterationsForInterfaceBC,
 	  materialInterfaceOption,
	  interfaceInitialized
	};
		  
	real rpar[]={ //
	  dx1[0],
	  dx1[1],
	  dx1[2],
	  mg1.gridSpacing(0),
	  mg1.gridSpacing(1),
	  mg1.gridSpacing(2),
	  dx2[0],
	  dx2[1],
	  dx2[2],
	  mg2.gridSpacing(0),
	  mg2.gridSpacing(1),
	  mg2.gridSpacing(2),
	  t,    
	  (real &)domainSolver[d1]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	  (real &)domainSolver[d2]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	  dt,    
	  ktc1,
	  ktc2,
	  kappa[0],     // rpar[18] 
	  kappa[1],
	  0., // for later use 
	  0., // for later use 
	  omegaForInterfaceIteration
	};
		  
	real *u1p=u1Local.getDataPointer();
	real *prsxy1=isRectangular1 ? u1p : mg1.inverseVertexDerivative().getLocalArray().getDataPointer();
	real *pxy1= !useForcing ? u1p : mg1.center().getLocalArray().getDataPointer(); 
	int *mask1p=mg1.mask().getLocalArray().getDataPointer();

	real *u2p=u2Local.getDataPointer();
	real *prsxy2=isRectangular2 ? u2p : mg2.inverseVertexDerivative().getLocalArray().getDataPointer();
	real *pxy2= !useForcing ? u2p : mg2.center().getLocalArray().getDataPointer(); 
	int *mask2p=mg2.mask().getLocalArray().getDataPointer();


// 	real *rwk=interface.rwk;
// 	int *iwk=interface.iwk;
// 	assert( rwk!=NULL && iwk!=NULL );
        real rwk[1];
	int iwk[1];
      
//	const int ndf = max(interface.ndf1,interface.ndf2); 
	const int ndf = 0;

	// assign pointers into the work spaces
	int pa2=0,pa4=0,pa8=0, pipvt2=0,pipvt4=0,pipvt8=0;
	if( orderOfAccuracyInSpace==2 )
	{
	  pa2=0; 
	  pa4=pa2 + 2*2*2*ndf;
	  pa8=0;  // not used
	
	  pipvt2=0;
	  pipvt4=pipvt2 + 2*ndf; 
	  pipvt8=0;
	}
	else if( orderOfAccuracyInSpace==4 )
	{
	  pa2=0; // not used
	  pa4=0;
	  pa8=pa4+4*4*2*ndf;
	
	  pipvt2=0;
	  pipvt4=0;
	  pipvt8=pipvt4+4*ndf;
	
	}
       #ifndef USE_PPP
	interfaceCgCm( mg1.numberOfDimensions(), 
		       u1Local.getBase(0),u1Local.getBound(0),
		       u1Local.getBase(1),u1Local.getBound(1),
		       u1Local.getBase(2),u1Local.getBound(2),
		       mg1.gridIndexRange(0,0), *u1p, *mask1p,*prsxy1, *pxy1, bc1(0,0), 
		       u2Local.getBase(0),u2Local.getBound(0),
		       u2Local.getBase(1),u2Local.getBound(1),
		       u2Local.getBase(2),u2Local.getBound(2),
		       mg2.gridIndexRange(0,0), *u2p, *mask2p,*prsxy2, *pxy2, bc2(0,0), 
		       ipar[0], rpar[0], 
		       rwk[pa2],rwk[pa4],rwk[pa8], iwk[pipvt2],iwk[pipvt4],iwk[pipvt8],
		       ierr );
	if( false && debug() & 4 )
	{
	  ::display(u1Local(I1,I2,I3,tc1),"u1Local(I1,I2,I3,tc1) after interfaceCgCm","%5.2f ");
	  ::display(u2Local(J1,J2,J3,tc2),"u2Local(J1,J2,J3,tc2) after interfaceCgCm","%5.2f ");
	  getGhostIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,1);
	  getGhostIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,1);
	  ::display(u1Local(I1,I2,I3,tc1),"u1Local(Ig1,Ig2,Ig3,tc1) after interfaceCgCm","%5.2f ");
	  ::display(u2Local(J1,J2,J3,tc2),"u2Local(Jg1,Jg2,Jg3,tc2) after interfaceCgCm","%5.2f ");
	}
	
      #else
        // --- parallel version ---
        // --- For now we assume that the grids match exactly ---

        // In parallel we solve the interface equations on each processor -- thus we
        // solve the equations twice, one for each side of the interface

        assert( dir1==dir2 );

        // ******** get local gid bc ...
        if( ok1 )
	{
	  int ipar[]={ //
	    side1, dir1, grid1,
	    n1a,n1b,n2a,n2b,n3a,n3b,
	    side2, dir2, grid2,        // use grid1 info here
	    n1a,n1b,n2a,n2b,n3a,n3b,   // use grid1 info here
	    gridType,            
	    orderOfAccuracyInSpace,    
	    orderOfExtrapolation,
	    useForcing,          
	    tc1,                  
	    tc2,        
	    np,
	    myid,
	    normalSign1,
	    normalSign2,
	    0,  // for use 
	    0,  // for use 
	    useWhereMask,       
	    parameters.dbase.get<int >("debug"),
	    numberOfIterationsForInterfaceBC,
	    materialInterfaceOption,
	    interfaceInitialized
	  };
	  real rpar[]={ //
	    dx1[0],
	    dx1[1],
	    dx1[2],
	    mg1.gridSpacing(0),
	    mg1.gridSpacing(1),
	    mg1.gridSpacing(2),
	    dx2[0],
	    dx2[1],
	    dx2[2],
	    mg1.gridSpacing(0),
	    mg1.gridSpacing(1),
	    mg1.gridSpacing(2),
	    t,    
	    (real &)domainSolver[d1]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	    (real &)domainSolver[d2]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	    dt,    
	    ktc1,
	    ktc2,
  	    kappa[0],     // rpar[18] 
	    kappa[1],
	    0., // for later use 
	    0., // for later use 
	    omegaForInterfaceIteration
	  };
	  interfaceCgCm( mg1.numberOfDimensions(), 
			 u1Local.getBase(0),u1Local.getBound(0),
			 u1Local.getBase(1),u1Local.getBound(1),
			 u1Local.getBase(2),u1Local.getBound(2),
			 gidLocal1(0,0), *u1p, *mask1p,*prsxy1, *pxy1, bcLocal1(0,0), 
			 u2bLocal.getBase(0),u2bLocal.getBound(0),
			 u2bLocal.getBase(1),u2bLocal.getBound(1),
			 u2bLocal.getBase(2),u2bLocal.getBound(2),
			 // note: use grid1 mesh data here    -- ASSUMES GRIDS MATCH *FIX ME*
			 gidLocal1(0,0), *u2bLocal.getDataPointer(), *mask1p,*prsxy1, *pxy1, bcLocal1(0,0), 
			 ipar[0], rpar[0], 
			 rwk[pa2],rwk[pa4],rwk[pa8], iwk[pipvt2],iwk[pipvt4],iwk[pipvt8],
			 ierr );

    	  // ::display(u1Local,"interfaceBC: u1Local after interfaceCgCm","%5.2f ");
	  // ::display(u2bLocal,"interfaceBC:u2bLocal after interfaceCgCm","%5.2f ");
	}
	if( ok2 )
	{
	  int ipar[]={ //
	    side1, dir1, grid1,          // use grid2 info here
	    m1a,m1b,m2a,m2b,m3a,m3b,     // use grid2 info here
	    side2, dir2, grid2,
	    m1a,m1b,m2a,m2b,m3a,m3b,
	    gridType,            
	    orderOfAccuracyInSpace,    
	    orderOfExtrapolation,
	    useForcing,          
	    tc1,               
	    tc2,                  
            np,
	    myid,
	    normalSign1,
	    normalSign2,
	    0,  // for use 
	    0,  // for use 
	    useWhereMask,       
	    parameters.dbase.get<int >("debug"),
	    numberOfIterationsForInterfaceBC,
	    materialInterfaceOption,
	    interfaceInitialized
	  };
	  real rpar[]={ //
	    dx1[0],
	    dx1[1],
	    dx1[2],
	    mg2.gridSpacing(0),
	    mg2.gridSpacing(1),
	    mg2.gridSpacing(2),
	    dx2[0],
	    dx2[1],
	    dx2[2],
	    mg2.gridSpacing(0),
	    mg2.gridSpacing(1),
	    mg2.gridSpacing(2),
	    t,    
	    (real &)domainSolver[d1]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	    (real &)domainSolver[d2]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	    dt,    
	    ktc1,
	    ktc2,
  	    kappa[0],     // rpar[18] 
	    kappa[1],
	    0., // for later use 
	    0., // for later use 
	    omegaForInterfaceIteration
	  };
	  interfaceCgCm( mg1.numberOfDimensions(), 
			 u1bLocal.getBase(0),u1bLocal.getBound(0),
			 u1bLocal.getBase(1),u1bLocal.getBound(1),
			 u1bLocal.getBase(2),u1bLocal.getBound(2),
			 // note: use grid 2 mesh data here  -- ASSUMES GRIDS MATCH *FIX ME*
			 gidLocal2(0,0), *u1bLocal.getDataPointer(), *mask2p,*prsxy2, *pxy2, bcLocal2(0,0), 
			 u2Local.getBase(0),u2Local.getBound(0),
			 u2Local.getBase(1),u2Local.getBound(1),
			 u2Local.getBase(2),u2Local.getBound(2),
			 gidLocal2(0,0), *u2p, *mask2p,*prsxy2, *pxy2, bcLocal2(0,0), 
			 ipar[0], rpar[0], 
			 rwk[pa2],rwk[pa4],rwk[pa8], iwk[pipvt2],iwk[pipvt4],iwk[pipvt8],
			 ierr );

	  // ::display(u1bLocal,"interfaceBC:u1bLocal after interfaceCgCm","%5.2f ");
    	  // ::display(u2Local,"interfaceBC: u2Local after interfaceCgCm","%5.2f ");

	}
	
//         u1.updateGhostBoundaries(); // ********not needed ***********************
//         u2.updateGhostBoundaries();
	
      #endif

      }
      else
      {
	// Here is a simple interface condition: 
	//     
	//     u1=u2 on the interface 
	//     Extrapolate(u1) 

	const int tc1 = domainSolver[d1]->parameters.dbase.get<int >("tc");    // *** fix this *** assume T for now 
        Range C=Range(tc1,tc1);

	if( false )
	{
// 	  u1(I1,I2,I3,C)=u2(J1,J2,J3,C); 
// 	  u[grid1].applyBoundaryCondition(C,BCTypes::extrapolate,Parameters::interfaceBoundaryCondition,0.,t);
// 	  if( true )
// 	  {
// 	    u[grid1].periodicUpdate();  // *** fix this ***
// 	  }

	}
	else
	{ // 
	  // This is a fake BC: 

	  // do this for now:
	  if( J1.length()!=I1.length() || J2.length()!=I2.length() ) continue;
		    
	  // *** C=Range(parameters.dbase.get<int >("tc"),parameters.dbase.get<int >("tc"));

	  u2(J1,J2,J3,C)=u1(I1,I2,I3,C);   // boundary value of u2 = boundary value of u1

	  getGhostIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,0);
	  getGhostIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,-1);
	  u1(I1,I2,I3,C)=u2(J1,J2,J3,C);   // ghost value of u1 = first line in of u2

	  if( true )
	  {
	    gf1.u[grid1].periodicUpdate();  // *** fix this ***
	    gf2.u[grid2].periodicUpdate();  // *** fix this ***
	  }
                    
      
	}
      }
      

/* ----------------

                  // Now look for any refinement grids that also share this interface
                  if( cg.numberOfRefinementLevels()>1 )
		  {
                    for( int level=1; level<cg.numberOfRefinementLevels(); level++ )
		    {
		      GridCollection & rl = cg.refinementLevel[level];
		      for( int g1=0; g1<rl.numberOfGrids(); g1++ )
		      {
			const int grid1r=rl.gridNumber(g1); 
			const int baseGrid1=rl.baseGridNumber(g1); 
			if( baseGrid1==grid1 && 
                            cg[grid1r].boundaryCondition(side1,dir1)==Parameters::interfaceBoundaryCondition )
			{
			  // *** this refinement grid shares the interface *****


                          // I: interpolate from the coarser grid values of the same side of the interface
			  for( int l=0; l<level; l++ )
			  {
			  }
			  

                          // II: look for refinement grids at the same level on the opposite side of the interface
			  for( int g2=0; g2<rl.numberOfGrids(); g2++ )
			  {
			    const int grid2r=rl.gridNumber(g2); 
			    const int baseGrid2=rl.baseGridNumber(g2); 
			    if( baseGrid2==grid2 &&
				cg[grid2r].sharedBoundaryFlag(side2,dir2)==share1(side1,dir1) )
			    {
                              // grid2r is on the opposite side of the interface 

			    }
			  }
			}
		      }
		    }
		  }
		  
  ------------ */



// 		  if( tz!=NULL && pDebugFile!=NULL )
// 		  {
		    
// 		    OGFunction & e = *tz;

// 		    getGhostIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3);

// 		    realArray err(I1,I2,I3);
		  
// 		    err=u1(I1,I2,I3,ex)-e(mg1,I1,I2,I3,ex,t+dt);
// 		    ::display(err,sPrintF("err in u1 (ex,ghost) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");

// 		    getGhostIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,-1);
// 		    err=u1(I1,I2,I3,ex)-e(mg1,I1,I2,I3,ex,t+dt);
// 		    ::display(err,sPrintF("err in u1 (ex,line 1) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");


// 		    getGhostIndex(mg2.gridIndexRange(),side2,dir2,I1,I2,I3);
                    
// 		    err=u2(I1,I2,I3,ex)-e(mg2,I1,I2,I3,ex,t+dt);
// 		    ::display(err,sPrintF("err in u2 (ex,ghost) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");

// 		    getGhostIndex(mg2.gridIndexRange(),side2,dir2,I1,I2,I3,-1);
                    
// 		    err=u2(I1,I2,I3,ex)-e(mg2,I1,I2,I3,ex,t+dt);
// 		    ::display(err,sPrintF("err in u2 (ex,line 1) after interface, t=%e",t+dt),pDebugFile,"%8.1e ");

//		  }
      
    } // end for face
  } // end for inter
  

  // We need to interpolate the grid function if an interface has interpolation points on it:
  // **** NOTE: I think this is needed because we do not check the mask array when assigning
  //            interface points and thus over-write interpolation points **** FIX ME ***
  ForDomain(d)
  {
    if( interpolateThisDomain[d] )
    {
      if( debug() & 8 )
	printF("\n ++++++++++  Cgmp:assignInterfaceBoundaryConditions: Interpolate domain %i after assigning the "
	       "interface values +++++++++++++++\n",d);
      GridFunction & gf = domainSolver[d]->gf[gfIndex[d]];
      gf.u.interpolate();
    }
  }
  


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;

  return 0;
}


// ===================================================================================================================
/// \brief Initialize the interface boundary conditions when they are solved by iteration.
/// \details When we iterate to solve the interface conditions we need to specify what sub-set
///   of the interface conditions we solve on each domain.
/// \param t (input) : current time
/// \param dt (input) : current time step
/// \param gfIndex (input) : Domain d should use the grid function : domainSolver[d]->gf[gfIndex[d]]
// ==================================================================================================================
int Cgmp::
initializeInterfaceBoundaryConditions( real t, real dt, std::vector<int> & gfIndex )
{
  real cpu0=getCPU();

  // ************* Much of this code is duplicated from assignInterfaceBoundaryConditions: FIX THIS *********

  if( !gridHasMaterialInterfaces ) return 0;

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  

  if( interfaceList.size()==0 )
  {
    // -- Initialize the list of interfaces --
    initializeInterfaces(gfIndex);

    if( interfaceList.size()>0 ) 
      gridHasMaterialInterfaces=true;
    else
    {
      gridHasMaterialInterfaces=false;
      return 0;
    }

    if( interfaceList.size()>0 )
    {
      printF("**** initializeInterfaceBoundaryConditions:initializeInterfaces: number of interfaces =%i\n",
	     interfaceList.size());
    }
  }
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  const int numberOfDomains=domainSolver.size();
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");

  const bool solveCoupledInterfaceEquations = parameters.dbase.get<bool>("solveCoupledInterfaceEquations");

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  

  // loop over interfaces
  for( int inter=0; inter < interfaceList.size(); inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // there may be multiple grid faces that lie on the interface:     
    int maxNumberOfFaces = max(interfaceDescriptor.gridListSide1.size(), interfaceDescriptor.gridListSide2.size());
    for( int face=0; face<maxNumberOfFaces; face++ )
    {
      const int face1 = min(face,interfaceDescriptor.gridListSide1.size()-1);
      const int face2 = min(face,interfaceDescriptor.gridListSide2.size()-1);
      
      GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face1];
      GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face2];
      
      const int d1=gridDescriptor1.domain, 
                grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
      const int d2=gridDescriptor2.domain, 
                grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;

      assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );

      GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];

      CompositeGrid & cg1 = gf1.cg;
      assert( grid1>=0 && grid1<=cg1.numberOfComponentGrids());
      MappedGrid & mg1 = cg1[grid1];
      const IntegerArray & bc1 = mg1.boundaryCondition();
      const IntegerArray & share1 = mg1.sharedBoundaryFlag();
      const IntegerArray & interfaceType1 = domainSolver[d1]->parameters.dbase.get<IntegerArray >("interfaceType");

      CompositeGrid & cg2 = gf2.cg;
      assert( grid2>=0 && grid2<=cg2.numberOfComponentGrids());
      MappedGrid & mg2 = cg2[grid2];
      const IntegerArray & bc2 = mg2.boundaryCondition();
      const IntegerArray & share2 = mg2.sharedBoundaryFlag();
      const IntegerArray & interfaceType2 = domainSolver[d2]->parameters.dbase.get<IntegerArray >("interfaceType");

      IntegerArray gidLocal1(2,3), dimLocal1(2,3), bcLocal1(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf1.u[grid1],gidLocal1,dimLocal1,bcLocal1 );

      IntegerArray gidLocal2(2,3), dimLocal2(2,3), bcLocal2(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf2.u[grid2],gidLocal2,dimLocal2,bcLocal2 );

      const int numberOfDimensions = cg1.numberOfDimensions();

      assert( interfaceType1(side1,dir1,grid1) == interfaceType2(side2,dir2,grid2) );

      if( interfaceType1(side1,dir1,grid1)==Parameters::heatFluxInterface )
      {
        // ********************************************
	// ********** Heat Flux Interface *************
        // ********************************************

	const int tc1 = domainSolver[d1]->parameters.dbase.get<int >("tc");    // *** fix this *** assume T for now 
	const int tc2 = domainSolver[d2]->parameters.dbase.get<int >("tc");

	real ktc[2]={1.,1.};

	ktc[0] = domainSolver[d1]->parameters.dbase.get<real>("thermalConductivity");
	ktc[1] = domainSolver[d2]->parameters.dbase.get<real>("thermalConductivity");
	if( ktc[0] <=0. || ktc[1]<=0. )
	{
	  printF("Cgmp::initializeInterfaceBoundaryConditions:ERROR: a negative thermal conductivity"
		 " was found, ktc[0]=%e ktc[1]=%e\n",ktc[0],ktc[1]);
	  Overture::abort("error");
	}

	real ktc1=ktc[0], ktc2=ktc[1];
      
	// ************ fix this ***********

	real ktd[2]={-1.,-1.}; //  coefficient of thermal diffusion 
	for( int dd=0; dd<=1; dd++ )
	{
	  const int d = dd==0 ? d1 : d2;
	  if( domainSolver[d]->parameters.dbase.has_key("kappa") )
	  {
	    ktd[dd]=domainSolver[d]->parameters.dbase.get<std::vector<real> >("kappa")[0];
	  } 
	  else if( domainSolver[d]->parameters.dbase.has_key("kThermal") )
	  {
	    ktd[dd]=domainSolver[d]->parameters.dbase.get<real>("kThermal");
	  }
	}

	fPrintF(interfaceFile,
		"Cgmp::initInterfaceBCs: interface %i : heat-flux interface (t=%e) (solveCoupled=%i):\n"
		"  (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i) kappa1=%9.3e (diffusion), k1=%9.3e (conduct.)\n"
		"  (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i) kappa2=%9.3e (diffusion), k2=%9.3e (conduct.)\n",
		inter,t,solveCoupledInterfaceEquations,
		d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),ktd[0],ktc1,
		d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2),ktd[1],ktc2);

        const real kRatio = (ktc[0]/ktc[1])*sqrt(ktd[1]/ktd[0]);

	if( solveCoupledInterfaceEquations )
	{
	  // Apply Neumann BC's on both sides if we solve the coupled interface equations 
	  gridDescriptor1.interfaceBC=Parameters::neumannInterface;  
	  gridDescriptor1.a[0]=0.; gridDescriptor1.a[1]=ktc[0];
	  gridDescriptor2.interfaceBC=Parameters::neumannInterface;; 
	  gridDescriptor2.a[0]=0.; gridDescriptor2.a[1]=ktc[1];
	}
	// else if( ktc1 > ktc2 ) // ** this condition must match that in assignInterfaceRightHandSide **
	else if( parameters.dbase.get<bool>("useMixedInterfaceConditions") )
	{
	  // use mixed interface conditions
	  //   a11=1, a12 = k2*beta2 = k2*sqrt( 1/(theta2*kappa2*dt) + k2^2 )
	  //   a12=1, a22 = k1*beta1 = k1*sqrt( 1/(theta1*kappa1*dt) + k1^2 )

          const real theta1 = domainSolver[d1]->parameters.dbase.get<real>("implicitFactor");
          const real theta2 = domainSolver[d2]->parameters.dbase.get<real>("implicitFactor");
          const real kFact1=.1;
          const real kFact2=.1; 

          const real kbeta1 = ktc[0]*sqrt( 1./(ktd[0]*theta1*dt) + kFact1/(ktd[0]*dt) );
          const real kbeta2 = ktc[1]*sqrt( 1./(ktd[1]*theta2*dt) + kFact2/(ktd[1]*dt) );

	  real a11=1., a12=kbeta2;
	  real a21=1., a22=kbeta1;

	  if( true )
	  {
            // try this instead -- this should work better in the limiting cases when 
            //         kbeta1 << kbeta2   or   kbeta1 >> kbeta2
            // a12=kbeta2/kbeta1;
	    // a22=kbeta1/kbeta2;
            a12=kbeta2*kbeta2/kbeta1;
	    a22=kbeta1*kbeta1/kbeta2;

            if( true )
	    { // this next seems to work better -- make one side nearly a dirichlet condition: 
	      if( kbeta1>=kbeta2 )
	      {
		a22=kbeta1*kbeta1;
	      }
	      else
	      {
		a12=kbeta2*kbeta2;
	      }
	      
	    }
	    
	  }

	  gridDescriptor1.interfaceBC=Parameters::neumannInterface;   
	  gridDescriptor1.a[0]=a12; gridDescriptor1.a[1]=a11*ktc[0];

	  gridDescriptor2.interfaceBC=Parameters::neumannInterface;  
	  gridDescriptor2.a[0]=a22; gridDescriptor2.a[1]=a21*ktc[1];

	 fPrintF(interfaceFile,"Cgmp:initInterfaceBCs: use mixed-interface %i: a11=%8.2e, a12=%8.2e; a21=%8.2e, a22=%8.2e"
                 " (theta1=%8.2e, theta2=%8.2e, dt=%8.2e)\n",
		 inter,a11,a12,a21,a22, theta1,theta2,dt);
	}
	else if( kRatio > 1. ) // *wdh* 080720 
	{

	  // use Dirichlet-Neumann interface conditions
          if( t < dt )
  	    fPrintF(interfaceFile,"Cgmp:initInterfaceBCs: use Dirichlet(domain %i)-Neumann(domain %i) interface\n",d1,d2);

	  // Domain 1 : Neumann interface BC, Domain 2: Dirichlet interface BC
	  gridDescriptor1.interfaceBC=Parameters::neumannInterface;  
	  gridDescriptor1.a[0]=0.; gridDescriptor1.a[1]=ktc1;
	  gridDescriptor2.interfaceBC=Parameters::dirichletInterface; 
	  gridDescriptor2.a[0]=1.; gridDescriptor2.a[1]=0.;
	}
	else
	{
	  // Domain 2 : Neumann interface BC, Domain 1: Dirichlet interface BC
          if( t < dt )
   	    fPrintF(interfaceFile,"Cgmp:initInterfaceBCs: use Dirichlet(domain %i)-Neumann(domain %i) interface\n",d2,d1);

	  gridDescriptor1.interfaceBC=Parameters::dirichletInterface;  
	  gridDescriptor1.a[0]=1.; gridDescriptor1.a[1]=0.;
	  gridDescriptor2.interfaceBC=Parameters::neumannInterface;    
	  gridDescriptor2.a[0]=0.; gridDescriptor2.a[1]=ktc2;
	}
	domainSolver[d1]->setInterfaceBoundaryCondition( gridDescriptor1 );
	domainSolver[d2]->setInterfaceBoundaryCondition( gridDescriptor2 );
      }
      else if( interfaceType1(side1,dir1,grid1)==Parameters::tractionInterface )
      {
        // *******************************************
	// ********** Traction Interface *************
        // *******************************************


        // -- for now nothing needs to be done in this case.

        // Currently there is a problem having a traction interface that is derivativePeriodic
        // using the new interface transfer -- the inverseMap will return a value periodic wrapped (e.g. SquareMapping) 
        // but this won't work when interpolating the displacement which is NOT periodic.
	if( parameters.dbase.get<bool>("useNewInterfaceTransfer") )
	{
	  const IntegerArray & isPeriodic1 = mg1.isPeriodic();
	  const IntegerArray & isPeriodic2 = mg2.isPeriodic();
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
	    if( isPeriodic1(axis)==Mapping::derivativePeriodic ||
		isPeriodic2(axis)==Mapping::derivativePeriodic )
	    {
	      printF("Cgmp::initializeInterfaceBoundaryConditions:ERROR: there is a grid on the interface that is"
		     " periodic with derivativePeriodic.\n"
		     " This will not currently work with the new interface transfer functions.\n");
	      OV_ABORT("error");
	    }
	  }
	  
	}

      }
      else
      {
	printF("Cgmp::initializeInterfaceBoundaryConditions:ERROR:unexpected interfaceType=%i\n",
	       interfaceType1(side1,dir1,grid1));
	Overture::abort("error");
      }

    } // end for face
  } // end for inter

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;
  return 0;
}


// ===================================================================================================================
/// \brief Assign the RHS for the interface equations on the target domain d
/// \details Assign values in the BoundaryDataArray : domainSolver[d]->getBoundaryData(grid); on domain
///    d by evaluating the RHS for the interface equations. For example, the RHS may be the solution value "u"
///    or the normal component of the "stress", k u.n from the opposite side of the interface.
///   
/// \param d (input) : target domain, assign RHS for this domain.
/// \param t (input) : current time
/// \param dt (input) : current time step
/// \param correct (input) : correction step number.
/// \param gfIndex (input) : Domain d should use the grid function : domainSolver[d]->gf[gfIndex[d]]
// 
// ==================================================================================================================
int Cgmp::
assignInterfaceRightHandSide( int d, real t, real dt, int correct, std::vector<int> & gfIndex )
{
  if( !parameters.dbase.get<bool>("useNewInterfaceTransfer") )
  {
    // call the old version
    return assignInterfaceRightHandSideOld( d,t,dt,correct,gfIndex );
  }
  

  real cpu0=getCPU();

  // ************* Much of this code is duplicated from assignInterfaceBoundaryConditions: FIX THIS *********

  if( !gridHasMaterialInterfaces ) return 0;
  
  if( debug() & 2 )
    printF("***  USE NEW assignInterfaceRightHandSide (NEW MULTI-FACE INTERFACE TRANSFER) ***\n");

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  
//   if( parameters.isAdaptiveGridProblem() )
//   {
//     printF("*** assignInterfaceRightHandSide This is a problem with AMR! for now clear the old interface info ...\n");
    
//     // -- do this for now: 
//     interfaceList.clear();
//   }


  if( interfaceList.size()==0 )
  {
    // -- Initialize the list of interfaces --
    initializeInterfaces(gfIndex);

    if( interfaceList.size()>0 ) gridHasMaterialInterfaces=true;

    if( interfaceList.size()>0 )
    {
      printF("**** assignInterfaceBoundaryConditions:initializeInterfaces: number of interfaces =%i\n",
	     interfaceList.size());
    }
  }
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  const int numberOfDomains=domainSolver.size();
  int numberOfInterfaceHistoryValuesToSave = parameters.dbase.get<int>("numberOfInterfaceHistoryValuesToSave");
  int numberOfInterfaceIterateValuesToSave = parameters.dbase.get<int>("numberOfInterfaceIterateValuesToSave");
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
  
  // real & omega = parameters.dbase.get<real>("interfaceOmega");

  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
  // Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
  
  // Index Iav[4], &Ia1=Iav[0], &Ia2=Iav[1], &Ia3=Iav[2], &Ia4=Iav[3];
  // Index Jav[4], &Ja1=Jav[0], &Ja2=Jav[1], &Ja3=Jav[2], &Ja4=Jav[3];
  

  // ************************************************
  // ******* Apply the interface conditions. ********
  // ************************************************

  // loop over interfaces
  for( int inter=0; inter < interfaceList.size(); inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // Does this interface lie on domain "d" : 
    if( interfaceDescriptor.domain1!=d && interfaceDescriptor.domain2!=d )
      continue;

    // Target domain is d1
    // Source domain is d2
    
    const int targetInterfaceSide = interfaceDescriptor.domain1==d ? 0 : 1;
    const int sourceInterfaceSide = interfaceDescriptor.domain1==d ? 1 : 0;
    
    int domainTarget = targetInterfaceSide==0 ? interfaceDescriptor.domain1 : interfaceDescriptor.domain2;
    int domainSource = targetInterfaceSide==0 ? interfaceDescriptor.domain2 : interfaceDescriptor.domain1;

    assert( domainTarget==d );
    

    GridFunction & gfTarget = domainSolver[domainTarget]->gf[gfIndex[domainTarget]]; // target 
    GridFunction & gfSource = domainSolver[domainSource]->gf[gfIndex[domainSource]]; // source 

    GridList & gridListTarget = targetInterfaceSide==0 ? interfaceDescriptor.gridListSide1 : 
                                                         interfaceDescriptor.gridListSide2;
    
    GridList & gridListSource = sourceInterfaceSide==0 ? interfaceDescriptor.gridListSide1 : 
                                                         interfaceDescriptor.gridListSide2;
    
    const int numberOfDimensions = gfSource.cg.numberOfDimensions();

    //  sourceArray[grid] : holds data on interface from grid "grid" of domainSource 
    //  targetArray[grid] : holds data on interface from grid "grid" of domainTarget
    const int sourceArraySize=gfSource.cg.numberOfComponentGrids();
    const int targetArraySize=gfTarget.cg.numberOfComponentGrids();
    RealArray **sourceArray = new RealArray * [sourceArraySize]; 
    RealArray **targetArray = new RealArray * [targetArraySize];
    for( int grid=0; grid<targetArraySize; grid++ )
      targetArray[grid]=NULL;
    for( int grid=0; grid<sourceArraySize; grid++ )
      sourceArray[grid]=NULL;


    // --- Find out the type of interface (heat-flux, traction, ...) ---
    assert( gridListSource.size()>0 );
    int face=0;
    GridFaceDescriptor & gridDescriptor = gridListSource[face];
    const int ds=gridDescriptor.domain, grids=gridDescriptor.grid, sides=gridDescriptor.side, dirs=gridDescriptor.axis;

    const IntegerArray & interfaceTypeSource = 
              domainSolver[domainSource]->parameters.dbase.get<IntegerArray >("interfaceType");

    const int interfaceType = interfaceTypeSource(sides,dirs,grids);

    Range Cs, Ct; // hold component ranges for source and target

    real ktc[2]={ 1., 1.};   // thermal conductivity
    real ktd[2]={-1.,-1.};   //  coefficient of thermal diffusion 
    bool matchFlux=false;
    if( interfaceType==Parameters::heatFluxInterface )
    {
      // ********** Heat Flux Interface *************
      const int tcSource = domainSolver[domainSource]->parameters.dbase.get<int >("tc"); 
      Cs = Range(tcSource,tcSource);
      const int tcTarget = domainSolver[domainTarget]->parameters.dbase.get<int >("tc"); 
      Ct = Range(tcTarget,tcTarget);


      // -- Look up heat flux parameters for the two domains --
      ktc[0] = domainSolver[domainSource]->parameters.dbase.get<real>("thermalConductivity");
      ktc[1] = domainSolver[domainTarget]->parameters.dbase.get<real>("thermalConductivity");
      if( ktc[0] <=0. || ktc[1]<=0. )
      {
	printF("assignInterfaceRightHandSide:ERROR: a negative thermal conductivity was found, "
	       "ktc[0]=%e ktc[1]=%e\n",ktc[0],ktc[1]);
	OV_ABORT("error");
      }
      for( int dd=0; dd<=1; dd++ ) // loop over source and target domains
      {
	const int domain = dd==0 ? domainSource : domainTarget;
	if( domainSolver[domain]->parameters.dbase.has_key("kappa") )
	{
	  ktd[dd]=domainSolver[domain]->parameters.dbase.get<std::vector<real> >("kappa")[0];
	} 
	else if( domainSolver[domain]->parameters.dbase.has_key("kThermal") )
	{
	  ktd[dd]=domainSolver[domain]->parameters.dbase.get<real>("kThermal");
	}
      }
      int face=0;
      const real a0 = gridListTarget[face].a[0];  // we assume that these are the same on all faces 
      const real a1 = gridListTarget[face].a[1];
      
      matchFlux= a1!=0.;  // check me 

    }
    else if( interfaceType==Parameters::tractionInterface )
    {
      // ********** Traction Interface *************

      // Old: 
      // Save space for velocity and traction
      // const int ucSource = domainSolver[domainSource]->parameters.dbase.get<int >("uc");
      // Cs = Range(ucSource,ucSource+2*numberOfDimensions-1);
      // const int ucTarget = domainSolver[domainTarget]->parameters.dbase.get<int >("uc");
      // Ct = Range(ucTarget,ucTarget+2*numberOfDimensions-1);

    }
    else
    {
      printF("Cgmp::assignInterfaceRightHandSide:ERROR:unexpected interfaceType=%i\n",
	     interfaceType);
      OV_ABORT("error");
    }

    // ---------------------------------------------------
    // ------- allocate space for the target data ------- 
    // ---------------------------------------------------
    int interfaceDataOptions=-1; // Will hold info on what data the target domain requires
    for( int face=0; face<gridListTarget.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor = gridListTarget[face];
      const int domain=gridDescriptor.domain, 
	grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
      assert( domain==domainTarget );

      // check that the interfaceType matches what we think it should be: 
      const IntegerArray & interfaceTypeTarget = 
                      domainSolver[domain]->parameters.dbase.get<IntegerArray >("interfaceType");
      assert( interfaceTypeTarget(side,dir,grid)==interfaceType );
      
      GridFaceDescriptor info(domain,grid,side,dir);

      // find out the interface data required by the target domain:
      int faceOption=0;
      int numDataItems = domainSolver[domainTarget]->getInterfaceDataOptions( info,faceOption );
      Ct=numDataItems;
      Cs=numDataItems;  // we could double check this 
      
      if( face==0 )
	interfaceDataOptions=faceOption;
      else
      {
        assert( interfaceDataOptions==faceOption ); // All faces should require the same info 
      }
      
      MappedGrid & mg = gfTarget.cg[grid];
      const intArray & mask = mg.mask();

      OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
      
      const int extra=0; // orderOfAccuracyInSpace/2;
      getBoundaryIndex(mg.gridIndexRange(),side,dir,I1,I2,I3,extra);
      int includeGhost=0;  
      bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);

      if( ok )
      {
	assert( targetArray[grid]==NULL );
	targetArray[grid]= new RealArray;

	RealArray & u = *targetArray[grid];
	u.redim(I1,I2,I3,Ct);
        u=0.;
      }
      
    }


    // ---------------------------------------------------
    // -------------- Get the source data ----------------
    // ---------------------------------------------------
    for( int face=0; face<gridListSource.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor = gridListSource[face];
      const int domain=gridDescriptor.domain, 
	grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
      assert( domain==domainSource );
       
      // allocate space for the source data:
      MappedGrid & mg = gfSource.cg[grid];
      const intArray & mask = mg.mask();
      OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
      const int extra=0; // orderOfAccuracyInSpace/2;
      getBoundaryIndex(mg.gridIndexRange(),side,dir,I1,I2,I3,extra);
      int includeGhost=0;  
      bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
      if( ok )
      {
	assert( sourceArray[grid]==NULL );
	sourceArray[grid]= new RealArray;
	RealArray & u = *sourceArray[grid];
	u.redim(I1,I2,I3,Cs);
        u=0.;
      }

      GridFaceDescriptor info(domain,grid,side,dir);
      info.u = sourceArray[grid];   // set the pointer as to where the source data should be saved below

      if( interfaceType==Parameters::heatFluxInterface )
      {
	if( parameters.dbase.get<bool>("useMixedInterfaceConditions") )
	{
	  // *note* flip the sign of the normal to match normal from domain da: 
	  // *note* flip k1 <--> k2 
	  // *wdh* 100523 info.a[0]=gridDescriptor.a[0]; info.a[1]=-gridDescriptor.a[1]*ktc[db]/ktc[da];
	  info.a[0]=gridDescriptor.a[0]; info.a[1]=-gridDescriptor.a[1]*ktc[0]/ktc[1]; // *check me*
	}
	else if( matchFlux )
	{
	  // *note* flip the sign of the normal to match normal from domain 1
	  const real kRatio =  (ktc[0]/ktc[1])*sqrt(ktd[1]/ktd[0]);
	  if( kRatio > 1. )
	  {
	    info.a[0]=0.; info.a[1]=-ktc[1];
	  }
	  else
	  {
	    info.a[0]=0.; info.a[1]=-ktc[0];
	  }
	}
	else
	{
	  info.a[0]=1.; info.a[1]=0.;
	}
      }
      else
      {
	// traction or other interface ... nothing to do 
      }
      
      // *** Should we get all faces at once ?? 
      // -- get the data from the source domain db : (save in info.u == ?? ) --
      if( ok )
      {
	
	domainSolver[domainSource]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info,
							    gridDescriptor,gfIndex[domainSource],t );
	
	if( false )
	{
	  RealArray & ua = *info.u;
	  ::display(ua,"****NEW: source: ub ");
	}
      }
      
    }
    
    
    // -----------------------------------------------------------------
    // ----- Transfer the source arrays to the target arrays -----------
    // -----------------------------------------------------------------

    // if( gridListSource.size()==1 && gridListTarget.size()==1 )
    //   check for matching interfaces ...

    // **NEW WAY** (not done yet)
    if( interfaceDescriptor.interfaceTransfer==NULL )
    {
      interfaceDescriptor.interfaceTransfer = new InterfaceTransfer;
    }
	
    InterfaceTransfer & interfaceTransfer = *interfaceDescriptor.interfaceTransfer;

    interfaceTransfer.transferData( domainSource, domainTarget, 
				    sourceArray, Cs,  // source
				    targetArray, Ct,  // target
				    interfaceDescriptor,
				    domainSolver,
				    gfIndex,
				    parameters );

    // -------------------------------------------------------
    // ------ Adjust the target data before assigning --------
    // -------------------------------------------------------
    //       1. Optionally extrapolate the first guess from previous times
    //       2. Under-relax the answer
    for( int face=0; face<gridListTarget.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor = gridListTarget[face];
      const int domain=gridDescriptor.domain, 
	grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
      assert( domain==domainTarget );
      
      bool ok = targetArray[grid]!=NULL;
      if( !ok ) continue;
      RealArray & ua = *targetArray[grid];
      I1=ua.dimension(0); I2=ua.dimension(1); I3=ua.dimension(2);

      if( false )
      {
	RealArray & ua = *targetArray[grid];
	::display(ua,"****NEW: targetArray after transfer");
      }

      // *********************************************************
      // *********** Extrapolate the initial guess ***************
      // *********************************************************

      bool extrapolateFirstGuess=parameters.dbase.get<bool>("extrapolateInitialInterfaceValues");
      if( interfaceType==Parameters::tractionInterface )
      {
        // we currently don't extrapolate for traction interfaces
	extrapolateFirstGuess=false;
      }
      

      // We should only need to extrapolate for one side the interface: 
      //   Extrapolate the side with the lower domain number -- this is assumed to appear first

      // extrapolateFirstGuess = extrapolateFirstGuess && (d1<d2 ? d==d1 : d==d2); // *wdh* 100522 
      extrapolateFirstGuess = extrapolateFirstGuess && domainTarget<domainSource; // is this right ? 

      if( extrapolateFirstGuess && correct==0 )
      {
	// We have saved both u and k*u.n from previous times (for TZ we have saved u-ue, k*u.n - k*ue.n ).
        // We can extrapolate, in time, the RHS for the interface condition

        //  If we are solving the interface condition by iteration: (j= iteration number)
        //    a*k1*u.n[j+1] + b*u[j+1] = a*k2*v.n[j] + b*v[j]  

        // As a first guess we can use
        //    a*k1*u.n[0] + b*u[0] = 2.*f(t-dt) - f(t-2*dt) 
        // where
        //    f(t) = a*k2*v.n(t) + b*v(t)
        // OR
        //    f(t) = a*k1*u.n(t) + b*u(t)   <- this is better to use for parallel
       
	// a history of interface values at past times: 
        // **** For parallel: I think we can use gridDescriptora here and switch sign of a1 below ****
	// ** InterfaceDataHistory & idh = gridDescriptorb.interfaceDataHistory;   // values from the opposite side of the interface
 
        // consistency check: we currently don't extrapolate for traction interfaces
        assert( interfaceType!=Parameters::tractionInterface );

	InterfaceDataHistory & idh = gridDescriptor.interfaceDataHistory;   // values from the same side of the interface

	if( idh.interfaceDataList.size()>=2 )
	{
	  const int prev = ( idh.current -1 + numberOfInterfaceHistoryValuesToSave ) % 
	    numberOfInterfaceHistoryValuesToSave;
	    
	  real tc = idh.interfaceDataList[idh.current].t;
	  real tp = idh.interfaceDataList[prev       ].t;
	    
	    
	  // extrap in time: f(t) = (t-t1)/(t2-t1)*u2 + (t2-t)/(t2-t1)*u1

	  // 2nd order extrap in time:
	  real cex1=2., cex2=-1.;
	  real dtex = tc-tp;
	  if( dtex > dt*.1 )
	  { // adjust for variable time step (if dtex is not too small)
	    cex1=(t-tp)/dtex; cex2=(tc-t)/dtex;
	  }
	  if( debug() & 2 )
	    fPrintF(interfaceFile," +++interfaceRHS: interface %i: domain d=%i, t=%9.3e extrap RHS in time from "
		    " tc=%8.2e tp=%8.2e cex=(%8.2e,%8.2e)\n",
		    inter,d,t,tc,tp,cex1,cex2);
	    
	  if( ok )
	  {
	    // new way 
	    RealArray & uc = idh.interfaceDataList[idh.current].u;
	    RealArray & fc = idh.interfaceDataList[idh.current].f;
	    RealArray & up = idh.interfaceDataList[prev       ].u;
	    RealArray & fp = idh.interfaceDataList[prev       ].f;

            const real a0 = gridDescriptor.a[0], a1=gridDescriptor.a[1]; 
	    ua(I1,I2,I3,Ct) = ( cex1*( a0*uc(I1,I2,I3,Ct) + a1*fc(I1,I2,I3,Ct) ) + 
				cex2*( a0*up(I1,I2,I3,Ct) + a1*fp(I1,I2,I3,Ct) ) );

	  }
	}
      } // end if extrapolateFirstGuess
      

      // *********************************************************
      // ************** Under-relaxed iteration ******************
      // *********************************************************

      // ***** NOTE: should we relax the first time through ??? --> I don't think so.

      const bool & relaxCorrectionSteps = parameters.dbase.get<bool>("relaxCorrectionSteps");

      bool underRelaxGuess=false;
      if( parameters.dbase.get<bool>("useMixedInterfaceConditions") )
      { // with mixed-conditions we under-relax the RHS for one side of the interface
	// underRelaxGuess= d==d1;
	underRelaxGuess= domainTarget<domainSource; // *wdh* 100522 -- is this right?
      }
      else if( matchFlux )
      { // for D-N conditions we under-relax the Neumann condition
	underRelaxGuess=true;
      }
      if( interfaceType==Parameters::tractionInterface )
      {
        // we sometimes relax correction steps for the traction
	underRelaxGuess=relaxCorrectionSteps;
      }      
      if( underRelaxGuess && correct>0 ) // *wdh* 080722 -- only relax for correct > 0 
      {

	// interface values at past iterates for the current time:
	InterfaceDataHistory & idi = gridDescriptor.interfaceDataIterates;

	if( idi.interfaceDataList.size()>=1 )
	{
	  real tp = idi.interfaceDataList[idi.current].t;
          real & omega = interfaceDescriptor.interfaceOmega;
	  if( omega != 1. )
	  {
	    if( debug() & 4 )
	      fPrintF(interfaceFile,"+++interfaceRHS: interface %i: relax flux RHS, t=%9.3e, omega=%5.2f "
		      "(data iterates: current=%i,tp=%9.3e)\n",inter,t,omega,idi.current,tp);


	    if( interfaceType==Parameters::heatFluxInterface )
	    {
	      // We have saved both u and k*u.n from previous times. These can be used to under-relax the 
	      // iteration. If we are solving the interface condition by iteration: (j= iteration number)
	      //    a*k1*u.n[j+1] + b*u[j+1] = a*k2*v.n[j] + b*v[j]  
	      // The the relaxed iteration is 
	      //    a*k1*u.n[j+1] + b*u[j+1] = omega*( a*k2*v.n[j] + b*v[j] ) + (1-omega)*( a*k1*u.n[j] + b*u[j] )
	      // If omega=1 : no-relaxation. If omega=0, u[j+1] = u[j] 

	      // new way 
	      RealArray & up = idi.interfaceDataList[idi.current].u; 
	      RealArray & fp = idi.interfaceDataList[idi.current].f; 

	      real a0 = gridDescriptor.a[0], a1=gridDescriptor.a[1];  
              // fp already includes the factor of k so we need to divide a1 by this amount
              // real ktca = domainSolver[da]->parameters.dbase.get<real>("thermalConductivity");
              real ktcTarget= ktc[1];  // *wdh* 100523 -- check me 

              a1 = a1/ktcTarget;
              bool ok = targetArray[grid]!=NULL;
	      if( ok )
	      {
		ua(I1,I2,I3,Ct) = ( omega*ua(I1,I2,I3,Ct) + (1.-omega)*( a0*up(I1,I2,I3,Ct) + a1*fp(I1,I2,I3,Ct) ) );
	      }
	    }
	    else if( interfaceType==Parameters::tractionInterface )
	    {
              RealArray & up = idi.interfaceDataList[idi.current].f;  // NOTE: We save tractions in f

	      Range Sc=numberOfDimensions;  // traction (stress) components
	      
	      printF("--MP-- relax traction on the interface, correct=%i, omega=%6.3f\n",correct,omega);
	      ::display(ua(I1,I2,I3,Sc),"ua");
	      ::display(up(I1,I2,I3,Sc),"up");
              real maxDiff = max(fabs(ua(I1,I2,I3,Sc)-up(I1,I2,I3,Sc)));
	      printF("       max-diff=%8.2e\n",correct,omega,maxDiff);
	      OV_ABORT("stop here for now");
	      if( ok )
	      {
		ua(I1,I2,I3,Sc) = omega*ua(I1,I2,I3,Sc) + (1.-omega)*up(I1,I2,I3,Sc);
	      }
	    }
            else
	    {
	      printF("interfaceRHS:WARNING: relaxation not applied to an interface of type=%i\n",(int)interfaceType);
	    }
	    
	  } // end if omega!=1
	  
	}
      } // end if underRelaxGuess ...
    } // end for target face 
    

    // ---------------------------------------------------
    // ------ Assign the target data ---------------------
    // ---------------------------------------------------
    for( int face=0; face<gridListTarget.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor = gridListTarget[face];
      const int domain=gridDescriptor.domain, 
	        grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
      assert( domain==domainTarget );
       

      GridFaceDescriptor info(domain,grid,side,dir);
      info.u = targetArray[grid];  // put results here in call below

      bool ok = targetArray[grid]!=NULL;

      // --- Set the data for the target side of the interface (values in info.u ) ---

      const real a0 = gridListTarget[face].a[0];  // we assume that these are the same on all faces 
      const real a1 = gridListTarget[face].a[1];
      info.a[0]=a0; info.a[1]= a1;  // These are used with "set" for TZ

      if( false )
      {
	RealArray & ua = *targetArray[grid];
	::display(ua,"****NEW: targetArray: ua ");
      }


      if( ok )
        domainSolver[domainTarget]->interfaceRightHandSide( setInterfaceRightHandSide,interfaceDataOptions,info,
                                                            gridDescriptor,gfIndex[domainTarget],t );
    }   

    // --- clean up ---
    for( int grid=0; grid<targetArraySize; grid++ )
      delete targetArray[grid];
    for( int grid=0; grid<sourceArraySize; grid++ )
      delete sourceArray[grid];

    delete [] targetArray;
    delete [] sourceArray;
    
    
  } // end for inter


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;
  return 0;
  
}


// ===================================================================================================================
/// \brief Evaluate the residuals in the interface equation and/or save a time history of interface values.
///       
/// \details This routine will query the domains for the current values of the interface variables.
///   These interface variables may be then saved in a time history list (to be used with extrapolating forward in time or
///   for computing time derivatives of the interface values). The interface values can also be used to evaluate the 
///   residual in the interface jump equations to see how well these equations are satisfied.
///
///   For interfaces with non-matching grid points we need to query the interface values from one side of the interface
///   and then transfer (i.e. interpolate) these values to the grid on the other side of the interface (and vice versa)
///   before we can compute the interface residuals.
///   
/// \param t (input) : current time
/// \param dt (input) : current time step
/// \param correct (input) : correction step number.
/// \param gfIndex (input) : Domain d should use the grid function : domainSolver[d]->gf[gfIndex[d]]
/// \param maxResidual (output) : Maximum residual in the interface equations for each interface.
/// \param saveInterfaceValues (input) : One of doNotSaveInterfaceValues, saveInterfaceTimeHistoryValues,
///    or saveInterfaceIterateValues to indicate whether interface values should be saved in a list for later use. 
// 
// ==================================================================================================================

int Cgmp::
getInterfaceResiduals( real t, real dt, std::vector<int> & gfIndex, std::vector<real> & maxResidual,
                       InterfaceValueEnum saveInterfaceValues /* =doNotSaveInterfaceValues */ )
{

  if( !parameters.dbase.get<bool>("useNewInterfaceTransfer") )
  {
    // call the old version
    return getInterfaceResidualsOld( t, dt, gfIndex, maxResidual, saveInterfaceValues );
  }

  if( t<2.*dt )
    printF("\n****** Entering *NEW* getInterfaceResiduals ********\n");
  
  real cpu0=getCPU();

  if( !gridHasMaterialInterfaces ) return 0;

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  assert( interfaceList.size()!=0 );

  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  const int numberOfDomains=domainSolver.size();
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");

  int numberOfInterfaceHistoryValuesToSave = parameters.dbase.get<int>("numberOfInterfaceHistoryValuesToSave");
  int numberOfInterfaceIterateValuesToSave = parameters.dbase.get<int>("numberOfInterfaceIterateValuesToSave");

  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];

  if( maxResidual.size() < interfaceList.size() )
    maxResidual.resize(interfaceList.size(),0.);


  // ------------------------------
  // ---- loop over interfaces ----
  // ------------------------------
  for( int inter=0; inter < interfaceList.size(); inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 
    const int domain1 = interfaceDescriptor.domain1;
    const int domain2 = interfaceDescriptor.domain2;
    
    maxResidual[inter]=0.;

    // We need source and target arrays for both sides
    //   sourceArray(grid,interfaceSide)
    RealArray **pSourceArray[2]={NULL,NULL};
    RealArray **pTargetArray[2]={NULL,NULL};
    #define sourceArray(grid,iside) pSourceArray[iside][grid]
    #define targetArray(grid,iside) pTargetArray[iside][grid]
    
    // ------------------------------------------------------------------
    // ---------- Get data from both sides of the interface -------------
    // ------------------------------------------------------------------
    int interfaceDataOptions=-1;
    int interfaceType=-1;
    Range Cv[2];  // holds component ranges for both sides...
    for( int interfaceSide=0; interfaceSide<=1; interfaceSide++ )
    {
      const int domain = interfaceSide==0 ? interfaceDescriptor.domain1 :  interfaceDescriptor.domain2;
      const int domain2= interfaceSide==0 ? interfaceDescriptor.domain2 :  interfaceDescriptor.domain1;

      GridFunction & gf = domainSolver[domain]->gf[gfIndex[domain]];
      CompositeGrid & cg = gf.cg;
      const int numberOfDimensions = cg.numberOfDimensions();
      const IntegerArray & interfaceTypeArray = domainSolver[domain]->parameters.dbase.get<IntegerArray >("interfaceType");

      // allocate arrays to hold the interface data:
      const int numberOfComponentGrids = cg.numberOfComponentGrids();
      pSourceArray[interfaceSide] = new RealArray * [numberOfComponentGrids];
      pTargetArray[interfaceSide] = new RealArray * [numberOfComponentGrids];
      for( int grid=0; grid<numberOfComponentGrids; grid++ )
      {
        sourceArray(grid,interfaceSide)=NULL;
        targetArray(grid,interfaceSide)=NULL;
      }
      
      Range & C = Cv[interfaceSide];
      GridList & gridList = interfaceSide==0 ? interfaceDescriptor.gridListSide1 : interfaceDescriptor.gridListSide2;
      // loop over multiple faces on this side of the interface:
      for( int face=0; face<gridList.size(); face++ ) 
      {
	GridFaceDescriptor & gridDescriptor = gridList[face];
	const int d=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
	assert( d==domain );
	
	if( face==0 )
	{ // first time thru look up the interfaceType:
	  interfaceType = interfaceTypeArray(side,dir,grid);
	}
	else
	{ // later times just check the consistency of the type: 
	  assert( interfaceType == interfaceTypeArray(side,dir,grid) );
	}
	if( interfaceType==Parameters::heatFluxInterface )
	{
	  // -- Heat Flux Interface --
          //  Get T and T.n 
	  const int tc = domainSolver[domain]->parameters.dbase.get<int >("tc");
	  C=Range(tc,tc+1);  // save both T and T.n 
	  interfaceDataOptions=Parameters::heatFluxInterfaceData;

	}
	else if( interfaceType==Parameters::tractionInterface )
	{
          // ------------------------
          // -- Traction Interface --
          // ------------------------

	  // const int uc = domainSolver[domain]->parameters.dbase.get<int >("uc");
          // C = Range(uc,uc+2*numberOfDimensions-1);

	  // find out what data is needed on the other side of the interface:
          GridList & gridList2 = interfaceSide==0 ? interfaceDescriptor.gridListSide2 : interfaceDescriptor.gridListSide1;	
          const int face2=0; // assume all faces want the same info
	  GridFaceDescriptor & gridDescriptor2 = gridList2[face2];
	  const int d2=gridDescriptor2.domain, grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, 
                    dir2=gridDescriptor2.axis;
	  assert( d2==domain2 );
          GridFaceDescriptor info2(domain2,grid2,side2,dir2);

	  int numDataItems=domainSolver[domain2]->getInterfaceDataOptions( info2,interfaceDataOptions );
          C=numDataItems;
	}
	else
	{
	  OV_ABORT("Error: unknown interfaceType");
	}
	

	MappedGrid & mg = cg[grid];
	const intArray & mask = mg.mask();

	OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
      
	const int extra=0; // orderOfAccuracyInSpace/2;
	getBoundaryIndex(mg.gridIndexRange(),side,dir,I1,I2,I3,extra);
	int includeGhost=0;  
	bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);

	if( ok )
	{

	  // allocate space for the target array
	  assert( targetArray(grid,interfaceSide)==NULL );
	  targetArray(grid,interfaceSide) = new RealArray;

	  RealArray & uTarget = *targetArray(grid,interfaceSide);
	  uTarget.redim(I1,I2,I3,C);

	  // allocate space for the source array 
	  assert( sourceArray(grid,interfaceSide)==NULL );
	  sourceArray(grid,interfaceSide) = new RealArray;

	  RealArray & uSource = *sourceArray(grid,interfaceSide);
	  uSource.redim(I1,I2,I3,C);

	  GridFaceDescriptor info(domain,grid,side,dir);
	  info.u = &uSource;

	  // --- Evaluate u from a0*u+a1*u.n by setting a0=1 and a1=0 ---
	  // *** We should be able to ask for both u and u.n ! ***************************** FIX ME **************
          // RealArray u(I1,I2,I3,
	  info.a[0]=1.; info.a[1]=0.;  // eval T 
	  domainSolver[domain]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info,
							gridDescriptor,gfIndex[domain],t );

	  if( interfaceType==Parameters::heatFluxInterface )
	  {
            // Do this for now:  evaluate T.n in a separate call  *** FIX ME -- this should be done in the above call too
            const int tc = domainSolver[domain]->parameters.dbase.get<int >("tc");
            RealArray u(I1,I2,I3,Range(tc,tc));			
            info.u = &u;
            const real ktc = domainSolver[domain]->parameters.dbase.get<real>("thermalConductivity");
	    if( ktc <=0. )
	    {
	      printF("getInterfaceResiduals:ERROR: a negative thermal conductivity was found, "
		     "domain=%i, ktc=%e \n",domain,ktc);
	      OV_ABORT("error");
	    }
	    info.a[0]=0.; info.a[1]=ktc; // eval k*T.n 
	    domainSolver[domain]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info,
							  gridDescriptor,gfIndex[domain],t );
            uSource(I1,I2,I3,tc+1)=u;  // save k*T.n here 
	  }
	  

	} // end if ok 
	  
        // *** save the interface solution values  ****
	if( saveInterfaceValues==saveInterfaceTimeHistoryValues ||
	    saveInterfaceValues==saveInterfaceIterateValues )
	{ 
	  GridFaceDescriptor & gfd = gridDescriptor;
	  RealArray & ui = sourceArray(grid,interfaceSide)!=NULL ? *sourceArray(grid,interfaceSide) : 
                                                                   Overture::nullRealArray(); 
	  InterfaceDataHistory & idh = (saveInterfaceValues==saveInterfaceTimeHistoryValues ? 
					gfd.interfaceDataHistory : gfd.interfaceDataIterates);
	
	  const int numToSave = (saveInterfaceValues==saveInterfaceTimeHistoryValues ? 
				 numberOfInterfaceHistoryValuesToSave : numberOfInterfaceIterateValuesToSave );

	  if( idh.interfaceDataList.size()<numToSave )
	  { // add a new entry: 
	    idh.interfaceDataList.push_back(InterfaceData());
	    idh.current = idh.interfaceDataList.size()-1;
	  }
	  else
	  { // over-write oldest entry: 
	    idh.current = (idh.current+1) % numToSave;
	  }
	  if( debug() & 4 )
	  {
	    if( saveInterfaceValues==saveInterfaceTimeHistoryValues )
	    {
	      fPrintF(interfaceFile,"interfaceRes: interface %i (face=%i): save interface history data, "
		      "t=%9.2e, current=%i\n",inter,face,t,idh.current);
	    }
	    else
	    {
	      fPrintF(interfaceFile,"interfaceRes: interface %i (face=%i): save interface iterate data, "
		      "t=%9.2e, current=%i\n",inter,face,t,idh.current);
	    }
	  }
	    
	  InterfaceData & id = idh.interfaceDataList[idh.current];
	  id.t=t;
	  RealArray & uSource = *sourceArray(grid,interfaceSide);
          if( interfaceType==Parameters::heatFluxInterface )
	  {
            // do this for now: 
            const int tc = domainSolver[domain]->parameters.dbase.get<int >("tc");
	    if( ok )
	    {
	      id.u=uSource(I1,I2,I3,tc  );  // save id.u 
              id.f.redim(I1,I2,I3,Range(tc,tc));
	      id.f(I1,I2,I3,tc)=uSource(I1,I2,I3,tc+1);  // save id.f 
	    }
	  }
	  else if( interfaceType==Parameters::tractionInterface )
	  {
	    id.f=uSource;  // NOTE: save id.f for traction *********
	  }
	  else
	  {
	    OV_ABORT("Error: unknown interfaceType");
	  }

	} // end save interface values 
	

	
      } // end for face
      

    } // end intefaceSide


    // ----------------------------------------------------------------------
    // ---- Transfer data to the opposite side of the interface -------------
    // ---- and evaluate the jump conditions                    -------------
    // ----------------------------------------------------------------------

    for( int interfaceSide=0; interfaceSide<=1; interfaceSide++ )
    {
      // For now we only evaluate the jump conditions for a heatFlux interface
      if( interfaceType!=Parameters::heatFluxInterface ) 
	continue;


      const int domainSource = interfaceSide==0 ? interfaceDescriptor.domain1 :  interfaceDescriptor.domain2;
      const int domainTarget = interfaceSide==0 ? interfaceDescriptor.domain2 :  interfaceDescriptor.domain1;
      const int sourceSide = interfaceSide;
      const int targetSide = (sourceSide+1) % 2;

//       GridFunction & gf = domainSolver[domain]->gf[gfIndex[domain]];
//       CompositeGrid & cg = gf.cg;


      // --- Transfer the data from the sourceDomain to the targetDomain ---
      if( interfaceDescriptor.interfaceTransfer==NULL )
      {
	interfaceDescriptor.interfaceTransfer = new InterfaceTransfer;
      }
	
      InterfaceTransfer & interfaceTransfer = *interfaceDescriptor.interfaceTransfer;

      interfaceTransfer.transferData( domainSource, domainTarget, 
				      &sourceArray(0,sourceSide), Cv[sourceSide],  // source
				      &targetArray(0,targetSide), Cv[targetSide],  // target
				      interfaceDescriptor,
				      domainSolver,
				      gfIndex,
				      parameters );


      const IntegerArray & interfaceTypeArray = domainSolver[domainTarget]->parameters.dbase.get<IntegerArray >("interfaceType");

      // --- Compute the difference between targetArray values with the source values on the SAME side --
      GridList & gridListTarget = targetSide==0 ? interfaceDescriptor.gridListSide1 : interfaceDescriptor.gridListSide2;
      for( int face=0; face<gridListTarget.size(); face++ )
      {
	GridFaceDescriptor & gridDescriptor = gridListTarget[face];
	const int d=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
	assert( d==domainTarget );

	const int interfaceType = interfaceTypeArray(side,dir,grid);

	MappedGrid & mg = cg[grid];
	const intArray & mask = mg.mask();

	OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
      
	const int extra=0; // orderOfAccuracyInSpace/2;
	getBoundaryIndex(mg.gridIndexRange(),side,dir,I1,I2,I3,extra);
	int includeGhost=0;  
	bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);

	const Range & C = Cv[interfaceSide];
        const int numberOfComponents = C.length();
        const int cBase = C.getBase();
        const int cBound = C.getBound();
	real *pJumpAtInterface = new real [numberOfComponents]; 
        #define jumpAtInterface(c) pJumpAtInterface[c-cBase]
        // We may need to flip the sign of the normal for some components: 
	real *pnSign = new real [numberOfComponents];
        #define nSign(c) pnSign[c-cBase]
	for( int c=cBase; c<=cBound; c++ )
	{
	  jumpAtInterface(c)=0.;
          nSign(c)=1.;
	}
	
	if( ok )
	{
          assert( sourceArray(grid,targetSide)!=NULL );
          assert( targetArray(grid,targetSide)!=NULL );
	  RealArray & us = *sourceArray(grid,targetSide);   // source values from target side of the interface
	  RealArray & ut = *targetArray(grid,targetSide);   // target array of transferred values 
	  if( interfaceType==Parameters::heatFluxInterface )
	  { // flip the sign of k*T.n to account for the normal : 
            const int tc = domainSolver[domainTarget]->parameters.dbase.get<int >("tc");
            nSign(tc+1)=-1.;
	  }
	  
	  
	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( maskLocal(i1,i2,i3) > 0 )
	    {
	      for( int c=cBase; c<=cBound; c++ )
		jumpAtInterface(c)=max(jumpAtInterface(c),fabs(nSign(c)*us(i1,i2,i3,c)-ut(i1,i2,i3,c)));
	    }
	  }
	}
        // get max value of jumpAtInterface(c) over all processors: 
	ParallelUtility::getMaxValues(pJumpAtInterface,pJumpAtInterface,numberOfComponents);

        real interfaceResidual=0.;
	for( int c=cBase; c<=cBound; c++ )
	  interfaceResidual=max(interfaceResidual,jumpAtInterface(c));

	if( interfaceType==Parameters::heatFluxInterface )
	{
	  if( debug() & 2 )
	  {
	    fPrintF(interfaceFile,
		    "interface %i step=%i: residuals: [u]=%8.2e [k*u.n]=%8.2e (omega=%9.3e,tol=%8.2e)\n",
		    inter,parameters.dbase.get<int >("globalStepNumber"),jumpAtInterface(0),jumpAtInterface(1),
		    parameters.dbase.get<real>("interfaceOmega"),interfaceDescriptor.interfaceTolerance);
	  }
	  if( false && debug() & 4 )
	  {
	    RealArray & us = *sourceArray(grid,targetSide);   // source values from target side of the interface
	    RealArray & ut = *targetArray(grid,targetSide);   // target array of transferred values 
	    ::display(us,sPrintF("Here is the sourceArray on targetSide=%i\n",targetSide),interfaceFile);
	    ::display(ut,sPrintF("Here is the targetArray on targetSide=%i\n",targetSide),interfaceFile);
	    
	  }
	  
	  maxResidual[inter]=max(maxResidual[inter],interfaceResidual);

	}

	delete [] pJumpAtInterface;
	delete [] pnSign;

      } // end for face

    } // end for interfaceSide

  } // end for inter 


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;



  return 0;
}


