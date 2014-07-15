#include <cmath>
#include "MultiComponent.h"
#include "CgConstants.h"

using namespace std;
using namespace CG;

//
// CLASS METHOD DEFINITIONS
//

CG::
Mixture::
Mixture()
{
  mat_list.reserve(10);
}

CG::
Mixture::
~Mixture()
{
  mat_list.clear();
}

bool
CG::
Mixture::
addMaterial(const CG::Material &mat)
{
  mat_list.push_back(mat);
  return true;
}

CG::real_t
CG::
Mixture::
getViscosity(const real_t &temp, const real_t &den, const real_t *X)
{
  // here we implement the Wilkes viscosity mixture formula as found in 
  // Bird, Stewart and Lightfoot, "Transport Phenomenon", 2nd edition, 2007.

  real_t mu = 0.;
  
  const int nmat = numberOfMaterials();
  for ( int i=0; i<nmat; i++ )
    {
      real_t denominator = 0.;
      real_t mu_i = mat_list[i].getViscosity(temp,den);
      real_t M_i  = mat_list[i].getMWeight();
      for ( int j=0; j<nmat; j++ )
	{
	  real_t mu_frac = mu_i/mat_list[j].getViscosity(temp,den);
	  real_t M_frac  = M_i/mat_list[j].getMWeight();
	  real_t f = (1.+sqrt(sqrt(1./M_frac)*mu_frac));
	  denominator += X[j] * f*f/sqrt(1.+M_frac);
	}

      mu += 2.*sqrt(2)*X[i] * mu_i/denominator;
    }
  return mu;
}

CG::real_t
CG::
Mixture::
getKThermal(const real_t &temp, const real_t &den, const real_t *X)
{
  // We currently use the averaging formula provided by 
  // Kee, Coltrin and Glarborg, "Chemically Reacting Flow Theory & Practice", 2003.
  real_t sum1=0., sum2=0.;

  const int nmat = numberOfMaterials();
  for ( int m=0; m<nmat; m++ )
    {
      real_t kth = mat_list[m].getKThermal(temp,den);
      sum1 += X[m] * kth;
      sum2 += X[m] / kth;
    }
  
  return 0.5*(sum1 + 1./sum2);
}

CG::
Material::
Material(std::string nm, real_t m_weight, real_t Cp, real_t k_thermal, real_t viscosity) 
  : name(nm), M(m_weight), c_p(Cp), k_th(k_thermal), mu(viscosity)
{
  
}

CG::
Material::
Material( const Material &mat ) : name(mat.name), M(mat.M), c_p(mat.c_p), k_th(mat.k_th), mu(mat.mu)
{
  CG_DEFINE_INTERACTION_POTENTIAL_COPY  ;
}

Material &
CG::
Material::
operator=(const Material &mat)
{
  name = mat.name;
  M = mat.M;
  c_p = mat.c_p;
  k_th = mat.k_th;
  mu = mat.mu;

  return *this;
}

real_t
CG::
IdealGasMixture::
getCp(const real_t &temp, const real_t &den, const real_t *X)
{
  real_t mbar = getMAvg(X);
  real_t cp = 0.;
  int c=0;
  for ( const_mat_iterator m=begin(); m!=end(); m++,c++ )
    {
      const Material &mat = *m;
      real_t Y = mat.getMWeight()*X[c]/mbar;
      cp += Y*mat.getCp(temp,den);
    }
  return cp;
}

real_t
CG::
IdealGasMixture::
getCv(const real_t &temp, const real_t &den, const real_t *X)
{
  real_t mbar = getMAvg(X);
  real_t cv = 0.;
  int c=0;
  for ( const_mat_iterator m=begin(); m!=end(); m++,c++ )
    {
      const Material &mat = *m;
      real_t Y = mat.getMWeight()*X[c]/mbar;
      cv += Y*mat.getCv(temp,den);
    }
  return cv;
}

real_t
CG::
Mixture::
getOmegaIntegral(const Material &mat1, const Material& mat2, const int&i, const int &j, const real_t &temp)
{
  real_t omega_ij;
  static CG::LennardJonesPotential lennardJones;

  if ( mat1.hasLennardJonesData() && mat2.hasLennardJonesData() )
    {
      const real_t &eps1 = mat1.getPotentialParameter( LennardJonesParameters::WELL_DEPTH );
      const real_t &sig1 = mat1.getPotentialParameter( LennardJonesParameters::SIGMA );
      const real_t &eps2 = mat2.getPotentialParameter( LennardJonesParameters::WELL_DEPTH );
      const real_t &sig2 = mat2.getPotentialParameter( LennardJonesParameters::SIGMA );

      real_t eps = sqrt(eps1*eps2);
      real_t sig = 0.5*(sig1+sig2);
      real_t Ts = CG::MKS::boltzmann * temp/eps;
      omega_ij = lennardJones.OmegaIntegral(i,j,Ts);

    }
  // add other potentials here
  else
    {
      cerr<<"ERROR : unknown interaction potential for materials "<<mat1.getName()<<" and "<<mat2.getName()<<endl;
      abort();
    }

  return omega_ij;
}

void
CG::
Mixture::
getBinaryDiffusionCoefficients(const real_t &temp, const real_t &press, real_t * &diffusionCoefficients)
{
  size_t nmat = numberOfMaterials();
  if ( !diffusionCoefficients )
    {
      diffusionCoefficients = new real_t[nmat*nmat];
    }

  for ( int i=0; i<nmat; i++ )
    for ( int j=i; j<nmat; j++ )
      {
	const Material &mat_i = getMaterial(i);
	const Material &mat_j = getMaterial(j);
	const real_t &sig1 = mat_i.getPotentialParameter( LennardJonesParameters::SIGMA );
	const real_t &sig2 = mat_j.getPotentialParameter( LennardJonesParameters::SIGMA );
	real_t sigma = 0.5*(sig1+sig2)*CG::MKS::angstrom; 
	real_t M   = mat_i.getMWeight()*mat_j.getMWeight()/( mat_i.getMWeight() + mat_j.getMWeight() );

	real_t Omega_11 = getOmegaIntegral(mat_i,mat_j, 1,1, temp);
	
	real_t kT = MKS::boltzmann*temp;
	// I worry about precision in the following computation... boltzmann and sigma are so small and avogadro is so big....
	// but it is multiplication an division so it should be ok...
	diffusionCoefficients[i+j*nmat] = (3./16.) * sqrt( (2.*CG::avogadro*kT*kT*kT)/(M_PI*M) )/(press*sigma*sigma*Omega_11);
	diffusionCoefficients[j+i*nmat] = diffusionCoefficients[i+j*nmat];
      }
}

namespace {
  inline real_t delta_f(const int &i, const int &j) { return i==j ? 1 : 0; }
}

#include "OvertureDefine.h"

#define DGESV EXTERN_C_NAME(dgesv)
extern "C" void DGESV(int &N, int &NRHS, double *A, int &LDA, int *IPIV, double *B, int &LDB, int &INFO);

void
CG::
Mixture::
getDiffusionCoefficients(const real_t &temp, const real_t &den, const real_t *X, real_t *&diffusionCoefficients)
{
  int nmat = numberOfMaterials();
  int nvar = nmat*nmat;
  if ( !diffusionCoefficients )
    {
      diffusionCoefficients = new real_t[nmat*nmat];
    }

  real_t *binaryD_ij = 0;//nmat>1 ? 0 : diffusionCoefficients;
  real_t press = getPressure(temp,den,X);
  getBinaryDiffusionCoefficients(temp,press,binaryD_ij);

  //  if ( nmat==1 ) return;
  
  real_t *matrix = new real_t[nvar*nvar];
  real_t *rhs    = diffusionCoefficients; //new real_t[nvar];
  for ( int k=0; k<nvar; k++ )
    {
      rhs[k] = 0;
      for ( int i=0; i<nvar; i++ )
	{
	  matrix[i+k*nvar] = 0;
	} // i
    } // k

  real_t mbar = getMAvg(X);


  // build the first i=1..N-1, k=1..N matrix equations
  int eqn=0;
  for ( int i=0; i<nmat-1; i++ )
    for ( int k=0; k<nmat; k++ )
      {
	const int row = i+k*nmat;
	real_t Y_i = X[i]*mat_list[i].getMWeight()/mbar;

	rhs[eqn] = Y_i - delta_f(i,k);
	for ( int j=0; j<nmat; j++ )
	  {
	    const real_t &Dij = binaryD_ij[i+j*nmat];
	    const int col = j+k*nmat;
	    matrix[eqn + row*nvar] +=  X[j]/Dij;
	    matrix[eqn + col*nvar] -=  X[i]/Dij;
	  } // j
	eqn++;
      } // k

  // fill in the last k=1..N matrix equations
  for ( int k=0; k<nmat; k++ )
    {
      for ( int i=0; i<nmat; i++ )
	{
	  const int row = i+(nmat-1)*nmat;
	  const int col = i+k*nmat;
	  real_t Y_i = X[i]*mat_list[i].getMWeight()/mbar;
	  matrix[eqn + col*nvar] = mat_list[i].getMWeight();
	}
      eqn++;
    }

  int *ipiv = new int[nvar];
  int info = 0;
  int nrhs = 1;
  DGESV(nvar, nrhs, matrix, nvar, ipiv, rhs, nvar, info);

  if ( info ) 
    {
      cerr<<"ERROR : DGESV info = "<<info<<endl;
      cout<<"temp = "<<temp<<endl;
      cout<<"dens = "<<den<<endl;

      for ( int i=0; i<nmat; i++ )
	cout<<"X("<<i+1<<") = "<<X[i]<<endl;

      for ( int i=0; i<nmat; i++ )
	for ( int j=0; j<nmat; j++ )
	  {
	    cout<<"Db("<<i+1<<","<<j+1<<") = "<<binaryD_ij[i+j*nmat]<<endl;
	  }

      for ( int i=0; i<nvar; i++ )
	for ( int j=0; j<nvar; j++ )
	  {
	    cout<<"L("<<i+1<<","<<j+1<<") = "<<matrix[i+j*nvar]<<endl;
	  }
      
      for ( int j=0; j<nvar; j++ )
	{
	  cout<<"rhs("<<j+1<<") = "<<rhs[j]<<endl;
	}
      abort();
    }

  delete [] ipiv;
  //  delete [] rhs;
  delete [] matrix;
  delete [] binaryD_ij;
  
}


#ifdef TEST_MAIN

int main(int argc, char *argv[])
{
  int n_errors = 0;
  Material CO("CO", 28.01/1000., 1.040*1000, 0.023027,1.66e-05);
  CO.getPotentialParameter(LennardJonesParameters::SIGMA) = 3.590;  // given in angstroms
  CO.getPotentialParameter(LennardJonesParameters::WELL_DEPTH) = 110.*MKS::boltzmann;  
  
  Material CO2("CO2", 44.01/1000., 840.71802, 0.01465, 1.372e-05);
  CO2.getPotentialParameter(LennardJonesParameters::SIGMA) = 3.996;  // given in angstroms
  CO2.getPotentialParameter(LennardJonesParameters::WELL_DEPTH) = 190.*MKS::boltzmann; 

  IdealGasMixture mix;
  mix.addMaterial(CO);
  mix.addMaterial(CO2);

  real_t T=296.1;
  real_t p=101325.;

  real_t *binary_Dij=0;
  mix.getBinaryDiffusionCoefficients(T,p,binary_Dij);
  for ( int i=0; i<mix.numberOfMaterials(); i++ )
    for ( int j=0; j<mix.numberOfMaterials(); j++ )
      {
	cout<<"Db("<<i<<","<<j<<") = "<<binary_Dij[i+j*mix.numberOfMaterials()]<<endl;
      }
    
  if ( fabs(1.-binary_Dij[2]/1.49e-5)>2e-3 || fabs(1.-binary_Dij[2]/1.49e-5)>2e-3 )
    {
      cout<<"ERROR: Binary diffusion coefficient does not match Bird, Stewart and Lightfoot"<<endl;
      n_errors++;
    }

  real_t X[] = {.5,.5};
  real_t *D_ij=0;
  real_t den = mix.getDensity(T,p,X);
  mix.getDiffusionCoefficients(T,den,X,D_ij);
  cout<<"X = ("<<X[0]<<", "<<X[1]<<")"<<endl;
  for ( int i=0; i<mix.numberOfMaterials(); i++ )
    for ( int j=0; j<mix.numberOfMaterials(); j++ )
      {
	cout<<"D("<<i<<","<<j<<") = "<<D_ij[i+j*mix.numberOfMaterials()]<<endl;
      }

  X[0] = 1.; X[1] = 1-X[0];
  mix.getDiffusionCoefficients(T,den,X,D_ij);
  cout<<"X = ("<<X[0]<<", "<<X[1]<<")"<<endl;
  for ( int i=0; i<mix.numberOfMaterials(); i++ )
    for ( int j=0; j<mix.numberOfMaterials(); j++ )
      {
	cout<<"D("<<i<<","<<j<<") = "<<D_ij[i+j*mix.numberOfMaterials()]<<endl;
      }

  X[0] = 0; X[1] = 1-X[0];
  mix.getDiffusionCoefficients(T,den,X,D_ij);
  cout<<"X = ("<<X[0]<<", "<<X[1]<<")"<<endl;
  for ( int i=0; i<mix.numberOfMaterials(); i++ )
    for ( int j=0; j<mix.numberOfMaterials(); j++ )
      {
	cout<<"D("<<i<<","<<j<<") = "<<D_ij[i+j*mix.numberOfMaterials()]<<endl;
      }

  delete [] D_ij;
  delete [] binary_Dij;
  return n_errors;
}

#endif
