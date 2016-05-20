/* $Id: generateMatrix.C,v 1.18 2010/04/28 21:11:14 chand Exp $ */
#include "Oges.h"
#include "SparseRep.h"
#include "conversion.h"
#include "EquationSolver.h"
#include "display.h"
#include "OgesExtraEquations.h"

#define EQUATIONNUMBER(i,n,I1,I2,I3) \
  EQUATIONNUMBERX(i+stencilDim*(n),I1,I2,I3)

#define EQUATION_NO(n,i1,i2,i3) (n+numberOfComponents*(i1+ndr*(i2+nds*(i3)))+eqnOffset )

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define CGESER EXTERN_C_NAME(cgeser)

#ifdef OV_USE_DOUBLE
#define SS2Y   EXTERN_C_NAME(ds2y)
#define QS2I1  EXTERN_C_NAME(qs2i1d)
#else
#define SS2Y   EXTERN_C_NAME(ss2y)
#define QS2I1  EXTERN_C_NAME(qs2i1r)
#endif

extern "C" { 
// add extra arguments for the lengths of strings

  void CGESER(char line[],const int &ierr,const int len_line);
  void SS2Y(const int &neq,const int &nze,const int &ja,const int &ia,const real &a,const int &isym);
  void QS2I1( const int &ia, const int &ja, const real &a, const int &neq, const int &kflag );

}


void Oges::
generateMatrixError( const int nda0, const int ieqn )  
{
  cerr << "ogesGenerateMatrix:ERROR: Not enough space to store Matrix"  << endl;
  cerr << " number of equations             " << numberOfEquations << endl;
  cerr << " processing stopped at eqn ieqn= " << ieqn           << endl;
  cerr << " allocated space for matrix, nda=" << nda0           << endl;
  cerr << " current  parameters.zeroRatio             =" << parameters.zeroRatio      << endl;
  cerr << " apparent zeroRatio : nda/ieqn  =" << nda0/real(ieqn)<< endl;
  cerr << " ***Specify a bigger zeroRatio and rerun*** " << endl;
  exit(1);
}

void Oges::
generateMatrix( int & errorNumber )
{
//=========================================================================================================
// /Description:
//   Create the sparse matrix representation (ia,ja,a) from the coefficient matrix 'coeff'.
// There are two supported representations for the sparse format,
// \begin{description}
//  \item[uncompressed] {\tt (ia(i),ja(i),a(i)) i=0,1,...numberOfNonzeros} are the elements
//     of the sparse matrix
//  \item[compressedRow] {\tt (ia(k),ia(k+1)-1)} : number of entries in row k, {\tt k=0,1,...,numberOfEquations},
//      {\tt (ja(i),a(i)) i=0,1,...numberOfNonzeros} are the column numbers and array
//     elements. 
// \end{description}
//=========================================================================================================
  #ifdef USE_PPP
    printF("Oges::ERROR:serial generateMatrix called in parallel!\n"
           "      You should use the PETSc solver option for parallel.\n");
    OV_ABORT("Oges::ERROR");
  #endif

  real cpu0=getCPU();
  
  errorNumber=0;
  
  if (Oges::debug & 4) 
  {
    cout << "generateMatrix Entering..." << endl;
  }

  //    ...some solvers store ia() in a compressed form
  //          Yale : compressed
  //          SOR  : compressed
  //          SLAP : uncompressed
  //          PETSc: compressed - unless we have to form the transpose,
  //                 then initially create the matrix in un-compressed form

  int isparse;
  if (sparseStorageFormat!=other) 
  {
    isparse = sparseStorageFormat==compressedRow ? 0 : 1;
    // when solving for the transpose we must initially generate the matrix in uncompressed form
    if( parameters.solveForTranspose  && 
      (parameters.solver==OgesParameters::SLAP || parameters.solver==OgesParameters::PETSc))
      isparse=1;   // uncompressed
  }
  else 
  {
    isparse = 2;
  }

  if (debug & 2) 
  {
    cout << "OgesGenerateMatrix: isparse = " << isparse << endl;
  }

  int i1,i2,i3;
  int n,ieqn,i,jeqn,grid;
  real scale;
  
//  int stencilLength = coeff[0].sparse->stencilSize;
  const int stencilLength = stencilSize;
  assert( stencilLength>0 );
  const int stencilDim    = stencilLength*numberOfComponents;

  real cpuFill=0.;

  int *iac = ia.getDataPointer();  iac--;  // equation numbers start at 1
  int *jac = ja.getDataPointer();  jac--;
  real *ac = a.getDataPointer();   ac--;
#define IA(i) iac[i]
#define JA(i) jac[i]
#define A(i)  ac[i]

  // *************************************************************************************
  // When there are in-active grids, we need to shift the equation numbers
  // that are stored in the sparseRep. The activeGridShift array is 
  // defined to do this:
  //    newEquationNumber(grid) = oldEquationNumber(grid) + activeGridShift[grid]
  // *************************************************************************************
  const bool useSomeGrids = !useAllGrids;
  int *activeGridShift = new int [numberOfGrids]; // delete this ****
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    if( useAllGrids )
    {
      activeGridShift[grid]=0;
    }
    else if( useThisGrid(grid) )
    {
      if( grid==0 ) 
        activeGridShift[grid]=0;
      else
        activeGridShift[grid]=activeGridShift[grid-1];
    }
    else 
    {
      // If this grid is NOT used then we shift all grids that follow.
      const int shift=numberOfComponents* arraySize(grid,axis1)*arraySize(grid,axis2)*arraySize(grid,axis3);
      if( grid==0 )
	activeGridShift[grid]=-shift;
      else        
	activeGridShift[grid]=activeGridShift[grid-1]-shift;
    }

    if( debug & 8 ) printf("+++ activeGridShift[%i]=%i\n",grid,activeGridShift[grid]);
    
  }

  int ii=0;
  for (grid=0;grid<numberOfGrids;grid++)
  {
    if( useSomeGrids && !useThisGrid(grid) )
      continue;  // this grid is in-active
    
    IntegerDistributedArray & equationNumberX = coeff[grid].sparse->equationNumber;
    IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;

    const int *classifyXp = classifyX.Array_Descriptor.Array_View_Pointer3;
    const int classifyXDim0=classifyX.getRawDataSize(0);
    const int classifyXDim1=classifyX.getRawDataSize(1);
    const int classifyXDim2=classifyX.getRawDataSize(2);
#define CLASSIFYX(i0,i1,i2,i3) classifyXp[i0+classifyXDim0*(i1+classifyXDim1*(i2+classifyXDim2*(i3)))]
    const int *equationNumberXp = equationNumberX.Array_Descriptor.Array_View_Pointer3;
    const int equationNumberXDim0=equationNumberX.getRawDataSize(0);
    const int equationNumberXDim1=equationNumberX.getRawDataSize(1);
    const int equationNumberXDim2=equationNumberX.getRawDataSize(2);
#define EQUATIONNUMBERX(i0,i1,i2,i3) \
             equationNumberXp[i0+equationNumberXDim0*(i1+equationNumberXDim1*(i2+equationNumberXDim2*(i3)))]

    if( Oges::debug & 4 || Oges::debug & 32 ) 
    {
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
      cout << " 1=interior, 2=bndry, 3=ghost1, 4=ghost2, -1=interp, -2=periodic, -3=extrap, 0=unused\n";
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
      classifyX.display("*** Oges:generateMatrix: Here is the classify array*** from SparseRep");

      displayCoeff(coeff[grid],"Oges:generateMatrix: Coefficients on input");
    }

    if (TRUE) 
    { // equationType == userSuppliedArray )
      if (coeff[grid].getLength(axis1) < stencilLength*SQR(numberOfComponents)) 
      {
	cout << "Oges:GenerateMatrix:ERROR first dimension of user supplied coeff array"
	  << " is too small " << endl;
	cout << "  ...need at least stencilLength*SQR(numberOfComponents) = " 
	  << stencilLength*SQR(numberOfComponents) << endl;
        cout << "(one extra point is needed for extrapolation and interpolation)\n";
        exit(1);
      }
    }
    
    // Here is where the user has defined extra equations of over-ridden existing equations:
    const bool & userSuppliedEquations = parameters.userSuppliedEquations;
    OgesExtraEquations & extraEquations = dbase.get<OgesExtraEquations>("extraEquations");
    assert( (userSuppliedEquations && extraEquations.neq>0) || (!userSuppliedEquations && extraEquations.neq<=0 ) );
    
    const IntegerArray & eqnExtra =extraEquations.eqn;  // equation numbers for extra user eqn's (not dense)
    // ::display(eqnExtra,"--OGES--GM-- eqnExtra");
    const IntegerArray & iaExtra =extraEquations.ia;
    const IntegerArray & jaExtra =extraEquations.ja;
    const RealArray & aExtra =extraEquations.a;
    int iExtraEquation=0; // counts extra equations

    bool addDenseExtraEquations = (coefficientsOfDenseExtraEquations != NULL &&
				   (*coefficientsOfDenseExtraEquations).numberOfComponentGrids()>0 ) 
                                  && numberOfExtraEquations > 0 ;

    const int ndra=cg[grid].dimension(Start,axis1), ndrb=cg[grid].dimension(End,axis1);
    const int ndsa=cg[grid].dimension(Start,axis2), ndsb=cg[grid].dimension(End,axis2);
    const int ndta=cg[grid].dimension(Start,axis3), ndtb=cg[grid].dimension(End,axis3);
    const int ndr=ndrb-ndra+1;
    const int nds=ndsb-ndsa+1;
    // int ndt=ndtb-ndta+1;

    int eqnOffset=1+ numberOfComponents*(-ndra+ndr*(-ndsa+nds*(-ndta))) +
                  gridEquationBase(grid) + activeGridShift[grid];
    
    int   ieqn0;
    int   extraEquationNumber0 = numberOfExtraEquations>0 ? extraEquationNumber(0) : -1;
    int currentExtraEquation = numberOfExtraEquations-1; // these things work backwards in index through the grid and components


    // currently we only allow one dense equation with possibly multiple components.
    int numberOfDenseExtraEquations=0;
    int currentExtraEquationCoeff = numberOfComponents-1;//currentExtraEquation;
    int numberOfExtraEquationCoeffs = numberOfComponents;  // number of compopen
    if( addDenseExtraEquations )
    {
      numberOfDenseExtraEquations=1;
      numberOfExtraEquationCoeffs = (*coefficientsOfDenseExtraEquations)[grid].getLength(3);
      if( debug & 4 )
	printF("--OGES-- addDenseExtraEquations=%i numberOfDenseExtraEquations=%i, numCoeff=%i \n",
	       (int)addDenseExtraEquations,numberOfDenseExtraEquations,numberOfExtraEquationCoeffs);
      
      // *wdh* 2015/10/12 -- for case when there are dense + extra equations
      if( userSuppliedEquations )
	currentExtraEquation=numberOfExtraEquationCoeffs-1;
      currentExtraEquationCoeff = min(currentExtraEquationCoeff, numberOfExtraEquationCoeffs-1);
    }

    real  cpu1=getCPU();
    Range S(0,stencilDim-1);
    real  coeffn;
    const realArray & rightNull = parameters.compatibilityConstraint ? rightNullVector[grid] 
                                                          : Overture::nullRealMappedGridFunction();
    const real *rightNullp = rightNull.Array_Descriptor.Array_View_Pointer2;
    const int rightNullDim0=rightNull.getRawDataSize(0);
    const int rightNullDim1=rightNull.getRawDataSize(1);
    const int rightNullDim2=rightNull.getRawDataSize(2);
    //#define RIGHTNULL(i0,i1,i2) rightNullp[i0+rightNullDim0*(i1+rightNullDim1*(i2))]
#define RIGHTNULL(i0,i1,i2,n) rightNullp[i0+rightNullDim0*(i1+rightNullDim1*(i2+rightNullDim2*(n)))]

    const realArray & coeffG = coeff[grid];
    const real *coeffGp = coeffG.Array_Descriptor.Array_View_Pointer3;
    const int coeffGDim0=coeffG.getRawDataSize(0);
    const int coeffGDim1=coeffG.getRawDataSize(1);
    const int coeffGDim2=coeffG.getRawDataSize(2);
#define COEFFG(i0,i1,i2,i3) coeffGp[i0+coeffGDim0*(i1+coeffGDim1*(i2+coeffGDim2*(i3)))]
#define COEFF(i,n,I1,I2,I3) COEFFG(i+stencilDim*(n),I1,I2,I3)

    const int *gridEquationBasep = gridEquationBase.getDataPointer();
#define gridEqnBase(grid) gridEquationBasep[grid]

    for( i3=ndta; i3<=ndtb; i3++ ) 
    {
      for( i2=ndsa; i2<=ndsb; i2++ ) 
      {
        for( i1=ndra; i1<=ndrb; i1++ ) 
        {
          // get equations in discrete form

	  int rightNullCoeff = 0;
	  int rightNullEqn = 0;
          //....load the matrix into (ia,ja,a) (Throw away small elements)
          for( n=0; n<numberOfComponents; n++) 
          {
	    ieqn=EQUATION_NO(n,i1,i2,i3);
            if (n==0) ieqn0=ieqn;
            if (ieqn<=0 || ieqn>numberOfEquations) 
            {
              cout << "Oges:generate: ieqn out of range, ieqn=" << ieqn << endl;
            }

            if( isparse==0 ) IA(ieqn)=ii+1;

            if( iExtraEquation<extraEquations.neq && eqnExtra(iExtraEquation)==ieqn )
	    {
              // --- user has defined this equation ---
              // This is either an extra "constraint" equation or an over-riden equation
	      if( false )
		printF("--OGES-- Add user extra equation %i: ieqn=%i (num-extra=%i)\n",
		       iExtraEquation,ieqn,extraEquations.neq);
	      
              for( int kk=iaExtra(iExtraEquation); kk<=iaExtra(iExtraEquation+1)-1; kk++ )
	      {
		ii++;
                if( isparse==1 ) IA(ii)=ieqn;
		JA(ii)=jaExtra(kk);
		A(ii)=aExtra(kk);
	      }
	      iExtraEquation++;
	    }
            else if( CLASSIFYX(i1,i2,i3,n)==SparseRepForMGF::unused ) 
            {
	      // null equation, set to the identity
	      ii++;
	      if( ii > Oges::ndja ) 
	      {
		cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
		generateMatrixError( Oges::nda,ieqn );
		return;
	      }
              if( isparse==1 ) IA(ii)=ieqn;
              if( sparseStorageFormat!=other ) 
              {
                JA(ii)=ieqn;
                A(ii)=1.;
              }
              else 
              {
                equationSolver[parameters.solver]->setMatrixElement(ii,ieqn,ieqn,1.0);
              }
	    }
	    else if( CLASSIFYX(i1,i2,i3,n)<10 ) // *wdh* 010227: don't fill in extra equation, problem if extra is not last
	    {
	      // compute a scale factor; used to throw away small values
	      // *wdh* scale = max(fabs(COEFF(S,n,i1,i2,i3)))*2.*parameters.matrixCutoff;
              scale=0;
              for( int s=0; s<stencilDim; s++)
                scale=max(scale,fabs(COEFF(s,n,i1,i2,i3)));
	      scale*=2.*parameters.matrixCutoff;
	      if (scale==0.) 
	      {
                if( CLASSIFYX(i1,i2,i3,n)<10 ) 
                {// ***** >=10 : extra equation point **** should be classifyX
  		  cout << "Oges:generateMatrix:ERROR matrix has a row with all zero coefficients \n";
                  printf("The offending equation is (i1,i2,i3,n,grid)=(%i,%i,%i,%i,%i)\n",i1,i2,i3,n,grid);
                  printf("...classify(i1,i2,i3,n)=%i\n",CLASSIFYX(i1,i2,i3,n));
		  Overture::abort("Oges:ERROR");
                }
		else 
		{
                  break;
		}
	      }

	      for( i=0; i<stencilDim; i++ ) 
	      {
		//  printf("i1=%i,I2=%i,i3=%i, ieqn=%i, coeff=%e \n",i1,i2,i3,ieqn,COEFF(i,n,i1,i2,i3));
                coeffn = COEFF(i,n,i1,i2,i3);
		if( fabs(coeffn)>scale ) 
		{
                  // ***NOTE*** here we assume jeqn is an equation on "grid"=grid --
                  //   This is usually always the case (except for interpolation equations)
                  //   unless the user plays some funny games.

		  jeqn = EQUATIONNUMBER(i,n,i1,i2,i3);
                  if( useSomeGrids )
		  { 
                    // jeqn may refer to a point on the current grid or another grid (e.g. interpolation equation)
                    int grid2=grid;
                    if( jeqn>=gridEqnBase(grid) && jeqn<=gridEqnBase(grid+1) )
		    {
		      // this equation is on the same grid
                    }
		    else
		    {
                      // this equation does not refer to a point on this grid
                      // find grid2 ... check if it is active, if not, do not include
                      // *** we could do better here for interpolation points ****
                      if( jeqn < gridEqnBase(grid2) )
		      {
                        grid2--;
			while( grid2>=0 && jeqn < gridEqnBase(grid2) )
			  grid2--;
		      }
		      else
		      {
                        grid2++;
			while( grid2<numberOfGrids && jeqn > gridEqnBase(grid2+1) )
			  grid2++;
		      }
                      if( grid2<0 || grid2>=numberOfGrids )
		      {
                        cout << "Oges::generateMatrix:useSomeGrids: error looking for grid2" << endl;
		        generateMatrixError(Oges::nda,ieqn);
		        return;
		      }

                      // printf("grid=%i: ieqn=%i jeqn=%i -> grid2=%i\n",grid,ieqn,jeqn,grid2);
		      
                      if( !useThisGrid(grid2) )
		      {
                        continue;  // skip this coefficient, it is not on an active grid
		      }
		    }
		    jeqn+=activeGridShift[grid2];
		  } // end if useSomeGrids

		  if( jeqn<=0 || jeqn>numberOfEquations ) 
		  {
		    cout << "generateMatrix: jeqn out of range, jeqn=" << jeqn 
		      << " <0 or > numberOfEquations=" << numberOfEquations<< endl;
		    printf(" i1=%i, i2=%i, i3=%i, grid=%i, i=%i, classify=%i "
			   " stencilLength=%i, coeff=%e \n", i1,i2,i3,grid,i,
			   CLASSIFYX(n,i1,i2,i3),stencilLength,COEFF(i,n,i1,i2,i3) );
		  }
		  ii++;
		  if( ii>Oges::ndja ) 
		  {
		    cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
		    generateMatrixError(Oges::nda,ieqn);
		    return;
		  }
                  if( isparse==1 ) IA(ii)=ieqn;
                  if( sparseStorageFormat!=other ) 
                  {
                    JA(ii)=jeqn;
                    A(ii)=coeffn;
                  }
                  else 
                  {
                    equationSolver[parameters.solver]->setMatrixElement(ii,ieqn,jeqn,coeffn);
                  }
		}
	      } // end for i...stencilDim
	      
	      // kkc 090903 generalized the following section for multiple dense constraint equations
	      //            Note that we assume that there are at most numberOfComponents extra equations right now and
	      //            that these contraint equations are like integral constraints, i.e. vector * soln(n) = value.
	      //            The last comment implies that the coefficients must be nonzero... (see the value!=0. thing below)
              if( parameters.compatibilityConstraint && rightNullCoeff<numberOfExtraEquationCoeffs //&&
		  //		  		  rightNullEqn<numberOfExtraEquations  // kkc 100308 added this last bit
		  /*n<numberOfExtraEquations*/ /*kkc 090903 n==0*/  ) //!!! kkc 060731 shouldn't RIGHTNULL have "n" as a subscript!?
              {

                // --------------------------------------------------------------------------------
	        // -- add compatibility constraint as the last entry in the matrix for this row ---
                // --------------------------------------------------------------------------------

                real value = RIGHTNULL(i1,i2,i3,rightNullCoeff); // kkc 090903 RIGHTNULL now has the extra subscript
		//		extraEquationNumber0 = extraEquationNumber(n); // and we need to get the correct extra equation number for this component
		//                if( value != 0. ) 
                if( fabs(value) > 10*REAL_MIN )// != 0. ) 
                {
		  extraEquationNumber0 = extraEquationNumber(rightNullEqn);
		  ii++;
		  if( ii>Oges::ndja ) 
		  {
		    cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
		    generateMatrixError(Oges::nda,ieqn);
		    return;
		  }
                  if( isparse==1 ) IA(ii)=ieqn;
                  if( sparseStorageFormat!=other ) 
                  {
                    JA(ii)=extraEquationNumber0;
                    A(ii)=value;
                  }
                  else 
                  {
                    equationSolver[parameters.solver]->setMatrixElement(ii,ieqn,extraEquationNumber0,value);
                  }
		  rightNullEqn++;
		} // if value!=0.
		rightNullCoeff++;
	      } // if add the right null vector coeffs

            } // end if CLASSIFYX<10
	  } // end for n
	  
	  
          // ------------------------------------------------------------------------------
          // Add in "dense" extra equations such as those equations that define
          // an "integral" type constraint (e.g. setting the mean pressure to zero)
          // *** Fix this -- only works for one dense equation **** FIXED 090903
          // ------------------------------------------------------------------------------

	  //          if (ieqn0==extraEquationNumber0 && addDenseExtraEquations ) 
	  // kkc 060801 !!!!! for coeff functions with more than one component, the extra equation
	  //                  needs to be placed in the last equation otherwise the current sparse matrix
	  //                  formatting gets messed up (or skipped depending on what findExtraEquations does)
	  //                  This issue should probably be fixed when fix the code to allow more than one extra eqn.
	  //                  See findExtraEquations for some more detail.
	  //
	  //                  Also note that the code only seemed to work with one component meaning that eqn0==eqn always.

	  // kkc 090903 fixed to work with up to numberOfComponents dense equations
	  // kkc 090903         if (ieqn==extraEquationNumber0 && addDenseExtraEquations ) 
                                        //this part of the if statement should not be executed when addDenseExtraEquations==false
          if (addDenseExtraEquations && ieqn==extraEquationNumber(currentExtraEquation) )
          {
            // NOTE: coefficientsOfDenseExtraEquations can point to the rightNullVector 


	    extraEquationNumber0 = extraEquationNumber(currentExtraEquation);
            if( debug & 2 ) 
            {
              printF("--OGES-- generate: adding denseExtraEquation currentExtraEquation=%i ieqn=%i ...\n",
		     currentExtraEquation,ieqn);
            }
            real cdc;
            scale=parameters.matrixCutoff;  // *******
            int gridc;	      
	    int nExtraCoeffAdded = 0;
	    bool found = false;
	    while ( !found && currentExtraEquationCoeff>=0 )
	      { 
		for (gridc=0;gridc<(*coefficientsOfDenseExtraEquations).numberOfComponentGrids();gridc++) 
		  {
		    RealDistributedArray & c = (*coefficientsOfDenseExtraEquations)[gridc];
		    const real *cp = c.Array_Descriptor.Array_View_Pointer3;
		    const int cDim0=c.getRawDataSize(0);
		    const int cDim1=c.getRawDataSize(1);
		    const int cDim2=c.getRawDataSize(2);
#define CC(i0,i1,i2,i3) cp[i0+cDim0*(i1+cDim1*(i2+cDim2*(i3)))]

		    int base4=c.getBase(axis3+1);
		    // **** should the nc loop go outside or inside
		    const int c1a=c.getBase(axis1), c1b=c.getBound(axis1);
		    const int c2a=c.getBase(axis2), c2b=c.getBound(axis2);
		    const int c3a=c.getBase(axis3), c3b=c.getBound(axis3);
		    
		    //kkc 090903   for (int nc=c.getBase(axis3+1);nc<=c.getBound(axis3+1);nc++) 
		    int nc = currentExtraEquationCoeff;
		    // if( true )
		    // {  // ********** TEMP **********
		    //   Range all;
		    //   ::display(c(all,all,all,nc),"extra","%6.2f ");
		    // }
		    
		    {
		      for(int i3c=c3a; i3c<=c3b; i3c++ ) 
			{
			  for(int i2c=c2a; i2c<=c2b; i2c++ ) 
			    {
			      for( int i1c=c1a; i1c<=c1b; i1c++ ) 
				{
				  cdc = CC(i1c,i2c,i3c,nc);
				  if( fabs(cdc)>scale ) 
				    {
				      jeqn = equationNo(nc-base4,i1c,i2c,i3c,gridc); 

				      // printF("--OGES-- extra-eqn: nc=%i base4=%i ieqn=%i jeqn=%i cdc=%9.3e\n",nc,base4,ieqn,jeqn,cdc);
				      
				      if (jeqn<0 || jeqn>numberOfEquations) 
					{
					  cout << "generate:2 jeqn out of range, jeqn=" << jeqn << endl;
					}
				      ii++;
				      if( ii>Oges::ndja ) 
					{
					  cout << "Oges::generateMatrix: ...not enough space to store matrix" << endl;
					  generateMatrixError(Oges::nda,ieqn);
					  return;
					}
				      if( isparse==1 ) IA(ii)=ieqn;
				      if( sparseStorageFormat!=other ) 
					{
					  JA(ii)=jeqn;
					  A(ii)=cdc;
					}
				      else 
					{
					  equationSolver[parameters.solver]->setMatrixElement(ii,ieqn,jeqn,cdc);
					}
				      nExtraCoeffAdded++;
				    }
				} // end for i1c
			    }// end for i2c
			}// end for i3c
		    }// end for nc
		  }// end for gridc
		currentExtraEquationCoeff--;
		found = nExtraCoeffAdded>0;
	      } // while !found
	    if ( found ) currentExtraEquation--;
	    addDenseExtraEquations = currentExtraEquation>=0;
	  } // end if add extra eqn
	} // end for i1
      } // end for i2
    } // end for i3
    cpuFill+=getCPU()-cpu1;
  } // end for grid
  numberOfNonzeros=ii;

  if (debug & 2) 
  {
    cout << "generateMatrx: numberOfNonzeros = " << numberOfNonzeros << endl;
  }

  int neqn=equationNo(numberOfComponents-1,
               arrayDims(numberOfGrids-1,End,axis1),
               arrayDims(numberOfGrids-1,End,axis2),
               arrayDims(numberOfGrids-1,End,axis3),numberOfGrids-1);
  if (debug & 2) 
  {
    cout << " generateMatrix: neqn = " << neqn << ", numberOfEquations=" 
         << numberOfEquations << endl;
  }
  if( useSomeGrids )
  {
    neqn=numberOfEquations;
  }

  if (neqn != numberOfEquations) 
  {
    cout << "Oges::generateMatrix:ERROR numberOfEquations,neqn,numberOfComponents =" << numberOfEquations 
         << ", "  << neqn << ", " << numberOfComponents << endl;
    Overture::abort("Error");
  }
  if (isparse==0) IA(neqn+1)=ii+1;
  
  if( parameters.solveForTranspose && 
      (parameters.solver==OgesParameters::SLAP || parameters.solver==OgesParameters::PETSc)) 
  {
    //    When using some iterative solvers like SLAP or ESSL routines we have to form the transpose
    //    of the matrix when explicitly asked 
    //    ...transpose matrix
    if (Oges::debug & 4) cout << "Oges::generateMatrix: transposing matrix for SLAP or PETSc" << endl;
    for (i=1;i<=numberOfNonzeros;i++) 
    {
      int itmp=IA(i);
      IA(i)=JA(i);
      JA(i)=itmp;
    }
    if( sparseStorageFormat==compressedRow )
    {
      //  ...Convert to compressed ia() use SLAP routine which
      //     converts to column format (note we have switched the
      //     roles of ia and ja so actually we are converting to
      //     compressed row format)
      // sort new ia
      if( debug & 1 ) cout << "Oges::generateMatrix: transpose case: compress the ia array...\n";
      int isym=0;
      SS2Y( numberOfEquations,numberOfNonzeros,JA(1),IA(1),A(1),isym );
    }
    
  }
  

  if( Oges::debug & 32) 
  {
    if (sparseStorageFormat==compressedRow) 
    { // print out the matrix
      if (debug & 2) cout << "generateMatrix: After loading matrix:" << endl;
      for (i=1;i<=neqn;i++) 
      {
        printf("Row i=%5i",i);
        for( int j=IA(i); j<=IA(i+1)-1; j++ )
          printf("(%5i,%8.2f) ",JA(j),A(j));
        printf("\n");
      }
    }
    else if (sparseStorageFormat!=other) 
    {
      if( parameters.solveForTranspose )
      {
	// sort new ia
	int kflag=1;
	QS2I1( IA(1), JA(1), A(1), numberOfNonzeros, kflag );
      }

      int j=1;
      for (i=1;i<=neqn;i++) 
      {
        printf("Row i=%5i",i);
        while (IA(j)==i && j<=numberOfNonzeros) {
          printf(" (%5i,%8.2f) ",JA(j),A(j));
          j++;
        }
        printf("\n");
      }
    }
    else 
    {
      equationSolver[parameters.solver]->displayMatrix();
    }      
  }
  
  delete [] activeGridShift;
  
  if (Oges::debug & 2) 
  {
    printf("Oges::time for fill=%e, total generateMatrix = %e \n",cpuFill,getCPU()-cpu0);
  }
}

//\begin{>>OgesInclude.tex}{\subsection{getErrorMessage}} 
aString Oges::
getErrorMessage(const int errorNumber)
//=====================================================================================
// /Purpose: Get the error message corresponding to the Oges error number.
// /errorNumber (input): Error number returned by solve.
// /Return Values: aString containing the error message.
//\end{OgesInclude.tex}
//==============================================================
{
  char buf[90];
  strcpy(buf,"00000000000000000000000000000000000000000000000000000000000");
  CGESER(buf,errorNumber,strlen(buf));
  return buf;
}  

