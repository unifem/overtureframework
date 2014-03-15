  *
  read iges file
  cat2.igs
  continue
    * front port
    *
    * 101 = big curved piece
    choose a list
      110 111 112 115 116 117 118 163 164 165
      101 182
    done
pause
*
* 112 : has a mistake. We fix this by adjusting the trimming curve
* and the un-trimmed surface
*
    edit a Mapping
      112
      * remove the extraneous piece of the trimming curve and
      * shift the curve back by .25
      edit trimming curves
        reparameterize
          .1916307 .8981165
        shift
          -.25 0.
        exit
      *
      * shift the parameterization of the surface to move the branch cut
      * Note that we cannot just rotate the surface since the parameterization
      * is not rotationally invariant.
      edit untrimmed surface
        periodicity
          2 0
        restrict
          .25 1.25 0. 1.
        exit
    exit
    create CompositeSurface
      add all
      mappingName
        valveInlet
      * pause
      determine topology
    exit
    save CompositeSurface
      valveInlet.hdf
    exit
  exit this menu
