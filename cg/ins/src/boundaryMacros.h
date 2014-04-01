// ===================================================================================
//   This macro extracts the boundary data arrays
//
//  *wdh* 110312 THIS WAS COPIED FROM cg/sm/src -- FIX ME ---
// ===================================================================================
#beginMacro extractBoundaryDataArrays()
  int pdbc[2*3*2*3];
  #define dbc(s,a,side,axis) (pdbc[(s)+2*((a)+3*((side)+2*(axis)))])
  int pAddBoundaryForcing[6];
  #define addBoundaryForcing(side,axis) (pAddBoundaryForcing[(side)+2*(axis)])
  real *pbcf[2][3];
  // long int pbcfOffset[6];
  // We need an 8 byte integer so we can pass to fortran: int64_t is in stdint.h 
  int64_t pbcfOffset[6];
  #define bcfOffset(side,axis) pbcfOffset[(side)+2*(axis)]
  for( int axis=0; axis<=2; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      // *** for now make sure the boundary data array is allocated on all sides
      if( false &&   // We do NOT need to always allocate the boundaryDataArray for INS *wdh* 110313
          ( pBoundaryData[side][axis]==NULL || parameters.isAdaptiveGridProblem() ) && 
          mg.boundaryCondition(side,axis)>0 )
      {
	parameters.getBoundaryData(side,axis,grid,mg);
        // RealArray & bd = *pBoundaryData[side][axis]; // this is now done in the above line *wdh* 090819
        // bd=0.;
      }
      
      if( pBoundaryData[side][axis]!=NULL )
      {
        addBoundaryForcing(side,axis)=true;
        RealArray & bd = *pBoundaryData[side][axis];
        pbcf[side][axis] = bd.getDataPointer();
	
	// if( debug & 8 )
        // ::display(bd,sPrintF(" ++++ Cgsm: Here is bd (side,axis)=(%i,%i) ++++",side,axis),"%4.2f ");

	for( int a=0; a<=2; a++ )
	{
	  dbc(0,a,side,axis)=bd.getBase(a);
	  dbc(1,a,side,axis)=bd.getBound(a);
	}
      }
      else
      {
        addBoundaryForcing(side,axis)=false;
	pbcf[side][axis] =bcData.getDataPointer();  // should not be used in this case 
	for( int a=0; a<=2; a++ )
	{
	  dbc(0,a,side,axis)=0;
	  dbc(1,a,side,axis)=0;
	}
      }

      // for now we save the offset in a 4 byte int (double check that this is ok)
      int64_t offset = pbcf[side][axis]- pbcf[0][0];
//       if( offset > INT_MAX )
//       {
// 	printF("ERROR: offset=%li INT_MAX=%li \n",offset,(long int)INT_MAX);
//       }
//       assert( offset < INT_MAX );
      bcfOffset(side,axis) = offset;
      // bcfOffset(side,axis) = pbcf[side][axis]- pbcf[0][0];

      // cout << " **** bcfOffset= " << bcfOffset(side,axis) << endl;

    }
  }
#endMacro

