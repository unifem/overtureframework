#ifndef ADAMS_PC_DATA_H
#define ADAMS_PC_DATA_H

#include "OvertureTypes.h"

// =========================================================================
// Ths class holds state data for the adams predictor-corrector methods
// =========================================================================


class AdamsPCData 
{
public:
AdamsPCData();
~AdamsPCData();

AdamsPCData(const AdamsPCData & x);
AdamsPCData& operator=(const AdamsPCData & x);

real dtb;
int mab0,mab1,mab2;
int nab0,nab1,nab2,nab3;
int ndt0;
real dtp[5]; 
};
 


#endif
