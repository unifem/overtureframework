  read iges file
  cat2.igs
    choose some
    112 112
   erase and exit
*
  copy a mapping
  trimmed-trimmed nurbs 2310
    edit trimming curves
      reparameterize
        .192 .898
      shift
        -.25 0.
      exit
    edit untrimmed surface
      periodicity
        2 0
      restrict
        .25 1.25 0. 1.
      exit
pause

      rotate
        90. 1
*       x=[ 5.7696991346e+01, 8.1692771089e+01] center=69.69488
*       z=[ 1.3056844002e+02, 1.5456560917e+02]       =142.567025
        69.69488 0 142.567025
      exit
    mappingName
     fixed
