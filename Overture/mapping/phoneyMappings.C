// ============================================================================================================
//   Here are phoney versions of some Mapping files that we will link to when we only build the
//   the OvertureMapping library

// ============================================================================================================

#include "Mapping.h"

void PlotIt::
plotMappingQuality(GenericGraphicsInterface &gi, Mapping & map,
		   GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */ )
{
  printf("Sorry -- plotMappingQuality is not available without the rest of Overture\n");
}

// --------------------------------------------------------------------------------------------------------
// Null version of Elliptic Transform
// --------------------------------------------------------------------------------------------------------

#include "EllipticTransform.h"

EllipticTransform::
EllipticTransform() 
: Mapping(2,2,parameterSpace,cartesianSpace) 
{ 
}

EllipticTransform::
EllipticTransform( const EllipticTransform& map, const CopyType copyType )
{
}

EllipticTransform::
~EllipticTransform()
{ 
}

EllipticTransform & EllipticTransform::
operator=( const EllipticTransform & X )
{
  return *this;
}

int EllipticTransform::
get( const GenericDataBase & dir, const aString & name)
{
  return 0;
}

int EllipticTransform::
put( GenericDataBase & dir, const aString & name) const
{  
  return 0;
}


Mapping *EllipticTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  return this;
}


void EllipticTransform::
map(const realArray & r, 
    realArray & x, 
    realArray & xr /* = Overture::nullRealDistributedArray() */,
    MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
}

void EllipticTransform::
inverseMap(const realArray & x, 
	   realArray & r, 
	   realArray & rx /* = Overture::nullRealDistributedArray() */,
	   MappingParameters & params /* =Overture::nullMappingParameters() */ )
{
}

int EllipticTransform::
setup()
{
  return 0;
}


void EllipticTransform::
generateGrid(GenericGraphicsInterface *gi /* = NULL */, 
             GraphicsParameters & parameters /* =Overture::nullMappingParameters() */ )
{

}
	
void EllipticTransform::
resetDataPointMapping( realArray & x, Index Ig, Index Jg, Index Kg )
{
}


void EllipticTransform::
initialize()
{

}


int EllipticTransform::
update( MappingInformation & mapInfo ) 
{
  return 0;
}


// ============================================================================================================

// null version of the EllipticGridGenerator

#include "EllipticGridGenerator.h"

EllipticGridGenerator::
EllipticGridGenerator()
{
}

int EllipticGridGenerator::
initializeParameters()
{
  return 0;
}


// hopefully this is not needed
// const RealMappedGridFunction & EllipticGridGenerator::
// solution() const
// {
//   RealMappedGridFunction a;
//   return a;
// }

  

void EllipticGridGenerator::
setup(Mapping & mapToUse, 
      Mapping *projectionMappingToUse /* =NULL */ )
{

}

int EllipticGridGenerator::    
updateForNewBoundaryConditions()
{
  return 0;
}


EllipticGridGenerator::
~EllipticGridGenerator()
{
}

realArray EllipticGridGenerator::
dot( const realArray & a, 
     const realArray & b, 
     const Index & I1 /* =nullIndex */, 
     const Index & I2 /* =nullIndex */, 
     const Index & I3 /* =nullIndex */ )
{
  return a;
}


int EllipticGridGenerator::
restrictMovement( const int & level,
		  const RealMappedGridFunction & u0, 
		  RealMappedGridFunction & u1,
		  const Index & I1_ /* =nullIndex */, 
		  const Index & I2_ /* =nullIndex */, 
		  const Index & I3_ /* =nullIndex */)               
{
  return 0;
}


realArray EllipticGridGenerator::
signOf( const realArray & uarray)
{
  realArray u1;

  return u1;
}



int EllipticGridGenerator::
plot( const RealMappedGridFunction & v, const aString & label )
{
  return 0;
}


int EllipticGridGenerator::
projectBoundaryPoints(const int & level,
		      RealMappedGridFunction & uu, 
		      const int & side,  
		      const int & axis,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3 )
{
  return 0;
}


int EllipticGridGenerator::
applyBoundaryConditions(const int & level,
                      RealMappedGridFunction & uu )
{
  return 0;
}


int EllipticGridGenerator::
stretchTheGrid(Mapping & mapToStretch)
{
  return 0;
}

int EllipticGridGenerator::
determineBoundarySpacing(const int & side, 
                         const int & axis,
                         real & averageSpacing,
                         real & minimumSpacing,
                         real & maximumSpacing )
{
  return 0;
}


int EllipticGridGenerator::
redBlack(const int & level, 
       RealMappedGridFunction & uu )
{ 
  return 0;
}


int EllipticGridGenerator::
jacobi(const int & level, 
       RealMappedGridFunction & uu )
{ 
  return 0;
}

int EllipticGridGenerator::
lineSmoother(const int & direction,
             const int & level,
             RealMappedGridFunction & uu )
{
  return 0;
}



int EllipticGridGenerator::
smooth(const int & level, 
       const SmoothingTypes & smoothingType,
       const int & numberOfSubIterations /* =1 */ )
// ====================================================================
// /Access: {\bf Protected}.
// /Description:
//   Handles the different smoothing methods.
//\end{EllipticGridGeneratorInclude.tex}
// ====================================================================

{
  return 0;
}



int EllipticGridGenerator::
fineToCoarse(const int & level, 
	     const RealMappedGridFunction & uFine, 
	     RealMappedGridFunction & uCoarse,
             const bool & isAGridFunction /* = FALSE */ )
{
  return 0;
}



int EllipticGridGenerator::
coarseToFine(const int & level,  
	     const RealMappedGridFunction & uCoarse, 
	     RealMappedGridFunction & uFineGF,
             const bool & isAGridFunction /* = FALSE */  )
{
  return 0;
}



int EllipticGridGenerator::
multigridVcycle(const int & level )
{
  return 0;
}
  

int EllipticGridGenerator::
generateGrid()
{
  return 0;
}

  
int EllipticGridGenerator::
update(DataPointMapping & dpm,
       GenericGraphicsInterface *gi_ /* = NULL */, 
       GraphicsParameters & parameters /* =Overture::nullMappingParameters() */ )
{
  return 0;
  
}





int EllipticGridGenerator:: 
startingGrid(const realArray & u0, 
             const realArray & r0 /* = Overture::nullRealDistributedArray() */,
             const IntegerArray & gridIndexBounds /* =Overture::nullIntArray() */ )
{
  return 0;
}



void EllipticGridGenerator::
getResidual(realArray & resid1, 
            const int & level )
{
}

void EllipticGridGenerator::
getResidual(realArray & resid1, 
            const int & level,
            Index Jv[3],
            realArray & coeff,
            const bool & computeCoefficients /* =TRUE */,
            const bool & includeRightHandSide /* = TRUE */,
            const bool & computeControlFunctions /* = TRUE */,
            const SmoothingTypes & lineSmoothType /* =jacobiSmooth */ )
{
}



void EllipticGridGenerator::
updateRightHandSideWithFASCorrection(int i )
{
}

int EllipticGridGenerator::
getCoefficients(realArray & coeff, 
		const Index & J1, 
		const Index & J2, 
		const Index & J3,
                const realArray & ur, 
                const realArray & us,
                const realArray & ut /* = Overture::nullRealDistributedArray() */ )
{
  return 0;
}

int EllipticGridGenerator::
estimateUnderRelaxationCoefficients()
{
  return 0;
}



int EllipticGridGenerator::
getControlFunctions(const int & level )
{
  return 0;
}

int EllipticGridGenerator::
defineBoundaryControlFunction()
{
  return 0;
}

int EllipticGridGenerator::
smoothJacobi( RealMappedGridFunction & v,  const int & numberOfSmooths /* = 4 */ )
{    
  return 0;
}


int EllipticGridGenerator::
defineSurfaceControlFunction()
{

  return 0;
}

int EllipticGridGenerator::
weightFunction( RealMappedGridFunction & weight_ )
{
  return 0;
}



int EllipticGridGenerator::
periodicUpdate(realMappedGridFunction & x, 
	       const Range & C /* =nullRange */,
	       const bool & isAGridFunction /* = TRUE */ )
{
  return 0;
}

#include "Mapping.h"
#include "MappingInformation.h"


int
readMappingsFromAnOverlappingGridFile( MappingInformation & mapInfo, const aString & ogFileName=nullString )
{
  return 0;
}

// Copied from ParallelUtility.C
// #include "broadCast.h"
// void 
// broadCast( int & value, const int & fromProcessor )
// {
//   if( Communication_Manager::Number_Of_Processors==0 )
//     return;

//   // broadcast the length to all processors
//   Partitioning_Type P(Range(fromProcessor,fromProcessor));
//   intArray buff(1,P);
//   buff(0)=value;
//   value=buff(0);

// }
// void 
// broadCast( float & value, const int & fromProcessor )
// {
// #ifdef USE_PPP 
//   if( Communication_Manager::Number_Of_Processors==0 )
//     return;

//   // *wdh* new way, 060524
//   MPI_Bcast( &value, 1, MPI_FLOAT, fromProcessor, MPI_COMM_WORLD); 

// #endif  
// }
// void 
// broadCast( double & value, const int & fromProcessor )
// {
// #ifdef USE_PPP 
//   if( Communication_Manager::Number_Of_Processors==0 )
//     return;

//   // *wdh* new way, 060524
//   MPI_Bcast( &value, 1, MPI_DOUBLE, fromProcessor, MPI_COMM_WORLD); 

// #endif  
// }
// void 
// broadCast( bool & value, const int & fromProcessor )
// {
// #ifdef USE_PPP 
//   if( Communication_Manager::Number_Of_Processors==0 )
//     return;

//   // *wdh* new way, 060524
//   MPI_Bcast( &value, 1, MPI_LOGICAL, fromProcessor, MPI_COMM_WORLD); 

// #endif  
// }

// void
// broadCast( intSerialArray & buff, const int & fromProcessor )
// // Broadcast a serial array to all processors
// {
//   if( Communication_Manager::Number_Of_Processors==0 )
//     return;

//   // broadcast the length to all processors
//   int length=buff.getLength(0);
//   broadCast(length,fromProcessor);
  
//   Partitioning_Type P(Range(fromProcessor,fromProcessor));
//   intArray distBuff(length,P);

//   int b=buff.getBase(0);
//   int i;
//   for( i=buff.getBase(0); i<=buff.getBound(0); i++ )
//     distBuff(i-b)=buff(i)  ;

//   for( i=buff.getBase(0); i<=buff.getBound(0); i++ )
//     buff(i)=distBuff(i-b)  ;

// }

// void
// broadCast( aString & string, const int & fromProcessor )
// // Broadcast a aString to all processors
// {
//   if( Communication_Manager::Number_Of_Processors==0 )
//     return;

//   // broadcast the length to all processors
//   int length=string.length();
//   broadCast(length,fromProcessor);

//   // copy the string to a serial array, broadcast, and copy back

//   intSerialArray buff(length+1);                // make same size on all processors
//   buff=0;
//   if( Communication_Manager::localProcessNumber()==fromProcessor )
//   {
//     for( int i=0; i<length; i++ )
//       buff(i)=string[i];
//   }
//   broadCast( buff, fromProcessor );
  
//   if( Communication_Manager::localProcessNumber()!=fromProcessor )
//   {
//     char *s = new char [length+1];
//     for( int i=0; i<length+1; i++ )
//       s[i]=(char)buff(i);

//     string=s;
//     delete s;
//   }
// }


// doubleSerialArray &
// getLocalArrayWithGhostBoundaries( const doubleArray & u, doubleSerialArray & uLocal )
// {
//   Overture::abort("error");
//   return uLocal;
// }

// floatSerialArray &
// getLocalArrayWithGhostBoundaries( const floatArray & u, floatSerialArray & uLocal )
// {
//   Overture::abort("error");
//   return uLocal;
// }

// intSerialArray &
// getLocalArrayWithGhostBoundaries( const intArray & u, intSerialArray & uLocal )
// {
//   Overture::abort("error");
//   return uLocal;
// }


