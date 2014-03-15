  * lower ports plus cylinder
  *
  read iges file
  cat2.igs
    *
    * back port
    *
    choose a list
      86  87  91  92  93  94  95  96  97 98 
      99 130 131 132 133 135 136 137 
      done
    *
    * cylinder : 74 and 75 are the little slices that go in
    *            the overhang of the port on the cylinder
    choose a list
      74 202 203 204 205 
      done
    *
    * front port
    *
    choose a list
      72  73  75  78  79 122 123 124 125 127 
      128 129
      done
*
    create CompositeSurface
      add all mappings
      plot normals (toggle)
      mappingName
        portCylinderEdge
      exit
    save CompositeSurface
      portCylinderEdge.hdf
    exit
  exit this menu


Number of sub-surfaces = 35 
sub-surface    0: visible=1, ID=86, name=trimmed nurbs 1967 
sub-surface    1: visible=1, ID=87, name=trimmed-trimmed nurbs 1979 
sub-surface    2: visible=1, ID=91, name=trimmed nurbs 2043 
sub-surface    3: visible=1, ID=92, name=trimmed-trimmed nurbs 2053 
sub-surface    4: visible=1, ID=93, name=trimmed-trimmed nurbs 2068 
sub-surface    5: visible=1, ID=94, name=trimmed nurbs 2077 
sub-surface    6: visible=1, ID=95, name=trimmed-trimmed nurbs 2088 
sub-surface    7: visible=1, ID=96, name=trimmed-trimmed nurbs 2097 
sub-surface    8: visible=1, ID=97, name=trimmed nurbs 2106 
sub-surface    9: visible=1, ID=98, name=trimmed nurbs 2119 
sub-surface   10: visible=1, ID=99, name=trimmed nurbs 2130 
sub-surface   11: visible=1, ID=130, name=trimmed nurbs 2518 
sub-surface   12: visible=1, ID=131, name=trimmed nurbs 2530 
sub-surface   13: visible=1, ID=132, name=trimmed nurbs 2542 
sub-surface   14: visible=1, ID=133, name=trimmed-trimmed nurbs 2552 
sub-surface   15: visible=1, ID=135, name=trimmed nurbs 2578 
sub-surface   16: visible=1, ID=136, name=trimmed nurbs 2590 
sub-surface   17: visible=1, ID=137, name=trimmed nurbs 2602 
sub-surface   18: visible=1, ID=74, name=trimmed-trimmed nurbs 1816 
sub-surface   19: visible=1, ID=202, name=trimmed-trimmed nurbs 3347 
sub-surface   20: visible=1, ID=203, name=trimmed-trimmed nurbs 3361 
sub-surface   21: visible=1, ID=204, name=trimmed-trimmed nurbs 3371 
sub-surface   22: visible=1, ID=205, name=trimmed-trimmed nurbs 3391 
sub-surface   23: visible=1, ID=72, name=trimmed nurbs 1785 
sub-surface   24: visible=1, ID=73, name=trimmed-trimmed nurbs 1798 
sub-surface   25: visible=1, ID=75, name=trimmed-trimmed nurbs 1827 
sub-surface   26: visible=1, ID=78, name=trimmed nurbs 1858 
sub-surface   27: visible=1, ID=79, name=trimmed-trimmed nurbs 1867 
sub-surface   28: visible=1, ID=122, name=trimmed nurbs 2422 
sub-surface   29: visible=1, ID=123, name=trimmed nurbs 2434 
sub-surface   30: visible=1, ID=124, name=trimmed nurbs 2446 
sub-surface   31: visible=1, ID=125, name=trimmed-trimmed nurbs 2456 
sub-surface   32: visible=1, ID=127, name=trimmed nurbs 2482 
sub-surface   33: visible=1, ID=128, name=trimmed nurbs 2494 
sub-surface   34: visible=1, ID=129, name=trimmed nurbs 2506 




Number of sub-surfaces = 35 
sub-surface    0: visible=1, ID=86, name=trimmed nurbs 1967 
sub-surface    1: visible=1, ID=87, name=trimmed-trimmed nurbs 1979 
sub-surface    2: visible=1, ID=91, name=trimmed nurbs 2043 
sub-surface    3: visible=1, ID=92, name=trimmed-trimmed nurbs 2053 
sub-surface    4: visible=1, ID=93, name=trimmed-trimmed nurbs 2068 
sub-surface    5: visible=1, ID=94, name=trimmed nurbs 2077 
sub-surface    6: visible=1, ID=95, name=trimmed-trimmed nurbs 2088 
sub-surface    7: visible=1, ID=96, name=trimmed-trimmed nurbs 2097 
sub-surface    8: visible=1, ID=97, name=trimmed nurbs 2106 
sub-surface    9: visible=1, ID=98, name=trimmed nurbs 2119 
sub-surface   10: visible=1, ID=99, name=trimmed nurbs 2130 
sub-surface   11: visible=1, ID=130, name=trimmed nurbs 2518 
sub-surface   12: visible=1, ID=131, name=trimmed nurbs 2530 
sub-surface   13: visible=1, ID=132, name=trimmed nurbs 2542 
sub-surface   14: visible=1, ID=133, name=trimmed-trimmed nurbs 2552 
sub-surface   15: visible=1, ID=135, name=trimmed nurbs 2578 
sub-surface   16: visible=1, ID=136, name=trimmed nurbs 2590 
sub-surface   17: visible=1, ID=137, name=trimmed nurbs 2602 
sub-surface   18: visible=1, ID=74, name=trimmed-trimmed nurbs 1816 
sub-surface   19: visible=1, ID=202, name=trimmed-trimmed nurbs 3347 
sub-surface   20: visible=1, ID=203, name=trimmed-trimmed nurbs 3361 
sub-surface   21: visible=1, ID=204, name=trimmed-trimmed nurbs 3371 
sub-surface   22: visible=1, ID=205, name=trimmed-trimmed nurbs 3391 
sub-surface   23: visible=1, ID=72, name=trimmed nurbs 1785 
sub-surface   24: visible=1, ID=73, name=trimmed-trimmed nurbs 1798 
sub-surface   25: visible=1, ID=75, name=trimmed-trimmed nurbs 1827 
sub-surface   26: visible=1, ID=78, name=trimmed nurbs 1858 
sub-surface   27: visible=1, ID=79, name=trimmed-trimmed nurbs 1867 
sub-surface   28: visible=1, ID=122, name=trimmed nurbs 2422 
sub-surface   29: visible=1, ID=123, name=trimmed nurbs 2434 
sub-surface   30: visible=1, ID=124, name=trimmed nurbs 2446 
sub-surface   31: visible=1, ID=125, name=trimmed-trimmed nurbs 2456 
sub-surface   32: visible=1, ID=127, name=trimmed nurbs 2482 
sub-surface   33: visible=1, ID=128, name=trimmed nurbs 2494 
sub-surface   34: visible=1, ID=129, name=trimmed nurbs 2506 

