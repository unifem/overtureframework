* Make a 4 sided 3D tfi mapping
  Box
  exit
* 
  reduce domain dimension
    reduce the domain dimension of which mapping?
    box
    choose the in-active axes
      active      
      active
      0.
    mappingName
      r3=0
    exit
* 
  reduce domain dimension
    reduce the domain dimension of which mapping?
    box
    choose the in-active axes
      active      
      active
      1.
    mappingName
      r3=1
    exit
* 
  reduce domain dimension
    reduce the domain dimension of which mapping?
    box
    choose the in-active axes
      active      
      0.
      active
    mappingName
      r2=0
    exit
* 
  reduce domain dimension
    reduce the domain dimension of which mapping?
    box
    choose the in-active axes
      active      
      1.
      active
    mappingName
      r2=1
    exit
* 
  reduce domain dimension
    reduce the domain dimension of which mapping?
    box
    choose the in-active axes
      0.
      active      
      active
    mappingName
      r1=0
    exit
* 
  reduce domain dimension
    reduce the domain dimension of which mapping?
    box
    choose the in-active axes
      1.
      active      
      active
    mappingName
      r1=1
    exit
*
  tfi
    mappingName
      tfi4
    choose bottom
      r1=0
    choose top
      r1=1
    choose back
      r3=0
    choose front
      r3=1
    exit
  check mapping
    tfi4    
  