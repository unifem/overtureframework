#include "GenericCompositeGridOperators.h"
#include "SparseRep.h"
#include "display.h"

#include <float.h>

#define Q11(x) (1.-(x))
#define Q21(x) (x)

#define Q12(x) .5*((x)-1.)*((x)-2.)
#define Q22(x) (x)*(2.-(x))
#define Q32(x) .5*(x)*((x)-1.)

#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123(m1,m2,m3) (m1)+width[axis1]*(m2+width[axis2]*(m3))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))

// ** const int isHiddenByRefinement =MappedGrid::IShiddenByRefinement; 
// *wdh* const int isHiddenByRefinement = CompositeGrid::THEinterpolationCondition;  // ** use this value instead **
const int isHiddenByRefinement = CompositeGrid::THEinverseMap;  // user this value *wd* 060815 -- is this ok?
 

static int
getInterpolationWeights(MappedGrid & mg, const int iiv[3], const real rv[3], const int width[3], RealArray & q )
{
  
  // q holds the interpolation weights
  Range all;
  int axis;
  for( axis=mg.numberOfDimensions(); axis<3; axis++ )
    q(axis,all)=1.;

  //.........First form 1D interpolation coefficients
  for( axis=axis1; axis<mg.numberOfDimensions(); axis++ ) 
  {
    // iiv(axis)=cg.interpoleeLocation[grid](m,axis);
    real rsb=rv[axis]/mg.gridSpacing(axis)+mg.indexRange(Start,axis);
    real px= mg.isCellCentered(axis)  ? rsb-iiv[axis]-.5 : rsb-iiv[axis];

    switch (width[axis])
    {
    case 3:
      //........quadratic interpolation
      q(axis,0)=Q12(px);
      q(axis,1)=Q22(px);
      q(axis,2)=Q32(px);
      break;
    case 2:
      //.......linear interpolation
      q(axis,0)=Q11(px);
      q(axis,1)=Q21(px);
      break;
    default:
    {
      // .....order >3 - compute lagrange interpolation
      for(int m1=0; m1<width[axis]; m1++ ) 
      {
	real qq=1.;
	for( int m2=0; m2<width[axis]; m2++ )
	{
	  if( m1 != m2  )
	    qq*=(px-m2)/(m1-m2);
	}
	q(axis,m1)=qq;
      }
    }
    }
  }
  return 0;
}


static inline bool
canInterpolate( const intArray & mask, const int iiv[3], const int width[3] )
// Return TRUE if we can interpolate
// /mask (input): 
// /iiv (input) : lower left corner of the stencil to check.
// /width (input) : width of stencil.
{
  int iv[3] = { iiv[0],iiv[1],iiv[2] };  //
    
//  for( int iter=0; iter<3; iter++ )
  for( int m3=0; m3< width[axis3]; m3++ ) 
    for( int m2=0; m2< width[axis2]; m2++ ) 
      for( int m1=0; m1< width[axis1]; m1++ ) 
      {
	if( mask(iv[0]+m1,iv[1]+m2,iv[2]+m3)==0 )
	{
	  return FALSE;
	}
      }

  return TRUE;
}

static bool
checkCanInterpolate( const int grid, 
		     const int gridInterpolee, 
		     const int iv[3],
		     const intArray & mask, 
		     const int iiv[3], 
		     const int width[3] )
// Return TRUE if we can interpolate
{
  if( !canInterpolate(mask,iiv,width) )
  {
    printf("checkCanInterpolate:ERROR: Invalid interpolation of point (%i,%i,%i) on grid %i from grid %i, "
           "iiv=(%i,%i,%i)\n",
	   iv[0],iv[1],iv[2],grid,gridInterpolee,iiv[0],iiv[1],iiv[2]);
    Index M1(iiv[0],width[0]), M2(iiv[1],width[1]), M3(iiv[2],width[2]);
    displayMask(mask(M1,M2,M3),"mask values");
    Overture::abort("error");
  }
  return TRUE;
}


//\begin{>>interpolatePointsInclude.tex}{\subsubsection{interpolateRefinements}
int
interpolateRefinements(realGridCollectionFunction & u,
                       const Range *C /* =0  */,
                       const BoundaryConditionParameters & bcParams /* = Overture::defaultBoundaryConditionParameters() */ )
// =======================================================================================
// /Access: normally this routine is not directly called by a user.
// /Purpose:
//    This routine is used to interpolate refinement grids. It serves two purposes.
//  If the input grid function is not a coefficient matrix then this routine will fill in
//  refinement grid interpolation points. If the input grid function is a coefficient matrix
//  then this routine will fill in the matrix with the equations for interpolating.
//  
// /u (input) : interpolate this grid function OR add interpolation equations to this coefficient matrix.
// /C[5] (input) : interpolate these components, C[0],C[1],... (if u is NOT a coefficientMatrix). 
//               By default interpolate all components.
// 
// /Description:
//  Refinement grids need to interpolate at their boundaries from other refinement
//  grids at the same level or from a coarser level. Refinement grid interior points
//  that lie beneath a higher level refinement grid need to interpolate from the 
//  finer grid. Note that refinement grid points that interpolate in an overlapping grid
//  fashion from a a grid with a different base grid need not be considered here as these points will
//  be in the overlapping grid interpolation arrays.
//\end{GridCollectionFunction.tex}{}
// =======================================================================================
{
  GridCollection & gc = *u.getGridCollection();
  if( !bcParams.interpolateRefinementBoundaries )
  {
    printf("interpolateRefinements: do NOT interpolateRefinementBoundaries\n");
  }
  if( !bcParams.interpolateHidden )
  {
    printf("interpolateRefinements: do NOT interpolateHidden\n");
  }
  
  if( gc.numberOfRefinementLevels()<=1 || !bcParams.interpolateRefinementBoundaries )
    return 0;

  int debug=0; // 3;
  if( debug & 1 )
    printf("interpolateRefinements...\n");
  

//   if( C!=0 && (C[1].length()!=1 || C[2].length()!=1) )
//   {
//     printf("ERROR::interpolateRefinements: not implemented to interpolate higher dimensional tensors! \n");
//     throw "error";
//   }

  
  const bool explicitlyInterpolate=!u.getIsACoefficientMatrix();
  if( explicitlyInterpolate )
    printf(" ****** INFO: interpolateRefinements called : explicit interpolation ******* \n");
    
  realGridCollectionFunction & coeff_ = u;

  const int & numberOfDimensions = gc.numberOfDimensions();
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  Range all;
  
  int  iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int  jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];  j1=j2=j3=0;
  int  kv[3]; // , &k1=kv[0], &k2=kv[1], &k3=kv[2];

  int width[3] = {1,1,1};
  int shift[3] = {0,0,0};
  int iiv[3]={0,0,0};
  int rf[3]={1,1,1};  // refinement factors to the base grid
  int rrf[3]={1,1,1};  // relative refinement factors to a grid on a lower level.
  
  // IntegerArray & interpolationWidth = gc.interpolationWidth;   // **** this array should be in a GridCollection.
  IntegerArray interpolationWidth; 
  interpolationWidth.redim(3,gc.numberOfComponentGrids(),gc.numberOfComponentGrids());
  interpolationWidth=3;
  RealArray q(3,5);
  real rv[3]={0.,0.,0.};
  int side,axis,dir;

  // *** start at the finest refinement level ****
  for( int l=gc.numberOfRefinementLevels()-1; l>0; l-- )
  {
    
    GridCollection & rl   = gc.refinementLevel[l];
    GridCollection & rlm1 = gc.refinementLevel[l-1];

    // ************************************************************************
    // *** Interpolate interior points covered by another refinement patch ***
    // ************************************************************************
    int g;
    if( (bcParams.getRefinementLevelToSolveFor()<0 || l<=bcParams.getRefinementLevelToSolveFor())
        && bcParams.interpolateHidden )
    {
      for( g=0; g<rl.numberOfComponentGrids(); g++ )
      {
	assert( rl[g].isAllVertexCentered() );
      
	const int grid =rl.gridNumber(g);        // index into cg
	const int baseGrid = gc.baseGridNumber(grid);  // base grid for this refinement
	realMappedGridFunction & coeff = coeff_[grid];
	int stencilSize = explicitlyInterpolate ? 0 : coeff.sparse->stencilSize;
	int numberOfComponentsForCoefficients = explicitlyInterpolate ? 0 : coeff.sparse->numberOfComponents;
	Index NC;
	if( !explicitlyInterpolate )
	  NC=Index(0,coeff.sparse->numberOfComponents);
	Range N= (C==NULL || C[0]==nullRange) ? u[0].dimension(3) : C[0];  
      
	MappedGrid & cr = rl[g];              // refined grid
	MappedGrid & cb = gc[baseGrid];             // base grid
	const IntegerArray & indexRange = cr.indexRange();
	// const IntegerArray & maskb = cb.mask();
	const intArray & mask = cr.mask();
       
	// rl.refinementFactor is the refinement to the base grid
	rf[0]=rl.refinementFactor(0,g); 
	rf[1]=rl.refinementFactor(1,g);
	rf[2]=rl.refinementFactor(2,g);
	if( debug & 1 )
	  printf(" refinement factors l=%i are (%i,%i,%i)\n",l,rf[0],rf[1],rf[2]);
	assert( rf[0]>0 && rf[1]>0 && rf[2]>0 );

	const bool isAllVertexCentered = !gc[0].isAllCellCentered();
	const real ccOffset= isAllVertexCentered ? 0. : .5;

	// Interpolate points that lie underneath this grid on level l-1. 
	//  *** should we do all levels below?? Not required if properly nested ****
	bool onlyOneBaseGrid=false;
      
	for( int g2=0; g2<rlm1.numberOfComponentGrids() && !onlyOneBaseGrid; g2++ )
	{
	  if( rlm1.baseGridNumber(g2)!=baseGrid )
	    continue;
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    rrf[axis]=rf[axis]/rlm1.refinementFactor(axis,g2); // refinement factor to coarser grid
	    assert( rrf[axis]>0 );

	    // Iv : Index for refinement patch, with a stride.
	    Iv[axis]=Range(cr.indexRange(Start,axis),
			   cr.indexRange(End  ,axis),rrf[axis]);
	    // Jv : coarse grid index values corresponding to fine grid values in Iv
	    Jv[axis]=Range(floorDiv(Iv[axis].getBase()+rrf[axis]-1,rrf[axis]),
			   floorDiv(Iv[axis].getBound(),rrf[axis]));
	  }

	  // const IntegerArray & extended2 = rlm1[g2].extendedIndexRange();
	  // const IntegerArray & extended = cr.extendedIndexRange();
	  const IntegerArray & extended2 = extendedGridIndexRange(rlm1[g2]);
	  const IntegerArray & extended = extendedGridIndexRange(cr);

	  int pShift[3]={0,0,0}; // shift for perioidicity.
	  // --> if the grid g2 is periodic, we may have 2 sub-patches to assign
	  bool done=false;
	  for( int periodicPatch=0; periodicPatch<2 && !done; periodicPatch++ )
	  {
	    done=true;
	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
	      if( rlm1[g2].isPeriodic(axis)==Mapping::functionPeriodic && 
		  Jv[axis].getBase() < extended2(Start,axis) )
	      {
		if( debug & 2 )
		  printf("*** refinement patch covers a branch cut ****\n");
	      
		if( periodicPatch==0 )
		{
		  done=false;  // 
		}
		else 
		{ // shift by the period
		  Jv[axis]+=rlm1[g2].gridIndexRange(End,axis)-rlm1[g2].gridIndexRange(Start,axis);
		  // remember the shift so we can use it below when computing iiv
		  pShift[axis]=(rlm1[g2].gridIndexRange(End,axis)-rlm1[g2].gridIndexRange(Start,axis))*rf[axis];
		}
	      }
	    }
	  
	    // determine K[dir]=intersection of Jv with c2.indexRange();
	    bool intersects=TRUE;
	    Kv[axis3]=Range(extended2(Start,axis3),extended2(Start,axis3));
	    for( axis=0; axis<numberOfDimensions; axis++ )
	    {
	      int base =max(Jv[axis].getBase(), extended2(Start,axis));
	      int bound=min(Jv[axis].getBound(),extended2(End  ,axis));
	      if( base>bound )
	      {
		intersects=FALSE;
		break;
	      }
	      Kv[axis]=Range(base,bound);
	    }
	    if( intersects ) // ***************************************************************************
	    {
	      const intArray & mask2 = rlm1[g2].mask();
	  

	      for( i3=K3.getBase(); i3<=K3.getBound(); i3++ )
	      {
		for( i2=K2.getBase(); i2<=K2.getBound(); i2++ )
		{
		  for( i1=K1.getBase(); i1<=K1.getBound(); i1++ )
		  {
		    if( debug & 2 )
		      printf(" Interior point l=%i, g2=%i, (i1,i2,i3)=(%i,%i,%i) interp's from finer level=%i, g=%i\n",
			     l-1,g2,i1,i2,i3,l,g2);

		    if( mask2(i1,i2,i3)!=0 )
		      mask2(i1,i2,i3)|= isHiddenByRefinement;   // ******** This should be done else where
		
		    // do not overwrite unused points.
		    // *** if( mask2(i1,i2,i3)==0 )
		    if( mask2(i1,i2,i3)<=0 )  // do not overwrite unused or interpolation points.
		      continue;
		
		    const int grid2 = rlm1.gridNumber(g2);


		    int m1,m2,m3,n;

		    if( isAllVertexCentered )
		    {
		      q=0.;
		      for( dir=0; dir<numberOfDimensions; dir++ )
			width[dir]=1;
		      // width[dir]=interpolationWidth(dir,grid2,grid);  // note order of grid2,grid
		      for( dir=0; dir<3; dir++ )
		      {
			iiv[dir]=iv[dir]*rrf[dir]-pShift[dir];
			q(dir,0)=1.;   // true for vertex centered, not cell centered.
		      }
		    }
		    else
		    {  // cell centered
		      for( dir=0; dir<numberOfDimensions; dir++ )
		      {
			width[dir]=interpolationWidth(dir,grid,grid2);
			rv[dir]=(iv[dir]*rrf[dir]-indexRange(Start,dir))*cr.gridSpacing(dir)+ccOffset;
			// lower left corner of the interpolation stencil: 
			iiv[dir]=max(extended(0,dir),
				     min(extended(1,dir)-width[dir]+1,iv[dir]*rrf[dir]-width[axis]/2+1-pShift[dir])); 
		      }
		      getInterpolationWeights(cr,iiv,rv,width,q);
		    }
                
		    if( debug & 2 )
		      printf(" Interior pt l=%i, g2=%i, i=(%i,%i,%i) interp's from finer lv=%i, g=%i, iiv=(%i,%i,%i)\n",
			     l-1,g2,i1,i2,i3,l,g2,iiv[0],iiv[1],iiv[2]);

		    if( mask2(i1,i2,i3)<=0 ) 
		    {
		      // For interpolation points, check the stencil to see if we can interpolate
		      // Even for vertex grids we need to check since some un-needed interpolation points
		      // on the refinemnt grid may have been removed.
		      Index M1(iiv[0],width[0]), M2(iiv[1],width[1]), M3(iiv[2],width[2]);
		      if( min(abs(mask(M1,M2,M3)))==0 )  
			continue;
		    }
		
		    if( debug & 2 )
		      checkCanInterpolate( grid,grid2,iv, mask,iiv,width );
		
		    if( explicitlyInterpolate )
		    {  // **** this is VERY slow *** fix this!
		      u[grid2](i1,i2,i3,N)=0.;
		      for( m3=0; m3< width[axis3]; m3++ ) 
			for( m2=0; m2< width[axis2]; m2++ ) 
			  for( m1=0; m1< width[axis1]; m1++ ) 
			  {
			    u[grid2](i1,i2,i3,N)+=u[grid](iiv[0]+m1,iiv[1]+m2,iiv[2]+m3,N)*
			      q(axis1,m1)*q(axis2,m2)*q(axis3,m3);
			  }
		    }
		    else
		    {
		      realMappedGridFunction & coeffInterpolee = coeff_[grid2];
		      // ******* NOTE interpolee grid is really not ****
		      coeffInterpolee(all,i1,i2,i3)=0.;  // zero out coefficients ****** could do better here *****


		      for( m3=0; m3< width[axis3]; m3++ ) 
			for( m2=0; m2< width[axis2]; m2++ ) 
			  for( m1=0; m1< width[axis1]; m1++ ) 
			  {
			    n=0;
			    coeffInterpolee(M123CE(m1,m2,m3,n,n),i1,i2,i3)=q(axis1,m1)*q(axis2,m2)*q(axis3,m3);
			    coeffInterpolee.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
									coeff.sparse->indexToEquation(n,iiv[0]+m1,iiv[1]+m2,iiv[2]+m3) );  
			    // just copy values for other components
			    for( n=1; n<numberOfComponentsForCoefficients; n++ )
			    {
	
			      coeffInterpolee(M123CE(m1,m2,m3,n,n),i1,i2,i3)=coeff(M123CE(m1,m2,m3,0,0),i1,i2,i3);
			      coeffInterpolee.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
									  coeff.sparse->indexToEquation(n,iiv[0]+m1,iiv[1]+m2,iiv[2]+m3) );  


			    }
			  }
		      // Now add coefficient of the point being interpolated
		      m1=width[axis1]-1;
		      m2=width[axis2]-1;
		      m3=width[axis3]-1;
		      for( n=0; n<numberOfComponentsForCoefficients; n++ )
		      {
			coeffInterpolee(M123CE(m1+1,m2,m3,n,n),i1,i2,i3)=-1.;
			coeffInterpolee.sparse->setCoefficientIndex(M123CE(m1+1,m2,m3,n,n), n,i1,i2,i3, n,i1,i2,i3);
		      }
		      coeffInterpolee.sparse->setClassify(SparseRepForMGF::interpolation,i1,i2,i3,NC); 
		    }

		  }
		}
	      }  // for i3

	      rlm1[g2].mask().periodicUpdate(); // *wdh* 000529
	  
	      // if the coarsening of the fine grid patch is entirly in the parne grid then there
	      // must only be one parent:
	      onlyOneBaseGrid= K1==J1 && K2==J2 && K3==J3;
	  
	    }  // if intersects
	  }
	}
      }  // end for g
    }
    else
    {
      if( debug & 2 )
        printf("*** interpolateRefinements: do not interpolate hidden coarse grid points on level %i \n",l-1);
    }
    
    
    // **************************************************
    // *** Interpolate boundaries of refinement grids ***
    // **************************************************
    if( bcParams.interpolateRefinementBoundaries )
    {
      for( g=0; g<rl.numberOfComponentGrids(); g++ )
      {
	assert( rl[g].isAllVertexCentered() );
      
	const int grid =rl.gridNumber(g);        // index into cg
	const int baseGrid = gc.baseGridNumber(grid);  // base grid for this refinement
	realMappedGridFunction & coeff = coeff_[grid];
	int stencilSize = explicitlyInterpolate ? 0 : coeff.sparse->stencilSize;
	int numberOfComponentsForCoefficients = explicitlyInterpolate ? 0 : coeff.sparse->numberOfComponents;
	Index NC;
	if( !explicitlyInterpolate )
	  NC=Index(0,coeff.sparse->numberOfComponents);
	Range N=u[0].dimension(3);   // ******************* assumes components sit in position 3! *******
      
	MappedGrid & cr = rl[g];              // refined grid
	MappedGrid & cb = gc[baseGrid];             // base grid
	const IntegerArray & indexRange = cr.indexRange();
	// const IntegerArray & maskb = cb.mask();
	const intArray & mask = cr.mask();
       
	// rl.refinementFactor is the refinement to the base grid
	rf[0]=rl.refinementFactor(0,g); 
	rf[1]=rl.refinementFactor(1,g);
	rf[2]=rl.refinementFactor(2,g);
	if( debug & 1 )
	  printf(" refinement factors l=%i are (%i,%i,%i)\n",l,rf[0],rf[1],rf[2]);
	assert( rf[0]>0 && rf[1]>0 && rf[2]>0 );

	const bool isAllVertexCentered = !gc[0].isAllCellCentered();
	const real ccOffset= isAllVertexCentered ? 0. : .5;


	// now interpolate boundaries.
	for( axis=0; axis<numberOfDimensions; axis++ )
	{
	  for( side=Start; side<=End; side++ )
	  {
	    if( cr.boundaryCondition(side,axis)==0 )
	    {
	      getBoundaryIndex(cr.extendedIndexRange(),side,axis,I1,I2,I3);
	      for( dir=0; dir<numberOfDimensions; dir++ )
	      {
		if( dir!=axis )
		{
		  // do not interpolate points outside a true boundary.
		  if( cr.boundaryCondition(Start,dir)>0 )
		    Iv[dir]=Range(cr.indexRange(Start,dir),Iv[dir].getBound());
		  if( cr.boundaryCondition(End,dir)>0 )
		    Iv[dir]=Range(Iv[dir].getBase(),cr.indexRange(End,dir));
		}
		if( dir<axis )
		{
		  // reduce range in tangential directions already done, to avoid duplication of points at corners.
		  if( cr.boundaryCondition(Start,dir)==0 )
		    Iv[dir]=Range(Iv[dir].getBase()+1,Iv[dir].getBound());
		  if( cr.boundaryCondition(End,dir)==0 )
		    Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound()-1);
		}
	      }
	    
	      // interpolate. point (i1,i2,i3)
	      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	      {
		for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
		{
		  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		  {
		    bool pointWasInterpolated=FALSE;
		    if( mask(i1,i2,i3)== MappedGrid::ISdiscretizationPoint && 
			!(mask(i1,i2,i3) & isHiddenByRefinement) )
		    {
		      // 1. prefer interpolation from another grid at this refinement level
		      int g2;
		      for( g2=0; g2<rl.numberOfComponentGrids(); g2++ )
		      {
			if( g2!=g && rl.baseGridNumber(g2)==baseGrid )
			{
			  MappedGrid & c2 = rl[g2];
			  const IntegerArray & extended = c2.extendedIndexRange();
			  if( i1>extended(Start,axis1) && i1<extended(End,axis1) &&
			      i2>extended(Start,axis2) && i2<extended(End,axis2) &&
			      (numberOfDimensions==2 ||
			       (i3>extended(Start,axis3) && i3<extended(End,axis3)) ) )
			  {
			    // interpolate from a related refinement
			    pointWasInterpolated=TRUE; // should now try nearby points
			    if( debug & 2 )
			      printf(" rl=%i, g=%i, (i1,i2,i3)=(%i,%i,%i) mask=%i interpolates from same level, g2=%i\n",
				     l,g,i1,i2,i3,mask(i1,i2,i3),g2);
                         
			    const int grid2 = rl.gridNumber(g2);

			    if( debug & 2 )
			      checkCanInterpolate( grid,grid2,iv, c2.mask(),iv,width );

			    if( explicitlyInterpolate )
			    {
			      u[grid](i1,i2,i3,N)=u[grid2](i1,i2,i3,N); // note: same index space
			    }
			    else
			    {
			      realMappedGridFunction & coeffInterpolee = coeff_[grid2];

			      for( dir=0; dir<numberOfDimensions; dir++ )    // +++++++++++++++++++++++++ fix
				width[dir]=1;
			      coeff(all,i1,i2,i3)=0.;  // zero out coefficients ****** could do better here *****
			      int m1=0,m2=0,m3=0,n=0;
			      coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=1.;
			      coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
								coeffInterpolee.sparse->indexToEquation(n,i1,i2,i3) );  
			      // just copy values for other components
			      for( n=1; n<numberOfComponentsForCoefficients; n++ )
			      {
	
				coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=coeff(M123CE(m1,m2,m3,0,0),i1,i2,i3);
				coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
								  coeffInterpolee.sparse->indexToEquation(n,i1,i2,i3) );
			      }
			      // Now add coefficient of the point being interpolated
			      m1=width[axis1]-1;
			      m2=width[axis2]-1;
			      m3=width[axis3]-1;
			      for( n=0; n<numberOfComponentsForCoefficients; n++ )
			      {
				coeff(M123CE(m1+1,m2,m3,n,n),i1,i2,i3)=-1.;
				coeff.sparse->setCoefficientIndex(M123CE(m1+1,m2,m3,n,n), n,i1,i2,i3, n,i1,i2,i3);
			      }
			      coeff.sparse->setClassify(SparseRepForMGF::interpolation,i1,i2,i3,NC); 
			    }
			  }
			}
		      }
		    
		      if( !pointWasInterpolated )
		      {

			// 2. prefer interpolation from the underlying grid(s) at level l-1.
			for( g2=0; g2<rlm1.numberOfComponentGrids() && !pointWasInterpolated; g2++ )
			{
			  if( rlm1.baseGridNumber(g2)!=baseGrid )
			    continue;

			  MappedGrid & c2 = rlm1[g2];
			  const int grid2 = rlm1.gridNumber(g2);   // index into cg
			  const IntegerArray & gid2=c2.gridIndexRange();
			  const IntegerArray & extended = c2.extendedIndexRange();
			  j3=extended(Start,axis3);
			  bool ok=TRUE;
			  // we need to watch out for the case when the refinement grid crosses
			  // a periodic boundary on the base grid. The refinement grid pacth may
			  // extend from [-10,10] for example.
			  int pshift[3]={0,0,0}; // holds shifts for periodicity
			  for( dir=0; dir<numberOfDimensions; dir++ )
			  {
			    rrf[dir]=rf[dir]/rlm1.refinementFactor(dir,g2); // refinement factor to coarser grid
			    assert( rrf[dir]>0 );
			    jv[dir]=floorDiv(iv[dir],rrf[dir]);
			    // adjust for periodicity
			    if( c2.isPeriodic(dir)==Mapping::functionPeriodic )
			    { // adjust for periodicity, remember adjustment for below.
			      int period=gid2(End,dir)-gid2(Start,dir);
			      kv[dir] =((jv[dir]+period-gid2(Start,dir)) % period)+gid2(Start,dir);
			      pshift[dir]=kv[dir]-jv[dir];
			      jv[dir]=kv[dir];
			    }
			    ok= jv[dir] >=extended(Start,dir) && jv[dir] <= extended(End,dir);
			    if( !ok )
			      break;
			  }
			  if( ok )
			  {
			    // interpolate from a grid on level l-1
			    pointWasInterpolated=TRUE;  // should now try nearby points
			    if( debug & 2 )
			      printf(" rl=%i, g=%i, (i1,i2,i3)=(%i,%i,%i) interpolates from coarser level, g2=%i\n",
				     l,g,i1,i2,i3,g2);

			    int m1,m2,m3,n;
			    iiv[2]=extended(0,2);
			    for( dir=0; dir<numberOfDimensions; dir++ )
			    {
			      if( (iv[dir]-cr.indexRange(Start,dir)) % rrf[dir] ==0 )
				width[dir]=1;
			      else
				width[dir]=interpolationWidth(dir,grid,grid2);

			      rv[dir]=(iv[dir]/real(rrf[dir])+pshift[dir]-gid2(Start,dir))*c2.gridSpacing(dir)+ccOffset;

			      // lower left corner of the interpolation stencil: we have a choice of
			      // which way to shift the stencil. Shift toward the refinement grid.
			      // 
			      //      X--+--X--x--X--+--X
			      //      1     2     3     4
			      // With width=3, fine grid point x can interpolate from either coarse grid points
			      // 1-2-3 or points 2-3-4
			      if( axis==dir && side==1 )
			      { // shift left if we have a choice
				iiv[dir]=floorDiv(iv[dir],rrf[dir])+pshift[dir] -(width[dir]-1)/2;
				shift[dir]=width[dir]==1 ? 0 : -1;  // we shifted to the left
			      }
			      else
			      {
				iiv[dir]=floorDiv(iv[dir]+rrf[dir]-1,rrf[dir])+pshift[dir]-(width[dir]-1)/2;
				// iiv[dir]=floorDiv(iv[dir]+rrf[dir]/2,rrf[dir])+pshift[dir]-(width[dir]-1)/2;
				shift[dir]=width[dir]==1 ? 0 : 1;  // we shifted to the right
			      }

			      iiv[dir]=max(extended(0,dir),min(extended(1,dir)-width[dir]+1,iiv[dir]));
			    } // end for dir

			    if( !canInterpolate( c2.mask(),iiv,width ) )
			    {
			      pointWasInterpolated=false;
			      // We failed to interpolate. Since we had a choice with the stencil
			      // try some of the other choices
			      /* --- *wdh*
				 for( dir=0; dir<numberOfDimensions; dir++ )
				 { // first compute the shift in each direction.
				 // shift[dir]==1 means the stencil was shifted 1 to the right. We could shift left.
				 shift[dir]=((iiv[dir]+(width[dir]-1)/2)*rrf[dir] -iv[dir]);
				 assert( abs(shift[dir])<=1 );
				 }
				 ---- */
			      if( debug & 2 )
				printf(" iv=%i,%i width=(%i,%i,%i), shift=(%i,%i,%i) iiv=%i,%i, g2=%i, grid2=%i\n",
				       i1,i2,width[0],width[1],width[2],
				       shift[0],shift[1],shift[2],iiv[0],iiv[1],g2,grid2);
			      for( int s3=0; s3<=abs(shift[2]); s3++ )
			      {
				iiv[2]-=s3*shift[2]; // undo the shift the second time through.
				for( int s2=0; s2<=abs(shift[1]); s2++ )
				{
				  iiv[1]-=s2*shift[1];
				  for( int s1=0; s1<=abs(shift[0]); s1++ )
				  {
				    if( s1==0 && s2==0 && s3==0 ) // we already tried this case.
				      continue;
				    iiv[0]-=s1*shift[0];
				    if( debug & 2 )
				      printf(" *** INFO: shifting the interp. stencil shift=(%i,%i,%i) iiv=%i,%i\n",
					     s1*shift[0],s2*shift[1],s3*shift[2],iiv[0],iiv[1]);
				    if( canInterpolate( c2.mask(),iiv,width ) )
				    {
				      if( debug & 2 )
					printf(" *** WARNING: shifting the interp. stencil shift=(%i,%i,%i) iiv=%i,%i\n",
					       s1*shift[0],s2*shift[1],s3*shift[2],iiv[0],iiv[1]);
				      pointWasInterpolated=TRUE;
				      break;
				    }
				    iiv[0]+=s1*shift[0];
				  }
				  if( pointWasInterpolated ) break;
				  iiv[1]+=s2*shift[1];
				}
				if( pointWasInterpolated ) break;
				iiv[2]+=s3*shift[2];
			      }
			      if( !pointWasInterpolated )
			      {
				if( debug & 2 )
				{
				  // this is not fatal if there is another grid at this level that
				  // we can interpolate from -- we could check for this, we must be
				  // on or just outside the boundary of this grid.
				  printf("...INFO: grid=%i, iv=[%i,%i] cannot interpolate from coarser grid=%i, "
					 " iiv=(%i,%i,%i)\n",
					 grid,i1,i2,grid2,iiv[0],iiv[1],iiv[2]);
				  Index M1(iiv[0]-1,width[0]+2), M2(iiv[1]-1,width[1]+2), M3(iiv[2]-1,width[2]+2);
				  const intArray & mask2 = c2.mask();
				  if( numberOfDimensions==2 )
				  {
				    displayMask(mask2(M1,M2,iiv[2]),"mask on the coarse grid with 1 neighbour");
				    displayMask(cr.mask()(Range(i1-1,i1+1),Range(i2-1,i2+1),i3),
						"Refined grid mask the neighbours");
				  }
				  else if( numberOfDimensions==3 )
				  {
				    displayMask(mask2(M1,M2,M3),"here is the mask on the coarse grid with 1 neighbour");
				    displayMask(cr.mask()(Range(i1-1,i1+1),Range(i2-1,i2+1),Range(i3-1,i3+1)),
						"Refined grid mask the neighbours");
				  }
				}
				// look for another grid at this level to interpolate from
			      }
			    
			    } // if( !canInterpolate )
			  
			    if( pointWasInterpolated )
			    {
			      getInterpolationWeights(c2,iiv,rv,width,q);
			  
			      if( debug & 2 )
				checkCanInterpolate( grid,grid2,iv, c2.mask(),iiv,width );

			      if( explicitlyInterpolate )
			      {
				u[grid](i1,i2,i3,N)=0.;
				for( m3=0; m3< width[axis3]; m3++ ) 
				  for( m2=0; m2< width[axis2]; m2++ ) 
				    for( m1=0; m1< width[axis1]; m1++ ) 
				    {
				      u[grid](i1,i2,i3,N)+=u[grid2](iiv[0]+m1,iiv[1]+m2,iiv[2]+m3,N)*
					q(axis1,m1)*q(axis2,m2)*q(axis3,m3);
				    }
			      }
			      else
			      {
				realMappedGridFunction & coeffInterpolee = coeff_[grid2];
				coeff(all,i1,i2,i3)=0.;  // zero out coefficients ****** could do better here *****

				if( debug & 2 )
				  printf("  ... iiv=(%i,%i,%i) rv=(%6.2e,%6.2e,%6.2e) width=(%i,%i,%i)\n",
					 iiv[0],iiv[1],iiv[2],rv[0],rv[1],rv[2],width[0],width[1],width[2]);
			    
				// **** should we check the mask values here??
				for( m3=0; m3< width[axis3]; m3++ ) 
				  for( m2=0; m2< width[axis2]; m2++ ) 
				    for( m1=0; m1< width[axis1]; m1++ ) 
				    {
				      n=0;
				      coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=q(axis1,m1)*q(axis2,m2)*q(axis3,m3);
				      coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
									coeffInterpolee.sparse->indexToEquation(n,iiv[0]+m1,iiv[1]+m2,iiv[2]+m3) );  
				      // just copy values for other components
				      for( n=1; n<numberOfComponentsForCoefficients; n++ )
				      {
	
					coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=coeff(M123CE(m1,m2,m3,0,0),i1,i2,i3);
					coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
									  coeffInterpolee.sparse->indexToEquation(n,iiv[0]+m1,iiv[1]+m2,iiv[2]+m3) );  
				      }
				    }
				// Now add coefficient of the point being interpolated
				m1=width[axis1]-1;
				m2=width[axis2]-1;
				m3=width[axis3]-1;
				for( n=0; n<numberOfComponentsForCoefficients; n++ )
				{
				  coeff(M123CE(m1+1,m2,m3,n,n),i1,i2,i3)=-1.;
				  coeff.sparse->setCoefficientIndex(M123CE(m1+1,m2,m3,n,n), n,i1,i2,i3, n,i1,i2,i3);
				}
				coeff.sparse->setClassify(SparseRepForMGF::interpolation,i1,i2,i3,NC); 

			      }
			    } // if point was interpolated
			  } // end if( ok )
			
			
			} // for g2 coarser level grids
		      }  // end if( !pointWasInterpolated )

		      if( !pointWasInterpolated )
		      {
			printf("interpolateRefinements::ERROR: unable to interpolate refinement point. rl=%i, g=%i, "
			       "(i1,i2,i3)=(%i,%i,%i) grid=%i\n",l,g,i1,i2,i3,grid);

			// as a last resort we just interpolate from the neighbours on the same grid.
			// -- this may happen for a rf=4 ---
                      
			// try to interpolate from points  iv[axisp1]-1,..,+1

			if( true )
			{
			  Overture::abort("error");
			}
/* ------
			if( explicitlyInterpolate )
			{
			  Overture::abort("error");
			}
			else
			{
			  for( dir=0; dir<numberOfDimensions; dir++ )  
			    width[dir]=3;
		      
			  coeff(all,i1,i2,i3)=0.;  // zero out coefficients ****** could do better here *****
			  int mv[3], &m1=mv[0], &m2=mv[1], &m3=mv[2], n;
			  m1=0,m2=0,m3=0;
			  int axisp1 = (axis+1) % numberOfDimensions;

			  iiv[0]=i1, iiv[1]=i2, iiv[2]=i3;  // lower corner of the interpolation stencil
			  iiv[axisp1]-=1;
			
			  //nt axisp2 = (axis+2) % numberOfDimensions;
			  // const int n2Count = numberOfDimensions==3 ? 1 : 0;
			  //  for( int n2=-n2Count; n2<=n2Count; n2+=2 )
			  // mv[axisp2]=n2;

			  mv[axis]=0;
			  for( int n1=0; n1<=2; n1+=2 )
			  {
			    mv[axisp1]=n1;
			    n=0;
			    // interpolation formula is u_i = .5*( u_{i+1) + u_{i-1} ) (2D)
			    printf(" interp coeff: m1=%i,m2=%i,m3=%i iiv[0]+m1=%i,iiv[1]+m2=%i,iiv[2]+m3=%i\n",
				   m1,m2,m3, iiv[0]+m1,iiv[1]+m2,iiv[2]+m3);
			  
			    coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=.5;
			    coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
							      n,iiv[0]+m1,iiv[1]+m2,iiv[2]+m3 );
			    // just copy values for other components
			    for( n=1; n<numberOfComponentsForCoefficients; n++ )
			    {
	
			      coeff(M123CE(m1,m2,m3,n,n),i1,i2,i3)=coeff(M123CE(m1,m2,m3,0,0),i1,i2,i3);
			      coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,n,n), n,i1,i2,i3, 
								n,iiv[0]+m1,iiv[1]+m2,iiv[2]+m3 );
			    }
			  }
		      
			  // Now add coefficient of the point being interpolated
			  m1=width[axis1]-1;
			  m2=width[axis2]-1;
			  m3=width[axis3]-1;
			  for( n=0; n<numberOfComponentsForCoefficients; n++ )
			  {
			    coeff(M123CE(m1+1,m2,m3,n,n),i1,i2,i3)=-1.;
			    coeff.sparse->setCoefficientIndex(M123CE(m1+1,m2,m3,n,n), n,i1,i2,i3, n,i1,i2,i3);
			  }
			  coeff.sparse->setClassify(SparseRepForMGF::interpolation,i1,i2,i3,NC); 
			}
--- */
			// throw "error";
		      }
		    
		    }
		  }  // end for i1
		} // end for i2
	      }  // end for i3
	    
	    }
	  }
	}  // end for axis
      
      }  // end for g
    } // end if( bcParams.interpolateRefinements
  }
  return 0;
}



#undef Q11
#undef Q21

#undef Q12
#undef Q22
#undef Q32

#undef CE
#undef M123
#undef M123CE

