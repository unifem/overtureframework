  * Make a grid for the front end of the ichiro geometry 
  * 
  * target grid spacing: 
  $ds=.1; 
  * 
  read iges file 
    /home/henshaw/Overture/mapping/ichiroPart4.igs 
    continue 
    * surface 48 is on the "end" -- leave off for now 
    choose a list 
      37 47 52 56 128 
      done 
    * 
    CSUP:selection function 5
    set view:0 0 0 0 1 0.945986 -0.0158708 0.323818 -0.0219958 0.993358 0.112943 -0.323459 -0.113965 0.939354
    examine a sub-surface 1
    view domain
    delete curves
    delete trim curve 1
    edit curves
    edit trim curve 0
    set view:0 0.69289 -0.310136 0 3.16972 1 0 0 0 1 0 0 0 1
    Mouse Mode Hide SubCurve
    hide curve 6
    hide curve 6
    hide curve 6
    hide curve 6
    hide curve 6
    hide curve 7
    hide curve 7
    hide curve 7
    Mouse Mode Join W/Line Segment
