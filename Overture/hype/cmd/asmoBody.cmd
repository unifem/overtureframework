  * 
  * mbuild command file: 
  * 
  *  Read the file generated with "rap asmoNoWheels.cmd" 
  * 
  read rap model 
  asmoWithNoWheels.hdf 
  * 
  *  Enter the mapping builder 
  * 
  builder 
    * 
    * surface grid around rear edge 
    * 
    create surface grid... 
      set view:0 0 0 0 1 0.364502 -0.0169989 0.931048 0.244298 0.966558 -0.0779946 -0.898586 0.255882 0.356465 
      * 
      * 
      * Choose a sequence of edges to use as a starting curve 
      * 
      *   We need to increase the matching tolerance between edge curves since 
      *   they do not match very well: 
      edge curve tolerance   0.001 
      choose edge curve -24 8.100000e+02 -2.464112e+01 1.915201e+02 
      choose edge curve -925 8.100000e+02 -4.717830e+01 1.861344e+02 
      choose edge curve -65 8.100000e+02 -5.682169e+01 1.715171e+02 
      choose edge curve -922 8.100000e+02 -6.836867e+01 1.427409e+02 
      choose edge curve -57 8.100000e+02 -8.137321e+01 1.215708e+02 
      choose edge curve -919 8.100000e+02 -1.023312e+02 1.134819e+02 
      choose edge curve -49 8.100000e+02 -1.127013e+02 9.659701e+01 
      choose edge curve -916 8.100000e+02 -1.149662e+02 6.590962e+01 
      choose edge curve -917 8.100000e+02 -1.099926e+02 2.734777e+01 
      choose edge curve -41 8.100000e+02 -5.318736e+01 2.334333e+01 
      done 
      set view:0 -0.209062 -0.00604126 0 2.08867 0.5 -0.150384 0.852869 0.866025 0.0868241 -0.492404 -1.20185e-16 0.984808 0.173648 
      * increase the smoothing parameter 
      uniform dissipation .2 
      * 
      * march the grid in both directions 
      * 
      forward and backward 
      lines to march 10 
      distance to march 26 26 (forward,backward) 
      points on initial curve 61 
      * show start curve for rear edge grid 
      pause 
      * 
      * now generate the grid 
      * 
      generate 
      * rear edge surface grid 
      pause 
      exit 
    * 
    * volume around the rear edge 
    * 
    create volume grid... 
      * name the volume grid: 
      name rearEdge 
      * 
      * Assign BC's and the shared boundary flag 
      *  Each distinct surface should be given a different share value. 
      *   share=1 : main body surface 
      *   share=2 : symmetry plane 
      * 
      Boundary Condition: left    1 
      Boundary Condition: right   1 
      Share Value: left    1 
      Share Value: right   1 
      Boundary Condition: back 2 
      Share Value: back 2 
      * 
      dissipation transition 5 
      backward 
      distance to march 30 
      lines to march 9 
      BC: left fix y, float x and z 
      BC: right fix y, float x and z 
      generate 
      * rear edge volume grid 
      pause 
      exit 
    * 
    * main body surface grid 
    * 
    create surface grid... 
      * 
      * Choose a sequence of edges to use as a starting curve 
      * 
      edge curve tolerance   0.001 
      choose edge curve -24 8.100000e+02 -2.464112e+01 1.915201e+02 
      choose edge curve -925 8.100000e+02 -4.717830e+01 1.861344e+02 
      choose edge curve -65 8.100000e+02 -5.682169e+01 1.715171e+02 
      choose edge curve -922 8.100000e+02 -6.836867e+01 1.427409e+02 
      choose edge curve -57 8.100000e+02 -8.137321e+01 1.215708e+02 
      choose edge curve -919 8.100000e+02 -1.023312e+02 1.134819e+02 
      choose edge curve -49 8.100000e+02 -1.127013e+02 9.659701e+01 
      choose edge curve -916 8.100000e+02 -1.149662e+02 6.590962e+01 
      choose edge curve -917 8.100000e+02 -1.099926e+02 2.734777e+01 
      choose edge curve -41 8.100000e+02 -5.318736e+01 2.334333e+01 
      done 
      points on initial curve 61 
      distance to march 801 
      *       124 -> lines=123 
      lines to march 124 
      generate 
      y+r:0 
      x+:0 
      x+:0 
      * main body surface grid 
      pause 
      exit 
    * 
    * main volume grid 
    * 
    create volume grid... 
      name body 
      BC: left fix y, float x and z 
      BC: right fix y, float x and z 
      Boundary Condition: left    1 
      Boundary Condition: right   1 
      Share Value: left 1 
      Share Value: right 1 
      * 
      Boundary Condition: back 2 
      Share Value: back 2 
      * 
      distance to march 30 50 30 
      lines to march 9 
      backward 
      generate 
      *  main body volume grid 
      pause 
      *      lines=61 121 9 now 
      exit 
    * 
    * surface grid on the front 
    * 
    create surface grid... 
      choose boundary curve 0 1.204306e+01 1.013605e-04 5.084237e+01 
      done 
      Start curve parameter bounds .46 .55   .475 .55 
      backward 
      distance to march 140 130  120 
      points on initial curve 23 15 21  31 
      lines to march 22  23  15 21  31 
      generate 
      set view:0 0.229607 0.148036 0 2.90833 0.339422 0.17101 -0.924958 -0.940151 0.0301537 -0.339422 -0.0301537 0.984808 0.17101 
      * surface grid on front 
      pause 
      exit 
    * 
    * volume grid on the front 
    * 
    create volume grid... 
      name front 
      BC: bottom fix y, float x and z 
      Boundary Condition: bottom  1 
      Share Value: bottom  1 
      Boundary Condition: back 2 
      Share Value: back 2 
      distance to march 30 50 30 
      BC: left outward splay 
      BC: right outward splay 
      lines to march 9 
      generate 
      * volume grid on front 
      pause 
      exit 
    * 
    * surface grid on back 
    * 
    create surface grid... 
      choose boundary curve 1 8.100000e+02 8.419298e-05 1.027524e+02 
      done 
      surface grid options... 
      Start curve parameter bounds .05 .95 .1 .9 .03 .97  .05 .95 
      backward 
      distance to march 99 101 90 111 101 
      lines to march 22 19  13 15  21 
      points on initial curve 31 29 17 19 21 31 
      generate 
      * surface grid on back 
      set view:0 -0.209062 -0.00604126 0 2.08867 0.5 -0.150384 0.852869 0.866025 0.0868241 -0.492404 -1.20185e-16 0.984808 0.173648 
      pause 
      exit 
    * 
    * volume grid on the back 
    * 
    create volume grid... 
      name back 
      BC: bottom fix y, float x and z 
      Boundary Condition: bottom  1 
      Share Value: bottom  1 
      Boundary Condition: back 2 
      Share Value: back 2 
      distance to march 30 50 
      lines to march 9 
      generate 
      * volume grid on back 
      pause 
      exit 
    * all grids 
    x+:0 
    x+:0 
    pause 
    save grids to a file... 
    file name: asmoBody.hdf 
    save file 
    exit 
    exit 
  exit 
