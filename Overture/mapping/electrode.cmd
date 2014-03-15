*
*  make a hyperbolic surface grid for the electrode
* 
open a data-base
  electrode.hdf
open an old file read-only
get from the data-base
  electrode
hyperbolic surface
    choose the initial curve
    create a curve from the surface
    choose an edge
    set view 0 0 0 1 -0.554782 -0.830321 0.0527673 -0.824773 0.557197 0.0963403 -0.109395 0.00992687 -0.993949
    set view -0.0388273 -0.0551251 0 5.96046 -0.554782 -0.830321 0.0527673 -0.824773 0.557197 0.0963403 -0.109395 0.00992687 -0.993949
    mogl-select 3 
          130 467776608 467800672  135 519478848 519502912  161 467776608 467800672  
    mogl-pick
    mogl-select 2 
          140 467680032 498395968  187 467680224 467766880  
    mogl-pick
    mogl-select 2 
          135 468303264 470209152  166 468303264 468325984  
    mogl-pick
    done
    exit
    edit initial curve
      lines
        41
      curvature weight
        1.
      exit
    number of lines in marching direction (KMAX)
      21
    boundary condition (IBCJA,IBCJB)
      2 2
    far field distance (ETAMX)
      30.



    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y-r
    y+r
    bigger
    bigger
    bigger
    bigger
    bigger
    bigger
    bigger
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    bigger
    bigger
    bigger
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    y-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    x-
    bigger
    bigger
    bigger
    mogl-select 3 
          130 476171872 476284960  135 519639872 524279488  161 476171872 476284960  
    mogl-pick
    mogl-select 2 
          140 478102592 482660160  187 478102592 478241984  
    mogl-pick
    mogl-select 2 
          135 476929248 481788352  166 476929248 477035040  
    mogl-pick
    done
    exit
    edit initial curve
      lines
        31
      curvature weight
        .5
      exit
    number of lines in marching direction (KMAX)
      21
    boundary condition (IBCJA,IBCJB)
      2 2
    far field distance (ETAMX)
      11
    
