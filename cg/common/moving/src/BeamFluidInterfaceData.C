#include "Overture.h"
#include "BeamFluidInterfaceData.h"


// ===================================================================================
/// \brief: Constructor for the class that holds the beam-fluid interface data used by the AMP algorithm
// ===================================================================================
BeamFluidInterfaceData::
BeamFluidInterfaceData()
{
}

// ===================================================================================
/// \brief: destructor of the class that holds the beam-fluid interface data used by the AMP algorithm
// ===================================================================================
BeamFluidInterfaceData::
~BeamFluidInterfaceData()
{
  // delete interfaceData for 2d case
  if( dbase.has_key("s0Array") )
  {
   delete [] dbase.get<RealArray*>("s0Array");
   delete [] dbase.get<RealArray*>("signedDistanceArray");
   delete [] dbase.get<IntegerArray*>("elementNumberArray");
   delete [] dbase.get<IntegerArray*>("donorInfoArray");
  }

  // Longfei 20170202: for pinned and clamped bc, we need adjust the physical beam ends
  if(dbase.has_key("physicalBeamEnds"))
    delete [] dbase.get<vector<IntegerArray>*>("physicalBeamEnds");

  // delete interfaceData for 3d case

}

