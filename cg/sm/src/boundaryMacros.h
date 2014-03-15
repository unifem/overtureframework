// ===================================================================================
//   This macro extracts the boundary data arrays
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
      if( ( pBoundaryData[side][axis]==NULL || parameters.isAdaptiveGridProblem() ) && 
          mg.boundaryCondition(side,axis)>0 )
      {
	parameters.getBoundaryData(side,axis,grid,mg);
        // RealArray & bd = *pBoundaryData[side][axis]; // this is now done in the above line *wdh* 090819
        // bd=0.;
      }
      
      if( pBoundaryData[side][axis]!=NULL )
      {
	if( debug & 8 )
	  printP("+++ Cgsm: add boundary forcing to (side,axis,grid)=(%i,%i,%i) useConservative=%i\n",side,axis,grid,
		 (int)useConservative);
	
        addBoundaryForcing(side,axis)=true;
        RealArray & bd = *pBoundaryData[side][axis];
        pbcf[side][axis] = bd.getDataPointer();
	
	// if( debug & 8 )
        //  ::display(bd," ++++ Cgsm: Here is bd ++++","%4.2f ");


	for( int a=0; a<=2; a++ )
	{
	  dbc(0,a,side,axis)=bd.getBase(a);
	  dbc(1,a,side,axis)=bd.getBound(a);
	}
      }
      else
      {
        addBoundaryForcing(side,axis)=false;
	pbcf[side][axis] = fptr;  // should not be used in this case 
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

// ===============================================================================================
// This macro determines the pointers to the variable material properties that are
// used when calling fortran routines.
// ===============================================================================================
#beginMacro getVariableMaterialPropertyPointers(defaultMatIndexPtr,defaultMatValPtr)

 // --- Variable material properies ---
 GridMaterialProperties::MaterialFormatEnum materialFormat = GridMaterialProperties::constantMaterialProperties;
 int ndMatProp=1;  // for piecewise constant materials, this is the leading dimension of the matVal array
 int *matIndexPtr=defaultMatIndexPtr;  // if not used, point to mask
 real*matValPtr=defaultMatValPtr;       // if not used, point to u
 if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
 {
   // Material properties do vary 
   std::vector<GridMaterialProperties> & materialProperties = 
	parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");

   GridMaterialProperties & matProp = materialProperties[grid];
   materialFormat = matProp.getMaterialFormat();
   
   if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
   {
	IntegerArray & matIndex = matProp.getMaterialIndexArray();
     matIndexPtr = matIndex.getDataPointer();
   }
   
   RealArray & matVal = matProp.getMaterialValuesArray();
   matValPtr = matVal.getDataPointer();
   ndMatProp = matVal.getLength(0);  


   // ::display(matVal,"matVal");
 }
#endMacro
