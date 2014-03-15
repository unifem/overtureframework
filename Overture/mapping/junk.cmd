* -------------- hom2 ---------------------
   create surface grid...
      edit initial curve 
        restrict the domain 
        .575 .975
        exit 
      forward and backward
      BC: left (forward) outward splay
      BC: right (forward) outward splay
      BC: left (backward) outward splay
      BC: right (backward) outward splay
      equidistribution .1 (in [0,1]) 
      outward splay 0.3 .3 (left, right for outward splay BC)
      lines to march 11, 15 (forward,backward)  
      generate
      name homSurf8
pause
      exit
*
    create volume grid...
      BC: right match to a mapping
        homCyl
      lines to march $nn
      generate
      name homVol8
pause
      exit


rectangle
mapping parameters
  periodicity: axis 0 derivative periodic
  close mapping dialog
    mapping parameters
    lines 22 11
    close mapping dialog
    mapping parameters
    Share Value: left    2
    close mapping dialog
