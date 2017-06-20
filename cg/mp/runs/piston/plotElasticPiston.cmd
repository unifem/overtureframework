    plot domain: fluid
    contour
      line plots
        set bogus value for points not interpolated
        -123
        clip to boundary
        specify lines
          1 201
        -.5 .5 1.5 .5
          r
          add u
          add p
          add rhoTrue
          add uTrue
          add rhoErr
          add uErr
          add TErr
          add T
          add x0
pause
        save results to a matlab file
          epf4.m
          exit this menu
        exit this menu
      exit this menu
    plot domain: solid
    contour
      adjust grid for displacement 0
    exit
    contour
      line plots
        clip to boundary
        specify lines
          1 101
        -1. .5 0. .5
          u
          add v1
          add s11
          add x0
          add uErr
          add v1Err
          add s11Err
pause
        save results to a matlab file
          eps4.m
        exit this menu
      exit this menu


      exit