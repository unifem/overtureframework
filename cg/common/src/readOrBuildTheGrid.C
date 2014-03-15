#include "CgSolverUtil.h"

#include "GenericGraphicsInterface.h"
#include "CompositeGrid.h"
#include "MappedGrid.h"
#include "GenericMappedGridOperators.h"
#include "MappingInformation.h"

int ogen(MappingInformation & mappingInfo, GenericGraphicsInterface & ps, const aString & commandFileName,CompositeGrid *cgp=0 );

// ======================================================================================================
/// \brief : Read a CompositeGrid from a data base file or create a CompositeGrid with ogen
/// \param ps : graphics object to use
/// \param cg : create this grid.
/// \param loadBalance : set to true to load balance (default)
/// \return value: the name of the grid file (or an empty string if the grid was built interatively)
// ======================================================================================================

aString
readOrBuildTheGrid(GenericGraphicsInterface &ps, CompositeGrid &cg, bool loadBalance /* = false */,
                   int numberOfParallelGhost /* =2 */, int maxWidthExtrapInterpNeighbours /* =3 */ )
{
  aString nameOfOGFile;
  ps.inputString(nameOfOGFile,"Enter the name of an (old) overlapping grid file or type 'ogen' to generate a grid:");

  // create and read in a CompositeGrid
  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    // const int numGhost=2;
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numberOfParallelGhost);


  #endif
  // Reduce the width of extrapolation for interp neighbours to 3 
  // and increase parallel ghost boundary width to 2 -- this will mean that we should
  // get the same result independent of np
  // NOTE: max-width=3 means 2nd-order extrapolation:  1 -2 1 
  GenericMappedGridOperators::setDefaultMaximumWidthForExtrapolateInterpolationNeighbours(maxWidthExtrapInterpNeighbours);


  MappingInformation mappingInfo;
  if ( nameOfOGFile != "ogen" ) 
  {
    printF("readOrBuildTheGrid:Try to read the overlapping grid file : %s\n",(const char*)nameOfOGFile);
  
    int rt = getFromADataBase(cg,nameOfOGFile,loadBalance);
    if( rt!=0 )
    {
      printF("readOrBuildTheGrid:ERROR return from getFromADataBase\n");
      OV_ABORT("error");
    }
    
    if( Communication_Manager::Number_Of_Processors >1 )
    { // display the parallel distribution
      cg.displayDistribution("readOrBuildTheGrid",stdout);
    }

  }
  else
  {
    printF("Calling ogen to generate the grid...\n");
    // create more mappings and/or make an overlapping grid
    ogen( mappingInfo,ps,"--", &cg);
  }

  return nameOfOGFile;
}
    
