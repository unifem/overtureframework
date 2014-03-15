int 
turbulenceModelsINS(realArray & nuT,
                    MappedGrid & mg,
                    const realArray & u, 
		    const realArray & uu, 
		    const realArray & ut, 
		    const realArray & ux, 
		    const realArray & uy, 
		    const realArray & uz, 
		    const realArray & uxx, 
		    const realArray & uyy, 
		    const realArray & uzz, 
                    const Index & I1, const Index & I2, const Index & I3, 
		    Parameters & parameters,
                    real nu,
		    const int numberOfDimensions,
                    const int grid, const real t  );

int
turbulenceModelBoundaryConditionsINS(const real & t,
                                     realMappedGridFunction & u,
                                     Parameters & parameters,
                                     int grid,
                                     realArray *pBoundaryData[2][3] );

