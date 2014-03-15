#include "Oges.h"

#include "OGTrigFunction.h"


#define ForBoundary(side,axis)   for( axis=0; axis<og.numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define ForAllGridPoints( i1,i2,i3 ) \
  for( i3=c.dimension()(Start,axis3); i3<=c.dimension()(End,axis3); i3++ ) \
  for( i2=c.dimension()(Start,axis2); i2<=c.dimension()(End,axis2); i2++ ) \
  for( i1=c.dimension()(Start,axis1); i1<=c.dimension()(End,axis1); i1++ )

void assignRightHandSide( Oges & og, realCompositeGridFunction & f, 
                          OGFunction & exactSolution )
{
  //  
  //================================================================
  //
  //   Forcing for Twilight-zone Flow
  //   ------------------------------
  //
  //  Add in the forcing so that the Solution is Known
  //
  //   Assign the function f at points where
  //       classify[grid](i1,i2,i3) > 0 : discretization points
  // 
  //================================================================

  OGFunction & e = exactSolution;

  int n,i1,i2,i3,axis,side;
  Range R[3];
  Index I1,I2,I3;
  real meanValue=0.;   // holds mean value of solution for singluar Neumann problem
  int nu=0,nv=1;       // for Biharmonic

  const Oges::equationTypes LaplaceDirichlet = og.LaplaceDirichlet;
  const Oges::equationTypes LaplaceNeumann   = og.LaplaceNeumann;
  const Oges::equationTypes LaplaceMixed     = og.LaplaceMixed;
//  const Oges::equationTypes Nonlinear1       = og.Nonlinear1;
//  const Oges::equationTypes Eigenvalue       = og.Eigenvalue;
  const Oges::equationTypes Biharmonic       = og.Biharmonic;
//  const Oges::equationTypes userSuppliedArray= og.userSuppliedArray;
  const Oges::equationTypes Interpolation    = og.Interpolation;
  


  int compatibilityConstraint;
  og.parameters.get(OgesParameters::THEcompatibilityConstraint,compatibilityConstraint);

  f=0.;
  int grid;
  for( grid=0; grid<og.numberOfGrids; grid++ )
  {
    MappedGrid & c = og.cg[grid];
    IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;
    getIndex( c.indexRange(),I1,I2,I3 );

    // assign interior points:
    switch( og.equationType)
    {
    case LaplaceDirichlet:
    case LaplaceMixed:
    case LaplaceNeumann:
      for( n=0; n<og.numberOfComponents; n++ )
      {
	where( og.classify[grid](I1,I2,I3,n) > 0  )
        {
          f[grid](I1,I2,I3,n)=e.xx(c,I1,I2,I3,n)+e.yy(c,I1,I2,I3,n)+e.zz(c,I1,I2,I3,n);
        }
        // note: For compatibility condition: choose solution to extra equation to be zero
        if( compatibilityConstraint )
        {
	  getIndex(c.dimension(),I1,I2,I3);  // ** this changes I1,I2,I3 ***
	  meanValue+=sum(og.rightNullVector[grid](I1,I2,I3)*e(c,I1,I2,I3,n));      
	}
      }
      break;
    case Biharmonic:
      where( og.classify[grid](I1,I2,I3,nu) > 0 )
        f[grid](I1,I2,I3,nu)=e.xx(c,I1,I2,I3,nu)+e.yy(c,I1,I2,I3,nu)+e.zz(c,I1,I2,I3,nu)
                              -e(c,I1,I2,I3,nv);
      where( og.classify[grid](I1,I2,I3,nv) > 0 )
        f[grid](I1,I2,I3,nv)=e.xx(c,I1,I2,I3,nv)+e.yy(c,I1,I2,I3,nv)+e.zz(c,I1,I2,I3,nv);
      break;
    case Interpolation:
      getIndex( c.dimension(),I1,I2,I3 );  // assign ALL points
      for( n=0; n<og.numberOfComponents; n++ )
        where( og.classify[grid](I1,I2,I3,n) > 0 )
          f[grid](I1,I2,I3,n)=e(c,I1,I2,I3,n);
      break;
    default:
      cout << "assignRightHandSide: unknown equationType = " << og.equationType << endl;
      cout << "...setting f=0." << endl;
      f[grid]=0.;
    }

    // loop over boundaries
    ForBoundary(side,axis)
    {
      if( c.boundaryCondition()(side,axis) > 0 )
      {
        Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3;
        getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
        getGhostIndex(c.gridIndexRange(),side,axis,Ig1,Ig2,Ig3);   // Index's for ghost points
        RealDistributedArray & normal = c.vertexBoundaryNormal(side,axis);
	
        switch (og.equationType)
        {
        case LaplaceDirichlet:
          if( !c.isCellCentered()(axis) )
	  { // vertex centred: BC on boundary, equation on ghost points:
            for( n=0; n<og.numberOfComponents; n++ )
	    {
              where( og.classify[grid](Ib1,Ib2,Ib3,n) > 0  )
                f[grid](Ib1,Ib2,Ib3,n)=e(c,Ib1,Ib2,Ib3,n);
              where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0 )
                f[grid](Ig1,Ig2,Ig3,n)=e.xx(c,Ib1,Ib2,Ib3,n)+e.yy(c,Ib1,Ib2,Ib3,n)
		  +e.zz(c,Ib1,Ib2,Ib3,n);
	    }
	  }
	  else
	  {
	    // cell centred: BC applied on ghost point
            for( n=0; n<og.numberOfComponents; n++ )
              where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0  )
                f[grid](Ig1,Ig2,Ig3,n)=.5*(e(c,Ib1,Ib2,Ib3,n)+e(c,Ig1,Ig2,Ig3,n));
	  }
          break;
        case LaplaceNeumann:
          for( n=0; n<og.numberOfComponents; n++ )
            if( og.numberOfDimensions==1 )
	    {
	      where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0 )
		f[grid](Ig1,Ig2,Ig3,n)= 
		  normal(Ib1,Ib2,Ib3,axis1)*e.x(c,Ib1,Ib2,Ib3,n);
	    }
            else if( og.numberOfDimensions==2 )
	    {
	      where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0 )
		f[grid](Ig1,Ig2,Ig3,n)= 
		  normal(Ib1,Ib2,Ib3,axis1)*e.x(c,Ib1,Ib2,Ib3,n)
		 +normal(Ib1,Ib2,Ib3,axis2)*e.y(c,Ib1,Ib2,Ib3,n);
	    }
            else
	    {
	      where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0 )
		f[grid](Ig1,Ig2,Ig3,n)= 
		  normal(Ib1,Ib2,Ib3,axis1)*e.x(c,Ib1,Ib2,Ib3,n)
		 +normal(Ib1,Ib2,Ib3,axis2)*e.y(c,Ib1,Ib2,Ib3,n)
		 +normal(Ib1,Ib2,Ib3,og.numberOfDimensions-1)*e.z(c,Ib1,Ib2,Ib3,n);
	    }
	  
          break;
        case LaplaceMixed:
          if( og.operators[grid].boundaryCondition()(side,axis,0)==MappedGridOperators::dirichlet )
	  { // BC on boundary, equation on ghost points:
            for( n=0; n<og.numberOfComponents; n++ )
	    {
              where( og.classify[grid](Ib1,Ib2,Ib3,n) > 0  )
                f[grid](Ib1,Ib2,Ib3,n)=e(c,Ib1,Ib2,Ib3,n);
              where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0 )
                f[grid](Ig1,Ig2,Ig3,n)=e.xx(c,Ib1,Ib2,Ib3,n)+e.yy(c,Ib1,Ib2,Ib3,n)
		  +e.zz(c,Ib1,Ib2,Ib3,n);
	    }
	  }
          else if( og.operators[grid].boundaryCondition()(side,axis,0)==MappedGridOperators::neumann )
	  {
	    for( n=0; n<og.numberOfComponents; n++ )
	      where( og.classify[grid](Ig1,Ig2,Ig3,n) > 0 )
		f[grid](Ig1,Ig2,Ig3,n)= 
		  normal(Ib1,Ib2,Ib3,axis1)*e.x(c,Ib1,Ib2,Ib3,n)
		 +normal(Ib1,Ib2,Ib3,axis2)*e.y(c,Ib1,Ib2,Ib3,n)
		 +normal(Ib1,Ib2,Ib3,og.numberOfDimensions-1)*e.z(c,Ib1,Ib2,Ib3,n);
	  }
          else
	  {
            cout << "Oges::assignRightHandSide:ERROR: unknown BC for a LaplaceMixed problem \n";
	  }
	  
          break;
        case Biharmonic:
          where( og.classify[grid](Ib1,Ib2,Ib3,nu) > 0 )
            f[grid](Ib1,Ib2,Ib3,nu)=e(c,Ib1,Ib2,Ib3,nu);
          where( og.classify[grid](Ib1,Ib2,Ib3,nv) > 0 )
            f[grid](Ib1,Ib2,Ib3,nv)=e.xx(c,Ib1,Ib2,Ib3,nu)+e.yy(c,Ib1,Ib2,Ib3,nu)
                                   +e.zz(c,Ib1,Ib2,Ib3,nu)-e(c,Ib1,Ib2,Ib3,nv);
          where( og.classify[grid](Ig1,Ig2,Ig3,nu) > 0 )
            f[grid](Ig1,Ig2,Ig3,nu)= 
              normal(Ib1,Ib2,Ib3,axis1)*e.x(c,Ib1,Ib2,Ib3,nu)
	     +normal(Ib1,Ib2,Ib3,axis2)*e.y(c,Ib1,Ib2,Ib3,nu)
	     +normal(Ib1,Ib2,Ib3,og.numberOfDimensions-1)*e.z(c,Ib1,Ib2,Ib3,nu);
          where( og.classify[grid](Ig1,Ig2,Ig3,nv) > 0 )
            f[grid](Ig1,Ig2,Ig3,nv)=e.xx(c,Ib1,Ib2,Ib3,nv)+e.yy(c,Ib1,Ib2,Ib3,nv)
	                           +e.zz(c,Ib1,Ib2,Ib3,nv);
          break;
	}
      }
    }
    // assign zero rhs to interpolation and extrapolation points
    for( int n1=0; n1<og.numberOfComponents; n1++ )
      where( og.classify[grid](I1,I2,I3,n1) <= 0  )
        f[grid](I1,I2,I3,n1)=0.;  
  }
  if( compatibilityConstraint )
  {
    // assign extra equation associated with Neumann problem
    og.equationToIndex( og.extraEquationNumber(0),n,i1,i2,i3,grid );
    f[grid](i1,i2,i3,n)=meanValue;
  }

}

void assignRightHandSide( Oges & og, realCompositeGridFunction & f, RealArray & constraintRHS )
{
  //================================================================
  //
  //   Forcing for Real Live Run
  //   -------------------------
  // Input -
  // Output
  //================================================================

  int grid;
  for( grid=0; grid<og.numberOfGrids; grid++ )
    f[grid]=0.;
  // assign rhs's for extra "constraint" equations
  int n,i1,i2,i3;
  for( int i=0; i<og.numberOfExtraEquations; i++ )
  {
    og.equationToIndex(og.extraEquationNumber(0),n,i1,i2,i3,grid);
    f[grid](i1,i2,i3,n)=constraintRHS(i);
    printf(" ogesrcNEW: assign constraintRHS(%4i) = %12.4e\n",i,constraintRHS(i));
  }
  
}    




