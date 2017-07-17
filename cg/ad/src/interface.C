// This file automatically generated from interface.bC with bpp.
#include "Cgad.h"
#include "AdParameters.h"
#include "Interface.h" 



// ===================================================================================
/// \brief Return the interface data required for a given type of interface.
/// \param info (input) : the descriptor for the interface.
/// \param interfaceDataOptions (output) : a list of items from Parameters::InterfaceDataEnum that define
///                    which data to get (or which data were set).  Multiple items are
///                     chosen by bit-wise or of the different options
/// \note: this function should be over-loaded.
// ===================================================================================
int Cgad::
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const
{
    int numberOfItems=0;
    const int numberOfDimensions = parameters.dbase.get<int>("numberOfDimensions");

    const int grid=info.grid, side=info.side, axis=info.axis;

    IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
    if( grid<0 || grid>interfaceType.getBound(2) ||
            side<0 || side>1 || axis<0 || axis>interfaceType.getBound(1) )
    {
        printP("Cgad::getInterfaceDataOptions:ERROR: invalid values: (grid,side,axis)=(%i,%i,%i)\n",
         	   grid,side,axis);
        OV_ABORT("Cgad::getInterfaceDataOptions:ERROR");
    }
    if( interfaceType(side,axis,grid)==Parameters::heatFluxInterface )
    {
        interfaceDataOptions=Parameters::heatFluxInterfaceData;
        numberOfItems+=1;
    }
    else if( interfaceType(side,axis,grid)==Parameters::tractionInterface ) 
    {
        printP("Cgad::getInterfaceDataOptions:ERROR: not expecting a tractionInterface!\n");
        OV_ABORT("Cgad::getInterfaceDataOptions:ERROR");
    }
    else
    {
        printP("Cgad::getInterfaceDataOptions:ERROR: interfaceType(grid=%i,side=%i,axis=%i)=%i\n",
         	   grid,side,axis,interfaceType(side,axis,grid));
        OV_ABORT("Cgad::getInterfaceDataOptions:ERROR");
    }
    return numberOfItems;
}


// =================================================================================================
/// \brief Set or get the right-hand-side for an interface boundary condition.
/// \details This function is used when solving the interface equations 
///           by iteration.
/// \param option (input) : option=getInterfaceRightHandSide : get the RHS, 
///                         option=setInterfaceRightHandSide : set the RHS
/// \param interfaceDataOptions (input) : a list of items from Parameters::InterfaceDataEnum that define
////                    which data to get (or which data were set).  Multiple items are
///                     chosen by bit-wise or of the different options   
/// \param info (input) : contains the GridFaceDescriptor info used to set the right-hand-side.
/// \param gfd (input) : the master GridFaceDescriptor. 
/// \param gfIndex (input) : use the solution from gf[gfIndex]
/// \param t (input) : current time.
/// \param saveTimeHistory (input) : if true, save a time-history of the requested data. This is the
///    new way to save a time-history when interfaceCommunicationMode==requestInterfaceDataWhenNeeded
// ===================================================================================================
int
Cgad::
interfaceRightHandSide( InterfaceOptionsEnum option, 
                                                int interfaceDataOptions,
                                                GridFaceDescriptor & info, 
                                                GridFaceDescriptor & gfd,
                  			int gfIndex, real t,
                                                bool saveTimeHistory /* = false */ )
{
    return DomainSolver::interfaceRightHandSide(option,interfaceDataOptions,info,gfd,gfIndex,t);
}
