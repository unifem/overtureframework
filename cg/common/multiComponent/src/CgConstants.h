#ifndef __CG_CONSTANTS_H__
#define __CG_CONSTANTS_H__

namespace CG {

  // "non-dimensional" constants
  const double avogadro = 6.02214179e23; // from wikipedia/CODATA

  // meters-kilograms-seconds
  namespace MKS {
    const double boltzmann = 1.3806504e-23; // kg m^2 / K s^2, also from wikipedia/CODATA
    const double gas_constant = CG::avogadro * MKS::boltzmann;
    const double angstrom = 1e-10;
  }

  // centimeters-grams-seconds
  namespace CGS {
    const double boltzmann = MKS::boltzmann * 1e7; // g cm^2/ K s^2 
    const double gas_constant = CG::avogadro * CGS::boltzmann;
    const double angstrom = 1e-8;
  }
}

#endif
