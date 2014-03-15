#include "DomainSolver.h"
#include "HDF_DataBase.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#include "ReductionMapping.h"
#include "NurbsMapping.h"
#include "ParallelUtility.h"

/// kkc data for blasius solution for flat plate boundary layer (the table is at the end of the file)
extern real blasiusFPData[101][2];
extern real blasiusFData[101][2];


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{timeIndependentBoundaryConditions}} 
int DomainSolver::
timeIndependentBoundaryConditions( GridFunction & cgf )
// ========================================================================
// /Description:
//   Compute the time independent and spatially varying boundary conditions.
//
//\end{CompositeGridSolverInclude.tex}  
// ========================================================================
{
  int returnValue=0;

  returnValue |= parabolicInflow(cgf);

  returnValue |= jetInflow(cgf);

  // assign user boundary conditions.
  for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
  {
    // Variable boundary values:
    setVariableBoundaryValues( cgf.t,cgf,grid );
    userDefinedBoundaryValues( cgf.t,cgf,grid );
  }
  
  return returnValue;
}



//\begin{>>CompositeGridSolverInclude.tex}{\subsection{parabolicInflow}} 
int DomainSolver::
parabolicInflow(GridFunction & cgf )
// ========================================================================
// /Description:
//   Determine the values for a BC with a parabolicInflow profile
//  
//\end{CompositeGridSolverInclude.tex}  
// ========================================================================
{
  CompositeGrid & cg = cgf.cg;

  FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  char buff[100];
  int grid,dir1,  side,axis, side1,grid2,side2,dir2;
  // const IntegerArray & bcType = parameters.dbase.get< >("bcType");

  const RealArray & bcParameters = parameters.dbase.get<RealArray>("bcParameters");
  const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");

  int numberOfInflowFaces=0;
  IntegerArray inflowFace(2,cg.numberOfDimensions(),cg.numberOfComponentGrids()); inflowFace=-1;
  Range all;
  // int maxNumberOfInflowFaces=sum( (bcType(all,all,all)-(int)Parameters::parabolicInflow)==0 );

  int numberOfBlasiusFaces=parameters.howManyBcTypes(all,all,all,Parameters::blasiusProfile);

  int maxNumberOfInflowFaces=
    parameters.howManyBcTypes(all,all,all,Parameters::parabolicInflow)+
    parameters.howManyBcTypes(all,all,all,Parameters::parabolicInflowRamped)+
    parameters.howManyBcTypes(all,all,all,Parameters::parabolicInflowOscillating)+
    parameters.howManyBcTypes(all,all,all,Parameters::parabolicInflowUserDefinedTimeDependence) +
    numberOfBlasiusFaces;
  if( maxNumberOfInflowFaces==0 )
    return 0;


  NurbsMapping blasiusFPInterpolant,blasiusFInterpolant;
  if( numberOfBlasiusFaces>0 )
  {
    #ifdef USE_PPP
      Overture::abort("DomainSolver::parabolicInflow:blasius: finish me for parallel!");
    #endif
    realArray blasiusFArray(101,2),blasiusFPArray(101,2),blasiusParameterization(101);
    for ( int i=0; i<101; i++ )
    {
      blasiusFPArray(i,0) = blasiusFPData[i][0];
      blasiusFPArray(i,1) = blasiusFPData[i][1];
      blasiusFArray(i,0) = blasiusFData[i][0];
      blasiusFArray(i,1) = blasiusFData[i][1];
      blasiusParameterization(i) = blasiusFPData[i][0]/6.;
    }

    blasiusFPInterpolant.interpolate(blasiusFPArray,0,blasiusParameterization);
    blasiusFInterpolant.interpolate(blasiusFArray,0,blasiusParameterization);
    //  PlotIt::plot(*Overture::getGraphicsInterface(),blasiusFPInterpolant);
    //  PlotIt::plot(*Overture::getGraphicsInterface(),blasiusFInterpolant);
  }
  
  int maxNumberOfBoundingCurves=maxNumberOfInflowFaces*4;
  IntegerArray boundingCurve(maxNumberOfInflowFaces,maxNumberOfBoundingCurves); boundingCurve=-1;
  IntegerArray numberOfBoundingCurves( maxNumberOfInflowFaces); numberOfBoundingCurves=0;
  
 
  // first make a list of the grid faces that make up each inflow boundary
  // initially all the grid faces will be considered to belong to distict inflow faces.
  // Later we will merge the faces that belong to the same inflow face.
  numberOfInflowFaces=0;
  bool facesNeedMerging=false;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflow ||
            parameters.bcType(side,axis,grid)==Parameters::parabolicInflowRamped ||
            parameters.bcType(side,axis,grid)==Parameters::parabolicInflowOscillating ||
            parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence ||
            parameters.bcType(side,axis,grid)==Parameters::blasiusProfile )
	{
          // This grid face is an inflow boundary
          const int i=numberOfInflowFaces;
          numberOfInflowFaces++;
          assert( numberOfInflowFaces<=maxNumberOfInflowFaces);
	  
          inflowFace(side,axis,grid)=i;
          printF("parabolicInflow: (grid=%s,side=%i,axis=%i) is an inflow face\n",
               (const char *)c.mapping().getName(Mapping::mappingName),side,axis);
	  
          // check edges for bounding curves
	  for( dir1=0; dir1<cg.numberOfDimensions()-1; dir1++ )
	  {
  	    const int & dir2 = (axis+dir1+1) % c.numberOfDimensions();
	    for( side2=Start; side2<=End; side2++ )
	    {
	      // *wdh* if( c.boundaryCondition(side2,dir2)> 0 )
	      if( c.boundaryCondition(side2,dir2)==Parameters::noSlipWall || c.boundaryCondition(side2,dir2)==Parameters::penaltyBoundaryCondition) // only count no slip walls
	      { // Here is a bounding curve to the inflow face
		boundingCurve(i,numberOfBoundingCurves(i))=side2+2*(dir2+3*(side+2*(axis+3*(grid))));
                numberOfBoundingCurves(i)++;
                assert( numberOfBoundingCurves(i)<=maxNumberOfBoundingCurves );
                printF("parabolicInflow: (grid=%s,side=%i,axis=%i,side2=%i,dir2=%i) is a bounding curve\n",
                   (const char *)c.mapping().getName(Mapping::mappingName),side,axis,side2,dir2);
	      }
	      else if( c.boundaryCondition(side2,dir2) == 0 )
	      {
                facesNeedMerging=true;
	      }
	    }
	  }
	}
      }
    }
  }
  if( numberOfInflowFaces==0 )
    return 0;
  
  if( numberOfInflowFaces > 0 && facesNeedMerging )
  {
    printF("parabolic: faces need merging, numberOfInflowFaces=%i \n",numberOfInflowFaces);
    
    // Try to merge the inflow faces that belong to the same inflow region.
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
    
      for( axis=0; axis<cg.numberOfDimensions(); axis++ )for( side=Start; side<=End; side++ )
      {
	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflow ||
	    parameters.bcType(side,axis,grid)==Parameters::parabolicInflowRamped ||
	    parameters.bcType(side,axis,grid)==Parameters::parabolicInflowOscillating ||
	    parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence ||
	    parameters.bcType(side,axis,grid)==Parameters::blasiusProfile )
	{

	  for( grid2=grid+1; grid2<cg.numberOfComponentGrids(); grid2++ )
	  {
	    for( dir2=0; dir2<cg.numberOfDimensions(); dir2++ )for( side2=Start; side2<=End; side2++ )
	    {
              // --- Look for same inflow type AND same share flag
	      if( parameters.bcType(side,axis,grid)==parameters.bcType(side2,dir2,grid2) &&
                  c.sharedBoundaryFlag(side,axis)==cg[grid2].sharedBoundaryFlag(side2,dir2) && 
                  inflowFace(side2,dir2,grid2)!=inflowFace(side,axis,grid) )
	      {
		// We share a face with grid2 
		const int i=inflowFace(side,axis,grid), i2=inflowFace(side2,dir2,grid2);
		printF("parabolicInflow: merging inflowFace=%i, (grid=%s,%i,%i) share=%i with "
                                                "inflowFace=%i, (grid=%s,%i,%i) share=%i \n",
		       i ,(const char *)c.mapping().getName(Mapping::mappingName),side,axis,
                       c.sharedBoundaryFlag(side,axis),
		       i2,(const char *)cg[grid2].mapping().getName(Mapping::mappingName),side2,dir2,
                       cg[grid2].sharedBoundaryFlag(side2,dir2));

		assert( i!=i2 );
		inflowFace(side2,dir2,grid2)=inflowFace(side,axis,grid);
		Range J(0,numberOfBoundingCurves(i2)-1);
		boundingCurve(i,numberOfBoundingCurves(i)+J)=boundingCurve(i2,J);
		numberOfBoundingCurves(i)+=numberOfBoundingCurves(i2);
		numberOfBoundingCurves(i2)=0;

	      }
	    }
	  } // end for grid2
	}
      }
    }
  } // end if numberOfInflowFaces > 0 && facesNeedMerging
  

  const int includeGhost=1;
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Ig1,Ig2,Ig3;
  ReductionMapping *edgeCurve = NULL;
  for( int i=0; i<numberOfInflowFaces; i++ )
  {
    // For each inflow face:
    if( numberOfBoundingCurves(i) > 0 )
    {
      if( cg.numberOfDimensions()==3 )
      {
	edgeCurve= new ReductionMapping [numberOfBoundingCurves(i)];
	for( int j=0; j<numberOfBoundingCurves(i); j++ )
	{
	  int code = boundingCurve(i,j); // side2+2*(dir2+3*(side+2*(axis+3*(grid))));
	  grid2 =code/36; code-=grid2*36;
	  axis =code/12; code-=axis*12;
	  side=code/6;   code-=side*6;
	  dir2=code/2;  code-=dir2*2;
	  side2=code;
	  assert( grid2>=0 && grid2<= cg.numberOfComponentGrids() && 
                  axis>=0 && axis<=cg.numberOfDimensions() &&
                  side>=0 && side<=1 &&
                  dir2>=0 && dir2<=cg.numberOfDimensions() &&
                  side2>=0 && side2<=1 );
          
          // define the inactive coordinate directions and their values in order to define
          // the bounding curve
          real r[3] = {-1.,-1.,-1.};
	  r[axis]=side;
	  r[dir2]=side2;
	  edgeCurve[j].set(cg[grid2].mapping().getMapping(),r[0],r[1],r[2]);
	}
      }

      // Range C(parameters.dbase.get<int >("uc"),parameters.dbase.get<int >("uc")+cg.numberOfDimensions()-1);
      Range C=parameters.dbase.get<Range >("Rt"); // *wdh* 020728 : apply to all time dependent variables.

      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & c = cg[grid];
	const intArray & mask = c.mask();

        #ifdef USE_PPP
	 intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
        #else
	 const intSerialArray & maskLocal = mask;
        #endif  

        const bool isRectangular = c.isRectangular();
        real dx[3],xab[2][3], xa,ya,za,dx0,dy0,dz0;
        int i0a,i1a,i2a;
        int i1,i2,i3;
        if( isRectangular )
	{
          c.getRectangularGridParameters( dx, xab );

	  i0a=c.gridIndexRange(0,0);
	  i1a=c.gridIndexRange(0,1);
	  i2a=c.gridIndexRange(0,2);
  
	  xa=xab[0][0], dx0=dx[0];
	  ya=xab[0][1], dy0=dx[1];
	  za=xab[0][2], dz0=dx[2];
	
	}
	else
	{// We could probably avoid building the whole center array!
	  c.update(MappedGrid::THEcenter | MappedGrid::THEvertex );  
	}
	
#define VERTEX0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define VERTEX1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define VERTEX2(i0,i1,i2) (za+dz0*(i2-i2a))
	
        if( !isRectangular )
	{ // We could probably avoid building the whole center array!
          c.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
	}
        #ifdef USE_PPP
    	  RealArray center; if( !isRectangular ) getLocalArrayWithGhostBoundaries(c.center(),center);
	#else
          const RealArray & center = isRectangular ? Overture::nullRealArray() : c.center();
        #endif  
	for( axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  for( side=Start; side<=End; side++ )
	  {
	    if( inflowFace(side,axis,grid)==i )
	    {
	      // For each point on this face find the min distance to the boundary

     	      // realArray & bd = mappedGridSolver[grid]->getBoundaryData(side,axis);
              getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);


     	      RealArray & bd = parameters.getBoundaryData(side,axis,grid,c);

              if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflow ||
                  parameters.bcType(side,axis,grid)==Parameters::parabolicInflowRamped ||
		  parameters.bcType(side,axis,grid)==Parameters::parabolicInflowOscillating ||
                  parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence   )
	      {
                // save the initial profile in the ghost points.
                // we need to allocate extra space for this.

                // this should match Parameters::getBoundaryData
                // int extra=1;
		// getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3,extra); 

                // Increase the dimensions from getBoundaryData *wdh* 110828
                I1=bd.dimension(0);
		I2=bd.dimension(1);
		I3=bd.dimension(2);
                Iv[axis]=side==0 ? Range(Iv[axis].getBase()-1,Iv[axis].getBound()  ) :
		                   Range(Iv[axis].getBase()  ,Iv[axis].getBound()+1) ;

                bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
		if( ok )
		{
  		  bd.redim(I1,I2,I3,bd.dimension(3));
                  bd=0.;
		}
                else
                  bd.redim(0);

                if( debug() & 4 )
		{
		  printF("*** allocate more space for parabolicInflowRamp or oscillate\n"
			 "bd =[%i,%i][%i,%i][%i,%i][%i,%i]\n",
			 bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),
			 bd.getBase(2),bd.getBound(2),bd.getBase(3),bd.getBound(3));
		}
		
	        // *assign both below **getGhostIndexIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
                getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
	      }

	      if ( parameters.bcType(side,axis,grid)==Parameters::blasiusProfile )
	      { // kkc
		// save the profile on the first upstream grid line too

                // this should match Parameters::getBoundaryData
                // int extra=1;
		// getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3,extra); 

                // Increase the dimensions from getBoundaryData *wdh* 110828
                I1=bd.dimension(0);
		I2=bd.dimension(1);
		I3=bd.dimension(2);
                Iv[axis]=side==0 ? Range(Iv[axis].getBase()-1,Iv[axis].getBound()  ) :
		                   Range(Iv[axis].getBase()  ,Iv[axis].getBound()+1) ;

                bool ok=ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,I1,I2,I3,includeGhost);
		if( ok )
		{
  		  bd.redim(I1,I2,I3,bd.dimension(3));
                  bd=0.;
		}
                else
                  bd.redim(0);
		getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
	      }


	      bool ok=ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,I1,I2,I3,includeGhost);
	      if( !ok ) // no points on this processor -- we cannot continue here since there is communication below
	      {
		I1=nullRange; I2=nullRange; I3=nullRange;
	      }
	      
	      RealArray dist;
	      if( ok )
	      {
		dist.redim(I1,I2,I3);
		dist=REAL_MAX;
	      }
	      
	      // dist : holds the distance of a point on the boundary to the edge (X or Y) of the boundary
              // 
	      //
	      //      X--+--+--o--+--+--+--+--+--+--+--Y
	      //      <--dist-->   
	      //
	      // The width of the boundary layer is 'width'

	      if( c.numberOfDimensions()==2 )
	      {
                // --- 2D ---  
	        i3=I3.getBase();
		for( int j=0; j<numberOfBoundingCurves(i); j++ )
		{
                  // code: holds the bounding curve info: 
                  //  The bounding curve is on face grid2(side1,dir1), edge(side2,dir2)
		  int code = boundingCurve(i,j); // side2+2*(dir2+3*(side1+2*(dir1*(grid2))));
		  grid2=code/36; code-=grid2*36;
		  dir1 =code/12; code-=dir1*12;
		  side1=code/6;  code-=side1*6;
		  dir2 =code/2;  code-=dir2*2;
		  side2=code;
		  assert( grid2>=0 && grid2< cg.numberOfComponentGrids() && 
			  dir1>=0 && dir1<cg.numberOfDimensions() &&
			  side1>=0 && side1<=1 &&
			  dir2>=0 && dir2<cg.numberOfDimensions() && dir2!=dir1 &&
			  side2>=0 && side2<=1 );
          
                  printF("parabolic: boundingCurve: code=%i, grid2=%i,dir1=%i,side1=%i,dir2=%i,side2=%i\n",
			 code,grid2,dir1,side1,dir2,side2);

                  // (iv[0],iv[1]) holds the index of the point on the boundary:
                  int iv[2];

                  MappedGrid & c2=cg[grid2];
		  iv[dir1]=c2.extendedIndexRange(side1,dir1);
		  iv[dir2]=c2.extendedIndexRange(side2,dir2);

                  // (center0,center1) : point X or Y in the above figure
                  real center0, center1;

                  const bool isRectangular2=c2.isRectangular();
                  if( isRectangular2 )
		  {
		    real dx2[3],xab2[2][3];
		    c2.getRectangularGridParameters( dx2, xab2 );
#define VERTEX20(i0,i1,i2) (xab2[0][0]+dx2[0]*(i0-c2.gridIndexRange(0,0)))
#define VERTEX21(i0,i1,i2) (xab2[0][1]+dx2[1]*(i1-c2.gridIndexRange(0,1)))
#define VERTEX22(i0,i1,i2) (xab2[0][2]+dx2[2]*(i2-c2.gridIndexRange(0,2)))
                      
                    center0=VERTEX20(iv[0],iv[1],i3);
		    center1=VERTEX21(iv[0],iv[1],i3);

		  }
		  else
		  {
		    if( true )
		    {
		      // new way *wdh* 110204
		      Mapping & map2 = cg[grid2].mapping().getMapping();
		      RealArray r(1,3), x(1,3);
		      r=0.;
		      r(0,dir1) = real(side1);
		      r(0,dir2) = real(side2);
		      map2.mapS(r,x);
		      center0=x(0,0);
		      center1=x(0,1);
		    
		    }
		    else
		    {
		      // old way
		      realArray & center2 = cg[grid2].center();  
		      // communication here in parallel (we could eval the map instead?)   // communication here -- fix me --
		      center0=center2(iv[0],iv[1],i3,axis1);    
		      center1=center2(iv[0],iv[1],i3,axis2);
		    }
		  }
		  
		  printF("parabolic: find the distance to the point (x,y)=(%e,%e)\n", center0,center1);

		  if( !ok ) continue;  // *wdh* 091005 

		  if( isRectangular )
		  {
		    FOR_3D(i1,i2,i3,I1,I2,I3)
		    {
		      dist(i1,i2,i3)=min(dist(i1,i2,i3),
					 SQR(VERTEX0(i1,i2,i3)-center0)+
					 SQR(VERTEX1(i1,i2,i3)-center1) );
		    }
		    
		  }
		  else
		  {
		    dist=min(dist,
			     SQR(center(I1,I2,I3,axis1)-center0)+
			     SQR(center(I1,I2,I3,axis2)-center1));
		  }
		}
	      }
	      else // 3D
	      {
                // --- 3D ---


		Range R3(0,2);
		RealArray x,r;
		if( ok )
		{
		  x.redim(I1,I2,I3,R3); r.redim(I1.length()*I2.length()*I3.length(),1);
		}
		
		for( int j=0; j<numberOfBoundingCurves(i); j++ )
		{
		  if( ok )
		  {
		    if( isRectangular )
		    {
		      FOR_3D(i1,i2,i3,I1,I2,I3)
		      {
			x(i1,i2,i3,0)=VERTEX0(i1,i2,i3);
			x(i1,i2,i3,1)=VERTEX1(i1,i2,i3);
			x(i1,i2,i3,2)=VERTEX2(i1,i2,i3);
		      }

		    }
		    else
		    {
		      x=center(I1,I2,I3,R3); 
		    }
		  
		    x.reshape(I1.length()*I2.length()*I3.length(),R3);
		    r=-1.;  // initial guess
		  }
		  
                  // project the point on the inflowFace onto the boundary edge curve 
                  #ifdef USE_PPP
  		    edgeCurve[j].inverseMapS(x, r);  
                  #else
		    edgeCurve[j].inverseMap(x, r);  
                  #endif
		    //		    r.display("R");
                  if( max(fabs(r)) >5. )
		  {
		    if( true ) 
                      printF("parabolicInflow: WARNING: grid=%i, (side,axis)=(%i,%i) : unable to find some closest "
                               " points to the boundary curve %i. I will now check more carefully...\n",
                              grid,side,axis,j);

 		    // There may be just a few points that could not be inverted -- go back and check these pts

                    x.reshape(I1,I2,I3,R3);
                    r.reshape(I1,I2,I3);

                    const int maxNumToPrint=100;
 		    int count=0;
		    RealArray xx(1,3),rr(1,1);
		    Range R3=3;
		    FOR_3D(i1,i2,i3,I1,I2,I3)
 		    {
 		      if( maskLocal(i1,i2,i3) !=0 )
 		      {
			if( fabs(r(i1,i2,i3,0)) > 5. )
 			{
                          // We could do a brute force search here on the edge curve grid
 			  if( count < maxNumToPrint )
 			    printF(" Unable to find closest boundary curve point to grid pt x=(%8.2e,%8.2e,%8.2e) "
				   "on the inflow grid. I will choose an arbitrary pt.\n",
				   x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2));
			  else if( count==maxNumToPrint )
 			    printF(" Too many pts found, I am not printing anymore...\n");

			  if( false )
			  {
			    for( int dir=0; dir<c.numberOfDimensions(); dir++ )
			      xx(0,dir)=x(i1,i2,i3,dir);

			    rr=-1;

			    if( count==0 ) Mapping::debug=31;
			    edgeCurve[j].inverseMapS(xx, rr);
			    Mapping::debug=0;
			  
			    // printf(" edgeCurve[j]: domainDimension=%i rangeDimension=%i\n",
			    //	 edgeCurve[j].getDomainDimension(),edgeCurve[j].getRangeDimension());
			  
			    printF(" Invert this pt again: x=(%8.2e,%8.2e,%8.2e), r=%8.2e\n",
				   x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),rr(0,0));
			  }
			  r(i1,i2,i3,0)=0.;
                          count++;

 			}
                        // else
			// {
                        //   printF(" Inflow Face: x=(%8.2e,%8.2e,%8.2e) --> boundary curve r=%8.2e\n",
			// 	 x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2),r(i1,i2,i3,0));
			// }
			
 		      }
 		    }
		    printF(" Unable to find the closest boundary pt for %i grid points of "
			   " (side,axis,grid)=(%i,%i,%i)\n",count,side,axis,grid);
		      
                    x.reshape(I1.length()*I2.length()*I3.length(),R3);
                    r.reshape(I1.length()*I2.length()*I3.length());
		    // x.reshape(I1,I2,I3,R3);
		    // continue;
		  }
		  #ifdef USE_PPP
                    edgeCurve[j].mapS(r,x);
                  #else
                    edgeCurve[j].map(r,x);
                  #endif
                  x.reshape(I1,I2,I3,R3);

                  if( !ok ) continue;

                  if( isRectangular )
		  {
                    FOR_3D(i1,i2,i3,I1,I2,I3)
		    {
                      dist(i1,i2,i3)=min(dist(i1,i2,i3),
					 SQR(VERTEX0(i1,i2,i3)-x(i1,i2,i3,axis1))+
					 SQR(VERTEX1(i1,i2,i3)-x(i1,i2,i3,axis2))+
					 SQR(VERTEX2(i1,i2,i3)-x(i1,i2,i3,axis3)) );
		    }

		  }
		  else
		  {
		    dist=min(dist,
			     SQR(center(I1,I2,I3,axis1)-x(I1,I2,I3,axis1))+
			     SQR(center(I1,I2,I3,axis2)-x(I1,I2,I3,axis2))+
			     SQR(center(I1,I2,I3,axis3)-x(I1,I2,I3,axis3)));
		  }
		  
		}
		
	      } // end 3D
	      
	      
	    
	      dist=SQRT(dist);
              if( false )
	      {
  	        display(dist,sPrintF(buff,"Here is the dist array for inflowFace=%i, (grid=%s,side=%i,axis=%i)",
				     i,(const char *)c.mapping().getName(Mapping::mappingName),side,axis),pDebugFile);
	      }



	      if( parameters.bcType(side,axis,grid)==Parameters::blasiusProfile )
	      {
                // ========== blasiusProfile =========
                const real & nu = parameters.dbase.get<real>("nu");

		getGhostIndex(c.extendedIndexRange(),side,axis,Ig1,Ig2,Ig3);

		bool ok=ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,Ig1,Ig2,Ig3,includeGhost);
		// no: if( !ok ) continue;  // no points on this processor
                #ifdef USE_PPP
		  Overture::abort("bcForcing:blasiusProfile:ERROR: finish me for parallel!\n");
                #endif

		RealArray u(dist.getLength(0)*dist.getLength(1)*dist.getLength(2),2),
                          distg(Ig1,Ig2,Ig3),blasiusF,ug,fg;

		distg(Ig1,Ig2,Ig3) = dist(I1,I2,I3);
		Range AXES(c.numberOfDimensions());
		RealArray centerg(Ig1,Ig2,Ig3,Range(AXES));

		real dd=0;
		if ( isRectangular )
		{
		  real dx2[3],xab2[2][3];
		  c.getRectangularGridParameters( dx2, xab2 );

		  FOR_3D(i1,i2,i3,Ig1,Ig2,Ig3)
		  {
		    for ( int a=0; a<c.numberOfDimensions(); a++ )
		      centerg(i1,i2,i3,a) = dx2[a];
		  }
		}
		else
		  centerg(Ig1,Ig2,Ig3,AXES) = center(Ig1,Ig2,Ig3,AXES)-center(I1,I2,I3,AXES);

		for ( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
		  for ( int i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
		    for ( int i3=Ig3.getBase(); i3<=Ig3.getBound(); i3++ )
		      dd += sqrt(sum(pow(centerg(i1,i2,i3,AXES),2)));

		dd /= Ig1.length()*Ig2.length()*Ig3.length();

		real Re = bcParameters(1,side,axis,grid);
		real xre = Re * nu/bcData(1,side,axis,grid);

		if ( (xre-dd)<0. ) 
		{
		  // yikes! the first ghost line is before the flat plate!
		  // adjust the reynolds number such that the ghost line is at Re_x=dd
		  Re = bcData(1,side,axis,grid) * (2*dd)/nu;
		  xre = 2*dd;
		  cout<<"WARNING : the blasius profile specified a reynolds number that is too small for this grid spacing!"<<endl;
		  cout<<"WARNING : new new Re_x is = "<<Re<<endl;
		}

		real Re2 = max(REAL_EPSILON,( bcData(1,side,axis,grid) * ( xre -dd )/nu));

		real eta_scale = bcData(1,side,axis,grid)/(nu*sqrt(2*Re))/6.; // the 6 adjusts for the Nurbs parameterization
		real etag_scale = bcData(1,side,axis,grid)/(nu*sqrt(2*Re2))/6.;

		dist *= eta_scale;
		distg *= etag_scale;

		  
		dist.reshape(dist.getLength(0)*dist.getLength(1)*dist.getLength(2));
		blasiusF=u;

		blasiusFPInterpolant.mapS(dist,u);
		blasiusFInterpolant.mapS(dist,blasiusF);

		distg.reshape(distg.getLength(0)*distg.getLength(1)*distg.getLength(2));
		ug.resize(distg.getLength(0)*distg.getLength(1)*distg.getLength(2),2);
		fg = ug;
		blasiusFPInterpolant.mapS(distg,ug);
		blasiusFInterpolant.mapS(distg,fg);
		  
		u.reshape(I1,I2,I3,2);
		blasiusF.reshape(I1,I2,I3,2);
		dist.reshape(I1,I2,I3);

		ug.reshape(Ig1,Ig2,Ig3,2);
		fg.reshape(Ig1,Ig2,Ig3,2);
		distg.reshape(Ig1,Ig2,Ig3);

		int igo[3];
		for ( int i=0; i<c.numberOfDimensions(); i++ )
		  igo[i] = i==axis ? (side==0 ? -1 : 1 ) : 0;
		  
		for ( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		  for ( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		    for ( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
		    {
		      if ( dist(i1,i2,i3)<1. )
		      {
			bd(i1,i2,i3,0) = bcData(0,side,axis,grid);
			bd(i1,i2,i3,1) = u(i1,i2,i3,1)*bcData(1,side,axis,grid);
			bd(i1,i2,i3,2) = bcData(1,side,axis,grid)/sqrt(2*Re)*(6.*dist(i1,i2,i3)*u(i1,i2,i3,1) - blasiusF(i1,i2,i3,1));
			      
			for( int n=3; n<=C.getBound(); n++ )
			  bd(i1,i2,i3,n) = bcData(n,side,axis,grid);

		      }
		      else
		      {
			bd(i1,i2,i3,0) = bcData(0,side,axis,grid);
			bd(i1,i2,i3,1) = bcData(1,side,axis,grid);
			bd(i1,i2,i3,2) = bcData(1,side,axis,grid)/sqrt(2*Re)*1.217;

			for( int n=3; n<=C.getBound(); n++ )
			  bd(i1,i2,i3,n) = bcData(n,side,axis,grid);

		      }
		    }

		for ( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
		  for ( int i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
		    for ( int i3=Ig3.getBase(); i3<=Ig3.getBound(); i3++ )
		    {
		      if ( distg(i1,i2,i3)<1. )
		      {
			bd(i1,i2,i3,0) = bcData(0,side,axis,grid);
			bd(i1,i2,i3,1) = ug(i1,i2,i3,1)*bcData(1,side,axis,grid);
			bd(i1,i2,i3,2) = bcData(1,side,axis,grid)/sqrt(2*Re2)*(6.*distg(i1,i2,i3)*ug(i1,i2,i3,1) - fg(i1,i2,i3,1));
			      
			for( int n=3; n<=C.getBound(); n++ )
			  bd(i1,i2,i3,n) = bcData(n,side,axis,grid);
			      
		      }
		      else
		      {
			bd(i1,i2,i3,0) = bcData(0,side,axis,grid);
			bd(i1,i2,i3,1) = bcData(1,side,axis,grid);
			bd(i1,i2,i3,2) = bcData(1,side,axis,grid)/sqrt(2*Re2)*1.217;
			      
			for( int n=3; n<=C.getBound(); n++ )
			  bd(i1,i2,i3,n) = bcData(n,side,axis,grid);

		      }
		    }

	      }
	      else 
	      {
                // =================================================
                // ============= parabolic profile =================
                // =================================================

		// -- Choose the wall values for a component by looking for an adjacent side --
		// *wdh* 2011/11/06
		RealArray wallValue(C);
		wallValue=0.;
		for( int dir2=0; dir2<cg.numberOfDimensions(); dir2++ )for( int side2=0; side2<=1; side2++ )
		{
		  if( c.boundaryCondition(side2,dir2)==Parameters::noSlipWall || c.boundaryCondition(side2,dir2)==Parameters::penaltyBoundaryCondition )
		  {
		    for( int n=C.getBase(); n<=C.getBound(); n++ )
		    {
		      wallValue(n)=bcData(n,side2,dir2,grid);
		    }
		    // ::display(wallValue,"Parabolic inflow: adjacent wall values");
		    break; // use values from the first wall found 
		  }
		}

		real width = bcParameters(0,side,axis,grid);
		if( width<=0. )
		{
		  width=.1;
		  printF("parabolicInflow:ERROR: the boundary layer width is <=0. I am going to use width=%e\n",width);
		  printF(" +++ parameters.bcParameters(0,%i,%i,%i)=%e\n",
			 side,axis,grid,bcParameters(0,side,axis,grid));
		}
#define PROFILE(d,w) (d)*(2.*w-d)
#define PROFILE3(d) ( (d)*(3.+(d)*( (d)-3.)) )
		where( dist < width )
		{
                  // ******************************************************************
                  //   Here is the "parabolic" inflow profile
                  // ******************************************************************
                  if( false )
		  {
		    // This profile is smoother:
		    RealArray profile;
		    profile=dist/width;
		    profile=PROFILE3(profile);
		    for( int n=C.getBase(); n<=C.getBound(); n++ )
		    {
		      bd(I1,I2,I3,n) = profile*bcData(n,side,axis,grid);
		    }
		  }
		  else
		  {
		    for( int n=C.getBase(); n<=C.getBound(); n++ )
		    {
                      // Parabolic profile from u0 at the wall to u1 at dist.
                      real u0=wallValue(n), u1 = bcData(n,side,axis,grid);      
		      bd(I1,I2,I3,n) = u0 + dist*(2.*width-dist) * ( (u1-u0)/(width*width) );
		    }
		  }
		  
		  
		}
		otherwise()
		{
		  for( int n=C.getBase(); n<=C.getBound(); n++ )
		    bd(I1,I2,I3,n) = bcData(n,side,axis,grid);
		}
		if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflow ||
                    parameters.bcType(side,axis,grid)==Parameters::parabolicInflowRamped ||
		    parameters.bcType(side,axis,grid)==Parameters::parabolicInflowOscillating ||
		    parameters.bcType(side,axis,grid)==Parameters::parabolicInflowUserDefinedTimeDependence )
		{

		  if( debug() & 4 )
		  {
		    printF("@@@parabolicInflow: assign parabolic profile for (grid,side,axis)="
			   "(%i,%i,%i) I1=[%i,%i] I2=[%i,%i] I3=[%i,%i] C=[%i,%i] bd=[%i,%i][%i,%i][%i,%i]\n",
			   grid,side,axis,
			   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
			   C.getBase(),C.getBound(),
			   bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),bd.getBase(2),bd.getBound(2));
		  }

                  // save the boundary data in the first ghost line too.
		  getGhostIndex(c.extendedIndexRange(),side,axis,Ig1,Ig2,Ig3);
                  ok=ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,Ig1,Ig2,Ig3,includeGhost);
		  if( ok )
		  {
		    bd(Ig1,Ig2,Ig3,C)=bd(I1,I2,I3,C);
		  }
		  
		  if( false ) 
		  {
		    display(bd,sPrintF(buff,"parabolicInflow: Here is the bd array for inflowFace=%i, (grid=%s,side=%i,axis=%i)",
				       i,(const char *)c.mapping().getName(Mapping::mappingName),side,axis),pDebugFile);
		  }
		  if( false ) // --- print out which boundary data arrays exist
		  {
		    for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
		    {
		      BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid2);
		      if( pBoundaryData!=NULL )
		      {
			for( int side=0; side<=1; side++ )for( int axis=0; axis<c.numberOfDimensions(); axis++ )
			{
			  if( pBoundaryData[side][axis]!=NULL )
			  {
			    RealArray & bd = *pBoundaryData[side][axis];
			    printF("@@@parabolicInflow: (grid,side,axis)=(%i,%i,%i) bd : [%i,%i][%i,%i][%i,%i]\n",grid2,side,axis,
				   bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),bd.getBase(2),bd.getBound(2));
			  }
			}
		      }
		      else
		      {
			printF("@@@parabolicInflow: grid=%i pBoundaryData=NULL\n",grid2);
		      }
		    }
		  }
		  
		}
	      }
	      

	    }
	  }
	}
      }
      if( cg.numberOfDimensions()==3 )
	delete [] edgeCurve;
      edgeCurve=NULL;
    }
  }
  
  return 0;
}



//\begin{>>CompositeGridSolverInclude.tex}{\subsection{parabolicInflow}} 
int DomainSolver::
jetInflow(GridFunction & cgf )
// ========================================================================
// /Description:
//   Determine the values for a BC with a jetInflow profile
//  
//\end{CompositeGridSolverInclude.tex}  
// ========================================================================
{
  CompositeGrid & cg = cgf.cg;
  
  int grid, side,axis;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Ig1,Ig2,Ig3;

  const RealArray & bcParameters = parameters.dbase.get<RealArray>("bcParameters");
  const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    
    for( axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
        if( parameters.bcType(side,axis,grid)==Parameters::jetInflow )
	{
	  if( c.isRectangular() )
	  {
	    printF("\n ****jetInflow:INFO: creating center array for a rectangular grid ****\n\n");
	    c.update(MappedGrid::THEcenter);
	  }
	
	  const RealArray & center = c.center().getLocalArray();


	  real jetRadius            =bcParameters(0,side,axis,grid);
	  real jetBoundaryLayerWidth=bcParameters(1,side,axis,grid);
	  real x0                   =bcParameters(2,side,axis,grid);
	  real y0                   =bcParameters(3,side,axis,grid);
	  real z0                   =bcParameters(4,side,axis,grid);

	  // realArray & bd = mappedGridSolver[grid]->getBoundaryData(side,axis);
     	  RealArray & bd = parameters.getBoundaryData(side,axis,grid,c);
	  getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);

	  if( parameters.bcType(side,axis,grid)==Parameters::jetInflowRamped ||
	      parameters.bcType(side,axis,grid)==Parameters::jetInflowOscillating ||
	      parameters.bcType(side,axis,grid)==Parameters::jetInflowUserDefinedTimeDependence   )
	  {
	    // save the initial profile in the ghost points.
	    // we need to allocate extra space for this.

	    // this should match Parameters::getBoundaryData
	    // int extra=1;
	    // getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3,extra); 

	    // Increase the dimensions from getBoundaryData *wdh* 110828
	    I1=bd.dimension(0);
	    I2=bd.dimension(1);
	    I3=bd.dimension(2);
	    Iv[axis]=side==0 ? Range(Iv[axis].getBase()-1,Iv[axis].getBound()  ) :
	                       Range(Iv[axis].getBase()  ,Iv[axis].getBound()+1) ;

	    if( debug() & 4 ) printF("*** allocate more space for jetInflowRamp or oscillate\n");
	    bd.redim(I1,I2,I3,bd.dimension(3));
            bd=0.;
	    // *assign both below **getGhostIndexIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
	    getBoundaryIndex(c.extendedIndexRange(),side,axis,I1,I2,I3);
	  }
	      
	  RealArray dist(I1,I2,I3);
	  // dist : holds the distance of a point to the centre of the jet (x0,y0,z0)
	  if( c.numberOfDimensions()==2 )
	  {
	    dist=SQR(center(I1,I2,I3,axis1)-x0)+
	         SQR(center(I1,I2,I3,axis2)-y0);
	  }
	  else
	  {
	    dist=SQR(center(I1,I2,I3,axis1)-x0)+
	         SQR(center(I1,I2,I3,axis2)-y0)+
	         SQR(center(I1,I2,I3,axis3)-z0);
	  }
	  dist=SQRT(dist);
 
	  if( jetRadius<=0. )
	  {
	    jetRadius=.5;
	    printF("jetInflow:ERROR: the jet radius is <=0. I am going to use %e\n",jetRadius);
	    printF(" +++ parameters.bcParameters(0,%i,%i,%i)=%e\n",
		   side,axis,grid,bcParameters(0,side,axis,grid));
	  }
          dist=dist-(jetRadius-jetBoundaryLayerWidth);
	  

          printF("****Assigning the jet bc values: u=%e\n",bcData(parameters.dbase.get<int >("uc"),side,axis,grid));
	  
          // apply jet to all time dependent variables:
          const int cBase  = parameters.dbase.get<Range >("Rt").getBase();
	  const int cBound = parameters.dbase.get<Range >("Rt").getBound();
	  where( dist < 0.  )
	  {
            // Inside the jet : u = Umax
	    for( int n=cBase; n<=cBound; n++ )
	      bd(I1,I2,I3,n) = bcData(n,side,axis,grid);
             
	  }
	  elsewhere( dist > jetBoundaryLayerWidth )
	  {
            // outside the jet
	    for( int n=cBase; n<=cBound; n++ )
	      bd(I1,I2,I3,n) = 0.;
	  }
	  otherwise()
	  {
            // parabolic profile on the edge of the jet
	    for( int n=cBase; n<=cBound; n++ )
	      bd(I1,I2,I3,n) = bcData(n,side,axis,grid)*(
		1.  - dist*dist*(1./SQR(jetBoundaryLayerWidth)) );
	    
	  }
	  if( parameters.bcType(side,axis,grid)==Parameters::jetInflowRamped ||
	      parameters.bcType(side,axis,grid)==Parameters::jetInflowOscillating ||
	      parameters.bcType(side,axis,grid)==Parameters::jetInflowUserDefinedTimeDependence )
	  {
	    getGhostIndex(c.extendedIndexRange(),side,axis,Ig1,Ig2,Ig3);
	    Range C(cBase,cBound);
	    bd(Ig1,Ig2,Ig3,C)=bd(I1,I2,I3,C);
	  }
	}
      }
    }
  }
  
  return 0;
}


// kkc data for blasius boundary layer profile (incompressible, no pressure gradient)
// plot is \eta vs u/U (where eta = \frac{y}{\sqrt(U/(2 \nu x))}) 
real blasiusFPData[101][2] = {
{0                    , 0.0},
{0.06                 , 0.028175838417420964},
{0.12                 , 0.0563500571247034},
{0.18                 , 0.08451793463541994},
{0.24                 , 0.11267251931029049},
{0.3                  , 0.14080519038019543},
{0.36                 , 0.16890035493720598},
{0.42                 , 0.19694656324849039},
{0.48                 , 0.2249194022642126},
{0.54                 , 0.25280583084538255},
{0.6                  , 0.28057275709516316},
{0.6599999999999999   , 0.3082050632520891},
{0.72                 , 0.3356629149188656},
{0.78                 , 0.3629267424922668},
{0.84                 , 0.38995451806368286},
{0.8999999999999999   , 0.4167187286573838},
{0.96                 , 0.4431798260045244},
{1.02                 , 0.4693034923907229},
{1.08                 , 0.4950450417741303},
{1.14                 , 0.5203764600829638},
{1.2                  , 0.5452478957213015},
{1.26                 , 0.569628950398433},
{1.3199999999999998   , 0.5934814563426373},
{1.38                 , 0.6167598138615984},
{1.44                 , 0.639438940377873},
{1.5                  , 0.6614753538638832},
{1.56                 , 0.6828429540742936},
{1.6199999999999999   , 0.7035138288883596},
{1.68                 , 0.7234505519617491},
{1.74                 , 0.7426419306236826},
{1.7999999999999998   , 0.7610584548134709},
{1.8599999999999999   , 0.7786880498015201},
{1.92                 , 0.7955199238754136},
{1.98                 , 0.8115358636207542},
{2.04                 , 0.8267404922382544},
{2.1                  , 0.8411277771043449},
{2.16                 , 0.8547020407841734},
{2.2199999999999998   , 0.8674708311158199},
{2.28                 , 0.8794434429962847},
{2.34                 , 0.8906351165957936},
{2.4                  , 0.9010633381437991},
{2.46                 , 0.9107497332462401},
{2.52                 , 0.9197139393180132},
{2.58                 , 0.9279904905697501},
{2.6399999999999997   , 0.9355991748849826},
{2.6999999999999997   , 0.9425722626432123},
{2.76                 , 0.9489453306996989},
{2.82                 , 0.954741770866418},
{2.88                 , 0.9600078559555867},
{2.94                 , 0.9647655383526301},
{3.                   , 0.9690503049800899},
{3.06                 , 0.9729023498184952},
{3.12                 , 0.9763440287741073},
{3.1799999999999997   , 0.9794175636210367},
{3.2399999999999998   , 0.9821461480103978},
{3.3                  , 0.9845602342364304},
{3.36                 , 0.9866934379742267},
{3.42                 , 0.9885663267065067},
{3.48                 , 0.9902083622561031},
{3.54                 , 0.9916410989427075},
{3.5999999999999996   , 0.992886826584682},
{3.6599999999999997   , 0.9939672659538579},
{3.7199999999999998   , 0.994899984969221},
{3.78                 , 0.9957030712871832},
{3.84                 , 0.9963904393404663},
{3.9                  , 0.9969792010372538},
{3.96                 , 0.9974790797949735},
{4.02                 , 0.997903124544007},
{4.08                 , 0.998263785624713},
{4.14                 , 0.9985639811437477},
{4.2                  , 0.9988175391662943},
{4.26                 , 0.9990312910619668},
{4.32                 , 0.9992059786038143},
{4.38                 , 0.9993528810084874},
{4.4399999999999995   , 0.9994749984188478},
{4.5                  , 0.9995737689191935},
{4.56                 , 0.999656492694651},
{4.62                 , 0.9997230414499254},
{4.68                 , 0.9997775706623225},
{4.74                 , 0.9998223379629102},
{4.8                  , 0.9998584375707694},
{4.859999999999999    , 0.9998873871307514},
{4.92                 , 0.9999109009036956},
{4.9799999999999995   , 0.9999295942863798},
{5.04                 , 0.9999445332448167},
{5.1                  , 0.9999563956833577},
{5.16                 , 0.9999658144049658},
{5.22                 , 0.9999732610523052},
{5.279999999999999    , 0.999979098401299},
{5.34                 , 0.999983665694131},
{5.3999999999999995   , 0.9999873212464976},
{5.46                 , 0.9999900157193795},
{5.52                 , 0.9999922332452913},
{5.58                 , 0.9999938434692299},
{5.64                 , 0.9999952019613022},
{5.7                  , 0.9999961126755089},
{5.76                 , 0.9999968917921728},
{5.819999999999999    , 0.9999974472086409},
{5.88                 , 0.9999978689012992},
{5.9399999999999995   , 0.9999982132378642},
{6.                   , 0.9999984709627375} };

real blasiusFData[101][2] = {
{0                    , 0.0},
{0.06                 , 0.0008466669171917951},
{0.12                 , 0.0033824593013752047},
{0.18                 , 0.007608551450465356},
{0.24                 , 0.013524277107094125},
{0.3                  , 0.021128778309058963},
{0.36                 , 0.03042007230008167},
{0.42                 , 0.04139581793796442},
{0.48                 , 0.05405211868369644},
{0.54                 , 0.0683844344665746},
{0.6                  , 0.08438637237190257},
{0.6599999999999999   , 0.102050534879755},
{0.72                 , 0.12136743580704155},
{0.78                 , 0.1423262559421589},
{0.84                 , 0.16491395264107422},
{0.8999999999999999   , 0.189115566802507},
{0.96                 , 0.21491411483750567},
{1.02                 , 0.24229049153319898},
{1.08                 , 0.2712228796593825},
{1.14                 , 0.3016876577646161},
{1.2                  , 0.3336587927258559},
{1.26                 , 0.36710765196604367},
{1.3199999999999998   , 0.40200376647837854},
{1.38                 , 0.43831389102571433},
{1.44                 , 0.47600292471730016},
{1.5                  , 0.5150336816447978},
{1.56                 , 0.555366652661354},
{1.6199999999999999   , 0.5969609555359288},
{1.68                 , 0.6397735362817333},
{1.74                 , 0.6837600820678076},
{1.7999999999999998   , 0.7288750751045842},
{1.8599999999999999   , 0.7750714333171942},
{1.92                 , 0.8223017062387121},
{1.98                 , 0.8705174143122164},
{2.04                 , 0.9196697721841375},
{2.1                  , 0.9697099430112522},
{2.16                 , 1.0205888785516735},
{2.2199999999999998   , 1.0722580722215078},
{2.28                 , 1.124669440750325},
{2.34                 , 1.1777756692608476},
{2.4                  , 1.2315303597445024},
{2.46                 , 1.2858884179804637},
{2.52                 , 1.3408058873203257},
{2.58                 , 1.3962404163647666},
{2.6399999999999997   , 1.4521513700524953},
{2.6999999999999997   , 1.5084995268737211},
{2.76                 , 1.5652480293718785},
{2.82                 , 1.622361463882347},
{2.88                 , 1.6798065328735112},
{2.94                 , 1.7375521825050877},
{3.                   , 1.7955688594025752},
{3.06                 , 1.8538295938430485},
{3.12                 , 1.9123089698670155},
{3.1799999999999997   , 1.9709835635037867},
{3.2399999999999998   , 2.0298321177681107},
{3.3                  , 2.0888347750726517},
{3.36                 , 2.1479737689712604},
{3.42                 , 2.2072327981451303},
{3.48                 , 2.266597117730411},
{3.54                 , 2.326053591166131},
{3.5999999999999996   , 2.385590308678583},
{3.6599999999999997   , 2.445196717620003},
{3.7199999999999998   , 2.504863424230489},
{3.78                 , 2.564582143374588},
{3.84                 , 2.6243454726859725},
{3.9                  , 2.684147012663985},
{3.96                 , 2.7439812083622903},
{4.02                 , 2.8038430177315092},
{4.08                 , 2.8637283097035997},
{4.14                 , 2.9236334155548955},
{4.2                  , 2.9835550827323947},
{4.26                 , 3.0434907497647643},
{4.32                 , 3.103438009773345},
{4.38                 , 3.163394907784585},
{4.4399999999999995   , 3.2233598878905267},
{4.5                  , 3.2833314286971715},
{4.56                 , 3.3433084111821905},
{4.62                 , 3.4032898651861565},
{4.68                 , 3.463274945385173},
{4.74                 , 3.5232629759919747},
{4.8                  , 3.5832534399971467},
{4.859999999999999    , 3.6432458415070443},
{4.92                 , 3.703239818658013},
{4.9799999999999995   , 3.7632350546472435},
{5.04                 , 3.823231295384675},
{5.1                  , 3.8832283372621315},
{5.16                 , 3.943226014245327},
{5.22                 , 4.0032241950670935},
{5.279999999999999    , 4.063222774414933},
{5.34                 , 4.123221662445263},
{5.3999999999999995   , 4.183220797471189},
{5.46                 , 4.243220119610308},
{5.52                 , 4.303219589156909},
{5.58                 , 4.363219174160798},
{5.64                 , 4.423218845961706},
{5.7                  , 4.483218588459764},
{5.76                 , 4.54321837900481},
{5.819999999999999    , 4.603218211111568},
{5.88                 , 4.663218071348752},
{5.9399999999999995   , 4.723217953098396},
{6.                   , 4.783217854345184}};
