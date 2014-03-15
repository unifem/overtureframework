#include <cmath>

#include "kk_ptr.hh"
#include "CgTypes.h"
#include "CgInteractionPotential.h"

using namespace std;
using namespace CG;

//
// STATIC DEFINITIONS
//
std::string CG::LennardJonesPotential::name = "Lennard-Jones";


real_t 
CG::
LennardJonesPotential::
OmegaIntegral(const int &l, const int &s, real_t Ts) const
{
  // We compute the collision integrals for for the Lennard-Jones potential from
  // a curve fit provided by:
  // Neufeld, Philip D., Janzen, A.R. and Aziz, R.A., "Emperical Equations to Calculate 16 of the
  // "Transport Collision Integrals \Omega^{(l,s)*} for the Lennard-Jones (12-6) Potential", The Journal 
  // of Chemical Physics, Volume 57, Number 3, August 1972.
  // 
  // Note that these formulae are more accurate than the tables in Hirschfelder, Curtiss and Bird.
  //
  // The general equation is:
  // \Omega^{(l,s)*}(T^*) = (A/(T^{*B})) + [C/exp(DT^*)] + [E/exp(FT^*)] + [G/exp(HT^*) + RT^{*B}sin(ST^{*W}-P)
  // where A-W are parameters given in the paper and T^* is the reduced temperature:
  // T^* = k T / eps
  // where k=Boltzmann's constant, and eps is the well depth (maximum attractive energy in the LJ potential)

  // jlf helped to implement the table in this function

  enum {
    A = 0,
    B,
    C,
    D, 
    E, 
    F,
    G,
    H,
    R,
    S,
    W,
    P, // yeah, P is here in the Neufeld et al.'s parameter table
    nParameters };

  const int i=l-1;
  const int j=s-1;
  // The array pls holds the coefficients for the above formula.  Note that the array is 4x7 even though we only have data for
  // 16 pairs of l,s .  We suck up the (little) extra storage to make the implementation a little more clean.  In the comments to
  // the left of each line a * means that the entry is a "real" one, i.e. in the original paper.
  //                                                  A        B        C         D       E        F         G       H      Rx10^4     S        W        P
  static const real_t pls[][7][nParameters] = { { {1.06036, 0.15610, 0.19300, 0.47635, 1.03587, 1.52996, 1.76474, 3.89411, 0.00000, 0.00000, 0.00000, 0.00000},  // 1,1 *
						  {1.00220, 0.15530, 0.16105, 0.72751, 0.86125, 2.06848, 1.95162, 4.84492, 0.00000, 0.00000, 0.00000, 0.00000},  // 1,2 *
						  {0.96573, 0.15611, 0.44067, 1.52420, 2.38981, 5.08063, 0.00000, 0.00000,-5.37300,19.28660,-1.30775, 6.58711},  // 1,3 *
						  {0.93477, 0.15578, 0.39478, 1.85761, 2.45988, 6.15727, 0.00000, 0.00000, 4.24600,12.98800,-1.36399, 3.33290},  // 1,4 *
						  {0.90972, 0.15565, 0.35967, 2.18528, 2.45169, 7.17936, 0.00000, 0.00000,-3.81400, 9.38191, 0.14025, 9.93802},  // 1,5 *
						  {0.88928, 0.15562, 0.33305, 2.51303, 2.36298, 8.11690, 0.00000, 0.00000,-4.64900, 9.86928, 0.12851, 9.82414},  // 1,6 *
						  {0.87208, 0.15568, 0.36583, 3.01399, 2.70659, 9.92310, 0.00000, 0.00000,-4.90200,10.22740, 0.12306, 9.97712}}, // 1,7 *
						{ {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 2,1
						  {1.16145, 0.14874, 0.52487, 0.77320, 2.16178, 2.43787, 0.00000, 0.00000,-6.43500,18.03230,-0.76830, 7.27371},  // 2,2 *
						  {1.11521, 0.14796, 0.44844, 0.99548, 2.30009, 3.06031, 0.00000, 0.00000, 4.56500,38.58680,-0.69403, 2.56375},  // 2,3 *
						  {1.08228, 0.14807, 0.47128, 1.31596, 2.42738, 3.90018, 0.00000, 0.00000,-5.62300, 3.08449, 0.28271, 3.22871},  // 2,4 *
						  {1.05581, 0.14822, 0.51203, 1.67007, 2.57317, 4.85939, 0.00000, 0.00000,-7.12000, 4.71210, 0.21730, 4.73530},  // 2,5 *
						  {1.03358, 0.14834, 0.53928, 2.01942, 2.72350, 5.84817, 0.00000, 0.00000,-8.57600, 7.66012, 0.15493, 7.60110},  // 2,6 *
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000}}, // 2,7
						{ {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 3,1
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 3,2
						  {1.05567, 0.14980, 0.30887, 0.86437, 1.35766, 2.44123, 1.29030, 5.55734, 2.33900,57.77570,-1.08980, 6.94750},  // 3,3 *
						  {1.02621, 0.15050, 0.55381, 1.40070, 2.06176, 4.26234, 0.00000, 0.00000, 5.22700,11.33310,-0.82090, 3.87185},  // 3,4 *
						  {0.99958, 0.15029, 0.50441, 1.64304, 2.06947, 4.87712, 0.00000, 0.00000,-5.18400, 3.45031, 0.26821, 3.73348},  // 3,5 *
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 3,6
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000}}, // 3,7
						{ {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 4,1
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 4,2
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 4,3
						  {1.12007, 0.14578, 0.53347, 1.11986, 2.28803, 3.27567, 0.00000, 0.00000, 7.42700,21.04800,-0.28759, 6.69149},  // 4,4 *
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 4,5
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000},  // 4,6
						  {0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000, 0.00000}}  // 4,7
  };
	
  //  const Ts = CG::MKS::boltzmann * T / well_depth;
  const real_t *p = pls[i][j];

  return
  // (A/(T^{*B}))     + [C/exp(DT^*)]     + [E/exp(FT^*)]     + [G/exp(HT^*)      + RT^{*B}sin(ST^{*W}-P)
    p[A]/pow(Ts,p[B]) + p[C]/exp(p[D]*Ts) + p[E]/exp(p[F]*Ts) + p[G]/exp(p[H]*Ts) + (p[R]*1e-4)*pow(Ts,p[B])*sin(p[S]*pow(Ts,p[W])-p[P]);
}

#ifdef TEST_MAIN

int main(int argc, char *argv[])
{
  int n_errors = 0;

  CG::LennardJonesPotential LJ_potential;

  size_t n_Ts = 6; // must match the number of entries in Ts[]
  real_t Ts[] =       {  0.5,   1.0,   2.0,    4.5,   16.0,  100.0}; // reduced temperatures at which to check the collision integrals
  real_t Omega_11[] = {2.066, 1.440, 1.075, 0.8617, 0.6878, 0.5180}; // data from Table E.2 (column 2) in Bird, Stewart and Lightfoot
  real_t Omega_22[] = {2.284, 1.593, 1.176, 0.9462, 0.7683, 0.5887}; // data from Table E.2 (column 1) in Bird, Stewart and Lightfoot 

  for ( int i=0; i<n_Ts; i++ )
    {
      real_t T_s = Ts[i];
      real_t O11 = LJ_potential.OmegaIntegral(1,1,T_s);
      real_t O22 = LJ_potential.OmegaIntegral(2,2,T_s);

      real_t diff11 = O11-Omega_11[i];
      real_t diff22 = O22-Omega_22[i];

      if ( fabs(diff11/Omega_11[i])>0.0025 ) // this precision should be taken from table II in Neufeld, Janzen and Aziz which would give 0.0011, but the value for O11 varies from table to table and apparently and it is not clear what table to compare the interpolant to
	{
	  cout<<"ERROR : Omega_11 failed test for temperature "<<i<<", Ts="<<T_s<<", Omega_11="<<Omega_11[i]<<", O11="<<O11<<", err="<<fabs(diff11/Omega_11[i])<<endl;
	  n_errors++;
	}
      if ( fabs(diff22/Omega_22[i])>0.0045 ) // this precision should be taken from table II in Neufeld, Janzen and Aziz which would give 0.0016, but the value for O22 varies from table to table and apparently and it is not clear what table to compare the interpolant to
	{
	  cout<<"ERROR : Omega_22 failed test for temperature "<<i<<", Ts="<<T_s<<", Omega_22="<<Omega_22[i]<<", O22="<<O22<<", err="<<fabs(diff22/Omega_22[i])<<endl;
	  n_errors++;
	}
    }

  if ( !n_errors ) cout<<"ALL TESTS PASSED"<<endl;

  return n_errors;
}

#endif
