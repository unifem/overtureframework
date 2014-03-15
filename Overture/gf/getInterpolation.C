
//\begin{>>GenericGridCollectionOperatorsInclude.tex}{\subsubsection{getInterpolationCoefficientsForRefinements}}
#ifdef COMPOSITE_GRID_OPERATORS
int GenericCompositeGridOperators::
#else
int GenericGridCollectionOperators::
#endif
getInterpolationCoefficientsForRefinements( realGridCollectionFunction & coeff  )
// =======================================================================================
// /Description:
//    Fill in the interpolation equations for refinement grids.
//  Refinement grids need to interpolate at their boundaries from other refinement
//  grids at the same level or from a coarser level. Refinement grid interior points
//  that lie beneath a higher level refinement grid need to interpolate from the 
//  finer grid. Note that refinement grid points that interpolate in an overlapping grid
//  fashion from a a grid with a different base grid need not be considered here as these points will
//  be in the overlapping grid interpolation arrays.
//\end{GenericGridCollectionOperatorsInclude.tex}{}
// =======================================================================================
{
  printf("GenericGridCollectionOperators::getInterpolationCoefficientsForRefinements\n");
  
  GridCollection & gc = *coeff.getGridCollection();

  if( gc.numberOfRefinementLevels()==0 )
    return 0;
  
  const int & numberOfDimensions = gc.numberOfDimensions();
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Iv[2];
  
  int  iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int  jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];

  for( int l=1; l<gc.numberOfRefinementLevels(); l++ )
  {
    
    GridCollection & rl   = gc.refinementLevel[l];
    GridCollection & rlm1 = gc.refinementLevel[l-1];
    for( int g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      const int grid =rl.gridNumber(g);        // index into cg
      const int bg = gc.baseGridNumber(grid);  // base grid for this refinement
      
      MappedGrid & cr = rl[g];              // refined grid
      MappedGrid & cb = gc[bg];             // base grid
      // const IntegerArray & maskb = cb.mask();
      const IntegerArray & mask = cr.mask();
       
      int rf[3];  // refinement factors
      rf[0]=rl.refinementFactor(0,g);
      rf[1]=rl.refinementFactor(1,g);
      rf[2]=rl.refinementFactor(2,g);
      
      assert( rf[0]>0 && rf[1]>0 && rf[2]>0 );


      // first interpolate boundaries.
      int side,axis;
      for( axis=0; axis<numberOfDimensions; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
          if( cr.boundaryCondition(side,axis)==0 )
	  {
	    getBoundaryIndex(cr.extendedIndexRange(),side,axis,I1,I2,I3);
	    
            // interpolate. point (i1,i2,i3)
            for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      {
		for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
		{
		  bool pointWasInterpolated=FALSE;
		  if( mask(i1,i2,i3)==MappedGrid::ISghostPoint )
		  {
		    // 1. prefer interpolation from another grid at this refinement level
		    int g2;
		    for( g2=0; g2<rl.numberOfComponentGrids(); g2++ )
		    {
		      if( g2!=g )
		      {
			MappedGrid & c2 = rl[g2];
			const IntegerArray & extended = c2.extendedIndexRange();
			if( i1>extended(Start,axis1) && i1<extended(End,axis1) &&
			    i2>extended(Start,axis2) && i2<extended(End,axis2) ) // ***** 3D ****
			{
			  // interpolate from a related refinement
			  pointWasInterpolated=TRUE; // should now try nearby points
			  printf(" rl=%i, g=%i, (i1,i2,i3)=(%i,%i,%i) interpolates from same level, g2=%i\n",
				 l,g,i1,i2,i3,g2);
			}
		      }
		    }
		    if( !pointWasInterpolated )
		    {
		      // 2. prefer interpolation from the underlying grid(s) at level l-1.
		      for( g2=0; g2<rlm1.numberOfComponentGrids(); g2++ )
		      {
			MappedGrid & c2 = rlm1[g2];
			const IntegerArray & extended = c2.extendedIndexRange();
			j3=extended(Start,axis3);
			for( axis=0; axis<numberOfDimensions; axis++ )
			  jv[axis]=iv[axis]/rf[axis];  
			if( j1>extended(Start,axis1) && j1<extended(End,axis1) &&
			    j2>extended(Start,axis2) && j2<extended(End,axis2) )
			{
			  // interpolate from a related refinement
			  pointWasInterpolated=TRUE;  // should now try nearby points
			  printf(" rl=%i, g=%i, (i1,i2,i3)=(%i,%i,%i) interpolates from next level, g2=%i\n",
				 l,g,i1,i2,i3,g2);

			}
		      }
		    }
		    if( !pointWasInterpolated )
		    {
		      printf("getInterp::ERROR: unable to interpolate refinement grid point. rl=%i, g=%i, "
			     "(i1,i2,i3)=(%i,%i,%i)\n",l,g,i1,i2,i3);
		    }
		
		  }
		}  // end for i1
	      } // end for i2
	    }  // end for i3
	    
            // Interpolate points that lie underneath this grid on level l-1.

            for( axis=0; axis<3; axis++ )
	    {
	      Iv[axis]=Range(cr.extendedIndexRange(Start,axis)+1,cr.extendedIndexRange(Start,axis)-1,rf[axis]);
              // Jv : coarse grid index values corresponding to fine grid values in Iv
              Jv[axis]=Range((Iv[axis].getBase()+rf[axis]-1)/rf[axis],Iv[axis].getBound()/rf[axis]);
	    }
            for( int g2=0; g2<rlm1.numberOfComponentGrids(); g2++ )
	    {
              // determine K[dir]=intersection of Jv with c2.indexRange();
              const IntegerArray & indexRange2 = rlm1[g2].indexRange();
              bool intersects=TRUE;
              Kv[axis3]=Range(indexRange2(Start,axis3),indexRange2(Start,axis3));
              for( axis=0; axis<numberOfDimensions; axis++ )
	      {
		Kv[axis]=Range( max(Jv[axis].getBase(), indexRange2(Start,axis)),
                                min(Jv[axis].getBound(),indexRange2(End  ,axis)) );
                if( Kv[axis].getBase()>Kv[axis].getBound() )
		{
		  intersects=FALSE;
		  break;
		}
	      }
	      if( intersects )
	      {
		for( int k3=K3.getBase(); k3<=K3.getBound(); k3++ )
		{
		  for( int k2=K2.getBase(); k2<=K2.getBound(); k2++ )
		  {
		    for( int k1=K1.getBase(); k1<=K1.getBound(); k1++ )
		    {
		      printf(" Interior point l=%i, g2=%i, (k1,k2,k3)=(%i,%i,%i) interpolates from next level, g=%i\n",
			     l,g2,k1,k2,k3,g2);
		    }
		  }
		}
		if( K1==J1 && K2==J2 && K3==J3 )
		  break;   // we are done.    **** could do better here ****
	      }
	    }
	  }
	}
      }
    }
  }

  return 0;
}




