#include "Oges.h"

#define ForAllGridPoints( i1,i2,i3 ) \
  for( i3=c.dimension()(Start,axis3); i3<=c.dimension()(End,axis3); i3++ ) \
  for( i2=c.dimension()(Start,axis2); i2<=c.dimension()(End,axis2); i2++ ) \
  for( i1=c.dimension()(Start,axis1); i1<=c.dimension()(End,axis1); i1++ )


void Oges::
assignDiscreteCoefficients( const int grid )
{
//========================================================================
//   Define the Matrix coeff that defines the discrete equations to
//  be solved on componentGrid c.
//
//  
// Output -
//   coeff(i,I1,I2,I3)   (RealArray)
//
//   ( should be (if A++ supported 5D arrays)
//      coeff(m,n,I1,I2,I3) m=0,..,stencilLength-1; n=0,...,numberOfComponents-1 )
//
//       : array to holding the coefficients. The coefficients for equation "n"
//         are stored in 
//              coeff(MN(m,n),I1,I2,I3) m=0,...,stencilLength-1
//        where 
//            n=0,...,numberOfComponents-1
//            MN(m,n)==m+stencilLength*(n)
//            I1,I2,I3 are as big as the component grid arrays
//   equationNumber(i,I1,I2,I3,m)   (IntegerArray)
//       : equation number for the corresponding coefficient, use function
//         eqn[grid](i1,i2,i3,n) which gives the equation number for each point
//
//    stencilLength : length of the largest stencil AND defining size for MN(m,n)
//      
//========================================================================

   printObsoleteMessage("assignDiscreteCoefficients"); 
//  int conservationForm=TRUE;

  if( ! cg[grid].isCellCentered()(0) )
  {
    // ----------Vertex Centred-----------
    switch (equationType)
    {
    case userSuppliedArray:
      userSuppliedCoefficients( grid );
      break;
    case LaplaceDirichlet:
      laplaceDirichletVertexNonConservative( grid );
      break;
    case LaplaceNeumann:
      laplaceNeumannVertexNonConservative( grid );
      break;
    case LaplaceMixed:
      laplaceMixedVertexNonConservative( grid );
      break;
    case Biharmonic:
      biharmonicDirichletVertexNonConservative( grid );
      break;
    case Interpolation:
      implicitInterpolation( grid );
      break;
    default:
      cerr << "assignDiscreteCoefficients::ERROR unknown equation type = " << equationType << endl;
      exit(1);
    }
  }
  else
  {
    // ------------Cell Centred----------
    
    switch (equationType)
    {
    case userSuppliedArray:
      userSuppliedCoefficients( grid );
      break;
    case LaplaceDirichlet:
      laplaceDirichletCellConservative( grid );
      break;
    case LaplaceNeumann:
      laplaceNeumannCellConservative( grid );
      break;
    case Interpolation:
      implicitInterpolation( grid );
      break;
    default:
      cerr << "assignDiscreteCoefficients::ERROR unknown equation type = " << equationType << endl;
      exit(1);
    }
  }
  
  
  if( Oges::debug & 64 )
  {
    int i1,i2,i3;
    cout << "*** assignDiscreteCoefficients: Here is the matrix ***" << endl;
    MappedGrid & c = cg[grid];
    ForAllGridPoints( i1,i2,i3 ) 
      printf("i1=%i, i2=%i, "
             "coeff = %6.2f %6.2f %6.2f %6.2f %6.2f %6.2f "
             "%6.2f %6.2f %6.2f %6.2f \n"
             ,i1,i2,
             coeff[grid](0,i1,i2,i3),
             coeff[grid](1,i1,i2,i3),
             coeff[grid](2,i1,i2,i3),
             coeff[grid](3,i1,i2,i3),
             coeff[grid](4,i1,i2,i3),
             coeff[grid](5,i1,i2,i3),
             coeff[grid](6,i1,i2,i3),
             coeff[grid](7,i1,i2,i3),
             coeff[grid](8,i1,i2,i3),
             coeff[grid](9,i1,i2,i3)
                         );
  }
  
}

