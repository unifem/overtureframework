  open a data-base 
  cabWithoutWheels.hdf 
  open an old file read-only 
  get all mappings from the data-base 
  close the data-base 
  * 
  * 
  builder 
    target grid spacing .3 .03 (tang,norm)((<0 : use default) 
    * 
    *      ** grid over the back edge of the tender *** 
    create surface grid... 
      choose edge curve 1293 4.026121e+01 4.468301e+00 5.166212e+00 
      choose edge curve 1299 4.026403e+01 4.365742e+00 4.801533e+00 
      choose edge curve 820 4.025764e+01 4.314383e+00 5.105953e-02 
      choose edge curve 1290 4.026619e+01 4.207299e+00 5.635706e+00 
      choose edge curve 1285 4.027028e+01 3.926609e+00 5.839111e+00 
      choose edge curve 1303 4.027489e+01 3.503461e+00 6.005431e+00 
      choose edge curve 1304 4.028415e+01 2.192116e+00 6.025208e+00 
      choose edge curve 1305 4.029388e+01 7.751542e-01 6.003028e+00 
      choose edge curve 1306 4.029775e+01 1.287034e-01 5.739870e+00 
      choose edge curve 847 4.029505e+01 4.072524e-02 5.090419e+00 
      choose edge curve 838 4.032312e+01 3.297302e-02 1.017815e-01 
      choose edge curve 835 4.029848e+01 4.013817e-02 4.780358e+00 
      choose edge curve 838 4.032312e+01 3.297302e-02 1.017815e-01 
      choose edge curve 816 4.026783e+01 4.365766e+00 -4.699407e+00 
      choose edge curve 858 4.026574e+01 4.468348e+00 -5.079934e+00 
      choose edge curve 859 4.026929e+01 4.270384e+00 -5.484406e+00 
      choose edge curve 860 4.027351e+01 3.944122e+00 -5.672058e+00 
      choose edge curve 861 4.027899e+01 3.502842e+00 -5.893557e+00 
      choose edge curve 862 4.029106e+01 2.135888e+00 -5.915736e+00 
      choose edge curve 863 4.030270e+01 7.803182e-01 -5.883031e+00 
      choose edge curve 864 4.030745e+01 1.069846e-01 -5.657088e+00 
      choose edge curve 855 4.030400e+01 2.453450e-02 -5.107329e+00 
      choose edge curve 840 4.030335e+01 2.530912e-02 -4.688905e+00 
      done 
      * 
      forward and backward 
      * initial spacing from distance and lines 
      target grid spacing .3 .1 
      points on initial curve 151 201 
      *      distance to march .5 .5 (forward,backward) 
      lines to march 10 10 (forward,backward) 
pause
      generate 
      pause 
      GSM:BC: top smoothed 
      GSM:BC: bottom smoothed 
     * new:
      GSM:number of iterations 1
      GSM:smooth grid 
      name backTender 
      pause 
      exit 
    * 
    create volume grid... 
      lines to march 21 
      spacing: geometric 
      BC: bottom free floating 
      *       these may not be needed: 
      dissipation transition 9 (>0 : use boundary dissipation) 
      boundary dissipation 0.001 
      volume smooths 10 
      backward 
      generate 
      name backTender 
      pause 
      exit 
    * 
    save grids to a file... 
    file name: backTender.hdf 
    save file 
    exit 
    exit 
  exit 
